#include "pch.h"
#include "garnet/GNbase.h"

#if GN_MSWIN
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
#if GN_CYGWIN
#define SHLIB_EXT                      ".dll"
#else
#define SHLIB_EXT                      ".so"
#endif
#else
#error "Unknown platform!"
#endif

static GN::Logger * sLogger = GN::getLogger("GN.base.SharedLib");

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
        GN_ERROR(sLogger)( "Library name can't be empty!" );
        return false;
    }

    // Handle libName
    StrA filename;
    if( path::getExt(libName).empty() )
        filename = path::toNative( StrA(libName) + SHLIB_EXT );
    else
        filename = path::toNative( libName );

    // load library
    mHandle = SHLIB_LOAD( filename.cptr() );
    if( 0 == mHandle )
    {
        GN_ERROR(sLogger)( "Fail to load library %s: %s!", filename.cptr(), SHLIB_ERROR() );
        return false;
    }

    GN_INFO(sLogger)( "Load library '%s'.", filename.cptr() );

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
        GN_INFO(sLogger)( "Unload library '%s'.", mFileName.cptr() );
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
        GN_ERROR(sLogger)( "Shared library is uninitialized!" );
        return 0;
    }

    if( strEmpty(symbol) )
    {
        GN_ERROR(sLogger)( "Symbol name can't be empty!" );
        return 0;
    }

    void * result = SHLIB_LOAD_SYMBOL( mHandle, symbol );
    if( 0 == result )
    {
        GN_ERROR(sLogger)( "Failed to get symbol named '%s': %s.", symbol, SHLIB_ERROR() );
    }

    return result;

    GN_UNGUARD;
}
