#pragma once

#include "submission.h"
#include "vk-texture.h"
#include <unordered_map>

namespace GN::rdg {

/// Tracks usage and layout of Vulkan resources across actions. Two passes: prepare() gathers
/// resource uses per action; execute() records barriers to transition resources to the right layout.
class ResourceTrackerVulkan {
public:
    struct ConstructParameters {
        SubmissionImpl & submission;
    };

    ResourceTrackerVulkan(const ConstructParameters &) {}

    void set(const TaskInfo & taskInfo, uint64_t textureSequenceNumber, uint32_t mip, uint32_t levels, uint32_t arrayLayer, uint32_t layers,
             const TextureState::ImageState &                newState,
             const TextureState::ImageStateTransitionFlags & flags = TextureState::ImageStateTransitionFlags::DEFAULT()) {
        (void) textureSequenceNumber;
        (void) mip;
        (void) levels;
        (void) arrayLayer;
        (void) layers;
        (void) flags;
        mTextureStates[taskInfo.index] = {newState, taskInfo.index};
    }

private:
    struct TextureStateForTask {
        TextureState::ImageState state;
        uint64_t                 taskIndex;
    };

    std::unordered_map<uint64_t, TextureStateForTask> mTextureStates;
};

} // namespace GN::rdg
