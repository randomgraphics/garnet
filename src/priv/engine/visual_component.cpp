#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine");

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
    for( size_t i = 0; i < StandardUniformType::NUM_STANDARD_UNIFORMS; ++i )
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

    StandardUniformManager * sum = getStandardUniformManager();
    GN_ASSERT( sum );

    // handle standard uniforms
    AutoRef<EffectResource> effect = model->effectResource();
    for( StandardUniformType type = 0; type < StandardUniformType::NUM_STANDARD_UNIFORMS; ++type )
    {
        const StandardUniformDesc & d = type.getDesc();

        if( effect->hasUniform( d.name ) )
        {
            if( d.global )
            {
                model->setUniformResource( d.name, sum->getGlobalUniformResource( type ) );
            }
            else
            {
                model->setUniformResource( d.name, getPerObjectUniform( type ) );
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
        StandardUniformManager * sum = getStandardUniformManager();
        GN_ASSERT( sum );

        const Matrix44f pv = *(const Matrix44f *)sum->getGlobalUniformResource(StandardUniformType::MATRIX_PV)->uniform()->getval();

        const Matrix44f & world = sc->getLocal2Root();

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
// VisualComponent private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::UniformResource *
GN::engine::VisualComponent::getPerObjectUniform( StandardUniformType type )
{
    AutoRef<UniformResource> & ur = mStandardPerObjectUniforms[type];

    StrA fullname = stringFormat( "GN.engine.visualcomponent.stduniform.%s", type.name() );

    GpuResourceDatabase * gdb = getGdb();

    ur = gdb->findOrCreateResource<UniformResource>( fullname );

    if( !ur->uniform() )
    {
        AutoRef<Uniform> u( gdb->getGpu().createUniform( type.getDesc().size ) );
        ur->setUniform( u );
    }

    return ur;
}

