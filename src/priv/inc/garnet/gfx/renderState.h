#ifndef __GN_GFX_RENDERSTATE_H__
#define __GN_GFX_RENDERSTATE_H__
// *****************************************************************************
//! \file    renderState.h
//! \brief   Graphics render states
//! \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    //! \def GNGFX_DEFINE_RS
    //! Define render states

    //!
    //! Render States
    //!
    enum RenderState
    {
        #define GNGFX_DEFINE_RS( tag, defval ) RS_##tag,
        #include "renderStateMeta.h"
        #undef GNGFX_DEFINE_RS

        NUM_RENDER_STATES,   //!< number of available render states
        RS_INVALID           //!< indicate invalid render state type
    };

    //!
    //! Convert render state type to string
    //!
    //! \return
    //!     Return error message if failed.
    //!
    const char * renderState2Str( RenderState rs );

    //!
    //! Convert render state type to string
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool renderState2Str( StrA & result, RenderState rs );

    //!
    //! Convert string to render state type
    //!
    //! \return
    //!     Return RS_INVALID if failed.
    //!
    RenderState str2RenderState( const char * );

    //!
    //! Convert string to render state type
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool str2RenderState( RenderState & result, const char * str );

    //! \def GNGFX_DEFINE_RSV
    //! Define render state values

    //!
    //! Render State Values
    //!
    enum RenderStateValue
    {
        #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) RSV_##tag,
        #include "renderStateValueMeta.h"
        #undef GNGFX_DEFINE_RSV

        NUM_RENDER_STATE_VALUES,    //!< number of available render states
        RSV_INVALID                 //!< indicate a invalid value
    };

    //!
    //! Convert render state value type to string,
    //!
    //! \return
    //!     Return error message if failed.
    //!
    const char * renderStateValue2Str( RenderStateValue );

    //!
    //! Convert render state value type to string
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool renderStateValue2Str( StrA & result, RenderStateValue rsval );

    //!
    //! Convert string to render value state type,
    //!
    //! \return
    //!     Return RSV_INVALID if failed, return false if failed.
    //!
    RenderStateValue str2RenderStateValue( const char * );

    //!
    //! Convert string to render value state type
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool str2RenderStateValue( RenderStateValue & result, const char * str );

    //! \def GNGFX_DEFINE_TS
    //! Define texture stage states

    enum
    {
        //!
        //! 最多16层贴图
        //!
        MAX_TEXTURE_STAGES = 16
    };

    //!
    //! Texture Stage States
    //!
    enum TextureState
    {
        #define GNGFX_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 ) TS_##tag,
        #include "textureStateMeta.h"
        #undef GNGFX_DEFINE_TS

        NUM_TEXTURE_STATES, //!< number of available texture stage states
        TS_INVALID,         //!< indicate an invalid TS type
    };

    //!
    //! Convert TS type to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * textureState2Str( TextureState );

    //!
    //! Convert TS type to string, return false if failed.
    //!
    bool textureState2Str( StrA & result, TextureState ts );

    //!
    //! Convert string to TS type, return TS_INVALID if failed.
    //!
    TextureState str2TextureState( const char * );

    //!
    //! Convert string to TS type, return false if failed.
    //!
    bool str2TextureState( TextureState & result, const char * str );

    //! \def GNGFX_DEFINE_TSV
    //! Define texture stage state values

    //!
    //! Texture Stage State Values
    //!
    enum TextureStateValue
    {
        #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) TSV_##tag,
        #include "textureStateValueMeta.h"
        #undef GNGFX_DEFINE_TSV

        NUM_TEXTURE_STATE_VALUES,   //!< number of available texture stage state values
        TSV_INVALID,                //!< indicate a invalid value
    };

    //!
    //! Convert TS value to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * textureStateValue2Str( TextureStateValue );

    //!
    //! Convert TS value to string, return false if failed.
    //!
    bool textureStateValue2Str( StrA & result, TextureStateValue tssval );

    //!
    //! Convert string to TS value, return TS_INVALID if failed.
    //!
    TextureStateValue str2TextureStateValue( const char * );

    //!
    //! Convert string to TS value, return false if failed.
    //!
    bool str2TextureStateValue( TextureStateValue & result, const char * str );

    //!
    //! Render State Block Description Structure
    //!
    struct RenderStateBlockDesc
    {
        static const RenderStateBlockDesc DEFAULT; //!< default rsblock
        static const RenderStateBlockDesc INVALID; //!< invalid rsblock

        //!
        //! reset flag
        //!
        enum ResetFlag
        {
            RESET_TO_INVALID, //!< reset all fields to "RSV_INVALID"
            RESET_TO_DEFAULT, //!< reset all fields to default value
        };

        //!
        //! render states
        //!
        RenderStateValue rs[NUM_RENDER_STATES];

        //!
        //! texture stage states
        //!
        TextureStateValue ts[MAX_TEXTURE_STAGES][NUM_TEXTURE_STATES];

        // ********************************
        //! \name constructors
        // ********************************
    public :

        //@{

        //!
        //! default constructor
        //!
        RenderStateBlockDesc() {}

        //!
        //! construct & reset a rsblock description structure
        //!
        RenderStateBlockDesc( ResetFlag flag )
        {
            reset( flag );
        }

        //@}

        // ********************************
        //! \name  public operations
        // ********************************
    public :

        //@{

        //!
        //! reset all fields to default/invalid value
        //!
        void reset( ResetFlag flag );

        //@}

        // ********************************
        //! \name   public operators
        // ********************************
    public :

        //@{

        //!
        //! 等值判定
        //!
        bool operator == ( const RenderStateBlockDesc & ) const;

        //!
        //! 等值判定
        //!
        bool operator != ( const RenderStateBlockDesc & ) const;

        //!
        //! 求和（将参数中所有的非invalid项复写到this中）.
        //! <b>注意，此加法运算不符合交换率，(A+B) != (B+A)</b>
        //!
        RenderStateBlockDesc & operator += ( const RenderStateBlockDesc & );

        //!
        //! 求和
        //!
        RenderStateBlockDesc   operator +  ( const RenderStateBlockDesc & ) const;

        //!
        //! 求差（相同的项相减结果为 RSV_INVALID 或者 TSV_INVALID）
        //!
        RenderStateBlockDesc & operator -= ( const RenderStateBlockDesc & );

        //!
        //! 求差
        //!
        RenderStateBlockDesc   operator  - ( const RenderStateBlockDesc & ) const;
    };
}}

#if GN_ENABLE_INLINE
#include "renderState.inl"
#endif

// *****************************************************************************
//                           End of renderState.h
// *****************************************************************************
#endif // __GN_GFX_RENDERSTATE_H__
