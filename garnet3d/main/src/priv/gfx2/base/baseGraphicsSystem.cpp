#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.BaseGraphicsSystem");

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

    deleteAllKernels();

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
void GN::gfx::BaseGraphicsSystem::registerKernelFactory(
    const StrA & name, KernelFactory factory, int quality )
{
    if( !factory )
    {
        GN_ERROR(sLogger)( "null factory for kernel %s.", name.cptr() );
        return;
    }
    
    if( !checkKernelName( name ) ) return;

    UInt32 h = mKernels.name2handle( name );

    if( h )
    {
        KernelItem & ei = mKernels[h];
        if( ei.quality < quality )
        {
            ei.factory = factory;
            ei.quality = quality;
        }
        else
        {
            GN_TRACE(sLogger)( "Factory for kernel %s is ignored : not pass quality bar.", name.cptr() );
        }
    }
    else
    {
        KernelItem ei;
        ei.factory  = factory;
        ei.quality  = quality;
        ei.instance = 0;
        mKernels.add( name, ei );
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Kernel * GN::gfx::BaseGraphicsSystem::getKernel( const StrA & name )
{
    GN_GUARD;

    if( !checkKernelName( name ) ) return 0;

    UInt32 h = mKernels.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "Factory for kernel '%s' is not found.", name.cptr() );
        return 0;
    }

    KernelItem & ei = mKernels[h];

    if( 0 == ei.instance )
    {
        ei.instance = ei.factory( *this );
        if( 0 == ei.instance )
        {
            GN_ERROR(sLogger)( "fail to create kernel named '%s'.", name.cptr() );
        }
    }

    return ei.instance;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseGraphicsSystem::deleteAllKernels()
{
    for ( UInt32 h = mKernels.first(); 0 != h; h = mKernels.next( h ) )
    {
        safeDelete( mKernels[h].instance );
    }
}
