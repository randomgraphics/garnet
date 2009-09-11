#include "pch.h"
#include "visualGraph.h"
#include "visualNode.h"
#include "lightNode.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene");

// *****************************************************************************
// Local stuff
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// VisualGraph::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::VisualGraph::Impl::Impl( VisualGraph & owner, GpuResourceDatabase & gdb )
    : mOwner(owner)
    , mGdb(gdb)
{
    for( StandardUniformType type = 0; type < GN_ARRAY_COUNT(mUniforms); ++type )
    {
        AutoRef<UniformResource> & ur = mUniforms[type];

        if( type.desc().global )
        {
            StrA fullname = strFormat( "GN.scene.visualgraph.stduniform.%s", type.desc().name );

            ur = gdb.findOrCreateResource<UniformResource>( fullname );
            if( ur && !ur->getUniform() )
            {
                AutoRef<Uniform> u( gdb.gpu().createUniform( type.desc().size ) );
                ur->setUniform( u );
            }
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::scene::VisualGraph::Impl::~Impl()
{
    for( StandardUniformType type = 0; type < GN_ARRAY_COUNT(mUniforms); ++type )
    {
        mUniforms[type].clear();
    }
}

//
//
// -----------------------------------------------------------------------------
UniformResource *
GN::scene::VisualGraph::Impl::getGlobalUniform( StandardUniformType type ) const
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
void GN::scene::VisualGraph::Impl::draw( Camera & camera )
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
GN::scene::VisualGraph::Impl::insertVisualNode( VisualNode::Impl * node )
{
    GN_ASSERT( node );
    return mVisualNodes.insert( mVisualNodes.end(), node );
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::VisualGraph::Impl::removeVisualNode( std::list<VisualNode::Impl*>::iterator iter )
{
    mVisualNodes.erase( iter );
}

//
//
// -----------------------------------------------------------------------------
std::list<LightNode::Impl*>::iterator
GN::scene::VisualGraph::Impl::insertLightNode( LightNode::Impl * node )
{
    GN_ASSERT( node );
    return mLightNodes.insert( mLightNodes.end(), node );
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::VisualGraph::Impl::removeLightNode( std::list<LightNode::Impl*>::iterator iter )
{
    mLightNodes.erase( iter );
}

// *****************************************************************************
// VisualGraph::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::VisualGraph::Impl::updateTransformation( Camera & c )
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

    mUniforms[StandardUniformType::MATRIX_PV]->getUniform()->update( pv );
    mUniforms[StandardUniformType::MATRIX_PV_INV]->getUniform()->update( ipv );
    mUniforms[StandardUniformType::MATRIX_PV_IT]->getUniform()->update( itpv );

    mUniforms[StandardUniformType::MATRIX_PROJ]->getUniform()->update( proj );
    mUniforms[StandardUniformType::MATRIX_PROJ_INV]->getUniform()->update( ip );
    mUniforms[StandardUniformType::MATRIX_PROJ_IT]->getUniform()->update( itp );

    mUniforms[StandardUniformType::MATRIX_VIEW]->getUniform()->update( view );
    mUniforms[StandardUniformType::MATRIX_VIEW_INV]->getUniform()->update( iv );
    mUniforms[StandardUniformType::MATRIX_VIEW_IT]->getUniform()->update( itv );
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::VisualGraph::Impl::updateDefaultLighting()
{
    if( mLightNodes.empty() )
    {
        GN_WARN(sLogger)( "There's no light defined in the visual graph." );
    }

    // get the first light as default light
    LightNode::Impl * light0 = mLightNodes.front();

    // update light color
    const LightDesc & light0Desc = light0->getDesc();
    mUniforms[StandardUniformType::LIGHT0_DIFFUSE]->getUniform()->update( light0Desc.diffuse );
    mUniforms[StandardUniformType::LIGHT0_AMBIENT]->getUniform()->update( light0Desc.ambient );
    mUniforms[StandardUniformType::LIGHT0_SPECULAR]->getUniform()->update( light0Desc.specular );

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
    mUniforms[StandardUniformType::LIGHT0_POSITION]->getUniform()->update( position );
    mUniforms[StandardUniformType::LIGHT0_DIRECTION]->getUniform()->update( direction );
}

// *****************************************************************************
// VisualGraph public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::VisualGraph::VisualGraph( GpuResourceDatabase & gdb )
{
    mImpl = new Impl( *this, gdb );
}

//
//
// -----------------------------------------------------------------------------
GN::scene::VisualGraph::~VisualGraph()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase & GN::scene::VisualGraph::gdb() const
{
    return mImpl->gdb();
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::VisualGraph::draw( Camera & camera )
{
    mImpl->draw( camera );
}
