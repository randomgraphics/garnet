#include "pch.h"

#if GN_MSVC

#if GN_XENON
#if GN_DEBUG_BUILD
#pragma comment(lib, "d3d9d.lib")
#pragma comment( lib, "xgraphicsd.lib" )
#pragma comment(lib, "xapilibd.lib")
#elif GN_PROFILE_BUILD
#pragma comment(lib, "d3d9i.lib")
#pragma comment( lib, "xgraphics.lib" )
#pragma comment(lib, "xapilibi.lib")
#else
#pragma comment(lib, "d3d9.lib")
#pragma comment( lib, "xgraphics.lib" )
#pragma comment(lib, "xapilib.lib")
#endif
#else
#if GN_DEBUG_BUILD
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif
#endif

#endif

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

// *****************************************************************************
// d3dArcBall from dxsdk
// *****************************************************************************

class CD3DArcBall
{
public:
    CD3DArcBall();

    // Functions to change behavior
    void Reset(); 
    void SetTranslationRadius( FLOAT fRadiusTranslation ) { m_fRadiusTranslation = fRadiusTranslation; }
    void SetWindow( INT nWidth, INT nHeight, FLOAT fRadius = 0.9f ) { m_nWidth = nWidth; m_nHeight = nHeight; m_fRadius = fRadius; m_vCenter = D3DXVECTOR2(m_nWidth/2.0f,m_nHeight/2.0f); }
    void SetOffset( INT nX, INT nY ) { m_Offset.x = nX; m_Offset.y = nY; }

    // Call these from client and use GetRotationMatrix() to read new rotation matrix
    void OnBegin( int nX, int nY );  // start the rotation (pass current mouse position)
    void OnMove( int nX, int nY );   // continue the rotation (pass current mouse position)
    void OnEnd();                    // end the rotation 

     // Functions to get/set state
    const D3DXMATRIX* GetRotationMatrix()                   { return D3DXMatrixRotationQuaternion(&m_mRotation, &m_qNow); };
    const D3DXMATRIX* GetTranslationMatrix() const          { return &m_mTranslation; }
    const D3DXMATRIX* GetTranslationDeltaMatrix() const     { return &m_mTranslationDelta; }
    bool        IsBeingDragged() const                      { return m_bDrag; }
    D3DXQUATERNION GetQuatNow() const                       { return m_qNow; }
    void        SetQuatNow( D3DXQUATERNION q ) { m_qNow = q; }

    static D3DXQUATERNION QuatFromBallPoints( const D3DXVECTOR3 &vFrom, const D3DXVECTOR3 &vTo );


protected:
    D3DXMATRIXA16  m_mRotation;         // Matrix for arc ball's orientation
    D3DXMATRIXA16  m_mTranslation;      // Matrix for arc ball's position
    D3DXMATRIXA16  m_mTranslationDelta; // Matrix for arc ball's position

    POINT          m_Offset;   // window offset, or upper-left corner of window
    INT            m_nWidth;   // arc ball's window width
    INT            m_nHeight;  // arc ball's window height
    D3DXVECTOR2    m_vCenter;  // center of arc ball 
    FLOAT          m_fRadius;  // arc ball's radius in screen coords
    FLOAT          m_fRadiusTranslation; // arc ball's radius for translating the target

    D3DXQUATERNION m_qDown;             // Quaternion before button down
    D3DXQUATERNION m_qNow;              // Composite quaternion for current drag
    bool           m_bDrag;             // Whether user is dragging arc ball

    POINT          m_ptLastMouse;      // position of last mouse point
    D3DXVECTOR3    m_vDownPt;           // starting point of rotation arc
    D3DXVECTOR3    m_vCurrentPt;        // current point of rotation arc

    D3DXVECTOR3    ScreenToVector( float fScreenPtX, float fScreenPtY );
};

//--------------------------------------------------------------------------------------
CD3DArcBall::CD3DArcBall()
{
    Reset();
    m_vDownPt = D3DXVECTOR3(0,0,0);
    m_vCurrentPt = D3DXVECTOR3(0,0,0);
    m_Offset.x = m_Offset.y = 0;

    RECT rc;
    GetClientRect( GetForegroundWindow(), &rc );
    SetWindow( rc.right, rc.bottom );
}





