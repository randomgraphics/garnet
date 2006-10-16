#ifndef __GN_GFX_NES_H__
#define __GN_GFX_NES_H__
// *****************************************************************************
//! \file    gfx/nes.h
//! \brief   New effect system interface (experimental)
//! \author  chenlee (2006.1.7)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! Namespace for new effect system
    //!
    namespace nes {}
}}

namespace GN { namespace gfx { namespace nes
{
    //! \name resource ID types.
    //! \note Zeor is always invalid ID.
    //@{
    typedef uint16_t BufferId;
    typedef uint16_t EffectId;
    //@}

    //!
    //! effect constant type
    //!
    enum ConstType
    {
        CT_BOOL,            //!< bool constant
        CT_INT,             //!< 32-bit integer constant
        CT_FLOAT,           //!< 32-bit floating point constant
        CT_VEC4,            //!< 4D float vector
        CT_MAT44,           //!< 4x4 float matrix
        CT_STRING,          //!< string
        CT_RAW,             //!< raw bytes
        NUM_CONSTANT_TYPES, //!< number of constant types
    };

    //!
    //! effect constant descriptor
    //!
    struct ConstDesc
    {
        StrA      name;  //!< constant name (optional)
        ConstType type;  //!< type of values in constant.
        uint16_t  count; //!< number of values in constant.
    };

    //!
    //! constant value array, that can hold either self-hosted or external data pointer.
    //!
    template<class T>
    class ConstValueArray
    {
        DynaArray<T> mArray;
        T *          mPointer;
        bool         mSelfOwned;
    public:
        //@{
        ConstValueArray( T * p = NULL ) : mPointer(p), mSelfOwned(false) {}
        void setp( T * p ) { mArray.clear(); mPointer = p; mSelfOwned = false; }
        void resize( size_t n ) { mArray.resize( n ); mPointer = mArray.cptr(); mSelfOwned = true; }
        T * cptr() { return mPointer; }
        const T * cptr() const { return mPointer; }
        //@}
    };

    //!
    //! Const data structure
    //!
    struct ConstData
    {
        ConstDesc                  desc; //!< constant descriptor

        //! \name value array for various types. Ignored if constant type <b>is</b> CT_REFLECTION
        //@{
        ConstValueArray<bool>      bools;
        ConstValueArray<int32_t>   ints;
        ConstValueArray<float>     floats;
        ConstValueArray<Vector4f>  vectors;
        ConstValueArray<Matrix44f> matrices;
        ConstValueArray<StrA>      strings;
        ConstValueArray<uint8_t>   raw;
        //@}
    };

    //!
    //! Buffer usage flags
    //!
    enum BufferUsage
    {
        BU_CPU_IMMUTABLE, //!< Input buffer. Once the stream is created, it won't change any more.
        BU_CPU_DEFAULT,   //!< Input buffer. Like D3DUSAGE_STATIC.
        BU_CPU_DYNAMIC,   //!< Input buffer. Like D3DUSAGE_DYNAMIC.
        BU_GPU,           //!< output buffer. Use to return GPU generated data back to CPU.
        NUM_STREAM_USAGES //!< number of buffer usage flags.
    };

    //!
    //! Buffer type
    //!
    enum BufferType
    {
        BT_TEXTURE,      //!< texture buffer
        BT_VERTEX,       //!< vertex buffer
        BT_INDEX,        //!< index buffer
        NUM_STREAM_TYPES //!< number of buffer types.
    };

    //! \name buffer descriptor for specific resource 
    //@{
    typedef ImageDesc  TextureBufferDesc; //!< texture stream descriptor
    typedef VtxFmtDesc VertexBufferDesc; //!< vertex stream descriptor
    //!
    //! index buffer descriptor
    //!
    struct IndexBufferDesc
    {
        PrimitiveType prim;   //!< primitive type
        size_t        numIdx; //!< number of indices
        bool          idx32;  //!< is 32-bit index buffer?
    };
    //@}

    //! \name stream interfaces
    //@{

    //!
    //! basic buffer class
    //!
    class Buffer : public NoCopy
    {
        BufferType  mType;
        BufferUsage mUsage;

    protected:

        //!
        //! ctor
        //!
        Buffer( BufferType t, BufferUsage u ) : mType( t ), mUsage(u)
        {
            GN_ASSERT( 0 <= t && t <= NUM_STREAM_TYPES );
            GN_ASSERT( 0 <= u && u <= NUM_STREAM_USAGES );
        }

    public:

        //!
        //! get stream type
        //!
        BufferType getType() const { return mType; }

        //!
        //! get stream usage
        //!
        BufferUsage getUsage() const { return mUsage; }
    };

    //!
    //! texture buffer class
    //!
    class TextureBuffer : public Buffer
    {
    protected :

        //!
        //! ctor
        //!
        TextureBuffer( BufferUsage u ) : Buffer( BT_TEXTURE, u ) {}

    public :

        //!
        //! get descriptor
        //!
        virtual const TextureBufferDesc & getDesc() const = 0;

        //!
        //! copy buffer content to somewhere
        //!
        virtual bool copyTo(
            void * dst,
            size_t face,
            size_t level,
            size_t rowPitch,
            size_t slicePitch ) const = 0;
    };

    //!
    //! vertex buffer class
    //!
    class VertexBuffer : public Buffer
    {
    protected :

