#include "pch.h"
#include "model-frag.spv.h"
#include "model-vert.spv.h"
#include "vk-shaders/model-material-ubo.h"

#include <assimp/cimport.h>
#include <assimp/GltfMaterial.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <array>
#include <cstddef>
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

void appendCuboid(ModelScene::Primitive & primitive, const glm::vec3 & minimum, const glm::vec3 & maximum, const glm::vec4 & color) {
    static constexpr uint32_t INDICES[] = {
        0, 2, 1, 1, 2, 3, 4, 5, 6, 5, 7, 6, 0, 1, 4, 1, 5, 4, 2, 6, 3, 3, 6, 7, 0, 4, 2, 2, 4, 6, 1, 3, 5, 3, 7, 5,
    };
    const uint32_t base = static_cast<uint32_t>(primitive.vertices.size());
    for (uint32_t corner = 0; corner < 8; ++corner) {
        ModelScene::Vertex vertex;
        vertex.position = {
            (corner & 1) ? maximum.x : minimum.x,
            (corner & 2) ? maximum.y : minimum.y,
            (corner & 4) ? maximum.z : minimum.z,
        };
        vertex.color = color;
        primitive.vertices.append(vertex);
        include(primitive.bounds, vertex.position);
    }
    for (uint32_t index : INDICES) primitive.indices.append(base + index);
}

void appendAxisAlignedSegment(ModelScene::Primitive & primitive, glm::vec3 start, glm::vec3 end, uint32_t axis, float width, const glm::vec4 & color) {
    glm::vec3 minimum = glm::min(start, end) - glm::vec3(width * 0.5f);
    glm::vec3 maximum = glm::max(start, end) + glm::vec3(width * 0.5f);
    minimum[axis]     = glm::min(start[axis], end[axis]);
    maximum[axis]     = glm::max(start[axis], end[axis]);
    appendCuboid(primitive, minimum, maximum, color);
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

    // External textures have no MIME hint; initialize it explicitly before moving the descriptor.
    ModelScene::Texture texture {.mimeType = ""};
    texture.srgb = type == aiTextureType_BASE_COLOR || type == aiTextureType_DIFFUSE || type == aiTextureType_EMISSIVE || type == aiTextureType_SPECULAR;
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
    DynaArray<glm::vec3> bitangents;
    bitangents.resize(primitive.vertices.size());
    for (glm::vec3 & bitangent : bitangents) bitangent = glm::vec3(0.0f);
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
        const glm::vec3 tangent   = (edge1 * uv2.y - edge2 * uv1.y) / determinant;
        const glm::vec3 bitangent = (edge2 * uv1.x - edge1 * uv2.x) / determinant;
        bitangents[i0] += bitangent;
        bitangents[i1] += bitangent;
        bitangents[i2] += bitangent;
        accumulated[i0] += tangent;
        accumulated[i1] += tangent;
        accumulated[i2] += tangent;
    }

    for (size_t i = 0; i < primitive.vertices.size(); ++i) {
        const glm::vec3 normal   = primitive.vertices[i].normal;
        glm::vec3       tangent  = accumulated[i] - normal * glm::dot(normal, accumulated[i]);
        const float     lengthSq = glm::dot(tangent, tangent);
        tangent                  = lengthSq > 0.0f ? tangent / glm::sqrt(lengthSq)
                                                   : glm::normalize(glm::cross(glm::abs(normal.z) < 0.999f ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0), normal));
        primitive.vertices[i].tangent = glm::vec4(tangent, glm::dot(glm::cross(normal, tangent), bitangents[i]) < 0.0f ? -1.0f : 1.0f);
    }
}

