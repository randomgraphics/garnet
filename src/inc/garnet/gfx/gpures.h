#ifndef __GN_GPU_GPURES_H__
#define __GN_GPU_GPURES_H__
// *****************************************************************************
/// \file
/// \brief   GPU resource classes
/// \author  chenli@@REDMOND (2009.8.13)
// *****************************************************************************

namespace GN::gfx {

class GpuResourceDatabase;

///
/// Gpu Resource baes class.
///
class GpuResource : public RefCounter {
    // *****************************
    // ctor / dtor
    // *****************************

    //@{
protected:
    GpuResource(GpuResourceDatabase & db);
    virtual ~GpuResource();
    //@}

    // *****************************
    // public methods
    // *****************************
public:
    /// common resource properties
    //@{
    GpuResourceDatabase & getGdb() const { return mDatabase; }
    inline const Guid &   type() const;
    inline const StrA &   name() const;
    //@}

    /// Cast GPU resource pointer with type check.
    //@{
    template<typename T>
    static inline T * castTo(GpuResource * r);
    template<typename T>
    static inline T & castTo(GpuResource & r);
    template<typename T>
    static inline const T & castTo(const GpuResource & r);
    //@}

    // *****************************
    // private members
    // *****************************
private:
    friend class GpuResourceDatabase;

    GpuResourceDatabase & mDatabase;

    // this implementation class is used by GpuResourceDatabase class to track
    // internal resource information
    class Impl;
    Impl * mImpl;
};

///
/// Texture resource
///
class GN_API TextureResource : public GpuResource {
public:
    /// Texture resource factory
    //@{

    /// return GUID of the texture resource class
    static const Guid & guid();

    /// load texture from file. Would return existing handle, if it is already
    /// loaded.
    static AutoRef<TextureResource> loadFromFile(GpuResourceDatabase & db, const char * filename);

    //@}

    /// events
    //@{
    Signal1<void, TextureResource &> sigTextureChanged;
    //@}

    /// Texture resource properties
    //@{
    bool                     reset(const TextureDesc * desc);
    void                     setTexture(const AutoRef<Texture> &);
    const AutoRef<Texture> & texture() const { return mTexture; }
    //@}

private:
    AutoRef<Texture> mTexture;

protected:
    //@{
    TextureResource(GpuResourceDatabase & db): GpuResource(db) {}
    virtual ~TextureResource() {}
    //@}
};

///
/// Uniform resource
///
class UniformResource : public GpuResource {
public:
    /// Uniform resource factory
    //@{

    /// return GUID of the uniform resource class
    static const Guid & guid();

    //@}

    /// events
    //@{
    Signal1<void, UniformResource &> sigUniformChanged;
    //@}

    //@{
    bool                     reset(uint32 length, const void * initialData);
    void                     setUniform(const AutoRef<Uniform> &);
    const AutoRef<Uniform> & uniform() const { return mUniform; }
    //@}

private:
    AutoRef<Uniform> mUniform;

protected:
    //@{
    UniformResource(GpuResourceDatabase & db): GpuResource(db) {}
    virtual ~UniformResource() {}
    //@}
};

///
/// definition of single mesh vertex element
///
struct MeshVertexElement {
    PixelFormat format       = PixelFormat::UNKNOWN(); ///< the vertex element format.
    uint8       stream       = 0;                      ///< vertex buffer index
    uint8       offset       = 0;                      ///< offset of the element in the vertex.
    char        semantic[16] = {};                     ///< Semantic name (null terminated string, 15 characters at most).

    /// Set vertex element semantic.
    void setSemantic(const char * s) {
        size_t len = str::length(s);
        if (0 == len) {
            GN_ERROR(getLogger("GN.gfx.gpu"))
            ("Empty semantic string is not allowed.");
            return;
        }

        if (len >= GN_ARRAY_COUNT(semantic)) {
            GN_ERROR(getLogger("GN.gfx.gpu"))
            ("Semantic string (%s) is too long. Maxinum length is 16 characters "
             "including ending NULL.",
             s);
        }
        len = math::getmin<size_t>(GN_ARRAY_COUNT(semantic), len + 1);
        memcpy(semantic, s, len);
    }

