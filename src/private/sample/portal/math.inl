

//
//
// ----------------------------------------------------------------------------
GN_INLINE int classify( const plane3_c & p, const vec3_c & v )
{
    float d = v * p;
    if ( -PLANE_HALF_THICKNESS > d ) return -1;
    else if ( PLANE_HALF_THICKNESS < d ) return 1;
    else return 0;
}


//
//
// ----------------------------------------------------------------------------
GN_INLINE bool intersection_plane_segment( vec3_c & ipoint,
                                           const plane3_c & plane,
                                           const vec3_c & start,
                                           const vec3_c & end )
{
    float ds = plane * start;
    float de = plane * end;

    // check if start is on the plane
    if ( -PLANE_HALF_THICKNESS < ds && ds < PLANE_HALF_THICKNESS )
    {
        return false;
    }

    // check if end point is on the plane
    if ( -PLANE_HALF_THICKNESS < de && de < PLANE_HALF_THICKNESS )
    {
        ipoint = end;
        return true;
    }

    // check parallel
    float k = ds - de;
    if ( -PLANE_THICKNESS < k && k < PLANE_THICKNESS ) return false;

    // neither start nor end is on the plane, we have to calculate the
    // intersection point
    float ratio = ds / k;
    if ( ratio < 0.0f || ratio > 1.0f ) return false;
    ipoint = start + ratio * ( end - start );
//    GN_ASSERT( 0 == classify(plane, ipoint) );
    return true;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE bool intersection_plane_ray( vec3_c & ipoint,
                                       const plane3_c & plane,
                                       const vec3_c & start,
                                       const vec3_c & direction )
{
    float ds    = plane * start;
    float dd    = vec3_c::dot( -plane.n, direction);

    // check if start is on the plane
    if ( -PLANE_HALF_THICKNESS < ds && ds < PLANE_HALF_THICKNESS )
    {
        return false;
    }

    // check parallel
    if ( 0.0f == dd ) return false;

    // get the intersection point
    float ratio = ds / dd;
    if ( ratio < 0.0f ) return false;
    ipoint = start + ratio * direction;
//    GN_ASSERT( 0 == classify(plane, ipoint) );
    return true;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE bool intersection_plane_ray( vec3_c & ipoint,
                                       float  & ratio,
                                       const plane3_c & plane,
                                       const vec3_c & start,
                                       const vec3_c & direction )
{
    float ds    = plane * start;
    float dd    = vec3_c::dot( -plane.n, direction );

    // check if start is on the plane
    if ( -PLANE_HALF_THICKNESS < ds && ds < PLANE_HALF_THICKNESS )
    {
        return false;
    }

    // check parallel
    if ( 0.0f == dd ) return false;

    // get the intersection point
    ratio = ds / dd;
    if ( ratio < 0.0f ) return false;
    ipoint = start + ratio * direction;
    //    GN_ASSERT( 0 == classify(plane, ipoint) );
    return true;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE bool intersection_plane_line( vec3_c & ipoint,
                                        const plane3_c & plane,
                                        const vec3_c & point,
                                        const vec3_c & dir )
{
    float ds    = plane * point;
    float dd    = vec3_c::dot( -plane.n, dir );

    // check parallel
    if ( 0.0f == dd ) return false;

    // get the intersection point
    float ratio = ds / dd;
    ipoint = point + ratio * dir;
//    GN_ASSERT( 0 == classify(plane, ipoint) );
    return true;
}
