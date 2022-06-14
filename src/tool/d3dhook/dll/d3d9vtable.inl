// script generated file. Do _NOT_ edit.

// -----------------------------------------------------------------------------
// IDirect3D9
// -----------------------------------------------------------------------------
struct IDirect3D9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3D9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3D9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3D9 * this_);
    HRESULT(STDMETHODCALLTYPE * RegisterSoftwareDevice)(IDirect3D9 * this_, void * pInitializeFunction);
    UINT(STDMETHODCALLTYPE * GetAdapterCount)(IDirect3D9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetAdapterIdentifier)(IDirect3D9 * this_, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 * pIdentifier);
    UINT(STDMETHODCALLTYPE * GetAdapterModeCount)(IDirect3D9 * this_, UINT Adapter, D3DFORMAT Format);
    HRESULT(STDMETHODCALLTYPE * EnumAdapterModes)(IDirect3D9 * this_, UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE * pMode);
    HRESULT(STDMETHODCALLTYPE * GetAdapterDisplayMode)(IDirect3D9 * this_, UINT Adapter, D3DDISPLAYMODE * pMode);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceType)
    (IDirect3D9 * this_, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceFormat)
    (IDirect3D9 * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceMultiSampleType)
    (IDirect3D9 * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType,
     DWORD * pQualityLevels);
    HRESULT(STDMETHODCALLTYPE * CheckDepthStencilMatch)
    (IDirect3D9 * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceFormatConversion)
    (IDirect3D9 * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
    HRESULT(STDMETHODCALLTYPE * GetDeviceCaps)(IDirect3D9 * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 * pCaps);
    HMONITOR(STDMETHODCALLTYPE * GetAdapterMonitor)(IDirect3D9 * this_, UINT Adapter);
    HRESULT(STDMETHODCALLTYPE * CreateDevice)
    (IDirect3D9 * this_, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters,
     IDirect3DDevice9 ** ppReturnedDeviceInterface);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DDevice9
// -----------------------------------------------------------------------------
struct IDirect3DDevice9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DDevice9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DDevice9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * TestCooperativeLevel)(IDirect3DDevice9 * this_);
    UINT(STDMETHODCALLTYPE * GetAvailableTextureMem)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * EvictManagedResources)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDirect3D)(IDirect3DDevice9 * this_, IDirect3D9 ** ppD3D9);
    HRESULT(STDMETHODCALLTYPE * GetDeviceCaps)(IDirect3DDevice9 * this_, D3DCAPS9 * pCaps);
    HRESULT(STDMETHODCALLTYPE * GetDisplayMode)(IDirect3DDevice9 * this_, UINT iSwapChain, D3DDISPLAYMODE * pMode);
    HRESULT(STDMETHODCALLTYPE * GetCreationParameters)(IDirect3DDevice9 * this_, D3DDEVICE_CREATION_PARAMETERS * pParameters);
    HRESULT(STDMETHODCALLTYPE * SetCursorProperties)(IDirect3DDevice9 * this_, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 * pCursorBitmap);
    void(STDMETHODCALLTYPE * SetCursorPosition)(IDirect3DDevice9 * this_, int X, int Y, DWORD Flags);
    BOOL(STDMETHODCALLTYPE * ShowCursor)(IDirect3DDevice9 * this_, BOOL bShow);
    HRESULT(STDMETHODCALLTYPE * CreateAdditionalSwapChain)
    (IDirect3DDevice9 * this_, D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DSwapChain9 ** ppSwapChain);
    HRESULT(STDMETHODCALLTYPE * GetSwapChain)(IDirect3DDevice9 * this_, UINT iSwapChain, IDirect3DSwapChain9 ** ppSwapChain);
    UINT(STDMETHODCALLTYPE * GetNumberOfSwapChains)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * Reset)(IDirect3DDevice9 * this_, D3DPRESENT_PARAMETERS * pPresentationParameters);
    HRESULT(STDMETHODCALLTYPE * Present)
    (IDirect3DDevice9 * this_, CONST RECT * pSourceRect, CONST RECT * pDestRect, HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion);
    HRESULT(STDMETHODCALLTYPE * GetBackBuffer)
    (IDirect3DDevice9 * this_, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 ** ppBackBuffer);
    HRESULT(STDMETHODCALLTYPE * GetRasterStatus)(IDirect3DDevice9 * this_, UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus);
    HRESULT(STDMETHODCALLTYPE * SetDialogBoxMode)(IDirect3DDevice9 * this_, BOOL bEnableDialogs);
    void(STDMETHODCALLTYPE * SetGammaRamp)(IDirect3DDevice9 * this_, UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp);
    void(STDMETHODCALLTYPE * GetGammaRamp)(IDirect3DDevice9 * this_, UINT iSwapChain, D3DGAMMARAMP * pRamp);
    HRESULT(STDMETHODCALLTYPE * CreateTexture)
    (IDirect3DDevice9 * this_, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 ** ppTexture,
     HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateVolumeTexture)
    (IDirect3DDevice9 * this_, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
     IDirect3DVolumeTexture9 ** ppVolumeTexture, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateCubeTexture)
    (IDirect3DDevice9 * this_, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9 ** ppCubeTexture,
     HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateVertexBuffer)
    (IDirect3DDevice9 * this_, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9 ** ppVertexBuffer, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateIndexBuffer)
    (IDirect3DDevice9 * this_, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9 ** ppIndexBuffer, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateRenderTarget)
    (IDirect3DDevice9 * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
     IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateDepthStencilSurface)
    (IDirect3DDevice9 * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard,
     IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * UpdateSurface)
    (IDirect3DDevice9 * this_, IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect, IDirect3DSurface9 * pDestinationSurface, CONST POINT * pDestPoint);
    HRESULT(STDMETHODCALLTYPE * UpdateTexture)(IDirect3DDevice9 * this_, IDirect3DBaseTexture9 * pSourceTexture, IDirect3DBaseTexture9 * pDestinationTexture);
    HRESULT(STDMETHODCALLTYPE * GetRenderTargetData)(IDirect3DDevice9 * this_, IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface);
    HRESULT(STDMETHODCALLTYPE * GetFrontBufferData)(IDirect3DDevice9 * this_, UINT iSwapChain, IDirect3DSurface9 * pDestSurface);
    HRESULT(STDMETHODCALLTYPE * StretchRect)
    (IDirect3DDevice9 * this_, IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect, IDirect3DSurface9 * pDestSurface, CONST RECT * pDestRect,
     D3DTEXTUREFILTERTYPE Filter);
    HRESULT(STDMETHODCALLTYPE * ColorFill)(IDirect3DDevice9 * this_, IDirect3DSurface9 * pSurface, CONST RECT * pRect, D3DCOLOR color);
    HRESULT(STDMETHODCALLTYPE * CreateOffscreenPlainSurface)
    (IDirect3DDevice9 * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * SetRenderTarget)(IDirect3DDevice9 * this_, DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget);
    HRESULT(STDMETHODCALLTYPE * GetRenderTarget)(IDirect3DDevice9 * this_, DWORD RenderTargetIndex, IDirect3DSurface9 ** ppRenderTarget);
    HRESULT(STDMETHODCALLTYPE * SetDepthStencilSurface)(IDirect3DDevice9 * this_, IDirect3DSurface9 * pNewZStencil);
    HRESULT(STDMETHODCALLTYPE * GetDepthStencilSurface)(IDirect3DDevice9 * this_, IDirect3DSurface9 ** ppZStencilSurface);
    HRESULT(STDMETHODCALLTYPE * BeginScene)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * EndScene)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * Clear)(IDirect3DDevice9 * this_, DWORD Count, CONST D3DRECT * pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    HRESULT(STDMETHODCALLTYPE * SetTransform)(IDirect3DDevice9 * this_, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix);
    HRESULT(STDMETHODCALLTYPE * GetTransform)(IDirect3DDevice9 * this_, D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix);
    HRESULT(STDMETHODCALLTYPE * MultiplyTransform)(IDirect3DDevice9 * this_, D3DTRANSFORMSTATETYPE TransformType, CONST D3DMATRIX * Value);
    HRESULT(STDMETHODCALLTYPE * SetViewport)(IDirect3DDevice9 * this_, CONST D3DVIEWPORT9 * pViewport);
    HRESULT(STDMETHODCALLTYPE * GetViewport)(IDirect3DDevice9 * this_, D3DVIEWPORT9 * pViewport);
    HRESULT(STDMETHODCALLTYPE * SetMaterial)(IDirect3DDevice9 * this_, CONST D3DMATERIAL9 * pMaterial);
    HRESULT(STDMETHODCALLTYPE * GetMaterial)(IDirect3DDevice9 * this_, D3DMATERIAL9 * pMaterial);
    HRESULT(STDMETHODCALLTYPE * SetLight)(IDirect3DDevice9 * this_, DWORD Index, CONST D3DLIGHT9 * pLight);
    HRESULT(STDMETHODCALLTYPE * GetLight)(IDirect3DDevice9 * this_, DWORD Index, D3DLIGHT9 * pLight);
    HRESULT(STDMETHODCALLTYPE * LightEnable)(IDirect3DDevice9 * this_, DWORD Index, BOOL Enable);
    HRESULT(STDMETHODCALLTYPE * GetLightEnable)(IDirect3DDevice9 * this_, DWORD Index, BOOL * pEnable);
    HRESULT(STDMETHODCALLTYPE * SetClipPlane)(IDirect3DDevice9 * this_, DWORD Index, CONST float * pPlane);
    HRESULT(STDMETHODCALLTYPE * GetClipPlane)(IDirect3DDevice9 * this_, DWORD Index, float * pPlane);
    HRESULT(STDMETHODCALLTYPE * SetRenderState)(IDirect3DDevice9 * this_, D3DRENDERSTATETYPE State, DWORD Value);
    HRESULT(STDMETHODCALLTYPE * GetRenderState)(IDirect3DDevice9 * this_, D3DRENDERSTATETYPE State, DWORD * pValue);
    HRESULT(STDMETHODCALLTYPE * CreateStateBlock)(IDirect3DDevice9 * this_, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 ** ppSB);
    HRESULT(STDMETHODCALLTYPE * BeginStateBlock)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * EndStateBlock)(IDirect3DDevice9 * this_, IDirect3DStateBlock9 ** ppSB);
    HRESULT(STDMETHODCALLTYPE * SetClipStatus)(IDirect3DDevice9 * this_, CONST D3DCLIPSTATUS9 * pClipStatus);
    HRESULT(STDMETHODCALLTYPE * GetClipStatus)(IDirect3DDevice9 * this_, D3DCLIPSTATUS9 * pClipStatus);
    HRESULT(STDMETHODCALLTYPE * GetTexture)(IDirect3DDevice9 * this_, DWORD Stage, IDirect3DBaseTexture9 ** ppTexture);
    HRESULT(STDMETHODCALLTYPE * SetTexture)(IDirect3DDevice9 * this_, DWORD Stage, IDirect3DBaseTexture9 * pTexture);
    HRESULT(STDMETHODCALLTYPE * GetTextureStageState)(IDirect3DDevice9 * this_, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue);
    HRESULT(STDMETHODCALLTYPE * SetTextureStageState)(IDirect3DDevice9 * this_, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    HRESULT(STDMETHODCALLTYPE * GetSamplerState)(IDirect3DDevice9 * this_, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue);
    HRESULT(STDMETHODCALLTYPE * SetSamplerState)(IDirect3DDevice9 * this_, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    HRESULT(STDMETHODCALLTYPE * ValidateDevice)(IDirect3DDevice9 * this_, DWORD * pNumPasses);
    HRESULT(STDMETHODCALLTYPE * SetPaletteEntries)(IDirect3DDevice9 * this_, UINT PaletteNumber, CONST PALETTEENTRY * pEntries);
    HRESULT(STDMETHODCALLTYPE * GetPaletteEntries)(IDirect3DDevice9 * this_, UINT PaletteNumber, PALETTEENTRY * pEntries);
    HRESULT(STDMETHODCALLTYPE * SetCurrentTexturePalette)(IDirect3DDevice9 * this_, UINT PaletteNumber);
    HRESULT(STDMETHODCALLTYPE * GetCurrentTexturePalette)(IDirect3DDevice9 * this_, UINT * PaletteNumber);
    HRESULT(STDMETHODCALLTYPE * SetScissorRect)(IDirect3DDevice9 * this_, CONST RECT * pRect);
    HRESULT(STDMETHODCALLTYPE * GetScissorRect)(IDirect3DDevice9 * this_, RECT * pRect);
    HRESULT(STDMETHODCALLTYPE * SetSoftwareVertexProcessing)(IDirect3DDevice9 * this_, BOOL bSoftware);
    BOOL(STDMETHODCALLTYPE * GetSoftwareVertexProcessing)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * SetNPatchMode)(IDirect3DDevice9 * this_, float nSegments);
    float(STDMETHODCALLTYPE * GetNPatchMode)(IDirect3DDevice9 * this_);
    HRESULT(STDMETHODCALLTYPE * DrawPrimitive)(IDirect3DDevice9 * this_, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    HRESULT(STDMETHODCALLTYPE * DrawIndexedPrimitive)
    (IDirect3DDevice9 * this_, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    HRESULT(STDMETHODCALLTYPE * DrawPrimitiveUP)
    (IDirect3DDevice9 * this_, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void * pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT(STDMETHODCALLTYPE * DrawIndexedPrimitiveUP)
    (IDirect3DDevice9 * this_, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void * pIndexData,
     D3DFORMAT IndexDataFormat, CONST void * pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT(STDMETHODCALLTYPE * ProcessVertices)
    (IDirect3DDevice9 * this_, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9 * pDestBuffer,
     IDirect3DVertexDeclaration9 * pVertexDecl, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * CreateVertexDeclaration)
    (IDirect3DDevice9 * this_, CONST D3DVERTEXELEMENT9 * pVertexElements, IDirect3DVertexDeclaration9 ** ppDecl);
    HRESULT(STDMETHODCALLTYPE * SetVertexDeclaration)(IDirect3DDevice9 * this_, IDirect3DVertexDeclaration9 * pDecl);
    HRESULT(STDMETHODCALLTYPE * GetVertexDeclaration)(IDirect3DDevice9 * this_, IDirect3DVertexDeclaration9 ** ppDecl);
    HRESULT(STDMETHODCALLTYPE * SetFVF)(IDirect3DDevice9 * this_, DWORD FVF);
    HRESULT(STDMETHODCALLTYPE * GetFVF)(IDirect3DDevice9 * this_, DWORD * pFVF);
    HRESULT(STDMETHODCALLTYPE * CreateVertexShader)(IDirect3DDevice9 * this_, CONST DWORD * pFunction, IDirect3DVertexShader9 ** ppShader);
    HRESULT(STDMETHODCALLTYPE * SetVertexShader)(IDirect3DDevice9 * this_, IDirect3DVertexShader9 * pShader);
    HRESULT(STDMETHODCALLTYPE * GetVertexShader)(IDirect3DDevice9 * this_, IDirect3DVertexShader9 ** ppShader);
    HRESULT(STDMETHODCALLTYPE * SetVertexShaderConstantF)(IDirect3DDevice9 * this_, UINT StartRegister, CONST float * pConstantData, UINT Vector4fCount);
    HRESULT(STDMETHODCALLTYPE * GetVertexShaderConstantF)(IDirect3DDevice9 * this_, UINT StartRegister, float * pConstantData, UINT Vector4fCount);
    HRESULT(STDMETHODCALLTYPE * SetVertexShaderConstantI)(IDirect3DDevice9 * this_, UINT StartRegister, CONST int * pConstantData, UINT Vector4iCount);
    HRESULT(STDMETHODCALLTYPE * GetVertexShaderConstantI)(IDirect3DDevice9 * this_, UINT StartRegister, int * pConstantData, UINT Vector4iCount);
    HRESULT(STDMETHODCALLTYPE * SetVertexShaderConstantB)(IDirect3DDevice9 * this_, UINT StartRegister, CONST BOOL * pConstantData, UINT BoolCount);
    HRESULT(STDMETHODCALLTYPE * GetVertexShaderConstantB)(IDirect3DDevice9 * this_, UINT StartRegister, BOOL * pConstantData, UINT BoolCount);
    HRESULT(STDMETHODCALLTYPE * SetStreamSource)
    (IDirect3DDevice9 * this_, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride);
    HRESULT(STDMETHODCALLTYPE * GetStreamSource)
    (IDirect3DDevice9 * this_, UINT StreamNumber, IDirect3DVertexBuffer9 ** ppStreamData, UINT * pOffsetInBytes, UINT * pStride);
    HRESULT(STDMETHODCALLTYPE * SetStreamSourceFreq)(IDirect3DDevice9 * this_, UINT StreamNumber, UINT Setting);
    HRESULT(STDMETHODCALLTYPE * GetStreamSourceFreq)(IDirect3DDevice9 * this_, UINT StreamNumber, UINT * pSetting);
    HRESULT(STDMETHODCALLTYPE * SetIndices)(IDirect3DDevice9 * this_, IDirect3DIndexBuffer9 * pIndexData);
    HRESULT(STDMETHODCALLTYPE * GetIndices)(IDirect3DDevice9 * this_, IDirect3DIndexBuffer9 ** ppIndexData);
    HRESULT(STDMETHODCALLTYPE * CreatePixelShader)(IDirect3DDevice9 * this_, CONST DWORD * pFunction, IDirect3DPixelShader9 ** ppShader);
    HRESULT(STDMETHODCALLTYPE * SetPixelShader)(IDirect3DDevice9 * this_, IDirect3DPixelShader9 * pShader);
    HRESULT(STDMETHODCALLTYPE * GetPixelShader)(IDirect3DDevice9 * this_, IDirect3DPixelShader9 ** ppShader);
    HRESULT(STDMETHODCALLTYPE * SetPixelShaderConstantF)(IDirect3DDevice9 * this_, UINT StartRegister, CONST float * pConstantData, UINT Vector4fCount);
    HRESULT(STDMETHODCALLTYPE * GetPixelShaderConstantF)(IDirect3DDevice9 * this_, UINT StartRegister, float * pConstantData, UINT Vector4fCount);
    HRESULT(STDMETHODCALLTYPE * SetPixelShaderConstantI)(IDirect3DDevice9 * this_, UINT StartRegister, CONST int * pConstantData, UINT Vector4iCount);
    HRESULT(STDMETHODCALLTYPE * GetPixelShaderConstantI)(IDirect3DDevice9 * this_, UINT StartRegister, int * pConstantData, UINT Vector4iCount);
    HRESULT(STDMETHODCALLTYPE * SetPixelShaderConstantB)(IDirect3DDevice9 * this_, UINT StartRegister, CONST BOOL * pConstantData, UINT BoolCount);
    HRESULT(STDMETHODCALLTYPE * GetPixelShaderConstantB)(IDirect3DDevice9 * this_, UINT StartRegister, BOOL * pConstantData, UINT BoolCount);
    HRESULT(STDMETHODCALLTYPE * DrawRectPatch)(IDirect3DDevice9 * this_, UINT Handle, CONST float * pNumSegs, CONST D3DRECTPATCH_INFO * pRectPatchInfo);
    HRESULT(STDMETHODCALLTYPE * DrawTriPatch)(IDirect3DDevice9 * this_, UINT Handle, CONST float * pNumSegs, CONST D3DTRIPATCH_INFO * pTriPatchInfo);
    HRESULT(STDMETHODCALLTYPE * DeletePatch)(IDirect3DDevice9 * this_, UINT Handle);
    HRESULT(STDMETHODCALLTYPE * CreateQuery)(IDirect3DDevice9 * this_, D3DQUERYTYPE Type, IDirect3DQuery9 ** ppQuery);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DStateBlock9
// -----------------------------------------------------------------------------
struct IDirect3DStateBlock9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DStateBlock9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DStateBlock9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DStateBlock9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DStateBlock9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * Capture)(IDirect3DStateBlock9 * this_);
    HRESULT(STDMETHODCALLTYPE * Apply)(IDirect3DStateBlock9 * this_);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DSwapChain9
// -----------------------------------------------------------------------------
struct IDirect3DSwapChain9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DSwapChain9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DSwapChain9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DSwapChain9 * this_);
    HRESULT(STDMETHODCALLTYPE * Present)
    (IDirect3DSwapChain9 * this_, CONST RECT * pSourceRect, CONST RECT * pDestRect, HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion, DWORD dwFlags);
    HRESULT(STDMETHODCALLTYPE * GetFrontBufferData)(IDirect3DSwapChain9 * this_, IDirect3DSurface9 * pDestSurface);
    HRESULT(STDMETHODCALLTYPE * GetBackBuffer)(IDirect3DSwapChain9 * this_, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 ** ppBackBuffer);
    HRESULT(STDMETHODCALLTYPE * GetRasterStatus)(IDirect3DSwapChain9 * this_, D3DRASTER_STATUS * pRasterStatus);
    HRESULT(STDMETHODCALLTYPE * GetDisplayMode)(IDirect3DSwapChain9 * this_, D3DDISPLAYMODE * pMode);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DSwapChain9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * GetPresentParameters)(IDirect3DSwapChain9 * this_, D3DPRESENT_PARAMETERS * pPresentationParameters);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DResource9
// -----------------------------------------------------------------------------
struct IDirect3DResource9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DResource9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DResource9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DResource9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DResource9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DResource9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DResource9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DResource9 * this_, REFGUID refguid);
    DWORD(STDMETHODCALLTYPE * SetPriority)(IDirect3DResource9 * this_, DWORD PriorityNew);
    DWORD(STDMETHODCALLTYPE * GetPriority)(IDirect3DResource9 * this_);
    void(STDMETHODCALLTYPE * PreLoad)(IDirect3DResource9 * this_);
    D3DRESOURCETYPE(STDMETHODCALLTYPE * GetType)(IDirect3DResource9 * this_);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DVertexDeclaration9
// -----------------------------------------------------------------------------
struct IDirect3DVertexDeclaration9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DVertexDeclaration9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DVertexDeclaration9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DVertexDeclaration9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DVertexDeclaration9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * GetDeclaration)(IDirect3DVertexDeclaration9 * this_, D3DVERTEXELEMENT9 * pElement, UINT * pNumElements);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DVertexShader9
// -----------------------------------------------------------------------------
struct IDirect3DVertexShader9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DVertexShader9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DVertexShader9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DVertexShader9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DVertexShader9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * GetFunction)(IDirect3DVertexShader9 * this_, void * pFunction, UINT * pSizeOfData);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DPixelShader9
// -----------------------------------------------------------------------------
struct IDirect3DPixelShader9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DPixelShader9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DPixelShader9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DPixelShader9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DPixelShader9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * GetFunction)(IDirect3DPixelShader9 * this_, void * pFunction, UINT * pSizeOfData);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DBaseTexture9
// -----------------------------------------------------------------------------
struct IDirect3DBaseTexture9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DBaseTexture9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DBaseTexture9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DBaseTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DBaseTexture9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DBaseTexture9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DBaseTexture9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DBaseTexture9 * this_, REFGUID refguid);
    DWORD(STDMETHODCALLTYPE * SetPriority)(IDirect3DBaseTexture9 * this_, DWORD PriorityNew);
    DWORD(STDMETHODCALLTYPE * GetPriority)(IDirect3DBaseTexture9 * this_);
    void(STDMETHODCALLTYPE * PreLoad)(IDirect3DBaseTexture9 * this_);
    D3DRESOURCETYPE(STDMETHODCALLTYPE * GetType)(IDirect3DBaseTexture9 * this_);
    DWORD(STDMETHODCALLTYPE * SetLOD)(IDirect3DBaseTexture9 * this_, DWORD LODNew);
    DWORD(STDMETHODCALLTYPE * GetLOD)(IDirect3DBaseTexture9 * this_);
    DWORD(STDMETHODCALLTYPE * GetLevelCount)(IDirect3DBaseTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * SetAutoGenFilterType)(IDirect3DBaseTexture9 * this_, D3DTEXTUREFILTERTYPE FilterType);
    D3DTEXTUREFILTERTYPE(STDMETHODCALLTYPE * GetAutoGenFilterType)(IDirect3DBaseTexture9 * this_);
    void(STDMETHODCALLTYPE * GenerateMipSubLevels)(IDirect3DBaseTexture9 * this_);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DTexture9
