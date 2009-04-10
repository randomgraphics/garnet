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
        const StrA                    & uname = iter->first;
        const EffectDesc::UniformDesc & udesc = iter->second;

        // create GPU program
        Uniform * u = mRenderer.createUniform( udesc.size );
        if( NULL == u ) return false;

        // add to uniform array
        mUniforms[uname].attach( u );
    }

    // create texture array
    for( std::map<StrA,EffectDesc::TextureDesc>::const_iterator iter = desc.textures.begin();
         iter != desc.textures.end();
         ++iter )
    {
        const StrA & tname = iter->first;

        // add new item, with NULL texture pointer, in texture array.
        mTextures[tname];
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
    int highestQuality = (int)0x80000000; // minimal signed integer
    Technique * defaultActiveTech = NULL;
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

        drawable.rc.textures[i] = tp;
        drawable.rc.bindTexture( i, t.binding.cptr() );
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

        const GpuProgramParameterDesc & gpuparam = p.gpuProgram->getParameterDesc();

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
            else if( GPU_PROGRAM_PARAMETER_NOT_FOUND == gpuparam.textures[tbind.cptr()] )
            {
                GN_ERROR(sLogger)(
                    "texture '%s' is binded to invalid parameter '%s' of shader '%s'.",
                    tname.cptr(), tbind.cptr(), sname.cptr() );
            }
            // TODO: check GPU parameter type. Make sure it is a texture parameter.
            else
            {
                PerShaderTextureParam tex;

                tex.iter    = mTextures.find(tname);
                tex.binding = tbind;
                tex.sampler = &tdesc->sampler;

                p.textures.append( tex );

                // this must be a valid texture parameter
                GN_ASSERT( mTextures.end() != tex.iter );
            }
        }
        if( p.textures.size() > RendererContext::MAX_TEXTURES )
        {
            GN_ERROR(sLogger)( "technique %s pass %u has too many textures.", name.cptr(), i );
        }

        // look up uniforms
        p.uniforms.resize( gpuparam.uniforms.count() );
        std::fill( p.uniforms.begin(), p.uniforms.end(), mUniforms.end() );
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
                size_t uidx = gpuparam.uniforms[ubind.cptr()];

                if( GPU_PROGRAM_PARAMETER_NOT_FOUND == uidx )
                {
                    GN_ERROR(sLogger)(
                        "uniform '%s' is binded to invalid parameter '%s' of shader '%s'.",
                        uname.cptr(), ubind.cptr(), sname.cptr() );
                }
                // TODO: check parameter type.
                else
                {
                    GN_ASSERT( uidx < p.uniforms.size() );

                    p.uniforms[uidx] = mUniforms.find( uname );

                    GN_ASSERT( mUniforms.end() != p.uniforms[uidx] );
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
        const StrA & tname = i->first;
        const Technique & tsrc = i->second;
        Technique & tdst = mTechniques[tname];

        // for each pass
        GN_ASSERT( tdst.passes.size() == tsrc.passes.size() );
        for( size_t i = 0; i < tdst.passes.size(); ++i )
        {
            Pass & pdst = tdst.passes[i];

            GN_ASSERT( pdst.gpuProgram == mGpuPrograms[mDesc.techniques[tname].passes[i].shader].get() );

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
