namespace GN { namespace gfx
{
    // *************************************************************************
    // resource management methods
    // *************************************************************************

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createShaderFromFile( ShaderType type,
                                    ShadingLanguage lang,
                                    const StrA & filename,
                                    const StrA & hints )
    {
        GN_GUARD;

        // open file
        AutoObjPtr<File> fp( core::openFile( filename , "rt" ) );
        if( !fp ) return 0;

        // read file content
        std::vector<char> code;
        code.resize( fp->size() + 1 );
        if( !fp->read( &code[0], fp->size(), 0 ) ) return false;

        // create shader
        return createShader( type, lang, &code[0], hints );
        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createVS( ShadingLanguage lang,
                        const StrA &    code,
                        const StrA &    hints )
    {
        GN_GUARD;
        return createShader( SHADER_VS, lang, code, hints );
        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    inline Shader *
    Renderer::createPS( ShadingLanguage lang,
                        const StrA &    code,
                        const StrA &    hints )
    {
        GN_GUARD;
        return createShader( SHADER_PS, lang, code, hints );
        GN_UNGUARD;
    }
}}