AutoRef<gpu2::Texture> makeSolidTexture(AutoRef<gpu2::GpuContext> gpu, gpu2::GpuCnC & cnc, const StrA & name, const std::array<uint8_t, 4> & color) {
    gpu2::Texture::Descriptor descriptor;
    descriptor.setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setDimensions(1, 1).setFaces(1).setLevels(1);
    auto texture = gpu2::Texture::create(name, {.context = gpu, .descriptor = descriptor});
    auto staging = gpu2::Buffer::create(name + ".staging", {.context = gpu, .size = 4, .mappable = true});
    if (!texture || !staging) return {};
    {
        auto mapped = staging->map();
        if (!mapped.data()) return {};
        memcpy(mapped.data(), color.data(), color.size());
    }
    gpu2::GpuCnC::Region region;
    region.imageExtent = {1, 1, 1};
    cnc.copyBufferToImage({.src = staging, .dst = texture, .regions = {&region, 1}});
    return texture;
}

} // namespace

ModelScene::SourceFormat ModelScene::sourceFormatFromPath(const StrA & path) {
    StrA extension = fs::extName(path);
    extension.toLower();

    if (extension == ".fbx") return SourceFormat::FBX;
    if (extension == ".gltf") return SourceFormat::GLTF;
    if (extension == ".glb") return SourceFormat::GLB;
    if (extension == ".stl") return SourceFormat::STL;
    if (extension == ".ase") return SourceFormat::ASE;
    return SourceFormat::UNKNOWN;
}

