#ifndef __GN_GFX_SURFACE_H__
#define __GN_GFX_SURFACE_H__
// *****************************************************************************
/// \file
/// \brief   Graphics surface classes, include texture, vtxbuf and idxbuf
/// \author  chen@@CHENLI-HOMEPC (2007.2.23)
// *****************************************************************************

namespace GN {
namespace gfx {
///
/// texture usage enumeration
///
struct TextureUsage {
    enum Enum {
        DEFAULT = 0,         ///< Default usage.
        COLOR_RENDER_TARGET, ///< use as color render target
        DEPTH_RENDER_TARGET, ///< use as depth buffer
        FAST_CPU_WRITE,      ///< Allow fast CPU write, in exchange of GPU rendering speed
        NUM_USAGES,          ///< number of usage flags
    };

    GN_DEFINE_ENUM_CLASS_HELPERS(TextureUsage, Enum);
};
GN_CASSERT(sizeof(TextureUsage) == sizeof(uint32_t));

///
/// Texture descriptor
///
struct TextureDesc {
    PixelFormat format; ///< pixel format.
    uint32_t    width;  ///< basemap width
    uint32_t    height; ///< basemap height
    uint32_t    depth;  ///< basemap depth
    uint32_t    faces;  ///< face count. When used as parameter of Gpu::createTexture(),
                        ///< you may set it to 0 to use default face count: 6 for cubemap, 1 for others.
    uint32_t levels;    ///< mipmap level count. When used as parameter of Gpu::createTexture(),
                        ///< you may set it to 0 to create full mipmap chain (down to 1x1).
    TextureUsage usage; ///< texture usage

    ///
    /// get basemap size
    ///
    const Vector3<uint32_t> & size() const { return *(Vector3<uint32_t> *) &width; }

    ///
    /// compose texture descriptor from image descriptor
    ///
    bool fromImageDesc(const ImageDesc & id) {
        format = id.format();
        width  = id.width();
        height = id.height();
        depth  = id.depth();
        faces  = id.layers;
        levels = id.levels;
        usage  = TextureUsage::DEFAULT;
        return validate();
    }

    ///
    /// validate texture descriptor
    ///
    bool validate() {
        // calculate maximum mipmap levels
        uint32_t nx = 0, ny = 0, nz = 0;
        uint32_t maxLevels;

        maxLevels = width;
        while (maxLevels > 0) {
            maxLevels >>= 1;
            ++nx;
        }

        maxLevels = height;
        while (maxLevels > 0) {
            maxLevels >>= 1;
            ++ny;
        }

        maxLevels = depth;
        while (maxLevels > 0) {
            maxLevels >>= 1;
            ++nz;
        }

        maxLevels = math::getmax(nx, ny, nz);

        levels = (0 == levels) ? maxLevels : math::getmin(maxLevels, levels);

        // check format
        if (!format.valid()) {
            static Logger * sLocalLogger = getLogger("GN.gfx.TextureDesc");
            GN_ERROR(sLocalLogger)("invalid texture format: %s", format.toString().c_str());
            return false;
        }

        // check usage
        if (usage < 0 && usage >= TextureUsage::NUM_USAGES) {
            static Logger * sLocalLogger = getLogger("GN.gfx.TextureDesc");
            GN_ERROR(sLocalLogger)("invalid texture usage: %d", (int)usage.toRawEnum());
            return false;
        }

        // success
        return true;
    }
};

struct MipmapData {
    uint32_t           rowPitch;
    uint32_t           slicePitch;
    DynaArray<uint8_t> data;
};

///
/// cube texture face
///
struct CubeFace {
    enum Enum {
        PX = 0, ///< positive X
        NX,     ///< negative X
        PY,     ///< positive Y
        NY,     ///< negative Y
        PZ,     ///< positive Z
        NZ,     ///< negative Z
    };

    GN_DEFINE_ENUM_CLASS_HELPERS(CubeFace, Enum);
};

///
/// Update flags
///
struct SurfaceUpdateFlag {
    enum Enum {
        DEFAULT,      ///< default update flag
        DISCARD,      ///< discard old content of whole surface.
        NO_OVERWRITE, ///< promise not to modify any section of the surface that is being used by GPU.
        NUM_FLAGS     ///< number of update flags.
    };

