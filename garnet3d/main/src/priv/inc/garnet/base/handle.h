#ifndef __GN_BASE_HANDLE_H__
#define __GN_BASE_HANDLE_H__
// *****************************************************************************
/// \file
/// \brief   Handle manager
/// \author  chenlee (2005.7.25)
// *****************************************************************************

#include <vector>
#include <map>

namespace GN
{
    ///
    /// Handle Manager
    ///
    template<typename T, typename HANDLE_TYPE = size_t >
    class HandleManager
    {
        ///
        /// Handle item used internally by manager
        ///
        struct Item
        {
            UInt8 buf[sizeof(T)];
            bool  occupied;

            Item() : occupied(false) {}

            T & t() const
            {
                GN_ASSERT( occupied );
                return *(T*)buf;
            }

            void ctor()
            {
                GN_ASSERT( !occupied );
                new (buf) T();
                occupied = true;
            }

            void ctor( const T & v )
            {
                GN_ASSERT( !occupied );
                new (buf) T( v );
                occupied = true;
            }

            void dtor()
            {
                GN_ASSERT( occupied );
                t().T::~T();
                occupied = false;
            }
        };

        std::vector<Item*>  mItems;
        std::vector<size_t> mFreeList;


        static inline size_t h2idx( HANDLE_TYPE h ) { return (UIntPtr)h - 1; }

        static HANDLE_TYPE idx2h( size_t idx ) { return (HANDLE_TYPE)( idx + 1 ); }

    public:

        ///
        /// dtor
        ///
        ~HandleManager() { clear(); }

        ///
        /// clear all handles
        ///
        void clear()
        {
            for( size_t i = 0; i < mItems.size(); ++i )
            {
                GN_ASSERT( mItems[i] );
                if( mItems[i]->occupied ) mItems[i]->dtor();
                delete mItems[i];
            }
            mItems.clear();
            mFreeList.clear();
        }

        ///
        /// Get number of handles
        ///
        size_t size() const
        {
            return mItems.size() - mFreeList.size();
        }

        ///
        /// Is the manager empty or not.
        ///
        bool empty() const
        {
            return mItems.size() == mFreeList.size();
        }

        ///
        /// get current capacity
        ///
        size_t capacity() const { return mItems.capacity(); }

        ///
        /// set capacity
        ///
        void reserve( size_t n ) { mItems.reserve(n); mFreeList.reserve(n); }

        ///
        /// return first handle
        ///
        HANDLE_TYPE first() const
        {
            if( empty() ) return (HANDLE_TYPE)0;
            size_t idx = 0;
            while( !mItems[idx]->occupied )
            {
                GN_ASSERT( idx < mItems.size() );
                ++idx;
            }
            return idx2h( idx );
        }

        ///
        /// return next handle
        ///
        HANDLE_TYPE next( HANDLE_TYPE h ) const
        {
            if( !validHandle(h) ) return (HANDLE_TYPE)0;
            size_t idx = h2idx( h ) + 1;
            while( idx < mItems.size() && !mItems[idx]->occupied )
            {
                GN_ASSERT( idx < mItems.size() );
                ++idx;
            }
            return idx < mItems.size() ? idx2h( idx ) : (HANDLE_TYPE)0;
        }

        ///
        /// Add new item with user define value
        ///
        HANDLE_TYPE add( const T & val )
        {
            if( mFreeList.empty() )
            {
                Item * newItem = new Item;
                if( 0 == newItem )
                {
                    GN_ERROR(getLogger("GN.base.HandleManager"))( "out of memory" );
                    return 0;
                }
                newItem->ctor( val );
                mItems.push_back( newItem );
                return (HANDLE_TYPE)mItems.size();
            }
            else
            {
                size_t i = mFreeList.back();
                mFreeList.pop_back();
                GN_ASSERT( mItems[i] );
                mItems[i]->ctor( val );
                return idx2h( i );
            }
        }

        ///
        /// Add new item, with undefined value
        ///
        HANDLE_TYPE newItem()
        {
            if( mFreeList.empty() )
            {
                Item * newItem = new Item;
                if( 0 == newItem )
                {
                    GN_ERROR(getLogger("GN.base.HandleManager"))( "out of memory" );
                    return 0;
                }
                newItem->ctor();
                mItems.push_back( newItem );
                return (HANDLE_TYPE)mItems.size();
            }
            else
            {
                size_t i = mFreeList.back();
                mFreeList.pop_back();
                GN_ASSERT( mItems[i] );
                mItems[i]->ctor();
                return idx2h( i );
            }
        }

        ///
        /// Remove item from manager
        ///
        void remove( HANDLE_TYPE h )
        {
            if( !validHandle(h) )
            {
                GN_ERROR(getLogger("GN.base.HandleManager"))( "Invalid handle!" );
            }
            else
            {
                size_t idx = h2idx(h);
                GN_ASSERT( mItems[idx] );
                mItems[idx]->dtor();
                mFreeList.push_back(idx);
            }
        }

        ///
        /// Find specific item (always return first found)
        ///
        HANDLE_TYPE find( const T & val ) const
        {
            for( size_t i = 0; i < mItems.size(); ++i )
            {
                if( !mItems[i]->occupied ) continue;
                if( mItems[i]->t() == val ) return idx2h( i ); // found!
            }
            return (HANDLE_TYPE)0; // not found
        }

