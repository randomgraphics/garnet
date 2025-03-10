#include "pch.h"
#include "meshresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

enum MeshFileType {
    MESH_FILE_UNKNOWN,
    MESH_FILE_XML,
    MESH_FILE_BIN,
};

struct MeshBinaryHeaderV1 {
    char     tag[2]; ///< must be "GN";
    uint16_t endian; ///< 0x0201 means file is in same endian as the host OS.
    uint32_t padding[7];
};
GN_CASSERT(32 == sizeof(MeshBinaryHeaderV1));

static const char MESH_BINARY_TAG_V1[] = "GN";

static const uint16_t MESH_BINARY_ENDIAN_TAG_V1 = 0x0201;

struct MeshBinaryFileHeaderV2 {
    char             tag[16];                                  ///< must be "GARNET MESH BIN\0"
    uint32_t         endian;                                   ///< endian tag: 0x01020304 means file is in the same endian as the host OS.
    uint32_t         version;                                  ///< mesh binary version must be 0x00010000
    uint32_t         prim;                                     ///< primitive type
    uint32_t         numvtx;                                   ///< number of vertices
    uint32_t         numidx;                                   ///< number of indices. 0 means non-indexed mesh
    uint8_t          idx32;                                    ///< true for 32-bit index buffer
    uint8_t          dynavb;                                   ///< true for dynamic vertex buffer
    uint8_t          dynaib;                                   ///< trur for dynamic index buffer
    uint8_t          _padding;                                 ///< padding for 32-bit alignment
    MeshVertexFormat vtxfmt;                                   ///< vertex format
    uint32_t         vertices[GpuContext::MAX_VERTEX_BUFFERS]; ///< The offset of vertex buffer data, not including the header.
    uint32_t         strides[GpuContext::MAX_VERTEX_BUFFERS];  ///< vertex buffer strides. 0 means using vertex size defined by vertex format.
    uint32_t         offsets[GpuContext::MAX_VERTEX_BUFFERS];  ///< vertex buffer offset.
    uint32_t         indices;                                  ///< The offset of index data. Ignored, if numidx is 0.
    uint32_t         bytes;                                    ///< total binary size in bytes, not including this header.
};

static const char MESH_BINARY_TAG_V2[] = "GARNET MESH BIN";
GN_CASSERT(sizeof(MESH_BINARY_TAG_V2) == 16);

static const uint32_t MESH_BINARY_ENDIAN_TAG_V2 = 0x01020304;

struct MeshVertexPosition {
    const float * x;
    size_t        strideX;
    const float * y;
    size_t        strideY;
    const float * z;
    size_t        strideZ;
};

