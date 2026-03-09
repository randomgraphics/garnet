#ifndef __GN_BASE_SMARTPTR_H__
#define __GN_BASE_SMARTPTR_H__
// *****************************************************************************
/// \file
/// \brief   Smart pointer classes and utilities
/// \author  chen@@chenli-homepc (2011.4.9)
// *****************************************************************************

#include <atomic>
#include <mutex>
#include <type_traits>

namespace GN {
/// Reference counted smart pointer. Support both strong and weak reference.
/// Note that behavior  of this class is different than the Windows COM pointer:
/// A newly  instance of RefCounter will have reference counter set to 0,
/// instead of 1. There are a few reasons why it is this way:
///  - The class can be used in non-ref-counted way. This makes it possible to
///    use subclass of RefCounted as a member of another class.
///  - This makes the AutoRef class logic much more consistent and less error prone.
///    Whenever a raw pointer is given to AutoRef class, always call addref(), regardless
///    the raw pointer is newly constructed or is passed from another AutoRef()
// -------------------------------------------------------------------------
class RefCounter {
public:
    /// Control block for weak refs. One per object, allocated on first weak ref. Lock-free: no mutex, no list.
    struct WeakObject {
        std::atomic<void *> ptr;       // RefCounter*; cleared when object is destroyed
        std::atomic<size_t> weakCount; // 1 = RefCounter owns; +1 per WeakRef

        GN_NO_COPY(WeakObject);

        WeakObject(void * object): ptr(object), weakCount(1) {}
    };

    // ********************************
    //       reference management
    // ********************************
public:
    GN_NO_COPY(RefCounter);

    GN_NO_MOVE(RefCounter);

    ///
    /// increase reference counter
    ///
    int32_t incref() const throw() { return mRef.fetch_add(1) - 1; }

    ///
    /// decrease reference counter, delete the object, if reference count reaches zero.
    ///
    int32_t decref() const {
        GN_ASSERT(mRef > 0);
        int ref = mRef.fetch_sub(1) - 1;
        if (0 == ref) delete this;
        return ref;
    }

    ///
    /// get current reference counter value
    ///
    int getref() const throw() { return mRef; }

    /// Increase reference counter if and only if the reference counter is greater than zero.
    /// Return the new reference counter value if the reference counter was increased, otherwise return 0.
    /// This method is currently used to promote weak references to strong references in a thread safe
    /// and lock-free way.
    int32_t increfIfNotZero() const throw() {
        auto expected = mRef.load();
        for (;;) {
            if (expected == 0) {
                // If the value is 0, we do not increment and break the loop
                return 0;
            }
            // 2. Try to swap the value if it's still 'expected'
            // If it fails, 'expected' is updated with the actual current value
            if (mRef.compare_exchange_strong(expected, expected + 1)) {
                // 3. If the swap succeeds, the operation is complete
                return expected + 1;
            }
            // 4. If it fails, the loop continues with the updated 'expected' value
        }
    }

    ///
    /// Return the weak control block. One allocation on first use; fast path is one atomic load.
    ///
    WeakObject * getWeakObj() const {
        WeakObject * wo = mWeakObj.load(std::memory_order_acquire);
        if (wo) GN_LIKELY return wo;
        std::lock_guard<std::mutex> g(mWeakLock);
        wo = mWeakObj.load(std::memory_order_relaxed);
        if (!wo) {
            wo = new WeakObject((void *) this);
            mWeakObj.store(wo, std::memory_order_release);
        }
        return wo;
    }

    ///
    /// For internal use. Do _NOT_ call.
    ///
    WeakObject * __getWeakObjNoLock() const { return mWeakObj.load(std::memory_order_relaxed); }

    // ********************************
    /// \name protective ctor/dtor
    // ********************************
protected:
    //@{

    ///
    /// Constructor
    ///
    RefCounter(): mRef(0), mWeakObj(nullptr) {}

    ///
    /// Destructor
    ///
    virtual ~RefCounter() {
        if (0 != mRef) {
            GN_UNEXPECTED_EX("Destructing reference counted object with non-zero reference counter usually means memory corruption, thus is not allowed!");
        }

        WeakObject * wo = mWeakObj.load(std::memory_order_acquire);
        if (wo) {
            wo->ptr.store(nullptr, std::memory_order_release);
            if (wo->weakCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete wo;
        }
    }

    //@}

    // ********************************
    // private data members
    // ********************************
private:
    ///
    /// reference counter
    ///
    mutable std::atomic_int           mRef;
    mutable std::mutex                mWeakLock;
    mutable std::atomic<WeakObject *> mWeakObj;
};

///
/// 配合 RefCounter 使用的自动指针类
// -------------------------------------------------------------------------
template<class X>
class AutoRef {
    ///
    /// pointer to class X
    ///
    typedef X * XPTR;

