#ifndef __GN_GFX_GPURESDB_H__
#define __GN_GFX_GPURESDB_H__
// *****************************************************************************
/// \file
/// \brief   GPU resource database implementation header
/// \author  chenli@@REDMOND (2009.8.21)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace gfx
{
    class GpuResourceHandle
    {
        union
        {
            UInt32 mU32;
            struct
            {
                UInt32 mIndexPlusOne   : 8;
                UInt32 mInternalHandle : 24;
            };
        };

    public:

        //@{

        enum { MAX_TYPES = 2^8-1 };

        explicit GpuResourceHandle( UInt32 u32 )
            : mU32( u32 )
        {
        }

        GpuResourceHandle( size_t managerIndex, UInt32 internalHandle )
        {
            mIndexPlusOne = managerIndex + 1;
            mInternalHandle = internalHandle;
        }

        void  Set( UInt32 managerIndex, UInt32 internalHandle )
        {
            mIndexPlusOne = managerIndex + 1;
            mInternalHandle = internalHandle;
        }

        UInt32 u32()            const { return mU32; }
        UInt32 managerIndex()   const { return mIndexPlusOne - 1; }
        UInt32 internalHandle() const { return mInternalHandle; }

        //@}
    };

    ///
    /// GPU resource implementation class
    ///
    class GpuResource::Impl
    {
    public:

        GpuResourceDatabase::Impl & database;
        GpuResource               & resource;
        GpuResourceHandle           handle;

        /// constructor
        Impl( GpuResourceDatabase::Impl & db, GpuResource & res );

        /// destructor
        ~Impl();
    };

    ///
    /// GPU resource database implementation class
    ///
    class GpuResourceDatabase::Impl
    {
        friend class GpuResource::Impl;

        // *********************************************************************
        // private types
        // *********************************************************************

    private:

        typedef NamedHandleManager<GpuResource::Impl*,UInt32> ResourceMap;

        struct ResourceManager
        {
            Guid                guid;
            StrA                desc;
            size_t              index; // index into manager array
            GpuResourceFactory  factory;
            ResourceMap         resources;
        };

        typedef StackArray<ResourceManager, GpuResourceHandle::MAX_TYPES> ManagerArray;

        // *********************************************************************
        // private data
        // *********************************************************************

    private:

        GpuResourceDatabase            & mDatabase;
        Gpu                            & mGpu;
        ManagerArray                     mManagers;
        DynaArray<AutoRef<GpuResource> > mBuiltInResources;

        // *********************************************************************
        //
        // *********************************************************************

    public:

        //@{

        Impl( GpuResourceDatabase & db, Gpu & gpu );
        virtual ~Impl();

        Gpu & GetGpu() const { return mGpu; }

        //@}

        //@{
        bool RegisterResourceFactory( const Guid & type, const char * desc, GpuResourceFactory factory );
        bool HasResourceFactory( const Guid & type );
        //@}

        //@{
        AutoRef<GpuResource> CreateResource( const Guid & type, const char * name );
        AutoRef<GpuResource> FindResource( const Guid & type, const char * name ) const;
        bool                 ValidResource( const Guid & type, const GpuResource * resource ) const;
        bool                 ValidResource( const GpuResource * resource ) const;
        const char *         GetResourceName( const GpuResource * ) const;
        const Guid         & GetResourceType( const GpuResource * ) const;
        //@}

        //@{
        void                 onResourceDelete( GpuResourceHandle handle );
        bool                 setupBuiltInResources();
        //@}

    private:

        inline const ResourceManager * getManager( const Guid & type ) const;
        inline ResourceManager       * getManager( const Guid & type );
        inline GpuResource::Impl     * getResourceImpl( const GpuResource * resource ) const;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_GPURESDB_H__
