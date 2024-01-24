#include "pch.h"
#include "ase.h"
#include <unordered_map>

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.util.ase");

///
/// raw ASE material (has sub material information)
///
struct AseMaterialInternal : public AseMaterial {
    DynaArray<AseMaterialInternal> submaterials; ///< sub material array. could be empty.
};

///
/// ASE vertex
///
struct AseVertex {
    Vector3f            p; ///< position
    DynaArray<Vector3f> t; ///< texcoord
    DynaArray<Vector3f> n; ///< normal

    uint32 addTexcoord(const Vector3f & v) {
        for (uint32 i = 0; i < t.size(); ++i) {
            if (t[i] == v) return i;
        }
        t.append(v);
        return (uint32) (t.size() - 1);
    }

    uint32 addNormal(const Vector3f & v) {
        for (uint32 i = 0; i < n.size(); ++i) {
            if (n[i] == v) return i;
        }
        n.append(v);
        return (uint32) (n.size() - 1);
    }
};

///
/// ASE triangle face
///
struct AseFace {
    uint32   v[3];   ///< vertices (index into AseMeshInternal.vertices)
    uint32   t[3];   ///< texcoords (index into AseVertex.t)
    uint32   vn[3];  ///< normal (index into AseVertex.n)
    Vector3f fn;     ///< face normal
    uint32   smooth; ///< smooth group ID
    uint32   submat; ///< sub material ID
};

///
/// ASE face chunks (faces with same sub-material)
///
struct AseFaceChunk {
    uint32            submat; ///< submaterial ID
    DynaArray<uint32> faces;  ///< indices into AseMeshInternal.faces
};

///
/// ASE mesh object
///
struct AseMeshInternal {
    ///
    /// this group is loaded directly from ASE file.
    //@{
    uint32               timevalue;
    DynaArray<AseVertex> vertices; ///< vertex array
    DynaArray<AseFace>   faces;    ///< face array
    //@}

    //@{
    DynaArray<AseFaceChunk> chunks; ///< faces sorted by material
    Boxf                    bbox;   ///< bounding box of the mesh itself
    //@}
};

///
/// ASE node (elemnet for mesh hierachy)
///
struct AseNode {
    StrA      parent;
    StrA      name;
    Matrix44f transform;
    Vector3f  pos;
    Vector3f  rotaxis;
    float     rotangle; // rotation angle in radian
    Vector3f  scale;
    Boxf      selfbbox; ///< bounding box of myself
    Boxf      treebbox; ///< bounding box of myself and my children.
};

///
/// An complete ASE geometry object that includes a mesh, a node and a meterial
///
struct AseGeoObject : public GN::TreeNode<AseGeoObject> {
    AseNode         node;
    AseMeshInternal mesh;
    uint32          matid; ///< material ID into global material array
};

///
/// Internal ASE scene structure, stores raw ASE information.
///
struct AseSceneInternal {
    DynaArray<AseMaterialInternal> materials;
    DynaArray<AseGeoObject>        objects;
    AseGeoObject                   root; ///< root object
};

///
/// ASE file structure
///
struct AseFile {
    DynaArray<char> buf;
    char *          str;
    int             line;
    StrA            filedir; // directory of the ASE file

    bool open(File & file) {
        // read ASE file
        auto sz = file.size();
        buf.resize(sz + 1);
        if (sz != file.read(buf.rawptr(), sz)) { return false; }
        buf[sz] = 0;

        str  = buf.rawptr();
        line = 0;

        // get file dir
        filedir = GN::fs::dirName(file.name());

        // success
        return true;
    }

    void err(const StrA & msg) const { GN_ERROR(sLogger)("ASEFILE: line %d : %s", line, msg.rawptr()); }
    void warn(const StrA & msg) const { GN_WARN(sLogger)("ASEFILE: line %d : %s", line, msg.rawptr()); }
    void verbose(const StrA & msg) const { GN_VERBOSE(sLogger)("ASEFILE: line %d : %s", line, msg.rawptr()); }

    enum ScanOptionEnum {
        IN_CURRENT_BLOCK            = 0,
        IN_CURRENT_AND_CHILD_BLOCKS = 1,
        IN_ALL_BLOCKS               = 2,
        SILENCE                     = 4,
    };

    union ScanOption {
        uint32 u32;

        struct {
            unsigned int scope   : 2;
            unsigned int silence : 1;
            unsigned int _       : 29; ///< reserved
        };

        ScanOption(uint32 bits): u32(bits) {}
    };

    static inline bool isWhiteSpace(char ch) { return ' ' == ch || '\t' == ch || '\n' == ch || '\r' == ch; }

    ///
    /// skip white spaces
    ///
    void skipWhiteSpaces() {
        GN_ASSERT(str);
        while (isWhiteSpace(*str)) {
            if ('\n' == *str) ++line;
            ++str;
        }
    }

