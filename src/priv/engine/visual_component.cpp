#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline bool sIsWorldTransform( StandardUniform::Index i )
{
    return StandardUniform::Index::MATRIX_PVW == i
        || StandardUniform::Index::MATRIX_PVW_INV == i
        || StandardUniform::Index::MATRIX_PVW_IT == i
        || StandardUniform::Index::MATRIX_WORLD == i
        || StandardUniform::Index::MATRIX_WORLD_INV == i
        || StandardUniform::Index::MATRIX_WORLD_IT == i;
}

// *****************************************************************************
// VisualComponent public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::Guid & GN::engine::VisualComponent::sGetType()
{
    static const GN::Guid MY_GUID =
    {
        0x910f5f26, 0x8183, 0x4186, { 0x81, 0x32, 0xbf, 0x19, 0xc5, 0x8b, 0xd8, 0x6b }
    };

    return MY_GUID;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::VisualComponent::VisualComponent()
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::VisualComponent::~VisualComponent()
{
    clear();
    for( size_t i = 0; i < StandardUniform::Index::NUM_STANDARD_UNIFORMS; ++i )
    {
        mStandardPerObjectUniforms[i].clear();
    }
}

//
//
// -----------------------------------------------------------------------------
int GN::engine::VisualComponent::addModel( ModelResource * model )
{
    if( NULL == model )
    {
        GN_ERROR(sLogger)( "fail to attach model to visual node: NULL model pointer." );
        return 0;
    }

    GN_ASSERT( ModelResource::guid() == model->type() );

    GpuResourceDatabase * gdb = getGdb();
    Gpu * gpu = getGpu();
    GN_ASSERT( gdb && gpu );

    // handle standard uniforms
    AutoRef<EffectResource> effect = model->effectResource();
    for( StandardUniform::Index type = 0; type < StandardUniform::Index::NUM_STANDARD_UNIFORMS; ++type )
    {
        const StandardUniform::Desc * d = StandardUniform::sIndex2Desc( type );

        if( effect->hasUniform( d->name ) )
        {
            AutoRef<UniformResource> ur;

            if( d->global )
            {
                ur = gdb->getStandardUniformResource( type );
                model->setUniformResource( d->name, ur );
            }
            else if( sIsWorldTransform( type ) )
            {
                ur = gdb->getStandardUniformResource( type );
                model->setUniformResource( d->name, ur );
                mStandardPerObjectUniforms[type] = ur;
            }
        }
    }

    // add the model into model list
    int h = mModels.newHandle();
    if( 0 != h ) mModels[h].set( model );

    // done
    return h;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::VisualComponent::clear()
{
    mModels.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::VisualComponent::draw() const
{
    // update standard transformation based on spacial information
    Entity * e = getEntity();
    SpacialComponent * sc = e ? e->getComponent<SpacialComponent>() : NULL;
    if( sc )
    {
        GpuResourceDatabase * gdb = getGdb();
        GN_ASSERT( gdb );

        const Matrix44f pv = *(const Matrix44f *)gdb->getStandardUniformResource(StandardUniform::Index::MATRIX_PV)->uniform()->getval();

        const Matrix44f & world = sc->getLocal2Root();

        Matrix44f pvw = pv * world;

        for( int index = 0; (size_t)index < mStandardPerObjectUniforms.size(); ++index )
        {
            UniformResource * ur = mStandardPerObjectUniforms[index];
            if( NULL == ur ) continue;

            // this should be per-object parameter
            GN_ASSERT( !StandardUniform::sIndex2Desc( index )->global );

            AutoRef<Uniform> u = ur->uniform();
            if( NULL == u ) continue;

            switch( index )
            {
                case StandardUniform::Index::MATRIX_PVW :
                    u->update( pvw );
                    break;

                case StandardUniform::Index::MATRIX_PVW_INV:
                    u->update( Matrix44f::sInverse( pvw ) );
                    break;

                case StandardUniform::Index::MATRIX_PVW_IT:
                    u->update( Matrix44f::sInvtrans( pvw ) );
                    break;

                case StandardUniform::Index::MATRIX_WORLD :
                    u->update( world );
                    break;

                case StandardUniform::Index::MATRIX_WORLD_INV:
                    u->update( Matrix44f::sInverse(world) );
                    break;

                case StandardUniform::Index::MATRIX_WORLD_IT:
                    u->update( Matrix44f::sInvtrans(world) );
                    break;

                default:
                    // should never be here.
                    GN_UNEXPECTED();
                    break;
            }
        }
    }

    // draw models
    GN_GPU_DEBUG_MARK_BEGIN( getGpu(), "VisualComponent::draw" );
    for( int i = mModels.first(); i != 0; i = mModels.next( i ) )
    {
        ModelResource * m = mModels[i];

        m->draw();
    }
    GN_GPU_DEBUG_MARK_END( getGpu() );
}

