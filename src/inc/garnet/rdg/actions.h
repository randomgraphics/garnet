#pragma once

#include <variant>

namespace GN::rdg {

struct RenderTarget {
    struct ColorTarget {
        std::variant<AutoRef<Texture>, AutoRef<Backbuffer>> target;
        Texture::SubresourceIndex                           subresourceIndex; ///< only used for texture targets
    };

    struct DepthStencil {
        std::variant<AutoRef<Texture>> target;
        Texture::SubresourceIndex      subresourceIndex {};
    };

    StackArray<ColorTarget, 8> colors;
    DepthStencil               depthStencil;

    /// Retrieve the GPU context that the render target is bound to.
    AutoRef<GpuContext> gpu() const {
        for (const auto & color : colors) {
            auto t = std::get_if<AutoRef<Texture>>(&color.target);
            if (t && *t) return AutoRef<GpuContext> {&(*t)->gpu()};
            auto b = std::get_if<AutoRef<Backbuffer>>(&color.target);
            if (b && *b) return AutoRef<GpuContext> {&(*b)->gpu()};
        }
        auto d = std::get_if<AutoRef<Texture>>(&depthStencil.target);
        if (d && *d) return AutoRef<GpuContext> {&(*d)->gpu()};
        return {};
    }
};

struct ClearRenderTarget : public Action {
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
        A(): Arguments(TYPE) {}
        struct ClearValues {
            union {
                float    f4[4];
                uint32_t u4[4];
                int32_t  i4[4];
            } colors[8];
            float    depth;
            uint32_t stencil;
        };
        ReadOnly<ClearValues>   clearValues;
        ReadWrite<RenderTarget> renderTarget;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Create a new instance and register to the database via admit(). Implementation provided by backend.
    static GN_API AutoRef<ClearRenderTarget> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

struct TextureLoader : public Action {
    inline static constexpr Guid TYPE = {0x825a7724, 0xfecb, 0x49e2, {0xb7, 0x1f, 0xfc, 0x9d, 0x3a, 0xa2, 0x8b, 0x11}};

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x825a7724, 0xfecb, 0x49e2, {0xb7, 0x1f, 0xfc, 0x9d, 0x3a, 0xa2, 0x8b, 0x11}};
        A(): Arguments(TYPE) {}
        WriteOnly<AutoRef<Texture>> texture;  // Output texture resource
        ReadOnly<StrA>              filename; // Path to texture file
    };

    struct CreateParameters {
        // For future use
    };

    static GN_API AutoRef<TextureLoader> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

struct PrepareBackbuffer : public Action {
    inline static constexpr Guid TYPE = {0x3e4f5a6b, 0x7c8d, 0x9e0f, {0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b}};

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x3e4f5a6b, 0x7c8d, 0x9e0f, {0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b}};
        A(): Arguments(TYPE) {}
        ReadWrite<AutoRef<Backbuffer>> backbuffer; // Backbuffer to prepare
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Create a new instance and register to the database via admit(). Implementation provided by backend.
    static GN_API AutoRef<PrepareBackbuffer> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

struct PresentBackbuffer : public Action {
    inline static constexpr Guid TYPE = {0x4f5a6b7c, 0x8d9e, 0x0f1a, {0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c}};

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x4f5a6b7c, 0x8d9e, 0x0f1a, {0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c}};
        A(): Arguments(TYPE) {}
        ReadOnly<AutoRef<Backbuffer>> backbuffer; // Backbuffer to present
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
    inline static constexpr Guid TYPE = {0x7c8d9e0f, 0x1a2b, 0x3c4d, {0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e, 0x2f}};

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

    enum ComparisonFunc {
        CMP_NEVER = 0,
        CMP_LESS,
        CMP_LESS_EQUAL,
        CMP_EQUAL,
        CMP_GREATER_EQUAL,
        CMP_GREATER,
        CMP_NOT_EQUAL,
        CMP_ALWAYS,
    };

    enum StencilOp {
        STENCIL_KEEP = 0,
        STENCIL_ZERO,
        STENCIL_REPLACE,
        STENCIL_INC_SAT,
        STENCIL_DEC_SAT,
        STENCIL_INVERT,
        STENCIL_INC,
        STENCIL_DEC,
    };

    enum BlendArg {
        BLEND_ZERO = 0,
        BLEND_ONE,
        BLEND_SRC_COLOR,
        BLEND_INV_SRC_COLOR,
        BLEND_SRC_ALPHA,
        BLEND_INV_SRC_ALPHA,
        BLEND_DEST_ALPHA,
        BLEND_INV_DEST_ALPHA,
        BLEND_DEST_COLOR,
        BLEND_INV_DEST_COLOR,
        BLEND_BLEND_FACTOR,
        BLEND_INV_BLEND_FACTOR,
    };

