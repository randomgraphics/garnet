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

    static const Parameter output = { IntegerArtifact::TYPE, "result", "w" };

    InitIntegerAction(const StrA & name, int value, uint64_t seq) : Action(Artifact::Identification{TYPE, name}, seq), initValue(value) {}

    // Define parameters: one output parameter
    DynaArray<const Parameter *> parameters() const override {
        return { &output };
    }

    // Execute: set the output artifact's value
    ExecutionResult execute(const std::unordered_map<StrA, AutoRef<Artifact>> & args) override {
        auto output_it = args.find("result");
        if (output_it == args.end()) {
            GN_ASSERT(false, "Output artifact not found");
            return Action::FAILED;
        }

        auto output_artifact = static_cast<IntegerArtifact*>(output_it->second.get());
        output_artifact->value = initValue;
        return Action::PASSED;
    }
};

// Define an action to add two integers
struct AddIntegersAction : public Action {
    static inline const Guid TYPE = {0xaaaaaaaa, 0xbbbb, 0xcccc, {0xdd, 0xdd, 0xee, 0xee, 0xff, 0xff, 0x00, 0x00}};

    static const Parameter input1 = { IntegerArtifact::TYPE, "input1", "r" };
    static const Parameter input2 = { IntegerArtifact::TYPE, "input2", "r" };
    static const Parameter output = { IntegerArtifact::TYPE, "result", "w" };

    AddIntegersAction(const StrA & name, uint64_t seq) : Action(Artifact::Identification{TYPE, name}, seq) {}

    // Define parameters: two inputs, one output
    DynaArray<const Parameter *> parameters() const override {
        return { &input1, &input2, &output };
    }

    // Execute: input1.value + input2.value -> output.value
    ExecutionResult execute(const std::unordered_map<StrA, AutoRef<Artifact>> & args) override {
        auto input1_it = args.find("input1");
        auto input2_it = args.find("input2");
        auto output_it = args.find("result");

        if (input1_it == args.end() || input2_it == args.end() || output_it == args.end()) {
            GN_ASSERT(false, "Required artifacts not found");
            return Action::FAILED;
        }

        auto input1_artifact = static_cast<IntegerArtifact*>(input1_it->second.get());
        auto input2_artifact = static_cast<IntegerArtifact*>(input2_it->second.get());
        auto output_artifact = static_cast<IntegerArtifact*>(output_it->second.get());

        output_artifact->value = input1_artifact->value + input2_artifact->value;
        return Action::PASSED;
    }
};

// Define an action to multiply two integers
struct MultiplyIntegersAction : public Action {
    static inline const Guid TYPE = {0x55555555, 0x6666, 0x7777, {0x88, 0x88, 0x99, 0x99, 0xaa, 0xaa, 0xbb, 0xbb}};

    static const Parameter input1 = { IntegerArtifact::TYPE, "input1", "r" };
    static const Parameter input2 = { IntegerArtifact::TYPE, "input2", "r" };
    static const Parameter output = { IntegerArtifact::TYPE, "result", "w" };

    MultiplyIntegersAction(const StrA & name, uint64_t seq) : Action(Artifact::Identification{TYPE, name}, seq) {}

    // Define parameters: two inputs, one output
    DynaArray<const Parameter *> parameters() const override {
        return { &input1, &input2, &output };
    }

    // Execute: input1.value * input2.value -> output.value
    ExecutionResult execute(const std::unordered_map<StrA, AutoRef<Artifact>> & args) override {
        auto input1_it = args.find("input1");
        auto input2_it = args.find("input2");
        auto output_it = args.find("result");

        if (input1_it == args.end() || input2_it == args.end() || output_it == args.end()) {
            GN_ASSERT(false, "Required artifacts not found");
            return Action::FAILED;
        }

        auto input1_artifact = static_cast<IntegerArtifact*>(input1_it->second.get());
        auto input2_artifact = static_cast<IntegerArtifact*>(input2_it->second.get());
        auto output_artifact = static_cast<IntegerArtifact*>(output_it->second.get());

        output_artifact->value = input1_artifact->value * input2_artifact->value;
        return Action::PASSED;
    }
};

// ============================================================================
// TEST EXECUTION FLOW
// ============================================================================

