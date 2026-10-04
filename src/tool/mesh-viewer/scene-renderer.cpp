#include "scene-renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <sstream>

namespace GN::viewer {
using namespace gpu2;
namespace {
AutoRef<Texture> loadTexture(AutoRef<GpuContext> gpu, GpuCnC & cnc, const ModelScene::Texture & source) {
    gfx::img::Image image;
    if (!source.path.empty()) {
        auto file = fs::openFile(source.path, std::ios::in | std::ios::binary);
        if (!file) return {};
        image = gfx::img::Image::load(file->input(), source.path.c_str());
    } else {
        std::string        bytes((const char *) source.embeddedData.data(), source.embeddedData.size());
        std::istringstream stream(bytes);
        image = gfx::img::Image::load(stream, source.mimeType.c_str());
    }
    if (image.empty()) return {};
    Texture::Descriptor descriptor;
    descriptor.setFormat(image.format())
        .setDimensions(image.width(), image.height(), image.depth())
        .setFaces((uint32_t) image.desc().faces)
        .setLevels((uint32_t) image.desc().levels);
    using PF  = gfx::img::PixelFormat;
    auto sign = source.srgb ? PF::SIGN_GNORM : PF::SIGN_UNORM;
    if (descriptor.format.sign0 == PF::SIGN_UNORM || descriptor.format.sign0 == PF::SIGN_GNORM) descriptor.format.sign0 = sign;
    if (descriptor.format.sign12 == PF::SIGN_UNORM || descriptor.format.sign12 == PF::SIGN_GNORM) descriptor.format.sign12 = sign;
    auto texture = Texture::create("viewer.texture", {.context = gpu, .descriptor = descriptor});
    if (texture) cnc.recordUploadImage(texture, image);
    return texture;
}
} // namespace
bool RenderModel::prepare(AutoRef<GpuContext> gpu, GpuCnC & cnc, const ModelScene & scene, Shading choice, const glm::mat4 & modelTransform) {
    DynaArray<ModelInstance> collected;
    if (!collectModelInstances(scene, collected)) return false;
    DynaArray<AutoRef<Texture>> textures;
    textures.resize(scene.textures.size());
    auto texture = [&](int32_t index, GpuResourceView & view) {
        if (index < 0) return true;
        if (static_cast<size_t>(index) >= textures.size()) return false;
        if (!textures[index]) textures[index] = loadTexture(gpu, cnc, scene.textures[index]);
        if (!textures[index]) return false;
        view.resource = textures[index];
        view.setImageViewType(GpuResourceView::ImageView::SAMPLED);
        return true;
    };
    for (const auto & source : scene.materials) {
        Material material;
        material.shading     = choice == Shading::IMPORTED ? (source.workflow == ModelScene::MaterialWorkflow::UNLIT ? Shading::UNLIT : Shading::PBR) : choice;
        material.doubleSided = source.doubleSided;
        auto & p             = material.parameters;
        p.color              = source.baseColor;
        p.emissive           = source.emissive;
        p.metallic           = source.metallic;
        p.roughness          = source.roughness;
        p.opaque             = source.alphaMode == ModelScene::AlphaMode::OPAQUE;
        p.alphaCutoff        = source.alphaMode == ModelScene::AlphaMode::MASK ? source.alphaCutoff : 0;
        p.useVertexColor     = true;
        if (!texture(source.baseColorMap, p.colorMap) || !texture(source.normalMap, p.normalMap) || !texture(source.emissiveMap, p.emissiveMap) ||
            !texture(source.occlusionMap, p.occlusionMap) || !texture(source.metalRoughMap, p.metalRoughMap))
            return false;
        materials.append(std::move(material));
    }
    DynaArray<RasterGeometry> meshes;
    for (const auto & primitive : scene.primitives) {
        if (primitive.vertices.empty() || primitive.indices.empty() || primitive.material >= scene.materials.size()) return false;
        for (auto index : primitive.indices)
            if (index >= primitive.vertices.size()) return false;
        const auto & m = scene.materials[primitive.material];
        if (!primitive.texcoords && (m.baseColorMap >= 0 || m.normalMap >= 0 || m.emissiveMap >= 0 || m.occlusionMap >= 0 || m.metalRoughMap >= 0))
            return false;
        auto vb = Buffer::create("viewer.vertices", {.context = gpu, .size = primitive.vertices.size() * sizeof(ModelScene::Vertex)});
        auto ib = Buffer::create("viewer.indices", {.context = gpu, .size = primitive.indices.size() * sizeof(uint32_t)});
        if (!vb || !ib) return false;
        cnc.recordUploadBuffer(vb, 0, {reinterpret_cast<const uint8_t *>(primitive.vertices.data()), primitive.vertices.size() * sizeof(ModelScene::Vertex)});
        cnc.recordUploadBuffer(ib, 0, {reinterpret_cast<const uint8_t *>(primitive.indices.data()), primitive.indices.size() * sizeof(uint32_t)});
        RasterGeometry mesh;
        mesh.vertices.push_back({.buffer = vb, .offset = 0, .stride = sizeof(ModelScene::Vertex)});
        mesh.indices     = {.buffer = ib, .offset = 0, .stride = sizeof(uint32_t)};
        mesh.vertexCount = static_cast<uint32_t>(primitive.vertices.size());
        mesh.indexCount  = static_cast<uint32_t>(primitive.indices.size());
        using V          = ModelScene::Vertex;
        using AF         = RasterGeometry::AttributeFormat;
        mesh.format.attributes.push_back({.location = 0, .binding = 0, .offset = offsetof(V, position), .format = AF::F32_3});
        mesh.format.attributes.push_back({.location = 1, .binding = 0, .offset = offsetof(V, normal), .format = AF::F32_3});
        mesh.format.attributes.push_back({.location = 2, .binding = 0, .offset = offsetof(V, texcoord), .format = AF::F32_2});
        mesh.format.attributes.push_back({.location = 3, .binding = 0, .offset = offsetof(V, tangent), .format = AF::F32_4});
        mesh.format.attributes.push_back({.location = 4, .binding = 0, .offset = offsetof(V, color), .format = AF::F32_4});
        meshes.append(std::move(mesh));
    }

    for (const auto & instance : collected) {
        const auto  transform   = modelTransform * instance.transform;
        const float determinant = glm::determinant(transform);
        if (!std::isfinite(determinant) || determinant == 0) return false;
        instances.append({meshes[instance.primitive], scene.primitives[instance.primitive].material, transform});
    }
    return true;
}

bool SceneRenderer::prepare(AutoRef<GpuContext> gpu, const ModelScene & source, Shading shading, float environmentLuminance) {
    auto uploads = GpuCnC::create({.gpu = gpu});
    if (!uploads) return false;
    ssc        = fx2::SharedShaderConstants::create({.gpu = gpu});
    pbr        = fx2::PbrKernel::create(gpu, *uploads);
    cel        = fx2::CelKernel::create(gpu, *uploads);
    lambertian = fx2::LambertianKernel::create(gpu, *uploads);
    unlit      = fx2::UnlitKernel::create(gpu);
    skybox     = fx2::SkyboxKernel::create(gpu);
    if (!ssc || !pbr || !cel || !lambertian || !unlit || !skybox) return false;
    ssc->set0.envLighting = {
        .skyboxPath                = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds",
        .irradiancePath            = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds",
        .prefilteredPath           = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds",
        .brdfLutPath               = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds",
        .environmentLuminanceScale = environmentLuminance,
    };
    const auto  center       = (source.bounds.minimum + source.bounds.maximum) * 0.5f;
    const auto  transform    = glm::translate(glm::mat4(1), -center);
    const float width        = std::max(glm::length(source.bounds.maximum - source.bounds.minimum) * 0.5f, 0.001f) * 0.004f;
    auto        boundsSource = ModelScene::createDebugVisualization(source.bounds, width, true, false);
    auto        axesSource   = ModelScene::createDebugVisualization(source.bounds, width, false, true);
    if (!boundsSource || !axesSource || !model.prepare(gpu, *uploads, source, shading, transform) ||
        !bounds.prepare(gpu, *uploads, *boundsSource, Shading::UNLIT, transform) || !axes.prepare(gpu, *uploads, *axesSource, Shading::UNLIT, transform))
        return false;
    initialization = uploads->seal();
    return !!initialization;
}

bool SceneRenderer::recordModel(const RenderModel & source, GpuRaster & raster, GpuCnC & uploads, const GpuResourceSet & shared) const {
    for (const auto & instance : source.instances) {
        const auto & material    = source.materials[instance.material];
        auto         inputs      = material.parameters;
        inputs.geometry          = instance.geometry;
        inputs.worldFromObject   = instance.transform;
        inputs.states.cullMode   = material.doubleSided ? RasterState::CULL_NONE : RasterState::CULL_BACK;
        inputs.states.frontFace  = glm::determinant(instance.transform) < 0 ? RasterState::FRONT_CW : RasterState::FRONT_CCW;
        inputs.states.depthState = RasterState::DepthState {RasterState::Compare::LESS, true};
        switch (material.shading) {
        case Shading::PBR:
            if (!pbr->record(raster, uploads, shared, inputs)) return false;
            break;
        case Shading::CEL: {
            fx2::CelKernel::Inputs celInputs;
            static_cast<fx2::LitKernelInputs &>(celInputs) = inputs;
            celInputs.emissiveMap                          = inputs.emissiveMap;
            celInputs.occlusionMap                         = inputs.occlusionMap;
            if (!cel->record(raster, uploads, shared, celInputs)) return false;
            break;
        }
        case Shading::LAMBERTIAN: {
            fx2::LambertianKernel::Inputs litInputs;
            static_cast<fx2::LitKernelInputs &>(litInputs) = inputs;
            if (!lambertian->record(raster, uploads, shared, litInputs)) return false;
            break;
        }
        case Shading::UNLIT: {
            fx2::UnlitKernel::Inputs unlitInputs;
            unlitInputs.geometry        = inputs.geometry;
            unlitInputs.worldFromObject = inputs.worldFromObject;
            unlitInputs.color           = inputs.color;
            unlitInputs.emissive        = inputs.emissive;
            unlitInputs.alphaCutoff     = inputs.alphaCutoff;
            unlitInputs.colorMap        = inputs.colorMap;
            unlitInputs.useVertexColor  = inputs.useVertexColor;
            unlitInputs.states          = inputs.states;
            if (!unlit->record(raster, shared, unlitInputs)) return false;
            break;
        }
        default:
            return false;
        }
    }
    return true;
}

bool SceneRenderer::record(GpuRaster & raster, GpuCnC & uploads, const GpuResourceSet & shared, bool showBounds, bool showAxes) const {
    return recordModel(model, raster, uploads, shared) && (!showBounds || recordModel(bounds, raster, uploads, shared)) &&
           (!showAxes || recordModel(axes, raster, uploads, shared)) && skybox->record(raster, shared);
}
} // namespace GN::viewer