    /// equality check
    bool operator==(const MeshVertexElement & rhs) const {
        return format == rhs.format && stream == rhs.stream && offset == rhs.offset && 0 == str::compare(semantic, rhs.semantic, sizeof(semantic));
    }

    /// equality check
    bool operator!=(const MeshVertexElement & rhs) const { return !operator==(rhs); }
};

///
/// define mesh vertex format
///
struct MeshVertexFormat {
    enum {
        MAX_VERTEX_ELEMENTS = 16,
    };

    uint32            numElements                   = 0;  ///< number of elements
    MeshVertexElement elements[MAX_VERTEX_ELEMENTS] = {}; ///< vertex element array

    bool operator==(const MeshVertexFormat & rhs) const {
        if (numElements != rhs.numElements) return false;
        for (uint32 i = 0; i < numElements; ++i) {
            if (elements[i] != rhs.elements[i]) return false;
        }
        return true;
    }

    bool operator!=(const MeshVertexFormat & rhs) const { return !operator==(rhs); }

    bool operator<(const MeshVertexFormat & rhs) const {
        if (this == &rhs) return false;

        const uint32 * a = (const uint32 *) this;
        const uint32 * b = (const uint32 *) &rhs;
        uint32         n = sizeof(*this) / 4;

        for (uint32 i = 0; i < n; ++i) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }

        return false;
    }

    ///
    /// clear the vertex format
    ///
    void clear() { numElements = 0; }

    ///
    /// Calculate number of streams.
    ///
    uint32 inline calcNumStreams() const {
        uint32 n = 0;
        for (uint32 i = 0; i < numElements; ++i) {
            const MeshVertexElement & e = elements[i];
            if (e.stream >= n) n = e.stream + 1;
        }
        return n;
    }

    ///
    /// Calculate stride of specific stream.
    ///
    uint16 inline calcStreamStride(uint32 stream) const {
        uint16 stride = 0;
        for (uint32 i = 0; i < numElements; ++i) {
            const MeshVertexElement & e = elements[i];

            uint16 elementEnd = e.offset + e.format.bytesPerBlock();

            if (stream == e.stream && stride < elementEnd) stride = elementEnd;
        }
        return stride;
    }

    ///
    /// Check if the vertex format has specific semantic.
    ///
    bool hasSemantic(const char * semantic, uint32 * elementIndex = NULL) const {
        for (uint32 i = 0; i < numElements; ++i) {
            if (0 == str::compareI(elements[i].semantic, semantic)) {
                if (NULL != elementIndex) *elementIndex = i;
                return true;
            }
        }
        return false;
    }

    ///
    /// return a vertex format definition for vertex like this:
    ///
    /// struct MeshVertexFormat
    /// {
    ///     float position[3];
    /// };
    ///
    static MeshVertexFormat XYZ() {
        MeshVertexFormat vf;

        vf.numElements = 1;

        vf.elements[0].setSemantic("POSITION");
        vf.elements[0].format = PixelFormat::FLOAT3;
        vf.elements[0].stream = 0;
        vf.elements[0].offset = 0;

        return vf;
    }

    ///
    /// return a vertex format definition for vertex like this:
    ///
    /// struct MeshVertexFormat
    /// {
    ///     float position[2];
    ///     float texcoord[2];
    /// };
    ///
    static MeshVertexFormat XY_UV() {
        MeshVertexFormat vf;

        vf.numElements = 2;

        vf.elements[0].setSemantic("POSITION");
        vf.elements[0].format = PixelFormat::FLOAT2;
        vf.elements[0].stream = 0;
        vf.elements[0].offset = 0;

        vf.elements[1].setSemantic("TEXCOORD");
        vf.elements[1].format = PixelFormat::FLOAT2;
        vf.elements[1].stream = 0;
        vf.elements[1].offset = 8;

        return vf;
    }

