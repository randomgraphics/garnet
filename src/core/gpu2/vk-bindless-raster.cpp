#include "pch.h"
#include "vk-bindless-raster.h"
#include "vk-bindless-cnc.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-format-utils.h"
#if GN_BUILD_HAS_VULKAN
    #include "gpu2-text-vert.spv.h"
    #include "gpu2-text-frag.spv.h"
    #include <array>
    #include <cmath>
#endif

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.raster");

namespace GN::gpu2 {

#if GN_BUILD_HAS_VULKAN
namespace {
using GlyphRows = std::array<uint8_t, 7>;

GlyphRows asciiGlyphRows(uint8_t c) {
    switch (c) {
    case ' ':
        return {0, 0, 0, 0, 0, 0, 0};
    case '0':
        return {14, 17, 19, 21, 25, 17, 14};
    case '1':
        return {4, 12, 4, 4, 4, 4, 14};
    case '2':
        return {14, 17, 1, 2, 4, 8, 31};
    case '3':
        return {30, 1, 1, 14, 1, 1, 30};
    case '4':
        return {2, 6, 10, 18, 31, 2, 2};
    case '5':
        return {31, 16, 16, 30, 1, 1, 30};
    case '6':
        return {14, 16, 16, 30, 17, 17, 14};
    case '7':
        return {31, 1, 2, 4, 8, 8, 8};
    case '8':
        return {14, 17, 17, 14, 17, 17, 14};
    case '9':
        return {14, 17, 17, 15, 1, 1, 14};
    case 'A':
        return {14, 17, 17, 31, 17, 17, 17};
    case 'B':
        return {30, 17, 17, 30, 17, 17, 30};
    case 'C':
        return {14, 17, 16, 16, 16, 17, 14};
    case 'D':
        return {30, 17, 17, 17, 17, 17, 30};
    case 'E':
        return {31, 16, 16, 30, 16, 16, 31};
    case 'F':
        return {31, 16, 16, 30, 16, 16, 16};
    case 'G':
        return {14, 17, 16, 23, 17, 17, 15};
    case 'H':
        return {17, 17, 17, 31, 17, 17, 17};
    case 'I':
        return {14, 4, 4, 4, 4, 4, 14};
    case 'J':
        return {7, 2, 2, 2, 2, 18, 12};
    case 'K':
        return {17, 18, 20, 24, 20, 18, 17};
    case 'L':
        return {16, 16, 16, 16, 16, 16, 31};
    case 'M':
        return {17, 27, 21, 21, 17, 17, 17};
    case 'N':
        return {17, 25, 21, 19, 17, 17, 17};
    case 'O':
        return {14, 17, 17, 17, 17, 17, 14};
    case 'P':
        return {30, 17, 17, 30, 16, 16, 16};
    case 'Q':
        return {14, 17, 17, 17, 21, 18, 13};
    case 'R':
        return {30, 17, 17, 30, 20, 18, 17};
    case 'S':
        return {15, 16, 16, 14, 1, 1, 30};
    case 'T':
        return {31, 4, 4, 4, 4, 4, 4};
    case 'U':
        return {17, 17, 17, 17, 17, 17, 14};
    case 'V':
        return {17, 17, 17, 17, 17, 10, 4};
    case 'W':
        return {17, 17, 17, 21, 21, 21, 10};
    case 'X':
        return {17, 17, 10, 4, 10, 17, 17};
    case 'Y':
        return {17, 17, 10, 4, 4, 4, 4};
    case 'Z':
        return {31, 1, 2, 4, 8, 16, 31};
    case 'a':
        return {0, 0, 14, 1, 15, 17, 15};
    case 'b':
        return {16, 16, 30, 17, 17, 17, 30};
    case 'c':
        return {0, 0, 14, 16, 16, 17, 14};
    case 'd':
        return {1, 1, 15, 17, 17, 17, 15};
    case 'e':
        return {0, 0, 14, 17, 31, 16, 14};
    case 'f':
        return {6, 8, 8, 30, 8, 8, 8};
    case 'g':
        return {0, 15, 17, 17, 15, 1, 14};
    case 'h':
        return {16, 16, 30, 17, 17, 17, 17};
    case 'i':
        return {4, 0, 12, 4, 4, 4, 14};
    case 'j':
        return {2, 0, 6, 2, 2, 18, 12};
    case 'k':
        return {16, 16, 18, 20, 24, 20, 18};
    case 'l':
        return {12, 4, 4, 4, 4, 4, 14};
    case 'm':
        return {0, 0, 26, 21, 21, 21, 21};
    case 'n':
        return {0, 0, 30, 17, 17, 17, 17};
    case 'o':
        return {0, 0, 14, 17, 17, 17, 14};
    case 'p':
        return {0, 30, 17, 17, 30, 16, 16};
    case 'q':
        return {0, 15, 17, 17, 15, 1, 1};
    case 'r':
        return {0, 0, 22, 24, 16, 16, 16};
    case 's':
        return {0, 0, 15, 16, 14, 1, 30};
    case 't':
        return {8, 8, 30, 8, 8, 9, 6};
    case 'u':
        return {0, 0, 17, 17, 17, 19, 13};
    case 'v':
        return {0, 0, 17, 17, 17, 10, 4};
    case 'w':
        return {0, 0, 17, 17, 21, 21, 10};
    case 'x':
        return {0, 0, 17, 10, 4, 10, 17};
    case 'y':
        return {0, 17, 17, 17, 15, 1, 14};
    case 'z':
        return {0, 0, 31, 2, 4, 8, 31};
    case '.':
        return {0, 0, 0, 0, 0, 12, 12};
    case ',':
        return {0, 0, 0, 0, 4, 4, 8};
    case ':':
        return {0, 12, 12, 0, 12, 12, 0};
    case ';':
        return {0, 12, 12, 0, 4, 4, 8};
    case '!':
        return {4, 4, 4, 4, 4, 0, 4};
    case '?':
        return {14, 17, 1, 2, 4, 0, 4};
    case '-':
        return {0, 0, 0, 31, 0, 0, 0};
    case '_':
        return {0, 0, 0, 0, 0, 0, 31};
    case '+':
        return {0, 4, 4, 31, 4, 4, 0};
    case '=':
        return {0, 0, 31, 0, 31, 0, 0};
    case '/':
        return {1, 2, 2, 4, 8, 8, 16};
    case '\\':
        return {16, 8, 8, 4, 2, 2, 1};
    case '(':
        return {2, 4, 8, 8, 8, 4, 2};
    case ')':
        return {8, 4, 2, 2, 2, 4, 8};
    case '[':
        return {14, 8, 8, 8, 8, 8, 14};
    case ']':
        return {14, 2, 2, 2, 2, 2, 14};
    case '<':
        return {2, 4, 8, 16, 8, 4, 2};
    case '>':
        return {8, 4, 2, 1, 2, 4, 8};
    case '"':
        return {10, 10, 10, 0, 0, 0, 0};
    case '\'':
        return {4, 4, 4, 0, 0, 0, 0};
    case '*':
        return {0, 17, 10, 31, 10, 17, 0};
    case '#':
        return {10, 10, 31, 10, 31, 10, 10};
    case '%':
        return {17, 2, 4, 8, 16, 17, 0};
    case '@':
        return {14, 17, 23, 21, 23, 16, 14};
    case '^':
        return {4, 10, 17, 0, 0, 0, 0};
    case '`':
        return {8, 4, 0, 0, 0, 0, 0};
    case '{':
        return {3, 4, 4, 8, 4, 4, 3};
    case '|':
        return {4, 4, 4, 4, 4, 4, 4};
    case '}':
        return {24, 4, 4, 2, 4, 4, 24};
    case '~':
        return {0, 0, 9, 22, 0, 0, 0};
    default:
        return {14, 17, 1, 2, 4, 0, 4};
    }
}

struct TextPushConstants {
    float    origin[2];
    float    glyphSize[2];
    float    viewportOrigin[2];
    float    viewportSize[2];
    float    color[4];
    uint32_t bitmap[2];
};
static_assert(sizeof(TextPushConstants) == 56);
} // namespace
#endif

static bool validateRasterTarget(const RasterTarget & target) {
    vk::Extent2D extent;
    for (const auto & color : target.colorTargets) {
        if (!attachmentExtent(color.view(), extent)) {
            GN_ERROR(sLogger, "createVkBindlessRaster: invalid color attachment subresource");
            return false;
        }
    }
    if (target.depthStencilTarget.texture && !attachmentExtent(target.depthStencilTarget.view(), extent)) {
        GN_ERROR(sLogger, "createVkBindlessRaster: invalid depth/stencil attachment subresource");
        return false;
    }
    return true;
}

#if GN_BUILD_HAS_VULKAN
bool GpuContextVulkan2::getOrCreateTextShaders(AutoRef<GpuShader> & vertex, AutoRef<GpuShader> & pixel) {
    std::lock_guard<std::mutex> lock(mTextShaderMutex);
    if (!mTextVertexShader || !mTextPixelShader) {
        AutoRef<GpuContext> context(this);
        mTextVertexShader =
            GpuShader::create({.context = context, .name = "gpu2-built-in-text-vertex", .binary = kGpu2TextVertSpv, .size = sizeof(kGpu2TextVertSpv)});
        mTextPixelShader =
            GpuShader::create({.context = context, .name = "gpu2-built-in-text-pixel", .binary = kGpu2TextFragSpv, .size = sizeof(kGpu2TextFragSpv)});
        if (!mTextVertexShader || !mTextPixelShader) {
            GN_ERROR(sLogger, "GpuContextVulkan2: failed to create built-in text shaders");
            mTextVertexShader = {};
            mTextPixelShader  = {};
            return false;
        }
    }
    vertex = mTextVertexShader;
    pixel  = mTextPixelShader;
    return true;
}
#endif

VkBindlessRaster::VkBindlessRaster(const StrA & name, AutoRef<GpuContextVulkan2> gpu, RasterTarget target, AutoRef<DescriptorHeap> heap, uint32_t heapSetIndex,
                                   vk::PipelineLayout pipelineLayout, vk::DescriptorPool passPool, std::vector<vk::DescriptorSet> passSets,
                                   size_t numberOfDrawsHint, uint32_t maxImmediateSize)
    : Raster(TYPE_INFO(), name), mGpu(std::move(gpu)), mRenderTarget(std::move(target)), mHeap(std::move(heap)), mHeapSetIndex(heapSetIndex),
      mPipelineLayout(pipelineLayout), mPassDescriptorPool(passPool), mPassDescriptorSets(std::move(passSets)), mMaxImmediateSize(maxImmediateSize),
      mStorage(std::make_unique<BindlessDrawStorage>(numberOfDrawsHint)) {
    if (maxImmediateSize && numberOfDrawsHint > mImmediateData.max_size() / maxImmediateSize)
        throw std::length_error("Bindless immediate storage size overflow");
    mImmediateData.reserve(numberOfDrawsHint * maxImmediateSize);
}

void VkBindlessRaster::recordText(const TextParameters & params) {
#if !GN_BUILD_HAS_VULKAN
    (void) params;
    GN_ERROR(sLogger, "VkBindlessRaster::recordText: built-in text rendering requires Vulkan");
#else
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordText: cannot record text into a sealed raster");
            return;
        }
    if (!params.text || !params.text[0]) return;
    if (!(params.glyphSize.x > 0.0f) || !(params.glyphSize.y > 0.0f) || !std::isfinite(params.glyphSize.x) || !std::isfinite(params.glyphSize.y)) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordText: glyph size must be finite and positive");
            return;
        }
    if (mMaxImmediateSize < sizeof(TextPushConstants)) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordText: Raster::CreateParameters::maxImmediateSize must be at least {} bytes", sizeof(TextPushConstants));
            return;
        }

    AutoRef<GpuShader> textVertexShader, textPixelShader;
    if (!mGpu->getOrCreateTextShaders(textVertexShader, textPixelShader)) return;

    RasterState state = mRenderTarget.states;
    // A positive Vulkan viewport reverses the apparent winding of these 2D glyph quads.
    state.cullMode                     = RasterState::CULL_NONE;
    const auto &            vp         = state.viewport;
    const Vector2<uint32_t> targetSize = mRenderTarget.calcRasterSizeInPixel();
    TextPushConstants       pc {};
    pc.origin[0]         = params.origin.x;
    pc.origin[1]         = params.origin.y;
    pc.glyphSize[0]      = params.glyphSize.x;
    pc.glyphSize[1]      = params.glyphSize.y;
    pc.viewportOrigin[0] = vp ? vp->x : 0.0f;
    pc.viewportOrigin[1] = vp ? vp->y : 0.0f;
    pc.viewportSize[0]   = (vp && vp->width != FLT_MAX) ? vp->width : static_cast<float>(targetSize.x);
    pc.viewportSize[1]   = (vp && vp->height != FLT_MAX) ? vp->height : static_cast<float>(targetSize.y);
    if (!(pc.viewportSize[0] > 0.0f) || !(pc.viewportSize[1] > 0.0f)) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordText: raster viewport must have positive dimensions");
            return;
        }
    pc.color[0] = params.color.x;
    pc.color[1] = params.color.y;
    pc.color[2] = params.color.z;
    pc.color[3] = params.color.w;

    RasterGeometry geometry;
    geometry.vertexCount = 6;
    for (const unsigned char * p = reinterpret_cast<const unsigned char *>(params.text); *p; ++p) {
        if (*p == '\r') continue;
        if (*p == '\n') {
            pc.origin[0] = params.origin.x;
            pc.origin[1] += params.glyphSize.y;
            continue;
        }
        const GlyphRows rows = asciiGlyphRows(*p);
        pc.bitmap[0] = pc.bitmap[1] = 0;
        for (uint32_t y = 0; y < 7; ++y) {
            for (uint32_t x = 0; x < 5; ++x) {
                if ((rows[y] & (1u << (4u - x))) == 0) continue;
                const uint32_t bit = y * 5u + x;
                pc.bitmap[bit / 32u] |= 1u << (bit % 32u);
            }
        }
        Raster::DrawParameters draw {.vs         = textVertexShader,
                                     .ps         = textPixelShader,
                                     .states     = state,
                                     .geometry   = geometry,
                                     .immediates = {reinterpret_cast<const uint8_t *>(&pc), sizeof(pc)}};
        recordDraw(draw);
        pc.origin[0] += params.glyphSize.x;
    }
