#include "../window-input.h"

#include <catch2/catch_test_macros.hpp>

namespace {
class TestWindow : public GN::win::WindowInput {
public:
    using WindowInput::notifyAxisMove;
    using WindowInput::notifyCharPress;
    using WindowInput::notifyKeyPress;
    using WindowInput::updateMousePosition;

    intptr_t              getDisplayHandle() const override { return 0; }
    intptr_t              getMonitorHandle() const override { return 0; }
    intptr_t              getWindowHandle() const override { return 0; }
    intptr_t              getModuleHandle() const override { return 0; }
    GN::Vector2<uint32_t> getClientSize() const override { return {640, 480}; }
    intptr_t              createVulkanSurfaceHandle(intptr_t) const override { return 0; }
    void                  destroyVulkanSurfaceHandle(intptr_t, intptr_t) const override {}
    void                  show() override {}
    void                  hide() override {}
    void                  minimize() override {}
    void                  moveTo(int, int) override {}
    void                  setClientSize(size_t, size_t) override {}
    bool                  runUntilNoNewEvents(bool) override { return true; }
};
} // namespace

TEST_CASE("Window input tracks key state, modifiers, and transitions", "[win][input]") {
    using namespace GN::win;
    TestWindow window;

    KeyEvent signaled;
    auto     onKey      = [&](KeyEvent event) { signaled = event; };
    auto     connection = window.keyPressSignal().connect(onKey);

    window.notifyKeyPress(KeyCode::LCTRL, true);
    window.notifyKeyPress(KeyCode::A, true);

    REQUIRE(window.getKeyStatus(KeyCode::LCTRL).down);
    REQUIRE(window.getKeyStatus(KeyCode::A).down);
    REQUIRE(signaled.code() == KeyCode::A);
    REQUIRE(signaled.status.ctrlDown());

    window.notifyKeyPress(KeyCode::A, true);
    window.notifyKeyPress(KeyCode::A, false);
    REQUIRE_FALSE(window.getKeyStatus(KeyCode::A).down);

    REQUIRE(window.popLastKeyEvent().code() == KeyCode::LCTRL);
    REQUIRE(window.popLastKeyEvent().code() == KeyCode::A);
    REQUIRE(window.popLastKeyEvent().code() == KeyCode::A);
    REQUIRE_FALSE(window.popLastKeyEvent());
}

TEST_CASE("Window input bounds its transition queue", "[win][input]") {
    using namespace GN::win;
    TestWindow window;

    for (int i = 1; i <= 33; ++i) window.notifyKeyPress((KeyCode) i, true);

    REQUIRE(window.popLastKeyEvent().code() == (KeyCode) 2);
    for (int i = 0; i < 31; ++i) REQUIRE(window.popLastKeyEvent());
    REQUIRE_FALSE(window.popLastKeyEvent());
}

TEST_CASE("Window input tracks pointer, wheel, and text events", "[win][input]") {
    using namespace GN::win;
    TestWindow window;

    wchar_t character = 0;
    Axis    axis      = Axis::NONE;
    int     delta     = 0;
    auto    onChar    = [&](wchar_t value) { character = value; };
    auto    onAxis    = [&](Axis value, int amount) {
        axis  = value;
        delta = amount;
    };
    auto charConnection = window.charPressSignal().connect(onChar);
    auto axisConnection = window.axisMoveSignal().connect(onAxis);

    window.updateMousePosition(10, 20, false);
    window.updateMousePosition(13, 25);
    int x = 0, y = 0;
    window.getMousePosition(x, y);
    REQUIRE(x == 13);
    REQUIRE(y == 25);
    REQUIRE(window.getAxisStatus()[(size_t) Axis::MOUSE_X] == 13);

    window.notifyAxisMove(Axis::MOUSE_WHEEL_0, 120);
    REQUIRE(axis == Axis::MOUSE_WHEEL_0);
    REQUIRE(delta == 120);
    REQUIRE(window.getAxisStatus()[(size_t) Axis::MOUSE_WHEEL_0] == 120);

    window.notifyCharPress(L'\u4F60');
    REQUIRE(character == L'\u4F60');
}

TEST_CASE("Window key names round-trip", "[win][input]") {
    using namespace GN::win;
    REQUIRE(str2kc("A") == KeyCode::A);
    REQUIRE(str2kc("KeyCode::NONE") == KeyCode::NONE);
    REQUIRE(str2kc("not-a-key") == KeyCode::NONE);
    REQUIRE(std::string(keyCode2String((int) KeyCode::MOUSEBTN_0)) == "MOUSEBTN_0");
}
