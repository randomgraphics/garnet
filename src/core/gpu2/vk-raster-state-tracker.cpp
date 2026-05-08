#include "vk-raster-state-tracker.h"
#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

GpuResourceView::SubresourceRange RasterStateTrackerVulkan::resolveRange(const GpuResourceView & view, const Texture::Descriptor & d) {
    GpuResourceView::SubresourceRange r  = view.imageView.range;
    uint32_t                          nm = r.e.numMipLevels;
    uint32_t                          na = r.e.numArrayLayers;
    if (nm == (uint32_t) -1) nm = d.levels > r.i.mip ? d.levels - r.i.mip : 0;
    if (na == (uint32_t) -1) na = d.faces > r.i.face ? d.faces - r.i.face : 0;
    r.e.numMipLevels   = nm;
    r.e.numArrayLayers = na;
    return r;
}

bool RasterStateTrackerVulkan::subresourceRangesOverlap(const GpuResourceView::SubresourceRange & a, const GpuResourceView::SubresourceRange & b) {
    uint32_t aMipEnd  = a.i.mip + a.e.numMipLevels;
    uint32_t bMipEnd  = b.i.mip + b.e.numMipLevels;
    uint32_t aFaceEnd = a.i.face + a.e.numArrayLayers;
    uint32_t bFaceEnd = b.i.face + b.e.numArrayLayers;
    // Standard interval-overlap test on both mip and face (array layer) axes.
    // Binding individual cubemap faces to separate color target slots is legal: each view
    // covers exactly one face (numArrayLayers == 1) at distinct face indices, so the face
    // intervals [0,1) and [1,2) don't overlap and no hazard is reported.
    return a.i.mip < bMipEnd && b.i.mip < aMipEnd && a.i.face < bFaceEnd && b.i.face < aFaceEnd;
}

bool RasterStateTrackerVulkan::checkHazard(const TrackedAttachment & incoming) const {
    auto it = mAttachments.find(incoming.tex->id);
    if (it == mAttachments.end()) return true;
    for (const auto & existing : it->second) {
        if (!subresourceRangesOverlap(existing.resolvedRange, incoming.resolvedRange)) continue;
        if (!existing.isWrite && !incoming.isWrite) continue; // read+read is always safe

        const char * hazardKind = (existing.isWrite && incoming.isWrite) ? "write/write" : "read/write";
        GN_ERROR(sLogger)("RasterStateTrackerVulkan: {} hazard on texture '{}' — '{}' ({}) and '{}' ({}) both access "
                          "overlapping subresources [mip={} count={} face={} count={}] vs [mip={} count={} face={} count={}]",
                          hazardKind, incoming.tex->name, existing.usageName, existing.isWrite ? "write" : "read", incoming.usageName,
                          incoming.isWrite ? "write" : "read", existing.resolvedRange.i.mip, existing.resolvedRange.e.numMipLevels,
                          existing.resolvedRange.i.face, existing.resolvedRange.e.numArrayLayers, incoming.resolvedRange.i.mip,
                          incoming.resolvedRange.e.numMipLevels, incoming.resolvedRange.i.face, incoming.resolvedRange.e.numArrayLayers);
        return false;
    }
    return true;
}

bool RasterStateTrackerVulkan::addAttachment(TrackedAttachment a) {
    if (!checkHazard(a)) return false; // hazard: reject, tracker state unchanged
    mAttachments[a.tex->id].push_back(std::move(a));
    return true;
}

bool RasterStateTrackerVulkan::addColorTarget(TextureVulkanBase * tex, const GpuResourceView & view) {
    if (!tex) return true;
    TrackedAttachment a;
    a.tex           = tex;
    a.resolvedRange = resolveRange(view, tex->descriptor());
    a.passLayout    = vk::ImageLayout::eColorAttachmentOptimal;
    a.passAccess    = vk::AccessFlagBits::eColorAttachmentWrite;
    a.passStages    = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    a.aspect        = vk::ImageAspectFlagBits::eColor;
    a.isWrite       = true;
    a.usageName     = "color target";
    return addAttachment(std::move(a));
}

