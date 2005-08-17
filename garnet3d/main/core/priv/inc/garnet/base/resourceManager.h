#ifndef __GN_BASE_RESOURCEMANAGER_H__
#define __GN_BASE_RESOURCEMANAGER_H__
// *****************************************************************************
//! \file    resourceManager.h
//! \brief   general resource manager: map name to instance
//! \author  chenlee (2005.8.17)
// *****************************************************************************

namespace GN
{
    namespace detail
    {
        //!
        //! Default parent class for resource manager
        //!
        struct DefaultParent {};
    }

    //!
    //! Resource manager.
    //!
    //! RES must support default constructor and assignment operation.
    //!
    template<typename RES, class PARENT = detail::DefaultParent>
    class ResourceManager : public PARENT
    {
    public:

        typedef uint32_t ResHandle; //!< resource Handle. 0 means invalid handle

        typedef RES ResType; //!< resource type

        typedef Functor2<bool,RES&,const StrA &> Creator; //!< Resource creation functor

        typedef Functor1<void,RES&> Deletor; //!< Resource deletion functor

        //!
        //! Default constructor
        //!
        ResourceManager(
            const Creator & defaultCreator = Creator(),
            const Creator & defaultNullor = Creator(),
            const Deletor & deletor = Deletor() )
            : mDefaultCreator(defaultCreator)
            , mDefaultNullor(defaultNullor)
            , mDeletor(deletor)
            , mNullInstance(0)
        {
        }

        //!
        //! Default destructor
        //!
        ~ResourceManager() { clear(); }

        //!
        //! Get default creator
        //!
        const Creator & getDefaultCreator() const { return mDefaultCreator; }

        //!
        //! Get defaut NULL instance creator
        //!
        const Creator & getDefaultNullor() const { return mDefaultNullor; }

        //!
        //! Get deletor
        //!
        const Deletor & getDeletor() const
        {
            if( !empty() )
            {
                GN_WARN( "It is not safe to change delete functor, while resource manager is not empty." );
            }
            return mDeletor;
        }

        //!
        //! Set default creator
        //!
        void setDefaultCreator( const Creator & c ) { mDefaultCreator = c; }

        //!
        //! Set default NULL instance creator
        //!
        void setDefaultNullor( const Creator & n ) { mDefaultNullor = n; }

        //!
        //! Set deletor
        //!
        void setDeletor( const Deletor & d ) { mDeletor = d; }

        //!
        //! Clear all resources.
        //!
        void clear()
        {
            GN_GUARD;

            // dispose all resources
            dispose();

            // delete resource descriptions
            ResHandle h = mResHandles.first();
            while( h )
            {
                GN_ASSERT( mResHandles[h] );
                delete mResHandles[h];
                h = mResHandles.next( h );
            }

            // clear handles and names
            mResHandles.clear();
            mResNames.clear();

            GN_UNGUARD;
        }

        //!
        //! Is resource manager empty or not?
        //!
        bool empty() const
        {
            GN_ASSERT( mResHandles.size() == mResNames.size() );
            return mResHandles.empty() && NULL == mNullInstance;
        }

        //!
        //! Invalidate all resources. But keep all resources items.
        //!
        void dispose()
        {
            GN_GUARD;
            ResHandle h = mResHandles.first();
            while( h )
            {
                doDispose( mResHandles.get(h) );
                h = mResHandles.next( h );
            }
            if( mNullInstance )
            {
                GN_ASSERT( mNullDeletor );
                mNullDeletor( *mNullInstance );
                mNullInstance = 0;
                mNullDeletor.clear();
            }
            GN_UNGUARD;
        }

        //!
        //! Preload all resources
        //!
        bool preload()
        {
            GN_GUARD;
            RES res;
            bool ok = true;
            ResHandle h;
            for( h = mResHandles.first(); h != 0; h = mResHandles.next(h) )
            {
                ok |= getResource( res, h );
            }
            return ok;
            GN_UNGUARD;
        }

        //!
        //! Add new resource item to manager
        //!
        ResHandle addResource(
            const StrA & name,
            const Creator & creator = Creator(),
            const Creator & nullor  = Creator(),
            bool overrideExistingResource = false )
        {
            GN_GUARD;

            ResHandle h;
            ResDesc * item;
            StringMap::const_iterator ci = mResNames.find(name);
            if( mResNames.end() != ci )
            {
                if( !overrideExistingResource )
                {
                    GN_ERROR( "resource '%s' already exist!", name.cstr() );
                    return 0;
                }
                GN_ASSERT( mResHandles.validHandle(ci->second) );
                h = ci->second;
                item = mResHandles.get( h );
                doDispose( item ); // dispose existing resource
            }
            else
            {
                item = new ResDesc;
                h = mResHandles.add( item );
                if( 0 == h )
                {
                    GN_ERROR( "Fail to create new resource item!" );
                    mResNames.erase(name);
                    delete item;
                    return 0;
                }
                mResNames[name] = h;
            }
            GN_ASSERT( mResNames.size() == mResHandles.size() );
            GN_ASSERT( mResHandles.validHandle(h) && item );
            item->creator = creator;
            item->nullor = nullor;
            item->name = name;
            item->disposed = true;
            return h;

            GN_UNGUARD;
        }

