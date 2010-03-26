#include "pch.h"

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
struct EnumNames
{
    int          value;
    const char * name;
};
#define ENUM_TABLE_BEGIN( name ) { -1, name }
#define ENUM_TABLE_END { -1, NULL }

template<class RESULT_TYPE>
static bool sParseEnum(
    RESULT_TYPE     & result,
    const char      * name,
    const EnumNames * table )
{
    const EnumNames * header = table;

    ++table;

    while( table->name )
    {
        if( 0 == StringCompareI( name, table->name ) )
        {
            result = (RESULT_TYPE)table->value;
            return true;
        }

        ++table;
    }

    GN_ERROR(sLogger)( "Invalid %s value: %s", header->name, name );
    return false;
}

template<class RESULT_TYPE>
static RESULT_TYPE sParseEnum(
    const char        * name,
    const EnumNames   * table,
    const RESULT_TYPE & defaultValue )
{
    RESULT_TYPE result;
    if( sParseEnum( result, name, table ) )
    {
        return result;
    }
    else
    {
        return defaultValue;
    }
}

//
// post error message
// -----------------------------------------------------------------------------
static void sPostError( const XmlNode & node, const StrA & msg )
{
    const XmlElement * e = node.ToElement();
    if( e )
    {
        GN_ERROR(sLogger)( "Effect XML error: element <%s> - %s", e->name.ToRawPtr(), msg.ToRawPtr() );
    }
    else
    {
        GN_ERROR(sLogger)( "Effect XML error: %s", msg.ToRawPtr() );
    }
}

