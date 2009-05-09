#ifndef __GN_BASE_AUTOPTR_H__
#define __GN_BASE_AUTOPTR_H__
// *****************************************************************************
/// \file
/// \brief   Common auto pointer classes
/// \author  chenli@@REDMOND (2009.3.31)
// *****************************************************************************

#include <malloc.h>

namespace GN
{
    namespace detail
    {
        ///
        /// Basic auto pointer class. Can NOT be used in STL containers.
        ///
        template<typename T, typename CLASS>
        class BaseAutoPtr : public NoCopy
        {
            T * mPtr;

            typedef BaseAutoPtr<T,CLASS> MyType;

            void release()
            {
                CLASS::sDoRelease(mPtr);
                mPtr = 0;
            }

        public:

            ///
            /// Construct from C-style
            ///
            explicit BaseAutoPtr( T * p ) throw() : mPtr(p) {}

            ///
            /// Destructor
            ///
            ~BaseAutoPtr() { release(); }

            ///
            /// Is pointer empty or not.
            ///
            bool empty() const { return 0 == mPtr; }

            ///
            /// Get internal C-style raw pointer
            ///
            T * cptr() const { return mPtr; }

            ///
            /// clear internal pointer. Same as attach(0)
            ///
            void clear() { if( mPtr ) release(), mPtr = 0; }

            ///
            /// attach to new pointer (release the old one)
            ///
            void attach( T * p )
            {
                if( p != mPtr )
                {
                    release();
                    mPtr = p;
                }
            }

            ///
            /// Release ownership of private pointer
            ///
            T * detach() throw()
            {
                T * tmp = mPtr;
                mPtr = 0;
                return tmp;
            }

            ///
            /// dereference the pointer
            ///
            T & dereference() const { GN_ASSERT( mPtr ); return *mPtr; }

            ///
            /// Convert to T *
            ///
            operator T*() const { return mPtr; }

            ///
            /// dereference operator
            ///
            T & operator*() const { GN_ASSERT( mPtr ); return *mPtr; }

            ///
            /// arrow operator
            ///
            T * operator->() const { return mPtr; }
        };
    }

#if GN_POSIX
    ///
    /// Automatic X resource pointer
    ///
    template<typename T>
    class AutoXPtr : public detail::BaseAutoPtr< T, AutoXPtr<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoXPtr<T> > ParentType;
        friend class detail::BaseAutoPtr< T, AutoXPtr<T> >;

        static void sDoRelease( T * p )
        {
            if( p ) XFree(p);
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoXPtr( T * p = 0 ) throw() : ParentType(p) {}
    };
#endif

    ///
    /// Automatic object pointer. Can NOT be used in STL containers.
    ///
    template<typename T>
    class AutoObjPtr : public detail::BaseAutoPtr< T, AutoObjPtr<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoObjPtr<T> > ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr< T, AutoObjPtr<T> >;
#else
        friend class ParentType;
#endif

        static void sDoRelease( T * p )
        {
            if( p ) delete p;
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoObjPtr( T * p = 0 ) throw() : ParentType(p) {}
    };

    ///
    /// Automatic object array. Can NOT be used in STL containers.
    ///
    template<typename T>
    class AutoObjArray : public detail::BaseAutoPtr< T, AutoObjArray<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoObjArray<T> > ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr< T, AutoObjArray<T> >;
#else
        friend class ParentType;
#endif

        static void sDoRelease( T * p )
        {
            if( p ) delete [] p;
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoObjArray( T * p = 0 ) throw() : ParentType(p) {}
    };

    ///
    /// Automatic C-style array created by heapAlloc. Can NOT be used in STL containers.
    ///
    template<typename T>
    class AutoTypePtr : public detail::BaseAutoPtr< T, AutoTypePtr<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoTypePtr<T> > ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr< T, AutoTypePtr<T> >;
#else
        friend class ParentType;
#endif

