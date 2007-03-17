#ifndef __GN_D3DWRAPPER_MYD3D9_H__
#define __GN_D3DWRAPPER_MYD3D9_H__
// *****************************************************************************
/// \file    d3d9Wrapper/myd3d9.h
/// \brief   IDirect3D9 wrapper
/// \author  chenlee (2006.6.24)
// *****************************************************************************

#include "baseInterface.h"

///
/// IDirect3D9 wrapper
///
class MyD3D9 : public BasicInterface<IDirect3D9>
{
public:

    MyD3D9() { addRuntimeType( IID_IDirect3D9, this ); }

    bool create( UINT sdkVersion ); ///< create D3D object

    /// \name IDirect3D9 methods
    //@{
    HRESULT  STDMETHODCALLTYPE RegisterSoftwareDevice(void* pInitializeFunction) { return realobj()->RegisterSoftwareDevice( pInitializeFunction ); }
    UINT     STDMETHODCALLTYPE GetAdapterCount() { return realobj()->GetAdapterCount(); }
    HRESULT  STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier) { return realobj()->GetAdapterIdentifier( Adapter, Flags, pIdentifier ); }
    UINT     STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter,D3DFORMAT Format) { return realobj()->GetAdapterModeCount( Adapter, Format ); }
    HRESULT  STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) { return realobj()->EnumAdapterModes( Adapter, Format, Mode, pMode ); }
    HRESULT  STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE* pMode) { return realobj()->GetAdapterDisplayMode( Adapter, pMode ); }
    HRESULT  STDMETHODCALLTYPE CheckDeviceType(UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) { return realobj()->CheckDeviceType( Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed ); }
    HRESULT  STDMETHODCALLTYPE CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) { return realobj()->CheckDeviceFormat( Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat ); }
    HRESULT  STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) { return realobj()->CheckDeviceMultiSampleType( Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels ); }
    HRESULT  STDMETHODCALLTYPE CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) { return realobj()->CheckDepthStencilMatch( Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat ); }
    HRESULT  STDMETHODCALLTYPE CheckDeviceFormatConversion(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) { return realobj()->CheckDeviceFormatConversion( Adapter, DeviceType, SourceFormat, TargetFormat ); }
    HRESULT  STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) { return realobj()->GetDeviceCaps( Adapter, DeviceType, pCaps ); }
    HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter) { return realobj()->GetAdapterMonitor( Adapter ); }
    HRESULT  STDMETHODCALLTYPE CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface);
    //@}
};

// *****************************************************************************
//                           End of myd3d9.h
// *****************************************************************************
#endif // __GN_D3DWRAPPER_MYD3D9_H__
