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

static inline uint64_t getTestTypeId() {
    static std::atomic<uint64_t> nextId = 1;
    return nextId.fetch_add(1, std::memory_order_relaxed);
}

struct IntegerArtifact : public Artifact {
    inline static const uint64_t         TYPE_ID   = getTestTypeId();
    inline static constexpr const char * TYPE_NAME = "IntegerArtifact";

    int value = 0;

    IntegerArtifact(ArtifactDatabase & db, const StrA & name): Artifact(db, TYPE_ID, TYPE_NAME, name) {}

    static GN::AutoRef<IntegerArtifact> create(ArtifactDatabase & db, const StrA & name) {
        auto * p = new IntegerArtifact(db, name);
        if (p->sequence == 0) {
            delete p;
            return {};
        }
        return GN::AutoRef<IntegerArtifact>(p);
    }
};

struct InitIntegerAction : public Action {
    inline static const uint64_t         TYPE_ID   = getTestTypeId();
    inline static constexpr const char * TYPE_NAME = "InitIntegerAction";

    int initValue = 0;

    InitIntegerAction(ArtifactDatabase & db, const StrA & name): Action(db, TYPE_ID, TYPE_NAME, name) {}

    static GN::AutoRef<InitIntegerAction> create(ArtifactDatabase & db, const StrA & name) {
        auto * p = new InitIntegerAction(db, name);
        if (p->sequence == 0) {
            delete p;
            return {};
        }
        return GN::AutoRef<InitIntegerAction>(p);
    }

