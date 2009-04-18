#ifndef __GN_TEST_CMDARGS_H__
#define __GN_TEST_CMDARGS_H__
// *****************************************************************************
/// \file
/// \brief   Command line utilities for test applications
/// \author  chen@@CHENLI-HOMEPC (2009.4.17)
// *****************************************************************************

#include "garnet/GNgfx.h"

class CommandLineArguments
{
public:

    // *************************************************************************
    // public types
    // *************************************************************************

    //@{

    /// indicate command line parsing result
    enum Status
    {
        /// Command line parsing succeed. Continue application execution.
        CONTINUE_EXECUTION,

        /// User is asking for command line help.
        SHOW_HELP,

        /// Common line parsing fail.
        INVALID_COMMAND_LINE,
    };
    //@}

    // *************************************************************************
    // public data
    // *************************************************************************

    //@{

    const char             * applicationName;
    GN::gfx::RendererOptions rendererOptions;
    bool                     useMultiThreadRenderer;
    Status                   status;

    //@}

    // *************************************************************************
    // public method
    // *************************************************************************

    /// ctor
    CommandLineArguments( int argc, const char * argv[] )
        : applicationName( argv[0] )
        , useMultiThreadRenderer( false )
        , status( INVALID_COMMAND_LINE )
        , mLogger(GN::getLogger( "GN.test.CommandLineArguments" ))
    {
        status = parseCommandLine( argc, argv );
    }

    /// show command line options
    void showStandardCommandLineOptions()
    {
        GN_INFO(mLogger)(
            "Standard command line options:\n"
            "\n"
            "   -fs   [on|off]          Full screen rendering. Default is off.\n"
            "\n"
            "   -h\n"
            "   -?                      Show help.\n"
            "\n"
            "   -mt   [on|off]          Use multithread renderer. Default is off.\n"
            "\n"
            "   -rapi [auto|ogl|d3d10]  Choose rendering API. Default is AUTO.\n"
            );
    }

    /// show default help screen, assuming there's no application specific arguments
    void showDefaultHelp()
    {
        GN_INFO(mLogger)( "Usage: %s [options]\n", applicationName );
        showStandardCommandLineOptions();
    }

    // *************************************************************************
    // private data
    // *************************************************************************

private:

    GN::Logger * mLogger;

    // *************************************************************************
    // private methods
    // *************************************************************************

private:

    /// parse bool argument
    bool parseBool( bool & result, const char * option, const char * value )
    {
        using namespace GN;

        if( 0 == strCmpI( "on", value ) ||
            0 == strCmpI( "yes", value ) ||
            0 == strCmpI( "true", value ) ||
            0 == strCmpI( "1", value ) )
        {
            result = true;
            return true;
        }
        else if(
            0 == strCmpI( "off", value ) ||
            0 == strCmpI( "no", value ) ||
            0 == strCmpI( "false", value ) ||
            0 == strCmpI( "0", value ) )
        {
            result = false;
            return true;
        }
        else
        {
            GN_ERROR(mLogger)(
                "Invalid boolean argument value (%s) for option %s",
                value, option );
            return false;
        }
    }

    bool parseRendererAPI( GN::gfx::RendererAPI & result, const char * value )
    {
        using namespace GN;
        using namespace GN::gfx;

        if( 0 == strCmpI( "auto", value ) )
        {
            result = API_AUTO;
        }
        else if( 0 == strCmpI( "ogl", value ) )
        {
            result = API_OGL;
        }
        else if( 0 == strCmpI( "d3d10", value ) )
        {
            result = API_D3D10;
        }
        else
        {
            GN_ERROR(mLogger)( "invalid renderer API: %s", value );
            return false;
        }

        return true;
    }

    Status parseCommandLine( int argc, const char * argv[] )
    {
        using namespace GN;

        for( int i = 1; i < argc; ++i )
        {
            const char * a = argv[i];

            if( 0 == strCmpI( "-h", a ) ||
                0 == strCmpI( "-?", a ) )
            {
                return SHOW_HELP;
            }
            else if( '-' == *a )
            {
                // this is a command line option name

                // each option must be followed by a value.
                if( i+1 == argc || '-' == *argv[i+1] )
                {
                    GN_ERROR(mLogger)( "Argument value of option %s is missing.", a );
                    return INVALID_COMMAND_LINE;
                }

                const char * value = argv[++i];

                if( 0 == strCmpI( "-fs", a ) )
                {
                    if( !parseBool( rendererOptions.fullscreen, a, value ) )
                        return INVALID_COMMAND_LINE;
                }
                else if( 0 == strCmpI( "-mt", a ) )
                {
                    if( !parseBool( useMultiThreadRenderer, a, value ) )
                        return INVALID_COMMAND_LINE;
                }
                else if( 0 == strCmpI( "-rapi", a ) )
                {
                    if( !parseRendererAPI( rendererOptions.api, value ) )
                        return INVALID_COMMAND_LINE;
                }
            }
        }

        // done
        return CONTINUE_EXECUTION;
    }
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_TEST_CMDARGS_H__