    ///
    /// return a vertex format definition for vertex like this:
    ///
    /// struct MeshVertexFormat
    /// {
    ///     float position[3];
    ///     float normal[3];
    ///     float texcoord[2];
    /// };
    ///
    static MeshVertexFormat XYZ_NORM_UV() {
        MeshVertexFormat vf;

        vf.numElements = 3;

        vf.elements[0].setSemantic("POSITION");
        vf.elements[0].format = PixelFormat::FLOAT3;
        vf.elements[0].stream = 0;
        vf.elements[0].offset = 0;

        vf.elements[1].setSemantic("NORMAL");
        vf.elements[1].format = PixelFormat::FLOAT3;
        vf.elements[1].stream = 0;
        vf.elements[1].offset = 12;

        vf.elements[2].setSemantic("TEXCOORD");
        vf.elements[2].format = PixelFormat::FLOAT2;
        vf.elements[2].stream = 0;
        vf.elements[2].offset = 24;

        return vf;
    }
};

///
/// Mesh resource descriptor base (no data pointers)
///
struct MeshResourceDescBase {
    PrimitiveType    prim   = PrimitiveType::POINT_LIST;      ///< primitive type
    uint32           numvtx = 0;                              ///< number of vertices
    uint32           numidx = 0;                              ///< number of indices. 0 means non-indexed mesh
    bool             idx32  = false;                          ///< true for 32-bit index buffer
    bool             dynavb = false;                          ///< true for dynamic vertex buffer
    bool             dynaib = false;                          ///< trur for dynamic index buffer
    MeshVertexFormat vtxfmt;                                  ///< vertex format
    uint16           strides[GpuContext::MAX_VERTEX_BUFFERS]; ///< vertex buffer strides. 0
                                                              ///< means using vertex size
                                                              ///< defined by vertex format.
    uint32 offsets[GpuContext::MAX_VERTEX_BUFFERS];           ///< Number of bytes from
                                                              ///< vertex buffer beginning
                                                              ///< to the first element that
                                                              ///< will be used.

    ///
    /// constructor
    ///
    MeshResourceDescBase() {}

    ///
    /// get vertex buffer size in bytes
    ///
    uint32 getVtxBufSize(uint32 stream) const;

    ///
    /// get indices buffer size in bytes
    ///
    uint32 getIdxBufSize() const;
};

///
/// Mesh resource descriptor with vertex and index data pointers
///
struct MeshResourceDesc : public MeshResourceDescBase {
    void * vertices[GpuContext::MAX_VERTEX_BUFFERS] = {}; ///< NULL pointer means vertex
                                                          ///< data are undefined
    void * indices = nullptr;                             ///< Null means that the mesh should be drawed as
                                                          ///< non-indexed mesh.

    ///
    /// constructor
    ///
    MeshResourceDesc() {}

    ///
    /// calculate bounding box
    ///
    void calculateBoundingBox(Box<float> &) const;

    ///
    /// calculate bounding sphere
    ///
    void calculateBoundingSphere(Sphere<float> &) const;

    ///
    /// Load descriptor from file, return the mesh data. Return a NULL blob for
    /// failure.
    ///
    AutoRef<Blob> loadFromFile(File &);

    ///
    /// Load descriptor from file
    ///
    AutoRef<Blob> loadFromFile(const char * filename);

    ///
    /// Save descriptor to file
    ///
    bool saveToFile(File &) const;

    ///
    /// Save descriptor to file
    ///
    bool saveToFile(const char * filename) const;
};