//--------------------------------------------------------------------------------------
void CD3DArcBall::Reset()
{
    D3DXQuaternionIdentity( &m_qDown );
    D3DXQuaternionIdentity( &m_qNow );
    D3DXMatrixIdentity( &m_mRotation );
    D3DXMatrixIdentity( &m_mTranslation );
    D3DXMatrixIdentity( &m_mTranslationDelta );
    m_bDrag = FALSE;
    m_fRadiusTranslation = 1.0f;
    m_fRadius = 1.0f;
}




//--------------------------------------------------------------------------------------
D3DXVECTOR3 CD3DArcBall::ScreenToVector( float fScreenPtX, float fScreenPtY )
{
    // Scale to screen
    FLOAT x   = -(fScreenPtX - m_Offset.x - m_nWidth/2)  / (m_fRadius*m_nWidth/2);
    FLOAT y   =  (fScreenPtY - m_Offset.y - m_nHeight/2) / (m_fRadius*m_nHeight/2);

    FLOAT z   = 0.0f;
    FLOAT mag = x*x + y*y;

    if( mag > 1.0f )
    {
        FLOAT scale = 1.0f/sqrtf(mag);
        x *= scale;
        y *= scale;
    }
    else
        z = sqrtf( 1.0f - mag );

    // Return vector
    return D3DXVECTOR3( x, y, z );
}




//--------------------------------------------------------------------------------------
D3DXQUATERNION CD3DArcBall::QuatFromBallPoints(const D3DXVECTOR3 &vFrom, const D3DXVECTOR3 &vTo)
{
    D3DXVECTOR3 vPart;
    float fDot = D3DXVec3Dot(&vFrom, &vTo);
    D3DXVec3Cross(&vPart, &vFrom, &vTo);

    return D3DXQUATERNION(vPart.x, vPart.y, vPart.z, fDot);
}




//--------------------------------------------------------------------------------------
void CD3DArcBall::OnBegin( int nX, int nY )
{
    // Only enter the drag state if the click falls
    // inside the click rectangle.
    if( nX >= m_Offset.x &&
        nX < m_Offset.x + m_nWidth &&
        nY >= m_Offset.y &&
        nY < m_Offset.y + m_nHeight )
    {
        m_bDrag = true;
        m_qDown = m_qNow;
        m_vDownPt = ScreenToVector( (float)nX, (float)nY );
    }
}




//--------------------------------------------------------------------------------------
void CD3DArcBall::OnMove( int nX, int nY )
{
    if (m_bDrag) 
    { 
        m_vCurrentPt = ScreenToVector( (float)nX, (float)nY );
        m_qNow = m_qDown * QuatFromBallPoints( m_vDownPt, m_vCurrentPt );
    }
}




//--------------------------------------------------------------------------------------
void CD3DArcBall::OnEnd()
{
    m_bDrag = false;
}

// *****************************************************************************
// main application class
// *****************************************************************************

class MyApp : public SampleApp
{
    struct Vertex
    {
        float p[3];
        float n[3];
        float t[2];
    };

    LPDIRECT3DDEVICE9 dev;

    AutoComPtr<IDirect3DVertexDeclaration9> decl;
    AutoComPtr<IDirect3DVertexShader9> vs;
    AutoComPtr<IDirect3DPixelShader9> ps;

    Vertex vb[24];
    UInt16 ib[36];

    Matrix44f world, view, proj;

    CD3DArcBall d3dArcBall;
    util::ArcBall arcball;

public:
    bool onAppInit()
    {
        static const float E = 160.0f;
        createBox(
            E, E, E,
            vb[0].p, sizeof(Vertex),
            vb[0].t, sizeof(Vertex),
            vb[0].n, sizeof(Vertex),
            ib, 0 );

        world.identity();
#if 1
        view.lookAtRh( Vector3f(0,0,-300), Vector3f(0,0,0), Vector3f(0,1,0) );
        proj.perspectiveD3DRh( 1.0f, 4.0f/3.0f, 10.0f, 1000.0f );
#else
        view.lookAtLh( Vector3f(0,0,-300), Vector3f(0,0,0), Vector3f(0,1,0) );
        proj.perspectiveD3DLh( 1.0f, 4.0f/3.0f, 10.0f, 1000.0f );
#endif

        return true;
    }

