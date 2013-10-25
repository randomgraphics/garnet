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
    if (FAILED(ptr->QueryInterface<QI_CLASS>(&result)))
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
/// Struct that contains all global varialbes used in this library.
struct Global
{
    HookedClassFactory hookedClassFactory;
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
/// Implementation of IUnknown and IHooked
class UnknownBase : public IHooked, public GN::WeakObject
{
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

    UnknownBase() : _refCount(0), _interfaceCount(0)
    {
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

    IUnknown * getHookedInternal(const IID & iid)
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
            return nullptr;
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

protected:

    virtual ~UnknownBase()
    {
        GN_ASSERT(0 == _refCount);

        for(UINT i = 0; i < _interfaceCount; ++i)
        {
            HookedClassFactory::sGetInstance().deleteOld( _interfaces[i].iid, _interfaces[i].hooked );
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
};

// -----------------------------------------------------------------------------
/// Weak refernce to UnknownBase
class WeakUnknownRef : public IUnknown
{
    ULONG                    _refCount;
    GN::WeakRef<UnknownBase> _base;
    CritSec                  _cs;

public:

    WeakUnknownRef()
    {
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

    void setBase(const GN::AutoComPtr<UnknownBase> & base)
    {
        _base.set(base);
    }

    GN::AutoComPtr<UnknownBase> getBase()
    {
        GN::AutoComPtr<UnknownBase> result;
        result.set(_base.rawptr());
        return result;
    }
};


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
            _realPtr = Qi<REAL_INTERFACE>(realobj);
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
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class INPUT_TYPE>
inline IUnknown * RealToHooked(const IID & realIId, INPUT_TYPE * realobj)
{

    // -----------------------------------------------------------------------------
    // {CF9120C7-4E7A-493A-96AA-0C33583803F6}
    /// GUID that is used to attach hooked object pointer to real interface.
    static const GUID HOOKED_OBJECT_GUID =
    { 0xcf9120c7, 0x4e7a, 0x493a, { 0x96, 0xaa, 0xc, 0x33, 0x58, 0x38, 0x3, 0xf6 } };

    if (nullptr == realobj)
    {
        return nullptr;
    }

    if (IsHooked(realobj))
    {
        // Expecting a realobj, not a hooked object.
        GN_UNEXPECTED();
        return realobj;
    }

    GN::AutoComPtr<UnknownBase> base;
    GN::AutoComPtr<WeakUnknownRef> unknownRef;
    UINT size = (UINT)sizeof(unknownRef);
    HRESULT hr = realobj->GetPrivateData(
        HOOKED_OBJECT_GUID,
        &size,
        &unknownRef);
    if (SUCCEEDED(hr))
    {
        base = unknownRef->getBase();
    }

    if (!base)
    {
        base = UnknownBase::sCreateNew(realobj);
        unknownRef.set(new WeakUnknownRef());
        unknownRef->setBase(base);
        realobj->SetPrivateDataInterface(HOOKED_OBJECT_GUID, unknownRef);
    }

    IUnknown * hooked;
    if (SUCCEEDED(base->QueryInterface(realIId, (void**)&hooked)))
    {
        GN_ASSERT(hooked);
        realobj->Release();
        return hooked;
    }
    else
    {
        // Fall back to real object.
        return realobj;
    }
}

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class REAL_INTERFACE>
inline REAL_INTERFACE * RealToHooked(REAL_INTERFACE * realobj)
{
    return (REAL_INTERFACE*)RealToHooked(__uuidof(REAL_INTERFACE), realobj);
}

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