///
/// Mesh resource
///
class GN_API MeshResource : public GpuResource {
public:
    /// factory
    //@{
    static const Guid &          guid();
    static AutoRef<MeshResource> loadFromFile(GpuResourceDatabase & db, const char * filename);
    //@}

    /// events
    //@{
    Signal1<void, MeshResource &> sigMeshChanged;
    //@}

    //@{
    bool                         reset(const MeshResourceDesc * desc);
    const MeshResourceDescBase & getDesc() const;
    void                         applyToContext(GpuContext & context) const;
    void                         calculateBoundingBox(Box<float> &) const; // AABB: axis aligned bounding box
    void                         calculateBoundingSphere(Sphere<float> &) const;
    //@}

protected:
    /// protected constructor
    MeshResource(GpuResourceDatabase & db);

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
struct GN_API EffectResourceDesc {
    // *****************************
    // types
    // *****************************

    ///
    /// Shader Prerequisites
    ///
    struct ShaderPrerequisites {
        uint32 numTextures;           ///< minimal number of textures required.
        uint32 numColorRenderTargets; ///< minimal number of color render targets.

        /// default constructor
        ShaderPrerequisites(): numTextures(0), numColorRenderTargets(0) {}
    };

    ///
    /// Texture descriptor
    ///
    struct EffectTextureDesc {
        /// sampler descriptor
        SamplerDesc sampler;

        /// default ctor
        EffectTextureDesc() { sampler.clear(); }
    };

    ///
    /// Uniform descriptor
    ///
    struct EffectUniformDesc {
        uint32 size; ///< uniform size in bytes

        EffectUniformDesc(): size(0) {}
    };

    ///
    /// Attribute descritor
    ///
    struct EffectAttributeDesc {};

    ///
    /// Shader descriptor
    ///
    struct GN_API EffectGpuProgramDesc {
        // TODO: add a name field
        ShaderPrerequisites prerequisites;      ///< prerequisites of the shader.
        GpuProgramDesc      gpd;                ///< GPU Program descriptor
        DynaArray<char>     shaderSourceBuffer; ///< optional buffer used to store store
                                                ///< shader source.
        StringMap<char, StrA> textures;         ///< textures. Key is shader parameter name, value is name of
                                                ///< one texture in EffectResourceDesc.textures.
        StringMap<char, StrA> uniforms;         ///< uniforms. Key is shader parameter name, value is name of
                                                ///< one uniform in EffectResourceDesc.textures.
        StringMap<char, StrA> attributes;       ///< attributes. Key is shader parameter name, value is name
                                                ///< of one attribute in EffectResourceDesc.attributes.

        /// default constructor
        EffectGpuProgramDesc() {}

        /// copy constructor
        EffectGpuProgramDesc(const EffectGpuProgramDesc & rhs);

        /// copy operator
        EffectGpuProgramDesc & operator=(const EffectGpuProgramDesc & rhs);
    };

    ///
    /// render state desriptor
    ///
    struct EffectRenderStateDesc {
        /// template for single variable that could inherite value from parent
        /// object
        template<typename T>
        struct OverridableVariable {
            T    value;
            bool overridden; ///< if true, then this variable will override the value
                             ///< from parent render state object.

            /// default ctor
            OverridableVariable(): overridden(false) {}

            /// copy ctor
            OverridableVariable(const OverridableVariable & rhs) {
                value      = rhs.value;
                overridden = rhs.overridden;
            }

            /// set value
            template<typename T2>
            OverridableVariable & operator=(const T2 & rhs) {
                value      = (T) rhs;
                overridden = true;
                return *this;
            }

            /// set value
            OverridableVariable & operator=(const OverridableVariable<T> & rhs) {
                value      = rhs.value;
                overridden = rhs.overridden;
                return *this;
            }
        };

        struct RenderTargetAlphaBlend {
            OverridableVariable<bool>  blendEnabled;
            OverridableVariable<uint8> blendSrc;
            OverridableVariable<uint8> blendDst;
            OverridableVariable<uint8> blendOp;
            OverridableVariable<uint8> blendAlphaSrc;
            OverridableVariable<uint8> blendAlphaDst;
            OverridableVariable<uint8> blendAlphaOp;
        };

