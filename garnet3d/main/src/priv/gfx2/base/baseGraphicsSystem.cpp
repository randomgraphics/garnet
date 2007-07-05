#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseGraphicsSystem");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BaseGraphicsSystem::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::BaseGraphicsSystem, () );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseGraphicsSystem::quit()
{
    GN_GUARD;

    unregisterAllKernels();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseGraphicsSystem::registerKernel(
    const StrA & name, const KernelFactory & factory )
{
    if( !factory.creator )
    {
        GN_ERROR(sLogger)( "null kernel creator: %s.", name.cptr() );
        return;
    }

    UInt32 h = mKernels.name2handle( name );

    if( h )
    {
        GN_ERROR(sLogger)( "kernel named '%s' does already exist.", name.cptr() );
        return;
    }

    KernelItem ei;
    ei.factory  = factory;
    ei.instance = 0;

    mKernels.add( name, ei );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Kernel * GN::gfx::BaseGraphicsSystem::getKernel( const StrA & name )
{
    UInt32 h = mKernels.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "kernel named '%s' not found.", name.cptr() );
        return 0;
    }

    KernelItem & ei = mKernels[h];

    if( 0 == ei.instance )
    {
        ei.instance = ei.factory.creator( *this );
        if( 0 == ei.instance )
        {
            GN_ERROR(sLogger)( "fail to create kernel named '%s'.", name.cptr() );
        }
    }

    return ei.instance;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseGraphicsSystem::unregisterKernel( const StrA & name )
{
    UInt32 h = mKernels.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "kernel named '%s' not found.", name.cptr() );
        return;
    }

    KernelItem & ei = mKernels[h];

    safeDelete( ei.instance );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseGraphicsSystem::unregisterAllKernels()
{
    for ( UInt32 h = mKernels.first(); 0 != h; h = mKernels.next( h ) )
    {
        safeDelete( mKernels[h].instance );
    }
}
