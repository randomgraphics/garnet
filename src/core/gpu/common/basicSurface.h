#ifndef __GN_GPUCOMMON_BASICSURFACE_H__
#define __GN_GPUCOMMON_BASICSURFACE_H__
// *****************************************************************************
/// \file
/// \brief   surface base classes
/// \author  chenlee (2005.10.1)
// *****************************************************************************

namespace GN {
namespace gfx {
///
/// basic texture implementation class
///
class BasicTexture : public Texture {
    // ********************************
    /// \name constructor
    // ********************************

    //@{
protected:
    BasicTexture() {}
    //@}

    // ********************************
    //  protected functions
    // ********************************
protected:
    ///
    /// Validate update parameters.
    ///
    bool validateUpdateParameters(uint32 face, uint32 level, const Box<uint32> * area, SurfaceUpdateFlag flag, Box<uint32> & clippedArea) {
        // check face
        if (face >= getDesc().faces) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicTexture");
            GN_ERROR(sLogger)("invalid lock face : %d", face);
            return false;
        }

        // check level
        if (level >= getDesc().levels) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicTexture");
            GN_ERROR(sLogger)("invalid lock level : %d", level);
            return false;
        }

        // check flag
        if (flag >= SurfaceUpdateFlag::NUM_FLAGS) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicTexture");
            GN_ERROR(sLogger)("invalid lock flag : %d", flag.toRawEnum());
            return false;
        }

        // get texture size
        const Vector3<uint32> & sz = getMipSize(level);

        // make sure lock area is valid
        if (area) {
            clippedArea = *area;
            if (!sAdjustOffsetAndRange(clippedArea.x, clippedArea.w, sz.x) || !sAdjustOffsetAndRange(clippedArea.y, clippedArea.h, sz.y) ||
                !sAdjustOffsetAndRange(clippedArea.z, clippedArea.d, sz.z))
                return false;
        } else {
            clippedArea.x = 0;
            clippedArea.y = 0;
            clippedArea.z = 0;
            clippedArea.w = sz.x;
            clippedArea.h = sz.y;
            clippedArea.d = sz.z;
        }

        // success
        return true;
    }

    // ********************************
    //  private variables
    // ********************************
private:
    // ********************************
    //  private functions
    // ********************************
private:
    template<typename T>
    static inline bool sAdjustOffsetAndRange(T & offset, T & length, T maxLength) {
        if (offset >= maxLength) {
            static Logger * sLocalLogger = getLogger("GN.gfx.gpu.common.BasicTexture");
            GN_ERROR(sLocalLogger)("offset is beyond the end of valid range.");
            return false;
        }
        if (0 == length) length = maxLength;
        if (offset + length > maxLength) length = maxLength - offset;
        return true;
    }
};

///
/// Basic vertex buffer class
///
class BasicVtxBuf : public VtxBuf {
    // ********************************
    /// \name constructor
    // ********************************

    //@{
protected:
    BasicVtxBuf() {}
    //@}

    // ********************************
    //  protected functions
    // ********************************
protected:
    ///
    /// Validate update parameters. Note that this function may modify "length" parameter
    ///
    bool validateUpdateParameters(uint32 offset, uint32 * length, const void * data, SurfaceUpdateFlag flag) {
        const VtxBufDesc & vbd = getDesc();

        if (NULL == data) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicVtxBuffer");
            GN_ERROR(sLogger)("NULL data pointer!");
            return false;
        }

        if (flag >= SurfaceUpdateFlag::NUM_FLAGS) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicVtxBuffer");
            GN_ERROR(sLogger)("Invalid update flag: %d.", flag.toRawEnum());
            return false;
        }

        if (offset >= vbd.length) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicVtxBuffer");
            GN_ERROR(sLogger)("offset is beyond the end of vertex buffer!");
            return false;
        }

        // adjust length
        if (0 == *length) *length = vbd.length;
        if (offset + *length > vbd.length) *length = vbd.length - offset;

        // success
        return true;
    }

    // ********************************
    //  private variables
    // ********************************
private:
    // ********************************
    //  private functions
    // ********************************
private:
};

///
/// Basic index buffer class
///
class BasicIdxBuf : public IdxBuf {
    // ********************************
    /// \name constructor
    // ********************************

    //@{
protected:
    BasicIdxBuf() {}
    //@}

    // ********************************
    //  protected functions
    // ********************************
protected:
    ///
    /// Validate update parameters.
    ///
    bool validateUpdateParameters(uint32 startidx, uint32 * numidx, const void * data, SurfaceUpdateFlag flag) {
        GN_ASSERT(numidx);

        const IdxBufDesc & ibd = getDesc();

        if (NULL == data) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicIdxBuffer");
            GN_ERROR(sLogger)("NULL data pointer!");
            return false;
        }

        if (flag >= SurfaceUpdateFlag::NUM_FLAGS) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicIdxBuffer");
            GN_ERROR(sLogger)("Invalid update flag: %d.", flag.toRawEnum());
            return false;
        }

        if (startidx >= ibd.numidx) {
            static Logger * sLogger = getLogger("GN.gfx.gpu.common.BasicIdxBuffer");
            GN_ERROR(sLogger)("startidx is beyond the end of index buffer!");
            return false;
        }

        // adjust startidx and numidx
        if (0 == *numidx) *numidx = ibd.numidx;
        if (startidx + *numidx > ibd.numidx) *numidx = ibd.numidx - startidx;

        // success
        return true;
    }

    // ********************************
    //  private variables
    // ********************************
private:
    // ********************************
    //  private functions
    // ********************************
private:
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPUCOMMON_BASICSURFACE_H__
