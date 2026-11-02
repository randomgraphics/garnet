# Assignment A92: Migrate synchronous resource setContent helpers

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A92
- Role: Implementer / rework
- Status: implementation complete; fresh core and affected test/sample translation-unit builds pass; linked runtime checks pending
- Requirement revision: 0.2 user-confirmed
- Owner: `/root`
- Dependencies: A90 Ninja build exposed stale upload payload helpers; A10 bindless CnC upload API

## Goal

Complete the bindless migration for `BufferVulkan::setContent` and
`TextureVulkanBase::setContent`, which the independent source audit missed because
the stale helpers had no remaining declarations/definitions and the default
Windows build had not reached C++ compilation.

## Change

Both synchronous helpers now create heap-free `bindless::CnC`, record the
appropriate buffer/image upload, seal and submit the payload, then wait for GPU
completion as before. This preserves blocking one-shot initialization semantics
and lets the bindless transfer tracker restore resource states.

## Progress and verification

- A90 first Ninja compile failed in `vk-buffer.cpp` because
  `createCncBufferUploadPayload` was undefined. Inspection found the same stale
  deleted helper in `vk-texture.cpp`; both were migrated in this repair.
- The next fresh compile exposed two migrated test-source issues: C++20
  designated initializers did not follow `ComputeParameters` declaration order,
  and a Catch assertion compared `void *` against a byte. Both were repaired and
  the affected test objects recompiled successfully.
- Mesh-viewer caller compilation exposed that `scene-renderer.h` used `glm::quat`
  without directly including its definition. Added `<glm/gtc/quaternion.hpp>`;
  both changed mesh-viewer translation units compile afterward.
- Fresh Ninja Debug `GNcore-objects` passes. The selected changed GPU2/FX2 test
  and sample/tool translation units compile. Full test executable linking is
  still blocked in third-party Assimp by its `/Zi` override causing MSVC C1902;
  A90 owns follow-up toolchain/runtime verification.
- Verify: fresh Ninja `GNcore-objects` and selected changed test/sample objects
  passed; focused GPU2 CnC/image runtime tests remain pending A90. Formatter and
  diff checks must be rerun after this repair.
- Commit: `fix(gpu2): route resource setContent through bindless cnc`
