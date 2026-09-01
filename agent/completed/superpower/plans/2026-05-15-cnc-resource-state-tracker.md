# CnC Resource State Tracker Integration Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace `GpuCncPayloadVulkan`'s private per-operation barrier helpers with the shared `GpuResourceStateTrackerVulkan`, ensuring compute and copy operations participate in the same cross-payload barrier graph as raster.

**Architecture:** Each stored CNC operation becomes its own "pass" from the tracker's perspective: register its resources via `add*()`, call `emitPrePassBarriers()`, then execute — exactly the same three-step rhythm raster uses per render pass. This correctly handles write→read dependencies between consecutive CNC ops in the same payload (e.g., a compute that writes buffer A followed by a copy that reads it). New transfer-specific `add*` methods are added to the tracker to cover `TRANSFER_SRC/DST` resource roles; existing `addSampledTexture`/`addStorageTexture`/`addStorageBuffer`/`addUniformBuffer` are reused for compute but called with `eComputeShader` pipeline stage flags. Because the batch tracker's `flushToResources()` in `GpuContextVulkan2::submit()` already writes final states back to every tracked resource, `onSubmitComplete` in `GpuCncPayloadVulkan` can be removed entirely.

**Tech Stack:** C++17, Vulkan-Hpp (`vk::`), rapid-vulkan (`rv::`), Catch2 for tests.

---

## File Map

| File | Change |
|---|---|
| `src/core/gpu2/vk-gpu-resource-state-tracker.h` | Add 3 new public `add*` method declarations |
| `src/core/gpu2/vk-gpu-resource-state-tracker.cpp` | Implement `addTransferSrcBuffer`, `addTransferDstBuffer`, `addTransferDstImage` |
| `src/core/gpu2/vk-gpu-cnc.cpp` | Remove private tracking state; rewrite `recordCompute`, `recordBufToBuf`, `recordBufToImg`, `recordForVulkanSubmit`; remove `onSubmitComplete` |

---

## Task 1: Add transfer resource-registration methods to `GpuResourceStateTrackerVulkan`

**Files:**
- Modify: `src/core/gpu2/vk-gpu-resource-state-tracker.h`
- Modify: `src/core/gpu2/vk-gpu-resource-state-tracker.cpp`

- [ ] **Step 1: Declare three new methods in the header**

  In `vk-gpu-resource-state-tracker.h`, after `addIndexBuffer` (line 39) and before `addGpuResourceTable` (line 41), add:

  ```cpp
  bool addTransferSrcBuffer(BufferVulkan * buf);
  bool addTransferDstBuffer(BufferVulkan * buf);
  bool addTransferDstImage(TextureVulkanBase * tex, const GpuResourceView::ImageView & view);
  ```

  Result — the buffer/texture method block should look like:

  ```cpp
  bool addUniformBuffer(BufferVulkan *         buf,
                        vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader);
  bool addStorageBuffer(BufferVulkan * buf, bool write = false, vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eFragmentShader);
  bool addVertexBuffer(BufferVulkan * buf);
  bool addIndexBuffer(BufferVulkan * buf);

  bool addTransferSrcBuffer(BufferVulkan * buf);
  bool addTransferDstBuffer(BufferVulkan * buf);
  bool addTransferDstImage(TextureVulkanBase * tex, const GpuResourceView::ImageView & view);

  std::vector<uint64_t> addGpuResourceTable(const GpuResourceTable & table);
  ```

- [ ] **Step 2: Implement the three methods in `vk-gpu-resource-state-tracker.cpp`**

  Add these three functions after `addIndexBuffer` (around line 250, before `addGpuResourceTable`):

  ```cpp
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
  ```

- [ ] **Step 3: Build to confirm no compile errors**

  ```bash
  python3 env/bin/build.py d
  ```

  Expected: build succeeds with no errors or warnings (warnings-as-errors is on).

- [ ] **Step 4: Commit**

  ```bash
  git add src/core/gpu2/vk-gpu-resource-state-tracker.h src/core/gpu2/vk-gpu-resource-state-tracker.cpp
  git commit -m "feat/gpu2: add transfer add* methods to GpuResourceStateTrackerVulkan"
  ```

---

## Task 2: Rewrite `GpuCncPayloadVulkan` to use the batch tracker

**Files:**
- Modify: `src/core/gpu2/vk-gpu-cnc.cpp`

The changes eliminate the private tracking state (`TrackedBuf`, `TrackedTex`, `mBufs`, `mTexs`, `getBuf`, `getTex`, `transitionBuf`, `transitionTex`) and `onSubmitComplete`, then rewrite the recording helpers. The full replacement for `vk-gpu-cnc.cpp` is shown below — replace the **entire content** of the file:

