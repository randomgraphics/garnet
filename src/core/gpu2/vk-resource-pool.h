#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <string_view>

namespace GN::gpu2 {

/// Generic pool for Vulkan handle types (e.g. vk::Semaphore, vk::Fence).
/// Avoids per-frame create/destroy by recycling idle handles.
///
/// T must be a lightweight Vulkan-hpp handle with a null sentinel T{} and
/// an explicit bool conversion that returns false for null handles.
template<typename T>
class ResourcePoolVulkan {
public:
    /// Lifecycle hooks. Subclass this in the TU that owns the pool so the
    /// concrete Vulkan device pointer doesn't have to be stored as a lambda capture.
    struct Traits {
        virtual ~Traits()                         = default;
        virtual T    create()                     = 0; ///< allocate a new handle; return T{} on failure
        virtual void destroy(T &)                 = 0; ///< free an idle handle at pool shutdown
        virtual void onAcquire(T &, const char *) = 0; ///< called when a (new or recycled) resource is re-acquired
        virtual void onRecycle(T &) {};                ///< optional. called when a resource is recycled back to pool.
    };

    /// RAII handle returned by acquire(). Returns the resource to the pool on last release.
    struct Entry : RefCounter {
        Entry(ResourcePoolVulkan & pool, T && value): mPool(pool), mValue(std::move(value)) {}
        ~Entry() override { mPool.recycle(std::move(mValue)); }

        const T & get() const { return mValue; }
        T &       get() { return mValue; }

    private:
        ResourcePoolVulkan & mPool;
        T                    mValue;
    };

    typedef AutoRef<Entry> PooledResource;

    // Non-movable: Entry holds a raw address into this pool.
    GN_NO_MOVE(ResourcePoolVulkan);
    GN_NO_COPY(ResourcePoolVulkan);

    explicit ResourcePoolVulkan(std::unique_ptr<Traits> traits): mTraits(std::move(traits)) { GN_ASSERT(mTraits); }

    ~ResourcePoolVulkan() {
        for (T & v : mPool) {
            if (v) GN_LIKELY mTraits->destroy(v);
        }
        mPool.clear();
    }

    PooledResource acquire(const char * name) {
        std::lock_guard lock(mMutex);
        T               value {};
        if (!mPool.empty()) {
            value = mPool.back();
            mPool.pop_back();
        } else {
            try {
                value = mTraits->create();
                if (!value) return {};
            } catch (const std::exception & e) {
                GN_ERROR(GN::getLogger("GN.gpu2.vk"), "ResourcePoolVulkan::acquire failed: {}", e.what());
                return {};
            }
        }
        GN_ASSERT(value);
        mTraits->onAcquire(value, name);
        return PooledResource(new Entry(*this, std::move(value)));
    }

private:
    std::unique_ptr<Traits> mTraits;
    std::mutex              mMutex;
    std::vector<T>          mPool;

    friend struct Entry;

    void recycle(T && value) {
        if (!value) GN_UNLIKELY return;
        std::lock_guard lock(mMutex);
        mTraits->onRecycle(value);
        mPool.push_back(std::move(value));
    }
};

} // namespace GN::gpu2
