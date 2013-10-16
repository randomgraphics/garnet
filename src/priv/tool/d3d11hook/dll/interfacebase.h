#pragma once

// *****************************************************************************
/// \file
/// \brief   Basic COM interface wrapper
/// \author  chen@@CHENLI-HOMEPC (2007.3.17)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <map>
#include <d3d11.h>

// -----------------------------------------------------------------------------
/// GUID for IHooked
const GUID GN_IID_IHook =
{ 0xad5d67ab, 0xc12b, 0x4194, { 0xb5, 0xe8, 0x2f, 0x89, 0x1f, 0x22, 0x12, 0xe6 } };

// -----------------------------------------------------------------------------
/// IHooked interface
MIDL_INTERFACE("AD5D67AB-C12B-4194-B5E8-2F891F2212E6")
IHooked : public IUnknown
{
    // No AddRef()
    virtual void * STDMETHODCALLTYPE GetRealObj(const IID & iid) = 0;
    virtual void * STDMETHODCALLTYPE GetHookedObj(const IID & iid) = 0;
    template<class T> T * GetRealObj() { return (T*)GetRealObj(__uuidof(T)); }
    template<class T> T * GetHookedObj() { return (T*)GetHookedObj(__uuidof(T)); }
};

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
/// Implementation of IUnknown and IHooked
class UnknownBase : public IHooked
{
    struct InterfaceInfo
    {
        IID    iid;
        void * hooked;
        void * real;
    };

    static const UINT MAX_INTERFACES = 16;

    CritSec                  _cs;
    ULONG                    _refCount;
    GN::AutoComPtr<IUnknown> _realUnknown;
    InterfaceInfo            _interfaces[MAX_INTERFACES];
    UINT                     _interfaceCount;

protected:

    UnknownBase() : _refCount(0), _interfaceCount(0)
    {
    }

    virtual ~UnknownBase()
    {
        GN_ASSERT(0 == _refCount);
    }

public:

    void AddInterface(const IID & iid, void * hooked, IUnknown * realUnknown, void * real)
    {
        if (_interfaceCount >= MAX_INTERFACES)
        {
            // should never happen.
            GN_UNEXPECTED();
            return;
        }

        if (!realUnknown || !hooked)
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

        if (!real)
        {
            // this is unusual, but expected.
            return;
        }

        // remember real unknown pointer
        if (!_realUnknown)
        {
            _realUnknown.set( realUnknown );
        }
        else if (realUnknown != _realUnknown)
        {
            // should never happen.
            GN_UNEXPECTED();
            return;
        }

        _interfaces[_interfaceCount].iid    = iid;
        _interfaces[_interfaceCount].hooked = hooked;
        _interfaces[_interfaceCount].real   = real;
        ++_interfaceCount;
    }

    template<class T>
    void AddInterface(void * hooked, IUnknown * real)
    {
        AddInterface(__uuidof(T), hooked, Qi<IUnknown>(real), Qi<T>(real));
    }


    // No AddRef()
    virtual void * STDMETHODCALLTYPE GetRealObj(const IID & iid)
    {
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
    virtual void * STDMETHODCALLTYPE GetHookedObj(const IID & iid)
    {
        for(size_t i = 0; i < _interfaceCount; ++i)
        {
            if (_interfaces[i].iid == iid)
            {
                return _interfaces[i].hooked;
            }
        }
        GN_UNEXPECTED();
        return nullptr;
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

        if (iid == __uuidof(IUnknown))
        {
            *ppvObject = _realUnknown;
            return S_OK;
        }

        if (iid == __uuidof(IHooked))
        {
            *ppvObject = this;
            return S_OK;
        }

        for(UINT i = 0; i < _interfaceCount; ++i)
        {
            const InterfaceInfo & ii = _interfaces[i];
            if (*(UINT64*)&iid == *(UINT64*)&ii.iid)
            {
                GN_ASSERT(ii.real);
                *ppvObject = ii.hooked;
                return S_OK;
            }
        }

        // Double check that all supported interface are added to interface table.
        IUnknown * real;
        if (SUCCEEDED(_realUnknown->QueryInterface( iid, (void**)ppvObject )))
        {
            real->Release();
            GN_UNEXPECTED();
        }

        return E_NOINTERFACE;
    }

    //@}
};

// -----------------------------------------------------------------------------
/// Hook class that is inherited from single COM interface
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
/// Factory to create hooked instance from real object pointer.
class HookedClassFactory
{
public:

    typedef IUnknown * (*FactoryFunc)(void * context, void * realobj);

    static HookedClassFactory & GetInstance() { return s_instance; }

    void Register(const IID & iid, FactoryFunc createNew, void * context)
    {
        FactoryInfo fi;
        fi.context = context;
        fi.createNew = createNew;

        std::pair<FactoryMap::iterator, bool> result = _factories.insert(std::make_pair(iid, fi));

        if (!result.second)
        {
            GN_UNEXPECTED_EX("The factory has been registered.");
        }
    }

    template<class REAL_CLASS>
    void Register(FactoryFunc factory, void * context)
    {
        return Register(__uuidof(REAL_CLASS), factory, context);
    }

    void * CreateNew(const IID & iid, void * realobj)
    {
        FactoryMap::iterator iter = _factories.find(iid);
        if (iter == _factories.end())
        {
            GN_UNEXPECTED();
            return nullptr;
        }
        FactoryInfo & fi = iter->second;
        return fi.createNew(fi.context, realobj);
    }

    template<class REAL_CLASS>
    REAL_CLASS * CreateNew(void * realobj)
    {
        return (REAL_CLASS*)CreateNew(__uuidof(REAL_CLASS), realobj);
    }

private:

    static HookedClassFactory s_instance;

    struct IIDLess
    {
        bool operator()( const IID & a, const IID & b ) const
        {
            return *(UINT64*)&a < *(UINT64*)&b;
        }
    };

    struct FactoryInfo
    {
        void *      context;
        FactoryFunc createNew;
    };

    typedef std::map<IID, FactoryInfo, IIDLess> FactoryMap;
    FactoryMap _factories;
};

// -----------------------------------------------------------------------------
// {CF9120C7-4E7A-493A-96AA-0C33583803F6}
/// GUID that is used to attach hooked object pointer to real interface.
static const GUID GN_D3D11HOOK_HOOKED_OBJECT_GUID =
{ 0xcf9120c7, 0x4e7a, 0x493a, { 0x96, 0xaa, 0xc, 0x33, 0x58, 0x38, 0x3, 0xf6 } };

// -----------------------------------------------------------------------------
/// Retrieve hooked ojbect pointer that is embedded in real D3D object.
template<class REAL_INTERFACE>
inline REAL_INTERFACE * RealToHooked(REAL_INTERFACE * realobj)
{
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

    IHooked * ihooked;
    UINT size = (UINT)sizeof(ihooked);
    HRESULT hr = realobj->GetPrivateData(
        GN_D3D11HOOK_HOOKED_OBJECT_GUID,
        &size,
        &ihooked);
    REAL_INTERFACE * hooked;
    if (SUCCEEDED(hr))
    {
        hooked = ihooked->GetHookedObj<REAL_INTERFACE>();
    }
    else
    {
        // This is a new D3D object that we never saw before. Create a new
        // hooked object for it.
        hooked = HookedClassFactory::GetInstance().CreateNew<REAL_INTERFACE>(realobj);
    }

    hooked->AddRef();
    realobj->Release();
    return hooked;
}

// -----------------------------------------------------------------------------
/// Convert real object poiner to hooked ojbect pointer
inline void* DXGIRealToHooked(const IID & iid, void * realobj)
{
    if (nullptr == realobj)
    {
        return nullptr;
    }

    if (IsHooked((IUnknown*)realobj))
    {
        // Expecting a realobj, not a hooked object.
        GN_UNEXPECTED();
        return realobj;
    }

    GN::AutoComPtr<IDXGIObject> dxgiobj = Qi<IDXGIObject>((IUnknown*)realobj);

    IHooked * ihooked;
    UINT size = (UINT)sizeof(ihooked);
    HRESULT hr = dxgiobj->GetPrivateData(
        GN_D3D11HOOK_HOOKED_OBJECT_GUID,
        &size,
        &ihooked);
    void *hooked;
    if (SUCCEEDED(hr))
    {
        hooked = ihooked->GetHookedObj(iid);
    }
    else
    {
        hooked = HookedClassFactory::GetInstance().CreateNew(iid, realobj);
    }

    ((IUnknown*)hooked)->AddRef();
    ((IUnknown*)realobj)->Release();

    return hooked;
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
        GN_UNEXPECTED();
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
