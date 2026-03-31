#pragma once
// gltf-loader.h — minimal GLTF mesh loader (single-header prototype)
//
// Loads the first mesh/primitive from a .gltf + .bin file pair into a
// GpuDraw::GpuGeometry. Only handles: indexed meshes with float
// POSITION/NORMAL/TEXCOORD_0 and uint16 indices. All material, skin,
// and animation data is ignored.
//
// Usage:
//   auto geom = gltf::loadGltfGeometry("media::pbr/DamagedHelmet/DamagedHelmet.gltf", gpu);
//   if (geom.vertexCount == 0) { /* loading failed */ }

#include <garnet/GNrdg.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace gltf {

// ─── Minimal JSON parser ──────────────────────────────────────────────────────

// std::unordered_map<K, Json> inside Json requires Json to be complete, which it
// isn't during the struct definition. Use parallel vectors instead; linear search
// is fine for the small key counts in a GLTF file.
struct Json {
    enum class Type { Null, Bool, Num, Str, Arr, Obj };

    Type                     type = Type::Null;
    double                   num  = 0;
    std::string              str;
    std::vector<Json>        arr;
    std::vector<std::string> objKeys;
    std::vector<Json>        objVals;

    int    asInt() const { return static_cast<int>(num); }
    size_t size() const { return arr.size(); }

    bool has(const std::string & k) const {
        for (const auto & key : objKeys)
            if (key == k) return true;
        return false;
    }

    const Json & operator[](const std::string & k) const {
        for (size_t i = 0; i < objKeys.size(); ++i)
            if (objKeys[i] == k) return objVals[i];
        throw std::out_of_range("[gltf-loader] JSON key not found: " + k);
    }

    const Json & operator[](size_t i) const { return arr[i]; }
};

static void skipWs(const char *& p) {
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p;
}

static Json parseValue(const char *& p) {
    skipWs(p);
    Json v;
    if (*p == '{') {
        ++p;
        v.type = Json::Type::Obj;
        for (;;) {
            skipWs(p);
            if (*p == '}') {
                ++p;
                break;
            }
            if (*p == ',') {
                ++p;
                continue;
            }
            Json key = parseValue(p);
            skipWs(p);
            ++p; // ':'
            v.objKeys.push_back(key.str);
            v.objVals.push_back(parseValue(p));
        }
    } else if (*p == '[') {
        ++p;
        v.type = Json::Type::Arr;
        for (;;) {
            skipWs(p);
            if (*p == ']') {
                ++p;
                break;
            }
            if (*p == ',') {
                ++p;
                continue;
            }
            v.arr.push_back(parseValue(p));
        }
    } else if (*p == '"') {
        ++p;
        v.type = Json::Type::Str;
        while (*p != '"') {
            if (*p == '\\') ++p;
            v.str += *p++;
        }
        ++p;
    } else if (*p == 't') {
        p += 4;
        v.type = Json::Type::Bool;
        v.num  = 1;
    } else if (*p == 'f') {
        p += 5;
        v.type = Json::Type::Bool;
    } else if (*p == 'n') {
        p += 4;
    } else {
        v.type = Json::Type::Num;
        char * end;
        v.num = std::strtod(p, &end);
        p     = end;
    }
    return v;
}

// ─── GLTF mesh loader ─────────────────────────────────────────────────────────

