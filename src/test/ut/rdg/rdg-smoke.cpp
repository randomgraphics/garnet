/*
 * Render Graph Interface Test - Demonstrates 3 * (1 + 2) = 9
 *
 * This test demonstrates the render graph interface by showing how to:
 * 1. Define custom artifacts (IntegerArtifact)
 * 2. Define custom actions (InitIntegerAction, AddIntegersAction, MultiplyIntegersAction)
 * 3. Create workflows with tasks
 * 4. Schedule and execute workflows through a render graph
 */

#include "../testCommon.h"

#include <garnet/GNrdg.h>

// ============================================================================
// CUSTOM ARTIFACTS AND ACTIONS
// ============================================================================

namespace GN::rdg {

static inline uint64_t getTestTypeId() {
    static std::atomic<uint64_t> nextId = 1;
    return nextId.fetch_add(1, std::memory_order_relaxed);
}

// Define a custom artifact that holds an integer
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

// Define an action to initialize an integer artifact
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
        WriteOnly<IntegerArtifact> output = {this, "output"};
    };

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo &, Arguments &) override { return std::make_pair(PASSED, nullptr); }

    // Execute: set the output artifact's value
    ExecutionResult execute(TaskInfo &, Arguments & args, ExecutionContext *) override {
        auto * a = args.castTo<A>();
        TS_ASSERT(a != nullptr); // null means wrong type
        auto * outputArtifact = a->output.value.get();
        TS_ASSERT(outputArtifact != nullptr);

        auto integerArtifact = outputArtifact->castTo<IntegerArtifact>();
        TS_ASSERT(integerArtifact != nullptr);

        GN_INFO(GN::getLogger("GN.rdg.test"))("InitIntegerAction: initializing integer output to {}", initValue);
        integerArtifact->value = initValue;
        return PASSED;
    }
};

// Define an action to add two integers
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
        ReadOnly<IntegerArtifact>  input1 = {this, "input1"};
        ReadOnly<IntegerArtifact>  input2 = {this, "input2"};
        WriteOnly<IntegerArtifact> output = {this, "output"};
    };

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo &, Arguments &) override { return std::make_pair(PASSED, nullptr); }

    // Execute: input1.value + input2.value -> output.value
    ExecutionResult execute(TaskInfo &, Arguments & args, ExecutionContext *) override {
        auto * a = args.castTo<A>();
        TS_ASSERT(a != nullptr); // null means wrong type
        auto * integerInput1 = a->input1.value.get() ? a->input1.value.get()->castTo<IntegerArtifact>() : nullptr;
        auto * integerInput2 = a->input2.value.get() ? a->input2.value.get()->castTo<IntegerArtifact>() : nullptr;
        auto * integerOutput = a->output.value.get() ? a->output.value.get()->castTo<IntegerArtifact>() : nullptr;

        TS_ASSERT(integerInput1 != nullptr);
        TS_ASSERT(integerInput2 != nullptr);
        TS_ASSERT(integerOutput != nullptr);

        GN_INFO(GN::getLogger("GN.rdg.test"))
        ("AddIntegersAction: {} + {} = {}", integerInput1->value, integerInput2->value, integerInput1->value + integerInput2->value);
        integerOutput->value = integerInput1->value + integerInput2->value;
        return PASSED;
    }
};

