#ifndef __GN_BASE_VARIANT_H__
#define __GN_BASE_VARIANT_H__
// *****************************************************************************
/// \file
/// \brief   General multi-type variables
/// \author  chenlee (2006.2.7)
// *****************************************************************************

namespace GN
{
    ///
    /// General variant class
    ///
    /// \note
    /// - boolean value can be translated to stirng value "0" or "1"
    /// - string value like "yes", "true", "on" and "1" can be translate to boolean value "true" (case insensitive)
    /// - string value like "no", "false", "off" and "0" can be translate to boolean value "false" (case insensitive)
    ///
    class Variant
    {
        StrA mValue;

    public:

        /// \name constructors
        //@{
        Variant() {}
        Variant( const StrA & s ) { SetS( s ); }
        Variant( bool b ) { SetB(b); }
        Variant( int i ) { SetI( i ); }
        Variant( float f ) { SetF( f ); }
        Variant( void * p ) { SetP( p ); }
        Variant( const Vector4f & v ) { SetV( v ); }
        Variant( const Matrix44f & m ) { SetM( m ); }
        //@}

        /// \name set variable value
        //@{
        void SetS( const StrA & s ) { mValue = s; }
        void SetB( bool b );
        void SetI( int i );
        void SetF( float f );
        void SetP( void * p );
        void SetV( const Vector4f & v );
        void SetM( const Matrix44f & m );
        //@}

        /// \name Get variable value. Return false for incompatible type.
        //@{
        const StrA & GetS() const { return mValue; }
        bool GetB( bool & b ) const;
        bool GetI( int & i ) const;
        bool GetF( float & f ) const;
        bool GetP( void * & p ) const;
        bool GetV( Vector4f & v ) const;
        bool GetM( Matrix44f & m ) const;
        //@}

        /// \name Get variable value. Return default value for incompatible type.
        //@{
        bool      GetdB( bool defval ) const { bool r; if( GetB(r) ) return r; else return defval; }
        int       GetdI( int defval ) const { int r; if( GetI(r) ) return r; else return defval; }
        float     GetdF( float defval ) const { float r; if( GetF(r) ) return r; else return defval; }
        void *    GetdP( void * defval ) const { void * r; if( GetP(r) ) return r; else return defval; }
        Vector4f  GetdV( const Vector4f & defval ) const { Vector4f r; if( GetV(r) ) return r; else return defval; }
        Matrix44f GetdM( const Matrix44f & defval ) const { Matrix44f r; if( GetM(r) ) return r; else return defval; }
        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_VARIANT_H__
