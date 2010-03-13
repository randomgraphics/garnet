#ifndef __GN_BASE_HANDLE_H__
#define __GN_BASE_HANDLE_H__
// *****************************************************************************
/// \file
/// \brief   Handle manager
/// \author  chenlee (2005.7.25)
// *****************************************************************************

namespace GN
{
    ///
    /// Handle Manager
    ///
    template<typename T, typename HANDLE_TYPE = size_t >
    class HandleManager : public NoCopy
    {
        ///
        /// Handle item used internally by manager
        ///
        struct Item
        {
            UInt8 buf[sizeof(T)];
            bool  occupied;

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

        FixSizedRawMemoryPool<sizeof(Item)> mPool;
        DynaArray<Item*>                  mItems;
        DynaArray<size_t>                 mFreeList;


        static inline size_t h2idx( HANDLE_TYPE h ) { return (UIntPtr)h - 1; }

        static HANDLE_TYPE idx2h( size_t idx ) { return (HANDLE_TYPE)( idx + 1 ); }

    public:

        ///
        /// dtor
        ///
        ~HandleManager() { Clear(); }

        ///
        /// clear all handles
        ///
        void Clear()
        {
            for( size_t i = 0; i < mItems.Size(); ++i )
            {
                GN_ASSERT( mItems[i] );
                if( mItems[i]->occupied ) mItems[i]->dtor();
                mPool.Dealloc( mItems[i] );
            }
            mItems.Clear();
            mFreeList.Clear();
        }

        ///
        /// Get number of handles
        ///
        size_t Size() const
        {
            return mItems.Size() - mFreeList.Size();
        }

        ///
        /// Is the manager empty or not.
        ///
        bool Empty() const
        {
            return mItems.Size() == mFreeList.Size();
        }

        ///
        /// Get current Capacity
        ///
        size_t Capacity() const { return mItems.Capacity(); }

        ///
        /// set Capacity
        ///
        void Reserve( size_t n ) { mItems.Reserve(n); mFreeList.Reserve(n); }

        ///
        /// return First handle
        ///
        HANDLE_TYPE First() const
        {
            if( Empty() ) return (HANDLE_TYPE)0;
            size_t idx = 0;
            while( !mItems[idx]->occupied )
            {
                GN_ASSERT( idx < mItems.Size() );
                ++idx;
            }
            return idx2h( idx );
        }

        ///
        /// return Next handle
        ///
        HANDLE_TYPE Next( HANDLE_TYPE h ) const
        {
            if( !ValidHandle(h) ) return (HANDLE_TYPE)0;
            size_t idx = h2idx( h ) + 1;
            while( idx < mItems.Size() && !mItems[idx]->occupied )
            {
                GN_ASSERT( idx < mItems.Size() );
                ++idx;
            }
            return idx < mItems.Size() ? idx2h( idx ) : (HANDLE_TYPE)0;
        }

        ///
        /// Add new item with user define value
        ///
        HANDLE_TYPE Add( const T & val )
        {
            if( mFreeList.Empty() )
            {
                Item * newItem = (Item*)mPool.Alloc();
                if( 0 == newItem )
                {
                    GN_ERROR(GetLogger("GN.base.HandleManager"))( "out of memory" );
                    return 0;
                }
                newItem->occupied = false;
                newItem->ctor( val );
                mItems.Append( newItem );
                return (HANDLE_TYPE)mItems.Size();
            }
            else
            {
                size_t i = mFreeList.Back();
                mFreeList.PopBack();
                GN_ASSERT( mItems[i] );
                mItems[i]->ctor( val );
                return idx2h( i );
            }
        }

        ///
        /// Add new item, with undefined value
        ///
        HANDLE_TYPE NewHandle()
        {
            if( mFreeList.Empty() )
            {
                Item * newItem = (Item*)mPool.Alloc();
                if( 0 == newItem )
                {
                    GN_ERROR(GetLogger("GN.base.HandleManager"))( "out of memory" );
                    return 0;
                }
                newItem->occupied = false;
                newItem->ctor();
                mItems.Append( newItem );
                return (HANDLE_TYPE)mItems.Size();
            }
            else
            {
                size_t i = mFreeList.Back();
                mFreeList.PopBack();
                GN_ASSERT( mItems[i] );
                mItems[i]->ctor();
                return idx2h( i );
            }
        }

        ///
        /// Remove item from manager
        ///
        void Remove( HANDLE_TYPE h )
        {
            if( !ValidHandle(h) )
            {
                GN_ERROR(GetLogger("GN.base.HandleManager"))( "Invalid handle!" );
            }
            else
            {
                size_t idx = h2idx(h);
                GN_ASSERT( mItems[idx] );
                mItems[idx]->dtor();
                mFreeList.Append(idx);
            }
        }

        ///
        /// Find specific item (always return First found)
        ///
        HANDLE_TYPE Find( const T & val ) const
        {
            for( size_t i = 0; i < mItems.Size(); ++i )
            {
                if( !mItems[i]->occupied ) continue;
                if( mItems[i]->t() == val ) return idx2h( i ); // found!
            }
            return (HANDLE_TYPE)0; // not found
        }