    struct A : public Arguments {
        inline static const uint64_t         TYPE_ID   = getTestTypeId();
        inline static constexpr const char * TYPE_NAME = "InitIntegerAction::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}
        AutoRef<IntegerArtifact> output;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (output) list.writeList.insert(output.get());
        }
    };

    ExecutionResult prepare(TaskInfo &, Arguments &) override { return PASSED; }

    ExecutionResult execute(TaskInfo &, Arguments & args) override {
        auto * a = args.castTo<A>();
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
    inline static const uint64_t         TYPE_ID   = getTestTypeId();
    inline static constexpr const char * TYPE_NAME = "AddIntegersAction";

    AddIntegersAction(ArtifactDatabase & db, const StrA & name): Action(db, TYPE_ID, TYPE_NAME, name) {}

    static GN::AutoRef<AddIntegersAction> create(ArtifactDatabase & db, const StrA & name) {
        auto * p = new AddIntegersAction(db, name);
        if (p->sequence == 0) {
            delete p;
            return {};
        }
        return GN::AutoRef<AddIntegersAction>(p);
    }

    struct A : public Arguments {
        inline static const uint64_t         TYPE_ID   = getTestTypeId();
        inline static constexpr const char * TYPE_NAME = "AddIntegersAction::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}
        AutoRef<IntegerArtifact> input1;
        AutoRef<IntegerArtifact> input2;
        AutoRef<IntegerArtifact> output;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (input1) list.readList.insert(input1.get());
            if (input2) list.readList.insert(input2.get());
            if (output) list.writeList.insert(output.get());
        }
    };

    ExecutionResult prepare(TaskInfo &, Arguments &) override { return PASSED; }

    ExecutionResult execute(TaskInfo &, Arguments & args) override {
        auto * a = args.castTo<A>();
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
    inline static const uint64_t         TYPE_ID   = getTestTypeId();
    inline static constexpr const char * TYPE_NAME = "MultiplyIntegersAction";

    MultiplyIntegersAction(ArtifactDatabase & db, const StrA & name): Action(db, TYPE_ID, TYPE_NAME, name) {}

    static GN::AutoRef<MultiplyIntegersAction> create(ArtifactDatabase & db, const StrA & name) {
        auto * p = new MultiplyIntegersAction(db, name);
        if (p->sequence == 0) {
            delete p;
            return {};
        }
        return GN::AutoRef<MultiplyIntegersAction>(p);
    }

    struct A : public Arguments {
        inline static const uint64_t         TYPE_ID   = getTestTypeId();
        inline static constexpr const char * TYPE_NAME = "MultiplyIntegersAction::A";
        A(): Arguments(TYPE_ID, TYPE_NAME) {}
        AutoRef<IntegerArtifact> input1;
        AutoRef<IntegerArtifact> input2;
        AutoRef<IntegerArtifact> output;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (input1) list.readList.insert(input1.get());
            if (input2) list.readList.insert(input2.get());
            if (output) list.writeList.insert(output.get());
        }
    };

    ExecutionResult prepare(TaskInfo &, Arguments &) override { return PASSED; }

    ExecutionResult execute(TaskInfo &, Arguments & args) override {
        auto * a = args.castTo<A>();
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
    inline static const uint64_t TYPE_ID = getTestTypeId();

    ReadIntegerAction(ArtifactDatabase & db, const StrA & name): Action(db, TYPE_ID, "ReadIntegerAction", name) {}

    static GN::AutoRef<ReadIntegerAction> create(ArtifactDatabase & db, const StrA & name) {
        auto * p = new ReadIntegerAction(db, name);
        if (p->sequence == 0) {
            delete p;
            return {};
        }
        return GN::AutoRef<ReadIntegerAction>(p);
    }

    struct A : public Arguments {
        inline static const uint64_t         TYPE      = getTestTypeId();
        inline static constexpr const char * TYPE_NAME = "ReadIntegerAction::A";
        A(): Arguments(TYPE, TYPE_NAME) {}
        AutoRef<IntegerArtifact> input;

        void addToReadWriteList(ArtifactReadWriteList & list) const override {
            if (input) list.readList.insert(input.get());
        }
    };

    ExecutionResult prepare(TaskInfo &, Arguments &) override { return PASSED; }
    ExecutionResult execute(TaskInfo &, Arguments &) override { return PASSED; }
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
    auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
    REQUIRE(db != nullptr);

    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);

    auto one    = GN::rdg::IntegerArtifact::create(*db, "one");
    auto two    = GN::rdg::IntegerArtifact::create(*db, "two");
    auto three  = GN::rdg::IntegerArtifact::create(*db, "three");
    auto sum    = GN::rdg::IntegerArtifact::create(*db, "sum");
    auto result = GN::rdg::IntegerArtifact::create(*db, "result");
    REQUIRE(one != nullptr);
    REQUIRE(two != nullptr);
    REQUIRE(three != nullptr);
    REQUIRE(sum != nullptr);
    REQUIRE(result != nullptr);

    GN::DynaArray<GN::rdg::Workflow *> workflows;

    // Workflow 1: initialize values
    {
        auto * workflow = renderGraph->createWorkflow("initialize_values");
        REQUIRE(workflow != nullptr);

        auto appendInit = [&](const char * name, GN::AutoRef<GN::rdg::IntegerArtifact> out, int val) {
            auto action       = GN::rdg::InitIntegerAction::create(*db, name);
            action->initValue = val;
            auto args         = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
            args->output      = out;
            GN::rdg::Workflow::Task task(name);
            task.action    = action;
            task.arguments = args;
            workflow->tasks.append(task);
        };
        appendInit("init_one", one, 1);
        appendInit("init_two", two, 2);
        appendInit("init_three", three, 3);
        workflows.append(workflow);
    }

    // Workflow 2: sum = 1 + 2
    {
        auto * workflow = renderGraph->createWorkflow("compute_sum");
        REQUIRE(workflow != nullptr);

        auto addAction  = GN::rdg::AddIntegersAction::create(*db, "add_1_2");
        auto addArgs    = GN::AutoRef<GN::rdg::AddIntegersAction::A>::make();
        addArgs->input1 = one;
        addArgs->input2 = two;
        addArgs->output = sum;
        GN::rdg::Workflow::Task task("add_1_2");
        task.action    = addAction;
        task.arguments = addArgs;
        workflow->tasks.append(task);
        workflows.append(workflow);
    }

    // Workflow 3: result = 3 * sum
    {
        auto * workflow = renderGraph->createWorkflow("compute_result");
        REQUIRE(workflow != nullptr);

        auto mulAction  = GN::rdg::MultiplyIntegersAction::create(*db, "multiply_3_sum");
        auto mulArgs    = GN::AutoRef<GN::rdg::MultiplyIntegersAction::A>::make();
        mulArgs->input1 = three;
        mulArgs->input2 = sum;
        mulArgs->output = result;
        GN::rdg::Workflow::Task task("multiply_3_sum");
        task.action    = mulAction;
        task.arguments = mulArgs;
        workflow->tasks.append(task);
        workflows.append(workflow);
    }

    auto submission = renderGraph->submit({.workflows = workflows});
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
    auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
    REQUIRE(db != nullptr);

    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);

    CHECK(renderGraph->createWorkflow("w1") != nullptr);
    CHECK(renderGraph->createWorkflow("w2") != nullptr);
    CHECK(renderGraph->createWorkflow("w3") != nullptr);
}

