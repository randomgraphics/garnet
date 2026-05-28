# Mip Level Handling in Garnet GPU2 and RDG2

## Overview
This document details how render targets with mip levels are initialized, tracked, and transitioned in the Garnet 3D engine, specifically focusing on the Vulkan backend's GPU2 abstraction layer and RDG2 implementation.

---

## 1. Core Data Structures for Mip Level Subresources

### GpuResourceView::SubresourceRange
**File:** [src/inc/garnet/gpu2/gpu2.h](src/inc/garnet/gpu2/gpu2.h)

```cpp
struct SubresourceRange {
    SubresourceIndex  i = {};     // default to (mip=0, face=0)
    SubresourceExtent e = {};     // default to (numMipLevels=-1, numArrayLayers=-1) = whole resource
};

struct SubresourceIndex {
    uint32_t mip  = 0;            // Starting mip level
    uint32_t face = 0;            // Starting array layer / face
};

struct SubresourceExtent {
    uint32_t numMipLevels   = (uint32_t) -1;   // Number of mip levels (-1 = remaining)
    uint32_t numArrayLayers = (uint32_t) -1;   // Number of array layers (-1 = remaining)
};
```

This structure is embedded in `ImageView`, allowing views to specify which mip/face subresources are accessed.

### GpuResourceView::ImageView
**File:** [src/inc/garnet/gpu2/gpu2.h](src/inc/garnet/gpu2/gpu2.h)

```cpp
struct ImageView {
    enum Type { SAMPLED, STORAGE };
    Type                  type   = SAMPLED;
    SubresourceRange      range  = {};        // Contains mip and face indices
    gfx::img::PixelFormat format = UNKNOWN(); // Optional override
};
```

### RenderTarget GenericTarget
**File:** [src/inc/garnet/gpu2/raster.h](src/inc/garnet/gpu2/raster.h)

```cpp
struct GenericTarget {
    AutoRef<Texture>      texture = {};
    gfx::img::PixelFormat format  = UNKNOWN();
    uint32_t              mip  : 16 = 0;      // Mip level for this render target
    uint32_t              face : 16 = 0;      // Array layer / face for this render target
    
    // Converts to a GpuResourceView for state tracking
    GpuResourceView view() const {
        GpuResourceView v;
        v.resource = texture;
        v.setSubresourceIndex(GpuResourceView::SubresourceIndex {.mip = mip, .face = face})
            .setSubresourceExtent(GpuResourceView::SubresourceExtent {
                .numMipLevels = 1,       // Single mip level
                .numArrayLayers = 1      // Single array layer
            })
            .setImageViewFormat(format);
        return v;
    }
};
```

---

## 2. GpuResourceStateTrackerVulkan Implementation

**File:** [src/core/gpu2/vk-gpu-resource-state-tracker.h](src/core/gpu2/vk-gpu-resource-state-tracker.h) / [.cpp](src/core/gpu2/vk-gpu-resource-state-tracker.cpp)

### Purpose
Tracks Vulkan image/buffer state transitions across one or more raster passes in a batch. Lifetime matches one `GpuContextVulkan2::submit()` call. Shared by all payloads via `RecordContext`.

### Key Per-Mip Data Structure

```cpp
class GpuResourceStateTrackerVulkan {
private:
    struct TrackedTexture {
        TextureVulkanBase * tex = nullptr;
        
        // Running batch baseline initialized from tex->getState() on first registration
        // Updated in-place by emitPrePassBarriers() whenever a barrier is emitted
        rv::Image::State incoming;
        
        // Per-pass intended states, keyed by packed (mip, face, aspectBit) tuple
        // Cleared by emitPrePassBarriers() between payloads
        std::unordered_map<uint64_t, rv::Image::State::PlaneState> registered;
    };
    
    std::unordered_map<uint64_t, TrackedTexture> mTextures;
};
```

### Mip Level Key Packing
**Lines 30-33 in vk-gpu-resource-state-tracker.cpp**

Each mip/face/aspect combination is packed into a single uint64_t key:

```cpp
// Pack a (mip, face, aspectBit) tuple into a single key for the registered map
inline uint64_t packPlaneKey(uint32_t mip, uint32_t face, vk::ImageAspectFlagBits aspect) {
    return (uint64_t(mip) << 48) | (uint64_t(face) << 16) | uint64_t(uint32_t(aspect));
}
```

This allows the tracker to independently track layout state for every subresource combination.

### Adding Color/Depth-Stencil Targets
**Lines 127-145 in vk-gpu-resource-state-tracker.cpp**

