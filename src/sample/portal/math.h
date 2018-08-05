# ifndef __EXAMPLES_portaldemo_math_H
# define __EXAMPLES_portaldemo_math_H
/*****************************************************************************\
  filename          : math.h
  create time       : 2002/10/25
  created by        : chenlee
  description       : some math routine
\*****************************************************************************/

/************************* 修改记录 (Modification Log) ***********************\

  =======================================
  2002/10/25 by chenlee
  =======================================
  * 创建

\*****************************************************************************/

#include <garnet/GNbase.h>
#include <vector>

// NOTE : 为了避免由于浮点计算引起的不确定性，在下面的计算函数中了引入了平
//        面厚度的概念。 缺省的厚度(0.1)大约可以保证尺度在1,0000个单位左右
//        的诸如剪裁等与平面相关计算的精度。假定单位1代表1米的话，这个误差
//        值就代表1pm，而有效的最大计算尺度就是10km。 用户可以根据实际的需
//        要自行调整这个值。

# define PLANE_THICKNESS      0.1f
# define PLANE_HALF_THICKNESS 0.05f

namespace GN {

//
// 判定一个点与本平面的位置关系
// ============================
//
//  -1 : 点在面的背面
//   0 : 点在面上
//   1 : 点在面的正面
//
int classify( const Plane3f &, const Vector3f & );

//
// 得到平面和线段的交点
// ====================
//
// 如果有交点，返回true；否则，返回false。
//
// NOTE : 1. 如果返回值是，则ipoint中的值是不确定的；
//        2. 如果起点正好落在plane上，本函数认为不相交；
//           ....终点...........................相交。
//
bool intersection_plane_segment( Vector3f & ipoint,
                                 const Plane3f & plane,
                                 const Vector3f & start,
                                 const Vector3f & end );

//
// 得到平面和射线的交点
// ====================
//
// 如果有交点，返回true；否则，返回false。
//
// NOTE : 1. 如果返回值是，则ipoint中的值是不确定的；
//        2. 如果起点正好落在plane上，则本函数认为不相交；
//
bool intersection_plane_ray( Vector3f & ipoint,
                                       const Plane3f & plane,
                                       const Vector3f & start,
                                       const Vector3f & direction );

// 除了返回交点以外，还返回交点所对应的ratio值，
// ratio越大，说明交点离start越远。
bool intersection_plane_ray( Vector3f & ipoint,
                             float  & ratio,
                             const Plane3f & plane,
                             const Vector3f & start,
                             const Vector3f & direction );

//
// 得到平面和直线的交点
// ====================
//
// 如果有交点，返回true；否则，返回false。
//
// NOTE : 1. 如果返回值是，则ipoint中的值是不确定的；
//
bool intersection_plane_line( Vector3f & ipoint,
                              const Plane3f & plane,
                              const Vector3f & point,
                              const Vector3f & direction );

//
// 平面多边形和线段的交点
// ======================
//
// return false if p and (end-start) is parallel, else return true
// NOTE : 'intersection' is undefined when parallel.
// ----------------------------------------------------------------------------
bool intersection_polygon_segment( Vector3f & intersection,    // 返回交点
                                   const Plane3f & p,        // 多边形所在平面
                                   const Vector3f  * verts,    // 多边形顶点列表
                                   size_t        numverts,   // 多边形顶点数目
                                   const Vector3f  & start,    // 线段起点
                                   const Vector3f  & end );    // 线段终点


//
// 平面多边形和射线的交点
// ======================
//
// return false if p and (end-start) is parallel, else return true
// NOTE : 'intersection' is undefined when parallel.
// ----------------------------------------------------------------------------
bool intersection_polygon_ray( Vector3f & intersection,    // 返回交点
                               const Plane3f & p,        // 多边形所在平面
                               const Vector3f  * verts,    // 多边形顶点列表
                               size_t        numverts,   // 多边形顶点数目
                               const Vector3f  & start,    // 射线起点
                               const Vector3f  & dir );    // 射线方向

//
// 平面多边形和直线的交点
// ======================
//
// return false if p and (end-start) is parallel, else return true
// NOTE : 'intersection' is undefined when parallel.
// ----------------------------------------------------------------------------
bool intersection_polygon_line( Vector3f & intersection,    // 返回交点
                                const Plane3f & p,        // 多边形所在平面
                                const Vector3f  * verts,    // 多边形顶点列表
                                size_t        numverts,   // 多边形顶点数目
                                const Vector3f  & point,    // 直线上任意一点
                                const Vector3f  & dir );    // 直线的方向


//
// 用一组平面对一个多边形进行剪裁
// ==============================
//
void clip_polygon_with_plane_list( std::vector<Vector3f> & result,
                                   const Vector3f * verts, size_t numvert,
                                   const Plane3f * planes, size_t numplane );

# include "math.inl"

}

/*****************************************************************************\
                              END of math.h
\*****************************************************************************/
# endif
