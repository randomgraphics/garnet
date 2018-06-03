# include "pch.h"
# include "portal.h"
# include "lvlFileStruct.h"

void portal_s::initialize( const lvlPortal & p, bool is_back_portal )
{
    // initialize geometry data
    verts.resize( p.numVert );
    if ( is_back_portal )
    {
        sector_to = p.frontSectorID;
        plane     = -p.plane;
        for ( uint i = 0; i < p.numVert; ++i )
        {
            verts[p.numVert-1-i] = p.vList[i];
        }
    }
    else
    {
        sector_to = p.backSectorID;
        plane     = p.plane;
        for ( uint i = 0; i < p.numVert; ++i )
        {
            verts[i] = p.vList[i];
        }
    }

    // initialize remained data members
    name = GN_mbs2str(p.name);
    script_name = GN_mbs2str(p.scriptName);
}
