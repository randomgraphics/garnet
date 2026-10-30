#include "pch.h"
#include "vk-gpu-resource-state-tracker.h"
#include "vk-gpu-context.h"
#include "vk-format-utils.h"
#include <unordered_set>

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

bool GpuResourceStateTrackerVulkan::addTexture(TextureVulkanBase * tex, const GpuResourceView::ImageView & view, const TexturePlaneStateVulkan & state) {
    auto & tracked = mTextures[tex->id];
    if (!tracked.tex) {
        tracked.tex          = tex;
        const auto & desc    = tex->descriptor();
        tracked.numMips      = desc.levels;
        tracked.numLayers    = desc.faces;
        tracked.validAspects = aspectFromViewFormat(desc.format, desc.format);
        if (!tracked.validAspects) tracked.validAspects = vk::ImageAspectFlagBits::eColor;
        GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: image '{}' tracked initialized ({} mips, {} faces)", tex->name, tracked.numMips, tracked.numLayers);
    }

    const auto & desc = tex->descriptor();

    // Aspect comes from the view's format (or the texture's format if the view doesn't override).
    // Intersect with validAspects so any bit that isn't actually a plane on this texture's
    // format gets dropped silently.
    auto aspects = aspectFromView(view, desc) & tracked.validAspects;
    if (!aspects) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuResourceStateTrackerVulkan: view of texture '{}' references no aspect plane the texture has", tex->name);
            return false;
        }

    const auto     resolved = resolveRange(view.range, desc);
    const uint32_t mipEnd   = resolved.i.mip + resolved.e.numMipLevels;
    const uint32_t faceEnd  = resolved.i.face + resolved.e.numArrayLayers;

    // Ensure incoming baseline exists for all referenced planes
    for (uint32_t mip = resolved.i.mip; mip < mipEnd; ++mip) {
        for (uint32_t face = resolved.i.face; face < faceEnd; ++face) {
            forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
                if (!tracked.getIncoming(mip, face, bit)) {
                    if (tex->isBackbuffer() || state.isWrite()) {
                        tracked.setIncoming(mip, face, bit, TexturePlaneStateVulkan::UNDEFINED());
                    } else {
                        auto ready   = TexturePlaneStateVulkan::SHADER_READ_ONLY();
                        ready.layout = shaderReadOnlyLayout(desc.format);
                        tracked.setIncoming(mip, face, bit, ready);
                    }
                }
            });
        }
    }

    // Hazard pass: check every (mip, face, plane) in this binding against the planes already
    // registered this pass. A plane is hazardous iff already registered AND at least one side is
    // a write. Pre-pass state (\c incoming) is intentionally ignored — a write left over from an
    // earlier pass isn't a hazard with a new use here.
    //
    // The lambda inside \c forEachAspectBit can't directly \c return from \c addTexture, so we
    // collect the verdict in \c hazardFound and bail out after the loops.
    bool hazardFound = false;
    if (state.isWrite() || tracked.hasWrite) {
        for (uint32_t mip = resolved.i.mip; mip < mipEnd; ++mip) {
            for (uint32_t face = resolved.i.face; face < faceEnd; ++face) {
                forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
                    uint64_t key = packPlaneKey(mip, face, bit);
                    auto     it  = tracked.registered.find(key);
                    if (it == tracked.registered.end()) return;
                    const auto & existing = it->second;
                    if (!existing.isWrite() && !state.isWrite()) return;

                    const char * hazardKind = (existing.isWrite() && state.isWrite()) ? "write/write" : "read/write";
                    GN_ERROR(sLogger,
                             "GpuResourceStateTrackerVulkan: {} hazard on texture '{}' aspect=0x{:x} — '{}' ({}) and '{}' ({}) "
                             "both access subresource [mip={} face={}]",
                             hazardKind, tracked.tex->name, static_cast<uint32_t>(bit), existing.usage ? existing.usage : "?",
                             existing.isWrite() ? "write" : "read", state.usage ? state.usage : "?", state.isWrite() ? "write" : "read", mip, face);
                    hazardFound = true;
                });
            }
        }
        if (hazardFound) return false;
    }
    if (!tracked.activeThisPass) {
        tracked.activeThisPass = true;
        mActiveTextures.push_back(&tracked);
    }
    if (state.isWrite()) tracked.hasWrite = true;

    // Lazy: only compute the representative incoming layout if verbose logging is actually active.
    [[maybe_unused]] auto firstIncomingLayout = [&]() -> vk::ImageLayout {
        vk::ImageLayout layout = vk::ImageLayout::eUndefined;
        forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
            if (layout != vk::ImageLayout::eUndefined) return;
            if (const auto * ps = tracked.getIncoming(resolved.i.mip, resolved.i.face, bit)) layout = ps->layout;
        });
        return layout;
    };
    GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: register image '{}' [mip={}-{} face={}-{}] as '{}' (incoming: {})", tex->name, resolved.i.mip,
               mipEnd - 1, resolved.i.face, faceEnd - 1, state.usage ? state.usage : "?", vk::to_string(firstIncomingLayout()));

    // No hazard — record each (mip, face, plane) → intended state.
    for (uint32_t mip = resolved.i.mip; mip < mipEnd; ++mip) {
        for (uint32_t face = resolved.i.face; face < faceEnd; ++face) {
            forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
                tracked.registered[packPlaneKey(mip, face, bit)] = state;
                GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: image '{}' [mip={} face={} {}] registered layout={}", tex->name, mip, face,
                           vk::to_string(bit), vk::to_string(state.layout));
            });
        }
    }
    return true;
}

