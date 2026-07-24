#include "pch.h"
#include "vk-gpu-resource-state-tracker.h"
#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.tracker");

namespace {
// Iterate each set bit in aspects, calling fn(vk::ImageAspectFlagBits).
template<typename Fn>
void forEachAspectBit(vk::ImageAspectFlags aspects, Fn && fn) {
    auto remaining = static_cast<vk::ImageAspectFlags::MaskType>(aspects);
    while (remaining) {
        auto lowBit = remaining & (~remaining + 1u);
        fn(static_cast<vk::ImageAspectFlagBits>(lowBit));
        remaining ^= lowBit;
    }
}
} // namespace

namespace GN::gpu2 {

namespace {

/// Pack a (mip, face, aspectBit) tuple into a single key for the \c registered map.
inline uint64_t packPlaneKey(uint32_t mip, uint32_t face, vk::ImageAspectFlagBits aspect) {
    return (uint64_t(mip) << 48) | (uint64_t(face) << 16) | uint64_t(uint32_t(aspect));
}

inline vk::ImageAspectFlags aspectFromView(const GpuResourceView::ImageView & view, const Texture::Descriptor & d) {
    const auto viewFormat = (view.format == gfx::img::PixelFormat::UNKNOWN()) ? d.format : view.format;
    return aspectFromViewFormat(viewFormat, d.format);
}

inline GpuResourceView::SubresourceRange resolveRange(GpuResourceView::SubresourceRange range, const Texture::Descriptor & d) {
    uint32_t nm = range.e.numMipLevels;
    uint32_t na = range.e.numArrayLayers;
    if (nm == (uint32_t) -1) nm = d.levels > range.i.mip ? d.levels - range.i.mip : 0;
    if (na == (uint32_t) -1) na = d.faces > range.i.face ? d.faces - range.i.face : 0;
    range.e.numMipLevels   = nm;
    range.e.numArrayLayers = na;
    return range;
}

} // namespace

bool GpuResourceStateTrackerVulkan::addTexture(TextureVulkanBase * tex, const GpuResourceView::ImageView & view, const rv::Image::State::PlaneState & state) {
    auto & tracked = mTextures[tex->id];
    if (!tracked.tex) {
        // First registration of this texture in the batch. Initialize incoming from the actual
        // resource state; emit barrier method keeps it up-to-date after each payload so subsequent
        // payloads in the same command buffer see the correct "from" layout.
        tracked.tex      = tex;
        tracked.incoming = tex->getState();
        GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image '{}' incoming initialized ({} mips, {} faces)", tex->name, tracked.incoming.numMips,
                            tracked.incoming.numLayers);
    }

    const auto & desc = tex->descriptor();