When registering render targets, the tracker sets optimal layouts per aspect:

```cpp
bool GpuResourceStateTrackerVulkan::addColorTarget(TextureVulkanBase * tex, const GpuResourceView & view) {
    if (!tex) GN_UNLIKELY return true;
    rv::Image::State::PlaneState state;
    state.layout = vk::ImageLayout::eColorAttachmentOptimal;
    state.access = vk::AccessFlagBits::eColorAttachmentWrite;
    state.stages = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    state.usage  = "color target";
    return addTexture(tex, view.imageView, state);
}

bool GpuResourceStateTrackerVulkan::addDepthStencilTarget(TextureVulkanBase * tex, 
                                                         const GpuResourceView & view, 
                                                         bool readOnly = false) {
    if (!tex) GN_UNLIKELY return true;
    rv::Image::State::PlaneState state;
    state.layout = readOnly ? vk::ImageLayout::eDepthStencilReadOnlyOptimal 
                            : vk::ImageLayout::eDepthStencilAttachmentOptimal;
    state.access = vk::AccessFlagBits::eDepthStencilAttachmentRead;
    if (!readOnly) state.access |= vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    state.stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
    state.usage  = readOnly ? "depth-stencil target (read-only)" : "depth-stencil target";
    return addTexture(tex, view.imageView, state);
}
```

### Core addTexture Method - Mip Level Iteration
**Lines 46-122 in vk-gpu-resource-state-tracker.cpp**

The `addTexture` method iterates over each mip level and face in the requested subresource range:

```cpp
bool GpuResourceStateTrackerVulkan::addTexture(TextureVulkanBase * tex, 
                                              const GpuResourceView::ImageView & view, 
                                              const rv::Image::State::PlaneState & state) {
    auto & tracked = mTextures[tex->id];
    if (!tracked.tex) {
        // First registration: snapshot incoming state from actual resource
        tracked.tex      = tex;
        tracked.incoming = tex->getState();
        GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image '{}' incoming initialized ({} mips, {} faces)", 
                            tex->name, tracked.incoming.numMips, tracked.incoming.numLayers);
    }

    const auto     resolved = resolveRange(view.range, desc);
    const uint32_t mipEnd   = resolved.i.mip + resolved.e.numMipLevels;
    const uint32_t faceEnd  = resolved.i.face + resolved.e.numArrayLayers;

    // Hazard pass: check every (mip, face, plane) in this binding
    bool hazardFound = false;
    for (uint32_t mip = resolved.i.mip; mip < mipEnd; ++mip) {
        for (uint32_t face = resolved.i.face; face < faceEnd; ++face) {
            forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
                uint64_t key = packPlaneKey(mip, face, bit);
                auto     it  = tracked.registered.find(key);
                if (it == tracked.registered.end()) return;
                
                const auto & existing = it->second;
                // Detect read/write or write/write hazards for same (mip, face, aspect)
                if (!existing.isWrite() && !state.isWrite()) return;
                // ... hazard logging ...
                hazardFound = true;
            });
        }
    }
    if (hazardFound) return false;

    // Register each (mip, face, plane) → intended state
    for (uint32_t mip = resolved.i.mip; mip < mipEnd; ++mip) {
        for (uint32_t face = resolved.i.face; face < faceEnd; ++face) {
            forEachAspectBit(aspects, [&](vk::ImageAspectFlagBits bit) {
                tracked.registered[packPlaneKey(mip, face, bit)] = state;
                GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image '{}' [mip={} face={} {}] registered layout={}", 
                                    tex->name, mip, face, vk::to_string(bit), vk::to_string(state.layout));
            });
        }
    }
    return true;
}
```

**Key points:**
- Resolves the subresource range into concrete mip/face bounds
- Checks each (mip, face, aspect) tuple for hazards against previously registered uses
- Registers the intended layout for each specific mip level independently

---

## 3. Layout Transition: emitPrePassBarriers

**Lines 451-526 in vk-gpu-resource-state-tracker.cpp**

This is where layout transitions are emitted as VkPipelineBarrier commands.

