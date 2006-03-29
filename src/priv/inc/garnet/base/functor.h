// *****************************************************************************
//! \file    functor.h
//! \brief   general functor class
//! \author  chenlee (2005.5.14)
// *****************************************************************************
#ifdef GN_FUNCTOR_TEMPL_N

#ifndef GN_JOIN
#define GN_JOIN(s1, s2)               GN_CONCATNATE_DIRECT(s1, s2)
#define GN_JOIN3(s1, s2, s3)          GN_JOIN( GN_JOIN( s1, s2 ), s3 )
#define GN_JOIN4(s1, s2, s3, s4)      GN_JOIN( GN_JOIN3( s1, s2, s3 ), s4 )
#define GN_CONCATNATE_DIRECT(s1, s2)  s1##s2
#endif

#define PARAM_TEMPLS_0
#define PARAM_TEMPLS_1 class Param1
#define PARAM_TEMPLS_2 PARAM_TEMPLS_1, class Param2
#define PARAM_TEMPLS_3 PARAM_TEMPLS_2, class Param3
#define PARAM_TEMPLS_4 PARAM_TEMPLS_3, class Param4
#define PARAM_TEMPLS_5 PARAM_TEMPLS_4, class Param5
#define PARAM_TEMPLS_6 PARAM_TEMPLS_5, class Param6
#define PARAM_TEMPLS_7 PARAM_TEMPLS_6, class Param7
#define PARAM_TEMPLS_8 PARAM_TEMPLS_7, class Param8
#define PARAM_TEMPLS_9 PARAM_TEMPLS_8, class Param9
#define PARAM_TEMPLS   GN_JOIN( PARAM_TEMPLS_, GN_FUNCTOR_TEMPL_N)

#define PARAM_TYPES_0
#define PARAM_TYPES_1 Param1
#define PARAM_TYPES_2 PARAM_TYPES_1, Param2
#define PARAM_TYPES_3 PARAM_TYPES_2, Param3
#define PARAM_TYPES_4 PARAM_TYPES_3, Param4
#define PARAM_TYPES_5 PARAM_TYPES_4, Param5
#define PARAM_TYPES_6 PARAM_TYPES_5, Param6
#define PARAM_TYPES_7 PARAM_TYPES_6, Param7
#define PARAM_TYPES_8 PARAM_TYPES_7, Param8
#define PARAM_TYPES_9 PARAM_TYPES_8, Param9
#define PARAM_TYPES   GN_JOIN( PARAM_TYPES_, GN_FUNCTOR_TEMPL_N)

#define PARAM_VALUES_0
#define PARAM_VALUES_1 p1
#define PARAM_VALUES_2 PARAM_VALUES_1, p2
#define PARAM_VALUES_3 PARAM_VALUES_2, p3
#define PARAM_VALUES_4 PARAM_VALUES_3, p4
#define PARAM_VALUES_5 PARAM_VALUES_4, p5
#define PARAM_VALUES_6 PARAM_VALUES_5, p6
#define PARAM_VALUES_7 PARAM_VALUES_6, p7
#define PARAM_VALUES_8 PARAM_VALUES_7, p8
#define PARAM_VALUES_9 PARAM_VALUES_8, p9
#define PARAM_VALUES   GN_JOIN( PARAM_VALUES_, GN_FUNCTOR_TEMPL_N)

#define PARAM_LIST_0
#define PARAM_LIST_1 Param1 p1
#define PARAM_LIST_2 PARAM_LIST_1, Param2 p2
#define PARAM_LIST_3 PARAM_LIST_2, Param3 p3
#define PARAM_LIST_4 PARAM_LIST_3, Param4 p4
#define PARAM_LIST_5 PARAM_LIST_4, Param5 p5
#define PARAM_LIST_6 PARAM_LIST_5, Param6 p6
#define PARAM_LIST_7 PARAM_LIST_6, Param7 p7
#define PARAM_LIST_8 PARAM_LIST_7, Param8 p8
#define PARAM_LIST_9 PARAM_LIST_8, Param9 p9
#define PARAM_LIST   GN_JOIN( PARAM_LIST_, GN_FUNCTOR_TEMPL_N)

