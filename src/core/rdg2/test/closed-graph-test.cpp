// API-shape exercise for the closed render graph design (closed-graph.h).
//
// The closed graph has no compiler or executor implementation yet. This test
// drives one mock PBR frame through the public interface only — declaration,
// mock compile, mock execution, payload gathering — to validate that the API
// is sound. Execution correctness is out of scope; the mocks implement just
// enough of the documented semantics (declaration-order scheduling,
// missing-producer validation, staged publication, undeclared-use rejection)
// to prove the interfaces can carry a real frame.

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg2.h>

using namespace GN;
using namespace GN::rdg2;

namespace {

// ============================================================================
// Mock relic content. Real clients would store gpu2 resource views, resource
// sets, or draw manifests; the API test only needs distinct entity types.
// ============================================================================

struct MockSceneConstants final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    int frameIndex = 0;

    explicit MockSceneConstants(int frameIndex_): Entity(TYPE_INFO(), "mock-scene-constants"), frameIndex(frameIndex_) {}
};

struct MockMeshAsset final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    StrA source;

    explicit MockMeshAsset(const StrA & source_): Entity(TYPE_INFO(), "mock-mesh-asset"), source(source_) {}
};

struct MockRenderTarget final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    uint32_t        width = 0, height = 0;
    int             drawCount  = 0;
    int             frameIndex = -1;
    DynaArray<StrA> history; ///< names of the passes whose output flowed into this target

    MockRenderTarget(uint32_t w, uint32_t h): Entity(TYPE_INFO(), "mock-render-target"), width(w), height(h) {}
};

/// gpu2::GpuPayload has no pure virtuals, so a name-only mock stands in for sealed GPU work.
struct MockGpuPayload final : public gpu2::GpuPayload {
    GN_REGISTER_RUNTIME_TYPE(gpu2::GpuPayload);

    explicit MockGpuPayload(const StrA & name_): gpu2::GpuPayload(TYPE_INFO(), name_) {}
};

// ============================================================================
// Mock execution plumbing. Plan, Execution, and QuestContext are pure
// interfaces; these mocks stand in for the future compiler/executor.
// ============================================================================

class MockQuestContext final : public QuestContext {
public:
    struct StagedPublish {
        ArtifactRef     artifact;
        AutoRef<Entity> content;
    };

    /// Declaration of the quest currently executing; set by the mock executor.
    const QuestDeclaration * declaration = nullptr;

    DynaArray<StagedPublish>             staged;         ///< publications staged by the current quest
    DynaArray<AutoRef<gpu2::GpuPayload>> payloads;       ///< gathered across the whole execution
    mutable int                          violations = 0; ///< undeclared reads/writes observed

    Artifact::Relic<> read(const ArtifactRef & artifact) const override {
        if (!declared(artifact, false)) {
            ++violations;
            return {};
        }
        // Mock resolution: latest committed relic. A real execution pins the
        // concrete version per the declared selector at a synchronization point.
        return artifact->content();
    }

    void publish(const ArtifactRef & artifact, AutoRef<Entity> content) override {
        if (!declared(artifact, true)) {
            ++violations;
            return;
        }
        staged.append({artifact, std::move(content)});
    }

    void emit(AutoRef<gpu2::GpuPayload> payload) override { payloads.append(std::move(payload)); }

    void await(AutoRef<gpu2::GpuPayload> dependency) override { awaited.append(std::move(dependency)); }

    void present(AutoRef<gpu2::Swapchain> swapchain) override { presentTarget = std::move(swapchain); }

    DynaArray<AutoRef<gpu2::GpuPayload>> awaited;       ///< external submit dependencies registered by quests
    AutoRef<gpu2::Swapchain>             presentTarget; ///< present requested by a quest, if any

