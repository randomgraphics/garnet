#include "pch.h"

// *****************************************************************************
// utilities
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.d3d9.thickline");

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

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::d3d9::D3D9ThickLineRenderer::D3D9ThickLineRenderer()
{
    clear();

    // initialize index buffer
    uint16 numpoly = MAX_VERTICES / 6;
    for( uint16 i = 0; i < numpoly; ++i )
    {
        uint16 * p = m_Indices + i * 12;

        uint16 v = i * 6;

        // v1-v2
        // |   \
        // v0   \
        //  \    \
        //   \    \
        //    \    \
        //     \    v3
        //      \   |
        //       v5-v4

        *p++ = v + 0;
        *p++ = v + 1;
        *p++ = v + 2;

        *p++ = v + 0;
        *p++ = v + 2;
        *p++ = v + 3;

        *p++ = v + 0;
        *p++ = v + 3;
        *p++ = v + 5;

        *p++ = v + 3;
        *p++ = v + 4;
        *p++ = v + 5;
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d9::D3D9ThickLineRenderer::onDeviceCreate( IDirect3DDevice9 * dev )
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

    // Create vertex decl
    D3DVERTEXELEMENT9 decl[] = {
        { 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
        { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    if( FAILED( dev->CreateVertexDeclaration( decl, &m_Decl ) ) )
    {
        LOG_ERROR( "Fail to create D3D decl." );
        return false;
    }

    // initialize D3D index buffer
    if( FAILED( dev->CreateIndexBuffer( sizeof(m_Indices), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_Ib, NULL ) ) )
    {
        LOG_ERROR( "Fail to create D3D index buffer." );
        return false;
    }
    void * indices;
    if( FAILED( m_Ib->Lock( 0, 0, &indices, 0 ) ) )
    {
        LOG_ERROR( "Fail to lock D3D index buffer." );
        return false;
    }
    memcpy( indices, m_Indices, sizeof(m_Indices) );
    m_Ib->Unlock();

    m_Device = dev;
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d9::D3D9ThickLineRenderer::onDeviceRestore()
{
    if( NULL == m_Device )
    {
        LOG_ERROR( "Null device pointer!" );
        return false;
    }

    if( FAILED( m_Device->CreateVertexBuffer( sizeof(m_Vertices), D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_Vb, NULL ) ) )
    {
        LOG_ERROR( "Fail to create D3D vertex buffer." );
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::onDeviceDispose()
{
    SAFE_RELEASE( m_Vb );
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::onDeviceDelete()
{
    SAFE_RELEASE( m_Vs );
    SAFE_RELEASE( m_Ps );
    SAFE_RELEASE( m_Decl );
    SAFE_RELEASE( m_Ib );
    clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d9::D3D9ThickLineRenderer::drawBegin( const ThickLineParameters & parameters )
{
    if( m_Drawing )
    {
        LOG_ERROR( "Unpaired drawBegin()" );
        return false;
    }

    // setup line parameters
    (ThickLineParameters&)m_Parameters = parameters;
    D3DVIEWPORT9 vp;
    m_Device->GetViewport( &vp );
    m_Parameters.wvp = parameters.worldview * parameters.proj;
    m_Parameters.screenWidth = (float)vp.Width;
    m_Parameters.screenHeight = (float)vp.Height;
    m_Parameters.endPointHalfWidth = (float)m_Parameters.width / vp.Width;
    m_Parameters.endPointHalfHeight = (float)m_Parameters.width / vp.Height;

    m_Drawing = true;
    m_NumVertices = 0;
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::drawEnd()
{
    if( !m_Drawing )
    {
        LOG_ERROR( "Unpaired drawEnd()" );
        return;
    }

    flush();
    m_Drawing = false;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::line( const ThickLineVertex & v0, const ThickLineVertex & v1 )
{
    if( !m_Drawing )
    {
        LOG_ERROR( "D3D9ThickLineRenderer::line() must be called in between D3D9ThickLineRenderer::drawBegin() and D3D9ThickLineRenderer::drawEnd()." );
        return;
    }

    EndPoint e0, e1;
    calcEndPoint( e0, v0 );
    calcEndPoint( e1, v1 );

    PrivateVertex * v = newPolygon6();

    if( e0.post * e1.posw < e1.post * e0.posw )
    {
        if( e0.posl * e1.posw < e1.posl * e0.posw )
        {
            // e1 in quadrant 1

            //     ----
            //    / e1|
            //   /    /
            //  /    /
            // /    /
            // |e0 /
            // ----

            e0.bottomRight( v++ );
            e0.bottomLeft( v++ );
            e0.topLeft( v++ );
            e1.topLeft( v++ );
            e1.topRight( v++ );
            e1.bottomRight( v++ );
        }
        else
        {
            // e1 in quadrant 2

            // ----
            // |e1 \
            // \    \
            //  \    \
            //   \    \
            //    \ e0|
            //     ----

            e0.topRight( v++ );
            e0.bottomRight( v++ );
            e0.bottomLeft( v++ );
            e1.bottomLeft( v++ );
            e1.topLeft( v++ );
            e1.topRight( v++ );
        }
    }
    else
    {
        if( e0.posl * e1.posw < e1.posl * e0.posw )
        {
            // e1 in quadrant 4

            // ----
            // |e0 \
            // \    \
            //  \    \
            //   \    \
            //    \ e1|
            //     ----

            e0.bottomLeft( v++ );
            e0.topLeft( v++ );
            e0.topRight( v++ );
            e1.topRight( v++ );
            e1.bottomRight( v++ );
            e1.bottomLeft( v++ );
        }
        else
        {
            // e1 in quadrant 3

            //     ----
            //    / e0|
            //   /    /
            //  /    /
            // /    /
            // |e1 /
            // ----

            e0.topLeft( v++ );
            e0.topRight( v++ );
            e0.bottomRight( v++ );
            e1.bottomRight( v++ );
            e1.bottomLeft( v++ );
            e1.topLeft( v++ );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::line( float x1, float y1, float z1, float x2, float y2, float z2, D3DCOLOR color )
{
    ThickLineVertex v[2] =
    {
        { x1, y1, z1, 0, 0, color },
        { x2, y2, z2, 1, 1, color },
    };
    lineList( v, 2 );
}


//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::lineList( const ThickLineVertex * vertices, size_t numverts )
{
    size_t numLines = numverts / 2;
    for( size_t i = 0; i < numLines; ++i )
    {
        line( vertices[i*2+0], vertices[i*2+1] );
    }
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::d3d9::D3D9ThickLineRenderer::PrivateVertex *
GN::d3d9::D3D9ThickLineRenderer::EndPoint::topLeft( PrivateVertex * v )
{
    v->position = XMVectorSet( posl, post, posz, posw );
    v->u = texl;
    v->v = text;
    v->color = color;
    return v;
}

//
//
// -----------------------------------------------------------------------------
GN::d3d9::D3D9ThickLineRenderer::PrivateVertex *
GN::d3d9::D3D9ThickLineRenderer::EndPoint::topRight( PrivateVertex * v )
{
    v->position = XMVectorSet( posr, post, posz, posw );
    v->u = texr;
    v->v = text;
    v->color = color;
    return v;
}

//
//
// -----------------------------------------------------------------------------
GN::d3d9::D3D9ThickLineRenderer::PrivateVertex *
GN::d3d9::D3D9ThickLineRenderer::EndPoint::bottomLeft( PrivateVertex * v )
{
    v->position = XMVectorSet( posl, posb, posz, posw );
    v->u = texl;
    v->v = texb;
    v->color = color;
    return v;
}

//
//
// -----------------------------------------------------------------------------
GN::d3d9::D3D9ThickLineRenderer::PrivateVertex *
GN::d3d9::D3D9ThickLineRenderer::EndPoint::bottomRight( PrivateVertex * v )
{
    v->position = XMVectorSet( posr, posb, posz, posw );
    v->u = texr;
    v->v = texb;
    v->color = color;
    return v;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::clear()
{
    m_Device = NULL;
    m_Vs = 0;
    m_Ps = 0;
    m_Decl = 0;
    m_Vb = 0;
    m_Ib = 0;
    m_Drawing = false;
}

//
// Expand thick line vertex to a quad
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::calcEndPoint(
    EndPoint              & endpoint,
    const ThickLineVertex & vertex )
{
    // determine center position and end point size
    XMVECTOR center;
    float half_w;
    float half_h;
    if( m_Parameters.widthInScreenSpace )
    {
        // translate vertex into clip space
        center = XMVectorSet( vertex.x, vertex.y, vertex.z, 1.0f );
        center = XMVector4Transform( center, m_Parameters.wvp );

        // get end point positions in clip space
        float w = XMVectorGetW( center );
        half_w = m_Parameters.endPointHalfWidth * w;
        half_h = m_Parameters.endPointHalfHeight * w;
    }
    else
    {
        // translate vertex into view space
        center = XMVectorSet( vertex.x, vertex.y, vertex.z, 1.0f );
        center = XMVector4Transform( center, m_Parameters.worldview );

        // get position of left-top corner in view space
        float half_size = m_Parameters.width / 2.0f * XMVectorGetW( center );
        XMVECTOR topleft = center + XMVectorSet( -half_size, half_size, 0.0f, 0.0f );

        // translate both center and left top into clip space
        center = XMVector4Transform( center, m_Parameters.proj );
        topleft = XMVector4Transform( topleft, m_Parameters.proj );

        half_w = fabs( XMVectorGetX( topleft ) - XMVectorGetX( center ) );
        half_h = fabs( XMVectorGetY( topleft ) - XMVectorGetY( center ) );

        float w = XMVectorGetW( center );
        float min_half_w = 1.0f / m_Parameters.screenWidth * w;
        float min_half_h = 1.0f / m_Parameters.screenHeight * w;
        if( half_w < min_half_w ) half_w = min_half_w;
        if( half_h < min_half_h ) half_h = min_half_h;
    }

    endpoint.posl = XMVectorGetX( center ) - half_w;
    endpoint.posr = XMVectorGetX( center ) + half_w;
    endpoint.post = XMVectorGetY( center ) + half_h;
    endpoint.posb = XMVectorGetY( center ) - half_h;
    endpoint.posz = XMVectorGetZ( center );
    endpoint.posw = XMVectorGetW( center );

    // TODO: interpolate end point texcoord for each corner
    endpoint.texl = vertex.u;
    endpoint.texr = vertex.u;
    endpoint.text = vertex.v;
    endpoint.texb = vertex.v;

    // TODO: interpolate end point color for each corner
    endpoint.color = vertex.color;
}

//
//
// -----------------------------------------------------------------------------
GN::d3d9::D3D9ThickLineRenderer::PrivateVertex *
GN::d3d9::D3D9ThickLineRenderer::newPolygon6()
{
    if( m_NumVertices + 6 > MAX_VERTICES )
    {
        flush();
    }

    PrivateVertex * nextVertex = &m_Vertices[m_NumVertices];
    m_NumVertices += 6;

    return nextVertex;
}

//
// push pending vertices into GPU pipeline
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9ThickLineRenderer::flush()
{
    if( m_NumVertices > 0 )
    {
        // update vertex buffer
        UINT vbsize = (UINT)( sizeof(PrivateVertex) * m_NumVertices );
        void * vertices;
        if( FAILED( m_Vb->Lock( 0, vbsize, &vertices, D3DLOCK_DISCARD ) ) )
        {
            LOG_ERROR( "Fail to lock D3D vertex buffer." );
            m_NumVertices = 0;
            return;
        }
        memcpy( vertices, m_Vertices, vbsize );
        m_Vb->Unlock();

        // do rendering
        m_Device->SetVertexDeclaration( m_Decl );
        m_Device->SetStreamSource( 0, m_Vb, 0, sizeof(PrivateVertex) );
        m_Device->SetIndices( m_Ib );
        m_Device->SetVertexShader( m_Vs );
        m_Device->SetPixelShader( m_Ps );
        m_Device->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST,
            0, // base vertex index
            0, // min index
            m_NumVertices, // num vertices
            0, // start index
            m_NumVertices / 6 * 4 ); // num primitives
    }

    m_NumVertices = 0;
}
