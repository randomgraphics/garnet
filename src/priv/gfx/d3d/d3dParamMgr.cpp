#include "pch.h"
#include "d3dRenderer.h"

//
// matrix computation
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::gfx::D3DRenderer::composePerspectiveMatrix(
    Matrix44f & result,
    float fovy,
    float ratio,
    float znear,
    float zfar ) const
{
    return result.perspectiveD3D( fovy, ratio, znear, zfar );
}
//
GN::Matrix44f &
GN::gfx::D3DRenderer::composeOrthoMatrix(
    Matrix44f & result,
    float left,
    float bottom,
    float width,
    float height,
    float znear,
    float zfar ) const
{
    return result.orthoD3D(
        left,
        left+width,
        bottom,
        bottom+height,
        znear,
        zfar );
}
