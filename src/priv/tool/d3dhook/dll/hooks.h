#pragma once

// *****************************************************************************
/// \file
/// \brief   Common header of hook dll (must be API neutral)
/// \author  chen@@CHENLI-HOMEPC (2007.3.17)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <map>

#define HOOK_ERROR_LOG GN_ERROR(GN::getLogger("GN.d3dhook"))
#define HOOK_INFO_LOG  GN_INFO(GN::getLogger("GN.d3dhook"))
#define HOOK_WARN_LOG  GN_WARN(GN::getLogger("GN.d3dhook"))

// -----------------------------------------------------------------------------
template<class QI_CLASS, class CURRENT_CLASS>
inline GN::AutoComPtr<QI_CLASS> Qi(CURRENT_CLASS * ptr)
{
    GN::AutoComPtr<QI_CLASS> result;
    if (FAILED(ptr->QueryInterface(__uuidof(QI_CLASS), (void**)&result)))
    {
        result = nullptr;
    }
    return result;
}

// -----------------------------------------------------------------------------
/// Pointer wrapper that'll nullify itself when constructed.
template<typename T>
struct NullPtr
{
    T _value;
    NullPtr() : _value(NULL) {}

    NullPtr & operator=( const T & t )
    {
        _value = t;
        return *this;
    }
};

// -----------------------------------------------------------------------------
// Critical Section c++ wrapper
class CritSec
{
public:

    CritSec()
    {
        InitializeCriticalSection(&_cs);
    }

    ~CritSec()
    {
        DeleteCriticalSection(&_cs);
    }

    CRITICAL_SECTION * GetRawCS() const
    {
        return &_cs;
    }

    BOOL TryEnter() const
    {
        return TryEnterCriticalSection(&_cs);
    }

    void Enter() const
    {
        EnterCriticalSection(&_cs);
    }

    void Leave() const
    {
        LeaveCriticalSection(&_cs);
    }

    // This class works with both CritSec and raw CRITICAL_SECTION
    class AutoLock
    {
        CRITICAL_SECTION * _pcs;

    public:

        explicit AutoLock(CRITICAL_SECTION & cs)
            : _pcs(&cs)
        {
            if(NULL != _pcs)
            {
                EnterCriticalSection(_pcs);
            }
        }

        explicit AutoLock(CritSec & cs)
            : _pcs(cs.GetRawCS())
        {
            if(NULL != _pcs)
            {
                EnterCriticalSection(_pcs);
            }
        }

        ~AutoLock()
        {
            Unlock();
        }

        void Unlock()
        {
            if (NULL != _pcs)
            {
                LeaveCriticalSection(_pcs);
                _pcs = NULL;
            }
        }
    };

private:

    mutable CRITICAL_SECTION _cs;
};

// -----------------------------------------------------------------------------
/// Descriptor of an interface
struct InterfaceDesc
{
    IID             iid;
    const char    * name;
    const wchar_t * namew;

    static const InterfaceDesc * sGetDescFromIID(const IID &);

    static inline const char * sIIDToInterfaceName(const IID & iid)
    {
        const InterfaceDesc * id = sGetDescFromIID(iid);
        return id ? id->name : ((GN::Guid*)&iid)->toStr();
    }
};

class UnknownBase;

// -----------------------------------------------------------------------------
/// Factory to create hooked instance from real object pointer.
class HookedClassFactory
{
public:

    typedef IUnknown * (*NewInstanceFunc)(void * context, UnknownBase & base, IUnknown * realobj);
    typedef void  (*DeleteInstanceFunc)(void * context, void * ptr);

    static HookedClassFactory & sGetInstance() { return s_instance; }

    void registerAll(); // implementation of this method script generated

    // Create new hooked instance for a particular real object.
    IUnknown * createNew(UnknownBase & base, const IID & iid, IUnknown * realobj)
    {
        FactoryMap::iterator iter = _factories.find(iid);
        if (iter == _factories.end())
        {
            HOOK_ERROR_LOG("Class factory for interface %s is not registered.", InterfaceDesc::sIIDToInterfaceName(iid));
            return nullptr;
        }
        FactoryInfo & fi = iter->second;

        return fi.createFunc(fi.context, base, realobj);
    }