#define PARAM_COMMA_0
#define PARAM_COMMA_1 ,
#define PARAM_COMMA_2 ,
#define PARAM_COMMA_3 ,
#define PARAM_COMMA_4 ,
#define PARAM_COMMA_5 ,
#define PARAM_COMMA_6 ,
#define PARAM_COMMA_7 ,
#define PARAM_COMMA_8 ,
#define PARAM_COMMA_9 ,
#define PARAM_COMMA   GN_JOIN( PARAM_COMMA_, GN_FUNCTOR_TEMPL_N)

#define METHODPTR_NAME                                GN_JOIN( FunctorMethodPtr, GN_FUNCTOR_TEMPL_N )
#define CLOSUREBASE_NAME                              GN_JOIN( ClosureBase, GN_FUNCTOR_TEMPL_N )
#define FREECLOSURE_NAME( CALL_CONVENSION, DUMMY )    GN_JOIN3( FreeClosure, CALL_CONVENSION, GN_FUNCTOR_TEMPL_N )
#define MEMCLOSURE_NAME( CALL_CONVENSION, CONSTNESS ) GN_JOIN4( MemClosure, CALL_CONVENSION, CONSTNESS, GN_FUNCTOR_TEMPL_N )
#define FUNCTOR_NAME                                  GN_JOIN( Functor, GN_FUNCTOR_TEMPL_N )

namespace GN
{
    namespace detail
    {
        template <typename R PARAM_COMMA PARAM_TEMPLS, class C>
        struct METHODPTR_NAME
        {
            typedef R ( GN_FASTCALL C::*FP_fastcall)( PARAM_TYPES );
            typedef R ( GN_STDCALL  C::*FP_stdcall)( PARAM_TYPES );
            typedef R ( GN_THISCALL C::*FP_thiscall)( PARAM_TYPES );
            typedef R ( GN_CDECL    C::*FP_cdecl)( PARAM_TYPES );
        };

        template <typename R PARAM_COMMA PARAM_TEMPLS, class C>
        struct METHODPTR_NAME<R PARAM_COMMA PARAM_TYPES, const C>
        {
            typedef R ( GN_FASTCALL C::*FP_fastcall)( PARAM_TYPES ) const;
            typedef R ( GN_STDCALL  C::*FP_stdcall)( PARAM_TYPES ) const;
            typedef R ( GN_THISCALL C::*FP_thiscall)( PARAM_TYPES ) const;
            typedef R ( GN_CDECL    C::*FP_cdecl)( PARAM_TYPES );
        };

        template <typename R PARAM_COMMA PARAM_TEMPLS>
        class CLOSUREBASE_NAME
        {
        private:

            size_t mRef;

            ClosureType mType;

            // make this class uncopyable
            CLOSUREBASE_NAME( const CLOSUREBASE_NAME & );

            CLOSUREBASE_NAME & operator=( const CLOSUREBASE_NAME & );

        protected:

            CLOSUREBASE_NAME( ClosureType type ) : mRef(1), mType(type) {}

            virtual ~CLOSUREBASE_NAME() { GN_ASSERT(0==mRef); }

        public:

            void addref() { ++mRef; }

            void release() { if( 0 == (--mRef) ) delete this; }

            ClosureType getType() const { return mType; }

            virtual bool isEqual( const CLOSUREBASE_NAME & other ) const = 0;

            virtual bool isLess( const CLOSUREBASE_NAME & other ) const = 0;

