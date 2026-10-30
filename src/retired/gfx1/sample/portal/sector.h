#ifndef __EXAMPLES_portaldemo_sector_H
#define __EXAMPLES_portaldemo_sector_H
/*****************************************************************************\
  filename          : sector.h
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

#include "bsptree.h"
#include "portal.h"

struct collideinfo_s;
class level_c;
class sector_c {
    // portal list
    typedef std::vector<portal_s> portal_list_c;

    // ***********************************
    //   public data members
    // ***********************************
public:
    level_c *     level; // 指向本sector所在的level
    bsptree_c     bsptree;
    portal_list_c portals;
    bool          vischeck; // visibility checking flag
                            // ( to prevent infinit-recursion )

    // ***********************************
    //   public operations
    // ***********************************
public:
    // 与射线的碰撞检测
    bool collide_with_radial(collideinfo_s &, const Vector3f & start, const Vector3f & dir) const;

    // 与线段的碰撞检测
    bool collide_with_segment(collideinfo_s &, const Vector3f & start, const Vector3f & end) const;

    // check if the point is in solid space of the sector
    // return value :
    //   -1 : solid space
    //    0 : 正好落在边界上
    //    1 : else
    int in_solid(const Vector3f & v) const { return bsptree.in_solid(v); }

    // ***********************************
    //   constructor/destructor
    // ***********************************
public:
    sector_c(): vischeck(0) {}
};

/*****************************************************************************\
                              END of sector.h
\*****************************************************************************/
#endif
