#include "pch.h"
#include "coreResourceDict.h"

// *****************************************************************************
// local functions
// *****************************************************************************

using namespace GN;
using namespace GN::gfx;

//
// Create a 1x1 pure blue shader.
// -----------------------------------------------------------------------------
static bool sCreateNullShader( Shader * & result, const StrA &, void * )
{
    result = NULL;
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool sCheckPrefix( const char * str, const char * prefix )
{
    GN_ASSERT( str && prefix );
    while( *str && *prefix )
    {
        if( *str != *prefix ) break;
        ++str;
        ++prefix;
    }
    return ( 0 == *prefix );
}

//
//
// -----------------------------------------------------------------------------
static bool sCreateShader( Shader * & result, const StrA & name, void * )
{
    GN_GUARD;

    // check for global renderer
    if( 0 == gRendererPtr )
    {
        GN_ERROR( "Shader '%s' creation failed: renderer is not ready." );
        return false;
    }

    // extract hints out of shader name
    struct Local
    {
        static bool isAt( char ch ) { return '@' == ch; }
    };
    StrA hints;
    StrA path = name;
    path.trimRightUntil( &Local::isAt );
    if( path.empty() ) path = name; // handle case
    if( path.size() < name.size() )
    {
        hints = name.subString( path.size(), name.size() );
        path.trimRight( '@' );
    }

    // get resouce path
    path = core::searchResourceFile( path );
    if( path.empty() )
    {
        GN_ERROR( "Shader '%s' creation failed: path not found.", name.cptr() );
        return false;
    }

    GN_INFO( "Load shader '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    AnsiFile fp;
    if( !fp.open( path::toNative(path), "rt" ) )
    {
        GN_ERROR( "Shader '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
        return false;
    }

    AutoObjArray<char> buf( new char[fp.size()+1] );
    AutoObjArray<char> typeStr( new char[fp.size()+1] );
    AutoObjArray<char> langStr( new char[fp.size()+1] );

    // read file
	memset( buf, 0, fp.size()+1 );
    if( 0 == fp.read( buf, fp.size() ) )
    {
        GN_ERROR( "Shader '%s' creation failed: can't read file '%s'.", name.cptr(), path.cptr() );
        return false;
    }
	fp.close();

    const char * code = buf;
    ShaderType type;
    ShadingLanguage lang;
    if( sCheckPrefix( code, "!!ARBvp" ) )
    {
        type = VERTEX_SHADER;
        lang = LANG_OGL_ARB;
    }
    else if( sCheckPrefix( code, "!!ARBfp" ) )
    {
        type = PIXEL_SHADER;
        lang = LANG_OGL_ARB;
    }
    else if(
        2 == ::sscanf( code, "// type=%s lang=%s", typeStr.get(), langStr.get() ) ||
        2 == ::sscanf( code, "# type=%s lang=%s", typeStr.get(), langStr.get() ) )
    {
        if( !str2ShaderType( type, typeStr ) )
        {
            GN_ERROR( "Shader '%s' creation failed: invalid shader type '%s'.", name.cptr(), typeStr.get() );
            return false;
        }
        if( !str2ShadingLanguage( lang, langStr ) )
        {
            GN_ERROR( "Shader '%s' creation failed: invalid shading language '%s'.", name.cptr(), langStr.get() );
            return false;
        }
    }
    else
    {
        GN_ERROR( "Shader '%s' creation failed: unknown/invalid shader header.", name.cptr() );
        return false;
    }

    // create shader instance
    result = gRenderer.createShader( type, lang, code, hints );
    return NULL != result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteShader( Shader * & ptr, void * )
{
    GN::safeDecref( ptr );
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool core::CoreShaderDict::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( core::CoreShaderDict, () );

    // register functors
    mDict.setCreator( &sCreateShader );
    mDict.setDeletor( &sDeleteShader );
    mDict.setNullor( &sCreateNullShader );

    // connect to renderer signals
    Renderer::sSigDestroy.connect( mDict, &ShaderDictionary::disposeAll );

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void core::CoreShaderDict::quit()
{
    GN_GUARD;

    Renderer::sSigDestroy.disconnect( mDict );
    mDict.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
