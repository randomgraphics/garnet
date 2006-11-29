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
    //! Resource manager. Used to do mapping among name and ID and instance.
    //!
    //! RES must support default constructor and assignment operation.
    //!
    //! 资源管理器使用4个回调函数来管理资源的创建和删除: 
    //! - Creator: 创建函数
    //!   - 用来创建资源实例. 管理器有一个全局的创建函数, 供所有的资源使用. 同时, 每个资源
    //!     也可以拥有自己特定的创建函数.
    //! - Deletor: 删除函数
    //!   - 用来删除资源实例. 管理器有且仅有一个全局的删除函数, 供所有资源使用.
    //! - Nullor: 空函数
    //!   - 当资源创建失败(Creator返回null)时, 管理器会调用Nullor来创建一个所谓的"空"对象,
    //!     用来替代那个失败的资源.
    //!   - 和Creator类似, 管理器有一个全局的空函数, 供所有资源使用. 同时, 每个资源也可以有
    //!     自己特定的空函数.
    //!   - 使用空对象的目的就是: 保证资源管理器总是会返回有效的资源实例. 这样在使用者的代码中
    //!     就可以避免如下的代码:
    //!         MyResource * ptr = myResourceMgr.getResource( theHandle );
    //!         if( ptr )
    //!             do_something_normal();
    //!         else
    //!             report_error();
    //!   - 空对象应尽量容易引起使用者的注意, 且不会引起程序崩溃.
    //!     - 比如可以用纯红色的1x1贴图作为空贴图, 用一个大方块作为空mesh.
    //! - NameChecker: 名字检查函数
    //!   - 当用户试图用名字引用一个不存在的资源时, 管理器会调用NameCheker来检查这个名字的有效性,
    //!     并把有效的名字自动加入资源管理器中(参见getResourceHandle()的代码).
    //!   - 一个常用的NameChecker就是检查该名字是否对应一个有效的磁盘文件. 这样, 当用户试图访问一个
    //!     不在资源管理器内, 但存在于磁盘上的资源时, 该资源就会被自动加入资源管理器.
    //!
    template<typename RES, typename HANDLE=uint32_t, bool SINGLETON=false>
    class ResourceManager : public detail::SingletonSelector<ResourceManager<RES,HANDLE,SINGLETON>,SINGLETON>::type
    {
    public:

        typedef HANDLE HandleType; //!< resource Handle. 0 means invalid handle

        typedef RES ResType; //!< resource type

        typedef Delegate3<bool,RES&,const StrA &,void*> Creator; //!< Resource creation functor

        typedef Delegate2<void,RES&,void*> Deletor; //!< Resource deletion functor

        typedef Delegate1<bool,const StrA&> NameChecker; //!< Resource name checker.

        //!
        //! Default constructor
        //!
        ResourceManager(
            const Creator & creator = Creator(),
            const Deletor & deletor = Deletor(),
            const Creator & nullor = Creator(),
            const NameChecker & checker = NameChecker() )
            : mCreator(creator)
            , mDeletor(deletor)
            , mNullor(nullor)
            , mNameChecker(checker)
            , mNullInstance(0)
        {
            mLRUHead.prev = NULL;
            mLRUHead.next = &mLRUTail;
            mLRUTail.prev = &mLRUHead;
            mLRUTail.next = NULL;
        }

        //!
        //! Default destructor
        //!
        ~ResourceManager() { clear(); }

        //!
        //! Get global creator.
        //!
        const Creator & getCreator() const { return mCreator; }

        //!
        //! Get global resource deletor.
        //!
        const Deletor & getDeletor() const
        {
            if( !empty() )
            {
                GN_WARN(sLogger)( "It is not safe to change delete functor, while resource manager is not empty." );
            }
            return mDeletor;
        }

        //!
        //! Get global NULL instance creator
        //!
        const Creator & getNullor() const { return mNullor; }

        //!
        //! Get global resource name checker
        //!
        const NameChecker & getNameChecker() const { return mNameChecker; }

        //!
        //! Set global creator
        //!
        void setCreator( const Creator & c ) { mCreator = c; }

        //!
        //! Set global NULL instance creator
        //!
        void setNullor( const Creator & n )
        {
            deleteNullInstance();
            mNullor = n;
        }

        //!
        //! Set global deletor
        //!
        void setDeletor( const Deletor & d ) { mDeletor = d; }

        //!
        //! Set global resource name checker
        //!
        void setNameChecker( const NameChecker & s ) { mNameChecker = s; }

        //!
        //! Clear all resources.
        //!
        void clear()
        {
            GN_GUARD;

            // dispose all resources
            disposeAll();

            // delete resource descriptions
            HandleType h = mResHandles.first();
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
        //! Return true for valid resource handle
        //!
        bool validResourceHandle( HandleType h ) const { return mResHandles.validHandle( h ); }

        //!
        //! Return true for valid resource name
        //!
        bool validResourceName( const StrA & n ) const { return mResNames.end() != mResNames.find( n ); }

        //!
        //! Get resource by handle.
        //!
        bool getResource( RES & result, HandleType handle )
        {
            GN_GUARD_SLOW;
            return getResourceImpl( result, handle, 0 );
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource by handle.
        //!
        //! If failed, return default constructed resource instance.
        //!
        RES getResource( HandleType handle )
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
        //! \sa getResourceHandle()
        //!
        bool getResource( RES & result, const StrA & name, bool autoAddNewName = true )
        {
            GN_GUARD_SLOW;
            HandleType h = getResourceHandle( name, autoAddNewName );
            return getResourceImpl( result, h, name.cptr() );
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource by name.
        //!
        //! If failed, return default constructed resource instance.
        //!
        //! \sa getResourceHandle()
        //!
        RES getResource( const StrA & name, bool autoAddNewName = true )
        {
            GN_GUARD_SLOW;
            RES res;
            if( getResource( res, name, autoAddNewName ) ) return res;
            else return RES();
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource handle
        //!
        //! \param name
        //!     User specified resource name.
        //! \param autoAddNewName
        //!     - If true, when the resource name that is not in manager currently but pass name-checker,
        //!       it'll be add to manager automatically, and a valid handle will be return.
        //!     - If false, return 0 for non-exist resource name.
        //!
        HandleType getResourceHandle( const StrA & name, bool autoAddNewName = true )
        {
            GN_GUARD_SLOW;
            typename StringMap::const_iterator iter = mResNames.find( name );
            if( mResNames.end() != iter ) return iter->second;
            if( autoAddNewName && ( !mNameChecker || mNameChecker(name) ) ) return addResource( name );
            return 0; // failed
            GN_UNGUARD_SLOW;
        }

        //!
        //! Get resource name
        //!
        const StrA & getResourceName( HandleType handle ) const
        {
            GN_GUARD_SLOW;
            if( validResourceHandle(handle) )
            {
                GN_ASSERT( mResHandles.get(handle) );
                return mResHandles.get(handle)->name;
            }
            else return StrA::EMPTYSTR;
            GN_UNGUARD_SLOW;
        }

        //!
        //! Add new resource item to manager
        //!
        HandleType addResource(
            const StrA & name,
            void * userData = 0,
            const Creator & creator = Creator(),
            const Creator & nullor  = Creator(),
            bool overrideExistingResource = false )
        {
            GN_GUARD;

            HandleType h;
            ResDesc * item;
            typename StringMap::const_iterator ci = mResNames.find(name);
            if( mResNames.end() != ci )
            {
                if( !overrideExistingResource )
                {
                    GN_ERROR(sLogger)( "resource '%s' already exist!", name.cptr() );
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
                    GN_ERROR(sLogger)( "Fail to create new resource item!" );
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
            item->userData = userData;
            item->disposed = true;
            return h;

            GN_UNGUARD;
        }

        //!
        //! Remove resource from manager
        //!
        void removeResourceByHandle( HandleType handle )
        {
            GN_GUARD;
            if( validResourceHandle(handle) )
                removeResourceByName( mResHandles[handle]->name );
            else
                GN_ERROR(sLogger)( "invalid resource handle: %d", handle );
            GN_UNGUARD;
        }

        //!
        //! Remove resource from manager (unimplemented)
        //!
        void removeResourceByName( const StrA & name )
        {
            GN_GUARD;

            // find the resource
            typename StringMap::iterator iter = mResNames.find( name );
            if( mResNames.end() == iter )
            {
                GN_ERROR(sLogger)( "invalid resource name: %s", name.cptr() );
                return;
            }

            // get the resource handle and pointer
            HandleType h = iter->second;
            ResDesc * r = mResHandles[h];
            GN_ASSERT( r );

            // dispose it
            doDispose( r );

            // remove it from handle and name manager
            mResHandles.remove( h );
            mResNames.erase( iter );

            // remove it from LRU list
            if( r->prev )
            {
                GN_ASSERT( r->next );
                r->prev->next = r->next;
                r->next->prev = r->prev;
            }

            // delete it
            delete r;

            GN_UNGUARD;
        }

        //!
        //! Dispose specific resource
        //!
        void disposeResourceByHandle( HandleType h )
        {
            GN_GUARD;
            if( !validResourceHandle( h ) )
            {
                GN_ERROR(sLogger)( "invalid resource handle: %d", h );
                return;
            }
            doDispose( mResHandles[h] );
            GN_UNGUARD;
        }

        //!
        //! Dispose specific resource
        //!
        void disposeResourceByName( const StrA & name )
        {
            GN_GUARD;
            typename StringMap::const_iterator iter = mResNames.find( name );
            if( mResNames.end() == iter )
            {
                GN_ERROR(sLogger)( "invalid resource name: %s", name.cptr() );
                return;
            }
            disposeResourceByHandle( iter->second );
            GN_UNGUARD;
        }

        //!
        //! Release all resource instances. But keep resource manager itself unchanged.
        //!
        void disposeAll()
        {
            GN_GUARD;
            HandleType h = mResHandles.first();
            while( h )
            {
                doDispose( mResHandles.get(h) );
                h = mResHandles.next( h );
            }
            deleteNullInstance();
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
            HandleType h;
            for( h = mResHandles.first(); h != 0; h = mResHandles.next(h) )
            {
                ok &= getResource( res, h );
            }
            return ok;
            GN_UNGUARD;
        }

        //!
        //! Set user data for specfic resource
        //!
        void setUserData( HandleType h, void * data )
        {
            if( !validResourceHandle(h) )
            {
                GN_ERROR(sLogger)( "invalid resource handle: %d", h );
                return;
            }
            GN_ASSERT( mResHandles[h] );
            mResHandles[h]->userData = data;
        }

        // *****************************
        // private variables
        // *****************************

    private:

        struct ResDesc
        {
            Creator creator;
            Creator nullor; // Use to create per-resource "NULL" instance.
            RES     res;
            StrA    name;
            void *  userData;
            bool    disposed;

            // LRU list
            ResDesc * prev;
            ResDesc * next;

            ResDesc() : userData(0), prev(0), next(0) {}
        };

        typedef std::map<StrA,HandleType>          StringMap;
        typedef HandleManager<ResDesc*,HandleType> ResHandleMgr;

        ResHandleMgr mResHandles;
        StringMap    mResNames;

        ResDesc      mLRUHead;
        ResDesc      mLRUTail;

        // global resource manipulators
        Creator      mCreator;
        Deletor      mDeletor;
        Creator      mNullor; // Use to create default "NULL" instance.
        NameChecker  mNameChecker;

        RES   * mNullInstance;
        Deletor mNullDeletor;

        static Logger * sLogger;

        // *****************************
        // private methods
        // *****************************

    private:

        bool getResourceImpl( RES & res, HandleType handle, const char * name )
        {
            GN_GUARD_SLOW;

            if( !validResourceHandle(handle) )
            {
                if( name )
                    GN_ERROR(sLogger)( "Resource '%s' is invalid. Fall back to null instance...", name );
                else
                    GN_ERROR(sLogger)( "Resource handle '%d' is invalid. Fall back to null instance...", handle );

                if( 0 == mNullInstance )
                {
                    RES * tmp = new RES;
                    if( !mNullor || !mNullor( *tmp, name, 0 ) )
                    {
                        if( name )
                            GN_ERROR(sLogger)( "Fail to create null instance for resource '%s'.", name );
                        else
                            GN_ERROR(sLogger)( "Fail to create null instance for resource handle '%d'.", handle );
                        delete tmp;
                        return false;
                    }
                    mNullInstance = tmp;
                    mNullDeletor = mDeletor;
                }
                GN_ASSERT( mNullInstance );
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
                    ok = item->creator( item->res, item->name, item->userData );
                }
                else if( mCreator )
                {
                    ok = mCreator( item->res, item->name, item->userData );
                }

                if( !ok )
                {
                    GN_WARN(sLogger)( "Fall back to null instance for resource '%s'.", item->name.cptr() );
                    if( item->nullor )
                    {
                        ok = item->nullor( item->res, item->name, item->userData );
                    }
                    if( !ok && mNullor )
                    {
                        ok = mNullor( item->res, item->name, item->userData );
                    }
                    if( !ok )
                    {
                        GN_ERROR(sLogger)( "Fail to create NULL instance for resource '%s'.", item->name.cptr() );
                        return false;
                    }
                }

                GN_ASSERT( ok );
                item->disposed = false;
            }

            // adjust access queue: move item to head of LRU list.
            if( item->prev )
            {
                GN_ASSERT( item->next );
                item->prev->next = item->next;
                item->next->prev = item->prev;
            }
            item->next = mLRUHead.next;
            item->prev = &mLRUHead;
            mLRUHead.next = item;

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
                if( mDeletor ) mDeletor( item->res, item->userData );
                item->disposed = true;
            }
        }

        void deleteNullInstance()
        {
            if( mNullInstance )
            {
                if( mNullDeletor )
                {
                    mNullDeletor( *mNullInstance, 0 );
                    mNullDeletor.clear();
                }
                delete mNullInstance;
                mNullInstance = 0;
            }
            GN_ASSERT( !mNullDeletor && !mNullInstance );
        }
    };

    template<typename RES, typename HANDLE, bool SINGLETON>
    GN::Logger * ResourceManager<RES,HANDLE,SINGLETON>::sLogger = getLogger("GN.base.ResourceManager");

}

// *****************************************************************************
//                           End of resourceManager.h
// *****************************************************************************
#endif // __GN_BASE_RESOURCEMANAGER_H__
