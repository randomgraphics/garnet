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
        GpuResourceDatabase & GetGdb() const { return mDatabase; }
        inline const Guid   & Type() const;
        inline const char   * Name() const;
        //@}

        /// Cast GPU resource pointer with type check.
        //@{
        template<typename T> static inline       T * CastTo( GpuResource * r );
        template<typename T> static inline       T & CastTo( GpuResource & r );
        template<typename T> static inline const T & CastTo( const GpuResource & r );
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
        GpuResource * (*CreateResource)( GpuResourceDatabase & db );
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
        Gpu  & GetGpu() const;
        //@}

        //@{
        bool RegisterResourceFactory( const Guid & type, const char * descriptiveName, GpuResourceFactory factory );
        bool HasResourceFactory( const Guid & type );
        //@}

        //@{
        AutoRef<GpuResource> CreateResource( const Guid & type, const char * name );
        AutoRef<GpuResource> FindResource( const Guid & type, const char * name ) const;
        AutoRef<GpuResource> FindOrCreateResource( const Guid & type, const char * name );
        bool                 ValidResource( const Guid & type, const GpuResource * resource ) const; // valid resource pointer of specific type
        bool                 ValidResource( const GpuResource * resource ) const; // valid resource pointer of whatever type.
        const char         * GetResourceName( const GpuResource * resource ) const;
        const Guid         & GetResourceType( const GpuResource * resource ) const;
        //@}

        // templated utilities
        //@{
        template<class T> inline AutoRef<T> CreateResource( const char * name );
        template<class T> inline AutoRef<T> FindResource( const char * name ) const;
        template<class T> inline AutoRef<T> FindOrCreateResource( const char * name );
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
        static const Guid & GetGuid();

        /// load texture from file. Would return existing handle, if it is already loaded.
        static AutoRef<TextureResource> LoadFromFile( GpuResourceDatabase & db, const char * filename );

        //@}

        /// events
        //@{
        Signal1<void, TextureResource&> sigTextureChanged;
        //@}

        /// Texture resource properties
        //@{
        bool                     Reset( const TextureDesc * desc );
        void                     SetTexture( const AutoRef<Texture> & );
        const AutoRef<Texture> & GetTexture() const { return mTexture; }
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
        static const Guid & GetGuid();

        //@}

        /// events
        //@{
        Signal1<void, UniformResource&> sigUniformChanged;
        //@}

        //@{
        bool                     Reset( size_t length, const void * initialData );
        void                     SetUniform( const AutoRef<Uniform> & );
        const AutoRef<Uniform> & GetUniform() const { return mUniform; }
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
        size_t GetVtxBufSize( size_t stream ) const;

        ///
        /// get indices buffer size in bytes
        ///
        size_t GetIdxBufSize() const;

        ///
        /// Load descriptor from file, return the mesh data. Return a NULL blob for failure.
        ///
        AutoRef<Blob> LoadFromFile( File & );

        ///
        /// Load descriptor from file
        ///
        AutoRef<Blob> LoadFromFile( const char * filename );

        ///
        /// Save descriptor to file
        ///
        bool SaveToFile( File & ) const;

        ///
        /// Save descriptor to file
        ///
        bool SaveToFile( const char * filename ) const;
    };

    ///
    /// Mesh resource
    ///
    class MeshResource : public GpuResource
    {
    public:

        /// factory
        //@{
        static const Guid          & GetGuid();
        static AutoRef<MeshResource> LoadFromFile( GpuResourceDatabase & db, const char * filename );
        //@}

        /// events
        //@{
        Signal1<void, MeshResource&> sigMeshChanged;
        //@}

        //@{
        bool                     Reset( const MeshResourceDesc * desc );
        const MeshResourceDesc & GetDesc() const;
        void                     ApplyToContext( GpuContext & context ) const;
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
            ShaderPrerequisites  prerequisites;      ///< prerequisites of the shader.
            GpuProgramDesc       gpd;                ///< GPU Program descriptor
            DynaArray<char>      shaderSourceBuffer; ///< optional buffer used to store store shader source.
            StringMap<char,StrA> textures;           ///< textures. Key is shader parameter name, value is user-visible texture name.
            StringMap<char,StrA> uniforms;           ///< uniforms. Key is shader parameter name, value is user-visible uniform name.

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

        StringMap<char,EffectTextureDesc>     textures;     ///< Texture list
        StringMap<char,EffectUniformDesc>     uniforms;     ///< Uniform list
        StringMap<char,EffectGpuProgramDesc>  gpuprograms;  ///< GPU program list
        StringMap<char,EffectTechniqueDesc>   techniques;   ///< Technique list. Technique name must be unique.
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
        bool LoadFromXml( const XmlNode & rootNode );

        ///
        /// write the descriptor to XML
        ///
        XmlElement * SaveToXml( XmlNode & rootNode ) const;
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
        static const Guid & GetGuid();

        /// load effect from file. Would return existing handle, if it is already loaded.
        static AutoRef<EffectResource> LoadFromFile( GpuResourceDatabase & db, const char * filename );

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

        bool                          Reset( const EffectResourceDesc * desc );

        size_t                        GetNumPasses() const;

        size_t                        GetNumTextures() const;
        size_t                        FindTexture( const char * name ) const;
        bool                          HasTexture( const char * name ) const { return PARAMETER_NOT_FOUND != FindTexture( name ); }
        const TextureProperties     & GetTextureProperties( size_t i ) const;

        size_t                        GetNumUniforms() const;
        size_t                        FindUniform( const char * name ) const;
        bool                          HasUniform( const char * name ) const { return PARAMETER_NOT_FOUND != FindUniform( name ); }
        const UniformProperties     & GetUniformProperties( size_t i ) const;

        const EffectResourceDesc::EffectRenderStateDesc &
                                      GetRenderStates( size_t pass ) const;

        void                          ApplyToContext( size_t pass, GpuContext & gc ) const;

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

        bool IsWholeMesh() const
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

        StrA                             effect;   ///< effect resource name.
        StringMap<char,ModelTextureDesc> textures; ///< key is effect parameter name
        StringMap<char,ModelUniformDesc> uniforms; ///< key is effect parameter name

        StrA                             mesh;     ///< Mesh resource name.
        MeshResourceSubset               subset;   ///< Mesh subset information.

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
        bool HasTexture( const char * name ) const { return NULL != textures.Find( name ); }

        ///
        /// check if the model has a uniform parameter with specific name
        ///
        bool HasUniform( const char * name ) const { return NULL != uniforms.Find( name ); }

        ///
        /// setup the descriptor from XML
        ///
        bool LoadFromXml( const XmlNode & root, const char * basedir );

        ///
        /// write the descriptor to XML
        ///
        XmlElement * SaveToXml( XmlNode & root, const char * basedir ) const;

        ///
        /// write the descriptor to XML file
        ///
        bool SaveToXmlFile( const char * filename ) const;
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
        static const Guid           & GetGuid();
        static AutoRef<ModelResource> LoadFromFile( GpuResourceDatabase & db, const char * filename );
        //@}

        //@{
        bool                     Reset( const ModelResourceDesc * desc );

        AutoRef<ModelResource>   MakeClone( const char * nameOfTheClone = NULL ) const;

        void                     SetTextureResource( const char * effectParameterName, GpuResource * );
        AutoRef<TextureResource> GetTextureResource( const char * effectParameterName ) const;

        void                     SetUniformResource( const char * effectParameterName, GpuResource * );
        AutoRef<UniformResource> GetUniformResource( const char * effectParameterName ) const;

        void                     SetMeshResource( GpuResource * mesh, const MeshResourceSubset * subset = NULL );
        AutoRef<MeshResource>    GetMeshResource( MeshResourceSubset * subset = NULL ) const;

        void                     SetEffectResource( GpuResource * effect );
        AutoRef<EffectResource>  GetEffectResource() const;

        void                     Draw() const;
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
