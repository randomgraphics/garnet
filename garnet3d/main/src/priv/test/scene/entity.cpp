#include "pch.h"
#include "entity.h"
#include "world.h"

using namespace GN;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene");

// *****************************************************************************
// Entity::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::Entity::Impl::~Impl()
{
    for( NodeMap::iterator i = mNodes.begin(); i != mNodes.end(); ++i )
    {
        NodeBase * n = i->second;

        GN_ASSERT( n );

        delete n;
    }

    mNodes.clear();
}

//
//
// -----------------------------------------------------------------------------
World & GN::scene::Entity::Impl::world() const
{
    return mWorld.owner();
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::scene::Entity::Impl::type() const
{
    return mWorld.getEntityType( mID );
}

//
//
// -----------------------------------------------------------------------------
const char * GN::scene::Entity::Impl::name() const
{
    return mWorld.getEntityName( mID );
}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::Entity::Impl::hasNode( const Guid & nodeType ) const
{
    return mNodes.find( nodeType ) != mNodes.end();
}

//
//
// -----------------------------------------------------------------------------
const NodeBase * GN::scene::Entity::Impl::getNode( const Guid & nodeType ) const
{
    NodeMap::const_iterator i = mNodes.find( nodeType );

    if( i == mNodes.end() )
    {
        GN_ERROR(sLogger)( "Invalid node type: %s", nodeType.toStr() );
        return NULL;
    }
    else
    {
        const NodeBase * n = i->second;
        GN_ASSERT( n );
        return n;
    }
}

//
//
// -----------------------------------------------------------------------------
NodeBase * GN::scene::Entity::Impl::getNode( const Guid & nodeType )
{
    NodeMap::iterator i = mNodes.find( nodeType );

    if( i == mNodes.end() )
    {
        GN_ERROR(sLogger)( "Invalid node type: %s", nodeType.toStr() );
        return NULL;
    }
    else
    {
        NodeBase * n = i->second;
        GN_ASSERT( n );
        return n;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Entity::Impl::attachNode( const Guid & nodeType, NodeBase * node )
{
    if( NULL == node )
    {
        GN_ERROR(sLogger)( "Fail to attach node to entity: NULl node pointer." );
        return;
    }

    NodeMap::iterator i = mNodes.find( nodeType );

    if( mNodes.end() != i )
    {
        NodeBase * oldnode = i->second;

        GN_ASSERT( oldnode );

        if( oldnode != node )
        {
            // TODO:
            //  This will crash memory, if one node is attached to more than one
            //  entities, or attach to single entity multiple times with different
            //  node type.
            //  Need a smart pointer here.
            delete oldnode;
            i->second = node;
        }
    }
    else
    {
        mNodes[nodeType] = node;
    }
}

// *****************************************************************************
// Entity::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// Entity public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::Entity::Entity( World & w, int id )
    : mImpl(NULL)
{
    mImpl = new Impl( *w.mImpl, *this, id );
}

//
//
// -----------------------------------------------------------------------------
GN::scene::Entity::~Entity()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
World             & GN::scene::Entity::world() const { return mImpl->world(); }
int                 GN::scene::Entity::id() const { return mImpl->id(); }
const char        * GN::scene::Entity::name() const { return mImpl->name(); }

bool                GN::scene::Entity::hasNode( const Guid & nodeType ) const { return mImpl->hasNode( nodeType ); }
const NodeBase    * GN::scene::Entity::getNode( const Guid & nodeType ) const { return mImpl->getNode( nodeType ); }
NodeBase          * GN::scene::Entity::getNode( const Guid & nodeType ) { return mImpl->getNode( nodeType ); }
void                GN::scene::Entity::attachNode( const Guid & nodeType, NodeBase * node ) { return mImpl->attachNode( nodeType, node ); }
