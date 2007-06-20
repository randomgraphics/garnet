#ifndef __GN_GFX2_D3D9SURFACE_H__
#define __GN_GFX2_D3D9SURFACE_H__
// *****************************************************************************
//! \file    gfx2/d3d9Surface.h
//! \brief   d3d9 base surface interface
//! \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// d3d9 effect port type
    ///
    enum D3D9SurfaceType
    {
        //@{
        SURFACE_TYPE_VB,
        SURFACE_TYPE_IB,
        SURFACE_TYPE_TEX_2D,
        SURFACE_TYPE_TEX_3D,
        SURFACE_TYPE_TEX_CUBE,
        SURFACE_TYPE_RTT_2D,    ///< 2D render target texture
        SURFACE_TYPE_RTT_CUBE,  ///< 2D cube texture
        SURFACE_TYPE_RTS_COLOR, ///< render target surface
        SURFACE_TYPE_RTS_DEPTH, ///< depth stencil surface
        SURFACE_TYPE_ANY = -1,  ///< indicate any kind of surfaces.
        //@}
    };

    ///
    /// convert surface type to string
    ///
    inline const char * d3d9SurfaceType2Str( int type )
    {
        using namespace GN::gfx2;

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
        if( 0 <= type && type <= SURFACE_TYPE_RTS_DEPTH ) return table[type];
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

    public:

        ///
        /// ctor
        ///
        D3D9Surface( const D3D9SurfaceDesc & desc ) : mDesc(desc) {}

        // from effect
        virtual const SurfaceDesc & getDesc() const { return mDesc; }
    };
}}

// *****************************************************************************
//                           End of d3d9Surface.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9SURFACE_H__
