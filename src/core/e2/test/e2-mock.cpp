
#include <catch2/catch_test_macros.hpp>
#include <garnet/GNengine2.h>

using namespace GN;
using namespace GN::e2;

struct MockUniverse : Universe {

};

struct MockWorld : World {
    GN_REGISTER_RUNTIME_TYPE(World);

    MockWorld(Universe & u) : World(TYPE_INFO(), u.generateUniqueIdentifier(), "hello world") {}

    void populate(ArrayView<AutoRef<Form>>) override {}

    void run() override {}

    auto captureVisualMoment() -> AutoRef<VisualMoment> override { return {}; }
};

TEST_CASE("E2: smoke test") {
    MockUniverse u;
    MockWorld w(u);
    w.run();
}
