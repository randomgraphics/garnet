#include "pch.h"
#include "d3d10Renderer.h"

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

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// Global functions
// *****************************************************************************

bool gD3D10EnablePixPerf = true; // default is enabled

#if GN_BUILD_STATIC
GN::gfx::Renderer *
GNgfxCreateD3D10Renderer( const GN::gfx::RendererOptions & o )
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::RendererOptions & o )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3D10Renderer> p( new GN::gfx::D3D10Renderer );
    if( !p->init( o ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::init( const GN::gfx::RendererOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10Renderer, ( o ) );

    // init sub-components
    if( !dispInit()        ) return failure();
    if( !capsInit()         ) return failure();
    if( !resourceInit()     ) return failure();
    if( !contextInit()      ) return failure();
    if( !drawInit()         ) return failure();

    // successful
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::quit()
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
void GN::gfx::D3D10Renderer::getBackBufferContent( BackBufferContent & bc )
{
    bc.data.clear();
    bc.format = ColorFormat::UNKNOWN;
    bc.width = 0;
    bc.height = 0;
    bc.pitch = 0;

    AutoComPtr<ID3D10Texture2D> backbuf;
    GN_DX10_CHECK_R( mSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backbuf ) );

    D3D10_TEXTURE2D_DESC desc;
    backbuf->GetDesc( &desc );

    /// create a temporary copy of back buffer in system memory for reading data back
    AutoComPtr<ID3D10Texture2D> sysbuf;
    desc.Usage = D3D10_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
    GN_DX10_CHECK_R( mDevice->CreateTexture2D( &desc, NULL, &sysbuf ) );
    mDevice->CopyResource( sysbuf, backbuf );

    D3D10_MAPPED_TEXTURE2D mt;
    GN_DX10_CHECK_R( sysbuf->Map( 0, D3D10_MAP_READ, 0, &mt ) );

    bc.format = dxgiFormat2ColorFormat( desc.Format );
    bc.width  = desc.Width;
    bc.height = desc.Height;
    bc.pitch  = mt.RowPitch;
    bc.data.append( (UInt8*)mt.pData, mt.RowPitch * desc.Height );

    sysbuf->Unmap( 0 );
}
