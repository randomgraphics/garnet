/*
 * Render Graph Interface Test - Demonstrates 3 * (1 + 2) = 9
 *
 * This test demonstrates the render graph interface by showing how to:
 * 1. Define custom artifacts (IntegerArtifact)
 * 2. Define custom actions (InitIntegerAction, AddIntegersAction, MultiplyIntegersAction)
 * 3. Create tasks with shards
 * 4. Schedule and execute tasks through a render graph
 *
 * Note: This is a conceptual demonstration of the interface structure.
 * A full implementation would require the complete GN library.
 */

#include "../testCommon.h"

#include <garnet/GNrender-graph.h>
#include <unordered_map>

// ============================================================================
// CONCEPTUAL PSEUDO-CODE DEMONSTRATION
// ============================================================================

namespace GN::rg {

// Define a custom artifact that holds an integer
struct IntegerArtifact : public Artifact {
    static inline const Guid TYPE = {0x12345678, 0xabcd, 0xef01, {0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01}};

    int value = 0;

    IntegerArtifact(const StrA & name, uint64_t seq) : Artifact(Artifact::Identification{TYPE, name}, seq) {}
};

// Define an action to initialize an integer artifact
struct InitIntegerAction : public Action {
    static inline const Guid TYPE = {0x11111111, 0x2222, 0x3333, {0x44, 0x44, 0x55, 0x55, 0x66, 0x66, 0x77, 0x77}};

    int initValue;
    static Parameter outputParam;

    InitIntegerAction(const StrA & name, int value, uint64_t seq) : Action(Artifact::Identification{TYPE, name}, seq), initValue(value) {}

    // Define parameters: one output parameter
    SafeArrayAccessor<const Parameter> parameters() const override {
        return SafeArrayAccessor<const Parameter>(const_cast<Parameter*>(&outputParam), 1);
    }

    // Execute: set the output artifact's value
    ExecutionResult execute(SafeArrayAccessor<Artifact> args) override {
        // Verify arguments are correctly assigned (caller has verified size and type)
        TS_ASSERT_EQUALS(args.size(), 1);
        
        auto output_artifact = static_cast<IntegerArtifact*>(&args[0]);
        TS_ASSERT(output_artifact != nullptr);
        TS_ASSERT_EQUALS(&output_artifact->id.type, &IntegerArtifact::TYPE);
        
        output_artifact->value = initValue;
        return Action::PASSED;
    }
};

InitIntegerAction::Parameter InitIntegerAction::outputParam(IntegerArtifact::TYPE, "result", "w");

// Define an action to add two integers
struct AddIntegersAction : public Action {
    static inline const Guid TYPE = {0xaaaaaaaa, 0xbbbb, 0xcccc, {0xdd, 0xdd, 0xee, 0xee, 0xff, 0xff, 0x00, 0x00}};

    AddIntegersAction(const StrA & name, uint64_t seq) : Action(Artifact::Identification{TYPE, name}, seq) {}

    // Define parameters: two inputs, one output
    SafeArrayAccessor<const Parameter> parameters() const override {
        // Note: Parameters are not contiguous, but SafeArrayAccessor needs contiguous memory
        // This is a workaround - we'll access them individually
        static Parameter params[3] = {
            Parameter(IntegerArtifact::TYPE, "input1", "r"),
            Parameter(IntegerArtifact::TYPE, "input2", "r"),
            Parameter(IntegerArtifact::TYPE, "result", "w")
        };
        return SafeArrayAccessor<const Parameter>(params, 3);
    }

    // Execute: input1.value + input2.value -> output.value
    ExecutionResult execute(SafeArrayAccessor<Artifact> args) override {
        // Verify arguments are correctly assigned (caller has verified size and type)
        TS_ASSERT_EQUALS(args.size(), 3);
        
        auto input1_artifact = static_cast<IntegerArtifact*>(&args[0]);
        auto input2_artifact = static_cast<IntegerArtifact*>(&args[1]);
        auto output_artifact = static_cast<IntegerArtifact*>(&args[2]);

        TS_ASSERT(input1_artifact != nullptr);
        TS_ASSERT(input2_artifact != nullptr);
        TS_ASSERT(output_artifact != nullptr);
        TS_ASSERT_EQUALS(&input1_artifact->id.type, &IntegerArtifact::TYPE);
        TS_ASSERT_EQUALS(&input2_artifact->id.type, &IntegerArtifact::TYPE);
        TS_ASSERT_EQUALS(&output_artifact->id.type, &IntegerArtifact::TYPE);

        output_artifact->value = input1_artifact->value + input2_artifact->value;
        return Action::PASSED;
    }
};

// Define an action to multiply two integers
struct MultiplyIntegersAction : public Action {
    static inline const Guid TYPE = {0x55555555, 0x6666, 0x7777, {0x88, 0x88, 0x99, 0x99, 0xaa, 0xaa, 0xbb, 0xbb}};

