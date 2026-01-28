#include "../../inc/garnet/GNrender-graph.h"
#include <mutex>
#include <unordered_set>
#include <unordered_map>

static GN::Logger * sLogger = GN::getLogger("GN.rg");

namespace GN::rg {

// Internal Task implementation class that can be instantiated
class TaskImpl : public Task {
public:
    TaskImpl() : Task() {}
    ~TaskImpl() = default;
};


class RenderGraphImpl : public RenderGraph {
    // Pending tasks waiting to be executed
    DynaArray<Task *> mPendingTasks;
    
    // Mutex for thread safety
    mutable std::mutex mScheduleMutex, mExecuteMutex;
    
    // Validated tasks (after validation in execute())
    DynaArray<Task *> mValidatedTasks;
    
    // Dependency graph: for each task index, stores indices of tasks it depends on
    // mDependencyGraph[i] contains the task indices that task i depends on
    DynaArray<DynaArray<size_t>> mDependencyGraph;

    // Validate a single shard: check if argument bindings match action parameters
    bool validateShard(const Task::Shard & shard, const StrA & taskName, size_t shardIndex) {
        if (!shard.action) {
            GN_ERROR(sLogger)("Task '{}' shard {}: action is null", taskName, shardIndex);
            return false;
        }
        
        // Get action parameters as a map
        const std::unordered_map<StrA, const Action::Parameter> & params = shard.action->parameters();
        
        // Check each parameter against the arguments map
        for (const auto & paramPair : params) {
            const StrA & paramName = paramPair.first;
            const Action::Parameter & param = paramPair.second;
            
            // Look up argument by parameter name
            auto argIt = shard.arguments.find(paramName);
            
            if (argIt == shard.arguments.end()) {
                // Argument not provided
                if (!param.optional) {
                    GN_ERROR(sLogger)("Task '{}' shard {}: required parameter '{}' is missing", taskName, shardIndex, paramName);
                    return false;
                }
                // Optional parameter missing - OK
                continue;
            }
            
            // Check type match
            const Artifact * artifact = argIt->second.get();
            if (!artifact) {
                GN_ERROR(sLogger)("Task '{}' shard {}: parameter '{}' has null artifact", taskName, shardIndex, paramName);
                return false;
            }
            
            if (&artifact->id.type != &param.type) {
                GN_ERROR(sLogger)("Task '{}' shard {}: parameter '{}' type mismatch", taskName, shardIndex, paramName);
                return false;
            }
        }
        
        // Check for extra arguments that weren't in the parameter list
        // (This is just a warning, not an error, as it might be intentional)
        for (const auto & argPair : shard.arguments) {
            if (params.find(argPair.first) == params.end()) {
                GN_TRACE(sLogger)("Task '{}' shard {}: extra argument '{}' provided that doesn't match any parameter", taskName, shardIndex, argPair.first);
            }
        }
        
        return true;
    }
    
    // Validate all tasks and build dependency graph
    bool validateAndBuildDependencyGraph() {
        mValidatedTasks.clear();
        mDependencyGraph.clear();
        
        // First pass: validate all tasks and shards
        for (size_t taskIdx = 0; taskIdx < mPendingTasks.size(); ++taskIdx) {
            Task * task = mPendingTasks[taskIdx];
            GN_ASSERT(task);
            
            // Validate all shards in this task
            for (size_t shardIdx = 0; shardIdx < task->shards.size(); ++shardIdx) {
                const Task::Shard & shard = task->shards[shardIdx];
                if (!validateShard(shard, task->name, shardIdx)) {
                    return false;
                }
            }

            // Task is valid, add to validated list
            mValidatedTasks.append(task);
        }
        
        // Second pass: build task-level dependency graph
        // Shards within a task are executed sequentially as part of the task
        // Resize dependency graph to match number of validated tasks
        mDependencyGraph.resize(mValidatedTasks.size());
        
        for (size_t taskIdx = 0; taskIdx < mValidatedTasks.size(); ++taskIdx) {
            Task * task = mValidatedTasks[taskIdx];
            
            // Add task-level dependencies
            for (Task * depTask : task->dependencies) {
                // Find the index of the dependency task
                size_t depTaskIdx = SIZE_MAX;
                for (size_t i = 0; i < mValidatedTasks.size(); ++i) {
                    if (mValidatedTasks[i] == depTask) {
                        depTaskIdx = i;
                        break;
                    }
                }
                
                if (depTaskIdx == SIZE_MAX) {
                    GN_ERROR(sLogger)("Task '{}' depends on task that is not in the validated task list", task->name);
                    return false;
                }
                
                // Store dependency as task index
                mDependencyGraph[taskIdx].append(depTaskIdx);
            }
        }
        
        return true;
    }