    /// convert enumeration to string
    const char * toString() const {
        static const char * TABLE[] = {
            "DEFAULT",
            "DISCARD",
            "NO_OVERWRITE",
        };

        const Enum & e = *this;
        if (0 <= e && e <= NUM_FLAGS)
            return TABLE[e];
        else
            return "INVALID_SURFACE_UPDATE_FLAG";
    }
    GN_DEFINE_ENUM_CLASS_HELPERS(SurfaceUpdateFlag, Enum);
};

///
/// GPU texture class
///
struct Texture : public RefCounter {
    ///
    /// Get texture descriptor
    ///
    const TextureDesc & getDesc() const { return mDesc; }

    ///
    /// get size of base map
    ///
    const Vector3<uint32_t> & getBaseSize() const { return mDesc.size(); }

    ///
    /// get size of base map
    ///
    template<typename T>
    void getBaseSize(T * sx, T * sy = 0, T * sz = 0) const {
        if (sx) *sx = (T) mDesc.width;
        if (sy) *sy = (T) mDesc.height;
        if (sz) *sz = (T) mDesc.depth;
    }

    ///
    /// get size of specific mip level
    ///
    const Vector3<uint32_t> & getMipSize(uint32_t level) const {
        GN_ASSERT(level < mDesc.levels);
        return mMipSize[level];
    }

    ///
    /// get size of specific mip level
    ///
    template<typename T>
    void getMipSize(uint32_t level, T * sx, T * sy = 0, T * sz = 0) const {
        const Vector3<uint32_t> & mipSize = getMipSize(level);
        if (sx) *sx = (T) mipSize.x;
        if (sy) *sy = (T) mipSize.y;
        if (sz) *sz = (T) mipSize.z;
    }

    ///
    /// update mipmap content
    ///
    /// \param face, level      Subsurface location
    /// \param area             Subsurface area that is going to be updated. Set to NULL to update whole subsurface.
    /// \param rowPitch         Row pitch in bytes of the data (single texel row, not single texel block row)
    /// \param slicePitch       Slice pitch in bytes of the data
    /// \param data             The data buffer that holds data that are going to be copied to texture.
    ///                         The data must be the same format as the texture.
    /// \param flag             Updte flag. Must be one of SurfaceUpdateFlag.
    ///
    virtual void updateMipmap(uint32_t face, uint32_t level, const Box<uint32_t> * area, uint32_t rowPitch, uint32_t slicePitch, const void * data,
                              SurfaceUpdateFlag flag = SurfaceUpdateFlag::DEFAULT) = 0;

    ///
    /// read mipmap content.
    ///
    virtual void readMipmap(uint32_t face, uint32_t level, MipmapData & data) = 0;

    ///
    /// generate content of all mipmap levels based on content in base level
    ///
    virtual void generateMipmapPyramid() = 0;

    ///
    /// Get low-level device handle of the texture. LPDIRECT3DBASETEXTURE9 for
    /// DirectX; name of texture object(GLuint) for OpenGL.
    ///
    virtual void * getAPIDependentData() const = 0;

    /// \name get reference to texture name.
    ///
    /// Name field is for debug purpose only, it is not used by garnet library except logging.
    /// Set it to any value you want.
    //@{
    const StrA & name() const { return mName; }
    StrA &       name() { return mName; }
    //@}

protected:
    ///
    /// Set texture descriptor. Subclass must call this function to set
    /// all texture properities to valid value.
    ///
    bool setDesc(const TextureDesc & desc) {
        mDesc = desc;

        if (!mDesc.validate()) return false;

        // allocate mipmap size array
        mMipSize.resize(mDesc.levels);

        return true;
    }