//
// get value of specific attribute
// -----------------------------------------------------------------------------
static const char * sGetAttrib(
    const XmlElement & node,
    const char * attribName,
    const char * defaultValue = NULL )
{
    const XmlAttrib * a = node.FindAttrib( attribName );
    return a ? a->value.ToRawPtr() : defaultValue;
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
template<typename T>
static T sGetIntAttrib( const XmlElement & node, const char * attribName, T defaultValue )
{
    const XmlAttrib * a = node.FindAttrib( attribName );

    T result;

    if( !a || 0 == String2Integer<T>( result, a->value.ToRawPtr() ) )
        return defaultValue;
    else
        return result;
}

//
// get value of boolean attribute
// -----------------------------------------------------------------------------
static bool sGetBoolAttrib( const XmlElement & node, const char * attribName, bool defaultValue )
{
    const XmlAttrib * a = node.FindAttrib( attribName );
    if( !a ) return defaultValue;

    if( 0 == StringCompareI( "1", a->value.ToRawPtr() ) ||
        0 == StringCompareI( "true", a->value.ToRawPtr() ) )
    {
        return true;
    }
    else if( 0 == StringCompareI( "0", a->value.ToRawPtr() ) ||
             0 == StringCompareI( "false", a->value.ToRawPtr() ) )
    {
        return false;
    }
    else
    {
        return defaultValue;
    }
}

//
// get value of name attribute
// -----------------------------------------------------------------------------
static const char * sGetItemName( const XmlElement & node, const char * nodeType )
{
    XmlAttrib * a = node.FindAttrib( "name" );
    if( !a )
    {
        sPostError( node, StringFormat("Unnamed %s node. Ignored.", nodeType) );
        return 0;
    }
    return a->value.ToRawPtr();
}

//
//
// -----------------------------------------------------------------------------
static void sParseTexture( EffectResourceDesc & desc, const XmlElement & node )
{
    static const EnumNames ADDRESS_MODE_TABLE[] =
    {
        ENUM_TABLE_BEGIN( "address mode" ),

        { SamplerDesc::ADDRESS_REPEAT,       "REPEAT" },
        { SamplerDesc::ADDRESS_REPEAT,       "WRAP" },
        { SamplerDesc::ADDRESS_CLAMP,        "CLAMP" },
        { SamplerDesc::ADDRESS_CLAMP_BORDER, "CLAMP_BOORDER" },
        { SamplerDesc::ADDRESS_MIRROR,       "MIRROR" },

        ENUM_TABLE_END
    };

    GN_ASSERT( "texture" == node.name );

    const char * name = sGetItemName( node, "texture" );
    if( !name ) return;

    EffectTextureDesc & texdesc = desc.textures[name];

    SamplerDesc & sampler = texdesc.sampler;

    const XmlAttrib * a = node.FindAttrib( "addressU", StringCompareCase::INSENSITIVE );
    if( a ) sampler.addressU = sParseEnum( a->value, ADDRESS_MODE_TABLE, SamplerDesc::ADDRESS_REPEAT );

    a = node.FindAttrib( "addressV", StringCompareCase::INSENSITIVE );
    if( a ) sampler.addressV = sParseEnum( a->value, ADDRESS_MODE_TABLE, SamplerDesc::ADDRESS_REPEAT );

    a = node.FindAttrib( "addressW", StringCompareCase::INSENSITIVE );
    if( a ) sampler.addressW = sParseEnum( a->value, ADDRESS_MODE_TABLE, SamplerDesc::ADDRESS_REPEAT );

    GN_TODO( "more samplers fields." );
}

//
//
// -----------------------------------------------------------------------------
static void sParseUniform( EffectResourceDesc & desc, const XmlElement & node )
{
    const char * name = sGetItemName( node, "uniform" );
    if( !name ) return;

    EffectResourceDesc::EffectUniformDesc & ud = desc.uniforms[name];

    const char * type = sGetAttrib( node, "type" );
    if( NULL == type )
    {
        ud.size = sGetIntAttrib<size_t>( node, "size", 0 );
    }
    else
    {
        if( 0 == StringCompareI( "matrix", type ) ||
            0 == StringCompareI( "matrix4x4", type ) ||
            0 == StringCompareI( "matrix44", type ) ||
            0 == StringCompareI( "matrix4", type ) ||
            0 == StringCompareI( "mat4", type ) ||
            0 == StringCompareI( "float4x4", type ) )
        {
            ud.size = sizeof(Matrix44f);
        }
        else if(
            0 == StringCompareI( "vector", type ) ||
            0 == StringCompareI( "vec4", type ) ||
            0 == StringCompareI( "vector4", type ) ||
            0 == StringCompareI( "float4", type ) )
        {
            ud.size = sizeof(float)*4;
        }
        else
        {
            sPostError( node, StringFormat( "Unrecognized uniform type: %s", type ) );
            ud.size = 0;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseParameters( EffectResourceDesc & desc, const XmlNode & root )
{
    for( const XmlNode * n = root.child; n; n = n->next )
    {
        const XmlElement * e = n->ToElement();
        if( !e ) continue;

        if( "texture" == e->name ) sParseTexture( desc, *e );
        else if( "uniform" == e->name ) sParseUniform( desc, *e );
        else sPostError( *e, StringFormat( "Unknown parameter '%s'. Ignored", e->name.ToRawPtr() ) );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseTexref( EffectResourceDesc::EffectGpuProgramDesc & sd, const XmlElement & node )
{
    const char * texture = sGetAttrib( node, "texture" );
    if( !texture ) return sPostError( node, "Attribute 'texture' is missing." );

    const char * shaderParameter = sGetAttrib( node, "shaderParameter", NULL );
    if( !shaderParameter ) return sPostError( node, "Attribute 'shaderParameter' is missing." );

    sd.textures[shaderParameter] = texture;
}

//
//
// -----------------------------------------------------------------------------
static void sParseUniref( EffectResourceDesc::EffectGpuProgramDesc & sd, const XmlElement & node )
{
    const char * uniform = sGetAttrib( node, "uniform" );
    if( !uniform ) return sPostError( node, "Attribute 'uniform' not found" );

    const char * shaderParameter = sGetAttrib( node, "shaderParameter" );
    if( !shaderParameter ) return sPostError( node, "Attribute 'shaderParameter' not found" );

    sd.uniforms[shaderParameter] = uniform;
}

//
//
// -----------------------------------------------------------------------------
static void sParsePrerequisites( EffectResourceDesc::ShaderPrerequisites& sp, const XmlElement & node )
{
    GN_ASSERT( "prerequisites" == node.name );
    GN_UNUSED_PARAM( sp );
    GN_UNUSED_PARAM( node );
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
static void sParseCode( EffectGpuProgramDesc & sd, ShaderCode & code, const XmlElement & node )
{
    // get shader code
    for( const XmlNode * n = node.child; n; n = n->next )
    {
        const XmlCdata * c = n->ToCdata();
        if( c )
        {
            size_t offset = sd.shaderSourceBuffer.Size();
            sd.shaderSourceBuffer.Append( c->text.ToRawPtr(), c->text.Size() + 1 );
            code.source = (const char*)offset;
            break;
        }
    }

    // get shader entry
    const char * entry = sGetAttrib( node, "entry", NULL );
    if( entry )
    {
        size_t offset = sd.shaderSourceBuffer.Size();
        sd.shaderSourceBuffer.Append( entry, StringLength(entry) + 1 );
        code.entry = (const char *)offset;
    }
    else
    {
        code.entry = NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseGpuProgram( EffectResourceDesc & desc, const XmlElement & node )
{
    GN_ASSERT( "gpuprogram" == node.name );

    // get shader name
    const char * name = sGetItemName( node, "gpuprogram" );
    if( !name ) return;

    EffectResourceDesc::EffectGpuProgramDesc sd;

    // Add some dummy data into shader source buffer,
    // to ensure that any valid shader source offset won't be zero.
    sd.shaderSourceBuffer.Append( "ABCD", 4 );

    // get shading language
    const char * lang = sGetAttrib( node, "lang" );
    sd.gpd.lang = GpuProgramLanguage::sFromString( lang );
    if( GpuProgramLanguage::INVALID == sd.gpd.lang )
    {
        sPostError( node, StringFormat("invalid shading language: %s",lang?lang:"<NULL>") );
        return;
    }

    sd.gpd.optimize = sGetBoolAttrib( node, "optimize", true );
    sd.gpd.debug    = sGetBoolAttrib( node, "debug", true );

    // parse children
    for( const XmlNode * n = node.child; n; n = n->next )
    {
        const XmlElement * e = n->ToElement();
        if( !e ) continue;

        if( "texref" == e->name ) sParseTexref( sd, *e );
        else if( "uniref" == e->name ) sParseUniref( sd, *e );
        else if( "prerequisites" == e->name ) sParsePrerequisites( sd.prerequisites, *e );
        else if( "vs" == e->name ) sParseCode( sd, sd.gpd.vs, *e );
        else if( "gs" == e->name ) sParseCode( sd, sd.gpd.gs, *e );
        else if( "ps" == e->name ) sParseCode( sd, sd.gpd.ps, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }

    // convert all shader source offsets to pointers
    const char * start = sd.shaderSourceBuffer.ToRawPtr();
    if( sd.gpd.vs.source ) sd.gpd.vs.source += (size_t)start;
    if( sd.gpd.vs.entry  ) sd.gpd.vs.entry  += (size_t)start;
    if( sd.gpd.gs.source ) sd.gpd.gs.source += (size_t)start;
    if( sd.gpd.gs.entry  ) sd.gpd.gs.entry  += (size_t)start;
    if( sd.gpd.ps.source ) sd.gpd.ps.source += (size_t)start;
    if( sd.gpd.ps.entry  ) sd.gpd.ps.entry  += (size_t)start;

    // success
    desc.gpuprograms[name] = sd;
}

//
//
// -----------------------------------------------------------------------------
static void sParseGpuPrograms( EffectResourceDesc & desc, const XmlElement & node )
{
    GN_ASSERT( "gpuprograms" == node.name );

    for( const XmlNode * n = node.child; n; n = n->next )
    {
        const XmlElement * e = n->ToElement();
        if( !e ) continue;

        if( "gpuprogram" == e->name ) sParseGpuProgram( desc, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseRenderStates( EffectResourceDesc::EffectRenderStateDesc & rsdesc, const XmlElement & node )
{
    static const EnumNames CULL_MODE_TABLE[] =
    {
        ENUM_TABLE_BEGIN( "CULL_MODE" ),

        { GpuContext::CULL_NONE,  "CULL_NONE" },
        { GpuContext::CULL_FRONT, "CULL_FRONT" },
        { GpuContext::CULL_BACK,  "CULL_BACK" },

        ENUM_TABLE_END
    };

    for( const XmlAttrib * a = node.attrib; a; a = a->next )
    {
        const char * rsname = a->name.ToRawPtr();
        const char * rsvalue = a->value.ToRawPtr();

        if( 0 == StringCompareI( "CULL_MODE", rsname ) )
        {
            rsdesc.cullMode = sParseEnum( rsvalue, CULL_MODE_TABLE, GpuContext::CULL_BACK );
        }
        else
        {
            GN_ERROR(sLogger)( "Unknow render state name: %s.", rsname );
        }
    }

    GN_TODO( "Load other render states from XML file" );
}

//
//
// -----------------------------------------------------------------------------
static void sParsePass( EffectResourceDesc::EffectTechniqueDesc & td, const XmlElement & node )
{
    const char * gpname = sGetAttrib( node, "gpuprogram" );
    if( !gpname )
    {
        sPostError( node, "required attribute 'gpuprogram' is missing." );
        return;
    }

    td.passes.Resize( td.passes.Size() + 1 );

    EffectResourceDesc::EffectPassDesc & pd = td.passes.Back();

    pd.gpuprogram = gpname;

    for( const XmlNode * n = node.child; n; n = n->next )
    {
        const XmlElement * e = n->ToElement();
        if( !e ) continue;

        if( "renderstates" == e->name ) sParseRenderStates( pd.renderstates, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sParseTechnique( EffectResourceDesc & desc, const XmlElement & node )
{
    GN_ASSERT( "technique" == node.name );

    // get techique name
    const char * techName = sGetItemName( node, "technique" );
    if( NULL == techName ) return;

    EffectResourceDesc::EffectTechniqueDesc td;

    // get technique quality, default is 100
    td.quality = sGetIntAttrib( node, "quality", 100 );

    // parse children
    for( const XmlNode * n = node.child; n; n = n->next )
    {
        const XmlElement * e = n->ToElement();
        if( !e ) continue;

        if( "renderstates" == e->name ) sParseRenderStates( td.renderstates, *e );
        else if( "pass" == e->name ) sParsePass( td, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }

    desc.techniques[techName] = td;
}

//
//
// -----------------------------------------------------------------------------
static void sParseTechniques( EffectResourceDesc & desc, const XmlElement & node )
{
    GN_ASSERT( "techniques" == node.name );

    for( const XmlNode * n = node.child; n; n = n->next )
    {
        const XmlElement * e = n->ToElement();
        if( !e ) continue;

        if( "renderstates" == e->name ) sParseRenderStates( desc.renderstates, *e );
        else if( "technique" == e->name ) sParseTechnique( desc, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sCopyShaderSourcePtr(
    const char          * & to,
    const DynaArray<char> & tobuf,
    const char            * from,
    const DynaArray<char> & frombuf )
{
    GN_ASSERT( tobuf.Size() == frombuf.Size() );

    const char * s = frombuf.ToRawPtr();
    const char * e = s + frombuf.Size();

    if( s <= from && from < e )
    {
        to = tobuf.ToRawPtr() + ( from - s );
    }
    else
    {
        to = from;
    }
}

//
//
// -----------------------------------------------------------------------------
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

// *****************************************************************************
// Public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectResourceDesc::EffectGpuProgramDesc::EffectGpuProgramDesc(
    const EffectGpuProgramDesc & rhs )
{
    prerequisites = rhs.prerequisites;
    gpd = rhs.gpd;
    textures = rhs.textures;
    uniforms = rhs.uniforms;
    sCopyShaderDesc( *this, rhs );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectResourceDesc::EffectGpuProgramDesc &
GN::gfx::EffectResourceDesc::EffectGpuProgramDesc::operator=(
    const EffectGpuProgramDesc & rhs )
{
    prerequisites = rhs.prerequisites;
    gpd = rhs.gpd;
    textures = rhs.textures;
    uniforms = rhs.uniforms;
    sCopyShaderDesc( *this, rhs );
    return *this;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResourceDesc::Clear()
{
    textures.Clear();
    uniforms.Clear();
    gpuprograms.Clear();
    techniques.Clear();

    GpuContext::RenderStates rs;
    rs.Clear();

    memset( &renderstates, 0, sizeof(renderstates) );

    // copy default render state values from GPU context
    renderstates.depthTestEnabled  = !!rs.depthTestEnabled;
    renderstates.depthWriteEnabled = !!rs.depthWriteEnabled;
    renderstates.depthFunc         = rs.depthFunc;

    renderstates.stencilEnabled    = !!rs.stencilEnabled;
    renderstates.stencilPassOp     = rs.stencilPassOp;
    renderstates.stencilFailOp     = rs.stencilFailOp;
    renderstates.stencilZFailOp    = rs.stencilZFailOp;

    renderstates.fillMode          = rs.fillMode;
    renderstates.cullMode          = rs.cullMode;
    renderstates.frontFace         = rs.frontFace;
    renderstates.msaaEnabled       = !!rs.msaaEnabled;

    renderstates.independentAlphaBlending = rs.independentAlphaBlending;
    for( size_t i = 0; i < GpuContext::MAX_COLOR_RENDER_TARGETS; ++i )
    {
        renderstates.alphaBlend[i].blendEnabled  = !!rs.alphaBlend[i].blendEnabled;
        renderstates.alphaBlend[i].blendSrc      = rs.alphaBlend[i].blendSrc;
        renderstates.alphaBlend[i].blendDst      = rs.alphaBlend[i].blendDst;
        renderstates.alphaBlend[i].blendOp       = rs.alphaBlend[i].blendOp;
        renderstates.alphaBlend[i].blendAlphaSrc = rs.alphaBlend[i].blendAlphaSrc;
        renderstates.alphaBlend[i].blendAlphaDst = rs.alphaBlend[i].blendAlphaDst;
        renderstates.alphaBlend[i].blendAlphaOp  = rs.alphaBlend[i].blendAlphaOp;
    }
    renderstates.blendFactors      = rs.blendFactors;

    // exept these:
    //renderstates.colorWriteMask    = rs.colorWriteMask;
    //renderstates.viewport          = rs.viewport;
    //renderstates.scissorRect       = rs.scissorRect;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectResourceDesc::loadFromXml( const XmlNode & root )
{
    Clear();

    const XmlElement * e = root.ToElement();

    if( 0 == e ||e->name != "effect" )
    {
        GN_ERROR(sLogger)( "root node must be \"<effect>\"." );
        return false;
    }

    for( const XmlNode * n = e->child; n; n = n->next )
    {
        e = n->ToElement();
        if( !e ) continue;

        if( "parameters" == e->name ) sParseParameters( *this, *e );
        else if( "gpuprograms" == e->name ) sParseGpuPrograms( *this, *e );
        else if( "techniques" == e->name ) sParseTechniques( *this, *e );
        else sPostError( *e, "Unknown node. Ignored" );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
XmlElement * GN::gfx::EffectResourceDesc::saveToXml( XmlNode & root ) const
{
    GN_UNUSED_PARAM( root );
    GN_UNIMPL();
    return NULL;
}
