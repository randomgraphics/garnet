#include "pch.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.Effect");

// *****************************************************************************
// local function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline T *
sFindNamedPtr( const std::map<StrA,AutoRef<T> > & container, const StrA & name )
{
    std::map<StrA,AutoRef<T> >::const_iterator iter = container.find( name );
    return ( container.end() == iter ) ? NULL : iter->second.get();
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline const T *
sFindNamedPtr( const std::map<StrA,T> & container, const StrA & name )
{
    std::map<StrA,T>::const_iterator iter = container.find( name );
    return ( container.end() == iter ) ? NULL : &iter->second;
}

//
// merge render states 'a' and 'b'.
// -----------------------------------------------------------------------------
static void
sMergeRenderStates(
    GN::gfx::EffectDesc::RenderStateDesc       & out,
    const GN::gfx::EffectDesc::RenderStateDesc & a,
    const GN::gfx::EffectDesc::RenderStateDesc & b )
{
    GN_UNUSED_PARAM( out );
    GN_UNUSED_PARAM( a );
    GN_UNUSED_PARAM( b );
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// EffectDesc class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectDesc::ShaderPrerequisites::check( Renderer & ) const
{
    GN_UNIMPL_WARNING();
    return true;
}

// *****************************************************************************
// Effect class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Effect::init( const EffectDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::Effect, () );

    mDesc = desc;

    // create GPU program array
    for( std::map<StrA,EffectDesc::ShaderDesc>::const_iterator iter = desc.shaders.begin();
         iter != desc.shaders.end();
         ++iter )
    {
        const StrA                   & sname = iter->first;
        const EffectDesc::ShaderDesc & sdesc = iter->second;

        // check shader requirements.
        if( !sdesc.prerequisites.check( mRenderer ) )
        {
            GN_VERBOSE(sLogger)( "shader '%s' is skipped due to missing renderer caps." );
            continue;
        }

        // create GPU program
        GpuProgram * gp = mRenderer.createGpuProgram( sdesc.gpd );
        if( NULL == gp )
        {
            // ignore problematic shader.
            continue;
        }

        mGpuPrograms[sname].attach( gp );
    }

    // create techniques
    if( desc.techniques.empty() )
    {
        GN_ERROR(sLogger)( "Effect descriptor must define at least one techniuqe." );
        return failure();
    }
    for( std::map<StrA,EffectDesc::TechniqueDesc>::const_iterator iter = desc.techniques.begin();
         iter != desc.techniques.end();
         ++iter )
    {
        const StrA                      & tname = iter->first;
        const EffectDesc::TechniqueDesc & tdesc = iter->second;

        Technique t;

        t.passes.resize( tdesc.passes.size() );

        // get common render state for the technique
        EffectDesc::RenderStateDesc techrsd;
        sMergeRenderStates( techrsd, tdesc.rsd, desc.rsd );

        // initialize each pass
        bool good = true;
        for( size_t i = 0; i < t.passes.size(); ++i )
        {
            const EffectDesc::PassDesc & pdesc = tdesc.passes[i];

            Pass & p = t.passes[i];

            // look up GPU program
            p.gpuProgram = sFindNamedPtr( mGpuPrograms, pdesc.shader );
            if( NULL == p.gpuProgram )
            {
                GN_ERROR(sLogger)(
                    "Technique '%s' is referencing non-exist or invalid shader '%s' in pass %u",
                    tname.cptr(),
                    pdesc.shader.cptr(),
                    i );
                good = false;
                break;
            }

            GN_ASSERT( desc.shaders.find(pdesc.shader) != desc.shaders.end() );
            const EffectDesc::ShaderDesc & sdesc = desc.shaders.find(pdesc.shader)->second;

            // look up textures
            for( std::map<StrA,StrA>::const_iterator iter = sdesc.textures.begin(); iter != sdesc.textures.end(); ++iter )
            {
                const StrA & texbind = iter->first;
                const StrA & texname = iter->second;

                const EffectDesc::TextureDesc * tdesc = sFindNamedPtr( desc.textures, texname );
                if( NULL == tdesc )
                {
                    GN_ERROR(sLogger)(
                        "shader '%s' is referencing non-exisit texture '%s'.",
                        pdesc.shader.cptr(), texname.cptr() );
                }
                else if( GpuProgram::PARAMETER_NOT_FOUND == p.gpuProgram->getParameterIndex( texbind.cptr() ) )
                {
                    GN_ERROR(sLogger)(
                        "texture '%s' is binded to invalid parameter '%s' of shader '%s'.",
                        texname.cptr(), texbind.cptr(),
                        pdesc.shader.cptr() );
                }
                else
                {
                    // TODO: check GPU parameter type. Make sure it is a texture parameter.

                    // this is a valid texture parameter
                    p.textures.append( texname );
                }
            }

            // look up uniforms
            p.uniforms.resize( p.gpuProgram->getNumParameters() );
            for( std::map<StrA,StrA>::const_iterator iter = sdesc.uniforms.begin(); iter != sdesc.uniforms.end(); ++iter )
            {
                const StrA & unibind = iter->first;
                const StrA & uniname = iter->second;

                const EffectDesc::UniformDesc * udesc = sFindNamedPtr( desc.uniforms, uniname );
                if( NULL == udesc )
                {
                    GN_ERROR(sLogger)(
                        "shader '%s' is referencing non-exisit uniform '%s'.",
                        pdesc.shader.cptr(), uniname.cptr() );
                }
                else
                {
                    size_t uidx = p.gpuProgram->getParameterIndex( unibind.cptr() );

                    if( GpuProgram::PARAMETER_NOT_FOUND == uidx )
                    {
                        GN_ERROR(sLogger)(
                            "uniform '%s' is binded to invalid parameter '%s' of shader '%s'.",
                            uniname.cptr(), unibind.cptr(),
                            pdesc.shader.cptr() );
                    }
                    else
                    {
                        // TODO: check parameter type.

                        // this is a valid uniform parameter
                        p.uniforms[uidx] = uniname;
                    }
                }
            }

            // get pass specific render states
            sMergeRenderStates( p.rsd, techrsd, pdesc.rsd );
        }
        if( !good ) continue; // skip wrong technique

        GN_TODO( "build vertex format structure" );

        // insert to technique array
        mTechniques[tname] = t;
    }
    if( mTechniques.empty() )
    {
        GN_ERROR(sLogger)( "No valid technique found." );
        return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Effect::quit()
{
    GN_GUARD;

    mGpuPrograms.clear();
    mTechniques.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::Effect::setupDrawable( Drawable & drawable, const StrA & tech, size_t pass ) const
{
    const Technique * t;
    if( tech.empty() )
    {
        t = &mTechniques.begin()->second;
    }
    else
    {
        std::map<StrA,Technique>::const_iterator iter = mTechniques.find( tech );
        if( iter == mTechniques.end() )
        {
            GN_ERROR(sLogger)( "invalid technique name '%s'.", tech.cptr() );
            return false;
        }
        t = &iter->second;
    }
    GN_ASSERT( t );

    if( pass >= t->passes.size() )
    {
        GN_ERROR(sLogger)( "Invalid pass: %u.", pass );
        return false;
    }

    const Pass & p = t->passes[pass];

    // setup uniforms
    std::map<StrA,GpuProgramParam*> uniforms; // this is used to handle multiple GPU program parameters refering to single effect uniform.
    GN_ASSERT( p.uniforms.size() == p.gpuProgram->getNumParameters() );
    drawable.gpps.resize( p.uniforms.size() );
    for( size_t i = 0; i < p.uniforms.size(); ++i )
    {
        const StrA & uname = p.uniforms[i];
        if( uname.empty() ) continue;

        const StrA                              & ubind = p.gpuProgram->getParameterDesc( i ).name;
        const EffectDesc::UniformDesc           & udesc = mDesc.uniforms.find( uname )->second;
        AutoRef<GpuProgramParam>                & u     = drawable.gpps[i];
        std::map<StrA,GpuProgramParam*>::iterator uiter = uniforms.find( ubind );

        if( uiter != uniforms.end() )
        {
            // using existing uniform
            u.set( uiter->second );
        }
        else
        {
            // create uniform
            if( udesc.shared )
            {
                u.attach( createSharedGpuProgramParam( uname, udesc.size ) );
            }
            else
            {
                u.attach( createPrivateGpuProgramParam( udesc.size ) );
            }
            if( NULL == u ) return false;

            // setup default value of the uniform
            if( !udesc.defval.empty() )
            {
                u->set( udesc.defval.cptr(), udesc.defval.size() );
            }
        }
    }

    GN_TODO( "setup render states, vertex format, texture bindings." );

    // success
    drawable.rndr = &mRenderer;
    drawable.rc.gpuProgram.set( p.gpuProgram );
    return true;
}
