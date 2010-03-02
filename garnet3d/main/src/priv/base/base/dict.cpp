#include "pch.h"
#include <map>

using namespace GN;

class MapKey
{
    typedef GN::TypelessDict::TypeTraits TypeTraits;

    TypeTraits & mTraits;
    void *       mData;

public:

    /*MapKey( TypeTraits & tt )
        : mTraits(tt)
        // TODO: use stack memory, when size is small
        , mData( HeapMemory::Alloc(tt.size) )
    {
        mTraits.ctor( mData );
    }*/

    MapKey( const MapKey & t )
        : mTraits( t.mTraits )
        , mData( HeapMemory::Alloc(t.mTraits.size) )
    {
        mTraits.cctor( mData, t.mData );
    }

    ~MapKey()
    {
        mTraits.dtor( mData );
        GN::HeapMemory::Free( mData );
    }

    void * Data() const { return mData; }

    MapKey & operator=( const MapKey & rhs )
    {
        mTraits.assign( mData, rhs.mData );
        return *this;
    }

    bool operator<( const MapKey & rhs ) const
    {
        return mTraits.less( mData, rhs.mData );
    }
};

class MapValue
{
    typedef GN::TypelessDict::TypeTraits TypeTraits;

    TypeTraits & mTraits;
    void *       mData;

public:

    MapValue( TypeTraits & tt )
        : mTraits(tt)
        // TODO: use stack memory, when size is small
        , mData( HeapMemory::Alloc(tt.size) )
    {
        // This should only be used when FindOrCreate() is called.
        mTraits.ctor( mData );
    }

    MapValue( const MapValue & t )
        : mTraits( t.mTraits )
        , mData( HeapMemory::Alloc(t.mTraits.size) )
    {
        mTraits.cctor( mData, t.mData );
    }

    ~MapValue()
    {
        mTraits.dtor( mData );
        GN::HeapMemory::Free( mData );
    }

    void * Data() const { return mData; }
};

template<typename T>
struct TypeProxy
{
    typedef GN::TypelessDict::TypeTraits TypeTraits;

    TypeTraits & tt;
    const void * ptr;

    TypeProxy( TypeTraits & tt_, const void * p )
        : tt(tt_)
        , ptr(p)
    {
    }

    operator T & () { return *(T*)this; }
    operator const T & () const { return *(const T*)this; }
};

typedef std::map<MapKey, MapValue> MapType;
typedef TypeProxy<MapKey> KeyProxy;
typedef TypeProxy<MapValue> ValueProxy;

// *****************************************************************************
// TypelessDict::Iterator
// *****************************************************************************

// check buffer size
GN_CASSERT( sizeof(MapType::iterator) <= sizeof(GN::TypelessDict::Iterator) );

#define TO_MAP_ITER(i) (*((MapType::iterator*)((i).mBuf)))

//
//
// -----------------------------------------------------------------------------
GN::TypelessDict::Iterator::Iterator()
{
    new (mBuf) MapType::iterator();
}

//
//
// -----------------------------------------------------------------------------
GN::TypelessDict::Iterator::Iterator( const Iterator & i )
{
    MapType::iterator & mi = TO_MAP_ITER(i);
    MapType::iterator haha( mi );
    new (mBuf) MapType::iterator( haha );
}

//
//
// -----------------------------------------------------------------------------
GN::TypelessDict::Iterator::~Iterator()
{
    ((MapType::iterator*)mBuf)->MapType::iterator::~iterator();
}

//
//
// -----------------------------------------------------------------------------
const void * GN::TypelessDict::Iterator::Key() const
{
    std::pair<const MapKey,MapValue> & p = *TO_MAP_ITER(*this);
    return p.first.Data();
}

//
//
// -----------------------------------------------------------------------------
void * GN::TypelessDict::Iterator::Value() const
{
    std::pair<const MapKey,MapValue> & p = *TO_MAP_ITER(*this);
    return p.second.Data();
}

//
//
// -----------------------------------------------------------------------------
void GN::TypelessDict::Iterator::GoToNext()
{
    ++TO_MAP_ITER(*this);
}

//
//
// -----------------------------------------------------------------------------
bool GN::TypelessDict::Iterator::Equal( const Iterator & i ) const
{
    return TO_MAP_ITER(*this) == TO_MAP_ITER( i );
}

//
//
// -----------------------------------------------------------------------------
GN::TypelessDict::Iterator &
GN::TypelessDict::Iterator::operator=( const Iterator & i )
{
    TO_MAP_ITER(*this) = TO_MAP_ITER( i );
    return *this;
}


