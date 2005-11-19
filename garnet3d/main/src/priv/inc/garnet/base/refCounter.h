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
        void incref() const  throw() { ++m_ref; }

        //!
        //! decrease reference counter, delete the object, if reference count reaches zero.
        //!
        void decref() const
        {
            GN_ASSERT( m_ref>0 );
            if( 0 == --m_ref ) delete this;
        }

        //!
        //! get current reference counter value
        //!
        int  getref() const throw() { return m_ref; }

        // ********************************
        //! \name protective ctor/dtor
        // ********************************
    protected:

        //@{

        //!
        //! Constructor
        //!
        RefCounter() : m_ref(0) {}

        //!
        //! Destructor
        //!
        virtual ~RefCounter()
        {
            if( m_ref > 0 )
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
        mutable int m_ref;
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
        //! default constructor
        //!
        AutoRef() throw() : mPtr(0) {}

        //!
        //! construct from a normal pointer
        //!
        //! Y should be a class that can be <b>implicitly</b> convert to
        //! X, such as a sub-class of X.
        //!
        //! if p is not NULL, increase its reference counter
        //!
        template <class Y>
        AutoRef<X>( Y * p ) throw()
            : mPtr(p)
        { if(mPtr) mPtr->incref(); }

        //!
        //! copy constructor
        //!
        //! Y should be a class that can be <b>implicitly</b> convert to
        //! X, such as a sub-class of X.
        //!
        template <class Y>
        AutoRef<X>( const AutoRef<Y> & p ) throw()
            : mPtr( p.get() )
        { if(mPtr) mPtr->incref(); }

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
        //! Y should be a class that can be <b>implicitly</b> convert to
        //! X, such as a sub-class of X.
        //!
        template <class Y> AutoRef<X> & operator = ( const AutoRef<Y> & rhs )
        {
            reset( rhs.get() );
            return *this;
        }

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
        //! reset with new pointer data
        //!
        //! this function will release old pointer, if not NULL; then increase
        //! the reference counter of new pointer, if not NULL.
        //!
        void reset( XPTR p = 0 )
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
            reset();
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
