#include "pch.h"

//
//
// -----------------------------------------------------------------------------
static float sGetNextValue( const float * & buffer, size_t stride )
{
    if( NULL == buffer ) return 0.0f;

    float f = *buffer;

    buffer = (const float*)((const UInt8*)buffer + stride);

    return f;
}

//
//
// -----------------------------------------------------------------------------
void GN::CalculateBoundingSphere(
    Spheref & result,
    const float * x, size_t strideX,
    const float * y, size_t strideY,
    const float * z, size_t strideZ,
    size_t count )
{
    Boxf box;

    CalculateBoundingBox( box, x, strideX, y, strideY, z, strideZ, count );

    result.center = box.Center();
    result.radius = Vector3f::sDistance( result.center, box.Pos() );
}

//
//
// -----------------------------------------------------------------------------
void GN::CalculateBoundingSphere( Spheref & result, const Vector3f * positions, size_t strideInBytes, size_t count )
{
    return CalculateBoundingSphere(
        result,
        positions ? &positions[0].x : NULL, strideInBytes,
        positions ? &positions[0].y : NULL, strideInBytes,
        positions ? &positions[0].z : NULL, strideInBytes,
        count );
}

//
//
// -----------------------------------------------------------------------------
void GN::CalculateBoundingSphereFromBoundingBox( Spheref & result, const Boxf & bbox )
{
    result.center = bbox.Center();
    result.radius = Vector3f::sDistance( result.center, bbox.Pos() );
}

//
//
// -----------------------------------------------------------------------------
void GN::CalculateBoundingBox(
    Boxf & result,
    const float * valueX, size_t strideX,
    const float * valueY, size_t strideY,
    const float * valueZ, size_t strideZ,
    size_t count )
{
    GN_GUARD;

    result.x = result.y = result.z = result.w = result.h = result.z = 0.0f;

    if( 0 == count ) return;

    float x, y, z;

    x = sGetNextValue( valueX, strideX );
    y = sGetNextValue( valueY, strideY );
    z = sGetNextValue( valueZ, strideZ );

    Vector3f vMin( x, y, z );
    Vector3f vMax( x, y, z );

    for( size_t i = 1; i < count; ++i )
    {
        x = sGetNextValue( valueX, strideX );
        y = sGetNextValue( valueY, strideY );
        z = sGetNextValue( valueZ, strideZ );

        vMin.x = math::GetMin( vMin.x, x );
        vMin.y = math::GetMin( vMin.y, y );
        vMin.z = math::GetMin( vMin.z, z );

        vMax.x = math::GetMax( vMax.x, x );
        vMax.y = math::GetMax( vMax.y, y );
        vMax.z = math::GetMax( vMax.z, z );
    }

    result.Pos() = vMin;
    result.Size() = vMax - vMin;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::CalculateBoundingBox( Boxf & result, const Vector3f * positions, size_t strideInBytes, size_t count )
{
    return CalculateBoundingBox(
        result,
        positions ? &positions[0].x : NULL, strideInBytes,
        positions ? &positions[0].y : NULL, strideInBytes,
        positions ? &positions[0].z : NULL, strideInBytes,
        count );
}