    AutoRef<gpu2::GpuContext> gpu() const override { return {}; } // no GPU device in this API-shape test

private:
    bool declared(const ArtifactRef & artifact, bool write) const {
        if (!declaration) return false;
        for (size_t i = 0; i < declaration->artifactUses.size(); ++i) {
            const auto & use = declaration->artifactUses[i];
            if (use.artifact.get() != artifact.get()) continue;
            const bool canWrite = use.access != ArtifactAccess::READ_ONLY;
            const bool canRead  = use.access != ArtifactAccess::DISCARD_WRITE;
            if (write ? canWrite : canRead) return true;
        }
        return false;
    }
};

/// Mock compiled plan: declaration-order scheduling plus missing-producer validation.
class MockPlan final : public Plan {
public:
    GN_REGISTER_RUNTIME_TYPE(Plan);

    struct CompiledQuest {
        QuestRef         quest;
        QuestDeclaration declaration;
    };

    DynaArray<CompiledQuest> compiled;

    size_t questCount() const override { return compiled.size(); }

    QuestRef quest(size_t index) const override { return compiled[index].quest; }

    /// Mock compile: keep declaration order; reject any read whose artifact has
    /// neither an earlier in-plan writer nor an imported (already published)
    /// relic. Compile-time version queries are execution-system territory, which
    /// is exactly what this mock is.
    static AutoRef<MockPlan> compile(const DynaArray<QuestRef> & quests) {
        AutoRef<MockPlan> plan(new MockPlan("mock-plan"));
        for (size_t i = 0; i < quests.size(); ++i) {
            auto declaration = quests[i]->declare();
            for (size_t u = 0; u < declaration.artifactUses.size(); ++u) {
                const auto & use = declaration.artifactUses[u];
                if (use.access == ArtifactAccess::DISCARD_WRITE) continue; // pure write: no producer needed
                const bool imported = use.artifact->version() != Artifact::Version::OOO();
                if (!imported && !writtenEarlier(*plan, use.artifact)) return {};
            }
            plan->compiled.append({quests[i], std::move(declaration)});
        }
        return plan;
    }

private:
    explicit MockPlan(const StrA & name_): Plan(TYPE_INFO(), name_) {}

    static bool writtenEarlier(const MockPlan & plan, const ArtifactRef & artifact) {
        for (size_t i = 0; i < plan.compiled.size(); ++i) {
            const auto & declaration = plan.compiled[i].declaration;
            for (size_t u = 0; u < declaration.artifactUses.size(); ++u) {
                const auto & use = declaration.artifactUses[u];
                if (use.artifact.get() == artifact.get() && use.access != ArtifactAccess::READ_ONLY) return true;
            }
        }
        return false;
    }
};

/// Mock execution: runs plan quests sequentially on the calling thread.
/// Staged publications commit when the owning quest succeeds (mock completion
/// policy), so later quests in the same execution read the fresh relics.
class MockExecution final : public Execution {
public:
    GN_REGISTER_RUNTIME_TYPE(Execution);

    MockQuestContext context;

    explicit MockExecution(AutoRef<MockPlan> plan_): Execution(TYPE_INFO(), "mock-execution"), mPlan(std::move(plan_)) {}

    Status status() const override { return mStatus; }

    void cancel() override {
        if (Status::READY == mStatus || Status::RUNNING == mStatus) mStatus = Status::CANCELED;
    }

    Status wait(std::chrono::milliseconds = std::chrono::milliseconds::max()) const override { return mStatus; } // mock runs synchronously

    void run() {
        mStatus = Status::RUNNING;
        for (size_t i = 0; i < mPlan->compiled.size(); ++i) {
            auto & cq           = mPlan->compiled[i];
            context.declaration = &cq.declaration;
            context.staged.clear();
            auto result         = cq.quest->execute(context);
            context.declaration = nullptr;
            if (QuestResult::Status::SUCCEEDED != result.status || context.violations > 0) {
                mStatus = Status::FAILED;
                return;
            }
            for (size_t p = 0; p < context.staged.size(); ++p) { context.staged[p].artifact->publish(std::move(context.staged[p].content)); }
        }
        mStatus = Status::SUCCEEDED;
    }

private:
    AutoRef<MockPlan> mPlan;
    Status            mStatus = Status::READY;
};

