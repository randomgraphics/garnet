#include <garnet/GNrdg.h>
#include <unordered_map>
#include <mutex>
#include <unordered_set>
#include <cstddef>
#include <limits>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// ============================================================================
// ArtifactDatabase implementation
// ============================================================================

// Hash function for Artifact::Identification
struct IdentificationHash {
    size_t operator()(const Artifact::Identification & id) const {
        // Combine hash of Guid and name
        Guid::Hash      guidHash;
        std::hash<StrA> nameHash;
        return guidHash(id.type) ^ (nameHash(id.name) << 1);
    }
};

// Equality function for Artifact::Identification
struct IdentificationEqual {
    bool operator()(const Artifact::Identification & a, const Artifact::Identification & b) const { return &a.type == &b.type && a.name == b.name; }
};

class ArtifactDatabaseImpl : public ArtifactDatabase {
    // Map from artifact type GUID to creator function
    std::unordered_map<Guid, std::function<AutoRef<Artifact>(const StrA &, uint64_t)>, Guid::Hash> mCreators;

    // Map from Identification to artifact (for search by Identification)
    std::unordered_map<Artifact::Identification, AutoRef<Artifact>, IdentificationHash, IdentificationEqual> mArtifactsById;

    // Map from sequence number to artifact (for fast lookup by sequence)
    std::unordered_map<uint64_t, AutoRef<Artifact>> mArtifactsBySeq;

    // Sequence number counter
    uint64_t mNextSequence = 1;

    // Mutex for thread safety
    mutable std::mutex mMutex;

public:
    ArtifactDatabaseImpl()  = default;
    ~ArtifactDatabaseImpl() = default;

    bool admit(const Guid & type, std::function<AutoRef<Artifact>(const StrA & name, uint64_t sequence)> && creator) override {
        std::lock_guard<std::mutex> lock(mMutex);

        // Check if type is already registered
        if (mCreators.find(type) != mCreators.end()) {
            // Type already registered, redundant registration not allowed
            GN_ERROR(sLogger)("Artifact type already registered: {}", type.toStr());
            return false;
        }

        mCreators[type] = std::move(creator);
        return true;
    }

    auto spawn(const Artifact::Identification & id) -> AutoRef<Artifact> override {
        std::lock_guard<std::mutex> lock(mMutex);

        // Check if artifact already exists
        auto it = mArtifactsById.find(id);
        if (it != mArtifactsById.end()) {
            // ID already exists, fail
            GN_ERROR(sLogger)("Failed to spawn artifact: artifact already exists: type={}, name={}", id.type.toStr(), id.name);
            return AutoRef<Artifact>();
        }

        // Find creator for this type
        auto creatorIt = mCreators.find(id.type);
        if (creatorIt == mCreators.end()) {
            // No creator registered for this type
            GN_ERROR(sLogger)("Failed to spawn artifact: type not registered: type={}, name={}", id.type.toStr(), id.name);
            return AutoRef<Artifact>();
        }

        // Create new artifact with next sequence number
        uint64_t          seq      = mNextSequence++;
        AutoRef<Artifact> artifact = creatorIt->second(id.name, seq);

        if (!artifact) {
            // Creator returned null
            GN_ERROR(sLogger)("Failed to spawn artifact: creator returned null: type={}, name={}", id.type.toStr(), id.name);
            return AutoRef<Artifact>();
        }

        // Store in both maps
        mArtifactsById[id]   = artifact;
        mArtifactsBySeq[seq] = artifact;

        return artifact;
    }

    auto fetch(const Artifact::Identification & id) -> AutoRef<Artifact> override {
        std::lock_guard<std::mutex> lock(mMutex);

        auto it = mArtifactsById.find(id);
        if (it != mArtifactsById.end()) { return it->second; }
        return AutoRef<Artifact>();
    }

    auto fetch(uint64_t sequence) -> AutoRef<Artifact> override {
        std::lock_guard<std::mutex> lock(mMutex);

        auto it = mArtifactsBySeq.find(sequence);
        if (it != mArtifactsBySeq.end()) { return it->second; }
        return AutoRef<Artifact>();
    }