// -----------------------------------------------------------------------------
struct IDirect3DTexture9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DTexture9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DTexture9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DTexture9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DTexture9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DTexture9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DTexture9 * this_, REFGUID refguid);
    DWORD(STDMETHODCALLTYPE * SetPriority)(IDirect3DTexture9 * this_, DWORD PriorityNew);
    DWORD(STDMETHODCALLTYPE * GetPriority)(IDirect3DTexture9 * this_);
    void(STDMETHODCALLTYPE * PreLoad)(IDirect3DTexture9 * this_);
    D3DRESOURCETYPE(STDMETHODCALLTYPE * GetType)(IDirect3DTexture9 * this_);
    DWORD(STDMETHODCALLTYPE * SetLOD)(IDirect3DTexture9 * this_, DWORD LODNew);
    DWORD(STDMETHODCALLTYPE * GetLOD)(IDirect3DTexture9 * this_);
    DWORD(STDMETHODCALLTYPE * GetLevelCount)(IDirect3DTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * SetAutoGenFilterType)(IDirect3DTexture9 * this_, D3DTEXTUREFILTERTYPE FilterType);
    D3DTEXTUREFILTERTYPE(STDMETHODCALLTYPE * GetAutoGenFilterType)(IDirect3DTexture9 * this_);
    void(STDMETHODCALLTYPE * GenerateMipSubLevels)(IDirect3DTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetLevelDesc)(IDirect3DTexture9 * this_, UINT Level, D3DSURFACE_DESC * pDesc);
    HRESULT(STDMETHODCALLTYPE * GetSurfaceLevel)(IDirect3DTexture9 * this_, UINT Level, IDirect3DSurface9 ** ppSurfaceLevel);
    HRESULT(STDMETHODCALLTYPE * LockRect)(IDirect3DTexture9 * this_, UINT Level, D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * UnlockRect)(IDirect3DTexture9 * this_, UINT Level);
    HRESULT(STDMETHODCALLTYPE * AddDirtyRect)(IDirect3DTexture9 * this_, CONST RECT * pDirtyRect);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DVolumeTexture9
