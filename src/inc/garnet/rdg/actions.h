#pragma once

#include <variant>
#include <map>
#include <optional>

namespace GN::rdg {

// Representa a view to a GPU image. Could be a texture or a backbuffer.
struct GpuImageView {
    struct SubresourceIndex {
        uint32_t mip  = 0; ///< index into mipmap chain
        uint32_t face = 0; ///< index into array of faces

        bool operator==(const SubresourceIndex & other) const { return mip == other.mip && face == other.face; }
        bool operator!=(const SubresourceIndex & other) const { return !operator==(other); }
    };

    struct SubresourceRange {
        uint32_t numMipLevels   = (uint32_t) -1; ///< -1 means all mip levels
        uint32_t numArrayLayers = (uint32_t) -1; ///< -1 means all array layers

        bool operator==(const SubresourceRange & other) const { return numMipLevels == other.numMipLevels && numArrayLayers == other.numArrayLayers; }
        bool operator!=(const SubresourceRange & other) const { return !operator==(other); }
    };

    std::variant<AutoRef<Texture>, AutoRef<Backbuffer>> image;
    gfx::img::PixelFormat                               format = gfx::img::PixelFormat::UNKNOWN();
    SubresourceIndex                                    subresourceIndex;
    SubresourceRange                                    subresourceRange;

    bool empty() const { return 0 == image.index() ? std::get<0>(image) == nullptr : std::get<1>(image) == nullptr; }
    bool isTexture() const { return image.index() == 0; }
    bool isBackbuffer() const { return image.index() == 1; }

    AutoRef<Artifact> artifact() const {
        if (image.index() == 0)
            return std::get<0>(image);
        else
            return std::get<1>(image);
    }

    bool operator==(const GpuImageView & other) const {
        return image == other.image && format == other.format && subresourceIndex == other.subresourceIndex && subresourceRange == other.subresourceRange;
    }
    bool operator!=(const GpuImageView & other) const { return !operator==(other); }
};

struct TextureView : GpuImageView {
    AutoRef<Sampler> sampler;
};

struct RenderTarget {
    struct DepthStencil {
        AutoRef<Texture>               target;
        gfx::img::PixelFormat          format = gfx::img::PixelFormat::UNKNOWN();
        GpuImageView::SubresourceIndex subresourceIndex {};

        bool empty() const { return !target; }

        bool operator==(const DepthStencil & other) const {
            if (target != other.target) return false;
            if (target && subresourceIndex != other.subresourceIndex) return false; // only check subresource index for non-empty texture targets
            return true;
        }

        bool operator!=(const DepthStencil & other) const { return !operator==(other); }
    };

    StackArray<GpuImageView, 8> colors;
    DepthStencil                depthStencil;

    bool empty() const { return colors.empty() && depthStencil.empty(); }

    bool operator==(const RenderTarget & other) const { return colors == other.colors && depthStencil == other.depthStencil; }
    bool operator!=(const RenderTarget & other) const { return !operator==(other); }
};

struct RenderTargetArgument : public Arguments::ArtifactArgument {
    RenderTargetArgument(Arguments * owner, const char * name)
        : Arguments::ArtifactArgument(owner, name, Arguments::Usage::Writing + Arguments::Usage::Reading) {}

    SafeArrayAccessor<const Artifact * const> artifacts() const override {
        mArtifacts.reserve(8 + 1);
        mArtifacts.clear();
        const auto & colors = value.colors;
        for (size_t i = 0; i < colors.size(); ++i) {
            if (!colors[i].empty()) {
                auto a = colors[i].artifact();
                if (a) mArtifacts.append(a.get());
            }
        }
        if (!value.depthStencil.empty()) mArtifacts.append(value.depthStencil.target.get());
        return mArtifacts;
    };