            virtual R run(PARAM_LIST) const = 0;
        };

#define DEFINE_FREECLOSURE( CALL_CONVENSION, DUMMY )                                                                    \
        template<typename R PARAM_COMMA PARAM_TEMPLS>                                                                   \
        class FREECLOSURE_NAME(CALL_CONVENSION,) : public CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES>                   \
        {                                                                                                               \
            typedef CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES> BaseType;                                               \
            typedef FREECLOSURE_NAME(CALL_CONVENSION,)<R PARAM_COMMA PARAM_TYPES> ThisType;                             \
            typedef R(CALL_CONVENSION*FreeFn)(PARAM_TYPES);                                                             \
                                                                                                                        \
            FreeFn mFunc;                                                                                               \
                                                                                                                        \
        public:                                                                                                         \
                                                                                                                        \
            FREECLOSURE_NAME(CALL_CONVENSION,)( const FreeFn & f )                                                      \
                : BaseType(FREE_FUNC), mFunc(f)                                                                         \
            {                                                                                                           \
                GN_ASSERT( f );                                                                                         \
            }                                                                                                           \
                                                                                                                        \
            virtual bool isEqual( const BaseType & other ) const                                                        \
            {                                                                                                           \
                return                                                                                                  \
                    BaseType::getType() == other.getType() &&                                                           \
                    mFunc == ((ThisType&)other).mFunc;                                                                  \
            }                                                                                                           \
                                                                                                                        \
            virtual bool isLess( const BaseType & other ) const                                                         \
            {                                                                                                           \
                if ( BaseType::getType() != other.getType() ) return BaseType::getType() < other.getType();             \
                return mFunc < ((ThisType&)other).mFunc;                                                                \
            }                                                                                                           \
                                                                                                                        \
            virtual R run( PARAM_LIST ) const                                                                           \
            {                                                                                                           \
                GN_ASSERT(  mFunc );                                                                                    \
                return mFunc( PARAM_VALUES );                                                                           \
            }                                                                                                           \
        }

#if GN_HAS_FASTCALL
        DEFINE_FREECLOSURE( GN_FASTCALL, );
#endif
#if GN_HAS_STDCALL
        DEFINE_FREECLOSURE( GN_STDCALL, );
#endif
#if GN_HAS_CDECL
        DEFINE_FREECLOSURE( GN_CDECL, );
#endif

#define DEFINE_MEMCLOSURE( CALL_CONVENSION, CONSTNESS )                                                                 \
        template<class X, typename R PARAM_COMMA  PARAM_TEMPLS>                                                         \
        class MEMCLOSURE_NAME(CALL_CONVENSION,CONSTNESS) : public CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES>           \
        {                                                                                                               \
            typedef CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES> BaseType;                                               \
            typedef MEMCLOSURE_NAME(CALL_CONVENSION,CONSTNESS)<X, R PARAM_COMMA PARAM_TYPES> ThisType;                  \
            typedef R(CALL_CONVENSION X::*MemFn)(PARAM_TYPES) CONSTNESS;                                                \
                                                                                                                        \
            CONSTNESS X & mClassRef;                                                                                    \
            MemFn mFunc;                                                                                                \
                                                                                                                        \
        public:                                                                                                         \
                                                                                                                        \
            MEMCLOSURE_NAME(CALL_CONVENSION,CONSTNESS)( CONSTNESS X & x, const MemFn & f )                              \
                : BaseType(MEM_FUNC), mClassRef(x), mFunc(f)                                                            \
            {                                                                                                           \
                GN_ASSERT( f );                                                                                         \
            }                                                                                                           \
                                                                                                                        \
            virtual bool isEqual( const BaseType & other ) const                                                        \
            {                                                                                                           \
                return                                                                                                  \
                    BaseType::getType() == other.getType() &&                                                           \
                    &mClassRef == &((ThisType&)other).mClassRef &&                                                      \
                    mFunc == ((ThisType&)other).mFunc;                                                                  \
            }                                                                                                           \
                                                                                                                        \
            virtual bool isLess( const BaseType & other ) const                                                         \
            {                                                                                                           \
                if ( BaseType::getType() != other.getType() ) return BaseType::getType() < other.getType();             \
                const ThisType & o = (ThisType&)other;                                                                  \
                if ( &mClassRef != &o.mClassRef ) return &mClassRef < &o.mClassRef;                                     \
                return ::memcmp( &mFunc,&o.mFunc,sizeof(mFunc) ) < 0;                                                   \
            }                                                                                                           \
                                                                                                                        \
            virtual R run( PARAM_LIST ) const                                                                           \
            {                                                                                                           \
                GN_ASSERT(  mFunc );                                                                                    \
                return (mClassRef.*mFunc)( PARAM_VALUES );                                                              \
            }                                                                                                           \
        }

#if GN_HAS_FASTCALL
        DEFINE_MEMCLOSURE( GN_FASTCALL, );
        DEFINE_MEMCLOSURE( GN_FASTCALL, const );
#endif
#if GN_HAS_STDCALL
        DEFINE_MEMCLOSURE( GN_STDCALL, );
        DEFINE_MEMCLOSURE( GN_STDCALL, const );
#endif
#if GN_HAS_THISCALL
        DEFINE_MEMCLOSURE( GN_THISCALL, );
        DEFINE_MEMCLOSURE( GN_THISCALL, const );
#endif
#if GN_HAS_CDECL
        DEFINE_MEMCLOSURE( GN_CDECL, );
        DEFINE_MEMCLOSURE( GN_CDECL, const );
#endif
    }

