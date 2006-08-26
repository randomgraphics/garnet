#ifndef __GN_BASE_STDCLASS_H__
#define __GN_BASE_STDCLASS_H__
// *****************************************************************************
//! \file    stdClass.h
//! \brief   basic class for standard init/quit procedures.
//! \author  chenlee (2005.8.13)
// *****************************************************************************

//!
//! 声明stdclass类型
//!
//! \param self    待声明类的名字
//! \param parent  待声明类的父类的名字
//! \note 在所有基于stdclass类型的声明中使用这个宏
//!
//! 这个宏声明了两个别名：MySelf和MyParent，和标准私有函数： selfOK()
//!
#define GN_DECLARE_STDCLASS(self, parent)                           \
            private : typedef self MySelf; typedef parent MyParent; \
            private : bool selfOK() const { return MySelf::ok(); }


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
    if( selfOK() )                                     \
    {                                                  \
        GN_ERROR(getLogger("GN.base.StdClass"))( "u call init() twice!" ); \
        quit(); return selfOK();                       \
    }                                                  \
    /* call parent's init() */                         \
    if( !MyParent::init param_list )  { quit(); return selfOK(); }

//!
//! stdclass类型的标准退出过程
//!
//! \note 在所有基于stdclass类型的quit()函数中使用这个宏
//!
//! 使用方法参见下面的例子
#define GN_STDCLASS_QUIT()  { clear(); MyParent::quit(); }

namespace GN
{
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
    //! - selfOK() : 私有函数。
    //!   - selfOK() 是 ok() 的“非虚”版本，强制调用本类的 ok() 函数。
    //!   - 关于 selfOK() 的详细使用规则见下面的注解。
    //! \n
    //! - clear()     : 私有函数。
    //!   - 用于将私有成员变量清零，被构造函数和 quit() 调用
    //!
    //! \note  selfOK() 是虚函数 ok() 的“非虚”版本，其功能是强制调用
    //!        本类型的 ok() ，而不是象通常的虚函数那样根据实例的类型
    //!        来调用相应的 ok() 。 selfOK() 是私有函数，只能在本类的成
    //!        员函数中使用。 举个例子：
    //! \code
    //!    class A : public StdClass
    //!    {
    //!        GN_DECLARE_STDCLASS(A, StdClass);
    //!    public:
    //!        virtual bool ok() const { return 1; }
    //!        bool test()             { return ok(); }
    //!        bool test2()            { return selfOK(); }
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
            if( selfOK() )
            {
                GN_ERROR(getLogger("GN.base.StdClass"))( "u call init() twice!" );
                quit(); return selfOK();
            }

            mOK = true;
            return selfOK();
        }

        //!
        //! 退出函数
        //!
        virtual void quit() { clear(); }

        //!
        //! 是否初始化过？
        //!
        virtual bool ok() const { return mOK; }

    private :

        //!
        //! initialize data members
        //!
        void clear() { mOK = false; }

        //!
        //! private initialization check routine
        //!
        bool selfOK() const { return StdClass::ok(); }

        //!
        //! initialization flag
        //!
        bool mOK;
    };

}

// *****************************************************************************
//                           End of stdClass.h
// *****************************************************************************
#endif // __GN_BASE_STDCLASS_H__
