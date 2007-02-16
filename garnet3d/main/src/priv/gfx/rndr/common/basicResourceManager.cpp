#include "pch.h"
#include "basicRenderer.h"

///
/// local Cg error handler
///
#ifdef HAS_CG
static void sCgErrorHandler( CGcontext ctx, CGerror err, void * )
{
    static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.common.cg");

    GN_ERROR(sLogger)( "Cg error: %s", cgGetErrorString(err) );
    const char * listing = cgGetLastListing(ctx);
    if( listing )
    {
        GN_ERROR(sLogger)( "Last listing = %s", listing );
    }
}
#endif

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::resInit()
{
#ifdef HAS_CG
    GN_GUARD;

    // set Cg error hanlder
    cgSetErrorHandler( &sCgErrorHandler, 0 );

    // create Cg context
    mCgContext = cgCreateContext();
    if( !mCgContext )
    {
        GN_ERROR(sLogger)( "Fail to create Cg context!" );
        return false;
    }

    GN_UNGUARD;
#endif

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::resQuit()
{
#ifdef HAS_CG
    GN_GUARD;

    // destroy Cg context
    if( mCgContext )
    {
        cgDestroyContext( mCgContext );
        mCgContext = 0;
    }

    GN_UNGUARD;
#endif
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::BasicRenderer::createTextureFromFile( File & file )
{
    GN_GUARD_ALWAYS;

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
    AutoRef<Texture> p( createTexture(type,w,h,d,desc.numFaces,desc.numLevels,desc.format,0 ) );
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

        const UInt8 * mipData = &buf[0] + desc.getSliceOffset( f, l, 0 );

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
