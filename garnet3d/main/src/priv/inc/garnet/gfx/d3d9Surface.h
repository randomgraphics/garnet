#ifndef __GN_GFX_D3D9SURFACE_H__
#define __GN_GFX_D3D9SURFACE_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 base surface interface
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx
{
    /// \name standard D3D9 surface GUIDs
    //@{
    extern const Guid D3D9_SURFACE_TYPE_VB;        ///< vertex buffer
    extern const Guid D3D9_SURFACE_TYPE_IB;        ///< index buffer
    extern const Guid D3D9_SURFACE_TYPE_TEX_2D;    ///< 2D texture
    extern const Guid D3D9_SURFACE_TYPE_TEX_3D;    ///< 3D texture
    extern const Guid D3D9_SURFACE_TYPE_TEX_CUBE;  ///< cube texture
    extern const Guid D3D9_SURFACE_TYPE_RTT_2D;    ///< 2D render target texture
    extern const Guid D3D9_SURFACE_TYPE_RTT_CUBE;  ///< 2D cube texture
    extern const Guid D3D9_SURFACE_TYPE_RTS_COLOR; ///< render target surface
    extern const Guid D3D9_SURFACE_TYPE_RTS_DEPTH; ///< depth stencil surface
    //@}

    ///
    /// base D3D9 surface
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9Surface : public Surface
    {
        SurfaceDesc mDesc;

        static inline bool
        sAdjustOffsetAndRange( size_t & offset, size_t & length, size_t maxLength )
        {
            if( offset >= maxLength )
            {
                GN_ERROR(getLogger("GN.gfx2.D3D9Surface"))( "offset is too large." );
                return false;
            }
            if( 0 == length ) length = maxLength;
            if( offset + length > maxLength ) length = maxLength - offset;
            return true;
        }

    protected:

        ///
        /// adjust area to valid range
        ///
        bool adjustArea( Box<size_t> & result, const Box<size_t> * original )
        {
            GN_ASSERT( &result != original );

            if( original )
            {
                result = *original;
                if( !sAdjustOffsetAndRange( result.x, result.w, mDesc.layout.basemap.width ) ||
                    !sAdjustOffsetAndRange( result.y, result.h, mDesc.layout.basemap.height ) ||
                    !sAdjustOffsetAndRange( result.z, result.d, mDesc.layout.basemap.depth ) )
                {
                    return false;
                }
            }
            else
            {
                result.x = 0;
                result.y = 0;
                result.z = 0;
                result.w = mDesc.layout.basemap.width;
                result.h = mDesc.layout.basemap.height;
                result.d = mDesc.layout.basemap.depth;
            }

            return true;
        }

    public:

        ///
        /// ctor
        ///
        D3D9Surface( const SurfaceDesc & desc ) : mDesc(desc) {}

        // from base class
        virtual const SurfaceDesc & getDesc() const { return mDesc; }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_D3D9SURFACE_H__