        static void sDoRelease( T * p )
        {
            if( p ) heapFree((void*)p);
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoTypePtr( T * p = 0 ) throw() : ParentType(p) {}
    };

    ///
    /// Automatic C-style array created by malloc. Can NOT be used in STL containers.
    ///
    template<typename T>
    class AutoMallocPtr : public detail::BaseAutoPtr< T, AutoMallocPtr<T> >
    {
        typedef detail::BaseAutoPtr< T, AutoMallocPtr<T> > ParentType;
#if GN_GCC
        friend class detail::BaseAutoPtr< T, AutoMallocPtr<T> >;
#else
        friend class ParentType;
#endif

        static void sDoRelease( T * p )
        {
            if( p ) ::free((void*)p);
        }

    public:

        ///
        /// Construct from C-style pointer
        ///
        explicit AutoMallocPtr( T * p = 0 ) throw() : ParentType(p) {}
    };

    ///
    /// Automatic COM pointer class. CAN be used in STL containers.
    ///
    template <class T>
    class AutoComPtr
    {
        T * mPtr;
    public:

        ///
        /// Default constructor
        ///
        AutoComPtr() throw() : mPtr(0) {}

        ///
        /// Construct from normal pointer
        ///
        explicit AutoComPtr( T * p ) throw() : mPtr(p) {}

        ///
        /// Copy constructor
        ///
        AutoComPtr( const AutoComPtr & other ) throw() : mPtr(other.mPtr)
        {
            if( mPtr ) mPtr->AddRef();
        }

        ///
        /// Destructor
        ///
        ~AutoComPtr() throw()
        {
            if (mPtr) mPtr->Release();
        }

        ///
        /// Assignment operator
        ///
        AutoComPtr & operator=( const AutoComPtr & rhs )
        {
            set( rhs.mPtr );
            return *this;
        }

        ///
        /// Convert to T *
        ///
        operator T*() const throw()
        {
            return mPtr;
        }

        ///
        /// dereference operator
        ///
        T & operator*() const throw()
        {
            GN_ASSERT( mPtr );
            return *mPtr;
        }

        ///
        /// Get address of pointer.
        ///
        /// This operator is normally used during resource creation.
        /// This function will ensure that the internal pointer is NULL.
        ///
        T ** operator&() throw()
        {
            GN_ASSERT( NULL == mPtr );
            return &mPtr;
        }

        ///
        /// Get address of pointer.
        ///
        T * const * operator&() const throw()
        {
            GN_ASSERT( mPtr );
            return &mPtr;
        }

        ///
        /// self explain.
        ///
        T * operator->() const throw()
        {
            GN_ASSERT( mPtr );
            return mPtr;
        }

        ///
        /// NOT operator
        ///
        bool operator!() const throw()
        {
            return mPtr == NULL;
        }

        ///
        /// LESS operator
        ///
        bool operator<( T * pT ) const throw()
        {
            return mPtr < pT;
        }

        ///
        /// EQ operator
        ///
        bool operator==( T * pT ) const throw()
        {
            return mPtr == pT;
        }

        ///
        /// Is empty pointer?
        ///
        bool empty() const { return 0 == mPtr; }

        ///
        /// Clear to empty. Same as set(NULL).
        ///
        void clear() { if( mPtr ) mPtr->Release(); mPtr = 0; }

        ///
        /// Release existing interface, then hold new interface
        ///
        void set( T * p ) throw()
        {
            if( p ) p->AddRef();
            if( mPtr ) mPtr->Release();
            mPtr = p;
        }

        ///
        /// Attach to an existing interface (does not AddRef)
        ///
        void attach( T * p2 ) throw()
        {
            if (mPtr) mPtr->Release();
            mPtr = p2;
        }

        ///
        /// Detach the interface (does not Release)
        ///
        T * detach() throw()
        {
            T * pt = mPtr;
            mPtr = NULL;
            return pt;
        }
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_AUTOPTR_H__
