#include "pch.h"
#include "vk-shaders/model-material-ubo.h"
#include "vk-shaders/cel-shading-ubo.h"
#include "vk-shaders/camera-ubo.h"
#include "vk-shaders/scene-ubo.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <array>
#include <cmath>
#include "model-kernel-0-vert.spv.h"
#include "model-kernel-1-vert.spv.h"
#include "model-kernel-2-vert.spv.h"
#include "model-kernel-3-vert.spv.h"
#include "model-kernel-4-vert.spv.h"
#include "model-kernel-5-vert.spv.h"
#include "model-kernel-6-vert.spv.h"
#include "model-kernel-7-vert.spv.h"
#include "cel-model-kernel-0-vert.spv.h"
#include "cel-model-kernel-1-vert.spv.h"
#include "cel-model-kernel-2-vert.spv.h"
#include "cel-model-kernel-3-vert.spv.h"
#include "cel-model-kernel-4-vert.spv.h"
#include "cel-model-kernel-5-vert.spv.h"
#include "cel-model-kernel-6-vert.spv.h"
#include "cel-model-kernel-7-vert.spv.h"
#include "cel-outline-kernel-0-vert.spv.h"
#include "cel-outline-kernel-1-vert.spv.h"
#include "cel-outline-kernel-2-vert.spv.h"
#include "cel-outline-kernel-3-vert.spv.h"
#include "cel-outline-kernel-4-vert.spv.h"
#include "cel-outline-kernel-5-vert.spv.h"
#include "cel-outline-kernel-6-vert.spv.h"
#include "cel-outline-kernel-7-vert.spv.h"
#include "model-frag.spv.h"
#include "cel-model-frag.spv.h"
#include "cel-model-kernel-0-frag.spv.h"
#include "cel-model-kernel-1-frag.spv.h"
#include "cel-model-kernel-2-frag.spv.h"
#include "cel-model-kernel-3-frag.spv.h"
#include "cel-model-kernel-4-frag.spv.h"
#include "cel-model-kernel-5-frag.spv.h"
#include "cel-model-kernel-6-frag.spv.h"
#include "cel-outline-frag.spv.h"
#include "lambertian-frag.spv.h"
namespace GN::fx2 {
namespace {
using namespace gpu2;
auto * logger = getLogger("GN.fx2.lit-kernel");
bool   uniform(const GpuResourceSet & shared, size_t slot, size_t size) {
    if (shared.size() <= slot || shared[slot].size() != 1) return false;
    const auto & view = shared[slot][0];
    return view.buffer() && view.bufferView.type == GpuResourceView::BufferView::UNIFORM && view.bufferView.size >= size;
}

bool attribute(const RasterGeometry & geometry, uint32_t location, uint32_t components) {
    using F                                       = RasterGeometry::AttributeFormat;
    const RasterGeometry::VertexAttribute * found = nullptr;
    for (const auto & a : geometry.format.attributes) {
        if (a.location != location) continue;
        if (found) return false;
        found = &a;
    }
    if (!found || found->binding >= geometry.vertices.size()) return false;
    const auto & buffer = geometry.vertices[found->binding];
    if (!buffer.buffer) return false;
    const auto f     = static_cast<uint32_t>(found->format);
    uint32_t   bytes = 0;
    if (f == static_cast<uint32_t>(F::F32_1) + components - 1) bytes = components * 4;
    if (f == static_cast<uint32_t>(F::F16_1) + components - 1) bytes = components * 2;
    return bytes && found->offset <= buffer.stride && bytes <= buffer.stride - found->offset;
}

bool finite(glm::vec3 v) { return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z); }
bool finite(glm::vec4 v) { return finite(glm::vec3(v)) && std::isfinite(v.w); }
struct Settings {
    LitKernelInputs   common;
    float             metallic = 0, roughness = 1, ambient = 1;
    GpuResourceView   emissiveMap, occlusionMap, metalRoughMap;
    CelKernel::Inputs cel;
};
enum class Kind { PBR, LAMBERTIAN, CEL };
struct Implementation {
    AutoRef<GpuContext>               gpu;
    std::array<AutoRef<GpuShader>, 8> vertex, outlineVertex, celFragment;
    AutoRef<GpuShader>                fragment, outlineFragment;
    AutoRef<Texture>                  white, normal;
    Kind                              kind;
    bool                              initialize(AutoRef<GpuContext> device, GpuCnC & initialization, Kind k) {
        gpu  = device;
        kind = k;
        if (!gpu) return false;
        const uint32_t * ModelCode[] = {kModelKernel0VertSpv, kModelKernel1VertSpv, kModelKernel2VertSpv, kModelKernel3VertSpv,
                                        kModelKernel4VertSpv, kModelKernel5VertSpv, kModelKernel6VertSpv, kModelKernel7VertSpv};
        const size_t     ModelSize[] = {sizeof(kModelKernel0VertSpv), sizeof(kModelKernel1VertSpv), sizeof(kModelKernel2VertSpv), sizeof(kModelKernel3VertSpv),
                                        sizeof(kModelKernel4VertSpv), sizeof(kModelKernel5VertSpv), sizeof(kModelKernel6VertSpv), sizeof(kModelKernel7VertSpv)};
        const uint32_t * CelModelCode[]    = {kCelModelKernel0VertSpv, kCelModelKernel1VertSpv, kCelModelKernel2VertSpv, kCelModelKernel3VertSpv,
                                              kCelModelKernel4VertSpv, kCelModelKernel5VertSpv, kCelModelKernel6VertSpv, kCelModelKernel7VertSpv};
        const size_t     CelModelSize[]    = {sizeof(kCelModelKernel0VertSpv), sizeof(kCelModelKernel1VertSpv), sizeof(kCelModelKernel2VertSpv),
                                              sizeof(kCelModelKernel3VertSpv), sizeof(kCelModelKernel4VertSpv), sizeof(kCelModelKernel5VertSpv),
                                              sizeof(kCelModelKernel6VertSpv), sizeof(kCelModelKernel7VertSpv)};
        const uint32_t * CelOutlineCode[]  = {kCelOutlineKernel0VertSpv, kCelOutlineKernel1VertSpv, kCelOutlineKernel2VertSpv, kCelOutlineKernel3VertSpv,
                                              kCelOutlineKernel4VertSpv, kCelOutlineKernel5VertSpv, kCelOutlineKernel6VertSpv, kCelOutlineKernel7VertSpv};
        const size_t     CelOutlineSize[]  = {sizeof(kCelOutlineKernel0VertSpv), sizeof(kCelOutlineKernel1VertSpv), sizeof(kCelOutlineKernel2VertSpv),
                                              sizeof(kCelOutlineKernel3VertSpv), sizeof(kCelOutlineKernel4VertSpv), sizeof(kCelOutlineKernel5VertSpv),
                                              sizeof(kCelOutlineKernel6VertSpv), sizeof(kCelOutlineKernel7VertSpv)};
        const uint32_t * CelFragmentCode[] = {kCelModelKernel0FragSpv, kCelModelKernel1FragSpv, kCelModelKernel2FragSpv, kCelModelKernel3FragSpv,
                                              kCelModelKernel4FragSpv, kCelModelKernel5FragSpv, kCelModelKernel6FragSpv, kCelModelFragSpv};
        const size_t     CelFragmentSize[] = {sizeof(kCelModelKernel0FragSpv), sizeof(kCelModelKernel1FragSpv), sizeof(kCelModelKernel2FragSpv),
                                              sizeof(kCelModelKernel3FragSpv), sizeof(kCelModelKernel4FragSpv), sizeof(kCelModelKernel5FragSpv),
                                              sizeof(kCelModelKernel6FragSpv), sizeof(kCelModelFragSpv)};
        for (size_t i = 0; i < 8; ++i) {
            vertex[i] = GpuShader::create({.context = gpu,
                                           .name    = "lit-kernel.vert",
                                           .binary  = k == Kind::CEL ? CelModelCode[i] : ModelCode[i],
                                           .size    = k == Kind::CEL ? CelModelSize[i] : ModelSize[i]});
            if (!vertex[i]) return false;
            if (k == Kind::CEL) {
                outlineVertex[i] = GpuShader::create({.context = gpu, .name = "cel-outline.vert", .binary = CelOutlineCode[i], .size = CelOutlineSize[i]});
                if (!outlineVertex[i]) return false;
                celFragment[i] = GpuShader::create({.context = gpu, .name = "cel-model.frag", .binary = CelFragmentCode[i], .size = CelFragmentSize[i]});
                if (!celFragment[i]) return false;
            }
        }
        if (k != Kind::CEL) {
            fragment = GpuShader::create({.context = gpu,
                                          .name    = "lit-kernel.frag",
                                          .binary  = k == Kind::PBR ? kModelFragSpv : kLambertianFragSpv,
                                          .size    = k == Kind::PBR ? sizeof(kModelFragSpv) : sizeof(kLambertianFragSpv)});
            if (!fragment) return false;
        }
        if (k == Kind::CEL) {
            outlineFragment = GpuShader::create({.context = gpu, .name = "cel-outline.frag", .binary = kCelOutlineFragSpv, .size = sizeof(kCelOutlineFragSpv)});
            if (!outlineFragment) return false;
        }
        for (int i = 0; i < 2; ++i) {
            Texture::Descriptor d;
            d.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(1).setLevels(1);
            auto t = Texture::create("lit-kernel.fallback", {.context = gpu, .descriptor = d});
            auto b = Buffer::create("lit-kernel.fallback-upload", {.context = gpu, .size = 4, .mappable = true});
            if (!t || !b) return false;
            {
                auto m = b->map();
                if (!m.data()) return false;
                const uint8_t whiteBytes[] = {255, 255, 255, 255}, normalBytes[] = {128, 128, 255, 255};
                memcpy(m.data(), i ? normalBytes : whiteBytes, 4);
            }
            GpuCnC::Region r;
            r.imageExtent = {1, 1, 1};
            initialization.recordCopyBufferToImage({.src = b, .dst = t, .regions = {&r, 1}});
            (i ? normal : white) = t;
        }
        return true;
    }
    bool record(GpuRaster & raster, GpuCnC & uploads, const GpuResourceSet & shared, const Settings & s,
                ArrayView<const glm::mat4> additionalTransforms = {}) const {
        const auto &                   in       = s.common;
        const auto &                   g        = in.geometry;
        std::array<GpuResourceView, 5> maps     = {in.colorMap, in.normalMap, s.emissiveMap, s.occlusionMap, s.metalRoughMap};
        bool                           textured = false;
        bool                           valid    = uniform(shared, 0, sizeof(shader::SceneUBO)) && uniform(shared, 1, sizeof(shader::CameraUBO));
        for (size_t i = 2; i < 6; ++i) valid &= shared.size() > i && shared[i].size() == 1 && shared[i][0].texture();
        for (auto & map : maps) {
            if (map.empty()) continue;
            textured = true;
            auto t   = map.texture();
            valid &= t && map.imageView.type == GpuResourceView::ImageView::SAMPLED;
            if (t) {
                const auto & d = t->descriptor();
                const auto & r = map.imageView.range;
                valid &= d.faces == 1 && d.depth == 1 && r.i.mip < d.levels && r.i.face == 0 &&
                         (r.e.numArrayLayers == 1 || r.e.numArrayLayers == uint32_t(-1)) &&
                         (r.e.numMipLevels == uint32_t(-1) || (r.e.numMipLevels > 0 && r.e.numMipLevels <= d.levels - r.i.mip));
            }
        }
        valid &= attribute(g, 0, 3) && attribute(g, 1, 3) && (!textured || attribute(g, 2, 2)) && (in.normalMap.empty() || attribute(g, 3, 4)) &&
                 (!in.useVertexColor || attribute(g, 4, 4));
        valid &= (g.indexCount ? g.indices.buffer && (g.indices.stride == 2 || g.indices.stride == 4) : g.vertexCount > 0);
        valid &= finite(in.color) && finite(in.emissive) && std::isfinite(in.alphaCutoff) && in.alphaCutoff >= 0 && in.alphaCutoff <= 1;
        auto validateMatrix = [&](const glm::mat4 & m) {
            for (int c = 0; c < 4; ++c)
                for (int r = 0; r < 4; ++r) valid &= std::isfinite(m[c][r]);
            const float determinant = glm::determinant(m);
            valid &= std::isfinite(determinant) && determinant != 0;
        };
        validateMatrix(in.worldFromObject);
        for (const auto & m : additionalTransforms) validateMatrix(m);
        valid &= std::isfinite(s.metallic) && s.metallic >= 0 && s.metallic <= 1 && std::isfinite(s.roughness) && s.roughness >= 0 && s.roughness <= 1 &&
                 std::isfinite(s.ambient) && s.ambient >= 0;
        if (kind == Kind::CEL) {
            const auto & c = s.cel;
            valid &= finite(c.shadowTint) && finite(c.deepShadowTint) && finite(c.rimTint) && finite(c.outlineColor);
            for (float v : {c.shadowThreshold, c.shadowFeather, c.deepShadowThreshold, c.deepShadowFeather, c.specularThreshold, c.specularShininess,
                            c.specularIntensity, c.rimIntensity, c.rimThreshold, c.rimFeather, c.outlineWidth})
                valid &= std::isfinite(v) && v >= 0;
        }
        if (!valid) GN_UNLIKELY {
                GN_ERROR(logger, "Lit kernel: invalid vertex layout, SSC, textures, or immediate parameters");
                return false;
            }
        shader::ModelMaterialUBO u;
        u.baseColor              = in.color;
        u.emissiveAndMetallic    = glm::vec4(in.emissive, s.metallic);
        uint32_t flags           = (in.normalMap.empty() ? 0u : 1u) | (in.useVertexColor ? 2u : 0u) | (in.opaque ? 4u : 0u);
        u.roughnessAlphaWorkflow = {s.roughness, in.alphaCutoff, float(flags), s.ambient};
        // Each recording owns its uniform storage: batching prerequisite uploads must not overwrite previous invocation values.
        auto material = Buffer::create("lit-kernel.parameters", {.context = gpu, .size = sizeof(u)});
        if (!material) return false;
        AutoRef<Buffer>       cel;
        shader::CelShadingUBO c;
        if (kind == Kind::CEL) {
            const auto & cfg = s.cel;
            c.shadowParams   = {cfg.shadowThreshold, cfg.shadowFeather, cfg.deepShadowThreshold, cfg.deepShadowFeather};
            c.shadowTint     = glm::vec4(cfg.shadowTint, 1);
            c.deepShadowTint = glm::vec4(cfg.deepShadowTint, 1);
            c.specularParams = {cfg.specularThreshold, cfg.specularShininess, cfg.specularIntensity, 0};
            c.rimParams      = {cfg.rimThreshold, cfg.rimFeather, cfg.rimIntensity, 0};
            c.rimTint        = glm::vec4(cfg.rimTint, 1);
            c.outlineParams  = {cfg.outlineWidth, 0, 0, 0};
            c.outlineColor   = cfg.outlineColor;
            cel              = Buffer::create("cel-kernel.parameters", {.context = gpu, .size = sizeof(c)});
            if (!cel) return false;
        }
        uploads.recordUploadBuffer(material, 0, {reinterpret_cast<const uint8_t *>(&u), sizeof(u)});
        if (cel) uploads.recordUploadBuffer(cel, 0, {reinterpret_cast<const uint8_t *>(&c), sizeof(c)});
        GpuResourceTable          drawResources;
        GpuRaster::DrawParameters draw {.geometry = g, .resources = drawResources};
        const size_t              variant = (textured ? 1 : 0) + (in.normalMap.empty() ? 0 : 2) + (in.useVertexColor ? 4 : 0);
        drawResources.resize(2);
        drawResources[0] = shared;
        auto & set       = drawResources[1];
        set.resize(cel ? 7 : 6);
        for (size_t i = 0; i < 5; ++i) {
            if (maps[i].empty()) {
                maps[i].resource = i == 1 ? normal : white;
                maps[i].setImageViewType(GpuResourceView::ImageView::SAMPLED);
            }
            set[i].append(maps[i]);
        }
        GpuResourceView mv;
        mv.resource = material;
        mv.setBufferViewType(GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(u));
        set[5].append(mv);
        if (cel) {
            mv.resource = cel;
            mv.setBufferViewSize(sizeof(c));
            set[6].append(mv);
        }

        struct Push {
            glm::mat4 world, normal;
        };

        auto recordPass = [&](AutoRef<GpuShader> vs, AutoRef<GpuShader> ps, const RasterState & states) {
            draw.vs     = vs;
            draw.ps     = ps;
            draw.states = states;
            Push push {in.worldFromObject, glm::transpose(glm::inverse(in.worldFromObject))};
            draw.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(push), reinterpret_cast<const uint8_t *>(&push)));
            raster.recordDraw(draw);
            for (const auto & matrix : additionalTransforms) {
                Push extraPush {matrix, glm::transpose(glm::inverse(matrix))};
                draw.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(extraPush), reinterpret_cast<const uint8_t *>(&extraPush)));
                raster.recordDraw(draw);
            }
        };

        recordPass(vertex[variant], kind == Kind::CEL ? celFragment[variant] : fragment, in.states);
        if (cel && s.cel.outlineWidth > 0) {
            RasterState outlineStates = in.states;
            outlineStates.cullMode    = RasterState::CULL_FRONT;
            outlineStates.depthState  = RasterState::DepthState {RasterState::Compare::LESS_EQUAL, true};
            recordPass(outlineVertex[variant], outlineFragment, outlineStates);
        }
        return true;
    }
};
class PbrImpl final : public PbrKernel {
    Implementation impl;

public:
    GN_REGISTER_RUNTIME_TYPE(PbrKernel);
    PbrImpl(): PbrKernel(TYPE_INFO(), "pbr-kernel") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<GpuContext> gpu, GpuCnC & init) { return impl.initialize(gpu, init, Kind::PBR); }
    bool      record(GpuRaster & raster, GpuCnC & uploads, const GpuResourceSet & shared, const Inputs & in,
                     ArrayView<const glm::mat4> additionalTransforms) const override {
        Settings s;
        s.common        = in;
        s.metallic      = in.metallic;
        s.roughness     = in.roughness;
        s.emissiveMap   = in.emissiveMap;
        s.occlusionMap  = in.occlusionMap;
        s.metalRoughMap = in.metalRoughMap;
        return impl.record(raster, uploads, shared, s, additionalTransforms);
    }
};
class LambertianImpl final : public LambertianKernel {
    Implementation impl;

public:
    GN_REGISTER_RUNTIME_TYPE(LambertianKernel);
    LambertianImpl(): LambertianKernel(TYPE_INFO(), "lambertian-kernel") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<GpuContext> gpu, GpuCnC & init) { return impl.initialize(gpu, init, Kind::LAMBERTIAN); }
    bool      record(GpuRaster & raster, GpuCnC & uploads, const GpuResourceSet & shared, const Inputs & in,
                     ArrayView<const glm::mat4> additionalTransforms) const override {
        Settings s;
        s.common  = in;
        s.ambient = in.ambientIntensity;
        return impl.record(raster, uploads, shared, s, additionalTransforms);
    }
};
class CelImpl final : public CelKernel {
    Implementation impl;

public:
    GN_REGISTER_RUNTIME_TYPE(CelKernel);
    CelImpl(): CelKernel(TYPE_INFO(), "cel-kernel") {}
    Execution execution() const override { return Execution::RASTER; }
    bool      initialize(AutoRef<GpuContext> gpu, GpuCnC & init) { return impl.initialize(gpu, init, Kind::CEL); }
    bool      record(GpuRaster & raster, GpuCnC & uploads, const GpuResourceSet & shared, const Inputs & in,
                     ArrayView<const glm::mat4> additionalTransforms) const override {
        Settings s;
        s.common       = in;
        s.cel          = in;
        s.emissiveMap  = in.emissiveMap;
        s.occlusionMap = in.occlusionMap;
        return impl.record(raster, uploads, shared, s, additionalTransforms);
    }
};
} // namespace
AutoRef<PbrKernel> PbrKernel::create(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & initialization) {
    AutoRef<PbrImpl> k(new PbrImpl);
    if (!k->initialize(gpu, initialization)) return {};
    return k;
}
AutoRef<LambertianKernel> LambertianKernel::create(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & initialization) {
    AutoRef<LambertianImpl> k(new LambertianImpl);
    if (!k->initialize(gpu, initialization)) return {};
    return k;
}
AutoRef<CelKernel> CelKernel::create(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & initialization) {
    AutoRef<CelImpl> k(new CelImpl);
    if (!k->initialize(gpu, initialization)) return {};
    return k;
}
} // namespace GN::fx2
