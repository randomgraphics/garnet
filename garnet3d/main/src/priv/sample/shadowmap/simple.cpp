#include "pch.h"
#include "simple.h"

using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.sample.ShadowMap");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::SimpleShadowMap::init( const StrA & actorName )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::SimpleShadowMap, () );

    RenderEngine & re = mApp.getRenderEngine();
    EntityManager & em = mApp.getEntityManager();

    // initialize shadow textures
    mColorMap  = re.create2DRenderTargetTexture( "colormap", 1024, 1024, 1, FMT_R_32_FLOAT );
    mShadowMap = re.create2DDepthTexture( "shadowmap", 1024, 1024 );
    if(0 == mColorMap || 0 == mShadowMap ) return failure();

    // load actor
    if( !loadActor( actorName ) ) return failure();

    float radius = mShadowProjectors->getBoundingSphere().radius;

    // intialize camera
    static bool first = true;
    if( first )
    {
        first = false;
        mCamera.setPosition( 0, 0, radius * 2.0f );
        mCamera.setMoveSpeed( radius * 0.5f );
    }

    // setup light
    mScene.light(0).position.set( radius * 1.5f, radius * 1.5f, radius * 1.5f );
    mLightView.lookAtRh( mScene.light(0).position, Vector3f(0,0,0), Vector3f(0,1,0) );
    re.composePerspectiveMatrixRh(
        mLightProj,
        1.0f,
        1.0f,
        mScene.light(0).position.length() - radius,
        mScene.light(0).position.length() + radius );

    // set light transform for shadow map look-up
    EffectItemID id = 0;
    Effect * e = entity2Object<Effect*>( loadEffectEntityFromXmlFile( em, re, "media::/effect/shadowmap.xml" ), 0 );
    if( e && e->hasUniform( "light0_pvw", &id ) )
    {
        float fOffsetX = 0.5f + (0.5f / 1024);
        float fOffsetY = 0.5f + (0.5f / 1024);
        float fZScale = 1.0f;
        float fBias = -0.01f;
        Matrix44f scaleBias(
            0.5f,  0.0f,    0.0f, fOffsetX,
            0.0f, -0.5f,    0.0f, fOffsetY,
            0.0f,  0.0f, fZScale, fBias,
            0.0f,  0.0f,    0.0f, 1.0f );
        Matrix44f lpvw = scaleBias * mLightProj * mLightView * mShadowProjectors->getLocal2Root();
        e->setUniform( id, lpvw );
    }

    mCtx.resetToDefault();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::SimpleShadowMap::quit()
{
    GN_GUARD;

    mColorMap = 0;
    mShadowMap = 0;

    releaseActorHiearacy( mShadowProjectors );
    releaseActorHiearacy( mShadowReceivers );
    mShadowProjectors = 0;
    mShadowReceivers = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::SimpleShadowMap::clear()
{
    mShadowProjectors = 0;
    mShadowReceivers = 0;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::SimpleShadowMap::update()
{
    // update camera
    mCamera.update( app::SampleApp::UPDATE_INTERVAL );

    RenderEngine & re = mApp.getRenderEngine();

    // update projection matrix
    const DispDesc & dd = re.getDispDesc();
    const Spheref & bs = mShadowProjectors->getBoundingSphere();
    float d = mCamera.getPosition().length();
    float n_min = bs.radius / 100.0f;
    float n = d - bs.radius;
    float f = d + bs.radius;
    if( n < n_min ) n = n_min;
    re.composePerspectiveMatrixRh( mProj, 0.6f, (float)dd.width/dd.height, n, f );
}

//
//
// -----------------------------------------------------------------------------
void GN::SimpleShadowMap::draw()
{
    RenderEngine & re = mApp.getRenderEngine();

    // draw to shadow map
    mScene.setProj( mLightProj );
    mScene.setView( mLightView );
    mCtx.setDrawToTextures( 1, mColorMap, 0, 0, 0, mShadowMap, MsaaType::NONE );
    re.setContext( mCtx );
    re.clearScreen();
    mShadowProjectors->draw();

    // draw to back buffer
    mScene.setProj( mProj );
    mScene.setView( mCamera.getViewMatrix() );
    mCtx.setDrawToBackBuf();
    re.setContext( mCtx );
    re.clearScreen();
    mShadowReceivers->draw();

    // draw axises
    static const float X[] = { 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f, 0.0f };
    static const float Y[] = { 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f };
    static const float Z[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f };
    const Matrix44f & view = mScene.getView();
    const Matrix44f & proj = mScene.getProj();
    re.drawLines( 0, X, 3*sizeof(float), 1, GN_RGBA32(255,0,0,255), Matrix44f::IDENTITY, view, proj );
    re.drawLines( 0, Y, 3*sizeof(float), 1, GN_RGBA32(0,255,0,255), Matrix44f::IDENTITY, view, proj );
    re.drawLines( 0, Z, 3*sizeof(float), 1, GN_RGBA32(0,0,255,255), Matrix44f::IDENTITY, view, proj );
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::SimpleShadowMap::loadActor( const StrA & name )
{
    GN_ASSERT( 0 == mShadowProjectors && 0 == mShadowReceivers );

    mShadowProjectors = mScene.loadActorHiearachyFromXmlFile( name, "root" );
    if( 0 == mShadowProjectors ) return false;

    // place actor to screen center
    Quaternionf r;
    r.fromRotation( Vector3f(1,0,0), -GN_HALF_PI );;
    mShadowProjectors->setRotation( r );
    mShadowProjectors->setPosition( -mShadowProjectors->getBoundingSphere().center );

    RenderEngine & re = mApp.getRenderEngine();
    EntityManager & em = mApp.getEntityManager();

    // load depth only effect
    Entity * effent = loadEffectEntityFromXmlFile( em, re, "media::/effect/depth_only.xml" );
    if( 0 == effent ) return false;
    Effect * eff = entity2Object<Effect*>( effent, 0 );
    if( 0 == eff ) return false;

    TreeTraversePreOrder<Actor> ttpre(mShadowProjectors);
    for( Actor * a = ttpre.first(); a; a = ttpre.next( a ) )
    {
        for( size_t i = 0; i < a->getNumDrawables(); ++i )
        {
            Drawable d = a->getDrawable( i );
            d.effect = effent;
            d.textures.clear();
            d.uniforms.clear();
            d.uniforms["pvw"].binding = eff->getUniformID( "pvw" );
            a->setDrawable( i, d );
        }
    }

    // create shadow receivers
    mShadowReceivers = cloneActorHiearacy( mShadowProjectors );

    // load shadowmap effect
    effent = loadEffectEntityFromXmlFile( em, re, "media::/effect/shadowmap.xml" );
    if( 0 == effent ) return false;
    eff = entity2Object<Effect*>( effent, 0 );
    if( 0 == eff ) return false;

    // prepare shadow receivers
    ttpre.reset( mShadowReceivers );
    for( Actor * a = ttpre.first(); a; a = ttpre.next( a ) )
    {
        for( size_t i = 0; i < a->getNumDrawables(); ++i )
        {
            Drawable d = a->getDrawable( i );
            d.effect = effent;
            d.textures.clear();
            d.textures["shadowmap"].texture = mShadowMap;
            d.textures["shadowmap"].binding = eff->getTextureID( "shadowmap" );

            d.uniforms.clear();

            a->setDrawable( i, d );
        }
    }

    // success
    return true;
}