    void deleteOld(const IID & iid, void * ptr)
    {
        FactoryMap::iterator iter = _factories.find(iid);
        if (iter == _factories.end())
        {
            // should never reach here
            GN_UNEXPECTED();
        }
        else
        {
            FactoryInfo & fi = iter->second;
            fi.deleteFunc(fi.context, ptr);
        }
    }

    template<class REAL_CLASS>
    REAL_CLASS * createNew(UnknownBase & base, REAL_CLASS * realobj)
    {
        return (REAL_CLASS*)createNew(UnknownBase & base, __uuidof(REAL_CLASS), realobj);
    }

private:

    void registerFactory(const IID & iid, NewInstanceFunc createFunc, DeleteInstanceFunc deleteFunc, void * context)
    {
        FactoryInfo fi;
        fi.context = context;
        fi.createFunc = createFunc;
        fi.deleteFunc = deleteFunc;

        std::pair<FactoryMap::iterator, bool> result = _factories.insert(std::make_pair(iid, fi));

        if (!result.second)
        {
            GN_UNEXPECTED_EX("The factory has been registered.");
        }
    }

    template<class REAL_CLASS>
    void registerFactory(NewInstanceFunc createFunc, DeleteInstanceFunc deleteFunc, void * context)
    {
        return registerFactory(__uuidof(REAL_CLASS), createFunc, deleteFunc, context);
    }

private:

    static HookedClassFactory s_instance;

    struct IIDLess
    {
        bool operator()( const IID & a, const IID & b ) const
        {
            const UINT64 * u1 = (UINT64*)&a;
            const UINT64 * u2 = (UINT64*)&b;
            if (u1[0] < u2[0])
                return true;
            else if (u1[0] > u2[0])
                return false;
            else
                return u1[1] < u2[1];
        }
    };

    struct FactoryInfo
    {
        void *             context;
        NewInstanceFunc    createFunc;
        DeleteInstanceFunc deleteFunc;
    };

    typedef std::map<IID, FactoryInfo, IIDLess> FactoryMap;
    FactoryMap _factories;
};

// -----------------------------------------------------------------------------
/// IHooked interface
MIDL_INTERFACE("AD5D67AB-C12B-4194-B5E8-2F891F2212E6")
IHooked : public IUnknown
{
    // No AddRef()
    virtual IUnknown * STDMETHODCALLTYPE GetRealObj(const IID & iid) = 0;
    virtual IUnknown * STDMETHODCALLTYPE GetHookedObj(const IID & iid) = 0;
    template<class T> T * GetRealObj() { return (T*)GetRealObj(__uuidof(T)); }
};

// -----------------------------------------------------------------------------
/// Check if the pointer is pointing to a hooked instance.
inline bool IsHooked(IUnknown * ptr)
{
    GN_ASSERT(ptr);
    GN::AutoComPtr<IHooked> ihook = Qi<IHooked>(ptr);
    return nullptr != ihook;
}

// -----------------------------------------------------------------------------
/// Check if the pointer is pointing to a real (non-hooked) instance.
inline bool IsReal(IUnknown * ptr)
{
    return !IsHooked(ptr);
}

// -----------------------------------------------------------------------------
/// The lock object for thread safe reference management.
struct WeakRefTracker : GN::RefCounter
{
    GN::Mutex       lock;
    UnknownBase *   base;     // pointer to the object that we are referencing to
    GN::DoubleLink  weakRefs; // double linked list of all weak references.
};

// -----------------------------------------------------------------------------
/// Implementation of IUnknown and IHooked
class UnknownBase : public IHooked
{
protected:

    virtual ~UnknownBase()
    {
        GN_ASSERT(0 == _refCount);
        GN_ASSERT(0 == _weakRefTracker->base);
        GN_ASSERT(0 == _weakRefTracker->weakRefs.next);
        GN_ASSERT(0 == _weakRefTracker->weakRefs.prev);
        for(UINT i = 0; i < _interfaceCount; ++i)
        {
            HookedClassFactory::sGetInstance().deleteOld( _interfaces[i].iid, _interfaces[i].hooked );
        }
        if (_destructNotif)
        {
            _destructNotif(this, _destructNotifContext);
        }
    }

public:

    static GN::AutoComPtr<UnknownBase> sCreateNew(IUnknown * realobj)
    {
        GN_ASSERT(realobj);
        GN::AutoComPtr<UnknownBase> p;
        try
        {
            p.set(new UnknownBase());
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory");
        }
        if (p)
        {
            p->_realUnknown = Qi<IUnknown>(realobj);
        }
        return p;
    }

    typedef void (*DestructNotif)(UnknownBase * base, void * context);

    void setDestructNotif(DestructNotif notif, void * context)
    {
        _destructNotif = notif;
        _destructNotifContext = context;
    }

    GN::AutoRef<WeakRefTracker> getWeakRefTracker() const { return _weakRefTracker; }

    // No AddRef()
    virtual IUnknown * STDMETHODCALLTYPE GetRealObj(const IID & iid)
    {
        if (__uuidof(IUnknown) == iid)
        {
            return _realUnknown;
        }
        for(size_t i = 0; i < _interfaceCount; ++i)
        {
            if (_interfaces[i].iid == iid)
            {
                return _interfaces[i].real;
            }
        }
        GN_UNEXPECTED();
        return nullptr;
    }

    /// This method is doing basically samething as GetHookedObj(), expect that it passes in
    /// the real D3D object pointer. This is to support scenario that sometimes, a D3D9
    /// interface A, although it is inherited from B, does not actually support QI of B. When
    /// it happens, getHookedInternal() will use the passed-in pointer, instead of the one from
    /// QI.
    ///
    /// This happens for the BackBuffer object returned by IDirect3DDevice9::GetBackbuffer().
    IUnknown * GetHookedParent(const IID & iid, IUnknown * real)
    {
        IUnknown * hooked = getHookedInternal(iid, real);
        if (nullptr == hooked)
        {
            GN_UNEXPECTED();
        }
        return hooked;
    }

    // No AddRef()
    virtual IUnknown * STDMETHODCALLTYPE GetHookedObj(const IID & iid)
    {
        IUnknown * hooked = getHookedInternal(iid);
        if (nullptr == hooked)
        {
            GN_UNEXPECTED();
        }
        return hooked;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
        _cs.Enter();
        ULONG c = ++_refCount;
        _cs.Leave();
        return c;
    }

