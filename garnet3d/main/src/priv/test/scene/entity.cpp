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
World & GN::scene::Entity::Impl::world() const
{
    return mWorld.owner();
}

//
//
// -----------------------------------------------------------------------------
const char * GN::scene::Entity::Impl::name() const
{
    GN_UNIMPL();
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::Entity::Impl::hasNode( const Guid & nodeType ) const
{
    GN_UNUSED_PARAM( nodeType );
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
const NodeBase * GN::scene::Entity::Impl::getNode( const Guid & nodeType ) const
{
    GN_UNUSED_PARAM( nodeType );
    GN_UNIMPL();
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
NodeBase * GN::scene::Entity::Impl::getNode( const Guid & nodeType )
{
    GN_UNUSED_PARAM( nodeType );
    GN_UNIMPL();
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Entity::Impl::attachNode( const Guid & nodeType, NodeBase * node )
{
    GN_UNUSED_PARAM( nodeType );
    GN_UNUSED_PARAM( node );
    GN_UNIMPL();
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