#endif
}

VkBindlessRaster::~VkBindlessRaster() {
    if (!mSealed && mPassDescriptorPool && mGpu && mGpu->ready()) {
        mGpu->vulkanDevice().handle().destroyDescriptorPool(mPassDescriptorPool);
        mPassDescriptorPool = vk::DescriptorPool {};
    }
}

void VkBindlessRaster::recordDraw(const DrawParameters & params) {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordDraw: cannot record draws into a sealed raster");
            return;
        }
    if (!params.vs) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordDraw: vertex shader is required");
            return;
        }

    RasterState mergedState = mRenderTarget.states;
    mergeRenderState(mergedState, params.states);

    uint32_t immOffset = 0;
    uint32_t immSize   = 0;
    if (!params.immediates.empty()) {
        immOffset = static_cast<uint32_t>(mImmediateData.size());
        immSize   = static_cast<uint32_t>(params.immediates.size());
        mImmediateData.insert(mImmediateData.end(), params.immediates.begin(), params.immediates.end());
    }

    mStorage->draws.emplace_back(params, mergedState, &mStorage->pool, immOffset, immSize);
}

void VkBindlessRaster::addCleanupCallback(std::function<void()> cleanup) {
    if (cleanup) { mRetainedCleanups.push_back(std::move(cleanup)); }
}

