#include "pch.h"
#include "cgShader.h"

#ifdef HAS_CG

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.cg");

static GN::StrA sAddLineCount( const GN::StrA & in )
{
    using namespace GN;

    GN::StrA out( "(  1) : " );

    int line = 1;
    for( const char * s = in.ToRawPtr(); *s; ++s )
    {
        if( '\n' == *s )
        {
            out.Append( StringFormat( "\n(%3d) : ", ++line ) );
        }
        else
        {
            out.Append( *s );
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
bool GN::gfx::CgShader::Init(
    CGcontext context,
    CGprofile profile,
    const StrA & code,
    const StrA & entry,
    const char ** args )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::CgShader, () );

    // create program
    mProgram = cgCreateProgram( context, CG_SOURCE, code.ToRawPtr(), profile, entry.ToRawPtr(), args );
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
                sAddLineCount( code ).ToRawPtr(),
                cgGetLastListing(context) );
        }
        return Failure();
    }

    GN_TRACE(sLogger)( "create Cg shader with profile: %s", cgGetProfileString( mProfile ) );

    // show compile result
    GN_TRACE(sLogger)(
        "\n\n"
        "==================== Cg compile success =================\n"
        "%s\n"
        "---------------------------------------------------------\n"
        "%s\n"
        "=========================================================\n"
        "\n",
        sAddLineCount( code ).ToRawPtr(),
        sAddLineCount( cgGetProgramString( mProgram, CG_COMPILED_PROGRAM ) ).ToRawPtr() );

    // success
    mContext = context;
    mProfile = profile;
    mCode = code;
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::CgShader::Quit()
{
    GN_GUARD;

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

#endif
