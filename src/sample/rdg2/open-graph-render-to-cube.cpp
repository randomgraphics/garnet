// open-graph-render-to-cube.cpp — RDG v2 render-to-cubemap demo
//
// Loads 6 images from media/texture/cube1/ and renders each to one face of a
// 512×512 RGBA8 cubemap using the RDG v2 / gpu2 open-graph API. A rotating 3D cube
// is then drawn to the swapchain, sampling the assembled cubemap.
//
// Two cubemap-fill implementations are available:
//
//   Impl 1 (--impl 1, default) — 6 render passes, one per face.
//     Each GpuRaster targets a single cube face via a per-face SubresourceIndex view.
//     The fragment shader samples the per-face source texture bound at set=0,binding=0.
//
//   Impl 2 (--impl 2) — 1 render pass with 6 simultaneous MRT color targets.
//     A single GpuRaster binds 6 color-target slots (one per cube face). The fragment
//     shader has 6 sampler2D bindings (set=0, binding=0..5) and 6 output locations.
//
// Dependency graph per frame (both impls):
//   [create cubemap shaders] ──────────────────────────────> faceVsArtifact / facePsArtifact
//   [create cube-draw shaders + geometry] ─────────────────> cubeDrawVsArtifact / cubeDrawPsArtifact
//   [render cubemap]  ──depends on──> shader ONE() tokens ──publishes──> cubemapReadyArtifact
//   [draw cube]       ──depends on──> cubemapReadyArtifact (OOO) + cubeDrawVsArtifact + cubeDrawPsArtifact
//
// Usage: open-graph-render-to-cube [t] [--impl 1|2]
//   t        — headless test mode: 10 frames, readback face 0, log verification
//   --impl N — 1 = face-by-face (default), 2 = single-pass MRT

#include <garnet/GNrdg2.h>
#include <garnet/GNfx2.h>
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>

#include "cube-face-vert.spv.h"
#include "cube-face-frag.spv.h"
#include "cube-all-faces-frag.spv.h"
#include "cube-draw-vert.spv.h"
#include "cube-draw-frag.spv.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace GN;
using namespace GN::rdg2;
using namespace GN::gpu2;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.open-graph-render-to-cube");

struct ShaderArtifactContent final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    AutoRef<GpuShader> shader;

    explicit ShaderArtifactContent(AutoRef<GpuShader> shader_): Entity(TYPE_INFO(), "ShaderArtifactContent"), shader(std::move(shader_)) {}
};

struct TextureArtifactContent final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    AutoRef<Texture> texture;

    explicit TextureArtifactContent(AutoRef<Texture> texture_): Entity(TYPE_INFO(), "TextureArtifactContent"), texture(std::move(texture_)) {}
};

// Source images for the 6 cubemap faces, loaded from the cube1 folder.
// Index 0 = +X, 1 = -X, 2 = +Y, 3 = -Y, 4 = +Z, 5 = -Z.
static constexpr std::array<const char *, 6> kFaceImagePaths = {
    "media::texture/cube1/1.bmp", "media::texture/cube1/2.bmp", "media::texture/cube1/3.bmp",
    "media::texture/cube1/4.bmp", "media::texture/cube1/5.bmp", "media::texture/cube1/6.bmp",
};

static constexpr uint32_t kCubemapSize = 512;

// ─── Shared context ────────────────────────────────────────────────────────────
//
// Resources common to all renderer classes, passed by const reference.
struct SharedCtx {
    GraphPtr                            graph;
    AutoRef<GpuContext>                 gpu;
    AutoRef<fx2::SharedShaderConstants> ssc;
    uint32_t                            width, height;
};

// Build a view that addresses exactly one face (one mip, one array layer).
static GpuResourceView makeFaceView(const AutoRef<Texture> & cubemap, uint32_t faceIndex) {
    GpuResourceView v;
    v.resource = cubemap;
    v.setSubresourceIndex(GpuResourceView::SubresourceIndex {.face = faceIndex})
        .setSubresourceExtent(GpuResourceView::SubresourceExtent {.numMipLevels = 1, .numArrayLayers = 1});
    return v;
}

