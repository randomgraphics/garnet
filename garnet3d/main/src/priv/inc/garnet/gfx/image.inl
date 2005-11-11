//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getTotalBytes() const
{
    size_t nbytes = 0;
    for( uint8_t i = 0; i < numMips; ++i )
    {
        nbytes += getLevelBytes( i );
    }
    return nbytes;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getLevelBytes( uint8_t level ) const
{
    GN_ASSERT( level < numMips );
    const MipDesc & m = mips[level];
    return m.levelPitch;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getPixelOffset(
    uint8_t level, uint16_t x, uint16_t y, uint16_t z ) const
{
    GN_ASSERT(
        level < numMips &&
        x < mips[level].width &&
        y <mips[level].height &&
        z < mips[level].depth );
    return getScanlineOffset(level,y,z) + getClrFmtDesc(format).bits * x / 8;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getScanlineOffset(
    uint8_t level, uint16_t y, uint16_t z ) const
{
    const MipDesc & m = mips[level];
    GN_ASSERT( level < numMips && y <m.height && z < m.depth );
    return getSliceOffset(level,z) + m.rowPitch * y;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE size_t GN::gfx::ImageDesc::getSliceOffset( uint8_t level, uint16_t z ) const
{
    const MipDesc & m = mips[level];
    GN_ASSERT( level < numMips && z < m.depth );
    size_t nbytes = 0;
    for( uint8_t i = 0; i < level; ++i )
    {
        nbytes += getLevelBytes( i );
    }
    return nbytes + m.slicePitch * z;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::ImageDesc::MipDesc &
GN::gfx::ImageDesc::operator [] ( size_t level )
{
    GN_ASSERT( level < numMips );
    return mips[level];
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const GN::gfx::ImageDesc::MipDesc &
GN::gfx::ImageDesc::operator [] ( size_t level ) const
{
    GN_ASSERT( level < numMips );
    return mips[level];
}