    virtual ULONG STDMETHODCALLTYPE Release()
    {
        _cs.Enter();
        _weakRefTracker->lock.lock();
        GN_ASSERT(_refCount > 0);
        ULONG c = --_refCount;
        // within the locks, we need to invalidate all weak references, if refcount reaches zero.
        if( 0 == c )
        {
            invalidateAllWeakRefs();
        }
        _weakRefTracker->lock.unlock();
        _cs.Leave();

        if( 0 == c )
        {
            delete this;
        }

        return c;
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID & iid, void **ppvObject)
    {
        if( 0 == ppvObject ) return E_INVALIDARG;

        if (iid == __uuidof(IHooked))
        {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        else
        {
            IUnknown * hooked = getHookedInternal(iid);
            if (hooked)
            {
                *ppvObject = hooked;
                AddRef();
                return S_OK;
            }
        }

        return E_NOINTERFACE;
    }

    //@}

private:

    struct InterfaceInfo
    {
        IID        iid;
        IUnknown * hooked;
        IUnknown * real;
    };

    static const UINT MAX_INTERFACES = 16;

    CritSec                  _cs;
    ULONG                    _refCount;
    InterfaceInfo            _interfaces[MAX_INTERFACES];
    UINT                     _interfaceCount;
    GN::AutoComPtr<IUnknown> _realUnknown;
    DestructNotif            _destructNotif;
    void *                   _destructNotifContext;

    // For weak references
    GN::AutoRef<WeakRefTracker> _weakRefTracker;

private:

    UnknownBase() : _refCount(0), _interfaceCount(0), _destructNotif(nullptr)
    {
        _weakRefTracker = GN::referenceTo(new WeakRefTracker());
        _weakRefTracker->base = this;
    }

    void addInterface(const IID & iid, IUnknown * hooked, IUnknown * real)
    {
        if (_interfaceCount >= MAX_INTERFACES)
        {
            // should never happen.
            GN_UNEXPECTED();
            return;
        }

        if (!hooked || !real)
        {
            // should never happen.
            GN_UNEXPECTED();
            return;
        }

        // check if the inteface has been added already.
        for(size_t i = 0; i < _interfaceCount; ++i)
        {
            if (_interfaces[i].iid == iid)
            {
                // should never happen.
                GN_UNEXPECTED();
                return;
            }
        }

        _interfaces[_interfaceCount].iid = iid;
        _interfaces[_interfaceCount].hooked = hooked;
        _interfaces[_interfaceCount].real = real;
        ++_interfaceCount;
    }

    template<class T>
    void addInterface(IUnknown * hooked, IUnknown * real)
    {
        addInterface(__uuidof(T), hooked, Qi<T>(real));
    }

    IUnknown * getHookedInternal(const IID & iid, IUnknown * useThisPointerWhenQiFromRealUnknownFailed = nullptr)
    {
        if (iid == __uuidof(IUnknown)) return this;

        for(size_t i = 0; i < _interfaceCount; ++i)
        {
            if (_interfaces[i].iid == iid)
            {
                return _interfaces[i].hooked;
            }
        }

        GN::AutoComPtr<IUnknown> real;
        if (FAILED(_realUnknown->QueryInterface( iid, (void**)&real )))
        {
            if (useThisPointerWhenQiFromRealUnknownFailed)
            {
                real.set(useThisPointerWhenQiFromRealUnknownFailed);
            }
            else
            {
                return nullptr;
            }
        }

        IUnknown * hooked = HookedClassFactory::sGetInstance().createNew(*this, iid, real);
        if (hooked)
        {
            addInterface( iid, hooked, real);
            return hooked;
        }
        else
        {
            HOOK_ERROR_LOG(
                "%s is supported by real D3D interface, but _NOT_ in our hooked system.",
                InterfaceDesc::sIIDToInterfaceName(iid));
            return nullptr;
        }
    }

    void invalidateAllWeakRefs();

};

// -----------------------------------------------------------------------------
/// Weak refernce to UnknownBase
class WeakUnknownRef : public IUnknown
{
    ULONG                        _refCount;
    CritSec                      _cs;

    GN::AutoRef<WeakRefTracker>  _tracker;
    GN::DoubleLink               _link;

public:

    WeakUnknownRef() : _refCount(0)
    {
    }

    ~WeakUnknownRef()
    {
        detach();
    }

    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
        _cs.Enter();
        ULONG c = ++_refCount;
        _cs.Leave();
        return c;
    }

    virtual ULONG STDMETHODCALLTYPE Release()
    {
        _cs.Enter();
        GN_ASSERT(_refCount > 0);
        ULONG c = --_refCount;
        _cs.Leave();

        if( 0 == c )
        {
            delete this;
        }

        return c;
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID & iid, void **ppvObject)
    {
        if( 0 == ppvObject ) return E_INVALIDARG;
        if (__uuidof(IUnknown) == iid)
        {
            *ppvObject = this;
            return S_OK;
        }
        else
        {
            GN_UNEXPECTED(); // should never reach here.
            return E_NOINTERFACE;
        }
    }

    void attach(UnknownBase * base)
    {
        detach();
        if( base )
        {
            _tracker = base->getWeakRefTracker();
            GN_ASSERT(_tracker->base == base);
            _tracker->lock.lock();
            _link.linkAfter( &_tracker->weakRefs );
            _link.context = this;
            _tracker->lock.unlock();
        }
    }

    void detach()
    {
        if( _tracker )
        {
            _tracker->lock.lock();
            _link.detach();
            _tracker->lock.unlock();
            _tracker.clear();
        }
        GN_ASSERT( NULL == _tracker );
        GN_ASSERT( NULL == _link.prev );
        GN_ASSERT( NULL == _link.next );
    }

