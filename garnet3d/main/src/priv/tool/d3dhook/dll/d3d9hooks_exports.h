#include <windows.h>
#include "d3d/d3d9.h"

#ifdef INSIDE_D3D9_HOOK
#define D3D9_HOOK_API __declspec(dllexport)
#else
#define D3D9_HOOK_API __declspec(dllimport)
#endif

D3D9_HOOK_API IDirect3D9 * WINAPI Direct3DCreate9Hook(UINT SDKVersion);

D3D9_HOOK_API HRESULT WINAPI Direct3DCreate9ExHook(UINT SDKVersion, IDirect3D9Ex **ppD3D);