//
//
// -----------------------------------------------------------------------------
void sSwapVertexEndianInplace(void *                   buffer,
                              uint32_t                 bufferSize, // buffer size in bytes
                              const MeshVertexFormat & format, uint32_t stream, uint32_t stride) {
    if (stride == 0) stride = format.calcStreamStride(stream);

    uint8_t * vertex = (uint8_t *) buffer;

    size_t count = bufferSize / stride;

    for (size_t i = 0; i < count; ++i, vertex += stride) {
        for (size_t i = 0; i < format.numElements; ++i) {
            const MeshVertexElement & e = format.elements[i];

            uint8_t * p = vertex + e.offset;

            switch (e.format.layout) {
            // 16 bits
            case PixelFormat::LAYOUT_4_4_4_4:
            case PixelFormat::LAYOUT_5_5_5_1:
            case PixelFormat::LAYOUT_5_6_5:
            case PixelFormat::LAYOUT_16:
                swap8in16(p, p, 1);
                break;

            case PixelFormat::LAYOUT_16_16:
                swap8in16(p, p, 2);
                break;

            case PixelFormat::LAYOUT_16_16_16_16:
                swap8in16(p, p, 4);
                break;

            // 32 bits
            case PixelFormat::LAYOUT_10_11_11:
            case PixelFormat::LAYOUT_11_11_10:
            case PixelFormat::LAYOUT_10_10_10_2:
            case PixelFormat::LAYOUT_32:
                swap8in32(p, p, 1);
                break;

            case PixelFormat::LAYOUT_32_32:
                swap8in32(p, p, 2);
                break;

            case PixelFormat::LAYOUT_32_32_32:
                swap8in32(p, p, 3);
                break;

            case PixelFormat::LAYOUT_32_32_32_32:
                swap8in32(p, p, 4);
                break;

            // other cases
            default:
                break;
            }
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void sSwapIndexEndianInplace(void * buffer,
                             size_t bufferSize, // buffer size in bytes
                             bool   idx32) {
    if (idx32) {
        swap8in32(buffer, buffer, bufferSize / 4);
    } else {
        swap8in16(buffer, buffer, bufferSize / 2);
    }
}

//
//
// -----------------------------------------------------------------------------
const MeshVertexElement * sFindPositionElement(const MeshVertexFormat & vf) {
    for (size_t i = 0; i < vf.numElements; ++i) {
        const MeshVertexElement & e = vf.elements[i];

        if (0 == str::compareI("position", e.semantic) || 0 == str::compareI("position0", e.semantic) || 0 == str::compareI("pos", e.semantic) ||
            0 == str::compareI("pos0", e.semantic) || 0 == str::compareI("gl_vertex", e.semantic)) {
            return &e;
        }
    }

    GN_ERROR(sLogger)("Position semantice is not found in vertex format.");
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
bool sGetMeshVertexPositions(MeshVertexPosition & pos, const MeshResourceDesc & desc) {
    const MeshVertexElement * positionElement = sFindPositionElement(desc.vtxfmt);
    if (NULL == positionElement) return false;

    const float * vertices = (const float *) (((const uint8_t *) desc.vertices[positionElement->stream]) + positionElement->offset);

    if (PixelFormat::FLOAT1() == positionElement->format) {
        pos.x = vertices;
        pos.y = 0;
        pos.z = 0;
    } else if (PixelFormat::FLOAT2() == positionElement->format) {
        pos.x = vertices;
        pos.y = vertices + 1;
        pos.z = 0;
    } else if (PixelFormat::FLOAT3() == positionElement->format) {
        pos.x = vertices;
        pos.y = vertices + 1;
        pos.z = vertices + 2;
    } else if (PixelFormat::FLOAT4() == positionElement->format) {
        pos.x = vertices;
        pos.y = vertices + 1;
        pos.z = vertices + 2;
    } else {
        GN_ERROR(sLogger)("AABB calculation failed: unsupported vertex format %s", positionElement->format.toString().c_str());
        return false;
    }
    pos.strideX = pos.strideY = pos.strideZ = desc.strides[positionElement->stream];

    return true;
}

//
//
// -----------------------------------------------------------------------------
MeshFileType sDetermineMeshFileType(File & fp) {
    auto currentPos = fp.input().tellg();

    char buf[16];

    if (16 != fp.read(buf, 16)) {
        GN_ERROR(sLogger)("Fail to read mesh file header.");
        return MESH_FILE_UNKNOWN;
    }

    if (!fp.input().seekg(currentPos, std::ios::beg)) {
        GN_ERROR(sLogger)("Fail to seek mesh file reading cursor back.");
        return MESH_FILE_UNKNOWN;
    }

    if (0 == memcmp(buf, MESH_BINARY_TAG_V2, sizeof(MESH_BINARY_TAG_V2))) {
        return MESH_FILE_BIN;
    } else if (0 == str::compareI(buf, "<?xml", 5)) {
        return MESH_FILE_XML;
    } else {
        GN_ERROR(sLogger)("Unknown mesh file type.");
        return MESH_FILE_UNKNOWN;
    }
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> sLoadFromMeshBinaryFile(File & fp, MeshResourceDesc & desc) {
    MeshBinaryFileHeaderV2 header;

    if (sizeof(header) != fp.read(&header, sizeof(header))) {
        GN_ERROR(sLogger)("Fail to read mesh header.");
        return AutoRef<Blob>::NULLREF;
    }

    // verify header
    if (0 != memcmp(header.tag, MESH_BINARY_TAG_V2, sizeof(MESH_BINARY_TAG_V2))) {
        GN_ERROR(sLogger)("Unrecognized binary tag");
        return AutoRef<Blob>::NULLREF;
    }
    if (MESH_BINARY_ENDIAN_TAG_V2 != header.endian) {
        GN_ERROR(sLogger)("Unsupported endian.");
        return AutoRef<Blob>::NULLREF;
    }
    if (0x00010000 != header.version) // version must be 1.0
    {
        GN_ERROR(sLogger)("Unsupported mesh version.");
        return AutoRef<Blob>::NULLREF;
    }

    // analyze vertex format
    VertexFormatProperties vfp;
    if (!vfp.analyze(header.vtxfmt)) return AutoRef<Blob>::NULLREF;

    // read mesh data
    AutoRef<Blob> blob = referenceTo(new SimpleBlob(header.bytes));
    if (header.bytes != fp.read(blob->data(), header.bytes)) {
        GN_ERROR(sLogger)("fail to read mesh data.");
        return AutoRef<Blob>::NULLREF;
    }
    uint8_t * start = (uint8_t *) blob->data();

    desc.prim   = (PrimitiveType) header.prim;
    desc.numvtx = header.numvtx;
    desc.numidx = header.numidx;
    desc.idx32  = !!header.idx32;
    desc.dynavb = !!header.dynavb;
    desc.dynaib = !!header.dynaib;
    desc.vtxfmt = header.vtxfmt;
    for (size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i) {
        if (vfp.used[i]) {
            desc.vertices[i] = header.vertices[i] + start;
            desc.strides[i]  = (uint16_t) header.strides[i];
            desc.offsets[i]  = header.offsets[i];
        } else {
            desc.vertices[i] = NULL;
            desc.strides[i]  = 0;
            desc.offsets[i]  = 0;
        }
    }
    if (desc.numidx > 0) {
        desc.indices = header.indices + start;
    } else {
        desc.indices = NULL;
    }

    return blob;
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
static const XmlAttrib * sGetRequiredAttrib(const XmlElement & node, const char * attribName) {
    const XmlAttrib * a = node.findAttrib(attribName);

    if (!a) { GN_ERROR(sLogger)("Element <%s>: attribute \"%s\" is missing.", node.name.data(), attribName ? attribName : "!!!NULLPTR!!!"); }

    return a;
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
template<typename T>
static bool sGetIntAttrib(T & result, const XmlElement & node, const char * attribName) {
    const XmlAttrib * a = node.findAttrib(attribName);
    return a && 0 != str::toInetger<T>(result, a->value.data());
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
template<typename T>
static T sGetIntAttrib(const XmlElement & node, const char * attribName, T defaultValue) {
    T result;

    if (!sGetIntAttrib<T>(result, node, attribName))
        return defaultValue;
    else
        return result;
}

//
// get value of integer attribute
// -----------------------------------------------------------------------------
template<typename T>
static bool sGetRequiredIntAttrib(T & result, const XmlElement & node, const char * attribName) {
    const XmlAttrib * a = node.findAttrib(attribName);
    if (!a || 0 == str::toInetger<T>(result, a->value.data())) {
        GN_ERROR(sLogger)
        ("Element <%s>: attribute \"%s\" is missing or is not a valid integer.", node.name.data(), attribName ? attribName : "!!!NULLPTR!!!");
        return false;
    } else {
        return true;
    }
}

//
// get value of boolean attribute
// -----------------------------------------------------------------------------
static bool sGetBoolAttrib(const XmlElement & node, const char * attribName, bool defaultValue) {
    const XmlAttrib * a = node.findAttrib(attribName);
    if (!a) return defaultValue;

    if (0 == str::compareI("1", a->value.data()) || 0 == str::compareI("true", a->value.data())) {
        return true;
    } else if (0 == str::compareI("0", a->value.data()) || 0 == str::compareI("false", a->value.data())) {
        return false;
    } else {
        return defaultValue;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sReadV1BinaryFile(MeshBinaryHeaderV1 & header, uint8_t * dst, size_t length, const char * filename) {
    auto fp = fs::openFile(filename, std::ios::binary | std::ios::in);
    if (!fp) return false;

    if (sizeof(header) != fp->read(&header, sizeof(header))) {
        GN_ERROR(sLogger)("Fail to read garnet binary file header: %s", filename);
        return false;
    }

    if (0 != memcmp(header.tag, MESH_BINARY_TAG_V1, sizeof(header.tag))) {
        GN_ERROR(sLogger)("Invalid garnet V1 binary file: %s", filename);
        return false;
    }

    if (length != fp->read(dst, length)) {
        GN_ERROR(sLogger)("Fail to read binary data from file: %s", filename);
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
static PixelFormat fromString(const char * str) {
    struct ColorFormatName {
        PixelFormat  format;
        const char * name;
    };

    static const ColorFormatName TABLE[] = {
        {PixelFormat::FLOAT1(), "float1"},
        {PixelFormat::FLOAT2(), "float2"},
        {PixelFormat::FLOAT3(), "float3"},
        {PixelFormat::FLOAT4(), "float4"},
    };

    if (0 == str || 0 == *str) return PixelFormat::UNKNOWN();

    for (size_t i = 0; i < GN_ARRAY_COUNT(TABLE); ++i) {
        const ColorFormatName & n = TABLE[i];

        if (0 == str::compareI(n.name, str)) { return n.format; }
    }

    return PixelFormat::UNKNOWN();
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> sLoadFromMeshXMLFile(File & fp, MeshResourceDesc & desc) {
    desc = {};

    XmlDocument    doc;
    XmlParseResult xpr;
    if (!doc.parse(xpr, fp)) {
        GN_ERROR(sLogger)
        ("Fail to parse XML file (%s):\n"
         "    line   : %d\n"
         "    column : %d\n"
         "    error  : %s",
         fp.name().data(), xpr.errLine, xpr.errColumn, xpr.errInfo.data());
        return AutoRef<Blob>::NULLREF;
    }
    GN_ASSERT(xpr.root);

    const XmlElement * root = xpr.root->toElement();
    if (!root || root->name != "mesh") {
        GN_ERROR(sLogger)("Invalid root element.");
        return AutoRef<Blob>::NULLREF;
    }

    const XmlAttrib * a = root->findAttrib("primtype");
    if (!a || PrimitiveType::INVALID == (desc.prim = PrimitiveType::sFromString(a->value))) {
        GN_ERROR(sLogger)("Element <%s> attribute \"%s\": missing or invalid.", root->name.data(), "primtype");
        return AutoRef<Blob>::NULLREF;
    }

    if (!sGetRequiredIntAttrib(desc.numvtx, *root, "numvtx")) { return AutoRef<Blob>::NULLREF; }

    if (!sGetRequiredIntAttrib(desc.numidx, *root, "numidx")) { return AutoRef<Blob>::NULLREF; }

    desc.idx32  = sGetBoolAttrib(*root, "idx32", false);
    desc.dynavb = sGetBoolAttrib(*root, "dynavb", false);
    desc.dynaib = sGetBoolAttrib(*root, "dynaib", false);

    // get vertex format
    const XmlElement * vtxfmtNode = root->findChildElement("vtxfmt");
    if (!vtxfmtNode) {
        GN_ERROR(sLogger)("<vtxfmt> element is missing.");
        return AutoRef<Blob>::NULLREF;
    }
    for (const XmlNode * n = vtxfmtNode->firstc; n != NULL; n = n->nexts) {
        const XmlElement * e = n->toElement();
        if (!e) continue;

        if ("attrib" != e->name) {
            GN_WARN(sLogger)("Ignore unrecognized vertex format element: <%s>.", e->name.data());
            continue;
        }

        if (desc.vtxfmt.numElements >= MeshVertexFormat::MAX_VERTEX_ELEMENTS) {
            GN_ERROR(sLogger)("Too many vertex elements.");
            return AutoRef<Blob>::NULLREF;
        }
        MeshVertexElement & ve = desc.vtxfmt.elements[desc.vtxfmt.numElements];

        if (!sGetRequiredIntAttrib(ve.stream, *e, "stream") || !sGetRequiredIntAttrib(ve.offset, *e, "offset") ||
            NULL == (a = sGetRequiredAttrib(*e, "semantic"))) {
            return AutoRef<Blob>::NULLREF;
        }

        ve.setSemantic(a->value);

        a = e->findAttrib("format");
        if (!a || (PixelFormat::UNKNOWN() == (ve.format = fromString(a->value)))) {
            GN_ERROR(sLogger)("Missing or invalid format attribute.");
            return AutoRef<Blob>::NULLREF;
        }

        desc.vtxfmt.numElements++;
    }

    // parse vtxbuf and idxbuf elements, calculate mesh data size
    uint32_t meshDataSize = 0;
    for (const XmlNode * n = root->firstc; n != NULL; n = n->nexts) {
        const XmlElement * e = n->toElement();
        if (!e) continue;

        if ("vtxbuf" == e->name) {
            uint32_t stream, offset;
            uint16_t stride;
            if (!sGetRequiredIntAttrib(stream, *e, "stream") || !sGetRequiredIntAttrib(offset, *e, "offset") || !sGetRequiredIntAttrib(stride, *e, "stride") ||
                NULL == (a = sGetRequiredAttrib(*e, "ref"))) {
                return AutoRef<Blob>::NULLREF;
            }

            if (stream >= GpuContext::MAX_VERTEX_BUFFERS) {
                GN_WARN(sLogger)("vtxbuf stream is too large.");
                return AutoRef<Blob>::NULLREF;
            }

            desc.offsets[stream] = offset;
            desc.strides[stream] = stride;

            meshDataSize += stride * desc.numvtx;
        } else if ("idxbuf" == e->name) {
            meshDataSize += desc.numidx * (desc.idx32 ? 4 : 2);
        } else if ("vtxfmt" == e->name) {
            // silently ignored, since it is handled already.
        } else {
            GN_WARN(sLogger)("Ignore unrecognized element: <%s>.", e->name.data());
        }
    }

    AutoRef<SimpleBlob> blob = referenceTo(new SimpleBlob(meshDataSize));
    if (!blob) {
        GN_ERROR(sLogger)("Out of memory");
        return AutoRef<Blob>::NULLREF;
    }

    StrA basedir = fs::dirName(fp.name());

    // parse vtxbuf and idxbuf elements, again, to read, calculate mesh data size
    ArrayProxy<uint8_t> meshData((uint8_t *) blob->data(), blob->size());
    uint32_t                   offset = 0;
    for (const XmlNode * n = root->firstc; n != NULL; n = n->nexts) {
        const XmlElement * e = n->toElement();
        if (!e) continue;

        if ("vtxbuf" == e->name) {
            uint32_t stream = sGetIntAttrib<uint32_t>(*e, "stream", 0xFFFFFFFF);
            GN_ASSERT(stream < GpuContext::MAX_VERTEX_BUFFERS);

            a = sGetRequiredAttrib(*e, "ref");
            GN_ASSERT(a);

            GN::ArrayProxy<uint8_t> vb = meshData.subrange(offset, desc.strides[stream] * desc.numvtx);

            MeshBinaryHeaderV1 header;
            if (!sReadV1BinaryFile(header, vb.data(), vb.size(), fs::resolvePath(basedir, a->value))) { return AutoRef<Blob>::NULLREF; }

            if (header.endian != MESH_BINARY_ENDIAN_TAG_V1) { sSwapVertexEndianInplace(vb.data(), vb.size(), desc.vtxfmt, stream, desc.strides[stream]); }

            desc.vertices[stream] = vb.data();

            offset += vb.size();
        } else if ("idxbuf" == e->name) {
            a = sGetRequiredAttrib(*e, "ref");
            GN_ASSERT(a);

            auto ib = meshData.subrange(offset, desc.numidx * (desc.idx32 ? 4 : 2));

            MeshBinaryHeaderV1 header;
            if (!sReadV1BinaryFile(header, ib.data(), ib.size(), fs::resolvePath(basedir, a->value))) { return AutoRef<Blob>::NULLREF; }

            if (header.endian != MESH_BINARY_ENDIAN_TAG_V1) { sSwapIndexEndianInplace(ib.data(), ib.size(), desc.idx32); }

            desc.indices = ib.data();

            offset += ib.size();
        } else if ("vtxfmt" == e->name) {
            // silently ignored, since it is handled already.
        } else {
            GN_WARN(sLogger)("Ignore unrecognized element: <%s>.", e->name.data());
        }
    }

    return blob;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::MeshResourceDescBase::getVtxBufSize(uint32_t stream) const {
    if (stream >= GpuContext::MAX_VERTEX_BUFFERS) {
        GN_ERROR(sLogger)("invalid stream index.");
        return 0;
    }

    VertexFormatProperties vfp;
    if (!vfp.analyze(vtxfmt)) return 0;

    if (!vfp.used[stream]) return 0;

    uint32_t stride = strides[stream];
    if (0 == stride) stride = vfp.minStrides[stream];

    return numvtx * stride;
}

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::MeshResourceDescBase::getIdxBufSize() const { return numidx * (idx32 ? 4 : 2); }

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MeshResourceDesc::calculateBoundingBox(Box<float> & box) const {
    box.x = box.y = box.w = box.h = 0.0f;

    MeshVertexPosition positions;

    if (!sGetMeshVertexPositions(positions, *this)) return;

    GN::calculateBoundingBox(box, positions.x, positions.strideX, positions.y, positions.strideY, positions.z, positions.strideZ, numvtx);
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MeshResourceDesc::calculateBoundingSphere(Sphere<float> & sphere) const {
    sphere.center.set(0, 0, 0);
    sphere.radius = 0;

    MeshVertexPosition positions;

    if (!sGetMeshVertexPositions(positions, *this)) return;

    GN::calculateBoundingSphere(sphere, positions.x, positions.strideX, positions.y, positions.strideY, positions.z, positions.strideZ, numvtx);
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> GN::gfx::MeshResourceDesc::loadFromFile(File & fp) {
    *this = {};

    switch (sDetermineMeshFileType(fp)) {
    case MESH_FILE_XML:
        return sLoadFromMeshXMLFile(fp, *this);

    case MESH_FILE_BIN:
        return sLoadFromMeshBinaryFile(fp, *this);

    case MESH_FILE_UNKNOWN:
    default:
        return AutoRef<Blob>::NULLREF;
    };
}

//
//
// -----------------------------------------------------------------------------
AutoRef<Blob> GN::gfx::MeshResourceDesc::loadFromFile(const char * filename) {
    GN_INFO(sLogger)("Load mesh from file: %s", filename ? filename : "<null filename>");

    *this = {};

    auto fp = fs::openFile(filename, std::ios::binary | std::ios::in);
    if (!fp) return AutoRef<Blob>::NULLREF;

    return loadFromFile(*fp);
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::saveToFile(File & fp) const {
    VertexFormatProperties vfp;
    if (!vfp.analyze(vtxfmt)) return false;

    MeshBinaryFileHeaderV2 header;
    memcpy(header.tag, MESH_BINARY_TAG_V2, sizeof(MESH_BINARY_TAG_V2));
    header.endian  = MESH_BINARY_ENDIAN_TAG_V2;
    header.version = 0x00010000;
    header.prim    = this->prim;
    header.numvtx  = (uint32_t) this->numvtx;
    header.numidx  = (uint32_t) this->numidx;
    header.idx32   = this->idx32;
    header.dynavb  = this->dynavb;
    header.dynaib  = this->dynaib;
    header.vtxfmt  = this->vtxfmt;

    // calculate mesh data size and vb/ib offsets
    size_t vbsizes[GpuContext::MAX_VERTEX_BUFFERS];
    header.bytes = 0;
    for (size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i) {
        if (vfp.used[i]) {
            size_t stride = this->strides[i];
            if (0 == stride) stride = vfp.minStrides[i];
            vbsizes[i] = numvtx * stride;

            header.vertices[i] = header.bytes;
            header.bytes += (uint32_t) vbsizes[i];
        }
        header.strides[i] = (uint32_t) this->strides[i];
        header.offsets[i] = (uint32_t) this->offsets[i];
    }
    size_t ibsize;
    if (numidx > 0) {
        ibsize         = numidx * (this->idx32 ? 4 : 2);
        header.indices = header.bytes;
        header.bytes += (uint32_t) ibsize;
    } else {
        ibsize         = 0;
        header.indices = 0;
    }

    // write header
    if (sizeof(header) != fp.write(&header, sizeof(header))) {
        GN_ERROR(sLogger)("Fail to write mesh header.");
        return false;
    }

    // write vertex buffers
    for (size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i) {
        if (vfp.used[i]) {
            if (vbsizes[i] != fp.write(this->vertices[i], vbsizes[i])) {
                GN_ERROR(sLogger)("Fail to write vertex buffer %i", i);
                return false;
            }
        }
    }

    // write index buffer
    if (numidx > 0) {
        if (ibsize != fp.write(this->indices, ibsize)) {
            GN_ERROR(sLogger)("Fail to write index buffer");
            return false;
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::saveToFile(const char * filename) const {
    GN_INFO(sLogger)("Save mesh to file: %s", filename ? filename : "<null filename>");

    auto fp = fs::openFile(filename, std::ios::binary | std::ios::out);
    if (!fp) return false;
    return saveToFile(*fp);
}
