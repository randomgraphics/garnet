# include "pch.h"
# include "level.h"
# include "lvlFileStruct.h"
# include "portaldemo.h"

/*****************************************************************************\
                          local type definitions
\*****************************************************************************/

//
// autofile_c
// ==========
//
class autofile_c
{
public :
    autofile_c(FILE * fp) : m_fp(fp) {}
    ~autofile_c() { close(); }
public :
    void close()  { if ( m_fp ) fclose(m_fp), m_fp = 0; }
private :
    FILE * m_fp;
};

/*****************************************************************************\
                         stdclass init / quit functions
\*****************************************************************************/

//
//
// ----------------------------------------------------------------------------
bool level_c::init( const char * filename )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( level_c, () );

    // check parameter
    if ( !filename )
    {
        GN_ERROR( GN_T("NULL filename!") );
        quit(); return ISOK();
    }

    // open file
    FILE * fp = fopen( filename, "rb" );
    if ( !fp )
    {
        GN_ERROR( GN_formatstr( GN_T("fail to open file %s!"), filename ) );
        quit(); return ISOK();
    }
    autofile_c autofile(fp);

    // read data into lvlFileStruct
    m_lvlfile = new lvlFileStruct;
    if ( !m_lvlfile->Read( fp ) )
    {
        GN_ERROR( GN_formatstr( GN_T("fail to read file %s!"), filename ) );
        quit(); return ISOK();
    }

    // initiate sector list
    m_sectors.resize( m_lvlfile->sectors.length );
    for ( uint i = 0; i < m_sectors.size(); ++i )
    {
        m_sectors[i].level = this;

        // setup bsp tree
        lvlSector & s = m_lvlfile->sectors.data[i];
        m_sectors[i].bsptree.initialize(
            s.bspNodeList.data, s.bspNodeList.length );

        // setup sector portal list
        uint j, k;
        m_sectors[i].portals.resize(
            s.frontPortalIDList.length + s.backPortalIDList.length );
        k = 0;
        for ( j = 0; j < s.frontPortalIDList.length; ++j, ++k )
        {
            m_sectors[i].portals[k].initialize(
                m_lvlfile->portals.data[s.frontPortalIDList.data[j]] );
        }
        for ( j = 0; j < s.backPortalIDList.length; ++j, ++k )
        {
            m_sectors[i].portals[k].initialize(
                m_lvlfile->portals.data[s.backPortalIDList.data[j]], true );
        }
    }

    // do pre-location of visible sector list
    m_visible_sectors.reserve( m_lvlfile->sectors.length );

    // success
    return ISOK();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void level_c::quit()
{
    GN_GUARD;

    // delete level file structure
    if ( m_lvlfile ) delete m_lvlfile, m_lvlfile = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

/*****************************************************************************\
                            public functions
\*****************************************************************************/

//
//
// ----------------------------------------------------------------------------
void level_c::draw( const matrix44_c & proj, const matrix44_c & view )
{
    GN_GUARD;

    // enable light
    g_im3drender.set_effect( g_efflib.find(GN_T("portaldemo_light.gfx")) );

    //for ( int i = 0; i < m_lvlfile->sectors.length; ++i )
    for ( uint i = 0; i < m_visible_sectors.size(); ++i )
    {
        // lvlSector & s = m_lvlfile->sectors.data[i];
        GN_ASSERT( m_visible_sectors[i] < m_lvlfile->sectors.length );
        lvlSector & s = m_lvlfile->sectors.data[m_visible_sectors[i]];

        // draw all drawfaces
        g_render.set_diffuse( GNgfx::float4_c(0 == (i%3), 1 == (i%3), 2 == (i%3), 0.4f) );
        g_im3drender.draw_begin( GNgfx::TRIANGLE_LIST );
        for ( uint j = 0; j < s.drawFaceIDList.length; ++j )
        {
            uint id = s.drawFaceIDList.data[j];
            GN_ASSERT( id < m_lvlfile->drawfaces.length );
            lvlDrawFace & f = m_lvlfile->drawfaces.data[id];
            GN_ASSERT( f.faceId < m_lvlfile->faces.length );
            lvlFace & brush = m_lvlfile->faces.data[f.faceId];

            // draw one face
            GN_ASSERT( f.num_vert >= 3 );
            for( uint i = 1; i < f.num_vert-1; ++i )
            {
                g_im3drender.normal( brush.plane.n );
                g_im3drender.coord( f.vList[0] );
                g_im3drender.coord( f.vList[i] );
                g_im3drender.coord( f.vList[i+1] );
            }
        }
        g_im3drender.draw_end();
    }

    //* // draw eye sight
    g_im3drender.set_effect( g_efflib.find(GN_T("portaldemo_nolight.gfx")) );
    collideinfo_s cinfo;
    cinfo.sector_id = -1;
    matrix44_c iview = matrix44_c::invert( view );
    vec3_c start = ( iview * vec4_c(0,0,0,1) ).to_vec3();
    vec3_c end   = ( iview * vec4_c(0,0,-5000,1) ).to_vec3();
    if ( collide_with_segment( cinfo, start, end ) )
    {
        g_im3drender.draw_begin( GNgfx::LINE_LIST, GNgfx::float4_c(1,1,1,1) );
            g_im3drender.coord( start );
            g_im3drender.coord( cinfo.ipoint );
        g_im3drender.draw_end();
    } /**/

    // update visibility information
    update_visinfo( proj, view );

    GN_UNGUARD;
}

/*****************************************************************************\
                               private functions
\*****************************************************************************/

