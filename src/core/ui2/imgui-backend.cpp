#include <garnet/GNui2.h>

#include "imgui-frag.spv.h"
#include "imgui-vert.spv.h"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <unordered_map>

namespace GN::ui2 {
namespace {

struct UiVertex {
    glm::vec2 position;
    glm::vec2 texcoord;
    glm::vec4 color;
};

ImGuiKey toImGuiKey(win::KeyCode key) {
    using K = win::KeyCode;
    switch (key) {
    case K::TAB:
        return ImGuiKey_Tab;
    case K::LEFT:
        return ImGuiKey_LeftArrow;
    case K::RIGHT:
        return ImGuiKey_RightArrow;
    case K::UP:
        return ImGuiKey_UpArrow;
    case K::DOWN:
        return ImGuiKey_DownArrow;
    case K::PAGEUP:
        return ImGuiKey_PageUp;
    case K::PAGEDOWN:
        return ImGuiKey_PageDown;
    case K::HOME:
        return ImGuiKey_Home;
    case K::END:
        return ImGuiKey_End;
    case K::INSERT:
        return ImGuiKey_Insert;
    case K::_DELETE:
        return ImGuiKey_Delete;
    case K::BACKSPACE:
        return ImGuiKey_Backspace;
    case K::SPACEBAR:
        return ImGuiKey_Space;
    case K::RETURN:
        return ImGuiKey_Enter;
    case K::ESCAPE:
        return ImGuiKey_Escape;
    case K::APOSTROPHE:
        return ImGuiKey_Apostrophe;
    case K::COMMA:
        return ImGuiKey_Comma;
    case K::MINUS:
        return ImGuiKey_Minus;
    case K::PERIOD:
        return ImGuiKey_Period;
    case K::SLASH:
        return ImGuiKey_Slash;
    case K::SEMICOLON:
        return ImGuiKey_Semicolon;
    case K::EQUALS:
        return ImGuiKey_Equal;
    case K::LBRACKET:
        return ImGuiKey_LeftBracket;
    case K::BACKSLASH:
        return ImGuiKey_Backslash;
    case K::RBRACKET:
        return ImGuiKey_RightBracket;
    case K::GRAVE:
        return ImGuiKey_GraveAccent;
    case K::CAPSLOCK:
        return ImGuiKey_CapsLock;
    case K::SCROLL:
        return ImGuiKey_ScrollLock;
    case K::PAUSE:
        return ImGuiKey_Pause;
    case K::NUMPAD_0:
        return ImGuiKey_Keypad0;
    case K::NUMPAD_1:
        return ImGuiKey_Keypad1;
    case K::NUMPAD_2:
        return ImGuiKey_Keypad2;
    case K::NUMPAD_3:
        return ImGuiKey_Keypad3;
    case K::NUMPAD_4:
        return ImGuiKey_Keypad4;
    case K::NUMPAD_5:
        return ImGuiKey_Keypad5;
    case K::NUMPAD_6:
        return ImGuiKey_Keypad6;
    case K::NUMPAD_7:
        return ImGuiKey_Keypad7;
    case K::NUMPAD_8:
        return ImGuiKey_Keypad8;
    case K::NUMPAD_9:
        return ImGuiKey_Keypad9;
    case K::NUMPAD_DECIMAL:
        return ImGuiKey_KeypadDecimal;
    case K::NUMPAD_DIVIDE:
        return ImGuiKey_KeypadDivide;
    case K::NUMPAD_MULTIPLY:
        return ImGuiKey_KeypadMultiply;
    case K::NUMPAD_SUBTRACT:
        return ImGuiKey_KeypadSubtract;
    case K::NUMPAD_ADD:
        return ImGuiKey_KeypadAdd;
    case K::NUMPAD_ENTER:
        return ImGuiKey_KeypadEnter;
    case K::LCTRL:
        return ImGuiKey_LeftCtrl;
    case K::LSHIFT:
        return ImGuiKey_LeftShift;
    case K::LALT:
        return ImGuiKey_LeftAlt;
    case K::RCTRL:
        return ImGuiKey_RightCtrl;
    case K::RSHIFT:
        return ImGuiKey_RightShift;
    case K::RALT:
        return ImGuiKey_RightAlt;
    default:
        break;
    }
    if (key >= K::A && key <= K::Z) return static_cast<ImGuiKey>(ImGuiKey_A + static_cast<int>(key) - static_cast<int>(K::A));
    if (key >= K::_0 && key <= K::_9) return static_cast<ImGuiKey>(ImGuiKey_0 + static_cast<int>(key) - static_cast<int>(K::_0));
    if (key >= K::F1 && key <= K::F12) return static_cast<ImGuiKey>(ImGuiKey_F1 + static_cast<int>(key) - static_cast<int>(K::F1));
    return ImGuiKey_None;
}

struct ImGuiBackendImpl final : ImGuiBackend {
    GN_REGISTER_RUNTIME_TYPE(ImGuiBackend);

