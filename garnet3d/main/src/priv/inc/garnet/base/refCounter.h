#ifndef __GN_BASE_REFCOUNTER_H__
#define __GN_BASE_REFCOUNTER_H__
// *****************************************************************************
/// \file
/// \brief   Reference counting classes
/// \author  chenlee (2005.8.13)
// *****************************************************************************

namespace GN
{
    ///
    /// 引用计数器
    ///
    /// 提供基本的引用技术功能，用于和 AutoRef 一起实现自动指针类
    ///
    /// \note
    /// 在使用 RefCounter 时，为保证系统的正常运作，有几条规则必须遵守：
    ///     - 尽可能的使用 AutoRef 来管理 RefCounter 类的实例
    ///     - 如果有相互引用的现象存在（不管是直接的还是间接的），在释放对象前
    ///       必须首先打开引用环，否则将造成内存泄漏，环中的变量将永远无法释放
    ///     - 就这些吧，其它的以后想到再说....
    ///
    class RefCounter : public NoCopy
    {
        // ********************************
        //       reference management
        // ********************************
    public :

        ///
        /// increase reference counter
        ///
        SInt32 IncRef() const  throw() { return AtomInc32(&mRef); }

        ///
        /// decrease reference counter, delete the object, if reference count reaches zero.
        ///
        SInt32 DecRef() const
        {
            GN_ASSERT( mRef > 0 );

            SInt32 ref = AtomDec32( &mRef ) ;

            if( 0 == ref )
            {
                // delete itself
                delete this;
            }

            return ref;
        }

        ///
        /// get current reference counter value
        ///
        SInt32 GetRef() const throw() { return AtomGet32(&mRef); }

        // ********************************
        //    weak reference management
        // ********************************
    public :

        std::list<void*>::iterator _AddWeakRef( void * p ) const
        {
            return mWeakRefList.insert( mWeakRefList.end(), p );
        }

        void _RemoveWeakRef( const std::list<void*>::iterator & iter ) const
        {
            mWeakRefList.erase( iter );
        }

        // ********************************
        /// \name protective ctor/dtor
        // ********************************
    protected:

        //@{

        ///
        /// Constructor
        ///
        RefCounter() : mRef(1) {}

        ///
        /// Destructor
        ///
        virtual ~RefCounter()
        {
            if( 0 != mRef )
            {
                GN_UNEXPECTED_EX( "Destructing reference counted object with non-zero reference counter usually means memory corruption, thus is not allowed!" );
            }

            // NULL all weak references
            while( !mWeakRefList.empty() )
            {
                NullifyWeakRef( *mWeakRefList.begin() );
            }
            GN_ASSERT( mWeakRefList.empty() );
        }

        //@}

        // ********************************
        // private methods
        // ********************************
    private:

        static inline void NullifyWeakRef( void * );

        // ********************************
        // private data members
        // ********************************
    private:

        ///
        /// reference counter
        ///
        mutable volatile SInt32 mRef;

        mutable std::list<void*> mWeakRefList;
    };

    ///
    /// 和 RefCounter 配合使用的自动指针类
    ///
    template <class X, class MUTEX=SingleThreadMutex> class AutoRef
    {
        ///
        /// pointer to class X
        ///
        typedef X * XPTR;

        ///
        /// reference to class X
        ///
        typedef X & XREF;

    private:

        XPTR  mPtr; ///< internal pointer

    public :

        ///
        /// Instance of Empty/null reference pointer
        ///
        static AutoRef<X,MUTEX> NULLREF;

        ///
        /// construct from a normal pointer
        ///
        explicit AutoRef( XPTR p = 0 ) throw() : mPtr(p)
        {
            // make sure sizeof(AutoRef) == sizeof(XPTR), which ensures that an array of autoref
            // can always be used as array of native pointer.
            GN_CASSERT( sizeof(AutoRef) == sizeof(XPTR) );
        }

        ///
        /// copy constructor
        ///
        AutoRef( const AutoRef & p ) throw() : mPtr( p )
        {
            if(mPtr) mPtr->IncRef();
        }

        ///
        /// copy constructor
        ///
        template <class Y,class MUTEX2>
        AutoRef( const AutoRef<Y,MUTEX2> & p ) throw() : mPtr( p )
        {
            if(mPtr) mPtr->IncRef();
        }

        ///
        /// destructor
        ///
        ~AutoRef()
        {
            if(mPtr) mPtr->DecRef();
        }

        ///
        /// 赋值语句
        ///
        AutoRef & operator = ( const AutoRef & rhs )
        {
            Set( rhs );
            return *this;
        }

        ///
        /// 赋值语句
        ///
        template <class Y,class MUTEX2>
        AutoRef & operator = ( const AutoRef<Y,MUTEX2> & rhs )
        {
            Set( rhs );
            return *this;
        }

        ///
        /// Convert to XPTR
        ///
        operator XPTR () const { return mPtr; }

        ///
        /// 比较操作
        ///
        bool operator == ( const AutoRef & rhs ) const throw()
        {
            return mPtr == rhs.mPtr;
        }

        ///
        /// 比较操作
        ///
        bool operator != ( const AutoRef & rhs ) const throw()
        {
            return mPtr != rhs.mPtr;
        }

        ///
        /// 比较操作
        ///
        bool operator < ( const AutoRef & rhs ) const throw()
        {
            return mPtr < rhs.mPtr;
        }

        ///
        /// NOT operator
        ///
        bool operator !() const throw() { return !mPtr; }

        ///
        /// dereference operator.
        ///
        /// TODO: is this thread safe?
        ///
        XREF operator *() const throw()  { GN_ASSERT(mPtr); return *mPtr; }

        ///
        /// arrow operator
        ///
        XPTR operator->() const throw()  { GN_ASSERT(mPtr); return  mPtr; }

        ///
        /// Get internal pointer
        ///
        XPTR Get() const throw() { return mPtr; }

        ///
        /// Get address of internal pointer.
        ///
        /// 这个函数主要用于将指向AutoRef的指针变成指向XPTR的指针（他们在内存中的映像其实是一样的）。
        /// 你可以用强制类型转换达同样的目的，不过用这个函数会更简洁一些。
        ///
        XPTR const * Addr() const throw() { return &mPtr; }

        ///
        /// return true if no pointer is currently being hold
        ///
        bool Empty() const throw()
        {
            return 0 == mPtr;
        }

        ///
        /// Clear to Empty. Same as Set(NULL).
        ///
        void Clear()
        {
            MUTEX m;
            m.Lock();

            if( mPtr ) mPtr->DecRef(); mPtr = 0;

            m.Unlock();
        }

        ///
        /// Set new pointer data
        ///
        /// this function will release old pointer, if not NULL; then increase
        /// the reference counter of new pointer, if not NULL.
        ///
        void Set( XPTR p )
        {
            MUTEX m;
            m.Lock();

            if( p ) p->IncRef();
            if( mPtr ) mPtr->DecRef();
            mPtr = p;

            m.Unlock();
        }

        ///
        /// acquire ownership of a XPTR
        ///
        /// this function will not modify pointer's refcount
        ///
        void Attach( XPTR ptr )
        {
            if( ptr == mPtr ) return;

            MUTEX m;
            m.Lock();

            if( mPtr ) mPtr->DecRef();
            mPtr = ptr;

            m.Unlock();
        }

        ///
        /// release ownership of internal XPTR
        ///
        /// this function will not modify pointer's refcount
        ///
        XPTR Detach() throw()
        {
            MUTEX m;
            m.Lock();

            XPTR tmp = mPtr;
            mPtr = 0;

            m.Unlock();

            return tmp;
        }
    };

