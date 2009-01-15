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
// public methods of effect class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Effect::init( const EffectDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::Effect, () );

    // store descriptor in member variable (used in method initTech())
    mDesc = desc;

    // create uniform array
    for( std::map<StrA,EffectDesc::UniformDesc>::const_iterator iter = desc.uniforms.begin();
         iter != desc.uniforms.end();
         ++iter )
    {
        const StrA                    & uname = iter->first;
        const EffectDesc::UniformDesc & udesc = iter->second;

        // create GPU program
        GpuProgramParam * gpp;
        if( udesc.shared )
        {
            gpp = createSharedGpuProgramParam( uname, udesc.size );
        }
        else
        {
            gpp = createPrivateGpuProgramParam( udesc.size );
        }
        if( NULL == gpp ) return false;

        // add to uniform array
        mUniforms[uname].attach( gpp );
    }

    // create texture array
    for( std::map<StrA,EffectDesc::TextureDesc>::const_iterator iter = desc.textures.begin();
         iter != desc.textures.end();
         ++iter )
    {
        const StrA & tname = iter->first;
        mTextures[tname]; // add new item in texture array.
    }

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
    int highestQuality = INT_MIN;
    mDefaultActiveTech = NULL;
    for( std::map<StrA,EffectDesc::TechniqueDesc>::const_iterator iter = desc.techniques.begin();
         iter != desc.techniques.end();
         ++iter )
    {
        const StrA                      & name = iter->first;
        const EffectDesc::TechniqueDesc & desc = iter->second;

        Technique & tech = mTechniques[name];
        if( !initTech( tech, name, desc ) )
        {
            mTechniques.erase( name );
            continue; // skip problematic technique
        }

        if( desc.quality > highestQuality )
        {
            highestQuality = desc.quality;
            mDefaultActiveTech = &tech;
        }
    }
    if( mTechniques.empty() )
    {
        GN_ERROR(sLogger)( "No valid technique found." );
        return failure();
    }
    GN_ASSERT( mDefaultActiveTech );
    mActiveTech = mDefaultActiveTech;

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

    mUniforms.clear();
    mTextures.clear();
    mGpuPrograms.clear();
    mTechniques.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuProgramParam *
