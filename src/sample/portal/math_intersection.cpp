# include "pch.h"
# include "math.h"

/*****************************************************************************\
                            private declarations
\*****************************************************************************/

struct flatten_x_s
{
    float no_use;
    float x;
    float y;
};
struct flatten_y_s
{
    float x;
    float no_use;
    float y;
};
struct flatten_z_s
{
    float x, y;
    float no_use;
};


struct line_equation_s // y = m * x + b or X = b(when vertical)
{
    bool vertical; // 是否垂直
    float m;       // 斜率
    float b;       // 截距/X value
};

enum { MAX_VERTS = 64 };

/*****************************************************************************\
                            private functions
\*****************************************************************************/

//
// check if the point is inside the 2D plane
// ----------------------------------------------------------------------------
template < typename POINT >
static bool flatten_point_in_polygon( const POINT & point,
                             const POINT * verts, size_t numvert )
{
    GN_GUARD_SLOW;

    GN_ASSERT( verts && numvert > 2 ); // must be a valid polygon
    GN_ASSERT( numvert <= MAX_VERTS );

    line_equation_s edges[MAX_VERTS];

    // get the center of the polygon
    const POINT * v = verts;
    POINT center;
    center.x = 0; center.y = 0;
    for ( size_t i = 0; i < numvert; ++i, ++v )
    {
        center.x += v->x;
        center.y += v->y;
    }
    center.x /= numvert; center.y /= numvert;

    // 计算各边的方程
    float x1, y1, x2, y2;
    v = verts;
    line_equation_s * e = edges;
    for ( size_t i = 0; i < numvert - 1; ++i, ++e )
    {
        x1 = v->x; y1 = v->y;
        ++v;
        x2 = v->x; y2 = v->y;
        if ( 0 == (x2 - x1) )
        {
            e->vertical = true;
            e->b = x1;
        }
        else
        {
            e->vertical = false;
            e->m = ( y2 - y1 ) / ( x2 - x1 );
            e->b = y1 - e->m * x1;
        }
    }
    x1 = v->x; y1 = v->y;
    x2 = verts->x; y2 = verts->y;
    if ( 0 == (x2 - x1) )
    {
        e->vertical = true;
        e->b = x1;
    }
    else
    {
        e->vertical = false;
        e->m = ( y2 - y1 ) / ( x2 - x1 );
        e->b = y1 - e->m * x1;
    }

    // for each edge, first get the side of center respect to the edge,
    // then check if the point is on the same side.
    // the point is in the polygon only if the point lies on the same side of
    // the center respect to each edge.
    uint32 inside = 0;
    e = edges;
    float x = point.x, y = point.y;
    for ( size_t i = 0; i < numvert; ++i, ++e )
    {
        if ( e->vertical )
        {
            // this edge is vertical , so we only need to compare the x value
            if ( (center.x>e->b) == (x>e->b) ) ++inside;
        }
        else
        {
            if ( (((center.x*e->m)+e->b) > center.y) == (((x*e->m)+e->b) > y) )
                ++inside;
        }
    }

    return numvert == inside;

    GN_UNGUARD_SLOW;
}

//
// check if the point is inside the 3D plane
// ----------------------------------------------------------------------------
static bool point_in_polygon( const Vector3f & point,
                              const Plane3f & p,      // 多边形所在平面
                              const Vector3f * verts,    // 多边形顶点列表
                              size_t        numvert )  // 多边形顶点数目
{
    GN_ASSERT( 0 == classify(p, point) );

    // flatten the i-point and plane to the longest coordinate
    float a = (float)fabs(p.n.x),
          b = (float)fabs(p.n.y),
          c = (float)fabs(p.n.z);
    if ( a > b )
    {
        if ( a > c )
        {   // 投影到y-z平面
            const flatten_x_s * fpoint = (const flatten_x_s*)&point;
            const flatten_x_s * fplane = (const flatten_x_s*)verts;
            return flatten_point_in_polygon<flatten_x_s>( *fpoint, fplane, numvert );
        }
        else
        {   // 投影到x,y平面
            const flatten_z_s * fpoint = (const flatten_z_s*)&point;
            const flatten_z_s * fplane = (const flatten_z_s*)verts;
            return flatten_point_in_polygon<flatten_z_s>( *fpoint, fplane, numvert );
        }
    }
    else
    {
        if ( b > c )
        {   // 投影到x-z平面
            const flatten_y_s * fpoint = (const flatten_y_s*)&point;
            const flatten_y_s * fplane = (const flatten_y_s*)verts;
            return flatten_point_in_polygon<flatten_y_s>( *fpoint, fplane, numvert );
        }
        else
        {   // 投影到x-y平面
            const flatten_z_s * fpoint = (const flatten_z_s*)&point;
            const flatten_z_s * fplane = (const flatten_z_s*)verts;
            return flatten_point_in_polygon<flatten_z_s>( *fpoint, fplane, numvert );
        }
    }
}

/*****************************************************************************\
                            public functions
\*****************************************************************************/


//
// 平面多边形和线段的交点
// ----------------------------------------------------------------------------
bool intersection_polygon_segment( Vector3f & ipoint,         // 返回交点
                                   const Plane3f & p,       // 多边形所在平面
                                   const Vector3f * verts,    // 多边形顶点列表
                                   size_t        numvert,   // 多边形顶点数目
                                   const Vector3f & start,    // 线段起点
                                   const Vector3f & end )     // 线段终点
{
    GN_GUARD_SLOW;

    // get the intersection point of segment and plane
    if ( !intersection_plane_segment( ipoint, p, start, end ) )
        return false;

    return point_in_polygon( ipoint, p, verts, numvert );

    GN_UNGUARD_SLOW;
}

//
// 平面多边形和射线的交点
// ----------------------------------------------------------------------------
bool intersection_polygon_ray( Vector3f & ipoint,         // 返回交点
                               const Plane3f & p,      // 多边形所在平面
                               const Vector3f * verts,    // 多边形顶点列表
                               size_t     numvert,      // 多边形顶点数目
                               const Vector3f & start,    // 射线起点
                               const Vector3f & dir )     // 射线方向
{
    GN_GUARD_SLOW;

    // get the intersection point of ray and plane
    if ( !intersection_plane_ray( ipoint, p, start, dir ) ) return false;

    return point_in_polygon( ipoint, p, verts, numvert );

    GN_UNGUARD_SLOW;
}

//
// 平面多边形和直线的交点
// ----------------------------------------------------------------------------
bool intersection_polygon_line( Vector3f & ipoint,        // 返回交点
                                const Plane3f & p,     // 多边形所在平面
                                const Vector3f * verts,   // 多边形顶点列表
                                size_t numvert,         // 多边形顶点数目
                                const Vector3f & point,   // 直线上一点
                                const Vector3f & dir )    // 直线方向
{
    GN_GUARD_SLOW;

    // get the intersection point of ray and plane
    if ( !intersection_plane_line( ipoint, p, point, dir ) ) return false;

    return point_in_polygon( ipoint, p, verts, numvert );

    GN_UNGUARD_SLOW;
}
