#include "pch.h"
#include "visualNode.h"
#include "visualGraph.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.util");

// *****************************************************************************
// VisualNode::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::VisualNode::Impl::Impl( VisualNode & owner, VisualGraph & graph )
    : mOwner(owner)
    , mGraph(graph)
{
    mGraphIter = mGraph.impl().insertVisualNode( this );
}

//
//
// -----------------------------------------------------------------------------
GN::util::VisualNode::Impl::~Impl()
{
    removeAllModels();

    for( size_t i = 0; i < StandardUniformType::NUM_STANDARD_UNIFORMS; ++i )
    {
        mStandardPerObjectUniforms[i].clear();
    }

    mGraph.impl().removeVisualNode( mGraphIter );
}

//
//
// -----------------------------------------------------------------------------
int GN::util::VisualNode::Impl::addModel( GpuResource * model )
{
    if( NULL == model )
    {
        GN_ERROR(sLogger)( "fail to attach model to visual node: NULL model pointer." );
        return 0;
    }

    if( ModelResource::guid() != model->type() )
    {
        GN_ERROR(sLogger)( "fail to attach model to visual node: the resource is not a model." );
        return 0;
    }

    GpuResourceDatabase & gdb = model->getGdb();

    if( &gdb != &mGraph.getGdb() )
    {
        GN_ERROR(sLogger)( "fail to attach model to visual node: the model and the node belong to different GPU resource database." );
        return 0;
    }

    // handle standard uniforms
    ModelResource * m = GpuResource::castTo<ModelResource>( model );
    AutoRef<EffectResource> effect = m->effectResource();
    for( StandardUniformType type = 0; type < StandardUniformType::NUM_STANDARD_UNIFORMS; ++type )
    {
        const StandardUniformDesc & d = type.getDesc();

        if( effect->hasUniform( d.name ) )
        {
            if( d.global )
            {
                m->setUniformResource( d.name, mGraph.impl().getGlobalUniform( type ) );
            }
            else
            {
                m->setUniformResource( d.name, getPerObjectUniform( type ) );
            }
        }
    }

    // add the model into model list
    int h = mModels.newHandle();
    if( 0 != h ) mModels[h].set( GpuResource::castTo<ModelResource>( model ) );

    // done
    return h;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualNode::Impl::removeAllModels()
{
    mModels.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::util::VisualNode::Impl::draw() const
{
    // update standard transfomration uniforms
    SpatialNode * sn = mOwner.entity().getNode<SpatialNode>();
    if( sn )
    {
        const Matrix44f pv = *(const Matrix44f *)mGraph.impl().getGlobalUniform(StandardUniformType::MATRIX_PV)->uniform()->getval();

        const Matrix44f & world = sn->getLocal2Root();

        Matrix44f pvw = pv * world;

        for( StandardUniformType type = 0; (size_t)type < mStandardPerObjectUniforms.size(); ++type )
        {
            UniformResource * ur = mStandardPerObjectUniforms[type];
            if( NULL == ur ) continue;

            // this should be per-object parameter
            GN_ASSERT( !type.getDesc().global );

            AutoRef<Uniform> u = ur->uniform();
            if( NULL == u ) continue;

            switch( type )
            {
                case StandardUniformType::MATRIX_PVW :
                    u->update( pvw );
                    break;

                case StandardUniformType::MATRIX_PVW_INV:
                    u->update( Matrix44f::sInverse( pvw ) );
                    break;

                case StandardUniformType::MATRIX_PVW_IT:
                    u->update( Matrix44f::sInvtrans( pvw ) );
                    break;

                case StandardUniformType::MATRIX_WORLD :
                    u->update( world );
                    break;

                case StandardUniformType::MATRIX_WORLD_INV:
                    u->update( Matrix44f::sInverse(world) );
                    break;

                case StandardUniformType::MATRIX_WORLD_IT:
                    u->update( Matrix44f::sInvtrans(world) );
                    break;

                default:
                    // do nothing
                    break;
            }
        }
    }

    // draw models
    for( int i = mModels.first(); i != 0; i = mModels.next( i ) )
    {
        ModelResource * m = mModels[i];

        m->draw();
    }
}

// *****************************************************************************
// VisualNode::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
UniformResource *
GN::util::VisualNode::Impl::getPerObjectUniform( StandardUniformType type )
{
    AutoRef<UniformResource> & ur = mStandardPerObjectUniforms[type];

    StrA fullname = stringFormat( "GN.scene.visualnode.stduniform.%s", type.name() );

    GpuResourceDatabase & gdb = mGraph.getGdb();

    ur = gdb.findOrCreateResource<UniformResource>( fullname );

    if( !ur->uniform() )
    {
        AutoRef<Uniform> u( gdb.getGpu().createUniform( type.getDesc().size ) );
        ur->setUniform( u );
    }

    return ur;
}

// *****************************************************************************
// VisualNode public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::VisualNode::VisualNode( Entity & entity, VisualGraph & graph )
    : NodeBase(entity)
    , mImpl(NULL)
{
    mImpl = new Impl( *this, graph );
}

//
//
// -----------------------------------------------------------------------------
GN::util::VisualNode::~VisualNode()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::util::VisualNode::guid()
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
VisualGraph & GN::util::VisualNode::graph() const { return mImpl->graph(); }
int           GN::util::VisualNode::addModel( gfx::GpuResource * model ) { return mImpl->addModel( model ); }
void          GN::util::VisualNode::removeAllModels() { return mImpl->removeAllModels(); }
bool          GN::util::VisualNode::loadModelsFromFile( const char * filename ) { return mImpl->loadModelsFromFile( filename ); }

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
VisualNode * GN::util::newVisualNode( Entity & entity, VisualGraph & graph )
{
    return new VisualNodeInternal( entity, graph );
}
