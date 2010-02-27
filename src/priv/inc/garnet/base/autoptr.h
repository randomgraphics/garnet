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

            void Release()
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
            ~BaseAutoPtr() { Release(); }

            ///
            /// Is pointer Empty or not.
            ///
            bool Empty() const { return 0 == mPtr; }

            ///
            /// Get internal C-style raw pointer
            ///
            T * ToRawPtr() const { return mPtr; }

            ///
            /// Clear internal pointer. Same as Attach(0)
            ///
            void Clear() { if( mPtr ) Release(), mPtr = 0; }

            ///
            /// Attach to new pointer (Release the old one)
            ///
            void Attach( T * p )
            {
                if( p != mPtr )
                {
                    Release();
                    mPtr = p;
                }
            }

            ///
            /// Release ownership of private pointer
            ///
            T * Detach() throw()
            {
                T * tmp = mPtr;
                mPtr = 0;
                return tmp;
            }

            ///
            /// Dereference the pointer
            ///
            T & Dereference() const { GN_ASSERT( mPtr ); return *mPtr; }

            ///
            /// Convert to T *
            ///
            operator T*() const { return mPtr; }

            ///
            /// Dereference operator
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
    /// Automatic C-style array created by HeapMemory::Alloc. Can NOT be used in STL containers.
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
            if( p ) HeapMemory::Free((void*)p);
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
            Set( rhs.mPtr );
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
        /// Is Empty pointer?
        ///
        bool Empty() const { return 0 == mPtr; }

        ///
        /// Clear to Empty. Same as Set(NULL).
        ///
        void Clear() { if( mPtr ) mPtr->Release(); mPtr = 0; }

        ///
        /// Release existing interface, then hold new interface
        ///
        void Set( T * p ) throw()
        {
            if( p ) p->AddRef();
            if( mPtr ) mPtr->Release();
            mPtr = p;
        }

        ///
        /// Attach to an existing interface (does not AddRef)
        ///
        void Attach( T * p2 ) throw()
        {
            if (mPtr) mPtr->Release();
            mPtr = p2;
        }

        ///
        /// Detach the interface (does not Release)
        ///
        T * Detach() throw()
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
