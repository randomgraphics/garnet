# include "pch.h"
# include "sector.h"
# include "level.h"


//
//
// ----------------------------------------------------------------------------
bool sector_c::collide_with_radial( collideinfo_s & cinfo,
                                    const vec3_c & start,
                                    const vec3_c & dir ) const
{
    // make sure start point is in myself
    GN_ASSERT( -1 != in_solid(start) );

    // find the portal which the ray going through
    vec3_c ipoint;
    portal_list_c::const_iterator pi = portals.begin(),
        pe = portals.end();
    for ( ; pi != pe; ++pi )
    {
        // jump off portal behind the start point
        int side = classify( pi->plane, start );
        if ( -1 == side ) continue;
        // check collision of the portal with start-end
        if ( 1 == side )
        {
            if ( intersection_polygon_ray( ipoint, pi->plane,
                GN_vec2ptr(pi->verts), pi->verts.size(), start, dir ) ) break;
        }
        else
        {
            // start point is on the portal, so
            GN_ASSERT( level );
            const sector_c & s = level->get_sector_list()[pi->sector_to];
            cinfo.sector_id = pi->sector_to;
            return s.collide_with_radial( cinfo, start, dir );
        }
    }
    if ( pi == pe )
    {
        // the ray is not going through any portal, we only need do collide
        // detection in this sector
        bsptree_c::cldinfo_s ci;
        bool collision = bsptree.check_radial( &ci, start, dir );
        if ( collision )
        {
            GN_ASSERT( ci.inodeidx > 0 );
            cinfo.ipoint = ci.ipoint;
            cinfo.iplane = bsptree.get_node_list()[ci.inodeidx].plane;
        }
        return collision;
    }
    else
    {
        // make sure ipoint is on the portal
        GN_ASSERT( 0 == classify( pi->plane, ipoint) );

        // first check collision in this sector
        bsptree_c::cldinfo_s ci;
        if ( bsptree.check_segment( &ci, start, ipoint ) )
        {
            cinfo.ipoint = ci.ipoint;
            cinfo.iplane = pi->plane;
            return true;
        }

        // the ray is colliding with bsptree only on the portal so, we have to
        // check collision in the portal._to_section
        GN_ASSERT( level );
        const sector_c & s = level->get_sector_list()[pi->sector_to];
        cinfo.sector_id = pi->sector_to;
        return s.collide_with_radial( cinfo, ipoint, dir );
    }
}

extern bool halt;
//
//
// ----------------------------------------------------------------------------
bool sector_c::collide_with_segment( collideinfo_s & cinfo,
                                     const vec3_c & start,
                                     const vec3_c & end ) const
{
    // make sure start point is in myself
    // GN_ASSERT( -1 != in_solid(start) );
    if ( -1 == in_solid(start) )
    {
        halt = true;
        return false;
    }

    // find the portal which the ray going through
    vec3_c ipoint;
    portal_list_c::const_iterator pi = portals.begin(),
        pe = portals.end();
    for ( ; pi != pe; ++pi )
    {
        // jump off portal which start point lay behind of
        int side = classify( pi->plane, start );
        if ( -1 == side ) continue;

        // check collision of the portal with start-end
        if ( !intersection_polygon_segment( ipoint, pi->plane,
            GN_vec2ptr(pi->verts), pi->verts.size(), start, end ) ) continue;

        // does start point lay on the portal ?
        if ( 0 == side )
        {
            // start point is on the portal, so...
            GN_ASSERT( level );
            const sector_c & s = level->get_sector_list()[pi->sector_to];
            cinfo.sector_id = pi->sector_to;
            return s.collide_with_segment( cinfo, start, end );
        }
        else break;
    }
    if ( pi == pe )
    {
        // the ray is not going through any portal, we only need do collide
        // detection in this sector
        bsptree_c::cldinfo_s ci;
        bool collision = bsptree.check_segment( &ci, start, end );
        if ( collision )
        {
            GN_ASSERT( ci.inodeidx >= 0 );
            cinfo.ipoint = ci.ipoint;
            cinfo.iplane = bsptree.get_node_list()[ci.inodeidx].plane;
        }
        return collision;
    }
    else
    {
        // make sure ipoint is on the portal
        GN_ASSERT( 0 == classify( pi->plane, ipoint) );

        // first check collision in this sector
        bsptree_c::cldinfo_s ci;
        if ( bsptree.check_segment( &ci, start, ipoint ) )
        {
            cinfo.ipoint = ci.ipoint;
            cinfo.iplane = pi->plane;
            return true;
        }

        // the ray is colliding with bsptree only on the portal so, we have to
        // check collision in the portal._to_section
        GN_ASSERT( level );
        const sector_c & s = level->get_sector_list()[pi->sector_to];
        cinfo.sector_id = pi->sector_to;
        return s.collide_with_segment( cinfo, ipoint, end );
    }
}