AutoRef<ModelScene> ModelScene::createDebugVisualization(const Bounds & sourceBounds, float lineWidth, bool includeBounds, bool includeAxes) {
    if (!sourceBounds.valid || lineWidth <= 0.0f || (!includeBounds && !includeAxes)) return {};

    AutoRef<ModelScene> result(new ModelScene(TYPE_INFO(), "model-debug-visualization"));
    result->sourcePath = "generated://model-debug-visualization";

    Material material;
    material.name      = "debug-vertex-color";
    material.workflow  = MaterialWorkflow::UNLIT;
    material.baseColor = glm::vec4(1.0f);
    result->materials.append(material);

    Primitive primitive;
    primitive.name            = "bounds-and-axes";
    primitive.sourceHadColors = true;
    if (includeBounds) {
        const glm::vec4 boundsColor(1.0f, 0.8f, 0.1f, 1.0f);
        for (uint32_t varyingAxis = 0; varyingAxis < 3; ++varyingAxis) {
            const uint32_t fixedAxis1 = (varyingAxis + 1) % 3;
            const uint32_t fixedAxis2 = (varyingAxis + 2) % 3;
            for (uint32_t corner = 0; corner < 4; ++corner) {
                glm::vec3 start   = sourceBounds.minimum;
                glm::vec3 end     = sourceBounds.minimum;
                end[varyingAxis]  = sourceBounds.maximum[varyingAxis];
                start[fixedAxis1] = end[fixedAxis1] = (corner & 1) ? sourceBounds.maximum[fixedAxis1] : sourceBounds.minimum[fixedAxis1];
                start[fixedAxis2] = end[fixedAxis2] = (corner & 2) ? sourceBounds.maximum[fixedAxis2] : sourceBounds.minimum[fixedAxis2];
                appendAxisAlignedSegment(primitive, start, end, varyingAxis, lineWidth, boundsColor);
            }
        }
    }

    if (includeAxes) {
        const float axisLength = std::max(glm::length(sourceBounds.maximum - sourceBounds.minimum) * 0.2f, lineWidth * 10.0f);
        appendAxisAlignedSegment(primitive, glm::vec3(0), glm::vec3(axisLength, 0, 0), 0, lineWidth, glm::vec4(1, 0, 0, 1));
        appendAxisAlignedSegment(primitive, glm::vec3(0), glm::vec3(0, axisLength, 0), 1, lineWidth, glm::vec4(0, 1, 0, 1));
        appendAxisAlignedSegment(primitive, glm::vec3(0), glm::vec3(0, 0, axisLength), 2, lineWidth, glm::vec4(0, 0.4f, 1, 1));
    }
    result->primitives.append(std::move(primitive));

    Node node;
    node.name = "bounds-and-axes";
    node.primitives.append(0);
    node.bounds = result->primitives[0].bounds;
    result->nodes.append(std::move(node));
    result->bounds = result->primitives[0].bounds;
    return result;
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

    // Assimp normalizes UVs to bottom-left; uploaded image rows use a top-left origin.
    constexpr unsigned importFlags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_SortByPType |
                                     aiProcess_ValidateDataStructure | aiProcess_FlipUVs;
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
                const aiVector3D & tangent   = mesh.mTangents[vertexIndex];
                vertex.tangent               = {tangent.x, tangent.y, tangent.z, 1.0f};
                const aiVector3D & bitangent = mesh.mBitangents[vertexIndex];
                vertex.tangent.w =
                    glm::dot(glm::cross(vertex.normal, glm::vec3(vertex.tangent)), glm::vec3(bitangent.x, bitangent.y, bitangent.z)) < 0.0f ? -1.0f : 1.0f;
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

AutoRef<ModelAsset> ModelAsset::create(AutoRef<gpu2::GpuContext> gpu, AutoRef<const ModelScene> scene) {
    if (!gpu || !scene || scene->primitives.empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "ModelAsset::create: missing GPU context or model geometry");
            return {};
        }

    auto cnc = gpu2::GpuCnC::create({.gpu = gpu});
    if (!cnc) GN_UNLIKELY return {};

    AutoRef<ModelAsset> result(new ModelAsset(TYPE_INFO(), "model-asset"));
    result->scene = std::move(scene);
    for (const ModelScene::Primitive & primitive : result->scene->primitives) {
        const uint64_t vertexBytes  = primitive.vertices.size() * sizeof(ModelScene::Vertex);
        const uint64_t indexBytes   = primitive.indices.size() * sizeof(uint32_t);
        auto           vertexBuffer = gpu2::Buffer::create(primitive.name + ".vertices", {.context = gpu, .size = vertexBytes});
        auto           indexBuffer  = gpu2::Buffer::create(primitive.name + ".indices", {.context = gpu, .size = indexBytes});
        if (!vertexBuffer || !indexBuffer) GN_UNLIKELY return {};

        cnc->uploadBuffer(vertexBuffer, 0,
                          ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(primitive.vertices.data()), static_cast<size_t>(vertexBytes)));
        cnc->uploadBuffer(indexBuffer, 0,
                          ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(primitive.indices.data()), static_cast<size_t>(indexBytes)));

        gpu2::RasterGeometry geometry;
        geometry.format.attributes.append(
            {.location = 0, .binding = 0, .offset = offsetof(ModelScene::Vertex, position), .format = gpu2::RasterGeometry::AttributeFormat::F32_3});
        geometry.format.attributes.append(
            {.location = 1, .binding = 0, .offset = offsetof(ModelScene::Vertex, normal), .format = gpu2::RasterGeometry::AttributeFormat::F32_3});
        geometry.format.attributes.append(
            {.location = 2, .binding = 0, .offset = offsetof(ModelScene::Vertex, texcoord), .format = gpu2::RasterGeometry::AttributeFormat::F32_2});
        geometry.format.attributes.append(
            {.location = 3, .binding = 0, .offset = offsetof(ModelScene::Vertex, tangent), .format = gpu2::RasterGeometry::AttributeFormat::F32_4});
        geometry.format.attributes.append(
            {.location = 4, .binding = 0, .offset = offsetof(ModelScene::Vertex, color), .format = gpu2::RasterGeometry::AttributeFormat::F32_4});
        geometry.vertices.append({.buffer = vertexBuffer, .offset = 0, .stride = sizeof(ModelScene::Vertex)});
        geometry.vertexCount = static_cast<uint32_t>(primitive.vertices.size());
        geometry.indices     = {.buffer = indexBuffer, .offset = 0, .stride = sizeof(uint32_t)};
        geometry.indexCount  = static_cast<uint32_t>(primitive.indices.size());
        result->primitives.append(std::move(geometry));
    }

    for (const ModelScene::Material & material : result->scene->materials) {
        shader::ModelMaterialUBO constants;
        constants.baseColor              = material.baseColor;
        constants.emissiveAndMetallic    = glm::vec4(material.emissive, material.metallic);
        constants.roughnessAlphaWorkflow = glm::vec4(material.roughness, material.alphaCutoff, static_cast<float>(material.workflow), 0.0f);
        auto buffer                      = gpu2::Buffer::create(material.name + ".constants", {.context = gpu, .size = sizeof(constants)});
        if (!buffer) GN_UNLIKELY return {};
        cnc->uploadBuffer(buffer, 0, ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(&constants), sizeof(constants)));
        result->materialBuffers.append(std::move(buffer));
    }

    for (size_t textureIndex = 0; textureIndex < result->scene->textures.size(); ++textureIndex) {
        const ModelScene::Texture & source = result->scene->textures[textureIndex];
        const StrA                  name   = StrA::format("model.texture.{}", textureIndex);
        gpu2::Buffer::StagedTexture staged;
        if (!source.path.empty()) {
            staged = gpu2::Buffer::loadTextureToStagingBuffer(name, gpu, source.path);
        } else if (!source.embeddedData.empty()) {
            staged = gpu2::Buffer::loadTextureToStagingBuffer(name, gpu, ArrayView<const uint8_t>(source.embeddedData.data(), source.embeddedData.size()),
                                                              source.mimeType);
        }
        if (staged.empty()) {
            GN_WARN(sLogger, "ModelAsset::create: texture {} could not be staged", textureIndex);
            result->textures.append(AutoRef<gpu2::Texture> {});
            continue;
        }

        // Choose transfer interpretation by material role, not the file container.
        using PF        = gfx::img::PixelFormat;
        const auto sign = source.srgb ? PF::SIGN_GNORM : PF::SIGN_UNORM;
        if (staged.descriptor.format.sign0 == PF::SIGN_UNORM || staged.descriptor.format.sign0 == PF::SIGN_GNORM) staged.descriptor.format.sign0 = sign;
        if (staged.descriptor.format.sign12 == PF::SIGN_UNORM || staged.descriptor.format.sign12 == PF::SIGN_GNORM) staged.descriptor.format.sign12 = sign;
        auto texture = gpu2::Texture::create(name, {.context = gpu, .descriptor = staged.descriptor});
        if (!texture) GN_UNLIKELY return {};
        cnc->copyBufferToImage(staged, texture);
        result->textures.append(std::move(texture));
    }

    result->gpuPayload = cnc->seal();
    return result->gpuPayload ? result : AutoRef<ModelAsset> {};
}

