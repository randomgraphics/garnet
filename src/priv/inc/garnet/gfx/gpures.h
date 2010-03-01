#ifndef __GN_GPU_GPURES_H__
#define __GN_GPU_GPURES_H__
// *****************************************************************************
/// \file
/// \brief   GPU resource classes
/// \author  chenli@@REDMOND (2009.8.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    class GpuResourceDatabase;

    ///
    /// Gpu Resource baes class.
    ///
    class GpuResource : public RefCounter
    {
        // *****************************
        // ctor / dtor
        // *****************************

        //@{
    protected:
        GpuResource( GpuResourceDatabase & db );
        virtual ~GpuResource();
        //@}

        // *****************************
        // public methods
        // *****************************
    public:

        /// common resource properties
        //@{
        GpuResourceDatabase & database() const { return mDatabase; }
        inline const Guid   & type() const;
        inline const char   * name() const;
        //@}

        /// Cast GPU resource pointer with type check.
        //@{
        template<typename T> static inline       T * castTo( GpuResource * r );
        template<typename T> static inline       T & castTo( GpuResource & r );
        template<typename T> static inline const T & castTo( const GpuResource & r );
        //@}

        // *****************************
        // private members
        // *****************************
    private:

        friend class GpuResourceDatabase;

        GpuResourceDatabase & mDatabase;

        // this implementation class is used by GpuResourceDatabase class to track internal resource information
        class Impl;
        Impl * mImpl;
    };

    ///
    /// GPU resource factory
    ///
    struct GpuResourceFactory
    {
        /// create new resource instance
        GpuResource * (*createResource)( GpuResourceDatabase & db );
    };

    ///
    /// This class manages GPU resource creation, deletion, as well as
    /// mapping betwen resource name and resource handle.
    ///
    class GpuResourceDatabase : public NoCopy
    {
        friend class GpuResource;

        class Impl;

        Impl * mImpl;

    public:

        //@{
        GpuResourceDatabase( Gpu & );
        virtual ~GpuResourceDatabase();
        //@}

        //@{
        Gpu  & gpu() const;
        //@}

        //@{
        bool registerResourceFactory( const Guid & type, const char * descriptiveName, GpuResourceFactory factory );
        bool hasResourceFactory( const Guid & type );
        //@}

        //@{
        AutoRef<GpuResource> createResource( const Guid & type, const char * name );
        AutoRef<GpuResource> findResource( const Guid & type, const char * name ) const;
        AutoRef<GpuResource> findOrCreateResource( const Guid & type, const char * name );
        bool                 validResource( const Guid & type, const GpuResource * resource ) const; // valid resource pointer of specific type
        bool                 validResource( const GpuResource * resource ) const; // valid resource pointer of whatever type.
        const char         * getResourceName( const GpuResource * resource ) const;
        const Guid         & getResourceType( const GpuResource * resource ) const;
        //@}

        // templated utilities
        //@{
        template<class T> inline AutoRef<T> createResource( const char * name );
        template<class T> inline AutoRef<T> findResource( const char * name ) const;
        template<class T> inline AutoRef<T> findOrCreateResource( const char * name );
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

        /// load texture from file. Would return existing handle, if it is already loaded.
        static AutoRef<TextureResource> loadFromFile( GpuResourceDatabase & db, const char * filename );

        //@}

        /// events
        //@{
        Signal1<void, TextureResource&> sigTextureChanged;
        //@}

        /// Texture resource properties
        //@{
        bool                     reset( const TextureDesc * desc );
        void                     setTexture( const AutoRef<Texture> & );
        const AutoRef<Texture> & getTexture() const { return mTexture; }
        //@}

    private:

        AutoRef<Texture> mTexture;

    protected:

        //@{
        TextureResource( GpuResourceDatabase & db ) : GpuResource(db) {}
        virtual ~TextureResource() {}
        //@}
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

        //@}

        /// events
        //@{
        Signal1<void, UniformResource&> sigUniformChanged;
        //@}

        //@{
        bool                     reset( size_t length, const void * initialData );
        void                     setUniform( const AutoRef<Uniform> & );
        const AutoRef<Uniform> & getUniform() const { return mUniform; }
        //@}

    private:

        AutoRef<Uniform> mUniform;

    protected:

        //@{
        UniformResource( GpuResourceDatabase & db ) : GpuResource(db) {}
        virtual ~UniformResource() {}
        //@}
    };

    ///
    /// Mesh resource descriptor
    ///
    struct MeshResourceDesc
    {
        PrimitiveType prim;   ///< primitive type
        size_t        numvtx; ///< number of vertices
        size_t        numidx; ///< number of indices. 0 means non-indexed mesh
        bool          idx32;  ///< true for 32-bit index buffer
        bool          dynavb; ///< true for dynamic vertex buffer
        bool          dynaib; ///< trur for dynamic index buffer
        VertexFormat  vtxfmt; ///< vertex format
        void *        vertices[GpuContext::MAX_VERTEX_BUFFERS]; ///< NULL pointer means vertex data are undefined
        size_t        strides[GpuContext::MAX_VERTEX_BUFFERS];  ///< vertex buffer strides. 0 means using vertex size defined by vertex format.
        size_t        offsets[GpuContext::MAX_VERTEX_BUFFERS];
        void *        indices; ///< Null means index data are undefined.

        ///
        /// constructor
        ///
        MeshResourceDesc() { Clear(); }

        ///
        /// clear to an empty descriptor
        ///
        void Clear()
        {
            memset( this, 0, sizeof(*this) );
        }

        ///
        /// calculate bounding box
        ///
        void CalculateBoundingBox( Box<float> & ) const;

        ///
        /// calculate bounding sphere
        ///
        void CalculateBoundingSphere( Sphere<float> & ) const;

        ///
        /// get vertex buffer size in bytes
        ///
        size_t getVtxBufSize( size_t stream ) const;

        ///
        /// get indices buffer size in bytes
        ///
        size_t getIdxBufSize() const;

        ///
        /// Load descriptor from file, return the mesh data. Return a NULL blob for failure.
        ///
        AutoRef<Blob> loadFromFile( File & );

        ///
        /// Load descriptor from file
        ///
        AutoRef<Blob> loadFromFile( const char * filename );

        ///
        /// Save descriptor to file
        ///
        bool saveToFile( File & ) const;

        ///
        /// Save descriptor to file
        ///
        bool saveToFile( const char * filename ) const;
    };

    ///
    /// Mesh resource
    ///
    class MeshResource : public GpuResource
    {
    public:

        /// factory
        //@{
        static const Guid          & guid();
        static AutoRef<MeshResource> loadFromFile( GpuResourceDatabase & db, const char * filename );
        //@}

        /// events
        //@{
        Signal1<void, MeshResource&> sigMeshChanged;
        //@}

        //@{
        bool                     reset( const MeshResourceDesc * desc );
        const MeshResourceDesc & getDesc() const;
        void                     applyToContext( GpuContext & context ) const;
        void                     CalculateBoundingBox( Box<float> & ) const; // AABB: axis aligned bounding box
        void                     CalculateBoundingSphere( Sphere<float> & ) const;
        //@}

    protected:

        /// protected constructor
        MeshResource( GpuResourceDatabase & db );

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
        // *****************************
        // types
        // *****************************

        ///
        /// Shader Prerequisites
        ///
        struct ShaderPrerequisites
        {
            UInt32 numTextures;           ///< minimal number of textures required.
            UInt32 numColorRenderTargets; ///< minimal number of color render targets.

            /// default constructor
            ShaderPrerequisites()
                : numTextures(0)
                , numColorRenderTargets(0)
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
            EffectTextureDesc() { sampler.Clear(); }
        };

        ///
        /// Uniform descriptor
        ///
        struct EffectUniformDesc
        {
            size_t size; ///< uniform size in bytes

            EffectUniformDesc() : size(0) {}
        };

        ///
        /// Shader descriptor
        ///
        struct EffectGpuProgramDesc
        {
            ShaderPrerequisites prerequisites;      ///< prerequisites of the shader.
            GpuProgramDesc      gpd;                ///< GPU Program descriptor
            DynaArray<char>     shaderSourceBuffer; ///< optional buffer used to store store shader source.
            GN::Dictionary<StrA,StrA> textures;           ///< textures. Key is shader parameter name, value is user-visible texture name.
            GN::Dictionary<StrA,StrA> uniforms;           ///< uniforms. Key is shader parameter name, value is user-visible uniform name.

            /// default constructor
            EffectGpuProgramDesc() {}

            /// copy constructor
            EffectGpuProgramDesc( const EffectGpuProgramDesc & rhs );

            /// copy operator
            EffectGpuProgramDesc & operator=( const EffectGpuProgramDesc & rhs );
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
                bool overridden; ///< if true, then this variable will override the value from parent render state object.

                /// default ctor
                OverridableVariable() : overridden(false) {}

                /// set value
                template<typename T2>
                OverridableVariable & operator=( const T2 & rhs )
                {
                    value      = (T)rhs;
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

            struct RenderTargetAlphaBlend
            {
                OverridableVariable<bool>  blendEnabled;
                OverridableVariable<UInt8> blendSrc;
                OverridableVariable<UInt8> blendDst;
                OverridableVariable<UInt8> blendOp;
                OverridableVariable<UInt8> blendAlphaSrc;
                OverridableVariable<UInt8> blendAlphaDst;
                OverridableVariable<UInt8> blendAlphaOp;
            };

            //@{

            OverridableVariable<bool>          depthTestEnabled;
            OverridableVariable<bool>          depthWriteEnabled;
            OverridableVariable<UInt8>         depthFunc;

            OverridableVariable<bool>          stencilEnabled;
            OverridableVariable<UInt8>         stencilPassOp;
            OverridableVariable<UInt8>         stencilFailOp;
            OverridableVariable<UInt8>         stencilZFailOp;
            OverridableVariable<UInt8>         stencilFunc;

            OverridableVariable<UInt8>         fillMode;
            OverridableVariable<UInt8>         cullMode;
            OverridableVariable<UInt8>         frontFace;
            OverridableVariable<bool>          msaaEnabled;

            OverridableVariable<bool>          independentAlphaBlending;
            RenderTargetAlphaBlend             alphaBlend[GpuContext::MAX_COLOR_RENDER_TARGETS];
            OverridableVariable<Vector4f>      blendFactors;

            OverridableVariable<UInt32>        colorWriteMask;
            OverridableVariable<Rect<UInt32> > viewport;
            OverridableVariable<Rect<UInt32> > scissorRect;

            //@}
        };

        ///
        /// Rendering pass descriptor
        ///
        struct EffectPassDesc
        {
            StrA                  gpuprogram;   ///< Name of gpuprogram used in this pass. Can't be empty
            EffectRenderStateDesc renderstates; ///< Pass specific render states
        };

        ///
        /// Technique descriptor structure
        ///
        struct EffectTechniqueDesc
        {
            int                       quality;      ///< user defined rendering quality. Effect class uses
                                                    ///< the technique with the hightest quality as default technique.
            DynaArray<EffectPassDesc> passes;       ///< pass list.
            EffectRenderStateDesc     renderstates; ///< Technique specific render states

            /// default ctor
            EffectTechniqueDesc() : quality(100) {}
        };

        // *****************************
        // data
        // *****************************

        GN::Dictionary<StrA,EffectTextureDesc>      textures;     ///< Texture list
        GN::Dictionary<StrA,EffectUniformDesc>      uniforms;     ///< Uniform list
        GN::Dictionary<StrA,EffectGpuProgramDesc>   gpuprograms;  ///< GPU program list
        GN::Dictionary<StrA,EffectTechniqueDesc>    techniques;   ///< Technique list. Technique name must be unique.
        EffectRenderStateDesc                 renderstates; ///< Root render state descriptor for the effect.

        // *****************************
        // methods
        // *****************************

        ///
        /// constructor
        ///
        EffectResourceDesc() { Clear(); }

        ///
        /// clear to a default descriptor
        ///
        void Clear();

        ///
        /// setup the descriptor from XML
        ///
        bool loadFromXml( const XmlNode & rootNode );

        ///
        /// write the descriptor to XML
        ///
        XmlElement * saveToXml( XmlNode & rootNode ) const;
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
        EffectResource( GpuResourceDatabase & db );
        ~EffectResource();
        //@}

    public:

        /// effect factory
        //@{

        /// return GUID of the effect resource class
        static const Guid & guid();

        /// load effect from file. Would return existing handle, if it is already loaded.
        static AutoRef<EffectResource> loadFromFile( GpuResourceDatabase & db, const char * filename );

        //@

        /// public data types
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
            size_t size; ///< uniform size in bytes
        };

        static const size_t PARAMETER_NOT_FOUND = 0xFFFFFFFF;

        //@}

        /// events
        //@{
        Signal1<void, EffectResource&> sigEffectChanged;
        //@}

        /// public properties
        //@{

        bool                          reset( const EffectResourceDesc * desc );

        size_t                        getNumPasses() const;

        size_t                        getNumTextures() const;
        size_t                        findTexture( const char * name ) const;
        bool                          hasTexture( const char * name ) const { return PARAMETER_NOT_FOUND != findTexture( name ); }
        const TextureProperties     & getTextureProperties( size_t i ) const;

        size_t                        getNumUniforms() const;
        size_t                        findUniform( const char * name ) const;
        bool                          hasUniform( const char * name ) const { return PARAMETER_NOT_FOUND != findUniform( name ); }
        const UniformProperties     & getUniformProperties( size_t i ) const;

        const EffectResourceDesc::EffectRenderStateDesc &
                                      getRenderStates( size_t pass ) const;

        void                          applyToContext( size_t pass, GpuContext & gc ) const;

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

        size_t basevtx;
        size_t numvtx;
        size_t startidx;
        size_t numidx;

        //@}

        /// methods
        //@{

        bool isWholeMesh() const
        {
            return 0 == basevtx
                && 0 == numvtx
                && 0 == startidx
                && 0 == numidx;
        }

        void Clear() { basevtx = numvtx = startidx = numidx = 0; }

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
            StrA             resourceName; ///< if empty, then create a new uniform
            size_t           size;
            DynaArray<UInt8> initialValue; ///< if empty, then no initial value.
        };

        //@}

        //@{

        StrA                            effect;   ///< effect resource name.
        GN::Dictionary<StrA,ModelTextureDesc> textures; ///< key is effect parameter name
        GN::Dictionary<StrA,ModelUniformDesc> uniforms; ///< key is effect parameter name

        StrA                            mesh;     ///< Mesh resource name.
        MeshResourceSubset              subset;   ///< Mesh subset information.

        //@}

        ///
        /// constructor
        ///
        ModelResourceDesc() { Clear(); }

        ///
        /// clear to an empty descriptor
        ///
        void Clear();

        ///
        /// check if the model has a texture parameter with specific name
        ///
        bool hasTexture( const char * name ) const { return textures.end() != textures.find( name ); }

        ///
        /// check if the model has a uniform parameter with specific name
        ///
        bool hasUniform( const char * name ) const { return uniforms.end() != uniforms.find( name ); }

        ///
        /// setup the descriptor from XML
        ///
        bool loadFromXml( const XmlNode & root, const char * basedir );

        ///
        /// write the descriptor to XML
        ///
        XmlElement * saveToXml( XmlNode & root, const char * basedir ) const;

        ///
        /// write the descriptor to XML file
        ///
        bool saveToXmlFile( const char * filename ) const;
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
        // public functions
        // ********************************
    public:

        //@{
        static const Guid           & guid();
        static AutoRef<ModelResource> loadFromFile( GpuResourceDatabase & db, const char * filename );
        //@}

        //@{
        bool                     reset( const ModelResourceDesc * desc );

        AutoRef<ModelResource>   makeClone( const char * nameOfTheClone = NULL ) const;

        void                     setTextureResource( const char * effectParameterName, GpuResource * );
        AutoRef<TextureResource> getTextureResource( const char * effectParameterName ) const;

        void                     setUniformResource( const char * effectParameterName, GpuResource * );
        AutoRef<UniformResource> getUniformResource( const char * effectParameterName ) const;

        void                     setMeshResource( GpuResource * mesh, const MeshResourceSubset * subset = NULL );
        AutoRef<MeshResource>    getMeshResource( MeshResourceSubset * subset = NULL ) const;

        void                     setEffectResource( GpuResource * effect );
        AutoRef<EffectResource>  getEffectResource() const;

        void                     draw() const;
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
