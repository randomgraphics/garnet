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

// Define a custom artifact that holds an integer
struct IntegerArtifact : public Artifact {
    static inline const Guid TYPE = {0x12345678, 0xabcd, 0xef01, {0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01}};

    int value = 0;

    IntegerArtifact(ArtifactDatabase & db, const Artifact::Identification & id, uint64_t seq): Artifact(db, id, seq) {}
};

// Define an action to initialize an integer artifact
struct InitIntegerAction : public Action {
    inline static constexpr Guid TYPE = {0x11111111, 0x2222, 0x3333, {0x44, 0x44, 0x55, 0x55, 0x66, 0x66, 0x77, 0x77}};

    int initValue = 0;

    InitIntegerAction(ArtifactDatabase & db, const Artifact::Identification & id, uint64_t seq): Action(db, id, seq) {}

    struct A : public Arguments {
        inline static constexpr Guid        TYPE = {0x11111112, 0x2222, 0x3333, {0x44, 0x44, 0x55, 0x55, 0x66, 0x66, 0x77, 0x77}};
        WriteOnly<AutoRef<IntegerArtifact>> output;
        A(ArtifactDatabase & db, const Artifact::Identification & id, uint64_t seq): Arguments(db, id, seq) {}
    };

    // Execute: set the output artifact's value
    ExecutionResult execute(Arguments & args) override {
        auto * a = args.castTo<A>();
        TS_ASSERT(a != nullptr); // null means wrong type
        auto * outputArtifact = a->output.get();
        TS_ASSERT(outputArtifact != nullptr);
        auto output = outputArtifact->get();
        TS_ASSERT(output != nullptr);

        auto integerArtifact = output->castTo<IntegerArtifact>();
        TS_ASSERT(integerArtifact != nullptr);

        GN_INFO(GN::getLogger("GN.rdg.test"))("InitIntegerAction: initializing integer output to {}", initValue);
        integerArtifact->value = initValue;
        return PASSED;
    }
};

// Define an action to add two integers
struct AddIntegersAction : public Action {
    inline static constexpr Guid TYPE = {0xaaaaaaaa, 0xbbbb, 0xcccc, {0xdd, 0xdd, 0xee, 0xee, 0xff, 0xff, 0x00, 0x00}};

    AddIntegersAction(ArtifactDatabase & db, const Artifact::Identification & id, uint64_t seq): Action(db, id, seq) {}

    struct A : public Arguments {
        inline static constexpr Guid        TYPE = {0xaaaaaaab, 0xbbbb, 0xcccc, {0xdd, 0xdd, 0xee, 0xee, 0xff, 0xff, 0x00, 0x00}};
        ReadOnly<AutoRef<IntegerArtifact>>  input1;
        ReadOnly<AutoRef<IntegerArtifact>>  input2;
        WriteOnly<AutoRef<IntegerArtifact>> output;
        A(ArtifactDatabase & db, const Artifact::Identification & id, uint64_t seq): Arguments(db, id, seq) {}
    };

