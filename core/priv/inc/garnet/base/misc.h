#ifndef __GN_BASE_MISC_H__
#define __GN_BASE_MISC_H__
// *****************************************************************************
//! \file    misc.h
//! \brief   misc functions and types
//! \author  chenlee (2005.5.4)
// *****************************************************************************

//!
//! Execute only limited times during the entir life of the
//! application, no matter how many time it is called.
//!
#define GN_DO_LIMITED_TIMES(n,X)      \
    if (true)                         \
    {                                 \
        static size_t s_counter = n;  \
        if( s_counter > 0 )           \
        {                             \
            --s_counter;              \
            X;                        \
        }                             \
    } else void(0)

//!
//! Do something only once. 通常用来在内层循环中输出一些调试和错误信息。
//!
#define GN_DO_ONCE(X) GN_DO_LIMITED_TIMES(1,X)

//!
//! "Use" unused parameter (to avoid compiler warnings)
//!
#define GN_UNUSED_PARAM(X) ((void)(X))

// *************************************************************************
//! \name StdCalss
// *************************************************************************

//@{

namespace GN
{
    //!
    //! type cast function
    //!
    //! perform dynamic cast in debug build, and static cast in release build.
    // ------------------------------------------------------------------------
    template < class TO, class FROM >
    GN_FORCE_INLINE TO safeCast( FROM from )
    {
    #if GN_DEBUG && ( !GN_MSVC || defined(_CPPRTTI) )
        GN_ASSERT( 0 == from || dynamic_cast<TO>(from) );
        return dynamic_cast<TO>(from);
    #else
        return static_cast<TO>(from);
    #endif
    }

    //!
    //! vector to pointer
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE T * vec2ptr( std::vector<T> & vec )
    {
        return vec.empty() ? 0 : &vec[0];
    }

    //!
    //! vector to pointer
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE const T * vec2ptr( const std::vector<T> & vec )
    {
        return vec.empty() ? 0 : &vec[0];
    }

    //!
    //! 将value限定在[vmin, vmax]区间内
    // ------------------------------------------------------------------------
    template < typename T >
    inline void clamp( T & value, const T & vmin, const T & vmax )
    {
        value = vmin > value ? vmin : vmax < value ? vmax : value;
    }

    //!
    //! general safe delLocation routine
    // ------------------------------------------------------------------------
    template < typename T, typename DEALLOC_FUNC >
    GN_FORCE_INLINE void safeDealloc( T * & ptr )
    {
        if( ptr )
        {
            DEALLOC_FUNC( ptr );
            ptr = 0;
        }
    }

    //!
    //! free a C-style pointer
    // ------------------------------------------------------------------------
    GN_FORCE_INLINE void safeMemFree( void * & ptr )
    {
        if( ptr )
        {
            memFree( ptr );
            ptr = 0;
        }
    }

    //!
    //! delete one object
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeDelete( T * & ptr )
    {
        if( ptr )
        {
            delete ptr;
            ptr = 0;
        }
    }

    //!
    //! delete object array
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeDeleteArray( T * & ptr )
    {
        if( ptr )
        {
            delete [] ptr;
            ptr = 0;
        }
    }

    //!
    //! Safe release COM interface
    //!
    template < typename T >
    GN_FORCE_INLINE void safeRelease( T * & ptr )
    {
        if( ptr )
        {
            ptr->Release();
            ptr = 0;
        }
    }

    //!
    //! get environment variable
    //!
    bool getEnv( StrA & result, const char * name );

    //!
    //! Data type with initialized value.
    //!
    template<typename T, T DEFAULT_VALUE>
    class Initialized
    {
        T mValue;

    public:

        //!
        //! Default constructor
        //!
        Initialized() : mValue(DEFAULT_VALUE) {}

        //!
        //! Construct from T
        //!
        Initialized( const T & value ) : mValue(value) {}

        //!
        //! Convert to T
        //!
        operator T & () { return mValue; }

        //!
        //! Convert to constant T
        //!
        operator const T & () const { return mValue; }
    };

    //!
    //! Disable copy semantic of all descendants.
    //!
    class NoCopy
    {
    protected:
        //!
        //! Default constructor
        //!
        NoCopy() {}
        //!
        //! Destructor
        //!
        virtual ~NoCopy() {}
    private:  // emphasize the following members are private
        NoCopy( const NoCopy & );
        const NoCopy & operator = ( const NoCopy& );
    };

    namespace detail
    {
        //!
        //! Basic auto pointer class
        //!
        template<typename T, void(*RELEASE)(T*&)>
        class BaseAutoPtr : public NoCopy
        {
            T * mPtr;

            typedef BaseAutoPtr<T,RELEASE> MyType;

            void release()
            {
                RELEASE(mPtr);
                mPtr = 0;
            }

