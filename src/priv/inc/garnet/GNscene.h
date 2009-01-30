#ifndef __GN_SCENE_GNSCENE_H__
#define __GN_SCENE_GNSCENE_H__
// *****************************************************************************
/// \file
/// \brief   public header of scene module
/// \author  chenli@@FAREAST (2007.2.17)
// *****************************************************************************

#include "GNgfx.h"
#include "scene/stdparam.h"

namespace GN
{
    ///
    /// namespace for scene module
    ///
    namespace scene
    {
        struct Scene;

        ///
        /// basic object that can be put in to 3D scene. Position is its only property.
        ///
        struct Node : private TreeNode<Node>, public NoCopy
        {
            enum NodeType
            {
                DUMMY,    ///< dummy node
                LIGHT,    ///< light node
                GEOMETRY, ///< geometry node
            };

            virtual void        setParent( Node * parent, Node * prevSibling = NULL );
            virtual void        setPosition( const Vector3f & );      ///< set position in parent space.
            virtual void        setPivot( const Vector3f & );         ///< set pivot position, in parent space, for scaling and rotation.
            virtual void        setRotation( const Quaternionf & );   ///< set node orientation, parent space.

            Scene             & getScene() const { return mScene; }
            NodeType            getType() const { return mType; }

            Node              * getParent() const { return (Node*)TreeNode<Node>::getParent(); }
            Node              * getPrevSibling() const { return (Node*)TreeNode<Node>::getPrevSibling(); }
            Node              * getNextSibling() const { return (Node*)TreeNode<Node>::getNextSibling(); }
            Node              * getFirstChild() const { return (Node*)TreeNode<Node>::getFirstChild(); }
            Node              * getLastChild() const;

            const Vector3f    & getPosition() const { return mPosition; }
            const Vector3f    & getPivot() const { return mPivot; }
            const Quaternionf & getRotation() const { return mRotation; }
            const Matrix44f   & getLocal2Parent() const { if( mTransformDirty ) { const_cast<Node*>(this)->calcTransform(); } return mLocal2Parent; }
            const Matrix44f   & getLocal2Root() const { if( mTransformDirty ) { const_cast<Node*>(this)->calcTransform(); } return mLocal2Root; }

            // signals
            //@{
            Signal1<void,Node*> sigCtor; ///< called after node is constructed
            Signal1<void,Node*> sigDtor; ///< called before node is about to destructed.
            //@}

            /// dtor
            virtual ~Node()
            {
                sigDtor(this);
            }

        protected:

            /// ctor (protected)
            Node( Scene & s, NodeType t )
                : mScene(s)
                , mType(t)
            {
                sigCtor(this);
            }

        private:

            void calcTransform();

        private:

            /// basic node information
            Scene        & mScene;
            const NodeType mType;

            /// node list iterator (used by scene class)
            std::list<Node>::iterator mIter;

            /// transformation
            Vector3f    mPosition;       ///< position in parent space
            Vector3f    mPivot;          ///< origin of rotation, in local space.
            Quaternionf mRotation;       ///< rotation in parent space
            Matrix44f   mLocal2Parent;   ///< local->parent space transformation
            Matrix44f   mParent2Local;   ///< parent->local space transformation
            Matrix44f   mLocal2Root;     ///< local->root space transformation
            Matrix44f   mRoot2Local;     ///< root->local transformation
            bool        mTransformDirty; ///< transformation dirty flag
        };

        ///
        /// Dummy Node
        ///
        class DummyNode : public Node
        {
        public:

            /// ctor
            DummyNode( Scene & s ) : Node( s, Node::DUMMY ) {}
        };

        ///
        /// light node
        ///
        class LightNode : public Node
        {
        public:

            /// Light type enumeration
            enum LightType
            {
                POINT_LIGHT, ///< point light
            };

            /// ctor
            LightNode( Scene & s, const LightType t ) : Node( s, Node::LIGHT ), mType(t) {}

        private:

            const LightType mType;
        };

        ///
        /// Scene Geometry
        ///
        class GeometryNode : public Node
        {
            struct GeometryBlock
            {
                gfx::Effect              effect;
                DynaArray<gfx::Drawable> drawables;

                GeometryBlock( gfx::Renderer & r ) : effect(r) {}
            };

            struct StdParam
            {
                StandardSceneParameterType    type;
                AutoRef<gfx::Uniform> param;
            };

            DynaArray<StdParam>      mStdPerObjParams; ///< standard per-object parameters
            DynaArray<GeometryBlock> mBlocks;
            Spheref                  mBoundingSphere;

        public:

            /// ctor
            GeometryNode( Scene & s ) : Node( s, Node::GEOMETRY ) {}

            /// dtor
            virtual ~GeometryNode();

            /// add new geometry block to the node
            virtual void addGeometryBlock( const gfx::Effect * effect, const gfx::Mesh * mesh, const gfx::MeshSubset * subset = NULL );

            /// get number of geometry blocks
            virtual size_t getNumGeometryBlocks() const { return mBlocks.size(); }

            /// get specific geometry block
            virtual const GeometryBlock & getGeometryBlock( size_t i ) const { return mBlocks[i]; }

            /// get specific geometry block
            virtual GeometryBlock & getGeometryBlock( size_t i ) { return mBlocks[i]; }

            /// draw the geometry
            virtual void draw();

            /// set bounding sphere. sphere center is in parent space
            virtual void setBoundingSphere( const Spheref & s ) { mBoundingSphere = s; }

            /// get bounding sphere. sphere center is in parent space
            const Spheref & getBoundingSphere() const { return mBoundingSphere; }
        };

        ///
        /// public scene interface
        ///
        struct Scene : public NoCopy
        {
            /// get renderer
            virtual gfx::Renderer & getRenderer() const = 0;

            /// \name global parameter management
            //@{
            virtual gfx::Uniform * const * getGlobalParam() const = 0;
            virtual void setProj( const Matrix44f & ) = 0;
            virtual void setView( const Matrix44f & ) = 0;
            //@}

            /// draw node hierarchy
            virtual void renderNodeHierarchy( Node * root ) = 0;
        };

        ///
        /// create simple scene object
        ///
        Scene * createScene( gfx::Renderer & );
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_GNSCENE_H__
