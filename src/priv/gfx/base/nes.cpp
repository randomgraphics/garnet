#include "pch.h"
#include "garnet/gfx/nes.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.EffectSystem");

// *****************************************************************************
// class GN::gfx::nes::EffectSystem::Impl
// *****************************************************************************

//!
//! Implementation class of effect system
//!
class GN::gfx::nes::EffectSystem::Impl
{
    struct Effect
    {
        EffectDesc desc;
        Shader *   shader;
    };

    struct ShaderDll
    {
        typedef size_t   (*FP_getNumShaders)();
        typedef Shader * (*FP_createShader)( size_t );

        SharedLib        dll;
        FP_getNumShaders getNumShaders;
        FP_createShader  createShader;

    public:

        ShaderDll() : getNumShaders(0), createShader(0) {}

        ~ShaderDll() { free(); }

        bool load( const StrA & dllName )
        {
            GN_GUARD;

            // load library
            if( !dll.load( path::toNative( dllName ).cptr() ) ) return false;

            // find symbol
            getNumShaders = (FP_getNumShaders)dll.getSymbol( "GNgetNumShaders" );
            createShader = (FP_createShader)dll.getSymbol( "GNcreateShader" );
            if( !getNumShaders || !createShader ) return false;

            // success
            return true;

            GN_UNGUARD;
        }

        void free()
        {
            getNumShaders = 0;
            createShader = 0;
            dll.free();
        }
    };

    struct SameDllPtr
    {
        const ShaderDll * ref;
        SameDllPtr( const ShaderDll * r ) : ref(r) {}
        bool operator()( const ShaderDll * p )
        {
            GN_ASSERT( ref && p );
            return ref == p || ref->dll.getHandle() == p->dll.getHandle();
        }
    };

    HandleManager<Stream*,StreamId> mStreams;

    HandleManager<Effect,EffectId>  mEffects;

    DynaArray<ShaderDll*> mDlls;
    DynaArray<Shader*>    mSharedShaders;
    DynaArray<Shader*>    mStaticShaders;

    bool mNeedRebuild;

    //!
    //! rebuild relationships between shaders and effects.
    //!
    void rebuild()
    {
        GN_UNIMPL();
    }

    // ********************************
    // ctor/dtor
    // ********************************

public:

    Impl() : mNeedRebuild(true) {}

    virtual ~Impl() { reset(); }

    void reset()
    {
        removeAllStreams();
        removeAllEffects();
        removeAllShaders();
    }

    // ********************************
    // stream manager
    // ********************************
public:

    StreamId registerStream( Stream * stream )
    {
        if( mStreams.find( stream ) )
        {
            GN_WARN(sLogger)( "You're registering the same stream more than once." );
        }
        return mStreams.add( stream );
    }

    Stream * removeStream( StreamId id )
    {
        Stream * s = getStream( id );
        if( s ) mStreams.remove( id );
        return s;
    }

    void removeAllStreams()
    {
        mStreams.clear();
    }

    Stream * getStream( StreamId id ) const
    {
        if( !mStreams.validHandle( id ) )
        {
            GN_ERROR(sLogger)( "Invalid stream ID: '%d'.", id );
            return NULL;
        }
        return mStreams.get( id );
    }

    // ********************************
    // effect manager
    // ********************************
public:

    EffectId registerEffect( const EffectDesc & desc )
    {
        GN_TODO( "make sure a valid effect descriptor before registering!" );
        EffectId eid = mEffects.newItem();
        if( 0 == eid ) return 0;

        Effect & e = mEffects.get( eid );
        e.desc = desc;
        e.shader = 0;

        mNeedRebuild = true;
        return eid;
    }

    void removeEffect( EffectId id )
    {
        mEffects.remove( id );
        mNeedRebuild = true;
    }

    void removeAllEffects()
    {
        mEffects.clear();
        mNeedRebuild = true;
    }

