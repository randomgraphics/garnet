#include "pch.h"
#include <garnet/base/filesys.h>
#include <sstream>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2");

namespace GN::gpu2 {

static Buffer::StagedTexture stageImage(const StrA & name, AutoRef<GpuContext> context, const gfx::img::Image & image, const StrA & sourceName) {
    gfx::img::PlaneCoord p0 {};
    Texture::Descriptor  desc;
    desc.format  = image.format(p0);
    desc.width   = image.width(p0);
    desc.height  = image.height(p0);
    desc.depth   = image.depth(p0);
    desc.faces   = (uint32_t) image.desc().faces;
    desc.levels  = image.desc().levels ? (uint32_t) image.desc().levels : 1u;
    desc.samples = 1;
    if (desc.width == 0 || desc.height == 0) {
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: zero-size image '{}', name='{}'", sourceName, name);
        return {};
    }

    auto staging = Buffer::create(name + "_stg", {.context = context, .size = image.size(), .mappable = true});
    if (!staging) {
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: staging buffer alloc failed, name='{}'", name);
        return {};
    }
    {
        auto mapped = staging->map();
        if (!mapped.data()) {
            GN_ERROR(sLogger, "loadTextureToStagingBuffer: map failed, name='{}'", name);
            return {};
        }
        memcpy(mapped.data(), image.data(), (size_t) image.size());
    }

    DynaArray<Buffer::StagedTexture::Region> regions;
    for (uint32_t face = 0; face < desc.faces; ++face) {
        for (uint32_t level = 0; level < desc.levels; ++level) {
            gfx::img::PlaneCoord pc {0, face, level};
            const uint32_t       width  = image.width(pc);
            const uint32_t       height = image.height(pc);
            if (width == 0 || height == 0) continue;
            Buffer::StagedTexture::Region region;
            region.mip          = level;
            region.face         = face;
            region.imageExtent  = {width, height, 1u};
            region.bufferOffset = (uint64_t) image.offset(pc);
            regions.append(region);
        }
    }
    return {std::move(staging), desc, std::move(regions)};
}

Buffer::StagedTexture Buffer::loadTextureToStagingBuffer(const StrA & name, AutoRef<GpuContext> context, const StrA & path) {
    if (!context) {
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: null context, name='{}'", name);
        return {};
    }

    StrA absPath = path;
    if (!GN::fs::isAbsPath(absPath)) absPath = GN::fs::resolvePath(GN::fs::getCurrentDir(), absPath);

    auto fp = GN::fs::openFile(absPath, std::ios::in | std::ios::binary);
    if (!fp) {
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: cannot open '{}', name='{}'", absPath, name);
        return {};
    }

    gfx::img::Image image = gfx::img::Image::load(fp->input(), absPath.c_str());
    if (image.empty()) {
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: failed to decode '{}', name='{}'", absPath, name);
        return {};
    }

    return stageImage(name, context, image, absPath);
}

Buffer::StagedTexture Buffer::loadTextureToStagingBuffer(const StrA & name, AutoRef<GpuContext> context, ArrayView<const uint8_t> encoded,
                                                         const StrA & sourceName) {
    if (!context || encoded.empty()) {
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: invalid encoded image, name='{}'", name);
        return {};
    }

    const std::string  bytes(reinterpret_cast<const char *>(encoded.data()), encoded.size());
    std::istringstream stream(bytes, std::ios::in | std::ios::binary);
    gfx::img::Image    image = gfx::img::Image::load(stream, sourceName.c_str());
    if (image.empty()) {
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: failed decode '{}', name='{}'", sourceName, name);
        return {};
    }
    return stageImage(name, context, image, sourceName);
}

} // namespace GN::gpu2
