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
    RenderTargetPixel getRenderTargetPixel( GN::gfx::Gpu & r, size_t rtidx, size_t x, size_t y )
    {
        using namespace GN;
        using namespace GN::gfx;

        const GpuContext & rc = r.GetContext();

        TS_ASSERT( rc.colortargets.Size() > rtidx );
        TS_ASSERT( rc.colortargets[rtidx].texture );

        const RenderTargetTexture & rtt = rc.colortargets[rtidx];

        RenderTargetPixel rtp;
        rtp.s32[0] = rand();
        rtp.s32[1] = rand();
        rtp.s32[2] = rand();
        rtp.s32[3] = rand();

        const Vector3<UInt32> & rtsize = rtt.texture->GetMipSize( rtt.level );
        if( x >= rtsize.x && y >= rtsize.y )
        {
            TS_ASSERT( 0 );
            return rtp;
        }

        MipmapData md;
        rtt.texture->ReadMipmap( rtt.face, rtt.level, md );

        size_t bytesPerPixel = rtt.texture->GetDesc().format.GetBytesPerBlock();
        size_t srcOffset = md.slicePitch * rtt.slice + md.rowPitch * y + x * bytesPerPixel;
        if( srcOffset >= md.data.Size() )
        {
            TS_ASSERT( 0 );
            return rtp;
        }
        const UInt8 * src = &md.data[srcOffset];
        size_t copiedBytes = math::GetMin( md.data.Size() - srcOffset, sizeof(rtp) );
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
        r.GetBackBufferContent( bc );

        TS_ASSERT( x < bc.width && y < bc.height );
        if( x >= bc.width && y >= bc.height ) return rtp;

        size_t bytesPerPixel = bc.format.GetBytesPerBlock();
        size_t srcOffset = bc.pitch * y + x * bytesPerPixel;
        TS_ASSERT( srcOffset < bc.data.Size() );
        if( srcOffset >= bc.data.Size() ) return rtp;
        const UInt8 * src = &bc.data[srcOffset];
        size_t copiedBytes = math::GetMin( bc.data.Size() - srcOffset, sizeof(rtp) );
        memcpy( &rtp, src, copiedBytes );

        return rtp;
    }

    GN::gfx::Gpu *
    createGpu( const GN::gfx::GpuOptions & ro )
    {
        GN::gfx::GpuOptions effectiveRO = ro;
        effectiveRO.api = mAPI;

        if( mMultiThreading )
        {
            return GN::gfx::CreateMultiThreadGpu( effectiveRO );
        }
        else
        {
            return GN::gfx::CreateSingleThreadGpu( effectiveRO );
        }
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
            if( mGpu ) GN::gfx::DeleteGpu( mGpu );
        }

        operator GN::gfx::Gpu *() const { return mGpu; }

        GN::gfx::Gpu *operator->() const { return mGpu; }
    };

    GN::gfx::GpuAPI mAPI;
    bool                 mMultiThreading;

protected :

    /// ctor
    GpuTest( GN::gfx::GpuAPI api, bool multithreading ) : mAPI( api ), mMultiThreading( multithreading )
    {
    }

    void externalWindow()
    {
        using namespace GN;
        using namespace GN::gfx;

        GN::AutoObjPtr<win::Window> win;
        win.Attach( win::NewWindow( win::WCP_WINDOWED_RENDER_WINDOW ) );
        TS_ASSERT( !win.Empty() );
        if( win.Empty() ) return;
        win->SetClientSize( 511, 236 );
        win->Show();

        GpuOptions ro;
        ro.useExternalWindow = true;
        ro.displayHandle     = win->GetDisplayHandle();
        ro.renderWindow      = win->GetWindowHandle();
        AutoGpu gpu( createGpu( ro ) );
        TS_ASSERT( gpu );
        if( !gpu ) return;
        Gpu & r = *gpu;

        // the renderer should be in same size as the external window client
        const DispDesc & dd = r.GetDispDesc();
        TS_ASSERT_EQUALS( dd.width, 511 );
        TS_ASSERT_EQUALS( dd.height, 236 );

        r.ClearScreen( Vector4f(1,1,0,1) );

        RenderTargetPixel rtp = getBackBufferPixel( r, 0, 0 );
        TS_ASSERT_EQUALS( rtp.u32[0], 0xFF00FFFF );
    }
};

/// \endcond

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_TEST_GFX_COMMON_H__
