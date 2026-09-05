#if !defined(__GN_INSIDE_FX2_H__)
    #error "Do not include <garnet/fx2/model.h> directly. Include <garnet/GNfx2.h> instead."
#endif

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace GN::fx2 {

/// Immutable, format-neutral scene imported for visualization and rendering.
///
/// ModelScene describes source content independently of GPU residency and shading. Importers
/// normalize supported file formats into these records; a later rendering stage may create
/// gpu2 resources and choose an appropriate effect without changing the imported scene.
struct ModelScene : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    /// Source container recognized by the importer.
    enum class SourceFormat : uint8_t {
        UNKNOWN,
        FBX,
        GLTF,
        GLB,
        STL,
    };

    /// Material workflow inferred from source data.
    enum class MaterialWorkflow : uint8_t {
        DEFAULT_LIT,
        METALLIC_ROUGHNESS,
        SPECULAR_GLOSSINESS,
        UNLIT,
    };

    /// Source material alpha behavior.
    enum class AlphaMode : uint8_t {
        OPAQUE,
        MASK,
        BLEND,
    };

    /// Axis-aligned bounds in normalized model-local coordinates.
    struct Bounds {
        glm::vec3 minimum = glm::vec3(0);
        glm::vec3 maximum = glm::vec3(0);
        bool      valid   = false;
    };

    /// Texture reference retained from the imported material.
    struct Texture {
        /// Resolved disk path for an external texture; empty for embedded content.
        StrA path;

        /// Embedded encoded bytes, such as a PNG or JPEG stored in GLB/FBX content.
        DynaArray<uint8_t> embeddedData;

        /// Optional source MIME type for embedded encoded bytes.
        StrA mimeType;
    };

    /// Renderer-neutral material properties. Unsupported source properties are ignored with
    /// an import warning, while missing properties retain these deterministic defaults.
    struct Material {
        StrA             name;
        MaterialWorkflow workflow       = MaterialWorkflow::DEFAULT_LIT;
        AlphaMode        alphaMode      = AlphaMode::OPAQUE;
        glm::vec4        baseColor      = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        glm::vec3        emissive       = glm::vec3(0);
        float            metallic       = 0.0f;
        float            roughness      = 1.0f;
        float            alphaCutoff    = 0.5f;
        bool             doubleSided    = false;
        int32_t          baseColorMap   = -1;
        int32_t          normalMap      = -1;
        int32_t          emissiveMap    = -1;
        int32_t          occlusionMap   = -1;
        int32_t          metalRoughMap  = -1;
        int32_t          specularMap    = -1;
    };

    /// One normalized vertex. Absent source attributes receive importer-generated or documented
    /// fallback values; clients should inspect the primitive attribute flags before debugging
    /// source content.
    struct Vertex {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 normal   = glm::vec3(0, 1, 0);
        glm::vec4 tangent  = glm::vec4(1, 0, 0, 1);
        glm::vec2 texcoord = glm::vec2(0);
        glm::vec4 color    = glm::vec4(1);
    };

    /// Triangle-list geometry and its material binding.
    struct Primitive {
        StrA                name;
        DynaArray<Vertex>   vertices;
        DynaArray<uint32_t> indices;
        uint32_t            material = 0;
        Bounds              bounds;
        bool                sourceHadNormals   = false;
        bool                sourceHadTangents  = false;
        bool                sourceHadTexcoords = false;
        bool                sourceHadColors    = false;
    };

    /// Scene hierarchy node. The transform is relative to the parent; primitive indices refer
    /// to entries in primitives. A parent value of -1 identifies a root node.
    struct Node {
        StrA                name;
        int32_t             parent = -1;
        glm::mat4           transform = glm::mat4(1);
        DynaArray<uint32_t> primitives;
        Bounds              bounds;
    };

    /// Parameters for synchronous CPU import. GPU resources are deliberately not created here.
    struct LoadParameters {
        StrA path;
    };

    SourceFormat        sourceFormat = SourceFormat::UNKNOWN;
    StrA                sourcePath;
    DynaArray<Texture>  textures;
    DynaArray<Material> materials;
    DynaArray<Primitive> primitives;
    DynaArray<Node>     nodes;
    Bounds              bounds;
    DynaArray<StrA>     warnings;

    /// Import FBX, glTF, GLB, or STL content into a normalized immutable scene. Returns an
    /// empty reference when the path cannot be resolved or contains no renderable triangles.
    GN_API static AutoRef<ModelScene> load(const LoadParameters &);

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2
