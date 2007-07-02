#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine.Texture");

// *****************************************************************************
// local functions
// *****************************************************************************

#if 1
class TextureLoader : public GraphicsResourceLoader
{
    const StrA mFileName;

public:

    TextureLoader( const StrA & filename ) : mFileName(filename) {}

    virtual bool load( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        // open file
        AutoObjPtr<File> fp( core::openFile( mFileName, "rb" ) );
        if( 0 == fp ) return false;

        // create temporary buffer
        size_t bytes = fp->size();
        AutoTypePtr<UInt8> buf( new UInt8[bytes] );

        // read file
        size_t readen;
        if( !fp->read( buf, bytes, &readen ) || readen != bytes ) return false;

        // success
        outbuf = buf.detach();
        outbytes = bytes;
        return true;
    }

    virtual bool decompress( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
    {
        MemFile<const UInt8> file( (const UInt8*)inbuf, inbytes );

        ImageReader ir;
        ImageDesc   id;

        if( !ir.reset( file ) ) return false;

        if( !ir.readHeader( id ) ) return false;

        // create temporary buffer
        size_t mipcount = id.numFaces*id.numLevels;
        size_t imgbytes = id.getTotalBytes();
        size_t mipbytes = sizeof(MipmapDesc) * mipcount;
        size_t bytes = imgbytes + sizeof(ImageDesc) + mipbytes;
        AutoTypePtr<UInt8> buf( new UInt8[bytes] );

        // fill header of output buffer
        ImageDesc  * outdesc = (ImageDesc*)buf.get();
        MipmapDesc * outmips = (MipmapDesc*)(outdesc+1);
        UInt8      * data    = (UInt8*)(outmips+mipcount);
        memcpy( outdesc, &id, sizeof(ImageDesc) );
        memcpy( outmips, id.mipmaps, mipbytes );
        outdesc->mipmaps = outmips;

        // read image data
        if( !ir.readImage( data ) ) return false;

        // success
        outbuf = buf.detach();
        outbytes = bytes;
        return true;
    }

    virtual bool copy( GraphicsResource & res, const void * inbuf, size_t inbytes, int )
    {
        Texture * tex = res.texture;
        GN_ASSERT( tex );

        const ImageDesc  * indesc = (ImageDesc*)inbuf;
        size_t           imgbytes = indesc->getTotalBytes();
        size_t             offset = sizeof(ImageDesc) + sizeof(MipmapDesc) * indesc->numFaces * indesc->numLevels;
        const UInt8      * data   = (const UInt8*)inbuf + offset;

        if( offset + imgbytes > inbytes )
        {
            GN_ERROR(sLogger)( "input buffer is incomplete!" );
            return false;
        }

        GN_ASSERT( indesc->numFaces == tex->getDesc().faces );
        GN_ASSERT( indesc->numLevels <= tex->getDesc().levels );

        for( size_t face = 0; face < indesc->numFaces; ++face )
        {
            for( size_t level = 0; level < indesc->numLevels; ++level )
            {
                const MipmapDesc & md = indesc->getMipmap( face, level );

                const UInt8 * src = data + indesc->getMipmapOffset( face, level );

                TexLockedResult tlr;
                if( !tex->lock( tlr, face, level, NULL, LOCK_DISCARD ) ) return false;

                if( tlr.sliceBytes == md.slicePitch )
                {
                    memcpy( tlr.data, src, tlr.sliceBytes * md.depth );
                }
                else
                {
                    GN_UNIMPL_WARNING();
                }

                tex->unlock();
            }
        }

        // success
        return true;
    }

    virtual void freebuf( void * inbuf, size_t )
    {
        safeDeleteArray( inbuf );
    }
};

#else

class TextureLoader : public GN::engine::GraphicsResourceLoader
{
    const StrA mFileName;

public:

    TextureLoader( const StrA & filename ) : mFileName(filename) {}

    virtual bool load( const GN::engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    bool decompress( const GN::engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void *, size_t, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GN::engine::GraphicsResource &, const void * , size_t, int )
    {
        return true;
    }

    virtual void freebuf( void *, size_t )
    {
    }
};

#endif

//
// get integer value of specific attribute
// -----------------------------------------------------------------------------
template<typename T>
static bool sGetIntAttrib( const XmlElement & node, const char * attribName, T & result, bool silence = false )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a || !str2Int<T>( result, a->value.cptr() ) )
    {
        if( !silence ) GN_ERROR(sLogger)( "attribute '%s' is missing!", attribName );
        return false;
    }
    else
    {
        return true;
    }
}

//
// get integer value of specific attribute
// -----------------------------------------------------------------------------
static bool sGetBoolAttrib( const XmlElement & node, const char * attribName, bool defval = false )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if( !a )
    {
        return defval;
    }
    else
    {
        StrA value(a->value);
        value.toLower();
        return "1" == value
            || "yes" == value
            || "true" == value;
    }
}

//
// get string value of specific attribute
// -----------------------------------------------------------------------------
static bool sGetStringAttrib( const XmlElement & node, const char * attribName, GN::StrA & result, bool silence = false )
{
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a )
    {
        if( !silence ) GN_ERROR(sLogger)( "attribute '%s' is missing!", attribName );
        return false;
    }
    else
    {
        result = a->value;
        return true;
    }
}


