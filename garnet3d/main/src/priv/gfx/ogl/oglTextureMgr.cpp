#include "pch.h"
#include "oglRenderer.h"
#if !GN_ENABLE_INLINE
#include "oglTextureMgr.inl"
#endif
#include "oglTexture.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//!
//! load 2D texture slice
// -----------------------------------------------------------------------------
static void
sLoad2DSlice(
    size_t width, size_t height, size_t bits,
    size_t blockWidth, size_t blockHeight,
    const uint8_t * src, size_t src_pitch,
    uint8_t * dst, size_t dst_pitch )
{
    GN_GUARD;

    if( src_pitch == dst_pitch )
    {
        // copy image as a whole
        memcpy( dst, src, src_pitch * height / blockHeight );
    }
    else
    {
        size_t pitch = width * blockWidth * blockHeight * bits / 8;
        GN_ASSERT( pitch <= src_pitch && pitch <= dst_pitch );

        // copy row by row
        size_t h = height / blockHeight;
        for (uint32_t y = 0; y < h; ++y)
        {
            // copy scanline as an whole
            memcpy( dst, src, pitch );

            // next scanline
            src += src_pitch;
            dst += dst_pitch;
        }
    }

    GN_UNGUARD;
}

//!
//! load cubemap from memory buffer
// -----------------------------------------------------------------------------
static GN::gfx::Texture *
sLoadTexCube( GN::gfx::Renderer & r, const GN::gfx::ImageDesc & desc, const uint8_t * data )
{
    GN_GUARD;

    GN_ASSERT( GN::gfx::ImageDesc::IMG_CUBE == desc.type );

    // create texture
    GN::AutoRef<GN::gfx::Texture> tex( r.createTexture(
        GN::gfx::TEXTYPE_CUBE,
        desc[0].width, desc[0].height, 1, desc.numMips,
        desc.format, 0 ) );
    if( tex.empty() ) return 0;

    uint8_t bw = GN::gfx::getClrFmtDesc(desc.format).blockWidth;
    uint8_t bh = GN::gfx::getClrFmtDesc(desc.format).blockHeight;

    for( uint16_t z = 0; z < 6; ++z )
    {
        // copy image data to texture
        size_t bits = GN::gfx::getClrFmtDesc(desc.format).bits;
        for( uint8_t l = 0; l < desc.numMips; ++l )
        {
            const GN::gfx::ImageDesc::MipDesc & m = desc[l];

            // lock texture
            GN::gfx::LockedRect rc;
            if( !tex->lockCube( rc, (GN::gfx::TexFace)z, l, 0, GN::gfx::LOCK_WO ) ) return 0;

            // fill data
            sLoad2DSlice(
                m.width, m.height, bits, bw, bh,
                data + desc.getSliceOffset( l, z ), m.rowPitch,
                (uint8_t*)rc.data, rc.rowBytes );

            // unlock texture
            tex->unlock();
        }
    }

    // success
    return tex.detach();

    GN_UNGUARD;
}