    template<typename X,typename M> AutoRef<X,M> AutoRef<X,M>::NULLREF;

    namespace detail
    {
        ///
        /// base class of weak reference
        ///
        class WeakRefBase
        {
        protected:

            const RefCounter         * mPtr;
            std::list<void*>::iterator mIter;

        public:

            /// constructor
            WeakRefBase( const RefCounter * ptr = NULL ) : mPtr(ptr)
            {
                if( mPtr )
                {
                    mIter = mPtr->_AddWeakRef( this );
                }
            }

            /// copy constructor
            WeakRefBase( const WeakRefBase & ref ) : mPtr(ref.mPtr)
            {
                if( mPtr )
                {
                    mIter = mPtr->_AddWeakRef( this );
                }
            }

            /// destructor
            virtual ~WeakRefBase()
            {
                Clear();
            }

            /// clear the reference
            void Clear()
            {
                if( mPtr )
                {
                    mPtr->_RemoveWeakRef( mIter );
                    mPtr = NULL;
                }
            }
        };
    }

    ///
    /// Weak reference class
    ///
    template<class X>
    class WeakRef : public detail::WeakRefBase
    {
        typedef X * XPTR;
        typedef X & XREF;

    public:

        /// constructor
        WeakRef( const X * ptr = NULL ) : detail::WeakRefBase(ptr)
        {
        }

        /// copy constructor
        WeakRef( const WeakRef & ref ) : detail::WeakRefBase(ref)
        {
        }

        // Get the pointer
        XPTR Get() const { return (X*)mPtr; }

        /// check for Empty reference
        bool Empty() const { return NULL == mPtr; }

        /// clear the reference
        void Clear() { return detail::WeakRefBase::Clear(); }

        /// Set/reset the pointer
        void Set( const X * ptr )
        {
            if( mPtr == ptr ) return;

            Clear();

            if( ptr )
            {
                mPtr = ptr;
                mIter = mPtr->_AddWeakRef( this );
            }
        }

        ///
        /// Convert to XPTR
        ///
        operator XPTR () const { return (XPTR)mPtr; }

        ///
        /// 比较操作
        ///
        bool operator == ( const WeakRef & rhs ) const throw()
        {
            return mPtr == rhs.mPtr;
        }

        ///
        /// 比较操作
        ///
        bool operator != ( const WeakRef & rhs ) const throw()
        {
            return mPtr != rhs.mPtr;
        }

        ///
        /// 比较操作
        ///
        bool operator < ( const WeakRef & rhs ) const throw()
        {
            return mPtr < rhs.mPtr;
        }

        ///
        /// NOT operator
        ///
        bool operator !() const throw() { return !mPtr; }

        ///
        /// dereference operator.
        ///
        /// TODO: is this thread safe?
        ///
        XREF operator *() const throw()  { GN_ASSERT(mPtr); return *mPtr; }

        ///
        /// arrow operator
        ///
        XPTR operator->() const throw()  { GN_ASSERT(mPtr); return  mPtr; }
    };

    ///
    ///
    // -------------------------------------------------------------------------
    inline void RefCounter::NullifyWeakRef( void * ptr )
    {
        detail::WeakRefBase * ref = (detail::WeakRefBase *)ptr;
        ref->Clear();
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_REFCOUNTER_H__
