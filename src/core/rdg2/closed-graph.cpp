#include "pch.h"

namespace GN::rdg2 {

namespace {

GN::Logger * sLogger = GN::getLogger("GN.rdg2.closed-graph");

/// Generic pass-style quest fully described by its creation parameters.
class ConfiguredQuest final : public Quest {
public:
    GN_REGISTER_RUNTIME_TYPE(Quest);

    explicit ConfiguredQuest(const Quest::CreateParameters & parameters): Quest(TYPE_INFO(), parameters.name), mParameters(parameters) {}

    QuestDeclaration declare() const override {
        QuestDeclaration d;
        d.name                 = mParameters.name;
        d.artifactUses         = mParameters.artifactUses;
        d.explicitDependencies = mParameters.explicitDependencies;
        return d;
    }

    QuestResult execute(QuestContext & context) override { return mParameters.execute(context); }

private:
    Quest::CreateParameters mParameters;
};

// =====================================================================================================================
// Plan
// =====================================================================================================================

/// First-slice compiled plan: declaration order is the execution order.
class PlanImpl final : public Plan {
public:
    GN_REGISTER_RUNTIME_TYPE(Plan);

    struct CompiledQuest {
        QuestRef         quest;
        QuestDeclaration declaration;
    };

    ArrayContainer<CompiledQuest> compiled;

    explicit PlanImpl(const StrA & name_): Plan(TYPE_INFO(), name_) {}

    size_t questCount() const override { return compiled.size(); }

    QuestRef quest(size_t index) const override { return index < compiled.size() ? compiled[index].quest : QuestRef {}; }

    bool writesEarlier(const ArtifactRef & artifact) const {
        for (size_t i = 0; i < compiled.size(); ++i) {
            const auto & declaration = compiled[i].declaration;
            for (size_t u = 0; u < declaration.artifactUses.size(); ++u) {
                const auto & use = declaration.artifactUses[u];
                if (use.artifact.get() == artifact.get() && use.access != ArtifactAccess::READ_ONLY) return true;
            }
        }
        return false;
    }

    bool contains(const QuestRef & quest) const {
        for (size_t i = 0; i < compiled.size(); ++i) {
            if (compiled[i].quest.get() == quest.get()) return true;
        }
        return false;
    }
};

// =====================================================================================================================
// Execution
// =====================================================================================================================

/// Execution-owned quest context: validates accesses against the running
/// quest's declaration, stages publications, and gathers GPU payloads plus
/// external dependencies for the single gathered submission.
class QuestContextImpl final : public QuestContext {
public:
    struct StagedPublish {
        ArtifactRef     artifact;
        AutoRef<Entity> content;
    };

    const QuestDeclaration *                  declaration = nullptr; ///< declaration of the currently running quest
    AutoRef<gpu2::GpuContext>                 gpuContext;
    ArrayContainer<StagedPublish>             staged;       ///< publications staged by the current quest
    ArrayContainer<AutoRef<gpu2::GpuPayload>> work;         ///< payloads gathered across the whole execution
    ArrayContainer<AutoRef<gpu2::GpuPayload>> dependencies; ///< external GPU dependencies of the gathered submission
    AutoRef<gpu2::Swapchain>                  presentTarget;
    mutable size_t                            violations = 0; ///< undeclared reads/writes observed

    Artifact::Relic<> read(const ArtifactRef & artifact) const override {
        if (!declared(artifact, false)) {
            GN_ERROR(sLogger)("Quest '{}': reads undeclared artifact '{}'.", questName(), artifact ? artifact->name : StrA::EMPTYSTR());
            ++violations;
            return {};
        }
        // First slice resolves reads to the latest committed relic. Symbolic
        // selector resolution at execution-chosen synchronization points comes
        // with the full compiler.
        return artifact->content();
    }

    void publish(const ArtifactRef & artifact, AutoRef<Entity> content) override {
        if (!declared(artifact, true)) {
            GN_ERROR(sLogger)("Quest '{}': publishes undeclared artifact '{}'.", questName(), artifact ? artifact->name : StrA::EMPTYSTR());
            ++violations;
            return;
        }
        staged.append({artifact, std::move(content)});
    }

    void emit(AutoRef<gpu2::GpuPayload> payload) override {
        if (payload) work.append(std::move(payload));
    }