    // Aspect comes from the view's format (or the texture's format if the view doesn't override).
    // Intersect with validAspects() so any bit that isn't actually a plane on this texture's
    // format gets dropped silently — this also guarantees every remaining bit is present in
    // \c incoming, so subsequent \c incoming.get(...) calls always succeed.
    auto aspects = aspectFromView(view, desc) & tracked.incoming.validAspects;
    if (!aspects) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuResourceStateTrackerVulkan: view of texture '{}' references no aspect plane the texture has", tex->name);
            return false;
        }

    const auto     resolved = resolveRange(view.range, desc);
    const uint32_t mipEnd   = resolved.i.mip + resolved.e.numMipLevels;
    const uint32_t faceEnd  = resolved.i.face + resolved.e.numArrayLayers;

    // Hazard pass: check every (mip, face, plane) in this binding against the planes already
    // registered this pass. A plane is hazardous iff already registered AND at least one side is
    // a write. Pre-pass state (\c incoming) is intentionally ignored — a write left over from an
    // earlier pass isn't a hazard with a new use here.
    //
    // The lambda inside \c forEachAspectBit can't directly \c return from \c addTexture, so we
    // collect the verdict in \c hazardFound and bail out after the loops.
    bool hazardFound = false;
    for (uint32_t mip = resolved.i.mip; mip < mipEnd; ++mip) {
        for (uint32_t face = resolved.i.face; face < faceEnd; ++face) {
            forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
                uint64_t key = packPlaneKey(mip, face, bit);
                auto     it  = tracked.registered.find(key);
                if (it == tracked.registered.end()) return;
                const auto & existing = it->second;
                if (!existing.isWrite() && !state.isWrite()) return;

                const char * hazardKind = (existing.isWrite() && state.isWrite()) ? "write/write" : "read/write";
                GN_ERROR(sLogger)("GpuResourceStateTrackerVulkan: {} hazard on texture '{}' aspect=0x{:x} — '{}' ({}) and '{}' ({}) "
                                  "both access subresource [mip={} face={}]",
                                  hazardKind, tracked.tex->name, static_cast<uint32_t>(bit), existing.usage ? existing.usage : "?",
                                  existing.isWrite() ? "write" : "read", state.usage ? state.usage : "?", state.isWrite() ? "write" : "read", mip, face);
                hazardFound = true;
            });
        }
    }
    if (hazardFound) return false;

    // Lazy: only compute the representative incoming layout if verbose logging is actually active.
    [[maybe_unused]] auto firstIncomingLayout = [&]() -> vk::ImageLayout {
        vk::ImageLayout layout = vk::ImageLayout::eUndefined;
        forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
            if (layout != vk::ImageLayout::eUndefined) return;
            if (const auto * ps = tracked.incoming.get(resolved.i.mip, resolved.i.face, bit)) layout = ps->layout;
        });
        return layout;
    };
    GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: register image '{}' [mip={}-{} face={}-{}] as '{}' (incoming: {})", tex->name, resolved.i.mip,
                        mipEnd - 1, resolved.i.face, faceEnd - 1, state.usage ? state.usage : "?", vk::to_string(firstIncomingLayout()));

    // No hazard — record each (mip, face, plane) → intended state.
    for (uint32_t mip = resolved.i.mip; mip < mipEnd; ++mip) {
        for (uint32_t face = resolved.i.face; face < faceEnd; ++face) {
            forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
                tracked.registered[packPlaneKey(mip, face, bit)] = state;
                GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image '{}' [mip={} face={} {}] registered layout={}", tex->name, mip, face,
                                    vk::to_string(bit), vk::to_string(state.layout));
            });
        }
    }
    return true;
}

bool GpuResourceStateTrackerVulkan::addColorTarget(TextureVulkanBase * tex, const GpuResourceView & view) {
    if (!tex) GN_UNLIKELY return true;
    rv::Image::State::PlaneState state;
    state.layout = vk::ImageLayout::eColorAttachmentOptimal;
    state.access = vk::AccessFlagBits::eColorAttachmentWrite;
    state.stages = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    state.usage  = "color target";
    return addTexture(tex, view.imageView, state);
}

bool GpuResourceStateTrackerVulkan::addDepthStencilTarget(TextureVulkanBase * tex, const GpuResourceView & view, bool readOnly) {
    if (!tex) GN_UNLIKELY return true;
    rv::Image::State::PlaneState state;
    state.layout = readOnly ? vk::ImageLayout::eDepthStencilReadOnlyOptimal : vk::ImageLayout::eDepthStencilAttachmentOptimal;
    state.access = vk::AccessFlagBits::eDepthStencilAttachmentRead;
    if (!readOnly) state.access |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    state.stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
    state.usage  = readOnly ? "depth-stencil target (read-only)" : "depth-stencil target";
    return addTexture(tex, view.imageView, state);
}

bool GpuResourceStateTrackerVulkan::addSampledTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages) {
    if (!tex) GN_UNLIKELY return true;
    rv::Image::State::PlaneState state;
    state.layout = vk::ImageLayout::eShaderReadOnlyOptimal;
    state.access = vk::AccessFlagBits::eShaderRead;
    state.stages = stages;
    state.usage  = "sampled texture";
    return addTexture(tex, view.imageView, state);
}

bool GpuResourceStateTrackerVulkan::addStorageTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages) {
    if (!tex) GN_UNLIKELY return true;
    rv::Image::State::PlaneState state;
    state.layout = vk::ImageLayout::eGeneral;
    state.access = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite;
    state.stages = stages;
    state.usage  = "storage texture";
    return addTexture(tex, view.imageView, state);
}