// Define an action to multiply two integers
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
        ReadOnly<IntegerArtifact>  input1 = {this, "input1"};
        ReadOnly<IntegerArtifact>  input2 = {this, "input2"};
        WriteOnly<IntegerArtifact> output = {this, "output"};
    };

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo &, Arguments &) override { return std::make_pair(PASSED, nullptr); }

    // Execute: input1.value * input2.value -> output.value
    ExecutionResult execute(TaskInfo &, Arguments & args, ExecutionContext *) override {
        auto * a = args.castTo<A>();
        TS_ASSERT(a != nullptr); // null means wrong type
        auto * integerInput1 = a->input1.value.get() ? a->input1.value.get()->castTo<IntegerArtifact>() : nullptr;
        auto * integerInput2 = a->input2.value.get() ? a->input2.value.get()->castTo<IntegerArtifact>() : nullptr;
        auto * integerOutput = a->output.value.get() ? a->output.value.get()->castTo<IntegerArtifact>() : nullptr;

        TS_ASSERT(integerInput1 != nullptr);
        TS_ASSERT(integerInput2 != nullptr);
        TS_ASSERT(integerOutput != nullptr);

        GN_INFO(GN::getLogger("GN.rdg.test"))
        ("MultiplyIntegersAction: {} * {} = {}", integerInput1->value, integerInput2->value, integerInput1->value * integerInput2->value);
        integerOutput->value = integerInput1->value * integerInput2->value;
        return PASSED;
    }

    using Action::Action;
};

// Read-only action (one ReadOnly<IntegerArtifact>) for dependency read-read tests.
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
        ReadOnly<IntegerArtifact> input = {this, "input"};
    };

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo &, Arguments &) override { return std::make_pair(PASSED, nullptr); }
    ExecutionResult                                execute(TaskInfo &, Arguments &, ExecutionContext *) override { return PASSED; }
};

} // namespace GN::rdg

// Helper: true if state.workflowDependencies[workflowIdx] contains requiredDep.
static bool workflowDependsOn(const std::unordered_map<uint64_t, GN::DynaArray<size_t>> & deps, size_t workflowIdx, size_t requiredDep) {
    auto it = deps.find((uint64_t) workflowIdx);
    if (it == deps.end()) return false;
    for (size_t k = 0; k < it->second.size(); ++k)
        if (it->second[k] == requiredDep) return true;
    return false;
}

// ============================================================================
// TEST EXECUTION FLOW
// ============================================================================