    // Promote weak reference to strong reference (might return null)
    GN::AutoComPtr<UnknownBase> promote()
    {
        GN::AutoComPtr<UnknownBase> result;
        if( _tracker )
        {
            _tracker->lock.lock();      // this lock prevents base from being released.
            result.set(_tracker->base); // this will increase base ref count by one.
            _tracker->lock.unlock();
        }
        return result;
    }
};

//
//
// -----------------------------------------------------------------------------
inline void UnknownBase::invalidateAllWeakRefs()
{
    GN::DoubleLink * next;
    WeakUnknownRef * ref;
    while( NULL != (next = _weakRefTracker->weakRefs.next) )
    {
        ref = (WeakUnknownRef*)next->context;
        ref->detach();
    }
    _weakRefTracker->base = nullptr;
}

// -----------------------------------------------------------------------------
/// Hook class that is inherited from single COM interface
///
/// Note: although it is inherited from IUnknown, life time of this class is _NOT_
/// tracked by reference counting.
template<class REAL_INTERFACE>
class HookBase : public REAL_INTERFACE
{
    UnknownBase                  & _unknown;
    GN::AutoComPtr<REAL_INTERFACE> _realPtr;

protected:

    HookBase(UnknownBase & unknown, IUnknown * realobj)
        : _unknown(unknown)
    {
        if (IsHooked(realobj))
        {
            GN_UNEXPECTED();
        }
        else
        {
            _realPtr.set((REAL_INTERFACE*)realobj);
        }
    }

    virtual ~HookBase()
    {
    }

public:

    typedef HookBase<REAL_INTERFACE> BASE_CLASS;
    typedef REAL_INTERFACE           REAL_CLASS;

    // No AddRef()
    REAL_INTERFACE * GetRealObj() const
    {
        GN_ASSERT(_realPtr);
        return _realPtr;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef() { return _unknown.AddRef(); }

    virtual ULONG STDMETHODCALLTYPE Release() { return _unknown.Release(); }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID & iid, void **ppvObject) { return _unknown.QueryInterface(iid, ppvObject); }
};

// -----------------------------------------------------------------------------
/// Convert pointer to hooked instance (in real object type though) to pointer
/// to real D3D object.
template<class REAL_INTERFACE>
inline REAL_INTERFACE * HookedToReal(REAL_INTERFACE * hooked)
{
    if (nullptr == hooked)
    {
        return nullptr;
    }

    GN::AutoComPtr<IHooked> ihook = Qi<IHooked>(hooked);
    if (nullptr == ihook)
    {
        // hooked is pointing to real object.
        //GN_UNEXPECTED();
        return hooked;
    }

    return ihook->GetRealObj<REAL_INTERFACE>();
}

// -----------------------------------------------------------------------------
/// Convert pointer to hooked instance (in real object type though) to pointer
/// to real D3D object.

template<class REAL_INTERFACE>
class HookedToRealArray
{
    REAL_INTERFACE * _items[128];
    size_t           _count;

public:

    HookedToRealArray(size_t count, REAL_INTERFACE * const * hooked)
    {
        if (count > _countof(_items))
        {
            GN_UNEXPECTED();
            count = _countof(_items);
        }

        for(size_t i = 0; i < count; ++i)
        {
            _items[i] = HookedToReal(hooked[i]);
        }

        _count = count;
    }

    ~HookedToRealArray()
    {
    }

    operator REAL_INTERFACE** ()
    {
        return _items;
    }
};

template<class REAL_INTERFACE>
inline HookedToRealArray<REAL_INTERFACE> HookedToReal(size_t count, REAL_INTERFACE * const * hooked)
{
    HookedToRealArray<REAL_INTERFACE> result(count, hooked);
    return result;
}

// -----------------------------------------------------------------------------
// common call trace utilities
// -----------------------------------------------------------------------------

namespace calltrace
{
    /// return call level (including current call)
    int enter(const wchar_t * text);

    /// return call level (including current call)
    int enter(const char * text);

    void leave();

    class AutoTrace
    {
        int _level;

    public:

        AutoTrace(const wchar_t * text) : _level(enter(text))
        {
        }

        AutoTrace(const char * text) : _level(enter(text))
        {
        }

        ~AutoTrace()
        {
            leave();
        }

        int getCurrentLevel() const { return _level; }
    };
};