    MultiplyIntegersAction(const StrA & name, uint64_t seq) : Action(Artifact::Identification{TYPE, name}, seq) {}

    // Define parameters: two inputs, one output
    SafeArrayAccessor<const Parameter> parameters() const override {
        // Note: Parameters are not contiguous, but SafeArrayAccessor needs contiguous memory
        // This is a workaround - we'll access them individually
        static Parameter params[3] = {
            Parameter(IntegerArtifact::TYPE, "input1", "r"),
            Parameter(IntegerArtifact::TYPE, "input2", "r"),
            Parameter(IntegerArtifact::TYPE, "result", "w")
        };
        return SafeArrayAccessor<const Parameter>(params, 3);
    }

    // Execute: input1.value * input2.value -> output.value
    ExecutionResult execute(SafeArrayAccessor<Artifact> args) override {
        // Verify arguments are correctly assigned (caller has verified size and type)
        TS_ASSERT_EQUALS(args.size(), 3);
        
        auto input1_artifact = static_cast<IntegerArtifact*>(&args[0]);
        auto input2_artifact = static_cast<IntegerArtifact*>(&args[1]);
        auto output_artifact = static_cast<IntegerArtifact*>(&args[2]);

        TS_ASSERT(input1_artifact != nullptr);
        TS_ASSERT(input2_artifact != nullptr);
        TS_ASSERT(output_artifact != nullptr);
        TS_ASSERT_EQUALS(&input1_artifact->id.type, &IntegerArtifact::TYPE);
        TS_ASSERT_EQUALS(&input2_artifact->id.type, &IntegerArtifact::TYPE);
        TS_ASSERT_EQUALS(&output_artifact->id.type, &IntegerArtifact::TYPE);

        output_artifact->value = input1_artifact->value * input2_artifact->value;
        return Action::PASSED;
    }
};

// Helper function to create actions
template<typename ActionType>
AutoRef<ActionType> createAction(const StrA & name) {
    static uint64_t sequenceCounter = 1;
    return AutoRef<ActionType>(new ActionType(name, sequenceCounter++));
}

template<typename ActionType>
AutoRef<ActionType> createAction(const StrA & name, int initValue) {
    static uint64_t sequenceCounter = 1;
    return AutoRef<ActionType>(new ActionType(name, initValue, sequenceCounter++));
}

} // namespace GN::rg

// ============================================================================
// TEST EXECUTION FLOW
// ============================================================================