    ImGuiBackendImpl(AutoRef<gpu2::GpuContext> gpu, win::Window & window)
        : ImGuiBackend(TYPE_INFO(), "imgui-gpu2-backend"), mGpu(std::move(gpu)), mWindow(window) {}

    ~ImGuiBackendImpl() override {
        mKeyTether.clear();
        mCharTether.clear();
        mAxisTether.clear();
        if (mContext) ImGui::DestroyContext(mContext);
    }

    bool init() {
        mContext = ImGui::CreateContext();
        if (!mContext) return false;
        selectContext();
        ImGuiIO & io           = ImGui::GetIO();
        io.IniFilename         = nullptr;
        io.BackendPlatformName = "garnet_win";
        io.BackendRendererName = "garnet_gpu2";
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        mKeyTether  = mWindow.keyPressSignal().connect<&ImGuiBackendImpl::onKey>(this);
        mCharTether = mWindow.charPressSignal().connect<&ImGuiBackendImpl::onCharacter>(this);
        mAxisTether = mWindow.axisMoveSignal().connect<&ImGuiBackendImpl::onAxis>(this);

        unsigned char * pixels = nullptr;
        int             width = 0, height = 0;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        if (!pixels || width <= 0 || height <= 0) return false;
        mFontTexture = gpu2::Texture::create("imgui-font", {
                                                               .context    = mGpu,
                                                               .descriptor = gpu2::Texture::Descriptor {}
                                                                                 .setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM())
                                                                                 .setDimensions(static_cast<uint32_t>(width), static_cast<uint32_t>(height))
                                                                                 .setFaces(1)
                                                                                 .setLevels(1),
                                                           });
        if (!mFontTexture) return false;
        gfx::img::Extent3D extent;
        extent.set(static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1);
        const auto      plane = gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), extent);
        gfx::img::Image fontImage(gfx::img::ImageDesc::make(plane, 1, 1, 1), pixels, static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
        // Font creation is one-shot initialization; the blocking helper avoids retaining atlas
        // staging resources across the first application frame.
        if (!mFontTexture->setContent(fontImage)) return false;
        mTextures.emplace(1, mFontTexture);
        io.Fonts->SetTexID(1);

        mVs = gpu2::GpuShader::create({.context = mGpu, .name = "imgui.vert", .binary = kImguiVertSpv, .size = sizeof(kImguiVertSpv)});
        mPs = gpu2::GpuShader::create({.context = mGpu, .name = "imgui.frag", .binary = kImguiFragSpv, .size = sizeof(kImguiFragSpv)});
        return mVs && mPs;
    }

    void selectContext() const { ImGui::SetCurrentContext(mContext); }

    void newFrame(float elapsedSeconds) override {
        selectContext();
        ImGuiIO &  io              = ImGui::GetIO();
        const auto size            = mWindow.getClientSize();
        io.DisplaySize             = ImVec2(static_cast<float>(size.x), static_cast<float>(size.y));
        io.DisplayFramebufferScale = ImVec2(1, 1);
        io.DeltaTime               = std::max(elapsedSeconds, 1.0f / 1000.0f);
        int x = 0, y = 0;
        mWindow.getMousePosition(x, y);
        io.AddMousePosEvent(static_cast<float>(x), static_cast<float>(y));
        ImGui::NewFrame();
    }

    void render() override {
        selectContext();
        ImGui::Render();
        mDrawData = ImGui::GetDrawData();
    }

    ImTextureID registerTexture(AutoRef<gpu2::Texture> texture) override {
        if (!texture) return ImTextureID_Invalid;
        const ImTextureID id = ++mNextTextureId;
        mTextures.emplace(id, std::move(texture));
        return id;
    }

    void unregisterTexture(ImTextureID texture) override {
        if (texture != 1) mTextures.erase(texture);
    }

    bool wantsMouse() const override {
        selectContext();
        return ImGui::GetIO().WantCaptureMouse;
    }

    bool wantsKeyboard() const override {
        selectContext();
        return ImGui::GetIO().WantCaptureKeyboard;
    }

