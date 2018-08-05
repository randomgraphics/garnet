# include "pch.h"
# include "level.h"
# include "clipfrustum.h"

//
//
// ----------------------------------------------------------------------------
void level_c::update_visinfo( const Matrix44f & proj,
                              const Matrix44f & view )
{
    GN_GUARD;

    // clear visible section list
    m_visible_sectors.clear();

    // clear all visibility checking flags
    for ( uint32 i = 0; i < m_sectors.size(); ++i )
    {
        m_sectors[i].vischeck = false;
    }

    // find out the sector where the camera is currently in
    //
    // TODO : may cache last camera sector to avoid searching each time.
    //
    int camera_sector = -1;
    Vector3f o = ( Matrix44f::invert(view) * GN::Vector4f(0,0,0,1) ).to_vec3();
    for ( uint32 i = 0; i < m_sectors.size(); ++i )
    {
        if ( -1 != m_sectors[i].in_solid(o) )
        {
            camera_sector = i;
            break;
        }
    }

    // output sector entrance/exit message
    static int last_sector = -2;
    if ( last_sector != camera_sector )
    {
        GN_INFO( GN_formatstr(
            GN_T("======= Go from sector %d to %d ========="),
            last_sector, camera_sector ) );
        last_sector = camera_sector;
    }

    // build initial frustum from camera
    clipfrustum_c frustum;
    frustum.from_projview( proj, view );

    // if camera is not in any sectors,
    // then we make all sector visble( just for test purpose )
    if ( -1 == camera_sector )
    {
        m_visible_sectors.resize( m_sectors.size() );
        for( uint32 i = 0; i < m_sectors.size(); ++i )
            m_visible_sectors[i] = i;
        return;
    }

    // recursive finding of all visible portals and sectors
    check_sector( camera_sector, frustum );

    GN_UNGUARD;
}

//
// recursive finding all visible sectors
// ----------------------------------------------------------------------------
void level_c::check_sector( uint32 sector_id, clipfrustum_c & cf )
{
    GN_GUARD_SLOW;

    sector_c & s = m_sectors[sector_id];

    // mark this sector as visible and set the visibility checking flag
    if ( !s.vischeck )
    {
        m_visible_sectors.push_back( sector_id );
        s.vischeck = true;
    }
//    else return;

    // check visibiliy of all portals in this sector
    for ( uint32 i = 0; i < s.portals.size(); ++i )
    {
        const portal_s & p = s.portals[i];
        // jump off portal which eye point lay behind of
        int v;
        v = classify(p.plane, cf.eye_point );
        if ( -1 == v ) continue;

        // jump off out-of-frustum portal
        if ( 0 == v )
        {
            v = cf.check_poly_visibility( &p.verts[0], p.verts.size(), p.plane );
            if ( -1 == v )
            {
                // 如果eye_point没有落在portal内，则该portal一定不可见
                Vector3f ipoint;
                if ( !intersection_polygon_line(ipoint, p.plane, &p.verts[0],
                    p.verts.size(), cf.eye_point, cf.eye_direction) )
                    continue;

                float k = Vector3f::dot( cf.eye_direction, p.plane.n );
                GN_ASSERT( k != 0.0f );
                // eyepoint正好落在portal上，且视线方向正好透过portal，
                // so we treat this portal visble.
                if ( k < 0.0f ) check_sector( p.sector_to, cf );
                continue;
            }
        }
        else
        {
            v = cf.check_poly_visibility( &p.verts[0], p.verts.size(), p.plane );
            if ( -1 == v ) continue;
        }

        // jump off occluded-by-bsp portal
        if ( !bsptree_check_poly(sector_id, cf,
            &p.verts[0], p.verts.size()) ) continue;

        // adjust frustum to get to minimum visible set
        clipfrustum_c cf1;
        if ( 0 == v )
        {
            // portal is partially visible, so we have to
            // clip it with frustum.
            std::vector<Vector3f> clipresult;
            clip_polygon_with_plane_list( clipresult, &p.verts[0],
                p.verts.size(), cf.planes, cf.num_planes );
            if ( clipresult.size() < 3 ) continue;

            // adjust frustum with portal
            GN_ASSERT( !clipresult.empty() );
            cf.clip_by_polygon( cf1, &clipresult[0], clipresult.size(), p.plane );
        }
        else /**/
        {
            // no clip is needed
            GN_ASSERT( !p.verts.empty() );
            cf.clip_by_polygon( cf1, &p.verts[0], p.verts.size(), p.plane );
        }  /**/

        // do recursion
        check_sector( p.sector_to, cf1 );
    }

    // draw the frustum
    cf.draw();

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
bool level_c::bsptree_check_poly( uint32 /*section_id*/,
                                  const clipfrustum_c & /*cf*/,
                                  const Vector3f * /*vlist*/,
                                  size_t /*numvert*/ )
{
    GN_GUARD_SLOW;

//*

    // return true, means polygon is always visible
    return true;

/*/ // 这个函数在多边形的所有顶点都不可见时，即判定该多边形为完全不可见，
    // 这显然是不正确的。因为正确而又高效的方法还没想出来，所以这个函数
    // 暂时被禁用了 :(


    // must be a valid sector ID
    GN_ASSERT( 0 <= section_id && section_id < m_sectors.size() );
    // must be a valid polygon
    GN_ASSERT( vlist && numvert >= 3 );

    const bsptree_c & bt = m_sectors[section_id].bsptree;

    if ( cf._use_near_verts )
    {
        for ( uint32 i = 0; i < numvert; ++i, ++vlist )
        {
            // get intersection of near-plane and [eyepoint,*vlist]
            Vector3f v;
            GN_VERIFY( intersection_plane_segment(v, cf.planes[0],
                cf.eye_point, *vlist) );

            // check visibility between intersection and *vlist
            if ( !bt.check_segment(0, v, *vlist) ) return true;
        }
    }
    else
    {
        for ( uint32 i = 0; i < numvert; ++i, ++vlist )
        {
            // 直接检查视点和目标多边形间的可见性
            if ( !bt.check_segment(0, cf.eye_point, *vlist) )
                return true;
        }
    }

    // not visible at all
    return false; //*/

    GN_UNGUARD_SLOW;
}
