#include "pch.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/common.hpp>
#include <functional>
#include <memory>

static GN::Logger * sLogger = GN::getLogger("GN.fx2.model");

namespace GN::fx2 {

namespace {

using Bounds = ModelScene::Bounds;

void include(Bounds & bounds, const glm::vec3 & point) {
    if (!bounds.valid) {
        bounds.minimum = point;
        bounds.maximum = point;
        bounds.valid   = true;
        return;
    }
    bounds.minimum = glm::min(bounds.minimum, point);
    bounds.maximum = glm::max(bounds.maximum, point);
}

void include(Bounds & bounds, const Bounds & other) {
    if (!other.valid) return;
    include(bounds, other.minimum);
    include(bounds, other.maximum);
}

Bounds transformBounds(const Bounds & bounds, const glm::mat4 & transform) {
    Bounds result;
    if (!bounds.valid) return result;

    for (uint32_t corner = 0; corner < 8; ++corner) {
        const glm::vec3 point((corner & 1) ? bounds.maximum.x : bounds.minimum.x, (corner & 2) ? bounds.maximum.y : bounds.minimum.y,
                              (corner & 4) ? bounds.maximum.z : bounds.minimum.z);
        const glm::vec4 transformed = transform * glm::vec4(point, 1.0f);
        include(result, glm::vec3(transformed) / transformed.w);
    }
    return result;
}

glm::mat4 toGlm(const aiMatrix4x4 & m) {
    // Assimp stores row-major members while GLM's constructor accepts columns.
    return glm::mat4(m.a1, m.b1, m.c1, m.d1, m.a2, m.b2, m.c2, m.d2, m.a3, m.b3, m.c3, m.d3, m.a4, m.b4, m.c4, m.d4);
}

StrA resolveSourcePath(const StrA & path) {
    StrA nativePath = fs::toNativeDiskFilePath(path);
    if (!nativePath.empty() && fs::isFile(nativePath)) return nativePath;

    nativePath = fs::resolvePath(fs::getCurrentDir(), path);
    return fs::isFile(nativePath) ? nativePath : StrA {};
}

} // namespace

ModelScene::SourceFormat ModelScene::sourceFormatFromPath(const StrA & path) {
    StrA extension = fs::extName(path);
    extension.toLower();

    if (extension == ".fbx") return SourceFormat::FBX;
    if (extension == ".gltf") return SourceFormat::GLTF;
    if (extension == ".glb") return SourceFormat::GLB;
    if (extension == ".stl") return SourceFormat::STL;
    return SourceFormat::UNKNOWN;
}

AutoRef<ModelScene> ModelScene::load(const LoadParameters & parameters) {
    const SourceFormat format = sourceFormatFromPath(parameters.path);
    if (format == SourceFormat::UNKNOWN) GN_UNLIKELY {
            GN_ERROR(sLogger, "ModelScene::load: unsupported model path '{}'", parameters.path);
            return {};
        }

    const StrA nativePath = resolveSourcePath(parameters.path);
    if (nativePath.empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "ModelScene::load: cannot resolve model path '{}'", parameters.path);
            return {};
        }

    constexpr unsigned importFlags =
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_SortByPType | aiProcess_ValidateDataStructure;
    std::unique_ptr<const aiScene, decltype(&aiReleaseImport)> imported(aiImportFile(nativePath.data(), importFlags), &aiReleaseImport);
    if (!imported || !imported->mRootNode || !imported->HasMeshes()) GN_UNLIKELY {
            GN_ERROR(sLogger, "ModelScene::load: Assimp failed to import '{}': {}", parameters.path, aiGetErrorString());
            return {};
        }

    AutoRef<ModelScene> result(new ModelScene(TYPE_INFO(), "model-scene"));
    result->sourceFormat = format;
    result->sourcePath   = nativePath;
    result->materials.append(Material {});

