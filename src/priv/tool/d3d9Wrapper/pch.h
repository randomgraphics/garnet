#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   Precompiled header
// \author  chenlee (2005.8.7)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"

#if GN_XENON
#include <xtl.h>
#elif GN_MSWIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <d3d9.h>
#include <dxerr9.h>
#endif

template<class T>
class BasicInterface : public T
{
protected:

    T *   mObject;
    ULONG mRefCounter;

    virtual ~BasicInterface() {}

public:

    BasicInterface() : mObject(0), mRefCounter(1) {}

    T * obj() const { GN_ASSERT(mObject); return mObject; }

    /*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface( const IID & riid, void** ppvObj )
    {
        GN_ASSERT( mObject );
        return mObject->QueryInterface( riid, ppvObj );
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        ++mRefCounter;
        mObject->AddRef();
        return mRefCounter;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG c = --mRefCounter;
        GN_ASSERT( mObject );
        mObject->Release();
        if( 0 == c ) delete this;
        return c;
    }
};

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
