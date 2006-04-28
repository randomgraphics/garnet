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
    typedef Delegate2<
        bool,          //!< return value. 
        const void *,  //!< from
        void*          //!< to
        > VariantTypeConvertFunctor;

    //! \cond NEVER
    namespace detail
    {
        template<bool boolean, bool integer, bool real, bool pointer, bool pchar, bool str, bool vector, bool matrix> struct VariantTypeHelper {};

        template<> struct VariantTypeHelper<1,0,0,0,0,0,0,0>
        {
            typedef bool ReturnType;
            typedef bool ParamType;
            static const VariantType variantType = VARIANT_BOOL;
        };

        template<> struct VariantTypeHelper<0,1,0,0,0,0,0,0>
        {
            typedef int ReturnType;
            typedef int ParamType;
            static const VariantType variantType = VARIANT_INT;
        };

        template<> struct VariantTypeHelper<0,0,1,0,0,0,0,0>
        {
            typedef float ReturnType;
            typedef float ParamType;
            static const VariantType variantType = VARIANT_FLOAT;
        };

        template<> struct VariantTypeHelper<0,0,0,1,0,0,0,0>
        {
            typedef void * ReturnType;
            typedef const void * ParamType;
            static const VariantType variantType = VARIANT_POINTER;
        };

        template<> struct VariantTypeHelper<0,0,0,1,1,1,0,0>
        {
            typedef const char * ReturnType;
            typedef const char * ParamType;
            static const VariantType variantType = VARIANT_STRING;
        };

        template<> struct VariantTypeHelper<0,0,0,0,0,1,0,0>
        {
            typedef const StrA & ReturnType;
            typedef const StrA & ParamType;
            static const VariantType variantType = VARIANT_STRING;
        };

        template<> struct VariantTypeHelper<0,0,0,1,0,0,1,0>
        {
            typedef const Vector4f & ReturnType;
            typedef const Vector4f & ParamType;
            static const VariantType variantType = VARIANT_VECTOR4;
        };

        template<> struct VariantTypeHelper<0,0,0,1,0,0,0,1>
        {
            typedef const Matrix44f & ReturnType;
            typedef const Matrix44f & ParamType;
            static const VariantType variantType = VARIANT_MATRIX44;
        };
    }
    //! \endcond

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
            typedef detail::VariantTypeHelper<
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

        // Note: these two functions have to be static, because gcc 3.4.x does not
        //       allow explicit specialization in non-namespace scope.
        template<typename T> static T sGetValue( const Variant & );
        template<typename T> static void sSetValue( Variant &, T );

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
            sSetValue<typename TypeCool<T>::ParamType>( *this, v );
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
            v = sGetValue<typename TypeCool<T>::ReturnType>( *this );
            return true;
        }

        //!
        //! Get variable value. Return default value for incompatible type.
        //!
        template<typename T>
        typename TypeCool<T>::ReturnType getd( const T & defval ) const
        {
            const VariantType t = TypeCool<T>::variantType;
            if( t != mType && mValues[t]->timeStamp != mTimeStamp && !convertTo( t ) )
                return defval;
            GN_ASSERT( t == mType || mValues[t]->timeStamp == mTimeStamp );
            return sGetValue<typename TypeCool<T>::ReturnType>( *this );
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

    //! \cond NEVER
    template<> inline bool Variant::sGetValue<bool>( const Variant & var ) { return var.mBool.value; }
    template<> inline int  Variant::sGetValue<int>( const Variant & var ) { return var.mInt.value; }
    template<> inline float Variant::sGetValue<float>( const Variant & var ) { return var.mFloat.value; }
    template<> inline void * Variant::sGetValue<void*>( const Variant & var ) { return (void*)var.mPointer.value; }
    template<> inline const char * Variant::sGetValue<const char*>( const Variant & var ) { return var.mString.value.cptr(); }
    template<> inline const StrA & Variant::sGetValue<const StrA&>( const Variant & var ) { return var.mString.value; }
    template<> inline const Vector4f & Variant::sGetValue<const Vector4f&>( const Variant & var ) { return var.mVector4.value; }
    template<> inline const Matrix44f & Variant::sGetValue<const Matrix44f&>( const Variant & var ) { return var.mMatrix44.value; }

    template<> inline void Variant::sSetValue<bool>( Variant & var, bool b ) { var.mBool.set( b, var.mTimeStamp ); }
    template<> inline void Variant::sSetValue<int>( Variant & var, int i ) { var.mInt.set( i, var.mTimeStamp ); }
    template<> inline void Variant::sSetValue<float>( Variant & var, float f ) { var.mFloat.set( f, var.mTimeStamp ); }
    template<> inline void Variant::sSetValue<const void*>( Variant & var, const void * p ) { var.mPointer.set( p, var.mTimeStamp ); }
    template<> inline void Variant::sSetValue<const char*>( Variant & var, const char * s ) { var.mString.set( s, var.mTimeStamp ); }
    template<> inline void Variant::sSetValue<const StrA&>( Variant & var, const StrA & s ) { var.mString.set( s, var.mTimeStamp ); }
    template<> inline void Variant::sSetValue<const Vector4f&>( Variant & var, const Vector4f & v ) { var.mVector4.set( v, var.mTimeStamp ); }
    template<> inline void Variant::sSetValue<const Matrix44f&>( Variant & var, const Matrix44f & m ) { var.mMatrix44.set( m, var.mTimeStamp ); }
    //! \endcond
}

// *****************************************************************************
//                           End of variant.h
// *****************************************************************************
#endif // __GN_BASE_VARIANT_H__
