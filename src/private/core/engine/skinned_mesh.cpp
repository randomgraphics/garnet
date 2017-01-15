#include "pch.h"
#include <garnet/gfx/fatModel.h>

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Local Stuff
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
#define MAX_JOINTS_PER_DRAW     40
#define MAX_JOINTS_PER_MESH_STR "40"

//
//
// -----------------------------------------------------------------------------
#define INIT_UNIFORM( name, type, defval ) \
    if( 1 ) { \
        md.uniforms[name].size = sizeof(type); \
        md.uniforms[name].initialValue.resize( sizeof(type) ); \
        type def defval; \
        memcpy( md.uniforms[name].initialValue.rawptr(), &def, sizeof(type) ); \
    } else void(0)

//
//
// -----------------------------------------------------------------------------
template<typename T>
struct Interpolate
{
    // Linear interpolation
    static inline void sDoWork( T & result, const T & a, const T & b, float factor )
    {
        result = a * (1-factor) + b * factor;
    }
};

//
//
// -----------------------------------------------------------------------------
template<>
struct Interpolate<Quaternionf>
{
    // Spherical interpolation
    static inline void sDoWork( Quaternionf & res, const Quaternionf & from, const Quaternionf & to, float factor )
    {
        float  to1[4];
        float  scale0, scale1;
        double omega, cosom, sinom;

        // calculate cosine
        cosom = from.v.x * to.v.x + from.v.y * to.v.y + from.v.z * to.v.z + from.w * to.w;

        // adjust signs (if necessary)
        if ( cosom <0.0 )
        {
            cosom = -cosom; to1[0] = - to.v.x;
            to1[1] = - to.v.y;
            to1[2] = - to.v.z;
            to1[3] = - to.w;
        }
        else
        {
            to1[0] = to.v.x;
            to1[1] = to.v.y;
            to1[2] = to.v.z;
            to1[3] = to.w;
        }

        // calculate coefficients
        if ( (1.0 - cosom) > 0.01 )
        {
            // standard case (slerp)
            omega  = acos(cosom);
            sinom  = sin(omega);
            scale0 = (float)( sin((1.0 - factor) * omega) / sinom );
            scale1 = (float)( sin(factor * omega) / sinom );
        }
        else
        {
            // "from" and "to" quaternions are very close
            //  ... so we can do a linear interpolation
            scale0 = 1.0f - factor;
            scale1 = factor;
        }

        // calculate final values
        res.v.x = scale0 * from.v.x + scale1 * to1[0];
        res.v.y = scale0 * from.v.y + scale1 * to1[1];
        res.v.z = scale0 * from.v.z + scale1 * to1[2];
        res.w   = scale0 * from.w   + scale1 * to1[3];
    }
};

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline bool
sGetInterpolatedValue( T & result, const DynaArray<FatKeyFrame<T> > & array, float time )
{
    if( array.empty() ) return false;

    // binary search for the appropriate key frame.
    size_t first = 0;
    size_t last  = array.size() - 1;
    size_t mid;
    float  midtime;
    while( (first+1) < last )
    {
        mid = (first + last) / 2;

        midtime = array[mid].time;

        if( midtime < time )
        {
            first = mid;
        }
        else
        {
            last = mid;
        }
    }

    if( first == last )
    {
        result = array[first].value;
    }
    else
    {
        GN_ASSERT( first < last );

        // Cache the first and last key frames.
        const FatKeyFrame<T> & key1 = array[first];
        const FatKeyFrame<T> & key2 = array[last];

        // Calculate interpolation factor basing on time.
        float factor = (time - key1.time) / (key2.time - key1.time);

        // Interpolate between first and last, basing on time.
        Interpolate<T>::sDoWork( result, key1.value, key2.value, factor );
    }

    return true;
}

// *****************************************************************************
// Skinned Diffuse Effect
// *****************************************************************************

//
// shaders
// -----------------------------------------------------------------------------
static const char * SKINNED_VS_HLSL9 =
    "uniform float4x4 pvw; \n"
    "uniform float4x4 world; \n"
    "uniform float4x4 wit; \n"
    "uniform float4x4 joint_matrices[" MAX_JOINTS_PER_MESH_STR "]; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float4 pos_world : POSITION1;  // vertex position in world space \n"
    "   float3 nml_world : NORMAL0;    // vertex normal in world space \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "   float3 normal    : NORMAL0; \n"
    "   float4 joints    : TEXCOORD1;  // joint indices \n"
    "   float4 weights   : TEXCOORD2;  // joint weights \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   float4 skinned_pos = i.weights.x * mul( joint_matrices[i.joints.x], i.position ) + \n"
    "                        i.weights.y * mul( joint_matrices[i.joints.y], i.position ) + \n"
    "                        i.weights.z * mul( joint_matrices[i.joints.z], i.position ) + \n"
    "                        i.weights.w * mul( joint_matrices[i.joints.w], i.position );  \n"
    "   float4 input_nml   = float4( i.normal, 0 ); \n"
    "   float4 skinned_nml = i.weights.x * mul( joint_matrices[i.joints.x], input_nml ) + \n"
    "                        i.weights.y * mul( joint_matrices[i.joints.y], input_nml ) + \n"
    "                        i.weights.z * mul( joint_matrices[i.joints.z], input_nml ) + \n"
    "                        i.weights.w * mul( joint_matrices[i.joints.w], input_nml );  \n"
    "   o.hpos      = mul( pvw, skinned_pos ); \n"
    "   o.pos_world = mul( world, skinned_nml ); \n"
    "   o.nml_world = mul( wit, skinned_nml ).xyz; \n"
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

