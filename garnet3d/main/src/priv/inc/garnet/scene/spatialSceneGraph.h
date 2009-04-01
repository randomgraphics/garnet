#ifndef __GN_SCENE_SPATIALSCENEGRAPH_H__
#define __GN_SCENE_SPATIALSCENEGRAPH_H__
// *****************************************************************************
/// \file
/// \brief   General spatial scene graph class
/// \author  chen@@CHENLI-HOMEPC (2009.3.29)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// Geneal spatial scene graph
    ///
    class SpatialSceneGraph : public NoCopy
    {
    public:

        // *********************************************************************
        // public classes
        // *********************************************************************

        ///
        /// spatial scene graph node
        ///
        class Node : private TreeNode<Node>, public NoCopy
        {
        public:

            /// ctor
            Node( SpatialSceneGraph & );

            /// dtor
            virtual ~Node();

            /// modify spatial properties
            //@{
            void                    setParent( Node * parent, Node * prevSibling = NULL );
            void                    setPosition( const Vector3f & );        ///< set position in parent space.
            void                    setPivot( const Vector3f  & );          ///< set pivot in parent space
            void                    setRotation( const Quaternionf & );     ///< set node orientation, parent space.
            void                    setBoundingSphere( const Spheref & s ); ///< set bounding sphere of this node itself (don't have to include descendent nodes), in local space
            //@}

            /// get spatial properties
            //@{
            const Vector3f        & getPosition() const { return mPosition; } ///< get position in parent space
            const Vector3f        & getPivot() const { return mPivot; } ///< get pivot in parent space
            const Quaternionf     & getRotation() const { return mRotation; } ///< get orientation, in parent space
            const Spheref         & getBoundingSphere() const { return mBoundingSphere; } // get bounding sphere, in local space
            const Matrix44f       & getLocal2Parent() const { if( mTransformDirty ) { const_cast<Node*>(this)->calcTransform(); } return mLocal2Parent; }
            const Matrix44f       & getLocal2Root() const { if( mTransformDirty ) { const_cast<Node*>(this)->calcTransform(); } return mLocal2Root; }
            //@}

            /// get graph properties
            //@{
            SpatialSceneGraph     & getSceneGraph() const { return mSceneGraph; }
            Node                  * getParent() const { return (Node*)TreeNode<Node>::getParent(); }
            Node                  * getPrevSibling() const { return (Node*)TreeNode<Node>::getPrevSibling(); }
            Node                  * getNextSibling() const { return (Node*)TreeNode<Node>::getNextSibling(); }
            Node                  * getFirstChild() const { return (Node*)TreeNode<Node>::getFirstChild(); }
            Node                  * getLastChild() const;
            //@}

        private:

            void calcTransform();

        private:

            /// the scene graph that this node belongs to
            SpatialSceneGraph & mSceneGraph;

            /// handle of the node, which is used by scene graph to idenfity the node.
            HandleType          mHandle;

            /// transformation
            Vector3f    mPosition;       ///< position in parent space
            Vector3f    mPivot;          ///< pivot (origin of the rotation) in parent space
            Quaternionf mRotation;       ///< rotation in parent space
            Spheref     mBoundingSphere; ///< bounding sphere, in local space
            Matrix44f   mLocal2Parent;   ///< local->parent space transformation
            Matrix44f   mParent2Local;   ///< parent->local space transformation
            Matrix44f   mLocal2Root;     ///< local->root space transformation
            Matrix44f   mRoot2Local;     ///< root->local transformation
            bool        mTransformDirty; ///< transformation dirty flag
        };

        ///
        /// Return value of query operations. Must be released after use.
        ///
        class QueryResult : public RefCounter
        {
        public:

            //@{
            virtual Node * getNodeAt( size_t i ) const = 0;
            virtual size_t numNodes() const = 0;
            //@}

            /// [] operator
            Node * operator[]( size_t i ) const { return getNodeAt( i ); }
        };

        // *********************************************************************
        // public methods
        // *********************************************************************

        /// update camera
        //@{
        virtual void setProj( const Matrix44f & ) = 0;
        virtual void setView( const Matrix44f & ) = 0;
        //@}

        /// spatial queries (note: query result must be released after use
        //@{
        virtual QueryResult * getVisibleNodes() = 0;
        virtual QueryResult * getNodesNearby( size_t maxNodeCount ) = 0;
        //@}
    };

    ///
    /// create new instance of spatial scene graph
    ///
    SpatialSceneGraph * createSpatialSceneGraph();
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_SPATIALSCENEGRAPH_H__