#include <catch2/catch_test_macros.hpp>
#include <garnet/GNengine2.h>
#include <garnet/GNfx2.h>

#include <concepts>
#include <utility>

using namespace GN;
using namespace GN::e2;

static_assert(std::derived_from<ModelVisualFacet, VisualFacet>);
static_assert(std::same_as<decltype(ModelVisualFacet::create(std::declval<const ModelVisualFacet::CreateParameters &>())), Ref<ModelVisualFacet>>);
static_assert(std::same_as<decltype(createModelForm(std::declval<Universe &>(), std::declval<const StrA &>(),
                                                   std::declval<AutoRef<const fx2::ModelScene>>())),
                           Ref<Form>>);

TEST_CASE("e2 model interface remains renderer independent", "[e2][model]") {
    CHECK(ModelVisualFacet::TYPE_INFO().isDerivedFrom(VisualFacet::TYPE_INFO()));
}
