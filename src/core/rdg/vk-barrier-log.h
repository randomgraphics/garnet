#pragma once

namespace GN {
class Logger;
}
namespace rapid_vulkan {
struct Barrier;
}

namespace GN::rdg {

/// GN_VERBOSE: log contents of a rapid_vulkan::Barrier before cmdWrite, for cross-checking against RDG state tracking.
void logBarrierBatchVerboseSlow(GN::Logger * logger, const char * label, const rapid_vulkan::Barrier & barrier);

inline void logBarrierBatchVerbose(GN::Logger * logger, const char * label, const rapid_vulkan::Barrier & barrier) {
    if (!logger || !logger->isOn(GN::Logger::VERBOSE)) GN_LIKELY return;
    return logBarrierBatchVerboseSlow(logger, label, barrier);
}

} // namespace GN::rdg