AutoRef<GpuPayload> VkBindlessRaster::seal() {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::seal: raster is already sealed");
            return {};
        }
    mSealed = true;

    VkBindlessPayload::ConstructParameters cp;
    cp.gpu                = mGpu;
    cp.target             = std::move(mRenderTarget);
    cp.heap               = std::move(mHeap);
    cp.heapSetIndex       = mHeapSetIndex;
    cp.pipelineLayout     = mPipelineLayout;
    cp.storage            = std::move(mStorage);
    cp.immediateData      = std::move(mImmediateData);
    cp.retainedCleanups   = std::move(mRetainedCleanups);
    cp.passDescriptorPool = mPassDescriptorPool;
    cp.passDescriptorSets = std::move(mPassDescriptorSets);
    mPassDescriptorPool   = vk::DescriptorPool {};

    return AutoRef<GpuPayload>(new VkBindlessPayload(name, std::move(cp)));
}

AutoRef<Raster> createVkBindlessRaster(const StrA & name, const Raster::CreateParameters & cp) {
    AutoRef<GpuContextVulkan2> vkGpu(RuntimeType::cast<GpuContextVulkan2>(cp.gpu.get()));
    if (!vkGpu || !vkGpu->ready()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: null or unready GpuContextVulkan2");
            return {};
        }

    // The descriptor heap and related data will take over the entire specified set (heapSetIndex).
    // Fast conflict check: if pass resources conflict with the heap (stay in the same set as where the descriptor heap is), creation will fail.
    if (cp.heap && cp.heapSetIndex < cp.passResources.size() && !cp.passResources[cp.heapSetIndex].empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: passResources conflicts with descriptor heap set {}", cp.heapSetIndex);
            return {};
        }

    if (!cp.target || cp.target->empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: invalid or empty RasterTarget");
            return {};
        }
    if (!validateRasterTarget(*cp.target)) return {};

    auto *             vkHeap = RuntimeType::cast<VkBindlessDescriptorHeap>(cp.heap.get());
    vk::PipelineLayout pl     = vkGpu->bindlessPipelineLayoutCache().getOrCreate(cp, vkHeap);
    if (!pl) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: failed to obtain VkPipelineLayout");
            return {};
        }

    vk::DescriptorPool             passPool {};
    std::vector<vk::DescriptorSet> passSets;

    if (!buildBindlessPassDescriptorSets(*vkGpu, cp.passResources, cp.heapSetIndex, vk::ShaderStageFlagBits::eAllGraphics, passPool, passSets)) {
        GN_ERROR(sLogger, "createVkBindlessRaster: failed to build pass descriptor sets");
        return {};
    }

    return AutoRef<Raster>(
        new VkBindlessRaster(name, vkGpu, *cp.target, cp.heap, cp.heapSetIndex, pl, passPool, passSets, cp.numberOfDrawsHint, cp.maxImmediateSize));
}

} // namespace GN::gpu2