- [ ] **Step 1: Replace `vk-gpu-cnc.cpp` with the new version**

  Write the entire file:

  ```cpp
  // Must be included first so rapid-vulkan defines/includes happen before any other vk-*.h
  #include "vk-gpu-cnc.h"
  #include "vk-gpu-context.h"
  #include "vk-gpu-payload.h"
  #include "vk-gpu-shader.h"
  #include "vk-gpu-resource-state-tracker.h"
  #include "vk-buffer.h"
  #include "vk-texture.h"
  #include "vk-format-utils.h"
  #include "gpu-context.h"

  #include <variant>
  #include <vector>

  static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

  namespace GN::gpu2 {

  namespace {

  // ── Stored operation records (built during compute/copy calls, consumed by seal/record) ──

  struct StoredCompute {
      AutoRef<GpuShader> cs;
      GpuResourceTable   resources;
      DynaArray<uint8_t> immediates;
      uint32_t           x = 1, y = 1, z = 1;
  };

  struct StoredBufferToBuffer {
      AutoRef<Buffer> src;
      AutoRef<Buffer> dst;
      uint64_t        srcOffset = 0;
      uint64_t        dstOffset = 0;
      uint64_t        size      = 0;
  };

  struct StoredBufferToImage {
      AutoRef<Buffer>           src;
      AutoRef<Texture>          dst;
      DynaArray<GpuCnC::Region> regions;
  };

  using StoredOp = std::variant<StoredCompute, StoredBufferToBuffer, StoredBufferToImage>;

  // ── GpuCncPayloadVulkan ──────────────────────────────────────────────────────────────

  class GpuCncPayloadVulkan final : public GpuPayloadVulkan {
  public:
      GpuCncPayloadVulkan(const StrA & name, std::vector<StoredOp> ops): GpuPayloadVulkan(name), mOps(std::move(ops)) {}

      void recordForVulkanSubmit(const RecordContext & ctx) override;

  private:
      std::vector<StoredOp> mOps;

      void recordCompute(const StoredCompute & op, const RecordContext & ctx);
      void recordBufToBuf(const StoredBufferToBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker);
      void recordBufToImg(const StoredBufferToImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker);
  };

  // ── Compute dispatch ─────────────────────────────────────────────────────────────────

  static rv::Sampler * ensureDefaultSampler(const rv::Device * dev, rv::Ref<rv::Sampler> & slot) {
      if (slot.valid()) return slot.get();
      rv::Sampler::ConstructParameters scp;
      scp.gi = dev->gi();
      scp.setLinear();
      slot = rv::Ref<rv::Sampler>::make(scp);
      return slot.get();
  }

  void GpuCncPayloadVulkan::recordCompute(const StoredCompute & op, const RecordContext & ctx) {
      auto * csVk = RuntimeType::cast<GpuShaderVulkan>(op.cs.get());
      if (!csVk || !csVk->rvShader()) GN_UNLIKELY {
              GN_ERROR(sLogger)("GpuCncPayloadVulkan: compute requires a valid compute shader");
              return;
          }

      vk::CommandBuffer                vkcb    = ctx.cmd.handle();
      GpuResourceStateTrackerVulkan &  tracker = *ctx.batchTracker;

      // Register all shader resources with compute-stage pipeline flags, then emit a single
      // pre-dispatch barrier. Using one batch barrier (rather than per-resource barriers) keeps
      // the pattern consistent with how raster passes work via the same tracker.
      for (size_t setIdx = 0; setIdx < op.resources.size(); ++setIdx) {
          const auto & set = op.resources[setIdx];
          for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
              const auto & slot = set[bindIdx];
              for (const auto & view : slot) {
                  if (view.empty()) continue;
                  if (view.isTexture()) {
                      auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                      if (!tex) continue;
                      if (view.imageView.type == GpuResourceView::ImageView::STORAGE)
                          tracker.addStorageTexture(tex, view, vk::PipelineStageFlagBits::eComputeShader);
                      else
                          tracker.addSampledTexture(tex, view, vk::PipelineStageFlagBits::eComputeShader);
                  } else if (view.isBuffer()) {
                      auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                      if (!buf) continue;
                      if (view.bufferView.type == GpuResourceView::BufferView::STORAGE)
                          // Treat storage buffers as read-write; the shader may write without
                          // declaring it in the resource table, so be conservative.
                          tracker.addStorageBuffer(buf, /*write=*/true, vk::PipelineStageFlagBits::eComputeShader);
                      else
                          tracker.addUniformBuffer(buf, vk::PipelineStageFlagBits::eComputeShader);
                  }
              }
          }
      }
      tracker.emitPrePassBarriers(vkcb);

      // Build compute pipeline. No PSO cache yet; create fresh per-dispatch.
      // TODO: add a compute PSO factory (keyed on shader ID) if per-frame dispatch overhead shows up in profiles.
      rv::ComputePipeline::ConstructParameters ccp;
      ccp.name = std::string(name.c_str()) + "/compute_pso";
      ccp.cs   = csVk->rvShader();
      rv::Ref<rv::ComputePipeline> pipeline(new rv::ComputePipeline(ccp));
      if (!pipeline->handle()) GN_UNLIKELY {
              GN_ERROR(sLogger)("GpuCncPayloadVulkan: failed to create compute pipeline");
              return;
          }

      // Build drawable for descriptor binding and push constants.
      rv::Drawable::ConstructParameters dcp;
      dcp.setPipeline(pipeline);
      rv::Drawable drawable(dcp);

      if (!op.immediates.empty()) {
          if (op.immediates.size() > 128) GN_UNLIKELY {
                  GN_ERROR(sLogger)("GpuCncPayloadVulkan: immediates size {} exceeds 128 bytes", op.immediates.size());
              }
          else { drawable.c(0, op.immediates.size(), op.immediates.data(), vk::ShaderStageFlagBits::eCompute); }
      }

      rv::Ref<rv::Sampler> defaultSampler;
      for (size_t setIdx = 0; setIdx < op.resources.size(); ++setIdx) {
          const auto & set = op.resources[setIdx];
          for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
              const auto & slot = set[bindIdx];
              if (slot.empty()) continue;
              rv::DescriptorIdentifier descId((uint32_t) setIdx, (uint32_t) bindIdx);
              if (slot[0].isTexture()) {
                  std::vector<rv::ImageSampler> imgs;
                  imgs.reserve(slot.size());
                  for (const auto & view : slot) {
                      if (view.empty() || !view.isTexture()) continue;
                      auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                      if (!tex) continue;
                      vk::ImageLayout layout =
                          (view.imageView.type == GpuResourceView::ImageView::STORAGE) ? vk::ImageLayout::eGeneral : vk::ImageLayout::eShaderReadOnlyOptimal;
                      rv::ImageSampler is;
                      is.view    = tex->nativeView(view.imageView);
                      is.layout  = layout;
                      is.sampler = rv::Ref<const rv::Sampler>(ensureDefaultSampler(ctx.dev, defaultSampler));
                      imgs.push_back(is);
                  }
                  if (!imgs.empty()) drawable.t(descId, vk::ArrayProxy<const rv::ImageSampler>((uint32_t) imgs.size(), imgs.data()));
              } else if (slot[0].isBuffer()) {
                  std::vector<rv::BufferView> bufs;
                  bufs.reserve(slot.size());
                  for (const auto & view : slot) {
                      if (view.empty() || !view.isBuffer()) continue;
                      auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                      if (!buf) continue;
                      rv::BufferView bv;
                      bv.buffer = buf->rvBuffer();
                      bv.offset = (vk::DeviceSize) view.bufferView.offset;
                      bv.size   = view.bufferView.size ? (vk::DeviceSize) view.bufferView.size : vk::DeviceSize(-1);
                      bufs.push_back(bv);
                  }
                  if (!bufs.empty()) drawable.b(descId, vk::ArrayProxy<const rv::BufferView>((uint32_t) bufs.size(), bufs.data()));
              }
          }
      }

      drawable.dispatch(rv::ComputePipeline::DispatchParameters {.width = op.x, .height = op.y, .depth = op.z});

      rv::Ref<const rv::DrawPack> pack = drawable.compile();
      if (!pack || pack->empty()) GN_UNLIKELY {
              GN_ERROR(sLogger)("GpuCncPayloadVulkan: Drawable::compile produced empty DrawPack for compute dispatch");
              return;
          }
      ctx.cmd.render(pack);
  }

  // ── Buffer-to-buffer copy ────────────────────────────────────────────────────────────

  void GpuCncPayloadVulkan::recordBufToBuf(const StoredBufferToBuffer & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
      if (op.size == 0) return;

      auto * srcVk = RuntimeType::cast<BufferVulkan>(op.src.get());
      auto * dstVk = RuntimeType::cast<BufferVulkan>(op.dst.get());
      if (!srcVk || !dstVk) GN_UNLIKELY {
              GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToBuffer: null Vulkan buffer");
              return;
          }
      if (srcVk == dstVk) GN_UNLIKELY {
              GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToBuffer: src and dst are the same buffer");
              return;
          }

      vk::Buffer srcBuf = srcVk->nativeBuffer();
      vk::Buffer dstBuf = dstVk->nativeBuffer();
      if (!srcBuf || !dstBuf) GN_UNLIKELY {
              GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToBuffer: buffer missing Vulkan handle");
              return;
          }

      tracker.addTransferSrcBuffer(srcVk);
      tracker.addTransferDstBuffer(dstVk);
      tracker.emitPrePassBarriers(cb);

      cb.copyBuffer(srcBuf, dstBuf, vk::BufferCopy(op.srcOffset, op.dstOffset, op.size));
  }

  // ── Buffer-to-image copy ─────────────────────────────────────────────────────────────

  void GpuCncPayloadVulkan::recordBufToImg(const StoredBufferToImage & op, vk::CommandBuffer cb, GpuResourceStateTrackerVulkan & tracker) {
      if (op.regions.empty()) return;

      auto * srcVk = RuntimeType::cast<BufferVulkan>(op.src.get());
      auto * dstVk = RuntimeType::cast<TextureVulkanBase>(op.dst.get());
      if (!srcVk || !dstVk) GN_UNLIKELY {
              GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToImage: null Vulkan resource");
              return;
          }

      vk::Buffer srcBuf = srcVk->nativeBuffer();
      vk::Image  dstImg = dstVk->nativeImage();
      if (!srcBuf || !dstImg) GN_UNLIKELY {
              GN_ERROR(sLogger)("GpuCncPayloadVulkan: copyBufferToImage: resource has no Vulkan handle");
              return;
          }

      tracker.addTransferSrcBuffer(srcVk);
      // Default ImageView covers all mips and all array layers (SubresourceRange defaults).
      GpuResourceView::ImageView fullRange;
      tracker.addTransferDstImage(dstVk, fullRange);
      tracker.emitPrePassBarriers(cb);

      // Derive the aspect flags from the destination texture's format.
      const auto & desc    = dstVk->descriptor();
      auto         aspects = aspectFromViewFormat(desc.format, desc.format);
      if (!aspects) aspects = vk::ImageAspectFlagBits::eColor;

      std::vector<vk::BufferImageCopy> copies;
      copies.reserve(op.regions.size());
      for (const auto & r : op.regions) {
          vk::BufferImageCopy c;
          c.setBufferOffset(r.bufferOffset)
              .setBufferRowLength(r.bufferRowLength)
              .setBufferImageHeight(r.bufferHeight)
              .setImageSubresource({aspects, r.imageSubresource.mip, r.imageSubresource.face, 1})
              .setImageOffset({(int32_t) r.imageOffset.x, (int32_t) r.imageOffset.y, (int32_t) r.imageOffset.z})
              .setImageExtent({r.imageExtent.x, r.imageExtent.y, r.imageExtent.z});
          copies.push_back(c);
      }

      cb.copyBufferToImage(srcBuf, dstImg, vk::ImageLayout::eTransferDstOptimal, (uint32_t) copies.size(), copies.data());
  }

  // ── recordForVulkanSubmit ────────────────────────────────────────────────────────────

  void GpuCncPayloadVulkan::recordForVulkanSubmit(const RecordContext & ctx) {
      if (!ctx.dev || ctx.cmd.empty() || !ctx.batchTracker) return;
      GpuResourceStateTrackerVulkan & tracker = *ctx.batchTracker;
      vk::CommandBuffer               vkcb    = ctx.cmd.handle();

      for (const auto & op : mOps) {
          std::visit(
              [&](const auto & o) {
                  using T = std::decay_t<decltype(o)>;
                  if constexpr (std::is_same_v<T, StoredCompute>)
                      recordCompute(o, ctx);
                  else if constexpr (std::is_same_v<T, StoredBufferToBuffer>)
                      recordBufToBuf(o, vkcb, tracker);
                  else if constexpr (std::is_same_v<T, StoredBufferToImage>)
                      recordBufToImg(o, vkcb, tracker);
              },
              op);
      }
  }

  // ── GpuCncVulkan2 ────────────────────────────────────────────────────────────────────

  class GpuCncVulkan2 final : public GpuCnC {
  public:
      GN_REGISTER_RUNTIME_TYPE(GpuCnC);

      GpuCncVulkan2(const StrA & entityName, const CreateParameters & cp): GpuCnC(TYPE_INFO(), entityName), mCreateParams(cp) {}

      void compute(const ComputeParameters & cp) override {
          if (mSealed) GN_UNLIKELY {
                  GN_ERROR(sLogger)("GpuCncVulkan2::compute: already sealed");
                  return;
              }
          StoredCompute op;
          op.cs        = cp.cs;
          op.resources = cp.resources;
          op.x         = cp.x;
          op.y         = cp.y;
          op.z         = cp.z;
          if (cp.immediates.data() && cp.immediates.size() > 0) op.immediates.append(cp.immediates.data(), (size_t) cp.immediates.size());
          mOps.emplace_back(std::move(op));
      }

      void copyBufferToBuffer(const BufferToBuffer & p) override {
          if (mSealed) GN_UNLIKELY {
                  GN_ERROR(sLogger)("GpuCncVulkan2::copyBufferToBuffer: already sealed");
                  return;
              }
          mOps.emplace_back(StoredBufferToBuffer {p.src, p.dst, p.srcOffset, p.dstOffset, p.size});
      }

      void copyBufferToImage(const BufferToImage & p) override {
          if (mSealed) GN_UNLIKELY {
                  GN_ERROR(sLogger)("GpuCncVulkan2::copyBufferToImage: already sealed");
                  return;
              }
          StoredBufferToImage op;
          op.src = p.src;
          op.dst = p.dst;
          for (const auto & r : p.regions) op.regions.append(r);
          mOps.emplace_back(std::move(op));
      }

      AutoRef<GpuPayload> seal() override {
          if (mSealed) GN_UNLIKELY {
                  GN_ERROR(sLogger)("GpuCncVulkan2::seal: double seal");
                  return {};
              }
          mSealed = true;
          return AutoRef<GpuPayload>(new GpuCncPayloadVulkan(name + "/payload", std::move(mOps)));
      }

  private:
      CreateParameters      mCreateParams;
      bool                  mSealed = false;
      std::vector<StoredOp> mOps;
  };

  } // anonymous namespace

  // ── Factory ──────────────────────────────────────────────────────────────────────────

  AutoRef<GpuCnC> createGpuCncVulkan2(const GpuCnC::CreateParameters & params) {
      if (!params.gpu) return {};
      auto vkGpu = params.gpu.staticCastTo<GpuContextVulkan2>();
      if (!vkGpu || !vkGpu->ready()) return {};
      StrA n = params.gpu->name.empty() ? StrA("cnc") : params.gpu->name + "/cnc";
      return AutoRef<GpuCnC>(new GpuCncVulkan2(n, params));
  }

  } // namespace GN::gpu2
  ```

