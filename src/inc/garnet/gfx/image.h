#ifndef __GN_GFX_IMAGE_H__
#define __GN_GFX_IMAGE_H__
// *****************************************************************************
/// \file
/// \brief   Image descriptor and image reader
/// \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN {
namespace gfx {
/// This represents a single 1D/2D/3D image in an more complex image structure.
// Note: avoid using size_t in this structure. So the size of the structure will never change, regardless of compile platform.
struct GN_API ImagePlaneDesc {

    /// pixel format
    ColorFormat format = ColorFormat::UNKNOWN;

    /// Plane width in pixels
    uint32_t width = 0;

    /// Plane height in pixels
    uint32_t height = 0;

    /// Plane depth in pixels
    uint32_t depth = 0;

    /// bits (not BYTES) from one pixel to next. Minimal valid value is pixel size.
    uint32_t step = 0;

    /// Bytes from one row to next. Minimal valid value is (width * step) and aligned to alignment.
    uint32_t pitch = 0;

    /// Bytes from one slice to next. Minimal valid value is (pitch * height)
    uint32_t slice = 0;

    /// Bytes of the whole plane. Minimal valid value is (slice * depth)
    uint32_t size = 0;

    /// Bytes between first pixel of the plane to the first pixel of the whole image.
    uint32_t offset = 0;

    /// row alignment
    uint32_t rowAlignment = 0;

    /// returns offset of particular pixel within the plane
    size_t pixel(size_t x, size_t y, size_t z = 0) const {
        GN_ASSERT(x < width && y < height && z < depth);
        size_t r = z * slice + y * pitch + x * step / 8;
        GN_ASSERT(r < size);
        return r + offset;
    }

    /// check if this is a valid image plane descriptor. Note that valid descriptor is never empty.
    bool valid() const;

    /// check if this is an empty descriptor. Note that empty descriptor is never valid.
    bool empty() const { return ColorFormat::UNKNOWN == format; }

    /// Create a new image plane descriptor
    static ImagePlaneDesc make(ColorFormat format, size_t width, size_t height = 1, size_t depth = 1, size_t step = 0, size_t pitch = 0, size_t slice = 0,
                               size_t alignment = 4);
};

///
/// Represent a complex image with optional mipmap chain
///
struct GN_API ImageDesc {

    // ****************************
    /// \name member data
    // ****************************

    //@{

    DynaArray<ImagePlaneDesc> planes;     ///< length of array = layers * mips;
    uint32_t                  layers = 0; ///< number of layers
    uint32_t                  levels = 0; ///< number of mipmap levels
    uint32_t                  size   = 0; ///< total size in bytes;

    //@}

    // ****************************
    /// \name ctor/dtor/copy/move
    // ****************************

    //@{

    ImageDesc() = default;

    ///
    /// Construct image descriptor from basemap and layer/level count. If anything goes wrong, constructs an empty image descriptor.
    ///
    /// \param basemap the base image
    /// \param layers number of layers. must be positive integer
    /// \param levels number of mipmap levels. set to 0 to automatically build full mipmap chain.
    ///
    ImageDesc(const ImagePlaneDesc & basemap, size_t layers = 1, size_t levels = 1) { reset(basemap, (uint32_t) layers, (uint32_t) levels); }

    // can copy. can move.
    GN_DEFAULT_COPY(ImageDesc);
    GN_DEFAULT_MOVE(ImageDesc);

    //@}

    // ****************************
    /// \name public methods
    // ****************************

    //@{

    ///
    /// check if the image is empty or not
    ///
    bool empty() const { return planes.empty(); }

    ///
    /// make sure this is a meaningfull image descriptor
    ///
    bool valid() const;