static const char * SKINNED_VS_GLSL =
    "attribute vec4 fjoints; \n"
    "attribute vec4 weights; \n"
    "\n"
    "uniform mat4 pvw; \n"
    "uniform mat4 world; \n"
    "uniform mat4 wit; \n"
    "uniform mat4 joint_matrices[" MAX_JOINTS_PER_MESH_STR "]; \n"
    "\n"
    "varying vec4  pos_world; // vertex position in world space \n"
    "varying vec3  nml_world; // vertex normal in world space \n"
    "varying vec2  texcoords; \n"
    "\n"
    "void main() { \n"
    "   ivec4 joints = ivec4(fjoints); \n"
    "   vec4 skinned_pos = weights.x * ( joint_matrices[joints.x] * gl_Vertex ) + \n"
    "                      weights.y * ( joint_matrices[joints.y] * gl_Vertex ) + \n"
    "                      weights.z * ( joint_matrices[joints.z] * gl_Vertex ) + \n"
    "                      weights.w * ( joint_matrices[joints.w] * gl_Vertex );  \n"
    "   vec4 input_nml   = vec4(gl_Normal,0); \n"
    "   vec4 skinned_nml = weights.x * ( joint_matrices[joints.x] * input_nml ) + \n"
    "                      weights.y * ( joint_matrices[joints.y] * input_nml ) + \n"
    "                      weights.z * ( joint_matrices[joints.z] * input_nml ) + \n"
    "                      weights.w * ( joint_matrices[joints.w] * input_nml );  \n"
    "   gl_Position = pvw * skinned_pos; \n"
    "   pos_world   = world * gl_Vertex; \n"
    "   nml_world   = (wit * skinned_nml).xyz; \n"
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
static AutoRef<EffectResource>
sRegisterSkinnedDiffuseEffect( GpuResourceDatabase & gdb )
{
    EffectResourceDesc ed;

    ed.uniforms[StandardUniform::Desc::MATRIX_PVW.name];
    ed.uniforms[StandardUniform::Desc::MATRIX_WORLD.name];
    ed.uniforms[StandardUniform::Desc::MATRIX_WORLD_IT.name];
    ed.uniforms[StandardUniform::Desc::LIGHT0_POSITION.name];
    ed.uniforms[StandardUniform::Desc::LIGHT0_DIFFUSE.name];
    ed.uniforms["JOINT_MATRICES"];
    ed.uniforms["ALBEDO_COLOR"];
    ed.textures["ALBEDO_TEXTURE"];
    ed.attributes["POSITION"];
    ed.attributes["NORMAL"];
    ed.attributes["JOINT_ID"];
    ed.attributes["JOINT_WEIGHT"];
    ed.attributes["TEXCOORD"];

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.shaderModels = ShaderModel::GLSL_1_00;
    ed.gpuprograms["glsl"].gpd.vs.source = SKINNED_VS_GLSL;
    ed.gpuprograms["glsl"].gpd.ps.source = DIFFUSE_PS_GLSL;
    ed.gpuprograms["glsl"].uniforms["pvw"] = StandardUniform::Desc::MATRIX_PVW.name;
    ed.gpuprograms["glsl"].uniforms["world"] = StandardUniform::Desc::MATRIX_WORLD.name;
    ed.gpuprograms["glsl"].uniforms["wit"] = StandardUniform::Desc::MATRIX_WORLD_IT.name;
    ed.gpuprograms["glsl"].uniforms["lightpos"] = StandardUniform::Desc::LIGHT0_POSITION.name;
    ed.gpuprograms["glsl"].uniforms["lightColor"] = StandardUniform::Desc::LIGHT0_DIFFUSE.name;
    ed.gpuprograms["glsl"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["glsl"].uniforms["joint_matrices"] = "JOINT_MATRICES";
    ed.gpuprograms["glsl"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.gpuprograms["glsl"].attributes["gl_Vertex"] = "POSITION0";
    ed.gpuprograms["glsl"].attributes["gl_Normal"] = "NORMAL0";
    ed.gpuprograms["glsl"].attributes["gl_MultiTexCoord0"] = "TEXCOORD0";
    ed.gpuprograms["glsl"].attributes["fjoints"] = "JOINT_ID";
    ed.gpuprograms["glsl"].attributes["weights"] = "JOINT_WEIGHT";

    ed.gpuprograms["hlsl9"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl9"].gpd.shaderModels = ShaderModel::SM_3_0 | ShaderModel::SM_3_X;
    ed.gpuprograms["hlsl9"].gpd.vs.source = SKINNED_VS_HLSL9;
    ed.gpuprograms["hlsl9"].gpd.vs.entry  = "main";
    ed.gpuprograms["hlsl9"].gpd.ps.source = DIFFUSE_PS_HLSL9;
    ed.gpuprograms["hlsl9"].gpd.ps.entry  = "main";
    ed.gpuprograms["hlsl9"].uniforms["pvw"] = StandardUniform::Desc::MATRIX_PVW.name;
    ed.gpuprograms["hlsl9"].uniforms["world"] = StandardUniform::Desc::MATRIX_WORLD.name;
    ed.gpuprograms["hlsl9"].uniforms["wit"] = StandardUniform::Desc::MATRIX_WORLD_IT.name;
    ed.gpuprograms["hlsl9"].uniforms["lightpos"] = StandardUniform::Desc::LIGHT0_POSITION.name;
    ed.gpuprograms["hlsl9"].uniforms["lightColor"] = StandardUniform::Desc::LIGHT0_DIFFUSE.name;
    ed.gpuprograms["hlsl9"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["hlsl9"].uniforms["joint_matrices"] = "JOINT_MATRICES";
    ed.gpuprograms["hlsl9"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.gpuprograms["hlsl9"].attributes["POSITION0"] = "POSITION";
    ed.gpuprograms["hlsl9"].attributes["NORMAL0"] = "NORMAL";
    ed.gpuprograms["hlsl9"].attributes["TEXCOORD0"] = "TEXCOORD";
    ed.gpuprograms["hlsl9"].attributes["TEXCOORD1"] = "JOINT_ID";
    ed.gpuprograms["hlsl9"].attributes["TEXCOORD2"] = "JOINT_WEIGHT";

    ed.techniques.resize( 2 );
    ed.techniques[0].name = "glsl";
    ed.techniques[0].passes.resize( 1 );
    ed.techniques[0].passes[0].gpuprogram = "glsl";
    ed.techniques[1].name = "hlsl9";
    ed.techniques[1].passes.resize( 1 );
    ed.techniques[1].passes[0].gpuprogram = "hlsl9";

    bool isExistingResource;
    AutoRef<EffectResource> e = gdb.findOrCreateResource<EffectResource>( "@SKINNED_DIFFUSE", &isExistingResource );
    if( !isExistingResource && !e->reset( &ed ) )
    {
        return AutoRef<EffectResource>::NULLREF;
    }

    return e;
}

//
//
// -----------------------------------------------------------------------------
static ModelResourceDesc sSkinnedDiffuseModelDesc()
{
    struct JointMatrices
    {
        Matrix44f matrices[MAX_JOINTS_PER_DRAW];

        JointMatrices()
        {
            for( size_t i = 0; i < GN_ARRAY_COUNT(matrices); ++i )
            {
                matrices[i].identity();
            }
        }
    };

    ModelResourceDesc md;
    md.effect = "@SKINNED_DIFFUSE";
    md.textures["ALBEDO_TEXTURE"].resourceName = "@WHITE";

    INIT_UNIFORM( "MATRIX_PVW"      , Matrix44f,     = Matrix44f::sIdentity() );
    INIT_UNIFORM( "MATRIX_WORLD"    , Matrix44f,     = Matrix44f::sIdentity() );
    INIT_UNIFORM( "MATRIX_WORLD_IT" , Matrix44f,     = Matrix44f::sIdentity() );
    INIT_UNIFORM( "LIGHT0_POSITION" , Vector4f,      = Vector4f(0,0,0,0) );
    INIT_UNIFORM( "LIGHT0_DIFFUSE"  , Vector4f,      = Vector4f(1,1,1,1) );
    INIT_UNIFORM( "ALBEDO_COLOR"    , Vector4f,      = Vector4f(1,1,1,1) );
    INIT_UNIFORM( "JOINT_MATRICES"  , JointMatrices, );

    return md;
}

#if 0
// *****************************************************************************
// Skinned Line Effect
// *****************************************************************************

//
// shaders
// -----------------------------------------------------------------------------
static const char * SKINNED_LINE_VS_HLSL9 =
    "uniform float4x4 pvw; \n"
    "uniform float4x4 joint_matrices[" MAX_JOINTS_PER_MESH_STR "]; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float4 joints    : TEXCOORD0;  // joint indices \n"
    "   float4 weights   : TEXCOORD1;  // joint weights \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   float4 skinned_pos = i.weights.x * mul( joint_matrices[i.joints.x], i.position ) + \n"
    "                        i.weights.y * mul( joint_matrices[i.joints.y], i.position ) + \n"
    "                        i.weights.z * mul( joint_matrices[i.joints.z], i.position ) + \n"
    "                        i.weights.w * mul( joint_matrices[i.joints.w], i.position );  \n"
    "   o.hpos = mul( pvw, skinned_pos ); \n"
    "   return o; \n"
    "}";

static const char * SKINNED_LINE_PS_HLSL9 =
    "uniform float4 albedoColor; \n"
    "float4 main() : COLOR0 { \n"
    "   return albedoColor; \n"
    "}";

static const char * SKINNED_LINE_VS_GLSL =
    "attribute vec4 fjoints; \n"
    "attribute vec4 weights; \n"
    "\n"
    "uniform mat4 pvw; \n"
    "uniform mat4 joint_matrices[" MAX_JOINTS_PER_MESH_STR "]; \n"
    "\n"
    "void main() { \n"
    "   ivec4 joints = ivec4(fjoints); \n"
    "   vec4 skinned_pos = weights.x * ( joint_matrices[joints.x] * gl_Vertex ) + \n"
    "                      weights.y * ( joint_matrices[joints.y] * gl_Vertex ) + \n"
    "                      weights.z * ( joint_matrices[joints.z] * gl_Vertex ) + \n"
    "                      weights.w * ( joint_matrices[joints.w] * gl_Vertex );  \n"
    "   gl_Position = pvw * skinned_pos; \n"
    "}";

static const char * SKINNED_LINE_PS_GLSL =
    "uniform vec4 albedoColor; \n"
    "void main() { \n"
    "   gl_FragColor = albedoColor; \n"
    "}";

//
//
// -----------------------------------------------------------------------------
static AutoRef<EffectResource>
sRegisterSkinnedLineEffect( GpuResourceDatabase & gdb )
{
    EffectResourceDesc ed;

    ed.uniforms[StandardUniform::Desc::MATRIX_PVW.name];
    ed.uniforms["JOINT_MATRICES"];
    ed.uniforms["ALBEDO_COLOR"];
    ed.attributes["POSITION"];
    ed.attributes["JOINT_ID"];
    ed.attributes["JOINT_WEIGHT"];

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.shaderModels = ShaderModel::GLSL_1_00;
    ed.gpuprograms["glsl"].gpd.vs.source = SKINNED_LINE_VS_GLSL;
    ed.gpuprograms["glsl"].gpd.ps.source = SKINNED_LINE_PS_GLSL;
    ed.gpuprograms["glsl"].uniforms["pvw"] = StandardUniform::Desc::MATRIX_PVW.name;
    ed.gpuprograms["glsl"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["glsl"].uniforms["joint_matrices"] = "JOINT_MATRICES";
    ed.gpuprograms["glsl"].attributes["gl_Vertex"] = "POSITION";
    ed.gpuprograms["glsl"].attributes["fjoints"] = "JOINT_ID";
    ed.gpuprograms["glsl"].attributes["weights"] = "JOINT_WEIGHT";

    ed.gpuprograms["hlsl9"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl9"].gpd.shaderModels = ShaderModel::SM_3_0 | ShaderModel::SM_3_X;
    ed.gpuprograms["hlsl9"].gpd.vs.source = SKINNED_LINE_VS_HLSL9;
    ed.gpuprograms["hlsl9"].gpd.vs.entry  = "main";
    ed.gpuprograms["hlsl9"].gpd.ps.source = SKINNED_LINE_PS_HLSL9;
    ed.gpuprograms["hlsl9"].gpd.ps.entry  = "main";
    ed.gpuprograms["hlsl9"].uniforms["pvw"] = StandardUniform::Desc::MATRIX_PVW.name;
    ed.gpuprograms["hlsl9"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.gpuprograms["hlsl9"].uniforms["joint_matrices"] = "JOINT_MATRICES";
    ed.gpuprograms["hlsl9"].attributes["POSITION0"] = "POSITION";
    ed.gpuprograms["hlsl9"].attributes["TEXCOORD0"] = "JOINT_ID";
    ed.gpuprograms["hlsl9"].attributes["TEXCOORD1"] = "JOINT_WEIGHT";

    ed.techniques.resize( 2 );
    ed.techniques[0].name = "glsl";
    ed.techniques[0].passes.resize( 1 );
    ed.techniques[0].passes[0].gpuprogram = "glsl";
    ed.techniques[1].name = "hlsl9";
    ed.techniques[1].passes.resize( 1 );
    ed.techniques[1].passes[0].gpuprogram = "hlsl9";

    bool isExistingResource;
    AutoRef<EffectResource> e = gdb.findOrCreateResource<EffectResource>( "@SKINNED_LINE", &isExistingResource );
    if( !isExistingResource && !e->reset( &ed ) )
    {
        return AutoRef<EffectResource>::NULLREF;
    }

    return e;
}

//
//
// -----------------------------------------------------------------------------
static ModelResourceDesc sSkinnedWireframeModelDesc()
{
    struct JointMatrices
    {
        Matrix44f matrices[MAX_JOINTS_PER_DRAW];

        JointMatrices()
        {
            for( size_t i = 0; i < GN_ARRAY_COUNT(matrices); ++i )
            {
                matrices[i].identity();
            }
        }
    };

    ModelResourceDesc md;
    md.effect = "@SKINNED_LINE";

    INIT_UNIFORM( "MATRIX_PVW"      , Matrix44f,     = Matrix44f::sIdentity() );
    INIT_UNIFORM( "ALBEDO_COLOR"    , Vector4f,      = Vector4f(1,1,1,1) );
    INIT_UNIFORM( "JOINT_MATRICES"  , JointMatrices, );

    return md;
}

#endif

// *****************************************************************************
// SkinnedVisualComponent
// *****************************************************************************

class GN::engine::SkinnedMesh::SkinnedVisualComponent : public VisualComponent
{
    SkinnedMesh  & mOwner;

protected:

    virtual void drawModelResource( uint32 index, ModelResource & model ) const
    {
        // Reference the subset
        GN_ASSERT( index < mOwner.mSubsets.size() );
        const SkinnedMesh::SkinnedSubset & subset = mOwner.mSubsets[index];

        Matrix44f matrices[MAX_JOINTS_PER_DRAW];

        // Upload joint matrices to GPU uniform
        if( FatMesh::NO_SKELETON != subset.skeleton )
        {
            const SkinnedMesh::Skeleton & sk = mOwner.mSkeletons[subset.skeleton];

            Uniform * uniform = sk.matrices->uniform().rawptr();

            GN_ASSERT( uniform->size() >= sizeof(Matrix44f)*MAX_JOINTS_PER_DRAW );
            GN_ASSERT( subset.joints.size() <= MAX_JOINTS_PER_DRAW );

            for( uint32 i = 0; i < subset.joints.size(); ++i )
            {
                uint32 jointIndex = subset.joints[i];
                matrices[i] = sk.bind2rest[jointIndex];
            }

            size_t uploadBytes = sizeof(Matrix44f) * subset.joints.size();

            uniform->update( 0, (uint32)uploadBytes, matrices );
        }

        // draw the GPU model resource.
        model.draw();
    }

public:

    SkinnedVisualComponent( SkinnedMesh & owner )
        : mOwner(owner)
    {
    }
};

// *****************************************************************************
// SkinnedAnimation
// *****************************************************************************

struct GN::engine::SkinnedMesh::SkinnedAnimation : public FatAnimation
{
    static void sDeleteAnimation( SkinnedAnimation * p )
    {
        delete p;
    }
};

// *****************************************************************************
// SkinnedMesh
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static const GN::Guid SKINNED_MESH_GUID = { 0x950374b7, 0x3b50, 0x4d6e, { 0x9b, 0xd3, 0xa2, 0x36, 0xbe, 0xdb, 0x8e, 0x83 } };
GN_ENGINE_IMPLEMENT_ENTITY( GN::engine::SkinnedMesh, SKINNED_MESH_GUID );

//
//
// -----------------------------------------------------------------------------
uint32 GN::engine::SkinnedMesh::sGetMaxJointsPerDraw()
{
    return MAX_JOINTS_PER_DRAW;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::SkinnedMesh::SkinnedMesh()
{
    setComponent<SpacialComponent>( &mRootSpacial );
    mVisual = new SkinnedVisualComponent( *this );
    setComponent<VisualComponent>( mVisual );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::SkinnedMesh::~SkinnedMesh()
{
    clear();
    setComponent<VisualComponent>( NULL );
    setComponent<SpacialComponent>( NULL );
    delete mVisual;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SkinnedMesh::clear()
{
    // Delete all animations
    std::for_each( mAnimations.begin(), mAnimations.end(), SkinnedAnimation::sDeleteAnimation );
    mAnimations.clear();

    // Delete all skeletons
    for( size_t i = 0; i < mSkeletons.size(); ++i )
    {
        Skeleton & sk = mSkeletons[i];
        safeHeapDealloc( sk.hierarchy );
        for( size_t j = 0; j < sk.jointCount; ++j )
        {
            safeDelete( sk.spacials[j] );
        }
        safeHeapDealloc( sk.spacials );
        safeHeapDealloc( sk.bindPose );
        safeHeapDealloc( sk.invRestPose );
        safeHeapDealloc( sk.bind2rest );
        safeDecref( sk.matrices );
    }
    mSkeletons.clear();

    // Delete all subsets.
    mSubsets.clear();

    // Delete the effect
    mSkinnedEffect.clear();

    // Clear the visual component (but do not delete the visual instance)
    mVisual->clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::SkinnedMesh::getAnimationInfo( size_t animationIndex, SkinnedAnimationInfo & info )
{
    if( animationIndex >= mAnimations.size() || NULL == mAnimations[animationIndex] )
    {
        // Invalid animation index
        return false;
    }

    const SkinnedAnimation * sa = mAnimations[animationIndex];

    info.name = sa->name;
    info.duration = sa->duration;

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SkinnedMesh::setAnimation( size_t animationIndex, float seconds )
{
    if( (size_t)-1 == animationIndex )
    {
        // reset back to bind pose.

        for( uint32 skeletonIndex = 0; skeletonIndex < mSkeletons.size(); ++skeletonIndex )
        {
            Skeleton & sk = mSkeletons[skeletonIndex];

            // bind pose is rest pose in this case.
            for( uint32 jointIndex = 0; jointIndex < sk.jointCount; ++jointIndex )
            {
                SpacialComponent * spacial = sk.spacials[jointIndex];
                spacial->setPosition( sk.bindPose[jointIndex].position );
                spacial->setRotation( sk.bindPose[jointIndex].rotation );
                spacial->setScale( sk.bindPose[jointIndex].scaling );
            }
            for( uint32 jointIndex = 0; jointIndex < sk.jointCount; ++jointIndex )
            {
                sk.invRestPose[jointIndex] = Matrix44f::sInverse( sk.bindPose[jointIndex].model2joint );
                sk.bind2rest[jointIndex].identity();
            }

            /* And the bind pose -> rest pose transformation should be identity.
            Uniform * uniform = sk.matrices->uniform().rawptr();
            GN_ASSERT( uniform->size() >= sizeof(identityMatrices) );
            uniform->update( 0, (uint32)sizeof(identityMatrices), identityMatrices );*/
        }
    }
    else
    {
        if( animationIndex >= mAnimations.size() || NULL == mAnimations[animationIndex] )
        {
            GN_ERROR(sLogger)( "Invalid animation index." );
            return;
        }

        const FatAnimation & fatanim = *mAnimations[animationIndex];

        // Mod time stamp by animation duration.
        seconds = fmod( seconds, (float)fatanim.duration );

        Vector3f    t;
        Quaternionf r;
        Vector3f    s;
        Matrix44f   local2parent;

        for( uint32 skeletonIndex = 0; skeletonIndex < fatanim.skeletonAnimations.size(); ++skeletonIndex )
        {
            const DynaArray<FatJointAnimation> & skanim = fatanim.skeletonAnimations[skeletonIndex];

            Skeleton & sk = mSkeletons[skeletonIndex];

            GN_ASSERT( skanim.size() == sk.jointCount );

            // Loop through all joints, update each spacial component with
            // the rest pose transformation.
            for( uint32 jointIndex = 0; jointIndex < sk.jointCount; ++jointIndex )
            {
                const FatJointAnimation & jointanim = skanim[jointIndex];

                SpacialComponent * spacial = sk.spacials[jointIndex];

                // Get rest pose tranformation of the joint. T*R*S is the
                // the transformation from local space to it's parent space.
                if( sGetInterpolatedValue( t, jointanim.positions, seconds ) &&
                    sGetInterpolatedValue( r, jointanim.rotations, seconds ) &&
                    sGetInterpolatedValue( s, jointanim.scalings, seconds ) )
                {
                    spacial->setPosition( t );
                    spacial->setRotation( r );
                    spacial->setScale( s );
                }
                else
                {
                    // No rest pose found for this joint at this time. We'll use
                    // bind pose as rest pose.
                    spacial->setPosition( sk.bindPose[jointIndex].position );
                    spacial->setRotation( sk.bindPose[jointIndex].rotation );
                    spacial->setScale( sk.bindPose[jointIndex].scaling );
                }
            }

            // Loop through all joints again. Caluclate bind pose -> rest post
            // transformation for each joint.
            for( uint32 jointIndex = 0; jointIndex < sk.jointCount; ++jointIndex )
            {
                // Reference the spacial component of the joint.
                SpacialComponent * spacial = sk.spacials[jointIndex];

                // Reference the bind pose matrix, which is the bind pose
                // (in model space) to joint space transformation.
                const Matrix44f & bind_to_joint = sk.bindPose[jointIndex].model2joint;

                // Reference the inverse of rest pose matrix. ( joint space -> rest pose )
                Matrix44f & joint_to_rest = sk.invRestPose[jointIndex];

                // Query rest pose tranformation of the joint from spacial component.
                joint_to_rest = spacial->getLocal2Root();

                // bind -> rest = bind -> joint -> rest
                sk.bind2rest[jointIndex] = joint_to_rest * bind_to_joint;
            }

            /* update the matrix uniform.
            Uniform * uniform = sk.matrices->uniform().rawptr();
            GN_ASSERT( uniform->size() >= sizeof(Matrix44f)*MAX_JOINTS_PER_DRAW );
            size_t bytes = sizeof(Matrix44f) * math::getmin<size_t>(MAX_JOINTS_PER_DRAW,sk.jointCount);
            uniform->update( 0, (uint32)bytes, matrices );*/
        }
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::SkinnedMesh::loadFromFatModel( const GN::gfx::FatModel & fatmodel )
{
    GN_SCOPE_PROFILER( SkinnedMesh_loadFromFatModel, "Load skinned mesh from FatModel" );

    // Clear existing data.
    clear();

    if( fatmodel.skeletons.empty() )
    {
        GN_ERROR(sLogger)( "The fat model does not contain any skeletons." );
        clear();
        return false;
    }

    GpuResourceDatabase & gdb = *getGdb();

    DynaArray<uint8> vb;

    // Load skinned effect
    mSkinnedEffect = sRegisterSkinnedDiffuseEffect( gdb );
    if( NULL == mSkinnedEffect ) return false;

    // Load skeleton array
    mSkeletons.resize( fatmodel.skeletons.size() );
    memset( mSkeletons.rawptr(), 0, sizeof(Skeleton)*mSkeletons.size() );
    for( uint32 i = 0; i < fatmodel.skeletons.size(); ++i )
    {
        const FatSkeleton & source = fatmodel.skeletons[i];

        Skeleton & dest = mSkeletons[i];
        dest.jointCount = source.joints.size();
        dest.hierarchy = (JointHierarchy*)HeapMemory::alloc( sizeof(JointHierarchy) * dest.jointCount );
        dest.spacials = (SpacialComponent**)HeapMemory::alloc( sizeof(void*) * dest.jointCount );
        dest.bindPose = (JointBindPose*)HeapMemory::alloc( sizeof(JointBindPose) * dest.jointCount );
        dest.invRestPose = (Matrix44f*)HeapMemory::alloc( sizeof(Matrix44f) * dest.jointCount );
        dest.bind2rest = (Matrix44f*)HeapMemory::alloc( sizeof(Matrix44f) * dest.jointCount );
        if( !dest.hierarchy || !dest.spacials || !dest.bindPose || !dest.invRestPose || !dest.bind2rest )
        {
            GN_ERROR(sLogger)( "Fail to load skinned mesh from FatModel: out of memory." );
            clear();
            return false;
        }

        // Loop through each joints (first pass)
        for( uint32 j = 0; j < source.joints.size(); ++j )
        {
            const FatJoint & srcjoint = source.joints[j];

            // Replicate the hierarchy
            dest.hierarchy[j].parent  = srcjoint.parent;
            dest.hierarchy[j].child   = srcjoint.child;
            dest.hierarchy[j].sibling = srcjoint.sibling;

            // Create one spacial component per joint.
            dest.spacials[j] = new SpacialComponent;

            // setup the local transformation of the spacial component
            dest.spacials[j]->setPosition( srcjoint.bindPose.position );
            dest.spacials[j]->setRotation( srcjoint.bindPose.rotation );
            dest.spacials[j]->setScale( srcjoint.bindPose.scaling );

            // copy bind pose transformation
            dest.bindPose[j].model2joint = srcjoint.bindPose.model2joint;
            dest.bindPose[j].position = srcjoint.bindPose.position;
            dest.bindPose[j].rotation = srcjoint.bindPose.rotation;
            dest.bindPose[j].scaling = srcjoint.bindPose.scaling;

            // Initial rest pose is same as bind pose.
            dest.invRestPose[j] = Matrix44f::sInverse( srcjoint.bindPose.model2joint );

            // And Initial bind->rest transformation would be identity
            dest.bind2rest[j].identity();
        }

        // Loop through each joints again (second pass)
        //
        // Connect spacial components in the same way as how joints are
        // connected. Note that joint spacial componets are not
        // linked to the root spacial component of the whole model.
        // The reason is that those joint spacial components are used
        // calculate transformation from joint space to model space.
        // Transfomration outside of the model space should not be
        // involved.
        for( uint32 j = 0; j < source.joints.size(); ++j )
        {
            uint32 parent = source.joints[j].parent;
            dest.spacials[j]->setParent( (parent == FatJoint::NO_JOINT) ? NULL : dest.spacials[parent] );
        }

        // Create a uniform resource for the skeleton
        dest.matrices = gdb.createResource<UniformResource>( NULL ).detach();
        if( NULL == dest.matrices || !dest.matrices->reset( sizeof(Matrix44f)*MAX_JOINTS_PER_DRAW, NULL ) )
        {
            clear();
            return false;
        }
    }

    // Load all meshes
    for( uint32 mi = 0; mi < fatmodel.meshes.size(); ++mi )
    {
        if( NULL == fatmodel.meshes[mi] ) continue;

        const FatMesh & fatmesh = *fatmodel.meshes[mi];

        StrA meshName = str::format( "%s.mesh.%d", fatmodel.name.rawptr(), mi );

        // use exising mesh, if possible
        AutoRef<MeshResource> mesh = gdb.findResource<MeshResource>( meshName );
        if( !mesh )
        {
            // setup mesh descriptor
            MeshResourceDesc merd;
            memset( &merd, 0, sizeof(merd) );
            merd.prim = fatmesh.primitive;
            merd.numvtx = fatmesh.vertices.getVertexCount();
            merd.numidx = fatmesh.indices.size();
            merd.idx32 = true; // TODO: use 16-bit index buffer, when possible.
            merd.offsets[0] = 0;
            merd.indices = (void*)fatmesh.indices.rawptr();

            // setup vertex format
            fatmesh.vertices.GenerateMeshVertexFormat( merd.vtxfmt );
            merd.strides[0] = math::alignToPowerOf2<uint16>( merd.vtxfmt.calcStreamStride( 0 ), 16 );

            // copy vertex data
            if( !vb.resize( merd.strides[0] * fatmesh.vertices.getVertexCount() ) ) continue;
            if( !fatmesh.vertices.GenerateVertexStream( merd.vtxfmt, 0, merd.strides[0], vb.rawptr(), vb.size() ) ) continue;
            merd.vertices[0] = vb.rawptr();

            // convert integer joint index to floats to workaround hardware liminations (not all hardware supports integer
            // vertex elements)
            uint32 jointSemanticIndex;
            if( merd.vtxfmt.hasSemantic( "JOINT_ID", &jointSemanticIndex ) )
            {
                MeshVertexElement & mve = merd.vtxfmt.elements[jointSemanticIndex];
                if( mve.format == ColorFormat::UINT4 )
                {
                    mve.format = ColorFormat::FLOAT4;
                    uint8 * p = (uint8*)merd.vertices[0] + mve.offset;
                    for( uint32 i = 0; i < merd.numvtx; ++i, p += merd.strides[0] )
                    {
                        // Offset the value by 0.5 to avoid float to integer rounding error. Or else,
                        // it is possible that an integer value, for example 10, could be converted to
                        // floating point value 9.999999999. When it is converted back to integer
                        // in shader, it becomes 9.
                        uint32 * joints = (uint32*)p;
                        ((float*)joints)[0] = (float)(joints[0]==FatJoint::NO_JOINT?255:joints[0]) + 0.5f;
                        ((float*)joints)[1] = (float)(joints[1]==FatJoint::NO_JOINT?255:joints[1]) + 0.5f;
                        ((float*)joints)[2] = (float)(joints[2]==FatJoint::NO_JOINT?255:joints[2]) + 0.5f;
                        ((float*)joints)[3] = (float)(joints[3]==FatJoint::NO_JOINT?255:joints[3]) + 0.5f;
                    }
                }
            }

            // create GPU mesh resource
            mesh = gdb.createResource<MeshResource>( meshName );
            if( !mesh || !mesh->reset( &merd ) )
            {
                continue;
            }
        }

        GN_ASSERT( mesh );

        // reserve memory for mesh subsets.
        if( !mSubsets.reserve( fatmesh.subsets.size() ) )
        {
            GN_ERROR(sLogger)( "Out of memory." );
            clear();
            return false;
        }

        // Loop through all mesh subsets. Create one model for each subset.
        for( size_t s = 0; s < fatmesh.subsets.size(); ++s )
        {
            const FatMeshSubset & fatsubset = fatmesh.subsets[s];
            const FatMaterial   & fatmat    = fatmodel.materials[fatsubset.material];

            uint32 jointCountInTheSubset = fatsubset.joints.size();
            if( jointCountInTheSubset > MAX_JOINTS_PER_DRAW )
            {
                GN_ERROR(sLogger)( "Ignore mesh %s subset %d. It contains too many joints (#%d) then the current code allowed (#%d)",
                    meshName.rawptr(), s, jointCountInTheSubset, MAX_JOINTS_PER_DRAW );
                continue;
            }

            // setup model descriptor
            ModelResourceDesc mord = sSkinnedDiffuseModelDesc();
            mord.mesh = meshName;
            mord.subset.basevtx = fatsubset.basevtx;
            mord.subset.numvtx = fatsubset.numvtx;
            mord.subset.startidx = fatsubset.startidx;
            mord.subset.numidx = fatsubset.numidx;

            // associate textures to the model
            if( mord.hasTexture("ALBEDO_TEXTURE") && !fatmat.albedoTexture.empty() )
            {
                mord.textures["ALBEDO_TEXTURE"].resourceName = fatmat.albedoTexture;
            }
            if( mord.hasTexture("NORMAL_TEXTURE") && !fatmat.normalTexture.empty() )
            {
                mord.textures["NORMAL_TEXTURE"].resourceName = fatmat.normalTexture;
            }

            // create new GPU model resource
            AutoRef<ModelResource> model = gdb.createResource<ModelResource>( NULL );
            if( !model || !model->reset( &mord ) ) continue;

            // add model to visual component.
            if( !mVisual->addModel( model ) ) continue;

            // remember the subset information.
            GN_VERIFY( mSubsets.resize( mSubsets.size() + 1 ) );
            mSubsets.back().skeleton = fatmesh.skeleton;
            mSubsets.back().joints = fatsubset.joints;

            // bind joint matrix uniform to the mesh.
            uint32 skeletonIndex = fatmesh.skeleton;
            if( skeletonIndex != FatMesh::NO_SKELETON )
            {
                model->setUniformResource( "JOINT_MATRICES", mSkeletons[skeletonIndex].matrices );
            }
        }
    }

    // loading all animations
    if( mAnimations.resize( fatmodel.animations.size() ) )
    {
        for( uint32 i = 0; i < fatmodel.animations.size(); ++i )
        {
            mAnimations[i] = new SkinnedAnimation;
            const FatAnimation & src = fatmodel.animations[i];
            FatAnimation & dst = *(FatAnimation*)mAnimations[i];
            dst = src;
            GN_ASSERT( dst.duration == src.duration );
        }
    }
    else
    {
        GN_ERROR(sLogger)( "Fail to load animations: out of memory." );
    }

    // update bounding box
    mRootSpacial.setSelfBoundingBox( fatmodel.bbox );

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::SkinnedMesh::loadFromFile( const StrA & filename )
{
    FatModel fm;
    if( !fm.loadFromFile( filename ) ) return false;
    return loadFromFatModel( fm );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SkinnedMesh::drawSkeletons( uint32 colorInRGBA, const Matrix44f & transform )
{
    LineRenderer * lr = getLineRenderer();
    if( NULL == lr ) return;

    if( !lr->batchingBegin() ) return;

    Vector3f line[2];

    Vector3f zero( 0, 0, 0 );

    Matrix44f parent, current;

    Matrix44f finalTransform = transform * mRootSpacial.getLocal2Root();

    // Loop through skeletons
    for( uint32 i = 0; i < mSkeletons.size(); ++i )
    {
        const Skeleton & sk = mSkeletons[i];

        // Loop through joints
        for( uint32 j = 0; j < sk.jointCount; ++j )
        {
            const JointHierarchy & h = sk.hierarchy[j];

            // if the joint has parent, then draw
            // a line between the joint and the parent.
            if( FatJoint::NO_JOINT != h.parent )
            {
#if 1
                //
                // Show rest pose skeleton
                //
                // rest pose is transformation from model space to
                // joint space. So we use invese of it to transform
                // Point (0,0,0) to joint position in model space.
                //
                parent  = sk.invRestPose[h.parent];
                current = sk.invRestPose[j];
                parent.transformPoint( line[0], zero );
                current.transformPoint( line[1], zero );
#elif 0
                //
                // Show bind pose skeleton.
                //
                // bind pose is transformation from model space to
                // joint space. So we use invese of it to transform
                // Point (0,0,0) to joint position in model space.
                //
                parent = Matrix44f::sInverse( sk.bindPose[h.parent].model2joint );
                current = Matrix44f::sInverse( sk.bindPose[j].model2joint );
                parent.transformPoint( line[0], zero );
                current.transformPoint( line[1], zero );
#else
                //
                // Show rest pose skeleton, using transformation stored
                // in the uniform resource.
                //
                // This is to verify correctness of matrices stored in
                // the uniform resource.
                //

                // transform to bind pose first
                parent = Matrix44f::sInverse( sk.bindPose[h.parent].model2joint );
                current = Matrix44f::sInverse( sk.bindPose[j].model2joint );
                parent.transformPoint( line[0], zero );
                current.transformPoint( line[1], zero );

                // then transform to rest pose
                const Matrix44f * b2r = (const Matrix44f*)sk.matrices->uniform()->getval();
                b2r[h.parent].transformPoint( line[0], line[0] );
                b2r[j].transformPoint( line[1], line[1] );
#endif
                // draw a line between parent and current joint.
                lr->drawLines(
                    line,
                    sizeof(line[0]),
                    2,
                    colorInRGBA,
                    finalTransform );
            }
        }
    }

    lr->batchingEnd();
}
