#ifndef __GN_GFX_MISC_H__
#define __GN_GFX_MISC_H__
// *****************************************************************************
/// \file
/// \brief   misc. utilities
/// \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{

    ///
    /// load texture from image file
    ///
    Texture * loadTextureFromFile( Gpu & gpu, const char * filename );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_MISC_H__