class RenderGraphTest : public CxxTest::TestSuite {
public:
    void testRenderGraphArithmetic() {
        // Create an artifact database (no type registration; each artifact creates itself and admits via Artifact ctor)
        auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
        TS_ASSERT(db != nullptr);

        // Create a render graph instance
        GN::rdg::RenderGraph::CreateParameters params;
        auto                                   renderGraph = GN::rdg::RenderGraph::create(params);
        TS_ASSERT(renderGraph != nullptr);

        // Create artifacts (each creates its own instance and registers via admit() in Artifact ctor)
        auto one    = GN::rdg::IntegerArtifact::create(*db, "one");
        auto two    = GN::rdg::IntegerArtifact::create(*db, "two");
        auto three  = GN::rdg::IntegerArtifact::create(*db, "three");
        auto sum    = GN::rdg::IntegerArtifact::create(*db, "sum");
        auto result = GN::rdg::IntegerArtifact::create(*db, "result");

        TS_ASSERT(one != nullptr);
        TS_ASSERT(two != nullptr);
        TS_ASSERT(three != nullptr);
        TS_ASSERT(sum != nullptr);
        TS_ASSERT(result != nullptr);

        // Workflow 1: Initialize values (1, 2, 3)
        DynaArray<GN::rdg::Workflow *> workflows;
        {
            auto * workflow = renderGraph->createWorkflow("initialize_values");
            TS_ASSERT(workflow != nullptr);

            // Task 1: Initialize 'one' to 1
            {
                auto initAction = GN::rdg::InitIntegerAction::create(*db, "init_one");
                TS_ASSERT(initAction != nullptr);
                initAction->initValue = 1;

                auto initArgs = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
                TS_ASSERT(initArgs != nullptr);
                initArgs->output.value = one;

                GN::rdg::Workflow::Task task("init_one");
                task.action    = initAction;
                task.arguments = initArgs;
                workflow->tasks.append(task);
            }

            // Task 2: Initialize 'two' to 2
            {
                auto initAction = GN::rdg::InitIntegerAction::create(*db, "init_two");
                TS_ASSERT(initAction != nullptr);
                initAction->initValue = 2;

                auto initArgs = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
                TS_ASSERT(initArgs != nullptr);
                initArgs->output.value = two;

                GN::rdg::Workflow::Task task("init_two");
                task.action    = initAction;
                task.arguments = initArgs;
                workflow->tasks.append(task);
            }

            // Task 3: Initialize 'three' to 3
            {
                auto initAction = GN::rdg::InitIntegerAction::create(*db, "init_three");
                TS_ASSERT(initAction != nullptr);
                initAction->initValue = 3;

                auto initArgs = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
                TS_ASSERT(initArgs != nullptr);
                initArgs->output.value = three;

                GN::rdg::Workflow::Task task("init_three");
                task.action    = initAction;
                task.arguments = initArgs;
                workflow->tasks.append(task);
            }

            workflows.append(workflow);
        }

        // Workflow 2: Compute sum = 1 + 2
        {
            auto * workflow = renderGraph->createWorkflow("compute_sum");
            TS_ASSERT(workflow != nullptr);

            auto addAction = GN::rdg::AddIntegersAction::create(*db, "add_1_2");
            TS_ASSERT(addAction != nullptr);

            auto addArgs = GN::AutoRef<GN::rdg::AddIntegersAction::A>::make();
            TS_ASSERT(addArgs != nullptr);
            addArgs->input1.value = one;
            addArgs->input2.value = two;
            addArgs->output.value = sum;

            GN::rdg::Workflow::Task task("add_1_2");
            task.action    = addAction;
            task.arguments = addArgs;
            workflow->tasks.append(task);

            workflows.append(workflow);
        }

        // Workflow 3: Compute result = 3 * sum
        {
            auto * workflow = renderGraph->createWorkflow("compute_result");
            TS_ASSERT(workflow != nullptr);

            auto multiplyAction = GN::rdg::MultiplyIntegersAction::create(*db, "multiply_3_sum");
            TS_ASSERT(multiplyAction != nullptr);

            auto multiplyArgs = GN::AutoRef<GN::rdg::MultiplyIntegersAction::A>::make();
            TS_ASSERT(multiplyArgs != nullptr);
            multiplyArgs->input1.value = three;
            multiplyArgs->input2.value = sum;
            multiplyArgs->output.value = result;

            GN::rdg::Workflow::Task task("multiply_3_sum");
            task.action    = multiplyAction;
            task.arguments = multiplyArgs;
            workflow->tasks.append(task);

            workflows.append(workflow);
        }

        // Submit all created workflows for execution
        auto submission = renderGraph->submit({.workflows = workflows});
        TS_ASSERT(submission != nullptr);

        // Wait for completion and get result
        auto submissionResult = submission->result();
        TS_ASSERT_EQUALS(submissionResult.executionResult, GN::rdg::Action::PASSED);

        // Dump submission state to stdout
        auto dumpState = submission->dumpState();
        fprintf(stdout, "%s", dumpState.state.c_str());
        fflush(stdout);

        // Verify result
        TS_ASSERT_EQUALS(result->value, 9);

        // Verify intermediate values are correct
        TS_ASSERT_EQUALS(one->value, 1);
        TS_ASSERT_EQUALS(two->value, 2);
        TS_ASSERT_EQUALS(three->value, 3);
        TS_ASSERT_EQUALS(sum->value, 3);
    }

    void testWorkflowSequenceOrder() {
        auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
        TS_ASSERT(db != nullptr);
        GN::rdg::RenderGraph::CreateParameters params;
        auto                                   renderGraph = GN::rdg::RenderGraph::create(params);
        TS_ASSERT(renderGraph != nullptr);

        GN::rdg::Workflow * w1 = renderGraph->createWorkflow("w1");
        GN::rdg::Workflow * w2 = renderGraph->createWorkflow("w2");
        GN::rdg::Workflow * w3 = renderGraph->createWorkflow("w3");
        TS_ASSERT(w1 != nullptr);
        TS_ASSERT(w2 != nullptr);
        TS_ASSERT(w3 != nullptr);
    }

