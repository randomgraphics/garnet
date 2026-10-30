#pragma once
#include <garnet/GNgpu2.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace gltf {

// Minimal JSON parser for glTF metadata
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
        for (const auto & key : objKeys) {
            if (key == k) return true;
        }
        return false;
    }

    const Json & operator[](const std::string & k) const {
        for (size_t i = 0; i < objKeys.size(); ++i) {
            if (objKeys[i] == k) return objVals[i];
        }
        throw std::out_of_range("[gltf-loader] JSON key not found: " + k);
    }

    const Json & operator[](size_t i) const { return arr[i]; }
};

inline void skipWs(const char *& p) {
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p;
}

inline Json parseValue(const char *& p);

inline std::string parseStr(const char *& p) {
    ++p; // skip opening quote
    std::string s;
    while (*p && *p != '"') {
        if (*p == '\\') {
            ++p;
            if (!*p) break;
        }
        s += *p++;
    }
    if (*p == '"') ++p;
    return s;
}

inline Json parseObj(const char *& p) {
    ++p; // skip '{'
    Json o;
    o.type = Json::Type::Obj;
    skipWs(p);
    if (*p == '}') {
        ++p;
        return o;
    }
    while (*p) {
        skipWs(p);
        if (*p != '"') break;
        std::string key = parseStr(p);
        skipWs(p);
        if (*p == ':') ++p;
        skipWs(p);
        o.objKeys.push_back(std::move(key));
        o.objVals.push_back(parseValue(p));
        skipWs(p);
        if (*p == ',') {
            ++p;
            continue;
        }
        if (*p == '}') {
            ++p;
            break;
        }
        break;
    }
    return o;
}

inline Json parseArr(const char *& p) {
    ++p; // skip '['
    Json a;
    a.type = Json::Type::Arr;
    skipWs(p);
    if (*p == ']') {
        ++p;
        return a;
    }
    while (*p) {
        a.arr.push_back(parseValue(p));
        skipWs(p);
        if (*p == ',') {
            ++p;
            continue;
        }
        if (*p == ']') {
            ++p;
            break;
        }
        break;
    }
    return a;
}

inline Json parseValue(const char *& p) {
    skipWs(p);
    Json v;
    if (!*p) return v;
    if (*p == '"') {
        v.type = Json::Type::Str;
        v.str  = parseStr(p);
    } else if (*p == '{') {
        return parseObj(p);
    } else if (*p == '[') {
        return parseArr(p);
    } else if (*p == 't' || *p == 'f') {
        v.type = Json::Type::Bool;
        v.num  = (*p == 't') ? 1.0 : 0.0;
        while (*p && *p != ',' && *p != '}' && *p != ']' && *p != ' ' && *p != '\n') ++p;
    } else if (*p == 'n') {
        v.type = Json::Type::Null;
        p += 4;
    } else {
        char * end = nullptr;
        v.num      = std::strtod(p, &end);
        v.type     = Json::Type::Num;
        p          = end;
    }
    return v;
}

/// Load the first mesh primitive from a .gltf file on disk into gpu2::RasterGeometry.
inline GN::gpu2::RasterGeometry loadGltfGeometry(const std::string & gltfPath, GN::AutoRef<GN::gpu2::GpuContext> gpu, GN::gpu2::bindless::CnC & uploads) {
    std::ifstream jf(gltfPath);
    if (!jf) return {};

    std::string  text((std::istreambuf_iterator<char>(jf)), {});
    const char * p    = text.c_str();
    const Json   root = parseValue(p);

    std::string   dir     = gltfPath.substr(0, gltfPath.find_last_of("/\\") + 1);
    std::string   binPath = dir + root["buffers"][size_t(0)]["uri"].str;
    std::ifstream bf(binPath, std::ios::binary);
    if (!bf) return {};

    std::vector<uint8_t> bin((std::istreambuf_iterator<char>(bf)), {});

    struct BV {
        int byteOffset, byteLength, byteStride;
    };
    std::vector<BV> bvs;
    for (size_t i = 0; i < root["bufferViews"].size(); ++i) {
        const Json & b = root["bufferViews"][i];
        bvs.push_back({b.has("byteOffset") ? b["byteOffset"].asInt() : 0, b["byteLength"].asInt(), b.has("byteStride") ? b["byteStride"].asInt() : 0});
    }

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

    const Json & prim    = root["meshes"][size_t(0)]["primitives"][size_t(0)];
    const Json & attrs   = prim["attributes"];
    int          idxAcc  = prim["indices"].asInt();
    int          posAcc  = attrs["POSITION"].asInt();
    int          normAcc = attrs["NORMAL"].asInt();
    int          uvAcc   = attrs["TEXCOORD_0"].asInt();

    auto readF32 = [&](const Acc & ac, int elem, int comp) -> float {
        const BV & bv     = bvs[ac.bv];
        int        comps  = (ac.type == "VEC3") ? 3 : (ac.type == "VEC2") ? 2 : 1;
        int        stride = bv.byteStride ? bv.byteStride : comps * 4;
        int        off    = bv.byteOffset + ac.byteOffset + elem * stride + comp * 4;
        float      val    = 0.0f;
        std::memcpy(&val, bin.data() + off, 4);
        return val;
    };

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

    const Acc &           ia       = accs[idxAcc];
    const BV &            ibv      = bvs[ia.bv];
    int                   idxCount = ia.count;
    std::vector<uint16_t> idxData(idxCount);
    std::memcpy(idxData.data(), bin.data() + ibv.byteOffset + ia.byteOffset, idxCount * 2u);

    auto vbuf = GN::gpu2::Buffer::create("gltf_vb", {.context = gpu, .size = static_cast<uint64_t>(vertCount * sizeof(Vtx))});
    auto ibuf = GN::gpu2::Buffer::create("gltf_ib", {.context = gpu, .size = static_cast<uint64_t>(idxCount * 2)});
    if (!vbuf || !ibuf) return {};

    uploads.recordUploadBuffer(vbuf, 0, {reinterpret_cast<const uint8_t *>(verts.data()), vertCount * sizeof(Vtx)});
    uploads.recordUploadBuffer(ibuf, 0, {reinterpret_cast<const uint8_t *>(idxData.data()), idxCount * 2u});

    GN::gpu2::RasterGeometry geom;
    geom.vertices.push_back({.buffer = vbuf, .offset = 0, .stride = sizeof(Vtx)});
    geom.format.attributes.push_back({.location = 0, .binding = 0, .offset = 0, .format = GN::gpu2::RasterGeometry::AttributeFormat::F32_3});
    geom.format.attributes.push_back({.location = 1, .binding = 0, .offset = 12, .format = GN::gpu2::RasterGeometry::AttributeFormat::F32_3});
    geom.format.attributes.push_back({.location = 2, .binding = 0, .offset = 24, .format = GN::gpu2::RasterGeometry::AttributeFormat::F32_2});
    geom.vertexCount = static_cast<uint32_t>(vertCount);
    geom.indices     = {.buffer = ibuf, .offset = 0, .stride = 2};
    geom.indexCount  = static_cast<uint32_t>(idxCount);
    return geom;
}

} // namespace gltf
