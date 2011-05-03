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
    class GN_API Variant
    {
        StrA mValue;

    public:

        /// \name constructors
        //@{
        Variant() {}
        Variant( const StrA & s ) { sets( s ); }
        Variant( bool b ) { setb(b); }
        Variant( int i ) { seti( i ); }
        Variant( float f ) { setf( f ); }
        Variant( void * p ) { setp( p ); }
        Variant( const Vector4f & v ) { setv( v ); }
        Variant( const Matrix44f & m ) { setm( m ); }
        //@}

        /// \name set variable value
        //@{
        void sets( const StrA & s ) { mValue = s; }
        void setb( bool b );
        void seti( int i );
        void setf( float f );
        void setp( void * p );
        void setv( const Vector4f & v );
        void setm( const Matrix44f & m );
        //@}

        /// \name Get variable value. Return false for incompatible type.
        //@{
        const StrA & gets() const { return mValue; }
        bool getb( bool & b ) const;
        bool geti( int & i ) const;
        bool getf( float & f ) const;
        bool getp( void * & p ) const;
        bool getv( Vector4f & v ) const;
        bool getm( Matrix44f & m ) const;
        //@}

        /// \name Get variable value. Return default value for incompatible type.
        //@{
        bool      getdb( bool defval ) const { bool r; if( getb(r) ) return r; else return defval; }
        int       getdi( int defval ) const { int r; if( geti(r) ) return r; else return defval; }
        float     getdf( float defval ) const { float r; if( getf(r) ) return r; else return defval; }
        void *    getdp( void * defval ) const { void * r; if( getp(r) ) return r; else return defval; }
        Vector4f  getdv( const Vector4f & defval ) const { Vector4f r; if( getv(r) ) return r; else return defval; }
        Matrix44f getdm( const Matrix44f & defval ) const { Matrix44f r; if( getm(r) ) return r; else return defval; }
        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_VARIANT_H__
