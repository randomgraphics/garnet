#ifndef __GN_GFX_OGLRESOURCE_H__
#define __GN_GFX_OGLRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief
/// \author  chenlee (2005.10.2)
// *****************************************************************************

namespace GN {
namespace gfx {
class OGLGpu; // forware declaration of OGL GPU.

///
/// General OGL resource class
///
class OGLResource {
protected:
    //@{
    OGLResource(OGLGpu &);
    virtual ~OGLResource();
    //@}

    ///
    /// Get renderer that the resource belongs to
    ///
    OGLGpu & getGpu() const { return mGpu; }

private:
    OGLGpu & mGpu;
};

} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_OGLRESOURCE_H__