bool GpuResourceStateTrackerVulkan::addColorTarget(TextureVulkanBase * tex, const GpuResourceView & view) {
    if (!tex) GN_UNLIKELY return true;
    TexturePlaneStateVulkan state;
    state.layout = vk::ImageLayout::eColorAttachmentOptimal;
    state.access = vk::AccessFlagBits::eColorAttachmentWrite;
    state.stages = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    state.usage  = "color target";
    return addTexture(tex, view.imageView, state);
}

bool GpuResourceStateTrackerVulkan::addDepthStencilTarget(TextureVulkanBase * tex, const GpuResourceView & view, bool readOnly) {
    if (!tex) GN_UNLIKELY return true;
    if (readOnly) mHasReadOnlyDepthStencil = true;
    TexturePlaneStateVulkan state;
    state.layout = readOnly ? vk::ImageLayout::eDepthStencilReadOnlyOptimal : vk::ImageLayout::eDepthStencilAttachmentOptimal;
    state.access = vk::AccessFlagBits::eDepthStencilAttachmentRead;
    if (!readOnly) state.access |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    state.stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
    state.usage  = readOnly ? "depth-stencil target (read-only)" : "depth-stencil target";
    return addTexture(tex, view.imageView, state);
}

bool GpuResourceStateTrackerVulkan::addSampledTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages) {
    if (!tex) GN_UNLIKELY return true;
    TexturePlaneStateVulkan state;
    state.layout = shaderReadOnlyLayout(tex->descriptor().format);
    state.access = vk::AccessFlagBits::eShaderRead;
    state.stages = stages;
    state.usage  = "sampled texture";
    return addTexture(tex, view.imageView, state);
}

bool GpuResourceStateTrackerVulkan::addStorageTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages) {
    if (!tex) GN_UNLIKELY return true;
    TexturePlaneStateVulkan state;
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
    GN_ERROR(sLogger, "GpuResourceStateTrackerVulkan: {} hazard on buffer '{}' — '{}' ({}) and '{}' ({})", hazardKind, incoming.buf->name, existing.usageName,
             existing.isWrite ? "write" : "read", incoming.usageName, incoming.isWrite ? "write" : "read");
    return false;
}

