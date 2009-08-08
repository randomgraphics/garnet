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
// merge single render state 'a' and 'b'.
// -----------------------------------------------------------------------------
template<typename T>
static void
sMergeRenderState(
    GN::gfx::EffectDesc::RenderState<T>       & out,
    const GN::gfx::EffectDesc::RenderState<T> & a,
    const GN::gfx::EffectDesc::RenderState<T> & b )
{
    out = b.inherited ? a : b;
}

//
// merge render states 'a' and 'b'. Values in B take over values in a.
// -----------------------------------------------------------------------------
static void
sMergeRenderStates(
    GN::gfx::EffectDesc::RenderStateDesc       & out,
    const GN::gfx::EffectDesc::RenderStateDesc & a,
    const GN::gfx::EffectDesc::RenderStateDesc & b )
{
    #define MERGE_SINGLE_RENDER_STATE( state ) out.state = b.state.inherited ? a.state : b.state

    MERGE_SINGLE_RENDER_STATE( fillMode );
    MERGE_SINGLE_RENDER_STATE( cullMode );
    MERGE_SINGLE_RENDER_STATE( frontFace );
    MERGE_SINGLE_RENDER_STATE( msaaEnabled );
    MERGE_SINGLE_RENDER_STATE( depthTest );
    MERGE_SINGLE_RENDER_STATE( depthWrite );
    MERGE_SINGLE_RENDER_STATE( depthFunc );

    #undef MERGE_SINGLE_RENDER_STATE
}

//
// apply render states to GPU context
// -----------------------------------------------------------------------------
static void
sApplyRenderStates(
    GN::gfx::GpuContext                   & context,
    const GN::gfx::EffectDesc::RenderStateDesc & rsd )
{
    #define APPLY_SINGLE_RENDER_STATE( state ) if( !rsd.state.inherited ) context.state = rsd.state.value; else void(0)

    APPLY_SINGLE_RENDER_STATE( fillMode );
    APPLY_SINGLE_RENDER_STATE( cullMode );
    APPLY_SINGLE_RENDER_STATE( frontFace );
    APPLY_SINGLE_RENDER_STATE( msaaEnabled );
    APPLY_SINGLE_RENDER_STATE( depthTest );
    APPLY_SINGLE_RENDER_STATE( depthWrite );
    APPLY_SINGLE_RENDER_STATE( depthFunc );

    #undef APPLY_SINGLE_RENDER_STATE
}

