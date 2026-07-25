#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Do not include <garnet/e2/universe.h> directly. Include <garnet/GNengine2.h> instead."
#endif

#include <atomic>

namespace GN::e2 {

/// This is the global singleton of the engine module. Always initialized first and available
/// for the entire life time of the engine.
struct Universe {
    Universe()          = default;
    virtual ~Universe() = default;

    GN_NO_COPY(Universe);
    GN_NO_MOVE(Universe);

    /// @brief Generate an identifier unique within the entire universe,
    int64_t generateUniqueIdentifier() {
        auto old = mNextID.load(std::memory_order_relaxed);
        for (;;) {
            auto next = old + 1;
            if (next == 0) next = 1;
            if (mNextID.compare_exchange_weak(old, next, std::memory_order_relaxed)) { return next; }
        }
    };

private:
    std::atomic<int64_t> mNextID = {};
};

} // namespace GN::e2
