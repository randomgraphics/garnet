#ifndef __GN_SCENE_RESOURCE_H__
#define __GN_SCENE_RESOURCE_H__
// *****************************************************************************
//! \file    scene/resource.h
//! \brief   Scene resource manager
//! \author  chen@@CHENLI-HOMEPC (2007.2.20)
// *****************************************************************************

///
/// alias to access scene resource manager
///
#define gSceneResMgr (GN::scene::ResourceManager::sGetInstance())

namespace GN { namespace scene
{
    typedef RefCounter                            BaseResource;       ///< basic resource class.
    typedef UInt32                                ResourceId;         ///< resource ID
    typedef Delegate1<BaseResource*,const StrA &> ResourceCreator;    ///< resource creator
    typedef Delegate1<void,BaseResource*>         ResourceDeletor;    ///< resource deletor

    ///
    /// scene resource manager.
    ///
    /// Now supports 3 types of resource: texture, effect and mesh.
    ///
    class ResourceManager : public Singleton<ResourceManager>
    {
        // *********************************************************************
        // ctor and dtor
        // *********************************************************************
    public:

        ///
        /// constructor
        ///
        ResourceManager();

        ///
        /// Destructor
        ///
        ~ResourceManager();

        // *********************************************************************
        // public functions
        // *********************************************************************
    public:

        ///
        /// return native resource file name. Return empty string if file does not exist.
        ///
        static GN::StrA sGetNativeResourceFileName( const StrA & name );

        ///
        /// register resource type
        ///
        void registerResourceType(
            const StrA & type,
            const ResourceCreator & creator,
            const ResourceDeletor & deletor );

        //@{

        ResourceId     addResource( const StrA & name, const StrA & type );
        void           removeResource( const StrA & name ) { removeResource( getResourceId(name) ); }
        void           removeResource( ResourceId );
        void           disposeResource( const StrA & name ) { disposeResource( getResourceId(name) ); }
        void           disposeResource( ResourceId );
        void           disposeAll();
        BaseResource * getResource( ResourceId id ) { return getResourceImpl( id, 0 ); }
        BaseResource * getResource( const StrA & name ) { return getResourceImpl( getResourceId(name), name.cptr() ); }
        ResourceId     getResourceId( const StrA & name ) const;
        const StrA &   getResourceName( ResourceId ) const;
        bool           validId( ResourceId id ) const { return mResources.validHandle( id ); }

        template<class T>
        inline T * getResourceT( ResourceId id )
        {
            BaseResource * r = getResource( id );
            return safeCast<T*>( r );
        }

        template<class T>
        inline T * getResourceT( const StrA & name )
        {
            BaseResource * r = getResource( name );
            return safeCast<T*>( r );
        }

        //@}

        // *********************************************************************
        // private data
        // *********************************************************************
    private :

        struct ResourceTypeDesc
        {
            StrA               type;
            ResourceCreator    creator;
            ResourceDeletor    deletor;
        };

        struct ResourceDesc
        {
            UInt32         type;     ///< index into resource type array
            StrA           name;     ///< resource name
            BaseResource * data;     ///< resource data
            bool           disposed; ///< true for disposed resource

            // LRU list
            ResourceDesc * prev;
            ResourceDesc * next;
        };

        typedef std::map<StrA,UInt32>               StringMap;
        typedef HandleManager<ResourceDesc*,UInt32> ResourcePool;

        std::vector<ResourceTypeDesc> mResourceTypes;
        StringMap                     mResourceNames; ///< mapping resource name to ID.
        ResourcePool                  mResources;     ///< hold instance of all resources.

        ResourceDesc mLRUHead; ///< head of LRU list
        ResourceDesc mLRUTail; ///< tail of LRU list

        static Logger * sLogger;

        // *********************************************************************
        // private functions
        // *********************************************************************
    private :

        BaseResource * getResourceImpl( ResourceId, const char * );
        void resolveName( StrA & out, const StrA & in ) const;
        void onRendererDispose();
    };

    ///
    /// add files under specific directory into resource manager.
    ///
    void addResourceDirectory( const StrA & path, bool recursive = true );
}}

#include "resource.inl"

// *****************************************************************************
//                           End of resource.h
// *****************************************************************************
#endif // __GN_SCENE_RESOURCE_H__
