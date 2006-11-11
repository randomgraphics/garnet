#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/vertexFormat.inl"
#endif

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
//
// *****************************************************************************

static Logger * sLogger = getLogger("GN.gfx.base.VertexFormatDesc");

static VtxFmtDesc sBuildXyzNormUv()
{
    VtxFmtDesc vfd;
    vfd.count = 3;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vfd.attribs[1].set( 0, 12, VTXSEM_NML0, FMT_FLOAT3 );
    vfd.attribs[2].set( 0, 24, VTXSEM_TEX0, FMT_FLOAT2 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 32 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

static VtxFmtDesc sBuildXyzUv()
{
    VtxFmtDesc vfd;
    vfd.count = 2;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vfd.attribs[1].set( 0, 12, VTXSEM_TEX0, FMT_FLOAT2 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 20 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

static VtxFmtDesc sBuildXyz()
{
    VtxFmtDesc vfd;
    vfd.count = 1;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 12 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

static VtxFmtDesc sBuildXyzwUv()
{
    VtxFmtDesc vfd;
    vfd.count = 2;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT4 );
    vfd.attribs[1].set( 0, 16, VTXSEM_TEX0, FMT_FLOAT2 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 24 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

static VtxFmtDesc sBuildXyzw()
{
    VtxFmtDesc vfd;
    vfd.count = 1;
    vfd.attribs[0].set( 0, 0, VTXSEM_POS0, FMT_FLOAT4 );
    //vfd.streams.resize( 1 );
    //vfd.streams[0].set( 16 );
    GN_ASSERT( vfd.validate() );
    return vfd;
}

// *****************************************************************************
//
// *****************************************************************************

const VtxFmtDesc VtxFmtDesc::XYZ_NORM_UV = sBuildXyzNormUv();
const VtxFmtDesc VtxFmtDesc::XYZ_UV = sBuildXyzUv();
const VtxFmtDesc VtxFmtDesc::XYZ = sBuildXyz();
const VtxFmtDesc VtxFmtDesc::XYZW_UV = sBuildXyzwUv();
const VtxFmtDesc VtxFmtDesc::XYZW = sBuildXyzw();

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::VtxFmtDesc::validate() const
{
    if( 0 == count || count > MAX_VERTEX_ATTRIBUTES )
    {
        GN_ERROR(sLogger)( "numAttributes must be in range (0,MAX_VERTEX_ATTRIBUTES]." );
        return false;
    }
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::VtxFmtDesc::addAttrib(
    size_t stream,
    size_t offset,
    VtxSem semantic,
    ClrFmt format )
{
    if( count == MAX_VERTEX_ATTRIBUTES )
    {
        GN_ERROR(sLogger)( "Can't have more attributes." );
        return false;
    }
    // TODO: check input parameters
    attribs[count].set( stream, offset, semantic, format );
    ++count;

    GN_ASSERT( validate() );
    return true;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::VtxFmtDesc::AttribDesc *
GN::gfx::VtxFmtDesc::findAttrib( VtxSem semantic ) const
{
    for( size_t i = 0; i < count; ++i )
    {
        if( attribs[i].semantic == semantic ) return &attribs[i];
    }
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::VtxFmtDesc::calcNumStreams()
{
    if( !validate() )
    {
        GN_ERROR(sLogger)( "invalid vertex format!" );
        return 0;
    }
    size_t n = 0;
    for( size_t i = 0; i < count; ++i )
    {
        const AttribDesc & a =  attribs[i];
        if( a.stream >= n ) n = a.stream + 1;
    }
    return n;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::VtxFmtDesc::calcStreamStride( size_t idx )
{
    if( !validate() )
    {
        GN_ERROR(sLogger)( "invalid vertex format!" );
        return 0;
    }
    if( idx >= calcNumStreams() )
    {
        GN_ERROR(sLogger)( "invalid stream index: %d!", idx );
        return 0;
    }
    size_t s = 0;
    for( size_t i = 0; i < count; ++i )
    {
        const AttribDesc & a =  attribs[i];
        if( a.stream == idx ) s += getClrFmtDesc(a.format).bits / 8;
    }
    return s;
}
