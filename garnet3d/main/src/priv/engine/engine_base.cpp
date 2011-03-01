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
    // remove all components.
    for( ComponentMap::KeyValuePair * i = mComponents.first(); i != NULL; i = mComponents.next( i ) )
    {
        i->value->decref();
    }
    mComponents.clear();

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
        comp->incref();
    }
    else
    {
        GN_ERROR(sLogger)( "NULL component pointer!" );
    }
}

// *****************************************************************************
// StandardUniformManager
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::StandardUniformManager::StandardUniformManager()
{
    gfx::GpuResourceDatabase * gdb = getGdb();
    GN_ASSERT( gdb );

    // initialize standard global uniforms
    for( StandardUniformType type = 0; (size_t)type < GN_ARRAY_COUNT(mUniforms); ++type )
    {
        AutoRef<gfx::UniformResource> & ur = mUniforms[type];

        if( type.getDesc().global )
        {
            ur = gdb->findOrCreateResource<gfx::UniformResource>( type.getDesc().name );
            AutoRef<gfx::Uniform> u( gdb->getGpu().createUniform( type.getDesc().size ) );
            ur->setUniform( u );
        }
    }

    // setup default transformation
    //setTransform( Matrix44f::IDENTITY, Matrix44f::IDENTITY );

    // setup default lighting
    Vector4f diffuse(1,1,1,1);
    Vector4f ambient(0.2f,0.2f,0.2f,1.0f);
    Vector4f specular(1,1,1,1);
    Vector3f position(0,0,0);
    Vector3f direction(0,0,1);
    setLight0(diffuse, ambient, specular, position, direction );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::StandardUniformManager::~StandardUniformManager()
{
    for( StandardUniformType type = 0; (size_t)type < GN_ARRAY_COUNT(mUniforms); ++type )
    {
        mUniforms[type].clear();
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::UniformResource *
GN::engine::StandardUniformManager::getGlobalUniformResource( StandardUniformType type ) const
{
    if( !type.isValid() )
    {
        GN_ERROR(sLogger)( "Invalid uniform type: %d", (StandardUniformType::ENUM)type );
        return NULL;
    }
    const StandardUniformDesc & desc = type.getDesc();
    if( !desc.global )
    {
        GN_ERROR(sLogger)( "Non-global parameter \"%s\" is not accessible through this function.", desc.name );
        return NULL;
    }

    return mUniforms[type];
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::StandardUniformManager::setGlobalUniform( StandardUniformType type, const void * data, size_t dataSize )
{
    // check parameters
    if( !type.isValid() )
    {
        GN_ERROR(sLogger)( "Invalid uniform type: %d", (StandardUniformType::ENUM)type );
        return;
    }
    const StandardUniformDesc & desc = type.getDesc();
    if( !desc.global )
    {
        GN_ERROR(sLogger)( "Non-global parameter \"%s\" is not accessible through this function.", desc.name );
        return;
    }
    if( NULL == data )
    {
        GN_ERROR(sLogger)( "Null point." );
        return;
    }
    if( dataSize != desc.size )
    {
        GN_ERROR(sLogger)( "Incorrect uniform data size: expected=%d, actual=%d.", desc.size, dataSize );
        return;
    }

    mUniforms[type]->uniform()->update( 0, dataSize, data );
}


//
//
// -----------------------------------------------------------------------------
void GN::engine::StandardUniformManager::setTransform( const Matrix44f & proj, const Matrix44f & view )
{
    Matrix44f pv   = proj * view;
    Matrix44f ipv  = Matrix44f::sInverse( pv );
    Matrix44f itpv = Matrix44f::sInverse( Matrix44f::sTranspose( pv ) );

    Matrix44f ip  = Matrix44f::sInverse( proj );
    Matrix44f itp = Matrix44f::sInverse( Matrix44f::sTranspose( proj ) );

    Matrix44f iv  = Matrix44f::sInverse( view );
    Matrix44f itv = Matrix44f::sInverse( Matrix44f::sTranspose( view ) );

    mUniforms[StandardUniformType::MATRIX_PV]->uniform()->update( pv );
    mUniforms[StandardUniformType::MATRIX_PV_INV]->uniform()->update( ipv );
    mUniforms[StandardUniformType::MATRIX_PV_IT]->uniform()->update( itpv );

    mUniforms[StandardUniformType::MATRIX_PROJ]->uniform()->update( proj );
    mUniforms[StandardUniformType::MATRIX_PROJ_INV]->uniform()->update( ip );
    mUniforms[StandardUniformType::MATRIX_PROJ_IT]->uniform()->update( itp );

    mUniforms[StandardUniformType::MATRIX_VIEW]->uniform()->update( view );
    mUniforms[StandardUniformType::MATRIX_VIEW_INV]->uniform()->update( iv );
    mUniforms[StandardUniformType::MATRIX_VIEW_IT]->uniform()->update( itv );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::StandardUniformManager::setLight0(
    const Vector4f & diffuse,
    const Vector4f & ambient,
    const Vector4f & specular,
    const Vector3f & position,
    const Vector3f & direction )
{
    mUniforms[StandardUniformType::LIGHT0_DIFFUSE]->uniform()->update( diffuse );
    mUniforms[StandardUniformType::LIGHT0_AMBIENT]->uniform()->update( ambient );
    mUniforms[StandardUniformType::LIGHT0_SPECULAR]->uniform()->update( specular );
    mUniforms[StandardUniformType::LIGHT0_POSITION]->uniform()->update( position );
    mUniforms[StandardUniformType::LIGHT0_DIRECTION]->uniform()->update( direction );
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
    StandardUniformManager   * standardUniforms;
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
static bool sGfxInitInternal( const GfxInitOptions & o )
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

    // create standard uniform managers
    s_engine.standardUniforms = new StandardUniformManager();

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
    s_engine.spriteRenderer = NULL;
    s_engine.lineRenderer = NULL;
    s_engine.fontRenderer = NULL;
    s_engine.gdb = NULL;
    s_engine.standardUniforms = NULL;
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
bool GN::engine::gfxInitialize( const GfxInitOptions & o )
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
    safeDelete( s_engine.standardUniforms );
    safeDelete( s_engine.gdb );
    safeDelete( s_engine.fontRenderer );
    safeDelete( s_engine.lineRenderer );
    safeDelete( s_engine.spriteRenderer );
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
GN::gfx::GpuResourceDatabase * GN::engine::getGdb()
{
    return s_engine.gdb;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::StandardUniformManager * GN::engine::getStandardUniformManager()
{
    return s_engine.standardUniforms;
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
