#include "pch.h"
#include <garnet/base/filesys.h>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2");

namespace GN::gpu2 {

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

    // Derive GPU texture descriptor from the decoded image.
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
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: zero-size image '{}', name='{}'", absPath, name);
        return {};
    }

    // Copy the whole image (all faces × mips) into one host-visible staging buffer.
    // image.size() accounts for all planes including inter-plane alignment gaps.
    // image.offset(pc) gives each plane's byte offset within this layout.
    // rapid-image does not add row padding within a plane, so bufferRowLength=0 (tight) is correct.
    auto staging = Buffer::create(name + "_stg", {.context = context, .size = image.size(), .mappable = true});
    if (!staging) {
        GN_ERROR(sLogger, "loadTextureToStagingBuffer: staging buffer alloc failed, name='{}'", name);
        return {};
    }

    {
        auto m = staging->map();
        if (!m.data()) {
            GN_ERROR(sLogger, "loadTextureToStagingBuffer: map failed, name='{}'", name);
            return {};
        }
        memcpy(m.data(), image.data(), (size_t) image.size());
        // m unmaps on scope exit (RAII)
    }

    // Build one copy region per face × mip.
    DynaArray<StagedTexture::Region> regions;
    for (uint32_t f = 0; f < desc.faces; ++f) {
        for (uint32_t l = 0; l < desc.levels; ++l) {
            gfx::img::PlaneCoord pc {0, f, l};
            uint32_t             w = image.width(pc);
            uint32_t             h = image.height(pc);
            if (w == 0 || h == 0) continue;
            StagedTexture::Region r;
            r.mip          = l;
            r.face         = f;
            r.imageOffset  = {0, 0, 0};
            r.imageExtent  = {w, h, 1u};
            r.bufferOffset = (uint64_t) image.offset(pc);
            regions.append(r);
        }
    }

    return {std::move(staging), desc, std::move(regions)};
}

} // namespace GN::gpu2
