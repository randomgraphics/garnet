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
/// \brief   fast delegate class
/// \author  chenlee (2005.5.14)
// *****************************************************************************

namespace GN {

namespace internal {

template<typename T>
class FastDelegateBase;

template<typename RET, typename... PARAMS>
class FastDelegateBase<RET(PARAMS...)> {
protected:
    using StubType = RET (*)(void * this_ptr, PARAMS...);
    struct InvocationElement {
        InvocationElement() = default;
        InvocationElement(void * this_ptr, StubType aStub): object(this_ptr), stub(aStub) {}
        void Clone(InvocationElement & target) const {
            target.stub   = stub;
            target.object = object;
        }                                                                                                                          // Clone
        bool      operator==(const InvocationElement & another) const { return another.stub == stub && another.object == object; } //==
        bool      operator!=(const InvocationElement & another) const { return another.stub != stub || another.object != object; } //!=
        void *    object = nullptr;
        StubType stub   = nullptr;
    }; // InvocationElement
};     // class internal::FastDelegateBase

} // namespace internal

template<typename T>
class FastDelegate;
template<typename T>
class MulticastFastDelegate;

template<typename RET, typename... PARAMS>
class FastDelegate<RET(PARAMS...)> final : private internal::FastDelegateBase<RET(PARAMS...)> {
public:
    FastDelegate() = default;

    FastDelegate(const FastDelegate & another) { another.invocation.Clone(invocation); }

    template<typename LAMBDA>
    FastDelegate(const LAMBDA & lambda) {
        assign((void *) (&lambda), lambdaStub<LAMBDA>);
    }

    FastDelegate & operator=(const FastDelegate & another) {
        another.invocation.Clone(invocation);
        return *this;
    }

    template<typename LAMBDA>
    FastDelegate & operator=(const LAMBDA & instance) {
        assign((void *) (&instance), lambdaStub<LAMBDA>);
        return *this;
    }

    bool empty() const { return invocation.stub == nullptr; }

    bool operator==(void * ptr) const { return (ptr == nullptr) && this->empty(); }

    bool operator!=(void * ptr) const { return (ptr != nullptr) || (!this->empty()); }

    bool operator==(const FastDelegate & another) const { return invocation == another.invocation; }
    bool operator!=(const FastDelegate & another) const { return invocation != another.invocation; }

    bool operator==(const MulticastFastDelegate<RET(PARAMS...)> & another) const { return another == (*this); }
    bool operator!=(const MulticastFastDelegate<RET(PARAMS...)> & another) const { return another != (*this); }

    template<class T, RET (T::*TMethod)(PARAMS...)>
    static FastDelegate create(T * instance) {
        return FastDelegate(instance, methodStub<T, TMethod>);
    }

    template<class T, RET (T::*TMethod)(PARAMS...) const>
    static FastDelegate create(T const * instance) {
        return FastDelegate(const_cast<T *>(instance), constMethodStub<T, TMethod>);
    }

    template<RET (*TMethod)(PARAMS...)>
    static FastDelegate create() {
        return FastDelegate(nullptr, functionStub<TMethod>);
    }

    template<typename LAMBDA>
    static FastDelegate create(const LAMBDA & instance) {
        return FastDelegate((void *) (&instance), lambdaStub<LAMBDA>);
    }

    RET operator()(PARAMS... arg) const { return (*invocation.stub)(invocation.object, arg...); }

private:
    FastDelegate(void * anObject, typename FastDelegateBase<RET(PARAMS...)>::StubType aStub) {
        invocation.object = anObject;
        invocation.stub   = aStub;
    }

    void assign(void * anObject, typename internal::FastDelegateBase<RET(PARAMS...)>::StubType aStub) {
        this->invocation.object = anObject;
        this->invocation.stub   = aStub;
    }

    template<class T, RET (T::*TMethod)(PARAMS...)>
    static RET methodStub(void * this_ptr, PARAMS... params) {
        T * p = static_cast<T *>(this_ptr);
        return (p->*TMethod)(params...);
    }

    template<class T, RET (T::*TMethod)(PARAMS...) const>
    static RET constMethodStub(void * this_ptr, PARAMS... params) {
        T * const p = static_cast<T *>(this_ptr);
        return (p->*TMethod)(params...);
    }

    template<RET (*TMethod)(PARAMS...)>
    static RET functionStub(void * this_ptr, PARAMS... params) {
        return (TMethod) (params...);
    }

    template<typename LAMBDA>
    static RET lambdaStub(void * this_ptr, PARAMS... arg) {
        LAMBDA * p = static_cast<LAMBDA *>(this_ptr);
        return (p->operator())(arg...);
    }

