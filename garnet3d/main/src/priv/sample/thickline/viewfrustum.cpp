#include "pch.h"
#include "viewfrustum.h"

// *****************************************************************************
// utilities
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.gfx.thickline");

//
//
// -----------------------------------------------------------------------------
static void LOG_ERROR(const char * format, ...)
{
    GN::StrA s;
    va_list arglist;
    va_start( arglist, format );
    s.formatv( format, arglist );
    va_end( arglist );
    GN_ERROR(sLogger)( "%s\n", s.cptr() );
}

#define SAFE_RELEASE( x ) if(x) { (x)->Release(); (x) = NULL; } else void(0)

// *****************************************************************************
// local codes
// *****************************************************************************

static const char * vscode =
"struct VSIO { \n"
"   float4 pos : POSITION0; \n"
"   float2 tex : TEXCOORD0; \n"
"   float4 clr : COLOR0;    \n"
"}; \n"
"VSIO main( VSIO i ) { \n"
"   VSIO o = i; \n"
"   return o; \n"
"}";

static const char * pscode =
"struct VSIO { \n"
"   float4 pos : POSITION0; \n"
"   float2 tex : TEXCOORD0; \n"
"   float4 clr : COLOR0;    \n"
"}; \n"
"float4 main( VSIO i ) : COLOR0 { \n"
"   return i.clr; \n"
"}";

static const D3DCOLOR SIDE_COLORS[4] =
{
    //A R G B
    0xFFFF0000; // left   : red
    0xFFFFFF00; // bottom : yellow
    0xFF00FF00; // right  : green
    0xFF0000FF; // top    : blue
};

static const D3DCOLOR FAR_END_COLOR = 0xFFFFFFFF; // white

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
D3D9ViewFrustum::D3D9ViewFrustum()
    : m_Device(0)
    , m_Vs(0)
    , m_Ps(0)
    , m_Vb(0)
{
    memset( &m_Sides, 0, sizeof(m_Sides) );
    memset( &m_FarEnd, 0, sizeof(m_FarEnd) );
}

//
//
// -----------------------------------------------------------------------------
bool D3D9ViewFrustum::OnDeviceCreate( IDirect3DDevice9 * dev )
{
    if( NULL == dev )
    {
        LOG_ERROR( "Null device pointer!" );
        return false;
    }

    // initialize shaders
    m_Vs = GN::d3d9::compileAndCreateVS( dev, vscode );
    m_Ps = GN::d3d9::compileAndCreatePS( dev, pscode );
    if( NULL == m_Vs || NULL == m_Ps ) return false;

    m_Device = dev;
    return m_LineRenderer.OnDeviceCreate( dev );
}

//
//
// -----------------------------------------------------------------------------
bool D3D9ViewFrustum::OnDeviceRestore()
{
    if( NULL == m_Device )
    {
        LOG_ERROR( "Null device pointer!" );
        return false;
    }

    // create vertex buffer
    if( FAILED( dev->CreateVertexBuffer( VIEW_FRUSTUM_VB_SIZE, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_VbSolid, NULL ) ) )
    {
        LOG_ERROR( "Fail to create vertex buffer." );
        return false;
    }

    return m_LineRenderer.OnDeviceRestore();
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::OnDeviceDispose()
{
    m_LineRenderer.OnDeviceDispose();
    SAFE_RELEASE( m_Vb );
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::OnDeviceDelete()
{
    m_LineRenderer.OnDeviceDelete();
    SAFE_RELEASE( m_Vs );
    SAFE_RELEASE( m_Ps );
    m_Device = 0;
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::UpdateViewFrustumRH(
    const XMVECTOR & eye,
    const XMVECTOR & at,
    const XMVECTOR & upVector,
    float            fovy,
    float            ratio,
    float            near,
    float            far )
{
    XMVECTOR forward = XMVector3Normalize( at - eye );
    XMVECTOR right   = XNVector3Normalize( XMVector3Cross( forward, up ) );
    XMVECTOR up      = XNVector3Normalize( XMVector3Cross( right, forward ) );

    float    farend_H  = tan( fovy / 2.0f ) * far;
    float    farend_W  = farend_H * ratio;
    XMVECTOR farend_C  = eye + ( forward * far );
    XMVECTOR farend_R  = right * farend_W;
    XMVECTOR farend_U  = up * farend_H;
    XMVECTOR farend_TL = farend_C - farend_R + farend_U;
    XMVECTOR farend_TR = farend_C + farend_R + farend_U;
    XMVECTOR farend_BL = farend_C - farend_R - farend_U;
    XMVECTOR farend_BR = farend_C + farend_R - farend_U;

    m_Sides[0].Set( eye, farend_TL, farend_BL ); // left
    m_Sides[1].Set( eye, farend_BL, farend_BR ); // bottom
    m_Sides[2].Set( eye, farend_BR, farend_TR ); // right
    m_Sides[3].Set( eye, farend_TR, farend_TL ); // top
    m_FarEnd.Set( eye, farend_TL, farend_TR, farend_BR, farend_BL );

    // rebuild vertex buffer
    Vertex * v = m_Vertices;
    BuildTriangleFaceVertices( v, m_Sides[0] );
    BuildQuadFaceVertices( v, m_FarEnd );
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::DrawRH( const XMMATRIX & viewRH, const XMMATRIX & projRH )
{
    if( NULL == m_Device || NULL == m_Vb ) return;

    /*ViewFrustumVertex * solidVertices;
    if( FAILED( m_VbSolid->Lock( 0, 0, (void**)&solidVertices, D3DLOCK_DISCARD ) ) )
    {
        return;
    }

    ViewFrustumVertex * transVertices;
    if( FAILED( m_VbTrans->Lock( 0, 0, (void**)&transVertices, D3DLOCK_DISCARD ) ) )
    {
        m_VbSolid->Unlock();
        return;
    }*/

    XMVECTOR forward = -viewRH.r[2];
    for( size_t i = 0; i < ARRAYSIZE(m_Sides); ++i )
    {
        D3DCOLOR c = SIDE_COLORS[i];
        if( XMVector3Dot( m_Sides[i].outterNormal, forward ) > 0 )
        {
            // show transparent color
        }
        else
        {
        }
    }
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::TriangleFace::Set(
    const XMVECTOR & v0, const XMVECTOR & v1, const XMVECTOR & v2 )
{
    outterNormal = XNVector3Normalize( XMVector3Cross( (v0 - v1), (v2 - v1) ) );
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::QuadFace::Set(
    const XMVECTOR & v0, const XMVECTOR & v1, const XMVECTOR & v2, const XMVECTOR & v3 )
{
    outterNormal = XNVector3Normalize( XMVector3Cross( (v0 - v1), (v2 - v1) ) );
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;
}