        public:

            //!
            //! Construct from C-style
            //!
            explicit BaseAutoPtr( T * p ) throw() : mPtr(p) {}

            //!
            //! Destructor
            //!
            ~BaseAutoPtr() { release(); }

            //!
            //! Get internel pointer
            //!
            T * get() const { return mPtr; }

            //!
            //! destroy old pointer and store new pointer
            //!
            void reset( T * p = 0 )
            {
                if( p != mPtr )
                {
                    release();
                    mPtr = p;
                }
            }

            //!
            //! Release ownership of private pointer
            //!
            T * detatch() throw()
            {
                T * tmp = mPtr;
                mPtr = 0;
                return tmp;
            }

            //!
            //! dereference operator
            //!
            T & operator*() const { GN_ASSERT( mPtr ); return *mPtr; }

            //!
            //! arrow operator
            //!
            T * operator->() const { return mPtr; }
        };
    }

    //!
    //! Automatic object pointer
    //!
    template<typename T>
    class AutoObjPtr : public detail::BaseAutoPtr< T, &safeDelete<T> >
    {
        typedef detail::BaseAutoPtr< T, &safeDelete<T> > ParentType;

        static void doRelease( T * p )
        {
            if( p ) delete p;
        }

    public:

        //!
        //! Construct from C-style pointer
        //!
        explicit AutoObjPtr( T * p = 0 ) throw() : ParentType(p) {}
    };

    //!
    //! Automatic object array
    //!
    template<typename T>
    class AutoObjArray : public detail::BaseAutoPtr< T, &safeDeleteArray<T> >
    {
        typedef detail::BaseAutoPtr< T, &safeDeleteArray<T> > ParentType;

        static void doRelease( T * p )
        {
            if( p ) delete [] p;
        }

    public:

        //!
        //! Construct from C-style pointer
        //!
        explicit AutoObjArray( T * p = 0 ) throw() : ParentType(p) {}
    };

    //!
    //! Automatic C-style array created by memAlloc
    //!
    template<typename T>
    class AutoTypePtr : public detail::BaseAutoPtr< T, &safeMemFree >
    {
        typedef detail::BaseAutoPtr< T, &safeMemFree > ParentType;

        static void doRelease( T * p )
        {
            if( p ) memFree(p);
        }

    public:

        //!
        //! Construct from C-style pointer
        //!
        explicit AutoTypePtr( T * p = 0 ) throw() : ParentType(p) {}
    };

    //!
    //! Automatic COM pointer class
    //!
    template <class T>
    class AutoComPtr
    {
        T * mPtr;
    public:

        //!
        //! Default constructor
        //!
        AutoComPtr() throw() : mPtr(0) {}

        //!
        //! Destructor
        //!
        ~AutoComPtr() throw()
        {
            if (mPtr) mPtr->Release();
        }

        //!
        //! Convert to T *
        //!
        operator T*() const throw()
        {
            return mPtr;
        }

        //!
        //! dereference operator
        //!
        T & operator*() const throw()
        {
            GN_ASSERT( mPtr );
            return *mPtr;
        }

        //!
        //! Get address of pointer.
        //!
        //! The assert on operator& usually indicates a bug.  If this is really
        //! what is needed, however, take the address of the mPtr member explicitly.
        //!
        T ** operator&() throw()
        {
            GN_ASSERT(mPtr==NULL);
            return &mPtr;
        }

        //!
        //! self explain.
        //!
        T * operator->() const throw()
        {
            GN_ASSERT( mPtr );
            return mPtr;
        }

        //!
        //! NOT operator
        //!
        bool operator!() const throw()
        {
            return mPtr == NULL;
        }

        //!
        //! LESS operator
        //!
        bool operator<( T * pT ) const throw()
        {
            return mPtr < pT;
        }

        //!
        //! EQ operator
        //!
        bool operator==( T * pT ) const throw()
        {
            return mPtr == pT;
        }

        //!
        //! Is empty pointer?
        //!
        bool empty() const { return 0 == mPtr; }

        //!
        //! Release existing interface, then hold new interface
        //!
        void reset( T * p = 0 ) throw()
        {
            if( p ) p->AddRef();
            if( mPtr ) mPtr->Release();
            mPtr = p;
        }

        //!
        //! Attach to an existing interface (does not AddRef)
        //!
        void attach( T * p2 ) throw()
        {
            if (mPtr) mPtr->Release();
            mPtr = p2;
        }

        //!
        //! Detach the interface (does not Release)
        //!
        T * detach() throw()
        {
            T * pt = mPtr;
            mPtr = NULL;
            return pt;
        }
    };
}

// *****************************************************************************
//                           End of misc.h
// *****************************************************************************
#endif // __GN_BASE_MISC_H__
