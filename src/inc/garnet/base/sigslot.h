/*
    Derived from the work by Sergey A Kryukov: "The Impossibly Fast C++ Delegates, Fixed", 2017
    https://www.codeproject.com/articles/The-Impossibly-Fast-Cplusplus-Delegates-Fixed,

    which is then based on the original work of Sergey Ryazanov: "The Impossibly Fast C++ Delegates", 18 Jul 2005
    https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

    MIT license:
    http://en.wikipedia.org/wiki/MIT_License
*/

#ifndef __GN_BASE_SIGSLOT_H__
#define __GN_BASE_SIGSLOT_H__
// *****************************************************************************
/// \file
/// \brief   fast signal and slot classes
/// \author  chenlee (2005.5.14)
// *****************************************************************************

#include <list>
#include <memory>

namespace GN {

namespace internal {

template<typename T>
class delegate_base;

template<typename RET, typename... PARAMS>
class delegate_base<RET(PARAMS...)> {

protected:
    using stub_type = RET (*)(const void * this_ptr, PARAMS...);

    struct InvocationElement {
        InvocationElement() = default;
        InvocationElement(const void * this_ptr, stub_type aStub): object(this_ptr), stub(aStub) {}
        void Clone(InvocationElement & target) const {
            target.stub   = stub;
            target.object = object;
        }                                                                                                                             // Clone
        bool         operator==(const InvocationElement & another) const { return another.stub == stub && another.object == object; } //==
        bool         operator!=(const InvocationElement & another) const { return another.stub != stub || another.object != object; } //!=
        const void * object = nullptr;
        stub_type    stub   = nullptr;
    }; // InvocationElement

}; // class delegate_base

template<typename T>
class delegate;

template<typename RET, typename... PARAMS>
class delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
public:
    delegate() = default;

    bool isNull() const { return invocation.stub == nullptr; }
    bool operator==(const void * ptr) const { return (ptr == nullptr) && this->isNull(); }    // operator ==
    bool operator!=(const void * ptr) const { return (ptr != nullptr) || (!this->isNull()); } // operator !=

    delegate(const delegate & another) { another.invocation.Clone(invocation); }

    template<typename LAMBDA>
    delegate(const LAMBDA & lambda) {
        assign(&lambda, lambda_stub<LAMBDA>);
    } // delegate

    delegate & operator=(const delegate & another) {
        another.invocation.Clone(invocation);
        return *this;
    } // operator =

    template<typename LAMBDA> // template instantiation is not needed, will be deduced (inferred):
    delegate & operator=(const LAMBDA & instance) {
        assign(&instance, lambda_stub<LAMBDA>);
        return *this;
    } // operator =

    bool operator==(const delegate & another) const { return invocation == another.invocation; }
    bool operator!=(const delegate & another) const { return invocation != another.invocation; }

    template<class T, RET (T::*TMethod)(PARAMS...)>
    static delegate createMethod(T * instance) {
        return delegate(instance, method_stub<T, TMethod>);
    } // create

    template<class T, RET (T::*TMethod)(PARAMS...) const>
    static delegate createMethod(T const * instance) {
        return delegate(instance, const_method_stub<T, TMethod>);
    } // create

    template<typename CLASS_, RET (CLASS_::*METHOD)(PARAMS...) volatile>
    static delegate createMethod(volatile CLASS_ * instance) {
        return delegate(instance, volatile_method_stub<CLASS_, METHOD>);
    } // create

    template<typename CLASS_, RET (CLASS_::*METHOD)(PARAMS...) const volatile>
    static delegate createMethod(volatile CLASS_ const * instance) {
        return delegate(instance, volatile_const_method_stub<CLASS_, METHOD>);
    } // create

    template<RET (*TMethod)(PARAMS...)>
    static delegate createFunction() {
        return delegate(nullptr, function_stub<TMethod>);
    } // create

    template<typename LAMBDA>
    static delegate createLambda(const LAMBDA & instance) {
        return delegate(&instance, lambda_stub<LAMBDA>);
    } // create

    RET operator()(PARAMS... arg) const { return (*invocation.stub)(invocation.object, arg...); } // operator()

private:
    delegate(const void * anObject, typename delegate_base<RET(PARAMS...)>::stub_type aStub) {
        invocation.object = anObject;
        invocation.stub   = aStub;
    } // delegate

    void assign(const void * anObject, typename delegate_base<RET(PARAMS...)>::stub_type aStub) {
        this->invocation.object = anObject;
        this->invocation.stub   = aStub;
    } // assign

