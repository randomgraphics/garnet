#include "pch.h"
#include "garnet/gfx/shape.h"

//
//
// -----------------------------------------------------------------------------
void GN::gfx::CreateBox(
    float sx, float sy, float sz,
    float * positions, size_t posStride,
    float * texcoords, size_t texStride,
    float * normals, size_t normStride,
    float * tangents, size_t tangStride,
    float * binormals, size_t n2Stride,
    UInt16 * triList,
    UInt16 * quadList )
{
    GN_GUARD;

    //
    // Construct a box like this:
    //
    //   7----6
    //  /|   /|
    // 3----2 |    Y  Z
    // | 4__|_5    | /
    // |/   |/     |/
    // 0----1       ----> X
    //

    if( positions )
    {
        sx /= 2.0f;
        sy /= 2.0f;
        sz /= 2.0f;
        Vector3f v[8] =
        {
            Vector3f( -sx, -sy, -sz ),
            Vector3f(  sx, -sy, -sz ),
            Vector3f(  sx,  sy, -sz ),
            Vector3f( -sx,  sy, -sz ),
            Vector3f( -sx, -sy,  sz ),
            Vector3f(  sx, -sy,  sz ),
            Vector3f(  sx,  sy,  sz ),
            Vector3f( -sx,  sy,  sz ),
        };
        #define VERTEX( index ) (*(Vector3f*)(((UInt8*)positions) + posStride*index))

        VERTEX( 0) = v[0];
        VERTEX( 1) = v[1];
        VERTEX( 2) = v[2];
        VERTEX( 3) = v[3];

        VERTEX( 4) = v[0];
        VERTEX( 5) = v[4];
        VERTEX( 6) = v[5];
        VERTEX( 7) = v[1];

        VERTEX( 8) = v[1];
        VERTEX( 9) = v[5];
        VERTEX(10) = v[6];
        VERTEX(11) = v[2];

        VERTEX(12) = v[2];
        VERTEX(13) = v[6];
        VERTEX(14) = v[7];
        VERTEX(15) = v[3];

        VERTEX(16) = v[3];
        VERTEX(17) = v[7];
        VERTEX(18) = v[4];
        VERTEX(19) = v[0];

        VERTEX(20) = v[7];
        VERTEX(21) = v[6];
        VERTEX(22) = v[5];
        VERTEX(23) = v[4];

        #undef VERTEX
    }

    if( texcoords )
    {
        Vector2f v[4] =
        {
            Vector2f( 0, 0 ),
            Vector2f( 1, 0 ),
            Vector2f( 1, 1 ),
            Vector2f( 0, 1 ),
        };
        #define VERTEX( index ) (*(Vector2f*)(((UInt8*)texcoords) + texStride*index))

        VERTEX( 0) = v[0];
        VERTEX( 1) = v[1];
        VERTEX( 2) = v[2];
        VERTEX( 3) = v[3];

        VERTEX( 4) = v[0];
        VERTEX( 5) = v[1];
        VERTEX( 6) = v[2];
        VERTEX( 7) = v[3];

        VERTEX( 8) = v[0];
        VERTEX( 9) = v[1];
        VERTEX(10) = v[2];
        VERTEX(11) = v[3];

        VERTEX(12) = v[0];
        VERTEX(13) = v[1];
        VERTEX(14) = v[2];
        VERTEX(15) = v[3];

        VERTEX(16) = v[0];
        VERTEX(17) = v[1];
        VERTEX(18) = v[2];
        VERTEX(19) = v[3];

        VERTEX(20) = v[0];
        VERTEX(21) = v[1];
        VERTEX(22) = v[2];
        VERTEX(23) = v[3];

        #undef VERTEX
    }

    if( normals )
    {
        Vector3f px = Vector3f(  1,  0,  0 );
        Vector3f nx = Vector3f( -1,  0,  0 );
        Vector3f py = Vector3f(  0,  1,  0 );
        Vector3f ny = Vector3f(  0, -1,  0 );
        Vector3f pz = Vector3f(  0,  0,  1 );
        Vector3f nz = Vector3f(  0,  0, -1 );
        #define VERTEX( index ) (*(Vector3f*)(((UInt8*)normals) + normStride*index))

        VERTEX( 0) = nz;
        VERTEX( 1) = nz;
        VERTEX( 2) = nz;
        VERTEX( 3) = nz;

        VERTEX( 4) = ny;
        VERTEX( 5) = ny;
        VERTEX( 6) = ny;
        VERTEX( 7) = ny;

        VERTEX( 8) = px;
        VERTEX( 9) = px;
        VERTEX(10) = px;
        VERTEX(11) = px;

        VERTEX(12) = py;
        VERTEX(13) = py;
        VERTEX(14) = py;
        VERTEX(15) = py;

        VERTEX(16) = nx;
        VERTEX(17) = nx;
        VERTEX(18) = nx;
        VERTEX(19) = nx;

        VERTEX(20) = pz;
        VERTEX(21) = pz;
        VERTEX(22) = pz;
        VERTEX(23) = pz;

        #undef VERTEX
    }

    if( tangents )
    {
        Vector3f px = Vector3f(  1,  0,  0 );
        Vector3f nx = Vector3f( -1,  0,  0 );
        Vector3f py = Vector3f(  0,  1,  0 );
        Vector3f ny = Vector3f(  0, -1,  0 );
        Vector3f pz = Vector3f(  0,  0,  1 );
        Vector3f nz = Vector3f(  0,  0, -1 );
        #define VERTEX( index ) (*(Vector3f*)(((UInt8*)tangents) + tangStride*index))

        VERTEX( 0) = px;
        VERTEX( 1) = px;
        VERTEX( 2) = px;
        VERTEX( 3) = px;

        VERTEX( 4) = pz;
        VERTEX( 5) = pz;
        VERTEX( 6) = pz;
        VERTEX( 7) = pz;

        VERTEX( 8) = pz;
        VERTEX( 9) = pz;
        VERTEX(10) = pz;
        VERTEX(11) = pz;

        VERTEX(12) = pz;
        VERTEX(13) = pz;
        VERTEX(14) = pz;
        VERTEX(15) = pz;

        VERTEX(16) = pz;
        VERTEX(17) = pz;
        VERTEX(18) = pz;
        VERTEX(19) = pz;

        VERTEX(20) = px;
        VERTEX(21) = px;
        VERTEX(22) = px;
        VERTEX(23) = px;

        #undef VERTEX
    }

    if( binormals )
    {
        Vector3f px = Vector3f(  1,  0,  0 );
        Vector3f nx = Vector3f( -1,  0,  0 );
        Vector3f py = Vector3f(  0,  1,  0 );
        Vector3f ny = Vector3f(  0, -1,  0 );
        Vector3f pz = Vector3f(  0,  0,  1 );
        Vector3f nz = Vector3f(  0,  0, -1 );
        #define VERTEX( index ) (*(Vector3f*)(((UInt8*)binormals) + n2Stride*index))

        VERTEX( 0) = py;
        VERTEX( 1) = py;
        VERTEX( 2) = py;
        VERTEX( 3) = py;

        VERTEX( 4) = px;
        VERTEX( 5) = px;
        VERTEX( 6) = px;
        VERTEX( 7) = px;

        VERTEX( 8) = py;
        VERTEX( 9) = py;
        VERTEX(10) = py;
        VERTEX(11) = py;

        VERTEX(12) = nx;
        VERTEX(13) = nx;
        VERTEX(14) = nx;
        VERTEX(15) = nx;

        VERTEX(16) = ny;
        VERTEX(17) = ny;
        VERTEX(18) = ny;
        VERTEX(19) = ny;

        VERTEX(20) = ny;
        VERTEX(21) = ny;
        VERTEX(22) = ny;
        VERTEX(23) = ny;

        #undef VERTEX
    }

    if( triList )
    {
        for( UInt16 i = 0; i < 6; ++i )
        {
            triList[0] = i*4+2;
            triList[1] = i*4+1;
            triList[2] = i*4+0;
            triList[3] = i*4+3;
            triList[4] = i*4+2;
            triList[5] = i*4+0;
            triList += 6;
        }
    }

    if( quadList )
    {
        for( UInt16 i = 0; i < 6; ++i )
        {
            quadList[0] = i*4+3;
            quadList[1] = i*4+2;
            quadList[2] = i*4+1;
            quadList[3] = i*4+0;
            quadList += 4;
        }
    }

    GN_UNGUARD;
}
