#include <garnet/GNgpu2.h>
#include <garnet/GNwin.h>
#include <garnet/GNutil.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <memory>
#include <random>
#include <vector>

#include "bindless-sample-vert.spv.h"
#include "bindless-sample-frag.spv.h"
#include "bindless-sample-gem-vert.spv.h"
#include "bindless-sample-gem-frag.spv.h"

using namespace GN;
using namespace GN::gpu2;
using namespace GN::win;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.gpu2-bindless");

struct Vertex {
    float pos[3];
    float normal[3];
    float uv[2];
};

struct PushConstants {
    float    mvp[16];      // 64 bytes: Model-View-Projection matrix
    float    rotation[4];  // 16 bytes: orientation quaternion (qx, qy, qz, qw)
    float    colorTint[4]; // 16 bytes: RGBA color multiplier
    uint32_t textureId;    // 4 bytes:  bindless descriptor slot in Set 0
    float    shininess;    // 4 bytes:  specular exponent / effect param
    float    uvScale[2];   // 8 bytes:  UV coordinate scaling
    float    lightDir[4];  // 16 bytes: normalized light dir (xyz) + time/ambient (w)
};
static_assert(sizeof(PushConstants) == 128, "PushConstants size must be exactly 128 bytes");

struct SwarmObject {
    float     orbitRadius;
    float     orbitSpeed;
    float     orbitPhase;
    float     verticalAmp;
    float     verticalFreq;
    float     verticalPhase;
    glm::vec3 spinAxis;
    float     spinSpeed;
    float     scale;
    uint32_t  textureSlot;
    glm::vec4 colorTint;
    float     shininess;
};

// ─── Procedural Texture Generation ──────────────────────────────────────────

static void hsvToRgb(float h, float s, float v, uint8_t & r, uint8_t & g, uint8_t & b) {
    float c       = v * s;
    float h_prime = std::fmod(h * 6.0f, 6.0f);
    if (h_prime < 0.0f) h_prime += 6.0f;
    float x  = c * (1.0f - std::abs(std::fmod(h_prime, 2.0f) - 1.0f));
    float m  = v - c;
    float rf = 0.0f, gf = 0.0f, bf = 0.0f;
    if (h_prime < 1.0f) {
        rf = c;
        gf = x;
    } else if (h_prime < 2.0f) {
        rf = x;
        gf = c;
    } else if (h_prime < 3.0f) {
        gf = c;
        bf = x;
    } else if (h_prime < 4.0f) {
        gf = x;
        bf = c;
    } else if (h_prime < 5.0f) {
        rf = x;
        bf = c;
    } else {
        rf = c;
        bf = x;
    }
    r = static_cast<uint8_t>(std::clamp((rf + m) * 255.0f, 0.0f, 255.0f));
    g = static_cast<uint8_t>(std::clamp((gf + m) * 255.0f, 0.0f, 255.0f));
    b = static_cast<uint8_t>(std::clamp((bf + m) * 255.0f, 0.0f, 255.0f));
}

