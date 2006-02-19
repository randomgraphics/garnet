#ifndef __GN_D3DAPP_GND3D_H__
#define __GN_D3DAPP_GND3D_H__
// *****************************************************************************
//! \file    GNd3d.h
//! \brief   Public interface for d3d module (collection of common D3D utils).
//! \author  chenlee (2005.8.11)
// *****************************************************************************

#include "GNbase.h"
#include "GNgfx.h"

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

namespace GN { namespace gfx
{
    //!
    //! Namespace for d3d module
    //!
    namespace d3d
    {
        //! \name Shader compilation utils
        //@{

        //!
        //! Compile vertex shader from string
        //!
        LPDIRECT3DVERTEXSHADER9 compileVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32_t flags = 0, const char * entryFunc = "main", const char * profile = 0 );

        //!
        //! Compile vertex shader from file
        //!
        LPDIRECT3DVERTEXSHADER9 compileVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags = 0, const char * entryFunc = "main", const char * profile = 0 );

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
        LPDIRECT3DPIXELSHADER9 compilePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32_t flags = 0, const char * entryFunc = "main", const char * profile = 0 );

        //!
        //! Compile pixel shader from file
        //!
        LPDIRECT3DPIXELSHADER9 compilePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags = 0, const char * entryFunc = "main", const char * profile = 0 );

        //!
        //! Assemble pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 assemblePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32_t flags = 0 );

        //!
        //! Assemble pixel shader from file
        //!
        LPDIRECT3DPIXELSHADER9 assemblePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags = 0 );

        //@}

        //! \name color format utils
        //@{

        //!
        //! D3DFORMAT to string. Return "INVALID D3DFORMAT" if failed.
        //!
        const char * d3dFormat2Str( D3DFORMAT );

        //!
        //! Convert D3DFORMAT to ClrFmt. Return FMT_INVALID if failed.
        //!
        ClrFmt d3dFormat2ClrFmt( D3DFORMAT );

        //!
        //! Convert ClrFmt to D3DFORMAT. Return D3DFMT_UNKNOWN if failed
        //!
        D3DFORMAT clrFmt2D3DFormat( ClrFmt );

        //@}

        //! \name Misc. utils
        //@{

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
}}

#endif // GN_MSWIN

// *****************************************************************************
//                           End of GNd3d.h
// *****************************************************************************
#endif // __GN_D3DAPP_GND3D_H__
