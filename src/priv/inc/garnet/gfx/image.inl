//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::ImageDesc::SetFaceAndLevel( size_t faces, size_t levels )
{
    SafeHeapFree( mipmaps );
    numFaces = (UInt32)faces;
    numLevels = (UInt32)levels;
    if( numFaces > 0 && numLevels > 0 )
    {
        mipmaps = (MipmapDesc*)HeapMemory::Alloc( numFaces * numLevels * sizeof(MipmapDesc) );
        if( 0 == mipmaps ) return false;
    }
    return true;
}

//
//
// -----------------------------------------------------------------------------
inline GN::gfx::MipmapDesc &
GN::gfx::ImageDesc::GetMipmap( size_t face, size_t level )
{
    GN_ASSERT( mipmaps && face < numFaces && level < numLevels );
    return mipmaps[face*numLevels+level];
}

//
//
// -----------------------------------------------------------------------------
inline const GN::gfx::MipmapDesc &
GN::gfx::ImageDesc::GetMipmap( size_t face, size_t level ) const
{
    GN_ASSERT( mipmaps && face < numFaces && level < numLevels );
    return mipmaps[face*numLevels+level];
}

//
//
// -----------------------------------------------------------------------------
inline GN::gfx::ImageType GN::gfx::ImageDesc::GetImageType() const
{
    if( 0 == numLevels || 0 == numFaces ) return IMAGE_UNKNOWN;

    const MipmapDesc & desc = GetMipmap( 0, 0 );

    if( 1 == numFaces )
    {
        if( 1 == desc.depth )
        {
            if( 1 == desc.height ) return IMAGE_1D;
            else return IMAGE_2D;
        }
        else return IMAGE_3D;
    }
    else if( 6 == numFaces && 1 == desc.depth && desc.width == desc.height )
    {
        return IMAGE_CUBE;
    }
    else return IMAGE_UNKNOWN;
}


//
//
// -----------------------------------------------------------------------------
inline size_t GN::gfx::ImageDesc::GetTotalBytes() const
{
    return GetFaceBytes() * numFaces;
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::gfx::ImageDesc::GetLevelBytes( size_t level ) const
{
    if( 0 == numLevels ) return 0;
    GN_ASSERT( mipmaps && 0 < numFaces && level < numLevels );
    const MipmapDesc & m = GetMipmap( 0, level );
    return m.levelPitch;
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::gfx::ImageDesc::GetFaceBytes() const
{
    size_t sz = 0;
    for( size_t i = 0; i < numLevels; ++i ) sz += GetLevelBytes( i );
    return sz;
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::gfx::ImageDesc::GetPixelOffset(
    size_t face, size_t level, size_t x, size_t y, size_t z ) const
{
    GN_ASSERT(
        x < GetMipmap( face, level ).width &&
        y < GetMipmap( face, level ).height &&
        z < GetMipmap( face, level ).depth );
    return SetScanlineOffset(face,level,y,z) + format.GetBitsPerPixel() * x / 8;
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::gfx::ImageDesc::SetScanlineOffset(
    size_t face, size_t level, size_t y, size_t z ) const
{
    const MipmapDesc & m = GetMipmap( face, level );
    GN_ASSERT( y < m.height && z < m.depth );
    return GetSliceOffset( face, level, z ) + m.rowPitch * y;
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::gfx::ImageDesc::GetSliceOffset(
    size_t face, size_t level, size_t z ) const
{
    const MipmapDesc & m = GetMipmap( face, level );
    GN_ASSERT( z < m.depth );
    return GetMipmapOffset(face,level) + m.slicePitch * z;
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::gfx::ImageDesc::GetMipmapOffset( size_t face, size_t level ) const
{
    size_t sz = 0;
    for( size_t i = 0; i < level; ++i )
    {
        sz += GetLevelBytes(i);
    }
    return GetFaceOffset(face) + sz;
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::gfx::ImageDesc::GetFaceOffset( size_t face ) const
{
    return face * GetFaceBytes();
}