// -----------------------------------------------------------------------------
struct IDirect3DVolumeTexture9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DVolumeTexture9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DVolumeTexture9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DVolumeTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DVolumeTexture9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DVolumeTexture9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DVolumeTexture9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DVolumeTexture9 * this_, REFGUID refguid);
    DWORD(STDMETHODCALLTYPE * SetPriority)(IDirect3DVolumeTexture9 * this_, DWORD PriorityNew);
    DWORD(STDMETHODCALLTYPE * GetPriority)(IDirect3DVolumeTexture9 * this_);
    void(STDMETHODCALLTYPE * PreLoad)(IDirect3DVolumeTexture9 * this_);
    D3DRESOURCETYPE(STDMETHODCALLTYPE * GetType)(IDirect3DVolumeTexture9 * this_);
    DWORD(STDMETHODCALLTYPE * SetLOD)(IDirect3DVolumeTexture9 * this_, DWORD LODNew);
    DWORD(STDMETHODCALLTYPE * GetLOD)(IDirect3DVolumeTexture9 * this_);
    DWORD(STDMETHODCALLTYPE * GetLevelCount)(IDirect3DVolumeTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * SetAutoGenFilterType)(IDirect3DVolumeTexture9 * this_, D3DTEXTUREFILTERTYPE FilterType);
    D3DTEXTUREFILTERTYPE(STDMETHODCALLTYPE * GetAutoGenFilterType)(IDirect3DVolumeTexture9 * this_);
    void(STDMETHODCALLTYPE * GenerateMipSubLevels)(IDirect3DVolumeTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetLevelDesc)(IDirect3DVolumeTexture9 * this_, UINT Level, D3DVOLUME_DESC * pDesc);
    HRESULT(STDMETHODCALLTYPE * GetVolumeLevel)(IDirect3DVolumeTexture9 * this_, UINT Level, IDirect3DVolume9 ** ppVolumeLevel);
    HRESULT(STDMETHODCALLTYPE * LockBox)(IDirect3DVolumeTexture9 * this_, UINT Level, D3DLOCKED_BOX * pLockedVolume, CONST D3DBOX * pBox, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * UnlockBox)(IDirect3DVolumeTexture9 * this_, UINT Level);
    HRESULT(STDMETHODCALLTYPE * AddDirtyBox)(IDirect3DVolumeTexture9 * this_, CONST D3DBOX * pDirtyBox);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DCubeTexture9
// -----------------------------------------------------------------------------
struct IDirect3DCubeTexture9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DCubeTexture9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DCubeTexture9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DCubeTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DCubeTexture9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DCubeTexture9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DCubeTexture9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DCubeTexture9 * this_, REFGUID refguid);
    DWORD(STDMETHODCALLTYPE * SetPriority)(IDirect3DCubeTexture9 * this_, DWORD PriorityNew);
    DWORD(STDMETHODCALLTYPE * GetPriority)(IDirect3DCubeTexture9 * this_);
    void(STDMETHODCALLTYPE * PreLoad)(IDirect3DCubeTexture9 * this_);
    D3DRESOURCETYPE(STDMETHODCALLTYPE * GetType)(IDirect3DCubeTexture9 * this_);
    DWORD(STDMETHODCALLTYPE * SetLOD)(IDirect3DCubeTexture9 * this_, DWORD LODNew);
    DWORD(STDMETHODCALLTYPE * GetLOD)(IDirect3DCubeTexture9 * this_);
    DWORD(STDMETHODCALLTYPE * GetLevelCount)(IDirect3DCubeTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * SetAutoGenFilterType)(IDirect3DCubeTexture9 * this_, D3DTEXTUREFILTERTYPE FilterType);
    D3DTEXTUREFILTERTYPE(STDMETHODCALLTYPE * GetAutoGenFilterType)(IDirect3DCubeTexture9 * this_);
    void(STDMETHODCALLTYPE * GenerateMipSubLevels)(IDirect3DCubeTexture9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetLevelDesc)(IDirect3DCubeTexture9 * this_, UINT Level, D3DSURFACE_DESC * pDesc);
    HRESULT(STDMETHODCALLTYPE * GetCubeMapSurface)(IDirect3DCubeTexture9 * this_, D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9 ** ppCubeMapSurface);
    HRESULT(STDMETHODCALLTYPE * LockRect)
    (IDirect3DCubeTexture9 * this_, D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * UnlockRect)(IDirect3DCubeTexture9 * this_, D3DCUBEMAP_FACES FaceType, UINT Level);
    HRESULT(STDMETHODCALLTYPE * AddDirtyRect)(IDirect3DCubeTexture9 * this_, D3DCUBEMAP_FACES FaceType, CONST RECT * pDirtyRect);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DVertexBuffer9
