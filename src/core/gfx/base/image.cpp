#include "pch.h"
#include "imageDDS.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT GN_ASSERT
#define STBI_MALLOC GN::HeapMemory::alloc
#define STBI_REALLOC GN::HeapMemory::realloc
#define STBI_FREE GN::HeapMemory::dealloc
#include "stb_image.h"
   
using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.image");

// *****************************************************************************
// ImagePlaneDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::gfx::ImagePlaneDesc::valid() const {
    // check format
    if (!format.valid()) {
        GN_ERROR(sLogger)("invalid format");
        return false;
    }

    // check dimension
    if (0 == width || 0 == height || 0 == depth) {
        GN_ERROR(sLogger)("dimension can't zero!");
        return false;
    }

    // check pitches
    auto & cld = format.layoutDesc();
    auto w = math::alignToPowerOf2<uint32>( width, cld.blockWidth );
    auto h = math::alignToPowerOf2<uint32>( height, cld.blockHeight );
    if (step < cld.bits) {
        GN_ERROR(sLogger)("step is too small!");
        return false;
    }
    if (pitch < w * cld.bits / 8) {
        GN_ERROR(sLogger)("pitch is too small!");
        return false;
    }
    if (slice < pitch * h) {
        GN_ERROR(sLogger)("slice is too small!");
        return false;
    }
    if (size < slice * depth) {
        GN_ERROR(sLogger)("size is too small!");
        return false;
    }

    // check alignment
    static_assert(!math::isPowerOf2(0));
    if (!math::isPowerOf2(rowAlignment)) {
        GN_ERROR(sLogger)("alignment is not power of 2.");
        return false;
    }
    if (offset % rowAlignment) {
        GN_ERROR(sLogger)("offset is not aligned.");
    }
    if (pitch % rowAlignment) {
        GN_ERROR(sLogger)("pitch is not aligned.");
        return false;
    }
    if (slice % rowAlignment) {
        GN_ERROR(sLogger)("slice is not aligned.");
        return false;
    }

    // done
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::gfx::ImagePlaneDesc GN::gfx::ImagePlaneDesc::make(ColorFormat format, size_t width, size_t height, size_t depth, size_t step, size_t pitch, size_t slice, size_t alignment) {

    if (!format.valid()) {
        GN_ERROR(sLogger)("invalid color format: 0x%X", format.u32);
        return {};
    }

    if (0 == alignment) {
        alignment = 4; // aligned to 32bits by default.
    } else if (!math::isPowerOf2(alignment)) {
        GN_WARN(sLogger)("image alignment must be power of 2.");
        alignment = math::ceilPowerOf2(alignment); // make sure alignment is power of 2.
    }

    const auto & fd = format.layoutDesc();

    ImagePlaneDesc p;
    p.format = format;
    p.width  = (uint32_t)(width ? width : 1);
    p.height = (uint32_t)(height ? height : 1);
    p.depth  = (uint32_t)(depth ? depth : 1);
    p.step   = std::max((uint32_t)step, (uint32_t)fd.bits);

    auto aw = math::alignToPowerOf2<uint32_t>(p.width, fd.blockWidth);
    auto ah = math::alignToPowerOf2<uint32_t>(p.height, fd.blockHeight); 

    p.pitch  = std::max(aw * p.step / 8u, (uint32_t)pitch);
    p.pitch  = (p.pitch + (uint32_t)alignment - 1) & ~((uint32_t)alignment - 1); // make sure pitch meets alignment requirement.
    p.slice  = std::max(p.pitch * ah, (uint32_t)slice);
    p.size   = p.slice * p.depth;
    p.rowAlignment = (uint32_t)alignment;

    // done
    GN_ASSERT(p.valid());
    return p;
}