```cpp
void GpuResourceStateTrackerVulkan::emitPrePassBarriers(vk::CommandBuffer cb) {
    DynaArray<vk::ImageMemoryBarrier> barriers;
    vk::PipelineStageFlags srcStages = {}, dstStages = {};

    for (auto & [id, tracked] : mTextures) {
        vk::Image vkImg = tracked.tex->nativeImage();
        if (!vkImg) continue;

        // Iterate only planes the pass registered; untouched planes need no barrier
        for (const auto & [key, next] : tracked.registered) {
            // Unpack (mip, face, aspect) from the registered key
            uint32_t                mip    = uint32_t((key >> 48) & 0xffffu);
            uint32_t                face   = uint32_t((key >> 16) & 0xffffu);
            vk::ImageAspectFlagBits aspect = static_cast<vk::ImageAspectFlagBits>(uint32_t(key & 0xffffu));

            // Get the previous (incoming) state for this specific subresource
            const auto * prev = tracked.incoming.get(mip, face, aspect);
            if (!prev) continue;
            if (*prev == next) continue;  // No state change needed

            GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image barrier '{}' [mip={} face={} {}] : {} -> {} ({})", 
                                tracked.tex->name, mip, face, vk::to_string(aspect),
                                vk::to_string(prev->layout), vk::to_string(next.layout), 
                                next.usage ? next.usage : "?");
            
            // Emit barrier for single subresource: mipLevel=mip, levelCount=1, baseArrayLayer=face, layerCount=1
            vk::ImageMemoryBarrier b;
            b.setOldLayout(prev->layout)
                .setNewLayout(next.layout)
                .setImage(vkImg)
                .setSubresourceRange({aspect, mip, 1, face, 1})  // <-- Single mip, single face
                .setSrcAccessMask(prev->access)
                .setDstAccessMask(next.access)
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
            barriers.append(b);
            srcStages |= prev->stages;
            dstStages |= next.stages;

            // Advance the running incoming state so next payload sees the correct post-barrier layout
            if (mip < tracked.incoming.numMips && face < tracked.incoming.numLayers) {
                auto & sr = tracked.incoming.subresources[tracked.incoming.subresourceIndex(mip, face)];
                auto   it = sr.planes.find(aspect);
                if (it != sr.planes.end()) {
                    it->second = next;
                    GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: image '{}' [mip={} face={} {}] incoming updated to {}", 
                                        tracked.tex->name, mip, face, vk::to_string(aspect), 
                                        vk::to_string(next.layout));
                }
            }
        }

        // Clear registered states for next payload
        tracked.registered.clear();
    }

    if (bufferBarriers.empty() && barriers.empty()) return;

    if (!srcStages) srcStages = vk::PipelineStageFlagBits::eTopOfPipe;
    if (!dstStages) dstStages = vk::PipelineStageFlagBits::eBottomOfPipe;

    // Single vkCmdPipelineBarrier for all subresources
    cb.pipelineBarrier(srcStages, dstStages, {}, nullptr,
                       vk::ArrayProxy<const vk::BufferMemoryBarrier>((uint32_t) bufferBarriers.size(), bufferBarriers.data()),
                       vk::ArrayProxy<const vk::ImageMemoryBarrier>((uint32_t) barriers.size(), barriers.data()));
}
```

**Key Insight:** Each `vk::ImageMemoryBarrier` is constructed with:
```cpp
.setSubresourceRange({aspect, mip, 1, face, 1})
```
Where:
- `baseMipLevel = mip` — specific mip level
- `levelCount = 1` — only this one mip level
- `baseArrayLayer = face` — specific array layer
- `layerCount = 1` — only this one array layer

This allows **independent layout transitions for different mip levels of the same texture**.

---

## 4. Querying Render Pass Attachment Layout: texturePassLayout

**Lines 403-449 in vk-gpu-resource-state-tracker.cpp**

This method queries the post-barrier layout for a specific (mip, face) pair of a tracked texture:

```cpp
vk::ImageLayout GpuResourceStateTrackerVulkan::texturePassLayout(const TextureVulkanBase * tex, 
                                                                  uint32_t mip, 
                                                                  uint32_t face) const {
    auto it = mTextures.find(tex->id);
    if (it == mTextures.end()) {
        GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: texturePassLayout('{}' mip={} face={}) -> eUndefined (not tracked)", 
                            tex->name, mip, face);
        return vk::ImageLayout::eUndefined;
    }
    const auto & tracked = it->second;

    // Query incoming (post-barrier authoritative state)
    // Note: registered is cleared by emitPrePassBarriers before this is ever called
    vk::ImageLayout colorLayout   = vk::ImageLayout::eUndefined;
    vk::ImageLayout depthLayout   = vk::ImageLayout::eUndefined;
    vk::ImageLayout stencilLayout = vk::ImageLayout::eUndefined;
    
    forEachAspectBit(tracked.incoming.validAspects, [&](vk::ImageAspectFlagBits bit) {
        // Get state for specific (mip, face, aspect)
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
        // Combine depth and stencil layouts (may require VK_KHR_separate_depth_stencil_layouts)
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

    GN_VERBOSE(sLogger)("GpuResourceStateTrackerVulkan: texturePassLayout('{}' mip={} face={}) -> {} ({})", 
                        tex->name, mip, face, vk::to_string(result), reason);
    return result;
}
```

