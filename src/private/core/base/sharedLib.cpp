#include "pch.h"
#include "garnet/GNbase.h"

#if GN_MSWIN
#define SHLIB_LOAD( libName )          ((void*)LoadLibraryA(libName))
#define SHLIB_UNLOAD( lib )            (!!FreeLibrary(HMODULE(lib)))
#define SHLIB_LOAD_SYMBOL( lib, symb ) ((void*)GetProcAddress(HMODULE(lib), symb))
#define SHLIB_ERROR()                  ::GN::GetWin32LastErrorInfo()
#define SHLIB_EXT                      ".dll"
#elif GN_POSIX
#include <dlfcn.h>
#include <stdio.h>
#define SHLIB_LOAD( libName )          ((void*)dlopen(libName, RTLD_NOW))
#define SHLIB_UNLOAD( lib )            (!dlclose( lib ))
#define SHLIB_LOAD_SYMBOL( lib, symb ) ((void*)dlsym(lib, symb))
#define SHLIB_ERROR()                  dlerror()
#if GN_CYGWIN
#define SHLIB_EXT                      ".dll"
#else
#define SHLIB_EXT                      ".so"
#endif
#else
#error "Unknown platform!"
#endif

static GN::Logger * sLogger = GN::GetLogger("GN.base.SharedLib");

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::SharedLib::load( const char * libName )
{
    GN_GUARD;

    // unload old library
    unload();

    // check parameter
    if( str::isEmpty(libName) )
    {
        GN_ERROR(sLogger)( "Library name can't be empty!" );
        return false;
    }

    // Handle libName
    StrA filename;
    if( fs::extName(libName).empty() )
        filename = StrA(libName) + SHLIB_EXT;
    else
        filename = libName;

    // load library
    mHandle = SHLIB_LOAD( filename.rawptr() );
    if( 0 == mHandle )
    {
        GN_ERROR(sLogger)( "Fail to load library %s: %s!", filename.rawptr(), SHLIB_ERROR() );
        return false;
    }

    GN_TRACE(sLogger)( "Load library '%s'.", filename.rawptr() );

    // success
    mFileName = filename;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::SharedLib::unload()
{
    GN_GUARD;

    if( mHandle )
    {
        SHLIB_UNLOAD( mHandle );
        mHandle = 0;
        GN_TRACE(sLogger)( "Unload library '%s'.", mFileName.rawptr() );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API void * GN::SharedLib::getSymbol( const char * symbol )
{
    GN_GUARD;

    if( 0 == mHandle )
    {
        GN_ERROR(sLogger)( "Shared library is uninitialized!" );
        return 0;
    }

    if( str::isEmpty(symbol) )
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
