#ifndef __GN_BASE_SYNC_H__
#define __GN_BASE_SYNC_H__
// *****************************************************************************
/// \file
/// \brief   platform independent sync operations
/// \author  chen@@CHENLI-HOMEPC (2007.4.8)
// *****************************************************************************

#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

namespace GN {
///
/// Spinloop lock
///
class SpinLoop : NoCopy {
    std::atomic_flag mLock;

public:
    //@{
    bool tryLock() { return mLock.test_and_set(); }
    void lock() {
        size_t i = 0;
        while (!mLock.test_and_set()) {
            ++i;
            if (i > 1000000) {
                std::this_thread::sleep_for(std::chrono::microseconds::min());
                i = 0;
            }
        }
    }
    void unlock() { mLock.clear(); }
    //@}
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SYNC_H__
