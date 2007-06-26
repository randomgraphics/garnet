#ifndef __GN_GFX_RENDERSTATE_H__
#define __GN_GFX_RENDERSTATE_H__
// *****************************************************************************
/// \file
/// \brief   Graphics render states
/// \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    /// \def GNGFX_DEFINE_RS
    /// Define render states

    ///
    /// Render States
    ///
    enum RenderState
    {
        #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) RS_##tag,
        #include "renderStateMeta.h"
        #undef GNGFX_DEFINE_RS

        NUM_RENDER_STATES,   ///< number of available render states
        RS_INVALID           ///< indicate invalid render state type
    };

    ///
    /// Render state descriptor
    ///
    struct RenderStateDesc
    {
        ///
        /// render state value type
        ///
        enum ValueType
        {
            VT_ENUM,  ///< value is enumeration (one of RenderStateValue)
            VT_INT,   ///< value is integer
            VT_FLOAT, ///< value is float
        };

        const char * name;   ///< render state name
        ValueType valueType; ///< value type
        union
        {
            SInt32 defI; ///< default value as enumeration or integer
            float   defF; ///< default value
        };
        union
        {
            SInt32 minI; ///< min value as enumeration or integer
            float   minF; ///< min value is float
        };
        union
        {
            SInt32 maxI; ///< max value as enumeration or integer
            float   maxF; ///< max value is float
        };

        ///
        /// Check render state value
        ///
        bool checkValueI( SInt32 ) const;

        ///
        /// Check render state value
        ///
        bool checkValueF( float ) const;
    };

    ///
    /// Get descriptor of specific render state
    ///
    const RenderStateDesc & getRenderStateDesc( RenderState );

    ///
    /// Convert render state type to string
    ///
    /// \return
    ///     Return error message if failed.
    ///
    const char * renderState2Str( RenderState rs );

    ///
    /// Convert render state type to string
    ///
    /// \return
    ///     Return false if failed.
    ///
    bool renderState2Str( StrA & result, RenderState rs );

    ///
    /// Convert string to render state type
    ///
    /// \return
    ///     Return RS_INVALID if failed.
    ///
    RenderState str2RenderState( const char * );

    ///
    /// Convert string to render state type
    ///
    /// \return
    ///     Return false if failed.
    ///
    bool str2RenderState( RenderState & result, const char * str );

    /// \def GNGFX_DEFINE_RSV
    /// Define render state values

    ///
    /// Render State Values
    ///
    enum RenderStateValue
    {
        #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) RSV_##tag,
        #include "renderStateValueMeta.h"
        #undef GNGFX_DEFINE_RSV

        NUM_RENDER_STATE_VALUES,              ///< number of available render states
        RSV_INVALID = NUM_RENDER_STATE_VALUES ///< indicate a invalid value
    };

    ///
    /// Convert render state value type to string,
    ///
    /// \return
    ///     Return error message if failed.
    ///
    const char * renderStateValue2Str( RenderStateValue );

    ///
    /// Convert render state value type to string
    ///
    /// \return
    ///     Return false if failed.
    ///
    bool renderStateValue2Str( StrA & result, RenderStateValue rsval );

    ///
    /// Convert string to render value state type,
    ///
    /// \return
    ///     Return RSV_INVALID if failed, return false if failed.
    ///
    RenderStateValue str2RenderStateValue( const char * );

    ///
    /// Convert string to render value state type
    ///
    /// \return
    ///     Return false if failed.
    ///
    bool str2RenderStateValue( RenderStateValue & result, const char * str );

    /// \def GNGFX_DEFINE_TS
    /// Define texture stage states

    enum
    {
        ///
        /// 最多32层贴图
        ///
        MAX_TEXTURE_STAGES = 32
    };

    ///
    /// Texture Stage States
    ///
    enum TextureState
    {
        #define GNGFX_DEFINE_TS( tag, defval0, d3dname, glname1, glname2 ) TS_##tag,
        #include "textureStateMeta.h"
        #undef GNGFX_DEFINE_TS

        NUM_TEXTURE_STATES, ///< number of available texture stage states
        TS_INVALID,         ///< indicate an invalid TS type
    };

    ///
    /// Convert TS type to string.
    ///
    /// \return Return error message if failed.
    ///
    const char * textureState2Str( TextureState );

    ///
    /// Convert TS type to string, return false if failed.
    ///
    bool textureState2Str( StrA & result, TextureState ts );

    ///
    /// Convert string to TS type, return TS_INVALID if failed.
    ///
    TextureState str2TextureState( const char * );

    ///
    /// Convert string to TS type, return false if failed.
    ///
    bool str2TextureState( TextureState & result, const char * str );

    /// \def GNGFX_DEFINE_TSV
    /// Define texture stage state values

    ///
    /// Texture Stage State Values
    ///
    enum TextureStateValue
    {
        #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) TSV_##tag,
        #include "textureStateValueMeta.h"
        #undef GNGFX_DEFINE_TSV
        NUM_TEXTURE_STATE_VALUES,               ///< number of available texture stage state values
        TSV_INVALID = NUM_TEXTURE_STATE_VALUES, ///< indicate a invalid value
    };

    ///
    /// Convert TS value to string.
    ///
    /// \return Return error message if failed.
    ///
    const char * textureStateValue2Str( TextureStateValue );

    ///
    /// Convert TS value to string, return false if failed.
    ///
    bool textureStateValue2Str( StrA & result, TextureStateValue tssval );

    ///
    /// Convert string to TS value, return TS_INVALID if failed.
    ///
    TextureStateValue str2TextureStateValue( const char * );

    ///
    /// Convert string to TS value, return false if failed.
    ///
    bool str2TextureStateValue( TextureStateValue & result, const char * str );

    ///
    /// Render State Block Description Structure
    ///
    class RenderStateBlockDesc
    {
        enum
        {
            // flag that indicates a "complete" block
            COMPLETE = ((UInt32)-1) >> ( 32 - NUM_RENDER_STATES ),
        };

        SInt32  mValues[NUM_RENDER_STATES]; // render state values
        UInt32 mFlags; // Render state validality flag

        ///
        /// Only used to construct static members.
        ///
        RenderStateBlockDesc( bool toEmpty )
        {
            GN_CASSERT( NUM_RENDER_STATES <= 32 ); // make sure flag is large enough
            if( toEmpty ) resetToEmpty();
            else resetToDefault();
        }

        // ********************************
        /// \name constructors
        // ********************************
    public :

        //@{

        ///
        /// default constructor, do nothing.
        ///
        RenderStateBlockDesc() {}

        ///
        /// copy constructor
        ///
        RenderStateBlockDesc( const RenderStateBlockDesc & another ) : mFlags( another.mFlags )
        {
            ::memcpy( mValues, another.mValues, sizeof(mValues) );
        }

        //@}

        // ********************************
        /// \name  public data members
        // ********************************
    public :

        //@{

        static const RenderStateBlockDesc DEFAULT; ///< default rsblock
        static const RenderStateBlockDesc EMPTY; ///< empty rsblock. All fields are RSV_EMPTY

        //@}

        // ********************************
        /// \name  public operations
        // ********************************
    public :

        //@{

        ///
        /// reset all fields to RSV_EMPTY
        ///
        void resetToEmpty();

        ///
        /// reset all fields to default value
        ///
        void resetToDefault();

        ///
        /// make sure a valid render state block
        ///
        bool valid() const;

        ///
        /// check if specific render state is set or not.
        ///
        bool isSet( RenderState type ) const;

        ///
        /// get specific render state. Assert failure will
        /// be triggered, if flag of the render state is *not* set.
        ///
        SInt32 get( RenderState type ) const;

        ///
        /// set specific render state
        ///
        void set( RenderState type, int value );

        ///
        /// 求和, 将参数中的有效项复写到this中.
        ///
        /// \note a.mergeWith(b) does *not* equal with b.mergeWith(a)
        ///
        void mergeWith( const RenderStateBlockDesc & );

        ///
        /// 求和, same as: r = a; r.mergeWith(b);
        ///
        static RenderStateBlockDesc & sMerge(
            RenderStateBlockDesc & r,
            const RenderStateBlockDesc & a,
            const RenderStateBlockDesc & b );

        //@}

        // ********************************
        /// \name   public operators
        // ********************************
    public :

        //@{

        ///
        /// assignment
        ///
        RenderStateBlockDesc & operator=( const RenderStateBlockDesc & rhs )
        {
            ::memcpy( mValues, rhs.mValues, sizeof(mValues) );
            mFlags = rhs.mFlags;
            return *this;
        }

        ///
        /// add, same as sMerge.
        ///
        friend RenderStateBlockDesc operator+( const RenderStateBlockDesc & a, const RenderStateBlockDesc & b )
        {
            RenderStateBlockDesc r;
            return sMerge( r, a, b );
        }

        ///
        /// equality check
        ///
        bool operator==( const RenderStateBlockDesc & rhs ) const;

        ///
        /// equality check
        ///
        bool operator!=( const RenderStateBlockDesc & rhs ) const;
    };

    ///
    /// Texture State Block Description Structure.
    ///
    /// \note Texture states are used ONLY for fixed function pipeline.
    ///
    class TextureStateBlockDesc
    {
        enum { COMPLETE = 255 >> ( 8 - NUM_TEXTURE_STATES ) };

        size_t            mNumStages;
        TextureStateValue mValues[MAX_TEXTURE_STAGES][NUM_TEXTURE_STATES]; // values
        UInt8           mFlags[MAX_TEXTURE_STAGES];

        ///
        /// Only used to construct static data members.
        ///
        TextureStateBlockDesc( bool toEmpty )
        {
            GN_CASSERT( NUM_TEXTURE_STATES <= 8 ); // make sure flag is large enough
            if( toEmpty ) resetToEmpty();
            else resetToDefault();
        }

        // ********************************
        /// \name constructors
        // ********************************
    public :

        //@{

        ///
        /// default constructor. Do nothing
        ///
        TextureStateBlockDesc() : mNumStages(0) {}

        ///
        /// copy constructor
        ///
        TextureStateBlockDesc( const TextureStateBlockDesc & another )
            : mNumStages( another.mNumStages )
        {
            ::memcpy( mValues, another.mValues, sizeof(mValues[0])*mNumStages );
            ::memcpy( mFlags, another.mFlags, sizeof(mFlags[0])*mNumStages );
        }

        //@}

        // ********************************
        /// \name  public data members
        // ********************************
    public :

        //@{

        static const TextureStateBlockDesc DEFAULT; ///< default rsblock
        static const TextureStateBlockDesc EMPTY;   ///< empty rsblock (all fields are TSV_EMPTY)

        //@}

        // ********************************
        /// \name  public operations
        // ********************************
    public :

        //@{

        ///
        /// reset all fields to TSV_EMPTY
        ///
        void resetToEmpty();

        ///
        /// reset all fields to default value
        ///
        void resetToDefault();

        ///
        /// make sure a valid texture state block
        ///
        bool valid() const;

        ///
        /// get effective stage count
        ///
        size_t getNumStages() const { return mNumStages; }

        ///
        /// check if specific render state is set or not.
        ///
        bool isSet( size_t stage, TextureState type ) const;

        ///
        /// get specific render state. Assert failure will
        /// be triggered, if flag of the render state is *not* set.
        ///
        TextureStateValue get( size_t stage, TextureState type ) const;

        ///
        /// set specific render state
        ///
        void set( size_t stage, TextureState type, TextureStateValue value );

        ///
        /// 求和（将参数中所有的非EMPTY项复写到this中）.
        ///
        void mergeWith( const TextureStateBlockDesc & );

        ///
        /// 求和. Note that sMerge(A,B,C) != sMerge(A,C,B)
        ///
        static TextureStateBlockDesc & sMerge(
            TextureStateBlockDesc &,
            const TextureStateBlockDesc &,
            const TextureStateBlockDesc & );

        //@}

        // ********************************
        /// \name   public operators
        // ********************************
    public :

        //@{

        ///
        /// copy assignment
        ///
        TextureStateBlockDesc & operator=( const TextureStateBlockDesc & rhs )
        {
            mNumStages = rhs.mNumStages;
            ::memcpy( mValues, rhs.mValues, sizeof(mValues[0])*mNumStages );
            ::memcpy( mFlags, rhs.mFlags, sizeof(mFlags[0])*mNumStages );
            return *this;
        }

        ///
        /// equality check
        ///
        bool operator==( const TextureStateBlockDesc & rhs ) const;

        ///
        /// equality check
        ///
        bool operator!=( const TextureStateBlockDesc & rhs ) const;
    };
}}

#if GN_ENABLE_INLINE
#include "renderState.inl"
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_RENDERSTATE_H__