bool GpuResourceStateTrackerVulkan::addBuffer(TrackedBuffer b) {
    auto it = mBuffers.find(b.buf->id);
    if (it == mBuffers.end()) {
        const auto readReady = BufferStateVulkan::READ_READY();
        b.committedAccess    = readReady.access;
        b.committedStages    = readReady.stages;
        b.activeThisPass     = true;
        GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: register buffer '{}' as '{}' committed={} pass={}", b.buf->name, b.usageName,
                   vk::to_string(b.committedAccess), vk::to_string(b.passAccess));
        auto entry = mBuffers.emplace(b.buf->id, std::move(b)).first;
        mActiveBuffers.push_back(&entry->second);
        return true;
    }
    auto & existing = it->second;
    // Only flag hazards for same-pass re-registrations; cross-payload re-use is expected and handled
    // by emitPrePassBarriers() via committedAccess.
    if (existing.activeThisPass) {
        if (!existing.isWrite && !b.isWrite) {
            existing.passAccess |= b.passAccess;
            existing.passStages |= b.passStages;
            return true;
        }
        if (!checkBufferHazard(b)) return false;
    }
    if (!existing.activeThisPass) {
        mActiveBuffers.push_back(&existing);
        // Cross-payload re-registration: log the committed state this payload inherits.
        GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: re-register buffer '{}' as '{}' committed={} pass={}", b.buf->name, b.usageName,
                   vk::to_string(existing.committedAccess), vk::to_string(b.passAccess));
    }
    existing.passAccess |= b.passAccess;
    existing.passStages |= b.passStages;
    existing.isWrite |= b.isWrite;
    existing.activeThisPass = true;
    GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: buffer '{}' pass access merged to {}", existing.buf->name, vk::to_string(existing.passAccess));
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
    TexturePlaneStateVulkan state;
    state.layout = vk::ImageLayout::eTransferDstOptimal;
    state.access = vk::AccessFlagBits::eTransferWrite;
    state.stages = vk::PipelineStageFlagBits::eTransfer;
    state.usage  = "transfer destination image";
    return addTexture(tex, view, state);
}

bool GpuResourceStateTrackerVulkan::addTransferSrcImage(TextureVulkanBase * tex, const GpuResourceView::ImageView & view) {
    if (!tex) GN_UNLIKELY return true;
    TexturePlaneStateVulkan state;
    state.layout = vk::ImageLayout::eTransferSrcOptimal;
    state.access = vk::AccessFlagBits::eTransferRead;
    state.stages = vk::PipelineStageFlagBits::eTransfer;
    state.usage  = "transfer source image";
    return addTexture(tex, view, state);
}

std::vector<int64_t> GpuResourceStateTrackerVulkan::addGpuResourceTable(const GpuResourceTable & table) {
    std::vector<int64_t> invalid;
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
            GN_ERROR(sLogger, "GpuResourceStateTrackerVulkan: render target hazard on color target slot {}; aborting render pass", i);
            ok = false;
        }
    }
    const GpuResourceView depthStencilView = rt.depthStencilTarget.view();
    if (depthStencilView.isTexture() && depthStencilView.texture()) {
        auto * tex = RuntimeType::cast<TextureVulkanBase>(depthStencilView.texture().get());
        if (tex) {
            // Render targets are cleared upon pass begin (loadOp = eClear) which is an attachment write;
            // depth-stencil target must be in attachment-optimal layout during rendering.
            if (!addDepthStencilTarget(tex, depthStencilView, /*readOnly=*/false)) {
                GN_ERROR(sLogger, "GpuResourceStateTrackerVulkan: render target hazard on depth-stencil target; aborting render pass");
                ok = false;
            }
        }
    }
    return ok;
}

void GpuResourceStateTrackerVulkan::upgradeForDrawRasterState(const RasterState & drawState) {
    if (!mHasReadOnlyDepthStencil) return;
    bool needsDepthWrite   = drawState.depthState && drawState.depthState->writeEnabled();
    bool needsStencilWrite = drawState.stencilState && drawState.stencilState->enabled();
    if (!needsDepthWrite && !needsStencilWrite) return;

    TexturePlaneStateVulkan promoted;
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
            GN_VERBOSE(sLogger,
                       "GpuResourceStateTrackerVulkan: promoting depth-stencil '{}' mip={} face={} from read-only to read-write "
                       "(draw requires {})",
                       tracked.tex->name, mip, face, needsDepthWrite ? "depth write" : "stencil write");
            intended = promoted;
        }
    }
    mHasReadOnlyDepthStencil = false;
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
        GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: texturePassLayout('{}' mip={} face={}) -> eUndefined (not tracked)", tex->name, mip, face);
        return vk::ImageLayout::eUndefined;
    }
    const auto & tracked = it->second;

    // Query incoming (post-barrier authoritative state) — registered is cleared by emitPrePassBarriers
    // before this function is ever called, so querying registered would always return eUndefined.
    vk::ImageLayout colorLayout   = vk::ImageLayout::eUndefined;
    vk::ImageLayout depthLayout   = vk::ImageLayout::eUndefined;
    vk::ImageLayout stencilLayout = vk::ImageLayout::eUndefined;
    forEachAspectBit(tracked.validAspects, [&](vk::ImageAspectFlagBits bit) {
        const auto * ps = tracked.getIncoming(mip, face, bit);
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

    GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: texturePassLayout('{}' mip={} face={}) -> {} ({})", tex->name, mip, face, vk::to_string(result),
               reason);
    return result;
}

