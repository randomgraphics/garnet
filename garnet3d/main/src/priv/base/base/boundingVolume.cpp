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
void GN::calculateBoundingSphere(
    Spheref & result,
    const float * x, size_t strideX,
    const float * y, size_t strideY,
    const float * z, size_t strideZ,
    size_t count )
{
    Boxf box;

    calculateBoundingBox( box, x, strideX, y, strideY, z, strideZ, count );

    result.center = box.center();
    result.radius = Vector3f::sDistance( result.center, box.pos() );
}

//
//
// -----------------------------------------------------------------------------
void GN::calculateBoundingSphere( Spheref & result, const Vector3f * positions, size_t strideInBytes, size_t count )
{
    return calculateBoundingSphere(
        result,
        positions ? &positions[0].x : NULL, strideInBytes,
        positions ? &positions[0].y : NULL, strideInBytes,
        positions ? &positions[0].z : NULL, strideInBytes,
        count );
}

//
//
// -----------------------------------------------------------------------------
void GN::calculateBoundingBox(
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

        vMin.x = math::getmin( vMin.x, x );
        vMin.y = math::getmin( vMin.y, y );
        vMin.z = math::getmin( vMin.z, z );

        vMax.x = math::getmax( vMax.x, x );
        vMax.y = math::getmax( vMax.y, y );
        vMax.z = math::getmax( vMax.z, z );
    }

    result.pos() = vMin;
    result.size() = vMax - vMin;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::calculateBoundingBox( Boxf & result, const Vector3f * positions, size_t strideInBytes, size_t count )
{
    return calculateBoundingBox(
        result,
        positions ? &positions[0].x : NULL, strideInBytes,
        positions ? &positions[0].y : NULL, strideInBytes,
        positions ? &positions[0].z : NULL, strideInBytes,
        count );
}