    template<class T, RET (T::*TMethod)(PARAMS...)>
    static RET method_stub(const void * this_ptr, PARAMS... params) {
        auto p = (T *) (this_ptr);
        return (p->*TMethod)(params...);
    } // method_stub

    template<class T, RET (T::*TMethod)(PARAMS...) const>
    static RET const_method_stub(const void * this_ptr, PARAMS... params) {
        auto p = (T const *) (this_ptr);
        return (p->*TMethod)(params...);
    } // const_method_stub

    template<class T, RET (T::*TMethod)(PARAMS...) volatile>
    static RET volatile_method_stub(const void * this_ptr, PARAMS... params) {
        auto p = (volatile T *) (this_ptr);
        return (p->*TMethod)(params...);
    } // volatile_method_stub

    template<class T, RET (T::*TMethod)(PARAMS...) const volatile>
    static RET volatile_const_method_stub(const void * this_ptr, PARAMS... params) {
        auto p = (volatile T const *) (this_ptr);
        return (p->*TMethod)(params...);
    } // volatile_const_method_stub

    template<RET (*TMethod)(PARAMS...)>
    static RET function_stub(const void *, PARAMS... params) {
        return TMethod(params...);
    } // function_stub

    template<typename LAMBDA>
    static RET lambda_stub(const void * this_ptr, PARAMS... arg) {
        auto p = (const LAMBDA *) (this_ptr);
        return (p->operator())(arg...);
    } // lambda_stub

    typename delegate_base<RET(PARAMS...)>::InvocationElement invocation;
}; // class delegate

/// Base class of all signals.
class SignalBase {
protected:
    SignalBase()          = default;
    virtual ~SignalBase() = default;
};

template<typename>
struct member_fn_traits; // primary

// non-const
template<typename C, typename R, typename... A>
struct member_fn_traits<R (C::*)(A...)> {
    using class_type     = C;
    using class_ptr_type = C *;
};

// const
template<typename C, typename R, typename... A>
struct member_fn_traits<R (C::*)(A...) const> {
    using class_type     = C;
    using class_ptr_type = const C *;
};

// volatile
template<typename C, typename R, typename... A>
struct member_fn_traits<R (C::*)(A...) volatile> {
    using class_type     = C;
    using class_ptr_type = volatile C *;
};

// volatile const
template<typename C, typename R, typename... A>
struct member_fn_traits<R (C::*)(A...) const volatile> {
    using class_type     = C;
    using class_ptr_type = volatile const C *;
};

// ref-qualifiers
template<typename C, typename R, typename... A>
struct member_fn_traits<R (C::*)(A...) &> : member_fn_traits<R (C::*)(A...)> {};

template<typename C, typename R, typename... A>
struct member_fn_traits<R (C::*)(A...) const &> : member_fn_traits<R (C::*)(A...) const> {};

template<typename C, typename R, typename... A>
struct member_fn_traits<R (C::*)(A...) &&> : member_fn_traits<R (C::*)(A...)> {};

template<typename C, typename R, typename... A>
struct member_fn_traits<R (C::*)(A...) const &&> : member_fn_traits<R (C::*)(A...) const> {};

} // namespace internal

///
/// Represents a connection between a signal and a slot. Destructing this object will disconnect the slot from the signal.
///
class Tether {
public:
    Tether() = default;

    Tether(const internal::SignalBase * signal, std::function<void()> && disconnFunc): mSignal(signal), mDisconnFunc(std::move(disconnFunc)) {}

    ~Tether() { clear(); }

    GN_NO_COPY(Tether); // Not copyable

    /// move constructor
    Tether(Tether && other): mSignal(other.mSignal), mDisconnFunc(std::move(other.mDisconnFunc)) {
        other.mSignal      = nullptr;
        other.mDisconnFunc = nullptr;
    }

    /// move assignment
    Tether & operator=(Tether && other) {
        if (this == &other) GN_UNLIKELY return *this;
        mSignal            = other.mSignal;
        mDisconnFunc       = std::move(other.mDisconnFunc);
        other.mSignal      = nullptr;
        other.mDisconnFunc = nullptr;
        return *this;
    }

    auto signal() const { return mSignal; }

    void clear() {
        if (mDisconnFunc) { mDisconnFunc(); }
        mSignal      = nullptr;
        mDisconnFunc = nullptr;
    }

private:
    const internal::SignalBase * mSignal {};
    std::function<void()>        mDisconnFunc;
};

