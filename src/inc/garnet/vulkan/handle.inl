#include <atomic>
#include <functional>

namespace GN::vulkan {

/// Pooled allocator to speed up dynamic memory allocation/deallocation in AutoHandle class.
//@{
GN_API void * allocateAutoHandle(size_t);
GN_API void   deallocateAutoHandle(void *, size_t);
//@}

/// Helper class to manage Vulkan handle life time.
template<typename T>
class AutoHandle {

public:
    /// default constructor
    AutoHandle() noexcept = default;

    /// copy constructor
    AutoHandle(const AutoHandle & rhs) noexcept {
        _handle = rhs._handle;
        _ref    = rhs._ref;
        if (_ref) _ref->add();
    }

    /// move constructor
    AutoHandle(AutoHandle && rhs) noexcept {
        _handle     = rhs._handle;
        _ref        = rhs._ref;
        rhs._handle = 0;
        rhs._ref    = 0;
    }

    /// destructor
    ~AutoHandle() noexcept {
        try {
            clear();
        } catch (...) {
            // log?
        }
    }

    /// copy operator
    AutoHandle & operator=(const AutoHandle & rhs) noexcept {
        if (this == rhs.addr()) { return *this; }
        if (_handle == rhs._handle) {
            GN_ASSERT(_ref == rhs._ref);
            return *this;
        }
        clear();
        _handle = rhs._handle;
        _ref    = rhs._ref;
        if (_ref) _ref->add();
        return *this;
    }

    /// move operator
    AutoHandle & operator=(AutoHandle && rhs) noexcept {
        if (this == rhs.addr()) { return *this; }
        clear();
        _handle     = rhs._handle;
        _ref        = rhs._ref;
        rhs._handle = 0;
        rhs._ref    = 0;
        return *this;
    }

    bool operator==(const AutoHandle & rhs) const noexcept { return _handle == rhs.handle; }
    bool operator==(T rhs) const noexcept { return _handle == rhs; }
    bool operator!=(const AutoHandle & rhs) const noexcept { return _handle == rhs.handle; }
    bool operator!=(T rhs) const noexcept { return _handle == rhs; }
    bool operator!() const noexcept { return !_handle; }

    // Only use this when referencing a valid handle for read. If you need to modify/recreate
    // the handle, call prepare().
    const T * operator&() const {
        // If your code triggers this assert failure, and you are creating new handle, please use
        // prepare() method instead.
        GN_ASSERT(_handle);

        return &_handle;
    }

    operator const T &() const { return _handle; }

    operator bool() const { return !!_handle; }

    const T & get() const { return _handle; }

    bool empty() const { return !_handle; }

    /// delete the underlying handle.
    void clear() {
        if (_ref && _ref->dec()) {
            GN_ASSERT(0 == _ref->c);
            if (_handle) _ref->d(_handle);
            delete _ref;
        }
        _handle = 0;
        _ref    = 0;
    }

    // TODO: void attach(T );

    /// Detach from underlying handle. Note that detach is only allowed if ths is the last reference to the handle.
    ///
    /// If there are more than one reference to the handle, detach() function will behave just like clear() and return
    /// a null handle.
    T detach() {
        T detached = 0;
        if (_ref) {
            if (_ref->dec()) {
                GN_ASSERT(0 == _ref->c);
                delete _ref;
                detached = _handle;
            } else {
                GN_ERROR(getLogger("GN.base.vulkan"))("Can't detach from VK handle, when it is referenced more than once.");
            }
            _handle = 0;
            _ref    = 0;
        }
        GN_ASSERT(!_handle);
        return detached;
    }

    /// Clear any preexisting handles. Return a pointer that is ready to pass to
    /// Vulkan's create function for creating a new handle.
    ///
    /// \param deleter This can be either a reference to VulkanGlobalInfo object, or a function that accepts
    ///               calling signature of: "void (T)". It'll be used delete the underlying handle.
    ///
    /// Here's an example of how it should be used:
    ///
    ///     AutoHandle<VkCommandPool> pool;
    ///     VkCreateCommandPool(device, &ci, allocator, pool.prepare([](auto h){ vkDestroyCommandPool(h); }));
    ///
    template<typename Deleter>
    T * prepare(Deleter deleter) {
        clear(); // release old handle.
        GN_ASSERT(0 == _handle && 0 == _ref);
        _ref    = new RefCounter();
        _ref->d = initDeleter<Deleter>(deleter);
        return &_handle;
    }

    /// Clear any preexisting handles. Return a pointer that is ready to pass to
    /// Vulkan's create function for creating a new handle.
    T * prepare(const VulkanGlobalInfo & vgi) {
        return prepare([&vgi](auto & h) { vgi.safeDestroy(h); });
    }

private:
    template<typename Deleter>
    Deleter & initDeleter(Deleter & d) {
        return d;
    }

    template<typename VulkanGlobalInfo>
    auto initDeleter(const VulkanGlobalInfo & vgi) {
        return [&](auto & h) { vgi.safeDestroy(h); };
    }

    // because & operator is overloaded. So we need alternative way to get the address of the class.
    const AutoHandle * addr() const { return this; }
    AutoHandle *       addr() { return this; }

    struct RefCounter {
        std::atomic<int>         c {1};
        std::function<void(T &)> d;

        void add() { ++c; }

        // Return true, means the refcountre reaches zero. thus, time to release it.
        bool dec() {
            int oldValue = c.fetch_sub(1);
            GN_ASSERT(oldValue > 0);
            return 1 == oldValue;
        }

        static void * operator new(size_t size) { return allocateAutoHandle(size); }
        static void   operator delete(void * ptr, size_t size) { return deallocateAutoHandle(ptr, size); }
    };

    T            _handle = 0;
    RefCounter * _ref    = 0;
};

} // namespace GN::vulkan