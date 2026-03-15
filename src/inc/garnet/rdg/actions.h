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
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    struct A : public Arguments {
        GN_API GN_RDG_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}

        AutoRef<RenderTarget> renderTarget;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (!renderTarget) return;
            for (const auto & color : renderTarget->colors) {
                if (color.target.artifact) { list.writeList.insert(color.target.artifact.get()); }
            }
            if (renderTarget->depthStencilTarget.artifact) {
                const Artifact * ptr = renderTarget->depthStencilTarget.artifact.get();
                if (renderTarget->depthState.testEnabled() || renderTarget->stencilState.enabled()) list.readList.insert(ptr);
                if (renderTarget->depthState.writeEnabled() || renderTarget->stencilState.enabled()) list.writeList.insert(ptr);
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
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    struct A : public Arguments {
        GN_API GN_RDG_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}

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
    static GN_API AutoRef<PrepareBackbuffer> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

/// This is the action to set the backbuffer into presented state and make it non-renderable.
/// It must be called in pair with PrepareBackbuffer action to properly
/// rotate the swapchain buffers for rendering and presenting.
struct PresentBackbuffer : public Action {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    struct A : public Arguments {
        GN_API GN_RDG_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}

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
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Action);

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
        GN_API GN_RDG_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}

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
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    /// Shader binary that can be used to create the actual GPU shader program.
    struct ShaderBinary {
        void *       binary = nullptr; ///< pointer to the shader binary code.
        size_t       size   = 0;       ///< size of the shader binary code.
        const char * entry  = nullptr; ///< entry point name.

        bool empty() const { return binary == nullptr || size == 0 || entry == nullptr; }
        bool valid() const { return binary != nullptr && size > 0 && entry != nullptr; }
    };

    /// Represent a list of GPU resources of same type. Bindable to a shader binding slot (usually mapped to a named shader
    /// variable).
    using GraphicsResourceSlot = DynaArray<GpuResourceView>;

    // A resource set/group is an array of resource slots.
    using GraphicsResourceSet = DynaArray<GraphicsResourceSlot>;

    /// A resource table is an array of resoruce sets/groups.
    /// It can also be treated as 3-D array of GPU resource views.
    /// The first dimension is the resource set/group index.
    /// The second dimension is the resource slot index inside a resource set/group.
    /// The third dimension is the resource view index inside a resource slot.
    using GpuResourceTable = DynaArray<GraphicsResourceSet>;

protected:
    using Action::Action;
};

/// Generic GPU draw action. This is the building block of all other draw actions and effects.
/// It depends on ClearRenderTarget action to set the render target. Without it, this action will simply fail.
struct GpuDraw : public GpuShaderAction {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(GpuShaderAction);

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

    struct GN_API A : public Arguments {
        GN_RDG_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}

        GpuGeometry geometry; ///< geometry
        /// 3-D resource table: resources[setIndex][bindingIndex][arrayIndex] = GpuResourceView.
        /// Backend feeds this into rapid_vulkan Drawable (layout/descriptors managed by DrawPack).
        GpuResourceTable   resources;  ///< shader resources
        DynaArray<uint8_t> immediates; ///< immediate constants. Backend copies to GPU when non-empty.

        /// Render target.
        /// \note Must specify render target as argument for each draw action.
        /// So that the backend can determine this action's full read and write list.
        AutoRef<RenderTarget> renderTarget;

        void addToReadWriteList(ArtifactReadWriteList & list) const override;
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
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(GpuShaderAction);

    /// Dispatch dimensions (thread group counts)
    struct DispatchSize {
        uint32_t x = 1;
        uint32_t y = 1;
        uint32_t z = 1;
    };

    struct GN_API A : public Arguments {
        GN_RDG_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}

        GpuResourceTable   resources;  ///< shader resources
        DynaArray<uint8_t> immediates; ///< immediate constants. Backend copies to GPU when non-empty.
        DispatchSize       groups;     ///< thread group counts

        void addToReadWriteList(ArtifactReadWriteList & list) const override;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
        ShaderBinary        cs; ///< compute shader
    };

    static GN_API AutoRef<GpuCompute> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using GpuShaderAction::GpuShaderAction;
};

/// Level 2: stateless gpu resource copy action.
///
/// Copies a byte range from a source to a destination buffer.
/// The source may be a regular Buffer artifact or a TransientBuffer (for
/// CPU→GPU uploads via the transient pool).
struct GpuCopy : public Action {
    GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    struct BufferToBuffer : public Arguments {
        GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Arguments);
        BufferToBuffer(): Arguments(TYPE_INFO()) {}

        AutoRef<Buffer> src;
        AutoRef<Buffer> dst;
        uint64_t        srcOffset = 0; ///< Additional offset within the source (added to TransientBuffer::offset() when applicable).
        uint64_t        dstOffset = 0; ///< Byte offset in the destination buffer.
        uint64_t        size      = 0; ///< Number of bytes to copy. 0 = copy nothing.

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (src) list.readList.insert(src.get());
            if (dst) list.writeList.insert(dst.get());
        }
    };

    struct Region {
        // Defines the image region that will be copied from/to.
        GpuResourceView::SubresourceIndex imageSubresource;
        Vector3<uint32_t>                 imageOffset;
        Vector3<uint32_t>                 imageExtent;

        /// Offset in bytes from the beginning of the source buffer to the start of the region to copy
        uint64_t bufferOffset = 0;

        // Row length, in pixels, of the image stored in buffer. Set to 0 to use imageExtent.x.
        uint32_t bufferRowLength = 0;

        // Height of the image data, in pixels,stored in buffer. Set to 0 to use imageExtent.y.
        uint32_t bufferHeight = 0;
    };

    struct BufferToImage : public Arguments {
        GN_API GN_RDG_REGISTER_RUNTIME_TYPE(Arguments);
        BufferToImage(): Arguments(TYPE_INFO()) {}

        AutoRef<TransientBuffer> src;
        AutoRef<Texture>         dst;
        DynaArray<Region>        regions;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (src) list.readList.insert(src.get());
            if (dst) list.writeList.insert(dst.get());
        }
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    static GN_API AutoRef<GpuCopy> create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

protected:
    using Action::Action;
};

} // namespace GN::rdg
