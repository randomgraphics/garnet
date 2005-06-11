#include "pch.h"
#include "garnet/GnBase.h"

// ****************************************************************************
//                   implementation of StdFile
// ****************************************************************************

//
size_t GN::StdFile::read( void * buffer, size_t size )
{
    GN_GUARD;
    size_t r = ::fread( buffer, 1, size, mFile );
    if ( (size_t)-1 == r ) GN_ERROR( "fread() failed!" );
    return r;
    GN_UNGUARD;
}
//
size_t GN::StdFile::write( const void * buffer, size_t size )
{
    GN_GUARD;
    size_t r = ::fwrite( buffer, 1, size, mFile );
    if ( (size_t)-1 == r ) GN_ERROR( "fwrite() failed!" );
    return r;
    GN_UNGUARD;
}