// -----------------------------------------------------------------------------
struct IDirect3DVertexBuffer9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DVertexBuffer9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DVertexBuffer9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DVertexBuffer9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DVertexBuffer9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DVertexBuffer9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DVertexBuffer9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DVertexBuffer9 * this_, REFGUID refguid);
    DWORD(STDMETHODCALLTYPE * SetPriority)(IDirect3DVertexBuffer9 * this_, DWORD PriorityNew);
    DWORD(STDMETHODCALLTYPE * GetPriority)(IDirect3DVertexBuffer9 * this_);
    void(STDMETHODCALLTYPE * PreLoad)(IDirect3DVertexBuffer9 * this_);
    D3DRESOURCETYPE(STDMETHODCALLTYPE * GetType)(IDirect3DVertexBuffer9 * this_);
    HRESULT(STDMETHODCALLTYPE * Lock)(IDirect3DVertexBuffer9 * this_, UINT OffsetToLock, UINT SizeToLock, void ** ppbData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * Unlock)(IDirect3DVertexBuffer9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDesc)(IDirect3DVertexBuffer9 * this_, D3DVERTEXBUFFER_DESC * pDesc);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DIndexBuffer9
// -----------------------------------------------------------------------------
struct IDirect3DIndexBuffer9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DIndexBuffer9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DIndexBuffer9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DIndexBuffer9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DIndexBuffer9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DIndexBuffer9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DIndexBuffer9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DIndexBuffer9 * this_, REFGUID refguid);
    DWORD(STDMETHODCALLTYPE * SetPriority)(IDirect3DIndexBuffer9 * this_, DWORD PriorityNew);
    DWORD(STDMETHODCALLTYPE * GetPriority)(IDirect3DIndexBuffer9 * this_);
    void(STDMETHODCALLTYPE * PreLoad)(IDirect3DIndexBuffer9 * this_);
    D3DRESOURCETYPE(STDMETHODCALLTYPE * GetType)(IDirect3DIndexBuffer9 * this_);
    HRESULT(STDMETHODCALLTYPE * Lock)(IDirect3DIndexBuffer9 * this_, UINT OffsetToLock, UINT SizeToLock, void ** ppbData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * Unlock)(IDirect3DIndexBuffer9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDesc)(IDirect3DIndexBuffer9 * this_, D3DINDEXBUFFER_DESC * pDesc);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DSurface9
// -----------------------------------------------------------------------------
struct IDirect3DSurface9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DSurface9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DSurface9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DSurface9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DSurface9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DSurface9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DSurface9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DSurface9 * this_, REFGUID refguid);
    DWORD(STDMETHODCALLTYPE * SetPriority)(IDirect3DSurface9 * this_, DWORD PriorityNew);
    DWORD(STDMETHODCALLTYPE * GetPriority)(IDirect3DSurface9 * this_);
    void(STDMETHODCALLTYPE * PreLoad)(IDirect3DSurface9 * this_);
    D3DRESOURCETYPE(STDMETHODCALLTYPE * GetType)(IDirect3DSurface9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetContainer)(IDirect3DSurface9 * this_, REFIID riid, void ** ppContainer);
    HRESULT(STDMETHODCALLTYPE * GetDesc)(IDirect3DSurface9 * this_, D3DSURFACE_DESC * pDesc);
    HRESULT(STDMETHODCALLTYPE * LockRect)(IDirect3DSurface9 * this_, D3DLOCKED_RECT * pLockedRect, CONST RECT * pRect, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * UnlockRect)(IDirect3DSurface9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDC)(IDirect3DSurface9 * this_, HDC * phdc);
    HRESULT(STDMETHODCALLTYPE * ReleaseDC)(IDirect3DSurface9 * this_, HDC hdc);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DVolume9
// -----------------------------------------------------------------------------
struct IDirect3DVolume9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DVolume9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DVolume9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DVolume9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DVolume9 * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * SetPrivateData)(IDirect3DVolume9 * this_, REFGUID refguid, CONST void * pData, DWORD SizeOfData, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * GetPrivateData)(IDirect3DVolume9 * this_, REFGUID refguid, void * pData, DWORD * pSizeOfData);
    HRESULT(STDMETHODCALLTYPE * FreePrivateData)(IDirect3DVolume9 * this_, REFGUID refguid);
    HRESULT(STDMETHODCALLTYPE * GetContainer)(IDirect3DVolume9 * this_, REFIID riid, void ** ppContainer);
    HRESULT(STDMETHODCALLTYPE * GetDesc)(IDirect3DVolume9 * this_, D3DVOLUME_DESC * pDesc);
    HRESULT(STDMETHODCALLTYPE * LockBox)(IDirect3DVolume9 * this_, D3DLOCKED_BOX * pLockedVolume, CONST D3DBOX * pBox, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * UnlockBox)(IDirect3DVolume9 * this_);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DQuery9
// -----------------------------------------------------------------------------
struct IDirect3DQuery9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DQuery9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DQuery9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DQuery9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DQuery9 * this_, IDirect3DDevice9 ** ppDevice);
    D3DQUERYTYPE(STDMETHODCALLTYPE * GetType)(IDirect3DQuery9 * this_);
    DWORD(STDMETHODCALLTYPE * GetDataSize)(IDirect3DQuery9 * this_);
    HRESULT(STDMETHODCALLTYPE * Issue)(IDirect3DQuery9 * this_, DWORD dwIssueFlags);
    HRESULT(STDMETHODCALLTYPE * GetData)(IDirect3DQuery9 * this_, void * pData, DWORD dwSize, DWORD dwGetDataFlags);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3D9Ex
