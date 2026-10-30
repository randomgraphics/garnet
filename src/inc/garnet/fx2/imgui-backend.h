#if !defined(__GN_INSIDE_FX2_H__)
    #error "Do not include <garnet/fx2/imgui-backend.h> directly. Include <garnet/GNfx2.h> instead."
#endif

#include <imgui.h>

namespace GN::win {
class Window;
}

namespace GN::gpu2::bindless {
class Raster;
}

namespace GN::fx2 {

/// Dear ImGui platform and renderer backend using GN::win input and gpu2 draw payloads.
/// The caller controls frame ordering and raster target blending.
struct ImGuiBackend : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);

    struct CreateParameters {
        AutoRef<gpu2::GpuContext> gpu;
        win::Window &             window;
        uint32_t                  resourceSetIndex = 0; ///< Descriptor set index for ImGui texture bindings (default 0).
    };

    /// Create a backend and a dedicated Dear ImGui context. Only one backend may be current
    /// on a thread while client code calls ImGui functions.
    GN_API static AutoRef<ImGuiBackend> create(const CreateParameters &);

    /// Select this backend's context and begin a UI frame.
    virtual void newFrame(float elapsedSeconds) = 0;

    /// Finalize UI construction. The resulting draw data is consumed by record().
    virtual void render() = 0;

    /// Append UI draws to raster and return their prerequisite upload work (empty for no draws).
    /// Submit uploads before the raster payload. On failure, discard the raster recording.
    /// Call before the next newFrame(); the recorded work retains its buffers and textures.
    virtual bool record(gpu2::GpuRaster & raster, AutoRef<gpu2::GpuPayload> & uploads) const = 0;

    /// Append UI draws to bindless raster and return their prerequisite upload work (empty for no draws).
    /// Uses recordBindBasedDraw with this backend's configured resourceSetIndex.
    /// Call before the next newFrame(); the recorded work retains its buffers and textures.
    virtual bool record(gpu2::bindless::Raster & raster, AutoRef<gpu2::GpuPayload> & uploads) const = 0;

    /// Register a sampled gpu2 texture and return an ImGui texture identifier.
    virtual ImTextureID registerTexture(AutoRef<gpu2::Texture> texture) = 0;

    /// Release a previously registered non-font texture identifier.
    virtual void unregisterTexture(ImTextureID texture) = 0;

    /// Whether Dear ImGui wants pointer input for the current frame.
    virtual bool wantsMouse() const = 0;

    /// Whether Dear ImGui wants keyboard input for the current frame.
    virtual bool wantsKeyboard() const = 0;

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2
