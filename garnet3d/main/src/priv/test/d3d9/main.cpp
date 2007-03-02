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

// *****************************************************************************
// main application class
// *****************************************************************************

class MyApp : public SampleApp
{
    struct Vertex
    {
        float p[3];
        float n[3];
        float t[2];
    };

    LPDIRECT3DDEVICE9 dev;

    AutoComPtr<IDirect3DVertexDeclaration9> decl;
    AutoComPtr<IDirect3DVertexShader9> vs;
    AutoComPtr<IDirect3DPixelShader9> ps;

    Vertex vb[24];
    UInt16 ib[36];

    Matrix44f world, view, proj;

    util::ArcBall arcball;

public:
    bool onAppInit()
    {
        static const float E = 150.0f;
        createBox(
            E, E, E,
            vb[0].p, sizeof(Vertex),
            vb[0].t, sizeof(Vertex),
            vb[0].n, sizeof(Vertex),
            ib, 0 );

        world.identity();
#if 1
        view.translate( 0, 0, -300 );
        proj.perspectiveD3DRh( 1.0f, 4.0f/3.0f, 10.0f, 1000.0f );
        arcball.setHandness( util::ArcBall::RIGHT_HAND );
        arcball.setViewMatrix( view );
#else
        view.translate( 0, 0, 300 );
        proj.perspectiveD3DLh( 1.0f, 4.0f/3.0f, 10.0f, 1000.0f );
        arcball.setHandness( util::ArcBall::LEFT_HAND );
        arcball.setViewMatrix( view );
#endif
        arcball.connectToInput();

        return true;
    }

    void onAppQuit()
    {
    }

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
        ip.ro.vsync = true;
    }

    bool onRendererRestore()
    {
        dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();

        D3DVERTEXELEMENT9 format[] = 
        {
            { 0,  0, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_TEXCOORD, 1 },
            { 0, 24, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 2 },
            D3DDECL_END()
        };
        GN_DX9_CHECK_RV( dev->CreateVertexDeclaration( format, &decl ), false );

        static const char * vs_code =
            "uniform float4x4 gPvw : register(c0);    \n"
            "struct vsi                               \n"
            "{                                        \n"
            "    float4 pos : TEXCOORD0;              \n"
            "    float3 nml : TEXCOORD1;              \n"
            "    float2 uv  : TEXCOORD0;              \n"
            "};                                       \n"
            "struct vso                               \n"
            "{                                        \n"
            "    float4 pos : POSITION;               \n"
            "    float4 clr : COLOR0;                 \n"
            "    float2 uv  : TEXCOORD0;              \n"
            "};                                       \n"
            "vso main( vsi i )                        \n"
            "{                                        \n"
            "    vso o;                               \n"
            "    o.pos = mul( gPvw, i.pos );          \n"
            "    float3 n = 2*abs(i.nml) + i.nml;     \n"
            "    o.clr = float4( n/3.0, 1.0 );        \n"
            "    o.uv = i.uv;                         \n"
            "    return o;                            \n"
            "}";
        vs.attach( GN::gfx::d3d9::compileVS( dev, vs_code, 0, D3DXSHADER_PACKMATRIX_ROWMAJOR ) );
        if( !vs ) return false;

        static const char * ps_code =
            "float4 main(                             \n"
            "    float4 clr : COLOR0,                 \n"
            "    float2 uv  : TEXCOORD0 )             \n"
            "    : COLOR0                             \n"
            "{                                        \n"
            "    return clr;                          \n"
            "}";
        ps.attach( GN::gfx::d3d9::compilePS( dev, ps_code ) );
        if( !ps ) return false;

        // initial arcball window
        const DispDesc & dd = gRenderer.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );

        // success
        return true;
    }

    void onRendererDispose()
    {
        dev = 0;
        decl.clear();
        vs.clear();
        ps.clear();
    }

    void onUpdate()
    {
        world = arcball.getRotationMatrix();
        Matrix44f pvw = proj * view * world;
        dev->SetVertexShaderConstantF( 0, (const float*)&pvw, 4 );
    }

    void onRender()
    {
        dev->Clear( 0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );

        // set render state
        dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

        // draw box
        dev->SetVertexShader( vs );
        dev->SetPixelShader( ps );
        dev->SetVertexDeclaration( decl );
        dev->DrawIndexedPrimitiveUP(
            D3DPT_TRIANGLELIST,
            0,
            24,
            12,
            ib,
            D3DFMT_INDEX16,
            vb,
            sizeof(Vertex) );

        // restore renderer states.
        RendererContext::FieldFlags ff;
        ff.u32 = 0;
        ff.setShaderBit( SHADER_VS );
        ff.setShaderBit( SHADER_PS );
        ff.vtxFmt = 1;
        ff.rsb = 1;
        gRenderer.rebindContext( ff );
    }
};
int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