        ///
        /// Find specific item (always return First found)
        ///
        template<typename FUNC>
        HANDLE_TYPE FindIf( const FUNC & fp ) const
        {
            for( size_t i = 0; i < mItems.Size(); ++i )
            {
                if( !mItems[i]->occupied ) continue;
                if( fp( mItems[i]->t() ) ) return idx2h( i ); // found!
            }
            return (HANDLE_TYPE)0; // not found
        }

        ///
        /// Is valid handle or not?
        ///
        bool ValidHandle( HANDLE_TYPE h ) const
        {
            size_t idx = h2idx(h);
            return idx < mItems.Size() && mItems[idx]->occupied;
        }

        ///
        /// Get item from manager. Handle must be valid.
        ///
        T & Get( HANDLE_TYPE h ) const
        {
            GN_ASSERT( ValidHandle(h) );
            return mItems[h2idx(h)]->t();
        }

        ///
        /// Get item from manager. Handle must be valid.
        ///
        T & operator[]( HANDLE_TYPE h ) const { return Get(h); }
    };

    ///
    /// expension to HandleManager class, that object can be referenced by both handle and name.
    ///
    template< typename T, typename H, bool CASE_INSENSITIVE = false>
    class NamedHandleManager
    {
        typedef StringMap<char,H> NameMap;

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
        ObjectPool<NamedItem>       mPool;  // named item pool

    public:

        //@{

        typedef H ItemHandle;

        ///
        /// dtor
        ///
        ~NamedHandleManager() { Clear(); }

        ///
        /// clear all handles
        ///
        void Clear()
        {
            for( H i = mItems.First(); i != 0; i = mItems.Next( i ) )
            {
                mPool.DeconstructAndFree( mItems[i] );
            }
            mItems.Clear();
            mNames.Clear();
        }

        ///
        /// Get number of handles
        ///
        size_t Size() const
        {
            GN_ASSERT( mItems.Size() == mNames.Size() );
            return mItems.Size();
        }

        ///
        /// Is the manager empty or not.
        ///
        bool Empty() const
        {
            GN_ASSERT( mItems.Size() == mNames.Size() );
            return mItems.Empty();
        }

        ///
        /// return First handle
        ///
        H First() const { return mItems.First(); }

        ///
        /// return Next handle
        ///
        H Next( H h ) const { return mItems.Next( h ); }

        ///
        /// name must be unique.
        ///
        H Add( const StrA & name )
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            if( NULL != mNames.Find( name ) )
            {
                GN_ERROR(GetLogger("GN.base.NamedHandleManager"))( "name '%s' is not unique.", name.ToRawPtr() );
                return 0;
            }

            // create new item
            NamedItem * p = mPool.AllocUnconstructed();
            if( 0 == p ) return 0;

            H handle = mItems.NewHandle();
            if( 0 == handle ) return 0;

            new (p) NamedItem(*this,handle,name);

            mItems[handle] = p;

            mNames.Insert( name, handle );

            return handle;
        }

        ///
        /// name must be unique.
        ///
        H Add( const StrA & name, const T & data )
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            if( NULL != mNames.Find( name ) )
            {
                GN_ERROR(GetLogger("GN.base.NamedHandleManager"))( "name '%s' is not unique.", name.ToRawPtr() );
                return 0;
            }

            NamedItem * p = mPool.AllocUnconstructed();
            if( 0 == p ) return 0;

            H handle = mItems.NewHandle();
            if( 0 == handle ) return 0;

            new (p) NamedItem(*this,handle,name,data);

            mItems[handle] = p;

            mNames.Insert( name, handle );

            return handle;
        }

        void Remove( H h )
        {
            if( !ValidHandle( h ) )
            {
                GN_ERROR(GetLogger("GN.base.NamedHandleManager"))( "invalid handle : %d.", h );
                return;
            }

            NamedItem * item = mItems[h];
            GN_ASSERT( item && item->handle == h );

            mNames.Remove( item->name );

            mItems.Remove( item->handle );

            mPool.DeconstructAndFree( item );
        }

        void Remove( const StrA & name )
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            if( !ValidName( name ) )
            {
                GN_ERROR(GetLogger("GN.base.NamedHandleManager"))( "invalid name: %s.", name.ToRawPtr() );
                return;
            }

            size_t handle = mNames[name];

            NamedItem * item = mItems[handle];

            mNames.Remove( name );

            mItems.Remove( handle );

            mPool.DeconstructAndFree( item );
        }

        bool ValidHandle( H h ) const
        {
            return mItems.ValidHandle( h );
        }

        bool ValidName( const StrA & name ) const
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            return NULL != mNames.Find( name );
        }

        H Name2Handle( const StrA & name ) const
        {
            if( CASE_INSENSITIVE )
            {
                GN_UNIMPL();
            }

            const H * h = mNames.Find( name );

            return h ? *h : (H)0;
        }

        const char * Handle2Name( H h ) const
        {
            if( !mItems.ValidHandle( h ) )
                return NULL;
            else
                return mItems[h]->name;
        }

        T & Get( H h ) const
        {
            return mItems[h]->data;
        }

        T & Get( const StrA & name ) const
        {
            GN_ASSERT( ValidName(name) );
            return mItems[mNames[name]]->data;
        }

        T & operator[]( H h ) const { return Get(h); }

        T & operator[]( const StrA & name ) const { return Get(name); }

        //@}
    };}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_HANDLE_H__