//!
//! load 2D texture from memory buffer
// -----------------------------------------------------------------------------
static GN::gfx::Texture *
sLoadTex2D( GN::gfx::Renderer & r, const GN::gfx::ImageDesc & desc, const uint8_t * data )
{
    GN_GUARD;

    // create new texture instance
    GN::AutoRef<GN::gfx::Texture> tex( r.createTexture(
        GN::gfx::TEXTYPE_2D,
        desc[0].width,
        desc[0].height,
        1,
        desc.numMips,
        desc.format,
        0 ) );
    if( tex.empty() ) return 0;

    uint8_t bw = GN::gfx::getClrFmtDesc(desc.format).blockWidth;
    uint8_t bh = GN::gfx::getClrFmtDesc(desc.format).blockHeight;

    // copy image data to texture
    size_t bits = GN::gfx::getClrFmtDesc(desc.format).bits;
    for( uint8_t l = 0; l < desc.numMips; ++l )
    {
        const GN::gfx::ImageDesc::MipDesc & m = desc[l];

        // lock texture
        GN::gfx::LockedRect rc;
        if( !tex->lock2D(rc,l,0,GN::gfx::LOCK_WO) ) return 0;

        // fill data
        sLoad2DSlice(
            m.width, m.height, bits, bw, bh,
            data + desc.getSliceOffset( l, 0 ), m.rowPitch,
            (uint8_t*)rc.data, rc.rowBytes );

        // unlock texture
        tex->unlock();
    }

    // success
    return tex.detach();

    GN_UNGUARD;
}
// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::OGLRenderer::createTexture( TexType  type,
                                     uint32_t sx, uint32_t sy, uint32_t sz,
                                     uint32_t levels,
                                     ClrFmt   format,
                                     uint32_t usage,
                                     const TextureLoader & loader )
{
    GN_GUARD;

    makeCurrent();

    if( TEXTYPE_1D == type )
    {
        AutoRef<OGLTex1D> p( new OGLTex1D(*this) );
        p->setLoader( loader );
        if( p->init(type,sx,sy,sz,levels,format,usage) ) return p.detach();
    }
    else if( TEXTYPE_2D == type )
    {
        AutoRef<OGLTex2D> p( new OGLTex2D(*this) );
        p->setLoader( loader );
        if( p->init(type,sx,sy,sz,levels,format,usage) ) return p.detach();
    }
    else if( TEXTYPE_3D == type )
    {
        AutoRef<OGLTex3D> p( new OGLTex3D(*this) );
        p->setLoader( loader );
        if( p->init(type,sx,sy,sz,levels,format,usage) ) return p.detach();
    }
    else if ( TEXTYPE_CUBE == type )
    {
        AutoRef<OGLTexCube> p( new OGLTexCube(*this) );
        p->setLoader( loader );
        if( p->init(type,sx,sy,sz,levels,format,usage) ) return p.detach();
    }
    else
    {
        GN_ERROR( "invalid texture type!" );
    }

    // failed
    return 0;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::OGLRenderer::createTextureFromFile( File & file )
{
    GN_GUARD;

    makeCurrent();

    // read image data
    ImageReader reader;
    ImageDesc desc;
    std::vector<uint8_t> buf;
    if( !reader.reset( file ) ) return 0;
    if( !reader.readHeader( desc ) ) return 0;
    buf.resize( desc.getTotalBytes() );
    if( !reader.readImage( &buf[0] ) ) return 0;

    // generate texture
    if( ImageDesc::IMG_1D == desc.type ||
        ImageDesc::IMG_2D == desc.type )
    {
        // load 2D texture
        return sLoadTex2D( *this, desc, &buf[0] );
    }
    else if( ImageDesc::IMG_CUBE == desc.type )
    {
        // load cube texture
        return sLoadTexCube( *this, desc, &buf[0] );
    }
    else if( ImageDesc::IMG_3D == desc.type )
    {
        // TODO: load volume texture
        GN_ASSERT_EX( 0, "do not support 3D texture!" );
        return 0;
    }
    else
    {
        GN_ERROR( "unknown image type!" );
        return 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::bindTextures( const Texture * const texlist[],
                                         uint32_t start, uint32_t numtex )
{
    GN_GUARD_SLOW;

    makeCurrent();

    GN_ASSERT( texlist || (0 == numtex) );

#if GN_DEBUG
    if( numtex > getCaps( CAPS_MAX_TEXTURE_STAGES ) )
        GNGFX_WARN( "texlist is loo long!" );
#endif

    uint32_t maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);

    numtex = min<uint32_t>( maxStages, numtex );

    // apply texture list
    uint32_t i;
    for ( i = start; i < numtex; ++i )
    {
        // if null handle, then disable this texture stage
        if( texlist[i] )
        {
            chooseTextureStage( i );
            safeCast<const OGLBasicTexture *>(texlist[i])->bind();
        }
        else
        {
            disableTextureStage( i );
        }
    }

    // disable remaining texture stages
    for( ; i < maxStages; ++i )
    {
        disableTextureStage( i );
    }

    GN_UNGUARD_SLOW;
}
