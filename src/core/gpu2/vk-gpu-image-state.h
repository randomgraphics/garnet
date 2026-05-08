#pragma once

#include "vk-gpu-context.h"
#include <bit>

namespace GN::gpu2 {

/// Compile-time string literal wrapper. Safety comes from the template parameter type: const char*
/// (a pointer) cannot bind to const char (&)[N] (an array reference), so runtime strings silently
/// fail to compile. No explicit deletions are needed — and are intentionally omitted because
/// deleted non-template overloads outrank the template in overload resolution, breaking literals.
struct StringLiteral {
    const char * data = nullptr; // non-const pointer: must stay assignable for ImageState copy
    size_t       size = 0;       // non-const: must stay assignable for ImageState copy

    constexpr StringLiteral() noexcept = default;

    template<size_t N>
    constexpr StringLiteral(const char (&str)[N]) noexcept: data(str), size(N - 1) {}

    // Explicit copy/assign — prevents linters from const-qualifying the member fields.
    constexpr StringLiteral(const StringLiteral & o) noexcept: data(o.data), size(o.size) {}
    constexpr StringLiteral & operator=(const StringLiteral & o) noexcept {
        data = o.data;
        size = o.size;
        return *this;
    }

    constexpr operator const char *() const noexcept { return data; }
};

}; // namespace GN::gpu2

/// string formatting for StringLiteral, for use in log messages. Not a general-purpose formatter.
template<>
struct fmt::formatter<GN::gpu2::StringLiteral> {
    constexpr auto parse(format_parse_context & ctx) { return ctx.begin(); }
    template<typename Context>
    constexpr auto format(GN::gpu2::StringLiteral const & lit, Context & ctx) const {
        return format_to(ctx.out(), "{}", lit.data ? lit.data : "<null>");
    }
};

namespace GN::gpu2 {

/// Invoke \p fn once per set bit in \p aspects, in low-to-high bit order. \p fn receives a
/// single \c vk::ImageAspectFlagBits. The `m & -m` idiom (written as `m & (~m + 1)` for unsigned
/// safety) isolates the lowest set bit each iteration, so any aspect bit Vulkan defines now or
/// later is picked up without a hard-coded list. Local to this TU for now — promote to a header
/// when a second caller appears.
template<typename Fn>
void forEachAspectBit(vk::ImageAspectFlags aspects, Fn && fn) {
    auto remaining = static_cast<vk::ImageAspectFlags::MaskType>(aspects);
    while (remaining) {
        auto lowBit = remaining & (~remaining + 1u);
        fn(static_cast<vk::ImageAspectFlagBits>(lowBit));
        remaining ^= lowBit;
    }
}

/// Tracks the latest known Vulkan image state per (mip x array layer x aspect plane).
///
/// Each subresource holds a hash map keyed by single-bit aspect flag, so depth-only and
/// stencil-only views of the same image — and YCbCr ePlane0/1/2 of multi-planar formats —
/// can coexist in independent states.
///
/// Convention: \c reset(format, ...) populates every plane that the format actually has
/// (e.g. {eColor} for RGBA, {eDepth, eStencil} for D24S8, {ePlane0, ePlane1} for NV12) with
/// the supplied initial state — defaults to UNDEFINED. After reset, the set of map keys
/// describes the texture's valid planes; the value is the per-plane state, which may be
/// UNDEFINED. \c set() updates only planes that already exist; a plane absent from the
/// subresource's \c planes map is one that does not exist on this texture, period.
struct TextureGpuImageState {

    /// State of a single plane of a single subresource.
    struct ImageState {
        vk::ImageLayout        layout = vk::ImageLayout::eUndefined;
        vk::AccessFlags        access = vk::AccessFlagBits::eNone;
        vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eBottomOfPipe;
        StringLiteral          usage  = StringLiteral("<unspecified>"); ///< for hazard error messages

