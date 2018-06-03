# include "pch.h"
# include "math.h"
# ifdef GN_DEBUG
# include "math.inl"
# endif

/*****************************************************************************\
                              private functions
\*****************************************************************************/

enum { MAX_VERTS = 64 };

//
//
// ----------------------------------------------------------------------------
static GN_INLINE
void clip_edge( vec3_c * & result,
               const vec3_c & start, const vec3_c & end, const plane3_c & p )
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
void clip_polygon_with_plane_list( std::vector<vec3_c> & result,
                                   const vec3_c * verts, size_t numvert,
                                   const plane3_c * planes, size_t numplane )
{
    GN_GUARD_SLOW;

    GN_ASSERT( numvert < MAX_VERTS );
    GN_ASSERT( verts && numvert >= 3 );
    GN_ASSERT( planes && numplane > 0 );

    uint i, j;
    vec3_c tmp1[MAX_VERTS];
    vec3_c tmp2[MAX_VERTS];
    const vec3_c * input = verts;
    vec3_c * output = tmp1;
    for ( i = 0; i < numplane; ++i, ++planes )
    {
        // change output to another buffer
        if ( output == tmp1 ) output = tmp2;
        else output = tmp1;

        GN_ASSERT( numvert > 0 );
        const vec3_c * v1 = input;
        vec3_c * v2 = output;
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
        memcpy( &result[0], output, sizeof(vec3_c) * numvert );
    }

    GN_UNGUARD_SLOW;
}
