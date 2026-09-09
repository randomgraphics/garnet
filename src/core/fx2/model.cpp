#include "pch.h"

#include <assimp/cimport.h>
#include <assimp/GltfMaterial.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
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

int32_t appendTexture(ModelScene & result, const aiScene & scene, const aiMaterial & material, aiTextureType type, const StrA & modelDirectory) {
    aiString sourcePath;
    if (AI_SUCCESS != aiGetMaterialTexture(&material, type, 0, &sourcePath)) return -1;

    ModelScene::Texture texture;
    if (const aiTexture * embedded = scene.GetEmbeddedTexture(sourcePath.C_Str())) {
        if (embedded->mHeight == 0) {
            texture.embeddedData.append(reinterpret_cast<const uint8_t *>(embedded->pcData), embedded->mWidth);
            texture.mimeType = StrA::format("image/{}", embedded->achFormatHint);
        } else {
            result.warnings.append(StrA::format("Ignoring uncompressed embedded texture '{}'", sourcePath.C_Str()));
            return -1;
        }
    } else {
        texture.path = fs::resolvePath(modelDirectory, sourcePath.C_Str());
        if (!fs::isFile(texture.path)) {
            result.warnings.append(StrA::format("Cannot resolve texture '{}'", sourcePath.C_Str()));
            return -1;
        }
    }

    const int32_t index = static_cast<int32_t>(result.textures.size());
    result.textures.append(std::move(texture));
    return index;
}

void generateNormals(ModelScene::Primitive & primitive) {
    for (ModelScene::Vertex & vertex : primitive.vertices) vertex.normal = glm::vec3(0.0f);
    for (size_t i = 0; i < primitive.indices.size(); i += 3) {
        const uint32_t  i0 = primitive.indices[i];
        const uint32_t  i1 = primitive.indices[i + 1];
        const uint32_t  i2 = primitive.indices[i + 2];
        const glm::vec3 faceNormal =
            glm::cross(primitive.vertices[i1].position - primitive.vertices[i0].position, primitive.vertices[i2].position - primitive.vertices[i0].position);
        primitive.vertices[i0].normal += faceNormal;
        primitive.vertices[i1].normal += faceNormal;
        primitive.vertices[i2].normal += faceNormal;
    }
    for (ModelScene::Vertex & vertex : primitive.vertices) {
        const float lengthSquared = glm::dot(vertex.normal, vertex.normal);
        vertex.normal             = lengthSquared > 0.0f ? vertex.normal / glm::sqrt(lengthSquared) : glm::vec3(0, 1, 0);
    }
}