- [ ] **Step 2: Build and confirm no compile errors**

  ```bash
  python3 env/bin/build.py d
  ```

  Expected: clean build, no errors or warnings.

- [ ] **Step 3: Run the CnC unit tests**

  ```bash
  python3 env/bin/cit.py -i
  ```

  Expected: all `GPU2/CnC` tests pass (copyBufferToBuffer, copyBufferToBuffer with offsets, copyBufferToImage, compute with null shader, create/seal/double-seal). Tests that require GPU will be skipped if no GPU is available in the test environment.

- [ ] **Step 4: Commit**

  ```bash
  git add src/core/gpu2/vk-gpu-cnc.cpp
  git commit -m "refactor/gpu2: CnC payload now uses GpuResourceStateTrackerVulkan for barriers"
  ```

---

## Self-Review

**Spec coverage:**
- Transfer `add*` methods: covered in Task 1.
- CNC private tracking removed: covered in Task 2 (no `TrackedBuf`/`TrackedTex`/`mBufs`/`mTexs`/`getBuf`/`getTex`/`transitionBuf`/`transitionTex`).
- `onSubmitComplete` removed: covered in Task 2 (no override in new class; `flushToResources()` in context handles it).
- Each CNC op is its own tracker pass: covered — each `record*` helper calls `add*()` then `emitPrePassBarriers()` then executes.
- Compute uses `eComputeShader` stages: covered in `recordCompute`.
- Storage buffers in compute treated as read-write: covered with `write=true`.
- Buffer-to-image default full-range view: covered with default `GpuResourceView::ImageView fullRange`.

**Placeholder scan:** No TBDs or TODOs (the PSO cache TODO was already in the original code; preserved as-is).

**Type consistency:**
- `addTransferSrcBuffer`, `addTransferDstBuffer`, `addTransferDstImage` — declared in header, used in `recordBufToBuf` and `recordBufToImg` with matching names.
- `recordBufToBuf(op, cb, tracker)` and `recordBufToImg(op, cb, tracker)` — declaration and definition signatures match.
- `recordCompute(op, ctx)` — unchanged signature; tracker reached via `ctx.batchTracker`.
