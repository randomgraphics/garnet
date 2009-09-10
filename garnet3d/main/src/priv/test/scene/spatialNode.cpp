#include "pch.h"
#include "spatialNode.h"

using namespace GN;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene");

// *****************************************************************************
// SpatialNode::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialNode::Impl::setParent( SpatialNode * parent, SpatialNode * prevSibling )
{
    // only nodes belong to same graph can be linked with each other.
    GN_ASSERT( NULL == parent || &parent->graph() == &mOwner.graph() );
    GN_ASSERT( NULL == prevSibling || &prevSibling->graph() == &mOwner.graph() );

    if( parent != getParent() || prevSibling != getPrevSibling() )
    {
        TreeNodeClass::setParent( toImplPtr(parent), toImplPtr(prevSibling) );
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialNode::Impl::setPosition( const Vector3f & p )
{
    if( p != mPosition )
    {
        mPosition = p;
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialNode::Impl::setPivot( const Vector3f & p )
{
    if( p != mPivot )
    {
        mPivot = p;
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialNode::Impl::setRotation( const Quaternionf & q )
{
    if( q != mRotation )
    {
        mRotation = q;
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialNode::Impl::setBoundingSphere( const Spheref & s )
{
    mBoundingSphere = s;
}

//
//
// -----------------------------------------------------------------------------
SpatialNode * GN::scene::SpatialNode::Impl::getLastChild() const
{
    GN_UNIMPL();
    return NULL;
}

// *****************************************************************************
// SpatialNode::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialNode::Impl::calcTransform()
{
    GN_ASSERT( mTransformDirty );

    Matrix33f r33;
    Matrix44f r44, t1, t2;

    // object will be:
    //
    //  1. moved to "position", in parent space
    //  2. rotated around "pivot" point in parent space, which includes 3 sub steps:
    //     a) moved -"pivot" points, in parent space
    //     b) rotated, in parent space
    //     c) moved +"pivot" points, in parent space

    mRotation.toMatrix33( r33 );
    r44.set( r33 );

    t1.translate( mPosition - mPivot );
    t2.translate( mPivot );

    mLocal2Parent = t2 * r44 * t1;
    mParent2Local = Matrix44f::sInverse( mLocal2Parent );

    Impl * parent = (Impl*)( TreeNodeClass::getParent() );
    if( parent )
    {
        mLocal2Root = parent->getLocal2Root() * mLocal2Parent;
        mRoot2Local = Matrix44f::sInverse( mLocal2Root );
    }
    else
    {
        mLocal2Root = mLocal2Parent;
        mRoot2Local = mParent2Local;
    }

    mTransformDirty = false;
}

// *****************************************************************************
// SpatialNode public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::SpatialNode::SpatialNode( Entity & entity, SpatialGraph & graph )
    : NodeBase(entity)
    , mImpl(NULL)
{
    mImpl = new Impl( *this, graph );
}

//
//
// -----------------------------------------------------------------------------
GN::scene::SpatialNode::~SpatialNode()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::scene::SpatialNode::guid()
{
    static const Guid SPATIAL_NODE_GUID =
    {
        0xaab1738d, 0x3bc7, 0x424d, { 0x90, 0x35, 0x49, 0x19, 0x47, 0xef, 0xa7, 0x20 },
    };

    return SPATIAL_NODE_GUID;
}

//
//
// -----------------------------------------------------------------------------
SpatialGraph      & GN::scene::SpatialNode::graph() const { return mImpl->graph(); }
void                GN::scene::SpatialNode::setParent( SpatialNode * parent, SpatialNode * prevSibling ) { return mImpl->setParent( parent, prevSibling ); }
void                GN::scene::SpatialNode::setPosition( const Vector3f & position ) { return mImpl->setPosition( position ); }
void                GN::scene::SpatialNode::setPivot( const Vector3f & pivot ) { return mImpl->setPivot( pivot ); }
void                GN::scene::SpatialNode::setRotation( const Quaternionf & rotation ) { return mImpl->setRotation( rotation ); }
void                GN::scene::SpatialNode::setBoundingSphere( const Spheref & sphere ) { return mImpl->setBoundingSphere( sphere ); }

SpatialNode       * GN::scene::SpatialNode::getParent() const { return mImpl->getParent(); }
SpatialNode       * GN::scene::SpatialNode::getPrevSibling() const { return mImpl->getPrevSibling(); }
SpatialNode       * GN::scene::SpatialNode::getNextSibling() const { return mImpl->getNextSibling(); }
SpatialNode       * GN::scene::SpatialNode::getFirstChild() const { return mImpl->getFirstChild(); }
SpatialNode       * GN::scene::SpatialNode::getLastChild() const { return mImpl->getLastChild(); }

const Vector3f    & GN::scene::SpatialNode::getPosition() const { return mImpl->getPosition(); }
const Vector3f    & GN::scene::SpatialNode::getPivot() const { return mImpl->getPivot(); }
const Quaternionf & GN::scene::SpatialNode::getRotation() const { return mImpl->getRotation(); }
const Spheref     & GN::scene::SpatialNode::getBoundingSphere() const { return mImpl->getBoundingSphere(); }
const Matrix44f   & GN::scene::SpatialNode::getLocal2Parent() const { return mImpl->getLocal2Parent(); }
const Matrix44f   & GN::scene::SpatialNode::getLocal2Root() const { return mImpl->getLocal2Root(); }

// *****************************************************************************
// SpatialNode factory
// *****************************************************************************

class SpatialNodeInternal : public SpatialNode
{
public:

    SpatialNodeInternal( Entity & entity, SpatialGraph & graph ) : SpatialNode( entity, graph ) {}

    virtual ~SpatialNodeInternal() {}

};

//
//
// -----------------------------------------------------------------------------
SpatialNode * GN::scene::newSpatialNode( Entity & entity, SpatialGraph & graph )
{
    return new SpatialNodeInternal( entity, graph );
}
