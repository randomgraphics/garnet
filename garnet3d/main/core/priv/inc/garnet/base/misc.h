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
    //! 声明stdclass类型
    //!
    //! \param self    待声明类的名字
    //! \param parent  待声明类的父类的名字
    //! \note 在所有基于stdclass类型的声明中使用这个宏
    //!
    //! 这个宏声明了两个别名：MySelf和MyParent，和标准私有函数： selfOk()
    //!
    #define GN_DECLARE_STDCLASS(self, parent)                           \
                private : typedef self MySelf; typedef parent MyParent; \
                private : bool selfOk() const { return MySelf::ok(); }


    //!
    //! stdclass类型的标准初始化过程。
    //!
    //! \param class_name 当前函数所属的类名
    //! \param param_list 形如：(param1, param2, param3,...)的，
    //!                   且符合父类init()要求的参数列表（括号不能省略）
    //! \note 在所有基于stdclass类型的init函数中使用这个宏
    //!
    //! 例如：GN_STDCLASS_INIT( myobject_c, (param1, param2) );
    //! 更多信息参见下面的例子
    #define GN_STDCLASS_INIT( class_name, param_list )     \
        /* check for twice init */                         \
        if( selfOk() )                                     \
        {                                                  \
            GN_ERROR( "u call init() twice!" );            \
            quit(); return selfOk();                       \
        }                                                  \
        /* call parent's init() */                         \
        if( !MyParent::init param_list )  { quit(); return selfOk(); }

    //!
    //! stdclass类型的标准退出过程
    //!
    //! \note 在所有基于stdclass类型的quit()函数中使用这个宏
    //!
    //! 使用方法参见下面的例子
    #define GN_STDCLASS_QUIT()  { clear(); MyParent::quit(); }


    //!
    //! standard class
    //!
    //! \par
    //! 关于 StdClass 的使用说明以及一些注意事项，见下面代码中的注释。
    //! \par
    //! 任何需要需要初始化/退出操作的类都应当从 StdClass 继承。
    //! \par
    //! StdClass实现了标准的init/quit接口，这个接口包括七个标准函数：\n
    //! - StdClass()  : 构造函数。
    //!   - 通过clear()将私有变量清零
    //!   - 一般情况下，除了 clear() 以外，构造函数中再不应包含其他代码，
    //!   - 任何有可能失败的初始化代码都应放到 init() 中。
    //! \n
    //! - ~StdClass() : 虚析构函数。
    //!   - 调用 quit() 释放资源。
    //! \n
    //! - init()      : 实初始化函数。
    //!   - 成功则返回1，否则返回0。
    //! \n
    //! - quit()      : 虚函数。
    //!   - 释放所有的资源，同时将私有变量清零（通过调用 clear() ）。
    //!   - 当类的实例被delete时，析构函数将自动调用 quit() 。
    //!   - quit() 函数应当可以被安全的、多次的调用，也就是说，
    //!   - 在释放资源时必须首先检查资源的有效性。
    //! \n
    //! - ok()     : 公有虚函数。
    //!   - 用来检查是否已经初始化过。成功调用 init() 后返回true，
    //!     调用 quit() 后返回false。
    //! \n
    //! - selfOk() : 私有函数。
    //!   - selfOk() 是 ok() 的“非虚”版本，强制调用本类的 ok() 函数。
    //!   - 关于 selfOk() 的详细使用规则见下面的注解。
    //! \n
    //! - clear()     : 私有函数。
    //!   - 用于将私有成员变量清零，被构造函数和 quit() 调用
    //!
    //! \note  selfOk() 是虚函数 ok() 的“非虚”版本，其功能是强制调用
    //!        本类型的 ok() ，而不是象通常的虚函数那样根据实例的类型
    //!        来调用相应的 ok() 。 selfOk() 是私有函数，只能在本类的成
    //!        员函数中使用。 举个例子：
    //! \code
    //!    class A : public StdClass
    //!    {
    //!        GN_DECLARE_STDCLASS(A, StdClass);
    //!    public:
    //!        virtual bool ok() const { return 1; }
    //!        bool test()             { return ok(); }
    //!        bool test2()            { return selfOk(); }
    //!    }
    //!    class B : public A
    //!    {
    //!        GN_DECLARE_STDCLASS(B, A);
    //!    public:
    //!        virtual bool ok() const { return 0; }
    //!    }
    //!    A * a;
    //!    B b;
    //!    a = \&b;
    //!    a->test();  // 由于a指向的是B类型的变量，所以按照
    //!                // 虚函数的调用规则，将会调用B::ok()，所以返回0
    //!    a->test2(); // 使用selfOk()宏后，强制调用A::ok()，返回1
    //! \endcode
    //!
    class StdClass
    {
    public:
        //!
        //! ctor
        //!
        StdClass() { clear(); }
        //!
        //! dtor
        //!
        virtual ~StdClass() { quit(); }

    public :

        //!
        //! 初始化函数
        //!
        bool init()
        {
            if( selfOk() )
            {
                GN_ERROR( "u call init() twice!" );
                quit(); return selfOk();
            }

            m_ok = 1;
            return selfOk();
        }

        //!
        //! 退出函数
        //!
        virtual void quit() { clear(); }

        //!
        //! 是否初始化过？
        //!
        virtual bool ok() const { return m_ok; }

    private :

        //!
        //! initialize data members
        //!
        void clear() { m_ok = 0; }

        //!
        //! private initialization check routine
        //!
        bool selfOk() const { return StdClass::ok(); }

        //!
        //! initialization flag
        //!
        bool m_ok;
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
        ~NoCopy() {}
    private:  // emphasize the following members are private
        NoCopy( const NoCopy & );
        const NoCopy & operator = ( const NoCopy& );
    };

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
        //! decrease reference counter,
        //!
        //! After decreation, if reference counter is "0", call "delete this"
        //!
        void decref() const
        {
            GN_ASSERT( m_ref>0 );
            if ( 0 == --m_ref ) delete this;
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
            if ( m_ref > 0 )
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
        //! default constructor
        //!
        AutoRef() throw() : mPtr(0) {}

        //!
        //! construct from a normal pointer
        //!
        //! if p is not NULL, increase its reference counter
        //!
        explicit AutoRef( XPTR p ) throw()
            : mPtr(p)
        { if(mPtr) mPtr->incref(); }

        //!
        //! copy constructor
        //!
        //! Y should be a class that can be <b>implicitly</b> convert to
        //! X, such as a sub-class of X.
        //!
        template <class Y> AutoRef<X>( const AutoRef<Y> & p ) throw()
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
            if ( p ) p->incref();
            if ( mPtr ) mPtr->decref();
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
        void acquire( XPTR ptr )
        {
            reset();
            mPtr = ptr;
        }

        //!
        //! release ownership of internal XPTR
        //!
        //! this function will not modify pointer's refcount
        //!
        XPTR dismiss() throw()
        {
            XPTR tmp = mPtr;
            mPtr = 0;
            return tmp;
        }
    };

    //!
    //! 单件类
    //!
    template<typename T>
    class Singleton
    {
        static T * msInstance;

    public:

        //!
        //! Constructor
        //!
        Singleton()
        {
            GN_ASSERT( 0 == msInstance );
            // This is code 64-bit compatible?
            int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
            msInstance = (T*)((int)this+offset);
        }

        //!
        //! Destructor
        //!
        virtual ~Singleton() { GN_ASSERT(msInstance); msInstance = 0; }

        //!
        //! Get the instance
        //!
        static T & getInstance() { GN_ASSERT(msInstance); return *msInstance; }

        //!
        //! Get the instance pointer (might be NULL)
        //!
        static T * getInstance() { return msInstance; }
    };

    //!
    //! 定义单件类中的静态变量
    //!
    //! 在实现单件类的源文件中使用该宏。
    //!
    #define GN_IMPLEMENT_SINGLETON( T ) T * T::msInstance = 0;

    //!
    //! type cast function
    //!
    //! perform dynamic cast in debug build, and static cast in release build.
    // ------------------------------------------------------------------------
    template < class TO, class FROM >
    GN_FORCE_INLINE TO safeCast( FROM from )
    {
    #if GN_DEBUG && ( !GN_MSVC || defined(_CPPRTTI) )
        GN_ASSERT( dynamic_cast<TO>(from) );
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

    //! \cond
    namespace detail
    {
        //!
        //! Free one object instance
        //!
        template < typename T >
        struct freeElement
        {
            //!
            //! Free one object instance
            //!
            static void deallocate( const void * ptr )
            {
                ::free( ptr );
            }
        };
        //!
        //! delete one object instance
        //!
        template < typename T >
        struct deleteElement
        {
            //!
            //! delete one object instance
            //!
            static void deallocate( T * ptr )
            {
                delete ptr;
            }
        };

        //!
        //! delete an object array
        //!
        template < typename T >
        struct deleteArray
        {
            //!
            //! delete an object array
            //!
            static void deallocate( T * ptr )
            {
                delete [] ptr;
            }
        };

        //template < typename T >
        //struct release_object
        //{
        //    static void deallocate( T * & ptr )
        //    {
        //        ptr->release();
        //    }
        //};
    }
    //! \endcond

    //!
    //! general safe delocation routine
    // ------------------------------------------------------------------------
    template < typename T, typename DeallocPolicy >
    GN_FORCE_INLINE void safeDealloc( T * & ptr )
    {
        if( ptr )
        {
            DeallocPolicy::deallocate( ptr );
            ptr = NULL;
        }
    }

    //!
    //! delete one object
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeDelete( T * & ptr )
    {
        safeDealloc<T,detail::deleteElement<T> >(ptr);
    }

    //!
    //! delete object array
    // ------------------------------------------------------------------------
    template < typename T >
    GN_FORCE_INLINE void safeDeleteArray( T * & ptr )
    {
        safeDealloc<T,detail::deleteArray<T> >(ptr);
    }
}

// *****************************************************************************
//                           End of misc.h
// *****************************************************************************
#endif // __GN_BASE_MISC_H__
