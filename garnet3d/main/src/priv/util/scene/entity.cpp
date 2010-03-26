#include "pch.h"
#include "entity.h"
#include "world.h"

using namespace GN;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util");

// *****************************************************************************
// Entity::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::Entity::Impl::~Impl()
{
    for( NodeMap::KeyValuePair * i = mNodes.First(); i != NULL; i = mNodes.Next( i ) )
    {
        NodeBase * n = i->value;

        GN_ASSERT( n );

        delete n;
    }

    mNodes.Clear();
}

//
//
// -----------------------------------------------------------------------------
World & GN::util::Entity::Impl::world() const
{
    return mWorld.owner();
}

//
//
// -----------------------------------------------------------------------------
const char * GN::util::Entity::Impl::Name() const
{
    return mWorld.getEntityName( mID );
}

//
//
// -----------------------------------------------------------------------------
bool GN::util::Entity::Impl::hasNode( const Guid & nodeType ) const
{
    return mNodes.Find( nodeType ) != NULL;
}

//
//
// -----------------------------------------------------------------------------
const NodeBase * GN::util::Entity::Impl::getNode( const Guid & nodeType ) const
{
    NodeBase * const * ppNode = mNodes.Find( nodeType );

    if( NULL == ppNode )
    {
        GN_ERROR(sLogger)( "Invalid node type: %s", nodeType.ToStr() );
        return NULL;
    }
    else
    {
        GN_ASSERT( *ppNode );
        return *ppNode;
    }
}

//
//
// -----------------------------------------------------------------------------
NodeBase * GN::util::Entity::Impl::getNode( const Guid & nodeType )
{
    NodeBase ** ppNode = mNodes.Find( nodeType );

    if( NULL == ppNode )
    {
        GN_ERROR(sLogger)( "Invalid node type: %s", nodeType.ToStr() );
        return NULL;
    }
    else
    {
        GN_ASSERT( *ppNode );
        return *ppNode;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::util::Entity::Impl::attachNode( const Guid & nodeType, NodeBase * node )
{
    if( NULL == node )
    {
        GN_ERROR(sLogger)( "Fail to attach node to entity: NULl node pointer." );
        return;
    }

    NodeBase ** ppNode = mNodes.Find( nodeType );

    if( NULL != ppNode )
    {
        NodeBase * oldnode = *ppNode;

        GN_ASSERT( oldnode );

        if( oldnode != node )
        {
            // TODO:
            //  This will crash memory, if one node is attached to more than one
            //  entities, or attach to single entity multiple times with different
            //  node type.
            //  Need a smart pointer here.
            delete oldnode;
            *ppNode = node;
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
GN::util::Entity::Entity( World & w, int id )
    : mImpl(NULL)
{
    mImpl = new Impl( *w.mImpl, *this, id );
}

//
//
// -----------------------------------------------------------------------------
GN::util::Entity::~Entity()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
World             & GN::util::Entity::world() const { return mImpl->world(); }
int                 GN::util::Entity::id() const { return mImpl->id(); }
const char        * GN::util::Entity::Name() const { return mImpl->Name(); }

bool                GN::util::Entity::hasNode( const Guid & nodeType ) const { return mImpl->hasNode( nodeType ); }
const NodeBase    * GN::util::Entity::getNode( const Guid & nodeType ) const { return mImpl->getNode( nodeType ); }
NodeBase          * GN::util::Entity::getNode( const Guid & nodeType ) { return mImpl->getNode( nodeType ); }
void                GN::util::Entity::attachNode( const Guid & nodeType, NodeBase * node ) { return mImpl->attachNode( nodeType, node ); }
