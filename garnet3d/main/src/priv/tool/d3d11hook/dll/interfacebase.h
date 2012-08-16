#pragma once

// *****************************************************************************
/// \file
/// \brief   Basic COM interface wrapper
/// \author  chen@@CHENLI-HOMEPC (2007.3.17)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <map>
#include "utils.h"

typedef void* (*WrapRealObjectFuncPtr)(void * realobj);

///
/// Wrapper for basic COM interface
///
template<class WRAP_CLASS, class REAL_CLASS, UINT METHOD_COUNT>
class InterfaceBase : public REAL_CLASS
{
    struct IIDLess
    {
        bool operator()( const IID & a, const IID & b ) const
        {
            return ::memcmp( &a, &b, sizeof(a) ) < 0;
        }
    };

    GN::AutoComPtr<REAL_CLASS>                   _realobj;
    CritSec                                      _cs;
    ULONG                                        _refcount;
    std::map<IID,WrapRealObjectFuncPtr, IIDLess> _factory;

private:

    static void * CreateInstanceFromRealObj(void * realobj)
    {
        GN_ASSERT(realobj != nullptr);
        WRAP_CLASS * wrapper = new WRAP_CLASS((REAL_CLASS*)realobj);
        wrapper->AddRef();
        return wrapper;
    }

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

    InterfaceBase(REAL_CLASS * realobj) : _refcount(0)
    {
        _realobj.attach(realobj);
        memset(_preFuncPtr, 0, sizeof(_preFuncPtr));
        memset(_postFuncPtr, 0, sizeof(_postFuncPtr));
    }

    virtual ~InterfaceBase()
    {
    }

    template<class WRAP_CLASS2, class REAL_CLASS2>
    void AddKnownInterface()
    {
        IID iid = __uuidof(REAL_CLASS2);
        GN_ASSERT(_factory.end() == _factory.find(iid));
        _factory[iid] = WRAP_CLASS2::CreateInstanceFromRealObj;
    }

public:

    REAL_CLASS * GetRealObj() const { GN_ASSERT(_realobj); return _realobj; }

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

        auto iter = _factory.find( riid );

        if( _factory.end() != iter )
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