namespace {

struct ModelShadingData {
    AutoRef<gpu2::GpuShader> vs;
    AutoRef<gpu2::GpuShader> ps;
    AutoRef<gpu2::Texture>   white;
    AutoRef<gpu2::Texture>   normal;
    AutoRef<gpu2::Texture>   black;
    AutoRef<gpu2::Texture>   metalRough;
};

struct ModelShadingAsset final : ModelShading::Asset {
    GN_REGISTER_RUNTIME_TYPE(Asset);
    ModelShadingData data;

    explicit ModelShadingAsset(ModelShadingData value): Asset(TYPE_INFO(), "model-shading"), data(std::move(value)) {}
};

} // namespace

AutoRef<ModelShading::Asset> ModelShading::create(AutoRef<gpu2::GpuContext> gpu) {
    if (!gpu) GN_UNLIKELY return {};

    ModelShadingData data;
    data.vs  = gpu2::GpuShader::create({.context = gpu, .name = "model.vert", .binary = kModelVertSpv, .size = sizeof(kModelVertSpv)});
    data.ps  = gpu2::GpuShader::create({.context = gpu, .name = "model.frag", .binary = kModelFragSpv, .size = sizeof(kModelFragSpv)});
    auto cnc = gpu2::GpuCnC::create({.gpu = gpu});
    if (!data.vs || !data.ps || !cnc) GN_UNLIKELY return {};

    data.white      = makeSolidTexture(gpu, *cnc, "model.white", {255, 255, 255, 255});
    data.normal     = makeSolidTexture(gpu, *cnc, "model.normal", {128, 128, 255, 255});
    data.black      = makeSolidTexture(gpu, *cnc, "model.black", {0, 0, 0, 255});
    data.metalRough = makeSolidTexture(gpu, *cnc, "model.metal-rough", {0, 255, 255, 255});
    if (!data.white || !data.normal || !data.black || !data.metalRough) GN_UNLIKELY return {};

    AutoRef<ModelShadingAsset> result(new ModelShadingAsset(std::move(data)));
    result->gpuPayload = cnc->seal();
    if (!result->gpuPayload) return {};
    return result;
}

