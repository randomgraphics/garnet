#pragma once

#include <variant>

namespace GN::rdg {

struct ClearRenderTarget : public Action {
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
        struct ClearColor {
            struct {
                float r, g, b, a;
            };
            struct {
                uint32_t x, y, z, w;
            };
        };
        ReadOnly<ClearColor>    color;
        ReadWrite<RenderTarget> renderTarget;
    };

protected:
    using Action::Action;
};

struct ClearDepthStencil : public Action {
    inline static constexpr Guid TYPE = {0xdb3ab1ef, 0xafc0, 0x4eca, {0x80, 0xfa, 0x49, 0xde, 0x23, 0x3c, 0xdf, 0x18}};
    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
        ReadOnly<float>              depth;
        ReadOnly<uint8_t>            stencil;
        ReadWrite<RenderTarget>      depthStencil;
    };

protected:
    using Action::Action;
};

struct LoadTextureFromFile : public Action {
    inline static constexpr Guid TYPE = {0x825a7724, 0xfecb, 0x49e2, {0xb7, 0x1f, 0xfc, 0x9d, 0x3a, 0xa2, 0x8b, 0x11}};
    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x825a7724, 0xfecb, 0x49e2, {0xb7, 0x1f, 0xfc, 0x9d, 0x3a, 0xa2, 0x8b, 0x11}};
        ReadOnly<StrA>               filename; // Path to texture file
        WriteOnly<AutoRef<Texture>>  texture;  // Output texture resource
    };

protected:
    using Action::Action;
};

struct PrepareBackbuffer : public Action {
    inline static constexpr Guid TYPE = {0x3e4f5a6b, 0x7c8d, 0x9e0f, {0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b}};
    struct A : public Arguments {
        inline static constexpr Guid   TYPE = {0x3e4f5a6b, 0x7c8d, 0x9e0f, {0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b}};
        ReadWrite<AutoRef<Backbuffer>> backbuffer; // Backbuffer to prepare
    };

protected:
    using Action::Action;
};

struct PresentBackbuffer : public Action {
    inline static constexpr Guid TYPE = {0x4f5a6b7c, 0x8d9e, 0x0f1a, {0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c}};
    struct A : public Arguments {
        inline static constexpr Guid  TYPE = {0x4f5a6b7c, 0x8d9e, 0x0f1a, {0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c}};
        ReadOnly<AutoRef<Backbuffer>> backbuffer; // Backbuffer to present
    };

protected:
    using Action::Action;
};

struct TextureReadback : public Action {
    inline static constexpr Guid TYPE = {0x5a6b7c8d, 0x9e0f, 0x1a2b, {0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d}};
    struct A : public Arguments {
        inline static constexpr Guid TYPE = {0x5a6b7c8d, 0x9e0f, 0x1a2b, {0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x8b, 0x9c, 0x0d}};
        ReadOnly<AutoRef<Texture>>   texture; ///< input texture
        WriteOnly<gfx::img::Image>   image;   ///< output image (will be cleared and filled with the texture content)
    };

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
        ReadOnly<RenderStateDesc>    renderStates; ///< render state descriptor
    };

protected:
    using Action::Action;
};

/// Base class for generic shader actions (draw and compute). Contains common shader resource binding definitions.
struct ShaderAction : public Action {
    struct ShaderResourceBinding {
        uint32_t set  = 0;
        uint32_t slot = 0;
        bool     operator==(const ShaderResourceBinding & other) const { return set == other.set && slot == other.slot; }
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
    };

    struct TextureParameter : public ImageParameter {
        AutoRef<Sampler>          sampler;
        Texture::SubresourceRange subresourceRange;
    };

protected:
    using Action::Action;
};

/// Composes one solid color and a set of textures into a single output texture.
/// Inputs: one color (set on the action) and up to MAX_INPUT_TEXTURES texture parameters.
/// Output: one texture (parameter "output").
struct Compose : public ShaderAction {
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    struct A : public Arguments {
        inline static constexpr Guid                            TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
        ReadOnly<AutoRef<Mesh>>                                 mesh;
        ReadOnly<Vector4f>                                      color;
        ReadOnlyArray<AutoRef<Texture>, 8, Arguments::OPTIONAL> textures;
        ReadWriteArray<RenderTarget, 8, Arguments::OPTIONAL>    renderTargets;
        ReadWrite<RenderTarget>                                 depthStencil;
    };

protected:
    using ShaderAction::ShaderAction;
};

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

        ReadOnly<AutoRef<Mesh>>                   mesh;
        ReadOnlyMap<StrA, BufferParameter>        uniforms;      ///< uniform buffers, key is shader variable name
        ReadOnlyMap<StrA, TextureParameter>       textures;      ///< textures, key is shader variable name
        ReadOnly<DrawParams>                      drawParams;    ///< draw parameters
        ReadWriteArray<RenderTarget, 8, OPTIONAL> renderTargets; ///< color render targets
        ReadWrite<RenderTarget, OPTIONAL>         depthStencil;  ///< depth/stencil render target (optional)
    };

    /// Shader stage description
    struct ShaderStageDesc {
        AutoRef<Blob> shaderBinary; ///< shader binary code
        StrA          entryPoint;   ///< entry point name
    };

    /// Reset the draw action with shader binaries for different stages.
    /// All parameters are optional - only provided stages will be set.
    virtual bool reset(const std::optional<ShaderStageDesc> & vs = {}, ///< vertex shader
                       const std::optional<ShaderStageDesc> & ds = {}, ///< domain shader
                       const std::optional<ShaderStageDesc> & hs = {}, ///< hull shader
                       const std::optional<ShaderStageDesc> & gs = {}, ///< geometry shader
                       const std::optional<ShaderStageDesc> & ps = {}  ///< pixel shader
                       ) = 0;

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

        ReadOnlyMap<ShaderResourceBinding, BufferParameter>  uniforms; ///< uniform buffers
        ReadOnlyMap<ShaderResourceBinding, TextureParameter> textures; ///< textures
        ReadWriteMap<ShaderResourceBinding, BufferParameter> buffers;  ///< storage buffers
        ReadWriteMap<ShaderResourceBinding, ImageParameter>  images;   ///< storage images
        ReadOnly<DispatchSize>                               groups;   ///< thread group counts
    };

    virtual bool reset(AutoRef<Blob> shaderBinary, const StrA & entryPoint) = 0;

protected:
    using ShaderAction::ShaderAction;
};

} // namespace GN::rdg