// ─── CubemapRenderer ───────────────────────────────────────────────────────────
//
// Copies 6 loaded disk images into the corresponding faces of a RGBA8 cubemap each frame.
// Impl 1 uses 6 separate render passes; impl 2 uses one pass with 6 MRT targets.
class CubemapRenderer {
public:
    static CubemapRenderer create(const SharedCtx & ctx, int impl) {
        CubemapRenderer r;
        r.mCtx  = ctx;
        r.mImpl = impl;

        // Load the 6 source images synchronously before the graph starts. Each is a
        // plain 2D texture that gets blitted into the corresponding cubemap face.
        for (uint32_t i = 0; i < 6; ++i) {
            r.mFaceTextures[i] = Texture::load({.context = ctx.gpu, .filename = kFaceImagePaths[i]});
            if (!r.mFaceTextures[i]) {
                GN_ERROR(sLogger, "CubemapRenderer: failed to load face texture '{}'", kFaceImagePaths[i]);
                return {};
            }
        }

        r.mCubemap = Texture::create(
            "cubemap",
            Texture::CreateParameters {
                .context = ctx.gpu,
                .descriptor =
                    Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(kCubemapSize, kCubemapSize).setFaces(6).setLevels(1),
            });
        if (!r.mCubemap) {
            GN_ERROR(sLogger, "CubemapRenderer: failed to create cubemap texture");
            return {};
        }

        r.mFaceVsArtifact     = ctx.graph->createArtifact("cube-face VS");
        r.mFacePsArtifact     = ctx.graph->createArtifact("cube-face PS");
        r.mAllFacesPsArtifact = ctx.graph->createArtifact("cube-all-faces PS");
        r.mCubemapReady       = ctx.graph->createArtifact("cubemap ready");

        ctx.graph->addNode(NodeDesc("create cubemap shaders", [graph = ctx.graph, gpu = ctx.gpu, vsArt = r.mFaceVsArtifact, psArt = r.mFacePsArtifact,
                                                               allArt = r.mAllFacesPsArtifact]() {
            auto vs = GpuShader::create(
                {.context = gpu, .name = "cube-face VS", .binary = kCubeFaceVertSpv, .size = kCubeFaceVertSpvSize * sizeof(unsigned int), .entry = "main"});
            auto facePs = GpuShader::create(
                {.context = gpu, .name = "cube-face PS", .binary = kCubeFaceFragSpv, .size = kCubeFaceFragSpvSize * sizeof(unsigned int), .entry = "main"});
            auto allFacesPs = GpuShader::create({.context = gpu,
                                                 .name    = "cube-all-faces PS",
                                                 .binary  = kCubeAllFacesFragSpv,
                                                 .size    = kCubeAllFacesFragSpvSize * sizeof(unsigned int),
                                                 .entry   = "main"});
            if (!vs || !facePs || !allFacesPs) {
                GN_ERROR(sLogger, "CubemapRenderer: failed to create one or more shaders");
                return;
            }
            graph->publishArtifact(vsArt, AutoRef<ShaderArtifactContent>(new ShaderArtifactContent(vs)));
            graph->publishArtifact(psArt, AutoRef<ShaderArtifactContent>(new ShaderArtifactContent(facePs)));
            graph->publishArtifact(allArt, AutoRef<ShaderArtifactContent>(new ShaderArtifactContent(allFacesPs)));
        }));

        // Pre-capture ONE() tokens so per-frame nodes can safely wait on a version that
        // will always be satisfied once the shader node runs once.
        r.mFaceVsReady     = ctx.graph->getArtifactVersionToken(r.mFaceVsArtifact, NeverOverflowingCounter::ONE());
        r.mFacePsReady     = ctx.graph->getArtifactVersionToken(r.mFacePsArtifact, NeverOverflowingCounter::ONE());
        r.mAllFacesPsReady = ctx.graph->getArtifactVersionToken(r.mAllFacesPsArtifact, NeverOverflowingCounter::ONE());

        return r;
    }

