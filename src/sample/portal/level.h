# ifndef __EXAMPLES_portaldemo_level_H
# define __EXAMPLES_portaldemo_level_H
/*****************************************************************************\
  filename          : level.h
  create time       : 2002/10/12
  created by        : chenlee
  description       :
\*****************************************************************************/

/************************* 修改记录 (Modification Log) ***********************\

  =======================================
  2002/10/12 by chenlee
  =======================================
  * 创建

\*****************************************************************************/

#include "sector.h"

//
// collideinfo_s
// =============
//
struct collideinfo_s
{
    int      sector_id; // which sector intersected point at.
    Vector3f ipoint;    // intersection point.
    Plane3f  iplane;    // intersection plane.
};

//
// level_c
// =======
//
// representing a whole level
//
class  camera_c;
class  clipfrustum_c;
struct lvlFileStruct;
struct lvlBspNode;
class level_c : public StdClass
{
    GN_DECLARE_STDCLASS( level_c, StdClass )

    // ********************************
    //    local type definitions
    // ********************************
public:

    // ********************************
    //    constructor / destructor
    // ********************************
public:
    level_c()          { clear(); }
    virtual ~level_c() { quit(); }

    // ********************************
    //    standard init / quit functions
    // ********************************
public:
    bool init( const char * filename );
    void quit();
private:
    void clear()
    {
        m_lvlfile = 0;
        m_sectors.clear();
        m_visible_sectors.clear();
    }

    // ********************************
    //    custom public functions
    // ********************************
public:

    //
    // 碰撞检测函数
    //
    // NOTE : 调用此函数时，collideinfo_s中的section_id应当为start点所在的
    //        sector，或者-1
    //
    bool collide_with_radial( collideinfo_s &, const Vector3f & start,
                              const Vector3f & end ) const;
    bool collide_with_segment( collideinfo_s &, const Vector3f & start,
                               const Vector3f & end ) const;

    void draw( const Matrix44f & proj, const Matrix44f & view );

    // 得到sector的列表
    const sector_c * get_sector_list() const
    {
        GN_ASSERT( !m_sectors.empty() );
        return &m_sectors[0];
    }

    // ********************************
    //    private variables
    // ********************************
private:

    // level data set
    lvlFileStruct * m_lvlfile;

    // sector list
    typedef std::vector<sector_c> sector_list_c;
    sector_list_c m_sectors;

    // visible sector list
    std::vector<uint32> m_visible_sectors;

    // ********************************
    //         Private functions
    // ********************************
private:

    // update level's visibility information
    void update_visinfo( const Matrix44f & proj,
                         const Matrix44f & view );

    // recursive check visiblity of all sectors
    void check_sector( uint32 sector_id, clipfrustum_c & );

    // 在一个section中用BSP树检查多边形在view-frustum中的可见性
    bool bsptree_check_poly( uint32 sector_id,
                             const clipfrustum_c & cf,
                             const Vector3f * vlist, size_t numvert );
};

/*****************************************************************************\
                              END of level.h
\*****************************************************************************/
# endif