bool RasterStateTrackerVulkan::addDepthStencilTarget(TextureVulkanBase * tex, const GpuResourceView & view, bool readOnly) {
    if (!tex) return true;
    TrackedAttachment a;
    a.tex           = tex;
    a.resolvedRange = resolveRange(view, tex->descriptor());
    a.passLayout    = readOnly ? vk::ImageLayout::eDepthStencilReadOnlyOptimal : vk::ImageLayout::eDepthStencilAttachmentOptimal;
    a.passAccess    = vk::AccessFlagBits::eDepthStencilAttachmentRead;
    if (!readOnly) a.passAccess |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    a.passStages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
    a.aspect     = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
    a.isWrite    = !readOnly;
    a.usageName  = readOnly ? "depth-stencil target (read-only)" : "depth-stencil target";
    return addAttachment(std::move(a));
}

bool RasterStateTrackerVulkan::addSampledTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages) {
    if (!tex) return true;
    TrackedAttachment a;
    a.tex           = tex;
    a.resolvedRange = resolveRange(view, tex->descriptor());
    a.passLayout    = vk::ImageLayout::eShaderReadOnlyOptimal;
    a.passAccess    = vk::AccessFlagBits::eShaderRead;
    a.passStages    = stages;
    a.aspect        = vk::ImageAspectFlagBits::eColor;
    a.isWrite       = false;
    a.usageName     = "sampled texture";
    return addAttachment(std::move(a));
}

bool RasterStateTrackerVulkan::checkBufferHazard(const TrackedBuffer & incoming) const {
    auto it = mBuffers.find(incoming.buf->id);
    if (it == mBuffers.end()) return true;
    const TrackedBuffer & existing = it->second;
    if (!existing.isWrite && !incoming.isWrite) return true; // read+read is always safe
    const char * hazardKind = (existing.isWrite && incoming.isWrite) ? "write/write" : "read/write";
    GN_ERROR(sLogger)("RasterStateTrackerVulkan: {} hazard on buffer '{}' — '{}' ({}) and '{}' ({})", hazardKind, incoming.buf->name, existing.usageName,
                      existing.isWrite ? "write" : "read", incoming.usageName, incoming.isWrite ? "write" : "read");
    return false;
}

bool RasterStateTrackerVulkan::addBuffer(TrackedBuffer b) {
    auto it = mBuffers.find(b.buf->id);
    if (it != mBuffers.end()) {
        if (!checkBufferHazard(b)) return false; // hazard: reject, existing entry unchanged
        // Compatible re-registration: merge access flags so the single barrier covers all usages.
        it->second.passAccess |= b.passAccess;
        it->second.passStages |= b.passStages;
        it->second.isWrite |= b.isWrite;
        return true;
    }
    mBuffers.emplace(b.buf->id, std::move(b));
    return true;
}

bool RasterStateTrackerVulkan::addUniformBuffer(BufferVulkan * buf, vk::PipelineStageFlags stages) {
    if (!buf) return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eUniformRead;
    b.passStages = stages;
    b.isWrite    = false;
    b.usageName  = "uniform buffer";
    return addBuffer(std::move(b));
}

bool RasterStateTrackerVulkan::addStorageBuffer(BufferVulkan * buf, bool write, vk::PipelineStageFlags stages) {
    if (!buf) return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eShaderRead | (write ? vk::AccessFlagBits::eShaderWrite : vk::AccessFlags {});
    b.passStages = stages;
    b.isWrite    = write;
    b.usageName  = write ? "storage buffer (read-write)" : "storage buffer (read-only)";
    return addBuffer(std::move(b));
}

bool RasterStateTrackerVulkan::addVertexBuffer(BufferVulkan * buf) {
    if (!buf) return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eVertexAttributeRead;
    b.passStages = vk::PipelineStageFlagBits::eVertexInput;
    b.isWrite    = false;
    b.usageName  = "vertex buffer";
    return addBuffer(std::move(b));
}