///
/// Base slot class. Derive your class from this, if you want automatic management of connections between signal and slot.
///
/// Example:
/// ```
/// class MySlot : public SlotBase {
/// public:
///     void mySlotMethod(int value) {
///         std::cout << "Signal received: " << value << std::endl;
///     }
///     void foo(const MySignal & signal) {
///         // The connection to the signal is now managed by this class and will get automatically disconnected when this class is destroyed.
///         connectToSignal<&MySlot::mySlotMethod>(signal);
///     }
/// };
///
/// ```
class SlotBase {
protected:
    SlotBase() = default;

    virtual ~SlotBase() = default;

    GN_NO_COPY(SlotBase);

    // movable
    SlotBase(SlotBase && other) {
        auto lock = std::lock_guard(other.mLock);
        mTethers  = std::move(other.mTethers);
        other.mTethers.clear();
    }

    // move operator
    SlotBase & operator=(SlotBase && other) {
        if (this == &other) GN_UNLIKELY return *this;
        std::scoped_lock lock(mLock, other.mLock);
        mTethers = std::move(other.mTethers);
        return *this;
    }

public:
    /// Make a managed connection to signal. The connection will be automatically disconnected when this slot is destructed.
    template<auto FUNCTION, typename SIGNAL>
    void connectToSignal(const SIGNAL & signal) {
        auto lock = std::lock_guard(mLock);
        if constexpr (std::is_member_function_pointer_v<decltype(FUNCTION)>) {
            using class_type = typename internal::member_fn_traits<decltype(FUNCTION)>::class_type;
            static_assert(std::is_base_of_v<SlotBase, class_type>, "FUNCTION must be a member function of the derived class of SlotBase");
            class_type * classPtr = static_cast<class_type *>(this);
            mTethers.push_back(signal.template connect<FUNCTION>(classPtr));
        } else if constexpr (std::is_pointer_v<decltype(FUNCTION)> && std::is_function_v<std::remove_pointer_t<decltype(FUNCTION)>>) {
            mTethers.push_back(signal.template connect<FUNCTION>());
        } else {
            static_assert(std::is_same_v<decltype(FUNCTION), void>, "FUNCTION must be a member function pointer or a function pointer or a function");
        }
    }

    /// Explicitly disconnect the slot from specified signal.
    void disconnectFromSignal(const internal::SignalBase & signal) const {
        auto lock = std::lock_guard(mLock);
        for (auto it = mTethers.begin(); it != mTethers.end();) {
            if (it->signal() == &signal) {
                it = mTethers.erase(it);
            } else {
                ++it;
            }
        }
    }

    void disconnectFromAllSignals() const {
        auto lock = std::lock_guard(mLock);
        mTethers.clear();
    }

    void manageTether(Tether && tether) {
        auto lock = std::lock_guard(mLock);
        mTethers.push_back(std::move(tether));
    }

private:
    mutable std::list<Tether>    mTethers;
    mutable std::recursive_mutex mLock;
};

template<class>
class Signal; // undefined.

template<typename RET>
struct SignalResults {
    DynaArray<RET> results;
};

template<>
struct SignalResults<void> {};

template<typename RET, typename... PARAMS>
class Signal<RET(PARAMS...)> : public internal::SignalBase {
    typedef internal::delegate<RET(PARAMS...)> DelegateType;

public:
    Signal(): mControlBlock(std::make_shared<ControlBlock>()) {}
    ~Signal() { mControlBlock.reset(); }

    template<RET (*STATIC_FUNCTION)(PARAMS...)>
    [[nodiscard]] Tether connect() const {
        // TODO: prevent re-entrancy here?
        auto lock = std::lock_guard(mControlBlock->mutex);
        return addDelegate(DelegateType::template createFunction<STATIC_FUNCTION>());
    }

    template<auto MEMBER_FN, typename = std::enable_if_t<std::is_member_function_pointer_v<decltype(MEMBER_FN)>>>
    [[nodiscard]] Tether connect(typename internal::member_fn_traits<decltype(MEMBER_FN)>::class_ptr_type object) const {
        // TODO: prevent re-entrancy here?
        typedef typename internal::member_fn_traits<decltype(MEMBER_FN)>::class_type ClassType;

        auto lock = std::lock_guard(mControlBlock->mutex);
        return addDelegate(DelegateType::template createMethod<ClassType, MEMBER_FN>(object));
    }

    // template<typename CLASS_, RET (CLASS_::*METHOD)(PARAMS...)>
    // [[nodiscard]] Tether connect(CLASS_ * classPtr) const {
    //     auto lock = std::lock_guard(mLock);
    //     return addDelegate(DelegateType::template createMethod<CLASS_, METHOD>(classPtr));
    // }