// *****************************************************************************
// TypelessDict::Impl
// *****************************************************************************

class GN::TypelessDict::Impl
{
    TypeTraits mKeyTraits;
    TypeTraits mValueTraits;
    MapType    mMap;

    static Iterator sMakeIter( MapType::iterator mi )
    {
        Iterator i( *(Iterator*)&mi );

        return i;
    }

public:

    Impl( TypeTraits key, TypeTraits value )
        : mKeyTraits( key )
        , mValueTraits( value )
    {
    }

    Impl( const Impl & i )
        : mKeyTraits( i.mKeyTraits )
        , mValueTraits( i.mValueTraits )
        , mMap( i.mMap )
    {
    }

    Iterator Begin() const
    {
        return sMakeIter( const_cast<MapType&>(mMap).begin() );
    }

    void Clear()
    {
        mMap.clear();
    }

    void CopyFrom( const TypelessDict & d )
    {
        if( this == d.mImpl ) return;

        mMap.clear();
        mKeyTraits = d.mImpl->mKeyTraits;
        mValueTraits = d.mImpl->mValueTraits;
        mMap = d.mImpl->mMap;
    }

    bool Empty() const
    {
        return mMap.empty();
    }

    Iterator End() const
    {
        return sMakeIter( const_cast<MapType&>(mMap).end() );
    }

    void * Find( const void * key )
    {
        KeyProxy kp( mKeyTraits, key );

        MapType::const_iterator iter = mMap.find( kp );

        if( iter == mMap.end() )
        {
            return NULL;
        }
        else
        {
            const MapValue & v = iter->second;
            return v.Data();
        }
    }

    void * FindOrInsert( const void * key, TypeTraits::CtorFunc ctor )
    {
        void * value = Find( key );
        if( value ) return value;

        GN_ASSERT( ctor );
        GN_ASSERT( NULL == mValueTraits.ctor || ctor == mValueTraits.ctor );
        mValueTraits.ctor = ctor;

        KeyProxy kp( mKeyTraits, key );
        MapKey & k = kp;
        MapValue v( mValueTraits );

        std::pair<MapType::iterator, bool> p = mMap.insert( std::make_pair(k, v) );

        MapType::iterator & iter = p.first;

        GN_ASSERT( p.second ); // insert must success.

        return iter->second.Data();
    }

    bool Insert( const void * key, const void * value, Iterator * iter )
    {
        KeyProxy kp( mKeyTraits, key );
        ValueProxy vp( mValueTraits, value );

        MapKey & k = kp;
        MapValue & v = vp;

        std::pair<MapType::iterator, bool> p = mMap.insert( std::make_pair(k, v) );

        MapType::iterator & mi = p.first;
        bool inserted = p.second;

        if( iter ) *iter = sMakeIter( mi );

        return inserted;
    }

    void Remove( const void * key )
    {
        KeyProxy kp( mKeyTraits, key );

        mMap.erase( kp );
    }

    size_t Size() const
    {
        return mMap.size();
    }
};


// *****************************************************************************
// TypelessDict
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::TypelessDict::TypelessDict( TypeTraits key, TypeTraits value )
    : mImpl( new Impl( key, value ) )
{
}

GN::TypelessDict::TypelessDict( const TypelessDict & d )
    : mImpl( new Impl( *d.mImpl ) )
{
}

GN::TypelessDict::~TypelessDict()
{
    delete mImpl;
}

GN::TypelessDict::Iterator  GN::TypelessDict::Begin() const { return mImpl->Begin(); }
void                        GN::TypelessDict::Clear() { return mImpl->Clear(); }
void                        GN::TypelessDict::CopyFrom( const TypelessDict & d ) { return mImpl->CopyFrom( d ); }
bool                        GN::TypelessDict::Empty() const { return mImpl->Empty(); }
GN::TypelessDict::Iterator  GN::TypelessDict::End() const { return mImpl->End(); }
void *                      GN::TypelessDict::Find( const void * key ) const { return mImpl->Find( key ); }
void *                      GN::TypelessDict::FindOrInsert( const void * key, TypeTraits::CtorFunc ctor ) { return mImpl->FindOrInsert( key, ctor ); }
bool                        GN::TypelessDict::Insert( const void * key, const void * value, Iterator * iter ) { return mImpl->Insert( key, value, iter ); }
void                        GN::TypelessDict::Remove( const void * key ) { return mImpl->Remove( key ); }
size_t                      GN::TypelessDict::Size() const { return mImpl->Size(); }
