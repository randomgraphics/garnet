namespace GN {
///
/// surface file header
///
struct SurfaceFileHeader {
    char     magic[6]; ///< must be "GNSURF"
    uint8_t  major;    ///< major version.
    uint8_t  minor;    ///< minor version.
    uint8_t  endian;   ///< 0: little-endian, 1: big-endian
    uint8_t  native;   ///< native data or not
    uint16_t chunks;   ///< chunk count, valid only when native is true.
    uint32_t levels;   ///< mipmap levels
    uint32_t faces;    ///< face count
    uint32_t width;    ///< width of basemap
    uint32_t height;   ///< height of basemap
    uint32_t depth;    ///< depth of basemap

    struct Format {
        uint32_t stride;
        uint32_t count; ///< attrib count
        struct Attrib {
            uint32_t offset;
            uint32_t format;
            char     semantic[8];
        } attribs[32]; ///< attribute array.
    } format;

    struct Subsurface {
        uint64_t offset; ///< offset subsurface data, starting from first byte of the header.
        uint32_t rowBytes;
        uint32_t sliceBytes;
    } subsurfaces[gfx::MAX_SUB_SURFACES];
};
} // namespace GN
