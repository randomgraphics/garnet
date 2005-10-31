#include "pch.h"
#include "mswInput.h"
#include "diInput.h"
//#include "xInput.h"

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
    GN_UNUSED_PARAM( useDirectInput );
    GN_UNIMPL();
    return 0;
#endif

    GN_UNGUARD;
}