TEST_CASE("RDG workflow: arguments artifact discovery", "[rdg][workflow]") {
    auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
    REQUIRE(db != nullptr);

    auto x = GN::rdg::IntegerArtifact::create(*db, "x");
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
    auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
    REQUIRE(db != nullptr);

    auto a1 = GN::rdg::IntegerArtifact::create(*db, "a1");
    auto a2 = GN::rdg::IntegerArtifact::create(*db, "a2");
    auto a3 = GN::rdg::IntegerArtifact::create(*db, "a3");
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
    auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
    REQUIRE(db != nullptr);
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);
    auto x = GN::rdg::IntegerArtifact::create(*db, "x");
    REQUIRE(x != nullptr);

    auto * w0        = renderGraph->createWorkflow("writer_first");
    auto   init0     = GN::rdg::InitIntegerAction::create(*db, "init0");
    init0->initValue = 1;
    auto args0       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args0->output    = x;
    w0->tasks.append(GN::rdg::Workflow::Task("init0"));
    w0->tasks.back().action    = init0;
    w0->tasks.back().arguments = args0;

    auto * w1        = renderGraph->createWorkflow("writer_second");
    auto   init1     = GN::rdg::InitIntegerAction::create(*db, "init1");
    init1->initValue = 2;
    auto args1       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args1->output    = x;
    w1->tasks.append(GN::rdg::Workflow::Task("init1"));
    w1->tasks.back().action    = init1;
    w1->tasks.back().arguments = args1;

    auto submission = renderGraph->submit({.workflows = GN::DynaArray<GN::rdg::Workflow *>({w0, w1})});
    REQUIRE(submission != nullptr);
    submission->result();
    auto state = submission->dumpState();
    CHECK(state.workflowDependencies.size() == 2u);
    CHECK(workflowDependsOn(state.workflowDependencies, 1, 0));
}

TEST_CASE("RDG workflow: dependency read-write", "[rdg][workflow]") {
    auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
    REQUIRE(db != nullptr);
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);
    auto x = GN::rdg::IntegerArtifact::create(*db, "x");
    auto y = GN::rdg::IntegerArtifact::create(*db, "y");
    REQUIRE(x != nullptr);
    REQUIRE(y != nullptr);

    auto * w0        = renderGraph->createWorkflow("writer");
    auto   init0     = GN::rdg::InitIntegerAction::create(*db, "init_x");
    init0->initValue = 10;
    auto args0       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args0->output    = x;
    w0->tasks.append(GN::rdg::Workflow::Task("init_x"));
    w0->tasks.back().action    = init0;
    w0->tasks.back().arguments = args0;

    auto * w1     = renderGraph->createWorkflow("reader");
    auto   add1   = GN::rdg::AddIntegersAction::create(*db, "add");
    auto   args1  = GN::AutoRef<GN::rdg::AddIntegersAction::A>::make();
    args1->input1 = x;
    args1->input2 = x;
    args1->output = y;
    w1->tasks.append(GN::rdg::Workflow::Task("add"));
    w1->tasks.back().action    = add1;
    w1->tasks.back().arguments = args1;

    auto submission = renderGraph->submit({.workflows = std::vector<GN::rdg::Workflow *>({w0, w1})});
    REQUIRE(submission != nullptr);
    submission->result();
    auto state = submission->dumpState();
    CHECK(state.workflowDependencies.size() == 2u);
    CHECK(workflowDependsOn(state.workflowDependencies, 1, 0));
    CHECK(y->value == 20);
}

