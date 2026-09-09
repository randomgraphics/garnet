#if !defined(__GN_INSIDE_UI2_H__)
    #error "Do not include <garnet/ui2/imgui-backend.h> directly. Include <garnet/GNui2.h> instead."
#endif

namespace GN::ui2 {

/// Dear ImGui platform and renderer backend using GN::win input and gpu2 draw payloads.
struct ImGuiBackend : e2::VisualOverlay {
    GN_API GN_REGISTER_RUNTIME_TYPE(e2::VisualOverlay);

    struct CreateParameters {
        AutoRef<gpu2::GpuContext> gpu;
        win::Window &              window;
    };

    /// Create a backend and a dedicated Dear ImGui context. Only one backend may be current
    /// on a thread while client code calls ImGui functions.
    GN_API static AutoRef<ImGuiBackend> create(const CreateParameters &);

    /// Select this backend's context and begin a UI frame.
    virtual void newFrame(float elapsedSeconds) = 0;

    /// Finalize UI construction. The resulting draw data is consumed by record().
    virtual void render() = 0;

    /// Register a sampled gpu2 texture and return an ImGui texture identifier.
    virtual ImTextureID registerTexture(AutoRef<gpu2::Texture> texture) = 0;

    /// Release a previously registered non-font texture identifier.
    virtual void unregisterTexture(ImTextureID texture) = 0;

    /// Whether Dear ImGui wants pointer input for the current frame.
    virtual bool wantsMouse() const = 0;

    /// Whether Dear ImGui wants keyboard input for the current frame.
    virtual bool wantsKeyboard() const = 0;

protected:
    using e2::VisualOverlay::VisualOverlay;
};

} // namespace GN::ui2
