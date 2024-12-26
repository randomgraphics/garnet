#ifndef __GN_BASE_AUTOPTR_H__
#define __GN_BASE_AUTOPTR_H__
// *****************************************************************************
/// \file
/// \brief   Common auto pointer classes
/// \author  chenli@@REDMOND (2009.3.31)
// *****************************************************************************

#include <mutex>

namespace GN {
namespace detail {
///
/// Basic auto pointer class. STL compatible and thread safe.
///
template<typename T, typename CLASS, typename MUTEX>
class BaseAutoPtr {
    struct Payload {
        T *                  ptr {};
        std::atomic<int64_t> counter {1};
    };

    typedef BaseAutoPtr<T, CLASS, MUTEX> MyType;

    Payload *          mPayload {};
    mutable std::mutex mMutex;

    void doAttach(T * p) {
        if (p) {
            mPayload      = new Payload;
            mPayload->ptr = p;
        }
    }

    void doClear(bool releaseRawPtr = true) {
        if (mPayload && 1 == mPayload->counter.fetch_sub(1)) {
            if (releaseRawPtr) CLASS::sDoRelease(mPayload->ptr);
            delete mPayload;
            mPayload = nullptr;
        }
    }

public:
    ///
    /// Construct from raw pointer
    ///
    explicit BaseAutoPtr(T * p = nullptr) { doAttach(p); }

    ///
    /// Copy constructor
    ///
    BaseAutoPtr(const MyType & other) throw() {
        auto lock = std::lock_guard(other.mMutex);
        mPayload  = other.mPayload;
        if (mPayload) mPayload->counter.fetch_add(1);
    }

    ///
    /// Move constructor
    ///
    BaseAutoPtr(MyType && other) throw() {
        auto lock      = std::lock_guard(other.mMutex);
        mPayload       = other.mPayload;
        other.mPayload = nullptr;
    }

    ///
    /// Destructor
    ///
    ~BaseAutoPtr() { clear(); }

    ///
    /// Is pointer empty or not.
    ///
    bool empty() const { return 0 == mPayload; }

    ///
    /// Get internal C-style raw pointer
    ///
    T * rawptr() const {
        auto lock = std::lock_guard(mMutex);
        return mPayload ? mPayload->ptr : nullptr;
    }

    ///
    /// clear internal pointer. Same as attach(0)
    ///
    void clear() {
        auto lock = std::lock_guard(mMutex);
        doClear();
    }

    ///
    /// attach to new pointer (release the old one)
    ///
    void attach(T * p) {
        auto lock = std::lock_guard(mMutex);
        doClear();
        doAttach(p);
    }

    ///
    /// Release ownership of private pointer
    ///
    T * detach() throw() {
        auto lock = std::lock_guard(mMutex);
        T *  tmp  = mPayload ? mPayload->ptr : nullptr;
        doClear(false);
        return tmp;
    }

    ///
    /// dereference the pointer
    ///
    T & dereference() const { return *rawptr(); }

    ///
    /// Convert to T *
    ///
    operator T *() const { return rawptr(); }

    ///
    /// dereference operator
    ///
    T & operator*() const { return dereference(); }

    ///
    /// arrow operator
    ///
    T * operator->() const { return rawptr(); }

    ///
    /// assignment operator
    ///
    MyType & operator=(const MyType & other) {
        if (this != &other) {
            auto lock = std::scoped_lock(mMutex, other.mMutex);
            if (mPayload != other.mPayload) {
                doClear();
                mPayload = other.mPayload;
                if (mPayload) mPayload->counter.fetch_add(1);
            }
        }
        return *this;
    }

    ///
    /// Move assignment operator
    ///
    MyType & operator=(MyType && other) {
        if (this != &other) {
            auto lock = std::scoped_lock(mMutex, other.mMutex);
            if (mPayload != other.mPayload) {
                doClear();
                mPayload       = other.mPayload;
                other.mPayload = nullptr;
            }
        }
        return *this;
    }
};
} // namespace detail

#if GN_POSIX
///
/// Automatic X resource pointer
///
template<typename T, typename MUTEX = std::mutex>
class AutoXPtr : public detail::BaseAutoPtr<T, AutoXPtr<T>, MUTEX> {
public:
    static void sDoRelease(T * p) {
        if (p) XFree(p);
    }

    ///
    /// Construct from C-style pointer
    ///
    explicit AutoXPtr(T * p = 0) throw(): detail::BaseAutoPtr<T, AutoXPtr<T>, MUTEX>(p) {}
};
#endif

///
/// Automatic object pointer.
///
template<typename T, typename MUTEX = std::mutex>
class AutoObjPtr : public detail::BaseAutoPtr<T, AutoObjPtr<T>, MUTEX> {
public:
    static void sDoRelease(T * p) {
        if (p) delete p;
    }

    ///
    /// Construct from C-style pointer
    ///
    explicit AutoObjPtr(T * p = 0) throw(): detail::BaseAutoPtr<T, AutoObjPtr<T>, MUTEX>(p) {}
};

///
/// Automatic object array.
///
template<typename T, typename MUTEX = std::mutex>
class AutoObjArray : public detail::BaseAutoPtr<T, AutoObjArray<T>, MUTEX> {
public:
    static void sDoRelease(T * p) {
        if (p) delete[] p;
    }