    RenderTarget value;

private:
    mutable DynaArray<const Artifact *> mArtifacts;
};

/// Clear render target to certain value. Discard existing content.
/// This is the recommended first action to start rendering to a render target.
/// It tells GPU to discard existing content thus avoid expensive image layout transitions.
struct ClearRenderTarget : public Action {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "ClearRenderTarget";

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "ClearRenderTarget::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}
        struct ClearValues {
            union {
                float    f4[4];
                uint32_t u4[4];
                int32_t  i4[4];
            } colors[8];
            float    depth;
            uint32_t stencil;
        };
        RenderTargetArgument renderTarget = {this, "renderTarget"};
        ClearValues          clearValues;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    /// Create a new instance and register to the database via admit(). Implementation provided by backend.
    static GN_API AutoRef<ClearRenderTarget> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

struct PrepareBackbuffer : public Action {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "PrepareBackbuffer";

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "PrepareBackbuffer::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        ReadWriteArtifact<Backbuffer> backbuffer = {this, "backbuffer"}; // Backbuffer to prepare
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
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "PresentBackbuffer";

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "PresentBackbuffer::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        ReadOnlyArtifact<Backbuffer> backbuffer = {this, "backbuffer"}; // Backbuffer to present
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
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "SetupRenderStates::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}
        RenderStateDesc renderStates; ///< render state descriptor
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    static GN_API AutoRef<SetupRenderStates> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

/// Represent a GPU renderable geometry.
struct GpuGeometry {
    struct VertexFormat {
        // TBD
    };

    struct GeometryBuffer : BufferView {
        /// For vertex buffers, this is the size of the vertex in bytes.
        /// For index buffers, this is the size of the index in bytes. Must be 2 or 4.
        /// For instanced buffers, this is the size of the instance in bytes.
        uint32_t stride = 0;

        /// Number of elements in the buffer.
        size_t count() const { return size / stride; }
    };

    VertexFormat              format;
    DynaArray<GeometryBuffer> instances;
    DynaArray<GeometryBuffer> vertices;
    GeometryBuffer            indices;
};

/// Base class for generic shader actions (draw and compute). Contains common shader resource binding definitions.
struct GpuShaderAction : public Action {
    // struct ShaderResourceBinding {
    //     uint32_t set  = 0;
    //     uint32_t slot = 0;

    //     bool operator==(const ShaderResourceBinding & other) const { return set == other.set && slot == other.slot; }
    //     bool operator!=(const ShaderResourceBinding & other) const { return !operator==(other); }
    //     bool operator<(const ShaderResourceBinding & other) const { return (set < other.set) || (set == other.set && slot < other.slot); }
    // };

    template<Arguments::UsageBits UFlags>
    struct BufferViewMap : public Arguments::ArtifactArgument {
        BufferViewMap(Arguments * owner, const char * name): Arguments::ArtifactArgument(owner, name, UFlags + Arguments::Usage::Optional) {}

        SafeArrayAccessor<const Artifact * const> artifacts() const override {
            mArtifacts.reserve(value.size());
            mArtifacts.clear();
            for (const auto & [name, view] : value) {
                (void) name;
                if (view.buffer) { mArtifacts.append(view.buffer.get()); }
            }
            return mArtifacts;
        }

        std::map<StrA, BufferView> value;

    private:
        mutable DynaArray<const Artifact *> mArtifacts;
    };

    template<Arguments::UsageBits UFlags>
    struct ImageViewMap : public Arguments::ArtifactArgument {
        ImageViewMap(Arguments * owner, const char * name): Arguments::ArtifactArgument(owner, name, UFlags + Arguments::Usage::Optional) {}

        SafeArrayAccessor<const Artifact * const> artifacts() const override {
            mArtifacts.clear();
            for (const auto & [name, view] : value) {
                (void) name;
                auto artifact = view.artifact();
                if (artifact) { mArtifacts.append(artifact.get()); }
            }
            return mArtifacts;
        }

        std::map<StrA, GpuImageView> value;

    private:
        mutable DynaArray<const Artifact *> mArtifacts;
    };

    struct TextureMap : public Arguments::ArtifactArgument {
        TextureMap(Arguments * owner, const char * name): Arguments::ArtifactArgument(owner, name, Arguments::Usage::Reading + Arguments::Usage::Optional) {}

