/*
 * Render Graph Workflow Test (migrated from src/test/ut/rdg/rdg-test.cpp)
 *
 * Tests custom artifact/action types, workflow scheduling, and the arithmetic
 * example: 3 * (1 + 2) = 9
 */

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>

// ============================================================================
// CUSTOM ARTIFACTS AND ACTIONS
// ============================================================================

namespace GN::rdg {

struct IntegerArtifact : public Artifact {
    GN_REGISTER_RUNTIME_TYPE(Artifact);

    int value = 0;

    IntegerArtifact(const StrA & name): Artifact(TYPE_INFO(), name) {}

    static GN::AutoRef<IntegerArtifact> create(const StrA & name) { return GN::AutoRef<IntegerArtifact>(new IntegerArtifact(name)); }
};

struct InitIntegerAction : public Action {
    GN_REGISTER_RUNTIME_TYPE(Action);

    int initValue = 0;

    InitIntegerAction(const StrA & name): Action(TYPE_INFO(), name) {}

    static GN::AutoRef<InitIntegerAction> create(const StrA & name) { return GN::AutoRef<InitIntegerAction>(new InitIntegerAction(name)); }

    struct A : public Arguments {
        GN_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}
        AutoRef<IntegerArtifact> output;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (output) list.writeList.insert(output.get());
        }
    };

    Action::PrepareResult prepare(TaskInfo &, Arguments &) override { return {PASSED, 1}; }

    ExecutionResult execute(TaskInfo &, size_t, Arguments & args) override {
        auto * a = RuntimeType::cast<A>(args);
        CHECK(a != nullptr);
        if (!a) return FAILED;
        auto * integerArtifact = a->output.get();
        CHECK(integerArtifact != nullptr);
        if (!integerArtifact) return FAILED;
        integerArtifact->value = initValue;
        return PASSED;
    }
};

struct AddIntegersAction : public Action {
    GN_REGISTER_RUNTIME_TYPE(Action);

    AddIntegersAction(const StrA & name): Action(TYPE_INFO(), name) {}

    static GN::AutoRef<AddIntegersAction> create(const StrA & name) { return GN::AutoRef<AddIntegersAction>(new AddIntegersAction(name)); }

    struct A : public Arguments {
        GN_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}
        AutoRef<IntegerArtifact> input1;
        AutoRef<IntegerArtifact> input2;
        AutoRef<IntegerArtifact> output;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (input1) list.readList.insert(input1.get());
            if (input2) list.readList.insert(input2.get());
            if (output) list.writeList.insert(output.get());
        }
    };

    Action::PrepareResult prepare(TaskInfo &, Arguments &) override { return {PASSED, 1}; }

    ExecutionResult execute(TaskInfo &, size_t, Arguments & args) override {
        auto * a = RuntimeType::cast<A>(args);
        CHECK(a != nullptr);
        if (!a) return FAILED;
        CHECK(a->input1 != nullptr);
        CHECK(a->input2 != nullptr);
        CHECK(a->output != nullptr);
        if (!a->input1 || !a->input2 || !a->output) return FAILED;
        a->output->value = a->input1->value + a->input2->value;
        return PASSED;
    }
};

struct MultiplyIntegersAction : public Action {
    GN_REGISTER_RUNTIME_TYPE(Action);

    MultiplyIntegersAction(const StrA & name): Action(TYPE_INFO(), name) {}

    static GN::AutoRef<MultiplyIntegersAction> create(const StrA & name) { return GN::AutoRef<MultiplyIntegersAction>(new MultiplyIntegersAction(name)); }