// -----------------------------------------------------------------------------
struct IDirect3D9ExVtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3D9Ex * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3D9Ex * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3D9Ex * this_);
    UINT(STDMETHODCALLTYPE * GetAdapterCount)(IDirect3D9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * GetAdapterIdentifier)(IDirect3D9Ex * this_, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 * pIdentifier);
    UINT(STDMETHODCALLTYPE * GetAdapterModeCount)(IDirect3D9Ex * this_, UINT Adapter, D3DFORMAT Format);
    HRESULT(STDMETHODCALLTYPE * EnumAdapterModes)(IDirect3D9Ex * this_, UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE * pMode);
    HRESULT(STDMETHODCALLTYPE * GetAdapterDisplayMode)(IDirect3D9Ex * this_, UINT Adapter, D3DDISPLAYMODE * pMode);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceType)
    (IDirect3D9Ex * this_, UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceFormat)
    (IDirect3D9Ex * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceMultiSampleType)
    (IDirect3D9Ex * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType,
     DWORD * pQualityLevels);
    HRESULT(STDMETHODCALLTYPE * CheckDepthStencilMatch)
    (IDirect3D9Ex * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceFormatConversion)
    (IDirect3D9Ex * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
    HRESULT(STDMETHODCALLTYPE * GetDeviceCaps)(IDirect3D9Ex * this_, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 * pCaps);
    HMONITOR(STDMETHODCALLTYPE * GetAdapterMonitor)(IDirect3D9Ex * this_, UINT Adapter);
    HRESULT(STDMETHODCALLTYPE * CreateDevice)
    (IDirect3D9Ex * this_, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters,
     IDirect3DDevice9 ** ppReturnedDeviceInterface);
    UINT(STDMETHODCALLTYPE * GetAdapterModeCountEx)(IDirect3D9Ex * this_, UINT Adapter, CONST D3DDISPLAYMODEFILTER * pFilter);
    HRESULT(STDMETHODCALLTYPE * EnumAdapterModesEx)
    (IDirect3D9Ex * this_, UINT Adapter, CONST D3DDISPLAYMODEFILTER * pFilter, UINT Mode, D3DDISPLAYMODEEX * pMode);
    HRESULT(STDMETHODCALLTYPE * GetAdapterDisplayModeEx)(IDirect3D9Ex * this_, UINT Adapter, D3DDISPLAYMODEEX * pMode, D3DDISPLAYROTATION * pRotation);
    HRESULT(STDMETHODCALLTYPE * CreateDeviceEx)
    (IDirect3D9Ex * this_, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters,
     D3DDISPLAYMODEEX * pFullscreenDisplayMode, IDirect3DDevice9Ex ** ppReturnedDeviceInterface);
    HRESULT(STDMETHODCALLTYPE * GetAdapterLUID)(IDirect3D9Ex * this_, UINT Adapter, LUID * pLUID);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DDevice9Ex
// -----------------------------------------------------------------------------
struct IDirect3DDevice9ExVtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DDevice9Ex * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DDevice9Ex * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * TestCooperativeLevel)(IDirect3DDevice9Ex * this_);
    UINT(STDMETHODCALLTYPE * GetAvailableTextureMem)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * EvictManagedResources)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * GetDirect3D)(IDirect3DDevice9Ex * this_, IDirect3D9 ** ppD3D9);
    HRESULT(STDMETHODCALLTYPE * GetDeviceCaps)(IDirect3DDevice9Ex * this_, D3DCAPS9 * pCaps);
    HRESULT(STDMETHODCALLTYPE * GetDisplayMode)(IDirect3DDevice9Ex * this_, UINT iSwapChain, D3DDISPLAYMODE * pMode);
    HRESULT(STDMETHODCALLTYPE * GetCreationParameters)(IDirect3DDevice9Ex * this_, D3DDEVICE_CREATION_PARAMETERS * pParameters);
    HRESULT(STDMETHODCALLTYPE * SetCursorProperties)(IDirect3DDevice9Ex * this_, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 * pCursorBitmap);
    void(STDMETHODCALLTYPE * SetCursorPosition)(IDirect3DDevice9Ex * this_, int X, int Y, DWORD Flags);
    BOOL(STDMETHODCALLTYPE * ShowCursor)(IDirect3DDevice9Ex * this_, BOOL bShow);
    HRESULT(STDMETHODCALLTYPE * CreateAdditionalSwapChain)
    (IDirect3DDevice9Ex * this_, D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DSwapChain9 ** ppSwapChain);
    HRESULT(STDMETHODCALLTYPE * GetSwapChain)(IDirect3DDevice9Ex * this_, UINT iSwapChain, IDirect3DSwapChain9 ** ppSwapChain);
    UINT(STDMETHODCALLTYPE * GetNumberOfSwapChains)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * Reset)(IDirect3DDevice9Ex * this_, D3DPRESENT_PARAMETERS * pPresentationParameters);
    HRESULT(STDMETHODCALLTYPE * Present)
    (IDirect3DDevice9Ex * this_, CONST RECT * pSourceRect, CONST RECT * pDestRect, HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion);
    HRESULT(STDMETHODCALLTYPE * GetBackBuffer)
    (IDirect3DDevice9Ex * this_, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 ** ppBackBuffer);
    HRESULT(STDMETHODCALLTYPE * GetRasterStatus)(IDirect3DDevice9Ex * this_, UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus);
    HRESULT(STDMETHODCALLTYPE * SetDialogBoxMode)(IDirect3DDevice9Ex * this_, BOOL bEnableDialogs);
    void(STDMETHODCALLTYPE * SetGammaRamp)(IDirect3DDevice9Ex * this_, UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp);
    void(STDMETHODCALLTYPE * GetGammaRamp)(IDirect3DDevice9Ex * this_, UINT iSwapChain, D3DGAMMARAMP * pRamp);
    HRESULT(STDMETHODCALLTYPE * CreateTexture)
    (IDirect3DDevice9Ex * this_, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 ** ppTexture,
     HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateVolumeTexture)
    (IDirect3DDevice9Ex * this_, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
     IDirect3DVolumeTexture9 ** ppVolumeTexture, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateCubeTexture)
    (IDirect3DDevice9Ex * this_, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9 ** ppCubeTexture,
     HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateVertexBuffer)
    (IDirect3DDevice9Ex * this_, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9 ** ppVertexBuffer, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateIndexBuffer)
    (IDirect3DDevice9Ex * this_, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9 ** ppIndexBuffer, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateRenderTarget)
    (IDirect3DDevice9Ex * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
     IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * CreateDepthStencilSurface)
    (IDirect3DDevice9Ex * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard,
     IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * UpdateSurface)
    (IDirect3DDevice9Ex * this_, IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect, IDirect3DSurface9 * pDestinationSurface,
     CONST POINT * pDestPoint);
    HRESULT(STDMETHODCALLTYPE * UpdateTexture)(IDirect3DDevice9Ex * this_, IDirect3DBaseTexture9 * pSourceTexture, IDirect3DBaseTexture9 * pDestinationTexture);
    HRESULT(STDMETHODCALLTYPE * GetRenderTargetData)(IDirect3DDevice9Ex * this_, IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface);
    HRESULT(STDMETHODCALLTYPE * GetFrontBufferData)(IDirect3DDevice9Ex * this_, UINT iSwapChain, IDirect3DSurface9 * pDestSurface);
    HRESULT(STDMETHODCALLTYPE * StretchRect)
    (IDirect3DDevice9Ex * this_, IDirect3DSurface9 * pSourceSurface, CONST RECT * pSourceRect, IDirect3DSurface9 * pDestSurface, CONST RECT * pDestRect,
     D3DTEXTUREFILTERTYPE Filter);
    HRESULT(STDMETHODCALLTYPE * ColorFill)(IDirect3DDevice9Ex * this_, IDirect3DSurface9 * pSurface, CONST RECT * pRect, D3DCOLOR color);
    HRESULT(STDMETHODCALLTYPE * CreateOffscreenPlainSurface)
    (IDirect3DDevice9Ex * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT(STDMETHODCALLTYPE * SetRenderTarget)(IDirect3DDevice9Ex * this_, DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget);
    HRESULT(STDMETHODCALLTYPE * GetRenderTarget)(IDirect3DDevice9Ex * this_, DWORD RenderTargetIndex, IDirect3DSurface9 ** ppRenderTarget);
    HRESULT(STDMETHODCALLTYPE * SetDepthStencilSurface)(IDirect3DDevice9Ex * this_, IDirect3DSurface9 * pNewZStencil);
    HRESULT(STDMETHODCALLTYPE * GetDepthStencilSurface)(IDirect3DDevice9Ex * this_, IDirect3DSurface9 ** ppZStencilSurface);
    HRESULT(STDMETHODCALLTYPE * BeginScene)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * EndScene)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * Clear)(IDirect3DDevice9Ex * this_, DWORD Count, CONST D3DRECT * pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    HRESULT(STDMETHODCALLTYPE * SetTransform)(IDirect3DDevice9Ex * this_, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix);
    HRESULT(STDMETHODCALLTYPE * GetTransform)(IDirect3DDevice9Ex * this_, D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix);
    HRESULT(STDMETHODCALLTYPE * MultiplyTransform)(IDirect3DDevice9Ex * this_, D3DTRANSFORMSTATETYPE TransformType, CONST D3DMATRIX * Value);
    HRESULT(STDMETHODCALLTYPE * SetViewport)(IDirect3DDevice9Ex * this_, CONST D3DVIEWPORT9 * pViewport);
    HRESULT(STDMETHODCALLTYPE * GetViewport)(IDirect3DDevice9Ex * this_, D3DVIEWPORT9 * pViewport);
    HRESULT(STDMETHODCALLTYPE * SetMaterial)(IDirect3DDevice9Ex * this_, CONST D3DMATERIAL9 * pMaterial);
    HRESULT(STDMETHODCALLTYPE * GetMaterial)(IDirect3DDevice9Ex * this_, D3DMATERIAL9 * pMaterial);
    HRESULT(STDMETHODCALLTYPE * SetLight)(IDirect3DDevice9Ex * this_, DWORD Index, CONST D3DLIGHT9 *);
    HRESULT(STDMETHODCALLTYPE * GetLight)(IDirect3DDevice9Ex * this_, DWORD Index, D3DLIGHT9 * pLight);
    HRESULT(STDMETHODCALLTYPE * LightEnable)(IDirect3DDevice9Ex * this_, DWORD Index, BOOL Enable);
    HRESULT(STDMETHODCALLTYPE * GetLightEnable)(IDirect3DDevice9Ex * this_, DWORD Index, BOOL * pEnable);
    HRESULT(STDMETHODCALLTYPE * SetClipPlane)(IDirect3DDevice9Ex * this_, DWORD Index, CONST float * pPlane);
    HRESULT(STDMETHODCALLTYPE * GetClipPlane)(IDirect3DDevice9Ex * this_, DWORD Index, float * pPlane);
    HRESULT(STDMETHODCALLTYPE * SetRenderState)(IDirect3DDevice9Ex * this_, D3DRENDERSTATETYPE State, DWORD Value);
    HRESULT(STDMETHODCALLTYPE * GetRenderState)(IDirect3DDevice9Ex * this_, D3DRENDERSTATETYPE State, DWORD * pValue);
    HRESULT(STDMETHODCALLTYPE * CreateStateBlock)(IDirect3DDevice9Ex * this_, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 ** ppSB);
    HRESULT(STDMETHODCALLTYPE * BeginStateBlock)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * EndStateBlock)(IDirect3DDevice9Ex * this_, IDirect3DStateBlock9 ** ppSB);
    HRESULT(STDMETHODCALLTYPE * SetClipStatus)(IDirect3DDevice9Ex * this_, CONST D3DCLIPSTATUS9 * pClipStatus);
    HRESULT(STDMETHODCALLTYPE * GetClipStatus)(IDirect3DDevice9Ex * this_, D3DCLIPSTATUS9 * pClipStatus);
    HRESULT(STDMETHODCALLTYPE * GetTexture)(IDirect3DDevice9Ex * this_, DWORD Stage, IDirect3DBaseTexture9 ** ppTexture);
    HRESULT(STDMETHODCALLTYPE * SetTexture)(IDirect3DDevice9Ex * this_, DWORD Stage, IDirect3DBaseTexture9 * pTexture);
    HRESULT(STDMETHODCALLTYPE * GetTextureStageState)(IDirect3DDevice9Ex * this_, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue);
    HRESULT(STDMETHODCALLTYPE * SetTextureStageState)(IDirect3DDevice9Ex * this_, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    HRESULT(STDMETHODCALLTYPE * GetSamplerState)(IDirect3DDevice9Ex * this_, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue);
    HRESULT(STDMETHODCALLTYPE * SetSamplerState)(IDirect3DDevice9Ex * this_, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    HRESULT(STDMETHODCALLTYPE * ValidateDevice)(IDirect3DDevice9Ex * this_, DWORD * pNumPasses);
    HRESULT(STDMETHODCALLTYPE * SetPaletteEntries)(IDirect3DDevice9Ex * this_, UINT PaletteNumber, CONST PALETTEENTRY * pEntries);
    HRESULT(STDMETHODCALLTYPE * GetPaletteEntries)(IDirect3DDevice9Ex * this_, UINT PaletteNumber, PALETTEENTRY * pEntries);
    HRESULT(STDMETHODCALLTYPE * SetCurrentTexturePalette)(IDirect3DDevice9Ex * this_, UINT PaletteNumber);
    HRESULT(STDMETHODCALLTYPE * GetCurrentTexturePalette)(IDirect3DDevice9Ex * this_, UINT * PaletteNumber);
    HRESULT(STDMETHODCALLTYPE * SetScissorRect)(IDirect3DDevice9Ex * this_, CONST RECT * pRect);
    HRESULT(STDMETHODCALLTYPE * GetScissorRect)(IDirect3DDevice9Ex * this_, RECT * pRect);
    HRESULT(STDMETHODCALLTYPE * SetSoftwareVertexProcessing)(IDirect3DDevice9Ex * this_, BOOL bSoftware);
    BOOL(STDMETHODCALLTYPE * GetSoftwareVertexProcessing)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * SetNPatchMode)(IDirect3DDevice9Ex * this_, float nSegments);
    float(STDMETHODCALLTYPE * GetNPatchMode)(IDirect3DDevice9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * DrawPrimitive)(IDirect3DDevice9Ex * this_, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    HRESULT(STDMETHODCALLTYPE * DrawIndexedPrimitive)
    (IDirect3DDevice9Ex * this_, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    HRESULT(STDMETHODCALLTYPE * DrawPrimitiveUP)
    (IDirect3DDevice9Ex * this_, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void * pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT(STDMETHODCALLTYPE * DrawIndexedPrimitiveUP)
    (IDirect3DDevice9Ex * this_, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void * pIndexData,
     D3DFORMAT IndexDataFormat, CONST void * pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT(STDMETHODCALLTYPE * ProcessVertices)
    (IDirect3DDevice9Ex * this_, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9 * pDestBuffer,
     IDirect3DVertexDeclaration9 * pVertexDecl, DWORD Flags);
    HRESULT(STDMETHODCALLTYPE * CreateVertexDeclaration)
    (IDirect3DDevice9Ex * this_, CONST D3DVERTEXELEMENT9 * pVertexElements, IDirect3DVertexDeclaration9 ** ppDecl);
    HRESULT(STDMETHODCALLTYPE * SetVertexDeclaration)(IDirect3DDevice9Ex * this_, IDirect3DVertexDeclaration9 * pDecl);
    HRESULT(STDMETHODCALLTYPE * GetVertexDeclaration)(IDirect3DDevice9Ex * this_, IDirect3DVertexDeclaration9 ** ppDecl);
    HRESULT(STDMETHODCALLTYPE * SetFVF)(IDirect3DDevice9Ex * this_, DWORD FVF);
    HRESULT(STDMETHODCALLTYPE * GetFVF)(IDirect3DDevice9Ex * this_, DWORD * pFVF);
    HRESULT(STDMETHODCALLTYPE * CreateVertexShader)(IDirect3DDevice9Ex * this_, CONST DWORD * pFunction, IDirect3DVertexShader9 ** ppShader);
    HRESULT(STDMETHODCALLTYPE * SetVertexShader)(IDirect3DDevice9Ex * this_, IDirect3DVertexShader9 * pShader);
    HRESULT(STDMETHODCALLTYPE * GetVertexShader)(IDirect3DDevice9Ex * this_, IDirect3DVertexShader9 ** ppShader);
    HRESULT(STDMETHODCALLTYPE * SetVertexShaderConstantF)(IDirect3DDevice9Ex * this_, UINT StartRegister, CONST float * pConstantData, UINT Vector4fCount);
    HRESULT(STDMETHODCALLTYPE * GetVertexShaderConstantF)(IDirect3DDevice9Ex * this_, UINT StartRegister, float * pConstantData, UINT Vector4fCount);
    HRESULT(STDMETHODCALLTYPE * SetVertexShaderConstantI)(IDirect3DDevice9Ex * this_, UINT StartRegister, CONST int * pConstantData, UINT Vector4iCount);
    HRESULT(STDMETHODCALLTYPE * GetVertexShaderConstantI)(IDirect3DDevice9Ex * this_, UINT StartRegister, int * pConstantData, UINT Vector4iCount);
    HRESULT(STDMETHODCALLTYPE * SetVertexShaderConstantB)(IDirect3DDevice9Ex * this_, UINT StartRegister, CONST BOOL * pConstantData, UINT BoolCount);
    HRESULT(STDMETHODCALLTYPE * GetVertexShaderConstantB)(IDirect3DDevice9Ex * this_, UINT StartRegister, BOOL * pConstantData, UINT BoolCount);
    HRESULT(STDMETHODCALLTYPE * SetStreamSource)
    (IDirect3DDevice9Ex * this_, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride);
    HRESULT(STDMETHODCALLTYPE * GetStreamSource)
    (IDirect3DDevice9Ex * this_, UINT StreamNumber, IDirect3DVertexBuffer9 ** ppStreamData, UINT * pOffsetInBytes, UINT * pStride);
    HRESULT(STDMETHODCALLTYPE * SetStreamSourceFreq)(IDirect3DDevice9Ex * this_, UINT StreamNumber, UINT Setting);
    HRESULT(STDMETHODCALLTYPE * GetStreamSourceFreq)(IDirect3DDevice9Ex * this_, UINT StreamNumber, UINT * pSetting);
    HRESULT(STDMETHODCALLTYPE * SetIndices)(IDirect3DDevice9Ex * this_, IDirect3DIndexBuffer9 * pIndexData);
    HRESULT(STDMETHODCALLTYPE * GetIndices)(IDirect3DDevice9Ex * this_, IDirect3DIndexBuffer9 ** ppIndexData);
    HRESULT(STDMETHODCALLTYPE * CreatePixelShader)(IDirect3DDevice9Ex * this_, CONST DWORD * pFunction, IDirect3DPixelShader9 ** ppShader);
    HRESULT(STDMETHODCALLTYPE * SetPixelShader)(IDirect3DDevice9Ex * this_, IDirect3DPixelShader9 * pShader);
    HRESULT(STDMETHODCALLTYPE * GetPixelShader)(IDirect3DDevice9Ex * this_, IDirect3DPixelShader9 ** ppShader);
    HRESULT(STDMETHODCALLTYPE * SetPixelShaderConstantF)(IDirect3DDevice9Ex * this_, UINT StartRegister, CONST float * pConstantData, UINT Vector4fCount);
    HRESULT(STDMETHODCALLTYPE * GetPixelShaderConstantF)(IDirect3DDevice9Ex * this_, UINT StartRegister, float * pConstantData, UINT Vector4fCount);
    HRESULT(STDMETHODCALLTYPE * SetPixelShaderConstantI)(IDirect3DDevice9Ex * this_, UINT StartRegister, CONST int * pConstantData, UINT Vector4iCount);
    HRESULT(STDMETHODCALLTYPE * GetPixelShaderConstantI)(IDirect3DDevice9Ex * this_, UINT StartRegister, int * pConstantData, UINT Vector4iCount);
    HRESULT(STDMETHODCALLTYPE * SetPixelShaderConstantB)(IDirect3DDevice9Ex * this_, UINT StartRegister, CONST BOOL * pConstantData, UINT BoolCount);
    HRESULT(STDMETHODCALLTYPE * GetPixelShaderConstantB)(IDirect3DDevice9Ex * this_, UINT StartRegister, BOOL * pConstantData, UINT BoolCount);
    HRESULT(STDMETHODCALLTYPE * DrawRectPatch)(IDirect3DDevice9Ex * this_, UINT Handle, CONST float * pNumSegs, CONST D3DRECTPATCH_INFO * pRectPatchInfo);
    HRESULT(STDMETHODCALLTYPE * DrawTriPatch)(IDirect3DDevice9Ex * this_, UINT Handle, CONST float * pNumSegs, CONST D3DTRIPATCH_INFO * pTriPatchInfo);
    HRESULT(STDMETHODCALLTYPE * DeletePatch)(IDirect3DDevice9Ex * this_, UINT Handle);
    HRESULT(STDMETHODCALLTYPE * CreateQuery)(IDirect3DDevice9Ex * this_, D3DQUERYTYPE Type, IDirect3DQuery9 ** ppQuery);
    HRESULT(STDMETHODCALLTYPE * SetConvolutionMonoKernel)(IDirect3DDevice9Ex * this_, UINT width, UINT height, float * rows, float * columns);
    HRESULT(STDMETHODCALLTYPE * ComposeRects)
    (IDirect3DDevice9Ex * this_, IDirect3DSurface9 * pSrc, IDirect3DSurface9 * pDst, IDirect3DVertexBuffer9 * pSrcRectDescs, UINT NumRects,
     IDirect3DVertexBuffer9 * pDstRectDescs, D3DCOMPOSERECTSOP Operation, int Xoffset, int Yoffset);
    HRESULT(STDMETHODCALLTYPE * PresentEx)
    (IDirect3DDevice9Ex * this_, CONST RECT * pSourceRect, CONST RECT * pDestRect, HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion, DWORD dwFlags);
    HRESULT(STDMETHODCALLTYPE * GetGPUThreadPriority)(IDirect3DDevice9Ex * this_, INT * pPriority);
    HRESULT(STDMETHODCALLTYPE * SetGPUThreadPriority)(IDirect3DDevice9Ex * this_, INT Priority);
    HRESULT(STDMETHODCALLTYPE * WaitForVBlank)(IDirect3DDevice9Ex * this_, UINT iSwapChain);
    HRESULT(STDMETHODCALLTYPE * CheckResourceResidency)(IDirect3DDevice9Ex * this_, IDirect3DResource9 ** pResourceArray, UINT32 NumResources);
    HRESULT(STDMETHODCALLTYPE * SetMaximumFrameLatency)(IDirect3DDevice9Ex * this_, UINT MaxLatency);
    HRESULT(STDMETHODCALLTYPE * GetMaximumFrameLatency)(IDirect3DDevice9Ex * this_, UINT * pMaxLatency);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceState)(IDirect3DDevice9Ex * this_, HWND hDestinationWindow);
    HRESULT(STDMETHODCALLTYPE * CreateRenderTargetEx)
    (IDirect3DDevice9Ex * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
     IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle, DWORD Usage);
    HRESULT(STDMETHODCALLTYPE * CreateOffscreenPlainSurfaceEx)
    (IDirect3DDevice9Ex * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle, DWORD Usage);
    HRESULT(STDMETHODCALLTYPE * CreateDepthStencilSurfaceEx)
    (IDirect3DDevice9Ex * this_, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard,
     IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle, DWORD Usage);
    HRESULT(STDMETHODCALLTYPE * ResetEx)
    (IDirect3DDevice9Ex * this_, D3DPRESENT_PARAMETERS * pPresentationParameters, D3DDISPLAYMODEEX * pFullscreenDisplayMode);
    HRESULT(STDMETHODCALLTYPE * GetDisplayModeEx)(IDirect3DDevice9Ex * this_, UINT iSwapChain, D3DDISPLAYMODEEX * pMode, D3DDISPLAYROTATION * pRotation);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DSwapChain9Ex
// -----------------------------------------------------------------------------
struct IDirect3DSwapChain9ExVtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DSwapChain9Ex * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DSwapChain9Ex * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DSwapChain9Ex * this_);
    HRESULT(STDMETHODCALLTYPE * Present)
    (IDirect3DSwapChain9Ex * this_, CONST RECT * pSourceRect, CONST RECT * pDestRect, HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion, DWORD dwFlags);
    HRESULT(STDMETHODCALLTYPE * GetFrontBufferData)(IDirect3DSwapChain9Ex * this_, IDirect3DSurface9 * pDestSurface);
    HRESULT(STDMETHODCALLTYPE * GetBackBuffer)(IDirect3DSwapChain9Ex * this_, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 ** ppBackBuffer);
    HRESULT(STDMETHODCALLTYPE * GetRasterStatus)(IDirect3DSwapChain9Ex * this_, D3DRASTER_STATUS * pRasterStatus);
    HRESULT(STDMETHODCALLTYPE * GetDisplayMode)(IDirect3DSwapChain9Ex * this_, D3DDISPLAYMODE * pMode);
    HRESULT(STDMETHODCALLTYPE * GetDevice)(IDirect3DSwapChain9Ex * this_, IDirect3DDevice9 ** ppDevice);
    HRESULT(STDMETHODCALLTYPE * GetPresentParameters)(IDirect3DSwapChain9Ex * this_, D3DPRESENT_PARAMETERS * pPresentationParameters);
    HRESULT(STDMETHODCALLTYPE * GetLastPresentCount)(IDirect3DSwapChain9Ex * this_, UINT * pLastPresentCount);
    HRESULT(STDMETHODCALLTYPE * GetPresentStats)(IDirect3DSwapChain9Ex * this_, D3DPRESENTSTATS * pPresentationStatistics);
    HRESULT(STDMETHODCALLTYPE * GetDisplayModeEx)(IDirect3DSwapChain9Ex * this_, D3DDISPLAYMODEEX * pMode, D3DDISPLAYROTATION * pRotation);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3D9ExOverlayExtension
// -----------------------------------------------------------------------------
struct IDirect3D9ExOverlayExtensionVtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3D9ExOverlayExtension * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3D9ExOverlayExtension * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3D9ExOverlayExtension * this_);
    HRESULT(STDMETHODCALLTYPE * CheckDeviceOverlayType)
    (IDirect3D9ExOverlayExtension * this_, UINT Adapter, D3DDEVTYPE DevType, UINT OverlayWidth, UINT OverlayHeight, D3DFORMAT OverlayFormat,
     D3DDISPLAYMODEEX * pDisplayMode, D3DDISPLAYROTATION DisplayRotation, D3DOVERLAYCAPS * pOverlayCaps);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DDevice9Video
// -----------------------------------------------------------------------------
struct IDirect3DDevice9VideoVtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DDevice9Video * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DDevice9Video * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DDevice9Video * this_);
    HRESULT(STDMETHODCALLTYPE * GetContentProtectionCaps)
    (IDirect3DDevice9Video * this_, CONST GUID * pCryptoType, CONST GUID * pDecodeProfile, D3DCONTENTPROTECTIONCAPS * pCaps);
    HRESULT(STDMETHODCALLTYPE * CreateAuthenticatedChannel)
    (IDirect3DDevice9Video * this_, D3DAUTHENTICATEDCHANNELTYPE ChannelType, IDirect3DAuthenticatedChannel9 ** ppAuthenticatedChannel, HANDLE * pChannelHandle);
    HRESULT(STDMETHODCALLTYPE * CreateCryptoSession)
    (IDirect3DDevice9Video * this_, CONST GUID * pCryptoType, CONST GUID * pDecodeProfile, IDirect3DCryptoSession9 ** ppCryptoSession, HANDLE * pCryptoHandle);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DAuthenticatedChannel9
// -----------------------------------------------------------------------------
struct IDirect3DAuthenticatedChannel9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DAuthenticatedChannel9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DAuthenticatedChannel9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DAuthenticatedChannel9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetCertificateSize)(IDirect3DAuthenticatedChannel9 * this_, UINT * pCertificateSize);
    HRESULT(STDMETHODCALLTYPE * GetCertificate)(IDirect3DAuthenticatedChannel9 * this_, UINT CertifacteSize, BYTE * ppCertificate);
    HRESULT(STDMETHODCALLTYPE * NegotiateKeyExchange)(IDirect3DAuthenticatedChannel9 * this_, UINT DataSize, VOID * pData);
    HRESULT(STDMETHODCALLTYPE * Query)(IDirect3DAuthenticatedChannel9 * this_, UINT InputSize, CONST VOID * pInput, UINT OutputSize, VOID * pOutput);
    HRESULT(STDMETHODCALLTYPE * Configure)
    (IDirect3DAuthenticatedChannel9 * this_, UINT InputSize, CONST VOID * pInput, D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT * pOutput);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// IDirect3DCryptoSession9