    //!
    //! General functor class
    //!
    template<typename R PARAM_COMMA  PARAM_TEMPLS>
    class FUNCTOR_NAME
    {
        typedef FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES> MyType;
        typedef R(*FreeFn)(PARAM_TYPES);
        typedef detail::CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES> ClosureBase;

        ClosureBase * mClosure;

    public:

        //!
        //! default constructor
        //!
        FUNCTOR_NAME() : mClosure(0) {}

        //!
        //! copy constructor
        //!
        FUNCTOR_NAME( const MyType & other )
            : mClosure(other.mClosure)
        {
            if( mClosure ) mClosure->addref();
        }

        //!
        //! construct from free function
        //!
        FUNCTOR_NAME( R(*f)(PARAM_TYPES) )
            : mClosure(0)
        {
            bind( f );
        }

        //!
        //! destructor
        //!
        ~FUNCTOR_NAME() { clear(); }

#define BIND_TO_FREEFUNC( CALL_CONVENSION, DUMMY )                                                                 \
        void bind( R(CALL_CONVENSION *f)(PARAM_TYPES) )                                                            \
        {                                                                                                          \
            if( 0 == f )                                                                                           \
            {                                                                                                      \
                GN_ERROR( "bind to null function pointer!" );                                                      \
                return;                                                                                            \
            }                                                                                                      \
            clear();                                                                                               \
            mClosure = new detail::FREECLOSURE_NAME(CALL_CONVENSION,DUMMY)<R PARAM_COMMA PARAM_TYPES>( f );        \
        }

        //! \name Bind to free(static) function
        //@{
#if GN_HAS_FASTCALL
        BIND_TO_FREEFUNC( GN_FASTCALL, )
#endif
#if GN_HAS_STDCALL
        BIND_TO_FREEFUNC( GN_STDCALL, )
#endif
#if GN_HAS_CDECL
        BIND_TO_FREEFUNC( GN_CDECL, )
#endif
        //@}

#define BIND_TO_MEMFUNC( CALL_CONVENSION, CONSTNESS )                                                             \
        template<class X, class Y>                                                                                \
        inline void bind( CONSTNESS Y & x,  R( CALL_CONVENSION X::*f)(PARAM_TYPES) CONSTNESS )                    \
        {                                                                                                         \
            if ( 0==f )                                                                                           \
            {                                                                                                     \
                GN_ERROR( "bind to null member function pointer!" );                                              \
                return;                                                                                           \
            }                                                                                                     \
            clear();                                                                                              \
            mClosure = new detail::MEMCLOSURE_NAME(CALL_CONVENSION,CONSTNESS)<X, R PARAM_COMMA PARAM_TYPES>(x,f); \
        }