---

## 5. Usage in Render Pass Recording

**File:** [src/core/gpu2/vk-gpu-raster.cpp](src/core/gpu2/vk-gpu-raster.cpp) - Lines 130-240

The tracker is used to determine which layout to put render target attachments in:

```cpp
bool GpuRasterPayloadVulkan::buildAndBeginRendering(vk::CommandBuffer vkcb, 
                                                    GpuResourceStateTrackerVulkan & tracker, 
                                                    vk::Extent2D & outExt,
                                                    PassFormats & outFormats) {
    // ... color attachments ...
    for (size_t i = 0; i < mRenderTarget.colorTargets.size(); ++i) {
        const GpuResourceView ctView = mRenderTarget.colorTargets[i].view();
        // ...
        
        // Verify the pre-pass barrier landed the attachment in the expected layout
        // Query layout for specific (mip, face) of this color attachment
        GN_ASSERT(tracker.texturePassLayout(
            RuntimeType::cast<TextureVulkanBase>(ctView.texture().get()), 
            ctView.imageView.range.i.mip,     // <-- Mip level from view
            ctView.imageView.range.i.face)    // <-- Face from view
            == vk::ImageLayout::eColorAttachmentOptimal);

        vk::RenderingAttachmentInfo att;
        att.setImageView(view)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setClearValue(vk::ClearValue(clearCv));
        colorAtts.push_back(att);
    }

    // ... depth-stencil attachment ...
    const GpuResourceView dst = mRenderTarget.depthStencilTarget.view();
    if (dst.isTexture() && dst.texture()) {
        auto *        depthTex  = RuntimeType::cast<TextureVulkanBase>(dst.texture().get());
        vk::ImageView depthView = depthTex ? depthTex->nativeView(dst.imageView) : vk::ImageView {};
        if (depthTex && depthView) {
            // Query layout for specific (mip, face) of this depth attachment
            vk::ImageLayout depthLayout = tracker.texturePassLayout(
                depthTex, 
                dst.imageView.range.i.mip,    // <-- Mip level from view
                dst.imageView.range.i.face);  // <-- Face from view
            
            depthAtt.setImageView(depthView)
                .setImageLayout(depthLayout)
                .setLoadOp(vk::AttachmentLoadOp::eClear)
                .setStoreOp(vk::AttachmentStoreOp::eStore)
                // ...
        }
    }
    
    vk::RenderingInfo ri;
    ri.setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), outExt))
        .setLayerCount(1)
        .setColorAttachments(colorAtts);
    if (hasDepth) ri.setPDepthAttachment(&depthAtt);

    vkcb.beginRendering(ri);
    return true;
}
```

---

## 6. Texture Native View Creation with Mip Levels

**File:** [src/core/gpu2/vk-texture.h](src/core/gpu2/vk-texture.h) - Lines 20-37

```cpp
vk::ImageView nativeView(const GpuResourceView::ImageView & v) const {
    if (!mRvImage) GN_UNLIKELY return vk::ImageView {};
    
    const auto                   viewFmt = (v.format != gfx::img::PixelFormat::UNKNOWN()) ? v.format : mDescriptor.format;
    const auto                   aspect  = aspectFromViewFormat(viewFmt, mDescriptor.format);
    
    rv::Image::GetViewParameters vp;
    vp.type  = getTypeFromSubresourceRange(v.range);  // 2D, 2D Array, etc. based on array layer count
    vp.range = toVkSubresourceRange(aspect, v.range); // Converts to VkImageSubresourceRange
    
    // For depth-stencil, view format must match image format
    const bool isDs = (aspect & (vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil)) != vk::ImageAspectFlags {};
    vp.format       = pixelFormatToVkFormat(isDs ? mDescriptor.format : viewFmt);
    
    return mRvImage->getView(vp);
}

// Helper: convert subresource range to Vulkan format
static inline vk::ImageSubresourceRange toVkSubresourceRange(vk::ImageAspectFlags aspect, 
                                                             const GpuResourceView::SubresourceRange & r) {
    return vk::ImageSubresourceRange(
        aspect,                 // aspectMask
        r.i.mip,                // baseMipLevel <-- Starting mip
        r.e.numMipLevels,       // levelCount   <-- Number of mips
        r.i.face,               // baseArrayLayer
        r.e.numArrayLayers);    // layerCount
}
```

