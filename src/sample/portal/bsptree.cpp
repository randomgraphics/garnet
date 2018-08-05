# include "pch.h"
# include "bsptree.h"
# include "lvlFileStruct.h"

//
//
// ----------------------------------------------------------------------------
void bsptree_c::initialize( lvlBspNode * root, uint32 numnodes )
{
    GN_ASSERT( root && numnodes );
    m_bsproot.resize( numnodes );
    bspnodelist_c::iterator i = m_bsproot.begin(), e = m_bsproot.end();
    for ( ; i != e; ++i, ++root )
    {
        i->plane    = root->plane;
        i->parent   = root->parent;
        i->child[0] = root->child[0];
        i->child[1] = root->child[1];
    }
}


//
//
// ----------------------------------------------------------------------------
int bsptree_c::in_solid( const Vector3f & v, int root_index ) const
{
    GN_ASSERT( root_index >= 0 );

    int side = -1;
    int nid = root_index;
    const bspnode_s * n;
    while ( nid >= 0 )
    {
        n = &m_bsproot[nid];

        // get side of point respect to current node
        side = classify( n->plane, v );

        if ( 1 == side )
            nid = n->child[0];
        else if ( -1 == side )
            nid = n->child[1];
        else
        {
            // v 正好落在当前剪裁平面上，需要同时考虑平面两边的情况
            int s1 = n->child[0] > 0 ? in_solid( v, n->child[0] ) : 1;
            int s2 = n->child[1] > 0 ? in_solid( v, n->child[1] ) : -1;
            int ss = s1 + s2;
            if (  2 == ss ) return  1;
            else if ( -2 == ss ) return -1;
            else return 0;
        }
    }

    // got it
    return side;
}


