// *****************************************************************************
//! \file    functor.h
//! \brief   general functor class
//! \author  chenlee (2005.5.14)
// *****************************************************************************
#ifdef GN_FUNCTOR_TEMPL_N

#ifndef GN_JOIN
#define GN_JOIN(s1, s2)         GN_CONCATNATE_DIRECT(s1, s2)
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

#define CLOSUREBASE_NAME     GN_JOIN(ClosureBase,GN_FUNCTOR_TEMPL_N)
#define FREECLOSURE_NAME     GN_JOIN(FreeClosure,GN_FUNCTOR_TEMPL_N)
#define MEMCLOSURE_NAME      GN_JOIN(MemClosure,GN_FUNCTOR_TEMPL_N)
#define CONSTCLOSURE_NAME    GN_JOIN(ConstMemClosure,GN_FUNCTOR_TEMPL_N)
#define FUNCTOR_NAME         GN_JOIN(Functor,GN_FUNCTOR_TEMPL_N)

namespace GN
{
    namespace detail
    {
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

        template<typename R PARAM_COMMA PARAM_TEMPLS>
        class FREECLOSURE_NAME : public CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES>
        {
            typedef CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES> BaseType;
            typedef FREECLOSURE_NAME<R PARAM_COMMA PARAM_TYPES> ThisType;
            typedef R(*FreeFn)(PARAM_TYPES);

            FreeFn mFunc;

        public:

            FREECLOSURE_NAME( const FreeFn & f )
                : BaseType(FREE_FUNC), mFunc(f)
            {
                GN_ASSERT( f );
            }

            virtual bool isEqual( const BaseType & other ) const
            {
                return
                    BaseType::getType() == other.getType() &&
                    mFunc == ((ThisType&)other).mFunc;
            }

            virtual bool isLess( const BaseType & other ) const
            {
                if ( BaseType::getType() != other.getType() ) return BaseType::getType() < other.getType();
                return mFunc < ((ThisType&)other).mFunc;
            }

            virtual R run( PARAM_LIST ) const
            {
                GN_ASSERT(  mFunc );
                return mFunc( PARAM_VALUES );
            }
        };

        template<class X, typename R PARAM_COMMA  PARAM_TEMPLS>
        class MEMCLOSURE_NAME : public CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES>
        {
            typedef CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES> BaseType;
            typedef MEMCLOSURE_NAME<X, R PARAM_COMMA PARAM_TYPES> ThisType;
            typedef R(X::*MemFn)(PARAM_TYPES);

            X *   mClassPtr;
            MemFn mFunc;

        public:

            MEMCLOSURE_NAME( X * x, const MemFn & f )
                : BaseType(MEM_FUNC), mClassPtr(x), mFunc(f)
            {
                GN_ASSERT( x && f );
            }

            virtual bool isEqual( const BaseType & other ) const
            {
                return
                    BaseType::getType() == other.getType() &&
                    mClassPtr == ((ThisType&)other).mClassPtr &&
                    mFunc == ((ThisType&)other).mFunc;
            }

            virtual bool isLess( const BaseType & other ) const
            {
                if ( BaseType::getType() != other.getType() ) return BaseType::getType() < other.getType();
                const ThisType & o = (ThisType&)other;
                if ( mClassPtr != o.mClassPtr ) return mClassPtr < o.mClassPtr;
                return ::memcmp( &mFunc,&o.mFunc,sizeof(mFunc) ) < 0;
            }

            virtual R run( PARAM_LIST ) const
            {
                GN_ASSERT(  mClassPtr && mFunc );
                return (mClassPtr->*mFunc)( PARAM_VALUES );
            }
        };

        template<class X, typename R PARAM_COMMA  PARAM_TEMPLS>
        class CONSTCLOSURE_NAME : public CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES>
        {
            typedef CLOSUREBASE_NAME<R PARAM_COMMA PARAM_TYPES> BaseType;
            typedef CONSTCLOSURE_NAME<X,R PARAM_COMMA PARAM_TYPES> ThisType;
            typedef R(X::*MemFn)(PARAM_TYPES) const;

            const X * mClassPtr;
            MemFn     mFunc;

        public:

            CONSTCLOSURE_NAME( const X * x, const MemFn & f )
                : BaseType(CONST_MEM_FUNC), mClassPtr(x), mFunc(f)
            {
                GN_ASSERT( x && f );
            }

            virtual bool isEqual( const BaseType & other ) const
            {
                return
                    BaseType::getType() == other.getType() &&
                    mClassPtr == ((ThisType&)other).mClassPtr &&
                    mFunc == ((ThisType&)other).mFunc;
            }

            virtual bool isLess( const BaseType & other ) const
            {
                if ( BaseType::getType() != other.getType() ) return BaseType::getType() < other.getType();
                const ThisType & o = (ThisType&)other;
                if ( mClassPtr != o.mClassPtr ) return mClassPtr < o.mClassPtr;
                return ::memcmp( &mFunc,&o.mFunc,sizeof(mFunc) ) < 0;
            }

            virtual R run( PARAM_LIST ) const
            {
                GN_ASSERT(  mClassPtr && mFunc );
                return (mClassPtr->*mFunc)( PARAM_VALUES );
            }
        };
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
        //! destructor
        //!
        ~FUNCTOR_NAME() { clear(); }

        //!
        //! Bind to free(static) function
        //!
        void bind( R(*f)(PARAM_TYPES) )
        {
            if( 0 == f )
            {
                GN_ERROR( "bind to null function pointer!" );
                return;
            }
            clear();
            mClosure = new detail::FREECLOSURE_NAME<R PARAM_COMMA PARAM_TYPES>( f );
        }

        //!
        //! Bind to non-const member function
        //!
        template<class X, class Y>
        inline void bind( Y * x,  R(X::*f)(PARAM_TYPES) )
        {
            if ( 0==x || 0==f )
            {
                GN_ERROR( "bind to null member function pointer!" );
                return;
            }
            clear();
            mClosure = new detail::MEMCLOSURE_NAME<X, R PARAM_COMMA PARAM_TYPES>(x,f);
        }

        //!
        //! Bind to const member function
        //!
        template<class X, class Y>
        inline void bind( const Y * x,  R(X::*f)(PARAM_TYPES) const )
        {
            if ( 0==x || 0==f )
            {
                GN_ERROR( "bind to null member function pointer!" );
                return;
            }
            clear();
            mClosure = new detail::CONSTCLOSURE_NAME<X, R PARAM_COMMA PARAM_TYPES>(x,f);
        }

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

    template<typename R PARAM_COMMA PARAM_TEMPLS>
    inline FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES>
    makeFunctor( R(*f)(PARAM_TYPES) )
    {
        FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES> fn;
        fn.bind(f);
        return fn;
    }

    template<class X, class Y, typename R PARAM_COMMA PARAM_TEMPLS>
    inline FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES>
    makeFunctor( Y * x, R(X::*f)(PARAM_TYPES) )
    {
        GN_CASSERT( !IsConst<Y>::value );
        FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES> fn;
        fn.bind<X,Y>(x,f);
        return fn;
    }

    template<class X, class Y, typename R PARAM_COMMA PARAM_TEMPLS>
    inline FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES>
    makeFunctor( const Y * x, R(X::*f)(PARAM_TYPES) const )
    {
        FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES> fn;
        fn.bind<X,Y>(x,f);
        return fn;
    }
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
#undef CONSTCLOSURE_NAME
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