static gfx::img::Image makeProceduralTextureImage(uint32_t w, uint32_t h, uint32_t patternId) {
    gfx::img::Extent3D extent;
    extent.set(w, h, 1);
    gfx::img::PlaneDesc planeDesc = gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), extent);
    gfx::img::ImageDesc imageDesc = gfx::img::ImageDesc::make(planeDesc, 1, 1, 1);
    gfx::img::Image     img(imageDesc);
    uint8_t *           p = reinterpret_cast<uint8_t *>(img.data());

    // Golden-ratio hue distribution for distinct, vibrant color harmonies
    float   baseHue = std::fmod(patternId * 0.618033988749895f, 1.0f);
    float   secHue  = std::fmod(baseHue + 0.35f, 1.0f);
    uint8_t r1, g1, b1, r2, g2, b2;
    hsvToRgb(baseHue, 0.85f, 0.95f, r1, g1, b1);
    hsvToRgb(secHue, 0.70f, 0.40f, r2, g2, b2);

    const uint32_t family = patternId % 16;
    for (uint32_t y = 0; y < h; ++y) {
        float ny = (float) y / (float) h;
        for (uint32_t x = 0; x < w; ++x) {
            float     nx = (float) x / (float) w;
            uint8_t * px = &p[(y * w + x) * 4];
            bool      fg = false;

            switch (family) {
            case 0: // Checkerboard
                fg = (((x / 16) ^ (y / 16)) & 1) != 0;
                break;
            case 1: { // Concentric rings / target
                float dx   = nx - 0.5f;
                float dy   = ny - 0.5f;
                float dist = std::sqrt(dx * dx + dy * dy);
                fg         = (static_cast<int>(dist * 20.0f) % 2) == 0;
                break;
            }
            case 2: // Diagonal hazard stripes
                fg = (((x + y) / 16) & 1) != 0;
                break;
            case 3: { // Hexagonal honeycomb pattern
                float hx = nx * 8.0f;
                float hy = ny * 8.0f * 1.1547f;
                int   cx = static_cast<int>(std::floor(hx));
                int   cy = static_cast<int>(std::floor(hy));
                fg       = ((cx + cy) & 1) != 0;
                break;
            }
            case 4: // Circuit board tracks
                fg = ((x % 32 < 4) || (y % 32 < 4)) && (((x / 32) ^ (y / 32)) & 1);
                break;
            case 5: // Crosshatch weave
                fg = ((x % 16 < 4) || (y % 16 < 4));
                break;
            case 6: { // Radial spiral vortex
                float dx    = nx - 0.5f;
                float dy    = ny - 0.5f;
                float angle = std::atan2(dy, dx);
                float dist  = std::sqrt(dx * dx + dy * dy);
                fg          = (std::sin(angle * 4.0f + dist * 30.0f) > 0.0f);
                break;
            }
            case 7: { // Voronoi / cellular texture
                int   cellX = x / 32;
                int   cellY = y / 32;
                float fx    = (float) (x % 32) - 16.0f;
                float fy    = (float) (y % 32) - 16.0f;
                fg          = (fx * fx + fy * fy < 160.0f) ^ ((cellX ^ cellY) & 1);
                break;
            }
            case 8: { // Sine wave interference
                float v = std::sin(nx * 12.0f) + std::cos(ny * 12.0f);
                fg      = (v > 0.0f);
                break;
            }
            case 9: { // Dot matrix / glowing LED
                float dx = (float) (x % 16) - 8.0f;
                float dy = (float) (y % 16) - 8.0f;
                fg       = (dx * dx + dy * dy < 36.0f);
                break;
            }
            case 10: // Diamond argyle grid
                fg = (((x + y) / 16) % 2) == (((x - y + 512) / 16) % 2);
                break;
            case 11: { // Starburst bloom
                float dx    = nx - 0.5f;
                float dy    = ny - 0.5f;
                float angle = std::atan2(dy, dx);
                fg          = (std::sin(angle * 8.0f) > 0.0f);
                break;
            }
            case 12: { // Masonry brick
                int  row   = y / 16;
                int  shift = (row & 1) ? 16 : 0;
                int  col   = (x + shift) / 32;
                bool edge  = (y % 16 == 0) || ((x + shift) % 32 == 0);
                fg         = !edge && ((row + col) % 2 == 0);
                break;
            }
            case 13: { // Digital camouflage
                uint32_t seed = (x / 16) * 1973 + (y / 16) * 9277 + patternId * 26699;
                fg            = ((seed ^ (seed >> 5)) & 3) > 1;
                break;
            }
            case 14: { // Quartered crest
                bool left = (nx < 0.5f);
                bool top  = (ny < 0.5f);
                fg        = (left ^ top) || (nx > 0.45f && nx < 0.55f) || (ny > 0.45f && ny < 0.55f);
                break;
            }
            default: { // Sci-fi reticle glyph
                float dx   = nx - 0.5f;
                float dy   = ny - 0.5f;
                float dist = std::sqrt(dx * dx + dy * dy);
                fg         = (dist > 0.2f && dist < 0.25f) || (std::abs(dx) < 0.02f) || (std::abs(dy) < 0.02f);
                break;
            }
            }

            if (fg) {
                px[0] = r1;
                px[1] = g1;
                px[2] = b1;
                px[3] = 255;
            } else {
                px[0] = r2;
                px[1] = g2;
                px[2] = b2;
                px[3] = 255;
            }
        }
    }
    return img;
}

static gfx::img::Image makeDynamicTextureImage(uint32_t w, uint32_t h, uint32_t slot, float animTime) {
    gfx::img::Extent3D extent;
    extent.set(w, h, 1);
    gfx::img::PlaneDesc planeDesc = gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), extent);
    gfx::img::ImageDesc imageDesc = gfx::img::ImageDesc::make(planeDesc, 1, 1, 1);
    gfx::img::Image     img(imageDesc);
    uint8_t *           p = reinterpret_cast<uint8_t *>(img.data());

    float t       = animTime * 2.5f + slot * 0.392f;
    float baseHue = std::fmod(t * 0.15f + slot * 0.0625f, 1.0f);

    uint8_t r1, g1, b1, r2, g2, b2;
    hsvToRgb(baseHue, 0.95f, 1.0f, r1, g1, b1);
    hsvToRgb(std::fmod(baseHue + 0.5f, 1.0f), 0.85f, 0.35f, r2, g2, b2);

    for (uint32_t y = 0; y < h; ++y) {
        float ny = (float) y / (float) h - 0.5f;
        for (uint32_t x = 0; x < w; ++x) {
            float     nx   = (float) x / (float) w - 0.5f;
            uint8_t * px   = &p[(y * w + x) * 4];
            float     dist = std::sqrt(nx * nx + ny * ny);
            float     ang  = std::atan2(ny, nx);

            float wave = std::sin(ang * 4.0f + dist * 25.0f - t * 4.0f);
            float mix  = (wave + 1.0f) * 0.5f;

            px[0] = static_cast<uint8_t>(r1 * mix + r2 * (1.0f - mix));
            px[1] = static_cast<uint8_t>(g1 * mix + g2 * (1.0f - mix));
            px[2] = static_cast<uint8_t>(b1 * mix + b2 * (1.0f - mix));
            px[3] = 255;
        }
    }
    return img;
}

