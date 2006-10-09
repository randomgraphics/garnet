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
    typedef uint16_t StreamId;
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
    //! Stream usage flags
    //!
    enum StreamUsage
    {
        SU_CPU_IMMUTABLE, //!< Input stream. Once the stream is created, it won't change any more.
        SU_CPU_DEFAULT,   //!< Input stream. Like D3DUSAGE_STATIC.
        SU_CPU_DYNAMIC,   //!< Input stream. Like D3DUSAGE_DYNAMIC.
        SU_GPU,           //!< output stream. Use to return GPU generated data back to CPU.
        NUM_STREAM_USAGES //!< number of stream usage flags.
    };

    //!
    //! Stream type
    //!
    enum StreamType
    {
        ST_TEXTURE,      //!< texture stream
        ST_VERTEX,       //!< vertex stream
        ST_INDEX,        //!< index stream
        NUM_STREAM_TYPES //!< number of stream types.
    };

    //! \name stream descriptor for specific resource 
    //@{
    typedef ImageDesc  TextureStreamDesc; //!< texture stream descriptor
    typedef VtxFmtDesc VertexStreamDesc; //!< vertex stream descriptor
    //!
    //! index stream descriptor
    //!
    struct IndexStreamDesc
    {
        PrimitiveType prim;   //!< primitive type
        size_t        numIdx; //!< number of indices
        bool          idx32;  //!< is 32-bit index buffer?
    };
    //@}

    //! \name stream interfaces
    //@{

    //!
    //! basic stream class
    //!
    class Stream : public NoCopy
    {
        StreamType  mType;
        StreamUsage mUsage;

    protected:

        //!
        //! ctor
        //!
        Stream( StreamType t, StreamUsage u ) : mType( t ), mUsage(u)
        {
            GN_ASSERT( 0 <= t && t <= NUM_STREAM_TYPES );
            GN_ASSERT( 0 <= u && u <= NUM_STREAM_USAGES );
        }

    public:

        //!
        //! get stream type
        //!
        StreamType getType() const { return mType; }

        //!
        //! get stream usage
        //!
        StreamUsage getUsage() const { return mUsage; }
    };

    //!
    //! texture stream class
    //!
    class TextureStream : public Stream
    {
    protected :

        //!
        //! ctor
        //!
        TextureStream( StreamUsage u ) : Stream( ST_TEXTURE, u ) {}

    public :

        //!
        //! get descriptor
        //!
        virtual const TextureStreamDesc & getDesc() const = 0;

        //!
        //! copy stream content to somewhere
        //!
        virtual bool copyTo(
            void * dst,
            size_t face,
            size_t level,
            size_t rowPitch,
            size_t slicePitch ) const = 0;
    };

    //!
    //! vertex stream class
    //!
    class VertexStream : public Stream
    {
    protected :

        //!
        //! ctor
        //!
        VertexStream( StreamUsage u ) : Stream( ST_VERTEX, u ) {}

    public :

        //!
        //! get descriptor
        //!
        virtual const VertexStreamDesc & getDesc() const = 0;

        //!
        //! copy stream content to somewhere
        //!
        virtual bool copyTo( void * dst, size_t stream ) const = 0;
    };

    //!
    //! index stream class
    //!
    class IndexStream : public Stream
    {
    protected :

        //!
        //! ctor
        //!
        IndexStream( StreamUsage u ) : Stream( ST_INDEX, u ) {}

    public :

        //!
        //! get descriptor
        //!
        virtual const IndexStreamDesc & getDesc() const = 0;

        //!
        //! copy stream content to somewhere
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
        virtual void setInputTextures( const StreamId *, size_t ) = 0;
        virtual void setInputVertices( StreamId ) = 0;
        virtual void setInputIndices( StreamId ) = 0;
        virtual void setInputIndices( PrimitiveType ) = 0;
        virtual void setInputConstants( const ConstData * const *, size_t ) = 0;
        virtual void setOutputTextures( const StreamId *, size_t ) = 0;
        virtual void setOutputVertices( StreamId ) = 0;
        virtual void draw( size_t startPrim = 0, size_t numPrims = 0 ) = 0;
        virtual StreamId getOutputTexture( size_t ) = 0;
        virtual StreamId getOutputVertices() = 0;
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

        //! \name stream manager
        //@{
        StreamId registerStream( Stream * stream );
        Stream * removeStream( StreamId );
        void     removeAllStreams();
        Stream * getStream( StreamId ) const;
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
