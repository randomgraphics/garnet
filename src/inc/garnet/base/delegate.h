/*
    Derived from the work by Sergey A Kryukov: "The Impossibly Fast C++ Delegates, Fixed", 2017
    https://www.codeproject.com/articles/The-Impossibly-Fast-Cplusplus-Delegates-Fixed,

    which is then based on the original work of Sergey Ryazanov: "The Impossibly Fast C++ Delegates", 18 Jul 2005
    https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

    MIT license:
    http://en.wikipedia.org/wiki/MIT_License
*/

#ifndef __GN_BASE_DELEGATE_H__
#define __GN_BASE_DELEGATE_H__
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
    using stub_type = RET (*)(void * this_ptr, PARAMS...);

    struct InvocationElement {
        InvocationElement() = default;
        InvocationElement(void * this_ptr, stub_type aStub): object(this_ptr), stub(aStub) {}
        void Clone(InvocationElement & target) const {
            target.stub   = stub;
            target.object = object;
        }                                                                                                                          // Clone
        bool      operator==(const InvocationElement & another) const { return another.stub == stub && another.object == object; } //==
        bool      operator!=(const InvocationElement & another) const { return another.stub != stub || another.object != object; } //!=
        void *    object = nullptr;
        stub_type stub   = nullptr;
    }; // InvocationElement

}; // class delegate_base

template<typename T>
class delegate;
template<typename T>
class multicast_delegate;

template<typename RET, typename... PARAMS>
class delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
public:
    delegate() = default;

    bool isNull() const { return invocation.stub == nullptr; }
    bool operator==(void * ptr) const { return (ptr == nullptr) && this->isNull(); }    // operator ==
    bool operator!=(void * ptr) const { return (ptr != nullptr) || (!this->isNull()); } // operator !=

    delegate(const delegate & another) { another.invocation.Clone(invocation); }

    template<typename LAMBDA>
    delegate(const LAMBDA & lambda) {
        assign((void *) (&lambda), lambda_stub<LAMBDA>);
    } // delegate

    delegate & operator=(const delegate & another) {
        another.invocation.Clone(invocation);
        return *this;
    } // operator =

    template<typename LAMBDA> // template instantiation is not needed, will be deduced (inferred):
    delegate & operator=(const LAMBDA & instance) {
        assign((void *) (&instance), lambda_stub<LAMBDA>);
        return *this;
    } // operator =

    bool operator==(const delegate & another) const { return invocation == another.invocation; }
    bool operator!=(const delegate & another) const { return invocation != another.invocation; }

    bool operator==(const multicast_delegate<RET(PARAMS...)> & another) const { return another == (*this); }
    bool operator!=(const multicast_delegate<RET(PARAMS...)> & another) const { return another != (*this); }

    template<class T, RET (T::*TMethod)(PARAMS...)>
    static delegate create(T * instance) {
        return delegate(instance, method_stub<T, TMethod>);
    } // create

    template<class T, RET (T::*TMethod)(PARAMS...) const>
    static delegate create(T const * instance) {
        return delegate(const_cast<T *>(instance), const_method_stub<T, TMethod>);
    } // create

    template<RET (*TMethod)(PARAMS...)>
    static delegate create() {
        return delegate(nullptr, function_stub<TMethod>);
    } // create

    template<typename LAMBDA>
    static delegate create(const LAMBDA & instance) {
        return delegate((void *) (&instance), lambda_stub<LAMBDA>);
    } // create

    RET operator()(PARAMS... arg) const { return (*invocation.stub)(invocation.object, arg...); } // operator()