TEST_CASE("RDG workflow: dependency write-read", "[rdg][workflow]") {
    auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
    REQUIRE(db != nullptr);
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);
    auto x = GN::rdg::IntegerArtifact::create(*db, "x");
    auto y = GN::rdg::IntegerArtifact::create(*db, "y");
    REQUIRE(x != nullptr);
    REQUIRE(y != nullptr);

    auto * w0        = renderGraph->createWorkflow("writer");
    auto   init0     = GN::rdg::InitIntegerAction::create(*db, "init_x");
    init0->initValue = 1;
    auto args0       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args0->output    = x;
    w0->tasks.append(GN::rdg::Workflow::Task("init_x"));
    w0->tasks.back().action    = init0;
    w0->tasks.back().arguments = args0;

    auto * w1    = renderGraph->createWorkflow("reader");
    auto   read1 = GN::rdg::ReadIntegerAction::create(*db, "read_x");
    auto   args1 = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
    args1->input = x;
    w1->tasks.append(GN::rdg::Workflow::Task("read_x"));
    w1->tasks.back().action    = read1;
    w1->tasks.back().arguments = args1;

    auto * w2        = renderGraph->createWorkflow("writer_second");
    auto   init2     = GN::rdg::InitIntegerAction::create(*db, "overwrite_x");
    init2->initValue = 2;
    auto args2       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args2->output    = x;
    w2->tasks.append(GN::rdg::Workflow::Task("overwrite_x"));
    w2->tasks.back().action    = init2;
    w2->tasks.back().arguments = args2;

    auto submission = renderGraph->submit({.workflows = GN::DynaArray<GN::rdg::Workflow *>({w0, w1, w2})});
    REQUIRE(submission != nullptr);
    submission->result();
    auto state = submission->dumpState();
    CHECK(state.workflowDependencies.size() == 3u);
    CHECK(workflowDependsOn(state.workflowDependencies, 1, 0));
    CHECK(workflowDependsOn(state.workflowDependencies, 2, 1));
}

TEST_CASE("RDG workflow: dependency read-read (no dependency)", "[rdg][workflow]") {
    auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
    REQUIRE(db != nullptr);
    auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
    REQUIRE(renderGraph != nullptr);
    auto x = GN::rdg::IntegerArtifact::create(*db, "x");
    REQUIRE(x != nullptr);

    auto * w0        = renderGraph->createWorkflow("writer");
    auto   init0     = GN::rdg::InitIntegerAction::create(*db, "init_x");
    init0->initValue = 5;
    auto args0       = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
    args0->output    = x;
    w0->tasks.append(GN::rdg::Workflow::Task("init_x"));
    w0->tasks.back().action    = init0;
    w0->tasks.back().arguments = args0;

    auto * w1    = renderGraph->createWorkflow("reader1");
    auto   read1 = GN::rdg::ReadIntegerAction::create(*db, "read1");
    auto   args1 = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
    args1->input = x;
    w1->tasks.append(GN::rdg::Workflow::Task("read1"));
    w1->tasks.back().action    = read1;
    w1->tasks.back().arguments = args1;

    auto * w2    = renderGraph->createWorkflow("reader2");
    auto   read2 = GN::rdg::ReadIntegerAction::create(*db, "read2");
    auto   args2 = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
    args2->input = x;
    w2->tasks.append(GN::rdg::Workflow::Task("read2"));
    w2->tasks.back().action    = read2;
    w2->tasks.back().arguments = args2;

    auto submission = renderGraph->submit({GN::DynaArray<GN::rdg::Workflow *>({w0, w1, w2})});
    REQUIRE(submission != nullptr);
    submission->result();
    auto state = submission->dumpState();
    CHECK(state.workflowDependencies.size() == 3u);
    CHECK(workflowDependsOn(state.workflowDependencies, 1, 0));
    CHECK(workflowDependsOn(state.workflowDependencies, 2, 0));
    CHECK(!workflowDependsOn(state.workflowDependencies, 2, 1));
}

TEST_CASE("RDG workflow: duplicate artifact name returns null", "[rdg][workflow]") {
    auto db = GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {});
    REQUIRE(db != nullptr);

    auto first = GN::rdg::IntegerArtifact::create(*db, "unique_name");
    REQUIRE(first != nullptr);
    CHECK(first->sequence != 0);

    auto duplicate = GN::rdg::IntegerArtifact::create(*db, "unique_name");
    CHECK(duplicate == nullptr);

    auto fetched = db->fetch(GN::rdg::IntegerArtifact::TYPE_ID, "unique_name");
    REQUIRE(fetched != nullptr);
    CHECK(fetched->sequence == first->sequence);
}
