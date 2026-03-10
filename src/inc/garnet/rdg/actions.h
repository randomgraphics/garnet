#pragma once

#include <variant>
#include <map>
#include <optional>

namespace GN::rdg {

/// Set render target for draw actions. It clears the render target to certain value, discarding existing content.
/// - This is the required first action to start rendering to a render target.
/// - It tells GPU to discard existing content thus avoid expensive image layout transitions.
/// - If the render target is backbuffer, it must be in the renderable state, meaning
///   the backbuffer is prepared and not presented. See PrepareBackbuffer and
///   PresentBackbuffer actions for more details.
struct ClearRenderTarget : public Action {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "ClearRenderTarget";

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "ClearRenderTarget::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        AutoRef<RenderTarget> renderTarget;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (!renderTarget) return;
            for (const auto & color : renderTarget->colors) {
                if (color.target.artifact) { list.writeList.insert(color.target.artifact); }
            }
            if (renderTarget->depthStencilTarget.artifact) {
                if (renderTarget->depthState.testEnabled() || renderTarget->stencilState.enabled())
                    list.readList.insert(renderTarget->depthStencilTarget.artifact);
                if (renderTarget->depthState.writeEnabled() || renderTarget->stencilState.enabled())
                    list.writeList.insert(renderTarget->depthStencilTarget.artifact);
            }
        }

        static AutoRef<A> make(AutoRef<RenderTarget> rt) {
            auto a          = AutoRef<A>(new A());
            a->renderTarget = std::move(rt);
            return a;
        }
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Create a new instance and register to the database via admit(). Implementation provided by backend.
    static GN_API AutoRef<ClearRenderTarget> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

/// This is the action to set the backbuffer into renderable state.
/// It must be called in pair with PresentBackbuffer action to properly
/// rotate the swapchain buffers for rendering and presenting.
struct PrepareBackbuffer : public Action {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "PrepareBackbuffer";

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "PrepareBackbuffer::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        AutoRef<Backbuffer> backbuffer;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (!backbuffer) return;
            list.readList.insert(backbuffer);
            list.writeList.insert(backbuffer);
        }

        static AutoRef<A> make(AutoRef<Backbuffer> bb) {
            auto a        = AutoRef<A>(new A());
            a->backbuffer = std::move(bb);
            return a;
        }
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Create a new instance and register to the database via admit(). Implementation provided by backend.
    static GN_API AutoRef<PrepareBackbuffer> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

/// This is the action to set the backbuffer into presented state and make it non-renderable.
/// It must be called in pair with PrepareBackbuffer action to properly
/// rotate the swapchain buffers for rendering and presenting.
struct PresentBackbuffer : public Action {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "PresentBackbuffer";

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "PresentBackbuffer::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        AutoRef<Backbuffer> backbuffer;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (!backbuffer) return;
            list.readList.insert(backbuffer.get());
            list.writeList.insert(backbuffer.get());
        }

        static AutoRef<A> make(AutoRef<Backbuffer> bb) {
            auto a        = AutoRef<A>(new A());
            a->backbuffer = std::move(bb);
            return a;
        }
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Create a new instance and register to the database via admit(). Implementation provided by backend.
    static GN_API AutoRef<PresentBackbuffer> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

/// Setup render states action for configuring GPU render pipeline state.
struct SetupRenderStates : public Action {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "SetupRenderStates";

    enum FillMode {
        FILL_SOLID = 0,
        FILL_WIREFRAME,
        FILL_POINT,
    };

    enum CullMode {
        CULL_NONE = 0,
        CULL_FRONT,
        CULL_BACK,
    };

    enum FrontFace {
        FRONT_CCW = 0,
        FRONT_CW,
    };

    /// Render state descriptor
    struct RenderStateDesc {
        // Rasterizer state
        std::optional<FillMode>  fillMode;  ///< fill mode (solid, wireframe, point)
        std::optional<CullMode>  cullMode;  ///< cull mode (none, front, back)
        std::optional<FrontFace> frontFace; ///< front face winding (CCW, CW)
    };

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "SetupRenderStates::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        RenderStateDesc renderStates; ///< render state descriptor

