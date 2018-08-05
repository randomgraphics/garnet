# include "pch.h"
# include "clipfrustum.h"
# include "portaldemo.h"

//
//
// ----------------------------------------------------------------------------
void clipfrustum_c::from_projview( const Matrix44f & proj,
                                   const Matrix44f & view )
{
    GN_GUARD_SLOW;

    // store eye position
    use_near_verts = false;
    vec4_c tmp;
    Matrix44f iview = Matrix44f::invert( view );
    tmp = iview*vec4_c(0,0,0,1);
    eye_point     = tmp.to_vec3();
    tmp = iview * vec4_c( 0,0,-1, 0 );
    eye_direction.set( tmp.x, tmp.y, tmp.z );

    // get inversion of projection and view tranformation
    Matrix44f m = proj * view;
    m.invert();

    // calculate far vectors
    far_verts[0] = m.transform_point( Vector3f(-1.0f,-1.0f,1.0f) );
    far_verts[1] = m.transform_point( Vector3f( 1.0f,-1.0f,1.0f) );
    far_verts[2] = m.transform_point( Vector3f( 1.0f, 1.0f,1.0f) );
    far_verts[3] = m.transform_point( Vector3f(-1.0f, 1.0f,1.0f) );

    // build near/far planes
    planes[1].from_3points( far_verts[0], far_verts[1], far_verts[2] );
    planes[1].normalize();
    planes[0].from_pointnormal( eye_point, -planes[1].n );

    // build side planes
    planes[2].from_3points( eye_point, far_verts[1], far_verts[0] );
    planes[3].from_3points( eye_point, far_verts[2], far_verts[1] );
    planes[4].from_3points( eye_point, far_verts[3], far_verts[2] );
    planes[5].from_3points( eye_point, far_verts[0], far_verts[3] );
    planes[2].normalize();
    planes[3].normalize();
    planes[4].normalize();
    planes[5].normalize();

    num_planes = 6;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void clipfrustum_c::clip_by_polygon( clipfrustum_c & result,
                                     const Vector3f * verts, size_t numvert,
                                     const Plane3f & plane ) const
{
    GN_GUARD_SLOW;

    // can not clip itself
    GN_ASSERT( this != &result );

    // must be a valid polygon
    GN_ASSERT( verts && 3 <= numvert && numvert < MAX_PLANES - 2 );

    size_t i;

    // get distance of eye_point and the plane
    float d = plane * eye_point;

    // check if eye_point is on the polygon
    //
    // NOTE : 此处用10倍的平面厚度做检测，是为了避免eye_point在距离平面过近
    //        时的浮点计算误差。
    //
    if ( -PLANE_THICKNESS * 10 < d && d < PLANE_THICKNESS * 10 )
    {
        // no need to calculate near vertices
        result.use_near_verts = false;

        // clip far vertices by the plane
        std::vector<Vector3f> varray;
        if ( Vector3f::dot( eye_direction, plane.n ) >= 0.0f )
        {
            clip_polygon_with_plane_list(varray, far_verts,
                num_planes - 2, &plane, 1 );
        }
        else
        {
            Plane3f reverse_plane = -plane;
            clip_polygon_with_plane_list(varray, far_verts,
                num_planes - 2, &reverse_plane, 1 );
        }
        GN_ASSERT( varray.size() < MAX_PLANES - 2 );
        numvert = varray.size();
        if ( numvert > 0 )
        {
            memcpy( result.far_verts, &varray[0], sizeof(Vector3f)*numvert );
        }

        // copy near/far planes from source frustum
        result.planes[NEAR_PLANE] = this->planes[NEAR_PLANE];
        result.planes[FAR_PLANE]  = this->planes[FAR_PLANE];
    }
    else
    {
        // calculate near/far vertices of result frustum
        result.use_near_verts = true;
        Vector3f dir;
        for ( i = 0; i < numvert; ++i )
        {
            result.near_verts[i] = verts[i];
            dir = (verts[i] - eye_point);
            dir.normalize();
            GN_VERIFY( intersection_plane_ray( result.far_verts[i],
                planes[FAR_PLANE],
                eye_point, dir ) );
        }

        // build near/far planes
        result.planes[NEAR_PLANE] = -plane;
        result.planes[FAR_PLANE].from_3points(
            result.far_verts[0], result.far_verts[1], result.far_verts[2] );
        result.planes[FAR_PLANE].normalize();
    }


    // build side planes ( except the last one )
    for ( i = 2; i <= numvert; ++i )
    {
        result.planes[i].from_3points(
            eye_point, result.far_verts[i-1], result.far_verts[i-2] );
        result.planes[i].normalize();
    }
    // build the last side plane
    result.planes[i].from_3points( eye_point,
        result.far_verts[0], result.far_verts[numvert-1] );
    result.planes[i].normalize();

    // success
    result.eye_point      = eye_point;
    result.eye_direction  = eye_direction;
    result.num_planes = numvert + 2;

    GN_UNGUARD_SLOW;
}

//
// Fast ( but not accurate ) determination of visibility of
// a poly with the clipfrustum :
//
// If all vertices are outside of one of the frustum-planes, then the polygon
// must be fully invisible, if all vertices are inside of clipfrustum, then
// the polygon must be fully visible, else it's partially visibile.
//
// ----------------------------------------------------------------------------
int clipfrustum_c::check_poly_visibility( const Vector3f * verts,
                                          size_t numvert,
                                          const Plane3f & plane ) const
{
    GN_GUARD_SLOW;

    GN_ASSERT( verts && numvert >= 3 ); // must be a valid polygon
    GN_ASSERT( num_planes >=5 ); // near, far, and three side planes at least

    size_t i;

    // 检查 eye_point 是否落在plane上
    int eye_side = classify( plane, eye_point );

    if ( eye_side != 0 )
    {
        // 检查多边形是否完全处于某一剪裁平面的外侧，或者完全被frustum包围
        const Plane3f * p = planes;
        bool fully_visible = true;
        for ( i = 0; i < num_planes; ++i, ++p )
        {
            const Vector3f * v = verts;
            uint counter = 0;
            for ( uint j = 0; j < numvert; ++j, ++v )
            {
                if ( 1 == classify(*p, *v) ) ++counter;
            }
            if( counter == 0 )
            {
                return -1; // fully invisible
            }
            else if ( numvert != counter )
            {
                fully_visible = false;
            }
        }
        if ( fully_visible ) return 1; // fully visible
    }
    else
    {
        // eye_point正好落在polygon上，因此，如果frustum的每个顶点都在polygon
        // 同侧的话，the polygon must be ouside of the frustum
        size_t n = num_planes - 2;
        GN_ASSERT( n > 0 );
        int side1 = 0;
        for ( i = 0; i < n; ++i )
        {
            // 跳过正好落在平面plane上的点
            side1 = classify( plane, far_verts[i] );
            if ( side1 != 0 ) break;
        }
        // 剩余的顶点应当与side1同侧
        int side2;
        if ( side1 > 0 ) for ( ; i < n; ++i )
        {
            side2 = classify( plane, far_verts[i] );
            if ( side2 < 0 ) return 0; // must be partially visible
        }
        else for (; i < n; ++i )
        {
            side2 = classify( plane, far_verts[i] );
            if ( side2 > 0 ) return 0; // must be partially visible
        }
        return -1; // must be fully invisible
    }

    // more check : if all vertices of frustum are on the same side of
    // the polygon, then the polygon must be invisible
    numvert = num_planes - 2;
    if ( use_near_verts ) for ( i = 0; i < numvert; ++i )
    {
        if ( eye_side != classify(plane, near_verts[i]) ) return 0;
    }
    for ( i = 0; i < numvert; ++i )
    {
        if ( eye_side != classify(plane, far_verts[i]) ) return 0;
    }
    return -1;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void clipfrustum_c::draw()
{
    GN_GUARD;

    // set render states
    g_im3drender.set_effect( g_efflib.find( GN_T("portaldemo_nolight.gfx") ) );

    // draw clip frustum
    g_im3drender.draw_begin( GNgfx::LINE_LIST, GNgfx::float4_c(1,1,0,1) );
        GN_ASSERT( num_planes >= 5 );
        size_t numvert = num_planes - 2;
        for ( uint i = 0; i < numvert; ++i )
        {
            // side planes
            g_im3drender.coord( eye_point );
            g_im3drender.coord( far_verts[i] );
            g_im3drender.coord( eye_point );
            g_im3drender.coord( far_verts[(i+1)%numvert] );
            // far plane
            g_im3drender.coord( far_verts[i] );
            g_im3drender.coord( far_verts[(i+1)%numvert] );
            // near plane
            if ( use_near_verts )
            {
                g_im3drender.coord( near_verts[i] );
                g_im3drender.coord( near_verts[(i+1)%numvert] );
            }
        }
    g_im3drender.draw_end();

    GN_UNGUARD;
}
