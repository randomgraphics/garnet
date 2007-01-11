#ifndef __GN_D3DWRAPPER_MYD3D9_H__
#define __GN_D3DWRAPPER_MYD3D9_H__
// *****************************************************************************
/// \file    d3d9Wrapper/myd3d9.h
/// \brief   IDirect3D9 wrapper
/// \author  chenlee (2006.6.24)
// *****************************************************************************

///
/// IDirect3D9 wrapper
///
class MyD3D9 : public BasicInterface<IDirect3D9>
{
public:

    bool create( UINT sdkVersion ); ///< create D3D object

    /// \name IDirect3D9 methods
    //@{
    HRESULT  STDMETHODCALLTYPE RegisterSoftwareDevice(void* pInitializeFunction) { return obj()->RegisterSoftwareDevice( pInitializeFunction ); }
    UINT     STDMETHODCALLTYPE GetAdapterCount() { return obj()->GetAdapterCount(); }
    HRESULT  STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier) { return obj()->GetAdapterIdentifier( Adapter, Flags, pIdentifier ); }
    UINT     STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter,D3DFORMAT Format) { return obj()->GetAdapterModeCount( Adapter, Format ); }
    HRESULT  STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) { return obj()->EnumAdapterModes( Adapter, Format, Mode, pMode ); }
    HRESULT  STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE* pMode) { return obj()->GetAdapterDisplayMode( Adapter, pMode ); }
    HRESULT  STDMETHODCALLTYPE CheckDeviceType(UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) { return obj()->CheckDeviceType( Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed ); }
    HRESULT  STDMETHODCALLTYPE CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) { return obj()->CheckDeviceFormat( Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat ); }
    HRESULT  STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) { return obj()->CheckDeviceMultiSampleType( Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels ); }
    HRESULT  STDMETHODCALLTYPE CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) { return obj()->CheckDepthStencilMatch( Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat ); }
    HRESULT  STDMETHODCALLTYPE CheckDeviceFormatConversion(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) { return obj()->CheckDeviceFormatConversion( Adapter, DeviceType, SourceFormat, TargetFormat ); }
    HRESULT  STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) { return obj()->GetDeviceCaps( Adapter, DeviceType, pCaps ); }
    HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter) { return obj()->GetAdapterMonitor( Adapter ); }
    HRESULT  STDMETHODCALLTYPE CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface);
    //@}
};

// *****************************************************************************
//                           End of myd3d9.h
// *****************************************************************************
#endif // __GN_D3DWRAPPER_MYD3D9_H__
