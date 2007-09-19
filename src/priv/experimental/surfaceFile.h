namespace GN
{
    ///
    /// surface file header
    ///
    struct SurfaceFileHeader
    {
        char   magic[6];  ///< must be "GNSURF"
        UInt8  major;     ///< major version.
        UInt8  minor;     ///< minor version.
        UInt8  endian;    ///< 0: little-endian, 1: big-endian
        UInt8  native;    ///< native data or not
        UInt16 chunks;    ///< chunk count, valid only when native is true.
        UInt32 levels;    ///< mipmap levels
        UInt32 faces;     ///< face count
        UInt32 width;     ///< width of basemap
        UInt32 height;    ///< height of basemap
        UInt32 depth;     ///< depth of basemap

        struct Format
        {
            UInt32 stride;
            UInt32 count; ///< attrib count
            struct Attrib
            {
                UInt32 offset;
                UInt32 format;
                char   semantic[8];
            }      attribs[32]; ///< attribute array.
        } format;

        struct Subsurface
        {
            UInt64 offset; ///< offset subsurface data, starting from first byte of the header.
            UInt32 rowBytes;
            UInt32 sliceBytes;
        } subsurfaces[gfx::MAX_SUB_SURFACES];
    };
}
