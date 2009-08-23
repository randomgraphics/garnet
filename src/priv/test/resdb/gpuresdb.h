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
    inline UInt32 retrieveGpuResourceInternalHandle( GpuResourceHandle handle )
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

    ///
    /// GPU resource database implementation class
    ///
    class GpuResourceDatabase::Impl
    {
        struct CreationParam : public GpuResourceCreationParameters
        {
            DynaArray<UInt8> mBuffer;
            CreationParam & operator=( const GpuResourceCreationParameters & );
        };

        struct ResourceItem
        {
            GpuResource * resource;
            CreationParam cp;
        };

        typedef NamedHandleManager<ResourceItem,UInt32> NamedGpuResMgr;

        Gpu          & mGpu;
        NamedGpuResMgr mResources[GpuResourceType::NUM_TYPES];

    public:

        //@{

        Impl( Gpu & );
        virtual ~Impl();

        void clear();

        //@}

        //@{
        GpuResourceHandle    addResource( GpuResourceType type, const char * name, const GpuResourceCreationParameters & lp );
        void                 removeResource( GpuResourceHandle );
        void                 removeAllResources();
        GpuResourceHandle    getResourceHandle( GpuResourceType type, const char * name );
        GpuResourceType      getResourceType( GpuResourceHandle );
        const char *         getResourceName( GpuResourceHandle );
        GpuResource        * getResource( GpuResourceHandle );
        void                 reloadResource( GpuResourceHandle );
        void                 reloadAllResources();
        //@}

    private:

        ResourceItem * getResourceItem( GpuResourceHandle );
        GpuResource  * createResourceInstance( GpuResourceType type, const GpuResourceCreationParameters & cp );
        void           reloadResourceItem( ResourceItem & );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_GPURESDB_H__