    struct A : public Arguments {
        GN_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}
        AutoRef<IntegerArtifact> input1;
        AutoRef<IntegerArtifact> input2;
        AutoRef<IntegerArtifact> output;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (input1) list.readList.insert(input1.get());
            if (input2) list.readList.insert(input2.get());
            if (output) list.writeList.insert(output.get());
        }
    };

    Action::PrepareResult prepare(TaskInfo &, Arguments &) override { return {PASSED, 1}; }

    ExecutionResult execute(TaskInfo &, size_t, Arguments & args) override {
        auto * a = RuntimeType::cast<A>(args);
        CHECK(a != nullptr);
        if (!a) return FAILED;
        CHECK(a->input1 != nullptr);
        CHECK(a->input2 != nullptr);
        CHECK(a->output != nullptr);
        if (!a->input1 || !a->input2 || !a->output) return FAILED;
        a->output->value = a->input1->value * a->input2->value;
        return PASSED;
    }

    using Action::Action;
};

struct ReadIntegerAction : public Action {
    GN_REGISTER_RUNTIME_TYPE(Action);

    ReadIntegerAction(const StrA & name): Action(TYPE_INFO(), name) {}

    static GN::AutoRef<ReadIntegerAction> create(const StrA & name) { return GN::AutoRef<ReadIntegerAction>(new ReadIntegerAction(name)); }

    struct A : public Arguments {
        GN_REGISTER_RUNTIME_TYPE();
        A(): Arguments(TYPE_INFO()) {}
        AutoRef<IntegerArtifact> input;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (input) list.readList.insert(input.get());
        }
    };

    PrepareResult   prepare(TaskInfo &, Arguments &) override { return {PASSED, 1}; }
    ExecutionResult execute(TaskInfo &, size_t, Arguments &) override { return PASSED; }
};

} // namespace GN::rdg

// Helper: true if workflowDependencies[workflowIdx] contains requiredDep.
static bool workflowDependsOn(const std::unordered_map<uint64_t, GN::DynaArray<size_t>> & deps, size_t workflowIdx, size_t requiredDep) {
    auto it = deps.find((uint64_t) workflowIdx);
    if (it == deps.end()) return false;
    for (size_t k = 0; k < it->second.size(); ++k)
        if (it->second[k] == requiredDep) return true;
    return false;
}

// ============================================================================
// TEST CASES
// ============================================================================

TEST_CASE("RDG workflow: arithmetic 3*(1+2)=9", "[rdg][workflow]") {
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);

    auto one    = GN::rdg::IntegerArtifact::create("one");
    auto two    = GN::rdg::IntegerArtifact::create("two");
    auto three  = GN::rdg::IntegerArtifact::create("three");
    auto sum    = GN::rdg::IntegerArtifact::create("sum");
    auto result = GN::rdg::IntegerArtifact::create("result");
    REQUIRE(one != nullptr);
    REQUIRE(two != nullptr);
    REQUIRE(three != nullptr);
    REQUIRE(sum != nullptr);
    REQUIRE(result != nullptr);

    GN::DynaArray<GN::rdg::Workflow> workflows;

    // Workflow 1: initialize values
    {
        auto workflow = renderGraph->createWorkflow("initialize_values");
        REQUIRE(workflow.name() == "initialize_values");

        auto appendInit = [&](const char * name, GN::AutoRef<GN::rdg::IntegerArtifact> out, int val) {
            auto action       = GN::rdg::InitIntegerAction::create(name);
            action->initValue = val;
            auto args         = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
            args->output      = out;
            GN::rdg::Workflow::Task task(name);
            task.action    = action;
            task.arguments = args;
            workflow.appendTask(std::move(task));
        };
        appendInit("init_one", one, 1);
        appendInit("init_two", two, 2);
        appendInit("init_three", three, 3);
        workflows.append(std::move(workflow));
    }

    // Workflow 2: sum = 1 + 2
    {
        auto workflow = renderGraph->createWorkflow("compute_sum");
        REQUIRE(workflow.name() == "compute_sum");

        auto addAction  = GN::rdg::AddIntegersAction::create("add_1_2");
        auto addArgs    = GN::AutoRef<GN::rdg::AddIntegersAction::A>::make();
        addArgs->input1 = one;
        addArgs->input2 = two;
        addArgs->output = sum;
        GN::rdg::Workflow::Task task("add_1_2");
        task.action    = addAction;
        task.arguments = addArgs;
        workflow.appendTask(std::move(task));
        workflows.append(std::move(workflow));
    }

    // Workflow 3: result = 3 * sum
    {
        auto workflow = renderGraph->createWorkflow("compute_result");
        REQUIRE(workflow.name() == "compute_result");

        auto mulAction  = GN::rdg::MultiplyIntegersAction::create("multiply_3_sum");
        auto mulArgs    = GN::AutoRef<GN::rdg::MultiplyIntegersAction::A>::make();
        mulArgs->input1 = three;
        mulArgs->input2 = sum;
        mulArgs->output = result;
        GN::rdg::Workflow::Task task("multiply_3_sum");
        task.action    = mulAction;
        task.arguments = mulArgs;
        workflow.appendTask(std::move(task));
        workflows.append(std::move(workflow));
    }

    auto submission = renderGraph->submit({.workflows = GN::ArrayView<GN::rdg::Workflow>(workflows.data(), workflows.size())});
    REQUIRE(submission != nullptr);

    auto submissionResult = submission->result();
    CHECK(submissionResult.executionResult == GN::rdg::Action::PASSED);
    CHECK(result->value == 9);
    CHECK(one->value == 1);
    CHECK(two->value == 2);
    CHECK(three->value == 3);
    CHECK(sum->value == 3);
}

