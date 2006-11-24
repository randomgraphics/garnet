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
    typedef uint16_t EffectId;
    typedef uint16_t BufferId;
    typedef uint16_t ConstId;
    //@}

    //!
    //! Buffer type
    //!
    enum BufferType
    {
        BT_PXLBUF,           //!< pixel buffer (texture)
        BT_VTXBUF,           //!< vertex buffer
        BT_IDXBUF,           //!< index buffer
        BT_RAW,              //!< typeless raw data
        BT_NUM_BUFFER_TYPES, //!< number of buffer types.
    };

    //!
    //! CPU access flags
    //!
    enum CpuAccess
    {
        CA_IMMUTABLE          = 0,
        CA_WRITE_INFREQUENTLY = 1,
        CA_WRITE_FREQUENTLY   = 2,
        CA_READ_BACK          = 4,
    };
    //!
    //! buffer descriptor
    //!
    struct BufferDesc
    {
        BufferType  type;  //!< buffer type
        BitField    ca;    //!< CPU access flags. Combinations of CpuAccess flags.

        union
        {
            struct
            {
                uint32_t width;  //!< texture width.
                uint32_t height; //!< texture height.
                uint32_t depth;  //!< texture depth. 1 for 2D texture.
                uint32_t faces;  //!< texture faces. 1 for single texture, 6 for cubemap, other values for texture array.
                uint32_t levels; //!< mipmap levels.
                ClrFmt   pxlfmt; //!< pixel format.
            } pb; //!< pixel buffer descriptor

            struct
            {
                VtxFmtDesc format;   //!< vertex buffer format
                uint32_t   capacity; //!< maximum number of vertices this buffer can hold.
            } vb; //!< vertex buffer descriptor

            struct
            {
                uint32_t capacity; //!< maximum number of indices this buffer can hold.
                bool     bit32;    //!< If true, means it is 32-bits index buffer; else it is 16-bits.
            } ib; //!< index buffer descriptor

            struct
            {
                uint32_t bytes; //!< bytes of the raw data.
            } raw; //!< raw buffer descriptor
        };
    };
    //!
    //! This is buffer creation parameters
    //!
    struct BufferCreationParameters : public BufferDesc
    {
        struct EffectBindingInfo
        {
            EffectId effect;
            StrA     port;
        };

        //! \name initial data.
        //@{
        const void * sysMem;
        size_t       sysMemRawPitch;
        size_t       sysMemSlicePitch;
        //@}

        //! \name sub buffer properties
        //@{
        BufferId parent; //!< parent buffer ID. Zero for top-level buffer.
        size_t   level;  //!< mipmap level in parent buffer.
        size_t   face;   //!< face index in parent buffer.
        size_t   slice;  //!< slice index in parent buffer.
        //@}

        //!
        //! effect binding information
        //!
        DynaArray<EffectBindingInfo> bindingToTheseEffects;

        //!
        //! buffer reusing information
        //!
        DynaArray<BufferId> reuseOneOfTheseIfPossible;

        //!
        //! util function that creates binding to specific effect.
        //!
        void bindToEffect( EffectId eff, const StrA & port )
        {
            bindingToTheseEffects.resize( bindingToTheseEffects.size() + 1 );
            EffectBindingInfo & ebi = bindingToTheseEffects.back();
            ebi.effect = eff;
            ebi.port = port;
        }
    };
    //!
    //! effect constant type
    //!
    enum ConstType
    {
        CT_BOOL32,          //!< 32-bit boolean constant
        CT_INT32,           //!< 32-bit integer constant
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
        ConstType type;  //!< type of values in constant.
        uint16_t  count; //!< number of values in constant.
    };
    struct ConstCreationParameters : public ConstDesc
    {
        DynaArray<ConstId> reuseOneOfTheseIfPossible;
    };
    //!
    //! Effect descriptor
    //!
    struct EffectDesc
    {
        struct BufferPort : public BufferDesc
        {
            enum
            {
                INPUT  = 1,
                OUTPUT = 2,
                IO     = 3,
            } io;
        };
        struct ConstPort : public ConstDesc
        {
            int defaultValue;
        };
        std::map<StrA,BufferPort> bufferPorts;
        std::map<StrA,ConstPort>  constPorts;
    };
    struct DrawParameters
    {
        EffectId                effect;
        std::map<StrA,BufferId> buffers;
        std::map<StrA,ConstId>  consts;
    };
    class EffectManager
    {
    public:

        //! \name ctor and dtor
        //@{
        EffectManager() {}
        virtual ~EffectManager() {}
        //@}

        //! \name effect manager
        //@{
        EffectId           registerEffect( const EffectDesc & );
        void               removeEffect( EffectId );
        const EffectDesc & getEffectDesc( EffectId ) const;
        //@}

        /*! \name shader manager
        //@{
        struct Shader : public NoCopy
        {
        };
        struct ShaderFactory : public NoCopy
        {
            virtual size_t   getNumShaders() const = 0;
            virtual Shader * createShader( size_t index ) const = 0;
            virtual void     deleteShader( Shader * );
        };
        bool registerShaderFactory( const ShaderFactory * );
        void removeShaderFactory( const SahderFactory * );
        //@}//**/

        //! \name buffer manager
        //@{
        BufferId           createBuffer( const BufferCreationParameters & );
        void               deleteBuffer( BufferId );
        const BufferDesc & getBufferDesc( BufferId ) const;
        //@}

        //! \name constant manager
        //@{
        ConstId           createConst( const ConstCreationParameters & );
        void              deleteConst( ConstId );
        const ConstDesc & getConstDesc( ConstId );
        //@}

        //!
        //! do rendering
        //!
        void draw( const DrawParameters & );
    };

    /*

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
    };
    //*/
}}}

// *****************************************************************************
//                           End of nes.h
// *****************************************************************************
#endif // __GN_GFX_NES_H__
