#ifndef __GN_GFX_GPURESDB_H__
#define __GN_GFX_GPURESDB_H__
// *****************************************************************************
/// \file
/// \brief   GPU resource database implementation header
/// \author  chenli@@REDMOND (2009.8.21)
// *****************************************************************************

#include "gpures.h"

namespace GN { namespace gfx
{
    ///
    /// utility to retrieve type of the resource
    ///
    inline GpuResourceType retrieveGpuResourceType( GpuResourceHandle handle )
    {
        return (GpuResourceType)(handle >> 29);
    }

    ///
    /// utility to retrieve internal handle of the resource
    ///
    inline UInt32 retriveGpuResourceInternalHandle( GpuResourceHandle handle )
    {
        return handle & 0x1FFFFFFF;
    }

    ///
    /// Compose a public resource handle
    ///
    inline GpuResourceHandle composeGpuResourceHandle( GpuResourceType type, UInt32 internalHandle )
    {
        return (((UInt32)type)<<29) | (internalHandle & 0x1FFFFFFF);
    }

    class GpuResourceDatabase::Impl
    {
        struct ResourceItem
        {
            GpuResource resource;
        };

        typedef NamedHandleManager<ResourceItem,UInt32> NamedGpuResMgr;

        Gpu            mGpu;
        NamedGpuResMgr mResources[GpuResourceType::NUM_TYPES];

    public:

        //@{

        Impl( GPU & );
        virtual ~Impl();

        // reset to database to intial data, that is:
        //  1. invalidate all existing resource
        //  2. delete all existing handles
        //  3. remove all existing resource loaders and factories
        void clear();

        //@}

        //@{
        const char * getFirstLoaderName() const;
        void insertResourceLoader( const StrA & where, const StrA & loaderName, GpuResourceLoaderFactory );
        void removeResourceLoader( const StrA & loaderName );
        void removeAllResourceLoaders();
        //@}

        //@{
        GpuResourceHandle    addResource( GpuResourceType type, const char * name, const GpuResourceLoadingParameters & lp );
        void                 removeResource( GpuResourceHandle );
        GpuResourceHandle    getResourceHandle( GpuResourceType type, const char * name );
        GpuResourceType      getResourceType( GpuResourceHandle );
        const char *         getResourceName( GpuResourceHandle );
        AutoRef<GpuResource> getResource( GpuResourceHandle );
        void                 reloadResource( GpuResourceHandle );
        void                 reloadAllResources();
        //@}

    private:

        ResourceItem * getResourceItem( GpuResourceHandle );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_GPURESDB_H__