    bool erase(const Artifact::Identification & id) override {
        std::lock_guard<std::mutex> lock(mMutex);

        auto it = mArtifactsById.find(id);
        if (it == mArtifactsById.end()) {
            // Artifact not found
            return false;
        }

        // Get the sequence number before erasing
        uint64_t seq = it->second->sequence;

        // Erase from both maps
        mArtifactsById.erase(it);
        mArtifactsBySeq.erase(seq);

        return true;
    }

    bool erase(uint64_t sequence) override {
        std::lock_guard<std::mutex> lock(mMutex);

        auto it = mArtifactsBySeq.find(sequence);
        if (it == mArtifactsBySeq.end()) {
            // Artifact not found
            return false;
        }

        // Get the identification before erasing
        const Artifact::Identification & id = it->second->id;

        // Erase from both maps
        mArtifactsBySeq.erase(it);
        mArtifactsById.erase(id);

        return true;
    }
};

ArtifactDatabase * ArtifactDatabase::create(const CreateParameters & params) {
    auto impl = std::make_unique<ArtifactDatabaseImpl>();

    // If autoAdmitAllBuiltInArtifacts is true, register all built-in artifact types
    if (params.autoAdmitAllBuiltInArtifacts) {
        // TODO: Register built-in artifact types here
        // For now, this is a placeholder
    }

    return impl.release();
}

// ============================================================================
// RenderGraph implementation
// ============================================================================

class RenderGraphImpl : public RenderGraph {
    // Pending workflows waiting to be executed
    DynaArray<Workflow *> mPendingWorkflows;

    // Mutex for thread safety
    mutable std::mutex mScheduleMutex, mExecuteMutex;

    // Validated workflows (after validation)
    DynaArray<Workflow *> mValidatedWorkflows;

    // Dependency graph: for each workflow index, stores indices of workflows it depends on
    DynaArray<DynaArray<size_t>> mDependencyGraph;

    // Validate a single task: check if arguments match action
    bool validateTask(const Workflow::Task & task, const StrA & workflowName, size_t taskIndex) {
        if (!task.action) {
            GN_ERROR(sLogger)("Workflow '{}' task {}: action is null", workflowName, taskIndex);
            return false;
        }

        if (!task.arguments) {
            GN_ERROR(sLogger)("Workflow '{}' task {}: arguments is null", workflowName, taskIndex);
            return false;
        }

        // Arguments must be of the correct type for the action
        // The action's execute() method will validate this via castTo<A>()
        return true;
    }

    // Validate all workflows and build dependency graph
    bool validateAndBuildDependencyGraph() {
        mValidatedWorkflows.clear();
        mDependencyGraph.clear();

        // First pass: validate all workflows and tasks
        for (size_t workflowIdx = 0; workflowIdx < mPendingWorkflows.size(); ++workflowIdx) {
            Workflow * workflow = mPendingWorkflows[workflowIdx];
            GN_ASSERT(workflow);

            // Validate all tasks in this workflow
            for (size_t taskIdx = 0; taskIdx < workflow->tasks.size(); ++taskIdx) {
                const Workflow::Task & task = workflow->tasks[taskIdx];
                if (!validateTask(task, workflow->name, taskIdx)) { return false; }
            }

            // Workflow is valid, add to validated list
            mValidatedWorkflows.append(workflow);
        }

        // Second pass: build workflow-level dependency graph
        // Tasks within a workflow are executed sequentially as part of the workflow
        // Resize dependency graph to match number of validated workflows
        mDependencyGraph.resize(mValidatedWorkflows.size());

        for (size_t workflowIdx = 0; workflowIdx < mValidatedWorkflows.size(); ++workflowIdx) {
            Workflow * workflow = mValidatedWorkflows[workflowIdx];

            // Add workflow-level dependencies
            for (Workflow * depWorkflow : workflow->dependencies) {
                // Find the index of the dependency workflow
                size_t depWorkflowIdx = std::numeric_limits<size_t>::max();
                for (size_t i = 0; i < mValidatedWorkflows.size(); ++i) {
                    if (mValidatedWorkflows[i] == depWorkflow) {
                        depWorkflowIdx = i;
                        break;
                    }
                }

                if (depWorkflowIdx == std::numeric_limits<size_t>::max()) {
                    GN_ERROR(sLogger)("Workflow '{}' depends on workflow that is not in the validated workflow list", workflow->name);
                    return false;
                }

                // Store dependency as workflow index
                mDependencyGraph[workflowIdx].append(depWorkflowIdx);
            }
        }

        return true;
    }

