#include "pch.h"
#include "simple.h"

using namespace GN::gfx;
using namespace GN::scene;

GN::Logger * sLogger = GN::getLogger("GN.sample.ShadowMap");

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

    Renderer & r = gRenderer;

    // initialize shadow textures
    mColorMap  = gSceneResMgr.getResourceId( "media::/texture/R32F_1Kx1K_2D.texture.xml" ); 
    mShadowMap = gSceneResMgr.getResourceId( "media::/texture/depth_texture_1Kx1K.texture.xml" );
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
    r.composePerspectiveMatrixRh(
        mLightProj,
        1.0f,
        1.0f,
        mScene.light(0).position.length() - radius,
        mScene.light(0).position.length() + radius );

    // set light transform for shadow map look-up
    EffectItemID id = 0;
    Effect * e = gSceneResMgr.getResourceT<Effect>( "media::/effect/shadowmap.xml" );
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

    // update projection matrix
    const DispDesc & dd = gRenderer.getDispDesc();
    const Spheref & bs = mShadowProjectors->getBoundingSphere();
    float d = mCamera.getPosition().length();
    float n_min = bs.radius / 100.0f;
    float n = d - bs.radius;
    float f = d + bs.radius;
    if( n < n_min ) n = n_min;
    gRenderer.composePerspectiveMatrixRh( mProj, 0.6f, (float)dd.width/dd.height, n, f );
}
 
//
//
// -----------------------------------------------------------------------------
void GN::SimpleShadowMap::draw()
{
    Renderer & r = gRenderer;

    // draw to shadow map
    Texture * t0 = gSceneResMgr.getResourceT<Texture>( mColorMap );
    Texture * t1 = gSceneResMgr.getResourceT<Texture>( mShadowMap );
    mScene.setProj( mLightProj );
    mScene.setView( mLightView );
    r.setDrawToTextures( 1, t0, 0, 0, 0, t1, MSAA_NONE );
    r.clearScreen();
    mShadowProjectors->draw();

    // draw to back buffer
    mScene.setProj( mProj );
    mScene.setView( mCamera.getViewMatrix() );
    r.setDrawToBackBuf();
    r.clearScreen();
    mShadowReceivers->draw();

    // draw axises
    static const float X[] = { 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f, 0.0f };
    static const float Y[] = { 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f };
    static const float Z[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f };
    const Matrix44f & view = mScene.getView();
    const Matrix44f & proj = mScene.getProj();
    r.drawLines( 0, X, 3*sizeof(float), 1, GN_RGBA32(255,0,0,255), Matrix44f::IDENTITY, view, proj );
    r.drawLines( 0, Y, 3*sizeof(float), 1, GN_RGBA32(0,255,0,255), Matrix44f::IDENTITY, view, proj );
    r.drawLines( 0, Z, 3*sizeof(float), 1, GN_RGBA32(0,0,255,255), Matrix44f::IDENTITY, view, proj );
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

    // load depth only effect
    ResourceId effid = gSceneResMgr.getResourceId( "media::/effect/depth_only.xml" );
    if( 0 == effid ) return false;
    Effect * eff = gSceneResMgr.getResourceT<Effect>( effid );
    if( 0 == eff ) return false;

    TreeTraversePreOrder<Actor> ttpre(mShadowProjectors);
    for( Actor * a = ttpre.first(); a; a = ttpre.next( a ) )
    {
        for( size_t i = 0; i < a->getNumDrawables(); ++i )
        {
            Drawable d = a->getDrawable( i );
            d.effect = effid;
            d.textures.clear();
            d.uniforms.clear();
            d.uniforms["pvw"].binding = eff->getUniformID( "pvw" );
            a->setDrawable( i, d );
        }
    }

    // create shadow receivers
    mShadowReceivers = cloneActorHiearacy( mShadowProjectors );

    // load shadowmap effect
    effid = gSceneResMgr.getResourceId( "media::/effect/shadowmap.xml" );
    if( 0 == effid ) return false;
    eff = gSceneResMgr.getResourceT<Effect>( effid );
    if( 0 == eff ) return false;

    // prepare shadow receivers
    ttpre.reset( mShadowReceivers );
    for( Actor * a = ttpre.first(); a; a = ttpre.next( a ) )
    {
        for( size_t i = 0; i < a->getNumDrawables(); ++i )
        {
            Drawable d = a->getDrawable( i );
            d.effect = effid;

            d.textures.clear();
            d.textures["shadowmap"].texid   = mShadowMap;
            d.textures["shadowmap"].binding = eff->getTextureID( "shadowmap" );

            d.uniforms.clear();

            a->setDrawable( i, d );
        }
    }

    // success
    return true;
}