    bool compileTaskGraph() {
        std::lock_guard<std::mutex> lock(mScheduleMutex);
        
        // Part 1: Validate tasks and build dependency graph
        if (!validateAndBuildDependencyGraph()) {
            GN_ERROR(sLogger)("Task validation failed");
            return false;
        }
                
        // Clear pending tasks after validation (they're now in mValidatedTasks)
        mPendingTasks.clear();    

        return true;
    }
    
    // Perform topological sort to determine execution order
    // Returns sorted task indices, or empty array if cycle detected
    // mDependencyGraph[i] contains tasks that task i depends on
    DynaArray<size_t> topologicalSort() {
        DynaArray<size_t> result;
        
        // Calculate in-degree for each task (number of tasks that depend on it)
        // Build reverse graph: for each task, which tasks depend on it
        DynaArray<DynaArray<size_t>> reverseGraph;
        reverseGraph.resize(mValidatedTasks.size());
        
        for (size_t i = 0; i < mDependencyGraph.size(); ++i) {
            for (size_t depIdx : mDependencyGraph[i]) {
                // Task i depends on depIdx, so depIdx has task i as a dependent
                reverseGraph[depIdx].append(i);
            }
        }
        
        // Calculate in-degree: number of tasks that depend on this task
        DynaArray<size_t> inDegree(mValidatedTasks.size(), 0);
        for (size_t i = 0; i < mDependencyGraph.size(); ++i) {
            inDegree[i] = mDependencyGraph[i].size();
        }
        
        // Find all tasks with no dependencies (in-degree = 0)
        DynaArray<size_t> queue;
        for (size_t i = 0; i < inDegree.size(); ++i) {
            if (inDegree[i] == 0) {
                queue.append(i);
            }
        }
        
        // Process tasks with no dependencies
        while (!queue.empty()) {
            size_t current = queue.front();
            queue.eraseIdx(0);
            result.append(current);
            
            // Reduce in-degree of tasks that depend on current
            for (size_t dependentIdx : reverseGraph[current]) {
                GN_ASSERT(inDegree[dependentIdx] > 0);
                inDegree[dependentIdx]--;
                if (inDegree[dependentIdx] == 0) {
                    queue.append(dependentIdx);
                }
            }
        }
        
        // Check for cycles (if result size != total tasks, there's a cycle)
        if (result.size() != mValidatedTasks.size()) {
            GN_ERROR(sLogger)("Circular dependency detected in task graph");
            result.clear();
            return result;
        }
        
        return result;
    }
    
    // Execute a single shard
    Action::ExecutionResult executeShard(const Task::Shard & shard, Task * task, size_t shardIndex) {
        if (!shard.action) {
            GN_ERROR(sLogger)("Task '{}' shard {}: action is null", task->name, shardIndex);
            return Action::ExecutionResult::FAILED;
        }
        
        // Execute the action with the arguments map directly
        return shard.action->execute(shard.arguments);
    }
    
