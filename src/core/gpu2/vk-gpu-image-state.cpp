#include "vk-gpu-image-state.h"
#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu2.vk");

namespace GN::gfx::gpu2 {

void TextureGpuImageState::reset(uint32_t mipLevels, uint32_t arrayLayers, const ImageState & initialState) {
    if (mipLevels == 0 || arrayLayers == 0) {
        mNumMips   = 0;
        mNumLayers = 0;
        mSubresourceStates.clear();
        return;
    }
    mNumMips   = mipLevels;
    mNumLayers = arrayLayers;
    mSubresourceStates.resize((size_t) mipLevels * (size_t) arrayLayers);
    for (auto & s : mSubresourceStates) s = initialState;
}

const TextureGpuImageState::ImageState * TextureGpuImageState::get(uint32_t mip, uint32_t arrayLayer) const {
    if (mip >= mNumMips || arrayLayer >= mNumLayers) return nullptr;
    return &mSubresourceStates[subResourceIndex(mip, arrayLayer)];
}

bool TextureGpuImageState::set(const GpuResourceView::SubresourceRange & range, const ImageState & newState, const StrA & resourceName) {
    uint32_t mip        = range.i.mip;
    uint32_t arrayLayer = range.i.face;
    uint32_t levels     = range.e.numMipLevels;
    uint32_t layers     = range.e.numArrayLayers;
    rv::clampRange(mip, levels, mNumMips);
    rv::clampRange(arrayLayer, layers, mNumLayers);
    if (levels == 0 || layers == 0) return false;
    const uint32_t mipEnd        = mip + levels;
    const uint32_t arrayLayerEnd = arrayLayer + layers;
    bool           anyChange     = false;
    for (uint32_t i = mip; i < mipEnd; ++i) {
        for (uint32_t j = arrayLayer; j < arrayLayerEnd; ++j) {
            ImageState & s = mSubresourceStates[subResourceIndex(i, j)];
            if (s != newState) {
                if (!resourceName.empty()) {
                    GN_VERBOSE(sLogger)("texture '{}': mip={} face={}: layout {} -> {}, access 0x{:x} -> 0x{:x}, stages 0x{:x} -> 0x{:x}", resourceName.c_str(),
                                        i, j, static_cast<uint32_t>(s.layout), static_cast<uint32_t>(newState.layout), static_cast<uint32_t>(s.access),
                                        static_cast<uint32_t>(newState.access), static_cast<uint32_t>(s.stages), static_cast<uint32_t>(newState.stages));
                }
                s         = newState;
                anyChange = true;
            }
        }
    }
    return anyChange;
}

} // namespace GN::gfx::gpu2
