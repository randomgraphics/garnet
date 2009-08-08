#ifndef __GN_GFX_CGSHADER_H__
#define __GN_GFX_CGSHADER_H__
// *****************************************************************************
/// \file
/// \brief   Common Cg shader class
/// \author  chen@@HOME-PC (2006.10.28)
// *****************************************************************************

/// \name Cg error check macros
//@{
#define GN_CG_CHECK_DO( func, something )                                   \
    if( true ) {                                                            \
        func;                                                               \
        CGerror error;                                                      \
        const char * str = cgGetLastErrorString( &error );                  \
        if( str )                                                           \
        {                                                                   \
            static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.cg");  \
            GN_ERROR(sLogger)( "Cg error : %s", str );                      \
            something                                                       \
        }                                                                   \
    } else void(0)
#define GN_CG_CHECK_R( func )        GN_CG_CHECK_DO( func, return; )
#define GN_CG_CHECK_RV( func, rval ) GN_CG_CHECK_DO( func, return rval; )
#if GN_BUILD_DEBUG
#define GN_CG_CHECK( func )          GN_CG_CHECK_DO( func, ; )
#else
#define GN_CG_CHECK( func )          func
#endif
//@}

namespace GN { namespace gfx
{
#ifdef HAS_CG

    // *************************************************************************
    // Cg context
    // *************************************************************************

    ///
    /// Cg context wrapper class
    ///
    class CgContextWrapper
    {
        CGcontext mContext;

        static void sCgErrorHandler( CGcontext ctx, CGerror err, void * )
        {
            static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.cg");

            GN_ERROR(sLogger)( "Cg error: %s", cgGetErrorString(err) );
            const char * listing = cgGetLastListing(ctx);
            if( listing )
            {
                GN_ERROR(sLogger)( "Last listing = %s", listing );
            }
        }

    public:

        ///
        /// create Cg context
        ///
        CgContextWrapper() : mContext(0)
        {
        }

        ///
        /// destroy Cg context
        ///
        ~CgContextWrapper()
        {
            quit();
        }

        ///
        /// initiate the context
        ///
        bool init()
        {
            GN_ASSERT( 0 == mContext );

            // set Cg error hanlder
            //cgSetErrorHandler( &sCgErrorHandler, 0 );

            // create Cg context
            mContext = cgCreateContext();
            if( !mContext )
            {
                static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.cg");
                GN_FATAL(sLogger)( "Fail to create Cg context!" );
                return false;
            }

            return true;
        }

        ///
        /// close the Cg context
        ///
        void quit()
        {
            if( mContext )
            {
                cgDestroyContext( mContext );
                mContext = 0;
            }
            //cgSetErrorHandler( 0, 0 );
        }

        ///
        /// cast to CGcontext type
        ///
        operator CGcontext() const
        {
            GN_ASSERT( mContext );
            return mContext;
        }
    };

    // *************************************************************************
    // Cg shader
    // *************************************************************************

    ///
    /// Cg Shader class
    ///
    class CgShader : public StdClass
    {
        GN_DECLARE_STDCLASS( CgShader, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        CgShader()          { clear(); }
        virtual ~CgShader() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init(
            CGcontext context,
            CGprofile profile,
            const StrA & code,
            const StrA & entry,
            const char ** args = NULL );
        void quit();
    private:
        void clear() { mProgram = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// get program handle
        ///
        CGprogram getProgram() const { return mProgram; }

        ///
        /// get uniform handle
        ///
        CGparameter getUniformHandle( const StrA & name ) const
        {
            GN_GUARD_SLOW;
            CGparameter param;
            GN_CG_CHECK_RV(
                param = cgGetNamedParameter( mProgram, name.cptr() ),
                0 );
            return param;
            GN_UNGUARD_SLOW;
        }

        ///
        /// get uniform name
        ///
        StrA getUniformName( CGparameter param ) const
        {
            GN_GUARD_SLOW;
            const char * name;
            GN_CG_CHECK_RV(
                name = cgGetParameterName( param ),
                "" );
            return name;
            GN_UNGUARD_SLOW;
        }

        // ********************************
        // private variables
        // ********************************
    private:

        CGprogram mProgram;
        CGcontext mContext;
        CGprofile mProfile;
        StrA      mCode;

        // ********************************
        // private functions
        // ********************************
    private:
    };
#endif
}}


// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_CGSHADER_H__
