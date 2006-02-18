#ifndef __GN_D3DAPP_GND3D_H__
#define __GN_D3DAPP_GND3D_H__
// *****************************************************************************
//! \file    GNd3d.h
//! \brief   Public interface for d3d module (collection of common D3D utils).
//! \author  chenlee (2005.8.11)
// *****************************************************************************

#include "GNbase.h"

#if GN_MSWIN

#if GN_DEBUG
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#if GN_XENON
#include <xtl.h>
#include <xgraphics.h>
#include <xboxmath.h>
#elif GN_PC
#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

// Check dx version
#if DIRECT3D_VERSION < 0x0900
#error "directx 9.0 or higher is required!"
#endif

namespace GN
{
    //!
    //! Namespace for d3d module
    //!
    namespace d3d
    {
        //! \name Util functions
        //@{

        //!
        //! Compile vertex shader from string
        //!
        LPDIRECT3DVERTEXSHADER9 compileVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Compile vertex shader from file
        //!
        LPDIRECT3DVERTEXSHADER9 compileVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Assemble vertex shader from string
        //!
        LPDIRECT3DVERTEXSHADER9 assembleVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32_t flags = 0 );

        //!
        //! Assemble vertex shader from file
        //!
        LPDIRECT3DVERTEXSHADER9 assembleVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags = 0 );

        //!
        //! Compile pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 compilePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Compile pixel shader from file
        //!
        LPDIRECT3DPIXELSHADER9 compilePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Assemble pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 assemblePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32_t flags = 0 );

        //!
        //! Assemble pixel shader from file
        //!
        LPDIRECT3DPIXELSHADER9 assemblePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags = 0 );

        //!
        //! Draw screen aligned quad on screen.
        //!
        //! \param dev
        //!     D3D device pointer
        //! \param left, top, right, bottom
        //!     Quad position.
        //!     - On PC platform, position (0,0,1,1) will draw quad that fulfill current viewport,
        //!       when there's no active vertex shader; or you may achieve same effect by using
        //!       a "pass-through" vertex shader.
        //!     - On Xenon, a valid vertex shader must be provided before calling this
        //!       function. Similar with PC case, position (0,0,1,1) will draw quad as large as
        //!       current viewport, if you pass-through vertex position in your shader.
        //!
        //! \param leftU, topV, rightU, bottomV
        //!     Texture coordinates of quad corners.
        //!
        void drawScreenAlignedQuad(
            LPDIRECT3DDEVICE9 dev,
            double left = 0.0, double top = 0.0, double right = 1.0, double bottom = 1.0,
            double leftU = 0.0, double topV = 0.0, double rightU = 1.0, double bottomV = 1.0 );

        //!
        //! Get backbuffer descriptor
        //!
        inline bool getBackBufferDesc( LPDIRECT3DDEVICE9 dev, D3DSURFACE_DESC & desc )
        {
            GN_GUARD_SLOW;
            GN_ASSERT( dev );
            AutoComPtr<IDirect3DSurface9> surf;
            GN_DX_CHECK_RV( dev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &surf ), false );
            GN_DX_CHECK_RV( surf->GetDesc( &desc ), false );
			return true;
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get backbuffer size
        //!
        inline Vector2<uint32_t> getBackBufferSize( LPDIRECT3DDEVICE9 dev )
        {
            GN_GUARD_SLOW;
            D3DSURFACE_DESC desc;
            return getBackBufferDesc( dev, desc )
                ? Vector2<uint32_t>( desc.Width, desc.Height )
                : Vector2<uint32_t>( 0, 0 );
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get depth buffer descriptor
        //!
        inline bool getDepthStencilDesc( LPDIRECT3DDEVICE9 dev, D3DSURFACE_DESC & desc )
        {
            GN_GUARD_SLOW;
            GN_ASSERT( dev );
            AutoComPtr<IDirect3DSurface9> surf;
            GN_DX_CHECK_RV( dev->GetDepthStencilSurface( &surf ), false );
            GN_DX_CHECK_RV( surf->GetDesc( &desc ), false );
			return true;
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get depth buffer size
        //!
        inline Vector2<uint32_t> getDepthStencilSize( LPDIRECT3DDEVICE9 dev )
        {
            GN_GUARD_SLOW;
            D3DSURFACE_DESC desc;
            return getDepthStencilDesc( dev, desc )
                ? Vector2<uint32_t>( desc.Width, desc.Height )
                : Vector2<uint32_t>( 0, 0 );
            GN_UNGUARD_SLOW;
        }
        //@}
    }
}

#endif // GN_MSWIN

// *****************************************************************************
//                           End of GNd3d.h
// *****************************************************************************
#endif // __GN_D3DAPP_GND3D_H__
