
#include <catch2/catch_test_macros.hpp>
#include <garnet/GNengine2.h>

using namespace GN;
using namespace GN::e2;

struct MockUniverse : Universe {};

struct MockWorld : World {
    GN_REGISTER_RUNTIME_TYPE(World);

    MockWorld(Universe & u): World(TYPE_INFO(), u.generateUniqueIdentifier(), "hello world") {}

    void populate(ArrayView<Ref<Form>>) override {}

    void run() override {}

    auto captureVisualMoment(const VisualMoment::CaptureParameters &) -> AutoRef<VisualMoment> override { return {}; }
};

struct MockForm : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);

    MockForm(Universe & u): Form(TYPE_INFO(), u.generateUniqueIdentifier(), "the first life form") {}

    /// update this form's internal state. called by the world, usually with an fixed interval.
    bool update() override { return true; }

    /// update the visual part of the form.
    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) override { return {}; }
};

TEST_CASE("E2: smoke test") {
    MockUniverse u;
    auto         w = referenceTo(new MockWorld(u));
    auto         f = referenceTo(new MockForm(u));
    w->populate({&f.cast<Form>(), 1});
    w->run();
}
