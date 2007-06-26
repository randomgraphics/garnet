#ifndef __GN_ENGINE_SHADER_H__
#define __GN_ENGINE_SHADER_H__
// *****************************************************************************
/// \file
/// \brief   shader utilities
/// \author  chenli@@FAREAST (2007.5.29)
// *****************************************************************************

namespace GN { namespace engine
{
    //@{

    GraphicsResource * loadShaderFromFile(
        RenderEngine       & eng,
        gfx::ShaderType      type,
        gfx::ShadingLanguage lang,
        const StrA         & hints,
        const StrA         & filename );

    //@}
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_SHADER_H__