// ============================================================================
// Mock quests for one PBR frame: constants upload, then a PBR raster pass.
// ============================================================================

struct MockSccQuest final : public Quest {
    GN_REGISTER_RUNTIME_TYPE(Quest);

    ArtifactRef sceneConstants;
    int         frameIndex = 0;

    explicit MockSccQuest(ArtifactRef sceneConstants_): Quest(TYPE_INFO(), "mock-scc-quest"), sceneConstants(std::move(sceneConstants_)) {}

    QuestDeclaration declare() const override {
        QuestDeclaration d;
        d.name = "mock-scc-quest";
        d.artifactUses.append(ArtifactUse {.name = "sceneConstants", .artifact = sceneConstants, .access = ArtifactAccess::DISCARD_WRITE});
        return d;
    }

    QuestResult execute(QuestContext & ctx) override {
        ctx.publish(sceneConstants, AutoRef<Entity>(new MockSceneConstants(frameIndex)));
        ctx.emit(AutoRef<gpu2::GpuPayload>(new MockGpuPayload("scc-upload")));
        return QuestResult::succeeded();
    }
};

struct MockPbrQuest final : public Quest {
    GN_REGISTER_RUNTIME_TYPE(Quest);

    ArtifactRef sceneConstants, mesh, color, depth;

    MockPbrQuest(ArtifactRef sceneConstants_, ArtifactRef mesh_, ArtifactRef color_, ArtifactRef depth_)
        : Quest(TYPE_INFO(), "mock-pbr-quest"), sceneConstants(std::move(sceneConstants_)), mesh(std::move(mesh_)), color(std::move(color_)),
          depth(std::move(depth_)) {}

    QuestDeclaration declare() const override {
        QuestDeclaration d;
        d.name = "mock-pbr-quest";
        d.artifactUses.append(ArtifactUse {.name = "sceneConstants", .artifact = sceneConstants}); // READ_ONLY, LATEST_AT_COMPILE defaults
        d.artifactUses.append(ArtifactUse {.name = "mesh", .artifact = mesh});
        d.artifactUses.append(ArtifactUse {.name = "color", .artifact = color, .access = ArtifactAccess::DISCARD_WRITE, .root = true});
        d.artifactUses.append(ArtifactUse {.name = "depth", .artifact = depth, .access = ArtifactAccess::DISCARD_WRITE});
        return d;
    }

    QuestResult execute(QuestContext & ctx) override {
        auto constants = ctx.read<MockSceneConstants>(sceneConstants);
        if (!constants) return QuestResult::failed("scene constants unavailable");
        auto meshRelic = ctx.read<MockMeshAsset>(mesh);
        if (!meshRelic) return QuestResult::failed("mesh asset unavailable");

        // DISCARD_WRITE: publish complete replacements; prior contents are irrelevant.
        auto colorTarget        = AutoRef<MockRenderTarget>(new MockRenderTarget(1920, 1080));
        auto depthTarget        = AutoRef<MockRenderTarget>(new MockRenderTarget(1920, 1080));
        colorTarget->drawCount  = 1; // "drew" the mesh
        colorTarget->frameIndex = constants->frameIndex;
        depthTarget->drawCount  = 1;

        ctx.publish(color, colorTarget);
        ctx.publish(depth, depthTarget);
        ctx.emit(AutoRef<gpu2::GpuPayload>(new MockGpuPayload("pbr-raster")));
        return QuestResult::succeeded();
    }
};