    DynaArray<uint32_t> meshToPrimitive;
    meshToPrimitive.resize(imported->mNumMeshes);
    for (uint32_t & primitiveIndex : meshToPrimitive) primitiveIndex = UINT32_MAX;
    for (uint32_t meshIndex = 0; meshIndex < imported->mNumMeshes; ++meshIndex) {
        const aiMesh & mesh = *imported->mMeshes[meshIndex];
        if (!mesh.HasPositions() || !mesh.HasFaces()) continue;

        Primitive primitive;
        primitive.name               = mesh.mName.C_Str();
        primitive.material           = 0;
        primitive.sourceHadNormals   = mesh.HasNormals();
        primitive.sourceHadTangents  = mesh.HasTangentsAndBitangents();
        primitive.sourceHadTexcoords = mesh.HasTextureCoords(0);
        primitive.sourceHadColors    = mesh.HasVertexColors(0);
        primitive.vertices.resize(mesh.mNumVertices);

        for (uint32_t vertexIndex = 0; vertexIndex < mesh.mNumVertices; ++vertexIndex) {
            Vertex &           vertex   = primitive.vertices[vertexIndex];
            const aiVector3D & position = mesh.mVertices[vertexIndex];
            vertex.position             = {position.x, position.y, position.z};
            include(primitive.bounds, vertex.position);

            if (primitive.sourceHadNormals) {
                const aiVector3D & normal = mesh.mNormals[vertexIndex];
                vertex.normal             = {normal.x, normal.y, normal.z};
            }
            if (primitive.sourceHadTangents) {
                const aiVector3D & tangent = mesh.mTangents[vertexIndex];
                vertex.tangent             = {tangent.x, tangent.y, tangent.z, 1.0f};
            }
            if (primitive.sourceHadTexcoords) {
                const aiVector3D & texcoord = mesh.mTextureCoords[0][vertexIndex];
                vertex.texcoord             = {texcoord.x, texcoord.y};
            }
            if (primitive.sourceHadColors) {
                const aiColor4D & color = mesh.mColors[0][vertexIndex];
                vertex.color            = {color.r, color.g, color.b, color.a};
            }
        }

        for (uint32_t faceIndex = 0; faceIndex < mesh.mNumFaces; ++faceIndex) {
            const aiFace & face = mesh.mFaces[faceIndex];
            if (face.mNumIndices != 3) continue;
            primitive.indices.append(face.mIndices[0]);
            primitive.indices.append(face.mIndices[1]);
            primitive.indices.append(face.mIndices[2]);
        }
        if (primitive.indices.empty()) continue;

        meshToPrimitive[meshIndex] = static_cast<uint32_t>(result->primitives.size());
        result->primitives.append(std::move(primitive));
    }

    if (result->primitives.empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "ModelScene::load: '{}' contains no renderable triangles", parameters.path);
            return {};
        }

    std::function<Bounds(const aiNode &, int32_t, const glm::mat4 &)> importNode;
    importNode = [&](const aiNode & source, int32_t parent, const glm::mat4 & parentTransform) {
        const uint32_t nodeIndex = static_cast<uint32_t>(result->nodes.size());
        Node           node;
        node.name      = source.mName.C_Str();
        node.parent    = parent;
        node.transform = toGlm(source.mTransformation);
        result->nodes.append(std::move(node));

        const glm::mat4 modelTransform = parentTransform * result->nodes[nodeIndex].transform;
        Bounds          subtreeBounds;
        for (uint32_t i = 0; i < source.mNumMeshes; ++i) {
            const uint32_t meshIndex = source.mMeshes[i];
            if (meshIndex >= meshToPrimitive.size()) continue;
            const uint32_t primitiveIndex = meshToPrimitive[meshIndex];
            if (primitiveIndex == UINT32_MAX) continue;
            result->nodes[nodeIndex].primitives.append(primitiveIndex);
            include(subtreeBounds, transformBounds(result->primitives[primitiveIndex].bounds, modelTransform));
        }

        for (uint32_t i = 0; i < source.mNumChildren; ++i) {
            include(subtreeBounds, importNode(*source.mChildren[i], static_cast<int32_t>(nodeIndex), modelTransform));
        }
        result->nodes[nodeIndex].bounds = subtreeBounds;
        return subtreeBounds;
    };

    result->bounds = importNode(*imported->mRootNode, -1, glm::mat4(1.0f));
    if (!result->bounds.valid) GN_UNLIKELY {
            GN_ERROR(sLogger, "ModelScene::load: '{}' has no bounded scene nodes", parameters.path);
            return {};
        }
    return result;
}

} // namespace GN::fx2
