#include "pch.h"
#include "mydevice9.h"

//static GN::Logger * sLogger = GN::getLogger("GN.tool.d3d9wrapper.MyDevice9.IdxBuf");

using namespace GN;

// *****************************************************************************
// MyDevice9
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::CreateIndexBuffer(
    THIS_ UINT Length,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DIndexBuffer9** ppIndexBuffer,
    HANDLE* pSharedHandle)
{
    return realobj()->CreateIndexBuffer( Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle );
}
