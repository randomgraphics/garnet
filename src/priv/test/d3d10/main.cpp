#include "pch.h"

static const char * fxcode =
#include "effect.h"
;

using namespace GN;
using namespace GN::d3d10;

// *****************************************************************************
// application class
// *****************************************************************************

class MyApp : public D3D10Application
{
    SimpleMesh               mesh;
    AutoComPtr<ID3D10Effect> effect;

public:

    bool onCreate()
    {
        ID3D10Device & dev = device();

        // create mesh
        if( !mesh.init( &dev ) ) return false;

        // load effect
        AutoComPtr<ID3D10Blob> err;
        GN_DX10_CHECK_RV(
            D3DX10CreateEffectFromMemory(
                fxcode, strLen(fxcode),
                "effect.h",
                0, 0,
                "fx_4_0",
                0,
                0,
                &dev,
                0,
                0,
                &effect,
                &err,
                0 ),
            false );

        return true;
    }

    void onDestroy()
    {
        mesh.quit();
    }

    void onUpdate()
    {
    }

    void onDraw()
    {
        // clear screen
        clearScreen( 0, 1.0f, 1.0f, 0, 1.0f, 0 );

        /*ID3D10EffectTechnique * tech = effect->GetTechniqueByIndex( 0 );
        D3D10_TECHNIQUE_DESC techDesc;
        tech->GetDesc( &techDesc );
        for( UINT p = 0; p < techDesc.Passes; ++p )
        {
            tech->GetPassByIndex( p )->Apply( 0 );
            mesh.drawIndexed();
        }*/
    }
};

// *****************************************************************************
// main function
// *****************************************************************************

int main()
{
    D3D10AppOption option;

    option.msaa = 1;

    MyApp          app;

    return app.run( option );
}
