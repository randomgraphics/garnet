#include "pch.h"
#include "d3d10Gpu.h"

#if GN_MSVC
#pragma comment(lib, "d3d9.lib") // for PIX routines.
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "dxguid.lib")
#if GN_BUILD_DEBUG
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx10.lib")
#endif

#endif // GN_MSVC

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.D3D10");

// *****************************************************************************
// Global functions
// *****************************************************************************

bool gD3D10EnablePixPerf = true; // default is enabled

#if GN_BUILD_STATIC
GN::gfx::Gpu *
GNgfxCreateD3D10Gpu( const GN::gfx::GpuOptions & o )
#else
extern "C" GN_EXPORT GN::gfx::Gpu *
GNgfxCreateGpu( const GN::gfx::GpuOptions & o )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3D10Gpu> p( new GN::gfx::D3D10Gpu );
    if( !p->Init( o ) ) return 0;
    return p.Detach();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Gpu::Init( const GN::gfx::GpuOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10Gpu, ( o ) );

    // init sub-components
    if( !dispInit()        ) return Failure();
    if( !capsInit()         ) return Failure();
    if( !resourceInit()     ) return Failure();
    if( !contextInit()      ) return Failure();
    if( !drawInit()         ) return Failure();

    // successful
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::Quit()
{
    GN_GUARD;

    drawQuit();
    contextQuit();
    resourceQuit();
    capsQuit();
    dispQuit();

    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Misc. utilities
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::getBackBufferContent( BackBufferContent & bc )
{
    bc.data.Clear();
    bc.format = ColorFormat::UNKNOWN;
    bc.width = 0;
    bc.height = 0;
    bc.pitch = 0;

    AutoComPtr<ID3D10Texture2D> backbuf;
    GN_DX_CHECK_RETURN_VOID( mSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backbuf ) );

    D3D10_TEXTURE2D_DESC desc;
    backbuf->GetDesc( &desc );

    /// create a temporary copy of back buffer in system memory for reading data back
    AutoComPtr<ID3D10Texture2D> sysbuf;
    desc.Usage = D3D10_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
    GN_DX_CHECK_RETURN_VOID( mDevice->CreateTexture2D( &desc, NULL, &sysbuf ) );
    mDevice->CopyResource( sysbuf, backbuf );

    D3D10_MAPPED_TEXTURE2D mt;
    GN_DX_CHECK_RETURN_VOID( sysbuf->Map( 0, D3D10_MAP_READ, 0, &mt ) );

    bc.format = dxgiFormat2ColorFormat( desc.Format );
    bc.width  = desc.Width;
    bc.height = desc.Height;
    bc.pitch  = mt.RowPitch;
    bc.data.Append( (UInt8*)mt.pData, mt.RowPitch * desc.Height );

    sysbuf->Unmap( 0 );
}
