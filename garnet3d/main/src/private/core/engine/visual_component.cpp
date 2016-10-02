#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::GetLogger("GN.engine");

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
static const GN::Guid VISUAL_COMPONENT_GUID =
{
    0x910f5f26, 0x8183, 0x4186, { 0x81, 0x32, 0xbf, 0x19, 0xc5, 0x8b, 0xd8, 0x6b }
};
GN_ENGINE_IMPLEMENT_ENTITY( GN::engine::VisualComponent, VISUAL_COMPONENT_GUID );

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
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::VisualComponent::addModel( ModelResource * model )
{
    if( NULL == model )
    {
        GN_ERROR(sLogger)( "fail to attach model to visual node: NULL model pointer." );
        return false;
    }

    GN_ASSERT( ModelResource::guid() == model->type() );

    GpuResourceDatabase * gdb = getGdb();
    GN_ASSERT( gdb );

    // handle standard uniforms
    AutoRef<EffectResource> effect = model->effectResource();
	if( NULL == effect )
	{
        GN_ERROR(sLogger)( "fail to attach model to visual node: No effect attached to the model." );
		return false;
	}
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
    if( !mModels.resize( mModels.size() + 1 ) )
    {
        GN_ERROR(sLogger)( "Out of memory." );
        return false;
    }
    mModels.back().set( model );

    // done
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::VisualComponent::clear()
{
    mModels.clear();
    for( size_t i = 0; i < StandardUniform::Index::NUM_STANDARD_UNIFORMS; ++i )
    {
        mStandardPerObjectUniforms[i].clear();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::VisualComponent::draw( const SpacialComponent * sc ) const
{
    // update world transformation.
    if( sc ) updateWorldTransform( sc->getLocal2Root() );

    // draw models
    GN_GPU_DEBUG_MARK_BEGIN( getGpu(), "VisualComponent::draw" );

    for( uint32 i = 0; i < mModels.size(); ++i )
    {
        drawModelResource( i, *mModels[i] );
    }
    GN_GPU_DEBUG_MARK_END( getGpu() );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::VisualComponent::drawModel( size_t modelIndex, const SpacialComponent * sc ) const
{
    if( modelIndex >= mModels.size() ) return;

    if( sc ) updateWorldTransform( sc->getLocal2Root() );

    GN_GPU_DEBUG_MARK_BEGIN( getGpu(), "VisualComponent::drawModel" );
    drawModelResource( (uint32)modelIndex, *mModels[(uint32)modelIndex] );
    GN_GPU_DEBUG_MARK_END( getGpu() );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::VisualComponent::updateWorldTransform( const Matrix44f & world ) const
{
    GpuResourceDatabase * gdb = getGdb();
    GN_ASSERT( gdb );

    const Matrix44f pv = *(const Matrix44f *)gdb->getStandardUniformResource(StandardUniform::Index::MATRIX_PV)->uniform()->getval();

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