TEST_CASE("RDG workflow: create three workflows", "[rdg][workflow]") {
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);

    auto w1 = renderGraph->createWorkflow("w1");
    auto w2 = renderGraph->createWorkflow("w2");
    auto w3 = renderGraph->createWorkflow("w3");
    CHECK(w1.name() == "w1");
    CHECK(w2.name() == "w2");
    CHECK(w3.name() == "w3");
}

TEST_CASE("RDG workflow: arguments artifact discovery", "[rdg][workflow]") {
    auto x = GN::rdg::IntegerArtifact::create("x");
    REQUIRE(x != nullptr);

    auto initArgs    = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    initArgs->output = x;

    std::unordered_set<const GN::rdg::Artifact *> readList, writeList;
    GN::rdg::Arguments::ArtifactReadWriteList     list {readList, writeList};
    initArgs->addToReadWriteList(list);
    CHECK(readList.size() == 0u);
    CHECK(writeList.size() == 1u);
    CHECK(writeList.find(x.get()) != writeList.end());
}

TEST_CASE("RDG workflow: argument discovery counts", "[rdg][workflow]") {
    auto a1 = GN::rdg::IntegerArtifact::create("a1");
    auto a2 = GN::rdg::IntegerArtifact::create("a2");
    auto a3 = GN::rdg::IntegerArtifact::create("a3");
    REQUIRE(a1);
    REQUIRE(a2);
    REQUIRE(a3);

    std::unordered_set<const GN::rdg::Artifact *> readList, writeList;
    GN::rdg::Arguments::ArtifactReadWriteList     list {readList, writeList};

    auto initArgs    = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    initArgs->output = a1;
    initArgs->addToReadWriteList(list);
    CHECK(readList.size() + writeList.size() == 1u);

    readList.clear();
    writeList.clear();
    auto addArgs    = GN::AutoRef<GN::rdg::AddIntegersAction::A>::make();
    addArgs->input1 = a1;
    addArgs->input2 = a2;
    addArgs->output = a3;
    addArgs->addToReadWriteList(list);
    CHECK(readList.size() == 2u);
    CHECK(writeList.size() == 1u);

    readList.clear();
    writeList.clear();
    auto mulArgs    = GN::AutoRef<GN::rdg::MultiplyIntegersAction::A>::make();
    mulArgs->input1 = a1;
    mulArgs->input2 = a2;
    mulArgs->output = a3;
    mulArgs->addToReadWriteList(list);
    CHECK(readList.size() == 2u);
    CHECK(writeList.size() == 1u);
}