    void testArgumentsArtifactArgumentsDiscovery() {
        auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
        TS_ASSERT(db != nullptr);
        auto initArgs = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
        TS_ASSERT(initArgs != nullptr);

        const GN::rdg::Arguments::ArtifactArgument * p = initArgs->firstArtifactArgument();
        TS_ASSERT(p != nullptr);
        TS_ASSERT_EQUALS(p->name(), "output");
        TS_ASSERT((p->usage() & GN::rdg::Arguments::UsageFlag::Writing) != GN::rdg::Arguments::UsageFlag::None);
        TS_ASSERT(p->next() == nullptr);
    }

    void testArgumentDiscoveryCounts() {
        auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
        TS_ASSERT(db != nullptr);

        auto   initArgs = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
        size_t n        = 0;
        for (const GN::rdg::Arguments::ArtifactArgument * p = initArgs->firstArtifactArgument(); p; p = p->next()) ++n;
        TS_ASSERT_EQUALS(n, 1u);

        auto addArgs = GN::AutoRef<GN::rdg::AddIntegersAction::A>::make();
        n            = 0;
        for (const GN::rdg::Arguments::ArtifactArgument * p = addArgs->firstArtifactArgument(); p; p = p->next()) ++n;
        TS_ASSERT_EQUALS(n, 3u);

        auto multiplyArgs = GN::AutoRef<GN::rdg::MultiplyIntegersAction::A>::make();
        n                 = 0;
        for (const GN::rdg::Arguments::ArtifactArgument * p = multiplyArgs->firstArtifactArgument(); p; p = p->next()) ++n;
        TS_ASSERT_EQUALS(n, 3u);
    }

    // Dependency tests: verify dumpState().workflowDependencies matches dependency-graph.h rules
    // (A depends on B when A is newer than B and: A reads/writes artifact B writes, or A writes artifact B reads)

    void testDependencyWriteWrite() {
        auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
        TS_ASSERT(db != nullptr);
        auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
        TS_ASSERT(renderGraph != nullptr);
        auto x = GN::rdg::IntegerArtifact::create(*db, "x");
        TS_ASSERT(x != nullptr);

        auto * w0 = renderGraph->createWorkflow("writer_first");
        TS_ASSERT(w0 != nullptr);
        auto init0          = GN::rdg::InitIntegerAction::create(*db, "init0");
        init0->initValue    = 1;
        auto args0          = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
        args0->output.value = x;
        w0->tasks.append(GN::rdg::Workflow::Task("init0"));
        w0->tasks.back().action    = init0;
        w0->tasks.back().arguments = args0;

        auto * w1 = renderGraph->createWorkflow("writer_second");
        TS_ASSERT(w1 != nullptr);
        auto init1          = GN::rdg::InitIntegerAction::create(*db, "init1");
        init1->initValue    = 2;
        auto args1          = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
        args1->output.value = x;
        w1->tasks.append(GN::rdg::Workflow::Task("init1"));
        w1->tasks.back().action    = init1;
        w1->tasks.back().arguments = args1;

        auto submission = renderGraph->submit({.workflows = GN::DynaArray<GN::rdg::Workflow *>({w0, w1})});
        TS_ASSERT(submission != nullptr);
        submission->result();
        auto state = submission->dumpState();
        TS_ASSERT_EQUALS(state.workflowDependencies.size(), 2u);
        TS_ASSERT(workflowDependsOn(state.workflowDependencies, 1, 0)); // w1 (newer) depends on w0 (older) when both write same artifact
    }