        void addToReadWriteList(ArtifactReadWriteList & list) const override { (void) list; }
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    static GN_API AutoRef<SetupRenderStates> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

/// Base class for generic shader actions (draw and compute). Contains common shader resource binding definitions.
struct GpuShaderAction : public Action {
    /// Shader binary that can be used to create the actual GPU shader program.
    struct ShaderBinary {
        void *       binary = nullptr; ///< pointer to the shader binary code.
        size_t       size   = 0;       ///< size of the shader binary code.
        const char * entry  = nullptr; ///< entry point name.

        bool empty() const { return binary == nullptr || size == 0 || entry == nullptr; }
        bool valid() const { return binary != nullptr && size > 0 && entry != nullptr; }
    };

    /// A 3-D table of shader resources.
    /// The first dimension is the resource set index
    /// The second dimension is the binding slot inside a resource set
    /// The third dimension is the resource view index inside a binding slot.
    using GpuResourceTable = DynaArray<DynaArray<DynaArray<GpuResourceView>>>;

protected:
    using Action::Action;
};

/// Generic GPU draw action. This is the building block of all other draw actions and effects.
/// It depends on ClearRenderTarget action to set the render target. Without it, this action will simply fail.
struct GpuDraw : public GpuShaderAction {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "GpuDraw";

    /// Represent a GPU renderable geometry.
    struct GpuGeometry {
        /// API-agnostic vertex attribute format; backend maps to native (e.g. VkFormat).
        enum class AttributeFormat : uint8_t {
            F32_1,
            F32_2,
            F32_3,
            F32_4,
            F16_1,
            F16_2,
            F16_3,
            F16_4,
            U32_1,
            U32_2,
            U32_3,
            U32_4,
            U16_1,
            U16_2,
            U16_3,
            U16_4,
            U8_1,
            U8_2,
            U8_3,
            U8_4,
            I32_1,
            I32_2,
            I32_3,
            I32_4,
            I16_1,
            I16_2,
            I16_3,
            I16_4,
            I8_1,
            I8_2,
            I8_3,
            I8_4,
        };

        /// Describes one vertex attribute (shader location, format, byte offset in vertex).
        struct VertexAttribute {
            uint32_t        location = 0; ///< index into the vertex buffer array.
            uint32_t        offset   = 0; ///< byte offset from the beginning of a vertex.
            AttributeFormat format   = AttributeFormat::F32_3;

            bool operator==(const VertexAttribute & other) const { return location == other.location && format == other.format && offset == other.offset; }
            bool operator!=(const VertexAttribute & other) const { return !operator==(other); }
        };

        /// Vertex layout description. Geometry loader and sample code must populate this to match vertex buffer layout.
        struct VertexFormat {
            DynaArray<VertexAttribute> attributes;

            bool empty() const { return attributes.empty(); }
            bool operator==(const VertexFormat & other) const { return attributes == other.attributes; }
            bool operator!=(const VertexFormat & other) const { return !operator==(other); }
        };

        struct GeometryBuffer {
            AutoRef<Buffer> buffer; // buffer containing the geometry data
            uint64_t        offset; ///< offset in bytes from the beginning of the buffer.
            uint32_t        stride; ///< size of one element in bytes
        };

        VertexFormat              format;
        DynaArray<GeometryBuffer> instances;
        uint32_t                  instanceCount = 0;
        DynaArray<GeometryBuffer> vertices;
        uint32_t                  vertexCount = 0;
        GeometryBuffer            indices;
        uint32_t                  indexCount = 0;
    };

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "GpuDraw::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        GpuGeometry        geometry;   ///< geometry
        GpuResourceTable   resources;  ///< shader resources
        DynaArray<uint8_t> immediates; ///< immediate constants. Backend copies to GPU when non-empty.

        /// Render target.
        /// \note Must specify render target as argument for each draw action.
        /// So that the backend can determine this action's full read and write list.
        AutoRef<RenderTarget> renderTarget;

        GN_API void addToReadWriteList(ArtifactReadWriteList & list) const override;
    };

    struct CreateParameters {
        AutoRef<GpuContext> context;
        ShaderBinary        vs; ///< vertex shader
        ShaderBinary        ds; ///< domain shader
        ShaderBinary        hs; ///< hull shader
        ShaderBinary        gs; ///< geometry shader
        ShaderBinary        ps; ///< pixel shader
    };

    static GN_API AutoRef<GpuDraw> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuShaderAction::GpuShaderAction;
};

/// Generic GPU compute action. It is the building block of all GPU compute operations.
struct GpuCompute : public GpuShaderAction {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "GenericCompute";

