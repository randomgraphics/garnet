#include "pch.h"
#include "spatialSceneGraphImpl.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.scene.SpatialSceneGraph");

// *****************************************************************************
// Node class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::SpatialSceneGraph::Node::Node( SpatialSceneGraph & sg )
    : mSceneGraph( sg )
{
    SpatialSceneGraphImpl & impl = (SpatialSceneGraphImpl &)sg;
    mHandle = impl.onNodeConstruction( *this );
}

//
//
// -----------------------------------------------------------------------------
GN::scene::SpatialSceneGraph::Node::~Node()
{
    SpatialSceneGraphImpl & impl = (SpatialSceneGraphImpl &)mSceneGraph;
    impl.onNodeDestruction( mHandle );
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialSceneGraph::Node::setParent( Node * parent, Node * prevSibling )
{
    if( parent != getParent() || prevSibling != getPrevSibling() )
    {
        TreeNode<Node>::setParent( parent, prevSibling );
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialSceneGraph::Node::setPosition( const Vector3f & p )
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
void GN::scene::SpatialSceneGraph::Node::setPivot( const Vector3f & p )
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
void GN::scene::SpatialSceneGraph::Node::setRotation( const Quaternionf & q )
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
void GN::scene::SpatialSceneGraph::Node::setBoundingSphere( const Spheref & s )
{
    mBoundingSphere = s;
}


//
//
// -----------------------------------------------------------------------------
void GN::scene::SpatialSceneGraph::Node::calcTransform()
{
    GN_ASSERT( mTransformDirty );

    // Transformation to the node is:
    //
    //  1. move to "position", in parent space
    //  2. rotate around "pivot" point in parent space, which includes 3 sub steps:
    //     a) move -"pivot" points, in parent space
    //     b) rotate, in parent space
    //     c) move +"pivot" points, in parent space

    Matrix44f r44, t1, t2;
    mRotation.toMatrix44( r44 );
    t1.translate( mPosition - mPivot );
    t2.translate( mPivot );

    mLocal2Parent = t2 * r44 * t1;
    mParent2Local = Matrix44f::sInverse( mLocal2Parent );

    Node * p = getParent();
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