        //@{

        OverridableVariable<bool>  depthTestEnabled;
        OverridableVariable<bool>  depthWriteEnabled;
        OverridableVariable<uint8> depthFunc;

        OverridableVariable<bool>  stencilEnabled;
        OverridableVariable<uint8> stencilPassOp;
        OverridableVariable<uint8> stencilFailOp;
        OverridableVariable<uint8> stencilZFailOp;
        OverridableVariable<uint8> stencilFunc;

        OverridableVariable<uint8> fillMode;
        OverridableVariable<uint8> cullMode;
        OverridableVariable<uint8> frontFace;
        OverridableVariable<bool>  msaaEnabled;

        OverridableVariable<bool>     independentAlphaBlending;
        RenderTargetAlphaBlend        alphaBlend[GpuContext::MAX_COLOR_RENDER_TARGETS];
        OverridableVariable<Vector4f> blendFactors;

        OverridableVariable<uint32>       colorWriteMask;
        OverridableVariable<Rect<uint32>> viewport;
        OverridableVariable<Rect<uint32>> scissorRect;

        //@}
    };

    ///
    /// Rendering pass descriptor
    ///
    struct EffectPassDesc {
        StrA                  gpuprogram;   ///< Name of gpuprogram used in this pass. Can't be empty
        EffectRenderStateDesc renderstates; ///< Pass specific render states
    };

    ///
    /// Technique descriptor structure
    ///
    struct EffectTechniqueDesc {
        StrA name;                              ///< Optional technique name.
        int  quality;                           ///< user defined rendering quality. Effect class uses
                                                ///< the technique with the hightest quality as default
                                                ///< technique.
        DynaArray<EffectPassDesc> passes;       ///< pass list.
        EffectRenderStateDesc     renderstates; ///< Technique specific render states

        /// default ctor
        EffectTechniqueDesc(): quality(100) {}
    };

    // *****************************
    // data
    // *****************************

    StringMap<char, EffectTextureDesc>    textures;     ///< Texture list
    StringMap<char, EffectUniformDesc>    uniforms;     ///< Uniform list
    StringMap<char, EffectAttributeDesc>  attributes;   ///< attribute list
    StringMap<char, EffectGpuProgramDesc> gpuprograms;  ///< GPU program list
    DynaArray<EffectTechniqueDesc>        techniques;   ///< Technique list.
    EffectRenderStateDesc                 renderstates; ///< Root render state descriptor for the effect.

    // *****************************
    // methods
    // *****************************

    ///
    /// constructor
    ///
    EffectResourceDesc() { clear(); }

    ///
    /// clear to a default descriptor
    ///
    void clear();

    ///
    /// setup the descriptor from XML
    ///
    bool loadFromXml(const XmlNode & rootNode);

    ///
    /// write the descriptor to XML
    ///
    XmlElement * saveToXml(XmlNode & rootNode) const;
};

///
/// Effect defines GPU program and render states, and how textures,
/// uniforms and render targets are binded to GPU context.
///
class GN_API EffectResource : public GpuResource {

    /// ctor / dtor
    //@{
protected:
    EffectResource(GpuResourceDatabase & db);
    ~EffectResource();
    //@}

public:
    /// effect factory
    //@{

    /// return GUID of the effect resource class
    static const Guid & guid();

    /// load effect from file. Would return existing handle, if it is already
    /// loaded.
    static AutoRef<EffectResource> loadFromFile(GpuResourceDatabase & db, const char * filename);

    //@

    /// public data types
    //@{

    struct BindingLocation {
        uint32 pass;                     // index of the pass
        uint32 gpuProgramParameterIndex; // index of the GPU program parameter
    };

