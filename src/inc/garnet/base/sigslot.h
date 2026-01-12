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

    Tether(const internal::SignalBase & signal, std::function<void()> && disconnFunc) : mSignal(&signal), mDisconnFunc(std::move(disconnFunc)) {}

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
///     void connectTo(const MySignal & signal) {
///         // The connection to the signal is now managed by this class and will get automatically disconnected when this class is destroyed.
///         manageTether(signal.connect(this, &MySlot::mySlotMethod));
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
    /// Manage the passed in tether. Automatically disconnect it, when this slot class is destroyed.
    void manageTether(Tether && t) const {
        auto lock = std::lock_guard(mLock);
        mTethers.push_back(std::move(t));
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

private:
    mutable std::list<Tether>    mTethers;
    mutable std::recursive_mutex mLock;
};

template<class>
class Signal; // undefined.

template<typename RET, typename... PARAMS>
class Signal<RET(PARAMS...)> : public internal::SignalBase {
    typedef internal::delegate<RET(PARAMS...)> DelegateType;

public:
    Signal() {}
    ~Signal() {}

    template<RET (*STATIC_FUNCTION)(PARAMS...)>
    [[nodiscard]] Tether connect() const {
        // TODO: prevent re-entrancy here?
        auto lock = std::lock_guard(mLock);
        return addDelegate(DelegateType::template createFunction<STATIC_FUNCTION>());
    }

    template<auto MEMBER_FN, typename = std::enable_if_t<std::is_member_function_pointer_v<decltype(MEMBER_FN)>>>
    [[nodiscard]] Tether connect(typename internal::member_fn_traits<decltype(MEMBER_FN)>::class_ptr_type object) const {
        // TODO: prevent re-entrancy here?
        typedef typename internal::member_fn_traits<decltype(MEMBER_FN)>::class_type ClassType;
        auto                                                                         lock = std::lock_guard(mLock);
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
    RET emit(ARGS &&... args) const {
        auto lock = std::lock_guard(mLock);
        if constexpr (std::is_same_v<RET, void>) {
            // For void return, all delegates get lvalue references to avoid moving rvalues multiple times
            for (const auto & delegate : mDelegates) { delegate(static_cast<std::remove_reference_t<ARGS> &>(args)...); }
        } else {
            if (mDelegates.empty()) {
                // Return default-constructed value - enables RVO (returning temporary)
                return RET();
            }
            // For single delegate, forward arguments - enables move semantics for rvalues
            if (mDelegates.size() == 1) { return mDelegates.front()(std::forward<ARGS>(args)...); }
            // For multiple delegates: convert to lvalues to avoid moving rvalues multiple times.
            // All delegates except the last receive lvalue references (safe for multiple calls).
            // The last delegate gets forwarded arguments (can move if rvalues).
            auto it   = mDelegates.begin();
            auto last = std::prev(mDelegates.end());
            for (; it != last; ++it) {
                (void) (*it)(static_cast<std::remove_reference_t<ARGS> &>(args)...); // force lvalue refs
            }
            return (*last)(std::forward<ARGS>(args)...); // forward on last call
        }
    }

    /// this is the operator form of emit().
    template<typename... ARGS>
    RET operator()(ARGS &&... args) const {
        return emit(std::forward<ARGS>(args)...);
    }

private:

    struct Connection {
        DelegateType          delegate;
        std::function<void()> disconnect;
    };


    mutable std::list<Connection> mConnections;
    mutable std::recursive_mutex  mLock;

    Tether addDelegate(DelegateType && delegate) const {
        auto iter = mDelegates.insert(mDelegates.end(), std::move(delegate));
        return Tether(this, std::function<void()>([this, iter]() {
                          auto lock = std::lock_guard(mLock);
                          mDelegates.erase(iter);
                      }));
    }
};

// inline void detail::SignalBase::connectToSlotClass(const SlotBase & slot) const { slot.mSignals.push_back(this); }

// inline void detail::SignalBase::disconnectFromSlotClass(const SlotBase & slot) const { slot.mSignals.remove(this); }

} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SIGSLOT_H__
