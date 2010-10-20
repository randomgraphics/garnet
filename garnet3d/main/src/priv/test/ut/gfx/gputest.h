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

class GpuTest
{
    union RenderTargetPixel
    {
        float  f32[4];
        uint32 u32[4];
        sint32 s32[4];
        uint16 u16[8];
        uint8  u8[16];
        struct
        {
            uint8 r, g, b, a;
        } rgba8888;
    };

    /// Get pixel value of specific color render target at specific position.
    /// \note
    ///     - position (0,0) is left-top corner of the render target
    ///     - this function may crash because of invalid parameters.
    RenderTargetPixel getRenderTargetPixel( GN::gfx::Gpu & r, size_t rtidx, size_t x, size_t y )
    {
        using namespace GN;
        using namespace GN::gfx;

        const GpuContext & rc = r.getContext();

        TS_ASSERT( rc.colortargets.size() > rtidx );
        TS_ASSERT( rc.colortargets[rtidx].texture );

        const RenderTargetTexture & rtt = rc.colortargets[rtidx];

        RenderTargetPixel rtp;
        rtp.s32[0] = rand();
        rtp.s32[1] = rand();
        rtp.s32[2] = rand();
        rtp.s32[3] = rand();

        const Vector3<uint32> & rtsize = rtt.texture->getMipSize( rtt.level );
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
        const uint8 * src = &md.data[srcOffset];
        size_t copiedBytes = math::getmin( md.data.size() - srcOffset, sizeof(rtp) );
        memcpy( &rtp, src, copiedBytes );

        return rtp;
    }

    /// Get backbuffer pixel at specific position.
    /// \note
    ///     - position (0,0) is left-top corner of the back buffer
    ///     - this function may crash because of invalid parameters.
    RenderTargetPixel getBackBufferPixel( GN::gfx::Gpu & r, size_t x, size_t y )
    {
        using namespace GN;
        using namespace GN::gfx;

        RenderTargetPixel rtp;
        rtp.s32[0] = rand();
        rtp.s32[1] = rand();
        rtp.s32[2] = rand();
        rtp.s32[3] = rand();

        Gpu::BackBufferContent bc;
        r.getBackBufferContent( bc );

        TS_ASSERT( x < bc.width && y < bc.height );
        if( x >= bc.width && y >= bc.height ) return rtp;

        size_t bytesPerPixel = bc.format.getBytesPerBlock();
        size_t srcOffset = bc.pitch * y + x * bytesPerPixel;
        TS_ASSERT( srcOffset < bc.data.size() );
        if( srcOffset >= bc.data.size() ) return rtp;
        const uint8 * src = &bc.data[srcOffset];
        size_t copiedBytes = math::getmin( bc.data.size() - srcOffset, sizeof(rtp) );
        memcpy( &rtp, src, copiedBytes );

        return rtp;
    }

    GN::gfx::Gpu *
    createGpu( const GN::gfx::GpuOptions & ro )
    {
        GN::gfx::GpuOptions effectiveRO = ro;
        effectiveRO.api = mAPI;
        return GN::gfx::createGpu( effectiveRO, mCreationFlags );
    }

    class AutoGpu
    {
        GN::gfx::Gpu * mGpu;

    public:

        AutoGpu( GN::gfx::Gpu * r ) : mGpu( r )
        {
        }

        ~AutoGpu()
        {
            if( mGpu ) GN::gfx::deleteGpu( mGpu );
        }

        operator GN::gfx::Gpu *() const { return mGpu; }

        GN::gfx::Gpu *operator->() const { return mGpu; }
    };

    GN::gfx::GpuAPI mAPI;
    UINT            mCreationFlags;

protected :

    /// ctor
    GpuTest( GN::gfx::GpuAPI api, bool multithreading )
        : mAPI( api )
        , mCreationFlags( multithreading ? GN::gfx::GPU_CREATION_MULTIPLE_THREADS : 0 )
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
        win->setClientSize( 511, 236 );
        win->show();

        GpuOptions ro;
        ro.useExternalWindow = true;
        ro.displayHandle     = win->getDisplayHandle();
        ro.renderWindow      = win->getWindowHandle();
        AutoGpu gpu( createGpu( ro ) );
        TS_ASSERT( gpu );
        if( !gpu ) return;
        Gpu & r = *gpu;

        // the renderer should be in same size as the external window client
        const DispDesc & dd = r.getDispDesc();
        TS_ASSERT_EQUALS( dd.width, 511 );
        TS_ASSERT_EQUALS( dd.height, 236 );

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
