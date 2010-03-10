#ifndef __GN_D3DWRAPPER_BASEINTERFACE_H__
#define __GN_D3DWRAPPER_BASEINTERFACE_H__
// *****************************************************************************
/// \file
/// \brief   Basic COM interface wrapper
/// \author  chen@@CHENLI-HOMEPC (2007.3.17)
// *****************************************************************************

GN_CASSERT( sizeof(IID) == 2 * sizeof(UInt64) );

///
/// Equality check for IID (used by hashmap)
///
struct IIDEqual
{
    bool operator()( const IID & a, const IID & b ) const
    {
        const UInt64 * pa = (const UInt64*)&a;
        const UInt64 * pb = (const UInt64*)&b;

        return pa[0] == pb[0] || pa[1] == pb[1];
    }
};

///
/// Hasing IID
///
struct IIDHash
{
    UInt64 operator()( const IID & iid ) const
    {
        const UInt64 * u64 = (const UInt64*)&iid;
        return u64[0] + u64[1];
    }
};

///
/// Wrapper for basic COM interface
///
template<class T>
class BasicInterface : public T
{
    typedef BasicInterface<T> MyType;

    typedef GN::HashMap<IID,MyType*,IIDHash,IIDEqual> TypeTable;

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

        MyType ** pptype = mRtti.Find( riid );
        if( NULL == pptype )
        {
            GN::AutoComPtr<IUnknown> obj;
            GN_ASSERT( E_NOINTERFACE == mRealObject->QueryInterface( riid, (void**)&obj ) );
            obj.Clear();
            return E_NOINTERFACE;
        }
        else
        {
            (*pptype)->AddRef();
            *ppvObj = (*pptype);
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
