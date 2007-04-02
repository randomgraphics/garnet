#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.scene.Actor");

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

// *****************************************************************************
// local functions
// *****************************************************************************

//
// get integer value of specific attribute
// -----------------------------------------------------------------------------
static bool sGetFloatAttrib( const XmlElement & node, const char * attribName, float & result )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a || !str2Float( result, a->value.cptr() ) )
    {
        GN_ERROR(sLogger)( "attribute '%s' is missing, or invalid float!", attribName );
        return false;
    }
    else
    {
        return true;
    }
}

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
void GN::scene::Actor::setBoundingSphere( const Spheref & s )
{
    mBoundingSphere = s;
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
    mBoundingSphere.center.set( 0, 0, 0 );
    mBoundingSphere.radius = 1.0f;

    mDirtyFlags = 0;
}

//
// 
// -----------------------------------------------------------------------------
bool GN::scene::Actor::loadFromXmlNode( const XmlNode & root, const StrA & basedir )
{
    GN_GUARD;

    // check root node
    const XmlElement * eroot = root.toElement();
    if( 0 == eroot || "actor" != eroot->name )
    {
        GN_ERROR(sLogger)( "root node must be \"<actor>\"." );
        return false;
    }

    // clear to empty
    clear();

    bool hasbs = false;
    bool hasdraw = true;
    for( const XmlNode * c = eroot->child; c; c = c->sibling )
    {
        const XmlElement * e = c->toElement();
        if( !e ) continue;

        if( "transform" == e->name )
        {
            // TODO: load transform
        }
        else if( "bsphere" == e->name )
        {
            Spheref s;
            if( !sGetFloatAttrib( *e, "x", s.center.x ) ) return false;
            if( !sGetFloatAttrib( *e, "y", s.center.y ) ) return false;
            if( !sGetFloatAttrib( *e, "z", s.center.z ) ) return false;
            if( !sGetFloatAttrib( *e, "r", s.radius ) ) return false;
            setBoundingSphere( s );
            hasbs = true;
        }
        else if( "drawable" == e->name )
        {
            Drawable d;
            if( !d.loadFromXmlNode( *e, basedir ) ) return false;
            setDrawable( d );
            hasdraw = true;
        }
        else
        {
            GN_ERROR(sLogger)( "Ignore unknown node '%s'.", e->name.cptr() );
        }
    }

    if( !hasbs )
    {
        GN_WARN(sLogger)( "No bounding sphere." );
    }

    if( !hasdraw )
    {
        GN_ERROR(sLogger)( "No drawable." );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::draw()
{
    if( mDrawable.empty() ) return;

    Effect * e = gSceneResMgr.getResourceT<Effect>( mDrawable.effect );

    EffectItemID id;

    const Matrix44f & world = getLocal2Parent();

    if( e->hasUniform( "pvw", &id ) )
    {
        Matrix44f pvw = mScene.getProj() * mScene.getView() * world;
        
        e->setUniform( id, pvw );
    }

    if( e->hasUniform( "invworld", &id ) )
    {
        Matrix44f invworld = Matrix44f::sInverse( world );
        e->setUniform( id, invworld );
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

    Matrix33f r33;
    Matrix44f r44, t44;

    mRotation.toMatrix33( r33 );
    r44.set( r33 );
    t44.translate( mPosition );

    mLocal2Parent = r44 * t44;

    mTransformDirty = false;
}