bool RasterStateTrackerVulkan::addIndexBuffer(BufferVulkan * buf) {
    if (!buf) return true;
    TrackedBuffer b;
    b.buf        = buf;
    b.passAccess = vk::AccessFlagBits::eIndexRead;
    b.passStages = vk::PipelineStageFlagBits::eVertexInput;
    b.isWrite    = false;
    b.usageName  = "index buffer";
    return addBuffer(std::move(b));
}

std::vector<uint64_t> RasterStateTrackerVulkan::addGpuResourceTable(const GpuResourceTable & table) {
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

bool RasterStateTrackerVulkan::addRasterGeometry(const RasterGeometry & geom) {
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

bool RasterStateTrackerVulkan::addRasterTarget(const RasterTarget & rt) {
    bool ok = true;
    for (size_t i = 0; i < rt.colorTargets.size(); ++i) {
        const auto & ct = rt.colorTargets[i];
        if (!ct.target.isTexture() || !ct.target.texture()) continue;
        auto * tex = RuntimeType::cast<TextureVulkanBase>(ct.target.texture().get());
        if (tex && !addColorTarget(tex, ct.target)) {
            GN_ERROR(sLogger)("RasterStateTrackerVulkan: render target hazard on color target slot {}; aborting render pass", i);
            ok = false;
        }
    }
    if (rt.depthStencilTarget.isTexture() && rt.depthStencilTarget.texture()) {
        auto * tex = RuntimeType::cast<TextureVulkanBase>(rt.depthStencilTarget.texture().get());
        if (tex) {
            const auto & ds = rt.states;
            bool         ro = ds.depthState && !ds.depthState->writeEnabled() && !(ds.stencilState && ds.stencilState->enabled());
            if (!addDepthStencilTarget(tex, rt.depthStencilTarget, ro)) {
                GN_ERROR(sLogger)("RasterStateTrackerVulkan: render target hazard on depth-stencil target; aborting render pass");
                ok = false;
            }
        }
    }
    return ok;
}

void RasterStateTrackerVulkan::upgradeForDrawRasterState(const RasterState & drawState) {
    bool needsDepthWrite   = drawState.depthState && drawState.depthState->writeEnabled();
    bool needsStencilWrite = drawState.stencilState && drawState.stencilState->enabled();
    if (!needsDepthWrite && !needsStencilWrite) return;

    for (auto & [id, attachments] : mAttachments) {
        for (auto & a : attachments) {
            if (a.passLayout != vk::ImageLayout::eDepthStencilReadOnlyOptimal) continue;
            GN_VERBOSE(sLogger)("RasterStateTrackerVulkan: promoting depth-stencil '{}' from read-only to read-write "
                                "(draw requires {})",
                                a.tex->name, needsDepthWrite ? "depth write" : "stencil write");
            a.passLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
            a.passAccess |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
            a.isWrite   = true;
            a.usageName = "depth-stencil target (promoted to read-write)";
        }
    }
}

vk::ImageLayout RasterStateTrackerVulkan::attachmentPassLayout(const TextureVulkanBase * tex) const {
    auto it = mAttachments.find(tex->id);
    if (it == mAttachments.end() || it->second.empty()) return vk::ImageLayout::eUndefined;
    return it->second.front().passLayout;
}

bool RasterStateTrackerVulkan::addStorageTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages) {
    if (!tex) return true;
    TrackedAttachment a;
    a.tex           = tex;
    a.resolvedRange = resolveRange(view, tex->descriptor());
    a.passLayout    = vk::ImageLayout::eGeneral;
    a.passAccess    = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite;
    a.passStages    = stages;
    a.aspect        = vk::ImageAspectFlagBits::eColor;
    a.isWrite       = true;
    a.usageName     = "storage texture";
    return addAttachment(std::move(a));
}

void RasterStateTrackerVulkan::emitPrePassBarriers(vk::CommandBuffer cb) {
    DynaArray<vk::BufferMemoryBarrier> bufferBarriers;
    DynaArray<vk::ImageMemoryBarrier>  barriers;
    vk::PipelineStageFlags             srcStages = {};
    vk::PipelineStageFlags             dstStages = {};

    for (const auto & [id, b] : mBuffers) {
        vk::Buffer vkBuf = b.buf->nativeBuffer();
        if (!vkBuf) {
            GN_WARN(sLogger)("RasterStateTrackerVulkan: buffer '{}' has no VkBuffer handle; skipping barrier", b.buf->name);
            continue;
        }
        const BufferStateVulkan & cur = b.buf->gpuState;
        if (cur.access == b.passAccess && cur.stages == b.passStages) continue;

        vk::BufferMemoryBarrier barrier;
        barrier.setSrcAccessMask(cur.access)
            .setDstAccessMask(b.passAccess)
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setBuffer(vkBuf)
            .setOffset(0)
            .setSize(VK_WHOLE_SIZE);
        bufferBarriers.append(barrier);
        srcStages |= cur.stages;
        dstStages |= b.passStages;
    }

    for (const auto & [id, attachments] : mAttachments) {
        for (const auto & a : attachments) {
            vk::Image vkImg = a.tex->nativeImage();
            if (!vkImg) {
                GN_WARN(sLogger)("RasterStateTrackerVulkan: texture '{}' has no VkImage handle; skipping barrier", a.tex->name);
                continue;
            }

            const auto & r       = a.resolvedRange;
            uint32_t     mipEnd  = r.i.mip + r.e.numMipLevels;
            uint32_t     faceEnd = r.i.face + r.e.numArrayLayers;

            for (uint32_t mip = r.i.mip; mip < mipEnd; ++mip) {
                for (uint32_t face = r.i.face; face < faceEnd; ++face) {
                    const TextureGpuImageState::ImageState * cur       = a.tex->gpuStates.get(mip, face);
                    vk::ImageLayout                          oldLayout = cur ? cur->layout : vk::ImageLayout::eUndefined;
                    vk::AccessFlags                          srcAccess = cur ? cur->access : vk::AccessFlags {};
                    vk::PipelineStageFlags                   srcStage  = cur ? cur->stages : vk::PipelineStageFlagBits::eTopOfPipe;

                    if (oldLayout == a.passLayout && srcAccess == a.passAccess && srcStage == a.passStages) continue;

                    vk::ImageMemoryBarrier b;
                    b.setOldLayout(oldLayout)
                        .setNewLayout(a.passLayout)
                        .setImage(vkImg)
                        .setSubresourceRange({a.aspect, mip, 1, face, 1})
                        .setSrcAccessMask(srcAccess)
                        .setDstAccessMask(a.passAccess)
                        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
                    barriers.append(b);
                    srcStages |= srcStage;
                    dstStages |= a.passStages;
                }
            }
        }
    }

    if (bufferBarriers.empty() && barriers.empty()) return;

    if (!srcStages) srcStages = vk::PipelineStageFlagBits::eTopOfPipe;
    if (!dstStages) dstStages = vk::PipelineStageFlagBits::eBottomOfPipe;

    cb.pipelineBarrier(srcStages, dstStages, {}, nullptr, vk::ArrayProxy<const vk::BufferMemoryBarrier>(bufferBarriers.size(), bufferBarriers.data()),
                       vk::ArrayProxy<const vk::ImageMemoryBarrier>(barriers.size(), barriers.data()));
}

void RasterStateTrackerVulkan::flushStatesToResources() {
    TextureGpuImageState::ImageState state;
    for (const auto & [id, attachments] : mAttachments) {
        for (const auto & a : attachments) {
            state.layout = a.passLayout;
            state.access = a.passAccess;
            state.stages = a.passStages;
            a.tex->gpuStates.set(a.resolvedRange, state, a.tex->name);
        }
    }
    for (const auto & [id, b] : mBuffers) {
        b.buf->gpuState.access = b.passAccess;
        b.buf->gpuState.stages = b.passStages;
    }
}

} // namespace GN::gpu2