private:
    delegate(void * anObject, typename delegate_base<RET(PARAMS...)>::stub_type aStub) {
        invocation.object = anObject;
        invocation.stub   = aStub;
    } // delegate

    void assign(void * anObject, typename delegate_base<RET(PARAMS...)>::stub_type aStub) {
        this->invocation.object = anObject;
        this->invocation.stub   = aStub;
    } // assign

    template<class T, RET (T::*TMethod)(PARAMS...)>
    static RET method_stub(void * this_ptr, PARAMS... params) {
        T * p = static_cast<T *>(this_ptr);
        return (p->*TMethod)(params...);
    } // method_stub

    template<class T, RET (T::*TMethod)(PARAMS...) const>
    static RET const_method_stub(void * this_ptr, PARAMS... params) {
        T * const p = static_cast<T *>(this_ptr);
        return (p->*TMethod)(params...);
    } // const_method_stub

    template<RET (*TMethod)(PARAMS...)>
    static RET function_stub(void * this_ptr, PARAMS... params) {
        return (TMethod) (params...);
    } // function_stub

    template<typename LAMBDA>
    static RET lambda_stub(void * this_ptr, PARAMS... arg) {
        LAMBDA * p = static_cast<LAMBDA *>(this_ptr);
        return (p->operator())(arg...);
    } // lambda_stub

    friend class multicast_delegate<RET(PARAMS...)>;
    typename delegate_base<RET(PARAMS...)>::InvocationElement invocation;

}; // class delegate

// template<typename RET, typename... PARAMS>
// class multicast_delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
// public:
//     multicast_delegate() = default;
//     ~multicast_delegate() {
//         for (auto & element : invocationList) delete element;
//         invocationList.clear();
//     } //~multicast_delegate

//     bool isNull() const { return invocationList.size() < 1; }
//     bool operator==(void * ptr) const { return (ptr == nullptr) && this->isNull(); }    // operator ==
//     bool operator!=(void * ptr) const { return (ptr != nullptr) || (!this->isNull()); } // operator !=

//     size_t size() const { return invocationList.size(); }

//     multicast_delegate & operator=(const multicast_delegate &) = delete;
//     multicast_delegate(const multicast_delegate &)             = delete;

//     bool operator==(const multicast_delegate & another) const {
//         if (invocationList.size() != another.invocationList.size()) return false;
//         auto anotherIt = another.invocationList.begin();
//         for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
//             if (**it != **anotherIt) return false;
//         return true;
//     } //==
//     bool operator!=(const multicast_delegate & another) const { return !(*this == another); }

//     bool operator==(const delegate<RET(PARAMS...)> & another) const {
//         if (isNull() && another.isNull()) return true;
//         if (another.isNull() || (size() != 1)) return false;
//         return (another.invocation == **invocationList.begin());
//     } //==
//     bool operator!=(const delegate<RET(PARAMS...)> & another) const { return !(*this == another); }

//     multicast_delegate & operator+=(const multicast_delegate & another) {
//         for (auto & item : another.invocationList) // clone, not copy; flattens hierarchy:
//             this->invocationList.push_back(new typename delegate_base<RET(PARAMS...)>::InvocationElement(item->object, item->stub));
//         return *this;
//     } // operator +=

//     template<typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
//     multicast_delegate & operator+=(const LAMBDA & lambda) {
//         delegate<RET(PARAMS...)> d = delegate<RET(PARAMS...)>::template create<LAMBDA>(lambda);
//         return *this += d;
//     } // operator +=

//     multicast_delegate & operator+=(const delegate<RET(PARAMS...)> & another) {
//         if (another.isNull()) return *this;
//         this->invocationList.push_back(new typename delegate_base<RET(PARAMS...)>::InvocationElement(another.invocation.object, another.invocation.stub));
//         return *this;
//     } // operator +=

//     // will work even if RET is void, return values are ignored:
//     // (for handling return values, see operator(..., handler))
//     void operator()(PARAMS... arg) const {
//         for (auto & item : invocationList) (*(item->stub))(item->object, arg...);
//     } // operator()

//     template<typename HANDLER>
//     void operator()(PARAMS... arg, HANDLER handler) const {
//         size_t index = 0;
//         for (auto & item : invocationList) {
//             RET value = (*(item->stub))(item->object, arg...);
//             handler(index, &value);
//             ++index;
//         } // loop
//     }     // operator()