    ///
    /// reference to class X
    ///
    typedef X & XREF;

private:
    XPTR mPtr; ///< internal pointer

public:
    ///
    /// Instance of empty/null reference pointer
    ///
    static AutoRef<X> NULLREF;

    // default ctor
    AutoRef() throw(): mPtr(NULL) {
        // make sure sizeof(AutoRef) == sizeof(XPTR), which ensures that an array of autoref
        // can always be used as array of native pointer.
        static_assert(sizeof(AutoRef) == sizeof(XPTR));
    }

    /// construct from a normal pointer
    /// We've already automatically cast AutoRef back to raw pointer via type cast operator.
    /// If this constructor is not marked as explict, it would create ambiguity for expression like: (AutoRef<T> == T*)
    explicit AutoRef(XPTR p) throw(): mPtr(p) {
        if (p) p->incref();
    }

    ///
    /// copy constructor
    ///
    AutoRef(const AutoRef & p) throw(): mPtr(p) {
        if (mPtr) mPtr->incref();
    }

    ///
    /// copy constructor
    ///
    template<class Y>
    AutoRef(const AutoRef<Y> & p) throw(): mPtr(p) {
        if (mPtr) mPtr->incref();
    }

    ///
    /// move constructor
    ///
    AutoRef(AutoRef && p) throw(): mPtr(p) { p.mPtr = nullptr; }

    ///
    /// destructor
    ///
    ~AutoRef() {
        if (mPtr) mPtr->decref();
    }

    ///
    /// 赋值语句
    ///
    AutoRef & operator=(const AutoRef & rhs) {
        set(rhs);
        return *this;
    }

    ///
    /// 赋值语句
    ///
    template<class Y>
    AutoRef & operator=(const AutoRef<Y> & rhs) {
        set(rhs);
        return *this;
    }

    ///
    /// 赋值语句
    ///
    AutoRef & operator=(XPTR ptr) {
        set(ptr);
        return *this;
    }

    ///
    /// move operator
    ///
    AutoRef & operator=(AutoRef && rhs) {
        if (this == &rhs) return *this;
        if (mPtr) mPtr->decref();
        mPtr     = rhs.mPtr;
        rhs.mPtr = nullptr;
        return *this;
    }

    ///
    /// Convert to XPTR
    ///
    operator XPTR() const { return mPtr; }

    ///
    /// 比较操作
    ///
    bool operator==(const AutoRef & rhs) const throw() { return mPtr == rhs.mPtr; }

    ///
    /// 比较操作
    ///
    bool operator!=(const AutoRef & rhs) const throw() { return mPtr != rhs.mPtr; }

    ///
    /// 比较操作
    ///
    bool operator<(const AutoRef & rhs) const throw() { return mPtr < rhs.mPtr; }

    ///
    /// NOT operator
    ///
    bool operator!() const throw() { return !mPtr; }

    ///
    /// dereference operator.
    ///
    /// TODO: is this thread safe?
    ///
    XREF operator*() const throw() {
        GN_ASSERT(mPtr);
        return *mPtr;
    }

    ///
    /// arrow operator
    ///
    XPTR operator->() const throw() {
        GN_ASSERT(mPtr);
        return mPtr;
    }

    ///
    /// get internal raw pointer, optionally cast to another, hopefully, compatible type.
    ///
    template<typename T = X>
    T * get() const throw() {
        if constexpr (std::is_convertible_v<X, T>) {
            return mPtr;
        } else {
            return safeCastPtr<T, X>(mPtr);
        }
    }

    ///
    /// get address of internal pointer.
    ///
    /// 这个函数主要用于将指向AutoRef的指针变成指向XPTR的指针（他们在内存中的映像其实是一样的）。
    /// 你可以用强制类型转换达同样的目的，不过用这个函数会更简洁一些。
    ///
    XPTR const * addr() const throw() { return &mPtr; }

    ///
    /// return true if no pointer is currently being hold
    ///
    bool empty() const throw() { return 0 == mPtr; }

    ///
    /// Clear to empty. Same as set(NULL).
    ///
    void clear() {
        if (mPtr) mPtr->decref();
        mPtr = 0;
    }

    ///
    /// set new pointer data
    ///
    /// this function will release old pointer, if not NULL; then increase
    /// the reference counter of new pointer, if not NULL.
    ///
    void set(XPTR p) {
        if (p) p->incref();
        if (mPtr) mPtr->decref();
        mPtr = p;
    }

