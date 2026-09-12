# UI2 Dear ImGui backend

UI2 is Garnet's reusable Dear ImGui integration for the gpu2/window stack.
Client code includes `garnet/GNui2.h`. The backend has no dependency on the
legacy GPU or effect pipeline.

`ImGuiBackend` owns an ImGui context, translates `GN::win::Window` mouse,
wheel, keyboard, modifier, and text events, and exposes ImGui mouse/keyboard
capture decisions to its host application. The font atlas is uploaded once
during initialization. Each frame converts ImGui draw data into gpu2 vertex
and index buffers, applies command clip rectangles as scissors, and resolves
registered texture identifiers before recording indexed draws.

The backend implements E2's `VisualOverlay` interface. `VisualDomain` appends
the overlay draws to its raster pass after world geometry, preserving the
closed RDG2 frame ordering and resource lifetime. Standalone gpu2 use remains
possible through the backend's frame and record API.

Important ownership rules:

- the application owns the native window and pumps its events;
- the backend owns its ImGui context and font texture;
- registered application textures remain owned by the caller;
- draw payloads retain streamed buffers until GPU completion;
- UI rendering is alpha blended and must target the active color/depth views.

The backend disables `imgui.ini` persistence by default, so tools do not write
configuration files into their working directory.