        ///
        /// Find specific item (always return first found)
        ///
        template<typename FUNC>
        HANDLE_TYPE findIf( const FUNC & fp ) const
        {
            for( size_t i = 0; i < mItems.size(); ++i )
            {
                if( !mItems[i]->occupied ) continue;
                if( fp( mItems[i]->t() ) ) return idx2h( i ); // found!
            }
            return (HANDLE_TYPE)0; // not found
        }

        ///
        /// Is valid handle or not?
        ///
        bool validHandle( HANDLE_TYPE h ) const
        {
            size_t idx = h2idx(h);
            return idx < mItems.size() && mItems[idx]->occupied;
        }

        ///
        /// Get item from manager. Handle must be valid.
        ///
        T & get( HANDLE_TYPE h ) const
        {
            GN_ASSERT( validHandle(h) );
            return mItems[h2idx(h)]->t();
        }

        ///
        /// Get item from manager. Handle must be valid.
        ///
        T & operator[]( HANDLE_TYPE h ) const { return get(h); }
    };

    ///
    /// expension to HandleManager class, that object can be referenced by both handle and name.
    ///
    template< class T, class H, bool CASE_INSENSITIVE = false>
    class NamedHandleManager
    {
        typedef std::map<StrA,H> NameMap;

        struct NamedItem
        {
            NamedHandleManager & mgr;
            const H              handle;
            const StrA           name;
            T                    data;

            NamedItem( NamedHandleManager & m, H h, const StrA & n, const T & d )
                : mgr(m), handle(h), name(n), data(d) {}

            NamedItem( NamedHandleManager & m, H h, const StrA & n ) 
                : mgr(m), handle(h), name(n) {}
        };

        NameMap                     mNames; // name -> handle
        HandleManager<NamedItem*,H> mItems; // handle -> name/data

    public:

        //@{

        typedef typename H ItemHandle;

        ///
        /// dtor
        ///
        ~NamedHandleManager() { clear(); }

        ///
        /// clear all handles
        ///
        void clear()
        {
            for( H i = mItems.first(); i != 0; i = mItems.next( i ) )
            {
                delete mItems[i];
            }
            mItems.clear();
            mNames.clear();
        }

        ///
        /// Get number of handles
        ///
        size_t size() const
        {
            GN_ASSERT( mItems.size() == mNames.size() );
            return mItems.size();
        }

        ///
        /// Is the manager empty or not.
        ///
        bool empty() const
        {
            GN_ASSERT( mItems.size() == mNames.size() );
            return mItems.empty();
        }

        ///
        /// return first handle
        ///
        H first() const { return mItems.first(); }

        ///
        /// return next handle
        ///
        H next( H h ) const { return mItems.next( h ); }

        ///
        /// name must be unique.
        ///
        H add( const StrA & name )
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            if( mNames.end() != mNames.find( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "name '%s' is not unique.", name.cptr() );
                return 0;
            }

            H handle = mItems.newItem();
            if( 0 == handle ) return 0;

            mItems[handle] = new NamedItem(*this,handle,name);

            mNames.insert( std::make_pair(name,handle) );

            return handle;
        }

        ///
        /// name must be unique.
        ///
        H add( const StrA & name, const T & data )
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            if( mNames.end() != mNames.find( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "name '%s' is not unique.", name.cptr() );
                return 0;
            }

            H handle = mItems.newItem();
            if( 0 == handle ) return 0;

            mItems[handle] = new NamedItem(*this,handle,name,data);

            mNames.insert( std::make_pair(name,handle) );

            return handle;
        }

        void remove( H h )
        {
            if( !validHandle( h ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "invalid handle : %d.", h );
                return;
            }

            NamedItem * item = mItems[h];
            GN_ASSERT( item && item->handle == h );

            mNames.erase( item->name );

            mItems.remove( item->handle );

            delete item;
        }

        void remove( const StrA & name )
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            if( !validName( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "invalid name: %s.", name.cptr() );
                return;
            }

            size_t handle = mNames[name];

            NamedItem * item = mItems[handle];

            mNames.erase( name );

            mItems.remove( handle );

            delete item;
        }

        bool validHandle( H h ) const
        {
            return mItems.validHandle( h );
        }

        bool validName( const StrA & name ) const
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            return mNames.end() != mNames.find( name );
        }

        H name2handle( const StrA & name ) const
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            NameMap::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                return (H)0;
            }
            else
            {
                return i->second;
            }
        }

        const StrA & handle2name( H h ) const
        {
            if( !mItems.validHandle( h ) )
                return StrA::EMPTYSTR;
            else
                return mItems[h]->name;
        }

        T & get( H h ) const
        {
            return mItems[h]->data;
        }

        T & get( const StrA & name ) const
        {
            GN_ASSERT( validName(name) );
            NameMap::const_iterator i = mNames.find( name );
            return mItems[mItems.get( i->second )]->data;
        }

        T & operator[]( H h ) const { return get(h); }

        T & operator[]( const StrA & name ) const { return get(name); }

        //@}
    };}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_HANDLE_H__

