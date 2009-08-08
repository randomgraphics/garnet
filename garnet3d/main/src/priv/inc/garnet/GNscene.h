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
        /// basic object that can be put in to 3D scene.
        ///
        struct Node : private TreeNode<Node>, public NoCopy
        {
            enum NodeType
            {
                DUMMY,    ///< dummy node
                LIGHT,    ///< light node
                GEOMETRY, ///< geometry node
            };

            void                setParent( Node * parent, Node * prevSibling = NULL );
            void                setPosition( const Vector3f & );        ///< set position in parent space.
            void                setPivot( const Vector3f  & );          ///< set pivot in parent space
            void                setRotation( const Quaternionf & );     ///< set node orientation, parent space.
            void                setBoundingSphere( const Spheref & s ); /// set bounding sphere, in local space

            Scene             & getScene() const { return mScene; }
            NodeType            getType() const { return mType; }

            Node              * getParent() const { return (Node*)TreeNode<Node>::getParent(); }
            Node              * getPrevSibling() const { return (Node*)TreeNode<Node>::getPrevSibling(); }
            Node              * getNextSibling() const { return (Node*)TreeNode<Node>::getNextSibling(); }
            Node              * getFirstChild() const { return (Node*)TreeNode<Node>::getFirstChild(); }
            Node              * getLastChild() const;

            const Vector3f    & getPosition() const { return mPosition; } ///< get position in parent space
            const Vector3f    & getPivot() const { return mPivot; } ///< get pivot in parent space
            const Quaternionf & getRotation() const { return mRotation; } ///< get orientation, in parent space
            const Spheref     & getBoundingSphere() const { return mBoundingSphere; } // get bounding sphere, in local space
            const Matrix44f   & getLocal2Parent() const { if( mTransformDirty ) { const_cast<Node*>(this)->calcTransform(); } return mLocal2Parent; }
            const Matrix44f   & getLocal2Root() const { if( mTransformDirty ) { const_cast<Node*>(this)->calcTransform(); } return mLocal2Root; }

            // signals
            //@{
            Signal1<void,Node*> sigCtor; ///< called after node is constructed.
            Signal1<void,Node*> sigDtor; ///< called before node is destructed.
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

                GeometryBlock( gfx::Gpu & r ) : effect(r) {}
            };

            struct StandardUniform
            {
                StandardSceneParameterType type;
                AutoRef<gfx::Uniform>      uniform;
            };

            DynaArray<StandardUniform> mStandardPerObjectUniforms; ///< standard per-object parameters
            DynaArray<GeometryBlock*>  mBlocks;

        public:

            /// ctor
            GeometryNode( Scene & s ) : Node( s, Node::GEOMETRY ) {}

            /// dtor
            virtual ~GeometryNode();

            /// Add new geometry block to the node.
            /// Note that caller is free to destroy input parameters after the function returns.
            virtual void addGeometryBlock( const gfx::Effect * effect, const gfx::Mesh * mesh, const gfx::MeshSubset * subset = NULL );

            /// get number of geometry blocks
            virtual size_t getNumGeometryBlocks() const { return mBlocks.size(); }

            /// get specific geometry block
            virtual const GeometryBlock & getGeometryBlock( size_t i ) const { return *mBlocks[i]; }

            /// get specific geometry block
            virtual GeometryBlock & getGeometryBlock( size_t i ) { return *mBlocks[i]; }

            /// draw the geometry
            virtual void draw();
        };

        ///
        /// public scene interface
        ///
        struct Scene : public NoCopy
        {
            ///
            /// Uniform collection
            ///
            class UniformCollection
            {
            public:

                /// bracket operator
                gfx::Uniform & operator[]( size_t type ) const
                {
                    if( type >= NUM_STANDARD_SCENE_PARAMETERS )
                    {
                        static Logger * sLogger = getLogger("GN.scene");
                        GN_ERROR(sLogger)( "Invalid scene parameter type: %d", type );
                        GN_ASSERT( mDummy );
                        return *mDummy;
                    }

                    const StandardSceneParameterDesc & desc = getStandardSceneParameterName( type );

                    if( !desc.global )
                    {
                        static Logger * sLogger = getLogger("GN.scene");
                        GN_ERROR(sLogger)( "Non-global parameter \"%s\" is not accessible through global uniform collection.", desc.name );
                        GN_ASSERT( mDummy );
                        return *mDummy;
                    }

                    GN_ASSERT( mUniforms[type] );
                    return *mUniforms[type];
                }

            protected:

                /// protected ctor
                UniformCollection() : mUniforms(NULL), mDummy(NULL) {}

                /// protected dtor
                virtual ~UniformCollection() {}

            protected:

                // Note: sub-class should initialize those pointers.

                /// global uniform array
                gfx::Uniform * const * mUniforms;

                /// dummy uniform pointer
                gfx::Uniform *         mDummy;
            };

            /// get GPU
            virtual gfx::Gpu & getGpu() const = 0;

            /// \name global parameter management
            //@{
            UniformCollection & globalUniforms;
            virtual void setProj( const Matrix44f & ) = 0;
            virtual void setView( const Matrix44f & ) = 0;
            virtual void setDefaultLight0Position( const Vector3f & ) = 0;
            //@}

            /// draw node hierarchy
            virtual void renderNodeHierarchy( Node * root ) = 0;

        protected:

            Scene( UniformCollection & uc ) : globalUniforms(uc) {}
        };

        ///
        /// create simple scene object
        ///
        Scene * createScene( gfx::Gpu & );
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_GNSCENE_H__