void generateTangents(ModelScene::Primitive & primitive) {
    DynaArray<glm::vec3> accumulated;
    accumulated.resize(primitive.vertices.size());
    for (glm::vec3 & tangent : accumulated) tangent = glm::vec3(0.0f);

    for (size_t i = 0; i < primitive.indices.size(); i += 3) {
        const uint32_t  i0          = primitive.indices[i];
        const uint32_t  i1          = primitive.indices[i + 1];
        const uint32_t  i2          = primitive.indices[i + 2];
        const glm::vec3 edge1       = primitive.vertices[i1].position - primitive.vertices[i0].position;
        const glm::vec3 edge2       = primitive.vertices[i2].position - primitive.vertices[i0].position;
        const glm::vec2 uv1         = primitive.vertices[i1].texcoord - primitive.vertices[i0].texcoord;
        const glm::vec2 uv2         = primitive.vertices[i2].texcoord - primitive.vertices[i0].texcoord;
        const float     determinant = uv1.x * uv2.y - uv1.y * uv2.x;
        if (glm::abs(determinant) <= 0.000001f) continue;
        const glm::vec3 tangent = (edge1 * uv2.y - edge2 * uv1.y) / determinant;
        accumulated[i0] += tangent;
        accumulated[i1] += tangent;
        accumulated[i2] += tangent;
    }

    for (size_t i = 0; i < primitive.vertices.size(); ++i) {
        const glm::vec3 normal        = primitive.vertices[i].normal;
        glm::vec3       tangent       = accumulated[i] - normal * glm::dot(normal, accumulated[i]);
        const float     lengthSq      = glm::dot(tangent, tangent);
        tangent                       = lengthSq > 0.0f ? tangent / glm::sqrt(lengthSq) : glm::vec3(1, 0, 0);
        primitive.vertices[i].tangent = glm::vec4(tangent, 1.0f);
    }
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

    const StrA modelDirectory = fs::dirName(nativePath);
    for (uint32_t materialIndex = 0; materialIndex < imported->mNumMaterials; ++materialIndex) {
        const aiMaterial & source = *imported->mMaterials[materialIndex];
        Material           material;
        aiString           name;
        aiColor4D          color;
        float              scalar = 0.0f;
        int                flag   = 0;

        if (AI_SUCCESS == aiGetMaterialString(&source, AI_MATKEY_NAME, &name)) material.name = name.C_Str();
        if (AI_SUCCESS == aiGetMaterialColor(&source, AI_MATKEY_BASE_COLOR, &color) ||
            AI_SUCCESS == aiGetMaterialColor(&source, AI_MATKEY_COLOR_DIFFUSE, &color)) {
            material.baseColor = {color.r, color.g, color.b, color.a};
        }
        if (AI_SUCCESS == aiGetMaterialColor(&source, AI_MATKEY_COLOR_EMISSIVE, &color)) material.emissive = {color.r, color.g, color.b};
        if (AI_SUCCESS == aiGetMaterialFloat(&source, AI_MATKEY_METALLIC_FACTOR, &scalar)) material.metallic = glm::clamp(scalar, 0.0f, 1.0f);
        if (AI_SUCCESS == aiGetMaterialFloat(&source, AI_MATKEY_ROUGHNESS_FACTOR, &scalar)) material.roughness = glm::clamp(scalar, 0.0f, 1.0f);
        if (AI_SUCCESS == aiGetMaterialFloat(&source, AI_MATKEY_GLTF_ALPHACUTOFF, &scalar)) material.alphaCutoff = scalar;
        if (AI_SUCCESS == aiGetMaterialInteger(&source, AI_MATKEY_TWOSIDED, &flag)) material.doubleSided = flag != 0;

        aiString alphaMode;
        if (AI_SUCCESS == aiGetMaterialString(&source, AI_MATKEY_GLTF_ALPHAMODE, &alphaMode)) {
            if (alphaMode == aiString("MASK")) material.alphaMode = AlphaMode::MASK;
            if (alphaMode == aiString("BLEND")) material.alphaMode = AlphaMode::BLEND;
        }

        material.workflow =
            (format == SourceFormat::GLTF || format == SourceFormat::GLB) ? MaterialWorkflow::METALLIC_ROUGHNESS : MaterialWorkflow::DEFAULT_LIT;
        if (AI_SUCCESS == aiGetMaterialFloat(&source, AI_MATKEY_GLOSSINESS_FACTOR, &scalar)) {
            material.workflow  = MaterialWorkflow::SPECULAR_GLOSSINESS;
            material.roughness = 1.0f - glm::clamp(scalar, 0.0f, 1.0f);
        }
        if (AI_SUCCESS == aiGetMaterialInteger(&source, "$mat.gltf.unlit", 0, 0, &flag) && flag != 0) material.workflow = MaterialWorkflow::UNLIT;
        material.baseColorMap = appendTexture(*result, *imported, source, aiTextureType_BASE_COLOR, modelDirectory);
        if (material.baseColorMap < 0) material.baseColorMap = appendTexture(*result, *imported, source, aiTextureType_DIFFUSE, modelDirectory);
        material.normalMap = appendTexture(*result, *imported, source, aiTextureType_NORMALS, modelDirectory);
        if (material.normalMap < 0) material.normalMap = appendTexture(*result, *imported, source, aiTextureType_HEIGHT, modelDirectory);
        material.emissiveMap   = appendTexture(*result, *imported, source, aiTextureType_EMISSIVE, modelDirectory);
        material.occlusionMap  = appendTexture(*result, *imported, source, aiTextureType_AMBIENT_OCCLUSION, modelDirectory);
        material.metalRoughMap = appendTexture(*result, *imported, source, aiTextureType_GLTF_METALLIC_ROUGHNESS, modelDirectory);
        material.specularMap   = appendTexture(*result, *imported, source, aiTextureType_SPECULAR, modelDirectory);
        result->materials.append(std::move(material));
    }
    if (result->materials.empty()) result->materials.append(Material {});

    DynaArray<uint32_t> meshToPrimitive;
    meshToPrimitive.resize(imported->mNumMeshes);
    for (uint32_t & primitiveIndex : meshToPrimitive) primitiveIndex = UINT32_MAX;
    for (uint32_t meshIndex = 0; meshIndex < imported->mNumMeshes; ++meshIndex) {
        const aiMesh & mesh = *imported->mMeshes[meshIndex];
        if (!mesh.HasPositions() || !mesh.HasFaces()) continue;

        Primitive primitive;
        primitive.name               = mesh.mName.C_Str();
        primitive.material           = mesh.mMaterialIndex < result->materials.size() ? mesh.mMaterialIndex : 0;
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
        if (!primitive.sourceHadNormals) generateNormals(primitive);
        if (!primitive.sourceHadTangents && primitive.sourceHadTexcoords) generateTangents(primitive);

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
