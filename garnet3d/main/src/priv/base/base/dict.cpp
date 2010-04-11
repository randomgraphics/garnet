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
        , mData( HeapMemory::alloc(tt.size) )
    {
        mTraits.ctor( mData );
    }*/

    MapKey( const MapKey & t )
        : mTraits( t.mTraits )
        , mData( HeapMemory::alloc(t.mTraits.size) )
    {
        mTraits.cctor( mData, t.mData );
    }

    ~MapKey()
    {
        mTraits.dtor( mData );
        GN::HeapMemory::dealloc( mData );
    }

    void * data() const { return mData; }

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
        , mData( HeapMemory::alloc(tt.size) )
    {
        // This should only be used when FindOrCreate() is called.
        mTraits.ctor( mData );
    }

    MapValue( const MapValue & t )
        : mTraits( t.mTraits )
        , mData( HeapMemory::alloc(t.mTraits.size) )
    {
        mTraits.cctor( mData, t.mData );
    }

    ~MapValue()
    {
        mTraits.dtor( mData );
        GN::HeapMemory::dealloc( mData );
    }

    void * data() const { return mData; }
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
const void * GN::TypelessDict::Iterator::key() const
{
    std::pair<const MapKey,MapValue> & p = *TO_MAP_ITER(*this);
    return p.first.data();
}

//
//
// -----------------------------------------------------------------------------
void * GN::TypelessDict::Iterator::value() const
{
    std::pair<const MapKey,MapValue> & p = *TO_MAP_ITER(*this);
    return p.second.data();
}

//
//
// -----------------------------------------------------------------------------
void GN::TypelessDict::Iterator::moveToNext()
{
    ++TO_MAP_ITER(*this);
}

//
//
// -----------------------------------------------------------------------------
bool GN::TypelessDict::Iterator::equal( const Iterator & i ) const
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

    Iterator begin() const
    {
        return sMakeIter( const_cast<MapType&>(mMap).begin() );
    }

    void clear()
    {
        mMap.clear();
    }

    void copyFrom( const TypelessDict & d )
    {
        if( this == d.mImpl ) return;

        mMap.clear();
        mKeyTraits = d.mImpl->mKeyTraits;
        mValueTraits = d.mImpl->mValueTraits;
        mMap = d.mImpl->mMap;
    }

    bool empty() const
    {
        return mMap.empty();
    }

    Iterator end() const
    {
        return sMakeIter( const_cast<MapType&>(mMap).end() );
    }

    void * find( const void * key )
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
            return v.data();
        }
    }

    void * findOrInsert( const void * key, TypeTraits::CtorFunc ctor )
    {
        void * value = find( key );
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

        return iter->second.data();
    }

    bool insert( const void * key, const void * value, Iterator * iter )
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

    void remove( const void * key )
    {
        KeyProxy kp( mKeyTraits, key );

        mMap.erase( kp );
    }

    size_t size() const
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

GN::TypelessDict::Iterator  GN::TypelessDict::begin() const { return mImpl->begin(); }
void                        GN::TypelessDict::clear() { return mImpl->clear(); }
void                        GN::TypelessDict::copyFrom( const TypelessDict & d ) { return mImpl->copyFrom( d ); }
bool                        GN::TypelessDict::empty() const { return mImpl->empty(); }
GN::TypelessDict::Iterator  GN::TypelessDict::end() const { return mImpl->end(); }
void *                      GN::TypelessDict::find( const void * key ) const { return mImpl->find( key ); }
void *                      GN::TypelessDict::findOrInsert( const void * key, TypeTraits::CtorFunc ctor ) { return mImpl->findOrInsert( key, ctor ); }
bool                        GN::TypelessDict::insert( const void * key, const void * value, Iterator * iter ) { return mImpl->insert( key, value, iter ); }
void                        GN::TypelessDict::remove( const void * key ) { return mImpl->remove( key ); }
size_t                      GN::TypelessDict::size() const { return mImpl->size(); }
