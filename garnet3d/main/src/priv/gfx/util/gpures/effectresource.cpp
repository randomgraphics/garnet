#include "pch.h"
#include "effectresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpures");

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
    const GpuCaps & caps = r.GetCaps();

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
    const EffectResourceDesc   & effectDesc,
    const EffectGpuProgramDesc & shaderDesc,
    const char                 * shaderName,
    const GpuProgram           & program )
{
    const GpuProgramParameterDesc & param = program.getParameterDesc();

    for( const StringMap<char,StrA>::KeyValuePair * iter = shaderDesc.textures.First();
         iter != NULL;
         iter = shaderDesc.textures.Next( iter ) )
    {
        const StrA & shaderParameterName = iter->key;
        const StrA & textureName = iter->value;

        if( GPU_PROGRAM_PARAMETER_NOT_FOUND == param.textures[shaderParameterName] )
        {
            GN_ERROR(sLogger)( "Invalid GPU program parameter named '%s' is referenced in shader '%s'.",
                shaderParameterName.ToRawPtr(),
                shaderName );
            return false;
        }
        else if( NULL == effectDesc.textures.Find( textureName ) )
        {
            GN_ERROR(sLogger)( "Invalid texture named '%s' is referenced in shader '%s'.",
                shaderParameterName.ToRawPtr(),
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

    for( const StringMap<char,StrA>::KeyValuePair * iter = shaderDesc.uniforms.First();
         iter != NULL;
         iter = shaderDesc.uniforms.Next( iter ) )
    {
        const StrA & shaderParameterName = iter->key;
        const StrA & uniformName = iter->value;

        if( GPU_PROGRAM_PARAMETER_NOT_FOUND == param.uniforms[shaderParameterName] )
        {
            GN_ERROR(sLogger)( "Invalid GPU program parameter named '%s' is referenced in shader '%s'.",
                shaderParameterName.ToRawPtr(),
                shaderName );
            return false;
        }
        else if( NULL == effectDesc.uniforms.Find( uniformName ) )
        {
            GN_ERROR(sLogger)( "Invalid uniform named '%s' is referenced in shader '%s'.",
                shaderParameterName.ToRawPtr(),
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
    Clear();

    bool ok;
    if( desc && !Init( *desc ) )
    {
        Clear();
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

    for( size_t i = 0; i < mTextures.Size(); ++i )
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

    for( size_t i = 0; i < mUniforms.Size(); ++i )
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
    if( passIndex >= mPasses.Size() )
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
bool GN::gfx::EffectResource::Impl::Init( const EffectResourceDesc & desc )
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
void GN::gfx::EffectResource::Impl::Clear()
{
    mPrograms.Clear();
    mPasses.Clear();
    mTextures.Clear();
    mUniforms.Clear();
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initGpuPrograms(
    const EffectResourceDesc & effectDesc )
{
    Gpu & gpu = database().gpu();

    for( const StringMap<char,EffectGpuProgramDesc>::KeyValuePair * iter = effectDesc.gpuprograms.First();
         iter != NULL;
         iter = effectDesc.gpuprograms.Next( iter ) )
    {
        const StrA                 & shaderName = iter->key;
        const EffectGpuProgramDesc & shaderDesc = iter->value;

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
        gpitem.prog.Attach( gpu.createGpuProgram( shaderDesc.gpd ) );
        if( !gpitem.prog ) continue;

        // check textures and uniforms
        if( !sCheckShaderTextures( effectDesc, shaderDesc, shaderName, *gpitem.prog ) ) continue;
        if( !sCheckShaderUniforms( effectDesc, shaderDesc, shaderName, *gpitem.prog ) ) continue;

        // add to GPU program array
        if( gpitem.prog ) mPrograms.Append( gpitem );
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

    for( const StringMap<char,EffectTechniqueDesc>::KeyValuePair * iter = effectDesc.techniques.First();
         iter != NULL;
         iter = effectDesc.techniques.Next( iter ) )
    {
        const StrA                & techName = iter->key;
        const EffectTechniqueDesc & techDesc = iter->value;

        // ignore the technique with lower quality
        if( techDesc.quality <= currentQuality )
        {
            continue;
        }

        DynaArray<RenderPass> passes = mPasses;
        mPasses.Clear();

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
    mPasses.Resize( techDesc.passes.Size() );

    // get common render state for the technique
    EffectRenderStateDesc commonRenderStates;
    sMergeRenderStates( commonRenderStates, techDesc.renderstates, effectDesc.renderstates );

    Gpu & gpu = database().gpu();

    // initialize each pass
    for( size_t ipass = 0; ipass < techDesc.passes.Size(); ++ipass )
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
            const EffectGpuProgramDesc * shaderDesc = effectDesc.gpuprograms.Find( shaderName );
            if( NULL == shaderDesc )
            {
                GN_ERROR(sLogger)(
                    "Technique '%s' referencs non-exist shader name '%s' in pass %u",
                    techName.ToRawPtr(),
                    shaderName.ToRawPtr(),
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
                    techName.ToRawPtr(),
                    shaderName.ToRawPtr(),
                    ipass );
                return false;
            }
            else
            {
                GN_ERROR(sLogger)(
                    "Shader '%s' referenced by technique '%s' in pass %u is not properly initialized",
                    shaderName.ToRawPtr(),
                    techName.ToRawPtr(),
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
    for( const StringMap<char,EffectTextureDesc>::KeyValuePair * iter = effectDesc.textures.First();
         iter != NULL;
         iter = effectDesc.textures.Next( iter ) )
    {
        TextureProperties tp;

        tp.parameterName = iter->key;
        tp.sampler = iter->value.sampler;

        // setup texture binding point array
        for( size_t ipass = 0; ipass < mPasses.Size(); ++ipass )
        {
            const GpuProgramItem & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc * shaderDesc = effectDesc.gpuprograms.Find( gpitem.name );

            for( const StringMap<char,StrA>::KeyValuePair * iter = shaderDesc->textures.First();
                 iter != NULL;
                 iter = shaderDesc->textures.Next( iter ) )
            {
                const StrA & shaderParameterName = iter->key;
                const StrA & textureName = iter->value;

                GN_ASSERT( NULL != effectDesc.textures.Find( textureName ) );

                if( textureName == tp.parameterName )
                {
                    BindingLocation b = { ipass, gpparam.textures[shaderParameterName] };
                    GN_ASSERT( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.stage );
                    tp.bindings.Append( b );
                }
            }
        }

        if( tp.bindings.Empty() )
        {
            GN_WARN(sLogger)( "Unused texture parameter '%s' in effect '%s'.",
                tp.parameterName.ToRawPtr(),
                effectName() );
        }

        mTextures.Append( tp );
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
    for( const StringMap<char,EffectUniformDesc>::KeyValuePair * iter = effectDesc.uniforms.First();
         iter != NULL;
         iter = effectDesc.uniforms.Next( iter ) )
    {
        UniformProperties up;

        up.parameterName = iter->key;

        const EffectUniformDesc & eud = iter->value;
        up.size = eud.size;

        // setup uniform binding point array
        for( size_t ipass = 0; ipass < mPasses.Size(); ++ipass )
        {
            const GpuProgramItem          & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc    * shaderDesc = effectDesc.gpuprograms.Find( gpitem.name );

            for( const StringMap<char,StrA>::KeyValuePair * iter = shaderDesc->uniforms.First();
                 iter != NULL;
                 iter = shaderDesc->uniforms.Next( iter ) )
            {
                const StrA & shaderParameterName = iter->key;
                const StrA & uniformName = iter->value;

                GN_ASSERT( NULL != effectDesc.uniforms.Find( uniformName ) );

                if( uniformName == up.parameterName )
                {
                    BindingLocation b = { ipass, gpparam.uniforms[shaderParameterName] };
                    GN_ASSERT( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.stage );
                    up.bindings.Append( b );
                }
            }
        }

        if( up.bindings.Empty() )
        {
            GN_WARN(sLogger)( "Unused uniform parameter '%s' in effect '%s'.",
                up.parameterName.ToRawPtr(),
                effectName() );
        }

        mUniforms.Append( up );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::findGpuProgram( const StrA & shaderName ) const
{
    for( size_t i = 0; i < mPrograms.Size(); ++i )
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
    StrA abspath = fs::ResolvePath( fs::GetCurrentDir(), filename );
    filename = abspath;

    // Try search for existing resource again with full path
    resource = db.findResource<EffectResource>( filename );
    if( resource ) return resource;

    // load new effect from file
    GN_INFO(sLogger)( "Load effect from file: %s", filename );

    // open XML file
    AutoObjPtr<File> fp( fs::OpenFile( filename, "rt" ) );
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
            fp->Name(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.ToRawPtr() );
        return AutoRef<EffectResource>::NULLREF;
    }
    fp.Clear();
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

size_t GN::gfx::EffectResource::getNumPasses() const { return mImpl->getNumPasses(); }

size_t GN::gfx::EffectResource::getNumTextures() const { return mImpl->getNumTextures(); }
size_t GN::gfx::EffectResource::findTexture( const char * name ) const { return mImpl->findTexture( name ); }
const GN::gfx::EffectResource::TextureProperties & GN::gfx::EffectResource::getTextureProperties( size_t i ) const { return mImpl->getTextureProperties( i ); }

size_t GN::gfx::EffectResource::getNumUniforms() const { return mImpl->getNumUniforms(); }
size_t GN::gfx::EffectResource::findUniform( const char * name ) const { return mImpl->findUniform( name ); }
const GN::gfx::EffectResource::UniformProperties & GN::gfx::EffectResource::getUniformProperties( size_t i ) const { return mImpl->getUniformProperties( i ); }

const EffectResourceDesc::EffectRenderStateDesc & GN::gfx::EffectResource::getRenderStates( size_t pass ) const { return mImpl->getRenderStates( pass ); }

void GN::gfx::EffectResource::applyToContext( size_t pass, GpuContext & gc ) const { return mImpl->applyToContext( pass, gc ); }