    // Execute: input1.value + input2.value -> output.value
    ExecutionResult execute(Arguments & args) override {
        auto * a = args.castTo<A>();
        TS_ASSERT(a != nullptr); // null means wrong type
        auto * input1Artifact = a->input1.get();
        auto * input2Artifact = a->input2.get();
        auto * outputArtifact = a->output.get();

        TS_ASSERT(input1Artifact != nullptr);
        TS_ASSERT(input2Artifact != nullptr);
        TS_ASSERT(outputArtifact != nullptr);

        auto input1 = input1Artifact->get();
        auto input2 = input2Artifact->get();
        auto output = outputArtifact->get();

        TS_ASSERT(input1 != nullptr);
        TS_ASSERT(input2 != nullptr);
        TS_ASSERT(output != nullptr);

        auto integerInput1 = input1->castTo<IntegerArtifact>();
        auto integerInput2 = input2->castTo<IntegerArtifact>();
        auto integerOutput = output->castTo<IntegerArtifact>();

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
    inline static constexpr Guid TYPE = {0x55555555, 0x6666, 0x7777, {0x88, 0x88, 0x99, 0x99, 0xaa, 0xaa, 0xbb, 0xbb}};

    MultiplyIntegersAction(ArtifactDatabase & db, const Artifact::Identification & id, uint64_t seq): Action(db, id, seq) {}

    struct A : public Arguments {
        inline static constexpr Guid        TYPE = {0x55555556, 0x6666, 0x7777, {0x88, 0x88, 0x99, 0x99, 0xaa, 0xaa, 0xbb, 0xbb}};
        ReadOnly<AutoRef<IntegerArtifact>>  input1;
        ReadOnly<AutoRef<IntegerArtifact>>  input2;
        WriteOnly<AutoRef<IntegerArtifact>> output;
        A(ArtifactDatabase & db, const Artifact::Identification & id, uint64_t seq): Arguments(db, id, seq) {}
    };

    // Execute: input1.value * input2.value -> output.value
    ExecutionResult execute(Arguments & args) override {
        auto * a = args.castTo<A>();
        TS_ASSERT(a != nullptr); // null means wrong type
        auto * input1Artifact = a->input1.get();
        auto * input2Artifact = a->input2.get();
        auto * outputArtifact = a->output.get();

        TS_ASSERT(input1Artifact != nullptr);
        TS_ASSERT(input2Artifact != nullptr);
        TS_ASSERT(outputArtifact != nullptr);

        auto input1 = input1Artifact->get();
        auto input2 = input2Artifact->get();
        auto output = outputArtifact->get();

        TS_ASSERT(input1 != nullptr);
        TS_ASSERT(input2 != nullptr);
        TS_ASSERT(output != nullptr);

        auto integerInput1 = input1->castTo<IntegerArtifact>();
        auto integerInput2 = input2->castTo<IntegerArtifact>();
        auto integerOutput = output->castTo<IntegerArtifact>();

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

// Template helper function to spawn a typed artifact
template<typename T>
GN::AutoRef<T> spawnTyped(GN::rdg::ArtifactDatabase & db, const GN::StrA & name) {
    auto a = db.spawn({T::TYPE, name});
    TS_ASSERT(a != nullptr);
    auto t = a->template castTo<T>();
    TS_ASSERT(t != nullptr);
    return GN::AutoRef<T>(t);
}

class RenderGraphTest : public CxxTest::TestSuite {
public:
    void testRenderGraphArithmetic() {
        // Create an artifact database
        GN::rdg::ArtifactDatabase::CreateParameters dbParams;
        dbParams.autoAdmitAllBuiltInArtifacts = false; // We'll register our custom types manually
        auto db                               = GN::rdg::ArtifactDatabase::create(dbParams);
        TS_ASSERT(db != nullptr);

        // Register custom artifact type
        bool artifactRegistered = db->admit(GN::rdg::IntegerArtifact::TYPE, [db](const GN::StrA & name, uint64_t sequence) -> GN::AutoRef<GN::rdg::Artifact> {
            return GN::AutoRef<GN::rdg::Artifact>(
                new GN::rdg::IntegerArtifact(*db, GN::rdg::Artifact::Identification {GN::rdg::IntegerArtifact::TYPE, name}, sequence));
        });
        TS_ASSERT(artifactRegistered);

        // Register custom action types
        bool initActionRegistered =
            db->admit(GN::rdg::InitIntegerAction::TYPE, [db](const GN::StrA & name, uint64_t sequence) -> GN::AutoRef<GN::rdg::Artifact> {
                return GN::AutoRef<GN::rdg::Artifact>(
                    new GN::rdg::InitIntegerAction(*db, GN::rdg::Artifact::Identification {GN::rdg::InitIntegerAction::TYPE, name}, sequence));
            });
        TS_ASSERT(initActionRegistered);

        bool addActionRegistered =
            db->admit(GN::rdg::AddIntegersAction::TYPE, [db](const GN::StrA & name, uint64_t sequence) -> GN::AutoRef<GN::rdg::Artifact> {
                return GN::AutoRef<GN::rdg::Artifact>(
                    new GN::rdg::AddIntegersAction(*db, GN::rdg::Artifact::Identification {GN::rdg::AddIntegersAction::TYPE, name}, sequence));
            });
        TS_ASSERT(addActionRegistered);

        bool multiplyActionRegistered =
            db->admit(GN::rdg::MultiplyIntegersAction::TYPE, [db](const GN::StrA & name, uint64_t sequence) -> GN::AutoRef<GN::rdg::Artifact> {
                return GN::AutoRef<GN::rdg::Artifact>(
                    new GN::rdg::MultiplyIntegersAction(*db, GN::rdg::Artifact::Identification {GN::rdg::MultiplyIntegersAction::TYPE, name}, sequence));
            });
        TS_ASSERT(multiplyActionRegistered);

        // Register Arguments types for actions
        bool initArgsRegistered =
            db->admit(GN::rdg::InitIntegerAction::A::TYPE, [db](const GN::StrA & name, uint64_t sequence) -> GN::AutoRef<GN::rdg::Artifact> {
                return GN::AutoRef<GN::rdg::Artifact>(
                    new GN::rdg::InitIntegerAction::A(*db, GN::rdg::Artifact::Identification {GN::rdg::InitIntegerAction::A::TYPE, name}, sequence));
            });
        TS_ASSERT(initArgsRegistered);

        bool addArgsRegistered =
            db->admit(GN::rdg::AddIntegersAction::A::TYPE, [db](const GN::StrA & name, uint64_t sequence) -> GN::AutoRef<GN::rdg::Artifact> {
                return GN::AutoRef<GN::rdg::Artifact>(
                    new GN::rdg::AddIntegersAction::A(*db, GN::rdg::Artifact::Identification {GN::rdg::AddIntegersAction::A::TYPE, name}, sequence));
            });
        TS_ASSERT(addArgsRegistered);

        bool multiplyArgsRegistered =
            db->admit(GN::rdg::MultiplyIntegersAction::A::TYPE, [db](const GN::StrA & name, uint64_t sequence) -> GN::AutoRef<GN::rdg::Artifact> {
                return GN::AutoRef<GN::rdg::Artifact>(
                    new GN::rdg::MultiplyIntegersAction::A(*db, GN::rdg::Artifact::Identification {GN::rdg::MultiplyIntegersAction::A::TYPE, name}, sequence));
            });
        TS_ASSERT(multiplyArgsRegistered);

        // Create a render graph instance
        GN::rdg::RenderGraph::CreateParameters params;
        auto                                   renderGraph = GN::rdg::RenderGraph::create(params);
        TS_ASSERT(renderGraph != nullptr);

        // Create artifacts using the database
        auto one    = spawnTyped<GN::rdg::IntegerArtifact>(*db, "one");
        auto two    = spawnTyped<GN::rdg::IntegerArtifact>(*db, "two");
        auto three  = spawnTyped<GN::rdg::IntegerArtifact>(*db, "three");
        auto sum    = spawnTyped<GN::rdg::IntegerArtifact>(*db, "sum");
        auto result = spawnTyped<GN::rdg::IntegerArtifact>(*db, "result");

        TS_ASSERT(one != nullptr);
        TS_ASSERT(two != nullptr);
        TS_ASSERT(three != nullptr);
        TS_ASSERT(sum != nullptr);
        TS_ASSERT(result != nullptr);

        // Workflow 1: Initialize values (1, 2, 3)
        {
            auto * workflow = renderGraph->schedule();
            TS_ASSERT(workflow != nullptr);
            workflow->name = "initialize_values";

            // Task 1: Initialize 'one' to 1
            {
                auto initAction = spawnTyped<GN::rdg::InitIntegerAction>(*db, "init_one");
                TS_ASSERT(initAction != nullptr);
                initAction->initValue = 1;

                auto initArgs = spawnTyped<GN::rdg::InitIntegerAction::A>(*db, "init_one_args");
                TS_ASSERT(initArgs != nullptr);
                initArgs->output.set(one);

                GN::rdg::Workflow::Task task;
                task.action    = initAction;
                task.arguments = initArgs;
                workflow->tasks.append(task);
            }

            // Task 2: Initialize 'two' to 2
            {
                auto initAction = spawnTyped<GN::rdg::InitIntegerAction>(*db, "init_two");
                TS_ASSERT(initAction != nullptr);
                initAction->initValue = 2;

                auto initArgs = spawnTyped<GN::rdg::InitIntegerAction::A>(*db, "init_two_args");
                TS_ASSERT(initArgs != nullptr);
                initArgs->output.set(two);

                GN::rdg::Workflow::Task task;
                task.action    = initAction;
                task.arguments = initArgs;
                workflow->tasks.append(task);
            }

            // Task 3: Initialize 'three' to 3
            {
                auto initAction = spawnTyped<GN::rdg::InitIntegerAction>(*db, "init_three");
                TS_ASSERT(initAction != nullptr);
                initAction->initValue = 3;

                auto initArgs = spawnTyped<GN::rdg::InitIntegerAction::A>(*db, "init_three_args");
                TS_ASSERT(initArgs != nullptr);
                initArgs->output.set(three);

                GN::rdg::Workflow::Task task;
                task.action    = initAction;
                task.arguments = initArgs;
                workflow->tasks.append(task);
            }
        }

        // Workflow 2: Compute sum = 1 + 2
        {
            auto * workflow = renderGraph->schedule();
            TS_ASSERT(workflow != nullptr);
            workflow->name = "compute_sum";

            auto addAction = spawnTyped<GN::rdg::AddIntegersAction>(*db, "add_1_2");
            TS_ASSERT(addAction != nullptr);

            auto addArgs = spawnTyped<GN::rdg::AddIntegersAction::A>(*db, "add_1_2_args");
            TS_ASSERT(addArgs != nullptr);
            addArgs->input1.set(one);
            addArgs->input2.set(two);
            addArgs->output.set(sum);

            GN::rdg::Workflow::Task task;
            task.action    = addAction;
            task.arguments = addArgs;
            workflow->tasks.append(task);
        }

        // Workflow 3: Compute result = 3 * sum
        {
            auto * workflow = renderGraph->schedule();
            TS_ASSERT(workflow != nullptr);
            workflow->name = "compute_result";

            auto multiplyAction = spawnTyped<GN::rdg::MultiplyIntegersAction>(*db, "multiply_3_sum");
            TS_ASSERT(multiplyAction != nullptr);

            auto multiplyArgs = spawnTyped<GN::rdg::MultiplyIntegersAction::A>(*db, "multiply_3_sum_args");
            TS_ASSERT(multiplyArgs != nullptr);
            multiplyArgs->input1.set(three);
            multiplyArgs->input2.set(sum);
            multiplyArgs->output.set(result);

            GN::rdg::Workflow::Task task;
            task.action    = multiplyAction;
            task.arguments = multiplyArgs;
            workflow->tasks.append(task);
        }

        // Submit all scheduled workflows for execution
        GN::rdg::Submission::Parameters execParams;
        execParams.debug = false;
        auto submission  = renderGraph->submit(execParams);
        TS_ASSERT(submission != nullptr);

        // Wait for completion and get result
        auto execResult = submission->result();
        TS_ASSERT_EQUALS(execResult.result, GN::rdg::Action::PASSED);

        // Verify result
        TS_ASSERT_EQUALS(result->value, 9);

        // Verify intermediate values are correct
        TS_ASSERT_EQUALS(one->value, 1);
        TS_ASSERT_EQUALS(two->value, 2);
        TS_ASSERT_EQUALS(three->value, 3);
        TS_ASSERT_EQUALS(sum->value, 3);
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
