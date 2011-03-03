#include "pch.h"
#include "gpuresdb.h"
#include "textureresource.h"
#include "uniformresource.h"
#include "meshresource.h"
#include "effectresource.h"
#include "modelresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Diffuse effect
// *****************************************************************************

static const char * DIFFUSE_VS_HLSL9 =
    "uniform float4x4 pvw; \n"
    "uniform float4x4 world; \n"
    "uniform float4x4 wit; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float4 pos_world : POSITION1;    // vertex position in world space \n"
    "   float3 nml_world : NORMAL0; // vertex normal in world space \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "   float3 normal    : NORMAL0; \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   o.hpos      = mul( pvw, i.position ); \n"
    "   o.pos_world = mul( world, i.position ); \n"
    "   o.nml_world = mul( wit, float4(i.normal,0) ).xyz; \n"
    "   o.texcoords = i.texcoords; \n"
    "   return o; \n"
    "}";

static const char * DIFFUSE_PS_HLSL9 =
    "uniform float4 lightpos; // light positin in world space \n"
    "uniform float4 lightColor; \n"
    "uniform float4 albedoColor; \n"
    "sampler t0; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float4 pos_world : POSITION1;    // vertex position in world space \n"
    "   float3 nml_world : NORMAL0; // vertex normal in world space \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "float4 main( in VSOUTPUT i ) : COLOR0 { \n"
    "   float3  L    = normalize( (lightpos - i.pos_world).xyz ); \n"
    "   float3  N    = normalize( i.nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   float4  tex  = tex2D( t0, i.texcoords ); \n"
    "   return float4( diff, diff, diff, 1.0 ) * lightColor * albedoColor * tex; \n"
    "}";

static const char * DIFFUSE_VS_GLSL =
    "uniform mat4 pvw; \n"
    "uniform mat4 world; \n"
    "uniform mat4 wit; \n"
    "varying vec4 pos_world; // vertex position in world space \n"
    "varying vec3 nml_world; // vertex normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_Position = pvw * gl_Vertex; \n"
    "   pos_world   = world * gl_Vertex; \n"
    "   nml_world   = (wit * vec4(gl_Normal,0)).xyz; \n"
    "   texcoords   = gl_MultiTexCoord0.xy; \n"
    "}";

static const char * DIFFUSE_PS_GLSL =
    "uniform vec4 lightpos; // light positin in world space \n"
    "uniform vec4 lightColor; \n"
    "uniform vec4 albedoColor; \n"
    "uniform sampler2D t0; \n"
    "varying vec4 pos_world; // position in world space \n"
    "varying vec3 nml_world; // normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   vec3  L      = normalize( (lightpos - pos_world).xyz ); \n"
    "   vec3  N      = normalize( nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   vec4  tex    = texture2D( t0, texcoords ); \n"
    "   gl_FragColor = vec4( diff, diff, diff, 1.0 ) * lightColor * albedoColor * tex; \n"
    "}";

//
//
// -----------------------------------------------------------------------------
static AutoRef<EffectResource> sRegisterDiffuseEffect( GpuResourceDatabase & gdb )
{
    EffectResourceDesc ed;

    ed.uniforms[StandardUniform::Desc::MATRIX_PVW.name];
    ed.uniforms[StandardUniform::Desc::MATRIX_WORLD.name];
    ed.uniforms[StandardUniform::Desc::MATRIX_WORLD_IT.name];
    ed.uniforms[StandardUniform::Desc::LIGHT0_POSITION.name];
    ed.uniforms[StandardUniform::Desc::LIGHT0_DIFFUSE.name];
    ed.uniforms["ALBEDO_COLOR"];
    ed.textures["ALBEDO_TEXTURE"];
    ed.attributes["POSITION"];
    ed.attributes["NORMAL"];
    ed.attributes["TEXCOORD"];

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.vs.source = DIFFUSE_VS_GLSL;
    ed.gpuprograms["glsl"].gpd.ps.source = DIFFUSE_PS_GLSL;
    ed.gpuprograms["glsl"].uniforms["pvw"] = StandardUniform::Desc::MATRIX_PVW.name;
    ed.gpuprograms["glsl"].uniforms["world"] = StandardUniform::Desc::MATRIX_WORLD.name;
    ed.gpuprograms["glsl"].uniforms["wit"] = StandardUniform::Desc::MATRIX_WORLD_IT.name;
    ed.gpuprograms["glsl"].uniforms["lightpos"] = StandardUniform::Desc::LIGHT0_POSITION.name;
    ed.gpuprograms["glsl"].uniforms["lightColor"] = StandardUniform::Desc::LIGHT0_DIFFUSE.name;
    ed.gpuprograms["glsl"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["glsl"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.gpuprograms["glsl"].attributes["gl_Vertex"] = "POSITION0";
    ed.gpuprograms["glsl"].attributes["gl_Normal"] = "NORMAL0";
    ed.gpuprograms["glsl"].attributes["gl_MultiTexCoord0"] = "TEXCOORD0";

    ed.gpuprograms["hlsl9"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl9"].gpd.vs.source = DIFFUSE_VS_HLSL9;
    ed.gpuprograms["hlsl9"].gpd.vs.entry  = "main";
    ed.gpuprograms["hlsl9"].gpd.ps.source = DIFFUSE_PS_HLSL9;
    ed.gpuprograms["hlsl9"].gpd.ps.entry  = "main";
    ed.gpuprograms["hlsl9"].uniforms["pvw"] = StandardUniform::Desc::MATRIX_PVW.name;
    ed.gpuprograms["hlsl9"].uniforms["world"] = StandardUniform::Desc::MATRIX_WORLD.name;
    ed.gpuprograms["hlsl9"].uniforms["wit"] = StandardUniform::Desc::MATRIX_WORLD_IT.name;
    ed.gpuprograms["hlsl9"].uniforms["lightpos"] = StandardUniform::Desc::LIGHT0_POSITION.name;
    ed.gpuprograms["hlsl9"].uniforms["lightColor"] = StandardUniform::Desc::LIGHT0_DIFFUSE.name;
    ed.gpuprograms["hlsl9"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["hlsl9"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.gpuprograms["hlsl9"].attributes["POSITION0"] = "POSITION";
    ed.gpuprograms["hlsl9"].attributes["NORMAL0"] = "NORMAL";
    ed.gpuprograms["hlsl9"].attributes["TEXCOORD0"] = "TEXCOORD";

    ed.techniques.resize( 2 );
    ed.techniques[0].name = "glsl";
    ed.techniques[0].passes.resize( 1 );
    ed.techniques[0].passes[0].gpuprogram = "glsl";
    ed.techniques[1].name = "hlsl9";
    ed.techniques[1].passes.resize( 1 );
    ed.techniques[1].passes[0].gpuprogram = "hlsl9";

    AutoRef<EffectResource> e = gdb.createResource<EffectResource>( "@DIFFUSE" );

    e->reset( &ed );

    return e;
}

// *****************************************************************************
// Wireframe effect
// *****************************************************************************

static const char * WIREFRAME_VS_HLSL9 =
    "uniform float4x4 pvw; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   o.hpos      = mul( pvw, i.position ); \n"
    "   return o; \n"
    "}";

static const char * WIREFRAME_PS_HLSL9 =
    "uniform float4 color; \n"
    "float4 main() : COLOR0 { \n"
    "   return color; \n"
    "}";

static const char * WIREFRAME_VS_GLSL =
    "uniform mat4 pvw; \n"
    "void main() { \n"
    "   gl_Position = pvw * gl_Vertex; \n"
    "}";

static const char * WIREFRAME_PS_GLSL =
    "uniform vec4 color; \n"
    "void main() { \n"
    "   gl_FragColor = color; \n"
    "}";

//
//
// -----------------------------------------------------------------------------
static AutoRef<EffectResource> sRegisterWireframeEffect( GpuResourceDatabase & gdb )
{
    EffectResourceDesc ed;

    ed.uniforms[StandardUniform::Desc::MATRIX_PVW.name];
    ed.uniforms["ALBEDO_COLOR"];
    ed.attributes["POSITION"];

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.vs.source = WIREFRAME_VS_GLSL;
    ed.gpuprograms["glsl"].gpd.ps.source = WIREFRAME_PS_GLSL;
    ed.gpuprograms["glsl"].uniforms["pvw"] = StandardUniform::Desc::MATRIX_PVW.name;
    ed.gpuprograms["glsl"].uniforms["color"] = "ALBEDO_COLOR";
    ed.gpuprograms["glsl"].attributes["gl_Vertex"] = "POSITION";

    ed.gpuprograms["hlsl"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl"].gpd.vs.source = WIREFRAME_VS_HLSL9;
    ed.gpuprograms["hlsl"].gpd.vs.entry  = "main";
    ed.gpuprograms["hlsl"].gpd.ps.source = WIREFRAME_PS_HLSL9;
    ed.gpuprograms["hlsl"].gpd.ps.entry  = "main";
    ed.gpuprograms["hlsl"].uniforms["pvw"] = StandardUniform::Desc::MATRIX_PVW.name;
    ed.gpuprograms["hlsl"].uniforms["color"] = "ALBEDO_COLOR";
    ed.gpuprograms["hlsl"].attributes["POSITION0"] = "POSITION";

    ed.techniques.resize( 2 );
    ed.techniques[0].name = "glsl";
    ed.techniques[0].passes.resize( 1 );
    ed.techniques[0].passes[0].gpuprogram = "glsl";
    ed.techniques[1].name = "hlsl";
    ed.techniques[1].passes.resize( 1 );
    ed.techniques[1].passes[0].gpuprogram = "hlsl";

    AutoRef<EffectResource> e = gdb.createResource<EffectResource>( "@WIREFRAME" );

    e->reset( &ed );

    return e;
}

// *****************************************************************************
// Normalmap effect
// *****************************************************************************

static const char * NORMALMAP_VS_HLSL9 =
    "uniform float4x4 pvw; \n"
    "uniform float4x4 world; \n"
    "uniform float4x4 wit; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0; // vertex position in homogenous space \n"
    "   float4 pos_world : POSITION1; // vertex position in world space \n"
    "   float3 nml_world : NORMAL0;   // vertex normal in world space \n"
    "   float3 tan_world : NORMAL1;   // vertex tangent in world space \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "   float3 normal    : NORMAL0; \n"
    "   float3 tangent   : TANGENT; \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   o.hpos      = mul( pvw, i.position ); \n"
    "   o.pos_world = mul( world, i.position ); \n"
    "   o.nml_world = mul( wit, float4(i.normal,0) ).xyz; \n"
    "   o.tan_world = mul( wit, float4(i.tangent,0) ).xyz; \n"
    "   o.texcoords = i.texcoords; \n"
    "   return o; \n"
    "}";

static const char * NORMALMAP_PS_HLSL9 =
    "uniform float4 lightpos; // light positin in world space \n"
    "uniform float4 lightColor; \n"
    "uniform float4 albedoColor; \n"
    "sampler t0; // albedo texture \n"
    "sampler t1; // normal texture \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0; // vertex position in homogenous space \n"
    "   float4 pos_world : POSITION1; // vertex position in world space \n"
    "   float3 nml_world : NORMAL0;   // vertex normal in world space \n"
    "   float3 tan_world : NORMAL1;   // vertex tangent in world space \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "float4 main( in VSOUTPUT i ) : COLOR0 { \n"
    "   float3  L    = normalize( (lightpos - i.pos_world).xyz ); \n"
    "   float3  N    = normalize( i.nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   float4  tex  = tex2D( t0, i.texcoords ) + tex2D( t1, i.texcoords ).xyyy; \n"
    "   return float4( diff, diff, diff, 1.0 ) * lightColor * albedoColor * tex; \n"
    "}";

static const char * NORMALMAP_VS_GLSL =
    "uniform mat4 pvw; \n"
    "uniform mat4 world; \n"
    "uniform mat4 wit; \n"
    "varying vec4 pos_world; // vertex position in world space \n"
    "varying vec3 nml_world; // vertex normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_Position = pvw * gl_Vertex; \n"
    "   pos_world   = world * gl_Vertex; \n"
    "   nml_world   = (wit * vec4(gl_Normal,0)).xyz; \n"
    "   texcoords   = gl_MultiTexCoord0.xy; \n"
    "}";

static const char * NORMALMAP_PS_GLSL =
    "uniform vec4 lightpos; // light positin in world space \n"
    "uniform vec4 lightColor; \n"
    "uniform vec4 albedoColor; \n"
    "uniform sampler2D t0; // albedo texture \n"
    "uniform sampler2D t1; // normal texture \n"
    "varying vec4 pos_world; // position in world space \n"
    "varying vec3 nml_world; // normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   vec3  L      = normalize( (lightpos - pos_world).xyz ); \n"
    "   vec3  N      = normalize( nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   vec4  tex    = texture2D( t0, texcoords ) + texture2D( t1, texcoords ); \n"
    "   gl_FragColor = vec4( diff, diff, diff, 1.0 ) * lightColor * albedoColor * tex; \n"
    "}";

//
//
// -----------------------------------------------------------------------------
static AutoRef<EffectResource> sRegisterNormalMapEffect( GpuResourceDatabase & gdb )
{
    EffectResourceDesc ed;

    ed.uniforms["MATRIX_PVW"];
    ed.uniforms["MATRIX_WORLD"];
    ed.uniforms["MATRIX_WORLD_IT"];
    ed.uniforms["LIGHT0_POSITION"];
    ed.uniforms["LIGHT0_DIFFUSE"];
    ed.uniforms["ALBEDO_COLOR"];
    ed.textures["ALBEDO_TEXTURE"];
    ed.textures["NORMAL_TEXTURE"];
    ed.attributes["POSITION"];
    ed.attributes["NORMAL"];
    ed.attributes["TANGENT"];
    ed.attributes["TEXCOORD"];

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.vs.source = NORMALMAP_VS_GLSL;
    ed.gpuprograms["glsl"].gpd.ps.source = NORMALMAP_PS_GLSL;
    ed.gpuprograms["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["glsl"].uniforms["world"] = "MATRIX_WORLD";
    ed.gpuprograms["glsl"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.gpuprograms["glsl"].uniforms["lightpos"] = "LIGHT0_POSITION";
    ed.gpuprograms["glsl"].uniforms["lightColor"] = "LIGHT0_DIFFUSE";
    ed.gpuprograms["glsl"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["glsl"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.gpuprograms["glsl"].textures["t1"] = "NORMAL_TEXTURE";
    ed.gpuprograms["glsl"].attributes["gl_Vertex"] = "POSITION";
    ed.gpuprograms["glsl"].attributes["gl_Normal"] = "NORMAL";
    ed.gpuprograms["glsl"].attributes["gl_MultiTexCoord0"] = "TEXCOORD";

    ed.gpuprograms["hlsl"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl"].gpd.vs.source = NORMALMAP_VS_HLSL9;
    ed.gpuprograms["hlsl"].gpd.vs.entry  = "main";
    ed.gpuprograms["hlsl"].gpd.ps.source = NORMALMAP_PS_HLSL9;
    ed.gpuprograms["hlsl"].gpd.ps.entry  = "main";
    ed.gpuprograms["hlsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["hlsl"].uniforms["world"] = "MATRIX_WORLD";
    ed.gpuprograms["hlsl"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.gpuprograms["hlsl"].uniforms["lightpos"] = "LIGHT0_POSITION";
    ed.gpuprograms["hlsl"].uniforms["lightColor"] = "LIGHT0_DIFFUSE";
    ed.gpuprograms["hlsl"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["hlsl"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.gpuprograms["hlsl"].textures["t1"] = "NORMAL_TEXTURE";
    ed.gpuprograms["hlsl"].attributes["POSITION0"] = "POSITION";
    ed.gpuprograms["hlsl"].attributes["NORMAL0"] = "NORMAL";
    ed.gpuprograms["hlsl"].attributes["TEXCOORD0"] = "TEXCOORD";

    ed.techniques.resize( 2 );
    ed.techniques[0].name = "glsl";
    ed.techniques[0].passes.resize( 1 );
    ed.techniques[0].passes[0].gpuprogram = "glsl";
    ed.techniques[1].name = "hlsl";
    ed.techniques[1].passes.resize( 1 );
    ed.techniques[1].passes[0].gpuprogram = "hlsl";

    AutoRef<EffectResource> e = gdb.createResource<EffectResource>( "@NORMAL_MAP" );

    e->reset( &ed );

    return e;
}

// *****************************************************************************
// Default Textures
// *****************************************************************************

//
// create a pure white 2x2 texture
// -----------------------------------------------------------------------------
static AutoRef<TextureResource> sRegisterWhiteTexture( GpuResourceDatabase & gdb )
{
    AutoRef<TextureResource> tr = gdb.createResource<TextureResource>( "@WHITE" );
    AutoRef<Texture> t( gdb.getGpu().create2DTexture( 2, 2, 0, ColorFormat::RGBA32 ) );
    uint32 white[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
    t->updateMipmap( 0, 0, NULL, sizeof(uint32)*2, sizeof(uint32)*4, white, SurfaceUpdateFlag::DEFAULT );
    tr->setTexture( t );
    return tr;
}

//
// create a pure black 2x2 texture
// -----------------------------------------------------------------------------
static AutoRef<TextureResource> sRegisterBlackTexture( GpuResourceDatabase & gdb )
{
    AutoRef<TextureResource> tr = gdb.createResource<TextureResource>( "@BLACK" );
    AutoRef<Texture> t( gdb.getGpu().create2DTexture( 2, 2, 0, ColorFormat::RGBA32 ) );
    uint32 black[4] = { 0, 0, 0, 0 };
    t->updateMipmap( 0, 0, NULL, sizeof(uint32)*2, sizeof(uint32)*4, black, SurfaceUpdateFlag::DEFAULT );
    tr->setTexture( t );
    return tr;
}

//
// create a normal map texture for flat surface
// -----------------------------------------------------------------------------
static AutoRef<TextureResource> sRegisterFlatNormalMap( GpuResourceDatabase & gdb )
{
    AutoRef<TextureResource> tr = gdb.createResource<TextureResource>( "@FLAT_NORMAL_MAP" );
    AutoRef<Texture> t(  gdb.getGpu().create2DTexture( 2, 2, 0, ColorFormat::RG_16_16_UNORM ) );
    uint32 up[4] = { 0x80008000, 0x80008000, 0x80008000, 0x80008000 };
    t->updateMipmap( 0, 0, NULL, sizeof(uint32)*2, sizeof(uint32)*4, up, SurfaceUpdateFlag::DEFAULT );
    tr->setTexture( t );
    return tr;
}


// *****************************************************************************
// GpuResource::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResource::Impl::Impl( GpuResourceDatabase::Impl & db, GpuResource & res )
    : database(db)
    , resource(res)
    , handle(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResource::Impl::~Impl()
{
    if( 0 != handle.u32() )
    {
        database.onResourceDelete( handle );
    }
}

// *****************************************************************************
// GpuResource public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResource::GpuResource( GpuResourceDatabase & db )
    : mDatabase(db), mImpl(NULL)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResource::~GpuResource()
{
    delete mImpl;
}

// *****************************************************************************
// GpuResourceDatabase::Impl public methods
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::Impl( GpuResourceDatabase & db, Gpu & g )
    : mDatabase(db), mGpu(g)
{
}

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::~Impl()
{
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::registerResourceFactory(
    const Guid       & type,
    const char       * descriptiveName,
    GpuResourceFactory factory )
{
    if( hasResourceFactory( type ) )
    {
        GN_ERROR(sLogger)( "Resource type exisits already!" );
        return false;
    }

    if( NULL == factory.createResource )
    {
        GN_ERROR(sLogger)( "Resource factory has NULL function pointer(s)." );
        return false;
    }

    if( mManagers.size() == mManagers.MAX_SIZE )
    {
        GN_ERROR(sLogger)( "Resource manager pool is full. Cannot register more resource types!" );
        return false;
    }

    mManagers.resize( mManagers.size() + 1 );

    ResourceManager & mgr = mManagers.back();

    mgr.guid = type;
    mgr.desc = descriptiveName ? descriptiveName : "unnamed resource";
    mgr.index = mManagers.size() - 1;
    mgr.factory = factory;
    GN_ASSERT( mgr.resources.empty() );

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::hasResourceFactory( const Guid & type )
{
    return NULL != getManager(type);
}

//
//
// -----------------------------------------------------------------------------
AutoRef<GpuResource>
GpuResourceDatabase::Impl::createResource(
    const Guid & type,
    const char * name )
{
    // get the manager for the resource type
    ResourceManager * mgr = getManager( type );
    if( NULL == mgr )
    {
        GN_ERROR(sLogger)( "Invalid resouce type: %s", type.toStr() );
        return AutoRef<GpuResource>::NULLREF;
    }

    // compose a unique name for unnamed resource
    char unnamed[256];
    if( 0 == name || 0 == *name )
    {
        static int i = 0;
        ++i;
        stringPrintf( unnamed, GN_ARRAY_COUNT(unnamed), "Unnamed %s #%d", mgr->desc.cptr(), i );
        name = unnamed;

        GN_ASSERT( !mgr->resources.validName( name ) );
    }

    // check if the resource with same name exisits already.
    if( mgr->resources.validName( name ) )
    {
        GN_ERROR(sLogger)( "Resource named \"%s\" exists already.", name );
        return AutoRef<GpuResource>::NULLREF;
    }

    // create new resource instance
    AutoRef<GpuResource> newres( mgr->factory.createResource( mDatabase ) );
    if( !newres ) return AutoRef<GpuResource>::NULLREF;

    // create new instance of resource implementation class, assign handle to it
    GpuResource::Impl * resimpl = new GpuResource::Impl( *this, *newres );
    if( !resimpl ) return AutoRef<GpuResource>::NULLREF;
    newres->mImpl = resimpl;

    // create new handle
    uint32 internalHandle = mgr->resources.add( name, resimpl );
    if( 0 ==internalHandle ) return AutoRef<GpuResource>::NULLREF;
    resimpl->handle.set( (uint32)mgr->index, internalHandle );

    // done
    return newres;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<GpuResource>
GpuResourceDatabase::Impl::findResource( const Guid & type, const char * name ) const
{
    const ResourceManager * mgr = getManager( type );
    if( NULL == mgr ) return AutoRef<GpuResource>::NULLREF;

    uint32 internalHandle = mgr->resources.name2handle(name);
    if( 0 == internalHandle ) return AutoRef<GpuResource>::NULLREF;

    GpuResource::Impl * resimpl = mgr->resources[internalHandle];
    GN_ASSERT( resimpl );

    AutoRef<GpuResource> result;
    result.set( &resimpl->resource );

    return result;
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::validResource( const Guid & type, const GpuResource * resource ) const
{
    if( NULL == resource ) return false;

    if( this != resource->getGdb().mImpl ) return false;

    const ResourceManager * mgr = getManager( type );
    if( NULL == mgr ) return false;

    return mgr->resources.validHandle( resource->mImpl->handle.internalHandle() );
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::validResource( const GpuResource * resource ) const
{
    if( NULL == resource ) return false;

    if( this != resource->getGdb().mImpl ) return false;

    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        const ResourceManager & mgr = mManagers[i];

        if( mgr.resources.validHandle( resource->mImpl->handle.internalHandle() ) ) return true;
    }

    return false;
}

//
//
// -----------------------------------------------------------------------------
const char *
GpuResourceDatabase::Impl::getResourceName( const GpuResource * resource ) const
{
    GpuResource::Impl * resimpl = getResourceImpl( resource );
    if( NULL == resimpl ) return NULL;

    const ResourceManager & mgr = mManagers[resimpl->handle.managerIndex()];

    const char * name = mgr.resources.handle2name( resimpl->handle.internalHandle() );

    if( NULL == name )
    {
        GN_ERROR(sLogger)( "Fail to get reosource name: Invalid resource pointer." );
    }

    return name;
}

//
//
// -----------------------------------------------------------------------------
const Guid &
GpuResourceDatabase::Impl::getResourceType( const GpuResource * resource ) const
{
    GpuResource::Impl * resimpl = getResourceImpl( resource );

    if( NULL == resimpl )
    {
        static const Guid INVALID_TYPE = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
        return INVALID_TYPE;
    }

    const ResourceManager & mgr = mManagers[resimpl->handle.managerIndex()];

    return mgr.guid;
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::onResourceDelete( GpuResourceHandle handle )
{
    GN_ASSERT( handle.u32() );

    ResourceManager & mgr = mManagers[handle.managerIndex()];

    mgr.resources.remove( handle.internalHandle() );
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::setupBuiltInResources()
{
    // register built-in resource factories
    if( !registerTextureResourceFactory( mDatabase ) ) return false;
    if( !registerUniformResourceFactory( mDatabase ) ) return false;
    if( !registerMeshResourceFactory( mDatabase ) ) return false;
    if( !registerEffectResourceFactory( mDatabase ) ) return false;
    if( !registerModelResourceFactory( mDatabase ) ) return false;

    // create some built-in resources
    mBuiltInResources.append( sRegisterDiffuseEffect( mDatabase ) );
    mBuiltInResources.append( sRegisterWireframeEffect( mDatabase ) );
    mBuiltInResources.append( sRegisterNormalMapEffect( mDatabase ) );
    mBuiltInResources.append( sRegisterWhiteTexture( mDatabase ) );
    mBuiltInResources.append( sRegisterBlackTexture( mDatabase ) );
    mBuiltInResources.append( sRegisterFlatNormalMap( mDatabase ) );

    // create built-in uniforms.
    for( int i = 0; i < StandardUniform::Index::NUM_STANDARD_UNIFORMS; ++i )
    {
        AutoRef<UniformResource> & ur = mStdUniforms[i];

        const StandardUniform::Desc * desc = StandardUniform::sIndex2Desc( i );

        // Note: the content of the uniform leaves uninitialized.
        AutoRef<Uniform> u( getGpu().createUniform( desc->size ) );

        ur = mDatabase.createResource<UniformResource>( desc->name );
        ur->setUniform( u );
    }

    // setup default uniform values
    // TODO: set default uniform value based on the uniform meta file
    Vector4f diffuse(1,1,1,1);
    Vector4f ambient(0.2f,0.2f,0.2f,1.0f);
    Vector4f specular(1,1,1,1);
    Vector3f position(0,0,0);
    Vector3f direction(0,0,1);
    setLight0( diffuse, ambient, specular, position, direction );

    return true;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<UniformResource>
GpuResourceDatabase::Impl::getStandardUniformResource( int index ) const
{
    if( !StandardUniform::sIsValidIndex( index ) )
    {
        GN_ERROR(sLogger)( "Invalid standard uniform index: %d", index );
        return AutoRef<UniformResource>::NULLREF;
    }
    return mStdUniforms[index];
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::setStandardUniform( int index, const void * data, size_t dataSize )
{
    // check parameters
    if( !StandardUniform::sIsValidIndex( index ) )
    {
        GN_ERROR(sLogger)( "Invalid uniform type: %d", index );
        return;
    }
    if( NULL == data )
    {
        GN_ERROR(sLogger)( "Null point." );
        return;
    }
    const StandardUniform::Desc * desc = StandardUniform::sIndex2Desc( index );
    if( dataSize != desc->size )
    {
        GN_ERROR(sLogger)( "Incorrect uniform data size: expected=%d, actual=%d.", desc->size, dataSize );
        return;
    }

    mStdUniforms[index]->uniform()->update( 0, dataSize, data );
}


//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::setTransform( const Matrix44f & proj, const Matrix44f & view )
{
    Matrix44f pv   = proj * view;
    Matrix44f ipv  = Matrix44f::sInverse( pv );
    Matrix44f itpv = Matrix44f::sInverse( Matrix44f::sTranspose( pv ) );

    Matrix44f ip  = Matrix44f::sInverse( proj );
    Matrix44f itp = Matrix44f::sInverse( Matrix44f::sTranspose( proj ) );

    Matrix44f iv  = Matrix44f::sInverse( view );
    Matrix44f itv = Matrix44f::sInverse( Matrix44f::sTranspose( view ) );

    mStdUniforms[StandardUniform::Index::MATRIX_PV]->uniform()->update( pv );
    mStdUniforms[StandardUniform::Index::MATRIX_PV_INV]->uniform()->update( ipv );
    mStdUniforms[StandardUniform::Index::MATRIX_PV_IT]->uniform()->update( itpv );

    mStdUniforms[StandardUniform::Index::MATRIX_PROJ]->uniform()->update( proj );
    mStdUniforms[StandardUniform::Index::MATRIX_PROJ_INV]->uniform()->update( ip );
    mStdUniforms[StandardUniform::Index::MATRIX_PROJ_IT]->uniform()->update( itp );

    mStdUniforms[StandardUniform::Index::MATRIX_VIEW]->uniform()->update( view );
    mStdUniforms[StandardUniform::Index::MATRIX_VIEW_INV]->uniform()->update( iv );
    mStdUniforms[StandardUniform::Index::MATRIX_VIEW_IT]->uniform()->update( itv );
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::setLight0(
    const Vector4f & diffuse,
    const Vector4f & ambient,
    const Vector4f & specular,
    const Vector3f & position,
    const Vector3f & direction )
{
    mStdUniforms[StandardUniform::Index::LIGHT0_DIFFUSE]->uniform()->update( diffuse );
    mStdUniforms[StandardUniform::Index::LIGHT0_AMBIENT]->uniform()->update( ambient );
    mStdUniforms[StandardUniform::Index::LIGHT0_SPECULAR]->uniform()->update( specular );
    mStdUniforms[StandardUniform::Index::LIGHT0_POSITION]->uniform()->update( position );
    mStdUniforms[StandardUniform::Index::LIGHT0_DIRECTION]->uniform()->update( direction );
}
// *****************************************************************************
// GpuResourceDatabase::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline const GpuResourceDatabase::Impl::ResourceManager *
GpuResourceDatabase::Impl::getManager( const Guid & type ) const
{
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        const ResourceManager & m = mManagers[i];
        if( type == m.guid )
        {
            return &m;
        }
    }

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
inline GpuResourceDatabase::Impl::ResourceManager *
GpuResourceDatabase::Impl::getManager( const Guid & type )
{
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        ResourceManager & m = mManagers[i];
        if( type == m.guid )
        {
            return &m;
        }
    }

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
inline GpuResource::Impl *
GpuResourceDatabase::Impl::getResourceImpl( const GpuResource * resource ) const
{
    if( NULL == resource )
    {
        GN_ERROR(sLogger)( "Null resource pointer." );
        return NULL;
    }

    if( this != resource->getGdb().mImpl )
    {
        GN_ERROR(sLogger)( "The resource belongs to another database." );
        return NULL;
    }

    GpuResource::Impl * impl = resource->mImpl;

    GN_ASSERT( impl );
    GN_ASSERT( impl->handle.managerIndex() < mManagers.size() );
    GN_ASSERT( mManagers[impl->handle.managerIndex()].resources.validHandle(impl->handle.internalHandle()) );

    return impl;
}

// *****************************************************************************
// GpuResourceDatabase
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::GpuResourceDatabase( Gpu & g ) : mImpl(NULL)
{
    mImpl = new Impl(*this,g);

    if( !mImpl->setupBuiltInResources() )
    {
        GN_THROW( "Fail to setup built-in resources." );
    }
}

                         GpuResourceDatabase::~GpuResourceDatabase() { delete mImpl; }
Gpu                    & GpuResourceDatabase::getGpu() const { return mImpl->getGpu(); }
bool                     GpuResourceDatabase::registerResourceFactory( const Guid & type, const char * desc, GpuResourceFactory factory ) { return mImpl->registerResourceFactory( type, desc, factory ); }
bool                     GpuResourceDatabase::hasResourceFactory( const Guid & type ) { return mImpl->hasResourceFactory( type ); }
AutoRef<GpuResource>     GpuResourceDatabase::createResource( const Guid & type, const char * name ) { return mImpl->createResource( type, name ); }
AutoRef<GpuResource>     GpuResourceDatabase::findResource( const Guid & type, const char * name ) const { return mImpl->findResource( type, name ); }
bool                     GpuResourceDatabase::validResource( const Guid & type, const GpuResource * resource ) const { return mImpl->validResource( type, resource ); }
bool                     GpuResourceDatabase::validResource( const GpuResource * resource ) const { return mImpl->validResource( resource ); }
const char             * GpuResourceDatabase::getResourceName( const GpuResource * resource ) const { return mImpl->getResourceName( resource ); }
const Guid             & GpuResourceDatabase::getResourceType( const GpuResource * resource ) const { return mImpl->getResourceType( resource ); }
AutoRef<UniformResource> GpuResourceDatabase::getStandardUniformResource( int index ) const { return mImpl->getStandardUniformResource( index ); }
void                     GpuResourceDatabase::setStandardUniform( int index, const void * data, size_t dataSize ) { return mImpl->setStandardUniform( index, data, dataSize ); }
void                     GpuResourceDatabase::setTransform( const Matrix44f & proj, const Matrix44f & view ) { return mImpl->setTransform( proj, view ); }
void                     GpuResourceDatabase::setLight0( const Vector4f & diffuse, const Vector4f & ambient, const Vector4f & specular, const Vector3f & position, const Vector3f & direction ) { return mImpl->setLight0( diffuse, ambient, specular, position, direction ); }
