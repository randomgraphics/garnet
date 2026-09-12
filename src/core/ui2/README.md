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

The backend implements E2's `VisualOverlay`, a specialized `VisualMoment`. After
`backend->render()` finalizes ImGui data, add it with `tableau->add(backend)` to
the opaque tableau returned by `World::snapshot()`. `VisualDomain::render(tableau)`
then invokes each moment's virtual `record()` method. The tableau exposes no
hierarchy or traversal API, and the domain has no persistent overlay setter.

Overlays render after all regular moments and environments, regardless of their
insertion position. Larger `zOrder()` values render first (far to near); equal-Z
overlays have unspecified relative order. UI2 defaults to zero; call `setZOrder()`
before rendering to place it relative to other overlays.

UI2 records through the borrowed `VisualMoment::RenderContext`. Its public interface
supplies the active raster, prerequisite upload scheduling, and prepared FX2 shared
shader constants through `ssc()`. ImGui's unlit shaders do not use those constants.
E2's resource caches and RDG2 adapters remain private. Standalone gpu2 hosts can
implement this context interface. Do not begin the next ImGui frame or change
registered textures until rendering the current tableau returns.

Important ownership rules:

- the application owns the native window and pumps its events;
- the backend owns its ImGui context and font texture;
- registered application textures remain owned by the caller;
- draw payloads retain streamed buffers until GPU completion;
- UI rendering is alpha blended and must target the active color/depth views.

The backend disables `imgui.ini` persistence by default, so tools do not write
configuration files into their working directory.
