#include "pch.h"
#include "effectresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

typedef GN::gfx::EffectResourceDesc::ShaderPrerequisites ShaderPrerequisites;
typedef GN::gfx::EffectResourceDesc::EffectUniformDesc EffectUniformDesc;
typedef GN::gfx::EffectResourceDesc::EffectTextureDesc EffectTextureDesc;
typedef GN::gfx::EffectResourceDesc::EffectGpuProgramDesc EffectGpuProgramDesc;
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
sCheckGpuCaps( Gpu & r, const EffectGpuProgramDesc & desc )
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

    // check number of render targets
    if( caps.maxColorRenderTargets < desc.prerequisites.numColorRenderTargets )
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

    MERGE_SINGLE_RENDER_STATE( blendEnabled );
    MERGE_SINGLE_RENDER_STATE( blendSrc );
    MERGE_SINGLE_RENDER_STATE( blendDst );
    MERGE_SINGLE_RENDER_STATE( blendOp );
    MERGE_SINGLE_RENDER_STATE( blendAlphaSrc );
    MERGE_SINGLE_RENDER_STATE( blendAlphaDst );
    MERGE_SINGLE_RENDER_STATE( blendAlphaOp );

    MERGE_SINGLE_RENDER_STATE( fillMode );
    MERGE_SINGLE_RENDER_STATE( cullMode );
    MERGE_SINGLE_RENDER_STATE( frontFace );
    MERGE_SINGLE_RENDER_STATE( msaaEnabled );

    MERGE_SINGLE_RENDER_STATE( blendFactors );
    MERGE_SINGLE_RENDER_STATE( colorWriteMask );
    MERGE_SINGLE_RENDER_STATE( viewport );
    MERGE_SINGLE_RENDER_STATE( scissorRect );

    #undef MERGE_SINGLE_RENDER_STATE
}

