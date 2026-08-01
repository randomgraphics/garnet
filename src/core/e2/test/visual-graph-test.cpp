// CPU-only coverage of E2's closed frame-plan composition policy. The quests use mock
// artifact work, so this verifies the fixed acquire -> render -> present skeleton without a
// window, swapchain, or GPU.

#include <catch2/catch_test_macros.hpp>

#include "e2/e2-internal.h"

using namespace GN;
using namespace GN::e2;
using namespace GN::rdg2;

namespace {

QuestRef makeQuest(const StrA & name, const ArtifactUse & use) {
    Quest::CreateParameters parameters;
    parameters.name = name;
    parameters.artifactUses.append(use);
    parameters.execute = [](QuestContext &) { return QuestResult::succeeded(); };
    return Quest::create(parameters);
}

} // namespace

TEST_CASE("E2 visual: frame plan fixes acquire, render, and present order", "[e2][visual][rdg2]") {
    auto backbuffer = Artifact::create("test.backbuffer");
    REQUIRE(backbuffer);

    auto frameBegin = makeQuest("frame-begin", {.name = "backbuffer", .artifact = backbuffer, .access = ArtifactAccess::DISCARD_WRITE});
    auto render     = makeQuest("e2-visual-render", {.name = "backbuffer", .artifact = backbuffer, .access = ArtifactAccess::READ_WRITE});
    auto frameEnd   = makeQuest("frame-end", {.name = "backbuffer", .artifact = backbuffer, .access = ArtifactAccess::READ_ONLY, .root = true});

    auto plan = compileVisualFramePlan(frameBegin, render, frameEnd);
    REQUIRE(plan);
    REQUIRE(plan->questCount() == 3);
    CHECK(plan->quest(0).get() == frameBegin.get());
    CHECK(plan->quest(1).get() == render.get());
    CHECK(plan->quest(2).get() == frameEnd.get());
}

TEST_CASE("E2 visual: frame plan rejects a render read without acquisition", "[e2][visual][rdg2]") {
    auto backbuffer = Artifact::create("test.missing-acquire");
    REQUIRE(backbuffer);

    auto unrelatedBegin = makeQuest("frame-begin", {.name = "unused", .artifact = Artifact::create("test.unused"), .access = ArtifactAccess::DISCARD_WRITE});
    auto render         = makeQuest("e2-visual-render", {.name = "backbuffer", .artifact = backbuffer, .access = ArtifactAccess::READ_WRITE});
    auto frameEnd       = makeQuest("frame-end", {.name = "backbuffer", .artifact = backbuffer, .access = ArtifactAccess::READ_ONLY, .root = true});

    CHECK(!compileVisualFramePlan(unrelatedBegin, render, frameEnd));
}
