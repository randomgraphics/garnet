
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

struct MockFacet : Facet {
    GN_REGISTER_RUNTIME_TYPE(Facet);

    MockFacet(Universe & u): Facet(TYPE_INFO(), u.generateUniqueIdentifier(), "the first facet") {}
};

TEST_CASE("E2: smoke test") {
    Universe u;
    auto     w  = referenceTo(new MockWorld(u));
    auto     f  = Form::create(u, "the first life form"); // Form is sealed; forms come from the factory
    auto     ft = referenceTo(new MockFacet(u));
    REQUIRE(f);
    CHECK(f->isKindOf<Form>());
    CHECK(f->addFacet(ft));
    CHECK(ft->isKindOf<Facet>());
    CHECK(ft->form() == f.get());
    w->populate({&f, 1});
    w->run();
    f->live();
}