        //! \name Bind to member function
        //@{
#if GN_HAS_FASTCALL
        BIND_TO_MEMFUNC( GN_FASTCALL, )
        BIND_TO_MEMFUNC( GN_FASTCALL, const )
#endif
#if GN_HAS_STDCALL
        BIND_TO_MEMFUNC( GN_STDCALL, )
        BIND_TO_MEMFUNC( GN_STDCALL, const )
#endif
#if GN_HAS_THISCALL
        BIND_TO_MEMFUNC( GN_THISCALL, )
        BIND_TO_MEMFUNC( GN_THISCALL, const )
#endif
#if GN_HAS_CDECL
        BIND_TO_MEMFUNC( GN_CDECL, )
        BIND_TO_MEMFUNC( GN_CDECL, const )
#endif
        //@}

        //!
        //! clear the functor
        //!
        void clear()
        {
            if( mClosure ) mClosure->release(), mClosure = 0;
        }

        //!
        //! Check emptiness of the functor
        //!
        bool empty() const
        {
            return 0 == mClosure;
        }

        //!
        //! Convert to boolean
        //!
        operator bool() const { return 0 != mClosure; }

        //!
        //! call embbed function pointer
        //!
        R operator()( PARAM_LIST ) const
        {
            GN_ASSERT( !empty() );
            return mClosure->run( PARAM_VALUES );
        }

        //!
        //! copy operator
        //!
        MyType & operator=( const MyType & other )
        {
            if( mClosure ) mClosure->release();
            mClosure = other.mClosure;
            if( mClosure ) mClosure->addref();
            return *this;
        }

        //!
        //! check equality
        //!
        bool operator==( const MyType & rhs ) const
        {
            if ( 0 == mClosure )
            {
                return 0 == rhs.mClosure;
            }
            else if ( 0 == rhs.mClosure )
            {
                return false;
            }
            else
            {
                return mClosure->isEqual(*rhs.mClosure);
            }
        }

        //!
        //! check equality
        //!
        bool operator!=( const MyType & rhs ) const
        {
            if ( 0 == mClosure )
            {
                return 0 != rhs.mClosure;
            }
            else if ( 0 == rhs.mClosure )
            {
                return false;
            }
            else
            {
                return !mClosure->isEqual(*rhs.mClosure);
            }
        }

        //!
        //! check equality
        //!
        bool operator<( const MyType & rhs ) const
        {
            if ( 0 == mClosure )
            {
                return 0 != rhs.mClosure;
            }
            else if ( 0 == rhs.mClosure )
            {
                return false;
            }
            else
            {
                return mClosure->isLess(*rhs.mClosure);
            }
        }

        //!
        //! check equality
        //!
        bool operator>( const MyType & rhs ) const
        {
            if ( 0 == mClosure )
            {
                return false;
            }
            else if ( 0 == rhs.mClosure )
            {
                return true;
            }
            else
            {
                return rhs.mClosure->isLess(*mClosure);
            }
        }
    };

#define MAKE_FREEFUNCTOR( CALL_CONVENSION, DUMMY )        \
    template<typename R PARAM_COMMA PARAM_TEMPLS>         \
    inline FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES>        \
    makeFunctor( R(CALL_CONVENSION *f)(PARAM_TYPES) )     \
    {                                                     \
        FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES> fn;       \
        fn.bind(f);                                       \
        return fn;                                        \
    }                                                     \

    //! \name make free functor
    //@{
#if GN_HAS_FASTCALL
    MAKE_FREEFUNCTOR( GN_FASTCALL, )
#endif
#if GN_HAS_STDCALL
    MAKE_FREEFUNCTOR( GN_STDCALL, )
#endif
#if GN_HAS_CDECL
    MAKE_FREEFUNCTOR( GN_CDECL, )
#endif
    //@}

#define MAKE_MEMFUNCTOR( CALL_CONVENSION, CONSTNESS )                                \
    template<class X, class Y, typename R PARAM_COMMA PARAM_TEMPLS>                  \
    inline FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES>                                   \
    makeFunctor( CONSTNESS Y & x, R( CALL_CONVENSION X::*f)(PARAM_TYPES) CONSTNESS ) \
    {                                                                                \
        GN_CASSERT( !IsConst<Y>::value );                                            \
        FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES> fn;                                  \
        fn.bind(x,f);                                                                \
        return fn;                                                                   \
    }

