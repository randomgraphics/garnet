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
    typedef Functor2<
        bool,          //!< return value. 
        const void *,  //!< from
        void*          //!< to
        > VariantTypeConvertFunctor;

    //!
    //! General multi-type class
    //!
    class Variant
    {
        //! \cond NEVER

        struct BaseValue
        {
            int    timeStamp;
            void * pointerToValue;

            BaseValue( void * p ) : timeStamp(0), pointerToValue(p) {}
        };

        template<typename T>
        struct TheValue : public BaseValue
        {
            T value;     //!< current value

            TheValue() : BaseValue(&value) {}

            void set( const T & newValue, int newTimeStamp )
            {
                value = newValue;
                timeStamp = newTimeStamp;
            }
        };

        template<typename T>
        struct TypeCool
        {
            template<bool boolean, bool integer, bool real, bool pointer, bool pchar, bool str, bool vector, bool matrix> struct Helper {};

            template<> struct Helper<1,0,0,0,0,0,0,0>
            {
                typedef bool ReturnType;
                typedef bool ParamType;
                static const VariantType variantType = VARIANT_BOOL;
            };

            template<> struct Helper<0,1,0,0,0,0,0,0>
            {
                typedef int ReturnType;
                typedef int ParamType;
                static const VariantType variantType = VARIANT_INT;
            };

            template<> struct Helper<0,0,1,0,0,0,0,0>
            {
                typedef float ReturnType;
                typedef float ParamType;
                static const VariantType variantType = VARIANT_FLOAT;
            };

            template<> struct Helper<0,0,0,1,0,0,0,0>
            {
                typedef void * ReturnType;
                typedef const void * ParamType;
                static const VariantType variantType = VARIANT_POINTER;
            };

            template<> struct Helper<0,0,0,1,1,1,0,0>
            {
                typedef const char * ReturnType;
                typedef const char * ParamType;
                static const VariantType variantType = VARIANT_STRING;
            };

            template<> struct Helper<0,0,0,0,0,1,0,0>
            {
                typedef const StrA & ReturnType;
                typedef const StrA & ParamType;
                static const VariantType variantType = VARIANT_STRING;
            };

            template<> struct Helper<0,0,0,1,0,0,1,0>
            {
                typedef const Vector4f & ReturnType;
                typedef const Vector4f & ParamType;
                static const VariantType variantType = VARIANT_VECTOR4;
            };

            template<> struct Helper<0,0,0,1,0,0,0,1>
            {
                typedef const Matrix44f & ReturnType;
                typedef const Matrix44f & ParamType;
                static const VariantType variantType = VARIANT_MATRIX44;
            };

            typedef Helper<
                IsSameType<T,bool>::value,
                IsSameType<T,int>::value,
                IsSameType<T,float>::value,
                IsConvertible<T,const void*>::value,
                IsConvertible<T,const char*>::value,
                IsConvertible<T,const StrA&>::value,
                IsConvertible<T,const Vector4f &>::value,
                IsConvertible<T,const Matrix44f &>::value > TheHelper;

            typedef typename TheHelper::ReturnType ReturnType;
            typedef typename TheHelper::ParamType ParamType;
            static const VariantType variantType = TheHelper::variantType;
        };

        typedef std::map<int16_t,VariantTypeConvertFunctor> ConvertFunctorMap;

        template<typename T> T getValue() const;
        template<> bool getValue<bool>() const { return mBool.value; }
        template<> int  getValue<int>() const { return mInt.value; }
        template<> float getValue<float>() const { return mFloat.value; }
        template<> void * getValue<void*>() const { return (void*)mPointer.value; }
        template<> const char * getValue<const char*>() const { return mString.value.cptr(); }
        template<> const StrA & getValue<const StrA&>() const { return mString.value; }
        template<> const Vector4f & getValue<const Vector4f&>() const { return mVector4.value; }
        template<> const Matrix44f & getValue<const Matrix44f&>() const { return mMatrix44.value; }

        template<typename T> void setValue( T ) const;
        template<> void setValue<bool>( bool b ) const { mBool.set( b, mTimeStamp ); }
        template<> void setValue<int>( int i ) const { mInt.set( i, mTimeStamp ); }
        template<> void setValue<float>( float f ) const { mFloat.set( f, mTimeStamp ); }
        template<> void setValue<const void*>( const void * p ) const { mPointer.set( p, mTimeStamp ); }
        template<> void setValue<const char*>( const char * s ) const { mString.set( s, mTimeStamp ); }
        template<> void setValue<const StrA&>( const StrA & s ) const { mString.set( s, mTimeStamp ); }
        template<> void setValue<const Vector4f&>( const Vector4f & v ) const { mVector4.set( v, mTimeStamp ); }
        template<> void setValue<const Matrix44f&>( const Matrix44f & m ) const { mMatrix44.set( m, mTimeStamp ); }

        void buildValueArray();

        bool convertTo( VariantType type ) const;

        int mTimeStamp; //!< time stamp for the lastest 'set' operation.
        VariantType mType;
        BaseValue * mValues[NUM_VARIANT_TYPES];
        mutable TheValue<bool> mBool;
        mutable TheValue<int> mInt;
        mutable TheValue<float> mFloat;
        mutable TheValue<const void *> mPointer;
        mutable TheValue<StrA> mString;
        mutable TheValue<Vector4f> mVector4;
        mutable TheValue<Matrix44f> mMatrix44;
        ConvertFunctorMap mConvertTable;

        //! \endcond

    public:

        //!
        //! Default constructor
        //!
        Variant() : mTimeStamp(1), mType(VARIANT_INVALID) { buildValueArray(); }

        //!
        //! Constructor from specific type
        //!
        template< typename T >
        Variant( const T & v ) : mTimeStamp(1), mType( TypeCool<T>::variantType )
        {
            GN_ASSERT( 0 <= mType && mType < VARIANT_INVALID );

            buildValueArray();

            set( v );
        }

        //!
        //! Destructor
        //!
        ~Variant() {}

        //!
        //! get type
        //!
        VariantType getType() const { return mType; }

        //!
        //! set variable value
        //!
        template<typename T>
        void set( const T & v )
        {   
            mType = TypeCool<T>::variantType;
            ++mTimeStamp;
            setValue<typename TypeCool<T>::ParamType>( v );
        }

        //!
        //! Get variable value. Return false for incompatible type.
        //!
        template<typename T>
        bool get( T & v ) const
        {
            const VariantType t = TypeCool<T>::variantType;
            if( t != mType && mValues[t]->timeStamp != mTimeStamp && !convertTo( t ) ) return false;
            GN_ASSERT( t == mType || mValues[t]->timeStamp == mTimeStamp );
            v = getValue<TypeCool<T>::ReturnType>();
            return true;
        }

        //!
        //! Get variable value. Return default value for incompatible type.
        //!
        template<typename T>
        typename typename TypeCool<T>::ReturnType getd( const T & defval ) const
        {
            const VariantType t = TypeCool<T>::variantType;
            if( t != mType && mValues[t]->timeStamp != mTimeStamp && !convertTo( t ) )
                return defval;
            GN_ASSERT( t == mType || mValues[t]->timeStamp == mTimeStamp );
            return getValue<typename TypeCool<T>::ReturnType>();
        }

        //!
        //! Register new type conversion functor
        //!
        void registerTypeConvertion( VariantType from, VariantType to, const VariantTypeConvertFunctor & functor );

        //!
        //! Remove specific type conversion functor
        //!
        void removeTypeConversion( VariantType from, VariantType to );
    };
 }

// *****************************************************************************
//                           End of variant.h
// *****************************************************************************
#endif // __GN_BASE_VARIANT_H__
