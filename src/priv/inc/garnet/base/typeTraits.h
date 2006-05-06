#ifndef __GN_BASE_TYPETRAITS_H__
#define __GN_BASE_TYPETRAITS_H__
// *****************************************************************************
//! \file    typeTraits.h
//! \brief   Type traits templates
//! \author  chenlee (2005.8.2)
// *****************************************************************************

namespace GN
{
    //! \cond NEVER
    namespace detail
    {
        template <typename T>
        struct IsClassHelper
        {
            template <class U> static long isClassTester(void(U::*)(void));
            template <class U> static char isClassTester(...);

            static const bool value = sizeof(isClassTester<T>(0)) == sizeof(long);
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
            static long isConvertibleTester( typename NonRefToConstRef<TO>::type );
            static char isConvertibleTester(...);
#if GN_ICL
#pragma warning(disable:1595) // non-POD class type passed through ellipsis
#endif
            static const bool value = sizeof(isConvertibleTester(mFrom)) == sizeof(long);
#if GN_ICL
#pragma warning(default:1595)
#endif
        };

        template<typename T> struct GetNumberSign {};
        template<> struct GetNumberSign<int8_t>{ static const bool value = true; };
        template<> struct GetNumberSign<uint8_t>{ static const bool value = false; };
        template<> struct GetNumberSign<int16_t>{ static const bool value = true; };
        template<> struct GetNumberSign<uint16_t>{ static const bool value = false; };
        template<> struct GetNumberSign<int32_t>{ static const bool value = true; };
        template<> struct GetNumberSign<uint32_t>{ static const bool value = false; };
        template<> struct GetNumberSign<int64_t>{ static const bool value = true; };
        template<> struct GetNumberSign<uint64_t>{ static const bool value = false; };
        template<> struct GetNumberSign<float>{ static const bool value = true; };
        template<> struct GetNumberSign<double>{ static const bool value = true; };
    }
    //! \endcond

    //!
    //! Is same type or not?
    //!
    template <typename T1, typename T2>
    struct IsSameType
    {
        static const bool value = false; //!< 如题.
    };

    //!
    //! Is same type or not?
    //!
    template <typename T>
    struct IsSameType<T,T>
    {
        static const bool value = true; //!< 如题.
    };

    //!
    //! Test constness
    //!
    template <typename T>
    struct IsConst
    {
        static const bool value = false; //!< 如题.
    };

    //!
    //! Test constness
    //!
    template <typename T>
    struct IsConst<const T>
    {
        static const bool value = true; //!< 如题.
    };

    //!
    //! Is class type or not?
    //!
    template <typename T>
    struct IsClass
    {
        static const bool value = detail::IsClassHelper<T>::value; //!< 如题.
    };

    //!
    //! Can type 'FROM' convert to type 'TO'
    //!
    template <typename FROM, typename TO>
    struct IsConvertible
    {
        static const bool value = detail::IsConvertibleHelper<FROM,TO>::value; //!< 如题.
    };

    //!
    //! Is class D derived from class B?
    //!
    template <typename B, typename D>
    struct IsBaseAndDerived
    {
        //!
        //! 如题.
        //!
        static const bool value =
            IsClass<B>::value && IsClass<D>::value && IsConvertible<D,B>::value;
    };

    //!
    //! Integer type based on signed/unsigned and bit-width
    //!
    template<typename T> struct SignedType
    {
        static const bool value = detail::GetNumberSign<T>::value; //!< is signed or not?
    };
}

// *****************************************************************************
//                           End of typeTraits.h
// *****************************************************************************
#endif // __GN_BASE_TYPETRAITS_H__