// -----------------------------------------------------------------------------
struct IDirect3DCryptoSession9Vtbl {
    BEGIN_INTERFACE
    HRESULT(STDMETHODCALLTYPE * QueryInterface)(IDirect3DCryptoSession9 * this_, REFIID riid, void ** ppvObj);
    ULONG(STDMETHODCALLTYPE * AddRef)(IDirect3DCryptoSession9 * this_);
    ULONG(STDMETHODCALLTYPE * Release)(IDirect3DCryptoSession9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetCertificateSize)(IDirect3DCryptoSession9 * this_, UINT * pCertificateSize);
    HRESULT(STDMETHODCALLTYPE * GetCertificate)(IDirect3DCryptoSession9 * this_, UINT CertifacteSize, BYTE * ppCertificate);
    HRESULT(STDMETHODCALLTYPE * NegotiateKeyExchange)(IDirect3DCryptoSession9 * this_, UINT DataSize, VOID * pData);
    HRESULT(STDMETHODCALLTYPE * EncryptionBlt)
    (IDirect3DCryptoSession9 * this_, IDirect3DSurface9 * pSrcSurface, IDirect3DSurface9 * pDstSurface, UINT DstSurfaceSize, VOID * pIV);
    HRESULT(STDMETHODCALLTYPE * DecryptionBlt)
    (IDirect3DCryptoSession9 * this_, IDirect3DSurface9 * pSrcSurface, IDirect3DSurface9 * pDstSurface, UINT SrcSurfaceSize,
     D3DENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo, VOID * pContentKey, VOID * pIV);
    HRESULT(STDMETHODCALLTYPE * GetSurfacePitch)(IDirect3DCryptoSession9 * this_, IDirect3DSurface9 * pSrcSurface, UINT * pSurfacePitch);
    HRESULT(STDMETHODCALLTYPE * StartSessionKeyRefresh)(IDirect3DCryptoSession9 * this_, VOID * pRandomNumber, UINT RandomNumberSize);
    HRESULT(STDMETHODCALLTYPE * FinishSessionKeyRefresh)(IDirect3DCryptoSession9 * this_);
    HRESULT(STDMETHODCALLTYPE * GetEncryptionBltKey)(IDirect3DCryptoSession9 * this_, VOID * pReadbackKey, UINT KeySize);
    END_INTERFACE
};