    void await(AutoRef<gpu2::GpuPayload> dependency) override {
        if (dependency) dependencies.append(std::move(dependency));
    }

    void present(AutoRef<gpu2::Swapchain> swapchain) override { presentTarget = std::move(swapchain); }

    AutoRef<gpu2::GpuContext> gpu() const override { return gpuContext; }

private:
    StrA questName() const { return declaration ? declaration->name : StrA::EMPTYSTR(); }

    bool declared(const ArtifactRef & artifact, bool write) const {
        if (!declaration || !artifact) return false;
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

/// First-slice execution: synchronous, sequential, single gathered submission.
class ExecutionImpl final : public Execution {
public:
    GN_REGISTER_RUNTIME_TYPE(Execution);

    explicit ExecutionImpl(const StrA & name_): Execution(TYPE_INFO(), name_) {}

    Status status() const override { return mStatus; }

    void cancel() override {
        if (Status::READY == mStatus || Status::RUNNING == mStatus) mStatus = Status::CANCELED;
    }

    // The first slice runs synchronously, so the status is terminal by the
    // time callers can wait on it.
    Status wait(std::chrono::milliseconds = std::chrono::milliseconds::max()) const override { return mStatus; }

    void runPlan(const RunParameters & parameters) {
        mStatus = Status::RUNNING;
        if (!parameters.plan) GN_UNLIKELY {
                GN_ERROR(sLogger)("Execution '{}': a plan is required.", name);
                mStatus = Status::FAILED;
                return;
            }

        QuestContextImpl context;
        context.gpuContext = parameters.gpu;

        // Phase 3 first slice: sequential execution in compiled order. Staged
        // publications commit when the owning quest succeeds, so later quests
        // in the same execution read the fresh relics.
        const size_t count = parameters.plan->questCount();
        for (size_t i = 0; i < count; ++i) {
            auto quest = parameters.plan->quest(i);
            if (!quest) GN_UNLIKELY {
                    GN_ERROR(sLogger)("Execution '{}': plan quest {} is null.", name, i);
                    mStatus = Status::FAILED;
                    return;
                }

            // Declarations are pure descriptions; re-deriving them here keeps
            // the executor working with any Plan implementation.
            auto declaration    = quest->declare();
            context.declaration = &declaration;
            context.staged.clear();
            auto result         = quest->execute(context);
            context.declaration = nullptr;

            if (QuestResult::Status::SUCCEEDED != result.status || context.violations > 0) {
                GN_ERROR(sLogger)("Execution '{}': quest '{}' failed. {}", name, declaration.name, result.message);
                mStatus = Status::FAILED; // fail fast: skip GPU submission entirely
                return;
            }

            for (size_t p = 0; p < context.staged.size(); ++p) { context.staged[p].artifact->publish(std::move(context.staged[p].content)); }
        }

        // Phase 4 first slice: one gathered submission in compiled order, with
        // external dependencies (e.g. swapchain acquire) as submit waits.
        if (!context.work.empty()) {
            if (!parameters.gpu) GN_UNLIKELY {
                    GN_ERROR(sLogger)("Execution '{}': quests emitted GPU payloads but no GPU context was provided.", name);
                    mStatus = Status::FAILED;
                    return;
                }
            auto submit = gpu2::GpuContext::SubmitParameters(name);
            for (size_t i = 0; i < context.work.size(); ++i) submit.appendWork(context.work[i]);
            for (size_t i = 0; i < context.dependencies.size(); ++i) submit.waitFor(context.dependencies[i]);
            parameters.gpu->submit(submit);
        }

        // Present policy: after the gathered submission, waiting on the last
        // submitted payload; an empty frame falls back to the acquire payload.
        if (context.presentTarget) {
            AutoRef<gpu2::GpuPayload> waitFor;
            if (!context.work.empty())
                waitFor = context.work[context.work.size() - 1];
            else if (!context.dependencies.empty())
                waitFor = context.dependencies[context.dependencies.size() - 1];
            if (!waitFor) GN_UNLIKELY {
                    GN_ERROR(sLogger)("Execution '{}': present requested but there is no payload to order the present after.", name);
                    mStatus = Status::FAILED;
                    return;
                }
            context.presentTarget->present(*waitFor);
        }

        mStatus = Status::SUCCEEDED;
    }

private:
    Status mStatus = Status::READY;
};

} // namespace

// =====================================================================================================================
// Public factories
// =====================================================================================================================

GN_API QuestRef Quest::create(const CreateParameters & parameters) {
    if (!parameters.execute) GN_UNLIKELY {
            GN_ERROR(sLogger)("Quest '{}': an execute callable is required.", parameters.name);
            return {};
        }
    return QuestRef(new ConfiguredQuest(parameters));
}

GN_API PlanRef Plan::compile(const CompileParameters & parameters) {
    AutoRef<PlanImpl> plan(new PlanImpl("plan"));
    for (size_t i = 0; i < parameters.quests.size(); ++i) {
        const auto & quest = parameters.quests[i];
        if (!quest) GN_UNLIKELY {
                GN_ERROR(sLogger)("Plan::compile: quest {} is null.", i);
                return {};
            }
        auto declaration = quest->declare();

        for (size_t u = 0; u < declaration.artifactUses.size(); ++u) {
            const auto & use = declaration.artifactUses[u];
            if (!use.artifact) GN_UNLIKELY {
                    GN_ERROR(sLogger)("Plan::compile: quest '{}' use '{}' has no artifact.", declaration.name, use.name);
                    return {};
                }
            if (ArtifactAccess::DISCARD_WRITE == use.access) continue; // pure write: no producer needed
            // Compile-time version queries are execution-system territory, which is exactly what the compiler is.
            const bool imported = use.artifact->version() != Artifact::Version::OOO();
            if (!imported && !plan->writesEarlier(use.artifact)) GN_UNLIKELY {
                    GN_ERROR(sLogger)("Plan::compile: quest '{}' reads artifact '{}' which has no earlier writer and no imported relic.", declaration.name,
                                      use.artifact->name);
                    return {};
                }
        }

        for (size_t d = 0; d < declaration.explicitDependencies.size(); ++d) {
            if (!plan->contains(declaration.explicitDependencies[d])) GN_UNLIKELY {
                    GN_ERROR(sLogger)("Plan::compile: quest '{}' has an explicit dependency that is not an earlier quest of this plan.", declaration.name);
                    return {};
                }
        }

        plan->compiled.append({quest, std::move(declaration)});
    }
    return PlanRef(plan);
}

GN_API ExecutionRef Execution::run(const RunParameters & parameters) {
    AutoRef<ExecutionImpl> execution(new ExecutionImpl(parameters.name));
    execution->runPlan(parameters);
    return ExecutionRef(execution);
}

GN_API QuestRef createFrameBeginQuest(const FrameBeginParameters & parameters) {
    if (!parameters.swapchain || !parameters.backbuffer) GN_UNLIKELY {
            GN_ERROR(sLogger)("createFrameBeginQuest: swapchain and backbuffer are required.");
            return {};
        }
    Quest::CreateParameters q;
    q.name = "frame-begin";
    q.artifactUses.append(ArtifactUse {.name = "backbuffer", .artifact = parameters.backbuffer, .access = ArtifactAccess::DISCARD_WRITE});
    q.execute = [swapchain = parameters.swapchain, backbuffer = parameters.backbuffer](QuestContext & ctx) -> QuestResult {
        auto frame = swapchain->prepare();
        if (frame.view.empty()) return QuestResult::failed("swapchain prepare failed");
        ctx.await(frame.ready);
        ctx.publish(backbuffer, AutoRef<Entity>(new SwapchainFrameEntity(std::move(frame))));
        return QuestResult::succeeded();
    };
    return Quest::create(q);
}

GN_API QuestRef createFrameEndQuest(const FrameEndParameters & parameters) {
    if (!parameters.swapchain || !parameters.backbuffer) GN_UNLIKELY {
            GN_ERROR(sLogger)("createFrameEndQuest: swapchain and backbuffer are required.");
            return {};
        }
    Quest::CreateParameters q;
    q.name = "frame-end";
    q.artifactUses.append(ArtifactUse {.name = "backbuffer", .artifact = parameters.backbuffer, .access = ArtifactAccess::READ_ONLY, .root = true});
    q.execute = [swapchain = parameters.swapchain, backbuffer = parameters.backbuffer](QuestContext & ctx) -> QuestResult {
        auto relic = ctx.read(backbuffer);
        if (!relic) return QuestResult::failed("no backbuffer relic to present");
        ctx.present(swapchain);
        return QuestResult::succeeded();
    };
    return Quest::create(q);
}

} // namespace GN::rdg2
