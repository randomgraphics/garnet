#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine.Texture");

// *****************************************************************************
// local functions
// *****************************************************************************

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

                const UInt8 * src = data + indesc->getLevelOffset( face, level );

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
GN::engine::Entity * GN::engine::loadTextureEntity(
    EntityManager & em, RenderEngine & re, const StrA & filename )
{
    GN_TODO( "convert filename to absolute/full path" );

    // check if the texture is already loaded
    Entity * e = em.getEntityByName( filename, true );
    if( e ) return e;

    // open file
    AutoObjPtr<File> fp( core::openFile( filename, "rb" ) );
    if( 0 == fp ) return false;

    // read image header
    ImageReader ir;
    ImageDesc   id;
    if( !ir.reset( *fp ) ) return false;
    if( !ir.readHeader( id ) ) return false;

    // close file
    fp.clear();

    // initialize resource descriptor
    GraphicsResourceDesc grd;
    grd.type = GRT_TEXTURE;
    if( !grd.td.fromImageDesc( id ) ) return 0;

    // create the resource
    GraphicsResource * res = re.allocResource( grd );
    if( 0 == res ) return 0;

    // do load
    AutoRef<TextureLoader> loader( new TextureLoader(filename) );
    re.updateResource( res, 0, loader );

    // success
    return em.createEntity( getTextureEntityType(em), filename, res );
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

    // create the resource (note that content of the texture is leaving undefined)
    GraphicsResourceDesc grd;
    grd.type = GRT_TEXTURE;
    grd.td = desc;
    GraphicsResource * res = re.allocResource( grd );
    if( 0 == res ) return 0;
    
    // success
    return em.createEntity( getTextureEntityType(em), name, res );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::deleteTextureEntity( Entity * e )
{
    if( 0 == e ) return;

    GraphicsResource * tex = entity2Object<GraphicsResource*>( e, 0 );
    if( 0 == tex ) return;

    tex->engine.freeResource( tex );

    e->manager.eraseEntity( e );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::deleteAllTextureEntities( EntityManager & )
{
    GN_UNIMPL();
}