// -----------------------------------------------------------------------------
// Global vtables for all D3D9/DXGI classes
// -----------------------------------------------------------------------------

struct D3D9VTables {
    VTable<IDirect3D9Vtbl>                     _IDirect3D9;
    VTable<IDirect3DDevice9Vtbl>               _IDirect3DDevice9;
    VTable<IDirect3DStateBlock9Vtbl>           _IDirect3DStateBlock9;
    VTable<IDirect3DSwapChain9Vtbl>            _IDirect3DSwapChain9;
    VTable<IDirect3DResource9Vtbl>             _IDirect3DResource9;
    VTable<IDirect3DVertexDeclaration9Vtbl>    _IDirect3DVertexDeclaration9;
    VTable<IDirect3DVertexShader9Vtbl>         _IDirect3DVertexShader9;
    VTable<IDirect3DPixelShader9Vtbl>          _IDirect3DPixelShader9;
    VTable<IDirect3DBaseTexture9Vtbl>          _IDirect3DBaseTexture9;
    VTable<IDirect3DTexture9Vtbl>              _IDirect3DTexture9;
    VTable<IDirect3DVolumeTexture9Vtbl>        _IDirect3DVolumeTexture9;
    VTable<IDirect3DCubeTexture9Vtbl>          _IDirect3DCubeTexture9;
    VTable<IDirect3DVertexBuffer9Vtbl>         _IDirect3DVertexBuffer9;
    VTable<IDirect3DIndexBuffer9Vtbl>          _IDirect3DIndexBuffer9;
    VTable<IDirect3DSurface9Vtbl>              _IDirect3DSurface9;
    VTable<IDirect3DVolume9Vtbl>               _IDirect3DVolume9;
    VTable<IDirect3DQuery9Vtbl>                _IDirect3DQuery9;
    VTable<IDirect3D9ExVtbl>                   _IDirect3D9Ex;
    VTable<IDirect3DDevice9ExVtbl>             _IDirect3DDevice9Ex;
    VTable<IDirect3DSwapChain9ExVtbl>          _IDirect3DSwapChain9Ex;
    VTable<IDirect3D9ExOverlayExtensionVtbl>   _IDirect3D9ExOverlayExtension;
    VTable<IDirect3DDevice9VideoVtbl>          _IDirect3DDevice9Video;
    VTable<IDirect3DAuthenticatedChannel9Vtbl> _IDirect3DAuthenticatedChannel9;
    VTable<IDirect3DCryptoSession9Vtbl>        _IDirect3DCryptoSession9;
};

extern D3D9VTables g_D3D9OriginVTables;
extern D3D9VTables g_D3D9HookedVTables;