        //!
        //! Remove resource from manager
        //!
        void removeResource( ResHandle handle );

        //!
        //! Remove resource from manager
        //!
        void removeResource( const StrA & name );


        //!
        //! Get resource handle
        //!
        ResHandle getResourceHandle( const StrA & name ) const
        {
            GN_GUARD_SLOW;
            StringMap::const_iterator iter = mResNames.find( name );
            return mResNames.end() != iter ? iter->second : 0;
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource name
        //!
        const StrA & getResourceName( ResHandle handle ) const
        {
            GN_GUARD_SLOW;
            if( mResHandles.validHandle(handle) )
            {
                GN_ASSERT( mResHandles.get(handle) );
                return mResHandles.get(handle)->name;
            }
            else return StrA::EMPTYSTR;
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource by handle.
        //!
        bool getResource( RES & result, ResHandle handle )
        {
            GN_GUARD_SLOW;
            return getResourceByHandle( result, handle, "UnknownName" );
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource by handle.
        //!
        //! If failed, return default constructed resource instance.
        //!
        RES getResource( ResHandle handle )
        {
            GN_GUARD_SLOW;
            RES res;
            if( getResource( res, handle ) ) return res;
            else return RES();
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource by name
        //!
        bool getResource( RES & result, const StrA & name )
        {
            GN_GUARD_SLOW;
            StringMap::const_iterator iter = mResNames.find( name );
            return getResourceByHandle(
                result,
                mResNames.end() != iter ? iter->second : 0,
                name.cstr() );
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource by name.
        //!
        //! If failed, return default constructed resource instance.
        //!
        RES getResource( const StrA & name )
        {
            GN_GUARD_SLOW;
            RES res;
            if( getResource( res, name ) ) return res;
            else return RES();
            GN_UNGUARD_SLOW;
        }

    private:

        typedef ResourceManager<RES,PARENT> MyType;

        struct ResDesc
        {
            Creator creator;
            Creator nullor; // Use to create per-resource "NULL" instance.
            RES     res;
            StrA    name;
            bool    disposed;
        };

        typedef std::map<StrA,ResHandle>          StringMap;
        typedef HandleManager<ResDesc*,ResHandle> ResHangleMgr;

        static MyType msInstance;

        ResHangleMgr mResHandles;
        StringMap    mResNames;
        Creator      mDefaultCreator;
        Creator      mDefaultNullor; // Use to create default "NULL" instance.
        Deletor      mDeletor;

        RES   * mNullInstance;
        Deletor mNullDeletor;

        bool getResourceByHandle( RES & res, ResHandle handle, const char * nullName )
        {
            GN_GUARD_SLOW;

            if( !mResHandles.validHandle(handle) )
            {
                GN_ERROR( "Resource '%s' is invalid. Fallback to null instance...", nullName );
                if( 0 == mNullInstance )
                {
                    RES * tmp = new RES;   
                    if( !mDefaultNullor ||
                        !mDefaultNullor(*tmp,nullName) )
                    {
                        GN_ERROR( "Fail to create null resource '%s'.", nullName );
                        delete tmp;
                        return false;
                    }
                    mNullInstance = tmp;
                    mNullDeletor = mDeletor;
                }
                res = *mNullInstance;
                return true;
            }

            ResDesc * item = mResHandles.get( handle );

            GN_ASSERT( item );

            if( item->disposed )
            {
                bool ok = false;
                if( item->creator )
                {
                    ok = item->creator( item->res, item->name );
                }
                else if( mDefaultCreator )
                {
                    ok = mDefaultCreator( item->res, item->name );
                }

                if( !ok )
                {
                    GN_WARN( "Fallback to null instance for resource '%s'.", item->name.cstr() );
                    if( item->nullor )
                    {
                        ok = item->nullor( item->res, item->name );
                    }
                    if( !ok && mDefaultNullor )
                    {
                        ok = mDefaultNullor( item->res, item->name );
                    }
                    if( !ok )
                    {
                        GN_ERROR( "Fail to create NULL instance for resource '%s'.", item->name.cstr() );
                        return false;
                    }
                }
                GN_ASSERT( ok );
            }

            // success
            res = item->res;
            return true;

            GN_UNGUARD_SLOW;
        }

        void doDispose( ResDesc * item )
        {
            GN_ASSERT( item );
            if( !item->disposed )
            {
                if( mDeletor ) mDeletor( item->res );
                item->disposed = true;
            }
        }
    };
}

// *****************************************************************************
//                           End of resourceManager.h
// *****************************************************************************
#endif // __GN_BASE_RESOURCEMANAGER_H__
