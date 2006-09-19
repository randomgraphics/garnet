#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/vertexFormat.inl"
#endif

using namespace GN;
using namespace GN::gfx;

Logger * VtxFmtDesc::sLogger = getLogger("GN.gfx.base.VertexFormatDesc");

static VtxFmtDesc sBuildXyzNormUv()
{
    VtxFmtDesc vfd;
    vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT3 );
    vfd.addAttrib( 0, 12, VTXSEM_NORMAL, FMT_FLOAT3 );
    vfd.addAttrib( 0, 24, VTXSEM_TEX0, FMT_FLOAT2 );
    return vfd;
}

static VtxFmtDesc sBuildXyzUv()
{
    VtxFmtDesc vfd;
    vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT3 );
    vfd.addAttrib( 0, 12, VTXSEM_TEX0, FMT_FLOAT2 );
    return vfd;
}

static VtxFmtDesc sBuildXyz()
{
    VtxFmtDesc vfd;
    vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT3 );
    return vfd;
}

static VtxFmtDesc sBuildXyzwUv()
{
    VtxFmtDesc vfd;
    vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT4 );
    vfd.addAttrib( 0, 16, VTXSEM_TEX0, FMT_FLOAT2 );
    return vfd;
}

static VtxFmtDesc sBuildXyzw()
{
    VtxFmtDesc vfd;
    vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT4 );
    return vfd;
}

const VtxFmtDesc VtxFmtDesc::XYZ_NORM_UV = sBuildXyzNormUv();
const VtxFmtDesc VtxFmtDesc::XYZ_UV = sBuildXyzUv();
const VtxFmtDesc VtxFmtDesc::XYZ = sBuildXyz();
const VtxFmtDesc VtxFmtDesc::XYZW_UV = sBuildXyzwUv();
const VtxFmtDesc VtxFmtDesc::XYZW = sBuildXyzw();