    void testDependencyReadWrite() {
        auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
        TS_ASSERT(db != nullptr);
        auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
        TS_ASSERT(renderGraph != nullptr);
        auto x = GN::rdg::IntegerArtifact::create(*db, "x");
        auto y = GN::rdg::IntegerArtifact::create(*db, "y");
        TS_ASSERT(x != nullptr);
        TS_ASSERT(y != nullptr);

        auto w0 = renderGraph->createWorkflow("writer");
        TS_ASSERT(w0 != nullptr);
        auto init0          = GN::rdg::InitIntegerAction::create(*db, "init_x");
        init0->initValue    = 10;
        auto args0          = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
        args0->output.value = x;
        w0->tasks.append(GN::rdg::Workflow::Task("init_x"));
        w0->tasks.back().action    = init0;
        w0->tasks.back().arguments = args0;

        auto w1 = renderGraph->createWorkflow("reader");
        TS_ASSERT(w1 != nullptr);
        auto add1           = GN::rdg::AddIntegersAction::create(*db, "add");
        auto args1          = GN::AutoRef<GN::rdg::AddIntegersAction::A>::make();
        args1->input1.value = x;
        args1->input2.value = x;
        args1->output.value = y;
        w1->tasks.append(GN::rdg::Workflow::Task("add"));
        w1->tasks.back().action    = add1;
        w1->tasks.back().arguments = args1;

        auto workflows  = std::vector<GN::rdg::Workflow *>({w0, w1});
        auto submission = renderGraph->submit({.workflows = workflows});
        TS_ASSERT(submission != nullptr);
        submission->result();
        auto state = submission->dumpState();
        TS_ASSERT_EQUALS(state.workflowDependencies.size(), 2u);
        TS_ASSERT(workflowDependsOn(state.workflowDependencies, 1, 0)); // reader (newer) depends on writer (older)
        TS_ASSERT_EQUALS(y->value, 20);
    }

    void testDependencyWriteRead() {
        auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
        TS_ASSERT(db != nullptr);
        auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
        TS_ASSERT(renderGraph != nullptr);
        auto x = GN::rdg::IntegerArtifact::create(*db, "x");
        auto y = GN::rdg::IntegerArtifact::create(*db, "y");
        TS_ASSERT(x != nullptr);
        TS_ASSERT(y != nullptr);

        auto * w0           = renderGraph->createWorkflow("writer");
        auto   init0        = GN::rdg::InitIntegerAction::create(*db, "init_x");
        init0->initValue    = 1;
        auto args0          = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
        args0->output.value = x;
        w0->tasks.append(GN::rdg::Workflow::Task("init_x"));
        w0->tasks.back().action    = init0;
        w0->tasks.back().arguments = args0;

        auto * w1          = renderGraph->createWorkflow("reader");
        auto   read1       = GN::rdg::ReadIntegerAction::create(*db, "read_x");
        auto   args1       = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
        args1->input.value = x;
        w1->tasks.append(GN::rdg::Workflow::Task("read_x"));
        w1->tasks.back().action    = read1;
        w1->tasks.back().arguments = args1;

        auto * w2           = renderGraph->createWorkflow("writer_second");
        auto   init2        = GN::rdg::InitIntegerAction::create(*db, "overwrite_x");
        init2->initValue    = 2;
        auto args2          = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
        args2->output.value = x;
        w2->tasks.append(GN::rdg::Workflow::Task("overwrite_x"));
        w2->tasks.back().action    = init2;
        w2->tasks.back().arguments = args2;

        auto submission = renderGraph->submit({.workflows = GN::DynaArray<GN::rdg::Workflow *>({w0, w1, w2})});
        TS_ASSERT(submission != nullptr);
        submission->result();
        auto state = submission->dumpState();
        TS_ASSERT_EQUALS(state.workflowDependencies.size(), 3u);
        TS_ASSERT(workflowDependsOn(state.workflowDependencies, 1, 0)); // reader depends on first writer
        TS_ASSERT(workflowDependsOn(state.workflowDependencies, 2, 1)); // second writer depends on reader (write-read: newer writes what older reads)
    }

