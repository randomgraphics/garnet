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
    return a.i.mip < bMipEnd && b.i.mip < aMipEnd && a.i.face < bFaceEnd && b.i.face < aFaceEnd;
}

bool RasterStateTrackerVulkan::checkHazard(const TrackedAttachment & incoming) const {
    for (const auto & existing : mAttachments) {
        if (existing.tex != incoming.tex) continue;
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

void RasterStateTrackerVulkan::addAttachment(TrackedAttachment a) {
    checkHazard(a);
    mAttachments.append(std::move(a));
}

void RasterStateTrackerVulkan::addColorTarget(TextureVulkanBase * tex, const GpuResourceView & view) {
    if (!tex) return;
    TrackedAttachment a;
    a.tex           = tex;
    a.resolvedRange = resolveRange(view, tex->descriptor());
    a.passLayout    = vk::ImageLayout::eColorAttachmentOptimal;
    a.passAccess    = vk::AccessFlagBits::eColorAttachmentWrite;
    a.passStages    = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    a.finalLayout   = vk::ImageLayout::eColorAttachmentOptimal;
    a.finalAccess   = vk::AccessFlagBits::eColorAttachmentWrite;
    a.finalStages   = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    a.aspect        = vk::ImageAspectFlagBits::eColor;
    a.isWrite       = true;
    a.usageName     = "color target";
    addAttachment(std::move(a));
}

void RasterStateTrackerVulkan::addDepthStencilTarget(TextureVulkanBase * tex, const GpuResourceView & view, bool readOnly) {
    if (!tex) return;
    TrackedAttachment a;
    a.tex           = tex;
    a.resolvedRange = resolveRange(view, tex->descriptor());
    a.passLayout    = readOnly ? vk::ImageLayout::eDepthStencilReadOnlyOptimal : vk::ImageLayout::eDepthStencilAttachmentOptimal;
    a.passAccess    = vk::AccessFlagBits::eDepthStencilAttachmentRead;
    if (!readOnly) a.passAccess |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    a.passStages  = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
    a.finalLayout = a.passLayout;
    a.finalAccess = a.passAccess;
    a.finalStages = a.passStages;
    a.aspect      = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
    a.isWrite     = !readOnly;
    a.usageName   = readOnly ? "depth-stencil target (read-only)" : "depth-stencil target";
    addAttachment(std::move(a));
}

void RasterStateTrackerVulkan::addSampledTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages) {
    if (!tex) return;
    TrackedAttachment a;
    a.tex           = tex;
    a.resolvedRange = resolveRange(view, tex->descriptor());
    a.passLayout    = vk::ImageLayout::eShaderReadOnlyOptimal;
    a.passAccess    = vk::AccessFlagBits::eShaderRead;
    a.passStages    = stages;
    a.finalLayout   = vk::ImageLayout::eShaderReadOnlyOptimal;
    a.finalAccess   = vk::AccessFlagBits::eShaderRead;
    a.finalStages   = stages;
    a.aspect        = vk::ImageAspectFlagBits::eColor;
    a.isWrite       = false;
    a.usageName     = "sampled texture";
    addAttachment(std::move(a));
}

void RasterStateTrackerVulkan::addStorageTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages) {
    if (!tex) return;
    TrackedAttachment a;
    a.tex           = tex;
    a.resolvedRange = resolveRange(view, tex->descriptor());
    a.passLayout    = vk::ImageLayout::eGeneral;
    a.passAccess    = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite;
    a.passStages    = stages;
    a.finalLayout   = vk::ImageLayout::eGeneral;
    a.finalAccess   = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite;
    a.finalStages   = stages;
    a.aspect        = vk::ImageAspectFlagBits::eColor;
    a.isWrite       = true;
    a.usageName     = "storage texture";
    addAttachment(std::move(a));
}

void RasterStateTrackerVulkan::emitPrePassBarriers(vk::CommandBuffer cb) {
    DynaArray<vk::ImageMemoryBarrier> barriers;
    vk::PipelineStageFlags            srcStages = {};
    vk::PipelineStageFlags            dstStages = {};

    for (const auto & a : mAttachments) {
        vk::Image vkImg = a.tex->vulkanNativeImage();
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

    if (barriers.empty()) return;

    if (!srcStages) srcStages = vk::PipelineStageFlagBits::eTopOfPipe;
    if (!dstStages) dstStages = vk::PipelineStageFlagBits::eBottomOfPipe;

    cb.pipelineBarrier(srcStages, dstStages, {}, nullptr, nullptr, vk::ArrayProxy<const vk::ImageMemoryBarrier>(barriers.size(), barriers.data()));
}

void RasterStateTrackerVulkan::flushStatesToResources() {
    TextureGpuImageState::ImageState finalState;
    for (const auto & a : mAttachments) {
        finalState.layout = a.finalLayout;
        finalState.access = a.finalAccess;
        finalState.stages = a.finalStages;
        a.tex->gpuStates.set(a.resolvedRange, finalState, a.tex->name);
    }
}

} // namespace GN::gpu2
