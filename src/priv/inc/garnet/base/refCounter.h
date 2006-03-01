#ifndef __GN_BASE_REFCOUNTER_H__
#define __GN_BASE_REFCOUNTER_H__
// *****************************************************************************
//! \file    refCounter.h
//! \brief   Reference counting classes
//! \author  chenlee (2005.8.13)
// *****************************************************************************

namespace GN
{
    //!
    //! 引用计数器
    //!
    //! 提供基本的引用技术功能，用于和 AutoRef 一起实现自动指针类
    //!
    //! \note
    //! 在使用 RefCounter 时，为保证系统的正常运作，有几条规则必须遵守：
    //!     - 尽可能的使用 AutoRef 来管理 RefCounter 类的实例
    //!     - 如果有相互引用的现象存在（不管是直接的还是间接的），在释放对象前
    //!       必须首先打开引用环，否则将造成内存泄漏，环中的变量将永远无法释放
    //!     - 就这些吧，其它的以后想到再说....
    //!
    class RefCounter : public NoCopy
    {
        // ********************************
        //       reference management
        // ********************************
    public :

        //!
        //! increase reference counter
        //!
        void incref() const  throw() { ++mRef; }

        //!
        //! decrease reference counter, delete the object, if reference count reaches zero.
        //!
        void decref() const
        {
            GN_ASSERT( mRef>0 );
            if( 0 == --mRef ) delete this;
        }

        //!
        //! get current reference counter value
        //!
        int  getref() const throw() { return mRef; }

        // ********************************
        //! \name protective ctor/dtor
        // ********************************
    protected:

        //@{

        //!
        //! Constructor
        //!
        RefCounter() : mRef(1) {}

        //!
        //! Destructor
        //!
        virtual ~RefCounter()
        {
            if( mRef > 0 )
            {
                GN_ERROR(
                    "destructing a refcounter with its reference counter "
                    "greater then zero!" );
            }
        }

        //@}

        // ********************************
        // private data members
        // ********************************
    private:

        // Make this class NoCopy.
        RefCounter( const RefCounter& );
        const RefCounter& operator=( const RefCounter& );

        //!
        //! reference counter
        //!
        mutable int mRef;
    };

    //!
    //! 和 RefCounter 配合使用的自动指针类
    //!
    template <class X> class AutoRef
    {
        //!
        //! pointer to class X
        //!
        typedef  X * XPTR;

        //!
        //! reference to class X
        //!
        typedef  X & XREF;

    private:

        XPTR  mPtr; //!< internal pointer

    public :

        //!
        //! Instance of empty reference pointer
        //!
        static AutoRef<X> EMPTYPTR;

        //!
        //! construct from a normal pointer
        //!
        explicit AutoRef( XPTR p = 0 ) throw() : mPtr(p) {}

        //!
        //! copy constructor
        //!
        AutoRef( const AutoRef & p ) throw() : mPtr( p )
        {
            if(mPtr) mPtr->incref();
        }

        //!
        //! copy constructor
        //!
        template <class Y>
        AutoRef( const AutoRef<Y> & p ) throw() : mPtr( p )
        {
            if(mPtr) mPtr->incref();
        }

        //!
        //! destructor
        //!
        ~AutoRef()
        {
            if(mPtr) mPtr->decref();
        }

        //!
        //! 赋值语句
        //!
        AutoRef & operator = ( const AutoRef & rhs )
        {
            reset( rhs );
            return *this;
        }

        //!
        //! 赋值语句
        //!
        template <class Y>
        AutoRef & operator = ( const AutoRef<Y> & rhs )
        {
            reset( rhs );
            return *this;
        }

        //!
        //! Convert to XPTR
        //!
        operator XPTR () const { return mPtr; }

        //!
        //! 比较操作
        //!
        bool operator == ( const AutoRef & rhs ) const throw()
        {
            return mPtr == rhs.mPtr;
        }

        //!
        //! 比较操作
        //!
        bool operator != ( const AutoRef & rhs ) const throw()
        {
            return mPtr != rhs.mPtr;
        }

        //!
        //! 比较操作
        //!
        bool operator < ( const AutoRef & rhs ) const throw()
        {
            return mPtr < rhs.mPtr;
        }

        //!
        //! NOT operator
        //!
        bool operator !() const throw() { return !mPtr; }

        //!
        //! dereference operator
        //!
        XREF operator *() const throw()  { GN_ASSERT(mPtr); return *mPtr; }

        //!
        //! arrow operator
        //!
        XPTR operator->() const throw()  { GN_ASSERT(mPtr); return  mPtr; }

        //!
        //! get internal pointer
        //!
        XPTR get() const throw() { return mPtr; }

        //!
        //! get address of internal pointer.
        //!
        //! 这个函数主要用于将指向AutoRef的指针变成指向XPTR的指针（他们在内存中的映像其实是一样的）。
        //! 你可以用强制类型转换达同样的目的，不过用这个函数会更简洁一些。
        //!
        XPTR const * addr() const throw() { return &mPtr; }

        //!
        //! Clear to empty. Same as reset(NULL).
        //!
        void clear() { if( mPtr ) mPtr->decref(); mPtr = 0; }

        //!
        //! reset with new pointer data
        //!
        //! this function will release old pointer, if not NULL; then increase
        //! the reference counter of new pointer, if not NULL.
        //!
        void reset( XPTR p )
        {
            if( p ) p->incref();
            if( mPtr ) mPtr->decref();
            mPtr = p;
        }

        //!
        //! return true if no pointer is currently being hold
        //!
        bool empty() const throw()
        {
            return 0 == mPtr;
        }


        //!
        //! acquire ownership of a XPTR
        //!
        //! this function will not modify pointer's refcount
        //!
        void attach( XPTR ptr )
        {
            clear();
            mPtr = ptr;
        }

        //!
        //! release ownership of internal XPTR
        //!
        //! this function will not modify pointer's refcount
        //!
        XPTR detach() throw()
        {
            XPTR tmp = mPtr;
            mPtr = 0;
            return tmp;
        }
    };

    template<typename X> AutoRef<X> AutoRef<X>::EMPTYPTR;
}

// *****************************************************************************
//                           End of refCounter.h
// *****************************************************************************
#endif // __GN_BASE_REFCOUNTER_H__
