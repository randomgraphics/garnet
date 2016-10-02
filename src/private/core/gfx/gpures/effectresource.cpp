#include "pch.h"
#include "effectresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpures.EffectResource");

typedef GN::gfx::EffectResourceDesc::ShaderPrerequisites ShaderPrerequisites;
typedef GN::gfx::EffectResourceDesc::EffectUniformDesc EffectUniformDesc;
typedef GN::gfx::EffectResourceDesc::EffectTextureDesc EffectTextureDesc;
typedef GN::gfx::EffectResourceDesc::EffectAttributeDesc EffectAttributeDesc;
typedef GN::gfx::EffectResourceDesc::EffectGpuProgramDesc EffectGpuProgramDesc;
typedef GN::gfx::EffectResourceDesc::EffectRenderStateDesc EffectRenderStateDesc;
typedef GN::gfx::EffectResourceDesc::EffectPassDesc EffectPassDesc;
typedef GN::gfx::EffectResourceDesc::EffectTechniqueDesc EffectTechniqueDesc;

#define LOAD_GPU_PROGRAM_ONDEMAND 1

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

    // check shader model
    if( 0 == (desc.gpd.shaderModels & caps.shaderModels) )
    {
        return false;
    }

    // check Cg language support.
    if( GpuProgramLanguage::CG == desc.gpd.lang && !caps.cg )
    {
        return false;
    }

    /* check vertex shader
    if( desc.gpd.vs.source && !caps.gpuProgramLanguage[ShaderStage::VS][desc.gpd.lang] )
    {
        return false;
    }

    // check geometry shader
    if( desc.gpd.gs.source && !caps.gpuProgramLanguage[ShaderStage::GS][desc.gpd.lang] )
    {
        return false;
    }

    // check pixel shader
    if( desc.gpd.ps.source && !caps.gpuProgramLanguage[ShaderStage::PS][desc.gpd.lang] )
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
    }*/

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
    const EffectGpuProgramDesc & programDesc,
    const char                 * programName,
    const GpuProgram           & program )
{
    const GpuProgramParameterDesc & param = program.getParameterDesc();

    for( const StringMap<char,StrA>::KeyValuePair * iter = programDesc.textures.first();
         iter != NULL;
         iter = programDesc.textures.next( iter ) )
    {
        const StrA & shaderParameterName = iter->key;
        const StrA & textureName = iter->value;

        if( GPU_PROGRAM_PARAMETER_NOT_FOUND == param.textures[shaderParameterName] )
        {
            GN_ERROR(sLogger)( "Invalid GPU program parameter named '%s' is referenced in shader '%s'.",
                shaderParameterName.rawptr(),
                programName );
            return false;
        }
        else if( NULL == effectDesc.textures.find( textureName ) )
        {
            GN_ERROR(sLogger)( "Invalid texture named '%s' is referenced in shader '%s'.",
                shaderParameterName.rawptr(),
                programName );
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
    const EffectGpuProgramDesc & programDesc,
    const char                 * programName,
    const GpuProgram           & program )
{
    const GpuProgramParameterDesc & param = program.getParameterDesc();

    for( const StringMap<char,StrA>::KeyValuePair * iter = programDesc.uniforms.first();
         iter != NULL;
         iter = programDesc.uniforms.next( iter ) )
    {
        const StrA & shaderParameterName = iter->key;
        const StrA & uniformName = iter->value;

        if( GPU_PROGRAM_PARAMETER_NOT_FOUND == param.uniforms[shaderParameterName] )
        {
            GN_ERROR(sLogger)( "Invalid GPU program parameter named '%s' is referenced in shader '%s'.",
                shaderParameterName.rawptr(),
                programName );
            return false;
        }
        else if( NULL == effectDesc.uniforms.find( uniformName ) )
        {
            GN_ERROR(sLogger)( "Invalid uniform named '%s' is referenced in shader '%s'.",
                shaderParameterName.rawptr(),
                programName );
            return false;
        }
    }

    return true;
}

static bool sAttributeNameEqual( const char * name1, const char * name2 )
{
    // Treat name like 'POSITION0' as 'POSITION'
    size_t len1 = str::length( name1 );
    if( len1 >= 2 && '0' == name1[len1-1] && ( name1[len1-2] < '0' || name1[len1-2] > '9' ) )
    {
        len1--;
    }

    size_t len2 = str::length( name2 );
    if( len2 >= 2 && '0' == name2[len2-1] && ( name2[len2-2] < '0' || name2[len2-2] > '9' ) )
    {
        len2--;
    }

    return len1 == len2 && 0 == str::compareI( name1, name2, len1 );
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
uint32 GN::gfx::EffectResource::Impl::findTexture( const char * name ) const
{
    if( NULL == name || 0 == *name ) return PARAMETER_NOT_FOUND;

    for( uint32 i = 0; i < mTextures.size(); ++i )
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
uint32 GN::gfx::EffectResource::Impl::findUniform( const char * name ) const
{
    if( NULL == name || 0 == *name ) return PARAMETER_NOT_FOUND;

    for( uint32 i = 0; i < mUniforms.size(); ++i )
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
uint32 GN::gfx::EffectResource::Impl::findAttribute( const char * name ) const
{
    for( uint32 i = 0; i < mAttributes.size(); ++i )
    {
        if( sAttributeNameEqual( name, mAttributes[i].parameterName ) )
        {
            return i;
        }
    }

    return PARAMETER_NOT_FOUND;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResource::Impl::applyToContext( uint32 passIndex, GpuContext & gc ) const
{
    if( passIndex >= mPasses.size() )
    {
        GN_ERROR(sLogger)( "Pass index is too large: %u", passIndex );
        return;
    }

    const RenderPass & pass = mPasses[passIndex];

    const GpuProgramItem & gpitem = mPrograms[pass.gpuProgramIndex];

    gc.gpuProgram = gpitem.prog;

    // Note: render states are handled in model class
}

// *****************************************************************************
// GN::gfx::EffectResource::Impl - private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectResource::Impl::init( const EffectResourceDesc & desc )
{
#if !LOAD_GPU_PROGRAM_ONDEMAND
    if( !initGpuPrograms( desc ) ) return false;
#endif
    if( !initTechniques( desc ) ) return false;
    if( !initTextures( desc ) ) return false;
    if( !initUniforms( desc ) ) return false;
    if( !initAttributes( desc ) ) return false;

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
    mAttributes.clear();
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initGpuPrograms(
    const EffectResourceDesc & effectDesc )
{
    for( const StringMap<char,EffectGpuProgramDesc>::KeyValuePair * iter = effectDesc.gpuprograms.first();
         iter != NULL;
         iter = effectDesc.gpuprograms.next( iter ) )
    {
        const StrA                 & programName = iter->key;
        const EffectGpuProgramDesc & programDesc = iter->value;
        initGpuProgram( effectDesc, programName, programDesc );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initGpuProgram(
    const EffectResourceDesc   & effectDesc,
    const StrA                 & programName,
    const EffectGpuProgramDesc & programDesc )
{
    GN_VERBOSE(sLogger)( "Initialize GPU program: name=%s, language=%s", programName.rawptr(), programDesc.gpd.lang.toString() );

    Gpu & gpu = getGdb().getGpu();

    // check GPU program requirements.
    // Note: it is expected scenario that some shaders are not supported by current hardware.
    //       So here only a verbose, instead of error, message is issued.
    if( !sCheckGpuCaps( gpu, programDesc ) )
    {
        GN_VERBOSE(sLogger)( "shader '%s' is skipped due to missing GPU caps.", programName.rawptr() );
        return false;
    }

    // create GPU program
    GpuProgramItem gpitem;
    gpitem.name = programName;
    gpitem.prog.attach( gpu.createGpuProgram( programDesc.gpd ) );
    if( !gpitem.prog ) return false;

    // check textures and uniforms
    if( !sCheckShaderTextures( effectDesc, programDesc, programName, *gpitem.prog ) ) return false;
    if( !sCheckShaderUniforms( effectDesc, programDesc, programName, *gpitem.prog ) ) return false;

    // add to GPU program array
    mPrograms.append( gpitem );
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
        GN_ERROR(sLogger)( "No valid technique is found in effect '%s'", getEffectName() );
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
    for( uint32 ipass = 0; ipass < techDesc.passes.size(); ++ipass )
    {
        const EffectPassDesc & passDesc = techDesc.passes[ipass];

        const StrA & programName = passDesc.gpuprogram; // shader techName alias for easy referencing

        RenderPass & p = mPasses[ipass];

        // look up GPU program
        p.gpuProgramIndex = findGpuProgram( effectDesc, programName );
        if( PARAMETER_NOT_FOUND == p.gpuProgramIndex )
        {
            // Shader is not found. Let's find out why. See if it is expected.

            // Look up GPU program description
            const EffectGpuProgramDesc * programDesc = effectDesc.gpuprograms.find( programName );
            if( NULL == programDesc )
            {
                GN_ERROR(sLogger)(
                    "Technique '%s' referencs non-exist shader name '%s' in pass %u",
                    techName.rawptr(),
                    programName.rawptr(),
                    ipass );
            }

            // check GPU caps against shader requirments
            else if( !sCheckGpuCaps( gpu, *programDesc ) )
            {
                // Note: it is expected scenario that some shaders are not supported by current hardware.
                //       So here only a verbose, instead of error, message is issued.
                GN_VERBOSE(sLogger)(
                    "Technique '%s' is skipped because shader '%s', which is referenced by the technique in pass %u, "
                    "is not supported by current graphics hardware.",
                    techName.rawptr(),
                    programName.rawptr(),
                    ipass );
                return false;
            }
            else
            {
                GN_ERROR(sLogger)(
                    "Shader '%s' referenced by technique '%s' in pass %u is not properly initialized",
                    programName.rawptr(),
                    techName.rawptr(),
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
        for( uint32 ipass = 0; ipass < mPasses.size(); ++ipass )
        {
            const GpuProgramItem & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc * programDesc = effectDesc.gpuprograms.find( gpitem.name );

            for( const StringMap<char,StrA>::KeyValuePair * iter = programDesc->textures.first();
                 iter != NULL;
                 iter = programDesc->textures.next( iter ) )
            {
                const StrA & shaderParameterName = iter->key;
                const StrA & textureName = iter->value;

                GN_ASSERT( NULL != effectDesc.textures.find( textureName ) );

                if( textureName == tp.parameterName )
                {
                    BindingLocation b = { ipass, gpparam.textures[shaderParameterName] };
                    GN_ASSERT( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.gpuProgramParameterIndex );
                    tp.bindings.append( b );
                }
            }
        }

        if( tp.bindings.empty() )
        {
            GN_WARN(sLogger)( "Unused texture parameter '%s' in effect '%s'.",
                tp.parameterName.rawptr(),
                getEffectName() );
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
        for( uint32 ipass = 0; ipass < mPasses.size(); ++ipass )
        {
            const GpuProgramItem          & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc    * programDesc = effectDesc.gpuprograms.find( gpitem.name );

            for( const StringMap<char,StrA>::KeyValuePair * iter = programDesc->uniforms.first();
                 iter != NULL;
                 iter = programDesc->uniforms.next( iter ) )
            {
                const StrA & shaderParameterName = iter->key;
                const StrA & uniformName = iter->value;

                GN_ASSERT( NULL != effectDesc.uniforms.find( uniformName ) );

                if( uniformName == up.parameterName )
                {
                    BindingLocation b = { ipass, gpparam.uniforms[shaderParameterName] };
                    GN_ASSERT( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.gpuProgramParameterIndex );
                    up.bindings.append( b );
                }
            }
        }

        if( up.bindings.empty() )
        {
            GN_WARN(sLogger)( "Unused uniform parameter '%s' in effect '%s'.",
                up.parameterName.rawptr(),
                getEffectName() );
        }

        mUniforms.append( up );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::EffectResource::Impl::initAttributes(
    const EffectResourceDesc  & effectDesc )
{
    for( const StringMap<char,EffectAttributeDesc>::KeyValuePair * iter = effectDesc.attributes.first();
         iter != NULL;
         iter = effectDesc.attributes.next( iter ) )
    {
        AttributeProperties ap;

        ap.parameterName = iter->key;

        // setup attribute binding point array
        for( uint32 ipass = 0; ipass < mPasses.size(); ++ipass )
        {
            const GpuProgramItem          & gpitem = mPrograms[mPasses[ipass].gpuProgramIndex];
            const GpuProgramParameterDesc & gpparam = gpitem.prog->getParameterDesc();
            const EffectGpuProgramDesc    * programDesc = effectDesc.gpuprograms.find( gpitem.name );

            for( const StringMap<char,StrA>::KeyValuePair * iter = programDesc->attributes.first();
                 iter != NULL;
                 iter = programDesc->attributes.next( iter ) )
            {
                const StrA & shaderParameterName = iter->key;
                const StrA & attributeName = iter->value;

                if( sAttributeNameEqual( attributeName, ap.parameterName ) )
                {
                    BindingLocation b = { ipass, gpparam.attributes[shaderParameterName] };
                    if( GPU_PROGRAM_PARAMETER_NOT_FOUND != b.gpuProgramParameterIndex )
                    {
                        ap.bindings.append( b );
                    }
                    else
                    {
                        GN_ERROR(sLogger)( "Effect attribute '%s' is binding to invalid GPU program parameter '%s'",
                            attributeName.rawptr(),
                            shaderParameterName.rawptr() );
                    }
                }
            }
        }

        if( ap.bindings.empty() )
        {
            GN_WARN(sLogger)( "Unused attribute parameter '%s' in effect '%s'.",
                ap.parameterName.rawptr(),
                getEffectName() );
        }

        mAttributes.append( ap );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
uint32 GN::gfx::EffectResource::Impl::findGpuProgram(
    const EffectResourceDesc & effectDesc,
    const StrA               & programName )
{
    for( uint32 i = 0; i < mPrograms.size(); ++i )
    {
        if( mPrograms[i].name == programName ) return i;
    }

#if LOAD_GPU_PROGRAM_ONDEMAND
    const EffectGpuProgramDesc * programDesc = effectDesc.gpuprograms.find( programName );
    if( NULL != programDesc && initGpuProgram( effectDesc, programName, *programDesc ) )
    {
        GN_ASSERT( mPrograms.size() > 0 );
        return (uint32)(mPrograms.size() - 1);
    }
#endif

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
        GpuResource * res = new EffectResourceInternal( db );
        res->incref();
        return res;
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
            fp->name().rawptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.rawptr() );
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

uint32 GN::gfx::EffectResource::numPasses() const { return mImpl->numPasses(); }

uint32 GN::gfx::EffectResource::numTextures() const { return mImpl->numTextures(); }
uint32 GN::gfx::EffectResource::findTexture( const char * name ) const { return mImpl->findTexture( name ); }
const GN::gfx::EffectResource::TextureProperties & GN::gfx::EffectResource::textureProperties( uint32 i ) const { return mImpl->textureProperties( i ); }

uint32 GN::gfx::EffectResource::numUniforms() const { return mImpl->numUniforms(); }
uint32 GN::gfx::EffectResource::findUniform( const char * name ) const { return mImpl->findUniform( name ); }
const GN::gfx::EffectResource::UniformProperties & GN::gfx::EffectResource::uniformProperties( uint32 i ) const { return mImpl->uniformProperties( i ); }

uint32 GN::gfx::EffectResource::numAttributes() const { return mImpl->numAttributes(); }
uint32 GN::gfx::EffectResource::findAttribute( const char * name ) const { return mImpl->findAttribute( name ); }
const GN::gfx::EffectResource::AttributeProperties & GN::gfx::EffectResource::attributeProperties( uint32 i ) const { return mImpl->attributeProperties( i ); }

const EffectResourceDesc::EffectRenderStateDesc & GN::gfx::EffectResource::renderStates( uint32 pass ) const { return mImpl->renderStates( pass ); }

void GN::gfx::EffectResource::applyToContext( uint32 pass, GpuContext & gc ) const { return mImpl->applyToContext( pass, gc ); }
