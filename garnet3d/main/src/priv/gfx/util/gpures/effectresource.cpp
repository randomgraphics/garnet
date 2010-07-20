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
    const GpuCaps & caps = r.caps();

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

    MERGE_SINGLE_RENDER_STATE( fillMode );
    MERGE_SINGLE_RENDER_STATE( cullMode );
    MERGE_SINGLE_RENDER_STATE( frontFace );
    MERGE_SINGLE_RENDER_STATE( msaaEnabled );

    MERGE_SINGLE_RENDER_STATE( independentAlphaBlending );
    for( int i = 0; i < GpuContext::MAX_COLOR_RENDER_TARGETS; ++i )
    {
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendEnabled );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendSrc );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendDst );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendOp );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendAlphaSrc );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendAlphaDst );
        MERGE_SINGLE_RENDER_STATE( alphaBlend[i].blendAlphaOp );
    }
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

    for( const StringMap<char,StrA>::KeyValuePair * iter = shaderDesc.textures.first();
         iter != NULL;
         iter = shaderDesc.textures.next( iter ) )
    {
        const StrA & shaderParameterName = iter->key;
        const StrA & textureName = iter->value;

        if( GPU_PROGRAM_PARAMETER_NOT_FOUND == param.textures[shaderParameterName] )
        {
            GN_ERROR(sLogger)( "Invalid GPU program parameter named '%s' is referenced in shader '%s'.",
                shaderParameterName.cptr(),
                shaderName );
            return false;
        }
        else if( NULL == effectDesc.textures.find( textureName ) )
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
    const EffectResourceDesc   & effectDesc,
    const EffectGpuProgramDesc & shaderDesc,
    const char                 * shaderName,
    const GpuProgram           & program )
{
    const GpuProgramParameterDesc & param = program.getParameterDesc();

    for( const StringMap<char,StrA>::KeyValuePair * iter = shaderDesc.uniforms.first();
         iter != NULL;
         iter = shaderDesc.uniforms.next( iter ) )
    {
        const StrA & shaderParameterName = iter->key;
        const StrA & uniformName = iter->value;

        if( GPU_PROGRAM_PARAMETER_NOT_FOUND == param.uniforms[shaderParameterName] )
        {
            GN_ERROR(sLogger)( "Invalid GPU program parameter named '%s' is referenced in shader '%s'.",
                shaderParameterName.cptr(),
                shaderName );
            return false;
        }
        else if( NULL == effectDesc.uniforms.find( uniformName ) )
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
    Gpu & gpu = getGdb().getGpu();

    for( const StringMap<char,EffectGpuProgramDesc>::KeyValuePair * iter = effectDesc.gpuprograms.first();
         iter != NULL;
         iter = effectDesc.gpuprograms.next( iter ) )
    {
        const StrA                 & shaderName = iter->key;
        const EffectGpuProgramDesc & shaderDesc = iter->value;

        // check shader requirements.
        // Note: it is expected scenario that some shaders are not supported by current hardware.
        //       So here only a verbose, instead of error, message is issued.
        if( !sCheckGpuCaps( gpu, shaderDesc ) )
        {
            GN_VERBOSE(sLogger)( "shader '%s' is skipped due to missing GPU caps.", shaderName.cptr() );
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

    for( const EffectTechniqueDesc * techDesc = effectDesc.techniques.first();
         techDesc != NULL;
         techDesc = effectDesc.techniques.next( techDesc ) )
    {
        // ignore the technique with lower quality
        if( techDesc->quality <= currentQuality )
        {
            continue;
        }

        DynaArray<RenderPass> passes = mPasses;
        mPasses.clear();

        if( initTech( effectDesc, techDesc->name, *techDesc ) )
        {
            currentQuality = techDesc->quality;
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

    Gpu & gpu = getGdb().getGpu();

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
            const EffectGpuProgramDesc * shaderDesc = effectDesc.gpuprograms.find( shaderName );
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
    for( const StringMap<char,EffectTextureDesc>::KeyValuePair * iter = effectDesc.textures.first();
         iter != NULL;
         iter = effectDesc.textures.next( iter ) )
    {
        TextureProperties tp;

        tp.parameterName = iter->key;
        tp.sampler = iter->value.sampler;

        // setup texture binding point array
        for( size_t ipass = 0; ipass < mPasses.size(); ++ipass )
        {
            const GpuProgramItem & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc * shaderDesc = effectDesc.gpuprograms.find( gpitem.name );

            for( const StringMap<char,StrA>::KeyValuePair * iter = shaderDesc->textures.first();
                 iter != NULL;
                 iter = shaderDesc->textures.next( iter ) )
            {
                const StrA & shaderParameterName = iter->key;
                const StrA & textureName = iter->value;

                GN_ASSERT( NULL != effectDesc.textures.find( textureName ) );

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
    for( const StringMap<char,EffectUniformDesc>::KeyValuePair * iter = effectDesc.uniforms.first();
         iter != NULL;
         iter = effectDesc.uniforms.next( iter ) )
    {
        UniformProperties up;

        up.parameterName = iter->key;

        const EffectUniformDesc & eud = iter->value;
        up.size = eud.size;

        // setup uniform binding point array
        for( size_t ipass = 0; ipass < mPasses.size(); ++ipass )
        {
            const GpuProgramItem          & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc    * shaderDesc = effectDesc.gpuprograms.find( gpitem.name );

            for( const StringMap<char,StrA>::KeyValuePair * iter = shaderDesc->uniforms.first();
                 iter != NULL;
                 iter = shaderDesc->uniforms.next( iter ) )
            {
                const StrA & shaderParameterName = iter->key;
                const StrA & uniformName = iter->value;

                GN_ASSERT( NULL != effectDesc.uniforms.find( uniformName ) );

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
    if( NULL == filename )
    {
        GN_INFO(sLogger)( "Null filename string." );
        return AutoRef<EffectResource>::NULLREF;
    }

    // Reuse existing resource, if possible
    AutoRef<EffectResource> resource( db.findResource<EffectResource>( filename ) );
    if( resource ) return resource;

    // convert to full (absolute) path
    StrA abspath = fs::resolvePath( fs::getCurrentDir(), filename );
    filename = abspath;

    // Try search for existing resource again with full path
    resource = db.findResource<EffectResource>( filename );
    if( resource ) return resource;

    // load new effect from file
    GN_INFO(sLogger)( "Load effect from file: %s", filename );

    // open XML file
    AutoObjPtr<File> fp( fs::openFile( filename, "rt" ) );
    if( !fp ) return AutoRef<EffectResource>::NULLREF;
    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, *fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp->name(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return AutoRef<EffectResource>::NULLREF;
    }
    fp.clear();
    GN_ASSERT( xpr.root );

    // load descriptor from file
    EffectResourceDesc desc;
    if( !desc.loadFromXml( *xpr.root ) ) return AutoRef<EffectResource>::NULLREF;

    // create new resource
    resource = db.createResource<EffectResource>( filename );
    if( 0 == resource || !resource->reset( &desc ) ) return AutoRef<EffectResource>::NULLREF;

    // done
    return resource;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectResource::reset( const EffectResourceDesc * desc ) { return mImpl->reset( desc ); }

size_t GN::gfx::EffectResource::numPasses() const { return mImpl->numPasses(); }

size_t GN::gfx::EffectResource::numTextures() const { return mImpl->numTextures(); }
size_t GN::gfx::EffectResource::findTexture( const char * name ) const { return mImpl->findTexture( name ); }
const GN::gfx::EffectResource::TextureProperties & GN::gfx::EffectResource::textureProperties( size_t i ) const { return mImpl->textureProperties( i ); }

size_t GN::gfx::EffectResource::numUniforms() const { return mImpl->numUniforms(); }
size_t GN::gfx::EffectResource::findUniform( const char * name ) const { return mImpl->findUniform( name ); }
const GN::gfx::EffectResource::UniformProperties & GN::gfx::EffectResource::uniformProperties( size_t i ) const { return mImpl->uniformProperties( i ); }

const EffectResourceDesc::EffectRenderStateDesc & GN::gfx::EffectResource::renderStates( size_t pass ) const { return mImpl->renderStates( pass ); }

void GN::gfx::EffectResource::applyToContext( size_t pass, GpuContext & gc ) const { return mImpl->applyToContext( pass, gc ); }