    ///
    /// get next token
    ///
    const char * next(const char * expectedValue = 0, ScanOption option = 0) {
        GN_ASSERT(str);

        skipWhiteSpaces();

        if (0 == str || 0 == *str) {
            if (!option.silence) verbose("EOF");
            return 0;
        }

        if ('"' == *str) {
            // quoted string

            const char * r = str;

            ++str;
            while ('"' != *str && 0 != *str) {
                if ('\n' == *str) ++line;
                ++str;
            }

            if (0 != *str) {
                GN_ASSERT('"' == *str);
                ++str;
                if ('\n' == *str) ++line;
                *str = 0;
                ++str;
            }

            if (expectedValue && 0 != str::compare(expectedValue, r)) {
                if (!option.silence) { err(str::format("expect '%s', but found '%s'.", expectedValue, r)); }
                return 0;
            }

            // success
            return r;

        } else {
            const char * r = str;

            while (!isWhiteSpace(*str)) { ++str; }

            if ('\n' == *str) ++line;

            if (0 != *str) *str = 0, ++str; // point to start of next token

            if (expectedValue && 0 != str::compare(expectedValue, r)) {
                if (!option.silence) { err(str::format("expect '%s', but found '%s'.", expectedValue, r)); }
                return 0;
            }

            return r;
        }
    }

    ///
    /// skip until meet specific token
    ///
    bool skipUntil(const char * endtoken, ScanOption option = 0) {
        GN_ASSERT(!str::isEmpty(endtoken));

        const char * token;
        int          level = 0;
        for (;;) {
            token = next();

            if (0 == str::compare("{", token))
                ++level;
            else if (0 == str::compare("}", token))
                --level;

            if (IN_CURRENT_BLOCK == option.scope && level > 0) continue; // skip sub levels

            if (IN_CURRENT_AND_CHILD_BLOCKS == option.scope && level < 0) {
                if (!option.silence) err(str::format("token '%s' not found inside current block!", endtoken));
                return false;
            }

            if (0 == token) {
                if (!option.silence) err(str::format("token '%s' not found!", endtoken));
                return false;
            }

            if (0 == str::compare(endtoken, token)) return true;
        }
    }

