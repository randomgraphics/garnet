# include "pch.h"
# include "math.h"
# ifdef GN_DEBUG
# include "math.inl"
# endif

using namespace GN;

/*****************************************************************************\
                              private functions
\*****************************************************************************/

enum { MAX_VERTS = 64 };

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void clip_edge( Vector3f * & result,
               const Vector3f & start, const Vector3f & end, const Plane3f & p )
{
    int s = classify( p, start );
    int e = classify( p, end );
    if ( 1 == s )
    {
        if ( -1 == e )
        {
            // start在内，end在外
            intersection_plane_segment( *result, p, start, end );
            ++result;
        }
        else
        {
            // start在内，end在外或正好在面上
            *result = end; ++result;
        }
    }
    else if ( -1 == s )
    {
        if ( 1 == e )
        {
            // start在外，end在内
            intersection_plane_segment( *result, p, start, end );
            ++result;
            *result = end; ++result;
        }
        else if ( 0 == e )
        {
            // start在外，end在面上
            *result = end; ++result;
        }
        else
        {
            // start和end都在剪裁面外
            //
            // do nothing
            //
        }
    }
    else
    {
        if ( -1 != e )
        {
            // start在边，end在内或者在面上
            *result = end; ++result;
        }
        else if ( -1 == e )
        {
            // start在边，end在外
            // do nothing
        }
    }
}


/*****************************************************************************\
                            public functions
\*****************************************************************************/

//
//
// ----------------------------------------------------------------------------
void clip_polygon_with_plane_list( std::vector<Vector3f> & result,
                                   const Vector3f * verts, size_t numvert,
                                   const Plane3f * planes, size_t numplane )
{
    GN_GUARD_SLOW;

    GN_ASSERT( numvert < MAX_VERTS );
    GN_ASSERT( verts && numvert >= 3 );
    GN_ASSERT( planes && numplane > 0 );

    uint32 i, j;
    Vector3f tmp1[MAX_VERTS];
    Vector3f tmp2[MAX_VERTS];
    const Vector3f * input = verts;
    Vector3f * output = tmp1;
    for ( i = 0; i < numplane; ++i, ++planes )
    {
        // change output to another buffer
        if ( output == tmp1 ) output = tmp2;
        else output = tmp1;

        GN_ASSERT( numvert > 0 );
        const Vector3f * v1 = input;
        Vector3f * v2 = output;
        for ( j = 0; j < numvert - 1; ++j, ++v1 )
        {
            clip_edge( v2, *v1, v1[1], *planes );
        }
        clip_edge( v2, *v1, *input, *planes );

        // use current ouput as next input
        input = output;
        numvert = v2 - output;
        GN_ASSERT( numvert < MAX_VERTS );

        if ( 0 == numvert )
        {
            // the polygon was completely clipped out
            result.clear();
            return;
        }
    }

    // copy output to result
    result.resize( numvert );
    if ( numvert > 0 )
    {
        memcpy( &result[0], output, sizeof(Vector3f) * numvert );
    }

    GN_UNGUARD_SLOW;
}
