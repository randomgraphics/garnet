# include "pch.h"
# include "lvlFileStruct.h"

//
//
// ----------------------------------------------------------------------------
bool lvlSector::Write( FILE * fp )
{
    GN_GUARD;

    collideFaceList.Write( fp );
    drawFaceIDList.Write( fp );
    bspNodeList.Write( fp );
    nsFaceIDList.Write( fp );
    curveIDList.Write( fp );
    modelList.Write( fp );
    pointLightIDList.Write( fp );
    spotLightIDList.Write( fp );
    entityIDList.Write( fp );
    spaceIDList.Write( fp );
    frontPortalIDList.Write( fp );
    backPortalIDList.Write( fp );
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool lvlSector::Read( FILE * fp )
{
    GN_GUARD;

    collideFaceList.Read( fp );
    bspNodeList.Read( fp );
    drawFaceIDList.Read( fp );
    nsFaceIDList.Read( fp );
    curveIDList.Read( fp );
    modelList.Read( fp );
    pointLightIDList.Read( fp );
    spotLightIDList.Read( fp );
    entityIDList.Read( fp );
    spaceIDList.Read( fp );
    frontPortalIDList.Read( fp );
    backPortalIDList.Read( fp );
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool lvlFileStruct::Write( FILE * fp )
{
    GN_GUARD;

    if ( !header.Write(fp) ) return false;
    if ( !faces.Write(fp) ) return false;
    if ( !drawfaces.Write(fp) ) return false;
    if ( !nsfaces.Write(fp) ) return false;
    if ( !curves.Write(fp) ) return false;
    if ( !modnames.Write(fp) ) return false;
    if ( !plights.Write(fp) ) return false;
    if ( !slights.Write(fp) ) return false;
    if ( !entites.Write(fp) ) return false;
    if ( !spaces.Write(fp) ) return false;
    if ( !portals.Write(fp) ) return false;
    if ( !sectors.Write(fp) ) return false;

    // write lightmaps
    fwrite( &lmap_num, sizeof(int), 1, fp );
    fwrite( &lmap_width, sizeof(int), 1, fp );
    fwrite( &lmap_height, sizeof(int), 1, fp );
    GN_ASSERT( lmap_num * lmap_width * lmap_height == lightmaps.size() );
    if ( !lightmaps.empty() )
    {
        fwrite( &lightmaps[0], sizeof(char), lightmaps.size(), fp );
    }

    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool lvlFileStruct::Read( FILE * fp )
{
    GN_GUARD;

    if ( !header.Read(fp) ) return false;
    if ( !faces.Read(fp) ) return false;
    if ( !drawfaces.Read(fp) ) return false;
    if ( !nsfaces.Read(fp) ) return false;
    if ( !curves.Read(fp) ) return false;
    if ( !modnames.Read(fp) ) return false;
    if ( !plights.Read(fp) ) return false;
    if ( !slights.Read(fp) ) return false;
    if ( !entites.Read(fp) ) return false;
    if ( !spaces.Read(fp) ) return false;
    if ( !portals.Read(fp) ) return false;
    if ( !sectors.Read(fp) ) return false;

    // read lightmaps
    fread( &lmap_num, sizeof(int), 1, fp );
    fread( &lmap_width, sizeof(int), 1, fp );
    fread( &lmap_height, sizeof(int), 1, fp );
    lightmaps.resize( lmap_num * lmap_width * lmap_height );
    if ( !lightmaps.empty() )
    {
        fread( &lightmaps[0], sizeof(char), lightmaps.size(), fp );
    }

    return true;

    GN_UNGUARD;
}