GN::gfx::Effect::getGpuProgramParam( const StrA & name )
{
    std::map<StrA,AutoRef<GpuProgramParam> >::iterator it = mUniforms.find( name );

    if( mUniforms.end() == it )
    {
        return mDummyUniform;
    }
    else
    {
        return it->second;
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectTextureParameter *
GN::gfx::Effect::getTextureParam( const StrA & name )
{
    std::map<StrA,EffectTextureParameterImpl>::iterator it = mTextures.find( name );

    if( mTextures.end() == it )
    {
        return &mDummyTexture;
    }
    else
    {
        return &it->second;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Effect::setActiveTechnique( const StrA & name )
{
    if( name.empty() )
    {
        GN_ASSERT( mDefaultActiveTech );
        mActiveTech = mDefaultActiveTech;
    }
    else
    {
        std::map<StrA,Technique>::iterator it = mTechniques.find( name );
        if( mTechniques.end() == it )
        {
            GN_ERROR(sLogger)( "invalid technique name: %s", name.cptr() );
        }
        else
        {
            mActiveTech = &it->second;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Effect::applyToDrawable( Drawable & drawable, size_t pass ) const
{
    GN_ASSERT( mActiveTech );

    if( pass >= mActiveTech->passes.size() )
    {
        GN_ERROR(sLogger)( "Invalid pass: %u", pass );
        return false;
    }

    const Pass & p = mActiveTech->passes[pass];

    // setup uniforms
    GN_ASSERT( p.uniforms.size() == p.gpuProgram->getNumParameters() );
    drawable.gpps.resize( p.uniforms.size() );
    for( size_t i = 0; i < p.uniforms.size(); ++i )
    {
        drawable.gpps[i].set( p.uniforms[i] );
    }

    // setup textures
    size_t numtex = std::min<size_t>( p.textures.size(), RendererContext::MAX_TEXTURES );
    for( size_t i = 0; i < numtex; ++i )
    {
        const PerShaderTextureParam & t = p.textures[i];

        drawable.rc.textures[i].set( t.param->getTexture() );
        memcpy( drawable.rc.texbinds[i], t.binding, sizeof(drawable.rc.texbinds[i]) );
    }

    // clear unused texture stages
    for( size_t i = numtex; i < RendererContext::MAX_TEXTURES; ++i )
    {
        drawable.rc.textures[i].clear();
        memset( drawable.rc.texbinds[i], 0, sizeof(drawable.rc.texbinds[i]) );
    }

    GN_TODO( "setup render states, vertex format." );

    // success
    drawable.rndr = &mRenderer;
    drawable.rc.gpuProgram.set( p.gpuProgram );
    return true;
}

// *****************************************************************************
// private methods of effect class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::Effect::initTech(
    Technique                       & tech,
    const StrA                      & name,
    const EffectDesc::TechniqueDesc & desc )
{
    tech.passes.resize( desc.passes.size() );

    // get common render state for the technique
    EffectDesc::RenderStateDesc commonRenderStates;
    sMergeRenderStates( commonRenderStates, desc.rsd, mDesc.rsd );

    // initialize each pass
    for( size_t i = 0; i < tech.passes.size(); ++i )
    {
        const EffectDesc::PassDesc & pdesc = desc.passes[i];

        const StrA & sname = pdesc.shader; // shader name alias for easy referencing

        Pass & p = tech.passes[i];

        // look up GPU program
        p.gpuProgram = sFindNamedPtr( mGpuPrograms, sname );
        if( NULL == p.gpuProgram )
        {
            GN_ERROR(sLogger)(
                "Technique '%s' referencs non-exist or invalid shader '%s' in pass %u",
                name.cptr(),
                sname.cptr(),
                i );
            return false;
        }

        GN_ASSERT( mDesc.shaders.find(sname) != mDesc.shaders.end() );
        const EffectDesc::ShaderDesc & sdesc = mDesc.shaders.find(sname)->second;

        // look up textures
        for( std::map<StrA,StrA>::const_iterator iter = sdesc.textures.begin(); iter != sdesc.textures.end(); ++iter )
        {
            const StrA & tbind = iter->first;
            const StrA & tname = iter->second;

            const EffectDesc::TextureDesc * tdesc = sFindNamedPtr( mDesc.textures, tname );
            if( NULL == tdesc )
            {
                GN_ERROR(sLogger)(
                    "shader '%s' referencs non-exisit texture '%s'.",
                    sname.cptr(), tname.cptr() );
            }
            else if( GpuProgram::PARAMETER_NOT_FOUND == p.gpuProgram->getParameterIndex( tbind.cptr() ) )
            {
                GN_ERROR(sLogger)(
                    "texture '%s' is binded to invalid parameter '%s' of shader '%s'.",
                    tname.cptr(), tbind.cptr(), sname.cptr() );
            }
            // TODO: check GPU parameter type. Make sure it is a texture parameter.
            else
            {
                // this is a valid texture parameter
                GN_ASSERT( mTextures.end() != mTextures.find(tname) );

                PerShaderTextureParam tex;

                tex.param = &mTextures.find(tname)->second;

                memset( tex.binding, 0, sizeof(tex.binding) );
                strcpy_s( tex.binding, tbind.cptr() );

                tex.sampler = &tdesc->sampler;

                p.textures.append( tex );
            }
        }
        if( p.textures.size() > RendererContext::MAX_TEXTURES )
        {
            GN_WARN(sLogger)( "technique %s pass %u has too many textures.", name.cptr(), i );
        }

        // look up uniforms
        p.uniforms.resize( p.gpuProgram->getNumParameters() );
        std::fill( p.uniforms.begin(), p.uniforms.end(), (GpuProgramParam*)NULL );
        for( std::map<StrA,StrA>::const_iterator iter = sdesc.uniforms.begin(); iter != sdesc.uniforms.end(); ++iter )
        {
            const StrA & ubind = iter->first;
            const StrA & uname = iter->second;

            const EffectDesc::UniformDesc * udesc = sFindNamedPtr( mDesc.uniforms, uname );
            if( NULL == udesc )
            {
                GN_ERROR(sLogger)(
                    "shader '%s' is referencing non-exisit uniform '%s'.",
                    sname.cptr(), uname.cptr() );
            }
            else
            {
                size_t uidx = p.gpuProgram->getParameterIndex( ubind.cptr() );

                if( GpuProgram::PARAMETER_NOT_FOUND == uidx )
                {
                    GN_ERROR(sLogger)(
                        "uniform '%s' is binded to invalid parameter '%s' of shader '%s'.",
                        uname.cptr(), ubind.cptr(), sname.cptr() );
                }
                // TODO: check parameter type.
                else
                {
                    // this is a valid uniform parameter
                    GN_ASSERT( uidx < p.uniforms.size() );
                    GN_ASSERT( mUniforms.end() != mUniforms.find( uname ) );

                    p.uniforms[uidx] = mUniforms.find( uname )->second.get();
                }
            }
        }

        // get pass specific render states
        sMergeRenderStates( p.rsd, commonRenderStates, pdesc.rsd );
    }

    GN_TODO( "build vertex format structure" );

    // success
    return true;
}
