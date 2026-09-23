#include <garnet/GNfiz.h>

namespace GN::fiz {

// SolidEngine factory placeholder for Phase 1.2 scaffolding.
// Concrete Jolt-backed SolidEngine implementation will be added in Phase 1.3.
AutoRef<SolidEngine> SolidEngine::create(const SolidEngineDesc & desc) {
    (void)desc;
    return {};
}

} // namespace GN::fiz
