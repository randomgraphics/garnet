#include <garnet/GNgpu2.h>
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>

#include <cmath>
#include <cstdio>
#include <vector>

#include "bindless-sample-vert.spv.h"
#include "bindless-sample-frag.spv.h"

using namespace GN;
using namespace GN::gpu2;
using namespace GN::win;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.gpu2-bindless");

struct Vertex {
    float pos[2];
    float uv[2];
};

struct PushConstants {
    float    transform[4]; // scaleX, scaleY, offsetX, offsetY
    uint32_t textureId;
    uint32_t pad[3];
};

// Generate procedural patterns for visual verification
static gfx::img::Image makePatternImage(uint32_t w, uint32_t h, int patternType) {
    gfx::img::Extent3D extent;
    extent.set(w, h, 1);
    gfx::img::PlaneDesc planeDesc = gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), extent);
    gfx::img::ImageDesc imageDesc = gfx::img::ImageDesc::make(planeDesc, 1, 1, 1);
    gfx::img::Image     img(imageDesc);
    uint8_t *           p = (uint8_t *) img.data();
    for (uint32_t y = 0; y < h; ++y) {
        for (uint32_t x = 0; x < w; ++x) {
            uint8_t * px = &p[(y * w + x) * 4];
            if (patternType == 0) {
                // Checkerboard red and white
                bool c = ((x / 16) ^ (y / 16)) & 1;
                px[0]  = c ? 240 : 255;
                px[1]  = c ? 50 : 255;
                px[2]  = c ? 50 : 255;
                px[3]  = 255;
            } else if (patternType == 1) {
                // Radial rings cyan / blue
                float dx   = (float) x - w * 0.5f;
                float dy   = (float) y - h * 0.5f;
                float dist = std::sqrt(dx * dx + dy * dy);
                bool  ring = (static_cast<int>(dist / 12.0f) % 2) == 0;
                px[0]      = ring ? 20 : 0;
                px[1]      = ring ? 200 : 100;
                px[2]      = ring ? 240 : 200;
                px[3]      = 255;
            } else if (patternType == 2) {
                // Diagonal stripes green / yellow
                bool stripe = ((x + y) / 16) & 1;
                px[0]       = stripe ? 230 : 20;
                px[1]       = stripe ? 220 : 180;
                px[2]       = stripe ? 30 : 40;
                px[3]       = 255;
            } else {
                // Orange / Magenta gradient
                px[0] = static_cast<uint8_t>(255 * x / (w - 1));
                px[1] = static_cast<uint8_t>(120 * y / (h - 1));
                px[2] = static_cast<uint8_t>(220 * (w - 1 - x) / (w - 1));
                px[3] = 255;
            }
        }
    }
    return img;
}

static bool verifyBackbuffer(const GpuResourceView & view, uint32_t width, uint32_t height) {
    auto tex = view.texture();
    if (!tex) {
        GN_ERROR(sLogger, "verifyBackbuffer: could not get backbuffer texture");
        return false;
    }
    gfx::img::Image image = tex->readback();
    if (image.empty()) {
        GN_ERROR(sLogger, "verifyBackbuffer: readback returned empty image");
        return false;
    }
    auto pixels = image.plane().toRGBA8(image.data());
    if (pixels.empty()) {
        GN_ERROR(sLogger, "verifyBackbuffer: could not convert pixels to RGBA8");
        return false;
    }

    // Check center pixel of top-left quadrant (Quad 0: red checkerboard)
    uint32_t q0X = width / 4;
    uint32_t q0Y = height / 4;
    auto     p0  = pixels[q0Y * width + q0X];
    if (p0.r < 30) {
        GN_ERROR(sLogger, "verifyBackbuffer: Q0 failed, expected reddish pixel, got ({},{},{},{})", p0.r, p0.g, p0.b, p0.a);
        return false;
    }

    // Check center pixel of top-right quadrant (Quad 1: cyan/blue rings)
    uint32_t q1X = (width * 3) / 4;
    uint32_t q1Y = height / 4;
    auto     p1  = pixels[q1Y * width + q1X];
    if (p1.b < 50) {
        GN_ERROR(sLogger, "verifyBackbuffer: Q1 failed, expected bluish pixel, got ({},{},{},{})", p1.r, p1.g, p1.b, p1.a);
        return false;
    }

    GN_INFO(sLogger, "verifyBackbuffer: PASSED (Q0=({},{},{},{}), Q1=({},{},{},{}))", p0.r, p0.g, p0.b, p0.a, p1.r, p1.g, p1.b, p1.a);
    return true;
}