// ─── 3D Geometry: Solid Cube & Holographic Octahedron Crystal ────────────────

static void createCubeGeometry(AutoRef<GpuContext> gpu, AutoRef<Buffer> & outVb, AutoRef<Buffer> & outIb, RasterGeometry & outGeom) {
    const Vertex vertices[24] = {
        // Front (+Z)
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        // Back (-Z)
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        // Top (+Y)
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        // Bottom (-Y)
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        // Right (+X)
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        // Left (-X)
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    };

    const uint16_t indices[36] = {
        0,  1,  2,  0,  2,  3,  // Front
        4,  5,  6,  4,  6,  7,  // Back
        8,  9,  10, 8,  10, 11, // Top
        12, 13, 14, 12, 14, 15, // Bottom
        16, 17, 18, 16, 18, 19, // Right
        20, 21, 22, 20, 22, 23, // Left
    };

    outVb = Buffer::create("cube-vb", {.context = gpu, .size = sizeof(vertices), .mappable = true});
    outIb = Buffer::create("cube-ib", {.context = gpu, .size = sizeof(indices), .mappable = true});
    GN_ASSERT(outVb && outIb);

    {
        auto mv = outVb->map();
        if (mv.data()) std::memcpy(mv.data(), vertices, sizeof(vertices));
        auto mi = outIb->map();
        if (mi.data()) std::memcpy(mi.data(), indices, sizeof(indices));
    }

    outGeom.format.attributes.clear();
    outGeom.format.attributes.append(RasterGeometry::VertexAttribute {
        .location = 0,
        .binding  = 0,
        .offset   = 0,
        .format   = RasterGeometry::AttributeFormat::F32_3,
    });
    outGeom.format.attributes.append(RasterGeometry::VertexAttribute {
        .location = 1,
        .binding  = 0,
        .offset   = sizeof(float) * 3,
        .format   = RasterGeometry::AttributeFormat::F32_3,
    });
    outGeom.format.attributes.append(RasterGeometry::VertexAttribute {
        .location = 2,
        .binding  = 0,
        .offset   = sizeof(float) * 6,
        .format   = RasterGeometry::AttributeFormat::F32_2,
    });

    outGeom.vertices.clear();
    RasterGeometry::GeometryBuffer geomVb;
    geomVb.buffer = outVb;
    geomVb.offset = 0;
    geomVb.stride = sizeof(Vertex);
    outGeom.vertices.append(geomVb);
    outGeom.vertexCount = 24;

    outGeom.indices.buffer = outIb;
    outGeom.indices.offset = 0;
    outGeom.indices.stride = sizeof(uint16_t);
    outGeom.indexCount     = 36;
}

