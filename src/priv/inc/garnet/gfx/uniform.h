#ifndef __GN_GFX_UNIFORM_H__
#define __GN_GFX_UNIFORM_H__
// *****************************************************************************
//! \file    gfx/uniform.h
//! \brief   Uniform value class
//! \author  chen@@CHENLI-HOMEPC (2007.2.22)
// *****************************************************************************

#include <vector>

namespace GN { namespace gfx
{
    ///
    /// Uniform value type
    ///
    enum UniformValueType
    {
        UVT_BOOL,     ///< boolean
        UVT_INT,      ///< signed integer (32-bit)
        UVT_FLOAT,    ///< single precision floating point
        UVT_VECTOR4,  ///< 4 floats
        UVT_MATRIX44, ///< 4x4 row major matrix
        NUM_UNIFORM_VALUE_TYPES, ///< number of avaliable types.
    };

    ///
    /// Uniform value structure
    ///
    struct UniformValue
    {
        UniformValueType       type;      ///< value type
        std::vector<SInt32>    bools;     ///< Boolean value
        std::vector<SInt32>    ints;      ///< integer value
        std::vector<float>     floats;    ///< float value
        std::vector<Vector4f>  vector4s;  ///< vector value
        std::vector<Matrix44f> matrix44s; ///< matrix value

        ///
        /// default ctor: set type to invalid value, to make sure setX(...) happens before getting uniform value.
        ///
        UniformValue() : type(NUM_UNIFORM_VALUE_TYPES) {}

        ///
        /// construct uniform value from a 4-D vector
        ///
        UniformValue( const Vector4f & v )
            : type( UVT_VECTOR4  )
        {
            vector4s.push_back( v );
        }

        ///
        /// construct uniform value from a matrix
        ///
        UniformValue( const Matrix44f & m )
            : type( UVT_MATRIX44 )
        {
            matrix44s.push_back( m );
        }

        /// \name set uniform value
        //@{
        inline void setB( const SInt32 *, size_t );
        inline void setI( const SInt32 *, size_t );
        inline void setF( const float *, size_t );
        inline void setV( const Vector4f *, size_t );
        inline void setM( const Matrix44f *, size_t );

        inline void setB( SInt32 v ) { setB( &v, 1 ); }
        inline void setI( SInt32 v ) { setI( &v, 1 ); }
        inline void setF( float v ) { setF( &v, 1 ); }
        inline void setV( const Vector4f & v ) { setV( &v, 1 ); }
        inline void setM( const Matrix44f & v ) { setM( &v, 1 ); }
        //@}

        ///
        /// load uniform value from XML
        ///
        bool loadFromXmlNode( const XmlNode & );
    };

    // *************************************************************************
    // inline methods
    // *************************************************************************

    // -------------------------------------------------------------------------
    inline void UniformValue::setB( const SInt32 * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_BOOL;
        bools.resize( count );
        if( count > 0 && values ) ::memcpy( &bools[0], values, count*sizeof(SInt32) );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void UniformValue::setI( const SInt32 * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_INT;
        ints.resize( count );
        if( count > 0 && values ) ::memcpy( &ints[0], values, count*sizeof(SInt32) );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void UniformValue::setF( const float * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_FLOAT;
        floats.resize( count );
        if( count > 0 && values ) ::memcpy( &floats[0], values, count*sizeof(float) );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void UniformValue::setV( const Vector4f * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_VECTOR4;
        vector4s.resize( count );
        if( count > 0 && values ) ::memcpy( &vector4s[0], values, count*sizeof(Vector4f) );
        GN_UNGUARD_SLOW;
    }
    // --
    inline void UniformValue::setM( const Matrix44f * values, size_t count )
    {
        GN_GUARD_SLOW;
        type = UVT_MATRIX44;
        matrix44s.resize( count );
        if( count > 0 && values ) ::memcpy( &matrix44s[0], values, count*sizeof(Matrix44f) );
        GN_UNGUARD_SLOW;
    }
}}

// *****************************************************************************
//                           End of uniform.h
// *****************************************************************************
#endif // __GN_GFX_UNIFORM_H__
