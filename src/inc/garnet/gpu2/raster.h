// GPU raster: render targets and raster pass types.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/raster.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

namespace GN::gpu2 {

// -----------------------------
// RasterState
// -----------------------------
// All fields are std::optional<>. Two construction modes:
//   RasterState{}            — default ctor, all fields empty (used by draw parameters: absent = inherit from RasterTarget)
//   RasterState{WithDefaults} — sets every field to its logical default (used by RasterTarget::renderState)
//
// Draw-level overrides are persistent within a render pass: if a draw sets viewport, all subsequent
// draws in the same GpuRaster are affected until overridden again. Changes do not propagate outside
// the pass; each GpuRaster owns its RasterTarget which carries the full default baseline.

struct RasterState {
    // ---- nested types ----

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
        Compare func  = Compare::ALWAYS; // depth disabled by default
        bool    write = false;

        constexpr bool testEnabled() const { return Compare::NEVER != func && Compare::ALWAYS != func; }
        constexpr bool writeEnabled() const { return Compare::NEVER != func && write; }
        constexpr bool operator==(const DepthState & o) const { return func == o.func && write == o.write; }
        constexpr bool operator!=(const DepthState & o) const { return !operator==(o); }
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

        Compare compare   = Compare::ALWAYS; ///< stencil comparison function; disabled by default
        Op      pass      = KEEP;            ///< stencil operation on pass
        Op      fail      = KEEP;            ///< stencil operation on fail
        Op      zFail     = KEEP;            ///< stencil operation on depth fail
        uint8_t ref       = 0;
        uint8_t readMask  = 0xFF;
        uint8_t writeMask = 0xFF;

        constexpr bool enabled() const {
            bool read  = (0 != readMask) && (Compare::NEVER != compare) && (Compare::ALWAYS != compare);
            bool write = (0 != writeMask) && ((KEEP != pass) || (KEEP != fail) || (KEEP != zFail));
            return read || write;
        }
        constexpr bool operator==(const StencilState & o) const {
            return compare == o.compare && pass == o.pass && fail == o.fail && zFail == o.zFail && ref == o.ref && readMask == o.readMask &&
                   writeMask == o.writeMask;
        }
        constexpr bool operator!=(const StencilState & o) const { return !operator==(o); }
    };

    /// Viewport: NDC (-1,1) top-left maps to window (0,0); (1,-1) bottom-right maps to (width,height).
    /// Set width/height to FLT_MAX to match the current render target size.
    struct Viewport {
        float x        = 0.0f;
        float y        = 0.0f;
        float width    = FLT_MAX;
        float height   = FLT_MAX;
        float minDepth = 0.0f;
        float maxDepth = 1.0f;

        constexpr bool fullScreen() const { return 0.0f == x && 0.0f == y && FLT_MAX == width && FLT_MAX == height; }
        constexpr bool operator==(const Viewport & o) const {
            return x == o.x && y == o.y && width == o.width && height == o.height && minDepth == o.minDepth && maxDepth == o.maxDepth;
        }
        constexpr bool operator!=(const Viewport & o) const { return !operator==(o); }
    };

    /// Scissor in window coordinates. Set width/height to (~0u) to cover the whole window.
    struct ScissorRect {
        int32_t  x      = 0;
        int32_t  y      = 0;
        uint32_t width  = (~0u);
        uint32_t height = (~0u);

        constexpr bool disabled() const { return (0 == x) && (0 == y) && (~0u == width) && (~0u == height); }
        constexpr bool operator==(const ScissorRect & o) const { return x == o.x && y == o.y && width == o.width && height == o.height; }
        constexpr bool operator!=(const ScissorRect & o) const { return !operator==(o); }
    };

    enum FillMode { FILL_SOLID = 0, FILL_WIREFRAME, FILL_POINT };
    enum CullMode { CULL_NONE = 0, CULL_FRONT, CULL_BACK };
    enum FrontFace { FRONT_CCW = 0, FRONT_CW };

    // ---- construction ----

    struct WithDefaults {}; ///< tag to request default-value initialization

    RasterState() = default; ///< all fields empty (nullopt); used by draw parameters

    RasterState(WithDefaults) { resetToDefault(); } ///< used by RasterTarget::renderState

    /// Fills every field with its logical default value (non-empty).
    void resetToDefault() {
        fillMode     = FILL_SOLID;
        cullMode     = CULL_BACK;
        frontFace    = FRONT_CCW;
        depthState   = DepthState {};
        stencilState = StencilState {};
        viewport     = Viewport {};
        scissorRect  = ScissorRect {};
    }

    // ---- fields ----

    std::optional<FillMode>     fillMode;
    std::optional<CullMode>     cullMode;
    std::optional<FrontFace>    frontFace;
    std::optional<DepthState>   depthState;
    std::optional<StencilState> stencilState;
    std::optional<Viewport>     viewport;
    std::optional<ScissorRect>  scissorRect;

    // ---- setters (chainable) ----