/// Load the first mesh primitive from a .gltf file on disk.
/// Returns an empty geometry (vertexCount == 0) on failure.
inline GN::rdg::GpuDraw::GpuGeometry loadGltfGeometry(const std::string & gltfPath, GN::AutoRef<GN::rdg::GpuContext> gpu) {
    using Geom = GN::rdg::GpuDraw::GpuGeometry;
    using Fmt  = Geom::AttributeFormat;
    using PB   = GN::rdg::PersistentBuffer;

    // Parse JSON
    std::ifstream jf(gltfPath);
    if (!jf) {
        fprintf(stderr, "[gltf-loader] Cannot open: %s\n", gltfPath.c_str());
        return {};
    }
    std::string  text((std::istreambuf_iterator<char>(jf)), {});
    const char * p    = text.c_str();
    const Json   root = parseValue(p);

    // Read binary buffer
    std::string   dir     = gltfPath.substr(0, gltfPath.find_last_of("/\\") + 1);
    std::string   binPath = dir + root["buffers"][size_t(0)]["uri"].str;
    std::ifstream bf(binPath, std::ios::binary);
    if (!bf) {
        fprintf(stderr, "[gltf-loader] Cannot open: %s\n", binPath.c_str());
        return {};
    }
    std::vector<uint8_t> bin((std::istreambuf_iterator<char>(bf)), {});

    // BufferViews
    struct BV {
        int byteOffset, byteLength, byteStride;
    };
    std::vector<BV> bvs;
    for (size_t i = 0; i < root["bufferViews"].size(); ++i) {
        const Json & b = root["bufferViews"][i];
        bvs.push_back({b.has("byteOffset") ? b["byteOffset"].asInt() : 0, b["byteLength"].asInt(), b.has("byteStride") ? b["byteStride"].asInt() : 0});
    }

    // Accessors
    struct Acc {
        int         bv, byteOffset, count, compType;
        std::string type;
    };
    std::vector<Acc> accs;
    for (size_t i = 0; i < root["accessors"].size(); ++i) {
        const Json & a = root["accessors"][i];
        accs.push_back(
            {a["bufferView"].asInt(), a.has("byteOffset") ? a["byteOffset"].asInt() : 0, a["count"].asInt(), a["componentType"].asInt(), a["type"].str});
    }

    // First mesh, first primitive attributes
    const Json & prim    = root["meshes"][size_t(0)]["primitives"][size_t(0)];
    const Json & attrs   = prim["attributes"];
    int          idxAcc  = prim["indices"].asInt();
    int          posAcc  = attrs["POSITION"].asInt();
    int          normAcc = attrs["NORMAL"].asInt();
    int          uvAcc   = attrs["TEXCOORD_0"].asInt();

    // Read one float component from accessor element [elem][comp]
    auto readF32 = [&](const Acc & ac, int elem, int comp) -> float {
        const BV & bv     = bvs[ac.bv];
        int        comps  = (ac.type == "VEC3") ? 3 : (ac.type == "VEC2") ? 2 : 1;
        int        stride = bv.byteStride ? bv.byteStride : comps * 4;
        int        off    = bv.byteOffset + ac.byteOffset + elem * stride + comp * 4;
        float      val;
        std::memcpy(&val, bin.data() + off, 4);
        return val;
    };

    // Build interleaved vertices: position(F32×3) + normal(F32×3) + uv(F32×2) = 32 bytes
    struct Vtx {
        float px, py, pz, nx, ny, nz, u, v;
    };
    static_assert(sizeof(Vtx) == 32);
    int              vertCount = accs[posAcc].count;
    std::vector<Vtx> verts(vertCount);
    for (int i = 0; i < vertCount; ++i) {
        const Acc &pa = accs[posAcc], &na = accs[normAcc], &ua = accs[uvAcc];
        verts[i] = {readF32(pa, i, 0), readF32(pa, i, 1), readF32(pa, i, 2), readF32(na, i, 0),
                    readF32(na, i, 1), readF32(na, i, 2), readF32(ua, i, 0), readF32(ua, i, 1)};
    }

    // Index buffer (uint16)
    const Acc &           ia       = accs[idxAcc];
    const BV &            ibv      = bvs[ia.bv];
    int                   idxCount = ia.count;
    std::vector<uint16_t> idxData(idxCount);
    std::memcpy(idxData.data(), bin.data() + ibv.byteOffset + ia.byteOffset, idxCount * 2u);

    // Upload to GPU
    auto vbuf = PB::create("gltf_vb", PB::CreateParameters {.context = gpu, .size = (uint64_t) (vertCount * (int) sizeof(Vtx))});
    auto ibuf = PB::create("gltf_ib", PB::CreateParameters {.context = gpu, .size = (uint64_t) (idxCount * 2)});
    if (!vbuf || !vbuf->setContent(verts.data(), (uint64_t) (vertCount * (int) sizeof(Vtx))) || !ibuf ||
        !ibuf->setContent(idxData.data(), (uint64_t) (idxCount * 2))) {
        fprintf(stderr, "[gltf-loader] Failed to upload GPU buffers\n");
        return {};
    }

    // Build GpuGeometry
    Geom geom;
    geom.format.attributes.append(Geom::VertexAttribute {0, 0, Fmt::F32_3});  // position
    geom.format.attributes.append(Geom::VertexAttribute {1, 12, Fmt::F32_3}); // normal
    geom.format.attributes.append(Geom::VertexAttribute {2, 24, Fmt::F32_2}); // texcoord

    Geom::GeometryBuffer vg;
    vg.buffer = vbuf;
    vg.offset = 0;
    vg.stride = sizeof(Vtx);
    geom.vertices.append(vg);
    geom.vertexCount = (uint32_t) vertCount;

    geom.indices.buffer = ibuf;
    geom.indices.offset = 0;
    geom.indices.stride = 2;
    geom.indexCount     = (uint32_t) idxCount;

    return geom;
}

} // namespace gltf