    //
    // True, if next token is a node (begin with '*')
    // -----------------------------------------------------------------------------
    bool nextIsNode() {
        skipWhiteSpaces();
        return '*' == *str;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool skipNode(ScanOption option = 0) {
        do {
            skipWhiteSpaces();
            if ('{' == *str) return skipUntil("}", option);
            if ('*' == *str || '}' == *str) return true; // found next node
        } while (next(0, option));
        return false;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readBlockStart(ScanOption option = 0) { return 0 != next("{", option); }

    //
    //
    // -----------------------------------------------------------------------------
    bool readBlockEnd(ScanOption option = 0) { return 0 != next("}", option); }

    //
    //
    // -----------------------------------------------------------------------------
    const char * readNode(ScanOption option = 0) {
        const char * token = next(0, option);
        if (!token) return NULL;
        GN_ASSERT(!str::isEmpty(token));

        if ('*' != *token) {
            if (!option.silence) err("Not a node token");
            return 0;
        }

        // success
        return token;
    }

    //
    //
    // -----------------------------------------------------------------------------
    const char * readString(ScanOption option = 0) {
        char * token = const_cast<char *>(next(0, option));
        if (!token) return NULL;
        GN_ASSERT(!str::isEmpty(token));

        if ('"' != *token) {
            if (!option.silence) err("Not a string token");
            return 0;
        }

        // trim quota
        ++token;
        GN_ASSERT((token + 2) <= str);
        GN_ASSERT('"' == token[str - token - 2]);
        token[str - token - 2] = 0;

        // success
        return token;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readString(StrA & result, ScanOption option = 0) {
        const char * s = readString(option);
        if (0 == s) return false;
        result = s;
        return true;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readAndResolveRelativePath(StrA & result, ScanOption option = 0) {
        StrA relpath;
        if (!readString(relpath, option)) return false;
        GN::fs::resolvePath(result, filedir, relpath);
        return true;
    }

    //
    // Note: symbol is a word w/o quotes and spaces.
    // -----------------------------------------------------------------------------
    bool readSymbol(StrA & result, ScanOption option = 0) {
        const char * s = next(0, option);
        if (0 == s) return false;
        if (('a' <= *s && *s <= 'z') || ('A' <= *s && *s <= 'Z') || '_' == *s) {
            result = s;
            return true;
        } else {
            err(str::format("Expect a symbol (start with [_a-zA-Z]), but met: %s", s));
            return false;
        }
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readFloat(float & result, ScanOption option = 0) {
        const char * token = next(0, option);

        if (str::toFloat(result, token))
            return true;
        else if (0 == str::compare("1.#QNB", token)) {
            result = 1.0f;
            return true;
        } else if (0 == str::compare("-1.#QNB", token)) {
            result = -1.0f;
            return true;
        } else {
            if (!option.silence) err(str::format("Not valid float : %s", token));
            return false;
        }
    }

    //
    //
    // -----------------------------------------------------------------------------
    template<typename INT_TYPE>
    bool readInt(INT_TYPE & result, ScanOption option = 0) {
        const char * token = next(0, option);
        if (0 == token) return false;

        if (0 == str::toInetger<INT_TYPE>(result, token)) {
            if (!option.silence) err(str::format("Not valid integer : %s", token));
            return false;
        }

        return true;
    }

    //
    //
    // -----------------------------------------------------------------------------
    template<typename INT_TYPE>
    INT_TYPE readOptionalInt(INT_TYPE defval, ScanOption option = 0) {
        INT_TYPE result;
        return readInt(result, option) ? result : defval;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readVector3(Vector3f & result, ScanOption option = 0) {
        return readFloat(result.x, option) && readFloat(result.y, option) && readFloat(result.z, option);
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readIndexedVector3Node(const char * nodename, uint32 index, Vector3f & result, ScanOption option = 0) {
        GN_ASSERT(!str::isEmpty(nodename));
        return next(nodename, option) && next(str::format("%d", index).rawptr(), option) && readVector3(result, option);
    }
};

//
//
// -----------------------------------------------------------------------------
static bool sReadMap(AseMap & m, AseFile & ase) {
    GN_GUARD;

    if (!ase.readBlockStart()) return false;

    const char * token;

    while (0 != (token = ase.next())) {
        if (0 == str::compare(token, "*MAP_NAME")) {
            if (!ase.readString(m.name)) return false;
        } else if (0 == str::compare(token, "*MAP_CLASS")) {
            if (!ase.readString(m.class_)) return false;
        } else if (0 == str::compare(token, "*MAP_SUBNO")) {
            if (!ase.readInt(m.subno)) return false;
        } else if (0 == str::compare(token, "*MAP_AMOUNT")) {
            if (!ase.readFloat(m.amount)) return false;
        } else if (0 == str::compare(token, "*BITMAP")) {
            if (!ase.readAndResolveRelativePath(m.bitmap)) return false;
        } else if (0 == str::compare(token, "*MAP_TYPE")) {
            if (!ase.readSymbol(m.type)) return false;
        } else if (0 == str::compare(token, "*UVW_U_OFFSET")) {
            if (!ase.readFloat(m.offset.s)) return false;
        } else if (0 == str::compare(token, "*UVW_V_OFFSET")) {
            if (!ase.readFloat(m.offset.t)) return false;
        } else if (0 == str::compare(token, "*UVW_U_TILING")) {
            if (!ase.readFloat(m.tiling.s)) return false;
        } else if (0 == str::compare(token, "*UVW_V_TILING")) {
            if (!ase.readFloat(m.tiling.t)) return false;
        } else if (0 == str::compare(token, "*UVW_ANGLE")) {
            if (!ase.readFloat(m.angle)) return false;
        } else if (0 == str::compare(token, "*UVW_BLUR")) {
            if (!ase.readFloat(m.blur)) return false;
        } else if (0 == str::compare(token, "*UVW_BLUR_OFFSET")) {
            if (!ase.readFloat(m.blur_offset)) return false;
        }
        // Note: this is 3dsmax ASE exporter bug. Should be MAP_NOISE_AMT
        else if (0 == str::compare(token, "*UVW_NOUSE_AMT")) {
            if (!ase.readFloat(m.noise_amt)) return false;
        } else if (0 == str::compare(token, "*UVW_NOISE_SIZE")) {
            if (!ase.readFloat(m.noise_size)) return false;
        } else if (0 == str::compare(token, "*UVW_NOISE_LEVEL")) {
            if (!ase.readInt(m.noise_level)) return false;
        } else if (0 == str::compare(token, "*UVW_NOISE_PHASE")) {
            if (!ase.readFloat(m.noise_phase)) return false;
        } else if (0 == str::compare(token, "*BITMAP_FILTER")) {
            if (!ase.readSymbol(m.filter)) return false;
        } else if ('*' == *token) {
            ase.verbose(str::format("skip node %s", token));
            if (!ase.skipNode()) return false;
        } else if (0 == str::compare(token, "}")) {
            // end of the block
            return true;
        } else {
            ase.err(str::format("expecting node or close-brace, but met '%s'!", token).rawptr());
            return false;
        }
    }

    // something wrong
    ase.err("fail to get next token!");
    return false;

    GN_UNGUARD;
}
//
//
// -----------------------------------------------------------------------------
static bool sReadMaterial(AseMaterialInternal & m, AseFile & ase) {
    GN_GUARD;

    if (!ase.readBlockStart()) return false;

    const char * token;

    while (0 != (token = ase.next())) {
        GN_ASSERT(!str::isEmpty(token));

        if (0 == str::compare(token, "*MATERIAL_NAME")) {
            if (!ase.readString(m.name)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_CLASS")) {
            if (!ase.readString(m.class_)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_AMBIENT")) {
            if (!ase.readVector3(m.ambient)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_DIFFUSE")) {
            if (!ase.readVector3(m.diffuse)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_SPECULAR")) {
            if (!ase.readVector3(m.specular)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_SHINE")) {
            if (!ase.readFloat(m.shine)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_SHINESTRENGTH")) {
            if (!ase.readFloat(m.shinestrength)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_TRANSPARENCY")) {
            if (!ase.readFloat(m.transparency)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_WIRESIZE")) {
            if (!ase.readFloat(m.wiresize)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_SHADING")) {
            if (!ase.readSymbol(m.shading)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_XP_FALLOFF")) {
            if (!ase.readFloat(m.xp_falloff)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_SELFILLUM")) {
            if (!ase.readFloat(m.selfillum)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_FALLOFF")) {
            if (!ase.readSymbol(m.falloff)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_XP_TYPE")) {
            if (!ase.readSymbol(m.xp_type)) return false;
        } else if (0 == str::compare(token, "*MAP_", 5)) {
            const char * map = token + 5;
            if (0 == str::compare(map, "DIFFUSE")) {
                if (!sReadMap(m.mapdiff, ase)) return false;
            } else if (0 == str::compare(map, "BUMP")) {
                if (!sReadMap(m.mapbump, ase)) return false;
            } else {
                ase.verbose(str::format("skip unsupport map %s", token));
                if (!ase.skipNode()) return false;
            }
        } else if (0 == str::compare(token, "*NUMSUBMTLS")) {
            uint32 count;
            if (!ase.readInt(count)) return false;
            m.submaterials.resize(count);

            // read sub-materials one by one
            for (uint32 i = 0; i < count; ++i) {
                if (!ase.next("*SUBMATERIAL")) return false;
                if (!ase.next(str::format("%d", i).rawptr())) return false;
                if (!sReadMaterial(m.submaterials[i], ase)) return false;
            }
        } else if ('*' == *token) {
            ase.verbose(str::format("skip node %s", token));
            if (!ase.skipNode()) return false;
        } else if (0 == str::compare(token, "}")) {
            // end of the block
            return true;
        } else {
            ase.err(str::format("expecting node or close-brace, but met '%s'!", token).rawptr());
            return false;
        }
    }

    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadMaterials(AseSceneInternal & scene, AseFile & ase) {
    GN_GUARD;

    ase.verbose("Read materials ...");

    if (!ase.readBlockStart()) return false;

    // read material count
    if (!ase.next("*MATERIAL_COUNT")) return false;
    uint32 matcount;
    if (!ase.readInt(matcount)) return false;

    scene.materials.resize(matcount);

    // read materials one by one
    for (uint32 i = 0; i < matcount; ++i) {
        if (!ase.next("*MATERIAL")) return false;
        if (!ase.next(str::format("%d", i).rawptr())) return false;
        if (!sReadMaterial(scene.materials[i], ase)) return false;
    }

    // done
    return ase.readBlockEnd();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadMesh(AseMeshInternal & m, const Matrix44f & transform, AseFile & ase) {
    GN_GUARD;

    if (!ase.readBlockStart()) return false;

    if (!ase.next("*TIMEVALUE") || !ase.readInt(m.timevalue)) return false;

    uint32 numvert, numface;
    if (!ase.next("*MESH_NUMVERTEX") || !ase.readInt(numvert)) return false;
    if (!ase.next("*MESH_NUMFACES") || !ase.readInt(numface)) return false;

    m.vertices.resize(numvert);
    m.faces.resize(numface);

    // read vertices
    if (!ase.next("*MESH_VERTEX_LIST") || !ase.readBlockStart()) return false;
    for (uint32 i = 0; i < numvert; ++i) {
        // Note: vertex position in ASE file is post-transformed.
        if (!ase.readIndexedVector3Node("*MESH_VERTEX", i, m.vertices[i].p)) return false;
    }
    if (!ase.readBlockEnd()) return false;

    // calculate mesh bounding box
    if (m.vertices.size() > 0) {
        calculateBoundingBox(m.bbox, &m.vertices[0].p, sizeof(AseVertex), m.vertices.size());
    } else {
        m.bbox.x = m.bbox.y = m.bbox.z = m.bbox.w = m.bbox.h = m.bbox.d = .0f;
    }

    // read faces
    if (!ase.next("*MESH_FACE_LIST") || !ase.readBlockStart()) return false;
    for (uint32 i = 0; i < numface; ++i) {
        AseFace & f = m.faces[i];
        int       dummy;
        if (!ase.next("*MESH_FACE")) return false;
        if (!ase.next(str::format("%d:", i).rawptr())) return false;
        if (!ase.next("A:") || !ase.readInt(f.v[0])) return false;
        if (!ase.next("B:") || !ase.readInt(f.v[1])) return false;
        if (!ase.next("C:") || !ase.readInt(f.v[2])) return false;
        if (!ase.next("AB:") || !ase.readInt(dummy)) return false;
        if (!ase.next("BC:") || !ase.readInt(dummy)) return false;
        if (!ase.next("CA:") || !ase.readInt(dummy)) return false;
        if (!ase.next("*MESH_SMOOTHING")) return false;
        if (!ase.nextIsNode()) {
            // then next must be smooth group ID
            if (!ase.readInt(f.smooth)) return false;
        } else {
            f.smooth = 0;
        }
        if (!ase.next("*MESH_MTLID") || !ase.readInt(f.submat)) return false;
    }
    if (!ase.readBlockEnd()) return false;

    // read texcoords
    uint32 numtexcoord;
    if (!ase.next("*MESH_NUMTVERTEX") || !ase.readInt(numtexcoord)) return false;
    if (numtexcoord > 0) {
        DynaArray<Vector3f> texcoords(numtexcoord);

        if (!ase.next("*MESH_TVERTLIST") || !ase.readBlockStart()) return false;
        for (uint32 i = 0; i < numtexcoord; ++i) {
            if (!ase.readIndexedVector3Node("*MESH_TVERT", i, texcoords[i])) return false;

            // Y axis has be to reversed.
            texcoords[i].y = 1.0f - texcoords[i].y;

            // NOTE: currently, we only supports 2D texcoord
            texcoords[i].z = .0f;
        }
        if (!ase.readBlockEnd()) return false;

        // read tface list
        if (!ase.next("*MESH_NUMTVFACES") || !ase.next(str::format("%d", numface).rawptr())) return false;
        if (!ase.next("*MESH_TFACELIST") || !ase.readBlockStart()) return false;
        for (uint32 i = 0; i < numface; ++i) {
            AseFace & f = m.faces[i];

            if (!ase.next("*MESH_TFACE")) return false;
            if (!ase.next(str::format("%d", i).rawptr())) return false;

            // for each vertex in the face
            for (uint32 j = 0; j < 3; ++j) {
                AseVertex & v = m.vertices[f.v[j]];

                // get the index into texcoord array
                uint32 t;
                if (!ase.readInt(t)) return false;

                // add to vertex's texcoord array, store index in the face.
                f.t[j] = v.addTexcoord(texcoords[t]);
            }
        }
        if (!ase.readBlockEnd()) return false;
    } else {
        // The mesh has no texcoord information.

        Vector3f zero(0, 0, 0);

        for (uint32 i = 0; i < m.vertices.size(); ++i) { m.vertices[i].addTexcoord(zero); }

        for (uint32 i = 0; i < m.faces.size(); ++i) {
            AseFace & f = m.faces[i];
            f.t[0]      = 0;
            f.t[1]      = 0;
            f.t[2]      = 0;
        }
    }

    // skip vertex colors
    uint32 numcolor;
    if (!ase.next("*MESH_NUMCVERTEX") || !ase.readInt(numcolor)) return false;
    if (numcolor > 0) {
        if (!ase.next("*MESH_CVERTLIST")) return false;
        if (!ase.skipNode()) return false;
    }

    // read normals
    if (!ase.next("*MESH_NORMALS") || !ase.readBlockStart()) return false;
    Matrix44f it = Matrix44f::sInvtrans(transform); // use to transform normal
    for (uint32 i = 0; i < numface; ++i) {
        AseFace & f = m.faces[i];
        if (!ase.readIndexedVector3Node("*MESH_FACENORMAL", i, f.fn)) return false;

        for (uint32 j = 0; j < 3; ++j) {
            if (!ase.next("*MESH_VERTEXNORMAL")) return false;

            uint32 vi;
            if (!ase.readInt(vi)) return false;

            AseVertex & v = m.vertices[vi];

            Vector3f n;
            if (!ase.readVector3(n)) return false;

            f.vn[j] = v.addNormal(it.transformVector(n));
        }
    }
    if (!ase.readBlockEnd()) return false;

    // end of mesh block
    return ase.readBlockEnd();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadNode(AseNode & n, AseFile & ase) {
    GN_GUARD;

    if (!ase.readBlockStart()) return false;

    n.transform.identity();

    const char * token;
    while (0 != (token = ase.next())) {
        if (0) {
        } else if (0 == str::compare("*TM_ROW0", token)) {
            Vector3f v;
            if (!ase.readVector3(v)) return false;
            n.transform[0][0] = v.x;
            n.transform[1][0] = v.y;
            n.transform[2][0] = v.z;
        } else if (0 == str::compare("*TM_ROW1", token)) {
            Vector3f v;
            if (!ase.readVector3(v)) return false;
            n.transform[0][1] = v.x;
            n.transform[1][1] = v.y;
            n.transform[2][1] = v.z;
        } else if (0 == str::compare("*TM_ROW2", token)) {
            Vector3f v;
            if (!ase.readVector3(v)) return false;
            n.transform[0][2] = v.x;
            n.transform[1][2] = v.y;
            n.transform[2][2] = v.z;
        } else if (0 == str::compare("*TM_ROW3", token)) {
            Vector3f v;
            if (!ase.readVector3(v)) return false;
            n.transform[0][3] = v.x;
            n.transform[1][3] = v.y;
            n.transform[2][3] = v.z;
        } else if (0 == str::compare("*TM_POS", token)) {
            if (!ase.readVector3(n.pos)) return false;
        } else if (0 == str::compare("*TM_ROTAXIS", token)) {
            if (!ase.readVector3(n.rotaxis)) return false;
        } else if (0 == str::compare("*TM_ROTANGLE", token)) {
            if (!ase.readFloat(n.rotangle)) return false;
        } else if (0 == str::compare("*TM_SCALE", token)) {
            if (!ase.readVector3(n.scale)) return false;
        } else if ('*' == *token) {
            ase.verbose(str::format("skip node %s", token));
            if (!ase.skipNode()) return false;
        } else if (0 == str::compare(token, "}")) {
            // end of the block. done.
            return true;
        }
    }

    // something wrong!
    ase.err("Fail to get next node!");
    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadGeomObject(AseSceneInternal & scene, AseFile & ase) {
    GN_GUARD;

    scene.objects.resize(scene.objects.size() + 1);
    AseGeoObject & o = scene.objects.back();

    if (!ase.readBlockStart()) return false;

    bool hasMaterial = false;

    const char * token;
    while (0 != (token = ase.next())) {
        if (0 == str::compare(token, "*NODE_NAME")) {
            o.node.name = ase.readString();
            if (o.node.name.empty()) {
                ase.err("Node name can't be empty!");
                return false;
            }
            ase.verbose(str::format("read geometry object '%s' ...", o.node.name.rawptr()));
        } else if (0 == str::compare(token, "*NODE_PARENT")) {
            o.node.parent = ase.readString();
        } else if (0 == str::compare(token, "*NODE_TM")) {
            if (!sReadNode(o.node, ase)) return false;
        } else if (0 == str::compare(token, "*MESH")) {
            if (!sReadMesh(o.mesh, o.node.transform, ase)) return false;
        } else if (0 == str::compare(token, "*MATERIAL_REF")) {
            if (!ase.readInt(o.matid)) return false;
            if (o.matid >= scene.materials.size()) {
                ase.err("material ID is out of range!");
                return false;
            }
            hasMaterial = true;
        } else if ('*' == *token) {
            ase.verbose(str::format("skip node %s", token));
            if (!ase.skipNode()) return false;
        } else if (0 == str::compare(token, "}")) {
            // end of the block. do some post processing.

            AseMeshInternal & m = o.mesh;

            if (!hasMaterial) {
                ase.warn(str::format("object '%s' has no material. Using default one.", o.node.name.rawptr()));
                o.matid = 0;
            }

            AseMaterialInternal & mtl = scene.materials[o.matid];

            // build face chunk array
            if ("Multi/Sub-Object" == mtl.class_) {
                m.chunks.reserve(mtl.submaterials.size());

                for (uint32 i = 0; i < m.faces.size(); ++i) {
                    const AseFace & f = m.faces[i];

                    uint32 cid;
                    for (cid = 0; cid < m.chunks.size(); ++cid) {
                        AseFaceChunk & c = m.chunks[cid];

                        if (c.submat == f.submat) break;
                    }
                    GN_ASSERT(cid <= m.chunks.size());

                    if (cid == m.chunks.size()) {
                        m.chunks.resize(m.chunks.size() + 1);
                        m.chunks.back().submat = f.submat;
                    }

                    m.chunks[cid].faces.append(i);
                }
            } else {
                m.chunks.resize(1);
                m.chunks[0].submat = 0;
                m.chunks[0].faces.resize(m.faces.size());
                for (uint32 i = 0; i < m.faces.size(); ++i) { m.chunks[0].faces[i] = i; }
            }

            // success
            return true;
        } else {
            ase.err(str::format("expecting node or close-brace, but met '%s'!", token).rawptr());
            return false;
        }
    }

    // something wrong!
    ase.err("Fail to get next node!");
    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadGroup(AseSceneInternal & scene, AseFile & ase) {
    GN_GUARD;

    if (!ase.readBlockStart()) return false;

    const char * token;

    while (0 != (token = ase.next())) {
        if (0) {
        }
        // else if( 0 == str::compare( token, "*SCENE" ) )
        //{
        //    ...
        //}
        else if (0 == str::compare(token, "*GEOMOBJECT")) {
            if (!sReadGeomObject(scene, ase)) return false;
        } else if ('*' == *token) {
            ase.verbose(str::format("skip node %s", token));
            if (!ase.skipNode()) return false;
        } else if (0 == str::compare(token, "}")) {
            // end of the block.
            return true;
        } else {
            ase.err(str::format("expecting node token, but met '%s'.", token));
            return false;
        }
    }

    // something wrong!
    ase.err("Fail to get next node!");
    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadAse(AseSceneInternal & scene, File & file) {
    GN_SCOPE_PROFILER(sReadAse, "Read ASE");

    GN_GUARD;

    // open ase file
    AseFile ase;
    if (!ase.open(file)) return false;

    // read ASE version
    const char * token = ase.next("*3DSMAX_ASCIIEXPORT");
    if (!token) return false;
    if (200 != ase.readOptionalInt(0)) {
        GN_ERROR(sLogger)("Not support ASE version other than 2.00!");
        return false;
    }

    while (0 != (token = ase.next())) {
        if (0) {
        }
        // else if( 0 == str::compare( token, "*SCENE" ) )
        //{
        //    ...
        //}
        else if (0 == str::compare(token, "*MATERIAL_LIST")) {
            if (!sReadMaterials(scene, ase)) return false;
        } else if (0 == str::compare(token, "*GROUP")) {
            if (!ase.readString()) return false; // skip group name
            if (!sReadGroup(scene, ase)) return false;
        } else if (0 == str::compare(token, "*GEOMOBJECT")) {
            if (!sReadGeomObject(scene, ase)) return false;
        } else if ('*' == *token) {
            ase.verbose(str::format("skip node %s", token));
            if (!ase.skipNode()) return false;
        } else {
            ase.err(str::format("expecting node token, but met '%s'.", token));
            return false;
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static AseGeoObject * sFindGeoObject(AseSceneInternal & scene, const StrA & name) {
    if (name.empty()) return &scene.root;
    for (AseGeoObject * o = scene.objects.begin(); o != scene.objects.end(); ++o) {
        if (name == o->node.name) return o;
    }
    return 0;
}

//
//
// -----------------------------------------------------------------------------
static bool sBuildNodeTree(AseSceneInternal & scene) {
    GN_SCOPE_PROFILER(sBuildNodeTree, "Build ASE node tree");

    GN_VERBOSE(sLogger)("\nASE: Build node tree:");

    // setup root node
    scene.root.node.name = "root";
    scene.root.node.pos.set(0, 0, 0);
    scene.root.node.rotaxis.set(0, 0, 1);
    scene.root.node.rotangle = 0;
    scene.root.node.scale.set(1, 1, 1);
    scene.root.node.transform.identity();
    scene.root.mesh.bbox.extend().set(0, 0, 0);

    // build node tree
    for (size_t i = 0; i < scene.objects.size(); ++i) {
        AseGeoObject & o = scene.objects[i];

        AseGeoObject * p = sFindGeoObject(scene, o.node.parent);

        if (0 == p) {
            GN_ERROR(sLogger)("Object %s has invalid parent: %s. Replace it with \"root\".", o.node.name.rawptr(), o.node.parent.rawptr());
            p = &scene.root;
        }

        o.setParent(p, 0);
    }

    // make sure all objects are linked into the tree.
    GN_ASSERT_EX(scene.root.calcChildrenCount() == scene.objects.size(),
                 str::format("numchildren=%d, numobjects=%d", scene.root.calcChildrenCount(), scene.objects.size()).rawptr());

    // calculate bounding box for each node, in post order
    TreeTraversePostOrder<AseGeoObject> ttpost(&scene.root);

    AseGeoObject * n = ttpost.first();
    GN_ASSERT(0 == n->getFirstChild());

    while (n) {
        // copy mesh bbox to node
        n->node.selfbbox = n->mesh.bbox;
        n->node.treebbox = n->mesh.bbox;

        // then merge with all childrens' bbox
        AseGeoObject * c = safeCastPtr<AseGeoObject>(n->getFirstChild());
        while (c) {
            Boxf::sGetUnion(n->node.treebbox, n->node.treebbox, c->node.treebbox);

            c = safeCastPtr<AseGeoObject>(c->getNextSibling());
        }

        // next node
        n = ttpost.next(n);
    }

    // print node tree
    TreeTraversePreOrder<AseGeoObject> ttpre(&scene.root);
    n         = ttpre.first();
    int level = 0;
    while (n) {
        StrA s("    ");

        for (int i = 0; i < level; ++i) s += "- ";
        s += str::format("%s : bbox_pos(%f,%f,%f), bbox_size(%f,%f,%f)", n->node.name.rawptr(), n->node.selfbbox.pos().x, n->node.selfbbox.pos().y,
                         n->node.selfbbox.pos().z, n->node.selfbbox.extend().x, n->node.selfbbox.extend().y, n->node.selfbbox.extend().z);

        GN_VERBOSE(sLogger)(s.rawptr());

        // next node
        n = ttpre.next(n, &level);
    }

    GN_VERBOSE(sLogger)("");

    // success
    return true;
}

/// unique vertex selector
struct VertexSelector {
    uint32 p; ///< position (index into AseMeshInternal.vertices)
    uint32 t; ///< texcoord (index into AseVertex.t)
    uint32 n; ///< normal   (index into AseVertex.n)

    friend bool operator==(const VertexSelector & a, const VertexSelector & b) { return a.p == b.p && a.t == b.t && a.n == b.n; }

    struct Hash {
        uint64 operator()(const VertexSelector & vs) const {
            uint64 h = ((((uint64) vs.p) << 32) | vs.t) ^ (((uint64) vs.n) << 16);
            return h;
        }
    };
};

/// collection of unique items
template<typename T>
class ElementCollection {
    typedef std::unordered_map<T, uint32, typename T::Hash> TypeMap;

    TypeMap      mMap;
    DynaArray<T> mBuffer;

public:
    ///
    /// Contructor
    ///
    ElementCollection(size_t potentialItemCount): mMap(potentialItemCount * 2) {}

    ///
    /// add element into buffer, ignore redundant element.
    ///
    uint32 add(const T & element) {
        auto inserted = mMap.insert({element, 0xbad});

        if (inserted.second) {
            // this is a new element
            auto & p = inserted.first->second;
            GN_ASSERT(0xbad == p); // make sure it is not initialized.
            GN_ASSERT(mBuffer.size() + 1 == mMap.size());
            p = (uint32) (mBuffer.size()); // assign correct value to p
            mBuffer.append(element);
            return p;
        } else {
            return mMap[element];
        }
    }

    ///
    /// get number of vertices in buffer
    ///
    size_t size() const {
        GN_ASSERT(mMap.size() == mBuffer.size());
        return mBuffer.size();
    }

    ///
    /// return specific element
    ///
    const T & operator[](size_t idx) const {
        GN_ASSERT(idx < mBuffer.size());
        return mBuffer[idx];
    }

    ///
    /// return specific element
    ///
    T & operator[](size_t idx) {
        GN_ASSERT(idx < mBuffer.size());
        return mBuffer[idx];
    }
};
typedef ElementCollection<VertexSelector> VertexCollection;

//
//
// -----------------------------------------------------------------------------
static bool operator==(const AseMaterial & a, const AseMaterial & b) {
    // TODO: check all fields.
    return a.name == b.name;
}

//
//
// -----------------------------------------------------------------------------
static uint32 sGetFaceChunkMatID(AseScene & dst, const AseSceneInternal & src, uint32 matid, uint32 submat) {
    const AseMaterial * mat;

    if ("Multi/Sub-Object" == src.materials[matid].class_) {
        mat = &src.materials[matid].submaterials[submat];
    } else {
        mat = &src.materials[matid];
    }

    for (uint32 i = 0; i < dst.materials.size(); ++i) {
        if (dst.materials[i] == *mat) {
            // this is a existing material
            return i;
        }
    }

    // this is a new material
    uint32 newidx = (uint32) dst.materials.size();
    dst.materials.resize(dst.materials.size() + 1);
    dst.materials.back() = *mat;
    return newidx;
}

struct OutputVertex {
    Vector3f position;
    Vector3f normal;
    Vector2f texcoord;
};

//
//
// -----------------------------------------------------------------------------
static bool sWriteGeoObject(AseScene & dst, const AseSceneInternal & src, const AseGeoObject & obj) {
    GN_GUARD;

    dst.meshes.resize(dst.meshes.size() + 1);

    AseMesh & dstmesh = dst.meshes.back();

    // setup hierarchy and transformation properties
    dstmesh.parent    = obj.node.parent;
    dstmesh.name      = obj.node.name;
    dstmesh.transform = obj.node.transform;
    dstmesh.pos       = obj.node.pos;
    dstmesh.rotaxis   = obj.node.rotaxis;
    dstmesh.rotangle  = obj.node.rotangle;
    dstmesh.scale     = obj.node.scale;
    dstmesh.selfbbox  = obj.node.selfbbox;
    dstmesh.treebbox  = obj.node.treebbox;

    // setup constant mesh properties
    dstmesh.vtxfmt.numElements        = 3;
    dstmesh.vtxfmt.elements[0].format = PixelFormat::FLOAT3();
    dstmesh.vtxfmt.elements[0].offset = 0;
    dstmesh.vtxfmt.elements[0].setSemantic("POSITION");
    dstmesh.vtxfmt.elements[1].format = PixelFormat::FLOAT3();
    dstmesh.vtxfmt.elements[1].offset = 12;
    dstmesh.vtxfmt.elements[1].setSemantic("NORMAL");
    dstmesh.vtxfmt.elements[2].format = PixelFormat::FLOAT2();
    dstmesh.vtxfmt.elements[2].offset = 24;
    dstmesh.vtxfmt.elements[2].setSemantic("TEXCOORD");
    dstmesh.prim       = PrimitiveType::TRIANGLE_LIST;
    dstmesh.strides[0] = sizeof(OutputVertex);

    // generate mesh
    VertexCollection  vc(obj.mesh.faces.size() * 3);
    DynaArray<uint32> ib; // index into vertex collection
    for (size_t i = 0; i < obj.mesh.chunks.size(); ++i) {
        const AseFaceChunk & c = obj.mesh.chunks[i];

        dst.subsets.resize(dst.subsets.size() + 1);
        AseMeshSubset & subset = dst.subsets.back();

        subset.matid    = sGetFaceChunkMatID(dst, src, obj.matid, c.submat);
        subset.meshid   = (uint32) dst.meshes.size() - 1;
        subset.startidx = (uint32) ib.size();
        subset.numidx   = (uint32) c.faces.size() * 3;

        uint32 minidx = 0xFFFFFFFF;
        uint32 maxidx = 0;

        for (size_t j = 0; j < c.faces.size(); ++j) {
            const AseFace & f = obj.mesh.faces[c.faces[j]];

            for (size_t k = 0; k < 3; ++k) {
                VertexSelector vs;
                vs.p = f.v[k];
                vs.t = f.t[k];
                vs.n = f.vn[k];

                uint32 idx = vc.add(vs);
                ib.append(idx);

                if (idx < minidx) minidx = idx;
                if (idx > maxidx) maxidx = idx;
            }
        }

        subset.basevtx = 0;
        subset.numvtx  = maxidx + 1;
    }

    // copy vertices into destination scene
    AutoRef<Blob>  blob     = referenceTo(new SimpleBlob((uint32) (sizeof(OutputVertex) * vc.size())));
    OutputVertex * vertices = (OutputVertex *) blob->data();
    if (NULL == vertices) return false;
    for (size_t i = 0; i < vc.size(); ++i) {
        const VertexSelector & vs = vc[i];

        const AseVertex & srcvert = obj.mesh.vertices[vs.p];

        const Vector3f & srctexcoord = srcvert.t[vs.t];

        OutputVertex & o = vertices[i];

        o.position = srcvert.p;
        o.normal   = srcvert.n[vs.n];
        o.texcoord = Vector2f(srctexcoord.x, srctexcoord.y);
    }
    dstmesh.numvtx      = (uint32) vc.size();
    dstmesh.vertices[0] = vertices;
    dst.meshdata.append(blob);

    // copy index data into destination scene
    dstmesh.numidx = (uint32) ib.size();
    if (vc.size() > 0x10000) {
        // 32bit index buffer
        blob = referenceTo(new SimpleBlob((uint32) (sizeof(uint32) * ib.size())));
        memcpy(blob->data(), ib.rawptr(), blob->size());
        dstmesh.idx32   = true;
        dstmesh.indices = blob->data();
        dst.meshdata.append(blob);
    } else {
        // 16bit index buffer
        blob           = referenceTo(new SimpleBlob((uint32) (sizeof(uint16) * ib.size())));
        uint16 * idx16 = (uint16 *) blob->data();
        for (size_t i = 0; i < ib.size(); ++i) {
            GN_ASSERT(ib[i] < 0x10000);
            idx16[i] = (uint16) ib[i];
        }
        dstmesh.idx32   = false;
        dstmesh.indices = idx16;
        dst.meshdata.append(blob);
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sWriteScene(AseScene & dst, const AseSceneInternal & src) {
    GN_SCOPE_PROFILER(sWriteScene, "Write ASE scene");

    GN_GUARD;

    for (size_t i = 0; i < src.objects.size(); ++i) { sWriteGeoObject(dst, src, src.objects[i]); }

    dst.bbox = src.root.node.treebbox;

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// public function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::AseScene::clear() {
    materials.clear();
    meshes.clear();
    subsets.clear();
    meshdata.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::AseScene::loadFromFile(File & file) {
    GN_SCOPE_PROFILER(AseScene_loadFromFile, "Load ASE scene from file");

    // clear existing content
    clear();

    AseSceneInternal internal;
    if (!sReadAse(internal, file)) return false;
    if (!sBuildNodeTree(internal)) return false;
    if (!sWriteScene(*this, internal)) return false;

    return true;
}
