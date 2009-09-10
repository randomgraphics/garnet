#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.base.BoundVolume");

//
//
// -----------------------------------------------------------------------------
void GN::calculateBoundingSphere( Spheref & result, const Vector3f * positions, size_t count, size_t strideInBytes )
{
    GN_GUARD;

    Boxf box;

    calculateAABB( box, positions, count, strideInBytes );

    result.center = box.center();
    result.radius = Vector3f::sDistance( result.center, box.pos() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::calculateAABB( Boxf & result, const Vector3f * positions, size_t count, size_t strideInBytes )
{
    GN_GUARD;

    if( 0 == positions )
    {
        GN_ERROR(sLogger)( "Null position array." );
        return;
    }
    if( 0 == count )
    {
        GN_ERROR(sLogger)( "There's no way to calculate bounding box for 0 vertices." );
        return;
    }

    Vector3f vMin( positions[0] );
    Vector3f vMax( positions[0] );

    const UInt8 * ptr = (const UInt8 *)positions;

    for( size_t i = 1; i < count; ++i, ptr += strideInBytes )
    {
        const Vector3f & v = ((const Vector3f*)ptr)[0];

        vMin.x = math::getmin( vMin.x, v.x );
        vMin.y = math::getmin( vMin.y, v.y );
        vMin.z = math::getmin( vMin.z, v.z );

        vMax.x = math::getmax( vMax.x, v.x );
        vMax.y = math::getmax( vMax.y, v.y );
        vMax.z = math::getmax( vMax.z, v.z );
    }

    result.pos() = vMin;
    result.size() = vMax - vMin;

    GN_UNGUARD;
}
