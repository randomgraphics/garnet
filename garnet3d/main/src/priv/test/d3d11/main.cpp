#include "pch.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "d3dx11effect.h"

static const char * fxcode =
#include "shaders.h"
;

using namespace GN;
using namespace GN::d3d11;
using namespace GN::util;

#define RENDER_TO_TEXTURE 0

// *****************************************************************************
// application class
// *****************************************************************************

class MyApp : public D3D11Application
{
#if RENDER_TO_TEXTURE
    RenderToTexture           rtt;
    ScreenAlignedQuad         quad;
#endif
    SimpleMesh                mesh;
    AutoComPtr<ID3DX11Effect> effect;
    ArcBall                   arcball;
    Matrix44f                 world, view, proj;

public:

    bool onCreateDevice()
    {
        GN_GUARD;

        ID3D11Device & dev = device();
        ID3D11DeviceContext & cxt = context();

        const D3D11AppOption & option = getOption();

#if RENDER_TO_TEXTURE
        RenderToTextureOption rtto = { option.width, option.height, DXGI_FORMAT_R8G8B8A8_UNORM, 1, MSAA_DISABLE, false };
        if( !rtt.init( &dev, rtto ) ) return false;
        ScreenAlignedQuadDesc saqd;
        saqd.makeDefault();
        if( !quad.init( dev, &saqd ) ) return false;
#endif

        // create mesh
        static SimpleMesh::Vertex vertices[24];
        static uint16             indices[36];
        gfx::createBox(
            1.0f, 1.0f, 1.0f,
            vertices[0].pos, sizeof(SimpleMesh::Vertex),
            vertices[0].tex, sizeof(SimpleMesh::Vertex),
            vertices[0].normal, sizeof(SimpleMesh::Vertex),
            0, 0, // tangent
            0, 0, // binormal
            indices,
            0 );  // quads
        if( !mesh.init( cxt ) ) return false;
        mesh.setVertices( vertices, GN_ARRAY_COUNT(vertices) );
        mesh.setTriangles( indices, GN_ARRAY_COUNT(indices)/3 );

        // compile effect
        AutoComPtr<ID3DBlob> binary = AutoComPtr<ID3DBlob>::sAttach(compileShader(
            "fx_5_0",
            fxcode, str::length(fxcode),
            D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
            NULL )); // entry
        if (nullptr == binary) return false;

        // create effect
        GN_RETURN_FALSE_ON_HR_FAILED( D3DX11CreateEffectFromMemory(
            binary->GetBufferPointer(), binary->GetBufferSize(),
            D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
            &dev,
            &effect));

        // initialize arcball
        world.identity();
        view.translate( 0, 0, 2 );
        proj.perspectiveD3DLh( 1.0f, 4.0f/3.0f, 0.1f, 10.0f );
        arcball.setHandness( util::LEFT_HAND );
        arcball.setViewMatrix( view );
        arcball.connectToInput();
        arcball.setMouseMoveWindow( 0, 0, (int)option.width, (int)option.height );

        // success
        return true;

        GN_UNGUARD;
    }

    void onDestroyDevice()
    {
        effect.clear();
        mesh.quit();
#if RENDER_TO_TEXTURE
        quad.quit();
        rtt.quit();
#endif
    }

    void onUpdate()
    {
        world = arcball.getRotationMatrix44();
        Matrix44f pvw = proj * view * world;
        effect->GetVariableByName( "gPVW" )->AsMatrix()->SetMatrix( (float*)&pvw );
    }

    void onDraw()
    {
        auto & cxt = context();

#if RENDER_TO_TEXTURE
        rtt.clearScreen( 0, 1.0f, 1.0f, 0, 1.0f, 0 );
        rtt.bindWithDepth();
#else
        clearScreen( 0, 1.0f, 1.0f, 0, 1.0f, 0 );
#endif

        ID3DX11EffectTechnique * tech = effect->GetTechniqueByIndex( 0 );
        D3DX11_TECHNIQUE_DESC techDesc;
        tech->GetDesc( &techDesc );
        for( UINT p = 0; p < techDesc.Passes; ++p )
        {
            tech->GetPassByIndex( p )->Apply( 0, &cxt );
            mesh.drawIndexed();
        }

#if RENDER_TO_TEXTURE
        resetToDefaultRenderTargets();
        quad.drawTexed( rtt.getColorBuffer(0).srv );
#endif
    }
};

// *****************************************************************************
// main function
// *****************************************************************************

int main()
{
    D3D11AppOption option;
    MyApp          app;
    return app.run( &option );
}
