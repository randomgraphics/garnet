#include "pch.h"
#include "garnet/GNapp.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

static GN::Logger * sLogger = GN::getLogger("GN.app.SampleResourceManager");

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
    StrA path = SampleResourceManager::sSearchResourceFile( name );
    if( path.empty() )
    {
        GN_ERROR(sLogger)( "Raw resource '%s' creation failed: path not found.", name.cptr() );
        return false;
    }

    GN_INFO(sLogger)( "Load raw resource '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    DiskFile fp;
    if( !fp.open( path, "rb" ) )
    {
        GN_ERROR(sLogger)( "Raw resource '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
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
        size_t readen;
        if( !fp.read( &r->buffer[0], fp.size(), &readen ) )
        {
            GN_ERROR(sLogger)( "Raw resource '%s' creation failed: read file '%s' error.", name.cptr(), path.cptr() );
            delete r;
            return false;
        }
        GN_ASSERT( readen <= fp.size() );
        if( readen < fp.size() ) r->buffer.resize( readen );
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
        GN_ERROR(sLogger)( "Shader '%s' creation failed: renderer is not ready." );
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
    path = SampleResourceManager::sSearchResourceFile( path );
    if( path.empty() )
    {
        GN_ERROR(sLogger)( "Shader '%s' creation failed: path not found.", name.cptr() );
        return false;
    }

    GN_INFO(sLogger)( "Load shader '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    DiskFile fp;
    if( !fp.open( path, "rt" ) )
    {
        GN_ERROR(sLogger)( "Shader '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
        return false;
    }

    AutoObjArray<char> buf( new char[fp.size()+1] );
    AutoObjArray<char> typeStr( new char[fp.size()+1] );
    AutoObjArray<char> langStr( new char[fp.size()+1] );

    // read file
    size_t readen;
    if( !fp.read( buf, fp.size(), &readen ) )
    {
        GN_ERROR(sLogger)( "Shader '%s' creation failed: can't read file '%s'.", name.cptr(), path.cptr() );
        return false;
    }
    GN_ASSERT( readen <= fp.size() );
	fp.close();
    buf[readen] = 0;

    const char * code = buf;
    ShaderType type;
    ShadingLanguage lang;
    if( sCheckPrefix( code, "!!ARBvp" ) )
    {
        type = SHADER_VS;
        lang = LANG_OGL_ARB;
    }
    else if( sCheckPrefix( code, "!!ARBfp" ) )
    {
        type = SHADER_PS;
        lang = LANG_OGL_ARB;
    }
    else if(
#if GN_MSVC8
        2 == sscanf_s( code, "// type=%s lang=%s", typeStr.get(), readen, langStr.get(), readen ) ||
		2 == sscanf_s( code, "# type=%s lang=%s", typeStr.get(), readen, langStr.get(), readen ) )
#else
        2 == sscanf( code, "// type=%s lang=%s", typeStr.get(), langStr.get() ) ||
        2 == sscanf( code, "# type=%s lang=%s", typeStr.get(), langStr.get() ) )
#endif
    {
        if( !str2ShaderType( type, typeStr ) )
        {
            GN_ERROR(sLogger)( "Shader '%s' creation failed: invalid shader type '%s'.", name.cptr(), typeStr.get() );
            return false;
        }
        if( !str2ShadingLanguage( lang, langStr ) )
        {
            GN_ERROR(sLogger)( "Shader '%s' creation failed: invalid shading language '%s'.", name.cptr(), langStr.get() );
            return false;
        }
    }
    else
    {
        GN_ERROR(sLogger)( "Shader '%s' creation failed: unknown/invalid shader header.", name.cptr() );
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
        GN_ERROR(sLogger)( "Null texture '%s' creation failed: renderer is not ready.", name.cptr() );
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
    result = SampleResourceManager::sCreateTextureFromFile( name );
    return NULL != result;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteTexture( Texture * & ptr, void * )
{
    GN::safeDecref( ptr );
}

// *****************************************************************************
// local effect loader from XML file
// *****************************************************************************

//
// Create empty effect
// -----------------------------------------------------------------------------
static bool sCreateNullEffect( Effect * & result, const StrA &, void * )
{
    GN_GUARD;

    EffectDesc desc;
    desc.shaders["vs"].type = SHADER_VS;
    desc.shaders["ps"].type = SHADER_PS;
    desc.techniques.resize(1);
    desc.techniques[0].name = "t0";
    desc.techniques[0].passes.resize(1);
    desc.techniques[0].passes[0].shaders[SHADER_VS] = "vs";
    desc.techniques[0].passes[0].shaders[SHADER_PS] = "ps";

    AutoObjPtr<Effect> eff( new Effect );
    if( !eff->init( desc ) ) return false;

    // success
    result = eff.detach();
    return true;

    GN_UNGUARD;
}

//
// Load empty effect from file
// -----------------------------------------------------------------------------
static bool sCreateEffect( Effect * & result, const StrA & name, void * )
{
    GN_GUARD;

    // get resouce path
    StrA path = SampleResourceManager::sSearchResourceFile( name );
    if( path.empty() )
    {
        GN_ERROR(sLogger)( "Effect '%s' creation failed: path not found.", name.cptr() );
        return false;
    }

    GN_INFO(sLogger)( "Load Effect '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    DiskFile fp;
    if( !fp.open( path, "rt" ) )
    {
        GN_ERROR(sLogger)( "Effect '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
        return false;
    }

    // parse XML file
    EffectDesc desc;
    if( !desc.fromXml( fp ) ) return false;

    // create effect instance
    AutoObjPtr<Effect> eff( new Effect );
    if( !eff->init( desc ) ) return false;

    // success
    result = eff.detach();
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteEffect( Effect * & ptr, void * )
{
    GN::safeDelete( ptr );
}

// *****************************************************************************
// SampleResourceManager
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::StrA GN::app::SampleResourceManager::sSearchResourceFile( const StrA & name )
{
    GN_GUARD;

    if( path::isFile( name ) ) return name;

    if( path::isAbsPath(name) ) return StrA::EMPTYSTR;

    StrA fullPath;

#define CHECK_PATH( X ) do { fullPath = path::join X ; if( path::isFile( fullPath ) ) return path::toNative(fullPath); } while(0)

    // search in startup directory
    CHECK_PATH( ("startup:", name) );
    CHECK_PATH( ("startup:/media", name) );

    // search in application directory
    CHECK_PATH( ("app:", name ) );
    CHECK_PATH( ( "app:/media", name ) );
    CHECK_PATH( ( "app:../media", name ) );

    // resource not found.
    return StrA::EMPTYSTR;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
gfx::Texture *
GN::app::SampleResourceManager::sCreateTextureFromFile( const StrA & name )
{
    GN_GUARD;

    // check for global renderer
    if( 0 == gRendererPtr )
    {
        GN_ERROR(sLogger)( "Texture '%s' creation failed: renderer is not ready." );
        return 0;
    }

    // get resource path
    StrA path = SampleResourceManager::sSearchResourceFile( name );
    if( path.empty() )
    {
        GN_ERROR(sLogger)( "Texture '%s' creation failed: path not found.", name.cptr() );
        return 0;
    }

    GN_INFO(sLogger)( "Load texture '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    DiskFile fp;
    if( !fp.open( path, "rb" ) )
    {
        GN_ERROR(sLogger)( "Texture '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
        return 0;
    }

    // create texture instance
    Texture * r = gRenderer.createTextureFromFile( fp );
#if !GN_RETAIL_BUILD
    if( r ) r->name() = name;
#endif
    return r;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
Shader *
GN::app::SampleResourceManager::sCreateShaderFromFile(
    ShaderType type, ShadingLanguage lang, const StrA & name, const StrA & hints )
{
    GN_GUARD;

    // check for global renderer
    if( 0 == gRendererPtr )
    {
        GN_ERROR(sLogger)( "Shader '%s' creation failed: renderer is not ready." );
        return 0;
    }

    // get resource path
    StrA path = SampleResourceManager::sSearchResourceFile( name );
    if( path.empty() )
    {
        GN_ERROR(sLogger)( "Shader '%s' creation failed: path not found.", name.cptr() );
        return 0;
    }

    GN_INFO(sLogger)( "Load shader '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    DiskFile fp;
    if( !fp.open( path, "rb" ) )
    {
        GN_ERROR(sLogger)( "Shader '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
        return 0;
    }

    // read file
    DynaArray<char> buf( fp.size() + 1 );
    size_t readen;
    if( !fp.read( buf.cptr(), fp.size(), &readen ) )
    {
        GN_ERROR(sLogger)( "Shader '%s' creation failed: can't read file '%s'.", name.cptr(), path.cptr() );
        return false;
    }
    GN_ASSERT( readen <= fp.size() );
	fp.close();
    buf[readen] = 0;

    // create shader instance
    Shader * r = gRenderer.createShader( type, lang, buf.cptr(), hints );
#if !GN_RETAIL_BUILD
    if( r ) r->name() = name;
#endif
    return r;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------

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

    effects.setCreator( &sCreateEffect );
    effects.setDeletor( &sDeleteEffect );
    effects.setNullor( &sCreateNullEffect );

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
