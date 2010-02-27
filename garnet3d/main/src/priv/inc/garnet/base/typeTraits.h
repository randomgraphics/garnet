#ifndef __GN_BASE_TYPETRAITS_H__
#define __GN_BASE_TYPETRAITS_H__
// *****************************************************************************
/// \file
/// \brief   Type traits templates
/// \author  chenlee (2005.8.2)
// *****************************************************************************

namespace GN
{
    /// \cond NEVER
    namespace detail
    {
        template <typename T>
        struct IsClassHelper
        {
            template <class U> static long IsClassTester(void(U::*)(void));
            template <class U> static char IsClassTester(...);

            static const bool value = sizeof(IsClassTester<T>(0)) == sizeof(long);
        };

        template <typename FROM,typename TO>
        struct IsConvertibleHelper
        {
            template <typename T> struct NonRefToConstRef { typedef const T & type; };
            template <typename T> struct NonRefToConstRef<T&> { typedef T & type; };
            template <typename T> struct NonRefToConstRef<const T> { typedef const T & type; };
            template <typename T> struct NonRefToConstRef<const T&> { typedef const T & type; };

            template <typename T> struct AddRef { typedef T & type; };
            template <typename T> struct AddRef<T&> {typedef T & type; };

            static typename AddRef<FROM>::type mFrom;
            static long IsConvertibleTester( typename NonRefToConstRef<TO>::type );
            static char IsConvertibleTester(...);
#if GN_ICL
#pragma warning(disable:1595) // non-POD class type passed through ellipsis
#endif
            static const bool value = sizeof(IsConvertibleTester(mFrom)) == sizeof(long);
#if GN_ICL
#pragma warning(default:1595)
#endif
        };

        template<typename T> struct NumericProperties {};
        template<> struct NumericProperties<SInt8>{ static const bool isSigned = true; };
        template<> struct NumericProperties<UInt8>{ static const bool isSigned = false; };
        template<> struct NumericProperties<SInt16>{ static const bool isSigned = true; };
        template<> struct NumericProperties<UInt16>{ static const bool isSigned = false; };
        template<> struct NumericProperties<SInt32>{ static const bool isSigned = true; };
        template<> struct NumericProperties<UInt32>{ static const bool isSigned = false; };
        template<> struct NumericProperties<SInt64>{ static const bool isSigned = true; };
        template<> struct NumericProperties<UInt64>{ static const bool isSigned = false; };
        template<> struct NumericProperties<float>{ static const bool isSigned = true; };
        template<> struct NumericProperties<double>{ static const bool isSigned = true; };
    }
    /// \endcond

    ///
    /// Is same type or not?
    ///
    template <typename T1, typename T2>
    struct IsSameType
    {
        static const bool value = false; ///< 如题.
    };

    ///
    /// Is same type or not?
    ///
    template <typename T>
    struct IsSameType<T,T>
    {
        static const bool value = true; ///< 如题.
    };

    ///
    /// Test constness
    ///
    template <typename T>
    struct IsConst
    {
        static const bool value = false; ///< 如题.
    };

    ///
    /// Test constness
    ///
    template <typename T>
    struct IsConst<const T>
    {
        static const bool value = true; ///< 如题.
    };

    ///
    /// Is class type or not?
    ///
    template <typename T>
    struct IsClass
    {
        static const bool value = detail::IsClassHelper<T>::value; ///< 如题.
    };

    ///
    /// Can type 'FROM' convert to type 'TO'
    ///
    template <typename FROM, typename TO>
    struct IsConvertible
    {
        static const bool value = detail::IsConvertibleHelper<FROM,TO>::value; ///< 如题.
    };

    ///
    /// Is class D derived from class B?
    ///
    template <typename B, typename D>
    struct IsBaseAndDerived
    {
        ///
        /// 如题.
        ///
        static const bool value =
            IsClass<B>::value && IsClass<D>::value && IsConvertible<D,B>::value;
    };

    ///
    /// Integer type based on signed/unsigned and bit-width
    ///
    template<typename T> struct SignedType
    {
        static const bool value = detail::NumericProperties<T>::isSigned; ///< is signed or not?
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_TYPETRAITS_H__