        bool operator==(const ImageState & o) const { return layout == o.layout && access == o.access && stages == o.stages; }
        bool operator!=(const ImageState & o) const { return !(*this == o); }

        /// Returns true if the access flags include any write operation.
        bool isWrite() const {
            // clang-format off
            return (access & (vk::AccessFlagBits::eShaderWrite |
                              vk::AccessFlagBits::eColorAttachmentWrite |
                              vk::AccessFlagBits::eDepthStencilAttachmentWrite |
                              vk::AccessFlagBits::eTransferWrite |
                              vk::AccessFlagBits::eHostWrite |
                              vk::AccessFlagBits::eMemoryWrite)) != vk::AccessFlags{};
            // clang-format on
        }

        static constexpr ImageState UNDEFINED() { return {vk::ImageLayout::eUndefined, vk::AccessFlagBits::eNone, vk::PipelineStageFlagBits::eBottomOfPipe}; }

        static constexpr ImageState TRANSFER_SRC() {
            return {vk::ImageLayout::eTransferSrcOptimal, vk::AccessFlagBits::eTransferRead, vk::PipelineStageFlagBits::eTransfer};
        }

        static constexpr ImageState TRANSFER_DST() {
            return {vk::ImageLayout::eTransferDstOptimal, vk::AccessFlagBits::eTransferWrite, vk::PipelineStageFlagBits::eTransfer};
        }
    };

    struct AspectHash {
        size_t operator()(vk::ImageAspectFlagBits aspect) const {
            return std::hash<std::underlying_type_t<vk::ImageAspectFlagBits>> {}(static_cast<std::underlying_type_t<vk::ImageAspectFlagBits>>(aspect));
        }
    };

    /// State of a single subresource. The set of planes is fixed at \c reset() time from the
    /// texture's format and never grows or shrinks afterward — callers can only read or update
    /// the state of an existing plane, never insert new keys. \c planes_ is private to enforce
    /// this; reach in via \c get() / \c set() / \c forEach().
    struct SubresourceState {
        /// Returns the per-plane state for \p aspect, or nullptr if the texture has no such
        /// plane (i.e. format-determined; mismatch with the texture's actual planes).
        const ImageState * get(vk::ImageAspectFlagBits aspect) const {
            auto it = planes_.find(aspect);
            if (it == planes_.end()) GN_UNLIKELY return nullptr;
            return &it->second;
        }

        /// Update an existing plane. Returns true if the plane existed AND its value changed.
        /// Returns false (and is a no-op) if the plane does not exist on this texture, or if
        /// \p newState equals the current value. By design this cannot insert a new plane —
        /// the plane set is fixed by \c TextureGpuImageState::reset().
        bool set(vk::ImageAspectFlagBits aspect, const ImageState & newState) {
            auto it = planes_.find(aspect);
            if (it == planes_.end()) GN_UNLIKELY return false;
            if (it->second == newState) return false;
            it->second = newState;
            return true;
        }

        /// Number of aspect planes on this subresource (1 for color, 2 for D24S8, 1..3 for YCbCr...).
        size_t numPlanes() const { return planes_.size(); }

        /// Iterate every plane. \p fn is invoked as `fn(vk::ImageAspectFlagBits, const ImageState &)`.
        template<typename Fn>
        void forEach(Fn && fn) const {
            for (const auto & [k, v] : planes_) fn(k, v);
        }

    private:
        friend struct TextureGpuImageState; ///< only the outer class is allowed to populate the plane set, in reset()
        std::unordered_map<vk::ImageAspectFlagBits, ImageState, AspectHash> planes_;
    };

    TextureGpuImageState() = default;

    /// Reallocates storage for texture of \p format with \p mipLevels x \p arrayLayers subresources.
    /// The \p format is used to determine number and type of aspect planes the texture has.
    /// Clear the state to empty when \p format is undefined, or \p mipLevels or \p arrayLayers is zero.
    void reset(vk::Format format, uint32_t mipLevels, uint32_t arrayLayers, const ImageState & initialState = ImageState::UNDEFINED());

