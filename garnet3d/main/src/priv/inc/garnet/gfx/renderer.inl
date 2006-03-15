// *****************************************************************************
// caps functions
// *****************************************************************************

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createVtxShader( ShadingLanguage lang,
                               const StrA &    code,
                               const StrA &    entry )
    {
        GN_GUARD;
        return createShader( VERTEX_SHADER, lang, code, entry );
        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createPxlShader( ShadingLanguage lang,
                               const StrA &    code,
                               const StrA &    entry )
    {
        GN_GUARD;
        return createShader( PIXEL_SHADER, lang, code, entry );
        GN_UNGUARD;
    }
}}