    ///
    /// Construct from C-style pointer
    ///
    explicit AutoObjArray(T * p = 0) throw(): detail::BaseAutoPtr<T, AutoObjArray<T>, MUTEX>(p) {}
};

///
/// Automatic C-style array created by HeapMemory::alloc.
///
template<typename T, typename MUTEX = std::mutex>
class AutoHeapPtr : public detail::BaseAutoPtr<T, AutoHeapPtr<T>, MUTEX> {
public:
    static void sDoRelease(T * p) {
        if (p) HeapMemory::dealloc((void *) p);
    }

    ///
    /// Construct from C-style pointer
    ///
    explicit AutoHeapPtr(T * p = 0) throw(): detail::BaseAutoPtr<T, AutoHeapPtr<T>, MUTEX>(p) {}
};

///
/// Automatic COM pointer class. STL compatible and thread safe.
///
template<class T>
class AutoComPtr {
    T *                mPtr;
    mutable std::mutex mMutex;

public:
    ///
    /// Default constructor
    ///
    AutoComPtr() throw(): mPtr(0) {}

#if GN_MSVC
    ///
    /// Default constructor
    ///
    AutoComPtr(nullptr_t) throw(): mPtr(0) {}
#endif

    ///
    /// Copy constructor
    ///
    AutoComPtr(const AutoComPtr & other) throw() {
        auto lock = std::lock_guard(other.mMutex);
        mPtr      = other.mPtr;
        if (mPtr) mPtr->AddRef();
    }

    ///
    /// Copy constructor
    ///
    template<typename T2>
    AutoComPtr(const AutoComPtr<T2> & other) throw() {
        auto lock = std::lock_guard(other.mMutex);
        mPtr      = other.mPtr;
        if (mPtr) mPtr->AddRef();
    }

    ///
    /// Destructor
    ///
    ~AutoComPtr() throw() {
        if (mPtr) mPtr->Release();
    }

    ///
    /// Assignment operator
    ///
    AutoComPtr & operator=(const AutoComPtr & rhs) {
        set((T *) rhs);
        return *this;
    }

    ///
    /// Assignment operator
    ///
    template<typename T2>
    AutoComPtr & operator=(const AutoComPtr<T2> & rhs) {
        set((T2 *) rhs);
        return *this;
    }

    ///
    /// Convert to T *
    ///
    operator T *() const throw() { return mPtr; }

    ///
    /// dereference operator
    ///
    T & operator*() const throw() {
        GN_ASSERT(mPtr);
        return *mPtr;
    }

    ///
    /// Get address of pointer.
    ///
    /// This operator is normally used during resource creation.
    /// This function will ensure that the internal pointer is NULL.
    ///
    T ** operator&() throw() {
        GN_ASSERT(NULL == mPtr);
        return &mPtr;
    }

    ///
    /// Get address of pointer.
    ///
    T * const * operator&() const throw() {
        GN_ASSERT(mPtr);
        return &mPtr;
    }

    ///
    /// self explain.
    ///
    T * operator->() const throw() {
        GN_ASSERT(mPtr);
        return mPtr;
    }

    ///
    /// NOT operator
    ///
    bool operator!() const throw() { return mPtr == NULL; }

    ///
    /// LESS operator
    ///
    bool operator<(T * pT) const throw() { return mPtr < pT; }

    ///
    /// EQ operator
    ///
    bool operator==(T * pT) const throw() { return mPtr == pT; }

    ///
    /// Is empty pointer?
    ///
    bool empty() const { return 0 == mPtr; }

    ///
    /// Clear to empty. Same as set(NULL).
    ///
    void clear() {
        auto lock = std::lock_guard(mMutex);
        if (mPtr) mPtr->Release();
        mPtr = 0;
    }

    ///
    /// self explain.
    ///
    T * get() const throw() { return mPtr; }

    ///
    /// self explain.
    ///
    T * rawptr() const throw() { return mPtr; }

    ///
    /// Release existing interface, then hold new interface
    ///
    template<typename T2>
    void set(T2 * p) throw() {
        auto lock = std::lock_guard(mMutex);
        if (p) p->AddRef();
        if (mPtr) mPtr->Release();
        mPtr = p;
    }

    ///
    /// Attach to an existing interface (does not AddRef)
    ///
    template<typename T2>
    void attach(T2 * p2) throw() {
        auto lock = std::lock_guard(mMutex);
        if (mPtr) mPtr->Release();
        mPtr = p2;
    }

    ///
    /// Attach to an existing interface (does not AddRef)
    ///
    static AutoComPtr<T> sAttach(T * p2) throw() {
        AutoComPtr<T> p;
        p.attach(p2);
        return p;
    }

    ///
    /// Detach the interface (does not Release)
    ///
    T * detach() throw() {
        auto lock = std::lock_guard(mMutex);
        T *  pt   = mPtr;
        mPtr      = NULL;
        return pt;
    }

#if !GN_GNUC
    ///
    /// templated QI.
    ///
    template<typename T2>
    long as(T2 ** ppResult) throw() {
        if (!ppResult) {
            return 0x80000003; // E_INVALIDARG
        }

        *ppResult = NULL;

        if (mPtr) {
            return mPtr->QueryInterface<T2>(ppResult);
        } else {
            return 0x80000005; // E_POINTER
        }
    }

    ///
    /// templated QI.
    ///
    template<typename T2>
    AutoComPtr<T2> as() throw() {
        AutoComPtr<T2> result;
        if (mPtr) mPtr->QueryInterface<T2>(&result);
        return result;
    }
#endif
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_AUTOPTR_H__