void GpuResourceStateTrackerVulkan::emitPrePassBarriers(vk::CommandBuffer cb) {
    auto & bufferBarriers = mBufferBarriers;
    auto & barriers       = mImageBarriers;
    bufferBarriers.clear();
    barriers.clear();
    vk::PipelineStageFlags srcStages = {};
    vk::PipelineStageFlags dstStages = {};

    for (auto * active : mActiveBuffers) {
        auto & b = *active;

        bool prevHadWrite = bool(b.committedAccess & (vk::AccessFlagBits::eTransferWrite | vk::AccessFlagBits::eShaderWrite));
        bool needsBarrier =
            prevHadWrite || b.isWrite || ((b.committedAccess & b.passAccess) != b.passAccess) || ((b.committedStages & b.passStages) != b.passStages);

        if (needsBarrier) {
            vk::Buffer vkBuf = b.buf->nativeBuffer();
            if (!vkBuf) GN_UNLIKELY {
                    GN_WARN(sLogger, "GpuResourceStateTrackerVulkan: buffer '{}' has no VkBuffer handle; skipping barrier", b.buf->name);
                }
            else {
                GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: buffer barrier '{}' [{}] : access {} -> {}", b.buf->name, b.usageName,
                           vk::to_string(b.committedAccess), vk::to_string(b.passAccess));
                vk::BufferMemoryBarrier barrier;
                barrier.setSrcAccessMask(b.committedAccess)
                    .setDstAccessMask(b.passAccess)
                    .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                    .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                    .setBuffer(vkBuf)
                    .setOffset(0)
                    .setSize(VK_WHOLE_SIZE);
                bufferBarriers.push_back(barrier);
                srcStages |= b.committedStages;
                dstStages |= b.passStages;
                b.committedAccess = b.passAccess;
                b.committedStages = b.passStages;
                GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: buffer '{}' committed updated to {}", b.buf->name, vk::to_string(b.committedAccess));
            }
        }

        // Commit: reset per-pass fields regardless of whether a barrier was emitted.
        b.passAccess     = {};
        b.passStages     = vk::PipelineStageFlagBits::eBottomOfPipe;
        b.isWrite        = false;
        b.activeThisPass = false;
        GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: buffer '{}' pass state reset", b.buf->name);
    }

    for (auto * active : mActiveTextures) {
        auto & tracked         = *active;
        tracked.activeThisPass = false;
        vk::Image vkImg        = tracked.tex->nativeImage();
        if (!vkImg) GN_UNLIKELY {
                GN_WARN(sLogger, "GpuResourceStateTrackerVulkan: texture '{}' has no VkImage handle; skipping barrier", tracked.tex->name);
                continue;
            }

        // Iterate only planes the pass registered; untouched planes need no barrier.
        for (const auto & [key, next] : tracked.registered) {
            uint32_t                mip    = uint32_t((key >> 48) & 0xffffu);
            uint32_t                face   = uint32_t((key >> 16) & 0xffffu);
            vk::ImageAspectFlagBits aspect = static_cast<vk::ImageAspectFlagBits>(uint32_t(key & 0xffffu));

            const auto * prev = tracked.getIncoming(mip, face, aspect);
            if (!prev) GN_UNLIKELY continue;
            if (*prev == next) continue;

            GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: image barrier '{}' [mip={} face={} {}] : {} -> {} ({})", tracked.tex->name, mip, face,
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
            barriers.push_back(b);
            srcStages |= prev->stages;
            dstStages |= next.stages;

            // Advance the running incoming state so the next payload's add*() calls see the
            // correct post-barrier layout without any extra bookkeeping.
            tracked.setIncoming(mip, face, aspect, next);
            GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: image '{}' [mip={} face={} {}] incoming updated to {}", tracked.tex->name, mip, face,
                       vk::to_string(aspect), vk::to_string(next.layout));
        }

        // Registered states are baked into barriers; clear so the next payload starts fresh.
        GN_VERBOSE(sLogger, "GpuResourceStateTrackerVulkan: image '{}' registered cleared ({} planes)", tracked.tex->name, tracked.registered.size());
        tracked.registered.clear();
        tracked.hasWrite = false;
    }
    mActiveBuffers.clear();
    mActiveTextures.clear();
    mHasReadOnlyDepthStencil = false;

    if (bufferBarriers.empty() && barriers.empty()) return;

    if (!srcStages) srcStages = vk::PipelineStageFlagBits::eTopOfPipe;
    if (!dstStages) dstStages = vk::PipelineStageFlagBits::eBottomOfPipe;

    cb.pipelineBarrier(srcStages, dstStages, {}, nullptr,
                       vk::ArrayProxy<const vk::BufferMemoryBarrier>((uint32_t) bufferBarriers.size(), bufferBarriers.data()),
                       vk::ArrayProxy<const vk::ImageMemoryBarrier>((uint32_t) barriers.size(), barriers.data()));
}