    //! \name make member functor
    //@{
#if GN_HAS_FASTCALL
    MAKE_MEMFUNCTOR( GN_FASTCALL, )
    MAKE_MEMFUNCTOR( GN_FASTCALL, const )
#endif
#if GN_HAS_STDCALL
    MAKE_MEMFUNCTOR( GN_STDCALL, )
    MAKE_MEMFUNCTOR( GN_STDCALL, const )
#endif
#if GN_HAS_THISCALL
    MAKE_MEMFUNCTOR( GN_THISCALL, )
    MAKE_MEMFUNCTOR( GN_THISCALL, const )
#endif
#if GN_HAS_CDECL
    MAKE_MEMFUNCTOR( GN_CDECL, )
    MAKE_MEMFUNCTOR( GN_CDECL, const )
#endif
    //@}

}

#undef PARAM_TEMPLS_0
#undef PARAM_TEMPLS_1
#undef PARAM_TEMPLS_2
#undef PARAM_TEMPLS_3
#undef PARAM_TEMPLS_4
#undef PARAM_TEMPLS_5
#undef PARAM_TEMPLS_6
#undef PARAM_TEMPLS_7
#undef PARAM_TEMPLS_8
#undef PARAM_TEMPLS_9
#undef PARAM_TEMPLS

#undef PARAM_TYPES_0
#undef PARAM_TYPES_1
#undef PARAM_TYPES_2
#undef PARAM_TYPES_3
#undef PARAM_TYPES_4
#undef PARAM_TYPES_5
#undef PARAM_TYPES_6
#undef PARAM_TYPES_7
#undef PARAM_TYPES_8
#undef PARAM_TYPES_9
#undef PARAM_TYPES

#undef PARAM_VALUES_0
#undef PARAM_VALUES_1
#undef PARAM_VALUES_2
#undef PARAM_VALUES_3
#undef PARAM_VALUES_4
#undef PARAM_VALUES_5
#undef PARAM_VALUES_6
#undef PARAM_VALUES_7
#undef PARAM_VALUES_8
#undef PARAM_VALUES_9
#undef PARAM_VALUES

#undef PARAM_LIST_0
#undef PARAM_LIST_1
#undef PARAM_LIST_2
#undef PARAM_LIST_3
#undef PARAM_LIST_4
#undef PARAM_LIST_5
#undef PARAM_LIST_6
#undef PARAM_LIST_7
#undef PARAM_LIST_8
#undef PARAM_LIST_9
#undef PARAM_LIST

#undef PARAM_COMMA_0
#undef PARAM_COMMA_1
#undef PARAM_COMMA_2
#undef PARAM_COMMA_3
#undef PARAM_COMMA_4
#undef PARAM_COMMA_5
#undef PARAM_COMMA_6
#undef PARAM_COMMA_7
#undef PARAM_COMMA_8
#undef PARAM_COMMA_9
#undef PARAM_COMMA

#undef CLOSUREBASE_NAME
#undef FREECLOSURE_NAME
#undef MEMCLOSURE_NAME
#undef FUNCTOR_NAME

#undef GN_FUNCTOR_TEMPL_N

#elif !defined(__GN_BASE_FUNCTOR_H__)
#define __GN_BASE_FUNCTOR_H__ //!< Including protector for functor.h

namespace GN
{
    namespace detail
    {
        //!
        //! Private type indicating functor type
        //!
        enum ClosureType
        {
            FREE_FUNC,      //!< Normal global/static function
            MEM_FUNC,       //!< non-const member function
            CONST_MEM_FUNC, //!< const member function
        };
    }
}

//!
//! Functor parameter count
//!
#define GN_FUNCTOR_TEMPL_N 0
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 1
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 2
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 3
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 4
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 5
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 6
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 7
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 8
#include "functor.h"

#define GN_FUNCTOR_TEMPL_N 9
#include "functor.h"

// *****************************************************************************
//                           End of functor.h
// *****************************************************************************
#endif // __GN_BASE_FUNCTOR_H__
