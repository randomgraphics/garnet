#ifndef __GN_D3D9WRAPPER_MYDEVICE9_H__
#define __GN_D3D9WRAPPER_MYDEVICE9_H__
// *****************************************************************************
//! \file    d3d9Wrapper/mydevice9.h
//! \brief   IDirect3DDevice9 wrapper
//! \author  chenlee (2006.6.24)
// *****************************************************************************

class MyD3D9;

class MyDevice9 : public BasicInterface<IDirect3DDevice9>
{
    MyD3D9 * mD3D9;

    ~MyDevice9() { GN::safeRelease( mD3D9 ); }

public:

    MyDevice9() : mD3D9(0) {}

    HRESULT create(
        MyD3D9 * d3d,
        UINT Adapter,
        D3DDEVTYPE DeviceType,
        HWND hFocusWindow,
        DWORD BehaviorFlags,
        D3DPRESENT_PARAMETERS* pPresentationParameters );

    STDMETHOD(TestCooperativeLevel)(THIS) { return obj()->TestCooperativeLevel(); }
    STDMETHOD_(UINT, GetAvailableTextureMem)(THIS) { return obj()->GetAvailableTextureMem(); }
    STDMETHOD(EvictManagedResources)(THIS) { return obj()->EvictManagedResources(); }
    STDMETHOD(GetDirect3D)(THIS_ IDirect3D9** ppD3D9);
    STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps) { return obj()->GetDeviceCaps( pCaps ); }
    STDMETHOD(GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode) { return obj()->GetDisplayMode( iSwapChain, pMode ); }
    STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters) { return obj()->GetCreationParameters( pParameters ); }
    STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap) { return obj()->SetCursorProperties( XHotSpot, YHotSpot, pCursorBitmap ); }
    STDMETHOD_(void, SetCursorPosition)(THIS_ int X,int Y,DWORD Flags) { return obj()->SetCursorPosition( X, Y, Flags ); }
    STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow) { return obj()->ShowCursor( bShow ); }
    STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain) { return obj()->CreateAdditionalSwapChain( pPresentationParameters, pSwapChain ); }
    STDMETHOD(GetSwapChain)(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain) { return obj()->GetSwapChain( iSwapChain, pSwapChain ); }
    STDMETHOD_(UINT, GetNumberOfSwapChains)(THIS) { return obj()->GetNumberOfSwapChains(); }
    STDMETHOD(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) { return obj()->Reset( pPresentationParameters ); }
    STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) { return obj()->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion ); }
    STDMETHOD(GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) { return obj()->GetBackBuffer( iSwapChain, iBackBuffer, Type, ppBackBuffer ); }
    STDMETHOD(GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus ) { return obj()->GetRasterStatus( iSwapChain, pRasterStatus ); }
    STDMETHOD(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) { return obj()->SetDialogBoxMode( bEnableDialogs ); }
    STDMETHOD_(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp) { return obj()->SetGammaRamp( iSwapChain, Flags, pRamp ); }
    STDMETHOD_(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp) { return obj()->GetGammaRamp( iSwapChain, pRamp ); }
    STDMETHOD(CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) { return obj()->CreateTexture( Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle ); }
    STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) { return obj()->CreateVolumeTexture( Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle ); }
    STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) { return obj()->CreateCubeTexture( EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle ); }
    STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) { return obj()->CreateVertexBuffer( Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle ); }
    STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) { return obj()->CreateIndexBuffer( Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle ); }
    STDMETHOD(CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) { return obj()->CreateRenderTarget( Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle ); }
    STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) { return obj()->CreateDepthStencilSurface( Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle ); }
    STDMETHOD(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) { return obj()->UpdateSurface( pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint ); }
    STDMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture) { return obj()->UpdateTexture( pSourceTexture, pDestinationTexture ); }
    STDMETHOD(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) { return obj()->GetRenderTargetData( pRenderTarget, pDestSurface ); }
    STDMETHOD(GetFrontBufferData)(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface) { return obj()->GetFrontBufferData( iSwapChain, pDestSurface ); }
    STDMETHOD(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) { return obj()->StretchRect( pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter ); }
    STDMETHOD(ColorFill)(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color) { return obj()->ColorFill( pSurface, pRect, color ); }
    STDMETHOD(CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) { return obj()->CreateOffscreenPlainSurface( Width, Height, Format, Pool, ppSurface, pSharedHandle ); }
    STDMETHOD(SetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget) { return obj()->SetRenderTarget( RenderTargetIndex, pRenderTarget ); }
    STDMETHOD(GetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) { return obj()->GetRenderTarget( RenderTargetIndex, ppRenderTarget ); }
    STDMETHOD(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) { return obj()->SetDepthStencilSurface( pNewZStencil ); }
    STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface) { return obj()->GetDepthStencilSurface( ppZStencilSurface ); }
    STDMETHOD(BeginScene)(THIS) { return obj()->BeginScene(); }
    STDMETHOD(EndScene)(THIS) { return obj()->EndScene(); }
    STDMETHOD(Clear)(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil) { return obj()->Clear( Count, pRects, Flags, Color, Z, Stencil ); }
    STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix) { return obj()->SetTransform( State, pMatrix ); }
    STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix ) { return obj()->GetTransform( State, pMatrix ); }
    STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE Type,CONST D3DMATRIX* pMatrix ) { return obj()->MultiplyTransform( Type, pMatrix ); }
    STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport) { return obj()->SetViewport( pViewport ); }
    STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT9* pViewport) { return obj()->GetViewport( pViewport ); }
    STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) { return obj()->SetMaterial( pMaterial ); }
    STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) { return obj()->GetMaterial( pMaterial ); }
    STDMETHOD(SetLight)(THIS_ DWORD Index,CONST D3DLIGHT9* pLight) { return obj()->SetLight( Index, pLight ); }
    STDMETHOD(GetLight)(THIS_ DWORD Index,D3DLIGHT9* pLight) { return obj()->GetLight( Index, pLight ); }
    STDMETHOD(LightEnable)(THIS_ DWORD Index,BOOL Enable) { return obj()->LightEnable( Index, Enable ); }
    STDMETHOD(GetLightEnable)(THIS_ DWORD Index,BOOL* pEnable) {  return obj()->GetLightEnable( Index, pEnable ); }
    STDMETHOD(SetClipPlane)(THIS_ DWORD Index,CONST float* pPlane) { return obj()->SetClipPlane( Index, pPlane ); }
    STDMETHOD(GetClipPlane)(THIS_ DWORD Index,float* pPlane) { return obj()->GetClipPlane( Index, pPlane ); }
    STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value) { return obj()->SetRenderState( State, Value ); }
    STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue) { return obj()->GetRenderState( State, pValue ); }
    STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) { return obj()->CreateStateBlock( Type, ppSB ); }
    STDMETHOD(BeginStateBlock)(THIS) { return obj()->BeginStateBlock(); }
    STDMETHOD(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) { return obj()->EndStateBlock( ppSB ); }
    STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) { return obj()->SetClipStatus( pClipStatus ); }
    STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) { return obj()->GetClipStatus( pClipStatus ); }
    STDMETHOD(GetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9** ppTexture) { return obj()->GetTexture( Stage, ppTexture ); }
    STDMETHOD(SetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9* pTexture) { return obj()->SetTexture( Stage, pTexture ); }
    STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue) { return obj()->GetTextureStageState( Stage, Type, pValue ); }
    STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value) { return obj()->SetTextureStageState( Stage, Type, Value ); }
    STDMETHOD(GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) { return obj()->GetSamplerState( Sampler, Type, pValue ); }
    STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) { return obj()->SetSamplerState( Sampler, Type, Value ); }
    STDMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses) { return obj()->ValidateDevice( pNumPasses ); }
    STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries) { return obj()->SetPaletteEntries( PaletteNumber, pEntries ); }
    STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries) { return obj()->GetPaletteEntries( PaletteNumber, pEntries ); }
    STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) { return obj()->SetCurrentTexturePalette( PaletteNumber ); }
    STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber) { return obj()->GetCurrentTexturePalette( PaletteNumber ); }
    STDMETHOD(SetScissorRect)(THIS_ CONST RECT* pRect) { return obj()->SetScissorRect( pRect ); }
    STDMETHOD(GetScissorRect)(THIS_ RECT* pRect) { return obj()->GetScissorRect( pRect ); }
    STDMETHOD(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) { return obj()->SetSoftwareVertexProcessing( bSoftware ); }
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)(THIS) { return obj()->GetSoftwareVertexProcessing(); }
    STDMETHOD(SetNPatchMode)(THIS_ float nSegments) { return obj()->SetNPatchMode( nSegments ); }
    STDMETHOD_(float, GetNPatchMode)(THIS) { return obj()->GetNPatchMode(); }
    STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) { return obj()->DrawPrimitive( PrimitiveType, StartVertex, PrimitiveCount ); }
    STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) { return obj()->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount ); }
    STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) { return obj()->DrawPrimitiveUP( PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride ); }
    STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) { return obj()->DrawIndexedPrimitiveUP( PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride ); }
    STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) { return obj()->ProcessVertices( SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags ); }
    STDMETHOD(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) { return obj()->CreateVertexDeclaration( pVertexElements, ppDecl ); }
    STDMETHOD(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) { return obj()->SetVertexDeclaration( pDecl ); }
    STDMETHOD(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) { return obj()->GetVertexDeclaration( ppDecl ); }
    STDMETHOD(SetFVF)(THIS_ DWORD FVF) { return obj()->SetFVF( FVF ); }
    STDMETHOD(GetFVF)(THIS_ DWORD* pFVF) { return obj()->GetFVF( pFVF ); }
    STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) { return obj()->CreateVertexShader( pFunction, ppShader ); }
    STDMETHOD(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader) { return obj()->SetVertexShader( pShader ); }
    STDMETHOD(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader) { return obj()->GetVertexShader( ppShader ); }
    STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) { return obj()->SetVertexShaderConstantF( StartRegister, pConstantData, Vector4fCount ); }
    STDMETHOD(GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) { return obj()->GetVertexShaderConstantF( StartRegister, pConstantData, Vector4fCount ); }
    STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) { return obj()->SetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount ); }
    STDMETHOD(GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) { return obj()->GetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount ); }
    STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) { return obj()->SetVertexShaderConstantB( StartRegister, pConstantData, BoolCount ); }
    STDMETHOD(GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) { return obj()->GetVertexShaderConstantB( StartRegister, pConstantData, BoolCount ); }
    STDMETHOD(SetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) { return obj()->SetStreamSource( StreamNumber, pStreamData, OffsetInBytes, Stride ); }
    STDMETHOD(GetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride) { return obj()->GetStreamSource( StreamNumber, ppStreamData, pOffsetInBytes, pStride ); }
    STDMETHOD(SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Setting) { return obj()->SetStreamSourceFreq( StreamNumber, Setting ); }
    STDMETHOD(GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* pSetting) { return obj()->GetStreamSourceFreq( StreamNumber, pSetting ); }
    STDMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) { return obj()->SetIndices( pIndexData ); }
    STDMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) { return obj()->GetIndices( ppIndexData ); }
    STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) { return obj()->CreatePixelShader( pFunction, ppShader ); }
    STDMETHOD(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader) { return obj()->SetPixelShader( pShader ); }
    STDMETHOD(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader) { return obj()->GetPixelShader( ppShader ); }
    STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) { return obj()->SetPixelShaderConstantF( StartRegister, pConstantData, Vector4fCount ); }
    STDMETHOD(GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) { return obj()->GetPixelShaderConstantF( StartRegister, pConstantData, Vector4fCount ); }
    STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) { return obj()->SetPixelShaderConstantI( StartRegister, pConstantData, Vector4iCount ); }
    STDMETHOD(GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) { return obj()->GetPixelShaderConstantI( StartRegister, pConstantData, Vector4iCount ); }
    STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) { return obj()->SetPixelShaderConstantB( StartRegister, pConstantData, BoolCount ); }
    STDMETHOD(GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) { return obj()->GetPixelShaderConstantB( StartRegister, pConstantData, BoolCount ); }
    STDMETHOD(DrawRectPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo) { return obj()->DrawRectPatch( Handle, pNumSegs, pRectPatchInfo ); }
    STDMETHOD(DrawTriPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo) { return obj()->DrawTriPatch( Handle, pNumSegs, pTriPatchInfo ); }
    STDMETHOD(DeletePatch)(THIS_ UINT Handle) { return obj()->DeletePatch( Handle ); }
    STDMETHOD(CreateQuery)(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) { return obj()->CreateQuery( Type, ppQuery ); }
};

// *****************************************************************************
//                           End of mydevice9.h
// *****************************************************************************
#endif // __GN_D3D9WRAPPER_MYDEVICE9_H__