    bool compileWorkflowGraph() {
        std::lock_guard<std::mutex> lock(mScheduleMutex);

        // Part 1: Validate workflows and build dependency graph
        if (!validateAndBuildDependencyGraph()) {
            GN_ERROR(sLogger)("Workflow validation failed");
            return false;
        }

        // Clear pending workflows after validation (they're now in mValidatedWorkflows)
        mPendingWorkflows.clear();

        return true;
    }

    // Perform topological sort to determine execution order
    // Returns sorted workflow indices, or empty array if cycle detected
    DynaArray<size_t> topologicalSort() {
        DynaArray<size_t> result;

        // Calculate in-degree for each workflow (number of workflows that depend on it)
        // Build reverse graph: for each workflow, which workflows depend on it
        DynaArray<DynaArray<size_t>> reverseGraph;
        reverseGraph.resize(mValidatedWorkflows.size());

        for (size_t i = 0; i < mDependencyGraph.size(); ++i) {
            for (size_t depIdx : mDependencyGraph[i]) {
                // Workflow i depends on depIdx, so depIdx has workflow i as a dependent
                reverseGraph[depIdx].append(i);
            }
        }

        // Calculate in-degree: number of workflows that depend on this workflow
        DynaArray<size_t> inDegree(mValidatedWorkflows.size(), 0);
        for (size_t i = 0; i < mDependencyGraph.size(); ++i) { inDegree[i] = mDependencyGraph[i].size(); }

        // Find all workflows with no dependencies (in-degree = 0)
        DynaArray<size_t> queue;
        for (size_t i = 0; i < inDegree.size(); ++i) {
            if (inDegree[i] == 0) { queue.append(i); }
        }

        // Process workflows with no dependencies
        while (!queue.empty()) {
            size_t current = queue.front();
            queue.eraseIdx(0);
            result.append(current);

            // Reduce in-degree of workflows that depend on current
            for (size_t dependentIdx : reverseGraph[current]) {
                GN_ASSERT(inDegree[dependentIdx] > 0);
                inDegree[dependentIdx]--;
                if (inDegree[dependentIdx] == 0) { queue.append(dependentIdx); }
            }
        }

        // Check for cycles (if result size != total workflows, there's a cycle)
        if (result.size() != mValidatedWorkflows.size()) {
            GN_ERROR(sLogger)("Circular dependency detected in workflow graph");
            result.clear();
            return result;
        }

        return result;
    }

    // Execute a single task
    Action::ExecutionResult executeTask(const Workflow::Task & task, Workflow * workflow, size_t taskIndex) {
        if (!task.action) {
            GN_ERROR(sLogger)("Workflow '{}' task {}: action is null", workflow->name, taskIndex);
            return Action::ExecutionResult::FAILED;
        }

        if (!task.arguments) {
            GN_ERROR(sLogger)("Workflow '{}' task {}: arguments is null", workflow->name, taskIndex);
            return Action::ExecutionResult::FAILED;
        }

        // Execute the action with the arguments
        return task.action->execute(*task.arguments);
    }

    // Execute a single workflow (all its tasks sequentially)
    Action::ExecutionResult executeWorkflow(size_t workflowIndex) {
        Workflow * workflow = mValidatedWorkflows[workflowIndex];
        GN_ASSERT(workflow);

        // Execute all tasks in sequential order
        for (size_t taskIdx = 0; taskIdx < workflow->tasks.size(); ++taskIdx) {
            const Workflow::Task &  task   = workflow->tasks[taskIdx];
            Action::ExecutionResult result = executeTask(task, workflow, taskIdx);

            if (result == Action::ExecutionResult::FAILED) {
                GN_ERROR(sLogger)("Workflow '{}' task {} execution failed", workflow->name, taskIdx);
                return Action::ExecutionResult::FAILED;
            }

            if (result == Action::ExecutionResult::WARNING) {
                GN_WARN(sLogger)("Workflow '{}' task {} execution completed with warnings", workflow->name, taskIdx);
            }
        }

        return Action::ExecutionResult::PASSED;
    }

public:
    RenderGraphImpl() {}
    ~RenderGraphImpl() {}

