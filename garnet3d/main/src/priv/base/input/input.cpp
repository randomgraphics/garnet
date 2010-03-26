#include "pch.h"
#include "inputXenon.h"
#include "inputMsw.h"
#include "inputDInput.h"
#include "inputX11.h"

static GN::Logger * sLogger = GN::GetLogger("GN.input");

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
        virtual bool AttachToWindow( HandleType displayHandle, HandleType windowHandle ) { return true; }
        virtual void ProcessInputEvents() {}
        virtual KeyEvent PopLastKeyEvent() { KeyEvent k; k.u16 = 0; return k; }
        virtual const KeyStatus * GetKeyboardStatus() const { static KeyStatus ks[256]; return ks; }
        virtual const int * GetAxisStatus() const { static int as[Axis::NUM_AXISES]; return as; }
        virtual void GetMousePosition( int & x, int & y ) const { x = 0; y = 0; }
    };
}}

// *****************************************************************************
// local functions
// *****************************************************************************

static GN::input::Input * sCreateNativeInputSystem()
{
#if GN_XENON
    GN::AutoObjPtr<GN::input::InputXenon> p( new GN::input::InputXenon );
    return p.Detach();
#elif GN_MSWIN
    GN::AutoObjPtr<GN::input::InputMsw> p( new GN::input::InputMsw );
    if( !p->Init() ) return 0;
    return p.Detach();
#else
    GN::AutoObjPtr<GN::input::InputX11> p( new GN::input::InputX11 );
    if( !p->Init() ) return 0;
    return p.Detach();
#endif
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::InitializeInputSystem( InputAPI api )
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
            if( !p->Init() ) return 0;
            return true;
        }
#endif

        case InputAPI::FAKE :
        {
            new FakeInput;
            return true;
        }

        default :
            GN_ERROR(sLogger)( "unknow or unsupport API : %d", api.ToRawEnum() );
            return 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::ShutdownInputSystem()
{
    GN_GUARD;

    if( gInputPtr ) delete gInputPtr;

    GN_UNGUARD;
}