    void testDependencyReadRead() {
        auto db = std::unique_ptr<GN::rdg::ArtifactDatabase>(GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {}));
        TS_ASSERT(db != nullptr);
        auto renderGraph = GN::rdg::RenderGraph::create(GN::rdg::RenderGraph::CreateParameters {});
        TS_ASSERT(renderGraph != nullptr);
        auto x = GN::rdg::IntegerArtifact::create(*db, "x");
        TS_ASSERT(x != nullptr);

        auto * w0           = renderGraph->createWorkflow("writer");
        auto   init0        = GN::rdg::InitIntegerAction::create(*db, "init_x");
        init0->initValue    = 5;
        auto args0          = GN::AutoRef<GN::rdg::InitIntegerAction::A>::make();
        args0->output.value = x;
        w0->tasks.append(GN::rdg::Workflow::Task("init_x"));
        w0->tasks.back().action    = init0;
        w0->tasks.back().arguments = args0;

        auto * w1          = renderGraph->createWorkflow("reader1");
        auto   read1       = GN::rdg::ReadIntegerAction::create(*db, "read1");
        auto   args1       = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
        args1->input.value = x;
        w1->tasks.append(GN::rdg::Workflow::Task("read1"));
        w1->tasks.back().action    = read1;
        w1->tasks.back().arguments = args1;

        auto * w2          = renderGraph->createWorkflow("reader2");
        auto   read2       = GN::rdg::ReadIntegerAction::create(*db, "read2");
        auto   args2       = GN::AutoRef<GN::rdg::ReadIntegerAction::A>::make();
        args2->input.value = x;
        w2->tasks.append(GN::rdg::Workflow::Task("read2"));
        w2->tasks.back().action    = read2;
        w2->tasks.back().arguments = args2;

        auto submission = renderGraph->submit({DynaArray<GN::rdg::Workflow *>({w0, w1, w2})});
        TS_ASSERT(submission != nullptr);
        submission->result();
        auto state = submission->dumpState();
        TS_ASSERT_EQUALS(state.workflowDependencies.size(), 3u);
        TS_ASSERT(workflowDependsOn(state.workflowDependencies, 1, 0));  // reader1 depends on writer
        TS_ASSERT(workflowDependsOn(state.workflowDependencies, 2, 0));  // reader2 depends on writer
        TS_ASSERT(!workflowDependsOn(state.workflowDependencies, 2, 1)); // reader2 does NOT depend on reader1 (read-read: no dependency)
    }

    void testCreateDuplicateTypeNameReturnsNull() {
        auto db = GN::rdg::ArtifactDatabase::create(GN::rdg::ArtifactDatabase::CreateParameters {});
        TS_ASSERT(db != nullptr);

        auto first = GN::rdg::IntegerArtifact::create(*db, "unique_name");
        TS_ASSERT(first != nullptr);
        TS_ASSERT(first->sequence != 0);

        // Duplicate type+name must cause create() to return null (sequence was 0, instance deleted)
        auto duplicate = GN::rdg::IntegerArtifact::create(*db, "unique_name");
        TS_ASSERT(duplicate == nullptr);

        // First instance still valid and fetchable
        auto fetched = db->fetch(GN::rdg::IntegerArtifact::TYPE_ID, "unique_name");
        TS_ASSERT(fetched != nullptr);
        TS_ASSERT(fetched->sequence == first->sequence);
    }

    void testArrayArtifactArgument() {
        struct A : public GN::rdg::Arguments {
            GN::rdg::Arguments::ArtifactArray<GN::rdg::IntegerArtifact, 3> array = {this, "array"};
        };
    }
};

/*
 * EXPECTED OUTPUT:
 *
 * Executing workflow: initialize_values
 *   Executing task: init_one -> one = 1
 *   Executing task: init_two -> two = 2
 *   Executing task: init_three -> three = 3
 *
 * Executing workflow: compute_sum
 *   Executing task: add_1_2 -> sum = 1 + 2 = 3
 *
 * Executing workflow: compute_result
 *   Executing task: multiply_3_sum -> result = 3 * 3 = 9
 *
 * Final result: 9 (SUCCESS!)
 */
