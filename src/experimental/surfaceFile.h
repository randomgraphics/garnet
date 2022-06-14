namespace GN {
///
/// surface file header
///
struct SurfaceFileHeader {
    char   magic[6]; ///< must be "GNSURF"
    uint8  major;    ///< major version.
    uint8  minor;    ///< minor version.
    uint8  endian;   ///< 0: little-endian, 1: big-endian
    uint8  native;   ///< native data or not
    uint16 chunks;   ///< chunk count, valid only when native is true.
    uint32 levels;   ///< mipmap levels
    uint32 faces;    ///< face count
    uint32 width;    ///< width of basemap
    uint32 height;   ///< height of basemap
    uint32 depth;    ///< depth of basemap

    struct Format {
        uint32 stride;
        uint32 count; ///< attrib count
        struct Attrib {
            uint32 offset;
            uint32 format;
            char   semantic[8];
        } attribs[32]; ///< attribute array.
    } format;

    struct Subsurface {
        uint64 offset; ///< offset subsurface data, starting from first byte of the header.
        uint32 rowBytes;
        uint32 sliceBytes;
    } subsurfaces[gfx::MAX_SUB_SURFACES];
};
} // namespace GN
