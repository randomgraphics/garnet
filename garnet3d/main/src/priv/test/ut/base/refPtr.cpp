#include "../testCommon.h"

#if 0
namespace GN2
{
    ///
    /// Utility class to support weak ref
    // -------------------------------------------------------------------------
    struct WeakObject
    {
        void      *    ptr; // pointer to RefCounter object
        GN::Mutex      lock;
        GN::DoubleLink references;

        bool deref(GN::DoubleLink & l)
        {
            lock.enter();
            l.detach();
            bool timeToDelete = !references.prev && !references.next;
            lock.leave();
            return timeToDelete;
        }
    };

    // -------------------------------------------------------------------------
    class RefCounter : public GN::NoCopy
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

            mWeakLock.enter();
            if (mWeakObj && mWeakObj->deref(mWeakLink))
            {
                delete mWeakObj;
                mWeakObj = NULL;
            }
            sint32 ref = atomDec32( &mRef ) ;
            mWeakLock.leave();

            if( 0 == ref )
            {
                delete this;
            }

            return ref;
        }

        ///
        /// get current reference counter value
        ///
        sint32 getref() const throw() { return atomGet32(&mRef); }

        WeakObject * getWeakObj() const
        {
            mWeakLock.enter();
            if (!mWeakObj)
            {
                mWeakObj = new WeakObject();
                mWeakObj->ptr = this;
                mWeakLink.linkAfter(&mWeakObj);
            }
            mWeakLock.leave();
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
        mutable volatile sint32 mRef;
        mutable GN::Mutex       mWeakLock;
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

        #if 1
        /// construct from a normal pointer
        ///
        explicit AutoRef( XPTR p = 0 ) throw() : mPtr(p)
        {
            // make sure sizeof(AutoRef) == sizeof(XPTR), which ensures that an array of autoref
            // can always be used as array of native pointer.
            GN_CASSERT( sizeof(AutoRef) == sizeof(XPTR) );

            if( p ) p->incref();
        }
        #else
        // default ctor
        AutoRef() throw() : mPtr(NULL)
        {
            // make sure sizeof(AutoRef) == sizeof(XPTR), which ensures that an array of autoref
            // can always be used as array of native pointer.
            GN_CASSERT( sizeof(AutoRef) == sizeof(XPTR) );
        }
        #endif

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

    ///
    /// 配合 WeakObject 使用的 weak reference pointer.
    // -------------------------------------------------------------------------
    template<typename X>
    class WeakRef
    {
        typedef X * XPTR;
        typedef X & XREF;

        WeakObject   * mObj;
        GN::DoubleLink mLink;

    public:

        ///
        /// constructor
        ///
        WeakRef( const AutoRef<XPTR> & ref ) : mObj(NULL)
        {
            set( ref );
        }

        ///
        /// copy constructor
        ///
        WeakRef( const WeakRef & ref )
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

        ///
        /// set/reset the pointer. Null pointer is allowed.
        ///
        void set( const AutoRef<XPTR> & ref )
        {
            if (!ref)
            {
                clear();
            }
            else
            {
                WeakObject * obj = ref->getWeakObj();
                if (obj != mObj)
                {
                    obj->lock.enter();
                    mLink.linkAfter( &obj->refs );
                    mLink.context = this;
                    obj->lock.leave();
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
                mObj->lock.enter();
                result.set((XPTR)mObj->ptr);
                mObj->lock.leave();
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
#endif

class RefPtrTest : public CxxTest::TestSuite
{
    struct MyRefObj : public GN::RefCounter
    {
    };
    typedef GN::AutoRef<MyRefObj> MyRefObjPtr;

    struct MyRefObj2 : public MyRefObj
    {
    };
    typedef GN::AutoRef<MyRefObj2> MyRefObj2Ptr;

    struct MyStruct
    {
        MyRefObjPtr p;
    };

public:

    void testCopyCtor()
    {
        MyRefObj2Ptr p2 = GN::referenceTo( new MyRefObj2 );
        TS_ASSERT_EQUALS( p2->getref(), 1 );

        MyRefObjPtr p1( p2 );
        TS_ASSERT_EQUALS( p1->getref(), 2 );

        MyRefObjPtr p3( p1 );
        TS_ASSERT_EQUALS( p3->getref(), 3 );
   }

    void testAssignment()
    {
        MyRefObjPtr p2 = GN::referenceTo( new MyRefObj2 );
        TS_ASSERT_EQUALS( p2->getref(), 1 );

        MyRefObjPtr p1a, p1b;

        p1a = p2;
        TS_ASSERT_EQUALS( p1a->getref(), 2 );

        p1b = p1a;
        TS_ASSERT_EQUALS( p1b->getref(), 3 );
    }

    void testWeakRef()
    {
        using namespace GN;

        WeakObject * o = new WeakObject;

        WeakRef<WeakObject> w1( o );
        WeakRef<WeakObject> w2;
        WeakRef<WeakObject> w3( w1 );

        w2.set( o );

        TS_ASSERT( w1 );
        TS_ASSERT( w2 );
        TS_ASSERT( w3 );
        TS_ASSERT( w1 == w2 );
        TS_ASSERT( w1 == w3 );

        delete o;

        TS_ASSERT( !w1 );
        TS_ASSERT( !w2 );
        TS_ASSERT( !w3 );
    }
};
