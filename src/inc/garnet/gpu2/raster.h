// GPU raster: render targets and raster pass types.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/raster.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

namespace GN::gpu2 {

// -----------------------------
// Render target
// -----------------------------

struct RenderTarget {
    struct BlendState {
        enum Arg {
            ZERO = 0,
            ONE,
            SRC_COLOR,
            INV_SRC_COLOR,
            SRC_ALPHA,
            INV_SRC_ALPHA,
            DEST_ALPHA,
            INV_DEST_ALPHA,
            DEST_COLOR,
            INV_DEST_COLOR,
            BLEND_FACTOR,
            INV_BLEND_FACTOR,
        };

        enum Op {
            ADD,
            SUB,
            REV_SUB,
            MIN,
            MAX,
        };

        // Default blend mode is disabled.
        Op       colorOp  = Op::ADD;
        Arg      colorSrc = Arg::ONE;
        Arg      colorDst = Arg::ZERO;
        Op       alphaOp  = Op::ADD;
        Arg      alphaSrc = Arg::ONE;
        Arg      alphaDst = Arg::ZERO;
        Vector4f factors  = {0.0f, 0.0f, 0.0f, 0.0f};

        constexpr bool enabled() const {
            return colorOp != Op::ADD || colorSrc != Arg::ONE || colorDst != Arg::ZERO || alphaOp != Op::ADD || alphaSrc != Arg::ONE || alphaDst != Arg::ZERO;
        }
        constexpr bool operator==(const BlendState & other) const {
            return colorOp == other.colorOp && colorSrc == other.colorSrc && colorDst == other.colorDst && alphaOp == other.alphaOp &&
                   alphaSrc == other.alphaSrc && alphaDst == other.alphaDst;
        }
        constexpr bool operator!=(const BlendState & other) const { return !operator==(other); }
    };

    enum class Compare {
        NEVER = 0,     // no read, no write
        LESS,          // read and write
        LESS_EQUAL,    // read and write
        EQUAL,         // read and write
        GREATER_EQUAL, // read and write
        GREATER,       // read and write
        NOT_EQUAL,     // read and write
        ALWAYS,        // write only
    };

    struct DepthState {
        // default state equals to depth disabled.
        Compare func  = Compare::ALWAYS;
        bool    write = false;

        constexpr bool testEnabled() const { return Compare::NEVER != func && Compare::ALWAYS != func; }
        constexpr bool writeEnabled() const { return Compare::NEVER != func && write; }
        constexpr bool operator==(const DepthState & other) const { return func == other.func && write == other.write; }
        constexpr bool operator!=(const DepthState & other) const { return !operator==(other); }
    };

    struct StencilState {
        enum Op {
            KEEP = 0, // no read, no write
            ZERO,     // write only
            REPLACE,  // write only
            INC_SAT,  // read and write
            DEC_SAT,  // read and write
            INVERT,   // read and write
            INC,      // read and write
            DEC,      // read and write
        };

        // default to an state that stencil is effectively disabled.
        Compare compare   = Compare::ALWAYS; ///< stencil comparison function
        Op      pass      = KEEP;            ///< stencil operation on pass
        Op      fail      = KEEP;            ///< stencil operation on fail
        Op      zFail     = KEEP;            ///< stencil operation on depth fail
        uint8_t ref       = 0;               ///< stencil reference value
        uint8_t readMask  = 0xFF;            ///< stencil read mask
        uint8_t writeMask = 0xFF;            ///< stencil write mask

        constexpr bool enabled() const {
            bool read  = (0 != readMask) && (Compare::NEVER != compare) && (Compare::ALWAYS != compare);
            bool write = (0 != writeMask) && ((Op::KEEP != pass) || (Op::KEEP != fail) || (Op::KEEP != zFail));
            return read || write;
        }
        constexpr bool operator==(const StencilState & other) const {
            return pass == other.pass && fail == other.fail && zFail == other.zFail && compare == other.compare && ref == other.ref &&
                   readMask == other.readMask && writeMask == other.writeMask;
        }
        constexpr bool operator!=(const StencilState & other) const { return !operator==(other); }
    };

    /// Viewport settings. Defines transform of normalized device coordinates (NDC) to Window coordinates.
    ///   - Left top is (-1, 1) in NDC space, map to Window space coordinate (0, 0).
    ///   - Right bottom is (1, -1) in NDC space, map to Window space coordinate (width, height).
    ///   - Set width and/or heigh to FLT_MAX indicating the current size of the render target.
    struct Viewport {
        float x        = 0.0f;
        float y        = 0.0f;
        float width    = FLT_MAX; ///< default to current size of the render target.
        float height   = FLT_MAX; ///< default to current size of the render target.
        float minDepth = 0.0f;
        float maxDepth = 1.0f;

        constexpr bool fullScreen() const { return 0.0f == x && 0.0f == y && FLT_MAX == width && FLT_MAX == height; }
        constexpr bool operator==(const Viewport & other) const {
            return x == other.x && y == other.y && width == other.width && height == other.height && minDepth == other.minDepth && maxDepth == other.maxDepth;
        }
        constexpr bool operator!=(const Viewport & other) const { return !operator==(other); }
    };

