# ifndef __EXAMPLES_portaldemo_portal_H
# define __EXAMPLES_portaldemo_portal_H
/*****************************************************************************\
  filename          : portal.h
  create time       : 2002/10/24
  created by        : chenlee
  description       :
\*****************************************************************************/

/************************* 修改记录 (Modification Log) ***********************\

  =======================================
  2002/10/24 by chenlee
  =======================================
  * 创建

\*****************************************************************************/

# include "math.h"

struct lvlPortal;
struct portal_s
{
    // ***********************************
    //   public data members
    // ***********************************
public :
    GN::StrA              name;        // 关卡编辑器中的名字，用来与脚本连接
    uint32                sector_to;   // 本portal通向哪个sector
    Plane3f               plane;       // 所在平面
    std::vector<Vector3f> verts;       // 多边形顶点列表
    GN::StrA              script_name; // 脚本文件名

    // ***********************************
    //   public operations
    // ***********************************
public :
    // initialize portal class from lvlPortal
    void initialize( const lvlPortal &, bool is_back_portal=false );
};

/*****************************************************************************\
                              END of portal.h
\*****************************************************************************/
# endif