//
// 使用当前node Plane clip the start&end vec.
// 1 : 如果他们在切分面的同侧：
//   a : 如果他们在切分面的后面：
//     i  : 如果后面是solid area,return FALSE;
//     ii : 如果后面不是solid area,递归当前的算法。
//   b : 如果他们在切分面的前面或是共面：
//     i  : 如果前面是树叶，return TRUE;
//     ii : 如果前面不是树叶，递归当前的算法。
//
// 2 : 如果start在面的前面，end在面的后面 : 得到star-end与node的交点'i'
//   a : 如果切分面的后面是solid area, return false.
//   b : 如果a不成立，且切分面的前面是leaf area, 递归调用 :
//       check_segment( ..., end, i, child[1] );
//   c : 如果a,b均不成立，说明切分面的前后都是子空间，则递归调用 :
//       check_segment( ..., start, i, child[0] )&&
//       check_segment( ..., end, i, child[1] );
//
// 3:  如果end在面的前面，start在面的后面：同2类似。
//
// 返回值：
//
//   如果可见，返回TRUE,
//   如果不可见，返回FALSE.
//
// ----------------------------------------------------------------------------
bool bsptree_c::check_segment( cldinfo_s * cinfo,
                               const Vector3f & start, const Vector3f & end,
                               uint32 root_index ) const
{
    GN_GUARD_SLOW;

    GN_ASSERT( !m_bsproot.empty() && root_index >= 0 );

    const bspnode_s & root = m_bsproot[root_index];

    // get side of start/end point respect to current root node
    int ss = classify( root.plane, start );
    int se = classify( root.plane, end );

    if ( 1 == ss )
    {
        if ( -1 == se ) // start在前面, end在后面
        {
            if ( root.child[1] < 0 )
            {
                // end point is in solid space, so it must be invisible
                if ( cinfo )
                {
                    GN_VERIFY( intersection_plane_segment(
                        cinfo->ipoint, root.plane, start, end) );
                    cinfo->inodeidx = root_index;

                    if ( root.child[0] > 0 )
                    {
                        // start is in sub-space so we need to check it to
                        // get the correct collide information
                        Vector3f i = cinfo->ipoint;
                        check_segment( cinfo, start, i, root.child[0] );
                    }
                }
                return true;
            }
            else if ( root.child[0] < 0 )
            {
                // start point is in leaf root, but end point need further
                // check, so :
                Vector3f i;
                GN_VERIFY( intersection_plane_segment(i, root.plane, start, end) );
                return check_segment( cinfo, i, end, root.child[1] );
            }
            else
            {
                GN_ASSERT( root.child[0] > 0 && root.child[1] > 0 );
                // check sub nodes for visibility
                Vector3f i;
                GN_VERIFY( intersection_plane_segment(i, root.plane, start, end) );
                return check_segment( cinfo, start, i, root.child[0] )
                    || check_segment( cinfo, i, end, root.child[1] );
            }
        }
        else // start在前, end在前/上
        {
            if ( root.child[0] > 0 )
            {
                return check_segment( cinfo, start, end, root.child[0] );
            }
            else
            {
                GN_ASSERT( root.child[0] < 0 );
                // leaf root, must no collision
                return false;
            }
        }
    }
    else if ( -1 == ss )
    {
        if ( 1 == se ) // start在后面, end在前面
        {
            if ( root.child[1] < 0 )
            {
                // start point is in solid space, so it must be invisible
                if ( cinfo )
                {
                    cinfo->ipoint = start;
                    cinfo->inodeidx = root.parent;
                }
                return true;
            }
            else if ( root.child[0] < 0 )
            {
                // end point is in leaf root, but start point need further
                // check, so :
                Vector3f i;
                GN_VERIFY( intersection_plane_segment(i, root.plane, start, end) );
                return check_segment( cinfo, start, i, root.child[1] );
            }
            else
            {
                // check sub nodes for collision
                Vector3f i;
                GN_VERIFY( intersection_plane_segment(i, root.plane, start, end) );
                return check_segment( cinfo, start, i, root.child[1] )
                    || check_segment( cinfo, i, end, root.child[0] );
            }
        }
        else
        {
            // start在后面, end在上面/后面
            if ( root.child[1] >= 0 )
            {
                return check_segment( cinfo, start, end, root.child[1] );
            }
            else
            {
                // start and end are both in solid space, must be invisible
                if ( cinfo )
                {
                    cinfo->ipoint = start;
                    cinfo->inodeidx = root.parent;
                }
                return true;
            }
        }
    }
    else // start恰好落在面上
    {
        if ( se == -1 )
        {
            if ( root.child[1] < 0 )
            {
                // end point is in solid space, must be invisible
                if ( cinfo )
                {
                    cinfo->ipoint = start;
                    cinfo->inodeidx = root_index;
                }
                return true;
            }
            else // recusive check sub-space
                return check_segment( cinfo, start, end, root.child[1] );
        }
        else if ( se == 1 )
        {
            if ( root.child[0] < 0 )
            {
                // end point is in leaf space, must be no collision
                return false;
            }
            else // recusive check sub-space
                return check_segment( cinfo, start, end, root.child[0] );
        }
        else
        {
            // start和end都落在同一个平面上，此时需要同时考虑平面两边的情况
            bool front, back;
            if ( root.child[0] > 0 )
            {
                front = check_segment( cinfo, start, end, root.child[0] );
            }
            else front = false;
            if ( root.child[1] > 0 )
            {
                back = check_segment( cinfo, start, end, root.child[1] );
            }
            else back = false;
            return front || back;
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
bool bsptree_c::check_radial( cldinfo_s * /*cinfo*/,
                              const Vector3f & /*start*/,
                              const Vector3f & /*dir*/,
                              uint32 root_index ) const
{
    GN_GUARD_SLOW;


    GN_ASSERT( !m_bsproot.empty() && root_index >= 0 );
/*
    const bspnode_s & root = m_bsproot[root_index];

    // get side of start/direction respect to current root node
    int   ss = classify( root.plane, start );
    float sd = root.plane.n * dir;

    if ( 1 == ss )
    {
        if ( sd > 0.0f ) // start在前，dir与面同向
        {
            if ( root.child[0] > 0 )
            {
                // start is in sub-space, need further check
                return check_radial( cinfo, start, dir, root.child[1] );
            }
            else
            {
                // start is in leaf space, 必定无交点
                return false;
            }
        }
        else if ( sd < 0.0f ) // start在前，dir与面反向
        {
            // get intersection point with current code
            Vector3f i;
            GN_VERIFY( intersection_plane_ray( i, root.plane, start, dir ) );

            if ( root.child[0] > 0 )
            {
                // 检查线段start-i与前子空间是否有交点
                cldinfo_s ci;
                if ( check_segment(ci, start, i, root.child[0]) )
                {
                    if ( cinfo ) *cinfo = ci;
                    return true;
                }
            }

            // NOTE : 程序执行到此处，说明交点必不在前半空间中

            if ( root.child[1] > 0 )
            {
                // 检查射线i-dir与后子空间是否有交点
                cldinfo_s ci;
                if ( check_radial(ci, i, dir, root.child[1]) )
                {
                    if ( cinfo ) *cinfo = ci;
                    return true;
                }
            }

            // NOTE : 程序执行到此处，说明交点也不在后半空间中


        }
        else
        {
        }
    } //*/

    return false;

    GN_UNGUARD_SLOW;
}