gpu2::GpuRaster::DrawParameters ModelShading::getDrawParams(const SharedShaderConstants::Snapshot & sscSnapshot, AutoRef<const Asset> shading,
                                                            AutoRef<const ModelAsset> model, uint32_t primitiveIndex, const glm::mat4 & worldTransform) {
    const auto * content = RuntimeType::cast<ModelShadingAsset>(shading.get());
    if (!content || !model || primitiveIndex >= model->primitives.size()) GN_UNLIKELY return {};
    const ModelScene::Primitive & primitive = model->scene->primitives[primitiveIndex];
    if (primitive.material >= model->scene->materials.size() || primitive.material >= model->materialBuffers.size()) GN_UNLIKELY return {};
    const ModelScene::Material & material = model->scene->materials[primitive.material];

    gpu2::GpuRaster::DrawParameters draw;
    draw.vs                = content->data.vs;
    draw.ps                = content->data.ps;
    draw.geometry          = model->primitives[primitiveIndex];
    draw.states.cullMode   = material.doubleSided ? gpu2::RasterState::CULL_NONE : gpu2::RasterState::CULL_BACK;
    draw.states.frontFace  = gpu2::RasterState::FRONT_CCW;
    draw.states.depthState = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::LESS, true};

    const glm::mat4 normalTransform = glm::transpose(glm::inverse(worldTransform));
    struct PushConstants {
        glm::mat4 world;
        glm::mat4 normal;
    };
    const PushConstants constants {worldTransform, normalTransform};
    draw.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(constants), reinterpret_cast<const uint8_t *>(&constants)));

    draw.resources.resize(2);
    draw.resources[0] = sscSnapshot.set0Resources;
    auto & set1       = draw.resources[1];
    set1.resize(6);
    auto bindTexture = [&](uint32_t binding, int32_t textureIndex, const AutoRef<gpu2::Texture> & fallback) {
        AutoRef<gpu2::Texture> texture = fallback;
        if (textureIndex >= 0 && static_cast<size_t>(textureIndex) < model->textures.size() && model->textures[textureIndex]) {
            texture = model->textures[textureIndex];
        }
        set1[binding].resize(1);
        set1[binding][0].resource = texture;
        set1[binding][0].setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
    };
    bindTexture(0, material.baseColorMap, content->data.white);
    bindTexture(1, material.normalMap, content->data.normal);
    bindTexture(2, material.emissiveMap, content->data.white);
    bindTexture(3, material.occlusionMap, content->data.white);
    bindTexture(4, material.metalRoughMap, content->data.metalRough);
    set1[5].resize(1);
    set1[5][0].resource = model->materialBuffers[primitive.material];
    set1[5][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(shader::ModelMaterialUBO));
    return draw;
}

} // namespace GN::fx2
