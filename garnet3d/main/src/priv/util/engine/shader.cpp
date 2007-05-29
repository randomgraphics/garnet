#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.Shader");

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::loadShaderFromFile(
    RenderEngine       & eng,
    gfx::ShaderType      type,
    gfx::ShadingLanguage lang,
    const StrA         & hints,
    const StrA         & filename )
{
    GN_GUARD;

    GN_INFO(sLogger)( "Load %s", filename.cptr() );

    AutoObjPtr<File> fp( core::openFile( filename, "rt" ) );
    if( 0 == fp ) return 0;

    DynaArray<char> buf( fp->size() );
    if( !fp->read( buf.cptr(), fp->size(), 0 ) ) return 0;

    return eng.createShader( filename, type, lang, buf.cptr(), hints );

    GN_UNGUARD;
}
