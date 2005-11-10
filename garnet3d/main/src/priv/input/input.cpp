#include "pch.h"
#include "mswInput.h"
#include "diInput.h"
#include "x11Input.h"

//
//
// -----------------------------------------------------------------------------
GN::input::Input * GN::input::createInputSystem( bool useDirectInput )
{
    GN_GUARD;

#if GN_MSWIN
    if( useDirectInput )
    {
        AutoObjPtr<DIInput> p( new DIInput );
        if( !p->init() ) return 0;
        return p.detach();
    }
    else
    {
        AutoObjPtr<MswInput> p( new MswInput );
        if( !p->init() ) return 0;
        return p.detach();
    }
#else
    AutoObjPtr<X11Input> p( new X11Input );
    if( !p->init() ) return 0;
    return p.detach();
#endif

    GN_UNGUARD;
}
