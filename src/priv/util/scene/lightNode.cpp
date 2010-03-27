#include "pch.h"
#include "lightNode.h"
#include "visualGraph.h"

using namespace GN;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util");

// *****************************************************************************
// LightNode::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::LightNode::Impl::Impl( LightNode & owner, VisualGraph & graph )
    : mOwner(owner)
    , mGraph(graph)
{
    mGraphIter = mGraph.impl().insertLightNode( this );
}

//
//
// -----------------------------------------------------------------------------
GN::util::LightNode::Impl::~Impl()
{
    mGraph.impl().removeLightNode( mGraphIter );
}

// *****************************************************************************
// LightNode::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// LightNode public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::LightNode::LightNode( Entity & entity, VisualGraph & graph )
    : NodeBase(entity)
    , mImpl(NULL)
{
    mImpl = new Impl( *this, graph );
}

//
//
// -----------------------------------------------------------------------------
GN::util::LightNode::~LightNode()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::util::LightNode::GetGuid()
{
    static const Guid MY_GUID = { 0x5cd50f8d, 0x69fc, 0x4836, { 0xb3, 0x28, 0xea, 0x60, 0xef, 0xcb, 0x21, 0xa8 } };
    return MY_GUID;
}

//
//
// -----------------------------------------------------------------------------
const LightDesc & GN::util::LightNode::GetDesc() const { return mImpl->GetDesc(); }
void              GN::util::LightNode::SetDesc( const LightDesc & desc ) { return mImpl->SetDesc( desc ); }

// *****************************************************************************
// LightNode factory
// *****************************************************************************

class LightNodeInternal : public LightNode
{
public:

    LightNodeInternal( Entity & entity, VisualGraph & graph ) : LightNode( entity, graph ) {}

    virtual ~LightNodeInternal() {}

};

//
//
// -----------------------------------------------------------------------------
LightNode * GN::util::newLightNode( Entity & entity, VisualGraph & graph )
{
    return new LightNodeInternal( entity, graph );
}
