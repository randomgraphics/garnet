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

//
//
// -----------------------------------------------------------------------------
GN::gfx::Surface *
GN::gfx::BaseGraphicsSystem::createSurface( const SurfaceCreationParameter & scp )
{
    GN_GUARD;

    // determine surface type
    DynaArray<Guid> types;
    DynaArray<Guid> tmp;
    SurfaceCreationRule rule;
    SurfaceCreationRuleRegistry::const_iterator newtype;
    for( size_t i = 0; i < scp.bindings.size(); ++i )
    {
        const SurfaceCreationParameter::SurfaceBindingParameter & bp = scp.bindings[i];

        // get kernel
        const BaseKernel * kernel = safeCastPtr<const BaseKernel>( getKernel( bp.kernel ) );
        if( 0 == kernel ) return 0;

        // get port
        const BaseKernelPort * port = kernel->getPortT<BaseKernelPort>( bp.port );
        if( 0 == port ) return 0;

        // check layout compability
        if( !port->getRefl().layout.compatible( scp.desc.layout ) )
        {
            GN_ERROR(sLogger)( "Requested surface layout is incompatible with port '%s' of kernel '%s'", bp.port.cptr(), bp.kernel.cptr() );
            return false;
        }

        if( 0 == i )
        {
            types = port->getAllowedSurfaceTypes();
        }
        else
        {
            const DynaArray<Guid> & haha = port->getAllowedSurfaceTypes();

            for( size_t i1 = 0; i1 < types.size(); ++i1 )
            for( size_t i2 = 0; i2 < haha.size(); ++i2 )
            {
                rule.type1 = types[i1];
                rule.type2 = haha[i2];

                if( rule.type1 == rule.type2 )
                {
                    if( tmp.end() == std::find( tmp.begin(), tmp.end(), rule.type1 ) )
                    {
                        tmp.append( rule.type1 );
                    }
                }
                else
                {
                    newtype = mSurfaceRules.find( rule );
                    if( mSurfaceRules.end() != newtype &&
                        tmp.end() == std::find( tmp.begin(), tmp.end(), newtype->second ) )
                    {
                        tmp.append( newtype->second );
                    }
                }

                types.swap( tmp );
            }
        }

        if( types.empty() )
        {
            GN_ERROR(sLogger)( "Port '%s' of kernel '%s' is incompatible with other ports.!", bp.port.cptr(), bp.kernel.cptr() );
            return 0;
        }
    }

    GN_ASSERT( !types.empty() );

    // choose first type in type array
    const Guid & surfaceType = types[0];

    if( mSurfaceFactories.end() == mSurfaceFactories.find( surfaceType ) )
    {
        GN_ERROR(sLogger)( "invalid Surface type!" );
        return 0;
    }

    GN_ASSERT( mSurfaceFactories[surfaceType] );

    return mSurfaceFactories[surfaceType]( *this, scp.desc, scp.hints );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseGraphicsSystem::registerSurfeceType( const Guid & type, SurfaceFactory factory, const StrA & name )
{
    GN_ASSERT( factory );

    if( mSurfaceFactories.end() != mSurfaceFactories.find( type ) )
    {
        GN_ERROR(sLogger)( "surface type '%s' does exist already.", name.cptr() );
        return;
    }

    mSurfaceFactories[type] = factory;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseGraphicsSystem::registerSurfaceCreationRule( const Guid & type1, const Guid & type2, const Guid & result )
{
    SurfaceCreationRule rule;
    rule.type1 = type1;
    rule.type2 = type2;

    if( mSurfaceRules.end() != mSurfaceRules.find( rule ) )
    {
        GN_ERROR(sLogger)( "ignore redundent rule." );
        return;
    }

    mSurfaceRules[rule] = result;
}
