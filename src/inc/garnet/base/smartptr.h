#ifndef __GN_BASE_SMARTPTR_H__
#define __GN_BASE_SMARTPTR_H__
// *****************************************************************************
/// \file
/// \brief   Smart pointer classes and utilities
/// \author  chen@@chenli-homepc (2011.4.9)
// *****************************************************************************

namespace GN
{
    // -------------------------------------------------------------------------
    /// Reference counted smart pointer. Support both strong and weak reference.
    class RefCounter : public GN::NoCopy
    {
    public:

        /// Utility class to support weak ref
        struct WeakObject
        {
            void      *    ptr; // pointer to RefCounter object
            std::mutex     lock;
            GN::DoubleLink references;

            // return true, only when reference list is empty
            bool deref(GN::DoubleLink & l)
            {
                lock.lock();
                l.detach();
                bool timeToDelete = !references.prev && !references.next;
                lock.unlock();
                return timeToDelete;
            }
        };

        // ********************************
        //       reference management
        // ********************************
    public :

        ///
        /// increase reference counter
        ///
        sint32 incref() const  throw() { return mRef.fetch_add(1) - 1; }

        ///
        /// decrease reference counter, delete the object, if reference count reaches zero.
        ///
        sint32 decref() const
        {
            GN_ASSERT( mRef > 0 );

            int ref = mRef.fetch_sub(1) - 1;// GN::atomDec32( &mRef ) ;
            if (0 == ref && mWeakObj)
            {
                mWeakLock.lock();
                mWeakObj->lock.lock();
                mWeakLink.detach();
                mWeakObj->ptr = NULL;
                bool timeToDelete = !mWeakObj->references.prev && !mWeakObj->references.next;
                mWeakObj->lock.unlock();
                if (timeToDelete)
                {
                    delete mWeakObj;
                }
                mWeakObj = NULL;
                mWeakLock.unlock();
            }

            if( 0 == ref )
            {
                delete this;
            }

            return ref;
        }

        ///
        /// get current reference counter value
        ///
        int getref() const throw() { return mRef; }

        ///
        /// Return the weak object associated with this reference counted object.
        ///
        WeakObject * getWeakObj() const
        {
            mWeakLock.lock();
            if (!mWeakObj)
            {
                mWeakObj = new WeakObject();
                mWeakObj->ptr = (void*)this;
                mWeakLink.linkAfter(&mWeakObj->references);
            }
            mWeakLock.unlock();
            return mWeakObj;
        }

        ///
        /// For internal use. Do _NOT_ call.
        ///
        WeakObject * __getWeakObjNoLock() const
        {
            return mWeakObj;
        }

        // ********************************
        /// \name protective ctor/dtor
        // ********************************
    protected:

        //@{

        ///
        /// Constructor
        ///
        RefCounter() : mRef(0), mWeakObj(NULL) {}

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

        //@}

        // ********************************
        // private data members
        // ********************************
    private:

        ///
        /// reference counter
        ///
        mutable std::atomic_int mRef;
        mutable std::mutex      mWeakLock;
        mutable WeakObject *    mWeakObj;
        mutable GN::DoubleLink  mWeakLink;
    };

    ///
    /// 配合 RefCounter 使用的自动指针类
    // -------------------------------------------------------------------------
    template <class X> class AutoRef
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
        static AutoRef<X> NULLREF;

        // default ctor
        AutoRef() throw() : mPtr(NULL)
        {
            // make sure sizeof(AutoRef) == sizeof(XPTR), which ensures that an array of autoref
            // can always be used as array of native pointer.
            static_assert(sizeof(AutoRef) == sizeof(XPTR));
        }

        /// construct from a normal pointer
        ///
        explicit AutoRef(XPTR p) throw() : mPtr(p)
        {
            if (p) p->incref();
        }

        ///
        /// copy constructor
        ///
        AutoRef( const AutoRef & p ) throw() : mPtr( p )
        {
            if( mPtr ) mPtr->incref();
        }

        ///
        /// copy constructor
        ///
        template <class Y>
        AutoRef( const AutoRef<Y> & p ) throw() : mPtr( p )
        {
            if( mPtr ) mPtr->incref();
        }

        ///
        /// destructor
        ///
        ~AutoRef()
        {
            if( mPtr ) mPtr->decref();
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
        AutoRef & operator = ( XPTR ptr )
        {
            set( ptr );
            return *this;
        }

        ///
        /// 赋值语句
        ///
        template <class Y>
        AutoRef & operator = ( const AutoRef<Y> & rhs )
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
            if( mPtr ) mPtr->decref();
            mPtr = 0;
        }