/// Build a pass-style quest through the public Quest::create() factory — the
/// generic configured-quest authoring tier; MockSccQuest/MockPbrQuest above
/// are the bespoke-subclass tier. The execute body reads every declared
/// input, publishes one render target per declared write carrying the
/// provenance history of the primary input plus this pass's name, and emits
/// one payload named after the pass.
QuestRef makePassQuest(const StrA & name, const DynaArray<ArtifactUse> & uses, ArtifactRef primaryInput, uint32_t width, uint32_t height) {
    Quest::CreateParameters p;
    p.name         = name;
    p.artifactUses = uses;
    p.execute      = [name, uses, primaryInput, width, height](QuestContext & ctx) -> QuestResult {
        // Read every declared input; provenance continues from the primary input.
        DynaArray<StrA> history;
        for (size_t i = 0; i < uses.size(); ++i) {
            const auto & use = uses[i];
            if (ArtifactAccess::DISCARD_WRITE == use.access) continue;
            auto relic = ctx.read<MockRenderTarget>(use.artifact);
            if (!relic) return QuestResult::failed(use.name);
            if (use.artifact.get() == primaryInput.get()) history = relic->history;
        }
        history.append(name);

        // Publish one complete output per declared write.
        for (size_t i = 0; i < uses.size(); ++i) {
            const auto & use = uses[i];
            if (ArtifactAccess::READ_ONLY == use.access) continue;
            auto target     = AutoRef<MockRenderTarget>(new MockRenderTarget(width, height));
            target->history = history;
            ctx.publish(use.artifact, target);
        }

        ctx.emit(AutoRef<gpu2::GpuPayload>(new MockGpuPayload(name)));
        return QuestResult::succeeded();
    };
    return Quest::create(p);
}

/// Client-side relic content wrapping the SCC set0 resource set.
struct SccResourcesEntity final : public Entity {
    GN_REGISTER_RUNTIME_TYPE(Entity);

    gpu2::GpuResourceSet value;

    explicit SccResourcesEntity(gpu2::GpuResourceSet value_): Entity(TYPE_INFO(), "scc-set0-resources"), value(std::move(value_)) {}
};

/// SCC validation-client adapter: the SccQuest of the design spec authored as
/// a configured quest instead of a public Quest subclass. SCC itself stays
/// concrete and graph-agnostic; only this thin adapter knows about quests.
QuestRef makeSccQuest(AutoRef<SharedShaderConstants> ssc, ArtifactRef sceneConstants) {
    Quest::CreateParameters p;
    p.name = "scc";
    p.artifactUses.append(ArtifactUse {.name = "sceneConstants", .artifact = sceneConstants, .access = ArtifactAccess::DISCARD_WRITE});
    p.execute = [ssc, sceneConstants](QuestContext & ctx) -> QuestResult {
        auto snapshot = ssc->takeSnapshot();
        ctx.publish(sceneConstants, AutoRef<Entity>(new SccResourcesEntity(std::move(snapshot.set0Resources))));
        for (size_t i = 0; i < snapshot.set0Payloads.size(); ++i) ctx.emit(snapshot.set0Payloads[i]);
        return QuestResult::succeeded();
    };
    return Quest::create(p);
}

/// Declares one write but publishes a different, undeclared artifact.
struct RogueQuest final : public Quest {
    GN_REGISTER_RUNTIME_TYPE(Quest);

    ArtifactRef declaredOut, undeclaredOut;

    RogueQuest(ArtifactRef declaredOut_, ArtifactRef undeclaredOut_)
        : Quest(TYPE_INFO(), "rogue-quest"), declaredOut(std::move(declaredOut_)), undeclaredOut(std::move(undeclaredOut_)) {}

    QuestDeclaration declare() const override {
        QuestDeclaration d;
        d.name = "rogue-quest";
        d.artifactUses.append(ArtifactUse {.name = "out", .artifact = declaredOut, .access = ArtifactAccess::DISCARD_WRITE});
        return d;
    }

    QuestResult execute(QuestContext & ctx) override {
        ctx.publish(undeclaredOut, AutoRef<Entity>(new MockSceneConstants(0)));
        return QuestResult::succeeded();
    }
};

} // namespace

