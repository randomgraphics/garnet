#ifndef __GN_GFX_XENONRESOURCE_H__
#define __GN_GFX_XENONRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Base xenon graphics resource class
/// \author  chenlee (2005.10.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    class XenonRenderer;

    ///
    /// General Xenon resource class
    ///
    class XenonResource
    {
    public:

        ///
        /// Get the renderer that the resource belongs to.
        ///
        XenonRenderer & getRenderer() const { return mRenderer; }

    protected :

        XenonResource( XenonRenderer & ); ///< ctor
        virtual ~XenonResource();         ///< dtor

    private:

        XenonRenderer & mRenderer;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONRESOURCE_H__
