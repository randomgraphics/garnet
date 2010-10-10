#include "pch.h"
#include "cgShader.h"

#ifdef HAS_CG

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.cg");

static GN::StrA sAddLineCountCG( const GN::StrA & in )
{
    using namespace GN;

    GN::StrA out( "(  1) : " );

    int line = 1;
    for( const char * s = in.cptr(); *s; ++s )
    {
        if( '\n' == *s )
        {
            out.append( stringFormat( "\n(%3d) : ", ++line ) );
        }
        else
        {
            out.append( *s );
        }
    }

    return out;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::CgShader::init(
    CGcontext context,
    CGprofile profile,
    const char * code,
    const char * entry,
    const char ** args )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::CgShader, () );

    // create program
    mProgram = cgCreateProgram( context, CG_SOURCE, code, profile, entry ? entry : "main", args );
    if( !mProgram )
    {
        CGerror err;
        const char * info = cgGetLastErrorString(&err);
        GN_ERROR(sLogger)( "Fail to create program: %s", info );
        if( CG_COMPILER_ERROR == err )
        {
            GN_ERROR(sLogger)(
                "\n\n"
                "==================== Cg compile error ===============\n"
                "%s\n"
                "-----------------------------------------------------\n"
                "%s\n"
                "=====================================================\n"
                "\n",
                sAddLineCountCG( code ).cptr(),
                cgGetLastListing(context) );
        }
        return failure();
    }

    GN_VTRACE(sLogger)( "create Cg shader with profile: %s", cgGetProfileString( mProfile ) );

    // show compile result
    GN_VTRACE(sLogger)(
        "\n\n"
        "==================== Cg compile success =================\n"
        "%s\n"
        "---------------------------------------------------------\n"
        "%s\n"
        "=========================================================\n"
        "\n",
        sAddLineCountCG( code ).cptr(),
        sAddLineCountCG( cgGetProgramString( mProgram, CG_COMPILED_PROGRAM ) ).cptr() );

    // success
    mContext = context;
    mProfile = profile;
#if GN_ENABLE_DEBUG
    mCode = code;
#endif
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::CgShader::quit()
{
    GN_GUARD;

    if( 0 != mProgram )
    {
        GN_ASSERT( cgIsProgram( mProgram ) );
        cgDestroyProgram( mProgram );
        mProgram = 0;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

#endif