TEST_CASE("rdg2::ClosedGraph: pbr frame through quest/plan/execution", "[rdg2][closed-graph]") {
    // Frame data as artifacts. The mesh is imported: published before compile.
    auto sceneConstants = Artifact::create("sceneConstants");
    auto mesh           = Artifact::create("meshAsset");
    auto color          = Artifact::create("mainColor");
    auto depth          = Artifact::create("mainDepth");
    REQUIRE(sceneConstants);
    REQUIRE(mesh);
    REQUIRE(color);
    REQUIRE(depth);
    CHECK(mesh->publish(AutoRef<Entity>(new MockMeshAsset("cube.gltf"))) == Artifact::Version::ONE());

    auto scc        = AutoRef<MockSccQuest>(new MockSccQuest(sceneConstants));
    scc->frameIndex = 42;
    auto pbr        = AutoRef<MockPbrQuest>(new MockPbrQuest(sceneConstants, mesh, color, depth));

    // Declaration shape: pure description, defaults as designed.
    auto pbrDecl = pbr->declare();
    CHECK(pbrDecl.name == "mock-pbr-quest");
    REQUIRE(pbrDecl.artifactUses.size() == 4);
    CHECK(pbrDecl.artifactUses[0].access == ArtifactAccess::READ_ONLY);
    CHECK(pbrDecl.artifactUses[0].selector == RelicSelector::LATEST_AT_COMPILE);
    CHECK(!pbrDecl.artifactUses[0].root);
    CHECK(pbrDecl.artifactUses[2].root);
    CHECK(pbrDecl.explicitDependencies.empty());

    // Compile: deterministic declaration order, reads validated.
    DynaArray<QuestRef> quests;
    quests.append(QuestRef(scc));
    quests.append(QuestRef(pbr));
    auto plan = MockPlan::compile(quests);
    REQUIRE(plan);
    CHECK(plan->questCount() == 2);
    CHECK(plan->quest(0).get() == scc.get());
    CHECK(plan->quest(1).get() == pbr.get());

    // Execute and gather.
    auto execution = AutoRef<MockExecution>(new MockExecution(plan));
    CHECK(execution->status() == Execution::Status::READY);
    execution->run();
    CHECK(execution->wait() == Execution::Status::SUCCEEDED);
    CHECK(execution->context.violations == 0);

    // Payload gather order is deterministic: scc upload before pbr raster.
    REQUIRE(execution->context.payloads.size() == 2);
    CHECK(execution->context.payloads[0]->name == "scc-upload");
    CHECK(execution->context.payloads[1]->name == "pbr-raster");

    // Published relics: pbr consumed the in-plan scc relic and the imported mesh.
    auto constantsRelic = sceneConstants->content<MockSceneConstants>();
    REQUIRE(constantsRelic);
    CHECK(constantsRelic->frameIndex == 42);
    auto colorRelic = color->content<MockRenderTarget>();
    REQUIRE(colorRelic);
    CHECK(colorRelic->drawCount == 1);
    CHECK(colorRelic->frameIndex == 42);
    CHECK(colorRelic.version == Artifact::Version::ONE());
    CHECK(mesh->version() == Artifact::Version::ONE()); // imported artifact was only read
}

TEST_CASE("rdg2::ClosedGraph: compile rejects reads without producer or import", "[rdg2][closed-graph]") {
    auto sceneConstants = Artifact::create("sceneConstants");
    auto mesh           = Artifact::create("meshAsset"); // never published, no in-plan producer
    auto color          = Artifact::create("mainColor");
    auto depth          = Artifact::create("mainDepth");

    auto pbr = AutoRef<MockPbrQuest>(new MockPbrQuest(sceneConstants, mesh, color, depth));

    DynaArray<QuestRef> quests;
    quests.append(QuestRef(pbr));
    CHECK(!MockPlan::compile(quests)); // missing producers for sceneConstants and mesh
}