    /// Dispatch dimensions (thread group counts)
    struct DispatchSize {
        uint32_t x = 1;
        uint32_t y = 1;
        uint32_t z = 1;
    };

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "GenericCompute::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        GpuResourceTable   resources;  ///< shader resources
        DynaArray<uint8_t> immediates; ///< immediate constants. Backend copies to GPU when non-empty.
        DispatchSize       groups;     ///< thread group counts

        GN_API void addToReadWriteList(ArtifactReadWriteList & list) const override;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
        ShaderBinary        cs; ///< compute shader
    };

    static GN_API AutoRef<GpuCompute> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuShaderAction::GpuShaderAction;
};

// namespace std {

// template<>
// struct hash<GN::rdg::GpuShaderAction::ShaderResourceBinding> {
//     size_t operator()(const GN::rdg::GpuShaderAction::ShaderResourceBinding & key) const {
//         auto hash = std::hash<uint32_t>()(key.set);
//         GN::combineHash(hash, key.slot);
//         return hash;
//     }
// };

// } // namespace std

// /// Composes one solid color and a set of textures into a single output texture.
// /// Inputs: one color (set on the action) and up to MAX_INPUT_TEXTURES texture parameters.
// /// Output: one texture (parameter "output").
// struct Compose : public GpuShaderAction {
//     GN_API static const uint64_t TYPE;

//     struct A : public Arguments {
//         GN_API static const uint64_t TYPE;
//         ReadOnly<AutoRef<Mesh>>                                 mesh;
//         ReadOnly<Vector4f>                                      color;
//         ReadOnlyArray<AutoRef<Texture>, 8, Arguments::Optional> textures;
//         ReadWriteArray<RenderTarget, 8, Arguments::Optional>    renderTargets;
//         ReadWrite<RenderTarget>                                 depthStencil;
//     };

//     virtual bool reset() = 0;

// protected:
//     using GpuShaderAction::GpuShaderAction;
// };

// /// Physically Based Rendering (PBR) action using Disney PBR shading model.
// /// Renders objects with realistic material properties including base color, metallic, roughness, and normal mapping.
// struct PBRShading : public GpuShaderAction {
//     GN_API static const uint64_t TYPE;

//     /// Light types for PBR lighting
//     enum LightType {
//         LIGHT_DIRECTIONAL = 0, ///< Directional light (sun)
//         LIGHT_POINT,           ///< Point light (bulb)
//         LIGHT_SPOT,            ///< Spot light (flashlight)
//     };

//     /// Light source definition
//     struct Light {
//         LightType type = LIGHT_DIRECTIONAL; ///< light type
//         Vector4f   color = {1.0f, 1.0f, 1.0f, 1.0f}; ///< light color (RGB) and intensity (A)
//         Vector4f   position = {0.0f, 0.0f, 0.0f, 1.0f}; ///< light position (for point/spot) or direction (for directional, w=0)
//         Vector4f   direction = {0.0f, 0.0f, -1.0f, 0.0f}; ///< light direction (for spot/directional)
//         float      range = 100.0f; ///< light range (for point/spot)
//         float      innerConeAngle = 0.0f; ///< inner cone angle in radians (for spot)
//         float      outerConeAngle = 0.0f; ///< outer cone angle in radians (for spot)
//     };

//     /// Material properties for Disney PBR
//     struct MaterialParams {
//         Vector4f baseColor = {1.0f, 1.0f, 1.0f, 1.0f}; ///< base color (albedo)
//         float    metallic = 0.0f;                       ///< metallic factor (0 = dielectric, 1 = metal)
//         float    roughness = 0.5f;                     ///< roughness factor (0 = smooth, 1 = rough)
//         float    specular = 0.5f;                      ///< specular factor (for non-metals)
//         float    specularTint = 0.0f;                  ///< specular tint (0 = white, 1 = tinted)
//         float    sheen = 0.0f;                         ///< sheen factor (for cloth)
//         float    sheenTint = 0.5f;                     ///< sheen tint
//         float    clearcoat = 0.0f;                     ///< clearcoat factor
//         float    clearcoatGloss = 1.0f;                ///< clearcoat glossiness
//         float    subsurface = 0.0f;                    ///< subsurface scattering factor
//         float    subsurfaceColorR = 1.0f;               ///< subsurface color R
//         float    subsurfaceColorG = 1.0f;               ///< subsurface color G
//         float    subsurfaceColorB = 1.0f;              ///< subsurface color B
//         float    anisotropic = 0.0f;                  ///< anisotropy factor
//         float    anisotropicRotation = 0.0f;           ///< anisotropy rotation
//         float    transmission = 0.0f;                  ///< transmission factor (for glass)
//         float    ior = 1.5f;                           ///< index of refraction
//         Vector4f emissive = {0.0f, 0.0f, 0.0f, 0.0f}; ///< emissive color and intensity
//     };

