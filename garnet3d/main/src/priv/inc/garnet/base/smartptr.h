#ifndef __GN_BASE_SMARTPTR_H__
#define __GN_BASE_SMARTPTR_H__
// *****************************************************************************
/// \file
/// \brief   Smart pointer classes and utilities
/// \author  chen@@chenli-homepc (2011.4.9)
// *****************************************************************************

namespace GN
{
    ///
    /// Base class that support weak referencing
    // -------------------------------------------------------------------------
    class WeakObject
    {
        friend class WeakRefBase;

        mutable DoubleLink mRefs;

    public:

        /// Default constructor
        WeakObject()
        {
            // This context pointer is never used.
            mRefs.context = (void*)0xbadbeef;
        }

        /// Copy constructor
        WeakObject( const WeakObject & )
        {
            // Nothing to copy
            mRefs.context = (void*)0xbadbeef;
        }

        /// Destructor
        virtual ~WeakObject();

        /// Copy operator
        WeakObject & operator=( const WeakObject & )
        {
            // Nothing to copy.
            return *this;
        }
    };

    ///
    /// Base class of weak referencing pointer. This class is a building
    /// block of weak-ref pointer. It should not be used directly by
    /// client code.
    // -------------------------------------------------------------------------
    class WeakRefBase
    {
        friend class WeakObject;

    protected:

        const WeakObject * mPtr;
        DoubleLink         mLink;

        /// Constructor
        WeakRefBase( const WeakObject * ptr ) : mPtr(ptr)
        {
            mLink.context = this;
            if( ptr ) mLink.linkAfter( &ptr->mRefs );
        }

        /// copy constructor
        WeakRefBase( const WeakRefBase & ref ) : mPtr(ref.mPtr)
        {
            mLink.context = this;
            if( mPtr ) mLink.linkAfter( &mPtr->mRefs );
        }

        /// non virtual destructor
        ~WeakRefBase()
        {
        }

        /// Attach to a new weak object (detach from current one)
        void attachTo( const WeakObject * ptr )
        {
            if( mPtr == ptr ) return;

            // detach from current object
            mLink.detach();

            // attach to new object
            if( ptr ) mLink.linkAfter( &ptr->mRefs );
            mPtr = ptr;
         }
    };

    ///
    /// Weak object destructor
    // -------------------------------------------------------------------------
    inline WeakObject::~WeakObject()
    {
        // Loop through reference list. Clear them all.
        DoubleLink * next;
        WeakRefBase * ref;
        while( NULL != (next = mRefs.next) )
        {
            ref = (WeakRefBase*)next->context;
            ref->mPtr = NULL;
            next->detach();
        }
    }

    ///
    /// 配合 WeakObject 使用的 weak reference pointer.
    // -------------------------------------------------------------------------
    template<typename X>
    class WeakRef : private WeakRefBase
    {
        typedef X * XPTR;
        typedef X & XREF;

    public:

        ///
        /// constructor
        ///
        WeakRef( XPTR ptr = NULL ) : WeakRefBase(ptr)
        {
        }

        ///
        /// copy constructor
        ///
        WeakRef( const WeakRef & ref ) : WeakRefBase(ref)
        {
        }

        ///
        /// Destructor
        ///
        ~WeakRef()
        {
            clear();
        }

        ///
        /// clear the reference
        ///
        void clear()
        {
            mLink.detach();
            mPtr = NULL;
        }

        ///
        /// get the raw pointer
        ///
        XPTR rawptr() const { return (XPTR)WeakRefBase::rawptr(); }

        ///
        /// check for empty reference
        ///
        bool empty() const { return NULL == mPtr; }