    enum BlendOp {
        BLEND_OP_ADD = 0,
        BLEND_OP_SUB,
        BLEND_OP_REV_SUB,
        BLEND_OP_MIN,
        BLEND_OP_MAX,
    };

    /// Viewport settings
    struct Viewport {
        float x        = 0.0f;
        float y        = 0.0f;
        float width    = 0.0f;
        float height   = 0.0f;
        float minDepth = 0.0f;
        float maxDepth = 1.0f;
    };

    /// Scissor rectangle
    struct ScissorRect {
        int32_t  x      = 0;
        int32_t  y      = 0;
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    /// Render state descriptor
    struct RenderStateDesc {
        // Rasterizer state
        std::optional<FillMode>  fillMode;    ///< fill mode (solid, wireframe, point)
        std::optional<CullMode>  cullMode;    ///< cull mode (none, front, back)
        std::optional<FrontFace> frontFace;   ///< front face winding (CCW, CW)
        std::optional<bool>      msaaEnabled; ///< MSAA enabled

        // Depth state
        std::optional<bool>           depthTestEnabled;  ///< enable depth testing
        std::optional<bool>           depthWriteEnabled; ///< enable depth writing
        std::optional<ComparisonFunc> depthFunc;         ///< depth comparison function

        // Stencil state
        std::optional<bool>           stencilEnabled;   ///< enable stencil testing
        std::optional<StencilOp>      stencilPassOp;    ///< stencil operation on pass
        std::optional<StencilOp>      stencilFailOp;    ///< stencil operation on fail
        std::optional<StencilOp>      stencilZFailOp;   ///< stencil operation on depth fail
        std::optional<ComparisonFunc> stencilFunc;      ///< stencil comparison function
        std::optional<uint8_t>        stencilRef;       ///< stencil reference value
        std::optional<uint8_t>        stencilReadMask;  ///< stencil read mask
        std::optional<uint8_t>        stencilWriteMask; ///< stencil write mask

        // Blend state
        std::optional<bool>     blendEnabled;  ///< enable blending
        std::optional<BlendArg> blendSrc;      ///< source blend factor
        std::optional<BlendArg> blendDst;      ///< destination blend factor
        std::optional<BlendOp>  blendOp;       ///< blend operation
        std::optional<BlendArg> blendAlphaSrc; ///< source alpha blend factor
        std::optional<BlendArg> blendAlphaDst; ///< destination alpha blend factor
        std::optional<BlendOp>  blendAlphaOp;  ///< alpha blend operation
        std::optional<Vector4f> blendFactors;  ///< blend factor color (RGBA)

        // Color write mask (per render target, 4 bits each: RGBA)
        std::optional<uint32_t> colorWriteMask; ///< color write mask (0xFFFFFFFF = all channels)

        // Viewport and scissor
        std::optional<Viewport>    viewport;    ///< viewport settings
        std::optional<ScissorRect> scissorRect; ///< scissor rectangle
    };

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x7c8d9e0f, 0x1a2b, 0x3c4d, {0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e, 0x2f}};
        A(): Arguments(TYPE) {}
        ReadOnly<RenderStateDesc> renderStates; ///< render state descriptor
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    static GN_API AutoRef<SetupRenderStates> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

/// Base class for generic shader actions (draw and compute). Contains common shader resource binding definitions.
struct ShaderAction : public Action {
    struct ShaderResourceBinding {
        uint32_t set  = 0;
        uint32_t slot = 0;

        bool operator==(const ShaderResourceBinding & other) const { return set == other.set && slot == other.slot; }
        bool operator!=(const ShaderResourceBinding & other) const { return !operator==(other); }
        bool operator<(const ShaderResourceBinding & other) const { return (set < other.set) || (set == other.set && slot < other.slot); }
    };

    struct BufferParameter {
        AutoRef<Buffer> buffer;
        uint32_t        offset = 0;
        uint32_t        size   = 0;
    };

    struct ImageParameter {
        AutoRef<Texture>          texture;
        gfx::img::PixelFormat     format = gfx::img::PixelFormat::UNKNOWN();
        Texture::SubresourceIndex subresourceIndex;
        Texture::SubresourceRange subresourceRange;
    };

    struct TextureParameter : public ImageParameter {
        AutoRef<Sampler> sampler;
    };

protected:
    using Action::Action;
};

} // namespace GN::rdg

namespace std {

template<>
struct hash<GN::rdg::ShaderAction::ShaderResourceBinding> {
    size_t operator()(const GN::rdg::ShaderAction::ShaderResourceBinding & key) const {
        auto hash = std::hash<uint32_t>()(key.set);
        GN::combineHash(hash, key.slot);
        return hash;
    }
};

} // namespace std

