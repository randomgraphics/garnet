#include "pch.h"

namespace GN::gpu2 {

namespace {

template<typename Recorder>
void uploadImage(Recorder & recorder, AutoRef<Texture> dst, const gfx::img::Image & image) {
    if (!dst || image.empty()) return;
    DynaArray<GpuCnC::Region> regions;
    const auto format = image.format();
    const uint32_t bytes = format.bytesPerBlock();
    if (!bytes) return;
    for (uint32_t face = 0; face < image.desc().faces; ++face) {
        for (uint32_t level = 0; level < image.desc().levels; ++level) {
            const gfx::img::PlaneCoord coord {0, face, level};
            const auto & plane = image.plane(coord);
            GpuCnC::Region region;
            region.face = face;
            region.mip = level;
            region.imageExtent = {(uint32_t) plane.extent.w, (uint32_t) plane.extent.h, (uint32_t) plane.extent.d};
            region.dataOffset = image.offset(coord);
            // The CPU image may pad rows/slices. Describe its layout rather than assuming tightly packed pixels.
            region.rowPitchBytes = plane.pitch;
            region.slicePitchBytes = plane.slice;
            regions.append(region);
        }
    }
    recorder.recordUploadImage(std::move(dst), {(const uint8_t *) image.data(), image.size()}, regions);
}

} // namespace

void GpuCnC::recordUploadImage(AutoRef<Texture> dst, const gfx::img::Image & content) { uploadImage(*this, std::move(dst), content); }

void bindless::CnC::recordUploadImage(AutoRef<Texture> dst, const gfx::img::Image & content) { uploadImage(*this, std::move(dst), content); }

} // namespace GN::gpu2