    bool valid() const { return mCubemap && mCubemapReady; }

    AutoRef<Texture> cubemap() const { return mCubemap; }

    // Capture the OOO token for this frame's cubemap publish. Must be called before
    // addFrameNode — the OOO token targets the *next* version, so it must be created
    // while the current version is still the latest.
    TokenPtr beginFrame() { return mCtx.graph->getArtifactVersionToken(mCubemapReady, NeverOverflowingCounter::OOO()); }

    // Adds the cubemap render node. outPayloads receives the command buffer(s) to submit.
    NodePtr addFrameNode(int frameCounter, std::vector<AutoRef<GpuPayload>> & outPayloads) {
        if (mImpl == 1) {
            NodeDesc desc("render cubemap", [this, frameCounter, &outPayloads]() {
                GN_VVTRACE(sLogger, "frame {}: rendering cubemap (impl 1)", frameCounter);
                auto vsContent = mFaceVsArtifact->content<ShaderArtifactContent>();
                auto psContent = mFacePsArtifact->content<ShaderArtifactContent>();
                if (!vsContent || !psContent || !vsContent->shader || !psContent->shader) {
                    GN_ERROR(sLogger, "Impl 1: missing shaders");
                    return;
                }

                for (uint32_t f = 0; f < 6; ++f) {
                    RasterTarget rt;
                    rt.colorTargets.append(RasterTarget::ColorTarget(makeFaceView(mCubemap, f)));
                    rt.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

                    GpuRaster::CreateParameters rcp;
                    rcp.gpu    = mCtx.gpu;
                    rcp.target = &rt;

                    // Bind the face's source image at set=0, binding=0 for the fragment sampler2D.
                    GpuResourceView faceView;
                    faceView.resource = mFaceTextures[f];
                    GpuResourceTable resources;
                    resources.resize(1);
                    resources[0].resize(1);
                    resources[0][0].resize(1);
                    resources[0][0][0] = faceView;

                    GpuRaster::DrawParameters dp;
                    dp.vs                   = vsContent->shader;
                    dp.ps                   = psContent->shader;
                    dp.geometry.vertexCount = 3; // fullscreen triangle, no VBO
                    dp.resources            = resources;

                    auto rast = GpuRaster::create("cubemap-face", rcp);
                    rast->draw(dp);
                    outPayloads.push_back(rast->seal());
                }
                mCtx.graph->publishArtifact(mCubemapReady, AutoRef<TextureArtifactContent>(new TextureArtifactContent(mCubemap)));
            });
            desc.dependsOn(mFaceVsReady).dependsOn(mFacePsReady);
            return mCtx.graph->addNode(desc);
        } else {
            // Single pass: 6 color targets simultaneously, fragment shader writes all 6 faces.
            NodeDesc desc("render cubemap", [this, frameCounter, &outPayloads]() {
                GN_VVTRACE(sLogger, "frame {}: rendering cubemap (impl 2)", frameCounter);
                auto vsContent = mFaceVsArtifact->content<ShaderArtifactContent>();
                auto psContent = mAllFacesPsArtifact->content<ShaderArtifactContent>();
                if (!vsContent || !psContent || !vsContent->shader || !psContent->shader) {
                    GN_ERROR(sLogger, "Impl 2: missing shaders");
                    return;
                }

                GpuRaster::CreateParameters rcp;
                rcp.gpu = mCtx.gpu;
                RasterTarget rt;
                for (uint32_t f = 0; f < 6; ++f) { rt.colorTargets.append(RasterTarget::ColorTarget(makeFaceView(mCubemap, f))); }
                rt.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                rcp.target = &rt;

                // Bind all 6 face textures at set=0, binding=0..5 for the fragment shader.
                GpuResourceTable resources;
                resources.resize(1);
                resources[0].resize(6);
                for (uint32_t f = 0; f < 6; ++f) {
                    resources[0][f].resize(1);
                    GpuResourceView v;
                    v.resource         = mFaceTextures[f];
                    resources[0][f][0] = v;
                }

                GpuRaster::DrawParameters dp;
                dp.vs                   = vsContent->shader;
                dp.ps                   = psContent->shader;
                dp.geometry.vertexCount = 3;
                dp.resources            = resources;

                auto rast = GpuRaster::create("cubemap-all-faces", rcp);
                rast->draw(dp);
                outPayloads.push_back(rast->seal());
                mCtx.graph->publishArtifact(mCubemapReady, AutoRef<TextureArtifactContent>(new TextureArtifactContent(mCubemap)));
            });
            desc.dependsOn(mFaceVsReady).dependsOn(mAllFacesPsReady);
            return mCtx.graph->addNode(desc);
        }
    }

