// GPU swapchain: presentation and frame management.
// Include via <garnet/GNgpu2.h>.
#if !defined(__GN_INSIDE_GPU2_H__)
    #error "Do not include <garnet/gpu2/swapchain.h> directly. Include <garnet/GNgpu2.h> instead."
#endif

namespace GN::gpu2 {

struct Swapchain : RootEntity {
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    struct CreateDesc {
        AutoRef<GpuContext> gpu;
        StrA                name = "swapchain";
        /// Native surface handle (e.g. VkSurfaceKHR as intptr_t). Zero is valid for headless swapchains.
        intptr_t              window = 0;
        gfx::img::PixelFormat format = gfx::img::PixelFormat::UNKNOWN();
        uint32_t              width  = 1280;
        uint32_t              height = 720;

        CreateDesc & setGpu(AutoRef<GpuContext> g) {
            gpu = std::move(g);
            return *this;
        }
        CreateDesc & setName(const StrA & n) {
            name = n;
            return *this;
        }
        CreateDesc & setWindow(intptr_t handle) {
            window = handle;
            return *this;
        }
        CreateDesc & setFormat(gfx::img::PixelFormat f) {
            format = f;
            return *this;
        }
        CreateDesc & setDimensions(uint32_t w, uint32_t h) {
            width  = w;
            height = h;
            return *this;
        }
    };

    static GN_API AutoRef<Swapchain> create(const CreateDesc &);

    struct Frame {
        GpuResourceView     view;  ///< Render into this. Empty on failure.
        AutoRef<GpuPayload> ready; ///< Waited by other GPU work to ensure the frame is ready to be drawn to (finished presenting of previous frame).
    };

    /// Acquire the next swapchain image. Returns an empty Frame on failure.
    virtual Frame prepare() = 0;

    /// Schedule present to execute on GPU after \p waitFor completes.
    virtual void present(GpuPayload & waitFor) = 0;

protected:
    using RootEntity::RootEntity;
};

} // namespace GN::gpu2