    /// methods to return properties of the specific plane.
    //@{
    const ImagePlaneDesc & plane(size_t layer = 0, size_t level = 0) const { return planes[index(layer, level)]; }
    ImagePlaneDesc &       plane(size_t layer = 0, size_t level = 0) { return planes[index(layer, level)]; }
    ColorFormat            format(size_t layer = 0, size_t level = 0) const { return planes[index(layer, level)].format; }
    uint32_t               width(size_t layer = 0, size_t level = 0) const { return planes[index(layer, level)].width; }
    uint32_t               height(size_t layer = 0, size_t level = 0) const { return planes[index(layer, level)].height; }
    uint32_t               depth(size_t layer = 0, size_t level = 0) const { return planes[index(layer, level)].depth; }
    uint32_t               step(size_t layer = 0, size_t level = 0) const { return planes[index(layer, level)].step; }
    uint32_t               pitch(size_t layer = 0, size_t level = 0) const { return planes[index(layer, level)].pitch; }
    uint32_t               slice(size_t layer = 0, size_t level = 0) const { return planes[index(layer, level)].slice; }
    //@}

    ///
    /// returns offset of particular pixel
    ///
    size_t pixel(size_t layer, size_t level, size_t x = 0, size_t y = 0, size_t z = 0) const {
        const auto & d = planes[index(layer, level)];
        auto         r = d.pixel(x, y, z);
        GN_ASSERT(r < size);
        return r;
    }

    // void vertFlipInpace(void * pixels, size_t sizeInBytes);

private:
    /// return plane index
    size_t index(size_t layer, size_t level) const {
        GN_ASSERT(layer < layers);
        GN_ASSERT(level < levels);
        return (level * layers) + layer;
    }

    /// reset the descriptor
    void reset(const ImagePlaneDesc & basemap, uint32_t layers, uint32_t levels);
};

///
/// A basic image class
///
class GN_API RawImage {

public:
    /// \name ctor/dtor/copy/move
    //@{
    RawImage() = default;
    RawImage(ImageDesc && desc, const void * initialContent = nullptr, size_t initialContentSizeInbytes = 0);
    GN_NO_COPY(RawImage);
    GN_DEFAULT_MOVE(RawImage);
    //@}

    /// \name basic property query
    //@{

    /// return descriptor of the whole image
    const ImageDesc & desc() const { return mDesc; }

    /// return descriptor of a image plane
    const ImagePlaneDesc & desc(size_t layer, size_t level) const { return mDesc.plane(layer, level); }

    /// return pointer to pixel buffer.
    const uint8_t * data() const { return mPixels; }

    /// return pointer to pixel buffer.
    uint8_t * data() { return mPixels; }

    /// return size of the whole image in bytes.
    uint32_t size() const { return mDesc.size; }

    /// check if the image is empty or not.
    bool empty() const { return mDesc.empty(); }

    //@}

    /// \name query properties of the specific plane.
    //@{
    ColorFormat format(size_t layer = 0, size_t level = 0) const { return mDesc.plane(layer, level).format; }
    uint32_t    width(size_t layer = 0, size_t level = 0) const { return mDesc.plane(layer, level).width; }
    uint32_t    height(size_t layer = 0, size_t level = 0) const { return mDesc.plane(layer, level).height; }
    uint32_t    depth(size_t layer = 0, size_t level = 0) const { return mDesc.plane(layer, level).depth; }
    uint32_t    step(size_t layer = 0, size_t level = 0) const { return mDesc.plane(layer, level).step; }
    uint32_t    pitch(size_t layer = 0, size_t level = 0) const { return mDesc.plane(layer, level).pitch; }
    uint32_t    slice(size_t layer = 0, size_t level = 0) const { return mDesc.plane(layer, level).slice; }
    //@}

    /// \name methods to return pointer to particular pixel
    //@{
    const uint8_t * pixel(size_t layer, size_t level, size_t x = 0, size_t y = 0, size_t z = 0) const { return mPixels + mDesc.pixel(layer, level, x, y, z); }
    uint8_t *       pixel(size_t layer, size_t level, size_t x = 0, size_t y = 0, size_t z = 0) { return mPixels + mDesc.pixel(layer, level, x, y, z); }
    //@}

    /// \name load & save
    //@{
    static RawImage load(File &);
    static RawImage load(const StrA & filename) {
        AutoObjPtr<File> fp(GN::fs::openFile(filename, "rb"));
        if (fp.empty()) return {};
        return load(*fp);
    }
    //@}

private:
    uint8_t * mPixels = nullptr;

    ImageDesc mDesc;
};
} // namespace gfx
} // namespace GN

// #include "image.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_IMAGE_H__
