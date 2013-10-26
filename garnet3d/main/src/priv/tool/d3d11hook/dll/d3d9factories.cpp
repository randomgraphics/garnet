// script generated file. DO NOT edit.

#include "pch.h"
#include "d3d9hooks.h"

void HookedClassFactory::registerAll()
{
    registerFactory<IDirect3DDevice9>(Direct3DDevice9Hook::sNewInstance, Direct3DDevice9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DIndexBuffer9>(Direct3DIndexBuffer9Hook::sNewInstance, Direct3DIndexBuffer9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DResource9>(Direct3DResource9Hook::sNewInstance, Direct3DResource9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DBaseTexture9>(Direct3DBaseTexture9Hook::sNewInstance, Direct3DBaseTexture9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DSwapChain9Ex>(Direct3DSwapChain9ExHook::sNewInstance, Direct3DSwapChain9ExHook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DQuery9>(Direct3DQuery9Hook::sNewInstance, Direct3DQuery9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3D9Ex>(Direct3D9ExHook::sNewInstance, Direct3D9ExHook::sDeleteInstance, nullptr);
    registerFactory<IDirect3D9ExOverlayExtension>(Direct3D9ExOverlayExtensionHook::sNewInstance, Direct3D9ExOverlayExtensionHook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DPixelShader9>(Direct3DPixelShader9Hook::sNewInstance, Direct3DPixelShader9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DCryptoSession9>(Direct3DCryptoSession9Hook::sNewInstance, Direct3DCryptoSession9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DCubeTexture9>(Direct3DCubeTexture9Hook::sNewInstance, Direct3DCubeTexture9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DSurface9>(Direct3DSurface9Hook::sNewInstance, Direct3DSurface9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DVolume9>(Direct3DVolume9Hook::sNewInstance, Direct3DVolume9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DVertexShader9>(Direct3DVertexShader9Hook::sNewInstance, Direct3DVertexShader9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DStateBlock9>(Direct3DStateBlock9Hook::sNewInstance, Direct3DStateBlock9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DAuthenticatedChannel9>(Direct3DAuthenticatedChannel9Hook::sNewInstance, Direct3DAuthenticatedChannel9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DSwapChain9>(Direct3DSwapChain9Hook::sNewInstance, Direct3DSwapChain9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DVertexBuffer9>(Direct3DVertexBuffer9Hook::sNewInstance, Direct3DVertexBuffer9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3D9>(Direct3D9Hook::sNewInstance, Direct3D9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DDevice9Video>(Direct3DDevice9VideoHook::sNewInstance, Direct3DDevice9VideoHook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DDevice9Ex>(Direct3DDevice9ExHook::sNewInstance, Direct3DDevice9ExHook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DTexture9>(Direct3DTexture9Hook::sNewInstance, Direct3DTexture9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DVolumeTexture9>(Direct3DVolumeTexture9Hook::sNewInstance, Direct3DVolumeTexture9Hook::sDeleteInstance, nullptr);
    registerFactory<IDirect3DVertexDeclaration9>(Direct3DVertexDeclaration9Hook::sNewInstance, Direct3DVertexDeclaration9Hook::sDeleteInstance, nullptr);
}
