
#include <catch2/catch_test_macros.hpp>
#include <garnet/GNengine2.h>

using namespace GN;
using namespace GN::e2;

struct MockWorld : World {
    GN_REGISTER_RUNTIME_TYPE(World);

    MockWorld(Universe & u): World(TYPE_INFO(), u.generateUniqueIdentifier(), "hello world", 1.0) {}

    void populate(ArrayView<Ref<Form>>) override {}

    void run() override {}

    void stop() override {}

    auto captureVisualMoment(const VisualMoment::CaptureParameters &) -> AutoRef<VisualMoment> override { return {}; }
};

struct MockForm : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);

    MockForm(Universe & u): Form(TYPE_INFO(), u.generateUniqueIdentifier(), "the first life form") {}
};

TEST_CASE("E2: smoke test") {
    Universe u;
    auto     w = referenceTo(new MockWorld(u));
    auto     f = referenceTo(new MockForm(u));
    w->populate({&f.cast<Form>(), 1});
    w->run();
}