void GpuResourceStateTrackerVulkan::restoreAttachmentToShaderReadOnly(TextureVulkanBase * tex, const GpuResourceView & view, vk::CommandBuffer cb) {
    // Swapchain images are presentation targets and commonly lack sampled usage; their
    // final layout is selected by present(), so never transition them to shader-read-only.
    if (!tex || !tex->nativeImage() || tex->isBackbuffer()) return;
    auto                 vkImg    = tex->nativeImage();
    const auto &         desc     = tex->descriptor();
    const auto           resolved = resolveRange(view.imageView.range, desc);
    uint32_t             mip      = resolved.i.mip;
    uint32_t             face     = resolved.i.face;
    uint32_t             numMips  = resolved.e.numMipLevels;
    uint32_t             numFaces = resolved.e.numArrayLayers;
    vk::ImageAspectFlags aspect   = aspectFromView(view.imageView, desc);
    if (!aspect) aspect = vk::ImageAspectFlagBits::eColor;
    bool            isDepthStencil = bool(aspect & (vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil));
    vk::ImageLayout targetLayout   = shaderReadOnlyLayout(desc.format);

    auto                   it        = mTextures.find(tex->id);
    vk::ImageLayout        oldLayout = isDepthStencil ? vk::ImageLayout::eDepthStencilAttachmentOptimal : vk::ImageLayout::eColorAttachmentOptimal;
    vk::AccessFlags        srcAccess = isDepthStencil ? vk::AccessFlagBits::eDepthStencilAttachmentWrite : vk::AccessFlagBits::eColorAttachmentWrite;
    vk::PipelineStageFlags srcStages = isDepthStencil ? (vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests)
                                                      : vk::PipelineStageFlagBits::eColorAttachmentOutput;

    if (it != mTextures.end()) {
        auto & tracked = it->second;
        forEachAspectBit(aspect, [&](vk::ImageAspectFlagBits bit) {
            const auto * prev = tracked.getIncoming(mip, face, bit);
            if (prev) {
                oldLayout = prev->layout;
                srcAccess = prev->access;
                srcStages = prev->stages;
            }
        });
    }

    if (isDepthStencil && srcAccess == vk::AccessFlagBits::eColorAttachmentWrite) {
        srcAccess = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        srcStages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
    }

    bool hasWrite = bool(srcAccess & (vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite |
                                      vk::AccessFlagBits::eTransferWrite | vk::AccessFlagBits::eShaderWrite));
    if (oldLayout == targetLayout && !hasWrite) {
        return; // Already in target read-only layout without pending write flushes
    }

    vk::AccessFlags dstAccess =
        isDepthStencil ? (vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eDepthStencilAttachmentRead) : vk::AccessFlagBits::eShaderRead;
    vk::PipelineStageFlags dstStages =
        isDepthStencil
            ? (vk::PipelineStageFlagBits::eFragmentShader | vk::PipelineStageFlagBits::eComputeShader | vk::PipelineStageFlagBits::eEarlyFragmentTests)
            : (vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader | vk::PipelineStageFlagBits::eComputeShader);

    vk::ImageMemoryBarrier b;
    b.setOldLayout(oldLayout)
        .setNewLayout(targetLayout)
        .setImage(vkImg)
        .setSubresourceRange({aspect, mip, numMips, face, numFaces})
        .setSrcAccessMask(srcAccess)
        .setDstAccessMask(dstAccess)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);

    cb.pipelineBarrier(srcStages, dstStages, {}, 0, nullptr, 0, nullptr, 1, &b);

    TexturePlaneStateVulkan next;
    next.layout = targetLayout;
    next.access = dstAccess;
    next.stages = dstStages;
    next.usage  = isDepthStencil ? "auto-restore depth-stencil read only" : "auto-restore shader read only";

    if (it != mTextures.end()) {
        auto & tracked = it->second;
        for (uint32_t m = mip; m < mip + numMips && m < tracked.numMips; ++m) {
            for (uint32_t f = face; f < face + numFaces && f < tracked.numLayers; ++f) {
                forEachAspectBit(aspect, [&](vk::ImageAspectFlagBits bit) { tracked.setIncoming(m, f, bit, next); });
            }
        }
    }
}

