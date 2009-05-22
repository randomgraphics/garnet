#ifndef __GN_TEST_GFX_COMMON_H__
#define __GN_TEST_GFX_COMMON_H__
// *****************************************************************************
// \file    common.h
// \brief   common header of renderer tests
// \author  chenlee (2005.10.7)
// *****************************************************************************

#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include "garnet/GNwin.h"

/// \cond

class RendererTest
{
    union RenderTargetPixel
    {
        float  f32[4];
        UInt32 u32[4];
        SInt32 s32[4];
        UInt16 u16[8];
        UInt8  u8[16];
        struct
        {
            UInt8 r, g, b, a;
        } rgba8888;
    };

    /// Get pixel value of specific color render target at specific position.
    /// \note
    ///     - position (0,0) is left-top corner of the render target
    ///     - this function may crash because of invalid parameters.
    RenderTargetPixel getRenderTargetPixel( GN::gfx::Renderer & r, size_t rtidx, size_t x, size_t y )
    {
        using namespace GN;
        using namespace GN::gfx;

        const RendererContext & rc = r.getContext();

        TS_ASSERT( rc.colortargets.size() > rtidx );
        TS_ASSERT( rc.colortargets[rtidx].texture );

        const RenderTargetTexture & rtt = rc.colortargets[rtidx];

        RenderTargetPixel rtp;
        rtp.s32[0] = rand();
        rtp.s32[1] = rand();
        rtp.s32[2] = rand();
        rtp.s32[3] = rand();

        const Vector3<UInt32> & rtsize = rtt.texture->getMipSize( rtt.level );
        if( x >= rtsize.x && y >= rtsize.y )
        {
            TS_ASSERT( 0 );
            return rtp;
        }

        MipmapData md;
        rtt.texture->readMipmap( rtt.face, rtt.level, md );

        size_t bytesPerPixel = rtt.texture->getDesc().format.getBytesPerBlock();
        size_t srcOffset = md.slicePitch * rtt.slice + md.rowPitch * y + x * bytesPerPixel;
        if( srcOffset >= md.data.size() )
        {
            TS_ASSERT( 0 );
            return rtp;
        }
        const UInt8 * src = &md.data[srcOffset];
        size_t copiedBytes = math::getmin( md.data.size() - srcOffset, sizeof(rtp) );
        memcpy( &rtp, src, copiedBytes );

        return rtp;
    }

    /// Get backbuffer pixel at specific position.
    /// \note
    ///     - position (0,0) is left-top corner of the back buffer
    ///     - this function may crash because of invalid parameters.
    RenderTargetPixel getBackBufferPixel( GN::gfx::Renderer & r, size_t x, size_t y )
    {
        GN_UNUSED_PARAM( r );
        GN_UNUSED_PARAM( x );
        GN_UNUSED_PARAM( y );

        RenderTargetPixel rtp;

        rtp.s32[0] = rand();
        rtp.s32[1] = rand();
        rtp.s32[2] = rand();
        rtp.s32[3] = rand();

        return rtp;
    }

    GN::gfx::Renderer *
    createRenderer( const GN::gfx::RendererOptions & ro )
    {
        GN::gfx::RendererOptions effectiveRO = ro;
        effectiveRO.api = mAPI;

        if( mMultiThreading )
        {
            return GN::gfx::createMultiThreadRenderer( effectiveRO );
        }
        else
        {
            return GN::gfx::createSingleThreadRenderer( effectiveRO );
        }
    }

    class AutoRenderer
    {
        GN::gfx::Renderer * mRndr;

    public:

        AutoRenderer( GN::gfx::Renderer * r ) : mRndr( r )
        {
        }

        ~AutoRenderer()
        {
            if( mRndr ) GN::gfx::deleteRenderer( mRndr );
        }

        operator GN::gfx::Renderer *() const { return mRndr; }

        GN::gfx::Renderer *operator->() const { return mRndr; }
    };

    GN::gfx::RendererAPI mAPI;
    bool                 mMultiThreading;

protected :

    /// ctor
    RendererTest( GN::gfx::RendererAPI api, bool multithreading ) : mAPI( api ), mMultiThreading( multithreading )
    {
    }

    void externalWindow()
    {
        using namespace GN;
        using namespace GN::gfx;

        GN::AutoObjPtr<win::Window> win;
        win.attach( win::createWindow( win::WCP_WINDOWED_RENDER_WINDOW ) );
        TS_ASSERT( !win.empty() );
        if( win.empty() ) return;
        win->resize( 511, 236 );
        win->show();

        RendererOptions ro;
        ro.useExternalWindow = true;
        ro.displayHandle     = win->getDisplayHandle();
        ro.renderWindow      = win->getWindowHandle();
        AutoRenderer rndr( createRenderer( ro ) );
        TS_ASSERT( rndr );
        if( !rndr ) return;
        Renderer & r = *rndr;

        r.clearScreen( Vector4f(1,1,0,1) );

        RenderTargetPixel rtp = getBackBufferPixel( r, 0, 0 );
        TS_ASSERT_EQUALS( rtp.u32[0], 0xFF00FFFF );
    }
};

/// \endcond

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_TEST_GFX_COMMON_H__