    struct EffectParameterProperties {
        StrA                       parameterName;
        DynaArray<BindingLocation> bindings;
    };

    struct TextureProperties : public EffectParameterProperties {
        SamplerDesc sampler;
    };

    struct UniformProperties : public EffectParameterProperties {
        uint32 size; ///< uniform size in bytes
    };

    struct AttributeProperties : public EffectParameterProperties {};

    static const uint32 PARAMETER_NOT_FOUND = 0xFFFFFFFF;

    //@}

    /// events
    //@{
    Signal1<void, EffectResource &> sigEffectChanged;
    //@}

    /// public properties
    //@{

    bool reset(const EffectResourceDesc * desc);

    uint32 numPasses() const;

    uint32                    numTextures() const;
    uint32                    findTexture(const char * name) const;
    bool                      hasTexture(const char * name) const { return PARAMETER_NOT_FOUND != findTexture(name); }
    const TextureProperties & textureProperties(uint32 i) const;

    uint32                    numUniforms() const;
    uint32                    findUniform(const char * name) const;
    bool                      hasUniform(const char * name) const { return PARAMETER_NOT_FOUND != findUniform(name); }
    const UniformProperties & uniformProperties(uint32 i) const;

    uint32                      numAttributes() const;
    uint32                      findAttribute(const char * name) const;
    bool                        hasAttribute(const char * name) const { return PARAMETER_NOT_FOUND != findAttribute(name); }
    const AttributeProperties & attributeProperties(uint32 i) const;

    const EffectResourceDesc::EffectRenderStateDesc & renderStates(uint32 pass) const;

    void applyToContext(uint32 pass, GpuContext & gc) const;

    //@}

protected:
    /// Effect implementation class
    //@{
    class Impl;
    Impl * mImpl;
    //@}
};

/// define a subset of the mesh resource. (0,0,0,0) means the whole mesh.
struct MeshResourceSubset {
    /// data members
    //@{

    uint32 basevtx;
    uint32 numvtx;
    uint32 startidx;
    uint32 numidx;

    //@}

    /// methods
    //@{

    bool isWholeMesh() const { return 0 == basevtx && 0 == numvtx && 0 == startidx && 0 == numidx; }

    void clear() { basevtx = numvtx = startidx = numidx = 0; }

    //@}
};

///
/// Model resource descriptor
///
struct GN_API ModelResourceDesc {
    //@{

    struct ModelTextureDesc {
        StrA resourceName; /// if empty, then create a new texture using the
                           /// descriptor
        TextureDesc desc;
    };

    struct ModelUniformDesc {
        StrA             resourceName; ///< if empty, then create a new uniform
        uint32           size;
        DynaArray<uint8> initialValue; ///< if empty, then no initial value.
    };

    //@}

    //@{

    StrA                              effect;   ///< effect resource name.
    StringMap<char, ModelTextureDesc> textures; ///< key is effect parameter name
    StringMap<char, ModelUniformDesc> uniforms; ///< key is effect parameter name

    StrA               mesh;   ///< Mesh resource name.
    MeshResourceSubset subset; ///< Mesh subset information.

    //@}

    ///
    /// constructor
    ///
    ModelResourceDesc() { clear(); }

    ///
    /// clear to an empty descriptor
    ///
    void clear();

    ///
    /// check if the model has a texture parameter that is mapped to specific
    /// effect parameter
    ///
    bool hasTexture(const char * effectParameterName) const { return NULL != textures.find(effectParameterName); }

    ///
    /// check if the model has an uniform parameter that is mapped to specific
    /// effect parameter
    ///
    bool hasUniform(const char * effectParameterName) const { return NULL != uniforms.find(effectParameterName); }

    ///
    /// setup the descriptor from XML
    ///
    bool loadFromXml(const XmlNode & root, const char * basedir);

    ///
    /// write the descriptor to XML
    ///
    XmlElement * saveToXml(XmlNode & root, const char * basedir) const;

