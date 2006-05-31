//
//
// -----------------------------------------------------------------------------
GN_INLINE bool GN::gfx::ImageDesc::setFaceAndLevel( size_t faces, size_t levels )
{
    safeHeapFree( mipmaps );
    numFaces = (uint32_t)faces;
    numLevels = (uint32_t)levels;
    if( numFaces > 0 && numLevels > 0 )
    {
        mipmaps = (MipmapDesc*)heapAlloc( numFaces * numLevels * sizeof(MipmapDesc) );
        if( 0 == mipmaps ) return false;
    }
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::MipmapDesc &
GN::gfx::ImageDesc::getMipmap( size_t face, size_t level )
{
    GN_ASSERT( mipmaps && face < numFaces && level < numLevels );
    return mipmaps[face*numFaces+level];
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const GN::gfx::MipmapDesc &
GN::gfx::ImageDesc::getMipmap( size_t face, size_t level ) const
{
    GN_ASSERT( mipmaps && face < numFaces && level < numLevels );
    return mipmaps[face*numFaces+level];
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getTotalBytes() const
{
    size_t nbytes = 0;
    for( size_t l = 0; l < numLevels; ++l )
    {
        nbytes += getLevelBytes( l );
    }
    return nbytes * numFaces;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getLevelBytes( size_t level ) const
{
    if( 0 == numLevels ) return 0;
    GN_ASSERT( mipmaps && 0 < numFaces && level < numLevels );
    const MipmapDesc & m = getMipmap( 0, level );
    return m.levelPitch;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getFaceBytes() const
{
    size_t sz = 0;
    for( size_t i = 0; i < numLevels; ++i ) sz += getLevelBytes( i );
    return sz;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getPixelOffset(
    size_t face, size_t level, size_t x, size_t y, size_t z ) const
{
    GN_ASSERT(
        x < getMipmap( face, level ).width &&
        y < getMipmap( face, level ).height &&
        z < getMipmap( face, level ).depth );
    return getScanlineOffset(face,level,y,z) + getClrFmtDesc(format).bits * x / 8;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getScanlineOffset(
    size_t face, size_t level, size_t y, size_t z ) const
{
    const MipmapDesc & m = getMipmap( face, level );
    GN_ASSERT( y < m.height && z < m.depth );
    return getSliceOffset( face, level, z ) + m.rowPitch * y;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getSliceOffset(
    size_t face, size_t level, size_t z ) const
{
    const MipmapDesc & m = getMipmap( face, level );
    GN_ASSERT( z < m.depth );
    return getLevelOffset(face,level) + m.slicePitch * z;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getLevelOffset( size_t face, size_t level ) const
{
    size_t sz = 0;
    for( size_t i = 0; i < level; ++i )
    {
        sz += getLevelBytes(i);
    }
    return getFaceOffset(face) + sz;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getFaceOffset( size_t face ) const
{
    return face * getFaceBytes();
}
