#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#if GN_MSVC
#pragma comment( lib, "dxerr9.lib" )
#if GN_BUILD_DEBUG
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif
#endif

using namespace GN;
using namespace GN::gfx;

struct DiffuseEffect : public BasicEffect
{
    bool create()
    {
        Renderer & r = gRenderer;

        if( !r.supportShader( "vs_1_1" ) || !r.supportShader( "ps_1_1" ) ) return false;

        // create VS
        static const char * vscode =
            "struct VSIN                                          \n"
            "{                                                    \n"
            "    float4 pos : POSITION;                           \n"
            "    float3 nrm : NORMAL;                             \n"
            "};                                                   \n"
            "struct VSOUT                                         \n"
            "{                                                    \n"
            "    float4 pos : POSITION;                           \n"
            "    float4 clr : COLOR0;                             \n"
            "};                                                   \n"
            "uniform float4x4 gPVW : register(c0);                \n"
            "VSOUT main( VSIN i )                                 \n"
            "{                                                    \n"
            "    const float3 LIGHT_DIR = {0,1,0};                \n"
            "    VSOUT o;                                         \n"
            "    o.pos = mul( i.pos, gPVW );                      \n"
            "    o.clr = saturate(dot(i.nrm,LIGHT_DIR)) + 0.2;    \n"
            "    return o;                                        \n"
            "}";
        vs.attach( r.createVS( LANG_D3D_HLSL, vscode, "sm30=no" ) );
        if( !vs ) return false;

        // create PS
        static const char * pscode = "float4 main( in float4 clr : COLOR0 ) : COLOR0 { return clr; }";
        ps.attach( r.createPS( LANG_D3D_HLSL, pscode, "sm30=no" ) );
        if( !ps ) return false;

        // initialize projection matrix

        // success
        return true;
    }
};

struct DenseMesh
{
    size_t PRIM_COUNT;
    size_t DRAW_COUNT;

    AutoComPtr<ID3DXMesh> mesh;
    DWORD numMaterials;
    Spheref boundingSphere;

    DenseMesh() : DRAW_COUNT(1) {}

    bool create()
    {
        Renderer & r = gRenderer;
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)r.getD3DDevice();
        if( 0 == dev )
        {
            GN_ERROR(sLogger)( "only work with D3D9 renderer" );
            return false;
        }

        // search media file
        StrA filename = app::SampleResourceManager::sGetNativeResourceFileName( "mesh\\knot.x" );
        if( filename.Empty() )
        {
            GN_ERROR(sLogger)( "mesh file not found!" );
            return false;
        }

        // load mesh from file
        AutoComPtr<ID3DXMesh> sysMesh;
        AutoComPtr<ID3DXBuffer> adjacency;
        GN_DX_CHECK_RETURN( D3DXLoadMeshFromXA(
            filename.GetRawPtr(),
            D3DXMESH_SYSTEMMEM,
            dev,
            &adjacency,
            NULL,
            NULL,
            &numMaterials,
            &sysMesh ), false );

        // Lock the vertex buffer, to generate a simple bounding sphere
        AutoComPtr<IDirect3DVertexBuffer9> vb;
        void * vertices;
        GN_DX_CHECK_RETURN( sysMesh->GetVertexBuffer( &vb ), false );
        GN_DX_CHECK_RETURN( vb->Lock( 0, 0, &vertices, D3DLOCK_NOSYSLOCK ), false );
        GN_DX_CHECK_RETURN( D3DXComputeBoundingSphere(
            (D3DXVECTOR3*)vertices, sysMesh->GetNumVertices(),
            D3DXGetFVFVertexSize( sysMesh->GetFVF() ),
            (D3DXVECTOR3*)&boundingSphere.center,
            &boundingSphere.radius ), false );
        vb->Unlock();

        // adjust FVF
        bool hasNormal = ( sysMesh->GetFVF() & D3DFVF_NORMAL ) != 0;
        if( sysMesh->GetFVF() != (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1) )
        {
            AutoComPtr<ID3DXMesh> tmp;
            GN_DX_CHECK_RETURN( sysMesh->CloneMeshFVF(
                    sysMesh->GetOptions(),
                    D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1,
                    dev, &tmp ), false );
            sysMesh = tmp;
        }

        // Compute normals for the mesh, if not present
        if( !hasNormal )
        {
            GN_DX_CHECK_RETURN( D3DXComputeNormals( sysMesh, NULL ), false );
        }

        // make sure vertex size is 32 bytes
        GN_ASSERT( sysMesh->GetNumBytesPerVertex() == 32 );

        // do optimize
        AutoComPtr<ID3DXMesh> optimizedMesh;
        GN_DX_CHECK_RETURN( sysMesh->Optimize(
            D3DXMESHOPT_VERTEXCACHE | sysMesh->GetOptions(),
            (DWORD*)adjacency->GetBufferPointer(),
            NULL, NULL, NULL,
            &optimizedMesh ), false );

        // move to default pool
        GN_DX_CHECK_RETURN( optimizedMesh->CloneMeshFVF(
            D3DXMESH_WRITEONLY,
            optimizedMesh->GetFVF(),
            dev, &mesh ), false );