// *****************************************************************************
// ImageDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::gfx::ImageDesc::valid() const {

    if (planes.size() == 0) {
        // supposed to be an empty descriptor
        if (0 != levels || 0 != layers || 0 != size) {
            GN_ERROR(sLogger)("empty descriptor should have zero on all members variables.");
            return false;
        } else {
            return true;
        }
    }

    if (layers * levels != planes.size()) {
        GN_ERROR(sLogger)("plane arary size must be eaul to (layers * levels)");
        return false;
    }

    // check mipmaps
    for( uint32 f = 0; f < layers; ++f )
    for( uint32 l = 0; l < levels; ++l )
    {
        auto & m = plane(f, l);

        if (!m.valid()) {
            GN_ERROR(sLogger)("plane descritor [%d] is invalid", index(f, l));
            return false;
        }
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::gfx::ImageDesc::reset(const ImagePlaneDesc & basemap, uint32_t layers_, uint32_t levels_) {

    if (!basemap.valid()) {
        planes.clear();
        layers = 0;
        levels = 0;
        size = 0;
        GN_ASSERT(valid());
        return;
    }

    if (0 == layers_) layers_ = 1;

    if (0 == levels_) levels_ = UINT_MAX;

    // build full mipmap chain
    ImagePlaneDesc mip = basemap;
    levels = 0;
    for(;;) {
        for(size_t i = 0; i < layers_; ++i) {
            planes.append(mip);
            mip.offset += mip.size;
        }
        ++levels;

        // check for loop end
        if (levels >= levels_) break;
        if (1 == mip.width && 1 == mip.height && 1 == mip.depth ) break;

        // next level
        if (mip.width > 1) mip.width >>= 1;
        if (mip.height > 1) mip.height >>= 1;
        if (mip.depth > 1) mip.depth >>= 1;
        mip = ImagePlaneDesc::make(mip.format, mip.width, mip.height, mip.depth, mip.step, 0, 0, mip.rowAlignment);
    }

    layers = layers_;
    size = mip.offset;

    // done
    GN_ASSERT(valid());
}

// *****************************************************************************
// RawImage
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::RawImage::RawImage(ImageDesc&& desc, const void * initialContent, size_t initialContentSizeInbytes)
    : mDesc(std::move(desc)) {

    // allocate pixel buffer
    size_t imageSize = size();
    mPixels = (uint8_t*)HeapMemory::alignedAlloc(imageSize, mDesc.plane(0, 0).rowAlignment); // TODO: may LSM of all planes' alignment?
    if (!mPixels) {
        return;
    }

    // store the initial content
    if (initialContent) {
        if (0 == initialContentSizeInbytes) {
            initialContentSizeInbytes = imageSize;
        } else if (initialContentSizeInbytes != imageSize) {
            GN_WARN(sLogger)("incoming pixel buffer size does not equal to calculated image size.");
        }
        memcpy(mPixels, initialContent, std::min(imageSize, initialContentSizeInbytes));
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::RawImage GN::gfx::RawImage::load(File & fp) {

    auto begin = fp.tell();

    // setup stbi io callback
    stbi_io_callbacks io = {};
    io.read = [](void* user, char* data, int size) -> int {
        auto fp = (GN::File*)user;
        size_t read = 0;
        if (!fp->read(data, size, &read)) return 0;
        return (int)read;
    };
    io.skip = [](void* user, int n) {
        auto fp = (GN::File*)user;
        fp->seek((size_t)n, GN::FileSeek::CUR);
    };
    io.eof = [](void* user) -> int {
        auto fp = (GN::File*)user;
        return fp->eof();
    };

    // Load from common image file via stb_image library
    // TODO: hdr/grayscale support
    int x,y,n;
    auto data = stbi_load_from_callbacks(&io, &fp, &x, &y, &n, 4);
    if (data) {
        auto image = RawImage(ImageDesc(ImagePlaneDesc::make(ColorFormat::RGBA_8_8_8_8_UNORM, (uint32_t)x, (uint32_t)y)), data);
        GN_ASSERT(image.desc().valid());
        stbi_image_free(data);
        return image;
    }

    // try read as DDS
    fp.seek(begin, GN::FileSeek::SET);
    DDSReader dds(fp);
    if (dds.checkFormat()) {
        auto image = RawImage(dds.readHeader());
        if (image.empty()) return {};
        if (!dds.readPixels(image.data(), image.size())) return {};
        return image;
    }

    GN_ERROR(sLogger)("Failed to load image from file: unrecognized file format.");
    return {};
}
