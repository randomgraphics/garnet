#include "pch.h"
#include <garnet/gfx/fatModel.h>

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Local stuff
// *****************************************************************************

//
// Shader codes
// -----------------------------------------------------------------------------
static const char * SKINNED_VS_HLSL9 =
    "uniform float4x4 pvw; \n"
    "uniform float4x4 world; \n"
    "uniform float4x4 wit; \n"
    "uniform float4x4 joint_matrices[20]; \n"
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
    "uniform mat4 joint_matrices[20]; \n"
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
static AutoRef<EffectResource> sRegisterSkinnedEffect( GpuResourceDatabase & gdb )
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
    AutoRef<EffectResource> e = gdb.findOrCreateResource<EffectResource>( "@SKINNED", &isExistingResource );
    if( !isExistingResource && !e->reset( &ed ) )
    {
        return AutoRef<EffectResource>::NULLREF;
    }

    return e;
}


//
//
// -----------------------------------------------------------------------------
static ModelResourceDesc sSkinnedModelDesc()
{
    struct JointMatrices
    {
        Matrix44f matrices[20];

        JointMatrices()
        {
            for( size_t i = 0; i < GN_ARRAY_COUNT(matrices); ++i )
            {
                matrices[i].identity();
            }
        }
    };

    ModelResourceDesc md;
    md.effect = "@SKINNED";
    md.textures["ALBEDO_TEXTURE"].resourceName = "@WHITE";

#define INIT_UNIFORM( name, type, defval ) \
    if( 1 ) { \
        md.uniforms[name].size = sizeof(type); \
        md.uniforms[name].initialValue.resize( sizeof(type) ); \
        type def defval; \
        memcpy( md.uniforms[name].initialValue.cptr(), &def, sizeof(type) ); \
    } else void(0)

    INIT_UNIFORM( "MATRIX_PVW"      , Matrix44f,     = Matrix44f::sIdentity() );
    INIT_UNIFORM( "MATRIX_WORLD"    , Matrix44f,     = Matrix44f::sIdentity() );
    INIT_UNIFORM( "MATRIX_WORLD_IT" , Matrix44f,     = Matrix44f::sIdentity() );
    INIT_UNIFORM( "LIGHT0_POSITION" , Vector4f,      = Vector4f(0,0,0,0) );
    INIT_UNIFORM( "LIGHT0_DIFFUSE"  , Vector4f,      = Vector4f(1,1,1,1) );
    INIT_UNIFORM( "ALBEDO_COLOR"    , Vector4f,      = Vector4f(1,1,1,1) );
    INIT_UNIFORM( "JOINT_MATRICES"  , JointMatrices, );

    return md;
}

// *****************************************************************************
// SkinnedMesh
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::SkinnedMesh::SkinnedMesh()
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
GN::engine::SkinnedMesh::~SkinnedMesh()
{
    clear();
    setComponent<VisualComponent>( NULL );
    setComponent<SpacialComponent>( NULL );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SkinnedMesh::clear()
{
    mJoints.resize( 1 );
    mJoints[0].attach( new SpacialComponent );
    setComponent<SpacialComponent>( mJoints[0] );

    mVisual.attach( new VisualComponent );
    setComponent<VisualComponent>( mVisual );

    mSkinnedEffect.clear();
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
        return false;
    }

    GpuResourceDatabase & gdb = *getGdb();

    DynaArray<uint8> vb;

    // Load skinned effect
    mSkinnedEffect = sRegisterSkinnedEffect( gdb );
    if( NULL == mSkinnedEffect ) return false;

    // Load all meshes
    for( uint32 i = 0; i < fatmodel.meshes.size(); ++i )
    {
        if( NULL == fatmodel.meshes[i] ) continue;

        const FatMesh & fatmesh = *fatmodel.meshes[i];

        StrA meshName = stringFormat( "%s.mesh.%d", fatmodel.name, i );

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
            merd.indices = (void*)fatmesh.indices.cptr();

            // setup vertex format
            fatmesh.vertices.GenerateMeshVertexFormat( merd.vtxfmt );
            merd.strides[0] = math::alignToPowerOf2<uint16>( merd.vtxfmt.calcStreamStride( 0 ), 16 );

            // copy vertex data
            if( !vb.resize( merd.strides[0] * fatmesh.vertices.getVertexCount() ) ) continue;
            if( !fatmesh.vertices.GenerateVertexStream( merd.vtxfmt, 0, merd.strides[0], vb.cptr(), vb.size() ) ) continue;
            merd.vertices[0] = vb.cptr();

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
                        *(float*)p = (float)*(uint32*)p;
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

        // create one model for each mesh subset
        for( size_t s = 0; s < fatmesh.subsets.size(); ++s )
        {
            const FatMeshSubset & fatsubset = fatmesh.subsets[s];
            const FatMaterial   & fatmat    = fatmodel.materials[fatsubset.material];

            // setup model descriptor
            ModelResourceDesc mord = sSkinnedModelDesc();
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

            // create model and add to visual
            AutoRef<ModelResource> model = gdb.createResource<ModelResource>( NULL );
            if( model && model->reset( &mord ) )
            {
                mVisual->addModel( model );
            }
        }
    }

    // update bounding box
    mJoints[0]->setSelfBoundingBox( fatmodel.bbox );

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

