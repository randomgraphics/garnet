#ifndef __GN_GFX_D3D10UTILS_H__
#define __GN_GFX_D3D10UTILS_H__
// *****************************************************************************
/// \file    gfx/d3d10utils.h
/// \brief   Public interface for D3D10 module (collection of common D3D utils).
/// \author  chenlee (2006.9.20)
// *****************************************************************************

#if GN_MSWIN && !GN_XENON

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <d3d10.h>

namespace GN { namespace gfx
{
    ///
    /// Namespace for d3d10 module
    ///
    namespace d3d10
    {
        /*! \name Shader compilation utils
        //@{

        ///
        /// Compile vertex shader from string
        ///
        LPDIRECT3DVERTEXSHADER10 compileVS( LPDIRECT3DDEVICE10 dev, const char * code, size_t len = 0, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

        ///
        /// Compile vertex shader from file
        ///
        LPDIRECT3DVERTEXSHADER10 compileVSFromFile( LPDIRECT3DDEVICE10 dev, const char * file, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

        ///
        /// Assemble vertex shader from string
        ///
        LPDIRECT3DVERTEXSHADER10 assembleVS( LPDIRECT3DDEVICE10 dev, const char * code, size_t len = 0, UInt32 flags = 0 );

        ///
        /// Assemble vertex shader from file
        ///
        LPDIRECT3DVERTEXSHADER10 assembleVSFromFile( LPDIRECT3DDEVICE10 dev, const char * file, UInt32 flags = 0 );

        ///
        /// Compile pixel shader from string
        ///
        LPDIRECT3DPIXELSHADER10 compilePS( LPDIRECT3DDEVICE10 dev, const char * code, size_t len = 0, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

        ///
        /// Compile pixel shader from file
        ///
        LPDIRECT3DPIXELSHADER10 compilePSFromFile( LPDIRECT3DDEVICE10 dev, const char * file, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

        ///
        /// Assemble pixel shader from string
        ///
        LPDIRECT3DPIXELSHADER10 assemblePS( LPDIRECT3DDEVICE10 dev, const char * code, size_t len = 0, UInt32 flags = 0 );

        ///
        /// Assemble pixel shader from file
        ///
        LPDIRECT3DPIXELSHADER10 assemblePSFromFile( LPDIRECT3DDEVICE10 dev, const char * file, UInt32 flags = 0 );

        //@}*/

        /// \name color format utils
        //@{

        ///
        /// DXGI_FORMAT to string. Return "INVALID DXGI_FORMAT" if failed.
        ///
        const char * dxgiFormat2Str( DXGI_FORMAT );

        ///
        /// Convert DXGI_FORMAT to ClrFmt. Return FMT_INVALID if failed.
        ///
        ClrFmt dxgiFormat2ClrFmt( DXGI_FORMAT );

        ///
        /// Convert ClrFmt to DXGI_FORMAT. Return DXGI_FORMAT_UNKNOWN if failed.
        ///
        /// Note 'tiled' parameter only used for Xenon platform.
        ///
        DXGI_FORMAT clrFmt2DxgiFormat( ClrFmt );

        //@}

        /*! \name Misc. utils
        //@{

        ///
        /// Get backbuffer descriptor
        ///
        inline bool getBackBufferDesc( LPDIRECT3DDEVICE10 dev, D3DSURFACE_DESC & desc )
        {
            GN_GUARD_SLOW;
            GN_ASSERT( dev );
            AutoComPtr<IDirect3DSurface10> surf;
            GN_DX10_CHECK_RV( dev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &surf ), false );
            GN_DX10_CHECK_RV( surf->GetDesc( &desc ), false );
			return true;
            GN_UNGUARD_SLOW;
        }

        ///
        /// Get backbuffer size
        ///
        inline Vector2<UInt32> getBackBufferSize( LPDIRECT3DDEVICE10 dev )
        {
            GN_GUARD_SLOW;
            D3DSURFACE_DESC desc;
            return getBackBufferDesc( dev, desc )
                ? Vector2<UInt32>( desc.Width, desc.Height )
                : Vector2<UInt32>( 0, 0 );
            GN_UNGUARD_SLOW;
        }

        ///
        /// Get depth buffer descriptor
        ///
        inline bool getDepthStencilDesc( LPDIRECT3DDEVICE10 dev, D3DSURFACE_DESC & desc )
        {
            GN_GUARD_SLOW;
            GN_ASSERT( dev );
            AutoComPtr<IDirect3DSurface10> surf;
            GN_DX10_CHECK_RV( dev->GetDepthStencilSurface( &surf ), false );
            GN_DX10_CHECK_RV( surf->GetDesc( &desc ), false );
			return true;
            GN_UNGUARD_SLOW;
        }

        ///
        /// Get depth buffer size
        ///
        inline Vector2<UInt32> getDepthStencilSize( LPDIRECT3DDEVICE10 dev )
        {
            GN_GUARD_SLOW;
            D3DSURFACE_DESC desc;
            return getDepthStencilDesc( dev, desc )
                ? Vector2<UInt32>( desc.Width, desc.Height )
                : Vector2<UInt32>( 0, 0 );
            GN_UNGUARD_SLOW;
        }

        //@}*/
    }
}}

#endif // GN_MSWIN

// *****************************************************************************
//                           End of gfx/d3d10utils.h
// *****************************************************************************
#endif // __GN_GFX_D3D10UTILS_H__