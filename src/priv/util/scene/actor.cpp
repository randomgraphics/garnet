#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.scene.Actor");

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
// 
// -----------------------------------------------------------------------------
GN::scene::Actor::Actor( Scene & s )
    : mScene( s )
{
    clear();
}

//
// 
// -----------------------------------------------------------------------------
GN::scene::Actor::~Actor()
{
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::setPosition( const Vector3f & p )
{
    if( p != mPosition )
    {
        mPosition = p;
        mTransformDirty = true;
    }
    else
    {
        GN_TRACE(sLogger)( "redundant position update." );
    }
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::setRotation( const Quaternionf & q )
{
    if( q != mRotation )
    {
        mRotation = q;
        mTransformDirty = true;
    }
    else
    {
        GN_TRACE(sLogger)( "redundant rotation update." );
    }
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::setDrawable( const Drawable & d )
{
    mDrawable = d;
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::clear()
{
    mDrawable.clear();

    mPosition.set( 0, 0, 0 );
    mRotation.identity();
    mLocal2Parent.identity();

    mDirtyFlags = 0;
}

//
// 
// -----------------------------------------------------------------------------
bool GN::scene::Actor::loadFromXmlNode( const XmlNode & node, const StrA & basedir )
{
    GN_UNUSED_PARAM( node );
    GN_UNUSED_PARAM( basedir );
    return false;
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::draw()
{
    if( mDrawable.empty() ) return;

    Effect * e = gSceneResMgr.getResourceT<Effect>( mDrawable.effect );

    EffectItemID id;

    if( e->hasUniform( "pvw", &id ) )
    {
        const Matrix44f & world = getLocal2Parent();

        Matrix44f pvw = mScene.getProj() * mScene.getView() * world;
        
        e->setUniform( id, pvw );
    }

    if( e->hasUniform( "view", &id ) )
    {
        e->setUniform( id, mScene.getView() );
    }

    if( e->hasUniform( "light0_pos", &id ) )
    {
        e->setUniform( id, Vector4f( mScene.light(0).position, 1.0f ) );
    }

    mDrawable.draw();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::calcTransform()
{
    GN_ASSERT( mTransformDirty );

    Matrix33f m33;
    mRotation.toMatrix33( m33 );
    mLocal2Parent.set( m33, mPosition );

    mTransformDirty = false;
}
