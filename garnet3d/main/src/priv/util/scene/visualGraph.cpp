#include "pch.h"
#include "visualGraph.h"
#include "visualNode.h"
#include "lightNode.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util");

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
    for( StandardUniformType type = 0; type < GN_ARRAY_COUNT(mUniforms); ++type )
    {
        AutoRef<UniformResource> & ur = mUniforms[type];

        if( type.desc().global )
        {
            ur = gdb.findOrCreateResource<UniformResource>( type.desc().name );
            AutoRef<Uniform> u( gdb.gpu().createUniform( type.desc().size ) );
            ur->setUniform( u );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::util::VisualGraph::Impl::~Impl()
{
    for( StandardUniformType type = 0; type < GN_ARRAY_COUNT(mUniforms); ++type )
    {
        mUniforms[type].Clear();
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
        GN_ERROR(sLogger)( "Invalid uniform type: %d", type );
        return NULL;
    }

    const StandardUniformDesc & desc = type.desc();
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
void GN::util::VisualGraph::Impl::Draw( Camera & camera )
{
    updateTransformation( camera );
    updateDefaultLighting();

    // updateTiming();

    std::for_each(
        mVisualNodes.begin(),
        mVisualNodes.end(),
        std::mem_fun<void,VisualNode::Impl>( &VisualNode::Impl::Draw ) );
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

    mUniforms[StandardUniformType::MATRIX_PV]->getUniform()->Update( pv );
    mUniforms[StandardUniformType::MATRIX_PV_INV]->getUniform()->Update( ipv );
    mUniforms[StandardUniformType::MATRIX_PV_IT]->getUniform()->Update( itpv );

    mUniforms[StandardUniformType::MATRIX_PROJ]->getUniform()->Update( proj );
    mUniforms[StandardUniformType::MATRIX_PROJ_INV]->getUniform()->Update( ip );
    mUniforms[StandardUniformType::MATRIX_PROJ_IT]->getUniform()->Update( itp );

    mUniforms[StandardUniformType::MATRIX_VIEW]->getUniform()->Update( view );
    mUniforms[StandardUniformType::MATRIX_VIEW_INV]->getUniform()->Update( iv );
    mUniforms[StandardUniformType::MATRIX_VIEW_IT]->getUniform()->Update( itv );
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
    const LightDesc & light0Desc = light0->GetDesc();
    mUniforms[StandardUniformType::LIGHT0_DIFFUSE]->getUniform()->Update( light0Desc.diffuse );
    mUniforms[StandardUniformType::LIGHT0_AMBIENT]->getUniform()->Update( light0Desc.ambient );
    mUniforms[StandardUniformType::LIGHT0_SPECULAR]->getUniform()->Update( light0Desc.specular );

    // update light position and direction
    SpatialNode * sn = light0->owner().entity().getNode<SpatialNode>();
    Vector3f position( 0, 0, 0 );
    Vector3f direction( 0, 0, -1.0f );
    if( sn )
    {
        GN_TODO( "translate light position into world space" );
        position = sn->getPosition();

        GN_TODO( "rotate light direction accordingly" );
        direction.Set( 0, 0, 1.0f );
    }
    mUniforms[StandardUniformType::LIGHT0_POSITION]->getUniform()->Update( position );
    mUniforms[StandardUniformType::LIGHT0_DIRECTION]->getUniform()->Update( direction );
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
GpuResourceDatabase & GN::util::VisualGraph::gdb() const
{
    return mImpl->gdb();
}

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualGraph::Draw( Camera & camera )
{
    mImpl->Draw( camera );
}