    // Tracks the last payload of the previous frame's cubemap render.
    // Used to prevent write-after-write hazard on the shared cubemap texture.
    AutoRef<GpuPayload> prevPayload;

private:
    SharedCtx                       mCtx;
    AutoRef<Texture>                mCubemap;
    std::array<AutoRef<Texture>, 6> mFaceTextures; // source images loaded from disk, one per face
    ArtifactPtr                     mCubemapReady;
    ArtifactPtr                     mFaceVsArtifact, mFacePsArtifact, mAllFacesPsArtifact;
    TokenPtr                        mFaceVsReady, mFacePsReady, mAllFacesPsReady;
    int                             mImpl = 1;
};

// ─── CubeDraw ─────────────────────────────────────────────────────────────────
//
// Draws a rotating 3D cube to the swapchain. Each cube face is shaded with the
// solid color that matches the corresponding cubemap face.
class CubeDraw {
public:
    static CubeDraw create(const SharedCtx & ctx) {
        CubeDraw cd;
        cd.mCtx = ctx;

        // Depth buffer sized to the swapchain; one mip, one face, depth format.
        auto depthFmt = ctx.gpu->caps().defaultDepthFormat;
        if (depthFmt == gfx::img::PixelFormat::UNKNOWN()) {
            GN_ERROR(sLogger, "CubeDraw: GPU has no usable depth format");
            return {};
        }
        cd.mDepthTex =
            Texture::create("cube-draw depth", Texture::CreateParameters {
                                                   .context    = ctx.gpu,
                                                   .descriptor = Texture::Descriptor {}.setFormat(depthFmt).setDimensions(ctx.width, ctx.height).setLevels(1),
                                               });
        if (!cd.mDepthTex) {
            GN_ERROR(sLogger, "CubeDraw: failed to create depth texture");
            return {};
        }

        GpuResourceView depthView;
        depthView.resource = cd.mDepthTex;
        cd.mRenderTarget.colorTargets.append(RasterTarget::ColorTarget {});
        cd.mRenderTarget.setDepthStencilTarget(depthView);
        cd.mRenderTarget.setClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        cd.mRenderTarget.setClearDepth(1.0f);
        cd.mRenderTarget.states.setCullMode(RasterState::CULL_NONE);
        cd.mRenderTarget.states.setDepthState(RasterState::DepthState {.func = RasterState::Compare::LESS, .write = true});

        // Upload cube vertex and index data synchronously; no need for graph-node machinery
        // since the data is static and the upload completes before any frame node runs.
        if (!cd.uploadGeometry()) return {};

        cd.mVsArtifact = ctx.graph->createArtifact("cube-draw VS");
        cd.mPsArtifact = ctx.graph->createArtifact("cube-draw PS");

        ctx.graph->addNode(NodeDesc("create cube-draw shaders", [graph = ctx.graph, gpu = ctx.gpu, vsArt = cd.mVsArtifact, psArt = cd.mPsArtifact]() {
            auto vs = GpuShader::create(
                {.context = gpu, .name = "cube-draw VS", .binary = kCubeDrawVertSpv, .size = kCubeDrawVertSpvSize * sizeof(unsigned int), .entry = "main"});
            auto ps = GpuShader::create(
                {.context = gpu, .name = "cube-draw PS", .binary = kCubeDrawFragSpv, .size = kCubeDrawFragSpvSize * sizeof(unsigned int), .entry = "main"});
            if (!vs || !ps) {
                GN_ERROR(sLogger, "CubeDraw: failed to create shaders");
                return;
            }
            graph->publishArtifact(vsArt, AutoRef<ShaderArtifactContent>(new ShaderArtifactContent(vs)));
            graph->publishArtifact(psArt, AutoRef<ShaderArtifactContent>(new ShaderArtifactContent(ps)));
        }));

        cd.mVsReady = ctx.graph->getArtifactVersionToken(cd.mVsArtifact, NeverOverflowingCounter::ONE());
        cd.mPsReady = ctx.graph->getArtifactVersionToken(cd.mPsArtifact, NeverOverflowingCounter::ONE());

        return cd;
    }

