# ifndef __EXAMPLES_portaldemo_bsptree_H
# define __EXAMPLES_portaldemo_bsptree_H
/*****************************************************************************\
  filename          : bsptree.h
  create time       : 2002/10/24
  created by        : chenlee
  description       : implement a bsp tree
\*****************************************************************************/

/************************* 修改记录 (Modification Log) ***********************\

  =======================================
  2002/10/24 by chenlee
  =======================================
  * 创建

\*****************************************************************************/

# include "math.h"

using namespace GN;

struct lvlBspNode;
class bsptree_c
{
    // ********************************
    //  public data type
    // ********************************
public :
    struct bspnode_s
    {
        Plane3f    plane;     // node的平面
        int        parent;    // 父节点索引，-1表示根节点
        int        child[2];  // 前后子节点索引，-1表示空
    };
    typedef std::vector<bspnode_s> bspnodelist_c;

    // ********************************
    //  public operations
    // ********************************
public :

    // initialize from lvlBspNode list
    void initialize( lvlBspNode * root, uint32 numnodes );

    // check if the point is in solid space of the sector
    //   -1 : solid space
    //    0 : 正好落在边界上
    //    1 : else
    int in_solid( const Vector3f & v, int root_index = 0 ) const;

    //
    // 检查点v1和点v2间是否可见（或者说是否与bsptree有交点），并在cldinfo
    // 中返回交点信息。返回真，若v1、v2和bsptree有交点
    //
    // NOTE : 如果是v1在solid中，则无论v2在什么位置，返回值均为true，且：
    //          cldinfo_s::ipoint等于v1
    //
    //        如果v1-v2之间可见，则cldinfo_s中的内容将不会被改变。
    //
    struct cldinfo_s
    {
        Vector3f ipoint;   // 与bsp树的交点
        int    inodeidx; // 与bsp树的交平面的序号
    };
    bool check_segment( cldinfo_s * cldinfo,
                        const Vector3f & v1, const Vector3f & v2,
                        uint32 root_index = 0 ) const;

    // 检查射线start-direction是否与bsp树相交
    bool check_radial( cldinfo_s * cldinfo,
                       const Vector3f & start, const Vector3f & direction,
                       uint32 root_index = 0 ) const;

    // get bspnode list
    const bspnode_s * get_node_list() const
    {
        GN_ASSERT( !m_bsproot.empty() );
        return &m_bsproot[0];
    }

    // ********************************
    //  constructors/destructors
    // ********************************
public :
    bsptree_c()  {}
    ~bsptree_c() {}

    // ********************************
    //  private data
    // ********************************
private :
    bspnodelist_c m_bsproot;
};

/*****************************************************************************\
                              END of bsptree.h
\*****************************************************************************/
# endif