TEST_CASE("RDG workflow: dependency write-write", "[rdg][workflow]") {
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);
    auto x = GN::rdg::IntegerArtifact::create("x");
    REQUIRE(x != nullptr);

    auto w0          = renderGraph->createWorkflow("writer_first");
    auto init0       = GN::rdg::InitIntegerAction::create("init0");
    init0->initValue = 1;
    auto args0       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args0->output    = x;
    {
        GN::rdg::Workflow::Task t("init0");
        t.action    = init0;
        t.arguments = args0;
        w0.appendTask(std::move(t));
    }
    auto w1          = renderGraph->createWorkflow("writer_second");
    auto init1       = GN::rdg::InitIntegerAction::create("init1");
    init1->initValue = 2;
    auto args1       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args1->output    = x;
    {
        GN::rdg::Workflow::Task t("init1");
        t.action    = init1;
        t.arguments = args1;
        w1.appendTask(std::move(t));
    }

    GN::DynaArray<GN::rdg::Workflow> toSubmit;
    toSubmit.append(std::move(w0));
    toSubmit.append(std::move(w1));
    auto submission = renderGraph->submit({.workflows = GN::ArrayView<GN::rdg::Workflow>(toSubmit.data(), toSubmit.size())});
    REQUIRE(submission != nullptr);
    submission->result();
    auto state = submission->dumpState();
    CHECK(state.workflowDependencies.size() == 2u);
    CHECK(workflowDependsOn(state.workflowDependencies, 1, 0));
}

TEST_CASE("RDG workflow: dependency read-write", "[rdg][workflow]") {
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);
    auto x = GN::rdg::IntegerArtifact::create("x");
    auto y = GN::rdg::IntegerArtifact::create("y");
    REQUIRE(x != nullptr);
    REQUIRE(y != nullptr);

    auto w0          = renderGraph->createWorkflow("writer");
    auto init0       = GN::rdg::InitIntegerAction::create("init_x");
    init0->initValue = 10;
    auto args0       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args0->output    = x;
    {
        GN::rdg::Workflow::Task t("init_x");
        t.action    = init0;
        t.arguments = args0;
        w0.appendTask(std::move(t));
    }
    auto w1       = renderGraph->createWorkflow("reader");
    auto add1     = GN::rdg::AddIntegersAction::create("add");
    auto args1    = GN::AutoRef<GN::rdg::AddIntegersAction::A>::make();
    args1->input1 = x;
    args1->input2 = x;
    args1->output = y;
    {
        GN::rdg::Workflow::Task t("add");
        t.action    = add1;
        t.arguments = args1;
        w1.appendTask(std::move(t));
    }

    GN::DynaArray<GN::rdg::Workflow> toSubmit;
    toSubmit.append(std::move(w0));
    toSubmit.append(std::move(w1));
    auto submission = renderGraph->submit({.workflows = GN::ArrayView<GN::rdg::Workflow>(toSubmit.data(), toSubmit.size())});
    REQUIRE(submission != nullptr);
    submission->result();
    auto state = submission->dumpState();
    CHECK(state.workflowDependencies.size() == 2u);
    CHECK(workflowDependsOn(state.workflowDependencies, 1, 0));
    CHECK(y->value == 20);
}

