#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.d3d10.SimpleMesh");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3d10::SimpleMesh::init( ID3D10Device * dev )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::d3d10::SimpleMesh, () );

    if( 0 == dev )
    {
        GN_ERROR(sLogger)( "NULL D3D10 device pointer!" );
        return failure();
    }

    // create a mini-shader (for its signature)
    static const char * vscode =
        "struct VSInput { float4 p : POSITION0; float3 n : NORMAL0; float2 t : TEXCOORD0; float4 c : COLOR0; float4 u : USER0; };\n"
        "float4 main( VSInput i ) : SV_Position0 { return 0; }";
    AutoComPtr<ID3D10VertexShader> vs;
    AutoComPtr<ID3D10Blob>  signature;
    vs.attach( compileVS( *dev, vscode, 0, 0, "main", "vs_4_0", &signature ) );
    if( vs.empty() ) return failure();

    // create input layout
    static const D3D10_INPUT_ELEMENT_DESC elements[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "USER"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
    GN_DX10_CHECK_RV(
        dev->CreateInputLayout( elements, GN_ARRAY_COUNT(elements), signature->GetBufferPointer(), signature->GetBufferSize(), &mLayout ),
        failure() );

    // success
    mDevice = dev;
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::quit()
{
    GN_GUARD;

    safeRelease( mLayout );
    safeRelease( mVtxBuf );
    safeRelease( mIdxBuf );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::beginVertices()
{
    mVertices.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::pos( float x, float y, float z )
{
    mNewVertex.pos.set( x, y, z );
    mVertices.append( mNewVertex );
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::normal( float x, float y, float z )
{
    mNewVertex.normal.set( x, y, z );
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::tex( float x, float y )
{
    mNewVertex.tex.set( x, y );
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::color( float r, float g, float b, float a )
{
    mNewVertex.color.set( r, g, b, a );
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::endVertices()
{
    if( mVertices.empty() ) return;

    UINT bytes = (UINT)( mVertices.size() * sizeof(Vertex) );

    if( mVtxBufCapacity < mVertices.size() )
    {
        safeRelease( mVtxBuf );
        mVtxBufCapacity = 0;

        D3D10_BUFFER_DESC desc = {
            bytes,
            D3D10_USAGE_DEFAULT,
            D3D10_BIND_VERTEX_BUFFER,
            0,
            0,
        };

        GN_DX10_CHECK_R( mDevice->CreateBuffer( &desc, 0, &mVtxBuf ) );

        mVtxBufCapacity = mVertices.size();
    }

    D3D10_BOX box = { 0, 0, 0, bytes, 1, 1 };

    mDevice->UpdateSubresource( mVtxBuf, 0, &box, mVertices.cptr(), bytes, bytes );

    mNumVertices = mVertices.size();

    // release memory allocated by mVertices.
    mVertices.purge();
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::beginTriangles()
{
    mIndices.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::triangle( size_t i0, size_t i1, size_t i2 )
{
    mIndices.append( (UInt16)i0 );
    mIndices.append( (UInt16)i1 );
    mIndices.append( (UInt16)i2 );
}


//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::endTriangles()
{
    if( mIndices.empty() ) return;

    UINT bytes = (UINT)( mIndices.size() * sizeof(UInt16) );

    if( mIdxBufCapacity < mIndices.size() )
    {
        safeRelease( mIdxBuf );
        mIdxBufCapacity = 0;

        D3D10_BUFFER_DESC desc = {
            bytes,
            D3D10_USAGE_DEFAULT,
            D3D10_BIND_INDEX_BUFFER,
            0,
            0,
        };

        GN_DX10_CHECK_R( mDevice->CreateBuffer( &desc, 0, &mIdxBuf ) );

        mIdxBufCapacity = mIndices.size();
    }

    D3D10_BOX box = { 0, 0, 0, bytes, 1, 1 };

    mDevice->UpdateSubresource( mIdxBuf, 0, &box, mIndices.cptr(), bytes, bytes );

    mNumIndices = mIndices.size();

    // release memory allocated by mIndices.
    mIndices.purge();
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::draw() const
{
    if( mNumVertices > 0 )
    {
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        mDevice->IASetInputLayout( mLayout );
        mDevice->IASetVertexBuffers( 0, 1, &mVtxBuf, &stride, &offset );
        mDevice->Draw( (UINT)mNumVertices, 0 );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::SimpleMesh::drawIndexed() const
{
    if( mNumIndices > 0 )
    {
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        mDevice->IASetInputLayout( mLayout );
        mDevice->IASetVertexBuffers( 0, 1, &mVtxBuf, &stride, &offset );
        mDevice->IASetIndexBuffer( mIdxBuf, DXGI_FORMAT_R16_UINT, 0 );
        mDevice->DrawIndexed( (UINT)mNumIndices, 0, 0 );
    }
}