//
// Check for texture errors in shader descriptor
// -----------------------------------------------------------------------------
static bool
sCheckShaderTextures(
    const EffectResourceDesc & effectDesc,
    const EffectGpuProgramDesc   & shaderDesc,
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
    const EffectGpuProgramDesc   & shaderDesc,
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
// GN::gfx::EffectResourceDesc
// *****************************************************************************

static void sCopyShaderSourcePtr(
    const char          * & to,
    const DynaArray<char> & tobuf,
    const char            * from,
    const DynaArray<char> & frombuf )
{
    GN_ASSERT( tobuf.size() == frombuf.size() );

    const char * s = frombuf.cptr();
    const char * e = s + frombuf.size();

    if( s <= from && from < e )
    {
        to = tobuf.cptr() + ( from - s );
    }
    else
    {
        to = from;
    }
}

static void sCopyShaderDesc( EffectGpuProgramDesc & to, const EffectGpuProgramDesc & from )
{
    to.shaderSourceBuffer = from.shaderSourceBuffer;

    #define COPY_SHADER_PTR( x ) sCopyShaderSourcePtr( to.gpd.x, to.shaderSourceBuffer, from.gpd.x, from.shaderSourceBuffer );

    COPY_SHADER_PTR( vs.source );
    COPY_SHADER_PTR( vs.entry );

    COPY_SHADER_PTR( gs.source );
    COPY_SHADER_PTR( gs.entry );

    COPY_SHADER_PTR( ps.source );
    COPY_SHADER_PTR( ps.entry );

    #undef COPY_SHADER_PTR
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectResourceDesc::EffectGpuProgramDesc::EffectGpuProgramDesc(
    const EffectGpuProgramDesc & rhs )
{
    sCopyShaderDesc( *this, rhs );
}

//
//
// -----------------------------------------------------------------------------
EffectGpuProgramDesc &
GN::gfx::EffectResourceDesc::EffectGpuProgramDesc::operator=(
    const EffectGpuProgramDesc & rhs )
{
    sCopyShaderDesc( *this, rhs );
    return *this;
}

// *****************************************************************************
// GN::gfx::EffectResourceDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResourceDesc::clear()
{
    textures.clear();
    uniforms.clear();
    gpuprograms.clear();
    techniques.clear();

    GpuContext gc;
    gc.clearToDefaultRenderStates();
    memset( &renderstates, 0, sizeof(renderstates) );

    // copy default render state values from GPU context
    renderstates.depthTestEnabled  = !!gc.rs.depthTestEnabled;
    renderstates.depthWriteEnabled = !!gc.rs.depthWriteEnabled;
    renderstates.depthFunc         = gc.rs.depthFunc;

    renderstates.stencilEnabled    = !!gc.rs.stencilEnabled;
    renderstates.stencilPassOp     = gc.rs.stencilPassOp;
    renderstates.stencilFailOp     = gc.rs.stencilFailOp;
    renderstates.stencilZFailOp    = gc.rs.stencilZFailOp;

    renderstates.blendEnabled      = !!gc.rs.blendEnabled;
    renderstates.blendSrc          = gc.rs.blendSrc;
    renderstates.blendDst          = gc.rs.blendDst;
    renderstates.blendOp           = gc.rs.blendOp;
    renderstates.blendAlphaSrc     = gc.rs.blendAlphaSrc;
    renderstates.blendAlphaDst     = gc.rs.blendAlphaDst;
    renderstates.blendAlphaOp      = gc.rs.blendAlphaOp;

    renderstates.fillMode          = gc.rs.fillMode;
    renderstates.cullMode          = gc.rs.cullMode;
    renderstates.frontFace         = gc.rs.frontFace;
    renderstates.msaaEnabled       = !!gc.rs.msaaEnabled;

    renderstates.blendFactors      = gc.rs.blendFactors;

    // exept these:
    //renderstates.colorWriteMask    = gc.rs.colorWriteMask;
    //renderstates.viewport          = gc.rs.viewport;
    //renderstates.scissorRect       = gc.rs.scissorRect;




}

// *****************************************************************************
// GN::gfx::EffectResource::Impl - public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectResource::Impl::reset( const EffectResourceDesc * desc )
{
    clear();

    bool ok;
    if( desc && !init( *desc ) )
    {
        clear();
        ok = false;
    }
    else
    {
        ok = true;
    }

    mOwner.sigEffectChanged( mOwner );

    return ok;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::findTexture( const char * name ) const
{
    if( NULL == name || 0 == *name ) return PARAMETER_NOT_FOUND;

    for( size_t i = 0; i < mTextures.size(); ++i )
    {
        if( name == mTextures[i].parameterName )
        {
            return i;
        }
    }

    return PARAMETER_NOT_FOUND;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::findUniform( const char * name ) const
{
    if( NULL == name || 0 == *name ) return PARAMETER_NOT_FOUND;

    for( size_t i = 0; i < mUniforms.size(); ++i )
    {
        if( name == mUniforms[i].parameterName )
        {
            return i;
        }
    }

    return PARAMETER_NOT_FOUND;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResource::Impl::applyToContext( size_t passIndex, GpuContext & gc ) const
{
    if( passIndex >= mPasses.size() )
    {
        GN_ERROR(sLogger)( "Pass index is too large: %u", passIndex );
        return;
    }

    const RenderPass & pass = mPasses[passIndex];

    const GpuProgramItem & gpitem = mPrograms[pass.gpuProgramIndex];

    gc.gpuProgram = gpitem.prog;
}

// *****************************************************************************
// GN::gfx::EffectResource::Impl - private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectResource::Impl::init( const EffectResourceDesc & desc )
{
    if( !initGpuPrograms( desc ) ) return false;
    if( !initTechniques( desc ) ) return false;
    if( !initTextures( desc ) ) return false;
    if( !initUniforms( desc ) ) return false;

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResource::Impl::clear()
{
    mPrograms.clear();
    mPasses.clear();
    mTextures.clear();
    mUniforms.clear();
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initGpuPrograms(
    const EffectResourceDesc & effectDesc )
{
    Gpu & gpu = database().gpu();

    for( std::map<StrA,EffectGpuProgramDesc>::const_iterator iter = effectDesc.gpuprograms.begin();
         iter != effectDesc.gpuprograms.end();
         ++iter )
    {
        const StrA             & shaderName = iter->first;
        const EffectGpuProgramDesc & shaderDesc = iter->second;

        // check shader requirements.
        // Note: it is expected scenario that some shaders are not supported by current hardware.
        //       So here only a verbose, instead of error, message is issued.
        if( !sCheckGpuCaps( gpu, shaderDesc ) )
        {
            GN_VERBOSE(sLogger)( "shader '%s' is skipped due to missing GPU caps." );
            continue;
        }

        // create GPU program
        GpuProgramItem gpitem;
        gpitem.name = shaderName;
        gpitem.prog.attach( gpu.createGpuProgram( shaderDesc.gpd ) );
        if( !gpitem.prog ) continue;

        // check textures and uniforms
        if( !sCheckShaderTextures( effectDesc, shaderDesc, shaderName, *gpitem.prog ) ) continue;
        if( !sCheckShaderUniforms( effectDesc, shaderDesc, shaderName, *gpitem.prog ) ) continue;

        // add to GPU program array
        if( gpitem.prog ) mPrograms.append( gpitem );
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
    sMergeRenderStates( commonRenderStates, techDesc.renderstates, effectDesc.renderstates );

    Gpu & gpu = database().gpu();

    // initialize each pass
    for( size_t ipass = 0; ipass < techDesc.passes.size(); ++ipass )
    {
        const EffectPassDesc & passDesc = techDesc.passes[ipass];

        const StrA & shaderName = passDesc.gpuprogram; // shader techName alias for easy referencing

        RenderPass & p = mPasses[ipass];

        // look up GPU program
        p.gpuProgramIndex = findGpuProgram( shaderName );
        if( PARAMETER_NOT_FOUND == p.gpuProgramIndex )
        {
            // Shader is not found. Let's find out why. See if it is expected.

            // Look up GPU program description
            const EffectGpuProgramDesc * shaderDesc = sFindNamedPtr( effectDesc.gpuprograms, shaderName );
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
        sMergeRenderStates( p.renderstates, commonRenderStates, passDesc.renderstates );
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
            const GpuProgramItem & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc * shaderDesc = sFindNamedPtr( effectDesc.gpuprograms, gpitem.name );

            for( std::map<StrA,StrA>::const_iterator iter = shaderDesc->textures.begin();
                 iter != shaderDesc->textures.end();
                 ++iter )
            {
                const StrA & shaderParameterName = iter->first;
                const StrA & textureName = iter->second;

                GN_ASSERT( effectDesc.textures.end() != effectDesc.textures.find( textureName ) );

                if( textureName == tp.parameterName )
                {
                    BindingLocation b = { ipass, gpparam.textures[shaderParameterName] };
                    GN_ASSERT( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.stage );
                    tp.bindings.append( b );
                }
            }
        }

        if( tp.bindings.empty() )
        {
            GN_WARN(sLogger)( "Unused texture parameter '%s' in effect '%s'.",
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
            const GpuProgramItem          & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc    * shaderDesc = sFindNamedPtr( effectDesc.gpuprograms, gpitem.name );

            for( std::map<StrA,StrA>::const_iterator iter = shaderDesc->uniforms.begin();
                 iter != shaderDesc->uniforms.end();
                 ++iter )
            {
                const StrA & shaderParameterName = iter->first;
                const StrA & uniformName = iter->second;

                GN_ASSERT( effectDesc.uniforms.end() != effectDesc.uniforms.find( uniformName ) );

                if( uniformName == up.parameterName )
                {
                    BindingLocation b = { ipass, gpparam.uniforms[shaderParameterName] };
                    GN_ASSERT( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.stage );
                    up.bindings.append( b );
                }
            }
        }

        if( up.bindings.empty() )
        {
            GN_WARN(sLogger)( "Unused uniform parameter '%s' in effect '%s'.",
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
// EffectResourceInternal
// *****************************************************************************

class EffectResourceInternal : public EffectResource
{
    EffectResourceInternal( GpuResourceDatabase & db )
        : EffectResource( db )
    {
    }

public:

    static GpuResource *
    sCreateInstance( GpuResourceDatabase & db )
    {
        return new EffectResourceInternal( db );
    }
};

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::registerEffectResourceFactory( GpuResourceDatabase & db )
{
    if( db.hasResourceFactory( EffectResource::guid() ) ) return true;

    GpuResourceFactory factory = { &EffectResourceInternal::sCreateInstance };

    return db.registerResourceFactory( EffectResource::guid(), "Effect Resource", factory );
}

// *****************************************************************************
// GN::gfx::EffectResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectResource::EffectResource( GpuResourceDatabase & db )
    : GpuResource( db )
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
AutoRef<EffectResource> GN::gfx::EffectResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    GN_UNUSED_PARAM( db );
    GN_UNUSED_PARAM( filename );
    GN_UNIMPL();

    return AutoRef<EffectResource>::NULLREF;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectResource::reset( const EffectResourceDesc * desc ) { return mImpl->reset( desc ); }

size_t GN::gfx::EffectResource::getNumPasses() const { return mImpl->getNumPasses(); }

size_t GN::gfx::EffectResource::getNumTextures() const { return mImpl->getNumTextures(); }
size_t GN::gfx::EffectResource::findTexture( const char * name ) const { return mImpl->findTexture( name ); }
const GN::gfx::EffectResource::TextureProperties & GN::gfx::EffectResource::getTextureProperties( size_t i ) const { return mImpl->getTextureProperties( i ); }

size_t GN::gfx::EffectResource::getNumUniforms() const { return mImpl->getNumUniforms(); }
size_t GN::gfx::EffectResource::findUniform( const char * name ) const { return mImpl->findUniform( name ); }
const GN::gfx::EffectResource::UniformProperties & GN::gfx::EffectResource::getUniformProperties( size_t i ) const { return mImpl->getUniformProperties( i ); }

const EffectResourceDesc::EffectRenderStateDesc & GN::gfx::EffectResource::getRenderStates( size_t pass ) const { return mImpl->getRenderStates( pass ); }

void GN::gfx::EffectResource::applyToContext( size_t pass, GpuContext & gc ) const { return mImpl->applyToContext( pass, gc ); }
