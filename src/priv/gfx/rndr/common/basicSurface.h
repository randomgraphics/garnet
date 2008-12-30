#ifndef __GN_GFXCOMMON_BASICSURFACE_H__
#define __GN_GFXCOMMON_BASICSURFACE_H__
// *****************************************************************************
/// \file
/// \brief   surface base classes
/// \author  chenlee (2005.10.1)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// 贴图类的基类
    ///
    class BasicTexture : public Texture
    {
        // ********************************
        /// \name constructor
        // ********************************

        //@{
    protected :
        BasicTexture() {}
        //@}

        // ********************************
        //  protected functions
        // ********************************
    protected:

        ///
        /// Validate update parameters.
        ///
        bool validateUpdateParameters( size_t face, size_t level, const Box<UInt32> * area, UpdateFlag flag, Box<UInt32> & clippedArea )
        {
            // check face
            if( face >= getDesc().faces )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicTexture");
                GN_ERROR(sLogger)( "invalid lock face : %d", face );
                return false;
            }

            // check level
            if( level >= getDesc().levels )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicTexture");
                GN_ERROR(sLogger)( "invalid lock level : %d", level );
                return false;
            }

            // check flag
            if( flag >= NUM_UPDATE_FLAGS )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicTexture");
                GN_ERROR(sLogger)( "invalid lock flag : %d", flag );
                return false;
            }

            // get texture size
            const Vector3<UInt32> & sz = getMipSize( level );

            // make sure lock area is valid
            if( area )
            {
                clippedArea = *area;
                if( !sAdjustOffsetAndRange( clippedArea.x, clippedArea.w, (int)sz.x ) ||
                    !sAdjustOffsetAndRange( clippedArea.y, clippedArea.h, (int)sz.y ) ||
                    !sAdjustOffsetAndRange( clippedArea.z, clippedArea.d, (int)sz.z ) )
                    return false;
            }
            else
            {
                clippedArea.x = 0;
                clippedArea.y = 0;
                clippedArea.z = 0;
                clippedArea.w = (int)sz.x;
                clippedArea.h = (int)sz.y;
                clippedArea.d = (int)sz.z;
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

        static inline bool
        sAdjustOffsetAndRange( size_t & offset, size_t & length, size_t maxLength )
        {
            if( offset >= maxLength )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicTexture");
                GN_ERROR(sLogger)( "offset is beyond the end of valid range." );
                return false;
            }
            if( 0 == length ) length = maxLength;
            if( offset + length > maxLength ) length = maxLength - offset;
            return true;
        }
    };

    ///
    /// Basic vertex buffer class
    ///
    class BasicVtxBuf : public VtxBuf
    {
        // ********************************
        /// \name constructor
        // ********************************

        //@{
    protected :
        BasicVtxBuf() {}
        //@}

        // ********************************
        //  protected functions
        // ********************************
    protected:

        ///
        /// Validate update parameters.
        ///
        bool validateUpdateParameters( size_t offset, size_t & length )
        {
            const VtxBufDesc & desc = getDesc();

            if( offset >= desc.length )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicVtxBuffer");
                GN_ERROR(sLogger)( "offset is beyond the end of vertex buffer!" );
                return false;
            }

            // adjust length
            if( 0 == length ) length = desc.length;
            if( offset + length > desc.length ) length = desc.length - offset;

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
    class BasicIdxBuf : public IdxBuf
    {
        // ********************************
        /// \name constructor
        // ********************************

        //@{
    protected :
        BasicIdxBuf() {}
        //@}

        // ********************************
        //  protected functions
        // ********************************
    protected:

        ///
        /// Validate update parameters.
        ///
        bool validateUpdateParameters( size_t startidx, size_t & numidx )
        {
            const IdxBufDesc & desc = getDesc();

            if( startidx >= desc.numidx )
            {
                static Logger * sLogger = getLogger("GN.gfx.rndr.common.BasicIdxBuffer");
                GN_ERROR(sLogger)( "startidx is beyond the end of index buffer!" );
                return false;
            }

            // adjust startidx and numidx
            if( 0 == numidx ) numidx = desc.numidx;
            if( startidx + numidx > desc.numidx ) numidx = desc.numidx - startidx;

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
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICSURFACE_H__
