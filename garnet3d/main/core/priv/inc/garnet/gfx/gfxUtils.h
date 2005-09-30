#ifndef __GN_GFX_GFXUTILS_H__
#define __GN_GFX_GFXUTILS_H__
// *****************************************************************************
//! \file    gfxUtils.h
//! \brief   Util functions of GFX module
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN
{
    namespace gfx
    {
        //!
        //! \name convert between shader usage tags to string
        //@{
        inline bool         shaderUsage2Str( StrA &, ShaderUsage );
        inline const char * shaderUsage2Str( ShaderUsage );
        inline bool         str2ShaderUsage( ShaderUsage &, const char * );
        inline bool         shadingLanguage2Str( StrA &, ShadingLanguage );
        inline const char * shadingLanguage2Str( ShadingLanguage );
        inline bool         str2ShadingLanguage( ShadingLanguage &, const char * );
        //@}

        //! \name convert between texture tags and string
        //@{
        inline bool         texType2Str( StrA &, TexType );
        inline const char * texType2Str( TexType );
        inline bool         str2TexType( TexType &, const char * );
        inline bool         texFilter2Str( StrA &, TexFilter );
        inline const char * texFilter2Str( TexFilter );
        inline bool         str2TexFilter( TexFilter &, const char * );
        inline bool         texWrap2Str( StrA &, TexWrap );
        inline const char * texWrap2Str( TexWrap );
        inline bool         str2TexWrap( TexWrap &, const char * );
        //@}
    }
}

#include "gfxUtils.inl"

// *****************************************************************************
//                           End of gfxUtils.h
// *****************************************************************************
#endif // __GN_GFX_GFXUTILS_H__