    // Execute a single task (all its shards sequentially)
    Action::ExecutionResult executeTask(size_t taskIndex) {
        Task * task = mValidatedTasks[taskIndex];
        GN_ASSERT(task);
        
        // Execute all shards in sequential order
        for (size_t shardIdx = 0; shardIdx < task->shards.size(); ++shardIdx) {
            const Task::Shard & shard = task->shards[shardIdx];
            Action::ExecutionResult result = executeShard(shard, task, shardIdx);
            
            if (result == Action::ExecutionResult::FAILED) {
                GN_ERROR(sLogger)("Task '{}' shard {} execution failed", task->name, shardIdx);
                return Action::ExecutionResult::FAILED;
            }
            
            if (result == Action::ExecutionResult::WARNING) {
                GN_WARN(sLogger)("Task '{}' shard {} execution completed with warnings", task->name, shardIdx);
            }
        }
        
        return Action::ExecutionResult::PASSED;
    }

public:
    RenderGraphImpl() {}
    ~RenderGraphImpl() {}

    Task * schedule() override {
        std::lock_guard<std::mutex> lock(mScheduleMutex);
        
        // Create a new empty task
        Task * newTask = new TaskImpl();
        mPendingTasks.append(newTask);
        
        return newTask;
    }

    RenderGraph::ExecutionResult execute(const ExecutionParams & params) override {
        std::lock_guard<std::mutex> lock(mExecuteMutex);

        RenderGraph::ExecutionResult execResult;
        execResult.debugStats.clear();

        // step 1: compile the task graph, validate tasks and build dependency graph
        if (!compileTaskGraph()) {
            execResult.result = Action::ExecutionResult::FAILED;
            if (params.debug) {
                execResult.debugStats = "Task graph compilation failed";
            }
            return execResult;
        }

        // step 2: execute the task graph sequentially based on topological order
        // Perform topological sort to determine execution order
        DynaArray<size_t> executionOrder = topologicalSort();
        if (executionOrder.empty()) {
            GN_ERROR(sLogger)("Failed to determine task execution order");
            execResult.result = Action::ExecutionResult::FAILED;
            if (params.debug) {
                execResult.debugStats = "Topological sort failed - circular dependency detected";
            }
            return execResult;
        }
        
        // Execute tasks sequentially in topological order
        bool hasWarning = false;
        const size_t totalTasks = executionOrder.size();
        
        for (size_t taskIdx : executionOrder) {
            Action::ExecutionResult result = executeTask(taskIdx);
            
            if (result == Action::ExecutionResult::FAILED) {
                GN_ERROR(sLogger)("Task '{}' execution failed, stopping execution", mValidatedTasks[taskIdx]->name);
                execResult.result = Action::ExecutionResult::FAILED;
                if (params.debug) {
                    execResult.debugStats = StrA::format("Execution failed at task index {}", taskIdx);
                }
                return execResult;
            }
            
            if (result == Action::ExecutionResult::WARNING) {
                GN_WARN(sLogger)("Task '{}' execution completed with warnings", mValidatedTasks[taskIdx]->name);
                hasWarning = true;
            }
        }
        
        if (hasWarning) {
            GN_TRACE(sLogger)("Successfully executed {} tasks sequentially with warnings", totalTasks);
            execResult.result = Action::ExecutionResult::WARNING;
        } else {
            GN_TRACE(sLogger)("Successfully executed {} tasks sequentially", totalTasks);
            execResult.result = Action::ExecutionResult::PASSED;
        }
        
        if (params.debug) {
            // Generate debug statistics
            StrA stats = StrA::format("Executed {} tasks successfully", totalTasks);
            if (hasWarning) {
                stats += " (with warnings)";
            }
            execResult.debugStats = stats;
        }

        // done
        return execResult;
    }
};

GN_API RenderGraph * createRenderGraph(const CreateRenderGraphParams & params) {
    (void)params; // unused for now
    return new RenderGraphImpl();
}

} // namespace GN::rg