TEST_CASE("rdg2::ClosedGraph: undeclared publish fails the execution", "[rdg2][closed-graph]") {
    auto declared   = Artifact::create("declared");
    auto undeclared = Artifact::create("undeclared");

    auto rogue = AutoRef<RogueQuest>(new RogueQuest(declared, undeclared));

    DynaArray<QuestRef> quests;
    quests.append(QuestRef(rogue));
    auto plan = MockPlan::compile(quests);
    REQUIRE(plan);

    auto execution = AutoRef<MockExecution>(new MockExecution(plan));
    execution->run();
    CHECK(execution->wait() == Execution::Status::FAILED);
    CHECK(execution->context.violations > 0);
    CHECK(undeclared->version() == Artifact::Version::OOO()); // rejected publish never became visible
}

TEST_CASE("rdg2::ClosedGraph: multi-pass frame: shadow, hdr scene, post, super-res, ui", "[rdg2][closed-graph]") {
    // Frame graph under test:
    //   shadow-map   : DISCARD_WRITE shadowMap (2048x2048, execution-local)
    //   main-scene   : READ shadowMap -> DISCARD_WRITE hdrColor + sceneDepth (1920x1080)
    //   post-process : READ hdrColor + sceneDepth (tone map, bloom, depth blur) -> DISCARD_WRITE ldrColor
    //   super-res    : READ ldrColor -> DISCARD_WRITE backbuffer (upscale to 3840x2160)
    //   ui           : READ_WRITE backbuffer (composite UI over the upscaled image), root output
    auto shadowMap  = Artifact::create("shadowMap");
    auto hdrColor   = Artifact::create("hdrColor");
    auto sceneDepth = Artifact::create("sceneDepth");
    auto ldrColor   = Artifact::create("ldrColor");
    auto backbuffer = Artifact::create("backbuffer");

    // The backbuffer is imported: the acquired swapchain image is pre-published.
    {
        auto acquired = AutoRef<MockRenderTarget>(new MockRenderTarget(3840, 2160));
        acquired->history.append("swapchain-acquire");
        CHECK(backbuffer->publish(acquired) == Artifact::Version::ONE());
    }

    // All five passes are authored through the public Quest::create() factory —
    // no bespoke Quest subclass involved (contrast with MockPbrQuest above).
    DynaArray<ArtifactUse> shadowUses;
    shadowUses.append(ArtifactUse {.name = "shadowMap", .artifact = shadowMap, .access = ArtifactAccess::DISCARD_WRITE});
    auto shadow = makePassQuest("shadow-map", shadowUses, {}, 2048, 2048);

    DynaArray<ArtifactUse> sceneUses;
    sceneUses.append(ArtifactUse {.name = "shadowMap", .artifact = shadowMap});
    sceneUses.append(ArtifactUse {.name = "hdrColor", .artifact = hdrColor, .access = ArtifactAccess::DISCARD_WRITE});
    sceneUses.append(ArtifactUse {.name = "sceneDepth", .artifact = sceneDepth, .access = ArtifactAccess::DISCARD_WRITE});
    auto scene = makePassQuest("main-scene", sceneUses, shadowMap, 1920, 1080);

    DynaArray<ArtifactUse> postUses; // tone map + bloom + depth blur
    postUses.append(ArtifactUse {.name = "hdrColor", .artifact = hdrColor});
    postUses.append(ArtifactUse {.name = "sceneDepth", .artifact = sceneDepth}); // depth blur input
    postUses.append(ArtifactUse {.name = "ldrColor", .artifact = ldrColor, .access = ArtifactAccess::DISCARD_WRITE});
    auto post = makePassQuest("post-process", postUses, hdrColor, 1920, 1080);

    DynaArray<ArtifactUse> superResUses;
    superResUses.append(ArtifactUse {.name = "ldrColor", .artifact = ldrColor});
    superResUses.append(ArtifactUse {.name = "backbuffer", .artifact = backbuffer, .access = ArtifactAccess::DISCARD_WRITE});
    auto superRes = makePassQuest("super-res", superResUses, ldrColor, 3840, 2160);

    DynaArray<ArtifactUse> uiUses;
    uiUses.append(ArtifactUse {.name = "backbuffer", .artifact = backbuffer, .access = ArtifactAccess::READ_WRITE, .root = true});
    auto ui = makePassQuest("ui", uiUses, backbuffer, 3840, 2160);

    REQUIRE(shadow);
    REQUIRE(ui);
    CHECK(shadow->name == "shadow-map"); // the factory preserves quest identity
    CHECK(!Quest::create({}));           // factories return empty refs on failure: execute is required

    // READ_WRITE declaration: the UI pass reads previous backbuffer contents
    // and publishes an updated relic; the use is also the frame's output root.
    auto uiDecl = ui->declare();
    REQUIRE(uiDecl.artifactUses.size() == 1);
    CHECK(uiDecl.artifactUses[0].access == ArtifactAccess::READ_WRITE);
    CHECK(uiDecl.artifactUses[0].root);

    DynaArray<QuestRef> quests;
    quests.append(QuestRef(shadow));
    quests.append(QuestRef(scene));
    quests.append(QuestRef(post));
    quests.append(QuestRef(superRes));
    quests.append(QuestRef(ui));
    auto plan = MockPlan::compile(quests);
    REQUIRE(plan);
    CHECK(plan->questCount() == 5);

    auto execution = AutoRef<MockExecution>(new MockExecution(plan));
    execution->run();
    CHECK(execution->wait() == Execution::Status::SUCCEEDED);
    CHECK(execution->context.violations == 0);

    // Deterministic gather order across all five passes.
    REQUIRE(execution->context.payloads.size() == 5);
    CHECK(execution->context.payloads[0]->name == "shadow-map");
    CHECK(execution->context.payloads[1]->name == "main-scene");
    CHECK(execution->context.payloads[2]->name == "post-process");
    CHECK(execution->context.payloads[3]->name == "super-res");
    CHECK(execution->context.payloads[4]->name == "ui");

    // Final backbuffer: acquire (v1) -> super-res discard (v2) -> ui composite (v3).
    auto expectedVersion = Artifact::Version::ONE();
    ++expectedVersion;
    ++expectedVersion;
    auto finalFrame = backbuffer->content<MockRenderTarget>();
    REQUIRE(finalFrame);
    CHECK(finalFrame.version == expectedVersion);
    CHECK(finalFrame->width == 3840);
    CHECK(finalFrame->height == 2160);

    // Provenance flowed through every pass in order. The swapchain-acquire tag
    // is absent because super-res DISCARD_WRITE promised not to depend on the
    // backbuffer's prior contents.
    REQUIRE(finalFrame->history.size() == 5);
    CHECK(finalFrame->history[0] == "shadow-map");
    CHECK(finalFrame->history[1] == "main-scene");
    CHECK(finalFrame->history[2] == "post-process");
    CHECK(finalFrame->history[3] == "super-res");
    CHECK(finalFrame->history[4] == "ui");

    // Intermediates were published exactly once and none is a root — a real
    // planner is free to recycle or alias their physical storage.
    CHECK(shadowMap->version() == Artifact::Version::ONE());
    CHECK(hdrColor->version() == Artifact::Version::ONE());
    CHECK(sceneDepth->version() == Artifact::Version::ONE());
    CHECK(ldrColor->version() == Artifact::Version::ONE());
}