static void createOctahedronGeometry(AutoRef<GpuContext> gpu, AutoRef<Buffer> & outVb, AutoRef<Buffer> & outIb, RasterGeometry & outGeom) {
    const glm::vec3 top(0.0f, 0.9f, 0.0f);
    const glm::vec3 bottom(0.0f, -0.9f, 0.0f);
    const glm::vec3 c0(0.65f, 0.0f, 0.0f);
    const glm::vec3 c1(0.0f, 0.0f, 0.65f);
    const glm::vec3 c2(-0.65f, 0.0f, 0.0f);
    const glm::vec3 c3(0.0f, 0.0f, -0.65f);

    const glm::vec3 triangles[8][3] = {
        // Top 4 faces (CCW from outside)
        {top, c1, c0},
        {top, c2, c1},
        {top, c3, c2},
        {top, c0, c3},
        // Bottom 4 faces (CCW from outside)
        {bottom, c0, c1},
        {bottom, c1, c2},
        {bottom, c2, c3},
        {bottom, c3, c0},
    };

    Vertex   vertices[24];
    uint16_t indices[24];

    for (int f = 0; f < 8; ++f) {
        glm::vec3 v0   = triangles[f][0];
        glm::vec3 v1   = triangles[f][1];
        glm::vec3 v2   = triangles[f][2];
        glm::vec3 norm = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        vertices[f * 3 + 0] = {{v0.x, v0.y, v0.z}, {norm.x, norm.y, norm.z}, {0.5f, 1.0f}};
        vertices[f * 3 + 1] = {{v1.x, v1.y, v1.z}, {norm.x, norm.y, norm.z}, {0.0f, 0.0f}};
        vertices[f * 3 + 2] = {{v2.x, v2.y, v2.z}, {norm.x, norm.y, norm.z}, {1.0f, 0.0f}};

        indices[f * 3 + 0] = static_cast<uint16_t>(f * 3 + 0);
        indices[f * 3 + 1] = static_cast<uint16_t>(f * 3 + 1);
        indices[f * 3 + 2] = static_cast<uint16_t>(f * 3 + 2);
    }

    outVb = Buffer::create("gem-vb", {.context = gpu, .size = sizeof(vertices), .mappable = true});
    outIb = Buffer::create("gem-ib", {.context = gpu, .size = sizeof(indices), .mappable = true});
    GN_ASSERT(outVb && outIb);

    {
        auto mv = outVb->map();
        if (mv.data()) std::memcpy(mv.data(), vertices, sizeof(vertices));
        auto mi = outIb->map();
        if (mi.data()) std::memcpy(mi.data(), indices, sizeof(indices));
    }

    outGeom.format.attributes.clear();
    outGeom.format.attributes.append(RasterGeometry::VertexAttribute {
        .location = 0,
        .binding  = 0,
        .offset   = 0,
        .format   = RasterGeometry::AttributeFormat::F32_3,
    });
    outGeom.format.attributes.append(RasterGeometry::VertexAttribute {
        .location = 1,
        .binding  = 0,
        .offset   = sizeof(float) * 3,
        .format   = RasterGeometry::AttributeFormat::F32_3,
    });
    outGeom.format.attributes.append(RasterGeometry::VertexAttribute {
        .location = 2,
        .binding  = 0,
        .offset   = sizeof(float) * 6,
        .format   = RasterGeometry::AttributeFormat::F32_2,
    });

    outGeom.vertices.clear();
    RasterGeometry::GeometryBuffer geomVb;
    geomVb.buffer = outVb;
    geomVb.offset = 0;
    geomVb.stride = sizeof(Vertex);
    outGeom.vertices.append(geomVb);
    outGeom.vertexCount = 24;

    outGeom.indices.buffer = outIb;
    outGeom.indices.offset = 0;
    outGeom.indices.stride = sizeof(uint16_t);
    outGeom.indexCount     = 24;
}

// ─── Backbuffer Verification for Test Mode ───────────────────────────────────

static bool verifyBackbuffer(const GpuResourceView & view, uint32_t /*width*/, uint32_t /*height*/, uint32_t drawCount) {
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

    uint32_t nonBackgroundCount = 0;
    for (const auto & px : pixels) {
        if (px.r > 25 || px.g > 25 || px.b > 35) { ++nonBackgroundCount; }
    }

    if (nonBackgroundCount < 500) {
        GN_ERROR(sLogger, "verifyBackbuffer: FAILED, expected >= 500 non-background pixels, got {}", nonBackgroundCount);
        return false;
    }

    GN_INFO(sLogger, "verifyBackbuffer: PASSED (draws={}, rendered non-background pixels={})", drawCount, nonBackgroundCount);
    return true;
}