    bool valid() const { return mDepthTex && mVb && mIb && mVsArtifact && mPsArtifact; }

    RasterTarget & rasterTarget() { return mRenderTarget; }

    // Adds the per-frame draw-cube node. outPayload is filled when the node executes.
    // Returns the node handle to wait on.
    NodePtr addFrameNode(const TokenPtr & cubemapToken, const fx2::SharedShaderConstants::Snapshot & sscSnapshot, const AutoRef<Texture> & cubemap,
                         float elapsed, AutoRef<GpuPayload> & outPayload) {
        return mCtx.graph->addNode(
            NodeDesc("draw cube",
                     [this, cubemap, elapsed, sscSnapshot, &outPayload]() {
                         auto vsContent = mVsArtifact->content<ShaderArtifactContent>();
                         auto psContent = mPsArtifact->content<ShaderArtifactContent>();
                         if (!vsContent || !psContent || !vsContent->shader || !psContent->shader) {
                             GN_ERROR(sLogger, "CubeDraw: missing shaders");
                             return;
                         }

                         glm::mat4 model = glm::rotate(glm::mat4(1.f), elapsed * 0.5f, glm::normalize(glm::vec3(0.5f, 1.0f, 0.3f)));

                         GpuRaster::CreateParameters rcp;
                         rcp.gpu    = mCtx.gpu;
                         rcp.target = &mRenderTarget;

                         RasterGeometry geom;
                         geom.format.attributes.append(
                             RasterGeometry::VertexAttribute {.location = 0, .offset = 0, .format = RasterGeometry::AttributeFormat::F32_3});
                         geom.format.attributes.append(
                             RasterGeometry::VertexAttribute {.location = 1, .offset = 12, .format = RasterGeometry::AttributeFormat::F32_1});
                         geom.vertices.append(RasterGeometry::GeometryBuffer {.buffer = mVb, .offset = 0, .stride = kVertexStride});
                         geom.indices    = RasterGeometry::GeometryBuffer {.buffer = mIb, .offset = 0, .stride = sizeof(uint16_t)};
                         geom.indexCount = kIndexCount;

                         // set=0 → SSC resources (camera + scene UBOs + env textures)
                         // set=1, binding=0 → assembled cubemap
                         GpuResourceView cubeView;
                         cubeView.resource = cubemap;
                         GpuResourceSet cubeSet;
                         cubeSet.resize(1);
                         cubeSet[0].resize(1);
                         cubeSet[0][0] = cubeView;

                         GpuResourceTable resources;
                         resources.append(sscSnapshot.set0Resources); // set 0
                         resources.append(cubeSet);                   // set 1

                         GpuRaster::DrawParameters dp;
                         dp.vs        = vsContent->shader;
                         dp.ps        = psContent->shader;
                         dp.geometry  = geom;
                         dp.resources = resources;
                         // Push constant: model matrix only (view+proj come from camera UBO)
                         dp.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(model), reinterpret_cast<const uint8_t *>(&model)));

                         auto rast = GpuRaster::create("cube-display", rcp);
                         rast->draw(dp);
                         outPayload = rast->seal();
                     })
                .dependsOn(cubemapToken)
                .dependsOn(mVsReady)
                .dependsOn(mPsReady));
    }