bool GpuResourceStateTrackerVulkan::checkBufferHazard(const TrackedBuffer & incoming) const {
    auto it = mBuffers.find(incoming.buf->id);
    if (it == mBuffers.end()) return true;
    const TrackedBuffer & existing = it->second;
    if (!existing.isWrite && !incoming.isWrite) return true; // read+read is always safe
    const char * hazardKind = (existing.isWrite && incoming.isWrite) ? "write/write" : "read/write";
    GN_ERROR(sLogger)("GpuResourceStateTrackerVulkan: {} hazard on buffer '{}' — '{}' ({}) and '{}' ({})", hazardKind, incoming.buf->name, existing.usageName,
                      existing.isWrite ? "write" : "read", incoming.usageName, incoming.isWrite ? "write" : "read");
    return false;
}

bool GpuResourceStateTrackerVulkan::addBuffer(TrackedBuffer b) {
    auto it = mBuffers.find(b.buf->id);
    if (it == mBuffers.end()) {
        // First registration in this batch: snapshot the actual resource state as the baseline
        // so emitPrePassBarriers() computes the correct "from" side of the first barrier.
        b.committedAccess = b.buf->gpuState.access;
        b.committedStages = b.buf->gpuState.stages;
        b.activeThisPass  = true;
        GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: register buffer '{}' as '{}' committed={} pass={}", b.buf->name, b.usageName,
                            vk::to_string(b.committedAccess), vk::to_string(b.passAccess));
        mBuffers.emplace(b.buf->id, std::move(b));
        return true;
    }
    auto & existing = it->second;
    // Only flag hazards for same-pass re-registrations; cross-payload re-use is expected and handled
    // by emitPrePassBarriers() via committedAccess.
    if (existing.activeThisPass && !checkBufferHazard(b)) return false;
    if (!existing.activeThisPass) {
        // Cross-payload re-registration: log the committed state this payload inherits.
        GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: re-register buffer '{}' as '{}' committed={} pass={}", b.buf->name, b.usageName,
                            vk::to_string(existing.committedAccess), vk::to_string(b.passAccess));
    }
    existing.passAccess |= b.passAccess;
    existing.passStages |= b.passStages;
    existing.isWrite |= b.isWrite;
    existing.activeThisPass = true;
    GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: buffer '{}' pass access merged to {}", existing.buf->name, vk::to_string(existing.passAccess));
    return true;
}

bool GpuResourceStateTrackerVulkan::addUniformBuffer(BufferVulkan * buf, vk::PipelineStageFlags stages) {
    if (!buf) GN_UNLIKELY return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eUniformRead;
    b.passStages = stages;
    b.isWrite    = false;
    b.usageName  = "uniform buffer";
    return addBuffer(std::move(b));
}

bool GpuResourceStateTrackerVulkan::addStorageBuffer(BufferVulkan * buf, bool write, vk::PipelineStageFlags stages) {
    if (!buf) GN_UNLIKELY return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eShaderRead | (write ? vk::AccessFlagBits::eShaderWrite : vk::AccessFlags {});
    b.passStages = stages;
    b.isWrite    = write;
    b.usageName  = write ? "storage buffer (read-write)" : "storage buffer (read-only)";
    return addBuffer(std::move(b));
}

bool GpuResourceStateTrackerVulkan::addVertexBuffer(BufferVulkan * buf) {
    if (!buf) GN_UNLIKELY return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eVertexAttributeRead;
    b.passStages = vk::PipelineStageFlagBits::eVertexInput;
    b.isWrite    = false;
    b.usageName  = "vertex buffer";
    return addBuffer(std::move(b));
}

bool GpuResourceStateTrackerVulkan::addIndexBuffer(BufferVulkan * buf) {
    if (!buf) GN_UNLIKELY return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eIndexRead;
    b.passStages = vk::PipelineStageFlagBits::eVertexInput;
    b.isWrite    = false;
    b.usageName  = "index buffer";
    return addBuffer(std::move(b));
}

