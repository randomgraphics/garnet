#include "pch.h"

using namespace GN;
using namespace GN::engine;

GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Component
// *****************************************************************************


// *****************************************************************************
// Entity
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity::Entity()
{
    mID = impl::onEntityCtor( this );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity::~Entity()
{
    impl::onEntityDtor( mID );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Component * GN::engine::Entity::getComponent( const Guid & type ) const
{
    Component ** pp = mComponents.find( type );
    if( pp )
    {
        GN_ASSERT( (*pp) );
        return *pp;
    }
    else
    {
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::Entity::setComponent( const Guid & type, Component * comp )
{
    if( NULL != comp && NULL != comp->mEntity )
    {
        GN_ERROR(sLogger)( "The component has attached to another entity." );
        return;
    }

    Component ** pp = mComponents.find( type );

    if( NULL != pp )
    {
        Component * old = *pp;
        GN_ASSERT( old );

        if( old != comp )
        {
            if( NULL != comp )
            {
                // replace existing component

                comp->mEntity = this;
                comp->incref();
                *pp = comp;

                old->mEntity = NULL;
                old->decref();
            }
            else
            {
                // remove existing component
                old->mEntity = NULL;
                old->decref();
                mComponents.remove( type );
            }
        }
    }
    else if( NULL != comp )
    {
        // add new component
        mComponents[type] = comp;
        comp->mEntity = this;
    }
    else
    {
        GN_ERROR(sLogger)( "NULL component pointer!" );
    }
}

// *****************************************************************************
// Global Engine functions
// *****************************************************************************

struct Engine
{
    // Entity manager
    //@{
    typedef HandleManager<engine::Entity*,int> EntityHandleManager;
    Mutex               entityLock;
    EntityHandleManager entities;
    //@}

    // Graphics manager
    //@{
    gfx::Gpu                 * gpu;
    gfx::GpuResourceDatabase * gdb;
    gfx::SpriteRenderer      * spriteRenderer;
    gfx::LineRenderer        * lineRenderer;
    gfx::BitmapFont          * fontRenderer;
    //@}

    /// Default constructor
    Engine();
};

static Engine s_engine;

// *****************************************************************************
// Private Engine Functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool sGfxInitInternal( const GfxOptions & o )
{
    using namespace GN::gfx;

    // initialize GPU
    s_engine.gpu = createGpu( o.gpuOptions, o.useMultithreadGpu ? GPU_CREATION_MULTIPLE_THREADS : 0 );
    if( NULL == s_engine.gpu ) return false;

    GN_TODO( "connect to renderer signal: post quit event, if render window is closed." );
    //s_engine.gpu->getSignals().rendererWindowClose.connect( this, &SampleApp::postExitEvent );
    //s_engine.gpu->getSignals().rendererWindowSizeMove.connect( this, &SampleApp::onRenderWindowResize );

    // create sprite renderer
    s_engine.spriteRenderer = new SpriteRenderer( *s_engine.gpu );
    if( !s_engine.spriteRenderer->init() ) return false;

    // create line renderer
    s_engine.lineRenderer = new LineRenderer( *s_engine.gpu );
    if( !s_engine.lineRenderer->init() ) return false;

    // try load default font face in mInitParam first
    MixedFontCreationDesc mfcd;
    mfcd.font = o.defaultAsciiFont;
    mfcd.firstChar = 0;
    mfcd.numChars = 127;
    AutoRef<FontFace> ff( createMixedFontFace(o.defaultNonAsciiFont, &mfcd, 1) );
    if( !ff )
    {
        // if failed, fallback to simple ASCII font face
        ff.attach( createSimpleAsciiFontFace() );
        if( !ff ) return false;
    }
    s_engine.fontRenderer = new BitmapFont();
    if( !s_engine.fontRenderer->init( s_engine.spriteRenderer, ff ) ) return false;

    // create GPU resource database
    s_engine.gdb = new GpuResourceDatabase( *s_engine.gpu );

    // attach to input subsystem
    if( gInputPtr )
    {
        const DispDesc & dd = s_engine.gpu->getDispDesc();
        if( !gInputPtr->attachToWindow( dd.displayHandle, dd.windowHandle ) ) return false;
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
static void sGfxClearInternal()
{
    s_engine.gpu = NULL;
    s_engine.gdb = NULL;
    s_engine.spriteRenderer = NULL;
    s_engine.lineRenderer = NULL;
    s_engine.fontRenderer = NULL;
}

//
//
// -----------------------------------------------------------------------------
static void sEngineClearInternal()
{
    sGfxClearInternal();
}

//
//
// -----------------------------------------------------------------------------
Engine::Engine()
{
    sEngineClearInternal();
}

// *****************************************************************************
// Public Engine Functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::initialize()
{
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::shutdown()
{
    // shutdown subsystems.
    gfxShutdown();
    inputShutdown();

    // clear engine structure
    sEngineClearInternal();
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::inputInitialize( input::InputAPI api )
{
    using namespace GN::input;

    if( !initializeInputSystem( api ) )
    {
        inputShutdown();
        return false;
    }

    // attach to graphics subsystem
    if( s_engine.gpu )
    {
        const gfx::DispDesc & dd = s_engine.gpu->getDispDesc();
        if( !gInputPtr->attachToWindow( dd.displayHandle, dd.windowHandle ) )
        {
            inputShutdown();
            return false;
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::inputShutdown()
{
    GN::input::shutdownInputSystem();
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::gfxInitialize( const GfxOptions & o )
{
    bool result = sGfxInitInternal( o );
    if( !result ) gfxShutdown();
    return result;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::gfxShutdown()
{
    safeDelete( s_engine.gdb );
    safeDelete( s_engine.fontRenderer );
    safeDelete( s_engine.lineRenderer );
    safeDelete( s_engine.spriteRenderer );
    //safeDelete( s_engine.gpu );
    gfx::deleteGpu( s_engine.gpu ); s_engine.gpu = NULL;

    sGfxClearInternal();
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Gpu * GN::engine::getGpu()
{
    return s_engine.gpu;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResourceDatabase * GN::engine::getGdb()
{
    return s_engine.gdb;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::SpriteRenderer * GN::engine::getSpriteRenderer()
{
    return s_engine.spriteRenderer;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::LineRenderer * GN::engine::getLineRenderer()
{
    return s_engine.lineRenderer;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::BitmapFont * GN::engine::getDefaultFontRenderer()
{
    return s_engine.fontRenderer;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::findEntity( int id )
{
    ScopeMutex<> lock( s_engine.entityLock );

    if( s_engine.entities.validHandle( id ) )
    {
        return s_engine.entities[id];
    }
    else
    {
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
int GN::engine::impl::onEntityCtor( Entity * e )
{
    GN_ASSERT( e );

    ScopeMutex<> lock( s_engine.entityLock );

    return s_engine.entities.add( e );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::impl::onEntityDtor( int id )
{
    ScopeMutex<> lock( s_engine.entityLock );

    GN_VERIFY( s_engine.entities.remove( id ) );
}