private:
    struct CubeVertex {
        float x, y, z;   // location=0 in cube-draw.vert
        float faceIndex; // location=1 in cube-draw.vert; 0–5 encoding the face
    };

    static constexpr uint32_t kVertexStride = sizeof(CubeVertex); // 16 bytes
    static constexpr uint32_t kIndexCount   = 36;                 // 6 faces × 2 triangles × 3 vertices

    bool uploadGeometry() {
        // clang-format off
        static constexpr CubeVertex kVertices[24] = {
            // face 0 (+X, right): CCW from outside
            { 0.5f, -0.5f,  0.5f, 0.f}, { 0.5f,  0.5f,  0.5f, 0.f}, { 0.5f,  0.5f, -0.5f, 0.f}, { 0.5f, -0.5f, -0.5f, 0.f},
            // face 1 (-X, left)
            {-0.5f, -0.5f, -0.5f, 1.f}, {-0.5f,  0.5f, -0.5f, 1.f}, {-0.5f,  0.5f,  0.5f, 1.f}, {-0.5f, -0.5f,  0.5f, 1.f},
            // face 2 (+Y, top)
            {-0.5f,  0.5f,  0.5f, 2.f}, { 0.5f,  0.5f,  0.5f, 2.f}, { 0.5f,  0.5f, -0.5f, 2.f}, {-0.5f,  0.5f, -0.5f, 2.f},
            // face 3 (-Y, bottom)
            {-0.5f, -0.5f, -0.5f, 3.f}, { 0.5f, -0.5f, -0.5f, 3.f}, { 0.5f, -0.5f,  0.5f, 3.f}, {-0.5f, -0.5f,  0.5f, 3.f},
            // face 4 (+Z, front)
            {-0.5f, -0.5f,  0.5f, 4.f}, { 0.5f, -0.5f,  0.5f, 4.f}, { 0.5f,  0.5f,  0.5f, 4.f}, {-0.5f,  0.5f,  0.5f, 4.f},
            // face 5 (-Z, back)
            { 0.5f, -0.5f, -0.5f, 5.f}, {-0.5f, -0.5f, -0.5f, 5.f}, {-0.5f,  0.5f, -0.5f, 5.f}, { 0.5f,  0.5f, -0.5f, 5.f},
        };
        static constexpr uint16_t kIndices[36] = {
             0, 1, 2,  0, 2, 3,  // face 0
             4, 5, 6,  4, 6, 7,  // face 1
             8, 9,10,  8,10,11,  // face 2
            12,13,14, 12,14,15,  // face 3
            16,17,18, 16,18,19,  // face 4
            20,21,22, 20,22,23,  // face 5
        };
        // clang-format on

        mVb = Buffer::create("cube VB", {.context = mCtx.gpu, .size = sizeof(kVertices)});
        mIb = Buffer::create("cube IB", {.context = mCtx.gpu, .size = sizeof(kIndices)});
        if (!mVb || !mIb) {
            GN_ERROR(sLogger, "CubeDraw: failed to create vertex/index buffers");
            return false;
        }

        if (!mVb->setContent(ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(kVertices), sizeof(kVertices)))) {
            GN_ERROR(sLogger, "CubeDraw: vertex buffer upload failed");
            return false;
        }
        if (!mIb->setContent(ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(kIndices), sizeof(kIndices)))) {
            GN_ERROR(sLogger, "CubeDraw: index buffer upload failed");
            return false;
        }
        return true;
    }

    SharedCtx        mCtx;
    AutoRef<Texture> mDepthTex;
    RasterTarget     mRenderTarget;
    AutoRef<Buffer>  mVb, mIb;
    ArtifactPtr      mVsArtifact, mPsArtifact;
    TokenPtr         mVsReady, mPsReady;
};

