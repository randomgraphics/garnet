#ifndef __GN_GFX_SHAPE_H__
#define __GN_GFX_SHAPE_H__
// *****************************************************************************
/// \file    gfx/shape.h
/// \brief   Generate mesh of simple shape (not implemented)
/// \author  chenlee (2006.1.7)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// Create a box with specified size and property.
    ///
    /// \param sx, sy, sz
    ///     Specify box size
    /// \param positions, posStride
    ///     Return box positions. Must be large enough to hold at least 24*3=72 floats.
    ///     Can _NOT_ be NULL.
    /// \param texcoords, texStride
    ///     Return texture coordinates. Must be large enough to hold at least 24*2=48 floats.
    ///     Can be NULL if you do not need it.
    /// \param tangents, tangStride
    ///     Return tangent vectors. Must be large enough to hold at least 24*3=72 floats.
    ///     Can be NULL if you do not need it.
    /// \param binormals, n2Stride
    ///     Return binormal coordinates. Must be large enough to hold at least 24*3=72 floats.
    ///     Can be NULL if you do not need it.
    /// \param normals, normStride
    ///     Return vertex normals. Must be large enough to hold at least 24*3=72 floats.
    ///     Can be NULL if you do not need normals.
    /// \param triList
    ///     Return indices of triangle list. Must be large enough to hold at least 12*3=36 indices.
    ///     Can be NULL if you do not need it.
    /// \param quadList
    ///     Return indices of quad list. Must be large enough to hold at least 6*4=24 indices.
    ///     Can be NULL if you do not need it.
    ///
    void createBox(
        float sx, float sy, float sz,
        float * positions, size_t posStride,
        float * texcoords, size_t texStride,
        float * normals, size_t normStride,
        float * tangents, size_t tangStride,
        float * binormals, size_t n2Stride,
        UInt16 * triList,
        UInt16 * quadList );
}}

// *****************************************************************************
//                           End of shape.h
// *****************************************************************************
#endif // __GN_GFX_SHAPE_H__
