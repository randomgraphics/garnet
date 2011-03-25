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
            uint32 mU32;
            struct
            {
                uint32 mIndexPlusOne   : 8;
                uint32 mInternalHandle : 24;
            };
        };

    public:

        //@{

        enum { MAX_TYPES = (2^8)-1 };

        explicit GpuResourceHandle( uint32 u32 )
            : mU32( u32 )
        {
        }

        GpuResourceHandle( size_t managerIndex, uint32 internalHandle )
        {
            mIndexPlusOne = managerIndex + 1;
            mInternalHandle = internalHandle;
        }

        void  set( uint32 managerIndex, uint32 internalHandle )
        {
            mIndexPlusOne = managerIndex + 1;
            mInternalHandle = internalHandle;
        }

        uint32 u32()            const { return mU32; }
        uint32 managerIndex()   const { return mIndexPlusOne - 1; }
        uint32 internalHandle() const { return mInternalHandle; }

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

        typedef NamedHandleManager<GpuResource::Impl*,uint32> ResourceMap;

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
        AutoRef<UniformResource>         mStdUniforms[StandardUniform::Index::NUM_STANDARD_UNIFORMS];

        // *********************************************************************
        //
        // *********************************************************************

    public:

        //@{

        Impl( GpuResourceDatabase & db, Gpu & gpu );
        virtual ~Impl();

        Gpu & getGpu() const { return mGpu; }

        //@}

        //@{
        bool registerResourceFactory( const Guid & type, const char * desc, GpuResourceFactory factory );
        bool hasResourceFactory( const Guid & type );
        //@}

        //@{
        AutoRef<GpuResource> createResource( const Guid & type, const char * name );
        AutoRef<GpuResource> findResource( const Guid & type, const char * name ) const;
        bool                 validResource( const Guid & type, const GpuResource * resource ) const;
        bool                 validResource( const GpuResource * resource ) const;
        const char *         getResourceName( const GpuResource * ) const;
        const Guid         & getResourceType( const GpuResource * ) const;
        //@}

        //@{
        void                 onResourceDelete( GpuResourceHandle handle );
        bool                 setupBuiltInResources();
        //@}

        //@{
        AutoRef<UniformResource> getStandardUniformResource( int index ) const;
        void setStandardUniform( int index, const void * data, uint32 dataSize );
        void setTransform( const Matrix44f & proj, const Matrix44f & view );
        void setLight0(
                const Vector4f & diffuse,
                const Vector4f & ambient,
                const Vector4f & specular,
                const Vector3f & position,
                const Vector3f & direction );
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
