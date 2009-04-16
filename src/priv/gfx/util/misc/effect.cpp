#include "pch.h"

using namespace GN;
using namespace GN::gfx;

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
    typename std::map<StrA,AutoRef<T> >::const_iterator iter = container.find( name );
    return ( container.end() == iter ) ? NULL : iter->second.get();
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline const T *
sFindNamedPtr( const std::map<StrA,T> & container, const StrA & name )
{
    typename std::map<StrA,T>::const_iterator iter = container.find( name );
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
bool GN::gfx::EffectDesc::ShaderPrerequisites::check( Renderer & r ) const
{
    const RendererCaps & caps = r.getCaps();

    if( (UInt32)vsProfile != (caps.vsProfiles & vsProfile) )
    {
        return false;
    }

    if( (UInt32)gsProfile != (caps.gsProfiles & gsProfile) )
    {
        return false;
    }

    if( (UInt32)psProfile != (caps.psProfiles & psProfile) )
    {
        return false;
    }

    if( caps.maxTextures < numTextures )
    {
        return false;
    }

    return true;
}

// *****************************************************************************
// public methods of effect class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::Effect::Effect( Renderer & r )
    : mRenderer(r)
    , mDummyUniform( r.createUniform( 1 ) )
{
    uniforms.mMap = &mUniforms;
    uniforms.mGetDummyFunc = makeDelegate(this,&Effect::getDummyUniform);

    textures.mMap = &mTextures;
    textures.mGetDummyFunc = makeDelegate(this,&Effect::getDummyTexture);
    clear();
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Effect::Effect( const Effect & e )
    : mRenderer( e.mRenderer )
    , mDummyUniform( e.mDummyUniform )
    , mDummyTexture( e.mDummyTexture )
{
    GN_ASSERT( this != &e ); // can't copy construct from itself.

    uniforms.mMap = &mUniforms;
    uniforms.mGetDummyFunc = makeDelegate(this,&Effect::getDummyUniform);

    textures.mMap = &mTextures;
    textures.mGetDummyFunc = makeDelegate(this,&Effect::getDummyTexture);

    clear();
    clone( e );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Effect::~Effect()
{
    quit();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Effect::init( const EffectDesc & desc, const StrA & activeTechName )
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
        const StrA                    & uniformName = iter->first;
        const EffectDesc::UniformDesc & udesc = iter->second;

        // create GPU program
        Uniform * u = mRenderer.createUniform( udesc.size );
        if( NULL == u ) return false;

        // add to uniform array
        mUniforms[uniformName].attach( u );
    }

    // create texture array
    for( std::map<StrA,EffectDesc::TextureDesc>::const_iterator iter = desc.textures.begin();
         iter != desc.textures.end();
         ++iter )
    {
        const StrA & texName = iter->first;

        // add new item, with NULL texture pointer, in texture array.
        mTextures[texName];
    }

    // create GPU program array
    for( std::map<StrA,EffectDesc::ShaderDesc>::const_iterator iter = desc.shaders.begin();
         iter != desc.shaders.end();
         ++iter )
    {
        const StrA                   & shaderName = iter->first;
        const EffectDesc::ShaderDesc & shaderDesc = iter->second;

        // check shader requirements.
        if( !shaderDesc.prerequisites.check( mRenderer ) )
        {
            GN_VERBOSE(sLogger)( "shader '%s' is skipped due to missing renderer caps." );
            continue;
        }

        // create GPU program
        GpuProgram * gp = mRenderer.createGpuProgram( shaderDesc.gpd );
        if( NULL == gp )
        {
            // ignore problematic shader.
            continue;
        }

        mGpuPrograms[shaderName].attach( gp );
    }

    // create techniques
    if( desc.techniques.empty() )
    {
        GN_ERROR(sLogger)( "Effect descriptor must define at least one techniuqe." );
        return failure();
    }
    int highestQuality = (int)0x80000000; // minimal signed integer
    Technique * defaultActiveTech = NULL;
    for( std::map<StrA,EffectDesc::TechniqueDesc>::const_iterator iter = desc.techniques.begin();
         iter != desc.techniques.end();
         ++iter )
    {
        const StrA                      & techName = iter->first;
        const EffectDesc::TechniqueDesc & techDesc = iter->second;

        Technique & tech = mTechniques[techName];
        if( !initTech( tech, techName, techDesc ) )
        {
            mTechniques.erase( techName );
            continue; // skip problematic technique
        }

        if( techDesc.quality > highestQuality )
        {
            highestQuality = techDesc.quality;
            defaultActiveTech = &tech;
        }
    }
    if( mTechniques.empty() )
    {
        GN_ERROR(sLogger)( "No valid technique found." );
        return failure();
    }
    GN_ASSERT( defaultActiveTech );
    if( activeTechName.empty() )
    {
        GN_ASSERT( defaultActiveTech );
        mActiveTech = defaultActiveTech;
    }
    else
    {
        std::map<StrA,Technique>::iterator it = mTechniques.find( activeTechName );
        if( mTechniques.end() == it )
        {
            GN_ERROR(sLogger)( "invalid deafult technique name: %s, using default one instead.", activeTechName.cptr() );
            mActiveTech = defaultActiveTech;
        }
        else
        {
            mActiveTech = &it->second;
        }
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
    GN_ASSERT( p.uniforms.size() == p.gpuProgram->getParameterDesc().uniforms.count() );
    drawable.rc.uniforms.resize( p.uniforms.size() );
    for( size_t i = 0; i < p.uniforms.size(); ++i )
    {
        const UniformIter & ui = p.uniforms[i];
        if( ui != mUniforms.end() )
        {
            AutoRef<Uniform> & u = ui->second;
            drawable.rc.uniforms[i] = u;
        }
    }

    // setup textures
    size_t numtex = math::getmin<size_t>( p.textures.size(), RendererContext::MAX_TEXTURES );
    for( size_t i = 0; i < numtex; ++i )
    {
        const PerShaderTextureParam & t = p.textures[i];

        AutoRef<Texture> & tp = t.iter->second;

        drawable.rc.textures[i].texture = tp;
        drawable.rc.textures[i].bindTo( t.binding.cptr() );
    }

    // clear unused texture stages
    for( size_t i = numtex; i < RendererContext::MAX_TEXTURES; ++i )
    {
        drawable.rc.textures[i].clear();
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
    const StrA                      & techName,
    const EffectDesc::TechniqueDesc & techDesc )
{
    tech.passes.resize( techDesc.passes.size() );

    // get common render state for the technique
    EffectDesc::RenderStateDesc commonRenderStates;
    sMergeRenderStates( commonRenderStates, techDesc.rsd, mDesc.rsd );

    // initialize each pass
    for( size_t i = 0; i < tech.passes.size(); ++i )
    {
        const EffectDesc::PassDesc & passDesc = techDesc.passes[i];

        const StrA & shaderName = passDesc.shader; // shader techName alias for easy referencing

        Pass & p = tech.passes[i];

        // Look up GPU program description
        const EffectDesc::ShaderDesc * shaderDesc = sFindNamedPtr( mDesc.shaders, shaderName );
        if( NULL == shaderDesc )
        {
            GN_ERROR(sLogger)(
                "Technique '%s' referencs non-exist shader name '%s' in pass %u",
                techName.cptr(),
                shaderName.cptr(),
                i );
            return false;
        }

        // check renderer caps against shader requirments
        if( !shaderDesc->prerequisites.check( mRenderer ) )
        {
            // Note: it is expected scenario that some shaders are not supported by current hardware.
            //       So here we just issue a verbose log, instead of error.
            GN_VERBOSE(sLogger)(
                "Technique '%s' is skipped because shader '%s', which is referenced by the technique in pass %u, "
                "is not supported by current graphics hardware.",
                techName.cptr(),
                shaderName.cptr(),
                i );
            return false;
        }

        // look up GPU program
        p.gpuProgram = sFindNamedPtr( mGpuPrograms, shaderName );
        if( NULL == p.gpuProgram )
        {
            GN_ERROR(sLogger)(
                "Shader '%s' referenced by technique '%s' in pass %u is not initialized",
                shaderName.cptr(),
                techName.cptr(),
                i );
            return false;
        }

        const GpuProgramParameterDesc & gpuparam = p.gpuProgram->getParameterDesc();

        GN_ASSERT( mDesc.shaders.find(shaderName) != mDesc.shaders.end() );
        const EffectDesc::ShaderDesc & sdesc = mDesc.shaders.find(shaderName)->second;

        // look up textures
        for( std::map<StrA,StrA>::const_iterator iter = sdesc.textures.begin(); iter != sdesc.textures.end(); ++iter )
        {
            const StrA & texBind = iter->first;
            const StrA & texName = iter->second;

            const EffectDesc::TextureDesc * tdesc = sFindNamedPtr( mDesc.textures, texName );
            if( NULL == tdesc )
            {
                GN_ERROR(sLogger)(
                    "shader '%s' referencs non-exisit texture '%s'.",
                    shaderName.cptr(), texName.cptr() );
            }
            else if( GPU_PROGRAM_PARAMETER_NOT_FOUND == gpuparam.textures[texBind.cptr()] )
            {
                GN_ERROR(sLogger)(
                    "texture '%s' is binded to invalid parameter '%s' of shader '%s'.",
                    texName.cptr(), texBind.cptr(), shaderName.cptr() );
            }
            // TODO: check GPU parameter type. Make sure it is a texture parameter.
            else
            {
                PerShaderTextureParam tex;

                tex.iter    = mTextures.find(texName);
                tex.binding = texBind;
                tex.sampler = &tdesc->sampler;

                p.textures.append( tex );

                // this must be a valid texture parameter
                GN_ASSERT( mTextures.end() != tex.iter );
            }
        }
        if( p.textures.size() > RendererContext::MAX_TEXTURES )
        {
            GN_ERROR(sLogger)( "technique %s pass %u has too many textures.", techName.cptr(), i );
        }

        // look up uniforms
        p.uniforms.resize( gpuparam.uniforms.count() );
        std::fill( p.uniforms.begin(), p.uniforms.end(), mUniforms.end() );
        for( std::map<StrA,StrA>::const_iterator iter = sdesc.uniforms.begin(); iter != sdesc.uniforms.end(); ++iter )
        {
            const StrA & uniformBind = iter->first;
            const StrA & uniformName = iter->second;

            const EffectDesc::UniformDesc * udesc = sFindNamedPtr( mDesc.uniforms, uniformName );
            if( NULL == udesc )
            {
                GN_ERROR(sLogger)(
                    "shader '%s' is referencing non-exisit uniform '%s'.",
                    shaderName.cptr(), uniformName.cptr() );
            }
            else
            {
                size_t uidx = gpuparam.uniforms[uniformBind.cptr()];

                if( GPU_PROGRAM_PARAMETER_NOT_FOUND == uidx )
                {
                    GN_ERROR(sLogger)(
                        "uniform '%s' is binded to invalid parameter '%s' of shader '%s'.",
                        uniformName.cptr(), uniformBind.cptr(), shaderName.cptr() );
                }
                // TODO: check parameter type.
                else
                {
                    GN_ASSERT( uidx < p.uniforms.size() );

                    p.uniforms[uidx] = mUniforms.find( uniformName );

                    GN_ASSERT( mUniforms.end() != p.uniforms[uidx] );
                }
            }
        }

        // get pass specific render states
        sMergeRenderStates( p.rsd, commonRenderStates, passDesc.rsd );
    }

    GN_TODO( "build vertex format structure" );

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Effect::clone( const Effect & e )
{
    GN_ASSERT( this != &e ); // can't clone from itself

    // delete previous effect
    quit();

    // there's nothing to do, if the source is uninitialized.
    if( !e.ok() ) return;

    // copy StdClass members
    *(StdClass*)this = (const StdClass&)e;

    // copy misc. members
    mDesc         = e.mDesc;
    mUniforms     = e.mUniforms;
    mTextures     = e.mTextures;
    mGpuPrograms  = e.mGpuPrograms;
    mTechniques   = e.mTechniques;
    mDummyUniform = e.mDummyUniform;
    mDummyTexture = e.mDummyTexture;

    // Fix up iterators and pointers for each technique
    for( std::map<StrA,Technique>::const_iterator i = e.mTechniques.begin(); i != e.mTechniques.end(); ++i )
    {
        const StrA & texName = i->first;
        const Technique & tsrc = i->second;
        Technique & tdst = mTechniques[texName];

        // for each pass
        GN_ASSERT( tdst.passes.size() == tsrc.passes.size() );
        for( size_t i = 0; i < tdst.passes.size(); ++i )
        {
            Pass & pdst = tdst.passes[i];

            GN_ASSERT( pdst.gpuProgram == mGpuPrograms[mDesc.techniques[texName].passes[i].shader].get() );

            // for each textures
            GN_ASSERT( pdst.textures.size() == tsrc.passes[i].textures.size() );
            for( size_t i = 0; i < pdst.textures.size(); ++i )
            {
                PerShaderTextureParam & tex = pdst.textures[i];

                const StrA & texname = tex.iter->first;

                // fix texture iterator
                tex.iter = mTextures.find(texname);

                GN_ASSERT( tex.iter != mTextures.end() );

                // fix sampler pointer
                const EffectDesc::TextureDesc * tdesc = sFindNamedPtr( mDesc.textures, texname );
                tex.sampler = &tdesc->sampler;
            }

            // for each uniforms
            for( size_t i = 0; i < pdst.uniforms.size(); ++i )
            {
                UniformIter & ui = pdst.uniforms[i];

                const StrA & uniformName = ui->first;

                // fix uniform iterator pointers
                ui = mUniforms.find( uniformName );

                GN_ASSERT( mUniforms.end() != pdst.uniforms[i] );
            }
        }

        // fix active technique pointer
        if( &tsrc == e.mActiveTech )
        {
            mActiveTech = &tdst;
        }
    }
}

//
// dummy uniform is lazy created
// -----------------------------------------------------------------------------
AutoRef<Uniform> & GN::gfx::Effect::getDummyUniform()
{
    if( !mDummyUniform )
    {
        mDummyUniform.attach( mRenderer.createUniform( 1 ) );
        GN_ASSERT( mDummyUniform );
    }

    return mDummyUniform;
}

//
// dummy texture is lazy created
// -----------------------------------------------------------------------------
AutoRef<Texture> & GN::gfx::Effect::getDummyTexture()
{
    if( !mDummyTexture )
    {
        mDummyTexture.attach( mRenderer.create1DTexture( 1 ) );
        GN_ASSERT( mDummyTexture );
    }

    return mDummyTexture;
}