//     /// Camera/view parameters
//     struct CameraParams {
//         Matrix44f viewMatrix;      ///< view matrix (world to view)
//         Matrix44f projMatrix;      ///< projection matrix (view to clip)
//         Matrix44f viewProjMatrix; ///< combined view-projection matrix
//         Vector4f  cameraPosition; ///< camera position in world space (xyz, w=1)
//     };

//     struct A : public Arguments {
//         GN_API static const uint64_t TYPE;

//         ReadOnly<AutoRef<Mesh>> mesh; ///< mesh to render

//         // Material properties
//         ReadOnly<MaterialParams> material; ///< material parameters

//         // Material textures (optional - if not provided, use material parameter values)
//         ReadOnly<TextureParameter, Arguments::Optional> baseColorTexture;     ///< base color texture (albedo)
//         ReadOnly<TextureParameter, Arguments::Optional> metallicRoughnessTexture; ///< metallic (R) and roughness (G) packed texture
//         ReadOnly<TextureParameter, Arguments::Optional> normalTexture;       ///< normal map texture
//         ReadOnly<TextureParameter, Arguments::Optional> emissiveTexture;     ///< emissive texture
//         ReadOnly<TextureParameter, Arguments::Optional> occlusionTexture;    ///< ambient occlusion texture
//         ReadOnly<TextureParameter, Arguments::Optional> clearcoatTexture;     ///< clearcoat texture
//         ReadOnly<TextureParameter, Arguments::Optional> clearcoatRoughnessTexture; ///< clearcoat roughness texture

//         // Lighting
//         ReadOnlyArray<Light, 8, Arguments::Optional> lights; ///< light sources (up to 8 lights)
//         ReadOnly<TextureParameter, Arguments::Optional> environmentMap; ///< environment map for IBL (cubemap)
//         ReadOnly<TextureParameter, Arguments::Optional> irradianceMap; ///< pre-computed irradiance map for IBL (cubemap)
//         ReadOnly<TextureParameter, Arguments::Optional> prefilteredMap; ///< pre-filtered environment map for IBL (cubemap)
//         ReadOnly<TextureParameter, Arguments::Optional> brdfLUT; ///< BRDF lookup texture for IBL

//         // Camera/view
//         ReadOnly<CameraParams> camera; ///< camera parameters

//         // Transform
//         ReadOnly<Matrix44f> modelMatrix; ///< model-to-world transformation matrix
//         ReadOnly<Matrix44f, Arguments::Optional> normalMatrix; ///< normal transformation matrix (if not provided, derived from modelMatrix)

//         // Render targets
//         ReadWriteArray<RenderTarget, 8, Arguments::Optional> renderTargets; ///< color render targets
//         ReadWrite<RenderTarget, Arguments::Optional> depthStencil;         ///< depth/stencil render target (optional)
//     };

//     /// Shader stage description
//     struct ShaderStageDesc {
//         AutoRef<Blob> shaderBinary; ///< shader binary code
//         StrA          entryPoint;    ///< entry point name
//     };

//     /// Reset the PBR shading action with shader binaries for different stages.
//     /// All parameters are optional - only provided stages will be set.
//     virtual bool reset(const std::optional<ShaderStageDesc> & vs = {}, ///< vertex shader
//                        const std::optional<ShaderStageDesc> & ds = {}, ///< domain shader
//                        const std::optional<ShaderStageDesc> & hs = {}, ///< hull shader
//                        const std::optional<ShaderStageDesc> & gs = {}, ///< geometry shader
//                        const std::optional<ShaderStageDesc> & ps = {}  ///< pixel shader
//                        ) = 0;

// protected:
//     using GpuShaderAction::GpuShaderAction;
// };

} // namespace GN::rdg
