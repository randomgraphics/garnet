#ifndef __GN_SCENE_H__
#define __GN_SCENE_H__
// *****************************************************************************
/// \file
/// \brief   Main header for scene management
/// \author  chen@@chenli-homepc (2009.9.6)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace scene2
{
    class NodeBase;

    class SpatialNode;
    class SpatialGraph;

    class VisualNode;
    class LightNode;
    class VisualGraph;

    class World;

    ///
    /// Represent a entity in the world
    ///
    class Entity : NoCopy
    {
    public:

        //@{

        World             & world() const;
        int               & id() const;
        const char        * name() const;

        bool                hasNode( const Guid & type ) const;

        // Note: invalid type triggers exception.
        const NodeBase    & getNode( const Guid & type ) const;
        NodeBase          & getNode( const Guid & type );

        // templated node helpers
        template<class T> const T & getNode() const { return (const T&)getNode( T::guid() ); }
        template<class T> T       & getNode()       { return (T&)getNode( T::guid() ); }

        //@}

    protected:

        //@{

        Entity( World & world, int id ) : mWorld(world), mId(id) {}
        virtual ~Entity() {}

        //@}

    private:

        World & mWorld;
        int     mId;
    };

    struct NodeFactory
    {
        NodeBase * (*createNode)();
        void       (*deleteNode)( NodeBase * );
    };

    class World
    {
    public:

        //@{

        World();
        virtual ~World();

        /// delete all entities, unregister all non-built-in factories
        void clear();

        //@}

        //@{
        bool registerNodeFactory( const Guid & nodeType, NodeFactory factory );
        void hasNodeFactory( const Guid & nodeType );
        //@}

        //@{
        Entity * createEntity( const char * name );
        void     deleteEntity( const char * name );
        void     deleteEntity( int id );
        void     deleteEntity( Entity * );
        void     deleteAllEntities();
        Entity * findEntity( const char * name );
        Entity * findEntity( int id );
        Entity * findOrCreateEntity( const char * name );
        Entity * findOrCreateEntity( int id );
        //@}
    };

    ///
    /// base class of all nodes
    ///
    class NodeBase : public NoCopy
    {
    public:

        //@{

        /// Get the entity that the node belongs to
        Entity & entity() const { return mEntity; }

        /// Get node type
        const Guid & type() const;

        //@}

    protected:

        //@{

        NodeBase( Entity & entity ) : mEntity(entity) {}
        virtual ~NodeBase() {}

        //@}

    private:

        Entity & mEntity;
    };

    ///
    /// Basic class that contains node's spatial inforamtion
    ///
    class SpatialNode : public NodeBase
    {
    public:

        //@{

        static const Guid & guid();
        SpatialGraph      & graph() const { return mGraph; }

        //@}

        //@{

        void                setParent( SpatialNode * parent, SpatialNode * prevSibling = NULL );
        void                setPosition( const Vector3f & );        ///< set position in parent space.
        void                setPivot( const Vector3f  & );          ///< set pivot in parent space
        void                setRotation( const Quaternionf & );     ///< set node orientation, parent space.
        void                setBoundingSphere( const Spheref & s ); /// set bounding sphere, in local space

        SpatialNode       * getParent() const;
        SpatialNode       * getPrevSibling() const;
        SpatialNode       * getNextSibling() const;
        SpatialNode       * getFirstChild() const;
        SpatialNode       * getLastChild() const;

        const Vector3f    & getPosition() const;       ///< get position in parent space
        const Vector3f    & getPivot() const;          ///< get pivot in parent space
        const Quaternionf & getRotation() const;       ///< get orientation, in parent space
        const Spheref     & getBoundingSphere() const; ///< get bounding sphere, in local space
        const Matrix44f   & getLocal2Parent() const;   ///< get local space to parent space transformation matrix
        const Matrix44f   & getLocal2Root() const;     ///< get local space to root space transformation matrix

        //@}

    protected:

        //@{

        SpatialNode( Entity & entity, SpatialGraph & graph ) : NodeBase(entity), mGraph(graph) {}
        virtual ~SpatialNode() {}

        //@}

    private:

        SpatialGraph & mGraph;
    };

    ///
    /// manage spatial information of all nodes.
    ///
    /// Q: how to support multiple cameras?
    ///
    class SpatialGraph
    {
    };

    ///
    /// contains visual information
    ///
    class VisualNode : public NodeBase
    {
    public:

        //@{

        void addModel( gfx::GpuResource * model );
        void draw();

        //@}

    protected:

        //@{

        VisualNode( Entity & entity, VisualGraph & graph )
            : NodeBase( entity )
            , mGraph( graph )
        {
        }

        virtual ~VisualNode()
        {
        }

        //@}

    private:

        VisualGraph & mGraph;

    };

    ///
    /// contains lighting information
    ///
    class LightNode : public NodeBase
    {
    public:

        //@{

        // public methods go here.

        //@}

    protected:

        //@{

        LightNode( Entity & entity, VisualGraph & graph )
            : NodeBase( entity )
            , mGraph( graph )
        {
        }

        virtual ~LightNode()
        {
        }

        //@}

    private:

        VisualGraph & mGraph;
    };

    ///
    /// manage rendering of all nodes
    ///
    class VisualGraph
    {
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_SCENE_H__