int main(int argc, const char ** argv) {
    bool testMode     = argc > 1 && argv[1][0] == 't';
    bool windowedTest = argc > 1 && argv[1][0] == 'w';

    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {});
    if (!gpu) {
        std::fprintf(stderr, "Failed to create GPU context\n");
        return -1;
    }

    uint32_t                W = 1280, H = 720;
    std::unique_ptr<Window> window;
    intptr_t                surface = 0;

    if (!testMode) {
        window.reset(createWindow(WindowCreateParameters {.caption = "Garnet - GPU2 Bindless Rendering", .clientWidth = W, .clientHeight = H}));
        if (!window) {
            std::fprintf(stderr, "Failed to create window\n");
            return -1;
        }
        window->show();
        surface = window->createVulkanSurfaceHandle(gpu->getVulkanInstanceHandle());
        if (!surface) {
            std::fprintf(stderr, "Failed to get Vulkan surface\n");
            return -1;
        }

        auto acquiredSize = window->getClientSize();
        if (acquiredSize.x != W || acquiredSize.y != H) {
            W = acquiredSize.x;
            H = acquiredSize.y;
        }
    }

    // 1. Swapchain for presentation or headless offscreen rendering
    Swapchain::CreateDesc scDesc;
    scDesc.setGpu(gpu).setName("swapchain").setDimensions(W, H);
    if (surface) scDesc.setSurface(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) {
        std::fprintf(stderr, "Failed to create swapchain\n");
        return -1;
    }

    // 2. Global persistent DescriptorHeap
    auto heap = bindless::DescriptorHeap::create("sample-heap", {.gpu = gpu, .capacity = 256});
    if (!heap) {
        std::fprintf(stderr, "Failed to create bindless descriptor heap\n");
        return -1;
    }

    // 3. Create 4 procedural textures and register them into the heap using atomic batch allocation
    constexpr uint32_t TEX_SIZE = 128;
    AutoRef<Texture>   textures[4];
    GpuResourceView    textureViews[4];
    uint32_t           textureSlots[4];

    for (int i = 0; i < 4; ++i) {
        textures[i] = Texture::create(
            StrA::format("pattern-{}", i),
            {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(TEX_SIZE, TEX_SIZE).setLevels(1)});
        if (!textures[i] || !textures[i]->setContent(makePatternImage(TEX_SIZE, TEX_SIZE, i))) {
            std::fprintf(stderr, "Failed to create and upload texture %d\n", i);
            return -1;
        }
        textureViews[i] = GpuResourceView(textures[i]);
    }

    if (!heap->allocate(ArrayView<const GpuResourceView>(textureViews, 4), ArrayView<uint32_t>(textureSlots, 4))) {
        std::fprintf(stderr, "Failed to batch allocate descriptor heap slots for textures\n");
        return -1;
    }

    // 4. Create vertex and index buffers for unit quad [-0.5, 0.5]
    const Vertex vertices[4] = {
        {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f}, {0.0f, 1.0f}},
    };
    const uint16_t indices[6] = {0, 2, 1, 0, 3, 2};

    auto vb = Buffer::create("quad-vb", {.context = gpu, .size = sizeof(vertices), .mappable = true});
    auto ib = Buffer::create("quad-ib", {.context = gpu, .size = sizeof(indices), .mappable = true});
    if (!vb || !ib) {
        std::fprintf(stderr, "Failed to create geometry buffers\n");
        return -1;
    }
    {
        auto mv = vb->map();
        if (mv.data()) std::memcpy(mv.data(), vertices, sizeof(vertices));
        auto mi = ib->map();
        if (mi.data()) std::memcpy(mi.data(), indices, sizeof(indices));
    }

    // 5. Setup RasterGeometry
    RasterGeometry geom;
    geom.format.attributes.append(RasterGeometry::VertexAttribute {
        .location = 0,
        .binding  = 0,
        .offset   = 0,
        .format   = RasterGeometry::AttributeFormat::F32_2,
    });
    geom.format.attributes.append(RasterGeometry::VertexAttribute {
        .location = 1,
        .binding  = 0,
        .offset   = sizeof(float) * 2,
        .format   = RasterGeometry::AttributeFormat::F32_2,
    });

    RasterGeometry::GeometryBuffer geomVb;
    geomVb.buffer = vb;
    geomVb.offset = 0;
    geomVb.stride = sizeof(Vertex);
    geom.vertices.append(geomVb);
    geom.vertexCount = 4;

    geom.indices.buffer = ib;
    geom.indices.offset = 0;
    geom.indices.stride = sizeof(uint16_t);
    geom.indexCount     = 6;

    // 6. Create Shaders
    auto vs = GpuShader::create({.context = gpu, .name = "bindless-vs", .binary = kBindlessSampleVertSpv, .size = sizeof(kBindlessSampleVertSpv)});
    auto ps = GpuShader::create({.context = gpu, .name = "bindless-ps", .binary = kBindlessSampleFragSpv, .size = sizeof(kBindlessSampleFragSpv)});
    if (!vs || !ps) {
        std::fprintf(stderr, "Failed to create bindless shaders\n");
        return -1;
    }

    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget {});
    rt.states.setCullMode(RasterState::CULL_NONE);
    rt.setClearColor(0.12f, 0.12f, 0.16f, 1.0f); // dark slate background

    int totalFrames  = (testMode || windowedTest) ? 10 : 0;
    int frameCounter = 0;

    while (totalFrames == 0 || frameCounter < totalFrames) {
        if (window && !window->runUntilNoNewEvents()) break;

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) {
            std::fprintf(stderr, "prepare() returned empty frame\n");
            return -1;
        }

        rt.setColorTarget(0, frame.view);

        // Bindless Raster configuration: POD struct with global descriptor heap at Set 0
        bindless::Raster::CreateParameters rcp;
        rcp.gpu              = gpu;
        rcp.target           = &rt;
        rcp.heap             = heap;
        rcp.heapSetIndex     = 0;
        rcp.maxImmediateSize = sizeof(PushConstants);

        auto raster = bindless::Raster::create("bindless-raster", rcp);
        if (!raster) {
            std::fprintf(stderr, "Failed to create bindless raster recorder\n");
            return -1;
        }

        // Draw 4 quadrant quads, each consuming a distinct texture index via push constants
        const float quadScale = 0.75f;
        const struct {
            float    ox, oy;
            uint32_t slot;
        } quadLayout[4] = {
            {-0.45f, -0.45f, textureSlots[0]}, // Top-Left: Red checkerboard
            {0.45f, -0.45f, textureSlots[1]},  // Top-Right: Cyan rings
            {-0.45f, 0.45f, textureSlots[2]},  // Bottom-Left: Diagonal stripes
            {0.45f, 0.45f, textureSlots[3]},   // Bottom-Right: Color gradient
        };

        for (int q = 0; q < 4; ++q) {
            PushConstants pc {};
            pc.transform[0] = quadScale;
            pc.transform[1] = quadScale;
            pc.transform[2] = quadLayout[q].ox;
            pc.transform[3] = quadLayout[q].oy;
            pc.textureId    = quadLayout[q].slot;

            raster->recordDraw({
                .vs         = vs,
                .ps         = ps,
                .geometry   = geom,
                .immediates = ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(&pc), sizeof(pc)),
            });
        }

        // Draw 5th center quad: pulsing and cycling textures over time to demonstrate UPDATE_AFTER_BIND & zero rebind
        float         pulse = 0.35f + 0.1f * std::sin(frameCounter * 0.08f);
        PushConstants centerPc {};
        centerPc.transform[0] = pulse;
        centerPc.transform[1] = pulse;
        centerPc.transform[2] = 0.0f;
        centerPc.transform[3] = 0.0f;
        centerPc.textureId    = textureSlots[(frameCounter / 30) % 4];

        raster->recordDraw({
            .vs         = vs,
            .ps         = ps,
            .geometry   = geom,
            .immediates = ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(&centerPc), sizeof(centerPc)),
        });

        // Seal recorded pass into self-contained GpuPayload
        auto payload = raster->seal();
        if (!payload) {
            std::fprintf(stderr, "Failed to seal bindless raster pass\n");
            return -1;
        }

        gpu->submit(GpuContext::SubmitParameters("bindless-frame").appendWork(payload).waitFor(frame.ready));

        // In test mode: verify backbuffer before presenting
        if (testMode && frameCounter == totalFrames - 1) {
            gpu->waitForIdle();
            if (!verifyBackbuffer(frame.view, W, H)) {
                std::fprintf(stderr, "Backbuffer verification failed\n");
                return -1;
            }
        }

        swapchain->present(*payload);
        ++frameCounter;
    }

    gpu->waitForIdle();
    rt.setColorTarget(0, {});
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);

    return 0;
}
