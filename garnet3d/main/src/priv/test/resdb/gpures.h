#ifndef __GN_GPU_GPURES_H__
#define __GN_GPU_GPURES_H__
// *****************************************************************************
/// \file
/// \brief   GPU resource classes
/// \author  chenli@@REDMOND (2009.8.13)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace gfx
{
    ///
    /// GPU resource type
    ///
    struct GpuResourceType
    {
        enum ENUM
        {
            TEXTURE,
            UNIFORM,
            EFFECT,
            MESH,
            MODEL,
            NUM_TYPES,
            INVALID = NUM_TYPES,
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( GpuResourceType, ENUM );
    };

    typedef UInt32 GpuResourceHandle;

    class GpuResourceDatabase;
    class GpuResource;
    class TextureResource;
    class UniformResource;
    class EffectResource;
    class GpuMeshResource;

    ///
    /// comments....
    ///
    class GpuResource : public NoCopy
    {
        // *****************************
        // public events
        // *****************************
    public:

        /// This event is fired when the underlying GPU resource pointer is changed, like
        /// reloaded by database, or explicity changed by user.
        ///
        /// Note that this event does not care about the content of the resource. If only
        /// the content of the resource is changed, but the resource pointer remains same,
        /// then this event won't be fired.
        Signal1<void, GpuResource&> sigUnderlyingResourcePointerChanged;

        // *****************************
        // public methods
        // *****************************
    public:

        GpuResourceDatabase & database() const { return mDatabase; }
        GpuResourceHandle     handle()   const { return mHandle; }

        // *****************************
        // protected members
        // *****************************
    protected:

        /// protected constructor
        GpuResource( GpuResourceDatabase & db ) : mDatabase(db)
        {
        }

        /// protected handle value
        GpuResourceHandle mHandle;

        // *****************************
        // private members
        // *****************************
    private:

        GpuResourceDatabase & mDatabase;
    };

    template<class T>
    class SimpleTypedGpuResource : public GpuResource
    {
        const T & get() const
        {
            return mValue;
        }

        void set( const T & v )
        {
            if( v != mValue )
            {
                mValue = v;
                sigUnderlyingResourcePointerChanged(*mValue);
            }
        }

    protected:

        /// protected constructor
        SimpleTypedGpuResource( GpuResourceDatabase & db ) : GpuResource(db) {}

    private:

        T mValue;
    };

    class TextureResource : public SimpleTypedGpuResource<AutoRef<Texture> >
    {
    protected:

        /// protected constructor
        TextureResource( GpuResourceDatabase & db ) : SimpleTypedGpuResource(db) {}

    };

    class UniformResource : public SimpleTypedGpuResource<AutoRef<Uniform> >
    {
    protected:

        /// protected constructor
        UniformResource( GpuResourceDatabase & db ) : SimpleTypedGpuResource(db) {}

    };

    class EffectResource : public GpuResource
    {
        Effect * mEffect;

    protected:

        /// protected constructor
        EffectResource( GpuResourceDatabase & db ) : GpuResource(db) {}

    };

    class GpuMeshResource : public GpuResource
    {
        GpuMesh * mMesh;

    protected:

        /// protected constructor
        GpuMeshResource( GpuResourceDatabase & db ) : GpuResource(db) {}

    };

    struct ModelResourceDesc
    {
        struct ModelTextureDesc
        {
            StrA        resourceName; /// if empty, then use the descriptor
            TextureDesc desc;
        };

        struct ModelUniformDesc
        {
            StrA             resourceName; ///< if empty, then create a new uniform
            size_t           size;
            DynaArray<UInt8> defaultValue; ///< if empty, then no default value.
        };

        struct ModelEffectDesc
        {
            StrA                resourceName;    // effect file name
            std::map<StrA,StrA> textureBindings; // bind texture (value) to effect parameter (key)
            std::map<StrA,StrA> uniformBindings; // bind uniform (value) to effect parameter (key)
        };

        struct ModelMeshDesc
        {
            StrA        resourceName; ///< if empty, then use the descriptor
            GpuMeshDesc desc;
        };

        struct ModelSubsetDesc
        {
            StrA   effect;
            StrA   mesh;

            // note: (0,0,0,0) means full mesh
            size_t startvtx;
            size_t numvtx;
            size_t startidx;
            size_t numidx;
        };

        std::map<StrA,ModelEffectDesc>  effects;
        std::map<StrA,ModelMeshDesc>    meshes;
        std::map<StrA,ModelTextureDesc> textures;
        std::map<StrA,ModelUniformDesc> uniforms;

        DynaArray<ModelSubsetDesc>      subsets;
    };

    ///
    /// Model, a glue class for various GPU resources, such as effect, mesh and textures.
    ///
    /// The top level class used for rendering: myModel->render(...);
    ///
    class ModelResource : public GpuResource, public StdClass
    {
        GN_DECLARE_STDCLASS( ModelResource, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    protected:
        ModelResource( GpuResourceDatabase & db ) : GpuResource(db) { clear(); }
        virtual ~ModelResource() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const ModelResourceDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public properties
        // ********************************
    public:

        template<typename T>
        class ParameterCollection
        {
        };

        ParameterCollection<TextureResource> textures;
        ParameterCollection<UniformResource> uniforms;

        // ********************************
        // public functions
        // ********************************
    public:

        void clone();

        void render();

        // ********************************
        // private variables
        // ********************************
    private:

        struct Subset
        {
        };

        DynaArray<GpuResourceHandle> mEffects;
        DynaArray<GpuResourceHandle> mMeshes;
        DynaArray<GpuResourceHandle> mTextures;
        DynaArray<GpuResourceHandle> mUniforms;
        DynaArray<Subset>            mSubsets;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    struct TextureSubsurfaceData
    {
        const void * data;
        size_t       rowPitch;
        size_t       slicePitch;
    };

    struct TextureCreationParameters
    {
        TextureDesc             desc;
        TextureSubsurfaceData * initalData;
    };

    struct UniformCreationParameters
    {
        size_t       length;
        const void * initialData;
    };

    struct GpuResourceCreationParameters
    {
        const char              * filename;
        TextureCreationParameters tcp;
        UniformCreationParameters ucp;
    };

    ///
    /// Maps name/handle to GPU resource instance. Also manages resource
    /// loading/unloading from file system.
    ///
    class GpuResourceDatabase
    {
        class Impl;

        Impl * mImpl;

    public:

        //@{
        GpuResourceDatabase( Gpu & );
        virtual ~GpuResourceDatabase();

        // reset the database to intial state, that is:
        //  1. unload all resources;
        //  2. remove all handles;
        //  3. remove all loaders and factories.
        void clear();
        //@}

        //@{
        GpuResourceHandle    addResource( GpuResourceType type, const char * name, const GpuResourceCreationParameters & cp );
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
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPU_GPURES_H__
