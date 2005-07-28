#ifndef __GN_BASE_HANDLE_H__
#define __GN_BASE_HANDLE_H__
// *****************************************************************************
//! \file    handle.h
//! \brief   Handle manager
//! \author  chenlee (2005.7.25)
// *****************************************************************************

#include <vector>

namespace GN
{
    template<typename T, typename HANDLE_TYPE = size_t >    
    class HandleManager
    {
        struct Item
        {
            mutable T value;
            bool      used;

            Item( const T & v ) : value(v), used(true) {}
        };

        std::vector<Item>   mItems;
        std::vector<size_t> mFreeList;

    public:

        //!
        //! clear all handles
        //!
        void clear()
        {
            mItems.clear();
            mFreeList.clear();
        }

        //!
        //! Get number of handles
        //!
        size_t size() const
        {
            return mItems.size() - mFreeList.size();
        }

        //!
        //! Is the manager empty or not.
        //!
        bool empty() const
        {
            return mItems.size() == mFreeList.size();
        }

        //!
        //! return first handle
        //!
        HANDLE_TYPE first() const
        {
            if ( empty() ) return (HANDLE_TYPE)0;
            size_t idx = 0;
            while( !mItems[idx].used )
            {
                GN_ASSERT( idx < mItems.size() );
                ++idx;
            }
            return (HANDLE_TYPE)(idx+1);
        }

        //!
        //! return next handle
        //!
        HANDLE_TYPE next( HANDLE_TYPE h ) const
        {
            if ( !validHandle(h) ) return (HANDLE_TYPE)0;
            size_t idx = h; // That is: (h-1)+1
            while( idx < mItems.size() && !mItems[idx].used )
            {
                GN_ASSERT( idx < mItems.size() );
                ++idx;
            }
            return idx < mItems.size() ? (HANDLE_TYPE)(idx+1) : (HANDLE_TYPE)0;
        }

        //!
        //! Add new item
        //!
        HANDLE_TYPE add( const T & val )
        {
            if ( mFreeList.empty() )
            {
                mItems.push_back( Item(val) );
                return (HANDLE_TYPE)mItems.size();
            }
            else
            {
                size_t i = mFreeList.back();
                mFreeList.pop_back();
                GN_ASSERT( !mItems[i].used );
                mItems[i].value = val;
                mItems[i].used = true;
                return (HANDLE_TYPE)(i+1);
            }
        }

        //!
        //! Remove item from manager
        //!
        void remove( HANDLE_TYPE h )
        {
            if ( !validHandle(h) )
            {
                GN_ERROR( "Invalid handle!" );
            }
            else
            {
                mFreeList.push_back(h-1);
                mItems[h-1].used = false;
            }
        }

        //!
        //! Find specific item (always return first found)
        //!
        HANDLE_TYPE find( const T & val ) const
        {
            for( size_t i = 0; i < mItems.size(); ++i )
            {
                if ( !mItems[i].used ) continue;
                if ( mItems[i].value == val ) return (HANDLE_TYPE)(i+1); // found!
            }
            return (HANDLE_TYPE)0; // not found
        }

        //!
        //! Find specific item (always return first found)
        //!
        template<typename FUNC>
        HANDLE_TYPE findIf( const FUNC & fp ) const
        {
            for( size_t i = 0; i < mItems.size(); ++i )
            {
                if ( !mItems[i].used ) continue;
                if ( fp(mItems[i].value) ) return (HANDLE_TYPE)(i+1); // found!
            }
            return (HANDLE_TYPE)0; // not found
        }

        //!
        //! Is valid handle or not?
        //!
        bool validHandle( HANDLE_TYPE h ) const
        {
            return 0 != h && h <= mItems.size() && mItems[h-1].used;
        }

        //!
        //! Get item from manager
        //!
        T & get( HANDLE_TYPE h ) const
        {
            GN_ASSERT( validHandle(h) );
            return mItems[h-1].value;
        }

        //!
        //! Get item from manager
        //!
        T & operator[]( HANDLE_TYPE h ) const { return get(h); }
    };
}

// *****************************************************************************
//                           End of handle.h
// *****************************************************************************
#endif // __GN_BASE_HANDLE_H__