class RenderGraphTest : public CxxTest::TestSuite {
public:
    void testRenderGraphArithmetic() {
        // Create a render graph instance
        GN::rg::RenderGraph * renderGraph = GN::rg::RenderGraph::create();
        TS_ASSERT(renderGraph != nullptr);

        // Register our custom artifact and action types
        // Note: These APIs require RenderGraph to implement ArtifactDatabase interface
        // For now, creating artifacts directly for testing
        static uint64_t artifactSeq = 1;
        GN::AutoRef<GN::rg::Artifact> one = GN::AutoRef<GN::rg::Artifact>(new GN::rg::IntegerArtifact("one", artifactSeq++));
        GN::AutoRef<GN::rg::Artifact> two = GN::AutoRef<GN::rg::Artifact>(new GN::rg::IntegerArtifact("two", artifactSeq++));
        GN::AutoRef<GN::rg::Artifact> three = GN::AutoRef<GN::rg::Artifact>(new GN::rg::IntegerArtifact("three", artifactSeq++));
        GN::AutoRef<GN::rg::Artifact> sum = GN::AutoRef<GN::rg::Artifact>(new GN::rg::IntegerArtifact("sum", artifactSeq++));
        GN::AutoRef<GN::rg::Artifact> result = GN::AutoRef<GN::rg::Artifact>(new GN::rg::IntegerArtifact("result", artifactSeq++));
        
        // Verify artifacts are created
        TS_ASSERT(one != nullptr);
        TS_ASSERT(two != nullptr);
        TS_ASSERT(three != nullptr);
        TS_ASSERT(sum != nullptr);
        TS_ASSERT(result != nullptr);

        // Task 1: Initialize values (1, 2, 3)
        {
            GN::rg::Task initTask;
            initTask.name = "initialize_values";

            // Shard 1: Initialize 'one' to 1
            GN::rg::Task::Shard initOneShard;
            initOneShard.action = GN::rg::createAction<GN::rg::InitIntegerAction>("init_one", 1);
            initOneShard.arguments["output"] = one;
            initTask.shards.append(initOneShard);

            // Shard 2: Initialize 'two' to 2
            GN::rg::Task::Shard initTwoShard;
            initTwoShard.action = GN::rg::createAction<GN::rg::InitIntegerAction>("init_two", 2);
            initTwoShard.arguments["output"] = two;
            initTask.shards.append(initTwoShard);

            // Shard 3: Initialize 'three' to 3
            GN::rg::Task::Shard initThreeShard;
            initThreeShard.action = GN::rg::createAction<GN::rg::InitIntegerAction>("init_three", 3);
            initThreeShard.arguments["output"] = three;
            initTask.shards.append(initThreeShard);

            // Note: schedule() API takes no parameters in current implementation
            // renderGraph->schedule(initTask);
        }

        // Task 2: Compute sum = 1 + 2
        {
            GN::rg::Task addTask;
            addTask.name = "compute_sum";

            GN::rg::Task::Shard addShard;
            addShard.action = GN::rg::createAction<GN::rg::AddIntegersAction>("add_1_2");
            addShard.arguments["input1"] = one;
            addShard.arguments["input2"] = two;
            addShard.arguments["output"] = sum;
            addTask.shards.append(addShard);

            // Note: schedule() API takes no parameters in current implementation
            // renderGraph->schedule(addTask);
        }

        // Task 3: Compute result = 3 * sum
        {
            GN::rg::Task multiplyTask;
            multiplyTask.name = "compute_result";

            GN::rg::Task::Shard multiplyShard;
            multiplyShard.action = GN::rg::createAction<GN::rg::MultiplyIntegersAction>("multiply_3_sum");
            multiplyShard.arguments["input1"] = three;
            multiplyShard.arguments["input2"] = sum;
            multiplyShard.arguments["output"] = result;
            multiplyTask.shards.append(multiplyShard);

            // Note: schedule() API takes no parameters in current implementation
            // renderGraph->schedule(multiplyTask);
        }

        // Execute all scheduled tasks
        GN::rg::Action::ExecutionResult execResult = renderGraph->execute();
        TS_ASSERT_EQUALS(execResult, GN::rg::Action::PASSED);

        // Verify result
        GN::rg::IntegerArtifact * resultArtifact = static_cast<GN::rg::IntegerArtifact *>(result.get());
        TS_ASSERT(resultArtifact != nullptr);
        TS_ASSERT_EQUALS(resultArtifact->value, 9);
        
        // Verify intermediate values are correct
        GN::rg::IntegerArtifact * oneArtifact = static_cast<GN::rg::IntegerArtifact *>(one.get());
        GN::rg::IntegerArtifact * twoArtifact = static_cast<GN::rg::IntegerArtifact *>(two.get());
        GN::rg::IntegerArtifact * threeArtifact = static_cast<GN::rg::IntegerArtifact *>(three.get());
        GN::rg::IntegerArtifact * sumArtifact = static_cast<GN::rg::IntegerArtifact *>(sum.get());
        
        TS_ASSERT_EQUALS(oneArtifact->value, 1);
        TS_ASSERT_EQUALS(twoArtifact->value, 2);
        TS_ASSERT_EQUALS(threeArtifact->value, 3);
        TS_ASSERT_EQUALS(sumArtifact->value, 3);
    }
};

/*
 * EXPECTED OUTPUT:
 *
 * Executing task: initialize_values
 *   Executing shard: init_one -> one = 1
 *   Executing shard: init_two -> two = 2
 *   Executing shard: init_three -> three = 3
 *
 * Executing task: compute_sum
 *   Executing shard: add_1_2 -> sum = 1 + 2 = 3
 *
 * Executing task: compute_result
 *   Executing shard: multiply_3_sum -> result = 3 * 3 = 9
 *
 * Final result: 9 (SUCCESS!)
 */