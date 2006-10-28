#ifndef __GN_GFX_CGSHADER_H__
#define __GN_GFX_CGSHADER_H__
// *****************************************************************************
//! \file    common/cgShader.h
//! \brief   Common Cg shader class
//! \author  chen@HOME-PC (2006.10.28)
// *****************************************************************************

//! \name Cg error check macros
//@{
#define CG_CHECK_ERROR_DO( func, something )                                \
    if( true ) {                                                            \
        func;                                                               \
        CGerror error;                                                      \
        const char * str = cgGetLastErrorString( &error );                  \
        if( str )                                                           \
        {                                                                   \
            static GN::Logger * sLogger = GN::getLogger("GN.gfx.Cg");       \
            GN_ERROR(sLogger)( "Cg error : %s", str );                      \
            something                                                       \
        }                                                                   \
    } else void(0)
#define CG_CHECK_ERROR_R( func )        CG_CHECK_ERROR_DO( func, return; )
#define CG_CHECK_ERROR_RV( func, rval ) CG_CHECK_ERROR_DO( func, return rval; )
#if GN_DEBUG_BUILD
#define CG_CHECK_ERROR( func )          CG_CHECK_ERROR_DO( func, ; )
#else
#define CG_CHECK_ERROR( func )          func
#endif
//@}

namespace GN { namespace gfx
{
#ifdef HAS_CG

    // *************************************************************************
    // Cg shader
    // *************************************************************************
    
    //!
    //! Cg Shader class
    //!
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
        bool ok() const { return MyParent::ok() && mProgram; }
    private:
        void clear() { mProgram = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! get program handle
        //!
        CGprogram getProgram() const { return mProgram; }

        //!
        //! get uniform handle
        //!
        CGparameter getUniformHandle( const StrA & name ) const
        {
            GN_GUARD_SLOW;
            CGparameter param;
            CG_CHECK_ERROR_RV(
                param = cgGetNamedParameter( mProgram, name.cptr() ),
                0 );
            return param;
            GN_UNGUARD_SLOW;
        }

        //!
        //! get uniform name
        //!
        StrA getUniformName( CGparameter param ) const
        {
            GN_GUARD_SLOW;
            const char * name;
            CG_CHECK_ERROR_RV(
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
//                           End of cgShader.h
// *****************************************************************************
#endif // __GN_GFX_CGSHADER_H__
