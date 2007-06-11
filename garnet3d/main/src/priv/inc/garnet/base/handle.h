#ifndef __GN_BASE_HANDLE_H__
#define __GN_BASE_HANDLE_H__
// *****************************************************************************
/// \file    handle.h
/// \brief   Handle manager
/// \author  chenlee (2005.7.25)
// *****************************************************************************

#include <vector>

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
}

// *****************************************************************************
//                           End of handle.h
// *****************************************************************************
#endif // __GN_BASE_HANDLE_H__

