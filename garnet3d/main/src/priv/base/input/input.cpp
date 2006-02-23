#include "pch.h"
#include "inputMsw.h"
#include "inputDInput.h"
#include "inputX11.h"

// *****************************************************************************
// Fake input system
// *****************************************************************************

#if GN_MSVC
#pragma warning(disable:4100) // 未引用的形参
#endif

namespace GN { namespace input
{
    class FakeInput : public Input
    {

    public:
        virtual bool attachToWindow( HandleType displayHandle, HandleType windowHandle ) { return true; }
        virtual void processInputEvents() {}
        virtual const KeyStatus * getKeyboardStatus() const { static KeyStatus ks[256]; return ks; }
        virtual void getMousePosition( int & x, int & y ) const { x = 0; y = 0; }
    };
}}

// *****************************************************************************
// local functions
// *****************************************************************************

static GN::input::Input * sCreateXInputSystem()
{
    GN_UNIMPL_WARNING();
    return new GN::input::FakeInput;
}

static GN::input::Input * sCreateNativeInputSystem()
{
#if GN_XENON
    return sCreateXInputSystem();
#elif GN_MSWIN
    GN::AutoObjPtr<GN::input::InputMsw> p( new GN::input::InputMsw );
    if( !p->init() ) return 0;
    return p.detach();
#else
    GN::AutoObjPtr<GN::input::InputX11> p( new GN::input::InputX11 );
    if( !p->init() ) return 0;
    return p.detach();
#endif
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::input::Input * GN::input::createInputSystem( InputApi api )
{
    GN_GUARD;

    // release old input system
    if( gInputPtr ) delete gInputPtr;

    // then create new one
    switch( api )
    {
        case API_NATIVE : return sCreateNativeInputSystem();

#if GN_MSWIN && !GN_XENON
        case API_DINPUT :
        {
            AutoObjPtr<InputDInput> p( new InputDInput );
            if( !p->init() ) return 0;
            return p.detach();
        }
#endif

        case API_XINPUT : return sCreateXInputSystem();

        case API_FAKE :
        {
            AutoObjPtr<FakeInput> p( new FakeInput );
            return p.detach();
        }

        default :
            GN_ERROR( "unknow or unsupport API : %d", api );
            return 0;
    }

    GN_UNGUARD;
}