    const EffectDesc * getEffectDesc( EffectId id ) const
    {
        if( !mEffects.validHandle( id ) )
        {
            GN_ERROR(sLogger)( "Invalid effect ID: %d.", id );
            return 0;
        }
        return &mEffects[id].desc;
    }

    Shader * getEffectShader( EffectId id )
    {
        if( !mEffects.validHandle( id ) )
        {
            GN_ERROR(sLogger)( "Invalid effect ID: %d.", id );
            return 0;
        }
        if( mNeedRebuild ) rebuild();
        GN_ASSERT( !mNeedRebuild );
        return mEffects[id].shader;
    }

    // ********************************
    // shader manager
    // ********************************
public:

    //
    //
    // -------------------------------------------------------------------------
    bool registerShaderDll( const StrA & dllName )
    {
        GN_GUARD;
        
        // load library
        AutoObjPtr<ShaderDll> dll( new ShaderDll );
        if( !dll->load( path::toNative( dllName ) ) ) return false;

        // check redundant DLL
        if( mDlls.end() != std::find_if( mDlls.begin(), mDlls.end(), SameDllPtr(dll) ) )
            return true;

        // success
        mDlls.append( dll.detach() );
        return true;

        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    bool registerShader( Shader * s )
    {
        if( 0 == s )
        {
            GN_ERROR(sLogger)( "NULL shader pointer!" );
            return false;
        }
        if( mStaticShaders.end() != std::find( mStaticShaders.begin(), mStaticShaders.end(), s ) )
            return true; // ignore redundant shader
        mStaticShaders.append( s );
        return true;
    }

    //
    //
    // -------------------------------------------------------------------------
    void removeAllShaders()
    {
        std::for_each( mSharedShaders.begin(), mSharedShaders.end(), &safeDelete<Shader> );
        std::for_each( mDlls.begin(), mDlls.end(), &safeDelete<ShaderDll> );
        mStaticShaders.clear();
        mSharedShaders.clear();
        mDlls.clear();
    }
};

// *****************************************************************************
//
// *****************************************************************************

GN::gfx::nes::EffectSystem::EffectSystem() : mImpl( new Impl ) {}
GN::gfx::nes::EffectSystem::~EffectSystem() { delete mImpl; }

GN::gfx::nes::StreamId GN::gfx::nes::EffectSystem::registerStream( Stream * stream ) { return mImpl->registerStream( stream ); }
GN::gfx::nes::Stream * GN::gfx::nes::EffectSystem::removeStream( StreamId id ) { return mImpl->removeStream( id ); }
void              GN::gfx::nes::EffectSystem::removeAllStreams() { mImpl->removeAllStreams(); }
GN::gfx::nes::Stream * GN::gfx::nes::EffectSystem::getStream( StreamId id ) const { return mImpl->getStream( id ); }

GN::gfx::nes::EffectId           GN::gfx::nes::EffectSystem::registerEffect( const EffectDesc & desc ) { return mImpl->registerEffect( desc ); }
void                        GN::gfx::nes::EffectSystem::removeEffect( EffectId id ) { mImpl->removeEffect( id ); }
void                        GN::gfx::nes::EffectSystem::removeAllEffects() { mImpl->removeAllEffects(); }
const GN::gfx::nes::EffectDesc * GN::gfx::nes::EffectSystem::getEffectDesc( EffectId id ) const { return mImpl->getEffectDesc( id ); }
GN::gfx::nes::Shader *           GN::gfx::nes::EffectSystem::getEffectShader( EffectId id ) const { return mImpl->getEffectShader( id ); }

bool GN::gfx::nes::EffectSystem::registerShaderDll( const StrA & dllName ) { return mImpl->registerShaderDll( dllName ); }
bool GN::gfx::nes::EffectSystem::registerShader( Shader * s ) { return mImpl->registerShader( s ); }
void GN::gfx::nes::EffectSystem::removeAllShaders() { return mImpl->removeAllShaders(); }