void GpuResourceStateTrackerVulkan::restoreBuffersToReadReady(ArrayView<BufferVulkan * const> bufs, vk::CommandBuffer cb) {
    if (bufs.empty()) return;

    const auto readReady = BufferStateVulkan::READ_READY();

    DynaArray<vk::BufferMemoryBarrier> barriers;
    vk::PipelineStageFlags             srcStages = {};

    std::unordered_set<int64_t> seen;

    for (auto * buf : bufs) {
        if (!buf || !buf->nativeBuffer()) continue;
        if (!seen.insert(buf->id).second) continue;

        auto                   it        = mBuffers.find(buf->id);
        vk::AccessFlags        srcAccess = {};
        vk::PipelineStageFlags bufStages = vk::PipelineStageFlagBits::eTopOfPipe;

        if (it != mBuffers.end()) {
            auto & tracked = it->second;
            if (tracked.committedAccess) {
                srcAccess = tracked.committedAccess;
                bufStages = tracked.committedStages;
            }
        } else {
            srcAccess = vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eTransferWrite;
            bufStages = vk::PipelineStageFlagBits::eAllCommands;
        }

        bool hasWrite = bool(srcAccess & (vk::AccessFlagBits::eTransferWrite | vk::AccessFlagBits::eShaderWrite));
        if (!hasWrite && (srcAccess & readReady.access) == readReady.access && (bufStages & readReady.stages) == readReady.stages) { continue; }

        if (!bufStages) bufStages = vk::PipelineStageFlagBits::eTopOfPipe;

        vk::BufferMemoryBarrier b;
        b.setSrcAccessMask(srcAccess)
            .setDstAccessMask(readReady.access)
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setBuffer(buf->nativeBuffer())
            .setOffset(0)
            .setSize(VK_WHOLE_SIZE);

        barriers.append(b);
        srcStages |= bufStages;

        if (it != mBuffers.end()) {
            auto & tracked          = it->second;
            tracked.committedAccess = readReady.access;
            tracked.committedStages = readReady.stages;
            tracked.passAccess      = {};
            tracked.passStages      = vk::PipelineStageFlagBits::eBottomOfPipe;
            tracked.isWrite         = false;
        }
    }

    if (barriers.empty()) return;
    if (!srcStages) srcStages = vk::PipelineStageFlagBits::eTopOfPipe;

    cb.pipelineBarrier(srcStages, readReady.stages, {}, 0, nullptr, static_cast<uint32_t>(barriers.size()), barriers.data(), 0, nullptr);
}

void GpuResourceStateTrackerVulkan::restoreBufferToReadReady(BufferVulkan * buf, vk::CommandBuffer cb) {
    if (!buf) return;
    restoreBuffersToReadReady(ArrayView<BufferVulkan * const>(&buf, 1), cb);
}

} // namespace GN::gpu2