    AutoRef<gpu2::GpuPayload> record(gpu2::GpuRaster & raster, bool & ok) override {
        ok = true;
        if (!mDrawData || !mDrawData->Valid || mDrawData->TotalVtxCount <= 0 || mDrawData->TotalIdxCount <= 0) return {};

        const uint64_t vertexBytes  = static_cast<uint64_t>(mDrawData->TotalVtxCount) * sizeof(UiVertex);
        const uint64_t indexBytes   = static_cast<uint64_t>(mDrawData->TotalIdxCount) * sizeof(ImDrawIdx);
        auto           vertexBuffer = gpu2::Buffer::create("imgui-frame-vb", {.context = mGpu, .size = vertexBytes});
        auto           indexBuffer  = gpu2::Buffer::create("imgui-frame-ib", {.context = mGpu, .size = indexBytes});
        auto           cnc          = gpu2::GpuCnC::create({.gpu = mGpu});
        if (!vertexBuffer || !indexBuffer || !cnc) {
            ok = false;
            return {};
        }

        DynaArray<uint8_t> vertices, indices;
        vertices.resize(vertexBytes);
        indices.resize(indexBytes);
        size_t vertexOffset = 0, indexOffset = 0;
        for (const ImDrawList * list : mDrawData->CmdLists) {
            const size_t listVertexBytes = static_cast<size_t>(list->VtxBuffer.Size) * sizeof(UiVertex);
            const size_t listIndexBytes  = static_cast<size_t>(list->IdxBuffer.Size) * sizeof(ImDrawIdx);
            auto *       destination     = reinterpret_cast<UiVertex *>(vertices.data() + vertexOffset);
            for (int i = 0; i < list->VtxBuffer.Size; ++i) {
                const ImDrawVert & source = list->VtxBuffer[i];
                destination[i]            = {
                    .position = {source.pos.x, source.pos.y},
                    .texcoord = {source.uv.x, source.uv.y},
                    .color =
                        {
                            static_cast<float>((source.col >> IM_COL32_R_SHIFT) & 0xFF) / 255.0f,
                            static_cast<float>((source.col >> IM_COL32_G_SHIFT) & 0xFF) / 255.0f,
                            static_cast<float>((source.col >> IM_COL32_B_SHIFT) & 0xFF) / 255.0f,
                            static_cast<float>((source.col >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f,
                        },
                };
            }
            std::memcpy(indices.data() + indexOffset, list->IdxBuffer.Data, listIndexBytes);
            vertexOffset += listVertexBytes;
            indexOffset += listIndexBytes;
        }
        cnc->uploadBuffer(vertexBuffer, 0, {vertices.data(), vertices.size()});
        cnc->uploadBuffer(indexBuffer, 0, {indices.data(), indices.size()});

        struct PushConstants {
            glm::vec2 scale;
            glm::vec2 translate;
        };
        const PushConstants constants {
            {2.0f / mDrawData->DisplaySize.x, 2.0f / mDrawData->DisplaySize.y},
            {-1.0f - mDrawData->DisplayPos.x * (2.0f / mDrawData->DisplaySize.x), -1.0f - mDrawData->DisplayPos.y * (2.0f / mDrawData->DisplaySize.y)},
        };

        uint64_t globalVertexOffset = 0, globalIndexOffset = 0;
        for (const ImDrawList * list : mDrawData->CmdLists) {
            for (const ImDrawCmd & command : list->CmdBuffer) {
                if (command.UserCallback) {
                    if (command.UserCallback != ImDrawCallback_ResetRenderState) command.UserCallback(list, &command);
                    continue;
                }
                const float   clipX = (command.ClipRect.x - mDrawData->DisplayPos.x) * mDrawData->FramebufferScale.x;
                const float   clipY = (command.ClipRect.y - mDrawData->DisplayPos.y) * mDrawData->FramebufferScale.y;
                const float   clipZ = (command.ClipRect.z - mDrawData->DisplayPos.x) * mDrawData->FramebufferScale.x;
                const float   clipW = (command.ClipRect.w - mDrawData->DisplayPos.y) * mDrawData->FramebufferScale.y;
                const int32_t x     = static_cast<int32_t>(std::max(clipX, 0.0f));
                const int32_t y     = static_cast<int32_t>(std::max(clipY, 0.0f));
                if (clipZ <= x || clipW <= y) continue;

                const auto texture = mTextures.find(command.GetTexID());
                if (texture == mTextures.end()) continue;

                gpu2::GpuRaster::DrawParameters draw;
                draw.vs = mVs;
                draw.ps = mPs;
                draw.geometry.format.attributes.append(
                    {.location = 0, .binding = 0, .offset = offsetof(UiVertex, position), .format = gpu2::RasterGeometry::AttributeFormat::F32_2});
                draw.geometry.format.attributes.append(
                    {.location = 1, .binding = 0, .offset = offsetof(UiVertex, texcoord), .format = gpu2::RasterGeometry::AttributeFormat::F32_2});
                draw.geometry.format.attributes.append(
                    {.location = 2, .binding = 0, .offset = offsetof(UiVertex, color), .format = gpu2::RasterGeometry::AttributeFormat::F32_4});
                draw.geometry.vertices.append({.buffer = vertexBuffer,
                                               .offset = globalVertexOffset + static_cast<uint64_t>(command.VtxOffset) * sizeof(UiVertex),
                                               .stride = sizeof(UiVertex)});
                draw.geometry.indices    = {.buffer = indexBuffer,
                                            .offset = globalIndexOffset + static_cast<uint64_t>(command.IdxOffset) * sizeof(ImDrawIdx),
                                            .stride = sizeof(ImDrawIdx)};
                draw.geometry.indexCount = command.ElemCount;
                draw.states.cullMode     = gpu2::RasterState::CULL_NONE;
                draw.states.depthState   = gpu2::RasterState::DepthState {};
                draw.states.scissorRect =
                    gpu2::RasterState::ScissorRect {.x = x, .y = y, .width = static_cast<uint32_t>(clipZ - x), .height = static_cast<uint32_t>(clipW - y)};
                draw.resources.resize(1);
                draw.resources[0].resize(1);
                draw.resources[0][0].resize(1);
                draw.resources[0][0][0].resource = texture->second;
                draw.immediates                  = referenceTo(new SimpleBlob<uint8_t>(sizeof(constants), reinterpret_cast<const uint8_t *>(&constants)));
                raster.draw(draw);
            }
            globalVertexOffset += static_cast<uint64_t>(list->VtxBuffer.Size) * sizeof(UiVertex);
            globalIndexOffset += static_cast<uint64_t>(list->IdxBuffer.Size) * sizeof(ImDrawIdx);
        }
        return cnc->seal();
    }

    void onKey(win::KeyEvent event) {
        selectContext();
        ImGuiIO & io          = ImGui::GetIO();
        const int mouseButton = static_cast<int>(event.key) - static_cast<int>(win::KeyCode::FIRST_MOUSE_BUTTON);
        if (mouseButton >= 0 && mouseButton <= 4) {
            io.AddMouseButtonEvent(mouseButton, event.status.down);
            return;
        }
        const ImGuiKey key = toImGuiKey(event.key);
        if (key != ImGuiKey_None) io.AddKeyEvent(key, event.status.down);
        io.AddKeyEvent(ImGuiMod_Ctrl, event.status.ctrlDown());
        io.AddKeyEvent(ImGuiMod_Shift, event.status.shiftDown());
        io.AddKeyEvent(ImGuiMod_Alt, event.status.altDown());
    }

    void onCharacter(wchar_t character) {
        selectContext();
        ImGui::GetIO().AddInputCharacter(static_cast<unsigned int>(character));
    }

    void onAxis(win::Axis axis, int delta) {
        if (axis != win::Axis::MOUSE_WHEEL_0 && axis != win::Axis::MOUSE_WHEEL_1) return;
        selectContext();
        ImGui::GetIO().AddMouseWheelEvent(axis == win::Axis::MOUSE_WHEEL_1 ? static_cast<float>(delta) / 120.0f : 0.0f,
                                          axis == win::Axis::MOUSE_WHEEL_0 ? static_cast<float>(delta) / 120.0f : 0.0f);
    }

    AutoRef<gpu2::GpuContext>                               mGpu;
    win::Window &                                           mWindow;
    ImGuiContext *                                          mContext  = nullptr;
    ImDrawData *                                            mDrawData = nullptr;
    AutoRef<gpu2::GpuShader>                                mVs, mPs;
    AutoRef<gpu2::Texture>                                  mFontTexture;
    ImTextureID                                             mNextTextureId = 1;
    std::unordered_map<ImTextureID, AutoRef<gpu2::Texture>> mTextures;
    Tether                                                  mKeyTether;
    Tether                                                  mCharTether;
    Tether                                                  mAxisTether;
};

} // namespace

AutoRef<ImGuiBackend> ImGuiBackend::create(const CreateParameters & parameters) {
    if (!parameters.gpu) return {};
    AutoRef<ImGuiBackendImpl> backend(new ImGuiBackendImpl(parameters.gpu, parameters.window));
    if (!backend->init()) return {};
    return backend;
}

} // namespace GN::ui2
