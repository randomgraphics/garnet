#pragma once

namespace GN {
class Logger;
}
namespace rapid_vulkan {
struct Barrier;
}

namespace GN::rdg {

/// GN_VERBOSE: log contents of a rapid_vulkan::Barrier before cmdWrite, for cross-checking against RDG state tracking.
void logBarrierBatchVerbose(GN::Logger * logger, const char * label, const rapid_vulkan::Barrier & barrier);

} // namespace GN::rdg