bool GpuResourceStateTrackerVulkan::addTransferSrcBuffer(BufferVulkan * buf) {
    if (!buf) GN_UNLIKELY return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eTransferRead;
    b.passStages = vk::PipelineStageFlagBits::eTransfer;
    b.isWrite    = false;
    b.usageName  = "transfer source buffer";
    return addBuffer(std::move(b));
}

bool GpuResourceStateTrackerVulkan::addTransferDstBuffer(BufferVulkan * buf) {
    if (!buf) GN_UNLIKELY return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eTransferWrite;
    b.passStages = vk::PipelineStageFlagBits::eTransfer;
    b.isWrite    = true;
    b.usageName  = "transfer destination buffer";
    return addBuffer(std::move(b));
}

bool GpuResourceStateTrackerVulkan::addTransferDstImage(TextureVulkanBase * tex, const GpuResourceView::ImageView & view) {
    if (!tex) GN_UNLIKELY return true;
    rv::Image::State::PlaneState state;
    state.layout = vk::ImageLayout::eTransferDstOptimal;
    state.access = vk::AccessFlagBits::eTransferWrite;
    state.stages = vk::PipelineStageFlagBits::eTransfer;
    state.usage  = "transfer destination image";
    return addTexture(tex, view, state);
}

bool GpuResourceStateTrackerVulkan::addTransferSrcImage(TextureVulkanBase * tex, const GpuResourceView::ImageView & view) {
    if (!tex) GN_UNLIKELY return true;
    rv::Image::State::PlaneState state;
    state.layout = vk::ImageLayout::eTransferSrcOptimal;
    state.access = vk::AccessFlagBits::eTransferRead;
    state.stages = vk::PipelineStageFlagBits::eTransfer;
    state.usage  = "transfer source image";
    return addTexture(tex, view, state);
}

std::vector<uint64_t> GpuResourceStateTrackerVulkan::addGpuResourceTable(const GpuResourceTable & table) {
    std::vector<uint64_t> invalid;
    for (size_t setIdx = 0; setIdx < table.size(); ++setIdx) {
        const auto & set = table[setIdx];
        for (size_t bindingIdx = 0; bindingIdx < set.size(); ++bindingIdx) {
            const auto & slot = set[bindingIdx];
            for (size_t arrIdx = 0; arrIdx < slot.size(); ++arrIdx) {
                const GpuResourceView & view = slot[arrIdx];
                if (view.empty()) continue;
                if (view.isTexture()) {
                    auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                    if (!tex) continue;
                    bool ok = (view.imageView.type == GpuResourceView::ImageView::STORAGE) ? addStorageTexture(tex, view) : addSampledTexture(tex, view);
                    if (!ok) invalid.push_back(tex->id);
                } else if (view.isBuffer()) {
                    auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                    if (!buf) continue;
                    bool ok = (view.bufferView.type == GpuResourceView::BufferView::STORAGE) ? addStorageBuffer(buf, false) : addUniformBuffer(buf);
                    if (!ok) invalid.push_back(buf->id);
                }
                // samplers carry no Vulkan resource state; skip
            }
        }
    }
    return invalid;
}

bool GpuResourceStateTrackerVulkan::addRasterGeometry(const RasterGeometry & geom) {
    bool ok = true;
    for (const auto & vb : geom.vertices) {
        if (!vb.buffer) continue;
        if (auto * buf = RuntimeType::cast<BufferVulkan>(vb.buffer.get()))
            if (!addVertexBuffer(buf)) ok = false;
    }
    for (const auto & ib : geom.instances) {
        if (!ib.buffer) continue;
        if (auto * buf = RuntimeType::cast<BufferVulkan>(ib.buffer.get()))
            if (!addVertexBuffer(buf)) ok = false;
    }
    if (geom.indices.buffer) {
        if (auto * buf = RuntimeType::cast<BufferVulkan>(geom.indices.buffer.get()))
            if (!addIndexBuffer(buf)) ok = false;
    }
    return ok;
}

