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
    /// Cast GPU resource pointer with type check.
    ///
    template<typename T>
    inline T * safeCastResource( GpuResource * r )
    {
        GN_ASSERT( 0 == r || T::guid() == *r->database().getResourceType( r->handle() ) );
        return (T*)r;
    }

    ///
    /// Cast GPU resource pointer with type check.
    ///
    template<typename T>
    inline T & safeCastResource( GpuResource & r )
    {
        GN_ASSERT( T::guid() == *r.database().getResourceType( r.handle() ) );
        return (T&)r;
    }

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
        bool                 checkHandle( GpuResourceHandle ) const;
        GpuResourceHandle    findResource( const Guid & type, const char * name ) const;
        const char         * getResourceName( GpuResourceHandle ) const;
        const Guid         * getResourceType( GpuResourceHandle ) const;
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
    /// Uniform resource
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

    ///
    /// comments....
    ///
    struct MeshResourceDesc
    {
        PrimitiveType prim;   //< primitive type
        size_t        numvtx; //< number of vertices
        size_t        numidx; //< number of indices. 0 means non-indexed mesh
        VertexFormat  vtxfmt; //< vertex format
        const void *  vertices[GpuContext::MAX_VERTEX_BUFFERS]; // NULL pointer means vertex data are undefined
        size_t        strides[GpuContext::MAX_VERTEX_BUFFERS];  // vertex buffer strides. 0 means using vertex size defined by vertex format.
        size_t        offsets[GpuContext::MAX_VERTEX_BUFFERS];
        const void *  indices; // Null means index data are undefined.
    };

    ///
    /// comments....
    ///
    class MeshResource : public GpuResource
    {
    public:

        /// factory
        //@{
        static const Guid & guid();
        static GpuResourceHandle create( GpuResourceDatabase & db, const char * name, const MeshResourceDesc & desc );
        static GpuResourceHandle loadFromFile( GpuResourceDatabase & db, const char * filename );
        //@}

        //@{
        void setContext( GpuContext * );
        //@}

    protected:

        /// protected constructor
        MeshResource( GpuResourceDatabase & db ) : GpuResource(db,0) {}

    private:

        class Impl;
        Impl * mImpl;
    };

    ///
    /// Effect resource description
    ///
    struct EffectResourceDesc
    {
        ///
        /// Shader Prerequisites
        ///
        struct ShaderPrerequisites
        {
            UInt32 numTextures; //< minimal number of textures required.

            /// default constructor
            ShaderPrerequisites()
                : numTextures(0)
            {
            }
        };

        ///
        /// Texture descriptor
        ///
        struct EffectTextureDesc
        {
            /// sampler descriptor
            SamplerDesc sampler;

            /// default ctor
            EffectTextureDesc() { sampler.clear(); }
        };

        ///
        /// Uniform descriptor
        ///
        struct EffectUniformDesc
        {
            size_t size;   //< uniform size

            /// default ctor
            EffectUniformDesc() : size(0) {}
        };

        ///
        /// Effect render target descriptor
        ///
        struct EffectRenderTargetDesc
        {
            // TODO: minimal size, format and usage requirements, and etc.
        };

        ///
        /// Shader descriptor
        ///
        struct EffectShaderDesc
        {
            ShaderPrerequisites prerequisites;      //< prerequisites of the shader.
            GpuProgramDesc      gpd;                //< GPU Program descriptor
            std::map<StrA,StrA> textures;           //< textures. Key is shader parameter name, value is user-visible texture name.
            std::map<StrA,StrA> uniforms;           //< uniforms. Key is shader parameter name, value is user-visible uniform name.
        };

        /// template for single render state
        template<typename T>
        struct EffectRenderState
        {
            T    value;
            bool inherited; //< if true, then this effect will inherit this value from current GPU context.

            /// default ctor
            EffectRenderState() : inherited(true) {}

            /// set render state value
            EffectRenderState & operator=( const T & rhs )
            {
                value = rhs;
                inherited = false;
                return *this;
            }

            /// set render state value
            EffectRenderState & operator=( const EffectRenderState<T> & rhs )
            {
                value = rhs.value;
                inherited = rhs.inherited;
                return *this;
            }
        };

        /// render state block
        struct EffectRenderStateDesc
        {
            EffectRenderState<UInt8> fillMode;
            EffectRenderState<UInt8> cullMode;
            EffectRenderState<UInt8> frontFace;
            EffectRenderState<bool>  msaaEnabled;
            EffectRenderState<bool>  depthTest;
            EffectRenderState<bool>  depthWrite;
            EffectRenderState<UInt8> depthFunc;

            // TODO: more render states.

            void clear()
            {
                fillMode.inherited = true;
                cullMode.inherited = true;
                frontFace.inherited = true;
                msaaEnabled.inherited = true;
                depthTest.inherited = true;
                depthWrite.inherited = true;
                depthFunc.inherited = true;
            }
        };

        ///
        /// Rendering pass descriptor
        ///
        struct EffectPassDesc
        {
            StrA                  shader;       //< Name of shader used in this pass. Can't be empty
            EffectRenderStateDesc rsd;          //< pass specific render states
            DynaArray<StrA>       colortargets; //< color render targets. Values are user-visible render target names.
            StrA                  depthstencil; //< depth render targets. Value is user-visible render target name.

            EffectPassDesc()
            {
                colortargets.resize( 1 );
                colortargets[0] = "color0";
                depthstencil = "depth";
            }
        };

        ///
        /// Technique descriptor structure
        ///
        struct EffectTechniqueDesc
        {
            int                       quality; //< user defined rendering quality. Effect class uses
                                               //< the technique with the hightest quality as default technique.
            DynaArray<EffectPassDesc> passes;  //< pass list.
            EffectRenderStateDesc     rsd;     //< Technique specific render states

            /// default ctor
            EffectTechniqueDesc() : quality(100) {}
        };

        std::map<StrA,EffectTextureDesc>      textures;      //< Texture list
        std::map<StrA,EffectUniformDesc>      uniforms;      //< Uniform list
        std::map<StrA,EffectRenderTargetDesc> rendertargets; //< Render taret list. Empty means using default setttings: one "color0", one "depth".
        std::map<StrA,EffectShaderDesc>       shaders;       //< Shader list
        std::map<StrA,EffectTechniqueDesc>    techniques;    //< Technique list. Technique name must be unique.
        EffectRenderStateDesc                 rsd;           //< effect specific render states

        ///
        /// constructor
        ///
        EffectResourceDesc()
        {
            clear();
        }

        ///
        /// Make sure the effect descriptor is valid.
        ///
        bool valid() const;

        ///
        /// clear to a default descriptor
        ///
        void clear()
        {
            textures.clear();
            uniforms.clear();
            rendertargets.clear();
            shaders.clear();
            techniques.clear();
            rsd.clear();

        }

        ///
        /// setup the descriptor from XML
        ///
        bool loadFromXmlNode( const XmlNode & root, const char * basedir );

        ///
        /// write the descriptor to XML
        ///
        void saveToXmlNode( const XmlNode & root );
    };

    ///
    /// Effect defines GPU program and render states, and how textures,
    /// uniforms and render targets are binded to GPU context.
    ///
    class EffectResource : public GpuResource
    {
    public:

        /// effect factory
        //@{

        /// return GUID of the effect resource class
        static const Guid & guid();

        /// create new effect resource. Would fail if the name exists.
        static GpuResourceHandle create( GpuResourceDatabase & db, const char * name, const EffectResourceDesc & desc );

        /// load effect from file. Would return existing handle, if it is already loaded.
        static GpuResourceHandle loadFromFile( GpuResourceDatabase & db, const char * filename );

        //@

        /// Effect properties
        //@{

        struct BindingLocation
        {
            UInt8 pass;
            UInt8 stage;
        };

        struct ParameterProperties
        {
            const char               * effectParameterName;
            DynaArray<BindingLocation> bindings;
        };

        struct TextureProperties : public ParameterProperties
        {
            SamplerDesc sampler;
        };

        static const size_t PARAMETER_NOT_FOUND = 0xFFFFFFFF;

        size_t                    getNumPasses() const;

        size_t                    getNumTextures() const;
        size_t                    findTextureByName( const char * name ) const;
        const TextureProperties & getTextureProperties( size_t i ) const;

        void                      applyGpuProgramAndRenderStates( size_t pass, GpuContext & gc ) const;

        //@}

     protected:

        /// protected constructor
        EffectResource( GpuResourceDatabase & db, GpuResourceHandle h ) : GpuResource(db,h) {}

    private:

        class Impl;
        Impl * mImpl;
    };

    ///
    /// Model resource descriptor
    ///
    struct ModelResourceDesc
    {
        struct ModelTextureDesc
        {
            StrA        resourceName; /// if empty, then create a new texture using the descriptor
            TextureDesc desc;
        };

        struct ModelUniformDesc
        {
            StrA             resourceName; //< if empty, then create a new uniform
            size_t           size;
            DynaArray<UInt8> defaultValue; //< if empty, then no default value.
        };

        StrA                            effectResourceName; //< effect resource name. If empty, then create a new effect using effectDesc
        EffectResourceDesc              effectResourceDesc; //< Used to create new effect, if effect resource name is empty.
        std::map<StrA,ModelTextureDesc> textures;           //< key is effect parameter name
        std::map<StrA,ModelUniformDesc> uniforms;           //< key is effect parameter name

        StrA                            meshResourceName; //< if empty, then create a new mesh using meshDesc
        MeshResourceDesc                meshResourceDesc; //< Used to create new mesh, if mesh resource name is empty
        GpuMeshSubset                   subset;           //< Mesh subset information.
    };

    ///
    /// Model resource, the basic class used for rendering: myModel->render(...);
    ///
    class ModelResource : public GpuResource
    {
        // ********************************
        // public functions
        // ********************************
    public:

        //@{
        static const Guid      & guid();
        static GpuResourceHandle create( GpuResourceDatabase & db, const char * name, const ModelResourceDesc & desc );
        static GpuResourceHandle loadFromFile( GpuResourceDatabase & db, const char * filename );
        //@}

        //@{
        void              setTexture( const char * effectParameterName, GpuResourceHandle );
        GpuResourceHandle getTexture( const char * effectParameterName ) const;

        void              setUniform( const char * effectParameterName, GpuResourceHandle );
        GpuResourceHandle getUniform( const char * effectParameterName ) const;

        void              setRenderTarget( const char * effectParameterName, GpuResourceHandle, size_t face, size_t level, size_t slice );
        GpuResourceHandle getRenderTarget( const char * effectParameterName, size_t * face = NULL, size_t * level = NULL, size_t * slice = NULL ) const;

        void              setMesh( GpuResourceHandle mesh, const GpuMeshSubset * subset = NULL );
        GpuResourceHandle getMesh( GpuMeshSubset * subset = NULL ) const;

        const GpuContext & getGC() const;
        //@}

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    protected:
        ModelResource( GpuResourceDatabase & db, GpuResourceHandle h );
        virtual ~ModelResource();
        //@}

        // ********************************
        // Impl
        // ********************************

    private:

        class Impl;
        Impl * mImpl;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPU_GPURES_H__
