#ifndef __GN_GFX2_D3D9SURFACE_H__
#define __GN_GFX2_D3D9SURFACE_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 base surface interface
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// d3d9 effect port type
    ///
    enum D3D9SurfaceType
    {
        //@{
        D3D9_SURFACE_TYPE_VB,
        D3D9_SURFACE_TYPE_IB,
        D3D9_SURFACE_TYPE_TEX,       ///< texture, no matter 2D, 3D or cube.
        D3D9_SURFACE_TYPE_TEX_2D,
        D3D9_SURFACE_TYPE_TEX_3D,
        D3D9_SURFACE_TYPE_TEX_CUBE,
        D3D9_SURFACE_TYPE_RTT_2D,    ///< 2D render target texture
        D3D9_SURFACE_TYPE_RTT_CUBE,  ///< 2D cube texture
        D3D9_SURFACE_TYPE_RTS_COLOR, ///< render target surface
        D3D9_SURFACE_TYPE_RTS_DEPTH, ///< depth stencil surface
        D3D9_SURFACE_TYPE_ANY = -1,  ///< indicate any kind of surfaces.
        //@}
    };

    ///
    /// convert surface type to string
    ///
    inline const char * d3d9SurfaceType2Str( int type )
    {
        using namespace GN::gfx;

        static const char * table[] =
        {
            "VB",
            "IB",
            "TEX_2D",
            "TEX_3D",
            "TEX_CUBE",
            "RTT_2D",
            "RTT_CUBE",
            "RTS_COLOR",
            "RTS_DEPTH",
            "RTS_BACKBUF",
        };
        if( 0 <= type && type <= D3D9_SURFACE_TYPE_RTS_DEPTH ) return table[type];
        if( -1 == type ) return "ANY";
        return "INVALID";
    }

    ///
    /// D3D9 surface descriptor
    ///
    struct D3D9SurfaceDesc : public SurfaceDesc
    {
        //@{
        D3D9SurfaceType type;
        //@}
    };

    ///
    /// base D3D9 surface
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9Surface : public Surface
    {
        D3D9SurfaceDesc mDesc;

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
        D3D9Surface( const D3D9SurfaceDesc & desc ) : mDesc(desc) {}

        // from effect
        virtual const SurfaceDesc & getDesc() const { return mDesc; }

        ///
        /// get D3D9 surface descriptor
        ///
        const D3D9SurfaceDesc & getD3D9Desc() const { return mDesc; }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9SURFACE_H__