bool GpuResourceStateTrackerVulkan::addRasterTarget(const RasterTarget & rt) {
    bool ok = true;
    for (size_t i = 0; i < rt.colorTargets.size(); ++i) {
        const auto &          ct   = rt.colorTargets[i];
        const GpuResourceView view = ct.view();
        if (!view.texture()) continue;
        auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
        if (tex && !addColorTarget(tex, view)) {
            GN_ERROR(sLogger)("GpuResourceStateTrackerVulkan: render target hazard on color target slot {}; aborting render pass", i);
            ok = false;
        }
    }
    const GpuResourceView depthStencilView = rt.depthStencilTarget.view();
    if (depthStencilView.isTexture() && depthStencilView.texture()) {
        auto * tex = RuntimeType::cast<TextureVulkanBase>(depthStencilView.texture().get());
        if (tex) {
            const auto & ds = rt.states;
            bool         ro = ds.depthState && !ds.depthState->writeEnabled() && !(ds.stencilState && ds.stencilState->enabled());
            if (!addDepthStencilTarget(tex, depthStencilView, ro)) {
                GN_ERROR(sLogger)("GpuResourceStateTrackerVulkan: render target hazard on depth-stencil target; aborting render pass");
                ok = false;
            }
        }
    }
    return ok;
}

void GpuResourceStateTrackerVulkan::upgradeForDrawRasterState(const RasterState & drawState) {
    bool needsDepthWrite   = drawState.depthState && drawState.depthState->writeEnabled();
    bool needsStencilWrite = drawState.stencilState && drawState.stencilState->enabled();
    if (!needsDepthWrite && !needsStencilWrite) return;

    rv::Image::State::PlaneState promoted;
    promoted.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
    promoted.access = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    promoted.stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
    promoted.usage  = "depth-stencil target (promoted to read-write)";

    for (auto & [id, tracked] : mTextures) {
        // Walk registered planes in place; any plane currently in DepthStencilReadOnlyOptimal gets
        // bumped up to read-write. Iterating the map is safe because we only mutate values, never
        // insert or erase.
        for (auto & [key, intended] : tracked.registered) {
            if (intended.layout != vk::ImageLayout::eDepthStencilReadOnlyOptimal) continue;
            uint32_t mip  = uint32_t((key >> 48) & 0xffffu);
            uint32_t face = uint32_t((key >> 16) & 0xffffu);
            GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: promoting depth-stencil '{}' mip={} face={} from read-only to read-write "
                                "(draw requires {})",
                                tracked.tex->name, mip, face, needsDepthWrite ? "depth write" : "stencil write");
            intended = promoted;
        }
    }
}

namespace {
// Given per-plane layouts of a combined depth-stencil subresource, return the single canonical
// Vulkan layout that covers both aspects — required because VkRenderingAttachmentInfo takes one
// layout for the whole subresource.
static vk::ImageLayout combineDepthStencilLayouts(vk::ImageLayout depth, vk::ImageLayout stencil) {
    if (depth == stencil) return depth;
    // Classify writable vs. read-only for each plane and pick the Vulkan split layout.
    const bool depthRW   = depth == vk::ImageLayout::eDepthStencilAttachmentOptimal || depth == vk::ImageLayout::eDepthAttachmentOptimal ||
                           depth == vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal || depth == vk::ImageLayout::eGeneral;
    const bool stencilRW = stencil == vk::ImageLayout::eDepthStencilAttachmentOptimal || stencil == vk::ImageLayout::eStencilAttachmentOptimal ||
                           stencil == vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal || stencil == vk::ImageLayout::eGeneral;
    if (depthRW && stencilRW) return vk::ImageLayout::eDepthStencilAttachmentOptimal;
    if (!depthRW && !stencilRW) return vk::ImageLayout::eDepthStencilReadOnlyOptimal;
    if (!depthRW && stencilRW) return vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal;
    return vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal;
}
} // namespace

