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
    "sampler s0; \n"
    "Texture2D<float4> t0; \n"
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
    "   float4  tex  = t0.Sample( s0, i.texcoords ); \n"
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

    ed.uniforms["MATRIX_PVW"];
    ed.uniforms["MATRIX_WORLD"];
    ed.uniforms["MATRIX_WORLD_IT"];
    ed.uniforms["LIGHT0_POSITION"];
    ed.uniforms["LIGHT0_DIFFUSE"];
    ed.uniforms["ALBEDO_COLOR"];
    ed.textures["ALBEDO_TEXTURE"];

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.vs.source = DIFFUSE_VS_GLSL;
    ed.gpuprograms["glsl"].gpd.ps.source = DIFFUSE_PS_GLSL;
    ed.gpuprograms["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["glsl"].uniforms["world"] = "MATRIX_WORLD";
    ed.gpuprograms["glsl"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.gpuprograms["glsl"].uniforms["lightpos"] = "LIGHT0_POSITION";
    ed.gpuprograms["glsl"].uniforms["lightColor"] = "LIGHT0_DIFFUSE";
    ed.gpuprograms["glsl"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["glsl"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].gpuprogram = "glsl";

    ed.gpuprograms["hlsl9"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl9"].gpd.vs.source = DIFFUSE_VS_HLSL9;
    ed.gpuprograms["hlsl9"].gpd.vs.entry  = "main";
    ed.gpuprograms["hlsl9"].gpd.ps.source = DIFFUSE_PS_HLSL9;
    ed.gpuprograms["hlsl9"].gpd.ps.entry  = "main";
    ed.gpuprograms["hlsl9"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["hlsl9"].uniforms["world"] = "MATRIX_WORLD";
    ed.gpuprograms["hlsl9"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.gpuprograms["hlsl9"].uniforms["lightpos"] = "LIGHT0_POSITION";
    ed.gpuprograms["hlsl9"].uniforms["lightColor"] = "LIGHT0_DIFFUSE";
    ed.gpuprograms["hlsl9"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["hlsl9"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.techniques["hlsl9"].passes.resize( 1 );
    ed.techniques["hlsl9"].passes[0].gpuprogram = "hlsl9";

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

    ed.uniforms["MATRIX_PVW"];
    ed.uniforms["COLOR"];

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.vs.source = WIREFRAME_VS_GLSL;
    ed.gpuprograms["glsl"].gpd.ps.source = WIREFRAME_PS_GLSL;
    ed.gpuprograms["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["glsl"].uniforms["color"] = "COLOR";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].gpuprogram = "glsl";

    ed.gpuprograms["hlsl"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl"].gpd.vs.source = WIREFRAME_VS_HLSL9;
    ed.gpuprograms["hlsl"].gpd.vs.entry  = "main";
    ed.gpuprograms["hlsl"].gpd.ps.source = WIREFRAME_PS_HLSL9;
    ed.gpuprograms["hlsl"].gpd.ps.entry  = "main";
    ed.gpuprograms["hlsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["hlsl"].uniforms["color"] = "COLOR";
    ed.techniques["hlsl"].passes.resize( 1 );
    ed.techniques["hlsl"].passes[0].gpuprogram = "hlsl";

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
    "sampler s0; \n"
    "Texture2D<float4> t0; // albedo texture \n"
    "Texture2D<float2> t1; // normal texture \n"
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
    "   float4  tex  = t0.Sample( s0, i.texcoords ) + t1.Sample( s0, i.texcoords ).xyyy; \n"
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
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].gpuprogram = "glsl";

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
    ed.techniques["hlsl"].passes.resize( 1 );
    ed.techniques["hlsl"].passes[0].gpuprogram = "hlsl";

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
    AutoRef<Texture> t( gdb.gpu().create2DTexture( 2, 2, 0, ColorFormat::RGBA32 ) );
    UInt32 white[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
    t->updateMipmap( 0, 0, NULL, sizeof(UInt32)*2, sizeof(UInt32)*4, white, SurfaceUpdateFlag::DEFAULT );
    tr->setTexture( t );
    return tr;
}

//
// create a pure black 2x2 texture
// -----------------------------------------------------------------------------
static AutoRef<TextureResource> sRegisterBlackTexture( GpuResourceDatabase & gdb )
{
    AutoRef<TextureResource> tr = gdb.createResource<TextureResource>( "@BLACK" );
    AutoRef<Texture> t( gdb.gpu().create2DTexture( 2, 2, 0, ColorFormat::RGBA32 ) );
    UInt32 black[4] = { 0, 0, 0, 0 };
    t->updateMipmap( 0, 0, NULL, sizeof(UInt32)*2, sizeof(UInt32)*4, black, SurfaceUpdateFlag::DEFAULT );
    tr->setTexture( t );
    return tr;
}

//
// create a normal map texture for flat surface
// -----------------------------------------------------------------------------
static AutoRef<TextureResource> sRegisterFlatNormalMap( GpuResourceDatabase & gdb )
{
    AutoRef<TextureResource> tr = gdb.createResource<TextureResource>( "@FLAT_NORMAL_MAP" );
    AutoRef<Texture> t(  gdb.gpu().create2DTexture( 2, 2, 0, ColorFormat::RG_16_16_UNORM ) );
    UInt32 up[4] = { 0x80008000, 0x80008000, 0x80008000, 0x80008000 };
    t->updateMipmap( 0, 0, NULL, sizeof(UInt32)*2, sizeof(UInt32)*4, up, SurfaceUpdateFlag::DEFAULT );
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
        strPrintf( unnamed, GN_ARRAY_COUNT(unnamed), "Unnamed %s #%d", mgr->desc.cptr(), i );
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
    UInt32 internalHandle = mgr->resources.add( name, resimpl );
    if( 0 ==internalHandle ) return AutoRef<GpuResource>::NULLREF;
    resimpl->handle.set( (UInt32)mgr->index, internalHandle );

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

    UInt32 internalHandle = mgr->resources.name2handle(name);
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

    if( this != resource->database().mImpl ) return false;

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

    if( this != resource->database().mImpl ) return false;

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

    return true;
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

    if( this != resource->database().mImpl )
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
Gpu                & GpuResourceDatabase::gpu() const { return mImpl->gpu(); }
bool                 GpuResourceDatabase::registerResourceFactory( const Guid & type, const char * desc, GpuResourceFactory factory ) { return mImpl->registerResourceFactory( type, desc, factory ); }
bool                 GpuResourceDatabase::hasResourceFactory( const Guid & type ) { return mImpl->hasResourceFactory( type ); }
AutoRef<GpuResource> GpuResourceDatabase::createResource( const Guid & type, const char * name ) { return mImpl->createResource( type, name ); }
AutoRef<GpuResource> GpuResourceDatabase::findResource( const Guid & type, const char * name ) const { return mImpl->findResource( type, name ); }
bool                 GpuResourceDatabase::validResource( const Guid & type, const GpuResource * resource ) const { return mImpl->validResource( type, resource ); }
bool                 GpuResourceDatabase::validResource( const GpuResource * resource ) const { return mImpl->validResource( resource ); }
const char         * GpuResourceDatabase::getResourceName( const GpuResource * resource ) const { return mImpl->getResourceName(resource); }
const Guid         & GpuResourceDatabase::getResourceType( const GpuResource * resource ) const { return mImpl->getResourceType(resource); }
