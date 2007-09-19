#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.scene.Actor");

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
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
        dirtyTransform();
    }
    else
    {
        GN_TRACE(sLogger)( "redundant position update." );
    }
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::setPivot( const Vector3f & p )
{
    if( p != mPivot )
    {
        mPivot = p;
        dirtyTransform();
    }
    else
    {
        GN_TRACE(sLogger)( "redundant pivot update." );
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
        dirtyTransform();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Actor::setNumDrawables( size_t n )
{
    mDrawables.resize( n );
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Actor::setDrawable( size_t idx, const Drawable & d )
{
    if( idx >= mDrawables.size() )
    {
        GN_ERROR(sLogger)( "invalid drawable index!" );
        return;
    }
    mDrawables[idx] = d;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Actor::addDrawable( const Drawable & d )
{
    mDrawables.append( d );
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
    mDrawables.clear();

    mPosition.set( 0, 0, 0 );
    mPivot.set( 0, 0, 0 );
    mRotation.identity();
    mLocal2Parent.identity();
    mParent2Local.identity();
    mLocal2Root.identity();
    mRoot2Local.identity();
    mBoundingSphere.center.set( 0, 0, 0 );
    mBoundingSphere.radius = 1.0f;

    mDirtyFlags = 0;
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::copyto( Actor & a ) const
{
    if( &a.mScene != &mScene )
    {
        GN_ERROR(sLogger)( "Clone target can not belong to different scene!" );
        return;
    }

    // Note: leave mNode untouched

    a.mDrawables = mDrawables;
    a.mPosition = mPosition;
    a.mPivot = mPivot;
    a.mRotation = mRotation;
    a.mLocal2Parent = mLocal2Parent;
    a.mParent2Local = mParent2Local;
    a.mLocal2Root = mLocal2Root;
    a.mRoot2Local = mRoot2Local;
    a.mBoundingSphere = mBoundingSphere;
    a.mDirtyFlags = mDirtyFlags;
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

    GN_LOG_BEGIN( sLogger, Logger::LL_INFO );
    const XmlAttrib * nodename = eroot->findAttrib( "name" );
    GN_INFO(sLogger)( "Load actor '%s'", nodename ? nodename->value.cptr() : "unnamed" );
    GN_LOG_END();

    // clear to empty
    clear();

    bool hasbs = false;
    for( const XmlNode * c = eroot->child; c; c = c->sibling )
    {
        const XmlElement * e = c->toElement();
        if( !e ) continue;

        if( "transform" == e->name )
        {
            // TODO: load transform
        }
        else if( "position" == e->name )
        {
            Vector3f p;
            if( !sGetFloatAttrib( *e, "x", p.x ) ) return false;
            if( !sGetFloatAttrib( *e, "y", p.y ) ) return false;
            if( !sGetFloatAttrib( *e, "z", p.z ) ) return false;
            setPosition( p );
        }
        else if( "pivot" == e->name )
        {
            Vector3f p;
            if( !sGetFloatAttrib( *e, "x", p.x ) ) return false;
            if( !sGetFloatAttrib( *e, "y", p.y ) ) return false;
            if( !sGetFloatAttrib( *e, "z", p.z ) ) return false;
            setPivot( p );
        }
        else if( "rotation" == e->name )
        {
            Quaternionf q;
            if( !sGetFloatAttrib( *e, "nx", q.v.x ) ) return false;
            if( !sGetFloatAttrib( *e, "ny", q.v.y ) ) return false;
            if( !sGetFloatAttrib( *e, "nz", q.v.z ) ) return false;
            if( !sGetFloatAttrib( *e, "d",  q.w ) ) return false;
            setRotation( q );
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
            UIntPtr d = mScene.loadDrawableFromXmlNode( *e, basedir );
            if( 0 == d ) return false;
            addDrawable( d );
        }
        else if( "actor" == e->name )
        {
            // ignore sub actors.
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

    // success
    return true;

    GN_UNGUARD;
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::draw()
{
    //const Matrix44f & world = getLocal2Root();

    // TODO: sort drawable by kernel

    for( size_t i = 0; i < mDrawables.size(); ++i )
    {
        const Drawable & d = mDrawables[i];

        /*
        if( d.hasUniform( "pvw" ) )
        {
            Matrix44f pvw = mScene.getProj() * mScene.getView() * world;
            d.uniforms["pvw"].value = pvw;
        }
        else if( e->hasUniform( "pvw", &id ) )
        {
            Matrix44f pvw = mScene.getProj() * mScene.getView() * world;
            e->setUniform( id, pvw );
        }

        if( d.hasUniform( "invworld" ) )
        {
            Matrix44f invworld = Matrix44f::sInverse( world );
            d.uniforms["invworld"].value = invworld;
        }
        else if( e->hasUniform( "invworld", &id ) )
        {
            Matrix44f invworld = Matrix44f::sInverse( world );
            e->setUniform( id, invworld );
        }

        if( d.hasUniform( "light0_pos" ) )
        {
            d.uniforms["light0_pos"].value = Vector4f( mScene.light(0).position, 1.0f );
        }
        else if( e->hasUniform( "light0_pos", &id ) )
        {
            e->setUniform( id, Vector4f( mScene.light(0).position, 1.0f ) );
        }*/

        mScene.getRenderEngine().render( d );
    }

    // draw children
    Actor * c = getFirstChild();
    while( c )
    {
        c->draw();
        c = c->getNextSibling();
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::dirtyTransform()
{
    if( mTransformDirty ) return;

    mTransformDirty = true;

    // dirty all children
    Actor * c = getFirstChild();
    while( c )
    {
        c->dirtyTransform();
        c = c->getNextSibling();
    }
}

//
// 
// -----------------------------------------------------------------------------
void GN::scene::Actor::calcTransform()
{
    GN_ASSERT( mTransformDirty );

    Matrix33f r3;
    Matrix44f r4, t1, t2;

    mRotation.toMatrix33( r3 );
    r4.set( r3 );

    t1.translate( mPosition + mPivot );
    t2.translate( -mPivot );

    mLocal2Parent =  t1 * r4 * t2;
    mParent2Local = Matrix44f::sInverse( mLocal2Parent );

    Actor * p = getParent();
    if( p )
    {
        mLocal2Root = p->getLocal2Root() * mLocal2Parent;
        mRoot2Local = Matrix44f::sInverse( mLocal2Root );
    }
    else
    {
        mLocal2Root = mLocal2Parent;
        mRoot2Local = mParent2Local;
    }

    mTransformDirty = false;
}
