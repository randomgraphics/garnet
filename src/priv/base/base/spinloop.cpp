#include "pch.h"

void GN::SpinLoop::Lock()
{
    size_t i = 0;
    while( 0 != AtomCmpXchg32( &mLock, 1, 0 ) )
    {
        ++i;
        if( i > 1000000 )
        {
            SleepCurrentThread( 0.0f );
            i = 0;
        }
    }
}