        ///
        /// set new pointer data
        ///
        /// this function will release old pointer, if not NULL; then increase
        /// the reference counter of new pointer, if not NULL.
        ///
        void set( XPTR p )
        {
            if( p ) p->incref();
            if( mPtr ) mPtr->decref();
            mPtr = p;
        }

        ///
        /// acquire ownership of a XPTR
        ///
        /// this function will not modify pointer's refcount
        ///
        void attach( XPTR ptr )
        {
            if( ptr == mPtr ) return;
            if( mPtr ) mPtr->decref();
            mPtr = ptr;
        }

        ///
        /// release ownership of internal XPTR
        ///
        /// this function will not modify pointer's refcount
        ///
        XPTR detach() throw()
        {
            XPTR tmp = mPtr;
            mPtr = 0;
            return tmp;
        }
    };

    template<typename X> AutoRef<X> AutoRef<X>::NULLREF;

    ///
    /// Attach to a C pointer without increasing its reference count.
    ///
    template<typename T>
    inline AutoRef<T> attachTo(T * ptr)
    {
        AutoRef<T> result;
        result.attach(ptr);
        return result;
    }

    ///
    /// Reference a C refcount pointer. Increase its ref count by one.
    ///
    template<typename T>
    inline AutoRef<T> referenceTo(T * ptr)
    {
        AutoRef<T> result;
        result.attach(ptr);
        result->incref();
        return result;
    }

    ///
    /// Weak refernce to smart pointer object
    // -------------------------------------------------------------------------
    template<typename X>
    class WeakRef
    {
        typedef X * XPTR;
        typedef X & XREF;

        RefCounter::WeakObject * mObj;
        GN::DoubleLink           mLink;

        typedef AutoRef<X> StrongRef;

    public:

        ///
        /// constructor
        ///
        WeakRef( XPTR ptr = NULL ) : mObj(NULL)
        {
            set( ptr );
        }

        ///
        /// copy constructor
        ///
        WeakRef( const WeakRef & ref ) : mObj(NULL)
        {
            set( ref.promote() );
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
            if (mObj && mObj->deref(mLink))
            {
                delete mObj;
            }
            mObj = NULL;
        }

        bool empty() const
        {
            bool result = true;
            if (mObj)
            {
                mObj->lock.lock();
                result = NULL == mObj->ptr;
                mObj->lock.unlock();
            }
            return result;
        }

        ///
        /// set/reset the pointer. Null pointer is allowed.
        ///
        void set( XPTR ptr )
        {
            if (!ptr)
            {
                clear();
            }
            else
            {
                RefCounter::WeakObject * obj = ptr->getWeakObj();
                if (obj != mObj)
                {
                    obj->lock.lock();
                    mLink.linkAfter( &obj->references );
                    mLink.context = this;
                    obj->lock.unlock();
                    mObj = obj;
                }
            }
        }

        ///
        /// promote to strong reference
        ///
        AutoRef<X> promote() const
        {
            AutoRef<X> result;
            if (mObj)
            {
                mObj->lock.lock();
                result.set((XPTR)mObj->ptr);
                mObj->lock.unlock();
            }
            return result;
        }

        ///
        /// copy operator
        ///
        WeakRef & operator = ( const WeakRef & rhs )
        {
            set( rhs.promote() ); // TODO: we might be able to do it slightly faster.
            return *this;
        }

        bool operator !() const { return empty(); }

        ///
        /// 比较操作
        ///
        bool operator == ( const WeakRef & rhs ) const throw()
        {
            return mObj == rhs.mObj;
        }

        ///
        /// 比较操作
        ///
        bool operator == ( XPTR ptr ) const throw()
        {
            return mObj == (ptr ? ptr->__getWeakObjNoLock() : NULL);
        }

        ///
        /// 比较操作
        ///
        friend inline bool operator == ( XPTR ptr, const WeakRef & rhs ) throw()
        {
            return rhs.mObj == (ptr ? ptr->__getWeakObjNoLock() : NULL);
        }

        ///
        /// 比较操作
        ///
        bool operator != ( const WeakRef & rhs ) const throw()
        {
            return mObj != rhs.mObj;
        }

        ///
        /// 比较操作
        ///
        bool operator < ( const WeakRef & rhs ) const throw()
        {
            return mObj < rhs.mObj;
        }
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SMARTPTR_H__
