#ifndef __GN_BASE_VARIANT_H__
#define __GN_BASE_VARIANT_H__
// *****************************************************************************
//! \file    base/variant.h
//! \brief   General multi-type variables
//! \author  chenlee (2006.2.7)
// *****************************************************************************

namespace GN
{
    //!
    //! variant type
    //!
    enum VariantType
    {
        VARIANT_BOOL,      //!< boolean
        VARIANT_INT,       //!< signed integer
        VARIANT_FLOAT,     //!< single-precision floating point
        VARIANT_POINTER,   //!< type-less pointer (void*)
        VARIANT_STRING,    //!< string
        VARIANT_VECTOR4,   //!< 4-D vector
        VARIANT_MATRIX44,  //!< 4x4 matrix
        NUM_VARIANT_TYPES, //!< number of variant types.
        VARIANT_INVALID    //!< invalid type
    };

    //!
    //! Convert variant type to string
    //!
    inline const char * variantType2Str( VariantType vt )
    {
        static const char * sTable[] =
        {
            "VARIANT_BOOL",
            "VARIANT_INT",
            "VARIANT_FLOAT",
            "VARIANT_POINTER",
            "VARIANT_STRING",
            "VARIANT_VECTOR4",
            "VARIANT_MATRIX44"
        };
        if( 0 <= vt && vt < NUM_VARIANT_TYPES ) return sTable[vt];
        else return "VARIANT_INVALID";
    }

    //!
    //! variant type convert functor
    //!
    typedef Functor2<bool,const void *, void*> VariantTypeConvertFunctor;

    //!
    //! General multi-type class
    //!
    class Variant
    {
    public:

        //! \name ctor / dtor
        //@{
        Variant() : mTimeStamp(1), mType(VARIANT_INVALID) { buildValueArray(); }
        Variant( bool b ) : mTimeStamp(1), mType(VARIANT_BOOL) { buildValueArray(); mBool.set( b, 1 ); }
        Variant( int i ) : mTimeStamp(1), mType(VARIANT_INT) { buildValueArray(); mInt.set( i, 1 ); }
        Variant( float f ) : mTimeStamp(1), mType(VARIANT_FLOAT) { buildValueArray(); mFloat.set( f, 1 ); }
        Variant( void * p ) : mTimeStamp(1), mType(VARIANT_POINTER) { buildValueArray(); mPointer.set( p, 1 ); }
        Variant( const StrA & s ) : mTimeStamp(1), mType(VARIANT_STRING) { buildValueArray(); mString.set( s, 1); }
        Variant( const Vector4f & v ) : mTimeStamp(1), mType(VARIANT_VECTOR4) { buildValueArray(); mVector4.set( v, 1); }
        Variant( const Matrix44f & m ) : mTimeStamp(1), mType(VARIANT_MATRIX44) { buildValueArray(); mMatrix44.set( m, 1 ); }
        ~Variant() {}
        //@}

        //!
        //! get type
        //!
        VariantType getType() const { return mType; }

        //! \name set variable value
        //@{

        void setB( bool v ) { mType = VARIANT_BOOL; ++mTimeStamp; mBool.set(v,mTimeStamp); }
        void setI( int v ) { mType = VARIANT_INT; ++mTimeStamp; mInt.set(v,mTimeStamp); }
        void setF( float v ) { mType = VARIANT_FLOAT; ++mTimeStamp; mFloat.set(v,mTimeStamp); }
        void setP( void * v ) { mType = VARIANT_POINTER; ++mTimeStamp; mPointer.set(v,mTimeStamp); }
        void setS( const StrA & v ) { mType = VARIANT_STRING; ++mTimeStamp; mString.set(v,mTimeStamp); }
        void setV( const Vector4f & v ) { mType = VARIANT_VECTOR4; ++mTimeStamp; mVector4.set(v,mTimeStamp); }
        void setM( const Matrix44f & v ) { mType = VARIANT_MATRIX44; ++mTimeStamp; mMatrix44.set(v,mTimeStamp); }

        //@}

        //! \name Get variable value. Return false for incompatible type.
        //@{

        #define _GN_VARIANT_GET( type, var ) \
            if( type != mType && var.timeStamp != mTimeStamp && !convertTo( type ) ) return false; \
            GN_ASSERT( type == mType || var.timeStamp == mTimeStamp ); \
            v = var.value; \
            return true;

        bool getB( bool & v ) const { _GN_VARIANT_GET(VARIANT_BOOL, mBool ); }
        bool getI( int & v ) const { _GN_VARIANT_GET( VARIANT_INT, mInt ); }
        bool getF( float & v ) const { _GN_VARIANT_GET( VARIANT_FLOAT, mFloat ); }
        bool getP( void * & v ) const { _GN_VARIANT_GET( VARIANT_POINTER, mPointer ); }
        bool getS( StrA & v ) const { _GN_VARIANT_GET( VARIANT_STRING, mString ); }
        bool getV( Vector4f & v ) const { _GN_VARIANT_GET( VARIANT_VECTOR4, mVector4 ); }
        bool getM( Matrix44f & v ) const { _GN_VARIANT_GET( VARIANT_MATRIX44, mMatrix44 ); }

        #undef _GN_VARIANT_GET

        //@}

        //! \name Type conversion management
        //@{

        void registerTypeConvertion( VariantType from, VariantType to, const VariantTypeConvertFunctor & functor );
        void removeTypeConversion( VariantType from, VariantType to );

        //@}

    private:

        struct BaseValue
        {
            int    timeStamp;
            void * pointerToValue;

            BaseValue( void * p ) : timeStamp(0), pointerToValue(p) {}
        };

        template<typename T>
        struct TheValue : BaseValue
        {
            T value;     //!< current value

            TheValue() : BaseValue(&value) {}

            void set( const T & newValue, int newTimeStamp )
            {
                value = newValue;
                timeStamp = newTimeStamp;
            }
        };

        int mTimeStamp; //!< time stamp for the lastest 'set' operation.

        VariantType mType;

        BaseValue * mValues[NUM_VARIANT_TYPES];
        mutable TheValue<bool> mBool;
        mutable TheValue<int> mInt;
        mutable TheValue<float> mFloat;
        mutable TheValue<void *> mPointer;
        mutable TheValue<StrA> mString;
        mutable TheValue<Vector4f> mVector4;
        mutable TheValue<Matrix44f> mMatrix44;

        typedef std::map<int16_t,VariantTypeConvertFunctor> ConvertFunctorMap;
        ConvertFunctorMap mConvertTable;

    private:
        void buildValueArray(); // called by constructor
        bool convertTo( VariantType type ) const;
    };
}

// *****************************************************************************
//                           End of variant.h
// *****************************************************************************
#endif // __GN_BASE_VARIANT_H__