    RasterState & setFillMode(FillMode v) {
        fillMode = v;
        return *this;
    }
    RasterState & setCullMode(CullMode v) {
        cullMode = v;
        return *this;
    }
    RasterState & setFrontFace(FrontFace v) {
        frontFace = v;
        return *this;
    }
    RasterState & setDepthState(DepthState v) {
        depthState = v;
        return *this;
    }
    RasterState & setStencilState(StencilState v) {
        stencilState = v;
        return *this;
    }
    RasterState & setViewport(Viewport v) {
        viewport = v;
        return *this;
    }
    RasterState & setScissorRect(ScissorRect v) {
        scissorRect = v;
        return *this;
    }
};

// -----------------------------
// Raster target
// -----------------------------

struct RasterTarget {
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

    union ClearColorValue {
        float    f4[4];
        uint32_t u4[4];
        int32_t  i4[4];
    };

    struct ColorTarget {
        GpuResourceView target     = {};
        BlendState      blendState = {};
        uint8_t         writeMask  = 0xFF; // 4 lower bits: write mask for R, G, B, A

        bool operator==(const ColorTarget & other) const { return target == other.target && blendState == other.blendState && writeMask == other.writeMask; }
        bool operator!=(const ColorTarget & other) const { return !operator==(other); }
    };

    StackArray<ColorTarget, 8> colorTargets;
    GpuResourceView            depthStencilTarget;
    ClearColorValue            clearColor   = {{0.0f, 0.0f, 0.0f, 1.0f}};
    float                      clearDepth   = 1.0f;
    uint32_t                   clearStencil = 0;
    RasterState                states       = RasterState::WithDefaults {}; ///< full render state baseline; every field has a value

    RasterTarget & setColorTarget(size_t index, const GpuResourceView & target) {
        if (index >= colorTargets.MAX_SIZE) GN_UNLIKELY {
                GN_ERROR(getLogger("GN.gpu2"))("Invalid color target index: %zu. Max supported is %zu.", index, colorTargets.MAX_SIZE);
                return *this;
            }
        if (index >= colorTargets.size()) GN_UNLIKELY {
                colorTargets.resize(index + 1);
            }
        colorTargets[index].target = target;
        return *this;
    }

    RasterTarget & setClearColor(float r, float g, float b, float a = 1.0f) {
        clearColor.f4[0] = r;
        clearColor.f4[1] = g;
        clearColor.f4[2] = b;
        clearColor.f4[3] = a;
        return *this;
    }

    RasterTarget & setDepthStencilTarget(GpuResourceView target) {
        depthStencilTarget = target;
        return *this;
    }

    RasterTarget & setClearDepth(float clearDepth_) {
        clearDepth = clearDepth_;
        return *this;
    }

    RasterTarget & setClearStencil(uint32_t clearStencil_) {
        clearStencil = clearStencil_;
        return *this;
    }
};

// -----------------------------
// RasterGeometry
// -----------------------------

/// GPU renderable geometry (mirrors v1 \c GpuDraw::GpuGeometry in \c actions.h).
struct RasterGeometry {
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

    struct VertexAttribute {
        uint32_t        location = 0;
        uint32_t        offset   = 0;
        AttributeFormat format   = AttributeFormat::F32_3;

        bool operator==(const VertexAttribute & other) const { return location == other.location && format == other.format && offset == other.offset; }
        bool operator!=(const VertexAttribute & other) const { return !operator==(other); }
    };

    struct VertexFormat {
        DynaArray<VertexAttribute> attributes;

        bool empty() const { return attributes.empty(); }
        bool operator==(const VertexFormat & other) const { return attributes == other.attributes; }
        bool operator!=(const VertexFormat & other) const { return !operator==(other); }
    };

    struct GeometryBuffer {
        AutoRef<Buffer> buffer;
        uint64_t        offset = 0;
        uint32_t        stride = 0;
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

/// Represent a graphical raster pass
class GpuRaster : public RootEntity {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
        RasterTarget        target;
        size_t              numberOfDrawsHint =
            1000; ///< optional hint for expected number of draw calls, used to minimize internal allocations. The number of draws can exceed this hint.
    };
    GN_API static AutoRef<GpuRaster> create(const CreateParameters &);

    struct DrawParameters {
        AutoRef<GpuShader> vs, hs, ds, gs, ps;
        RasterState states; ///< raster state overrides. overrides are transient and only affect the current draw call. empty fields are inherited from the
                            ///< RasterTarget's baseline state.
        RasterGeometry      geometry;
        GpuResourceTable    resources;
        ArrayProxy<uint8_t> immediates;
    };
    virtual void draw(const DrawParameters &) = 0;

    /// @brief Get the render target associated with this raster. The target is immutable and fully defined at creation time. It can be used to inspect the
    /// target attachments and their properties, but not to modify them.
    virtual const RasterTarget & target() const = 0;

    /// Finalize the render pass. Returns a payload to pass to GpuContext::submit().
    /// After this call the GpuRaster is unusable and safe to release.
    virtual AutoRef<GpuPayload> seal() = 0;

protected:
    using RootEntity::RootEntity;
};

} // namespace GN::gpu2
