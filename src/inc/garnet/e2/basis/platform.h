#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/basis/platform.h> directly. Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2::basis {

/// Host window, input and OS-event abstraction, independent of World simulation.
/// Create through Platform::create(), pump processEvents(), and pass its surface/window to presentation.
struct Platform : Being {
    GN_E2_DEFINE_A_BEING(Being);

    /// Parameters for creating a Platform and its main window.
    struct CreateParameters {
        Universe & universe; ///< Supplies the identity for the created Platform.
        StrA       caption = "Garnet engine2";
        uint32_t   width   = 1280; ///< initial client width in pixels. 0 = pick a platform default.
        uint32_t   height  = 720;  ///< initial client height in pixels. 0 = pick a platform default.
    };

    /// Create the host platform (main window + event pump). Returns null on failure.
    GN_API static Ref<Platform> create(const CreateParameters &);

    /// Creates a native render surface for the given graphics-API instance handle (e.g. a
    /// VkSurfaceKHR built from a VkInstance, both passed/returned as intptr_t). Returns 0 if
    /// unsupported. The Platform keeps no reference to it: ownership passes to the caller, who
    /// must pass it back to destroyRenderSurface() before the instance dies.
    virtual intptr_t createRenderSurface(intptr_t graphicsInstanceHandle) const = 0;

    /// Destroys a render surface previously returned by createRenderSurface() for the same
    /// instance. Must be called after any swapchain using the surface is destroyed and while
    /// the instance is still alive. No-op if either handle is 0.
    virtual void destroyRenderSurface(intptr_t graphicsInstanceHandle, intptr_t surfaceHandle) const = 0;

    /// Current client-area size of the main window, in pixels.
    virtual Vector2<uint32_t> clientSize() const = 0;

    /// Borrow the Platform-owned main window for input observation. The pointer remains valid
    /// until this Platform is destroyed and must not be deleted by the caller.
    virtual win::Window * window() const = 0;

    /// Pump the OS event queue once. Returns false when the user has requested the
    /// application to quit (e.g. the main window was closed).
    virtual bool processEvents() = 0;
};

} // namespace GN::e2::basis