class RenderGraphTest : public CxxTest::TestSuite {
public:
    void testRenderGraphArithmetic() {
        // Create a render graph instance
        std::unique_ptr<RenderGraph> renderGraph = RenderGraph::create();
        TS_ASSERT(renderGraph != nullptr);

        // Register our custom artifact and action types
    renderGraph->registerArtifactType(IntegerArtifact::TYPE,
        [](const StrA & name, uint64_t sequence) -> AutoRef<Artifact> {
            return new IntegerArtifact(name, sequence);
        });
    renderGraph->registerArtifactType(InitIntegerAction::TYPE,
        [](const StrA & name, uint64_t sequence) -> AutoRef<Artifact> {
            return new InitIntegerAction(name, sequence);
        });
    renderGraph->registerArtifactType(AddIntegersAction::TYPE,
        [](const StrA & name, uint64_t sequence) -> AutoRef<Artifact> {
            return new AddIntegersAction(name, sequence);
        });
    renderGraph->registerArtifactType(MultiplyIntegersAction::TYPE,
        [](const StrA & name, uint64_t sequence) -> AutoRef<Artifact> {
            return new MultiplyIntegersAction(name, sequence);
        });
    // ... register action types ...

    // Create artifacts for our computation: 3 * (1 + 2)
    AutoRef<Artifact> one = renderGraph->create({"one", IntegerArtifact::TYPE});
    AutoRef<Artifact> two = renderGraph->create({"two", IntegerArtifact::TYPE});
    AutoRef<Artifact> three = renderGraph->create({"three", IntegerArtifact::TYPE});
    AutoRef<Artifact> sum = renderGraph->create({"sum", IntegerArtifact::TYPE});
    AutoRef<Artifact> result = renderGraph->create({"result", IntegerArtifact::TYPE});
    
    // Verify artifacts are created
    TS_ASSERT(one != nullptr);
    TS_ASSERT(two != nullptr);
    TS_ASSERT(three != nullptr);
    TS_ASSERT(sum != nullptr);
    TS_ASSERT(result != nullptr);

    // Task 1: Initialize values (1, 2, 3)
    {
        Task initTask{"initialize_values"};

        // Shard 1: Initialize 'one' to 1
        Task::Shard initOneShard;
        initOneShard.action = createAction<InitIntegerAction>("init_one", 1);
        initOneShard.arguments["output"] = one;
        initTask.shards.pushBack(initOneShard);

        // Shard 2: Initialize 'two' to 2
        Task::Shard initTwoShard;
        initTwoShard.action = createAction<InitIntegerAction>("init_two", 2);
        initTwoShard.arguments["output"] = two;
        initTask.shards.pushBack(initTwoShard);

        // Shard 3: Initialize 'three' to 3
        Task::Shard initThreeShard;
        initThreeShard.action = createAction<InitIntegerAction>("init_three", 3);
        initThreeShard.arguments["output"] = three;
        initTask.shards.pushBack(initThreeShard);

        renderGraph->schedule(initTask);
    }

    // Task 2: Compute sum = 1 + 2
    {
        Task addTask{"compute_sum"};

        Task::Shard addShard;
        addShard.action = createAction<AddIntegersAction>("add_1_2");
        addShard.arguments["input1"] = one;
        addShard.arguments["input2"] = two;
        addShard.arguments["output"] = sum;
        addTask.shards.pushBack(addShard);

        renderGraph->schedule(addTask);
    }

    // Task 3: Compute result = 3 * sum
    {
        Task multiplyTask{"compute_result"};

        Task::Shard multiplyShard;
        multiplyShard.action = createAction<MultiplyIntegersAction>("multiply_3_sum");
        multiplyShard.arguments["input1"] = three;
        multiplyShard.arguments["input2"] = sum;
        multiplyShard.arguments["output"] = result;
        multiplyTask.shards.pushBack(multiplyShard);

        renderGraph->schedule(multiplyTask);
    }

    // Execute all scheduled tasks
    Action::ExecutionResult execResult = renderGraph->execute();
    TS_ASSERT_EQUALS(execResult, Action::PASSED);

    // Verify result
    IntegerArtifact * resultArtifact = static_cast<IntegerArtifact *>(result.get());
    TS_ASSERT(resultArtifact != nullptr);
    TS_ASSERT_EQUALS(resultArtifact->value, 9);
    
    // Verify intermediate values are correct
    IntegerArtifact * oneArtifact = static_cast<IntegerArtifact *>(one.get());
    IntegerArtifact * twoArtifact = static_cast<IntegerArtifact *>(two.get());
    IntegerArtifact * threeArtifact = static_cast<IntegerArtifact *>(three.get());
    IntegerArtifact * sumArtifact = static_cast<IntegerArtifact *>(sum.get());
    
    TS_ASSERT_EQUALS(oneArtifact->value, 1);
    TS_ASSERT_EQUALS(twoArtifact->value, 2);
    TS_ASSERT_EQUALS(threeArtifact->value, 3);
    TS_ASSERT_EQUALS(sumArtifact->value, 3);
    }
};

} // namespace GN::rg

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