TEST_CASE("RDG workflow: dependency write-read", "[rdg][workflow]") {
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);
    auto x = GN::rdg::IntegerArtifact::create("x");
    auto y = GN::rdg::IntegerArtifact::create("y");
    REQUIRE(x != nullptr);
    REQUIRE(y != nullptr);

    auto w0          = renderGraph->createWorkflow("writer");
    auto init0       = GN::rdg::InitIntegerAction::create("init_x");
    init0->initValue = 1;
    auto args0       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args0->output    = x;
    {
        GN::rdg::Workflow::Task t("init_x");
        t.action    = init0;
        t.arguments = args0;
        w0.appendTask(std::move(t));
    }
    auto w1      = renderGraph->createWorkflow("reader");
    auto read1   = GN::rdg::ReadIntegerAction::create("read_x");
    auto args1   = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
    args1->input = x;
    {
        GN::rdg::Workflow::Task t("read_x");
        t.action    = read1;
        t.arguments = args1;
        w1.appendTask(std::move(t));
    }
    auto w2          = renderGraph->createWorkflow("writer_second");
    auto init2       = GN::rdg::InitIntegerAction::create("overwrite_x");
    init2->initValue = 2;
    auto args2       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args2->output    = x;
    {
        GN::rdg::Workflow::Task t("overwrite_x");
        t.action    = init2;
        t.arguments = args2;
        w2.appendTask(std::move(t));
    }

    GN::DynaArray<GN::rdg::Workflow> toSubmit;
    toSubmit.append(std::move(w0));
    toSubmit.append(std::move(w1));
    toSubmit.append(std::move(w2));
    auto submission = renderGraph->submit({.workflows = GN::ArrayView<GN::rdg::Workflow>(toSubmit.data(), toSubmit.size())});
    REQUIRE(submission != nullptr);
    submission->result();
    auto state = submission->dumpState();
    CHECK(state.workflowDependencies.size() == 3u);
    CHECK(workflowDependsOn(state.workflowDependencies, 1, 0));
    CHECK(workflowDependsOn(state.workflowDependencies, 2, 1));
}

TEST_CASE("RDG workflow: dependency read-read (no dependency)", "[rdg][workflow]") {
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);
    auto x = GN::rdg::IntegerArtifact::create("x");
    REQUIRE(x != nullptr);

    auto w0          = renderGraph->createWorkflow("writer");
    auto init0       = GN::rdg::InitIntegerAction::create("init_x");
    init0->initValue = 5;
    auto args0       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args0->output    = x;
    {
        GN::rdg::Workflow::Task t("init_x");
        t.action    = init0;
        t.arguments = args0;
        w0.appendTask(std::move(t));
    }
    auto w1      = renderGraph->createWorkflow("reader1");
    auto read1   = GN::rdg::ReadIntegerAction::create("read1");
    auto args1   = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
    args1->input = x;
    {
        GN::rdg::Workflow::Task t("read1");
        t.action    = read1;
        t.arguments = args1;
        w1.appendTask(std::move(t));
    }
    auto w2      = renderGraph->createWorkflow("reader2");
    auto read2   = GN::rdg::ReadIntegerAction::create("read2");
    auto args2   = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
    args2->input = x;
    {
        GN::rdg::Workflow::Task t("read2");
        t.action    = read2;
        t.arguments = args2;
        w2.appendTask(std::move(t));
    }

    GN::DynaArray<GN::rdg::Workflow> toSubmit;
    toSubmit.append(std::move(w0));
    toSubmit.append(std::move(w1));
    toSubmit.append(std::move(w2));
    auto submission = renderGraph->submit({.workflows = GN::ArrayView<GN::rdg::Workflow>(toSubmit.data(), toSubmit.size())});
    REQUIRE(submission != nullptr);
    submission->result();
    auto state = submission->dumpState();
    CHECK(state.workflowDependencies.size() == 3u);
    CHECK(workflowDependsOn(state.workflowDependencies, 1, 0));
    CHECK(workflowDependsOn(state.workflowDependencies, 2, 0));
    CHECK(!workflowDependsOn(state.workflowDependencies, 2, 1));
}

// Name/sequence lookup and duplicate-name policy are the responsibility of higher-level layers (e.g. scene management).
// RDG no longer provides ArtifactDatabase or duplicate-name rejection.
