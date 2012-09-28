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

typedef void* (*WrapRealObjectFuncPtr)(void * realobj);

///
/// Wrapper of classes that is directly inherited from IUnknown
///
template<class WRAP_CLASS, class REAL_INTERFACE, UINT METHOD_COUNT>
class UnknownHook : public REAL_INTERFACE
{
    struct IIDLess
    {
        bool operator()( const IID & a, const IID & b ) const
        {
            return ::memcmp( &a, &b, sizeof(a) ) < 0;
        }
    };

    GN::AutoComPtr<REAL_INTERFACE>               _realobj;
    CritSec                                      _cs;
    ULONG                                        _refcount;

    static std::map<IID,WrapRealObjectFuncPtr, IIDLess> s_factory;

protected:

    template<typename T>
    struct NullPtr
    {
        T _value;

        NullPtr() : _value(NULL) {}
    };

    typedef void (WRAP_CLASS::*METHOD_FUNC_PTR)();

    METHOD_FUNC_PTR _preFuncPtr[METHOD_COUNT];
    METHOD_FUNC_PTR _postFuncPtr[METHOD_COUNT];

    UnknownHook(REAL_INTERFACE * realobj) : _refcount(0)
    {
        _realobj.attach(realobj);
        memset(_preFuncPtr, 0, sizeof(_preFuncPtr));
        memset(_postFuncPtr, 0, sizeof(_postFuncPtr));
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

// {CF9120C7-4E7A-493A-96AA-0C33583803F6}
static const GUID GN_D3D11HOOK_HOOKED_OBJECT_GUID =
{ 0xcf9120c7, 0x4e7a, 0x493a, { 0x96, 0xaa, 0xc, 0x33, 0x58, 0x38, 0x3, 0xf6 } };

///
/// Wrapper of classes that is directly inherited from IUnknown
///
template<class WRAP_CLASS, class REAL_INTERFACE, UINT METHOD_COUNT>
class D3D11DeviceChildHook : public UnknownHook<WRAP_CLASS, REAL_INTERFACE, METHOD_COUNT>
{

protected:

    D3D11DeviceChildHook(REAL_INTERFACE * realobj) : IUNKNOWN_BASE_TYPE(realobj)
    {
    }

    virtual ~D3D11DeviceChildHook()
    {
    }

public:

    typedef D3D11DeviceChildHook<WRAP_CLASS, REAL_INTERFACE, METHOD_COUNT> ID3D11DEVICECHILD_BASE_TYPE;

    virtual void STDMETHODCALLTYPE GetDevice(
        /* [annotation] */
        __out  ID3D11Device **ppDevice)
    {
        if (nullptr == ppDevice) return;

        GN::AutoComPtr<ID3D11Device> pRealDevice;
        GetRealObj()->GetDevice(&pRealDevice);

        ID3D11Device * pHookedDevice;
        UINT size = (UINT)sizeof(pHookedDevice);
        GN_VERIFY(S_OK == pRealDevice->GetPrivateData(
            GN_D3D11HOOK_HOOKED_OBJECT_GUID,
            &size,
            &pHookedDevice));

        *ppDevice = pHookedDevice;
    }

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        /* [annotation] */
        __in  REFGUID guid,
        /* [annotation] */
        __inout  UINT *pDataSize,
        /* [annotation] */
        __out_bcount_opt( *pDataSize )  void *pData)
    {
        return GetRealObj()->GetPrivateData(guid, pDataSize, pData);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        /* [annotation] */
        __in  REFGUID guid,
        /* [annotation] */
        __in  UINT DataSize,
        /* [annotation] */
        __in_bcount_opt( DataSize )  const void *pData)
    {
        return GetRealObj()->SetPrivateData(guid, DataSize, pData);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        /* [annotation] */
        __in  REFGUID guid,
        /* [annotation] */
        __in_opt  const IUnknown *pData)
    {
        return GetRealObj()->SetPrivateDataInterface(guid, pData);
    }
};

template<class WRAP_CLASS, class REAL_INTERFACE, UINT METHOD_COUNT>
interface DXGIObjectHook : public UnknownHook<WRAP_CLASS, REAL_INTERFACE, METHOD_COUNT>
{

protected:

    DXGIObjectHook(REAL_INTERFACE * realobj) : IUNKNOWN_BASE_TYPE(realobj)
    {
    }

    virtual ~DXGIObjectHook()
    {
    }

public:

    typedef DXGIObjectHook<WRAP_CLASS, REAL_INTERFACE, METHOD_COUNT> IDXGIOBJECT_BASE_TYPE;

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        /* [annotation][in] */
        __in  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        __in_bcount(DataSize)  const void *pData)
    {
        return GetRealObj()->SetPrivateData(Name, DataSize, pData);
    }

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        /* [annotation][in] */
        __in  REFGUID Name,
        /* [annotation][in] */
        __in  const IUnknown *pUnknown)
    {
        return GetRealObj()->SetPrivateDataInterface(Name, pUnknown);
    }

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        /* [annotation][in] */
        __in  REFGUID Name,
        /* [annotation][out][in] */
        __inout  UINT *pDataSize,
        /* [annotation][out] */
        __out_bcount(*pDataSize)  void *pData)
    {
        return GetRealObj()->GetPrivateData(Name, pDataSize, pData);
    }

    virtual HRESULT STDMETHODCALLTYPE GetParent(
        /* [annotation][in] */
        __in  REFIID riid,
        /* [annotation][retval][out] */
        __out  void **ppParent)
    {
        // TODO: Need to return the hooked parent object.
        GN_UNUSED_PARAM(riid);
        GN_UNUSED_PARAM(ppParent);
        GN_UNIMPL();
    }
};