---

## 7. Render Target Initialization Workflow

### Step 1: Create RenderTarget with Mip Levels
```cpp
gpu2::RasterTarget rt;
rt.setColorTarget(0, gpuView.setSubresourceIndex({.mip = 3, .face = 0})
                            .setSubresourceExtent({.numMipLevels = 1, .numArrayLayers = 1}));
```

### Step 2: Register with State Tracker
When `GpuRaster::draw()` is called, the payload's `collectPassResources()` registers the render target:
```cpp
bool GpuRasterPayloadVulkan::collectPassResources(GpuResourceStateTrackerVulkan & tracker) {
    if (!tracker.addRasterTarget(mRenderTarget)) return false;
    // ...
}
```

Which calls (lines 323-350 in tracker):
```cpp
bool GpuResourceStateTrackerVulkan::addRasterTarget(const RasterTarget & rt) {
    // Register each color target, querying its (mip, face) from GenericTarget
    for (size_t i = 0; i < rt.colorTargets.size(); ++i) {
        // ...
        if (tex && !addColorTarget(tex, view)) { /* hazard */ }
    }
    // Register depth-stencil target with same mip/face awareness
    if (rt.depthStencilTarget.texture) {
        // ...
    }
    return true;
}
```

### Step 3: Emit Layout Transitions
```cpp
tracker.emitPrePassBarriers(vkcb);
```

This emits separate `VkPipelineBarrier` commands for each (mip, face, aspect) combination that needs a layout transition.

### Step 4: Begin Rendering
```cpp
tracker.buildAndBeginRendering(vkcb, tracker, extent, formats);
```

Which calls `texturePassLayout(tex, mip, face)` for each attachment to get its layout after barriers.

### Step 5: Finalize State
After GPU submission:
```cpp
tracker.flushToResources();
```

This writes the final batch-end layout back to each texture:
```cpp
void GpuResourceStateTrackerVulkan::flushToResources() {
    for (const auto & [id, tracked] : mTextures) {
        if (!tracked.tex) continue;
        for (uint32_t mip = 0; mip < tracked.incoming.numMips; ++mip) {
            for (uint32_t face = 0; face < tracked.incoming.numLayers; ++face) {
                forEachAspectBit(tracked.incoming.validAspects, 
                    [t = tracked, m = mip, f = face](vk::ImageAspectFlagBits aspect) {
                    const auto * ps = t.incoming.get(m, f, aspect);
                    if (!ps) return;
                    // Write per-subresource state back to texture
                    t.tex->setState(*ps, vk::ImageSubresourceRange(aspect, m, 1, f, 1));
                });
            }
        }
    }
}
```

---

## 8. Key Patterns and Design

| Aspect | Implementation |
|--------|-----------------|
| **Per-Mip Tracking** | Each (mip, face, aspect) has independent layout state in `rv::Image::State` |
| **Key Scheme** | 64-bit packed key: `(mip<<48) \| (face<<16) \| aspect` |
| **Barrier Emission** | Each subresource gets its own `VkImageMemoryBarrier` with `levelCount=1, layerCount=1` |
| **Hazard Detection** | Read/write and write/write hazards detected per (mip, face, aspect) tuple |
| **Incoming State** | Snapshot of texture state at batch start, updated after each payload's barriers |
| **Registered State** | Per-payload intended states, cleared after barriers are emitted |
| **View Creation** | `VkImageView` created from `SubresourceRange` with mip/face bounds |

---

## 9. Summary: How Mip Levels Work

1. **Declaration**: `GenericTarget` and `ColorTarget` specify `mip` and `face` fields (each 16-bit)
2. **View Specification**: `GpuResourceView::SubresourceRange` encodes starting mip/face and count
3. **Tracking**: `GpuResourceStateTrackerVulkan` maintains per-(mip, face, aspect) layout state
4. **Transitions**: `emitPrePassBarriers()` emits individual barriers for each subresource
5. **Barriers**: Each `VkImageMemoryBarrier` targets a single mip with `levelCount=1`
6. **Query**: `texturePassLayout(tex, mip, face)` returns the post-barrier layout for that subresource
7. **Finalization**: `flushToResources()` writes final states back to textures for next batch

This design allows **per-mip-level rendering** without texture copy overhead — different mip levels of the same texture can have independent layouts and be render targets in the same or different passes.

