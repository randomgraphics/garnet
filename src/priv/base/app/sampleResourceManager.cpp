#include "pch.h"
#include "garnet/GNapp.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

//
//
// -----------------------------------------------------------------------------
static GN::StrA sSearchResourceFile( const StrA & name )
{
    GN_GUARD;

    if( path::isFile( name ) ) return name;

    if( path::isAbsPath(name) ) return StrA::EMPTYSTR;

    StrA fullPath;

#define CHECK_PATH( X ) do { fullPath = path::join X ; if( path::isFile( fullPath ) ) return fullPath; } while(0)

    // search in startup directory
    CHECK_PATH( ("startup:", name) );
    CHECK_PATH( ("startup:/media", name) );

    // search in application directory
    CHECK_PATH( ("app:", name ) );
    CHECK_PATH( ( "app:/media", name ) );

    // search in garnet binary tree.
    CHECK_PATH( ( getEnv("GARNET_ROOT"), "bin/media", name ) );

    // hack for special cases
    CHECK_PATH( ( "app:../../../../../../media", name ) ); // for temporary build binaries

    // resource not found.
    return StrA::EMPTYSTR;

    GN_UNGUARD;
}

// *****************************************************************************
// local raw data loader
// *****************************************************************************

struct SampleRawData : public RawData
{
    std::vector<uint8_t> buffer;
    SampleRawData( size_t sz ) : buffer(sz) {}
    virtual size_t size() const { GN_ASSERT(!buffer.empty()); return buffer.size(); }
    virtual void * data() const { GN_ASSERT(!buffer.empty()); return (void*)&buffer[0]; }
};

//
// 
// -----------------------------------------------------------------------------
static bool sCreateNullRawData( RawData * & result, const StrA &, void * )
{
    GN_GUARD;
    SampleRawData * r = new SampleRawData(1);
    r->buffer[0] = 0;
    result = r;
    return true;
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sCreateRawData( RawData * & result, const StrA & name, void * )
{
    GN_GUARD;

    // get resource path
    StrA path = sSearchResourceFile( name );
    if( path.empty() )
    {
        GN_ERROR( "Raw resource '%s' creation failed: path not found.", name.cptr() );
        return false;
    }

    GN_INFO( "Load raw resource '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    AnsiFile fp;
    if( !fp.open( path::toNative(path), "rb" ) )
    {
        GN_ERROR( "Raw resource '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
        return false;
    }

    // read file content
    SampleRawData * r = new SampleRawData( fp.size() );
    if( 0 == fp.size() )
    {
        r = new SampleRawData(1);
        r->buffer[0] = 0;
    }
    else
    {
        r = new SampleRawData( fp.size() );
        if( fp.size() != fp.read( &r->buffer[0], fp.size() ) )
        {
            GN_ERROR( "Raw resource '%s' creation failed: read file '%s' error.", name.cptr(), path.cptr() );
            delete r;
            return false;
        }
    }

    // success
    result = r;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteRawData( RawData * & ptr, void * )
{
    GN::safeDelete( ptr );
}

// *****************************************************************************
// local shder loader
// *****************************************************************************

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
    path = sSearchResourceFile( path );
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
// local texture loader
// *****************************************************************************

//
// Create a 1x1 pure blue texture.
// -----------------------------------------------------------------------------
static bool sCreateNullTexture( Texture * & result, const StrA & name, void * )
{
    GN_GUARD;

    // check for global renderer
    if( 0 == gRendererPtr )
    {
        GN_ERROR( "Null texture '%s' creation failed: renderer is not ready.", name.cptr() );
        return false;
    }

    AutoRef<Texture> tex( gRenderer.create1DTexture( 1 ) );
    uint32_t * texData = (uint32_t*)tex->lock1D( 0, 0, 0, GN::gfx::LOCK_WO );
    if( 0 == texData ) return false;
    //           A R G B
    *texData = 0xFF0000FF;
    tex->unlock();

    // success
    result = tex.detach();
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sCreateTexture( Texture * & result, const StrA & name, void * )
{
    GN_GUARD;

    // check for global renderer
    if( 0 == gRendererPtr )
    {
        GN_ERROR( "Texture '%s' creation failed: renderer is not ready." );
        return false;
    }

    // get resource path
    StrA path = sSearchResourceFile( name );
    if( path.empty() )
    {
        GN_ERROR( "Texture '%s' creation failed: path not found.", name.cptr() );
        return false;
    }

    GN_INFO( "Load texture '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    AnsiFile fp;
    if( !fp.open( path::toNative(path), "rb" ) )
    {
        GN_ERROR( "Texture '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
        return false;
    }

    // create texture instance
    result = gRenderer.createTextureFromFile( fp );
    return NULL != result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteTexture( Texture * & ptr, void * )
{
    GN::safeDecref( ptr );
}

// *****************************************************************************
// SampleResourceManager
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::app::SampleResourceManager::SampleResourceManager()
{
    shaders.setCreator( &sCreateShader );
    shaders.setDeletor( &sDeleteShader );
    shaders.setNullor( &sCreateNullShader );

    textures.setCreator( &sCreateTexture );
    textures.setDeletor( &sDeleteTexture );
    textures.setNullor( &sCreateNullTexture );

    rawData.setCreator( &sCreateRawData );
    rawData.setDeletor( &sDeleteRawData );
    rawData.setNullor( &sCreateNullRawData );

    Renderer::sSigDispose.connect( this, &SampleResourceManager::onRendererDispose );
    Renderer::sSigDestroy.connect( this, &SampleResourceManager::onRendererDestroy );
}

//
//
// -----------------------------------------------------------------------------
GN::app::SampleResourceManager::~SampleResourceManager()
{
    Renderer::sSigDispose.disconnect( this );
    Renderer::sSigDestroy.disconnect( this );
    disposeAll();
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleResourceManager::onRendererDispose()
{
    effects.disposeAll();
    textures.disposeAll();
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleResourceManager::onRendererDestroy()
{
    shaders.disposeAll();
}
