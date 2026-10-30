---
name: garnet-vulkan-verification
description: Verification requirements for Vulkan-based applications and tests. Use when running, testing, or verifying any Vulkan/gpu2 sample, test, or engine application.
---

# Vulkan Application Verification

When verifying any Vulkan-based application, tool, or test in Garnet:

## Core Requirement

The verification process **must** run the application with the Vulkan validation layers enabled (`VK_LAYER_KHRONOS_validation`) and ensure that the entire execution is **completely error- and warning-free**.

## Invariants

1. **Validation Layers Enabled**:
   - Debug builds of Garnet enable validation layers by default (`DebugMode::ENABLED` in `GpuContext::CreateParameters`).
   - If running in non-debug configurations or custom test harnesses, ensure validation layers are explicitly active (e.g., via `DebugMode::ENABLED` or `VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation`).

2. **Zero Errors and Zero Warnings**:
   - The entire run must complete with zero Vulkan validation layer errors, warnings, performance warnings, or object lifetime leaks.
   - Any validation VUID violation (e.g., invalid layout transitions, missing image usage flags, descriptor binding mismatches, synchronization hazards) constitutes a verification failure and must be resolved before committing or signing off.
   - Inspect console output (`stdout`, `stderr`) and logger output for any `[ERROR]`, `[WARN]`, or `Validation :` diagnostic messages.

3. **Exception: Negative Testing**:
   - Deliberate negative tests (tests designed specifically to verify that invalid inputs, error codes, or boundary conditions are properly trapped and handled) are the **only** permitted exception.
   - Negative tests must clearly document the expected error condition, isolate the error to the specific tested scope, and verify the intended failure mode without causing unintended state corruption.

4. **Interactive and Offscreen Execution**:
   - Samples and applications that support both offscreen/headless smoke-test mode (e.g., `t` argument) and interactive windowed mode should be verified in both paths where appropriate.
   - Pay particular attention to swapchain presentation, acquire/present semaphore synchronization, and backbuffer image layout transitions (`COLOR_ATTACHMENT_OPTIMAL` -> `PRESENT_SRC_KHR`), which only occur during windowed presentation.