vk::ImageLayout GpuResourceStateTrackerVulkan::texturePassLayout(const TextureVulkanBase * tex, uint32_t mip, uint32_t face) const {
    auto it = mTextures.find(tex->id);
    if (it == mTextures.end()) {
        GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: texturePassLayout('{}' mip={} face={}) -> eUndefined (not tracked)", tex->name, mip, face);
        return vk::ImageLayout::eUndefined;
    }
    const auto & tracked = it->second;

    // Query incoming (post-barrier authoritative state) — registered is cleared by emitPrePassBarriers
    // before this function is ever called, so querying registered would always return eUndefined.
    vk::ImageLayout colorLayout   = vk::ImageLayout::eUndefined;
    vk::ImageLayout depthLayout   = vk::ImageLayout::eUndefined;
    vk::ImageLayout stencilLayout = vk::ImageLayout::eUndefined;
    forEachAspectBit(tracked.incoming.validAspects, [&](vk::ImageAspectFlagBits bit) {
        const auto * ps = tracked.incoming.get(mip, face, bit);
        if (!ps) return;
        if (bit == vk::ImageAspectFlagBits::eColor)
            colorLayout = ps->layout;
        else if (bit == vk::ImageAspectFlagBits::eDepth)
            depthLayout = ps->layout;
        else if (bit == vk::ImageAspectFlagBits::eStencil)
            stencilLayout = ps->layout;
    });

    vk::ImageLayout result;
    const char *    reason;
    if (colorLayout != vk::ImageLayout::eUndefined) {
        result = colorLayout;
        reason = "color";
    } else if (depthLayout != vk::ImageLayout::eUndefined && stencilLayout != vk::ImageLayout::eUndefined) {
        result = combineDepthStencilLayouts(depthLayout, stencilLayout);
        reason = "depth+stencil combined";
    } else if (depthLayout != vk::ImageLayout::eUndefined) {
        result = depthLayout;
        reason = "depth only";
    } else if (stencilLayout != vk::ImageLayout::eUndefined) {
        result = stencilLayout;
        reason = "stencil only";
    } else {
        result = vk::ImageLayout::eUndefined;
        reason = "no plane at this subresource";
    }

    GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: texturePassLayout('{}' mip={} face={}) -> {} ({})", tex->name, mip, face, vk::to_string(result),
                        reason);
    return result;
}