    // template<typename CLASS_, RET (CLASS_::*METHOD)(PARAMS...) const>
    // [[nodiscard]] Tether connect(const CLASS_ * classPtr) const {
    //     auto lock = std::lock_guard(mLock);
    //     return addDelegate(DelegateType::template createMethod<CLASS_, METHOD>(classPtr));
    // }

    // template<typename CLASS_, RET (CLASS_::*METHOD)(PARAMS...) volatile>
    // [[nodiscard]] Tether connect(volatile CLASS_ * classPtr) const {
    //     auto lock = std::lock_guard(mLock);
    //     return addDelegate(DelegateType::template createMethod<CLASS_, METHOD>(classPtr));
    // }

    // template<typename CLASS_, RET (CLASS_::*METHOD)(PARAMS...) const volatile>
    // [[nodiscard]] Tether connect(volatile CLASS_ const * classPtr) const {
    //     auto lock = std::lock_guard(mLock);
    //     return addDelegate(DelegateType::template createMethod<CLASS_, METHOD>(classPtr));
    // }

    /// this is simpler form of emit() that returns the return value of the last delegate.
    template<typename... ARGS>
    SignalResults<RET> emit(ARGS &&... args) const {
        auto               lock = std::lock_guard(mControlBlock->mutex);
        SignalResults<RET> results;
        if constexpr (std::is_same_v<RET, void>) {
            // For void return, all delegates get lvalue references to avoid moving rvalues multiple times
            for (const auto & d : mControlBlock->delegates) { d(static_cast<std::remove_reference_t<ARGS> &>(args)...); }
            return results;
        } else {
            if (mControlBlock->delegates.empty()) {
                // Return default-constructed value - enables RVO (returning temporary)
                return results;
            }
            // // For single delegate, forward arguments - enables move semantics for rvalues
            // if (mControlBlock->delegates.size() == 1) { return mControlBlock->delegates.front()(std::forward<ARGS>(args)...); }
            // For multiple delegates: convert to lvalues to avoid moving rvalues multiple times.
            // All delegates except the last receive lvalue references (safe for multiple calls).
            // The last delegate gets forwarded arguments (can move if rvalues).
            results.results.reserve(mControlBlock->delegates.size());
            for (const auto & d : mControlBlock->delegates) { results.results.append(d(static_cast<std::remove_reference_t<ARGS> &>(args)...)); }
            return results;
        }
    }

    /// this is the operator form of emit().
    template<typename... ARGS>
    SignalResults<RET> operator()(ARGS &&... args) const {
        return emit(std::forward<ARGS>(args)...);
    }

private:
    // Control block that keeps the Signal's data alive even if the Signal object is destroyed
    struct ControlBlock {
        std::recursive_mutex    mutex;
        std::list<DelegateType> delegates;

        ~ControlBlock() {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            delegates.clear();
        }
    };

    mutable std::shared_ptr<ControlBlock> mControlBlock;

    Tether addDelegate(DelegateType && delegate) const {
        // Note: We don't need to lock here because addDelegate is only called from connect()
        auto iter = mControlBlock->delegates.emplace(mControlBlock->delegates.end(), std::move(delegate));

        // Capture a weak_ptr to the control block so we can check if it's still alive
        std::weak_ptr<ControlBlock> weakControl = mControlBlock;

        return Tether(this, std::function<void()>([weakControl, iter]() {
                          // Try to lock the weak_ptr. If successful, the control block (and thus the signal data) is still alive.
                          // The lock() operation is atomic, so this prevents the race condition.
                          // Once we have the shared_ptr, the control block is guaranteed to stay alive for the duration
                          // of this lambda execution, even if the Signal object itself is destroyed.
                          if (auto control = weakControl.lock()) {
                              // Acquire the lock atomically - no window for another thread to destroy the signal
                              // The shared_ptr ensures the control block stays alive, and the lock ensures exclusive access
                              std::lock_guard<std::recursive_mutex> lock(control->mutex);
                              // The control block is alive and we hold the lock, so we can safely erase the connection.
                              // The iterator remains valid until we erase it, and we hold the lock so no other thread
                              // can modify the list concurrently.
                              control->delegates.erase(iter);
                          }
                          // If the weak_ptr cannot be locked, the control block (and signal) has been destroyed, so we do nothing.
                      }));
    }
};

} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SIGSLOT_H__