//
//
// -----------------------------------------------------------------------------
static bool
sCheckGpuCaps( Gpu & r, const GN::gfx::EffectDesc::ShaderDesc & desc )
{
    const GpuCaps & caps = r.getCaps();

    // check vertex shader
    if( desc.gpd.vs.source && (UInt32)desc.gpd.lang != (caps.vsLanguages & desc.gpd.lang) )
    {
        return false;
    }

    // check geometry shader
    if( desc.gpd.gs.source && (UInt32)desc.gpd.lang != (caps.gsLanguages & desc.gpd.lang) )
    {
        return false;
    }

    // check pixel shader
    if( desc.gpd.ps.source && (UInt32)desc.gpd.lang != (caps.psLanguages & desc.gpd.lang) )
    {
        return false;
    }

    // check number of textures
    if( caps.maxTextures < desc.prerequisites.numTextures )
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
GN::gfx::Effect::Effect( Gpu & r )
    : mGpu(r)
    , mDummyUniform( r.createUniform( 1 ) )
{
    uniforms.mMap = &mUniforms;
    uniforms.mGetDummyFunc = makeDelegate(this,&Effect::getDummyUniform);

    textures.mMap = &mTextures;
    textures.mGetDummyFunc = makeDelegate(this,&Effect::getDummyTexture);

    rendertargets.mMap = &mRenderTargets;
    rendertargets.mGetDummyFunc = makeDelegate(this, &Effect::getDummyRenderTargetTexture);

    clear();
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Effect::Effect( const Effect & e )
    : mGpu( e.mGpu )
    , mDummyUniform( e.mDummyUniform )
    , mDummyTexture( e.mDummyTexture )
{
    GN_ASSERT( this != &e ); // can't copy construct from itself.

    uniforms.mMap = &mUniforms;
    uniforms.mGetDummyFunc = makeDelegate(this,&Effect::getDummyUniform);

    textures.mMap = &mTextures;
    textures.mGetDummyFunc = makeDelegate(this,&Effect::getDummyTexture);

    rendertargets.mMap = &mRenderTargets;
    rendertargets.mGetDummyFunc = makeDelegate(this, &Effect::getDummyRenderTargetTexture);

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
        Uniform * u = mGpu.createUniform( udesc.size );
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

    // create render target array
    for( std::map<StrA,EffectDesc::RenderTargetDesc>::const_iterator iter = desc.rendertargets.begin();
        iter != desc.rendertargets.end();
        ++iter )
    {
        const StrA & rtname = iter->first;
        mRenderTargets[rtname];
    }

    // create GPU program array
    for( std::map<StrA,EffectDesc::ShaderDesc>::const_iterator iter = desc.shaders.begin();
         iter != desc.shaders.end();
         ++iter )
    {
        const StrA                   & shaderName = iter->first;
        const EffectDesc::ShaderDesc & shaderDesc = iter->second;

        // check shader requirements.
        if( !sCheckGpuCaps( mGpu, shaderDesc ) )
        {
            GN_VERBOSE(sLogger)( "shader '%s' is skipped due to missing GPU caps." );
            continue;
        }

        // create GPU program
        GpuProgram * gp = mGpu.createGpuProgram( shaderDesc.gpd );
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

    // setup textures and samplers
    GN_ASSERT( p.textures.size() == p.gpuProgram->getParameterDesc().textures.count() );
    GN_ASSERT( p.textures.size() <= GpuContext::MAX_TEXTURES );
    for( size_t i = 0; i < p.textures.size(); ++i )
    {
        const PerShaderTextureParam & t = p.textures[i];

        if( t.iter != mTextures.end() )
        {
            AutoRef<Texture> & tp = t.iter->second;
            drawable.rc.textures[i].texture = tp;
            drawable.rc.textures[i].sampler = *t.sampler;
        }
    }

    // clear unused texture stages
    for( size_t i = p.textures.size(); i < GpuContext::MAX_TEXTURES; ++i )
    {
        drawable.rc.textures[i].clear();
    }

    // setup render states
    sApplyRenderStates( drawable.rc, p.rsd );

    // setup render targets
    bool noColorTargets = true;
    drawable.rc.colortargets.resize( p.colortargets.size() );
    for( size_t i = 0; i < p.colortargets.size(); ++i )
    {
        GN_ASSERT( p.colortargets[i] != mRenderTargets.end() );

        const RenderTargetTexture & rtt = p.colortargets[i]->second;

        drawable.rc.colortargets[i] = rtt;

        if( rtt.texture ) noColorTargets = false;
    }
    if( noColorTargets )
    {
        drawable.rc.colortargets.clear();
    }
    const RenderTargetTexture & depthstencil = p.depthstencil->second;
    drawable.rc.depthstencil = depthstencil;

    // success
    drawable.gpu = &mGpu;
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
    for( size_t ipass = 0; ipass < tech.passes.size(); ++ipass )
    {
        const EffectDesc::PassDesc & passDesc = techDesc.passes[ipass];

        const StrA & shaderName = passDesc.shader; // shader techName alias for easy referencing

        Pass & p = tech.passes[ipass];

        // Look up GPU program description
        const EffectDesc::ShaderDesc * shaderDesc = sFindNamedPtr( mDesc.shaders, shaderName );
        if( NULL == shaderDesc )
        {
            GN_ERROR(sLogger)(
                "Technique '%s' referencs non-exist shader name '%s' in pass %u",
                techName.cptr(),
                shaderName.cptr(),
                ipass );
            return false;
        }

        // check GPU caps against shader requirments
        if( !sCheckGpuCaps( mGpu, *shaderDesc ) )
        {
            // Note: it is expected scenario that some shaders are not supported by current hardware.
            //       So here we just issue a verbose log, instead of error.
            GN_VERBOSE(sLogger)(
                "Technique '%s' is skipped because shader '%s', which is referenced by the technique in pass %u, "
                "is not supported by current graphics hardware.",
                techName.cptr(),
                shaderName.cptr(),
                ipass );
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
                ipass );
            return false;
        }

        const GpuProgramParameterDesc & gpuparam = p.gpuProgram->getParameterDesc();

        GN_ASSERT( mDesc.shaders.find(shaderName) != mDesc.shaders.end() );
        const EffectDesc::ShaderDesc & sdesc = mDesc.shaders.find(shaderName)->second;

        // look up textures
        p.textures.resize( gpuparam.textures.count() );
        GN_ASSERT( p.textures.size() <= GpuContext::MAX_TEXTURES );
        for( size_t itex = 0; itex < p.textures.size(); ++itex )
        {
            p.textures[itex].iter = mTextures.end();
        }
        for( std::map<StrA,StrA>::const_iterator iter = sdesc.textures.begin(); iter != sdesc.textures.end(); ++iter )
        {
            const StrA & texBind = iter->first;
            const StrA & texName = iter->second;

            const EffectDesc::TextureDesc * tdesc = sFindNamedPtr( mDesc.textures, texName );
            if( NULL == tdesc )
            {
                GN_ERROR(sLogger)(
                    "Effect technique '%s', pass #%d: "
                    "'%s' is not a valid effect texture parameter name.",
                    techName.cptr(),
                    ipass,
                    texName.cptr() );
                return false;
            }

            size_t tidx = gpuparam.textures[texBind];
            if( GPU_PROGRAM_PARAMETER_NOT_FOUND == tidx )
            {
                GN_ERROR(sLogger)(
                    "Effect technique '%s', pass #%d: "
                    "'%s' is not a valid texture parameter name of shader '%s'.",
                    techName.cptr(),
                    ipass,
                    texBind.cptr(),
                    shaderName.cptr() );
                return false;
            }

            // this must be a valid texture parameter
            p.textures[tidx].iter = mTextures.find(texName);
            p.textures[tidx].sampler = &tdesc->sampler;
            GN_ASSERT( mTextures.end() != p.textures[tidx].iter );
        }
        for( size_t itex = 0; itex < p.textures.size(); ++itex )
        {
            if( mTextures.end() == p.textures[itex].iter )
            {
                // Note: Although uninitialize shader parameter is allowed,
                // it usually implies error in effect definition.
                GN_WARN(sLogger)(
                    "Effect technique '%s', pass #%d: "
                    "texture parameter '%s' of shader '%s' is left uninitialized.",
                    techName.cptr(),
                    ipass,
                    gpuparam.textures[itex].name,
                    shaderName.cptr() );
            }
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
                return false;
            }

            size_t uidx = gpuparam.uniforms[uniformBind.cptr()];
            if( GPU_PROGRAM_PARAMETER_NOT_FOUND == uidx )
            {
                GN_ERROR(sLogger)(
                    "uniform '%s' is binded to invalid parameter '%s' of shader '%s'.",
                    uniformName.cptr(), uniformBind.cptr(), shaderName.cptr() );
                return false;
            }

            // must be a valid uniform parameter
            GN_ASSERT( uidx < p.uniforms.size() );
            p.uniforms[uidx] = mUniforms.find( uniformName );
            GN_ASSERT( mUniforms.end() != p.uniforms[uidx] );
        }
        for( size_t iuniform = 0; iuniform < p.uniforms.size(); ++iuniform )
        {
            if( mUniforms.end() == p.uniforms[iuniform] )
            {
                // Note: Although uninitialize shader parameter is allowed,
                // technically speaking, it usually implies error
                // in effect definition.
                GN_WARN(sLogger)(
                    "Effect technique '%s', pass #%d: "
                    "uniform parameter '%s' of shader '%s' is left uninitialized.",
                    techName.cptr(),
                    ipass,
                    gpuparam.uniforms[iuniform].name,
                    shaderName.cptr() );
            }
        }

        // lookup render targets
        p.colortargets.resize( passDesc.colortargets.size() );
        std::fill( p.colortargets.begin(), p.colortargets.end(), mRenderTargets.end() );
        for( size_t itarget = 0; itarget < p.colortargets.size(); ++itarget )
        {
            const StrA & rtName = passDesc.colortargets[itarget];

            const EffectDesc::RenderTargetDesc * rtdesc = sFindNamedPtr( mDesc.rendertargets, rtName );
            if( NULL == rtdesc )
            {
                GN_ERROR(sLogger)(
                    "Effect technique '%s', pass #%d: invalid color render target name: %s.",
                    techName.cptr(),
                    ipass,
                    rtName.cptr() );
                return false;
            }

            p.colortargets[itarget] = mRenderTargets.find( rtName );
            GN_ASSERT( mRenderTargets.end() != p.colortargets[itarget] );
        }
        p.depthstencil = mRenderTargets.find( passDesc.depthstencil );
        if( mRenderTargets.end() == p.depthstencil )
        {
            GN_ERROR(sLogger)(
                "Effect technique '%s', pass #%d: invalid depth render target name: %s.",
                techName.cptr(),
                ipass,
                passDesc.depthstencil.cptr() );
            return false;
        }

        // get pass specific render states
        sMergeRenderStates( p.rsd, commonRenderStates, passDesc.rsd );
    }

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
    mDesc          = e.mDesc;
    mUniforms      = e.mUniforms;
    mTextures      = e.mTextures;
    mRenderTargets = e.mRenderTargets;
    mGpuPrograms   = e.mGpuPrograms;
    mTechniques    = e.mTechniques;
    mDummyUniform  = e.mDummyUniform;
    mDummyTexture  = e.mDummyTexture;

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

            // for each color render targets
            for( size_t i = 0; i < pdst.colortargets.size(); ++i )
            {
                RenderTargetIter & ri = pdst.colortargets[i];

                const StrA & rtname = ri->first;

                ri = mRenderTargets.find( rtname );

                GN_ASSERT( mRenderTargets.end() != ri );
            }

            // fix depth render target iterator
            pdst.depthstencil = mRenderTargets.find( pdst.depthstencil->first );
            GN_ASSERT( mRenderTargets.end() != pdst.depthstencil );
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
        mDummyUniform.attach( mGpu.createUniform( 1 ) );
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
        mDummyTexture.attach( mGpu.create1DTexture( 1 ) );
        GN_ASSERT( mDummyTexture );
    }

    return mDummyTexture;
}
