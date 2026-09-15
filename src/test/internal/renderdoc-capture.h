#pragma once

namespace GN::test {

/// Acquires RenderDoc's in-application API when it is already injected.
///
/// This must be called before creating a graphics device. It is intentionally
/// a no-op when the test is not launched under RenderDoc.
void initializeRenderDoc();

/// Starts a manual RenderDoc capture, or does nothing when the API is absent.
void startRenderDocCapture();

/// Ends a manual RenderDoc capture, or does nothing when the API is absent.
void endRenderDocCapture();

/// Starts a capture for a scope and always ends it during stack unwinding.
class RenderDocCapture {
public:
    RenderDocCapture() { startRenderDocCapture(); }
    ~RenderDocCapture() { endRenderDocCapture(); }

    RenderDocCapture(RenderDocCapture const &)             = delete;
    RenderDocCapture & operator=(RenderDocCapture const &) = delete;
};

} // namespace GN::test