    ///
    /// setup mip size
    ///
    template<typename T>
    void setMipSize(uint32_t level, T sx, T sy, T sz) {
        GN_ASSERT(level < mDesc.levels);
        GN_ASSERT(level > 0 || (sx == (T) mDesc.width && sy == (T) mDesc.height && sz == (T) mDesc.depth));
        mMipSize[level].set((uint32_t) sx, (uint32_t) sy, (uint32_t) sz);
    }

    ///
    /// setup mip size
    ///
    void setMipSize(uint32_t level, const Vector3<uint32_t> & s) { setMipSize(level, s.x, s.y, s.z); }

private:
    TextureDesc                  mDesc;    ///< descriptor
    DynaArray<Vector3<uint32_t>> mMipSize; ///< mipmap size of each level
    StrA                         mName;    ///< texture name. Only for debug purpose.
};

///
/// vertex buffer descriptor
///
struct VtxBufDesc {
    uint32_t length;       ///< length in bytes of the vertex buffer
    bool     fastCpuWrite; ///< support fast CPU write (rendering speed compromised)
};

///
/// Vertex buffer interface.
///
struct VtxBuf : public RefCounter {
    ///
    /// get vertex buffer descriptor
    ///
    const VtxBufDesc & getDesc() const { return mDesc; }

    ///
    /// update vertex buffer content. offset and length are in number of bytes.
    ///
    virtual void update(uint32_t offset, uint32_t length, const void * data, SurfaceUpdateFlag flag = SurfaceUpdateFlag::DEFAULT) = 0;

    ///
    /// Read buffer content.
    ///
    virtual void readback(DynaArray<uint8_t> & data) = 0;

protected:
    ///
    /// Set buffer properties
    ///
    void setDesc(const VtxBufDesc & desc) {
        GN_ASSERT(desc.length > 0);
        mDesc = desc;
    }

private:
    VtxBufDesc mDesc; ///< Buffer descriptor
};

///
/// index buffer descriptor
///
struct IdxBufDesc {
    uint32_t numidx;       ///< number of indices in index buffer
    bool     bits32;       ///< is 32-bit index buffer or not.
    bool     fastCpuWrite; ///< support fast CPU write (rendering speed compromised)
};

///
/// Index Buffer
///
struct IdxBuf : public RefCounter {
    ///
    /// Get descriptor
    ///
    const IdxBufDesc & getDesc() const { return mDesc; }

    ///
    /// update index buffer content
    ///
    virtual void update(uint32_t startidx, uint32_t numidx, const void * data, SurfaceUpdateFlag flag = SurfaceUpdateFlag::DEFAULT) = 0;

    ///
    /// Read buffer content.
    ///
    virtual void readback(DynaArray<uint8_t> & data) = 0;

protected:
    ///
    /// Set buffer properties
    ///
    void setDesc(const IdxBufDesc & desc) {
        GN_ASSERT(desc.numidx > 0);
        mDesc = desc;
    }

private:
    IdxBufDesc mDesc;
};

struct BufferUsage {
    enum Enum {
        UNIFORM,        ///< uniform buffer
        VERTEX,         ///< vertex buffer
        INDEX,          ///< index buffer
        FAST_CPU_WRITE, ///< Allow fast CPU write, in exchange of GPU rendering speed
        NUM_USAGES,     ///< number of usage flags
    };

    GN_DEFINE_ENUM_CLASS_HELPERS(BufferUsage, Enum);
};
GN_CASSERT(sizeof(TextureUsage) == sizeof(uint32_t));

struct BufferDesc {
    uint32_t sizeInBytes;
    uint32_t usages; // combination of BufferUsage flags.
};

struct Buffer : public RefCounter {
    ///
    /// Get descriptor
    ///
    const BufferDesc & getDesc() const { return mDesc; }

    ///
    /// update index buffer content
    ///
    virtual void update(uint32_t startidx, uint32_t numidx, const void * data) = 0;

    ///
    /// Read buffer content.
    ///
    virtual void readback(DynaArray<uint8_t> & data) = 0;

protected:
    ///
    /// Set buffer properties
    ///
    void setDesc(const BufferDesc & desc) { mDesc = desc; }

private:
    BufferDesc mDesc;
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SURFACE_H__
