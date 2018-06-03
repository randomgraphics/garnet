# ifndef __EXAMPLES_portaldemo_clipfrustum_H
# define __EXAMPLES_portaldemo_clipfrustum_H
/*****************************************************************************\
  filename          : clipfrustum.h
  create time       : 2002/10/18
  created by        : chenlee
  description       :
\*****************************************************************************/

/************************* 修改记录 (Modification Log) ***********************\

  =======================================
  2002/10/18 by chenlee
  =======================================
  * 创建

\*****************************************************************************/

# include "garnet/types/types.h"
# include "math.h"

class  camera_c;
struct render_i;
class clipfrustum_c
{
public :
    enum
    {
        NEAR_PLANE = 0,
        FAR_PLANE  = 1,
        MAX_PLANES = 32,
    };

    // number of clip planes
    size_t  num_planes;
    // clip plane array
    plane3_c planes[MAX_PLANES];
    // corner vertices of far/near planes
    vec3_c  far_verts[ MAX_PLANES - 2 ];
    vec3_c  near_verts[ MAX_PLANES - 2 ];
    bool    use_near_verts;
    // eye position
    vec3_c  eye_point;
    // eye direction
    vec3_c  eye_direction;

    // ********************************
    //  public operations
    // ********************************
public :

    // construct clipfrustum from a project matrix and camera frame
    void from_projview( const matrix44_c & proj,
                        const matrix44_c & view );

    // clip current clipfrustum by a polygon, write result to 'result'.
    //
    // verts, numvert : 多边形顶点列表
    // plane          : 多边形所在的平面
    //
    void clip_by_polygon( clipfrustum_c & result,
                          const vec3_c * verts, size_t numvert,
                          const plane3_c & plane ) const;

    // check visibility of a polygon
    // param :
    //   p : polygon所在的平面
    // return value :
    //   1 : fully visible,
    //   0 : partially visible,
    //  -1 : fully invisible
    //
    int check_poly_visibility( const vec3_c *, size_t, const plane3_c & ) const;

    // draw this frustum onto screen ( just for testing )
    void draw();

    // ********************************
    //  constructor/destructor
    // ********************************
public :
};

/*****************************************************************************\
                              END of clipfrustum.h
\*****************************************************************************/
# endif