// ─── Test helpers ──────────────────────────────────────────────────────────────

static void verifyFace0(const AutoRef<GpuContext> & gpu, const AutoRef<Texture> & cubemap, int frame) {
    gpu->waitForIdle();
    gfx::img::Image image = cubemap->readback();
    if (image.empty()) {
        GN_WARN(sLogger, "Frame {}: cubemap readback returned empty image", frame);
        return;
    }
    auto pixels = image.plane().toRGBA8(image.data());
    if (pixels.empty()) {
        GN_WARN(sLogger, "Frame {}: toRGBA8 returned empty vector", frame);
        return;
    }
    // Just log — face content is loaded from disk so the exact color is not known ahead of time.
    const auto & px = pixels[0];
    GN_INFO(sLogger, "Frame {}: cubemap face 0 pixel[0] = ({},{},{},{})", frame, px.r, px.g, px.b, px.a);
    GN_INFO(sLogger, "Frame {}: readback PASSED (non-empty pixel data)", frame);
}

// ─── main ──────────────────────────────────────────────────────────────────────

int main(int argc, const char ** argv) {
    // ── Parse arguments ────────────────────────────────────────────────────────
    bool testMode = false;
    int  impl     = 1;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == 't') {
            testMode = true;
        } else if (strcmp(argv[i], "--impl") == 0 && i + 1 < argc) {
            impl = std::atoi(argv[++i]);
        }
    }
    if (impl != 1 && impl != 2) {
        GN_ERROR(sLogger, "--impl must be 1 or 2 (got {})", impl);
        return -1;
    }

    enableCRTMemoryCheck();
    GN_INFO(sLogger, "open-graph-render-to-cube  impl={}  testMode={}", impl, testMode);

    // ── GPU context ────────────────────────────────────────────────────────────
    auto gpuContext = GpuContext::create("gpu_context", GpuContext::CreateParameters {});
    if (!gpuContext) return -1;

    // ── Window + Vulkan surface ────────────────────────────────────────────────
    uint32_t                     windowWidth  = 1280;
    uint32_t                     windowHeight = 720;
    std::unique_ptr<win::Window> window;
    intptr_t                     surface = 0;
    if (!testMode) {
        const char * captions[2] = {"Garnet 3D - Render To Cube (face-by-face)", "Garnet 3D - Render To Cube (single-pass MRT)"};
        window                   = std::unique_ptr<win::Window>(win::createWindow(win::WindowCreateParameters {
            .caption      = captions[impl - 1],
            .clientWidth  = windowWidth,
            .clientHeight = windowHeight,
        }));
        if (!window) return -1;
        window->show();
        auto clientSize = window->getClientSize();
        windowWidth     = clientSize.x;
        windowHeight    = clientSize.y;
        surface         = window->createVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle());
        if (!surface) return -1;
    }

    // ── Open graph ─────────────────────────────────────────────────────────────
    auto graph = Graph::create();
    if (!graph) return -1;

    auto ssc = fx2::SharedShaderConstants::create({.gpu = gpuContext});
    if (!ssc) return -1;

    SharedCtx ctx {.graph = graph, .gpu = gpuContext, .ssc = ssc, .width = windowWidth, .height = windowHeight};

    // ── Swapchain ──────────────────────────────────────────────────────────────
    Swapchain::CreateDesc scDesc {.gpu = gpuContext, .width = windowWidth, .height = windowHeight};
    if (surface) scDesc.setSurface(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) return -1;

    // ── Renderer setup ─────────────────────────────────────────────────────────
    auto cubemapRenderer = CubemapRenderer::create(ctx, impl);
    if (!cubemapRenderer.valid()) return -1;

    auto cubeDraw = CubeDraw::create(ctx);
    if (!cubeDraw.valid()) return -1;

    GN_INFO(sLogger, "Starting render loop...");

    const auto startTime    = std::chrono::steady_clock::now();
    int        totalFrames  = testMode ? 10 : 0;
    int        frameCounter = 1;
    while (totalFrames == 0 || frameCounter++ <= totalFrames) {
        if (window && !window->runUntilNoNewEvents()) break;

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) {
            GN_ERROR(sLogger, "Swapchain prepare failed");
            return -1;
        }

        const float elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - startTime).count();

        // Camera: fixed at +Z looking at origin.
        // Build camToWorld from the lookAt view inverse so the orientation quaternion
        // that SSC's UBO packer expects is consistent with the view matrix it will derive.
        {
            static const glm::vec3 kEye(0.f, 0.f, 3.f), kTarget(0.f), kUp(0.f, 1.f, 0.f);
            glm::mat4              camToWorld     = glm::inverse(glm::lookAtRH(kEye, kTarget, kUp));
            ssc->set0.camera.cameraPosition       = kEye;
            ssc->set0.camera.cameraOrientation    = glm::quat_cast(glm::mat3(camToWorld));
            ssc->set0.camera.cameraFov            = ArcDegree(45.f);
            ssc->set0.camera.nearPlane            = 0.1f;
            ssc->set0.camera.farPlane             = 100.f;
            ssc->set0.frameConstants.frameCounter = frameCounter;
            cubeDraw.rasterTarget().setColorTarget(0, frame.view);
            const auto rasterSize              = cubeDraw.rasterTarget().calcRasterSizeInPixel();
            ssc->set0.camera.aspectRatio       = (float) rasterSize.x / (float) rasterSize.y;
            ssc->set0.camera.viewWidthInPixel  = rasterSize.x;
            ssc->set0.camera.viewHeightInPixel = rasterSize.y;
        }
        fx2::SharedShaderConstants::Snapshot sscSnapshot = ssc->takeSnapshot();

        std::vector<AutoRef<GpuPayload>> cubemapPayloads;
        AutoRef<GpuPayload>              presentPayload;

        // Capture OOO token before adding the cubemap render node so it targets
        // exactly the version that this frame's node will publish.
        TokenPtr cubemapThisFrame = cubemapRenderer.beginFrame();

        cubemapRenderer.addFrameNode(frameCounter, cubemapPayloads);

        // The draw-cube node depends on cubemapThisFrame so it only runs after this
        // frame's cubemap has been published.
        NodePtr presentNode = cubeDraw.addFrameNode(cubemapThisFrame, sscSnapshot, cubemapRenderer.cubemap(), elapsed, presentPayload);

        graph->waitForToken(graph->getNodeCompletionToken(presentNode));

        // ── GPU submission ─────────────────────────────────────────────────────
        // Single submit: cubemap work followed by the cube draw. Ordering between
        // them is implicit (shared command buffer). We wait for prevPayload (WAW
        // guard against the previous frame's cubemap write) and frame.ready
        // (swapchain image acquisition).
        {
            GpuContext::SubmitParameters sp(StrA::format("frame {}", frameCounter));
            if (cubemapRenderer.prevPayload) sp.waitFor(cubemapRenderer.prevPayload);
            // Submit UBO + env uploads before the draw that samples them.
            for (const auto & p : sscSnapshot.set0Payloads) sp.appendWork(p);
            sp.waitFor(frame.ready);
            sp.appendWorks(cubemapPayloads);
            sp.appendWork(presentPayload);
            gpuContext->submit(sp);
            if (!cubemapPayloads.empty()) cubemapRenderer.prevPayload = cubemapPayloads.back();
        }
        swapchain->present(*presentPayload);

        if (testMode && frameCounter == 2) { verifyFace0(gpuContext, cubemapRenderer.cubemap(), frameCounter - 1); }
    }

    graph->waitForIdle();
    gpuContext->waitForIdle();
    // Destroy the sample-owned surface after the swapchain, before the GPU context (the Vulkan instance).
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpuContext->getVulkanInstanceHandle(), surface);
    GN_INFO(sLogger, "open-graph-render-to-cube finished.");
    return 0;
}