    friend class MulticastFastDelegate<RET(PARAMS...)>;
    typename internal::FastDelegateBase<RET(PARAMS...)>::InvocationElement invocation;
};

template<typename RET, typename... PARAMS>
class MulticastFastDelegate<RET(PARAMS...)> final : private internal::FastDelegateBase<RET(PARAMS...)> {
public:
    MulticastFastDelegate() = default;
    ~MulticastFastDelegate() {
        for (auto & element : invocationList) delete element;
        invocationList.clear();
    } //~MulticastFastDelegate

    bool empty() const { return invocationList.size() < 1; }
    bool operator==(void * ptr) const { return (ptr == nullptr) && this->empty(); }    // operator ==
    bool operator!=(void * ptr) const { return (ptr != nullptr) || (!this->empty()); } // operator !=

    size_t size() const { return invocationList.size(); }

    MulticastFastDelegate & operator=(const MulticastFastDelegate &) = delete;
    MulticastFastDelegate(const MulticastFastDelegate &)             = delete;

    bool operator==(const MulticastFastDelegate & another) const {
        if (invocationList.size() != another.invocationList.size()) return false;
        auto anotherIt = another.invocationList.begin();
        for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
            if (**it != **anotherIt) return false;
        return true;
    } //==
    bool operator!=(const MulticastFastDelegate & another) const { return !(*this == another); }

    bool operator==(const FastDelegate<RET(PARAMS...)> & another) const {
        if (empty() && another.empty()) return true;
        if (another.empty() || (size() != 1)) return false;
        return (another.invocation == **invocationList.begin());
    } //==
    bool operator!=(const FastDelegate<RET(PARAMS...)> & another) const { return !(*this == another); }

    MulticastFastDelegate & operator+=(const MulticastFastDelegate & another) {
        for (auto & item : another.invocationList) // clone, not copy; flattens hierarchy:
            this->invocationList.push_back(new typename internal::FastDelegateBase<RET(PARAMS...)>::InvocationElement(item->object, item->stub));
        return *this;
    } // operator +=

    template<typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
    MulticastFastDelegate & operator+=(const LAMBDA & lambda) {
        FastDelegate<RET(PARAMS...)> d = FastDelegate<RET(PARAMS...)>::template create<LAMBDA>(lambda);
        return *this += d;
    } // operator +=

    MulticastFastDelegate & operator+=(const FastDelegate<RET(PARAMS...)> & another) {
        if (another.empty()) return *this;
        this->invocationList.push_back(
            new typename internal::FastDelegateBase<RET(PARAMS...)>::InvocationElement(another.invocation.object, another.invocation.stub));
        return *this;
    } // operator +=

    // will work even if RET is void, return values are ignored:
    // (for handling return values, see operator(..., handler))
    void operator()(PARAMS... arg) const {
        for (auto & item : invocationList) (*(item->stub))(item->object, arg...);
    } // operator()

    template<typename HANDLER>
    void operator()(PARAMS... arg, HANDLER handler) const {
        size_t index = 0;
        for (auto & item : invocationList) {
            RET value = (*(item->stub))(item->object, arg...);
            handler(index, &value);
            ++index;
        } // loop
    }     // operator()

    void operator()(PARAMS... arg, FastDelegate<void(size_t, RET *)> handler) const { operator()<decltype(handler)>(arg..., handler); }  // operator()
    void operator()(PARAMS... arg, std::function<void(size_t, RET *)> handler) const { operator()<decltype(handler)>(arg..., handler); } // operator()

private:
    std::list<typename internal::FastDelegateBase<RET(PARAMS...)>::InvocationElement *> invocationList;

}; // class MulticastFastDelegate

///
/// Base slot class. Derive your class from this, if you want automatic
/// management of connections between signal and slot.
///
class SlotBase {
protected:
    SlotBase() {}

    virtual ~SlotBase() {
        // disconnect with all signals
        for (SignalContainer::iterator i = mSignals.begin(); i != mSignals.end(); ++i) { (*i)->removeBaseSlotClass(*this); }
        mSignals.clear();
    }

public:
    /** 返回与当前slot连接的信号数 */
    size_t getNumSignals() const { return mSignals.size(); }

private:
    friend class detail::SignalBase;
    typedef std::list<const detail::SignalBase *> SignalContainer;
    mutable SignalContainer                       mSignals;
};

inline void detail::SignalBase::connectToSlotClass(const SlotBase & slot) const { slot.mSignals.push_back(this); }

inline void detail::SignalBase::disconnectFromSlotClass(const SlotBase & slot) const { slot.mSignals.remove(this); }

} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_DELEGATE_H__