        // success
        PRIM_COUNT = mesh->GetNumFaces();
        return true;
    }

    void destroy()
    {
        mesh.clear();
    }

    void draw()
    {
        Renderer & r = gRenderer;

        for( size_t i = 0; i < DRAW_COUNT; ++i )
        {
            for( DWORD j = 0; j < numMaterials; ++j )
            {
                GN_DX_CHECK( mesh->DrawSubset( j ) );
            }
        }

        // restore renderer states
        RendererContext::FieldFlags ff;
        ff.u32 = 0;
        ff.vtxfmt = 1;
        ff.vtxbufs = 1;
        ff.idxbuf = 1;
        r.rebindContext( ff );
    }
};

class VerticeThroughput : public BasicTestCase
{
    DenseMesh       mGeometry;
    DiffuseEffect   mEffect;
    RendererContext mContext;

    float    mRadius;
    Vector3f mEuler;
    Vector3f mEye;

    float    mMoveSpeed;
    Vector3f mRollSpeed;

    StrA                mThroughputStr;
    AverageValue<float> mThroughput;

public:

    VerticeThroughput( app::SampleApp & app, const StrA & name )
        : BasicTestCase( app, name )
    {
    }

    bool create()
    {
        GN_GUARD;

        // create geometry and geometry
        if( !mGeometry.create() ) return false;
        if( !mEffect.create() ) return false;

        // initialize context
        mContext.clearToNull();
        mContext.setShaders( mEffect.vs, mEffect.ps, 0 );
        //mContext.setRenderState( RS_CULL_MODE, RSV_CULL_NONE );

        // initialize camera
        const Spheref & s = mGeometry.boundingSphere;
        mRadius = s.radius * 2.0f;
        mEuler.set( 0, 0, 0 );

        // success
        return true;

        GN_UNGUARD;
    }

    void destroy()
    {
        mGeometry.destroy();
        mEffect.destroy();
    }

    void onkey( input::KeyEvent key )
    {
        if( !key.status.down )
        {
            switch( key.code )
            {
                case input::KEY_XB360_RIGHT_SHOULDER :
                case input::KEY_NUMPAD_ADD:
                    mGeometry.DRAW_COUNT += 10;
                    break;

                case input::KEY_XB360_LEFT_SHOULDER:
                case input::KEY_NUMPAD_SUBTRACT:
                    if( mGeometry.DRAW_COUNT >=10 ) mGeometry.DRAW_COUNT -= 10;
                    break;

                default : ; // do nothing
            }
        }
    }

    void onmove( input::Axis, int ) {}

    void update()
    {
        // update moving speed
        using namespace GN::input;
        const int * a = gInput.getAxisStatus();
        GN_ASSERT( a );
        static const float MIN_SPEED = 0.000001f;
        float speed = max( MIN_SPEED, mRadius * 0.000001f );
        mMoveSpeed = speed * a[Axis::XB360_THUMB_LY];
        mRollSpeed.set( speed * a[Axis::XB360_THUMB_RY], speed * a[Axis::XB360_THUMB_RX], 0 );

        // update radius and euler
        mRadius += mMoveSpeed;
        if( mRadius < 0.0f ) mRadius = .0f;
        mEuler += mRollSpeed;

        // update mEye position
        mEye.x = mRadius * cosf( mEuler.pitch ) * cosf( mEuler.yaw + GN_HALF_PI );
        mEye.y = mRadius * sinf( mEuler.pitch );
        mEye.z = mRadius * cosf( mEuler.pitch ) * sinf( mEuler.yaw + GN_HALF_PI );
        mEye += mGeometry.boundingSphere.center;

        // calculate throughput
        size_t triangles = mGeometry.DRAW_COUNT * mGeometry.PRIM_COUNT;
        float throughput = triangles / 1000000.0f * getApp().getFps();
        mThroughput = throughput;
        mThroughputStr.Format(
            "%s\n"
            "\n"
            "throughput = %f Mtri/sec\n"
            "mesh count = %d\n"
            "triangles  = %d\n"
            "\n"
            "radius = %f\n"
            "euler  = %f, %f\n"
            "eye    = %f, %f, %f",
            getName().GetRawPtr(),
            throughput,
            mGeometry.DRAW_COUNT,
            triangles,
            mRadius,
            mEuler.x, mEuler.y,
            mEye.x, mEye.y, mEye.z );
    }

    void render()
    {
        Renderer & r = gRenderer;
        r.clearScreen();

        // update transformation matrix
        Matrix44f proj, view;
        const DispDesc & dd = gRenderer.getDispDesc();
        const Spheref & s = mGeometry.boundingSphere;
        if( mRadius < s.radius * 2.0f )
        {
            proj.perspectiveD3D( 1.0f, (float)dd.width/dd.height, s.radius / 10.0f, s.radius * 10.0f );
        }
        else
        {
            proj.perspectiveD3D( 1.0f, (float)dd.width/dd.height, mRadius - s.radius, mRadius + s.radius );
        }
        static const Vector3f UP(0,1,0);
        view.lookAt( mEye, s.center, UP );
        mEffect.vs->setUniformByNameM( "gPVW", proj * view );

        // draw the mesh
        r.setContext( mContext );
        mGeometry.draw();

        // draw statistics
        scene::gAsciiFont.drawText( mThroughputStr.GetRawPtr(), 0, 100, GN_RGBA32(255,0,0,255) );
    }

    StrA printResult()
    {
        return StringFormat( "throughput(%f)", mThroughput.getAverageValue() );
    }
};
