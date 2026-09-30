#pragma once

#include <garnet/GNfx2.h>
#include <array>

namespace GN::fx2 {

/// Source container recognized by the importer.
enum class ModelSourceFormat : uint8_t {
    UNKNOWN,
    FBX,
    GLTF,
    GLB,
    STL,
    ASE,
};

// Importer dispatch helper, also used by module-local format classification tests.
ModelSourceFormat classifyModelSourcePath(const StrA & path);

struct ModelAssetImpl final : ModelAsset {
    GN_REGISTER_RUNTIME_TYPE(ModelAsset);

    AutoRef<const ModelScene>         scene;
    DynaArray<gpu2::RasterGeometry>   primitives;
    DynaArray<AutoRef<gpu2::Texture>> textures;
    DynaArray<AutoRef<gpu2::Buffer>>  materialBuffers;
    AutoRef<gpu2::GpuPayload>         gpuPayload;

    ModelAssetImpl(): ModelAsset(TYPE_INFO(), "model-asset") {}
    AutoRef<gpu2::GpuPayload> uploadPayload() const override { return gpuPayload; }
};

inline AutoRef<gpu2::Texture> makeSolidTexture(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc, const StrA & name, const std::array<uint8_t, 4> & color) {
    gpu2::Texture::Descriptor descriptor;
    descriptor.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(1).setLevels(1);
    auto texture = gpu2::Texture::create(name, {.context = gpu, .descriptor = descriptor});
    auto staging = gpu2::Buffer::create(name + ".staging", {.context = gpu, .size = 4, .mappable = true});
    if (!texture || !staging) return {};
    {
        auto mapped = staging->map();
        if (!mapped.data()) return {};
        memcpy(mapped.data(), color.data(), color.size());
    }
    gpu2::GpuCnC::Region region;
    region.imageExtent = {1, 1, 1};
    cnc.copyBufferToImage({.src = staging, .dst = texture, .regions = {&region, 1}});
    return texture;
}

} // namespace GN::fx2
