#include "pch.h"
#include "visualGraph.h"
#include "visualNode.h"
#include "lightNode.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.util");

// *****************************************************************************
// VisualGraph::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::VisualGraph::Impl::Impl( VisualGraph & owner, GpuResourceDatabase & gdb )
    : mOwner(owner)
    , mGdb(gdb)
{
    // initialize standard global uniforms
    for( StandardUniformType type = 0; (size_t)type < GN_ARRAY_COUNT(mUniforms); ++type )
    {
        AutoRef<UniformResource> & ur = mUniforms[type];

        if( type.getDesc().global )
        {
            ur = gdb.findOrCreateResource<UniformResource>( type.getDesc().name );
            AutoRef<Uniform> u( gdb.getGpu().createUniform( type.getDesc().size ) );
            ur->setUniform( u );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::util::VisualGraph::Impl::~Impl()
{
    for( StandardUniformType type = 0; (size_t)type < GN_ARRAY_COUNT(mUniforms); ++type )
    {
        mUniforms[type].clear();
    }
}

//
//
// -----------------------------------------------------------------------------
UniformResource *
GN::util::VisualGraph::Impl::getGlobalUniform( StandardUniformType type ) const
{
    if( type >= StandardUniformType::NUM_STANDARD_UNIFORMS )
    {
        GN_ERROR(sLogger)( "Invalid uniform type: %d", (StandardUniformType::ENUM)type );
        return NULL;
    }

    const StandardUniformDesc & desc = type.getDesc();
    if( !desc.global )
    {
        GN_ERROR(sLogger)( "Non-global parameter \"%s\" is not accessible through this function.", desc.name );
        return NULL;
    }

    return mUniforms[type];
}

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualGraph::Impl::draw( Camera & camera )
{
    updateTransformation( camera );
    updateDefaultLighting();

    // updateTiming();

    std::for_each(
        mVisualNodes.begin(),
        mVisualNodes.end(),
        std::mem_fun<void,VisualNode::Impl>( &VisualNode::Impl::draw ) );
}

//
//
// -----------------------------------------------------------------------------std:list<VisualNode>::iterator
std::list<VisualNode::Impl*>::iterator
GN::util::VisualGraph::Impl::insertVisualNode( VisualNode::Impl * node )
{
    GN_ASSERT( node );
    return mVisualNodes.insert( mVisualNodes.end(), node );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualGraph::Impl::removeVisualNode( std::list<VisualNode::Impl*>::iterator iter )
{
    mVisualNodes.erase( iter );
}

//
//
// -----------------------------------------------------------------------------
std::list<LightNode::Impl*>::iterator
GN::util::VisualGraph::Impl::insertLightNode( LightNode::Impl * node )
{
    GN_ASSERT( node );
    return mLightNodes.insert( mLightNodes.end(), node );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualGraph::Impl::removeLightNode( std::list<LightNode::Impl*>::iterator iter )
{
    mLightNodes.erase( iter );
}

// *****************************************************************************
// VisualGraph::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualGraph::Impl::updateTransformation( Camera & c )
{
    const Matrix44f & proj = c.getProjectionMatrix();
    const Matrix44f & view = c.getViewMatrix();

    Matrix44f pv   = proj * view;
    Matrix44f ipv  = Matrix44f::sInverse( pv );
    Matrix44f itpv = Matrix44f::sInverse( Matrix44f::sTranspose( pv ) );

    Matrix44f ip  = Matrix44f::sInverse( proj );
    Matrix44f itp = Matrix44f::sInverse( Matrix44f::sTranspose( proj ) );

    Matrix44f iv  = Matrix44f::sInverse( view );
    Matrix44f itv = Matrix44f::sInverse( Matrix44f::sTranspose( view ) );

    mUniforms[StandardUniformType::MATRIX_PV]->uniform()->update( pv );
    mUniforms[StandardUniformType::MATRIX_PV_INV]->uniform()->update( ipv );
    mUniforms[StandardUniformType::MATRIX_PV_IT]->uniform()->update( itpv );

    mUniforms[StandardUniformType::MATRIX_PROJ]->uniform()->update( proj );
    mUniforms[StandardUniformType::MATRIX_PROJ_INV]->uniform()->update( ip );
    mUniforms[StandardUniformType::MATRIX_PROJ_IT]->uniform()->update( itp );

    mUniforms[StandardUniformType::MATRIX_VIEW]->uniform()->update( view );
    mUniforms[StandardUniformType::MATRIX_VIEW_INV]->uniform()->update( iv );
    mUniforms[StandardUniformType::MATRIX_VIEW_IT]->uniform()->update( itv );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualGraph::Impl::updateDefaultLighting()
{
    if( mLightNodes.empty() )
    {
        GN_WARN(sLogger)( "There's no light defined in the visual graph." );
    }

    // get the first light as default light
    LightNode::Impl * light0 = mLightNodes.front();

    // update light color
    const LightDesc & light0Desc = light0->getDesc();
    mUniforms[StandardUniformType::LIGHT0_DIFFUSE]->uniform()->update( light0Desc.diffuse );
    mUniforms[StandardUniformType::LIGHT0_AMBIENT]->uniform()->update( light0Desc.ambient );
    mUniforms[StandardUniformType::LIGHT0_SPECULAR]->uniform()->update( light0Desc.specular );

    // update light position and direction
    SpatialNode * sn = light0->owner().entity().getNode<SpatialNode>();
    Vector3f position( 0, 0, 0 );
    Vector3f direction( 0, 0, -1.0f );
    if( sn )
    {
        GN_TODO( "translate light position into world space" );
        position = sn->getPosition();

        GN_TODO( "rotate light direction accordingly" );
        direction.set( 0, 0, 1.0f );
    }
    mUniforms[StandardUniformType::LIGHT0_POSITION]->uniform()->update( position );
    mUniforms[StandardUniformType::LIGHT0_DIRECTION]->uniform()->update( direction );
}

// *****************************************************************************
// VisualGraph public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::VisualGraph::VisualGraph( GpuResourceDatabase & gdb )
{
    mImpl = new Impl( *this, gdb );
}

//
//
// -----------------------------------------------------------------------------
GN::util::VisualGraph::~VisualGraph()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase & GN::util::VisualGraph::getGdb() const
{
    return mImpl->getGdb();
}

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualGraph::draw( Camera & camera )
{
    mImpl->draw( camera );
}