    ///
    /// acquire ownership of a XPTR
    ///
    /// this function will not modify pointer's refcount
    ///
    void attach(XPTR ptr) {
        if (ptr == mPtr) return;
        if (mPtr) mPtr->decref();
        mPtr = ptr;
    }

    ///
    /// release ownership of internal XPTR
    ///
    /// this function will not modify pointer's refcount
    ///
    XPTR detach() throw() {
        XPTR tmp = mPtr;
        mPtr     = 0;
        return tmp;
    }

    ///
    /// Cast to another type. Caller is responsible for ensuring the cast is valid.
    ///
    template<typename T2>
    AutoRef<T2> castTo() const {
        return AutoRef<T2>(static_cast<T2 *>(mPtr));
    }

    ///
    /// Creates an AutoRef by constructing X with the given arguments.
    /// Similar to std::make_shared.
    ///
    template<typename... Args>
    static AutoRef<X> make(Args &&... args) {
        return AutoRef<X>(new X(std::forward<Args>(args)...));
    }
};

// Make sure the size of AutoRef is the same as the size of the pointer it holds.
// This is important. So that we can cast an array of AutoRef to an array of pointer safely.
static_assert(sizeof(AutoRef<int>) == sizeof(int *));

template<typename X>
AutoRef<X> AutoRef<X>::NULLREF;

///
/// Attach to a C pointer without increasing its reference count.
///
template<typename T>
inline AutoRef<T> attachTo(T * ptr) {
    AutoRef<T> result;
    result.attach(ptr);
    return result;
}

///
/// Reference a C refcount pointer. Increase its ref count by one.
///
template<typename T>
inline AutoRef<T> referenceTo(T * ptr) {
    AutoRef<T> result;
    result.attach(ptr);
    result->incref();
    return result;
}

///
/// Weak reference to a RefCounter. Lock-free promote() and empty(); one atomic control block per object.
// -------------------------------------------------------------------------
template<typename X>
class WeakRef {
    static_assert(std::is_base_of_v<RefCounter, X>, "WeakRef<X> requires X to inherit from RefCounter");

    typedef X * XPTR;

    RefCounter::WeakObject * mBlock = nullptr;

    void addRef() {
        if (mBlock) mBlock->weakCount.fetch_add(1, std::memory_order_relaxed);
    }

    void releaseRef() {
        if (!mBlock) return;
        if (mBlock->weakCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete mBlock;
        mBlock = nullptr;
    }

public:
    WeakRef() = default;

    explicit WeakRef(XPTR ptr): mBlock(nullptr) { set(ptr); }

    WeakRef(const WeakRef & ref): mBlock(ref.mBlock) { addRef(); }

    WeakRef(WeakRef && ref) noexcept: mBlock(ref.mBlock) { ref.mBlock = nullptr; }

    ~WeakRef() { releaseRef(); }

    void clear() { releaseRef(); }

    /// Lock-free.
    bool empty() const { return !mBlock || mBlock->ptr.load(std::memory_order_acquire) == nullptr; }

    void set(XPTR ptr) {
        if (!ptr) {
            clear();
            return;
        }
        RefCounter::WeakObject * b = ptr->getWeakObj();
        if (b == mBlock) return;
        releaseRef();
        mBlock = b;
        addRef();
    }

    /// Lock-free.
    AutoRef<X> promote() const {
        AutoRef<X> result;
        if (!mBlock) return result;
        void * p = mBlock->ptr.load(std::memory_order_acquire);
        if (p && static_cast<RefCounter *>(p)->increfIfNotZero() > 0) result.attach(static_cast<XPTR>(p));
        return result;
    }

    WeakRef & operator=(const WeakRef & rhs) {
        if (this == &rhs) return *this;
        if (rhs.mBlock == mBlock) return *this;
        releaseRef();
        mBlock = rhs.mBlock;
        addRef();
        return *this;
    }

    WeakRef & operator=(WeakRef && rhs) noexcept {
        if (this == &rhs) return *this;
        releaseRef();
        mBlock     = rhs.mBlock;
        rhs.mBlock = nullptr;
        return *this;
    }

    bool operator!() const { return empty(); }

    bool operator==(const WeakRef & rhs) const throw() { return mBlock == rhs.mBlock; }

    bool operator==(XPTR ptr) const throw() { return mBlock == (ptr ? ptr->__getWeakObjNoLock() : nullptr); }

    friend bool operator==(XPTR ptr, const WeakRef & rhs) throw() { return rhs.mBlock == (ptr ? ptr->__getWeakObjNoLock() : nullptr); }

    bool operator!=(const WeakRef & rhs) const throw() { return mBlock != rhs.mBlock; }

    bool operator<(const WeakRef & rhs) const throw() { return mBlock < rhs.mBlock; }
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SMARTPTR_H__
