#include "pch.h"

#if GN_MSVC

#if GN_XENON
#if GN_DEBUG_BUILD
#pragma comment(lib, "d3d9d.lib")
#pragma comment( lib, "xgraphicsd.lib" )
#pragma comment(lib, "xapilibd.lib")
#elif GN_PROFILE_BUILD
#pragma comment(lib, "d3d9i.lib")
#pragma comment( lib, "xgraphics.lib" )
#pragma comment(lib, "xapilibi.lib")
#else
#pragma comment(lib, "d3d9.lib")
#pragma comment( lib, "xgraphics.lib" )
#pragma comment(lib, "xapilib.lib")
#endif
#else
#if GN_DEBUG_BUILD
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif
#endif

#endif

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

class MyApp : public SampleApp
{
    AutoComPtr<IDirect3DVertexDeclaration9> decl;
    AutoComPtr<IDirect3DVertexShader9> vs;
    AutoComPtr<IDirect3DPixelShader9> ps;
public:
    bool onAppInit()
    {
        return true;
    }

    void onAppQuit()
    {
    }

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
    }

    bool onRendererRestore()
    {
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();

        D3DVERTEXELEMENT9 format[] = 
        {
            { 0, 0, D3DDECLTYPE_FLOAT1, 0, D3DDECLUSAGE_POSITION, 0 },
            { 0, 4, D3DDECLTYPE_FLOAT1, 0, D3DDECLUSAGE_BLENDINDICES, 0 },
            D3DDECL_END()
        };
        GN_DX9_CHECK_RV( dev->CreateVertexDeclaration( format, &decl ), false );

        static const char * vs_code =
            "struct vsio                              \n"
            "{                                        \n"
            "    float4 pos : POSITION;               \n"
            "    float2 uv : TEXCOORD0;               \n"
            "};                                       \n"
            "vsio main( vsio i )                      \n"
            "{                                        \n"
            "    vsio o;                              \n"
            "    o.pos = i.pos - float4(0.5,0.5,0,0); \n"
            "    o.pos.xy *= 2;                       \n"
            "    o.uv.x = i.pos.x * (2e37-1e37) + 1e37; \n"
            "    o.uv.y = 0;                          \n"
            "    return o;                            \n"
            "}\n";
        vs.attach( GN::gfx::d3d9::compileVS( dev, vs_code ) );
        if( !vs ) return false;

        static const char * ps_code =
            "float4 main( float2 uv : TEXCOORD0 ) : COLOR0 \n"
            "{                               \n"
            "    const float k = 1.5e37;     \n"
            "    return (uv.x > k) ? 1 : 0;  \n"
            "}\n";
        ps.attach( GN::gfx::d3d9::compilePS( dev, ps_code ) );
        if( !ps ) return false;

        // success
        return true;
    }

    void onRendererDispose()
    {
        decl.clear();
        vs.clear();
        ps.clear();
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)r.getD3DDevice();
        dev->Clear( 0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );

        // draw quad
        dev->SetVertexShader( vs );
        dev->SetPixelShader( ps );
        r.draw2DTexturedQuad( DQ_USE_CURRENT_VS | DQ_USE_CURRENT_PS );

        // restore renderer states.
        RendererContext::FieldFlags ff;
        ff.u32 = 0;
        ff.vtxShaderBit();
        ff.pxlShaderBit();
        r.rebindContext( ff );
    }
};
int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
