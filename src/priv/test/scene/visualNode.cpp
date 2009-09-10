#include "pch.h"
#include "visualNode.h"

using namespace GN;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene");

// *****************************************************************************
// VisualNode::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::VisualNode::Impl::addModel( gfx::GpuResource * model )
{
    GN_UNUSED_PARAM( model );
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::VisualNode::Impl::draw( Camera & ) const
{
    GN_UNIMPL();
}

// *****************************************************************************
// VisualNode::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// VisualNode public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::VisualNode::VisualNode( Entity & entity, VisualGraph & graph )
    : NodeBase(entity)
    , mImpl(NULL)
{
    mImpl = new Impl( *this, graph );
}

//
//
// -----------------------------------------------------------------------------
GN::scene::VisualNode::~VisualNode()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::scene::VisualNode::guid()
{
    static const Guid MY_GUID =
    {
        0x910f5f26, 0x8183, 0x4186, { 0x81, 0x32, 0xbf, 0x19, 0xc5, 0x8b, 0xd8, 0x6b }
    };

    return MY_GUID;
}

//
//
// -----------------------------------------------------------------------------
VisualGraph & GN::scene::VisualNode::graph() const { return mImpl->graph(); }
void          GN::scene::VisualNode::addModel( gfx::GpuResource * model ) { return mImpl->addModel( model ); }
void          GN::scene::VisualNode::draw( Camera & camera ) const { return mImpl->draw( camera ); }

// *****************************************************************************
// VisualNode factory
// *****************************************************************************

class VisualNodeInternal : public VisualNode
{
public:

    VisualNodeInternal( Entity & entity, VisualGraph & graph ) : VisualNode( entity, graph ) {}

    virtual ~VisualNodeInternal() {}

};

//
//
// -----------------------------------------------------------------------------
VisualNode * GN::scene::newVisualNode( Entity & entity, VisualGraph & graph )
{
    return new VisualNodeInternal( entity, graph );
}