void GpuResourceStateTrackerVulkan::emitPrePassBarriers(vk::CommandBuffer cb) {
    DynaArray<vk::BufferMemoryBarrier> bufferBarriers;
    DynaArray<vk::ImageMemoryBarrier>  barriers;
    vk::PipelineStageFlags             srcStages = {};
    vk::PipelineStageFlags             dstStages = {};

    for (auto & [id, b] : mBuffers) {
        if (!b.activeThisPass) continue; // not registered this pass; skip

        if (b.committedAccess != b.passAccess || b.committedStages != b.passStages) {
            vk::Buffer vkBuf = b.buf->nativeBuffer();
            if (!vkBuf) GN_UNLIKELY {
                    GN_WARN(sLogger)("GpuResourceStateTrackerVulkan: buffer '{}' has no VkBuffer handle; skipping barrier", b.buf->name);
                }
            else {
                GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: buffer barrier '{}' [{}] : access {} -> {}", b.buf->name, b.usageName,
                                    vk::to_string(b.committedAccess), vk::to_string(b.passAccess));
                vk::BufferMemoryBarrier barrier;
                barrier.setSrcAccessMask(b.committedAccess)
                    .setDstAccessMask(b.passAccess)
                    .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                    .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                    .setBuffer(vkBuf)
                    .setOffset(0)
                    .setSize(VK_WHOLE_SIZE);
                bufferBarriers.append(barrier);
                srcStages |= b.committedStages;
                dstStages |= b.passStages;
                b.committedAccess = b.passAccess;
                b.committedStages = b.passStages;
                GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: buffer '{}' committed updated to {}", b.buf->name, vk::to_string(b.committedAccess));
            }
        }

        // Commit: reset per-pass fields regardless of whether a barrier was emitted.
        b.passAccess     = {};
        b.passStages     = vk::PipelineStageFlagBits::eBottomOfPipe;
        b.isWrite        = false;
        b.activeThisPass = false;
        GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: buffer '{}' pass state reset", b.buf->name);
    }

    for (auto & [id, tracked] : mTextures) {
        vk::Image vkImg = tracked.tex->nativeImage();
        if (!vkImg) GN_UNLIKELY {
                GN_WARN(sLogger)("GpuResourceStateTrackerVulkan: texture '{}' has no VkImage handle; skipping barrier", tracked.tex->name);
                continue;
            }

        // Iterate only planes the pass registered; untouched planes need no barrier.
        for (const auto & [key, next] : tracked.registered) {
            uint32_t                mip    = uint32_t((key >> 48) & 0xffffu);
            uint32_t                face   = uint32_t((key >> 16) & 0xffffu);
            vk::ImageAspectFlagBits aspect = static_cast<vk::ImageAspectFlagBits>(uint32_t(key & 0xffffu));

            const auto * prev = tracked.incoming.get(mip, face, aspect);
            if (!prev) GN_UNLIKELY continue; // incoming should always have the plane (snapshot from gpuStates)
            if (*prev == next) continue;

            GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image barrier '{}' [mip={} face={} {}] : {} -> {} ({})", tracked.tex->name, mip, face,
                                vk::to_string(aspect), vk::to_string(prev->layout), vk::to_string(next.layout), next.usage ? next.usage : "?");
            vk::ImageMemoryBarrier b;
            b.setOldLayout(prev->layout)
                .setNewLayout(next.layout)
                .setImage(vkImg)
                .setSubresourceRange({aspect, mip, 1, face, 1})
                .setSrcAccessMask(prev->access)
                .setDstAccessMask(next.access)
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
            barriers.append(b);
            srcStages |= prev->stages;
            dstStages |= next.stages;

            // Advance the running incoming state so the next payload's add*() calls see the
            // correct post-barrier layout without any extra bookkeeping.
            if (mip < tracked.incoming.numMips && face < tracked.incoming.numLayers) {
                auto & sr = tracked.incoming.subresources[tracked.incoming.subresourceIndex(mip, face)];
                auto   it = sr.planes.find(aspect);
                if (it != sr.planes.end()) {
                    it->second = next;
                    GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image '{}' [mip={} face={} {}] incoming updated to {}", tracked.tex->name, mip, face,
                                        vk::to_string(aspect), vk::to_string(next.layout));
                }
            }
        }

        // Registered states are baked into barriers; clear so the next payload starts fresh.
        GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image '{}' registered cleared ({} planes)", tracked.tex->name, tracked.registered.size());
        tracked.registered.clear();
    }

    if (bufferBarriers.empty() && barriers.empty()) return;

    if (!srcStages) srcStages = vk::PipelineStageFlagBits::eTopOfPipe;
    if (!dstStages) dstStages = vk::PipelineStageFlagBits::eBottomOfPipe;

    cb.pipelineBarrier(srcStages, dstStages, {}, nullptr,
                       vk::ArrayProxy<const vk::BufferMemoryBarrier>((uint32_t) bufferBarriers.size(), bufferBarriers.data()),
                       vk::ArrayProxy<const vk::ImageMemoryBarrier>((uint32_t) barriers.size(), barriers.data()));
}

void GpuResourceStateTrackerVulkan::flushToResources() {
    // Write the batch-final image layout for every tracked subresource back to each texture so
    // the next submit's barriers use the correct "from" layout.
    for (const auto & [id, tracked] : mTextures) {
        if (!tracked.tex) continue;
        for (uint32_t mip = 0; mip < tracked.incoming.numMips; ++mip) {
            for (uint32_t face = 0; face < tracked.incoming.numLayers; ++face) {
                forEachAspectBit(tracked.incoming.validAspects, [t = tracked, m = mip, f = face](vk::ImageAspectFlagBits aspect) {
                    const auto * ps = t.incoming.get(m, f, aspect);
                    if (!ps) return;
                    t.tex->setState(*ps, vk::ImageSubresourceRange(aspect, m, 1, f, 1));
                });
            }
        }
    }
    // Write the batch-final buffer access/stage back to each buffer.
    for (const auto & [id, b] : mBuffers) {
        if (!b.buf) continue;
        b.buf->gpuState.access = b.committedAccess;
        b.buf->gpuState.stages = b.committedStages;
    }
}

} // namespace GN::gpu2
