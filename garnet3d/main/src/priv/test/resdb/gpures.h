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
    typedef UInt32 GpuResourceHandle;

    class GpuResourceDatabase;

    ///
    /// GpuResource provides one level of indirection to those real graphics classes
    /// like Texture, Uniform and etc, which is necessary to support runtime resource
    /// modification and reloading.
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
        GpuResourceHandle     handle() const { return mHandle; }

        // *****************************
        // protected members
        // *****************************
    protected:

        /// protected constructor
        GpuResource( GpuResourceDatabase & db, GpuResourceHandle h )
            : mDatabase(db), mHandle(h)
        {
        }

        // *****************************
        // private members
        // *****************************
    private:

        GpuResourceDatabase & mDatabase;
        GpuResourceHandle     mHandle;
    };

    ///
    /// GPU resource factory
    ///
    struct GpuResourceFactory
    {
        GpuResource * (*createResource)( GpuResourceDatabase & db,
                                         GpuResourceHandle     handle,
                                         const void          * parameters );

        void          (*deleteResource)( GpuResource * );
    };

    ///
    /// Maps name/handle to GPU resource instance.
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
        //  1. delete all handles;
        //  3. unregister all factories.
        void clear();
        //@}

        //@{
        bool registerResourceFactory( const Guid & type, const char * descriptiveName, GpuResourceFactory factory );
        bool hasResourceFactory( const Guid & type );
        //@}

        //@{
        GpuResourceHandle    createResource( const Guid & type, const char * name, const void * parameters = NULL );
        void                 deleteResource( GpuResourceHandle );
        void                 deleteAllResources();
        GpuResourceHandle    findResource( const Guid & type, const char * name );
        const char         * getResourceName( GpuResourceHandle );
        GpuResource        * getResource( GpuResourceHandle );
        //@}

    private:
    };

    ///
    /// Texture resource
    ///
    class TextureResource : public GpuResource
    {
    public:

        /// return GUID of the texture resource class
        static const Guid & guid();

        /// create new texture resource. Would fail if the name exists.
        static GpuResourceHandle create( GpuResourceDatabase & db, const char * name, const TextureDesc & desc );

        /// load texture from file. Would return existing handle, if it is already loaded.
        static GpuResourceHandle loadFromFile( GpuResourceDatabase & db, const char * filename );

        //@{
        void      setTexture( Texture * );
        Texture * getTexture() const;
        //@}

    protected:

        /// protected constructor
        TextureResource( GpuResourceDatabase & db, GpuResourceHandle h ) : GpuResource(db,h) {}
    };

    ///
    /// comments....
    ///
    class UniformResource : public GpuResource
    {
    public:

        /// return GUID of the uniform resource class
        static const Guid & guid();

        /// create new uniform resource. Would fail if the name exists.
        static GpuResourceHandle create( GpuResourceDatabase & db, const char * name, size_t length, const void * initialData = NULL );

        /// load uniform from file. Would return existing handle, if it is already loaded.
        static GpuResourceHandle loadFromFile( GpuResourceDatabase & db, const char * filename );

        //@{
        void      setUniform( Uniform * );
        Uniform * getUniform() const;
        //@}

    protected:

        /// protected constructor
        UniformResource( GpuResourceDatabase & db, GpuResourceHandle h ) : GpuResource(db,h) {}
    };

    /*
    /// comments....
    ///
    struct GpuMeshDesc
    {
        VertexFormat  vtxfmt; ///< vertex format
        VtxBuf     *  vertices[GpuContext::MAX_VERTEX_BUFFERS]; // NULL pointer means vertex data undefined
        size_t        strides[GpuContext::MAX_VERTEX_BUFFERS];  // vertex buffer strides. 0 means using minimal stride defined by vertex format.
        size_t        offsets[GpuContext::MAX_VERTEX_BUFFERS];
        const void *  indices; // Null means index data undefined.
        PrimitiveType prim;   ///< primitive type
        size_t        numvtx; ///< number of vertices
        size_t        numidx; ///< number of indices. 0 means non-indexed mesh
    };

    ///
    /// comments....
    ///
    class GpuMeshResource : public GpuResource
    {
    public:

        void setMesh( const GpuMeshDesc & );

    protected:

        /// protected constructor
        GpuMeshResource( GpuResourceDatabase & db ) : GpuResource(db,0) {}
    };

    ///
    /// comments....
    ///
    class EffectResource : public GpuResource
    {
    public:

    protected:

        /// protected constructor
        EffectResource( GpuResourceDatabase & db ) : GpuResource(db,0) {}
    };*/

    ///
    /// comments....
    ///
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
    /// Model resource, the basic class used for rendering: myModel->render(...);
    ///
    class ModelResource : public GpuResource
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    protected:
        ModelResource( GpuResourceDatabase & db );
        virtual ~ModelResource();
        //@}

        // ********************************
        // public properties
        // ********************************
    public:

        // ********************************
        // public functions
        // ********************************
    public:

        void setTexture( const StrA & name, GpuResourceHandle );
        GpuResourceHandle getTexture( const StrA & name ) const;

        void setUniform( const StrA & name, GpuResourceHandle );
        GpuResourceHandle getUniform( const StrA & name ) const;

        void setRenderTarget( const StrA & name, GpuResourceHandle texture, size_t face, size_t leve, size_t slice );
        GpuResourceHandle getRenderTargetTexture( const StrA & name ) const;

        void setMesh( const StrA & name, GpuResourceHandle mesh );
        GpuResourceHandle getMesh( const StrA & name ) const;

        void setEffect( const StrA & name, GpuResourceHandle mesh );
        GpuResourceHandle getEffect( const StrA & name ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        /*struct Subset
        {
        };

        DynaArray<GpuResourceHandle> mEffects;
        DynaArray<GpuResourceHandle> mMeshes;
        DynaArray<GpuResourceHandle> mTextures;
        DynaArray<GpuResourceHandle> mUniforms;
        DynaArray<Subset>            mSubsets;*/

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPU_GPURES_H__
