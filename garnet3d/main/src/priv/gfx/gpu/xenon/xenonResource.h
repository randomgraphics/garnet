#ifndef __GN_GFX_XENONRESOURCE_H__
#define __GN_GFX_XENONRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Base xenon graphics resource class
/// \author  chenlee (2005.10.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    class XenonGpu;

    ///
    /// General Xenon resource class
    ///
    class XenonResource
    {
    public:

        ///
        /// Get the renderer that the resource belongs to.
        ///
        XenonGpu & getGpu() const { return mGpu; }

    protected :

        XenonResource( XenonGpu & ); ///< ctor
        virtual ~XenonResource();         ///< dtor

    private:

        XenonGpu & mGpu;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONRESOURCE_H__