        ///
        /// set/reset the pointer. Null pointer is allowed.
        ///
        void set( XPTR ptr )
        {
            if( mPtr == ptr ) return;
            attachTo( ptr );
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
    // -------------------------------------------------------------------------
    class RefCounter : public WeakObject, public NoCopy
    {
        // ********************************
        //       reference management
        // ********************************
    public :

        ///
        /// increase reference counter
        ///
        sint32 incref() const  throw() { return atomInc32(&mRef); }

        ///
        /// decrease reference counter, delete the object, if reference count reaches zero.
        ///
        sint32 decref() const
        {
            GN_ASSERT( mRef > 0 );

            sint32 ref = atomDec32( &mRef ) ;

            if( 0 == ref )
            {
                // delete itself
                selfDestruct();
            }

            return ref;
        }

        ///
        /// get current reference counter value
        ///
        sint32 getref() const throw() { return atomGet32(&mRef); }

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
        }

        ///
        /// Override this function to change the behavior when reference counter reaches zero.
        /// This is useful when RefCounter object is created on stack or declared as a member of
        /// another structure/class.
        ///
        virtual void selfDestruct() const
        {
            delete this;
        }

        //@}

        // ********************************
        // private data members
        // ********************************
    private:

        ///
        /// reference counter
        ///
        mutable volatile sint32 mRef;
    };

    ///
    /// 配合 RefCounter 使用的自动指针类
    // -------------------------------------------------------------------------
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
        /// Instance of empty/null reference pointer
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
            if(mPtr) mPtr->incref();
        }

        ///
        /// copy constructor
        ///
        template <class Y,class MUTEX2>
        AutoRef( const AutoRef<Y,MUTEX2> & p ) throw() : mPtr( p )
        {
            if(mPtr) mPtr->incref();
        }

        ///
        /// destructor
        ///
        ~AutoRef()
        {
            if(mPtr) mPtr->decref();
        }

        ///
        /// 赋值语句
        ///
        AutoRef & operator = ( const AutoRef & rhs )
        {
            set( rhs );
            return *this;
        }

        ///
        /// 赋值语句
        ///
        template <class Y,class MUTEX2>
        AutoRef & operator = ( const AutoRef<Y,MUTEX2> & rhs )
        {
            set( rhs );
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
        /// get internal raw pointer
        ///
        XPTR rawptr() const throw() { return mPtr; }

        ///
        /// get address of internal pointer.
        ///
        /// 这个函数主要用于将指向AutoRef的指针变成指向XPTR的指针（他们在内存中的映像其实是一样的）。
        /// 你可以用强制类型转换达同样的目的，不过用这个函数会更简洁一些。
        ///
        XPTR const * addr() const throw() { return &mPtr; }

        ///
        /// return true if no pointer is currently being hold
        ///
        bool empty() const throw()
        {
            return 0 == mPtr;
        }

        ///
        /// Clear to empty. Same as set(NULL).
        ///
        void clear()
        {
            MUTEX m;
            m.lock();

            if( mPtr ) mPtr->decref(); mPtr = 0;

            m.unlock();
        }

        ///
        /// set new pointer data
        ///
        /// this function will release old pointer, if not NULL; then increase
        /// the reference counter of new pointer, if not NULL.
        ///
        void set( XPTR p )
        {
            MUTEX m;
            m.lock();

            if( p ) p->incref();
            if( mPtr ) mPtr->decref();
            mPtr = p;

            m.unlock();
        }

        ///
        /// acquire ownership of a XPTR
        ///
        /// this function will not modify pointer's refcount
        ///
        void attach( XPTR ptr )
        {
            if( ptr == mPtr ) return;

            MUTEX m;
            m.lock();

            if( mPtr ) mPtr->decref();
            mPtr = ptr;

            m.unlock();
        }

        ///
        /// release ownership of internal XPTR
        ///
        /// this function will not modify pointer's refcount
        ///
        XPTR detach() throw()
        {
            MUTEX m;
            m.lock();

            XPTR tmp = mPtr;
            mPtr = 0;

            m.unlock();

            return tmp;
        }
    };

    template<typename X,typename M> AutoRef<X,M> AutoRef<X,M>::NULLREF;

    ///
    /// This is used to create referenced counted class on stack (or as a member of another class)
    // -------------------------------------------------------------------------
    template<class REF_COUNTED_CLASS>
    class StackRefCounter : public REF_COUNTED_CLASS
    {
    public:

        /// Destructor
        virtual ~StackRefCounter()
        {
            REF_COUNTED_CLASS::decref();
        }

    protected:

        /// Do nothing for self destruction
        virtual void selfDestruct() const
        {
            // do nothing here.
        }
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SMARTPTR_H__