    /// Scissor rectangle in Window coordinates. (0, 0) is the left top corner of the window.
    struct ScissorRect {
        int32_t  x      = 0;
        int32_t  y      = 0;
        uint32_t width  = (~0u); ///< Set to (~0u) indicating with of the current window.
        uint32_t height = (~0u); ///< Set to (~0u) indicating height of the current window.

        constexpr bool disabled() const { return (0 == x) && (0 == y) && (~0u == width) && (~0u == height); }
        constexpr bool operator==(const ScissorRect & other) const { return x == other.x && y == other.y && width == other.width && height == other.height; }
        constexpr bool operator!=(const ScissorRect & other) const { return !operator==(other); }
    };

    union ClearColorValue {
        float    f4[4];
        uint32_t u4[4];
        int32_t  i4[4];
    };

    struct ColorTarget {
        GpuResourceView target     = {};   // color target: typically \c Texture (e.g. swapchain frame from \c Swapchain::prepare())
        BlendState      blendState = {};   // blend state for the color target
        uint8_t         writeMask  = 0xFF; // 4 lower bits are write mask for R, G, B, A. Other bits are ignored.

        bool operator==(const ColorTarget & other) const { return target == other.target && blendState == other.blendState && writeMask == other.writeMask; }
        bool operator!=(const ColorTarget & other) const { return !operator==(other); }
    };

    StackArray<ColorTarget, 8> colors;
    ClearColorValue            clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}}; // clear to solid black.
    GpuResourceView            depthStencilTarget;                      // must be a texture
    DepthState                 depthState   = {};
    StencilState               stencilState = {};
    float                      clearDepth   = 1.0;
    uint32_t                   clearStencil = 0;
    Viewport                   viewport     = {};
    ScissorRect                scissorRect  = {};

    /// Add a color target from an existing view.
    RenderTarget & addColorTarget(const GpuResourceView & view) {
        colors.append(ColorTarget {.target = view, .blendState = {}});
        return *this;
    }

    RenderTarget & setColorTarget(size_t index, const GpuResourceView & target) {
        GN_ASSERT(index < colors.size());
        colors[index].target = target;
        return *this;
    }

    RenderTarget & setClearColor(float r, float g, float b, float a = 1.0f) {
        clearColor.f4[0] = r;
        clearColor.f4[1] = g;
        clearColor.f4[2] = b;
        clearColor.f4[3] = a;
        return *this;
    }

    RenderTarget & setDepthStencilTarget(GpuResourceView target) {
        depthStencilTarget = target;
        return *this;
    }

    RenderTarget & setDepthState(DepthState depthState_) {
        depthState = depthState_;
        return *this;
    }

    RenderTarget & setStencilState(StencilState stencilState_) {
        stencilState = stencilState_;
        return *this;
    }

    RenderTarget & setClearDepth(float clearDepth_) {
        clearDepth = clearDepth_;
        return *this;
    }

    RenderTarget & setClearStencil(uint32_t clearStencil_) {
        clearStencil = clearStencil_;
        return *this;
    }

    RenderTarget & setViewport(Viewport viewport_) {
        viewport = viewport_;
        return *this;
    }

    RenderTarget & setScissorRect(ScissorRect scissorRect_) {
        scissorRect = scissorRect_;
        return *this;
    }
};

// -----------------------------
// Render states
// -----------------------------

struct RenderStates {
    /// Raster fill mode (mirrors v1 \c SetupRenderStates::FillMode).
    enum FillMode {
        FILL_SOLID = 0,
        FILL_WIREFRAME,
        FILL_POINT,
    };

    /// Face culling (mirrors v1 \c SetupRenderStates::CullMode).
    enum CullMode {
        CULL_NONE = 0,
        CULL_FRONT,
        CULL_BACK,
    };

    /// Front-face winding (mirrors v1 \c SetupRenderStates::FrontFace).
    enum FrontFace {
        FRONT_CCW = 0,
        FRONT_CW,
    };

    std::optional<FillMode>  fillMode;  ///< fill mode (solid, wireframe, point)
    std::optional<CullMode>  cullMode;  ///< cull mode (none, front, back)
    std::optional<FrontFace> frontFace; ///< front face winding (CCW, CW)
};

// -----------------------------
// RenderGeometry
// -----------------------------

/// GPU renderable geometry (mirrors v1 \c GpuDraw::GpuGeometry in \c actions.h).
struct RenderGeometry {
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
        AutoRef<Buffer> buffer; ///< buffer containing the geometry data
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

// -----------------------------
// Raster
// -----------------------------

/// Represent a graphical render pass
class GpuRaster : public RootEntity {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
        RenderTarget        renderTarget;
    };
    GN_API static AutoRef<GpuRaster> create(const CreateParameters &);

    struct DrawParameters {
        AutoRef<GpuShader>  vs, hs, ds, gs, ps;
        RenderStates        renderStates;
        RenderGeometry      geometry;
        GpuResourceTable    resources;
        ArrayProxy<uint8_t> immediates;
    };
    virtual void draw(const DrawParameters &) = 0;

    /// Finalize the render pass. Returns a GpuPayload* to pass to GpuContext::submit().
    /// After this call this GpuRaster object becomes unusable and is safe to release.
    /// All necessary data need to render are sealed inside the returned GpuPayload object.
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RootEntity::RootEntity;
};

} // namespace GN::gpu2