    Workflow * schedule() override {
        std::lock_guard<std::mutex> lock(mScheduleMutex);

        // Create a new empty workflow
        Workflow * newWorkflow = new Workflow();
        mPendingWorkflows.append(newWorkflow);

        return newWorkflow;
    }

    AutoRef<Submission> submit(const Submission::Parameters & params) override {
        std::lock_guard<std::mutex> lock(mExecuteMutex);

        // Implementation class for Submission
        class SubmissionImpl : public Submission {
            Action::ExecutionResult mResult;
            StrA                    mDebugStats;
            bool                    mFinished;

        public:
            SubmissionImpl(Action::ExecutionResult result, StrA debugStats): mResult(result), mDebugStats(std::move(debugStats)), mFinished(true) {}

            bool isFinished() override { return mFinished; }

            Result result() override {
                Result r;
                r.result     = mResult;
                r.debugStats = mDebugStats;
                return r;
            }
        };

        // step 1: compile the workflow graph, validate workflows and build dependency graph
        if (!compileWorkflowGraph()) {
            auto submission = AutoRef<Submission>(new SubmissionImpl(Action::ExecutionResult::FAILED, "Workflow graph compilation failed"));
            return submission;
        }

        // step 2: execute the workflow graph sequentially based on topological order
        // Perform topological sort to determine execution order
        DynaArray<size_t> executionOrder = topologicalSort();
        if (executionOrder.empty()) {
            GN_ERROR(sLogger)("Failed to determine workflow execution order");
            auto submission =
                AutoRef<Submission>(new SubmissionImpl(Action::ExecutionResult::FAILED, "Topological sort failed - circular dependency detected"));
            return submission;
        }

        // Execute workflows sequentially in topological order
        bool         hasWarning     = false;
        const size_t totalWorkflows = executionOrder.size();

        for (size_t workflowIdx : executionOrder) {
            Action::ExecutionResult result = executeWorkflow(workflowIdx);

            if (result == Action::ExecutionResult::FAILED) {
                GN_ERROR(sLogger)("Workflow '{}' execution failed, stopping execution", mValidatedWorkflows[workflowIdx]->name);
                StrA debugStats = params.debug ? StrA::format("Execution failed at workflow index {}", workflowIdx) : StrA();
                auto submission = AutoRef<Submission>(new SubmissionImpl(Action::ExecutionResult::FAILED, debugStats));
                return submission;
            }

            if (result == Action::ExecutionResult::WARNING) {
                GN_WARN(sLogger)("Workflow '{}' execution completed with warnings", mValidatedWorkflows[workflowIdx]->name);
                hasWarning = true;
            }
        }

        Action::ExecutionResult finalResult = hasWarning ? Action::ExecutionResult::WARNING : Action::ExecutionResult::PASSED;

        if (hasWarning) {
            GN_TRACE(sLogger)("Successfully executed {} workflows sequentially with warnings", totalWorkflows);
        } else {
            GN_TRACE(sLogger)("Successfully executed {} workflows sequentially", totalWorkflows);
        }

        StrA debugStats;
        if (params.debug) {
            // Generate debug statistics
            debugStats = StrA::format("Executed {} workflows successfully", totalWorkflows);
            if (hasWarning) { debugStats += " (with warnings)"; }
        }

        auto submission = AutoRef<Submission>(new SubmissionImpl(finalResult, debugStats));
        return submission;
    }
};

RenderGraph * RenderGraph::create(const CreateParameters & params) {
    (void) params; // unused for now
    return new RenderGraphImpl();
}

} // namespace GN::rdg
