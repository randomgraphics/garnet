#pragma once

// *****************************************************************************
/// \file
/// \brief   Basic COM interface wrapper
/// \author  chen@@CHENLI-HOMEPC (2007.3.17)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <map>
#include <d3d11.h>
#include "utils.h"

// {CF9120C7-4E7A-493A-96AA-0C33583803F6}
static const GUID GN_D3D11HOOK_HOOKED_OBJECT_GUID =
{ 0xcf9120c7, 0x4e7a, 0x493a, { 0x96, 0xaa, 0xc, 0x33, 0x58, 0x38, 0x3, 0xf6 } };


typedef void* (*HookFactoryFuncPtr)(void * realobj);

template<class HOOKED_CLASS, class REAL_INTERFACE>
inline HOOKED_CLASS * GetHookedInstanceFromReal(REAL_INTERFACE * realobj)
{
    HOOKED_CLASS * hooked;
    UINT size = (UINT)sizeof(hooked);
    GN_VERIFY(S_OK == realobj->GetPrivateData(
        GN_D3D11HOOK_HOOKED_OBJECT_GUID,
        &size,
        &hooked));
    return hooked;
}

///
/// Pointer wrapper that'll nullify itself when constructed.
///
template<typename T>
struct NullPtr
{
    T _value;
    NullPtr() : _value(NULL) {}
};

template<typename HOOK_CLASS>
class UnknownHookBase
{
    const size_t MAX_INTERFACES = 16;

    struct InterfaceInfo
    {
        IID                iid;
        HookFactoryFuncPtr createNew;
    };

    InterfaceInfo _interfaces[MAX_INTERFACES];
    size_t        _count;


public:

    UnknownHookBase() : _count(0)
    {
    }



    void AddInterface(const IID & iid, HookFactoryFuncPtr factory)
    {
        if (_count >= MAX_INTERFACES)
        {
            // should never happen.
            DebugBreak();
            return;
        }

        if (nullptr == factory)
        {
            // should never happen.
            DebugBreak();
            return;
        }

        for(size_t i = 0; i < _count; ++i)
        {
            if (_interfaces.iid == iid)
            {
                // should never happen.
                DebugBreak();
                return;
            }
        }

        _interfaces[_count].iid = iid;
        _interfaces[_count].createNew = factory;
        ++_count;
    }
};

///
/// Hook class that is directly inherited from IUnknown
///
template<class WRAP_CLASS, class REAL_INTERFACE>
class UnknownHook : public REAL_INTERFACE
{
    GN::AutoComPtr<REAL_INTERFACE> _realobj;
    CritSec                        _cs;
    ULONG                          _refcount;
    static std::map<IID,HookFactoryFuncPtr, IIDLess> s_factory;

protected:

    UnknownHook(REAL_INTERFACE * realobj) : _refcount(0)
    {
        _realobj.attach(realobj);
    }

    virtual ~UnknownHook()
    {
    }

public:

    typedef UnknownHook<WRAP_CLASS, REAL_INTERFACE, METHOD_COUNT> IUNKNOWN_BASE_TYPE;
    typedef REAL_INTERFACE REAL_INTERFACE_TYPE;

    // Add interface that this class can QI
    template<class WRAP_CLASS2>
    static void AddQIInterface()
    {
        IID iid = __uuidof(WRAP_CLASS2::REAL_INTERFACE_TYPE);

        if (s_factory.end() == s_factory.find(iid))
        {
            s_factory[iid] = WRAP_CLASS2::CreateInstanceFromRealObj;
        }

        // Add QI to the other class too.
        WRAP_CLASS2::AddQIInterface<WRAP_CLASS>();
    }

    static void * CreateInstanceFromRealObj(void * realobj)
    {
        //GN_ASSERT(realobj != nullptr);
        //WRAP_CLASS * wrapper = new WRAP_CLASS((REAL_INTERFACE*)realobj);
        //wrapper->AddRef();
        //return wrapper;
        GN_UNUSED_PARAM(realobj);
        return nullptr;
    }

    static WRAP_CLASS * CreateTypedInstanceFromRealObj(REAL_INTERFACE * realobj)
    {
        return (WRAP_CLASS*)CreateInstanceFromRealObj(realobj);
    }

    REAL_INTERFACE * GetRealObj() const { GN_ASSERT(_realobj); return _realobj; }

    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
        _cs.Enter();
        ULONG c = ++_refcount;
        _cs.Leave();
        return c;
    }

    virtual ULONG STDMETHODCALLTYPE Release()
    {
        _cs.Enter();
        GN_ASSERT(_refcount > 0);
        ULONG c = --_refcount;
        _cs.Leave();

        if( 0 == c )
        {
            delete this;
        }

        return c;
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject)
    {
        GN_ASSERT( _realobj );

        if( 0 == ppvObject ) return E_INVALIDARG;

        void* realResult;
        HRESULT hr = _realobj->QueryInterface( riid, &realResult );
        if( FAILED(hr) )
        {
            return hr;
        }

        auto iter = s_factory.find( riid );

        if( s_factory.end() != iter )
        {
            *ppvObject = iter->second(realResult);
            return hr;
        }
        else
        {
            // should never reach here.
            GN_UNEXPECTED();
            return E_NOINTERFACE;
        }
    }

    //@}
};

///
/// Hook class that is inherited from single COM interface
///
template<class WRAP_CLASS, class REAL_INTERFACE>
class HookBase : public REAL_INTERFACE
{
    GN::AutoComPtr<REAL_INTERFACE> _realobj;
    CritSec                        _cs;
    ULONG                          _refcount;

protected:

    HookBase(REAL_INTERFACE * realobj) : _refcount(0)
    {
        _realobj.attach(realobj);
    }

    virtual ~HookBase()
    {
    }

public:

    typedef HookBase<WRAP_CLASS, REAL_INTERFACE, METHOD_COUNT> BASE_CLASS;
    typedef REAL_INTERFACE                                        REAL_CLASS;
    REAL_INTERFACE * GetRealObj() const { GN_ASSERT(_realobj); return _realobj; }
};

