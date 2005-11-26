#include "pch.h"
#include "oglRenderer.h"

//
// matrix computation
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::gfx::OGLRenderer::composePerspectiveMatrix(
    Matrix44f & result,
    float fovy,
    float ratio,
    float znear,
    float zfar ) const
{
    return result.perspectiveOGL( fovy, ratio, znear, zfar );
}
//
GN::Matrix44f &
GN::gfx::OGLRenderer::composeOrthoMatrix(
    Matrix44f & result,
    float left,
    float bottom,
    float width,
    float height,
    float znear,
    float zfar ) const
{
    return result.orthoOGL(
        left,
        left+width,
        bottom,
        bottom+height,
        znear,
        zfar );
}