TEST_CASE("rdg2::ClosedGraph: shared shader constants as a configured quest", "[rdg2][closed-graph][gpu]") {
    auto gpu =
        gpu2::GpuContext::create("closed_graph_scc_gpu", gpu2::GpuContext::CreateParameters {.howToPrintDeviceCaps = gpu2::GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);
    ssc->set0.frameConstants.frameCounter = 7;

    auto sceneConstants = Artifact::create("sceneConstants");
    auto scc            = makeSccQuest(ssc, sceneConstants);
    REQUIRE(scc);

    DynaArray<QuestRef> quests;
    quests.append(scc);
    auto plan = MockPlan::compile(quests);
    REQUIRE(plan);

    auto execution = AutoRef<MockExecution>(new MockExecution(plan));
    execution->run();
    CHECK(execution->wait() == Execution::Status::SUCCEEDED);
    CHECK(execution->context.violations == 0);

    // The published relic carries the real set0 resource set (6 bindings), and
    // the snapshot's upload payloads were gathered for later submission.
    auto relic = sceneConstants->content<SccResourcesEntity>();
    REQUIRE(relic);
    CHECK(relic->value.size() == 6u);
    CHECK(execution->context.payloads.size() >= 2u); // fallback + env uploads on the first snapshot
}

TEST_CASE("rdg2::ClosedGraph: real compile and run, cpu only", "[rdg2][closed-graph]") {
    auto input  = Artifact::create("input");
    auto output = Artifact::create("output");
    CHECK(input->publish(AutoRef<Entity>(new MockSceneConstants(5))) == Artifact::Version::ONE()); // imported

    Quest::CreateParameters producer;
    producer.name = "producer";
    producer.artifactUses.append(ArtifactUse {.name = "input", .artifact = input});
    producer.artifactUses.append(ArtifactUse {.name = "output", .artifact = output, .access = ArtifactAccess::DISCARD_WRITE, .root = true});
    producer.execute = [input, output](QuestContext & ctx) -> QuestResult {
        auto in = ctx.read<MockSceneConstants>(input);
        if (!in) return QuestResult::failed("missing input");
        ctx.publish(output, AutoRef<Entity>(new MockSceneConstants(in->frameIndex + 1)));
        return QuestResult::succeeded();
    };
    auto quest = Quest::create(producer);
    REQUIRE(quest);

    // Real compile: happy path plus missing-producer rejection.
    Plan::CompileParameters compileParams;
    compileParams.quests.append(quest);
    auto plan = Plan::compile(compileParams);
    REQUIRE(plan);
    CHECK(plan->questCount() == 1);
    CHECK(plan->quest(0).get() == quest.get());
    {
        auto                    orphanInput = Artifact::create("orphanInput"); // never published, no producer
        Quest::CreateParameters orphan;
        orphan.name = "orphan";
        orphan.artifactUses.append(ArtifactUse {.name = "orphanInput", .artifact = orphanInput});
        orphan.execute = [](QuestContext &) { return QuestResult::succeeded(); };
        Plan::CompileParameters bad;
        bad.quests.append(Quest::create(orphan));
        CHECK(!Plan::compile(bad));
    }

    // Real execution, no GPU context: the plan emits no payloads.
    auto execution = Execution::run({.plan = plan});
    REQUIRE(execution);
    CHECK(execution->status() == Execution::Status::SUCCEEDED);
    CHECK(execution->wait() == Execution::Status::SUCCEEDED);

    auto relic = output->content<MockSceneConstants>();
    REQUIRE(relic);
    CHECK(relic->frameIndex == 6); // data flowed from the imported relic through the quest
    CHECK(relic.version == Artifact::Version::ONE());

    // Undeclared publication fails the real execution and never becomes visible.
    {
        auto                    declared   = Artifact::create("declared");
        auto                    undeclared = Artifact::create("undeclared");
        Quest::CreateParameters rogue;
        rogue.name = "rogue";
        rogue.artifactUses.append(ArtifactUse {.name = "out", .artifact = declared, .access = ArtifactAccess::DISCARD_WRITE});
        rogue.execute = [undeclared](QuestContext & ctx) {
            ctx.publish(undeclared, AutoRef<Entity>(new MockSceneConstants(0)));
            return QuestResult::succeeded();
        };
        Plan::CompileParameters cp;
        cp.quests.append(Quest::create(rogue));
        auto roguePlan = Plan::compile(cp);
        REQUIRE(roguePlan);
        auto rogueRun = Execution::run({.plan = roguePlan});
        REQUIRE(rogueRun);
        CHECK(rogueRun->status() == Execution::Status::FAILED);
        CHECK(undeclared->version() == Artifact::Version::OOO());
    }
}