//     void operator()(PARAMS... arg, delegate<void(size_t, RET *)> handler) const { operator()<decltype(handler)>(arg..., handler); }      // operator()
//     void operator()(PARAMS... arg, std::function<void(size_t, RET *)> handler) const { operator()<decltype(handler)>(arg..., handler); } // operator()

// private:
//     std::list<typename delegate_base<RET(PARAMS...)>::InvocationElement *> invocationList;

// }; // class multicast_delegate

/// Base class of all signals.
class SignalBase {
protected:
    SignalBase()          = default;
    virtual ~SignalBase() = default;
};
    
} // namespace internal

///
/// Represents a connection between a signal and a slot. Destructing this object will disconnect the slot from the signal.
///
class Tether {
public:
    Tether() = default;

    Tether(const internal::SignalBase * signal, std::function<void()> && disconnFunc) : mSignal(signal), mDisconnFunc(std::move(disconnFunc)) {}

    ~Tether() {
        mSignal = nullptr;
        mDisconnFunc = nullptr;
    }

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

    GN_DEFAULT_MOVE(SlotBase);

public:
    void manageTether(Tether && t) const {
        auto lock = std::lock_guard(mLock);
        mTethers.push_back(std::move(t));
    }

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

    [[nodiscard]] Tether connect(RET (*staticFuncPtr)(PARAMS...)) const {
        auto lock = std::lock_guard(mLock);
        return addDelegate(DelegateType::create(staticFuncPtr));
    }


    template<typename CLASS, RET (CLASS::*METHOD)(PARAMS...)>
    [[nodiscard]] Tether connect(CLASS * classPtr) const {
        auto lock = std::lock_guard(mLock);
        (void)classPtr;
        return {}; // addDelegate(DelegateType::create<CLASS, METHOD>(classPtr));
    }

    template<typename CLASS, RET (CLASS::*METHOD)(PARAMS...) const>
    [[nodiscard]] Tether connect(const CLASS * classPtr) const {
        auto lock = std::lock_guard(mLock);
        (void)classPtr;
        return {}; //return addDelegate(DelegateType::create<const CLASS, METHOD>(classPtr));
    }

    template<typename... ARGS>
    RET emit(ARGS &&...) const {
        return RET();
    }

    template<typename... ARGS>
    RET operator()(ARGS &&... args) const {
        return emit(std::forward<ARGS>(args)...);
    }

private:
    template <typename>
    struct member_fn_traits; // primary

    // non-const
    template <typename C, typename R, typename... A>
    struct member_fn_traits<R (C::*)(A...)> {
        using class_type  = C;
        using return_type = R;
    };

    // const
    template <typename C, typename R, typename... A>
    struct member_fn_traits<R (C::*)(A...) const> {
        using class_type  = C;
        using return_type = R;
    };

    // ref-qualifiers
    template <typename C, typename R, typename... A>
    struct member_fn_traits<R (C::*)(A...) &> : member_fn_traits<R (C::*)(A...)> {};

    template <typename C, typename R, typename... A>
    struct member_fn_traits<R (C::*)(A...) const&> : member_fn_traits<R (C::*)(A...) const> {};

    template <typename C, typename R, typename... A>
    struct member_fn_traits<R (C::*)(A...) &&> : member_fn_traits<R (C::*)(A...)> {};

    template <typename C, typename R, typename... A>
    struct member_fn_traits<R (C::*)(A...) const&&> : member_fn_traits<R (C::*)(A...) const> {};

    // // noexcept variants (add if you use them)
    // template <typename C, typename R, typename... A>
    // struct member_fn_traits<R (C::*)(A...) noexcept> : member_fn_traits<R (C::*)(A...)> {};

    // template <typename C, typename R, typename... A>
    // struct member_fn_traits<R (C::*)(A...) const noexcept> : member_fn_traits<R (C::*)(A...) const> {}

    mutable std::list<DelegateType>            mDelegates;
    mutable std::recursive_mutex               mLock;

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
#endif // __GN_BASE_DELEGATE_H__