        //!
        //! ctor
        //!
        VertexBuffer( BufferUsage u ) : Buffer( BT_VERTEX, u ) {}

    public :

        //!
        //! get descriptor
        //!
        virtual const VertexBufferDesc & getDesc() const = 0;

        //!
        //! copy buffer content to somewhere
        //!
        virtual bool copyTo( void * dst, size_t stream ) const = 0;
    };

    //!
    //! index buffer class
    //!
    class IndexBuffer : public Buffer
    {
    protected :

        //!
        //! ctor
        //!
        IndexBuffer( BufferUsage u ) : Buffer( BT_INDEX, u ) {}

    public :

        //!
        //! get descriptor
        //!
        virtual const IndexBufferDesc & getDesc() const = 0;

        //!
        //! copy buffer content to somewhere
        //!
        virtual bool copyTo( void * dst ) const = 0;
    };
    //@}

    //!
    //! Effect descriptor
    //!
    struct EffectDesc
    {
        //!
        //! texture input declaration
        //!
        struct TextureInput
        {
            StrA      name;   //!< texture name (optional)
            StrA      desc;   //!< long/detail description (optional)
            ImageType type;   //!< required image type, if not IMAGE_UNKNOWN.
            ClrFmt    format; //!< required image format, if not FMT_UNKNOWN.
        };

        //!
        //! effect constant input descriptor
        //!
        struct ConstInput
        {
            StrA      name;  //!< constant name (optional)
            StrA      desc;  //!< long/detail description (optional)
            ConstType type;  //!< type of values in constant.
            uint16_t  count; //!< number of values in constant.
        };

        //!
        //! texture output declaration
        //!
        struct TextureOutput
        {
            StrA      name;   //!< texture name (optional)
            StrA      desc;   //!< long/detail description (optional)
        };

        // properties
        StrA name;        //!< effect name (optional)
        StrA desc;        //!< long/detail description (optional)
        bool transparent; //!< is this a transparent effect?

        // inputs
        struct
        {
            DynaArray<TextureInput> textures;  //!< texture streams.
            VtxFmtDesc              vtxbuf;    //!< vertex stream.
            DynaArray<ConstInput>   constants; //!< input constants.
        } inputs; //!< effect inputs

        // outputs
        struct
        {
            DynaArray<TextureOutput> textures; //!< output texture streams
            VtxFmtDesc               vtxbuf;   //!< output vertex stream
        } outputs; //!< effect outputs
    };

    //!
    //! Shader interface.
    //!
    //! Shader is API/platform dependant implementation that implements one or multiple effects.
    //!
    struct Shader : public NoCopy
    {
        //! \name get supported effect information
        //@{
        //!
        //! effect support information
        //!
        struct ShaderSupportToEffect
        {
            EffectId effect;    //!< effect ID
            int      quality;   //!< rendering quality
            int      numPasses; //!< number of passes required.
        };
        virtual const ShaderSupportToEffect * getSupportedEffects() const = 0;
        virtual size_t getNumSupportedEffects() const = 0;
        //@}

        //! \name shader creation/deletion
        //@{
        virtual bool create() = 0;  //!< called once to initialize the shader (create all API/platform dependant data)
        virtual void destroy() = 0; //!< called once to destroy the shader (destroy all API/platformdependant data)
        //@}

        //! \name rendering functions
        //@{
        virtual void begin( EffectId ) = 0; //!< called every frame to prepare for rendering. like glBegin()
        virtual void setInputTextures( const BufferId *, size_t ) = 0;
        virtual void setInputVertices( BufferId ) = 0;
        virtual void setInputIndices( BufferId ) = 0;
        virtual void setInputIndices( PrimitiveType ) = 0;
        virtual void setInputConstants( const ConstData * const *, size_t ) = 0;
        virtual void setOutputTextures( const BufferId *, size_t ) = 0;
        virtual void setOutputVertices( BufferId ) = 0;
        virtual void draw( size_t startPrim = 0, size_t numPrims = 0 ) = 0;
        virtual BufferId getOutputTexture( size_t ) = 0;
        virtual BufferId getOutputVertices() = 0;
        virtual void end() = 0; //!< called every frame to end rendering. like glEnd().
        //@}
    };

    //!
    //! Garnet effect system.
    //!
    class EffectSystem
    {
        class Impl;
        Impl * mImpl;
    public:

        //! \name ctor and dtor
        //@{
        EffectSystem();
        virtual ~EffectSystem();
        //@}

        //! \name buffer manager
        //@{
        BufferId registerBuffer( Buffer * );
        Buffer * removeBuffer( BufferId );
        void     removeAllBuffers();
        Buffer * getBuffer( BufferId ) const;
        //@}

        //! \name Effect manager
        //@{
        EffectId           registerEffect( const EffectDesc & desc );
        void               removeEffect( EffectId );
        void               removeAllEffects();
        const EffectDesc * getEffectDesc( EffectId ) const;
        Shader *           getEffectShader( EffectId ) const;
        //@}

        //! \name shader manager
        //@{
        bool registerShaderDll( const StrA & dllName );
        bool registerShader( Shader * );
        void removeAllShaders();
        //@}
    };
}}}

// *****************************************************************************
//                           End of nes.h
// *****************************************************************************
#endif // __GN_GFX_NES_H__
