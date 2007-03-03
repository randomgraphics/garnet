#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene.ResourceManager");
GN::Logger * GN::scene::ResourceManager::sLogger = GN::getLogger("GN.scene.ResourceManager");

// *****************************************************************************
// resource type classifiers
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool sIsTexture( const StrA & name )
{
    StrA ext;
    GN::extName( ext, name );
    ext.toUpper();

    return ".BMP" == ext
        || ".DDS" == ext
        || ".JPG" == ext
        || ".PNG" == ext
        || ".TGA" == ext;
}

//
//
// -----------------------------------------------------------------------------
static bool sIsXml( const StrA & name, const StrA & root )
{
    if( !fs::isFile( name ) ) return false;

    AutoObjPtr<File> fp( fs::openFile( name, "rt" ) );
    if( !fp ) return false;

    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, *fp ) ) return false;

    GN_ASSERT( xpr.root );
    XmlElement * e = xpr.root->toElement();

    return e && root == e->name;
}

//
//
// -----------------------------------------------------------------------------
static const StrA & sGetResourceType( const StrA & name )
{
    static const StrA & texture("texture");
    static const StrA & effect("effect");
    static const StrA & mesh("mesh");
    
    if( sIsTexture( name ) ) return texture;
    if( sIsXml( name, effect ) ) return effect;
    if( sIsXml( name, mesh ) ) return mesh;
    return StrA::EMPTYSTR;
}

// *****************************************************************************
// local resource manipulators
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::scene::BaseResource * sCreateTexture( const StrA & name )
{
    GN_GUARD;

    // check for global renderer
    if( 0 == gRendererPtr )
    {
        GN_ERROR(sLogger)( "Texture '%s' creation failed: renderer is not ready." );
        return 0;
    }

    Texture * result;
    if( name.empty() )
    {
        // Create a 1x1 pure blue texture.
        AutoRef<Texture> tex( gRenderer.create1DTexture( 1 ) );
        UInt32 * texData = (UInt32*)tex->lock1D( 0, 0, 0, GN::gfx::LOCK_WO );
        if( 0 == texData ) return 0;
        //           A R G B
        *texData = 0xFF0000FF;
        tex->unlock();
        result = tex.detach();
    }
    else
    {
        // open file
        AutoObjPtr<File> fp( fs::openFile( name, "rb" ) );
        if( !fp )
        {
            GN_ERROR(sLogger)( "Fail to open texture file '%s'.", name.cptr() );
            return 0;
        }

        // create texture instance
        result = createTextureFromFile( *fp );
    }

#if !GN_RETAIL_BUILD
    if( result ) result->name() = name;
#endif
    return result;

    GN_UNGUARD;
}

//
// Load effect from file
// -----------------------------------------------------------------------------
static GN::scene::BaseResource * sCreateEffect( const StrA & name )
{
    GN_GUARD;

    if( name.empty() )
    {
        EffectDesc desc;
        desc.shaders["vs"].type = SHADER_VS;
        desc.shaders["ps"].type = SHADER_PS;
        desc.shaders["gs"].type = SHADER_GS;
        desc.techniques.resize(1);
        desc.techniques[0].name = "t0";
        AutoRef<Effect> eff( new Effect );
        if( !eff->init( desc ) ) { GN_UNEXPECTED(); return 0; }
        return eff.detach();
    }
    else
    {
        // open file
        AutoObjPtr<File> fp( fs::openFile( name, "rt" ) );
        if( !fp )
        {
            GN_ERROR(sLogger)( "Fail to open effect file '%s'.", name.cptr() );
            return 0;
        }

        // parse XML file
        EffectDesc desc;
        if( !desc.fromXml( *fp ) ) return 0;

        // create effect instance
        AutoRef<Effect> eff( new Effect );
        if( !eff->init( desc ) ) return 0;

        // success
        return eff.detach();
    }

    GN_UNGUARD;
}

