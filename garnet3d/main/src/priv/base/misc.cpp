#include "pch.h"

//
//
// -----------------------------------------------------------------------------
bool GN::getEnv( StrA & result, const char * name )
{
    const char * var;
    if( strEmpty(name) || NULL == (var=::getenv(name)) )
    {
        result.clear();
        return false;
    }
    else
    {
        result.assign( var );
        return true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::sleep( uint32_t microSeconds )
{
#if GN_WINNT
    ::Sleep( microSeconds );
#elif GN_POSIX
    ::usleep( microSeconds );
#else
    GN_CASSERT_EX( 0, "Unimplmented" );
#endif
}

//
//
// -----------------------------------------------------------------------------
void GN::processWindowMessages( bool * quit )
{
#if GN_WINNT

    GN_GUARD;

    if( *quit ) *quit = false;

    MSG msg;
    while( true )
    {
        if( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            if( WM_QUIT == msg.message )
            {
                if( quit ) *quit = true;
                return;
            }
            ::TranslateMessage( &msg );
            ::DispatchMessage(&msg);
        }
        else if( ::IsIconic(msg.hwnd) ) // minimized
        {
            ::WaitMessage();
        }
        else return; // Idle time!
    }

    GN_UNGUARD;

#endif
}
