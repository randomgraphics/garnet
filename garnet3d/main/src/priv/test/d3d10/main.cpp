#include "pch.h"
#include "garnet/GNgfx.h"
#include "garnet/GNutil.h"

static const char * fxcode =
#include "effect.h"
;

using namespace GN;
using namespace GN::d3d10;
using namespace GN::util;

// *****************************************************************************
// application class
// *****************************************************************************

class MyApp : public D3D10Application
{
    RenderToTexture          rtt;
    ScreenAlignedQuad        quad;
    SimpleMesh               mesh;
    AutoComPtr<ID3D10Effect> effect;
    ArcBall                  arcball;
    Matrix44f                world, view, proj;

public:

    bool onCreate()
    {
        GN_GUARD;

        ID3D10Device & dev = device();

        const D3D10AppOption & option = getOption();

        RenderToTextureOption rtto = { option.width, option.height, DXGI_FORMAT_R8G8B8A8_UNORM, 1, MSAA_DISABLE, false };
        if( !rtt.init( &dev, rtto ) ) return false;

        ScreenAlignedQuadDesc saqd;
        saqd.makeDefault();
        if( !quad.init( &dev, saqd ) ) return false;

        // create mesh
        static SimpleMesh::Vertex vertices[24];
        static UInt16             indices[36];
        gfx::createBox(
            1.0f, 1.0f, 1.0f,
            vertices[0].pos, sizeof(SimpleMesh::Vertex),
            vertices[0].tex, sizeof(SimpleMesh::Vertex),
            vertices[0].normal, sizeof(SimpleMesh::Vertex),
            0, 0, // tangent
            0, 0, // binormal
            indices,
            0 );  // quads
        if( !mesh.init( &dev ) ) return false;
        mesh.setVertices( vertices, GN_ARRAY_COUNT(vertices) );
        mesh.setTriangles( indices, GN_ARRAY_COUNT(indices)/3 );

        // load effect
        AutoComPtr<ID3D10Blob> err;
        GN_DX_CHECK_RETURN(
            D3DX10CreateEffectFromMemory(
                fxcode, StringLength(fxcode),
                "effect.h",
                0, 0,
                "fx_4_0",
                D3D10_SHADER_PACK_MATRIX_ROW_MAJOR,
                0,
                &dev,
                0,
                0,
                &effect,
                &err,
                0 ),
            false );

        // initialize arcball
        world.Identity();
        view.Translate( 0, 0, 2 );
        proj.PerspectiveD3DLh( 1.0f, 4.0f/3.0f, 0.1f, 10.0f );
        arcball.setHandness( util::LEFT_HAND );
        arcball.setViewMatrix( view );
        arcball.connectToInput();
        arcball.setMouseMoveWindow( 0, 0, (int)option.width, (int)option.height );

        // success
        return true;

        GN_UNGUARD;
    }

    void onDestroy()
    {
        effect.clear();
        mesh.quit();
        quad.quit();
        rtt.quit();
    }

    void onUpdate()
    {
        world = arcball.getRotationMatrix44();
        Matrix44f pvw = proj * view * world;
        effect->GetVariableByName( "gPVW" )->AsMatrix()->SetMatrix( (float*)&pvw );
    }

    void onDraw()
    {
        rtt.clearScreen( 0, 1.0f, 1.0f, 0, 1.0f, 0 );
        rtt.bindWithDepth();

        ID3D10EffectTechnique * tech = effect->GetTechniqueByIndex( 0 );
        D3D10_TECHNIQUE_DESC techDesc;
        tech->GetDesc( &techDesc );
        for( UINT p = 0; p < techDesc.Passes; ++p )
        {
            tech->GetPassByIndex( p )->Apply( 0 );
            mesh.drawIndexed();
        }

        resetToDefaultRenderTargets();

        quad.drawTexed( rtt.getColorBuffer(0).srv );
    }
};

// *****************************************************************************
// main function
// *****************************************************************************

int main()
{
    D3D10AppOption option;
    MyApp          app;

    return app.run( option );
}
