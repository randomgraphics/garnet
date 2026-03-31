#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>
#include "../submission.h"

namespace GN::rdg {

struct Payload : RefCounter {
    int    value         = 0;
    bool * destroyedFlag = nullptr;
    explicit Payload(int v, bool * flag = nullptr): value(v), destroyedFlag(flag) {}
    ~Payload() {
        if (destroyedFlag) *destroyedFlag = true;
    }
};

struct TaskInfoTestAction : public Action {
    GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    int capturedValue = -1;

    TaskInfoTestAction(const StrA & name): Action(TYPE_INFO(), name) {}

    static AutoRef<TaskInfoTestAction> create(const StrA & name) { return AutoRef<TaskInfoTestAction>(new TaskInfoTestAction(name)); }

    struct A : public Arguments {
        GN_RDG_REGISTER_RUNTIME_TYPE(Arguments);
        A(): Arguments(TYPE_INFO()) {}
        void addToReadWriteList(ArtifactReadWriteList &) const override {}
    };

    Action::PrepareResult prepare(TaskInfo & taskInfo, Arguments &) override {
        taskInfo.context = AutoRef<Payload>(new Payload(42));
        return {PASSED, 1};
    }

    ExecutionResult execute(TaskInfo & taskInfo, size_t step, Arguments &) override {
        if (0 != step) return FAILED;
        auto * p = taskInfo.getContext<Payload>();
        if (!p) return FAILED;
        capturedValue = p->value;
        return PASSED;
    }
};

struct GrabAction : public Action {
    GN_RDG_REGISTER_RUNTIME_TYPE(Action);

    AutoRef<Payload> payloadToAttach;

    GrabAction(const StrA & name): Action(TYPE_INFO(), name) {}

    static AutoRef<GrabAction> create(const StrA & name) { return AutoRef<GrabAction>(new GrabAction(name)); }

    struct A : public Arguments {
        GN_RDG_REGISTER_RUNTIME_TYPE(Arguments);
        A(): Arguments(TYPE_INFO()) {}
        void addToReadWriteList(ArtifactReadWriteList &) const override {}
    };

    Action::PrepareResult prepare(TaskInfo & taskInfo, Arguments &) override {
        taskInfo.context = payloadToAttach;
        return {PASSED, 1};
    }
    ExecutionResult execute(TaskInfo &, size_t, Arguments &) override { return PASSED; }
};

} // namespace GN::rdg

TEST_CASE("TaskInfo context: set in prepare, read in execute", "[rdg][taskinfo]") {
    auto rg = std::unique_ptr<GN::rdg::RenderGraph>(GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {}));
    REQUIRE(rg != nullptr);

    auto action = GN::rdg::TaskInfoTestAction::create("ctx-test");
    REQUIRE(action);
    auto args = GN::AutoRef<GN::rdg::TaskInfoTestAction::A>::make();

    auto wf = rg->createWorkflow("ctx-wf");
    wf.appendTask("ctx-task", action, args);

    auto sub = rg->submit({.workflows = GN::SafeArrayAccessor<GN::rdg::Workflow>(&wf, 1)});
    REQUIRE(sub);
    auto result = sub->result();
    CHECK(result.executionResult == GN::rdg::Action::PASSED);
    CHECK(action->capturedValue == 42);
}

TEST_CASE("TaskInfo context: released after submission ends", "[rdg][taskinfo]") {
    auto rg = std::unique_ptr<GN::rdg::RenderGraph>(GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {}));
    REQUIRE(rg != nullptr);

    bool destroyed          = false;
    auto action             = GN::rdg::GrabAction::create("grab");
    action->payloadToAttach = GN::AutoRef<GN::rdg::Payload>(new GN::rdg::Payload(99, &destroyed));
    CHECK(!destroyed);

    auto args = GN::AutoRef<GN::rdg::GrabAction::A>::make();
    auto wf   = rg->createWorkflow("grab-wf");
    wf.appendTask("grab-task", action, args);

    auto sub = rg->submit({.workflows = GN::SafeArrayAccessor<GN::rdg::Workflow>(&wf, 1)});
    REQUIRE(sub);
    auto result = sub->result();
    CHECK(result.executionResult == GN::rdg::Action::PASSED);

    // The action still holds a ref; drop it so the only ref was in TaskInfo (now gone).
    action->payloadToAttach.clear();
    CHECK(destroyed);
}