//
//
// -----------------------------------------------------------------------------
static GraphicsResource * sLoadTextureFromImageFile(
    RenderEngine  & re,
    const StrA    & name,
    File          & file,
    BitFields     usage )
{
    GN_GUARD;

    GN_INFO(sLogger)( "Load %s", file.name().cptr() );

    // read image header
    ImageReader ir;
    ImageDesc   id;
    if( !ir.reset( file ) ) return false;
    if( !ir.readHeader( id ) ) return false;

    // initialize texture descriptor
    gfx::TextureDesc td;
    if( !td.fromImageDesc( id ) ) return 0;
    td.usage.u32 = usage;

    // create the resource
    GraphicsResource * res = re.createTexture( name, td );
    if( 0 == res ) return 0;

    // do load
    AutoRef<TextureLoader> loader( new TextureLoader(file.name()) );
    re.updateResource( res, 0, loader );

    // success
    return res;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static GraphicsResource * sLoadTextureFromXml(
    RenderEngine  & re,
    const StrA    & name,
    File          & fp,
    const StrA    & dirname )
{
    GN_GUARD;

    GN_INFO(sLogger)( "Load %s", name.cptr() );

    // parse texture definition
    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to read XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp.name().cptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }

    // check root node
    if( 0 == xpr.root )
    {
        GN_ERROR(sLogger)( "NULL parameter!" );
        return false;
    }
    const XmlElement * e = xpr.root->toElement();
    if( 0 == e || "texture" != e->name )
    {
        GN_ERROR(sLogger)( "root node must be \"<texture>\"." );
        return false;
    }

    gfx::TextureDesc desc;

    // get texture usages
    desc.usage.u32          = 0;
    desc.usage.dynamic      = sGetBoolAttrib( *e, "dynamic" );
    desc.usage.automip      = sGetBoolAttrib( *e, "automip" );
    desc.usage.rendertarget = sGetBoolAttrib( *e, "rendertarget" );
    desc.usage.depthstencil = sGetBoolAttrib( *e, "depthstencil" );
    desc.usage.readback     = sGetBoolAttrib( *e, "readback" );
    desc.usage.tiled        = sGetBoolAttrib( *e, "tiled" );

    const XmlAttrib * ref = e->findAttrib( "ref" );
    if( ref )
    {
        StrA texname;
        core::resolvePath( texname, dirname, ref->value );
        AutoObjPtr<File> texfile( core::openFile( texname, "rb" ) );
        if( !texfile ) return 0;
        return sLoadTextureFromImageFile( re, name, *texfile, desc.usage.u32 );
    }
    else
    {
        StrA s;

        // get texture dimension
        if( !sGetStringAttrib( *e, "dim", s ) ) return false;
        if( !str2TexDim( desc.dim, s.cptr() ) )
        {
            GN_ERROR(sLogger)( "invalid texture dimension: %s", s.cptr() );
            return false;
        }

        // get texture size
        if( !sGetIntAttrib( *e, "faces" , desc.faces  ) ) return false;
        if( !sGetIntAttrib( *e, "width" , desc.width  ) ) return false;
        if( !sGetIntAttrib( *e, "height", desc.height ) ) return false;
        if( !sGetIntAttrib( *e, "depth" , desc.depth  ) ) return false;
        if( !sGetIntAttrib( *e, "levels", desc.levels ) ) return false;

        // get texture format, optional, default is FMT_UNKNOWN
        if( sGetStringAttrib( *e, "format", s, true ) )
        {
            if( !str2ClrFmt( desc.format, s ) )
            {
                GN_ERROR(sLogger)( "invalid texture format: %s", s.cptr() );
                return false;
            }
        }
        else
        {
            desc.format = FMT_UNKNOWN;
        }

        return re.createTexture( name, desc );
    }

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityTypeId GN::engine::getTextureEntityType( EntityManager & em )
{
    static EntityTypeId type = em.createEntityType( "texture" );
    return type;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::loadTextureFromFile( RenderEngine & re, const StrA & filename )
{
    GN_GUARD;

    StrA ext = extName( filename );
    if( 0 == strCmpI( ".xml", ext.cptr() ) )
    {
        // open file
        AutoObjPtr<File> fp( core::openFile( filename, "rt" ) );
        if( !fp ) return 0;

        // parse texture definition
        return sLoadTextureFromXml( re, filename, *fp, dirName(filename) );
    }
    else
    {
        // open texture file
        AutoObjPtr<File> fp( core::openFile( filename, "rb" ) );
        if( !fp )
        {
            GN_ERROR(sLogger)( "Fail to open texture file '%s'.", filename.cptr() );
            return 0;
        }

        // create texture instance
        return sLoadTextureFromImageFile( re, filename, *fp, 0 );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::loadTextureEntityFromFile(
    EntityManager & em, RenderEngine & re, const StrA & filename )
{
    StrA fullpath;
    normalizePathSeparator( fullpath, core::toNative( filename ) );

    // check if the texture is already loaded
    Entity * e = em.getEntityByName( fullpath, true );
    if( e ) return e;

    GN_INFO(sLogger)( "Create texture entity: %s", fullpath.cptr() );

    GraphicsResource * res = loadTextureFromFile( re, fullpath );
    if( 0 == res ) return false;

    return em.createEntity<GraphicsResource*>( getTextureEntityType(em), fullpath, res, &safeFreeGraphicsResource );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::createTextureEntity(
    EntityManager & em,
    RenderEngine & re,
    const StrA & name,
    const gfx::TextureDesc & desc )
{
    // check if the texture is already loaded
    Entity * e = em.getEntityByName( name, true );
    if( e )
    {
        GN_ERROR(sLogger)( "entity named '%s' does exist already!", name.cptr() );
        return 0;
    }

    GN_INFO(sLogger)( "Create texture entity: %s", name.cptr() );

    // create the resource (note that content of the texture is leaving undefined)
    GraphicsResource * res = re.createTexture( name, desc );
    if( 0 == res ) return 0;

    // success
    return em.createEntity<GraphicsResource*>( getTextureEntityType(em), name, res, &safeFreeGraphicsResource );
}
