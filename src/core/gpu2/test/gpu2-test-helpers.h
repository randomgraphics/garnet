#pragma once

// Shared test helpers for gpu2 GPU tests. All helpers are inline to avoid ODR issues
// when included by multiple translation units in the same test binary.

#include <garnet/GNgpu2.h>
#include <atomic>

namespace GN::gpu2::test {

inline AutoRef<GpuContext> makeGpu() {
    return GpuContext::create("gpu", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE, .debug = GpuContext::DebugMode::ENABLED});
}

inline AutoRef<Texture> makeRgba8Tex(const AutoRef<GpuContext> & gpu, const char * name, uint32_t w, uint32_t h) {
    // Vulkan render-attachment image views require levelCount=1; single mip also avoids
    // mip-chain allocation for these small test textures.
    return Texture::create(name,
                           {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(w, h).setLevels(1)});
}

inline AutoRef<GpuShader> makeShader(const AutoRef<GpuContext> & gpu, const char * name, const void * binary, size_t byteSize) {
    return GpuShader::create({.context = gpu, .name = name, .binary = binary, .size = byteSize});
}

template<typename... Payloads>
inline void submitAndWait(const AutoRef<GpuContext> & gpu, const char * label, Payloads &&... payloads) {
    std::atomic<bool> done = false;
    auto              sp   = GpuContext::SubmitParameters(label);
    (sp.appendWork(payloads), ...);
    sp.setOnComplete([&done] { done = true; });
    gpu->submit(sp);
    while (!done) gpu->pump();
}

} // namespace GN::gpu2::test

using namespace GN::gpu2::test; // bring helpers into scope for test files
