#include "pch.h"
#include "garnet/GNbase.h"

#if GN_WINNT
#define SHLIB_LOAD( libName )          static_cast<void*>(LoadLibraryA(libName))
#define SHLIB_FREE( lib )              (!!FreeLibrary(HMODULE(lib)))
#define SHLIB_LOAD_SYMBOL( lib, symb ) static_cast<void*>(GetProcAddress(HMODULE(lib), symb))
#define SHLIB_ERROR()                  ::GN::getOSErrorInfo()
#define SHLIB_EXT                      ".dll"
#elif GN_POSIX
#include <dlfcn.h>
#include <stdio.h>
#define SHLIB_LOAD( libName )          static_cast<void*>(dlopen(libName, RTLD_NOW))
#define SHLIB_FREE( lib )              (!dlclose( lib ))
#define SHLIB_LOAD_SYMBOL( lib, symb ) static_cast<void*>(dlsym(lib, symb))
#define SHLIB_ERROR()                  dlerror()
#define SHLIB_EXT                      ".so"
#else
#error "Unknown platform!"
#endif

//
//
// -----------------------------------------------------------------------------
bool GN::SharedLib::load( const char * libName )
{
    GN_GUARD;

    // free old library
    free();

    // check parameter
    if( strEmpty(libName) )
    {
        GN_ERROR( "Library name can't be empty!" );
        return false;
    }

    // Handle libName
    StrA filename;
    if( path::getExt(libName).empty() )
        filename = path::toNative( StrA(libName) + SHLIB_EXT );
    else
        filename = path::toNative( libName );

    // load library
    mHandle = SHLIB_LOAD( filename.cstr() );
    if( 0 == mHandle )
    {
        GN_ERROR( "Fail to load library %s: %s!", filename.cstr(), SHLIB_ERROR() );
        return false;
    }

    GN_INFO( "Load library '%s'.", filename.cstr() );

    // success
    mFileName = filename;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::SharedLib::free()
{
    GN_GUARD;

    if( mHandle )
    {
        SHLIB_FREE( mHandle );
        mHandle = 0;
        GN_INFO( "Unload library '%s'.", mFileName.cstr() );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void * GN::SharedLib::getSymbol( const char * symbol )
{
    GN_GUARD;

    if( 0 == mHandle )
    {
        GN_ERROR( "Shared library is uninitialized!" );
        return 0;
    }

    if( strEmpty(symbol) )
    {
        GN_ERROR( "Symbol name can't be empty!" );
        return 0;
    }

    void * result = SHLIB_LOAD_SYMBOL( mHandle, symbol );
    if( 0 == result )
    {
        GN_ERROR( "Failed to get symbol named '%s': %s.", symbol, SHLIB_ERROR() );
    }

    return result;

    GN_UNGUARD;
}