// ─── Main Application ────────────────────────────────────────────────────────

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
        window.reset(createWindow(WindowCreateParameters {
            .caption      = "Garnet - GPU2 Bindless Rendering Demonstration (Alternating Pipelines)",
            .clientWidth  = W,
            .clientHeight = H,
        }));
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

    // 1. Swapchain for presentation or offscreen rendering
    Swapchain::CreateDesc scDesc;
    scDesc.setGpu(gpu).setName("swapchain").setDimensions(W, H);
    if (surface) scDesc.setSurface(surface);
    auto swapchain = Swapchain::create(scDesc);
    if (!swapchain) {
        std::fprintf(stderr, "Failed to create swapchain\n");
        return -1;
    }

    // 2. Depth buffer texture
    auto depthTex = Texture::create("depth-buffer",
                                    {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(W, H)});
    if (!depthTex) {
        std::fprintf(stderr, "Failed to create depth texture\n");
        return -1;
    }
    GpuResourceView depthView(depthTex);

    // 3. Global persistent DescriptorHeap
    constexpr uint32_t HEAP_CAPACITY = 1024;
    auto               heap          = bindless::DescriptorHeap::create("global-bindless-heap", {.gpu = gpu, .capacity = HEAP_CAPACITY});
    if (!heap) {
        std::fprintf(stderr, "Failed to create bindless descriptor heap\n");
        return -1;
    }

    // 4. Create 256 distinct procedural textures and batch allocate into heap
    constexpr uint32_t            NUM_TEXTURES = 256;
    constexpr uint32_t            TEX_SIZE     = 128;
    std::vector<AutoRef<Texture>> textures(NUM_TEXTURES);
    std::vector<GpuResourceView>  textureViews(NUM_TEXTURES);
    std::vector<uint32_t>         textureSlots(NUM_TEXTURES);

    for (uint32_t i = 0; i < NUM_TEXTURES; ++i) {
        textures[i] = Texture::create(
            StrA::format("proc-tex-{}", i),
            {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(TEX_SIZE, TEX_SIZE).setLevels(1)});
        if (!textures[i] || !textures[i]->setContent(makeProceduralTextureImage(TEX_SIZE, TEX_SIZE, i))) {
            std::fprintf(stderr, "Failed to create and upload procedural texture %u\n", i);
            return -1;
        }
        textureViews[i] = GpuResourceView(textures[i]);
    }

    if (!heap->allocate(ArrayView<const GpuResourceView>(textureViews.data(), NUM_TEXTURES), ArrayView<uint32_t>(textureSlots.data(), NUM_TEXTURES))) {
        std::fprintf(stderr, "Failed to batch allocate descriptor heap slots for %u textures\n", NUM_TEXTURES);
        return -1;
    }

    // 5. Dynamic Streaming Textures: dedicated textures updated live in-place (demonstrating UPDATE_AFTER_BIND)
    constexpr uint32_t            NUM_STREAMING_SLOTS = 16;
    std::vector<AutoRef<Texture>> streamingTextures(NUM_STREAMING_SLOTS);
    for (uint32_t s = 0; s < NUM_STREAMING_SLOTS; ++s) {
        streamingTextures[s] = Texture::create(
            StrA::format("stream-tex-{}", s),
            {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(TEX_SIZE, TEX_SIZE).setLevels(1)});
        GN_ASSERT(streamingTextures[s]);
    }

    // 6. Geometry buffers for two distinct meshes: Cube and Crystal Octahedron
    AutoRef<Buffer> cubeVb, cubeIb, gemVb, gemIb;
    RasterGeometry  cubeGeom, gemGeom;
    createCubeGeometry(gpu, cubeVb, cubeIb, cubeGeom);
    createOctahedronGeometry(gpu, gemVb, gemIb, gemGeom);

    // 7. Shaders for 2 Distinct Pipelines:
    //    Pipeline 1: Solid Lit 3D Cubes (Blinn-Phong directional lighting + specular)
    //    Pipeline 2: Holographic 3D Crystals (normal-extruding vertex wave + chromatic dispersion + Fresnel rim glow)
    auto vsCube = GpuShader::create({.context = gpu, .name = "cube-vs", .binary = kBindlessSampleVertSpv, .size = sizeof(kBindlessSampleVertSpv)});
    auto psCube = GpuShader::create({.context = gpu, .name = "cube-ps", .binary = kBindlessSampleFragSpv, .size = sizeof(kBindlessSampleFragSpv)});
    auto vsGem  = GpuShader::create({.context = gpu, .name = "gem-vs", .binary = kBindlessSampleGemVertSpv, .size = sizeof(kBindlessSampleGemVertSpv)});
    auto psGem  = GpuShader::create({.context = gpu, .name = "gem-ps", .binary = kBindlessSampleGemFragSpv, .size = sizeof(kBindlessSampleGemFragSpv)});
    if (!vsCube || !psCube || !vsGem || !psGem) {
        std::fprintf(stderr, "Failed to create bindless shaders\n");
        return -1;
    }

    // 8. Configure Render Target with Depth Testing & Backface Culling
    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget {});
    rt.setDepthStencilTarget(depthView);
    rt.setClearColor(0.04f, 0.04f, 0.07f, 1.0f); // deep space background
    rt.setClearDepth(1.0f);
    rt.states.setDepthState({RasterState::Compare::LESS, true});
    rt.states.setCullMode(RasterState::CULL_BACK);
    rt.states.setFrontFace(RasterState::FRONT_CCW);

    // 9. Pre-generate up to 100,000 Swarm Objects alternating between Cubes and Crystals
    constexpr uint32_t       MAX_OBJECTS = 100000;
    std::vector<SwarmObject> swarm(MAX_OBJECTS);

    std::mt19937                   rng(1337);
    std::uniform_real_distribution distRadius(3.5f, 54.0f);
    std::uniform_real_distribution distSpeed(0.15f, 0.75f);
    std::uniform_real_distribution distAngle(0.0f, 6.2831853f);
    std::uniform_real_distribution distVertAmp(0.5f, 9.0f);
    std::uniform_real_distribution distVertFreq(0.4f, 1.8f);
    std::uniform_real_distribution distSpinSpeed(0.5f, 3.5f);
    std::uniform_real_distribution distScale(0.35f, 0.70f);
    std::uniform_real_distribution distAxis(-1.0f, 1.0f);
    std::uniform_real_distribution distShininess(16.0f, 64.0f);
    std::uniform_real_distribution distTint(0.80f, 1.0f);

    for (uint32_t i = 0; i < MAX_OBJECTS; ++i) {
        SwarmObject & obj = swarm[i];
        obj.orbitRadius   = distRadius(rng);
        obj.orbitSpeed    = distSpeed(rng) * (obj.orbitRadius > 22.0f ? 0.35f : 0.85f);
        obj.orbitPhase    = distAngle(rng);
        obj.verticalAmp   = distVertAmp(rng);
        obj.verticalFreq  = distVertFreq(rng);
        obj.verticalPhase = distAngle(rng);

        glm::vec3 axis(distAxis(rng), distAxis(rng), distAxis(rng));
        if (glm::length(axis) < 0.01f) axis = glm::vec3(0.0f, 1.0f, 0.0f);
        obj.spinAxis    = glm::normalize(axis);
        obj.spinSpeed   = distSpinSpeed(rng);
        obj.scale       = distScale(rng);
        obj.textureSlot = textureSlots[i % NUM_TEXTURES];
        obj.shininess   = distShininess(rng);
        obj.colorTint   = glm::vec4(distTint(rng), distTint(rng), distTint(rng), 1.0f);
    }

    // Application state
    // Default: 20,000 draws (10,000 Cubes + 10,000 Crystals alternating every draw call!)
    uint32_t activeDrawCount  = testMode ? 2000 : 20000;
    bool     streamingEnabled = true;
    bool     animPaused       = false;
    bool     autoCameraOrbit  = true;

    float cameraDistance  = 46.0f;
    float cameraAngle     = 0.0f;
    float cameraElevation = 0.40f;
    float simTime         = 0.0f;

    float avgFps      = 0.0f;
    float avgFrameMs  = 0.0f;
    float avgRecordMs = 0.0f;

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    int totalFrames  = (testMode || windowedTest) ? 10 : 0;
    int frameCounter = 0;

    bool prevKeyStates[static_cast<size_t>(KeyCode::NUM_KEYS)] = {};
    auto isKeyJustPressed                                      = [&](KeyCode code) -> bool {
        if (!window) return false;
        bool down                                = window->getKeyStatus(code).down;
        bool wasDown                             = prevKeyStates[static_cast<size_t>(code)];
        prevKeyStates[static_cast<size_t>(code)] = down;
        return down && !wasDown;
    };

    while (totalFrames == 0 || frameCounter < totalFrames) {
        if (window && !window->runUntilNoNewEvents()) break;

        auto  now        = std::chrono::high_resolution_clock::now();
        float frameDt    = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime    = now;
        float instantFps = frameDt > 0.0f ? (1.0f / frameDt) : 60.0f;
        avgFps           = (avgFps == 0.0f) ? instantFps : (avgFps * 0.95f + instantFps * 0.05f);
        avgFrameMs       = (avgFrameMs == 0.0f) ? (frameDt * 1000.0f) : (avgFrameMs * 0.95f + (frameDt * 1000.0f) * 0.05f);

        // ─── Input Handling ──────────────────────────────────────────────────
        if (window) {
            if (window->getKeyStatus(KeyCode::ESCAPE).down) break;

            // Draw count presets: 1 -> 10k, 2 -> 25k, 3 -> 50k, 4 -> 75k, 5 -> 100k
            if (isKeyJustPressed(KeyCode::_1)) activeDrawCount = 10000;
            if (isKeyJustPressed(KeyCode::_2)) activeDrawCount = 25000;
            if (isKeyJustPressed(KeyCode::_3)) activeDrawCount = 50000;
            if (isKeyJustPressed(KeyCode::_4)) activeDrawCount = 75000;
            if (isKeyJustPressed(KeyCode::_5)) activeDrawCount = 100000;

            // Fine adjustments with UP / DOWN arrows (±5000 draws)
            if (isKeyJustPressed(KeyCode::UP) && activeDrawCount + 5000 <= MAX_OBJECTS) { activeDrawCount += 5000; }
            if (isKeyJustPressed(KeyCode::DOWN) && activeDrawCount >= 5000) { activeDrawCount -= 5000; }

            // Feature toggles
            if (isKeyJustPressed(KeyCode::SPACEBAR)) animPaused = !animPaused;
            if (isKeyJustPressed(KeyCode::S)) streamingEnabled = !streamingEnabled;
            if (isKeyJustPressed(KeyCode::C)) autoCameraOrbit = !autoCameraOrbit;
            if (isKeyJustPressed(KeyCode::R)) {
                cameraDistance  = 46.0f;
                cameraElevation = 0.40f;
                cameraAngle     = 0.0f;
                activeDrawCount = 20000;
            }

            // Interactive Camera Controls
            if (window->getKeyStatus(KeyCode::A).down || window->getKeyStatus(KeyCode::LEFT).down) { cameraAngle -= 0.03f; }
            if (window->getKeyStatus(KeyCode::D).down || window->getKeyStatus(KeyCode::RIGHT).down) { cameraAngle += 0.03f; }
            if (window->getKeyStatus(KeyCode::W).down) { cameraElevation = std::min(1.45f, cameraElevation + 0.02f); }
            if (window->getKeyStatus(KeyCode::PAGEUP).down) { cameraElevation = std::min(1.45f, cameraElevation + 0.02f); }
            if (window->getKeyStatus(KeyCode::PAGEDOWN).down) { cameraElevation = std::max(-1.45f, cameraElevation - 0.02f); }
            if (window->getKeyStatus(KeyCode::Q).down) { cameraDistance = std::min(140.0f, cameraDistance + 0.6f); }
            if (window->getKeyStatus(KeyCode::E).down) { cameraDistance = std::max(6.0f, cameraDistance - 0.6f); }
        }

        if (!animPaused) {
            // Frame-rate dependent animation: advances by a constant increment per rendered frame
            // rather than elapsed wall time, so higher FPS results directly in faster animation speed.
            constexpr float kStepPerFrame = 0.0035f;
            simTime += kStepPerFrame;
            if (autoCameraOrbit) { cameraAngle += kStepPerFrame * 0.45f; }
        }

        // ─── Dynamic Live Texture Streaming (UPDATE_AFTER_BIND in action) ───
        if (streamingEnabled && !testMode) {
            for (uint32_t s = 0; s < NUM_STREAMING_SLOTS; ++s) {
                auto dynImg = makeDynamicTextureImage(TEX_SIZE, TEX_SIZE, s, simTime);
                streamingTextures[s]->setContent(dynImg);
                heap->update(textureSlots[s], GpuResourceView(streamingTextures[s]));
            }
        }

        Swapchain::Frame frame = swapchain->prepare();
        if (frame.view.empty()) {
            std::fprintf(stderr, "prepare() returned empty frame\n");
            return -1;
        }

        rt.setColorTarget(0, frame.view);

        // ─── Camera & Projection Setup ───────────────────────────────────────
        float     camX = cameraDistance * std::cos(cameraElevation) * std::sin(cameraAngle);
        float     camY = cameraDistance * std::sin(cameraElevation);
        float     camZ = cameraDistance * std::cos(cameraElevation) * std::cos(cameraAngle);
        glm::vec3 eye(camX, camY, camZ);
        glm::vec3 center(0.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);

        glm::mat4 view = glm::lookAtRH(eye, center, up);
        glm::mat4 proj = glm::perspectiveRH_ZO(glm::radians(55.0f), static_cast<float>(W) / static_cast<float>(H), 0.1f, 500.0f);
        proj[1][1] *= -1.0f; // Vulkan clip space Y inversion

        glm::mat4 viewProj = proj * view;

        const glm::vec3 lightDir = glm::normalize(glm::vec3(0.577f, 0.707f, 0.408f));
        constexpr float ambient  = 0.22f;

        // ─── Bindless Raster Recorder Creation ───────────────────────────────
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

        // Pre-allocate storage for high draw counts (up to 100k) to eliminate vector reallocations
        raster->reserve(activeDrawCount, activeDrawCount * sizeof(PushConstants));

        // ─── Alternating Pipeline Draw Call Recording Benchmark ─────────────
        // Every single draw alternates between:
        //   - Even draw: Pipeline 1 (Solid Lit Cube) + Cube Geometry
        //   - Odd draw:  Pipeline 2 (Holographic Crystal Gem) + Gem Geometry
        // Instancing CANNOT batch across alternating pipelines.
        // Bindless binds Set 0 once and switches pipelines with zero descriptor rebinding!
        auto tRecordStart = std::chrono::high_resolution_clock::now();

        for (uint32_t i = 0; i < activeDrawCount; ++i) {
            const SwarmObject & obj = swarm[i];

            float theta = obj.orbitPhase + obj.orbitSpeed * simTime;
            float posX  = obj.orbitRadius * std::cos(theta);
            float posZ  = obj.orbitRadius * std::sin(theta);
            float posY  = obj.verticalAmp * std::sin(obj.verticalFreq * simTime + obj.verticalPhase);

            float     spinAngle = obj.spinSpeed * simTime;
            glm::quat q         = glm::angleAxis(spinAngle, obj.spinAxis);

            float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
            float xx = q.x * x2, xy = q.x * y2, xz = q.x * z2;
            float yy = q.y * y2, yz = q.y * z2, zz = q.z * z2;
            float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;

            glm::mat4 model(1.0f);
            model[0][0] = (1.0f - (yy + zz)) * obj.scale;
            model[0][1] = (xy + wz) * obj.scale;
            model[0][2] = (xz - wy) * obj.scale;

            model[1][0] = (xy - wz) * obj.scale;
            model[1][1] = (1.0f - (xx + zz)) * obj.scale;
            model[1][2] = (yz + wx) * obj.scale;

            model[2][0] = (xz + wy) * obj.scale;
            model[2][1] = (yz - wx) * obj.scale;
            model[2][2] = (1.0f - (xx + yy)) * obj.scale;

            model[3][0] = posX;
            model[3][1] = posY;
            model[3][2] = posZ;

            glm::mat4 mvp = viewProj * model;

            bool isGem = (i & 1) != 0;

            PushConstants pc {};
            std::memcpy(pc.mvp, glm::value_ptr(mvp), sizeof(pc.mvp));
            pc.rotation[0]  = q.x;
            pc.rotation[1]  = q.y;
            pc.rotation[2]  = q.z;
            pc.rotation[3]  = q.w;
            pc.colorTint[0] = obj.colorTint.r;
            pc.colorTint[1] = obj.colorTint.g;
            pc.colorTint[2] = obj.colorTint.b;
            pc.colorTint[3] = obj.colorTint.a;
            pc.textureId    = obj.textureSlot;
            pc.shininess    = obj.shininess;
            pc.uvScale[0]   = 1.0f;
            pc.uvScale[1]   = 1.0f;
            pc.lightDir[0]  = lightDir.x;
            pc.lightDir[1]  = lightDir.y;
            pc.lightDir[2]  = lightDir.z;
            pc.lightDir[3]  = isGem ? simTime : ambient; // Gem shader uses lightDir.w as time

            // Alternating draw call: switches vertex shader, fragment shader, and geometry buffer!
            raster->recordDraw({
                .vs         = isGem ? vsGem : vsCube,
                .ps         = isGem ? psGem : psCube,
                .geometry   = isGem ? gemGeom : cubeGeom,
                .immediates = ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(&pc), sizeof(pc)),
            });
        }

        auto  tRecordEnd = std::chrono::high_resolution_clock::now();
        float recordMs   = std::chrono::duration<float, std::milli>(tRecordEnd - tRecordStart).count();
        avgRecordMs      = (avgRecordMs == 0.0f) ? recordMs : (avgRecordMs * 0.95f + recordMs * 0.05f);

        // Seal recorded pass into self-contained GpuPayload
        auto payload = raster->seal();
        if (!payload) {
            std::fprintf(stderr, "Failed to seal bindless raster pass\n");
            return -1;
        }

        gpu->submit(GpuContext::SubmitParameters("bindless-frame").appendWork(payload).waitFor(frame.ready));

        // ─── Test Mode Verification ──────────────────────────────────────────
        if (testMode && frameCounter == totalFrames - 1) {
            gpu->waitForIdle();
            if (!verifyBackbuffer(frame.view, W, H, activeDrawCount)) {
                std::fprintf(stderr, "Backbuffer verification failed\n");
                return -1;
            }
        }

        swapchain->present(*payload);

        // ─── Real-Time Telemetry HUD ─────────────────────────────────────────
        if (window && frameCounter % 10 == 0) {
            std::string title =
                StrA::format("Garnet Bindless | Draws: {} (Alternating Pipelines) | Textures: {} | CPU Record: {:.2f} ms | Frame: {:.2f} ms ({:.0f} FPS) "
                             "| Stream: {} | [1-5]: 10k-100k, [S]: Stream, [Space]: Pause, [WASD]: Orbit",
                             activeDrawCount, heap->size(), avgRecordMs, avgFrameMs, avgFps, streamingEnabled ? "ON" : "OFF")
                    .data();

#if GN_BUILD_HAS_MSW
            ::SetWindowTextA(reinterpret_cast<HWND>(window->getWindowHandle()), title.c_str());
#endif

            if (frameCounter % 60 == 0) { GN_INFO(sLogger, "{}", title); }
        }

        ++frameCounter;
    }

    gpu->waitForIdle();
    rt.setColorTarget(0, {});
    rt.setDepthStencilTarget({});
    depthTex.clear();
    swapchain.clear();
    if (window) window->destroyVulkanSurfaceHandle(gpu->getVulkanInstanceHandle(), surface);

    return 0;
}