    // Convenience method to clear the state to empty.
    void clear() { reset(vk::Format::eUndefined, 0, 0); }

    /// Returns the \c SubresourceState for (\p mip, \p arrayLayer), whose \c planes map describes
    /// every aspect plane the texture has at that subresource and the per-plane state. Returns
    /// nullptr when the indices are out of range.
    const SubresourceState * get(uint32_t mip, uint32_t arrayLayer) const {
        if (mip >= mNumMips || arrayLayer >= mNumLayers) GN_UNLIKELY return nullptr;
        return &mSubresourceStates[subresourceIndex(mip, arrayLayer)];
    }

    /// Convenience overload: returns the per-plane state for (\p mip, \p arrayLayer, \p aspect).
    /// \p aspect must be a single aspect bit (e.g. eColor, eDepth, eStencil, ePlane0). Returns
    /// nullptr when the indices are out of range or when the texture's format has no such plane.
    /// The returned state may carry \c layout == \c eUndefined — that means "this plane exists
    /// but no state has been recorded yet"; callers that need to distinguish that case from a
    /// recorded transition must inspect \c s->layout themselves.
    const ImageState * get(uint32_t mip, uint32_t arrayLayer, vk::ImageAspectFlagBits aspect) const {
        const SubresourceState * sr = get(mip, arrayLayer);
        if (!sr) GN_UNLIKELY return nullptr;
        return sr->get(aspect);
    }

    /// Set state for the subresources in \p range. The default \c {} range covers the whole
    /// resource (every mip × array layer); pass an explicit range to scope to a subset. The
    /// \p aspects mask is intersected with the texture's valid plane set up front, so bits
    /// the format does not have are stripped silently — and the default \c allFlags collapses
    /// to "every plane the texture has." The \p resourceName is for logging only. Returns true
    /// if any slot changed.
    ///
    /// Use \c set(name, state) (defaults on the rest) for "transition the whole image to a
    /// single state" — safer than \c reset() because it only writes values and never reshapes
    /// the storage. \c reset() requires passing back the same format/dimensions, and getting
    /// either wrong silently rewrites the texture's plane structure.
    bool set(const StrA & resourceName, const ImageState & newState, const GpuResourceView::SubresourceRange & range = {},
             vk::ImageAspectFlags aspects = vk::FlagTraits<vk::ImageAspectFlagBits>::allFlags);

    /// The set of aspect planes this texture has, derived from the format passed to \c reset().
    /// Empty before any \c reset() has populated the storage.
    vk::ImageAspectFlags validAspects() const { return mValidAspects; }

    uint32_t mipLevels() const { return mNumMips; }
    uint32_t arrayLayers() const { return mNumLayers; }

    /// Iterate every set entry. \p fn is invoked as `fn(uint32_t mip, uint32_t face, vk::ImageAspectFlagBits aspect, const ImageState &)`.
    template<typename Fn>
    void forEach(Fn && fn) const {
        for (uint32_t m = 0; m < mNumMips; ++m) {
            for (uint32_t f = 0; f < mNumLayers; ++f) {
                const auto & subresource = mSubresourceStates[subresourceIndex(m, f)];
                subresource.forEach([&](vk::ImageAspectFlagBits k, const ImageState & v) { fn(m, f, k, v); });
            }
        }
    }

private:
    size_t subresourceIndex(uint32_t mip, uint32_t arrayLayer) const { return mip * mNumLayers + arrayLayer; }

    vk::Format                  mFormat       = vk::Format::eUndefined;
    vk::ImageAspectFlags        mValidAspects = {}; ///< planes the format has; cached at reset()
    uint32_t                    mNumMips      = 0;
    uint32_t                    mNumLayers    = 0;
    DynaArray<SubresourceState> mSubresourceStates; ///< size = mNumMips * mNumLayers
};

} // namespace GN::gpu2
