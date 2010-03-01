#ifndef __GN_D3DWRAPPER_BASEINTERFACE_H__
#define __GN_D3DWRAPPER_BASEINTERFACE_H__
// *****************************************************************************
/// \file
/// \brief   Basic COM interface wrapper
/// \author  chen@@CHENLI-HOMEPC (2007.3.17)
// *****************************************************************************

///
/// less operator for IID class (required by GN::Dictionary)
///
inline bool operator < ( const IID & lhs, const IID & rhs )
{
    return *(UInt64*)(&lhs) < *(UInt64*)(&rhs);
}

///
/// Wrapper for basic COM interface
///
template<class T>
class BasicInterface : public T
{
    typedef BasicInterface<T> MyType;

    typedef GN::Dictionary<IID,MyType*> TypeTable;

    TypeTable mRtti; ///< My runtime type information, used by query interface

protected:

    //@{

    T *   mRealObject;
    ULONG mRefCounter;

    virtual ~BasicInterface() {}

    void addRuntimeType( const IID & iid, MyType * ptr )
    {
        GN_ASSERT( ptr );
        GN_ASSERT( NULL == mRtti.Find( iid ) );
        mRtti[iid] = ptr;
    }

    //@}

public:

    //@{

    BasicInterface() : mRealObject(0), mRefCounter(1) {}

    T * realobj() const { GN_ASSERT(mRealObject); return mRealObject; }

    /*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface( const IID & riid, void** ppvObj )
    {
        //GN_ASSERT( mRealObject );
        //return mRealObject->QueryInterface( riid, ppvObj );

        if( 0 == ppvObj ) return E_INVALIDARG;

        TypeTable::const_iterator i = mRtti.find( riid );
        if( mRtti.end() == i )
        {
            GN::AutoComPtr<IUnknown> obj;
            GN_ASSERT( E_NOINTERFACE == mRealObject->QueryInterface( riid, (void**)&obj ) );
            obj.Clear();
            return E_NOINTERFACE;
        }
        else
        {
            i->second->AddRef();
            *ppvObj = i->second;
            return S_OK;
        }
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        ++mRefCounter;
        mRealObject->AddRef();
        return mRefCounter;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG c = --mRefCounter;
        GN_ASSERT( mRealObject );
        mRealObject->Release();
        if( 0 == c ) delete this;
        return c;
    }

    //@}
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3DWRAPPER_BASEINTERFACE_H__
