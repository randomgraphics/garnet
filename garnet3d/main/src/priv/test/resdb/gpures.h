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
        // ctor / dtor
        // *****************************

        //@{
    protected:
        GpuResource( GpuResourceDatabase & db, GpuResourceHandle h );
        virtual ~GpuResource();
        //@}

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

        /// common resource properties
        //@{
        GpuResourceDatabase & database() const { return mDatabase; }
        GpuResourceHandle     handle() const { return mHandle; }
        //@}

        /// Cast GPU resource pointer with type check.
        //@{
        template<typename T> static inline T * castTo( GpuResource * r );
        template<typename T> static inline T & castTo( GpuResource & r );
        template<typename T>        inline T & castTo();
        template<typename T>        inline const T & castTo() const;
        //@}

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
        /// create new resource instance
        GpuResource * (*createResource)( GpuResourceDatabase & db,
                                         GpuResourceHandle     handle,
                                         const void          * parameters );

        /// delete resource instance
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
        //  2. unregister all factories.
        void clear();

        Gpu & gpu() const;

        //@}

        //@{
        bool registerResourceFactory( const Guid & type, const char * descriptiveName, GpuResourceFactory factory );
        bool hasResourceFactory( const Guid & type );
        //@}

        //@{
        GpuResourceHandle    createResource( const Guid & type, const char * name, const void * parameters );
        void                 deleteResource( GpuResourceHandle );
        void                 deleteAllResources();
        bool                 checkHandle( GpuResourceHandle ) const;
        GpuResourceHandle    findResource( const Guid & type, const char * name ) const;
        const char         * getResourceName( GpuResourceHandle ) const;
        const Guid         * getResourceType( GpuResourceHandle ) const;
        GpuResource        * getResource( GpuResourceHandle );
        GpuResource        * getResource( const Guid & type, const char * name ) { return getResource( findResource( type, name ) ); }
        //@}

    private:
    };

    ///
    /// Texture resource
    ///
    class TextureResource : public GpuResource
    {
    public:

        /// Texture resource factory
        //@{

        /// return GUID of the texture resource class
        static const Guid & guid();

        /// create new texture resource. Would fail if the name exists.
        static GpuResourceHandle create( GpuResourceDatabase & db, const char * name, const TextureDesc & desc );

        /// load texture from file. Would return existing handle, if it is already loaded.
        static GpuResourceHandle loadFromFile( GpuResourceDatabase & db, const char * filename );

        //@}

        /// Texture resource properties
        //@{
        void                     setTexture( const AutoRef<Texture> & );
        const AutoRef<Texture> & getTexture() const { return mTexture; }
        //@}

    private:

        AutoRef<Texture> mTexture;

        TextureResource( GpuResourceDatabase & db, GpuResourceHandle h ) : GpuResource(db,h) {}
        virtual ~TextureResource() {}

        static GpuResource * createInstance( GpuResourceDatabase &, GpuResourceHandle, const void *);
        static void          deleteInstance( GpuResource * );
    };

    ///
    /// Uniform resource
    ///
    class UniformResource : public GpuResource
    {
    public:

        /// Uniform resource factory
        //@{

        /// return GUID of the uniform resource class
        static const Guid & guid();

        /// create new uniform resource. Would fail if the name exists.
        static GpuResourceHandle create( GpuResourceDatabase & db, const char * name, size_t length, const void * initialData = NULL );

        /// load uniform from file. Would return existing handle, if it is already loaded.
        static GpuResourceHandle loadFromFile( GpuResourceDatabase & db, const char * filename );

        //@}

        //@{
        void                     setUniform( const AutoRef<Uniform> & );
        const AutoRef<Uniform> & getUniform() const { return mUniform; }
        //@}

    private:

        AutoRef<Uniform> mUniform;

        UniformResource( GpuResourceDatabase & db, GpuResourceHandle h ) : GpuResource(db,h) {}
        virtual ~UniformResource() {}

        static GpuResource * createInstance( GpuResourceDatabase &, GpuResourceHandle, const void *);
        static void          deleteInstance( GpuResource * );
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

        ///
        /// clear to an empty descriptor
        ///
        void clear()
        {
            numvtx = 0;
            numidx = 0;
        }
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
        void applyToContext( GpuContext & ) const;
        //@}

    protected:

        /// protected constructor
        MeshResource( GpuResourceDatabase & db, GpuResourceHandle h );

        /// protected destructor
        virtual ~MeshResource();

    protected:

        /// implementation class
        //@{
        class Impl;
        Impl * mImpl;
        //@}
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

        ///
        /// render state desriptor
        ///
        struct EffectRenderStateDesc
        {
            /// template for single variable that could inherite value from parent object
            template<typename T>
            struct OverridableVariable
            {
                T    value;
                bool overridden; //< if true, then this variable will override the value from parent render state object.

                /// default ctor
                OverridableVariable() : overridden(false) {}

                /// set value
                OverridableVariable & operator=( const T & rhs )
                {
                    value      = rhs;
                    overridden = true;
                    return *this;
                }

                /// set value
                OverridableVariable & operator=( const OverridableVariable<T> & rhs )
                {
                    value      = rhs.value;
                    overridden = rhs.overridden;
                    return *this;
                }
            };

            //@{

            OverridableVariable<bool>     depthTestEnabled;
            OverridableVariable<bool>     depthWriteEnabled;
            OverridableVariable<UInt8>    depthFunc;

            OverridableVariable<bool>     stencilEnabled;
            OverridableVariable<UInt8>    stencilPassOp;
            OverridableVariable<UInt8>    stencilFailOp;
            OverridableVariable<UInt8>    stencilZFailOp;

            OverridableVariable<bool>     blendingEnabled;
            OverridableVariable<Vector4f> blendFactors;

            OverridableVariable<UInt8>    fillMode;
            OverridableVariable<UInt8>    cullMode;
            OverridableVariable<UInt8>    frontFace;
            OverridableVariable<bool>     msaaEnabled;

            //@}

            //@{

            void clear()
            {
                // so all render states are inherited
                memset( this, 0, sizeof(*this) );
            }

            //@}
        };

        ///
        /// Rendering pass descriptor
        ///
        struct EffectPassDesc
        {
            StrA                  shader;       //< Name of shader used in this pass. Can't be empty
            EffectRenderStateDesc rsdesc;       //< Pass specific render states
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
            EffectRenderStateDesc     rsdesc;  //< Technique specific render states

            /// default ctor
            EffectTechniqueDesc() : quality(100) {}
        };

        std::map<StrA,EffectTextureDesc>      textures;      //< Texture list
        std::map<StrA,EffectUniformDesc>      uniforms;      //< Uniform list
        std::map<StrA,EffectRenderTargetDesc> rendertargets; //< Render taret list. Empty means using default setttings: one "color0", one "depth".
        std::map<StrA,EffectShaderDesc>       shaders;       //< Shader list
        std::map<StrA,EffectTechniqueDesc>    techniques;    //< Technique list. Technique name must be unique.
        EffectRenderStateDesc                 rsdesc;        //< Root render state descriptor for the effect.

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
            rsdesc.clear();
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

        /// ctor / dtor
        //@{
    protected:
        EffectResource( GpuResourceDatabase & db, GpuResourceHandle h );
        ~EffectResource();

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
            size_t pass;
            size_t stage;
        };

        struct ParameterProperties
        {
            StrA                       parameterName;
            DynaArray<BindingLocation> bindings;
        };

        struct TextureProperties : public ParameterProperties
        {
            SamplerDesc sampler;
        };

        struct UniformProperties : public ParameterProperties
        {
        };

        static const size_t PARAMETER_NOT_FOUND = 0xFFFFFFFF;

        size_t                    getNumPasses() const;

        size_t                    getNumTextures() const;
        size_t                    findTexture( const char * name ) const;
        const TextureProperties & getTextureProperties( size_t i ) const;

        size_t                    getNumUniforms() const;
        size_t                    findUniform( const char * name ) const;
        const UniformProperties & getUniformProperties( size_t i ) const;

        void                      applyToContext( size_t pass, GpuContext & gc ) const;

        //@}

    protected:

        /// Effect implementation class
        //@{
        class Impl;
        Impl * mImpl;
        //@}
    };

    /// define a subset of the mesh resource. (0,0,0,0) means the whole mesh.
    struct MeshResourceSubset
    {
        /// data members
        //@{

        size_t startvtx;
        size_t numvtx;
        size_t startidx;
        size_t numidx;

        //@}

        /// methods
        //@{

        void clear() { startvtx = numvtx = startidx = numidx = 0; }

        //@}
    };

    ///
    /// Model resource descriptor
    ///
    struct ModelResourceDesc
    {
        //@{

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

        //@}

        //@{

        StrA                            effectResourceName; //< effect resource name. If empty, then create a new effect using effectDesc
        EffectResourceDesc              effectResourceDesc; //< Used to create new effect, if effect resource name is empty.
        std::map<StrA,ModelTextureDesc> textures;           //< key is effect parameter name
        std::map<StrA,ModelUniformDesc> uniforms;           //< key is effect parameter name

        StrA                            meshResourceName; //< if empty, then create a new mesh using meshDesc
        MeshResourceDesc                meshResourceDesc; //< Used to create new mesh, if mesh resource name is empty
        MeshResourceSubset              subset;           //< Mesh subset information.

        //@}

        ///
        /// clear to an empty descriptor
        ///
        void clear();

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
    /// Model resource, the basic class used for rendering: myModel->render(...);
    ///
    class ModelResource : public GpuResource
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    protected:
        ModelResource( GpuResourceDatabase & db, GpuResourceHandle h );
        virtual ~ModelResource();
        //@}

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

        void              setMesh( GpuResourceHandle mesh, const MeshResourceSubset * subset = NULL );
        GpuResourceHandle getMesh( MeshResourceSubset * subset = NULL ) const;

        void              draw() const;
        //@}

        // ********************************
        // Impl
        // ********************************

    protected:

        /// Model implementation class
        //@{
        class Impl;
        Impl * mImpl;
        //@}
    };
}}

#include "gpures.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPU_GPURES_H__