        SafeArrayAccessor<const Artifact * const> artifacts() const override {
            mArtifacts.clear();
            for (const auto & [name, view] : value) {
                (void) name;
                auto artifact = view.artifact();
                if (artifact) { mArtifacts.append(artifact.get()); }
                if (view.sampler) { mArtifacts.append(view.sampler.get()); }
            }
            return mArtifacts;
        }

        std::map<StrA, TextureView> value;

    private:
        mutable DynaArray<const Artifact *> mArtifacts;
    };

    /// Shader binary that can be used to create the actual GPU shader program.
    struct ShaderBinary {
        void *       binary = nullptr; ///< pointer to the shader binary code.
        size_t       size   = 0;       ///< size of the shader binary code.
        const char * entry  = nullptr; ///< entry point name.

        bool empty() const { return binary == nullptr || size == 0 || entry == nullptr; }
        bool valid() const { return binary != nullptr && size > 0 && entry != nullptr; }
    };

    using UniformMap  = BufferViewMap<Arguments::Usage::Reading>;
    using RwBufferMap = BufferViewMap<Arguments::Usage::RW>;
    using RoBufferMap = BufferViewMap<Arguments::Usage::Reading>;
    using RwImagesMap = ImageViewMap<Arguments::Usage::RW>;
    using RoImagesMap = ImageViewMap<Arguments::Usage::Reading>;

protected:
    using Action::Action;
};

/// Generic GPU draw action. This is the building block of all other draw actions and effects.
struct GpuDraw : public GpuShaderAction {
    GN_API static const uint64_t         TYPE_ID;
    inline static constexpr const char * TYPE_NAME = "GpuDraw";

    struct GeometryArgument : public Arguments::ArtifactArgument {
        GeometryArgument(Arguments * owner, const char * name)
            : Arguments::ArtifactArgument(owner, name, Arguments::Usage::Reading + Arguments::Usage::Optional) {}

        SafeArrayAccessor<const Artifact * const> artifacts() const override {
            mArtifacts.reserve(value.instances.size() + value.vertices.size() + 1);
            mArtifacts.clear();
            for (const auto & vb : value.instances) {
                if (vb.buffer) { mArtifacts.append(vb.buffer.get()); }
            }
            for (const auto & vb : value.vertices) {
                if (vb.buffer) { mArtifacts.append(vb.buffer.get()); }
            }
            if (value.indices.buffer) { mArtifacts.append(value.indices.buffer.get()); }
            return mArtifacts;
        }

        GpuGeometry value;

    private:
        mutable DynaArray<const Artifact *> mArtifacts;
    };

    struct A : public Arguments {
        GN_API static const uint64_t         TYPE_ID;
        inline static constexpr const char * TYPE_NAME = "GpuDraw::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}

        struct GeometryView : BufferView {
            /// For vertex buffers, this is the size of the vertex in bytes.
            /// For index buffers, this is the size of the index in bytes. Must be 2 or 4.
            uint32_t stride = 0;
        };

        UniformMap           uniforms     = {this, "uniforms"};           ///< uniforms
        TextureMap           textures     = {this, "textures"};           ///< textures
        RwImagesMap          images       = {this, "read-write images"};  ///< read-write images
        RoImagesMap          roImages     = {this, "read-only images"};   ///< read-only images
        RwBufferMap          buffers      = {this, "read-write buffers"}; ///< read-write random access buffers
        RoBufferMap          roBuffers    = {this, "read-only buffers"};  ///< read-only random access buffers
        GeometryArgument     geometry     = {this, "geometry"};           ///< geometry
        RenderTargetArgument renderTarget = {this, "renderTarget"};       ///< render target
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

        UniformMap   uniforms  = {this, "uniforms"};           ///< uniform buffers
        TextureMap   textures  = {this, "textures"};           ///< textures
        RwBufferMap  buffers   = {this, "read-write buffers"}; ///< read-write random access buffers
        RoBufferMap  roBuffers = {this, "read-only buffers"};  ///< read-only random access buffers
        RwImagesMap  images    = {this, "read-write images"};  ///< read-write images
        RoImagesMap  roImages  = {this, "read-only images"};   ///< read-only images
        DispatchSize groups;                                   ///< thread group counts
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
