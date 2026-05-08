#include "vk-gpu-image-state.h"
#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

void TextureGpuImageState::reset(vk::Format format, uint32_t mipLevels, uint32_t arrayLayers, const ImageState & initialState) {
    mFormat = format;
    if (format == vk::Format::eUndefined || mipLevels == 0 || arrayLayers == 0) {
        mValidAspects = {};
        mNumMips      = 0;
        mNumLayers    = 0;
        mSubresourceStates.clear();
        return;
    }
    mNumMips   = mipLevels;
    mNumLayers = arrayLayers;
    mSubresourceStates.resize((size_t) mipLevels * (size_t) arrayLayers);

    // Determine the set of aspect planes the texture has from its format. For RGBA this is
    // {eColor}; for D24S8 it's {eDepth, eStencil}; for NV12-class formats it's {ePlane0, ePlane1}.
    // Cache it for later AND-against-incoming-mask in set().
    mValidAspects = rv::Image::determineImageAspect(format);
    for (auto & sr : mSubresourceStates) {
        forEachAspectBit(mValidAspects, [&](vk::ImageAspectFlagBits bit) { sr.planes_.emplace(bit, initialState); });
    }
}

bool TextureGpuImageState::set(const StrA & resourceName, const ImageState & newState, const GpuResourceView::SubresourceRange & range,
                               vk::ImageAspectFlags aspects) {
    uint32_t mip        = range.i.mip;
    uint32_t arrayLayer = range.i.face;
    uint32_t levels     = range.e.numMipLevels;
    uint32_t layers     = range.e.numArrayLayers;
    rv::clampRange(mip, levels, mNumMips);
    rv::clampRange(arrayLayer, layers, mNumLayers);
    if (levels == 0 || layers == 0) return false;

    // Intersect with the texture's valid plane set up front. Any bit the format doesn't have
    // is dropped silently — and \c allFlags collapses cleanly to mValidAspects, so there's no
    // longer a special case for it.
    aspects &= mValidAspects;
    if (!aspects) return false;

    const uint32_t mipEnd        = mip + levels;
    const uint32_t arrayLayerEnd = arrayLayer + layers;
    bool           anyChange     = false;

    for (uint32_t i = mip; i < mipEnd; ++i) {
        for (uint32_t j = arrayLayer; j < arrayLayerEnd; ++j) {
            SubresourceState & sr = mSubresourceStates[subresourceIndex(i, j)];
            forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
                const ImageState * beforePtr = sr.get(bit);
                if (!beforePtr) GN_UNLIKELY return; // every bit in `aspects` is in mValidAspects, so this should never trip
                ImageState before = *beforePtr;     // snapshot before set() mutates in place
                if (!sr.set(bit, newState)) return; // value unchanged
                anyChange = true;
                if (!resourceName.empty()) {
                    GN_VERBOSE(sLogger)("texture '{}': mip={} face={} aspect=0x{:x}: layout {} -> {}, access 0x{:x} -> 0x{:x}, stages 0x{:x} -> 0x{:x}",
                                        resourceName.c_str(), i, j, static_cast<uint32_t>(bit), static_cast<uint32_t>(before.layout),
                                        static_cast<uint32_t>(newState.layout), static_cast<uint32_t>(before.access), static_cast<uint32_t>(newState.access),
                                        static_cast<uint32_t>(before.stages), static_cast<uint32_t>(newState.stages));
                }
            });
        }
    }
    return anyChange;
}

} // namespace GN::gpu2