    void onAppQuit()
    {
    }

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
        ip.ro.vsync = true;
    }

    bool onRendererRestore()
    {
        dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();

        D3DVERTEXELEMENT9 format[] = 
        {
            { 0,  0, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_NORMAL, 0 },
            { 0, 24, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };
        GN_DX9_CHECK_RV( dev->CreateVertexDeclaration( format, &decl ), false );

        static const char * vs_code =
            "uniform float4x4 gPvw : register(c0);    \n"
            "struct vsi                               \n"
            "{                                        \n"
            "    float4 pos : POSITION;               \n"
            "    float3 nml : NORMAL;                 \n"
            "    float2 uv  : TEXCOORD0;              \n"
            "};                                       \n"
            "struct vso                               \n"
            "{                                        \n"
            "    float4 pos : POSITION;               \n"
            "    float4 clr : COLOR0;                 \n"
            "    float2 uv  : TEXCOORD0;              \n"
            "};                                       \n"
            "vso main( vsi i )                        \n"
            "{                                        \n"
            "    vso o;                               \n"
            "    o.pos = mul( gPvw, i.pos );          \n"
            "    float3 n = 2*abs(i.nml) + i.nml;     \n"
            "    o.clr = float4( n/3.0, 1.0 );        \n"
            "    o.uv = i.uv;                         \n"
            "    return o;                            \n"
            "}";
        vs.attach( GN::gfx::d3d9::compileVS( dev, vs_code, 0, D3DXSHADER_PACKMATRIX_ROWMAJOR ) );
        if( !vs ) return false;

        static const char * ps_code =
            "float4 main(                             \n"
            "    float4 clr : COLOR0,                 \n"
            "    float2 uv  : TEXCOORD0 )             \n"
            "    : COLOR0                             \n"
            "{                                        \n"
            "    return clr;                          \n"
            "}";
        ps.attach( GN::gfx::d3d9::compilePS( dev, ps_code ) );
        if( !ps ) return false;

        // initial arcball window
        const DispDesc & dd = gRenderer.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );
        d3dArcBall.SetWindow( (int)dd.width, (int)dd.height );

        // success
        return true;
    }

    void onRendererDispose()
    {
        dev = 0;
        decl.clear();
        vs.clear();
        ps.clear();
    }

    void onKeyPress( input::KeyEvent key )
    {
        GN::app::SampleApp::onKeyPress( key );
        if( input::KEY_MOUSEBTN_0 == key.code )
        {
            if( key.status.down )
            {
                int x, y;
                gInput.getMousePosition( x, y );
                d3dArcBall.OnBegin( x, y );
                arcball.onMouseButtonDown( x, y );
            }
            else
            {
                d3dArcBall.OnEnd();
                arcball.onMouseButtonUp();
            }
        }
    }

    void onAxisMove( input::Axis, int )
    {
        int x, y;
        gInput.getMousePosition( x, y );
        d3dArcBall.OnMove( x, y );
        arcball.onMouseMove( x, y );
    }

    void onUpdate()
    {
#if 1
        world.set( arcball.getRotationMatrix() );
#else
        world = *(const Matrix44f*)d3dArcBall.GetRotationMatrix();
        world.transpose();
#endif
        Matrix44f pvw = proj * view * world;
        dev->SetVertexShaderConstantF( 0, (const float*)&pvw, 4 );
    }

    void onRender()
    {
        dev->Clear( 0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );

        // set render state
        dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

        // draw box
        dev->SetVertexShader( vs );
        dev->SetPixelShader( ps );
        dev->SetVertexDeclaration( decl );
        dev->DrawIndexedPrimitiveUP(
            D3DPT_TRIANGLELIST,
            0,
            24,
            12,
            ib,
            D3DFMT_INDEX16,
            vb,
            sizeof(Vertex) );

        // restore renderer states.
        RendererContext::FieldFlags ff;
        ff.u32 = 0;
        ff.setShaderBit( SHADER_VS );
        ff.setShaderBit( SHADER_PS );
        ff.vtxFmt = 1;
        ff.rsb = 1;
        gRenderer.rebindContext( ff );
    }
};
int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