namespace GN::rdg {

/// Generic draw action for quick GPU draw prototyping. It emphasizes ease of use and flexibility over extreme performance.
struct GenericDraw : public ShaderAction {
    inline static constexpr Guid TYPE = {0x6b7c8d9e, 0x0f1a, 0x2b3c, {0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e}};

    /// Draw parameters
    struct DrawParams {
        uint32_t vertexCount   = 0; ///< number of vertices to draw
        uint32_t instanceCount = 1; ///< number of instances to draw
        uint32_t firstVertex   = 0; ///< first vertex index
        uint32_t firstInstance = 0; ///< first instance index
    };

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x6b7c8d9e, 0x0f1a, 0x2b3c, {0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d, 0x1e}};
        A(): Arguments(TYPE) {}

        ReadOnly<AutoRef<Mesh>>                       mesh;
        ReadOnlyMap<StrA, BufferParameter>            uniforms;      ///< uniform buffers, key is shader variable name
        ReadOnlyMap<StrA, TextureParameter>           textures;      ///< textures, key is shader variable name
        ReadOnly<DrawParams>                          drawParams;    ///< draw parameters
        ReadWriteArray<RenderTarget, 8, UsageFlag::O> renderTargets; ///< color render targets
        ReadWrite<RenderTarget, UsageFlag::O>         depthStencil;  ///< depth/stencil render target (optional)
    };

    /// Shader stage description
    struct ShaderStageDesc {
        AutoRef<Blob> shaderBinary; ///< shader binary code
        StrA          entryPoint;   ///< entry point name
    };

    struct CreateParameters {
        AutoRef<GpuContext>            context;
        std::optional<ShaderStageDesc> vs; ///< vertex shader
        std::optional<ShaderStageDesc> ds; ///< domain shader
        std::optional<ShaderStageDesc> hs; ///< hull shader
        std::optional<ShaderStageDesc> gs; ///< geometry shader
        std::optional<ShaderStageDesc> ps; ///< pixel shader
    };

    static GN_API AutoRef<GenericDraw> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using ShaderAction::ShaderAction;
};

/// Generic compute action for quick GPU compute prototyping. It emphasizes ease of use and flexibility over extreme performance.
struct GenericCompute : public ShaderAction {
    inline static constexpr Guid TYPE = {0x5a6b7c8d, 0x9e0f, 0x1a2b, {0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d}};

    /// Dispatch dimensions (thread group counts)
    struct DispatchSize {
        uint32_t x = 1;
        uint32_t y = 1;
        uint32_t z = 1;
    };

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x5a6b7c8d, 0x9e0f, 0x1a2b, {0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d}};
        A(): Arguments(TYPE) {}

        ReadOnlyMap<ShaderResourceBinding, BufferParameter>  uniforms; ///< uniform buffers
        ReadOnlyMap<ShaderResourceBinding, TextureParameter> textures; ///< textures
        ReadWriteMap<ShaderResourceBinding, BufferParameter> buffers;  ///< storage buffers
        ReadWriteMap<ShaderResourceBinding, ImageParameter>  images;   ///< storage images
        ReadOnly<DispatchSize>                               groups;   ///< thread group counts
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    static GN_API AutoRef<GenericCompute> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using ShaderAction::ShaderAction;
};

// /// Composes one solid color and a set of textures into a single output texture.
// /// Inputs: one color (set on the action) and up to MAX_INPUT_TEXTURES texture parameters.
// /// Output: one texture (parameter "output").
// struct Compose : public ShaderAction {
//     inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

//     struct A : public Arguments {
//         inline static constexpr Guid                            TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
//         ReadOnly<AutoRef<Mesh>>                                 mesh;
//         ReadOnly<Vector4f>                                      color;
//         ReadOnlyArray<AutoRef<Texture>, 8, Arguments::Optional> textures;
//         ReadWriteArray<RenderTarget, 8, Arguments::Optional>    renderTargets;
//         ReadWrite<RenderTarget>                                 depthStencil;
//     };

//     virtual bool reset() = 0;

// protected:
//     using ShaderAction::ShaderAction;
// };

// /// Physically Based Rendering (PBR) action using Disney PBR shading model.
// /// Renders objects with realistic material properties including base color, metallic, roughness, and normal mapping.
// struct PBRShading : public ShaderAction {
//     inline static constexpr Guid TYPE = {0x8b9c0d1e, 0x2f3a, 0x4b5c, {0x6d, 0x7e, 0x8f, 0x9a, 0x0b, 0x1c, 0x2d, 0x3e}};

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
//         inline static constexpr Guid TYPE = {0x8b9c0d1e, 0x2f3a, 0x4b5c, {0x6d, 0x7e, 0x8f, 0x9a, 0x0b, 0x1c, 0x2d, 0x3e}};

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
//     using ShaderAction::ShaderAction;
// };

} // namespace GN::rdg