//
// Load mesh from file
// -----------------------------------------------------------------------------
static GN::scene::BaseResource * sCreateMesh( const StrA & name )
{
    GN_GUARD;

    if( name.empty() )
    {
        Mesh * mesh = new Mesh;
        mesh->clear();
        return mesh;
    }
    else
    {
        // open file
        AutoObjPtr<File> fp( fs::openFile( name, "rt" ) );
        if( !fp )
        {
            GN_ERROR(sLogger)( "Mesh '%s' creation failed.", name.cptr() );
            return 0;
        }

        // get mesh directory
        StrA meshdir = dirName( name );

        // create mesh instance
        AutoRef<Mesh> mesh( new Mesh );
        if( !mesh->loadFromXmlFile( *fp, meshdir, gRenderer ) ) return 0;

        // success
        return mesh.detach();
    }

    GN_UNGUARD;
}

//
// Load mesh from file
// -----------------------------------------------------------------------------
static void sDeleteResource( GN::scene::BaseResource * r )
{
    GN_GUARD;

    safeDecref( r );

    GN_UNGUARD;
}

// *****************************************************************************
// ResourceManager public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::ResourceManager::ResourceManager()
{
    GN_GUARD;

    mLRUHead.prev = 0;
    mLRUHead.next = &mLRUTail;
    mLRUTail.prev = &mLRUHead;
    mLRUTail.next = 0;

    registerResourceType( "texture" , &sCreateTexture , &sDeleteResource );
    registerResourceType( "effect"  , &sCreateEffect  , &sDeleteResource );
    registerResourceType( "mesh"    , &sCreateMesh    , &sDeleteResource );

    Renderer::sSigDispose.connect( this, &ResourceManager::onRendererDispose );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::scene::ResourceManager::~ResourceManager()
{
    GN_GUARD;

    Renderer::sSigDispose.disconnect( this );

    for( ResourceId id = mResources.first(); id; id = mResources.next( id ) )
    {
        ResourceDesc * desc = mResources[id];
        GN_ASSERT( desc );
        if( !desc->disposed )
        {
            disposeResource( id );
        }
        delete desc;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::StrA GN::scene::ResourceManager::sGetNativeResourceFileName( const StrA & name )
{
    GN_GUARD;

    if( !fs::isFile( name ) ) return StrA::EMPTYSTR;

    return fs::toNative( name );    

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::ResourceManager::registerResourceType(
    const StrA & type,
    const ResourceCreator & creator,
    const ResourceDeletor & deletor )
{
    // check resource type
    UInt32 typeidx = 0;
    for( ; typeidx < mResourceTypes.size(); ++typeidx )
    {
        if( mResourceTypes[typeidx].type == type ) break;
    }
    if( typeidx < mResourceTypes.size() )
    {
        GN_ERROR(sLogger)( "resource type '%s' does exist already", type.cptr() );
        return;
    }

    // check functors
    if( !creator || !deletor )
    {
        GN_ERROR(sLogger)( "creator and deletor can't be null." );
        return;
    }

    // create new type descriptor
    mResourceTypes.resize( mResourceTypes.size() + 1 );
    ResourceTypeDesc & rtd = mResourceTypes.back();
    rtd.type = type;
    rtd.creator = creator;
    rtd.deletor = deletor;
}

//
//
// -----------------------------------------------------------------------------
GN::scene::ResourceId
GN::scene::ResourceManager::addResource( const StrA & name, const StrA & type )
{
    GN_GUARD;

    // get real name
    StrA realname;
    resolveName( realname, name );

    // check if it is an already existing resource
    ResourceDesc * desc;
    StringMap::const_iterator ci = mResourceNames.find( realname );
    if( mResourceNames.end() != ci )
    {
        GN_ERROR(sLogger)( "resource '%s' exists already!", name.cptr() );
        return 0;
    }

    // check resource type
    UInt32 typeidx = 0;
    for( ; typeidx < mResourceTypes.size(); ++typeidx )
    {
        if( mResourceTypes[typeidx].type == type ) break;
    }
    if( typeidx >= mResourceTypes.size() )
    {
        GN_ERROR(sLogger)( "invalid resource type: %s", type.cptr() );
        return 0;
    }

    // create new resource descriptor
    desc = new ResourceDesc;
    desc->type = typeidx;
    desc->name = realname;
    desc->data = 0;
    desc->disposed = true;

    // add to resource pool
    ResourceId id = mResources.add( desc );
    if( 0 == id )
    {
        GN_ERROR(sLogger)( "Fail to create new resource descriptor!" );
        delete desc;
        return 0;
    }

    // add to name list
    mResourceNames[realname] = id;
    GN_ASSERT( mResourceNames.size() == mResources.size() );

    // insert to head of LRU table
    desc->prev = &mLRUHead;
    desc->next = mLRUHead.next;
    desc->next->prev = desc;
    mLRUHead.next = desc;

    // success
    return id;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::ResourceManager::removeResource( ResourceId id )
{
    if( !validId( id ) )
    {
        GN_ERROR(sLogger)( "invalid resource ID: %d", id );
        return;
    }

    // dispose the resource first
    disposeResource( id );

    ResourceDesc * desc = mResources[id];
    GN_ASSERT( desc );
    GN_ASSERT( desc->disposed );

    // remove from pool
    mResources.remove( id );

    // remove from LRU list
    GN_ASSERT( desc->prev );
    GN_ASSERT( desc->next );
    ResourceDesc * prev = desc->prev;
    ResourceDesc * next = desc->next;
    prev->next = desc->next;
    next->prev = desc->prev;

    // delete resource descriptor itself
    delete desc;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::ResourceManager::disposeResource( ResourceId id )
{
    if( !validId( id ) )
    {
        GN_ERROR(sLogger)( "invalid resource ID: %d", id );
        return;
    }

    ResourceDesc * desc = mResources[id];
    GN_ASSERT( desc );

    if( desc->disposed ) return; // do nothing, if already disposed.

    GN_ASSERT( desc->type < mResourceTypes.size() );
    ResourceTypeDesc & rtd = mResourceTypes[desc->type];

    rtd.deletor( desc->data );
    desc->disposed = true;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::ResourceManager::disposeAll()
{
    for( ResourceId id = mResources.first(); id; id = mResources.next( id ) )
    {
        ResourceDesc * desc = mResources[id];
        GN_ASSERT( desc );
        if( !desc->disposed )
        {
            disposeResource( id );
        }
    }
}

// *****************************************************************************
// ResourceManager private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::BaseResource *
GN::scene::ResourceManager::getResourceImpl( ResourceId id )
{
    GN_GUARD_SLOW;

    GN_ASSERT( validId(id) );

    ResourceDesc * desc = mResources[id];
    GN_ASSERT( desc );

    if( desc->disposed )
    {
        // get resource type descriptor
        GN_ASSERT( desc->type < mResourceTypes.size() );
        const ResourceTypeDesc & rtd = mResourceTypes[desc->type];
        GN_ASSERT( rtd.creator && rtd.deletor );

        // recreate the resource
        GN_INFO(sLogger)( "Load %s '%s'", rtd.type.cptr(), desc->name.cptr() );
        desc->data = rtd.creator( desc->name );

        // receation failed, fallback to null resource
        if( !desc->data )
        {
            // create null resource instance
            GN_INFO(sLogger)( "Fallback to null instance for %s '%s'", rtd.type.cptr(), desc->name.cptr() );
            desc->data = rtd.creator( StrA::EMPTYSTR );
        }

        desc->disposed = false;
    }

    // move desc to head of LRU list.
    GN_ASSERT( desc->prev );
    GN_ASSERT( desc->next );
    ResourceDesc * prev = desc->prev;
    ResourceDesc * next = desc->next;
    prev->next = desc->next;
    next->prev = desc->prev;
    desc->next = mLRUHead.next;
    desc->next->prev = desc;
    mLRUHead.next = desc;

    // success
    return desc->data;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::ResourceManager::resolveName( StrA & out, const StrA & in ) const
{
    if( fs::isFile(in) )
    {
        fs::toNative( out, in );
    }
    else
    {
        out = in;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::ResourceManager::onRendererDispose()
{
    disposeAll();
}

// *****************************************************************************
// global public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::addResourceDirectory( const StrA & path, bool recursive )
{
    // glob all files
    std::vector<StrA> filenames;
    fs::glob( filenames, path, "*.*", recursive, false );

    ResourceManager & rm = ResourceManager::sGetInstance();

    for( size_t i = 0; i < filenames.size(); ++i )
    {
        const StrA & name = filenames[i];

        const StrA & type = sGetResourceType( name );

        if( type.empty() )
        {
            GN_TRACE(sLogger)( "ignore unclassified file: %s", name.cptr() );
            continue;
        }

        rm.addResource( name, type );
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::scene::createTextureFromFile( File & file )
{
    GN_GUARD_ALWAYS;

    using namespace GN::gfx;

    // read image data
    ImageReader reader;
    ImageDesc desc;
    std::vector<UInt8> buf;
    if( !reader.reset( file ) ) return 0;
    if( !reader.readHeader( desc ) ) return 0;
    buf.resize( desc.getTotalBytes() );
    if( !reader.readImage( &buf[0] ) ) return 0;

    // get image size
    UInt32 w = desc.mipmaps[0].width;
    UInt32 h = desc.mipmaps[0].height;
    UInt32 d = desc.mipmaps[0].depth;

    // determine texture type, based on image demension
    TexType type;
    if( 1 == desc.numFaces )
    {
        type = 1 == d ? TEXTYPE_2D : TEXTYPE_3D;
    }
    else if( 6 == desc.numFaces && w == h && 1 == d )
    {
        type = TEXTYPE_CUBE;
    }
    else if( 1 == d )
    {
        GN_ASSERT( desc.numFaces > 1 );
        type = TEXTYPE_STACK;
    }
    else
    {
        GN_ERROR(sLogger)( "Can't determine texture type for image: face(%d), width(%d), height(%d), depth:%d)." );
        return 0;
    }

    // create texture instance
    AutoRef<Texture> p( gRenderer.createTexture(type,w,h,d,desc.numFaces,desc.numLevels,desc.format,0 ) );
    if( !p ) return 0;

#define ASSERT_RANGE( x, l, s, e ) GN_ASSERT( (s) <= (x) && ((x)+(l)) <= ((s)+(e)) )

    // copy data to texture
    for( size_t f = 0; f < desc.numFaces; ++f )
    for( size_t l = 0; l < desc.numLevels; ++l )
    {
        TexLockedResult tlr;

        const MipmapDesc & m = desc.getMipmap( f, l );

        if( !p->lock( tlr, f, l, 0, LOCK_DISCARD ) ) return false;

        GN_ASSERT(
            m.width <= p->getMipSize(l).x &&
            m.height <= p->getMipSize(l).y &&
            m.depth <= p->getMipSize(l).z &&
            m.slicePitch <= tlr.sliceBytes &&
            m.rowPitch <= tlr.rowBytes );

        const UInt8 * mipData = &buf[0] + desc.getLevelOffset( f, l );

        if( m.slicePitch == tlr.sliceBytes )
        {
            ::memcpy( tlr.data, mipData, m.levelPitch );
        }
        else if( m.rowPitch == tlr.rowBytes )
        {
            const UInt8 * src = mipData;
            UInt8 * dst = (UInt8*)tlr.data;
            for( size_t z = 0; z < m.depth; ++z )
            {
               ASSERT_RANGE( src, m.slicePitch, mipData, m.levelPitch );
               ASSERT_RANGE( dst, m.slicePitch, (UInt8*)tlr.data, tlr.sliceBytes*p->getMipSize(l).z );
               ::memcpy( dst, src, m.slicePitch );
               src += m.slicePitch;
               dst += tlr.sliceBytes;
            }
        }
        else
        {
            for( size_t z = 0; z < m.depth; ++z )
            {
                const UInt8 * src = mipData + z * m.slicePitch;
                UInt8 * dst = (UInt8*)tlr.data + z * tlr.sliceBytes;
                for( size_t y = 0; y < m.height; ++y )
                {
                   ASSERT_RANGE( src, m.rowPitch, mipData, m.levelPitch );
                   ASSERT_RANGE( dst, m.rowPitch, (UInt8*)tlr.data, tlr.sliceBytes*p->getMipSize(l).z );
                   ::memcpy( dst, src, m.rowPitch );
                   src += m.rowPitch;
                   dst += tlr.rowBytes;
                }
            }
        }

        p->unlock();
    }

    // success
    return p.detach();

    GN_UNGUARD_ALWAYS_DO( return 0; );
}

