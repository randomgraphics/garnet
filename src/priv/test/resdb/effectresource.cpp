#include "pch.h"
#include "effectresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

typedef GN::gfx::EffectResourceDesc::ShaderPrerequisites ShaderPrerequisites;
typedef GN::gfx::EffectResourceDesc::EffectUniformDesc EffectUniformDesc;
typedef GN::gfx::EffectResourceDesc::EffectTextureDesc EffectTextureDesc;
typedef GN::gfx::EffectResourceDesc::EffectShaderDesc EffectShaderDesc;
typedef GN::gfx::EffectResourceDesc::EffectRenderStateDesc EffectRenderStateDesc;
typedef GN::gfx::EffectResourceDesc::EffectPassDesc EffectPassDesc;
typedef GN::gfx::EffectResourceDesc::EffectTechniqueDesc EffectTechniqueDesc;

// *****************************************************************************
// Local stuff
// *****************************************************************************

 //
//
// -----------------------------------------------------------------------------
static bool
sCheckGpuCaps( Gpu & r, const EffectShaderDesc & desc )
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
// merge render states 'a' and 'b'. Values in B take priorities than values in B.
// -----------------------------------------------------------------------------
static void
sMergeRenderStates(
    EffectRenderStateDesc       & out,
    const EffectRenderStateDesc & a,
    const EffectRenderStateDesc & b )
{
    #define MERGE_SINGLE_RENDER_STATE( state ) out.state = b.state.overridden ? b.state : a.state

    MERGE_SINGLE_RENDER_STATE( depthTestEnabled );
    MERGE_SINGLE_RENDER_STATE( depthWriteEnabled );
    MERGE_SINGLE_RENDER_STATE( depthFunc );

    MERGE_SINGLE_RENDER_STATE( stencilEnabled );
    MERGE_SINGLE_RENDER_STATE( stencilPassOp );
    MERGE_SINGLE_RENDER_STATE( stencilFailOp );
    MERGE_SINGLE_RENDER_STATE( stencilZFailOp );

    MERGE_SINGLE_RENDER_STATE( blendingEnabled );
    MERGE_SINGLE_RENDER_STATE( blendFactors );

    MERGE_SINGLE_RENDER_STATE( fillMode );
    MERGE_SINGLE_RENDER_STATE( cullMode );
    MERGE_SINGLE_RENDER_STATE( frontFace );
    MERGE_SINGLE_RENDER_STATE( msaaEnabled );

    #undef MERGE_SINGLE_RENDER_STATE
}

//
// Check for texture errors in shader descriptor
// -----------------------------------------------------------------------------
static bool
sCheckShaderTextures(
    const EffectResourceDesc & effectDesc,
    const EffectShaderDesc   & shaderDesc,
    const char               * shaderName,
    const GpuProgram         & program )
{
    const GpuProgramParameterDesc & param = program.getParameterDesc();

    for( std::map<StrA,StrA>::const_iterator iter = shaderDesc.textures.begin();
         iter != shaderDesc.textures.end();
         ++iter )
    {
        const StrA & shaderParameterName = iter->first;
        const StrA & textureName = iter->second;

        if( GPU_PROGRAM_PARAMETER_NOT_FOUND == param.textures[shaderParameterName] )
        {
            GN_ERROR(sLogger)( "Invalid GPU program parameter named '%s' is referenced in shader '%s'.",
                shaderParameterName.cptr(),
                shaderName );
            return false;
        }
        else if( effectDesc.textures.end() == effectDesc.textures.find( textureName ) )
        {
            GN_ERROR(sLogger)( "Invalid texture named '%s' is referenced in shader '%s'.",
                shaderParameterName.cptr(),
                shaderName );
            return false;
        }
    }

    return true;
}

