//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::ImageDesc::setFaceAndLevel(uint32_t faces, uint32_t levels) {
    safeHeapDealloc(mipmaps);
    numFaces  = (uint32_t) faces;
    numLevels = (uint32_t) levels;
    if (numFaces > 0 && numLevels > 0) {
        mipmaps = (MipmapDesc *) HeapMemory::alloc(numFaces * numLevels * sizeof(MipmapDesc));
        if (0 == mipmaps) return false;
    }
    return true;
}

//
//
// -----------------------------------------------------------------------------
inline GN::gfx::MipmapDesc & GN::gfx::ImageDesc::getMipmap(uint32_t face, uint32_t level) {
    GN_ASSERT(mipmaps && face < numFaces && level < numLevels);
    return mipmaps[face * numLevels + level];
}

//
//
// -----------------------------------------------------------------------------
inline const GN::gfx::MipmapDesc & GN::gfx::ImageDesc::getMipmap(uint32_t face, uint32_t level) const {
    GN_ASSERT(mipmaps && face < numFaces && level < numLevels);
    return mipmaps[face * numLevels + level];
}

//
//
// -----------------------------------------------------------------------------
inline GN::gfx::ImageType GN::gfx::ImageDesc::getImageType() const {
    if (0 == numLevels || 0 == numFaces) return IMAGE_UNKNOWN;

    const MipmapDesc & desc = getMipmap(0, 0);

    if (1 == numFaces) {
        if (1 == desc.depth) {
            if (1 == desc.height)
                return IMAGE_1D;
            else
                return IMAGE_2D;
        } else
            return IMAGE_3D;
    } else if (6 == numFaces && 1 == desc.depth && desc.width == desc.height) {
        return IMAGE_CUBE;
    } else
        return IMAGE_UNKNOWN;
}

//
//
// -----------------------------------------------------------------------------
inline uint32_t GN::gfx::ImageDesc::getTotalBytes() const { return getFaceBytes() * numFaces; }

//
//
// -----------------------------------------------------------------------------
inline uint32_t GN::gfx::ImageDesc::getLevelBytes(uint32_t level) const {
    if (0 == numLevels) return 0;
    GN_ASSERT(mipmaps && 0 < numFaces && level < numLevels);
    const MipmapDesc & m = getMipmap(0, level);
    return m.levelPitch;
}

//
//
// -----------------------------------------------------------------------------
inline uint32_t GN::gfx::ImageDesc::getFaceBytes() const {
    uint32_t sz = 0;
    for (uint32_t i = 0; i < numLevels; ++i) sz += getLevelBytes(i);
    return sz;
}

//
//
// -----------------------------------------------------------------------------
inline uint32_t GN::gfx::ImageDesc::getPixelOffset(uint32_t face, uint32_t level, uint32_t x, uint32_t y, uint32_t z) const {
    GN_ASSERT(x < getMipmap(face, level).width && y < getMipmap(face, level).height && z < getMipmap(face, level).depth);
    return getScanlineOffset(face, level, y, z) + format.getBitsPerPixel() * x / 8;
}

//
//
// -----------------------------------------------------------------------------
inline uint32_t GN::gfx::ImageDesc::getScanlineOffset(uint32_t face, uint32_t level, uint32_t y, uint32_t z) const {
    const MipmapDesc & m = getMipmap(face, level);
    GN_ASSERT(y < m.height && z < m.depth);
    return getSliceOffset(face, level, z) + m.rowPitch * y;
}

//
//
// -----------------------------------------------------------------------------
inline uint32_t GN::gfx::ImageDesc::getSliceOffset(uint32_t face, uint32_t level, uint32_t z) const {
    const MipmapDesc & m = getMipmap(face, level);
    GN_ASSERT(z < m.depth);
    return getMipmapOffset(face, level) + m.slicePitch * z;
}

//
//
// -----------------------------------------------------------------------------
inline uint32_t GN::gfx::ImageDesc::getMipmapOffset(uint32_t face, uint32_t level) const {
    uint32_t sz = 0;
    for (uint32_t i = 0; i < level; ++i) { sz += getLevelBytes(i); }
    return getFaceOffset(face) + sz;
}

//
//
// -----------------------------------------------------------------------------
inline uint32_t GN::gfx::ImageDesc::getFaceOffset(uint32_t face) const { return face * getFaceBytes(); }
