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

} // namespace GN::rdg

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
        {
            auto * workflow = renderGraph->schedule("initialize_values");
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
        }

        // Workflow 2: Compute sum = 1 + 2
        {
            auto * workflow = renderGraph->schedule("compute_sum");
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
        }

        // Workflow 3: Compute result = 3 * sum
        {
            auto * workflow = renderGraph->schedule("compute_result");
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
        }

        // Submit all scheduled workflows for execution
        auto submission = renderGraph->submit({});
        TS_ASSERT(submission != nullptr);

        // Wait for completion and get result
        auto submissionResult = submission->result();
        TS_ASSERT_EQUALS(submissionResult.executionResult, GN::rdg::Action::PASSED);

        // Dump submission state to stdout
        GN::StrA dumpStr = submission->dumpState();
        fprintf(stdout, "%s", dumpStr.c_str());
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

        GN::rdg::Workflow * w1 = renderGraph->schedule("w1");
        GN::rdg::Workflow * w2 = renderGraph->schedule("w2");
        GN::rdg::Workflow * w3 = renderGraph->schedule("w3");
        TS_ASSERT(w1 != nullptr);
        TS_ASSERT(w2 != nullptr);
        TS_ASSERT(w3 != nullptr);
        TS_ASSERT(w1->sequence < w2->sequence);
        TS_ASSERT(w2->sequence < w3->sequence);
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