//
// Check for uniform errors in shader descriptor
// -----------------------------------------------------------------------------
static bool
sCheckShaderUniforms(
    const EffectResourceDesc & effectDesc,
    const EffectShaderDesc   & shaderDesc,
    const char               * shaderName,
    const GpuProgram         & program )
{
    const GpuProgramParameterDesc & param = program.getParameterDesc();

    for( std::map<StrA,StrA>::const_iterator iter = shaderDesc.uniforms.begin();
         iter != shaderDesc.uniforms.end();
         ++iter )
    {
        const StrA & shaderParameterName = iter->first;
        const StrA & uniformName = iter->second;

        if( GPU_PROGRAM_PARAMETER_NOT_FOUND == param.uniforms[shaderParameterName] )
        {
            GN_ERROR(sLogger)( "Invalid GPU program parameter named '%s' is referenced in shader '%s'.",
                shaderParameterName.cptr(),
                shaderName );
            return false;
        }
        else if( effectDesc.uniforms.end() == effectDesc.uniforms.find( uniformName ) )
        {
            GN_ERROR(sLogger)( "Invalid uniform named '%s' is referenced in shader '%s'.",
                shaderParameterName.cptr(),
                shaderName );
            return false;
        }
    }

    return true;
}

// *****************************************************************************
// GN::gfx::EffectResource::Impl - Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectResource::Impl::init( const EffectResourceDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::EffectResource::Impl, () );

    if( !initGpuPrograms( desc ) ) return failure();
    if( !initTechniques( desc ) ) return failure();
    if( !initTextures( desc ) ) return failure();
    if( !initUniforms( desc ) ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResource::Impl::quit()
{
    GN_GUARD;

    mPrograms.clear();
    mPasses.clear();
    mTextures.clear();
    mUniforms.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// GN::gfx::EffectResource::Impl - public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::findTexture( const char * name ) const
{
    GN_UNIMPL_WARNING();
    GN_UNUSED_PARAM( name );
    return PARAMETER_NOT_FOUND;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::findUniform( const char * name ) const
{
    GN_UNIMPL_WARNING();
    GN_UNUSED_PARAM( name );
    return PARAMETER_NOT_FOUND;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResource::Impl::applyToContext( size_t pass, GpuContext & gc ) const
{
    GN_UNUSED_PARAM( pass );
    GN_UNUSED_PARAM( gc );
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// GN::gfx::EffectResource::Impl - private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initGpuPrograms(
    const EffectResourceDesc & effectDesc )
{
    Gpu & gpu = database().gpu();

    for( std::map<StrA,EffectShaderDesc>::const_iterator iter = effectDesc.shaders.begin();
         iter != effectDesc.shaders.end();
         ++iter )
    {
        const StrA             & shaderName = iter->first;
        const EffectShaderDesc & shaderDesc = iter->second;

        // check shader requirements.
        // Note: it is expected scenario that some shaders are not supported by current hardware.
        //       So here only a verbose, instead of error, message is issued.
        if( !sCheckGpuCaps( gpu, shaderDesc ) )
        {
            GN_VERBOSE(sLogger)( "shader '%s' is skipped due to missing GPU caps." );
            continue;
        }

        // create GPU program
        GpuProgramProperties gpp;
        gpp.name = shaderName;
        gpp.prog.attach( gpu.createGpuProgram( shaderDesc.gpd ) );
        if( !gpp.prog ) continue;

        // check textures and uniforms
        if( !sCheckShaderTextures( effectDesc, shaderDesc, shaderName, *gpp.prog ) ) continue;
        if( !sCheckShaderUniforms( effectDesc, shaderDesc, shaderName, *gpp.prog ) ) continue;

        // add to GPU program array
        if( gpp.prog ) mPrograms.append( gpp );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initTechniques(
    const EffectResourceDesc & effectDesc )
{
    int currentQuality = (int)0x80000000; // minimal signed integer

    for( std::map<StrA,EffectTechniqueDesc>::const_iterator iter = effectDesc.techniques.begin();
         iter != effectDesc.techniques.end();
         ++iter )
    {
        const StrA                & techName = iter->first;
        const EffectTechniqueDesc & techDesc = iter->second;

        // ignore the technique with lower quality
        if( techDesc.quality <= currentQuality )
        {
            continue;
        }

        DynaArray<RenderPass> passes = mPasses;
        mPasses.clear();

        if( initTech( effectDesc, techName, techDesc ) )
        {
            currentQuality = techDesc.quality;
        }
        else
        {
            mPasses = passes;
        }
    }

    if( (int)0x80000000 == currentQuality )
    {
        GN_WARN(sLogger)( "No valid technique is found in effect '%s'", effectName() );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initTech(
    const EffectResourceDesc  & effectDesc,
    const StrA                & techName,
    const EffectTechniqueDesc & techDesc )
{
    mPasses.resize( techDesc.passes.size() );

    // get common render state for the technique
    EffectRenderStateDesc commonRenderStates;
    sMergeRenderStates( commonRenderStates, techDesc.rsdesc, effectDesc.rsdesc );

    Gpu & gpu = database().gpu();

    // initialize each pass
    for( size_t ipass = 0; ipass < techDesc.passes.size(); ++ipass )
    {
        const EffectPassDesc & passDesc = techDesc.passes[ipass];

        const StrA & shaderName = passDesc.shader; // shader techName alias for easy referencing

        RenderPass & p = mPasses[ipass];

        // look up GPU program
        p.gpuProgramIndex = findGpuProgram( shaderName );
        if( PARAMETER_NOT_FOUND == p.gpuProgramIndex )
        {
            // Shader is not found. Let's find out why. See if it is expected.

            // Look up GPU program description
            const EffectShaderDesc * shaderDesc = sFindNamedPtr( effectDesc.shaders, shaderName );
            if( NULL == shaderDesc )
            {
                GN_ERROR(sLogger)(
                    "Technique '%s' referencs non-exist shader name '%s' in pass %u",
                    techName.cptr(),
                    shaderName.cptr(),
                    ipass );
            }

            // check GPU caps against shader requirments
            else if( !sCheckGpuCaps( gpu, *shaderDesc ) )
            {
                // Note: it is expected scenario that some shaders are not supported by current hardware.
                //       So here only a verbose, instead of error, message is issued.
                GN_VERBOSE(sLogger)(
                    "Technique '%s' is skipped because shader '%s', which is referenced by the technique in pass %u, "
                    "is not supported by current graphics hardware.",
                    techName.cptr(),
                    shaderName.cptr(),
                    ipass );
                return false;
            }
            else
            {
                GN_ERROR(sLogger)(
                    "Shader '%s' referenced by technique '%s' in pass %u is not properly initialized",
                    shaderName.cptr(),
                    techName.cptr(),
                    ipass );
            }

            return false;
        }

        // get pass specific render states
        sMergeRenderStates( p.rsdesc, commonRenderStates, passDesc.rsdesc );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initTextures(
    const EffectResourceDesc  & effectDesc )
{
    for( std::map<StrA,EffectTextureDesc>::const_iterator iter = effectDesc.textures.begin();
         iter != effectDesc.textures.end();
         ++iter )
    {
        TextureProperties tp;

        tp.parameterName = iter->first;
        tp.sampler = iter->second.sampler;

        // setup texture binding point array
        for( size_t ipass = 0; ipass < mPasses.size(); ++ipass )
        {
            const GpuProgramProperties & gpp = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpuparam = gpp.prog->getParameterDesc();
            const EffectShaderDesc * shaderDesc = sFindNamedPtr( effectDesc.shaders, gpp.name );

            for( std::map<StrA,StrA>::const_iterator iter = shaderDesc->textures.begin();
                 iter != shaderDesc->textures.end();
                 ++iter )
            {
                const StrA & shaderParameterName = iter->first;
                const StrA & textureName = iter->second;

                GN_ASSERT( effectDesc.textures.end() != effectDesc.textures.find( textureName ) );

                if( textureName == tp.parameterName )
                {
                    BindingLocation b = { ipass, gpuparam.textures[shaderParameterName] };
                    GN_ASSERT( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.stage );
                    tp.bindings.append( b );
                }
            }
        }

        if( tp.bindings.empty() )
        {
            GN_WARN(sLogger)( "Non used texture parameter '%s' in effect '%s'.",
                tp.parameterName.cptr(),
                effectName() );
        }

        mTextures.append( tp );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initUniforms(
    const EffectResourceDesc  & effectDesc )
{
    for( std::map<StrA,EffectUniformDesc>::const_iterator iter = effectDesc.uniforms.begin();
         iter != effectDesc.uniforms.end();
         ++iter )
    {
        UniformProperties up;

        up.parameterName = iter->first;

        // setup uniform binding point array
        for( size_t ipass = 0; ipass < mPasses.size(); ++ipass )
        {
            const GpuProgramProperties & gpp = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpuparam = gpp.prog->getParameterDesc();
            const EffectShaderDesc * shaderDesc = sFindNamedPtr( effectDesc.shaders, gpp.name );

            for( std::map<StrA,StrA>::const_iterator iter = shaderDesc->uniforms.begin();
                 iter != shaderDesc->uniforms.end();
                 ++iter )
            {
                const StrA & shaderParameterName = iter->first;
                const StrA & uniformName = iter->second;

                GN_ASSERT( effectDesc.uniforms.end() != effectDesc.uniforms.find( uniformName ) );

                if( uniformName == up.parameterName )
                {
                    BindingLocation b = { ipass, gpuparam.uniforms[shaderParameterName] };
                    GN_ASSERT( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.stage );
                    up.bindings.append( b );
                }
            }
        }

        if( up.bindings.empty() )
        {
            GN_WARN(sLogger)( "Non used uniform parameter '%s' in effect '%s'.",
                up.parameterName.cptr(),
                effectName() );
        }

        mUniforms.append( up );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::findGpuProgram( const StrA & shaderName ) const
{
    for( size_t i = 0; i < mPrograms.size(); ++i )
    {
        if( mPrograms[i].name == shaderName ) return i;
    }
    return PARAMETER_NOT_FOUND;
}

// *****************************************************************************
// GN::gfx::EffectResource
// *****************************************************************************

class EffectResourceInternal : public EffectResource
{
    EffectResourceInternal( GpuResourceDatabase & db, GpuResourceHandle handle )
        : EffectResource( db, handle )
    {
    }

    bool init( const void * parameters )
    {
        if( NULL == parameters )
        {
            GN_ERROR(sLogger)( "Null parameter pointer." );
            return false;
        }
        return mImpl->init( *(const EffectResourceDesc*)parameters );
    }

    static GpuResource *
    createInstance( GpuResourceDatabase & db,
                    GpuResourceHandle     handle,
                    const void          * parameters )
    {
        AutoObjPtr<EffectResourceInternal> m( new EffectResourceInternal( db, handle ) );
        if( !m->init( parameters ) ) return NULL;
        return m.detach();
    }

    static void deleteInstance( GpuResource * p )
    {
        delete GpuResource::castTo<EffectResourceInternal>( p );
    }

public:

    static bool checkAndRegisterFactory( GpuResourceDatabase & db )
    {
        if( db.hasResourceFactory( guid() ) ) return true;

        GpuResourceFactory factory = { &createInstance, &deleteInstance };

        return db.registerResourceFactory( guid(), "Effect Resource", factory );
    }
};

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectResource::EffectResource( GpuResourceDatabase & db, GpuResourceHandle h )
    : GpuResource( db, h )
    , mImpl(NULL)
{
    mImpl = new Impl(*this);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectResource::~EffectResource()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::EffectResource::guid()
{
    static const Guid EFFECT_GUID = { 0x621c00ed, 0xcd51, 0x4cc5, { 0x89, 0x9f, 0xd4, 0xd5, 0xb1, 0xd5, 0xb2, 0xa4 } };
    return EFFECT_GUID;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::EffectResource::create(
    GpuResourceDatabase      & db,
    const char               * name,
    const EffectResourceDesc & desc )
{
    if( !EffectResourceInternal::checkAndRegisterFactory( db ) ) return NULL;

    return db.createResource( EffectResource::guid(), name, &desc );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::EffectResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    if( !EffectResourceInternal::checkAndRegisterFactory( db ) ) return NULL;

    GN_UNUSED_PARAM( filename );
    GN_UNIMPL();

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::getNumPasses() const { return mImpl->getNumPasses(); }

size_t GN::gfx::EffectResource::getNumTextures() const { return mImpl->getNumTextures(); }
size_t GN::gfx::EffectResource::findTexture( const char * name ) const { return mImpl->findTexture( name ); }
const GN::gfx::EffectResource::TextureProperties & GN::gfx::EffectResource::getTextureProperties( size_t i ) const { return mImpl->getTextureProperties( i ); }

size_t GN::gfx::EffectResource::getNumUniforms() const { return mImpl->getNumUniforms(); }
size_t GN::gfx::EffectResource::findUniform( const char * name ) const { return mImpl->findUniform( name ); }
const GN::gfx::EffectResource::UniformProperties & GN::gfx::EffectResource::getUniformProperties( size_t i ) const { return mImpl->getUniformProperties( i ); }

void GN::gfx::EffectResource::applyToContext( size_t pass, GpuContext & gc ) const { return mImpl->applyToContext( pass, gc ); }
