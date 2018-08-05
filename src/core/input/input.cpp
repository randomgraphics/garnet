#include "pch.h"
#include "inputMsw.h"
#include "inputDInput.h"
#include "inputX11.h"

static GN::Logger * sLogger = GN::getLogger("GN.input");

// *****************************************************************************
// Fake input system
// *****************************************************************************

#if GN_MSVC
#pragma warning(disable:4100) // δ���õ��β�
#endif

namespace GN { namespace input
{
    class FakeInput : public Input
    {

    public:
        virtual bool attachToWindow( intptr_t displayHandle, intptr_t windowHandle ) { return true; }
        virtual void processInputEvents() {}
        virtual KeyEvent popLastKeyEvent() { KeyEvent k; k.u16 = 0; return k; }
        virtual const KeyStatus * getKeyboardStatus() const { static KeyStatus ks[256]; return ks; }
        virtual const int * getAxisStatus() const { static int as[Axis::NUM_AXISES]; return as; }
        virtual void getMousePosition( int & x, int & y ) const { x = 0; y = 0; }
    };
}}

// *****************************************************************************
// local functions
// *****************************************************************************

static GN::input::Input * sCreateNativeInputSystem()
{
#if GN_XBOX2
    GN::AutoObjPtr<GN::input::InputXenon> p( new GN::input::InputXenon );
    return p.detach();
#elif GN_WINPC
    GN::AutoObjPtr<GN::input::InputMsw> p( new GN::input::InputMsw );
    if( !p->init() ) return 0;
    return p.detach();
#elif defined(HAS_X11)
    GN::AutoObjPtr<GN::input::InputX11> p( new GN::input::InputX11 );
    if( !p->init() ) return 0;
    return p.detach();
#else
    GN_ERROR(sLogger)("No input system available.");
    return nullptr;
#endif
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::input::initializeInputSystem( InputAPI api )
{
    GN_GUARD;

    // release old input system
    if( gInputPtr ) delete gInputPtr;

    // then create new one
    switch( api )
    {
        case InputAPI::NATIVE : return NULL != sCreateNativeInputSystem();

#ifdef HAS_DINPUT
        case InputAPI::DINPUT :
        {
            InputDInput * p = new InputDInput;
            if( !p->init() ) return 0;
            return true;
        }
#endif

        case InputAPI::FAKE :
        {
            new FakeInput;
            return true;
        }

        default :
            GN_ERROR(sLogger)( "unknow or unsupport API : %d", api.toRawEnum() );
            return 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::input::shutdownInputSystem()
{
    GN_GUARD;

    if( gInputPtr ) delete gInputPtr;

    GN_UNGUARD;
}