    ///
    /// write the descriptor to XML file
    ///
    bool saveToXmlFile(const char * filename) const;
};

///
/// Model resource, the basic class used for rendering: myModel->render(...);
///
class GN_API ModelResource : public GpuResource {
    // ********************************
    // ctor/dtor
    // ********************************

    //@{
protected:
    ModelResource(GpuResourceDatabase & db);
    virtual ~ModelResource();
    //@}

    // ********************************
    // public functions
    // ********************************
public:
    //@{
    static const Guid &           guid();
    static AutoRef<ModelResource> loadFromFile(GpuResourceDatabase & db, const char * filename);
    //@}

    //@{
    bool reset(const ModelResourceDesc * desc);

    AutoRef<ModelResource> makeClone(const char * nameOfTheClone = NULL) const;

    void                     setTextureResource(const char * effectParameterName, GpuResource *);
    AutoRef<TextureResource> textureResource(const char * effectParameterName) const;

    void                     setUniformResource(const char * effectParameterName, GpuResource *);
    AutoRef<UniformResource> uniformResource(const char * effectParameterName) const;

    void                  setMeshResource(GpuResource * mesh, const MeshResourceSubset * subset = NULL);
    AutoRef<MeshResource> meshResource(MeshResourceSubset * subset = NULL) const;

    void                    setEffectResource(GpuResource * effect);
    AutoRef<EffectResource> effectResource() const;

    void draw() const;
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

///
/// GPU resource factory
///
struct GpuResourceFactory {
    /// create new resource instance
    GpuResource * (*createResource)(GpuResourceDatabase & db);
};

///
/// This class manages GPU resource creation, deletion, as well as
/// mapping betwen resource name and resource instance.
///
/// Note that resource name is optional
///
class GN_API GpuResourceDatabase : public NoCopy {
    friend class GpuResource;

    class Impl;

    Impl * mImpl;

public:
    //@{
    GpuResourceDatabase(Gpu &);
    virtual ~GpuResourceDatabase();
    //@}

    //@{
    Gpu & getGpu() const;
    //@}

    //@{
    bool registerResourceFactory(const Guid & type, const char * descriptiveName, GpuResourceFactory factory);
    bool hasResourceFactory(const Guid & type);
    //@}

    //@{
    AutoRef<GpuResource> createResource(const Guid & type, const char * name);
    AutoRef<GpuResource> findResource(const Guid & type, const char * name) const;
    AutoRef<GpuResource> findOrCreateResource(const Guid & type, const char * name, bool * isExistingResource = NULL);
    bool                 validResource(const Guid & type, const GpuResource * resource) const; // valid resource pointer of specific type
    bool                 validResource(const GpuResource * resource) const;                    // valid resource pointer of whatever type.
    const StrA *         getResourceName(const GpuResource * resource) const;
    const Guid *         getResourceType(const GpuResource * resource) const;
    //@}

    // templated utilities
    //@{
    template<class T>
    inline AutoRef<T> createResource(const char * name);
    template<class T>
    inline AutoRef<T> findResource(const char * name) const;
    template<class T>
    inline AutoRef<T> findOrCreateResource(const char * name, bool * isExistingResource = NULL);
    //@}

    /// standard uniform utilities
    //@{

    AutoRef<UniformResource> getStandardUniformResource(int index) const;

    void setStandardUniform(int index, const void * data, uint32 dataSize);

    template<typename T>
    void setStandardUniform(int index, const T & value) {
        setStandardUniform(index, &value, sizeof(T));
    }

    void setTransform(const Matrix44f & proj, const Matrix44f & view);

    void setLight0(const Vector4f & diffuse, const Vector4f & ambient, const Vector4f & specular, const Vector3f & position, const Vector3f & direction);

    //@}

private:
};

} // namespace GN::gfx

#include "gpures.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPU_GPURES_H__