// -----------------------------------------------------------------------------
// Real -> Hook Functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3D9(IDirect3D9 * p) {
    if (p) RealToHooked_General(**(IDirect3D9Vtbl **) p, g_D3D9OriginVTables._IDirect3D9, g_D3D9HookedVTables._IDirect3D9, "IDirect3D9");
}
template<>
inline void RealToHooked9<IDirect3D9>(IDirect3D9 * p) {
    return RealToHooked9_IDirect3D9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DDevice9(IDirect3DDevice9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DDevice9Vtbl **) p, g_D3D9OriginVTables._IDirect3DDevice9, g_D3D9HookedVTables._IDirect3DDevice9, "IDirect3DDevice9");
}
template<>
inline void RealToHooked9<IDirect3DDevice9>(IDirect3DDevice9 * p) {
    return RealToHooked9_IDirect3DDevice9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DStateBlock9(IDirect3DStateBlock9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DStateBlock9Vtbl **) p, g_D3D9OriginVTables._IDirect3DStateBlock9, g_D3D9HookedVTables._IDirect3DStateBlock9,
                             "IDirect3DStateBlock9");
}
template<>
inline void RealToHooked9<IDirect3DStateBlock9>(IDirect3DStateBlock9 * p) {
    return RealToHooked9_IDirect3DStateBlock9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DSwapChain9(IDirect3DSwapChain9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DSwapChain9Vtbl **) p, g_D3D9OriginVTables._IDirect3DSwapChain9, g_D3D9HookedVTables._IDirect3DSwapChain9,
                             "IDirect3DSwapChain9");
}
template<>
inline void RealToHooked9<IDirect3DSwapChain9>(IDirect3DSwapChain9 * p) {
    return RealToHooked9_IDirect3DSwapChain9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DResource9(IDirect3DResource9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DResource9Vtbl **) p, g_D3D9OriginVTables._IDirect3DResource9, g_D3D9HookedVTables._IDirect3DResource9,
                             "IDirect3DResource9");
}
template<>
inline void RealToHooked9<IDirect3DResource9>(IDirect3DResource9 * p) {
    return RealToHooked9_IDirect3DResource9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DVertexDeclaration9(IDirect3DVertexDeclaration9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DVertexDeclaration9Vtbl **) p, g_D3D9OriginVTables._IDirect3DVertexDeclaration9,
                             g_D3D9HookedVTables._IDirect3DVertexDeclaration9, "IDirect3DVertexDeclaration9");
}
template<>
inline void RealToHooked9<IDirect3DVertexDeclaration9>(IDirect3DVertexDeclaration9 * p) {
    return RealToHooked9_IDirect3DVertexDeclaration9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DVertexShader9(IDirect3DVertexShader9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DVertexShader9Vtbl **) p, g_D3D9OriginVTables._IDirect3DVertexShader9, g_D3D9HookedVTables._IDirect3DVertexShader9,
                             "IDirect3DVertexShader9");
}
template<>
inline void RealToHooked9<IDirect3DVertexShader9>(IDirect3DVertexShader9 * p) {
    return RealToHooked9_IDirect3DVertexShader9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DPixelShader9(IDirect3DPixelShader9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DPixelShader9Vtbl **) p, g_D3D9OriginVTables._IDirect3DPixelShader9, g_D3D9HookedVTables._IDirect3DPixelShader9,
                             "IDirect3DPixelShader9");
}
template<>
inline void RealToHooked9<IDirect3DPixelShader9>(IDirect3DPixelShader9 * p) {
    return RealToHooked9_IDirect3DPixelShader9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DBaseTexture9(IDirect3DBaseTexture9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DBaseTexture9Vtbl **) p, g_D3D9OriginVTables._IDirect3DBaseTexture9, g_D3D9HookedVTables._IDirect3DBaseTexture9,
                             "IDirect3DBaseTexture9");
}
template<>
inline void RealToHooked9<IDirect3DBaseTexture9>(IDirect3DBaseTexture9 * p) {
    return RealToHooked9_IDirect3DBaseTexture9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DTexture9(IDirect3DTexture9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DTexture9Vtbl **) p, g_D3D9OriginVTables._IDirect3DTexture9, g_D3D9HookedVTables._IDirect3DTexture9,
                             "IDirect3DTexture9");
}
template<>
inline void RealToHooked9<IDirect3DTexture9>(IDirect3DTexture9 * p) {
    return RealToHooked9_IDirect3DTexture9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DVolumeTexture9(IDirect3DVolumeTexture9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DVolumeTexture9Vtbl **) p, g_D3D9OriginVTables._IDirect3DVolumeTexture9, g_D3D9HookedVTables._IDirect3DVolumeTexture9,
                             "IDirect3DVolumeTexture9");
}
template<>
inline void RealToHooked9<IDirect3DVolumeTexture9>(IDirect3DVolumeTexture9 * p) {
    return RealToHooked9_IDirect3DVolumeTexture9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DCubeTexture9(IDirect3DCubeTexture9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DCubeTexture9Vtbl **) p, g_D3D9OriginVTables._IDirect3DCubeTexture9, g_D3D9HookedVTables._IDirect3DCubeTexture9,
                             "IDirect3DCubeTexture9");
}
template<>
inline void RealToHooked9<IDirect3DCubeTexture9>(IDirect3DCubeTexture9 * p) {
    return RealToHooked9_IDirect3DCubeTexture9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DVertexBuffer9(IDirect3DVertexBuffer9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DVertexBuffer9Vtbl **) p, g_D3D9OriginVTables._IDirect3DVertexBuffer9, g_D3D9HookedVTables._IDirect3DVertexBuffer9,
                             "IDirect3DVertexBuffer9");
}
template<>
inline void RealToHooked9<IDirect3DVertexBuffer9>(IDirect3DVertexBuffer9 * p) {
    return RealToHooked9_IDirect3DVertexBuffer9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DIndexBuffer9(IDirect3DIndexBuffer9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DIndexBuffer9Vtbl **) p, g_D3D9OriginVTables._IDirect3DIndexBuffer9, g_D3D9HookedVTables._IDirect3DIndexBuffer9,
                             "IDirect3DIndexBuffer9");
}
template<>
inline void RealToHooked9<IDirect3DIndexBuffer9>(IDirect3DIndexBuffer9 * p) {
    return RealToHooked9_IDirect3DIndexBuffer9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DSurface9(IDirect3DSurface9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DSurface9Vtbl **) p, g_D3D9OriginVTables._IDirect3DSurface9, g_D3D9HookedVTables._IDirect3DSurface9,
                             "IDirect3DSurface9");
}
template<>
inline void RealToHooked9<IDirect3DSurface9>(IDirect3DSurface9 * p) {
    return RealToHooked9_IDirect3DSurface9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DVolume9(IDirect3DVolume9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DVolume9Vtbl **) p, g_D3D9OriginVTables._IDirect3DVolume9, g_D3D9HookedVTables._IDirect3DVolume9, "IDirect3DVolume9");
}
template<>
inline void RealToHooked9<IDirect3DVolume9>(IDirect3DVolume9 * p) {
    return RealToHooked9_IDirect3DVolume9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DQuery9(IDirect3DQuery9 * p) {
    if (p) RealToHooked_General(**(IDirect3DQuery9Vtbl **) p, g_D3D9OriginVTables._IDirect3DQuery9, g_D3D9HookedVTables._IDirect3DQuery9, "IDirect3DQuery9");
}
template<>
inline void RealToHooked9<IDirect3DQuery9>(IDirect3DQuery9 * p) {
    return RealToHooked9_IDirect3DQuery9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3D9Ex(IDirect3D9Ex * p) {
    if (p) RealToHooked_General(**(IDirect3D9ExVtbl **) p, g_D3D9OriginVTables._IDirect3D9Ex, g_D3D9HookedVTables._IDirect3D9Ex, "IDirect3D9Ex");
}
template<>
inline void RealToHooked9<IDirect3D9Ex>(IDirect3D9Ex * p) {
    return RealToHooked9_IDirect3D9Ex(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DDevice9Ex(IDirect3DDevice9Ex * p) {
    if (p)
        RealToHooked_General(**(IDirect3DDevice9ExVtbl **) p, g_D3D9OriginVTables._IDirect3DDevice9Ex, g_D3D9HookedVTables._IDirect3DDevice9Ex,
                             "IDirect3DDevice9Ex");
}
template<>
inline void RealToHooked9<IDirect3DDevice9Ex>(IDirect3DDevice9Ex * p) {
    return RealToHooked9_IDirect3DDevice9Ex(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DSwapChain9Ex(IDirect3DSwapChain9Ex * p) {
    if (p)
        RealToHooked_General(**(IDirect3DSwapChain9ExVtbl **) p, g_D3D9OriginVTables._IDirect3DSwapChain9Ex, g_D3D9HookedVTables._IDirect3DSwapChain9Ex,
                             "IDirect3DSwapChain9Ex");
}
template<>
inline void RealToHooked9<IDirect3DSwapChain9Ex>(IDirect3DSwapChain9Ex * p) {
    return RealToHooked9_IDirect3DSwapChain9Ex(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3D9ExOverlayExtension(IDirect3D9ExOverlayExtension * p) {
    if (p)
        RealToHooked_General(**(IDirect3D9ExOverlayExtensionVtbl **) p, g_D3D9OriginVTables._IDirect3D9ExOverlayExtension,
                             g_D3D9HookedVTables._IDirect3D9ExOverlayExtension, "IDirect3D9ExOverlayExtension");
}
template<>
inline void RealToHooked9<IDirect3D9ExOverlayExtension>(IDirect3D9ExOverlayExtension * p) {
    return RealToHooked9_IDirect3D9ExOverlayExtension(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DDevice9Video(IDirect3DDevice9Video * p) {
    if (p)
        RealToHooked_General(**(IDirect3DDevice9VideoVtbl **) p, g_D3D9OriginVTables._IDirect3DDevice9Video, g_D3D9HookedVTables._IDirect3DDevice9Video,
                             "IDirect3DDevice9Video");
}
template<>
inline void RealToHooked9<IDirect3DDevice9Video>(IDirect3DDevice9Video * p) {
    return RealToHooked9_IDirect3DDevice9Video(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DAuthenticatedChannel9(IDirect3DAuthenticatedChannel9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DAuthenticatedChannel9Vtbl **) p, g_D3D9OriginVTables._IDirect3DAuthenticatedChannel9,
                             g_D3D9HookedVTables._IDirect3DAuthenticatedChannel9, "IDirect3DAuthenticatedChannel9");
}
template<>
inline void RealToHooked9<IDirect3DAuthenticatedChannel9>(IDirect3DAuthenticatedChannel9 * p) {
    return RealToHooked9_IDirect3DAuthenticatedChannel9(p);
}

// -----------------------------------------------------------------------------
inline void RealToHooked9_IDirect3DCryptoSession9(IDirect3DCryptoSession9 * p) {
    if (p)
        RealToHooked_General(**(IDirect3DCryptoSession9Vtbl **) p, g_D3D9OriginVTables._IDirect3DCryptoSession9, g_D3D9HookedVTables._IDirect3DCryptoSession9,
                             "IDirect3DCryptoSession9");
}
template<>
inline void RealToHooked9<IDirect3DCryptoSession9>(IDirect3DCryptoSession9 * p) {
    return RealToHooked9_IDirect3DCryptoSession9(p);
}
