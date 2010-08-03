#include "pch.h"

void GN::SpinLoop::lock()
{
    size_t i = 0;
    while( 0 != atomCmpXchg32( &mLock, 1, 0 ) )
    {
        ++i;
        if( i > 1000000 )
        {
            sleepCurrentThread( 0 );
            i = 0;
        }
    }
}
