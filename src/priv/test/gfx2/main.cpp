#include "pch.h"
using namespace GN;
using namespace GN::gfx2;
using namespace GN::input;

struct InputInitiator
{
    InputInitiator( GraphicsSystem * gs )
    {
        Input * i = createInputSystem();

        const GraphicsSystemDesc & desc = gs->getDesc();

        i->attachToWindow( desc.display, desc.window );
    }

    ~InputInitiator()
    {
        delete gInputPtr;
    }
};

int run( GraphicsSystem & gs )
{
    Input & i = gInput;

    KeyEvent key;

    Effect * clearEffect = gs.getEffect( "clear" );
    if( 0 == clearEffect ) return -1;

    AutoObjPtr<EffectParameterSet> clearParam( clearEffect->createParameterSet() );
    if( 0 == clearParam ) return -1;

    clearParam->setParameter( "COLOR_VALUE", EffectParameter( 0.0f, 0.0f, 1.0f, 1.0f ) );
    clearParam->setParameter( "DEPTH_VALUE", 1.0f );
    clearParam->setParameter( "STENCIL_VALUE", 0 );

    while( true )
    {
        i.processInputEvents();
        key = i.popLastKeyEvent();
        if( key.status.down && KEY_ESCAPE == key.code ) break;

        clearEffect->render( *clearParam , 0 );

        gs.present();
    }

    return 0;
}

int main()
{
    GraphicsSystemCreationParameter gscp =
    {
        FOURCC::sMake( 'D','X','9',0 ),
        0,               // first monitor
        640, 480, 32, 0, // fullscreen properties
        800, 600,        // windowed properties
        false,           // windowed mode
        false,           // no vsync
    };

    GraphicsSystemCreator gs;

    if( !gs.create( gscp ) ) return -1;

    InputInitiator input( gs.get() );

    return run( *gs.get() );
}
