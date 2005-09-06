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
        //! Singleton selector
        //!
        template<class C, int>
        struct SingletonSelector
        {
            typedef Singleton<C> type; //!< singleton type
        };

        //!
        //! Singleton selector
        //!
        template<class C>
        struct SingletonSelector<C,0>
        {
            //!
            //! non-singleton type
            //!
            struct type {};
        };
    }

    //!
    //! Resource manager.
    //!
    //! RES must support default constructor and assignment operation.
    //!
    template<typename RES, bool SINGLETON=false>
    class ResourceManager : public detail::SingletonSelector<ResourceManager<RES,SINGLETON>,SINGLETON>::type
    {
    public:

        typedef uint32_t ResHandle; //!< resource Handle. 0 means invalid handle

        typedef RES ResType; //!< resource type

        typedef Functor2<bool,RES&,const StrA &> Creator; //!< Resource creation functor

        typedef Functor1<void,RES&> Deletor; //!< Resource deletion functor

        typedef Functor1<bool,const StrA&> Searcher; //!< Resource searcher.

        //!
        //! Default constructor
        //!
        ResourceManager(
            const Creator & creator = Creator(),
            const Deletor & deletor = Deletor(),
            const Creator & nullor = makeFunctor(&defaultNullor),
            const Searcher & searcher = makeFunctor(&path::isFile) )
            : mCreator(creator)
            , mDeletor(deletor)
            , mNullor(nullor)
            , mSearcher(searcher)
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
        const Creator & getCreator() const { return mCreator; }

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
        //! Get defaut NULL instance creator
        //!
        const Creator & getNullor() const { return mNullor; }

        //!
        //! Get resource searcher
        //!
        const Searcher & getSearcher() const { return mSearcher; }

        //!
        //! Set default creator
        //!
        void setCreator( const Creator & c ) { mCreator = c; }

        //!
        //! Set default NULL instance creator
        //!
        void setNullor( const Creator & n )
        {
            mNullor = n;
            // delete old null resource instance
            if( mNullInstance )
            {
                if( mNullDeletor ) mNullDeletor( *mNullInstance );
                mNullInstance = 0;
                mNullDeletor.clear();
            }
        }

        //!
        //! Set deletor
        //!
        void setDeletor( const Deletor & d ) { mDeletor = d; }

        //!
        //! Set resource searcher
        //!
        void setSearcher( const Searcher & s ) { mSearcher = s; }

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
                if( mNullDeletor ) mNullDeletor( *mNullInstance );
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
                ok &= getResource( res, h );
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
        //! Add a bunch of new resources to manager
        //!
        void addResources(
            const std::vector<StrA> & names,
            const Creator & creator = Creator(),
            const Creator & nullor  = Creator(),
            bool overrideExistingResource = false )
        {
            GN_GUARD;
            std::vector<StrA>::const_iterator i;
            for( i = names.begin(); i != names.end(); ++i )
            {
                addResources( (*i), creator, nullor, overrideExistingResource );
            }
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
        ResHandle getResourceHandle( const StrA & name )
        {
            GN_GUARD_SLOW;
            StringMap::const_iterator iter = mResNames.find( name );
            if( mResNames.end() != iter ) return iter->second;
            if( mSearcher && mSearcher(name) ) return addResource( name );
            return 0; // failed
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
        bool getResource( RES & result, const StrA & name, bool addInvalidNameToManager = true )
        {
            GN_GUARD_SLOW;
            ResHandle h = getResourceHandle( name );
            if( 0 == h && addInvalidNameToManager ) h = addResource( name );
            return getResourceByHandle( result, h, name.cstr() );
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource by name.
        //!
        //! If failed, return default constructed resource instance.
        //!
        RES getResource( const StrA & name, bool addInvalidNameToManager = true )
        {
            GN_GUARD_SLOW;
            RES res;
            if( getResource( res, name, addInvalidNameToManager ) ) return res;
            else return RES();
            GN_UNGUARD_SLOW;
        }

    private:

        typedef ResourceManager<RES,SINGLETON> MyType;

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

        // global resource manipulators
        Creator      mCreator;
        Deletor      mDeletor;
        Creator      mNullor; // Use to create default "NULL" instance.
        Searcher     mSearcher;

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
                    if( !mNullor ||
                        !mNullor(*tmp,nullName) )
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
                else if( mCreator )
                {
                    ok = mCreator( item->res, item->name );
                }

                if( !ok )
                {
                    GN_WARN( "Fallback to null instance for resource '%s'.", item->name.cstr() );
                    if( item->nullor )
                    {
                        ok = item->nullor( item->res, item->name );
                    }
                    if( !ok && mNullor )
                    {
                        ok = mNullor( item->res, item->name );
                    }
                    if( !ok )
                    {
                        GN_ERROR( "Fail to create NULL instance for resource '%s'.", item->name.cstr() );
                        return false;
                    }
                }

                GN_ASSERT( ok );
                item->disposed = false;
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

        static inline bool defaultNullor( RES & result, const StrA & )
        {
            result = RES();
            return true;
        }
    };
}

// *****************************************************************************
//                           End of resourceManager.h
// *****************************************************************************
#endif // __GN_BASE_RESOURCEMANAGER_H__
