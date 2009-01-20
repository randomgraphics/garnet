#ifndef __GN_SCENE_GNSCENE_H__
#define __GN_SCENE_GNSCENE_H__
// *****************************************************************************
/// \file
/// \brief   public header of scene module
/// \author  chenli@@FAREAST (2007.2.17)
// *****************************************************************************

#include "GNgfx.h"

namespace GN
{
    ///
    /// namespace for scene module
    ///
    namespace scene
    {
        ///
        /// standard effect parameters
        ///
        struct EffectParameterDesc
        {
            const char * name;
            size_t       size;
        };

        ///
        /// global parameter table
        ///
        extern const EffectParameterDesc STANDARD_EFFECT_PARAMETER_DESCRIPTIONS[];

        ///
        /// basic object that can be put in to 3D scene. Position is its only property.
        ///
        struct Node : private TreeNode<Node>, public NoCopy
        {
            enum NodeType
            {
                DUMMY,    ///< dummy node, invisible, no extra properties.
                LIGHT,    ///< light node,
                GEOMETRY, ///< geometry node (visible on screen)
            };

            virtual void        setParent( Node * parent, Node * prevSibling = NULL ) { TreeNode<Node>::setParent( parent, prevSibling ); mTransformDirty = true; }
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
        /// light
        ///
        class LightNode : public Node
        {
        public:

            /// Light type enumeration
            enum LightType
            {
                POINT_LIGHT, ///< point light
            };

        protected:

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
            struct GeomBlock
            {
                AutoRef<gfx::Effect>     effect;
                DynaArray<gfx::Drawable> drawables;
            };

            DynaArray<GeomBlock> mBlocks;
            Spheref              mBoundingSphere;

        public:

            /// dtor
            virtual ~GeometryNode();

            /// add new geometry to the node
            virtual void addGeometry( gfx::Effect * effect, gfx::Mesh * mesh, size_t firstidx, size_t numidx );

            /// draw the geometry
            virtual void draw();

            /// set bounding sphere. sphere center is in parent space
            virtual void setBoundingSphere( const Spheref & s ) { mBoundingSphere = s; }

            /// get bounding sphere. sphere center is in parent space
            const Spheref & getBoundingSphere() const { return mBoundingSphere; }

        protected:

            /// ctor
            GeometryNode( Scene & s ) : Node( s, Node::GEOMETRY ) {}
        };

        ///
        /// pure scene interface
        ///
        struct Scene : public NoCopy
        {
            /// clear the scene, release all resources.
            virtual void clear() = 0;

            /// \name resource factory
            //@{
            virtual GeometryNode * createGeometry() = 0;
            virtual LightNode    * createLight( LightNode::LightType type ) = 0;
            //@}

            /// \name set camera properties
            //@{
            virtual void setProj( const Matrix44f & ) = 0;
            virtual void setView( const Matrix44f & ) = 0;
            virtual void setViewport( const Rect<UInt32> & ) = 0; ///< viewport (0,0,0,0) represents current render target size.
            //@}
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
