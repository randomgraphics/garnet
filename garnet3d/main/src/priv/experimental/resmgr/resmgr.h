#ifndef __GN_SCENE_RESMGR_H__
#define __GN_SCENE_RESMGR_H__
// *****************************************************************************
/// \file
/// \brief   Resource manager used in scene model
/// \author  chenli@@REDMOND (2009.7.13)
// *****************************************************************************

namespace GN { namespace scene
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
        };
    };

    ///
    /// GPU resource handle type
    ///
    UInt32 GpuResourceHandle;

    class GpuResourceDatabase;
    class GpuResource;
    class TextureResource;
    class UniformResource;
    class EffectResource;
    class GpuMeshResource;

    class GpuResource : public RefCounter
    {
    public:

        GpuResourceHandle handle() const;
        void addReferencer( GpuResource & referencer ) const;
        void removeReferencer( GpuResource & referencer ) const;

    protected:

        void NotifyAllReferencers( int notification );

        virtual void handlNodificationFromReferencee(
            GpuResource & referencee,
            int notification ) = 0;

    private:

        ///< Other GPU resources that are referencing this resource.
        mutable DynaArray<WeakRef<GpuResource> > mReferencers;
    };

    class TextureResource : public GpuResource
    {
        gfx::Texture * mTexture;
    };

    class UniformResource : public GpuResource
    {
        gfx::Uniform * mUniform;
    };

    class EffectResource : public GpuResource
    {
        gfx::Effect * mEffect;
    };

    class GpuMeshResource : public GpuResource
    {
        gfx::GpuMesh * mMesh;
    };

    struct ModelResourceDesc
    {
        struct ModelTextureDesc
        {
            StrA             resourceName; /// if empty, then use the descriptor
            gfx::TextureDesc desc;
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
            StrA             resourceName; ///< if empty, then use the descriptor
            gfx::GpuMeshDesc desc;
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
        GN_DECLARE_STDCLASS( Model, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    protected:
        Model( GpuResourceDatabase & gdb ) : mDatabase(gdb) { clear(); }
        virtual ~Model() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const ModelResourceDesc & desc );
        void quit();
    private:
        void clear() { mDatabase = NULL; }
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

        GpuResourceDatabase        & mDatabase;
        DynaArray<GpuResourceHandle> mEffect;
        DynaArray<GpuResourceHandle> mMeshes;
        DynaArray<GpuResourceHandle> mTextures;
        DynaArray<GpuResourceHandle> mUniforms;
        DynaArray<Subset>            mSubsets;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    struct GpuResourceLoader : public NoCopy
    {
        virtual GpuResourceHandle load( const StrA & name ) = 0;
    };

    typedef GpuResourceLoader * (*GpuResourceLoaderFactory)();

    ///
    /// Map name/handle to GPU resource instance.
    ///
    class GpuResourceDatabase
    {
    public:

        //@{
        GpuResourceDatabase( gfx::GPU & );
        virtual ~GpuResourceDatabase();
        //@}

        //@{
        void prependResourceLoader( const StrA & loaderName, GpuResourceLoaderFactory );
        void appendResourceLoader( const StrA & loaderName, GpuResourceLoaderFactory );
        void removeResourceLoader( const StrA & loaderName );
        void removeAllResourceLoaders();
        //@}

        //@{
        GpuResourceHandle openResourceHandle( GpuResourceType type, const StrA & name );
        void              closeResourceHandle( GpuResourceHandle );
        void              closeAllResourceHandles();
        GpuResource     & handle2Resource( GpuResourceHandle );
        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_RESMGR_H__
