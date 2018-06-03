# include "pch.h"
# include "level.h"


//
//
// ----------------------------------------------------------------------------
bool level_c::collide_with_radial( collideinfo_s & cinfo,
                                   const vec3_c & start,
                                   const vec3_c & end ) const
{
    GN_GUARD_SLOW;

    if ( cinfo.sector_id < 0 )
    {
        // find start sector first
        sector_list_c::const_iterator si, se = m_sectors.end();
        int i = 0;
        for ( si = m_sectors.begin(); si != se; ++si, ++i )
        {
            if ( -1 != si->in_solid(start) )
            {
                cinfo.sector_id = i;
                return si->collide_with_radial(cinfo, start, end );
            }
        }

        // start point is outside of any sector
        GN_ERROR( GN_T("start point is not in any sector!") );
        return false;
    }
    else
    {
        GN_ASSERT( uint(cinfo.sector_id) < m_sectors.size() );
        return m_sectors[cinfo.sector_id].collide_with_radial(
           cinfo, start, end );
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
bool level_c::collide_with_segment( collideinfo_s & cinfo,
                                    const vec3_c & start,
                                    const vec3_c & end ) const
{
    GN_GUARD_SLOW;

    if ( cinfo.sector_id < 0 )
    {
        // find start sector first
        sector_list_c::const_iterator si, se = m_sectors.end();
        int i = 0;
        for ( si = m_sectors.begin(); si != se; ++si, ++i )
        {
            if ( -1 != si->in_solid(start) )
            {
                cinfo.sector_id = i;
                return si->collide_with_segment(cinfo, start, end );
            }
        }

        // start point is outside of any sector
        GN_WARN( GN_T("start point is not in any sector!") );
        return false;
    }
    else
    {
        GN_ASSERT( uint(cinfo.sector_id) < m_sectors.size() );
        return m_sectors[cinfo.sector_id].collide_with_segment(
            cinfo, start, end );
    }

    GN_UNGUARD_SLOW;
}
