#ifndef __GN_BASE_STDCLASS_H__
#define __GN_BASE_STDCLASS_H__
// *****************************************************************************
/// \file
/// \brief   basic class for standard init/quit procedures.
/// \author  chenlee (2005.8.13)
// *****************************************************************************

///
/// 声明stdclass类型
///
/// \param self    待声明类的名字
/// \param parent  待声明类的父类的名字
/// \note 在所有基于stdclass类型的声明中使用这个宏
///
/// 这个宏声明了两个别名：MySelf和MyParent
///
#define GN_DECLARE_STDCLASS(self, parent) \
            private : typedef self MySelf; typedef parent MyParent;


///
/// stdclass类型的标准初始化过程。
///
/// \param class_name 当前函数所属的类名
/// \param param_list 形如：(param1, param2, param3,...)的，
///                   且符合父类init()要求的参数列表（括号不能省略）
/// \note 在所有基于stdclass类型的init函数中使用这个宏
///
/// 例如：GN_STDCLASS_INIT( myobject_c, (param1, param2) );
/// 更多信息参见下面的例子
#if GN_BUILD_DEBUG
#define GN_STDCLASS_INIT( class_name, param_list )                               \
    /* call parent's init() */                                                   \
    if( !MyParent::init param_list ) return failure();                           \
    preInit();                                                                   \
    /* define sanity checker to ensure calling either failure() or success(). */ \
    SanityChecker __GN_sc(*this);
#else
#define GN_STDCLASS_INIT( class_name, param_list )     \
    /* call parent's init() */                         \
    if( !MyParent::init param_list ) return failure(); \
    preInit();
#endif

///
/// stdclass类型的标准退出过程
///
/// \note 在所有基于stdclass类型的quit()函数中使用这个宏
///
/// 使用方法参见下面的例子
#define GN_STDCLASS_QUIT()  { clear(); MyParent::quit(); }

namespace GN
{
    ///
    /// standard class
    ///
    /// \par
    /// 关于 StdClass 的使用说明以及一些注意事项，见下面代码中的注释。
    /// \par
    /// 任何需要需要初始化/退出操作的类都应当从 StdClass 继承。
    /// \par
    /// StdClass实现了标准的init/quit接口，这个接口包括6个标准函数：\n
    /// - StdClass()  : 构造函数。
    ///   - 通过clear()将私有变量清零
    ///   - 一般情况下，除了 clear() 以外，构造函数中再不应包含其他代码，
    ///   - 任何有可能失败的初始化代码都应放到 init() 中。
    /// \n
    /// - ~StdClass() : 虚析构函数。
    ///   - 调用 quit() 释放资源。
    /// \n
    /// - init()      : 初始化函数。
    ///   - 成功则返回true，否则返回false。
    /// \n
    /// - quit()      : 虚函数。
    ///   - 释放所有的资源，同时将私有变量清零（通过调用 clear() ）。
    ///   - 当类的实例被delete时，析构函数将自动调用 quit() 。
    ///   - quit() 函数应当可以被安全的、多次的调用，也就是说，
    ///   - 在释放资源时必须首先检查资源的有效性。
    /// \n
    /// - ok()        : 检测函数。
    ///   - 用来检查是否已经初始化过。成功调用 init() 后返回true，
    ///     调用 quit() 后返回false。
    /// \n
    /// - clear()     : 私有函数。
    ///   - 用于将私有成员变量清零，被构造函数和 quit() 调用
    ///
    class StdClass
    {
    public:
        ///
        /// ctor
        ///
        StdClass() { clear(); }
        ///
        /// dtor
        ///
        virtual ~StdClass() { quit(); }

    public :

        ///
        /// 初始化函数
        ///
        bool init()
        {
            if( StdClass::ok() )
            {
                GN_ERROR(getLogger("GN.base.StdClass"))( "u call init() twice!" );
                return failure();
            }

            return success();
        }

        ///
        /// 退出函数
        ///
        virtual void quit() { clear(); }

        ///
        /// 是否初始化过？
        ///
        bool ok() const { return IS_SUCCESS == mOK; }

    protected:

#if GN_BUILD_DEBUG
        ///
        /// Sanity checker for calling either failure() or success() in init().
        ///
        class SanityChecker
        {
            ///
            /// Boolean value to indicate whether failure() or success() is called.
            ///
            StdClass & theClass;

        public:

            ///
            /// ctor.
            ///
            SanityChecker( StdClass & c ) : theClass(c)
            {
                GN_ASSERT( StdClass::IS_UNDEFINED == c.mOK );
            }

            ///
            /// dtor. Check the boolean value.
            ///
            ~SanityChecker()
            {
                GN_ASSERT_EX(
                    StdClass::IS_FAILURE == theClass.mOK || StdClass::IS_SUCCESS == theClass.mOK,
                    "Neither failure() nor success() is called!" );
            }
        };
#endif

        ///
        /// helper function called by macro GN_STDCLASS_INIT() to invalidate mOK.
        ///
        void preInit() { GN_ASSERT(IS_SUCCESS == mOK); mOK = IS_UNDEFINED; }

        ///
        /// helper function called in init() to indicate initialization failure.
        ///
        bool failure()
        {
            quit();
            mOK = IS_FAILURE;
            return false;
        }

        ///
        /// helper function called in init() to indicate initialization success.
        ///
        bool success()
        {
            mOK = IS_SUCCESS;
            return true;
        }

    private :

        friend class SanityChecker;

        ///
        /// initialization status
        ///
        enum InitStatus
        {
            IS_FAILURE   = 0, ///< init() failed.
            IS_SUCCESS   = 1, ///< init() succedded.
            IS_UNDEFINED = 2, ///< init() not called.
        };

        ///
        /// initialization flag
        ///
        InitStatus mOK;

        ///
        /// initialize data members
        ///
        void clear() { mOK = IS_UNDEFINED; }
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_STDCLASS_H__
