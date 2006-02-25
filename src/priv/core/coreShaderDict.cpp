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

    // extract entry name out of shader name
    struct Local
    {
        static bool isAt( char ch ) { return '@' == ch; }
    };
    StrA entry;
    StrA path = name;
    path.trimRightUntil( &Local::isAt );
    if( path.empty() ) path = name; // handle case
    if( path.size() < name.size() )
    {
        entry = name.subString( path.size(), name.size() );
        path.trimRight( '@' );
    }

    // get resouce path
    path = core::searchResource( path );
    if( path.empty() )
    {
        GN_ERROR( "Shader '%s' creation failed: path not found.", name.cstr() );
        return false;
    }

    GN_INFO( "Load shader '%s' from file '%s'.", name.cstr(), path.cstr() ); 

    // open file
    AnsiFile fp;
    if( !fp.open( path::resolve(path), "rt" ) )
    {
        GN_ERROR( "Shader '%s' creation failed: can't open file '%s'.", name.cstr(), path.cstr() );
        return false;
    }

    AutoObjArray<char> buf( new char[fp.size()+1] );
    AutoObjArray<char> typeStr( new char[fp.size()+1] );
    AutoObjArray<char> langStr( new char[fp.size()+1] );
    AutoObjArray<char> entryStr( new char[fp.size()+1] );

    // read file
	memset( buf.get(), 0, fp.size()+1 );
    if( 0 == fp.read( buf.get(), fp.size() ) )
    {
        GN_ERROR( "Shader '%s' creation failed: can't read file '%s'.", name.cstr(), path.cstr() );
        return false;
    }
	fp.close();

    const char * code = buf.get();
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
        3 == ::sscanf( code, "// TYPE=%s LANG=%s ENTRY=%s", typeStr.get(), langStr.get(), entryStr.get() ) ||
        3 == ::sscanf( code, "# TYPE=%s LANG=%s ENTRY=%s", typeStr.get(), langStr.get(), entryStr.get() ) )
    {
        if( !str2ShaderType( type, typeStr.get() ) )
        {
            GN_ERROR( "Shader '%s' creation failed: invalid shader type '%s'.", name.cstr(), typeStr.get() );
            return false;
        }
        if( !str2ShadingLanguage( lang, langStr.get() ) )
        {
            GN_ERROR( "Shader '%s' creation failed: invalid shading language '%s'.", name.cstr(), langStr.get() );
            return false;
        }
    }
    else
    {
        GN_ERROR( "Shader '%s' creation failed: unknown/invalid shader header.", name.cstr() );
        return false;
    }

    // create shader instance
    result = gRenderer.createShader( type, lang, code, entry.empty() ? entryStr.get() : entry.cstr() );
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

    // create the dictionary instance
    mDict = new ShaderDictionary;

    // register functors
    mDict->setCreator( &sCreateShader );
    mDict->setDeletor( &sDeleteShader );
    mDict->setNullor( &sCreateNullShader );

    // connect to renderer signals
    Renderer::sSigDeviceDestroy.connect( mDict, &ShaderDictionary::disposeAll );

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

    if( mDict )
    {
        Renderer::sSigDeviceDestroy.disconnect( mDict );
        delete mDict;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
