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

// NOTE : 为了避免由于浮点计算引起的不确定性，在下面的计算函数中了引入了平
//        面厚度的概念。 缺省的厚度(0.1)大约可以保证尺度在1,0000个单位左右
//        的诸如剪裁等与平面相关计算的精度。假定单位1代表1米的话，这个误差
//        值就代表1pm，而有效的最大计算尺度就是10km。 用户可以根据实际的需
//        要自行调整这个值。

# define PLANE_THICKNESS      0.1f
# define PLANE_HALF_THICKNESS 0.05f

//
// 判定一个点与本平面的位置关系
// ============================
//
//  -1 : 点在面的背面
//   0 : 点在面上
//   1 : 点在面的正面
//
int classify( const plane3_c &, const vec3_c & );

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
bool intersection_plane_segment( vec3_c & ipoint,
                                 const plane3_c & plane,
                                 const vec3_c & start,
                                 const vec3_c & end );

//
// 得到平面和射线的交点
// ====================
//
// 如果有交点，返回true；否则，返回false。
//
// NOTE : 1. 如果返回值是，则ipoint中的值是不确定的；
//        2. 如果起点正好落在plane上，则本函数认为不相交；
//
bool intersection_plane_ray( vec3_c & ipoint,
                                       const plane3_c & plane,
                                       const vec3_c & start,
                                       const vec3_c & direction );

// 除了返回交点以外，还返回交点所对应的ratio值，
// ratio越大，说明交点离start越远。
bool intersection_plane_ray( vec3_c & ipoint,
                             float  & ratio,
                             const plane3_c & plane,
                             const vec3_c & start,
                             const vec3_c & direction );

//
// 得到平面和直线的交点
// ====================
//
// 如果有交点，返回true；否则，返回false。
//
// NOTE : 1. 如果返回值是，则ipoint中的值是不确定的；
//
bool intersection_plane_line( vec3_c & ipoint,
                              const plane3_c & plane,
                              const vec3_c & point,
                              const vec3_c & direction );

//
// 平面多边形和线段的交点
// ======================
//
// return false if p and (end-start) is parallel, else return true
// NOTE : 'intersection' is undefined when parallel.
// ----------------------------------------------------------------------------
bool intersection_polygon_segment( vec3_c & intersection,    // 返回交点
                                   const plane3_c & p,        // 多边形所在平面
                                   const vec3_c  * verts,    // 多边形顶点列表
                                   size_t        numverts,   // 多边形顶点数目
                                   const vec3_c  & start,    // 线段起点
                                   const vec3_c  & end );    // 线段终点


//
// 平面多边形和射线的交点
// ======================
//
// return false if p and (end-start) is parallel, else return true
// NOTE : 'intersection' is undefined when parallel.
// ----------------------------------------------------------------------------
bool intersection_polygon_ray( vec3_c & intersection,    // 返回交点
                               const plane3_c & p,        // 多边形所在平面
                               const vec3_c  * verts,    // 多边形顶点列表
                               size_t        numverts,   // 多边形顶点数目
                               const vec3_c  & start,    // 射线起点
                               const vec3_c  & dir );    // 射线方向

//
// 平面多边形和直线的交点
// ======================
//
// return false if p and (end-start) is parallel, else return true
// NOTE : 'intersection' is undefined when parallel.
// ----------------------------------------------------------------------------
bool intersection_polygon_line( vec3_c & intersection,    // 返回交点
                                const plane3_c & p,        // 多边形所在平面
                                const vec3_c  * verts,    // 多边形顶点列表
                                size_t        numverts,   // 多边形顶点数目
                                const vec3_c  & point,    // 直线上任意一点
                                const vec3_c  & dir );    // 直线的方向


//
// 用一组平面对一个多边形进行剪裁
// ==============================
//
void clip_polygon_with_plane_list( std::vector<vec3_c> & result,
                                   const vec3_c * verts, size_t numvert,
                                   const plane3_c * planes, size_t numplane );

# include "math.inl"

/*****************************************************************************\
                              END of math.h
\*****************************************************************************/
# endif
