#ifndef __GN_TEST_TESTAPP_H__
#define __GN_TEST_TESTAPP_H__
// *****************************************************************************
/// \file
/// \brief   Common header for test applications
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
    GN::gfx::GpuOptions      rendererOptions;
    bool                     useMultiThreadGpu;
    Status                   status;

    // command line arguments that are not recoganized by the parser
    size_t                   extraArgc;
    const char     * const * extraArgv;

    GN::Logger             * logger;

    //@}

    // *************************************************************************
    // public method
    // *************************************************************************

    /// ctor
    CommandLineArguments( int argc, const char * argv[] )
        : applicationName( argv[0] )
        , useMultiThreadGpu( !GN_POSIX )
        , status( INVALID_COMMAND_LINE )
        , extraArgc(0)
        , extraArgv(NULL)
        , logger(GN::getLogger( "GN.test.CommandLineArguments" ))
    {
        status = parseCommandLine( argc, argv );

        if( CONTINUE_EXECUTION == status )
        {
            extraArgc = mExtraArgs.size();
            extraArgv = mExtraArgs.rawptr();
        }
    }

    /// show command line options
    void showStandardCommandLineOptions() const
    {
        GN_INFO(logger)(
            "Standard command line options:\n"
            "\n"
            "   -dm   [f|b|w]          Set Display Mode: fullscreen|borderless|windowed. Default is windowed.\n"
            "\n"
            "   -h\n"
            "   -?                     Show help.\n"
            "\n"
            "   -mt   [on|off]         Use multithread renderer. Default is on.\n"
            "\n"
            "   -gpu [auto|ogl|d3d11]  Choose GPU API. Default is AUTO.\n"
            "\n"
            "   -vsync [on|off]        Enable/Disable vsync. Default is off.\n"
            );
    }

    /// show default help screen, assuming there's no application specific arguments
    void showDefaultHelp() const
    {
        using namespace GN;

        StrA executableName = fs::baseName( applicationName ) + fs::extName( applicationName );

        GN_INFO(logger)( "Usage: %s [options]\n", executableName.rawptr() );
        showStandardCommandLineOptions();
    }

    // *************************************************************************
    // private data
    // *************************************************************************

private:

    GN::DynaArray<const char*> mExtraArgs;

    // *************************************************************************
    // private methods
    // *************************************************************************

private:

    /// parse bool argument
    bool parseBool( bool & result, const char * option, const char * value )
    {
        using namespace GN;

        if( 0 == str::compareI( "on", value ) ||
            0 == str::compareI( "yes", value ) ||
            0 == str::compareI( "true", value ) ||
            0 == str::compareI( "1", value ) )
        {
            result = true;
            return true;
        }
        else if(
            0 == str::compareI( "off", value ) ||
            0 == str::compareI( "no", value ) ||
            0 == str::compareI( "false", value ) ||
            0 == str::compareI( "0", value ) )
        {
            result = false;
            return true;
        }
        else
        {
            GN_ERROR(logger)(
                "Invalid boolean argument value (%s) for option %s",
                value, option );
            return false;
        }
    }

    int parseStrings(const char * option, const char * value, const char * strings[], size_t count )
    {
        using namespace GN;

        for(size_t i = 0; i < count; ++i) {
            if( 0 == str::compareI( strings[i], value ) ) return (int)i;
        }

        GN_ERROR(logger)(
            "Invalid argument value (%s) for option %s",
            value, option );
        return -1;
    }

    bool parseGpuAPI( GN::gfx::GpuAPI & result, const char * value )
    {
        using namespace GN;
        using namespace GN::gfx;

        if( 0 == str::compareI( "auto", value ) )
        {
            result = GpuAPI::AUTO;
        }
        else if( 0 == str::compareI( "ogl", value ) )
        {
            result = GpuAPI::OGL;
        }
        else if( 0 == str::compareI( "d3d11", value ) )
        {
            result = GpuAPI::D3D11;
        }
        else
        {
            GN_ERROR(logger)( "invalid renderer API: %s", value );
            return false;
        }

        return true;
    }

    const char * getOptionValue( int argc, const char * argv[], int & i )
    {
        if( i+1 == argc || '-' == *argv[i+1] )
        {
            GN_ERROR(logger)( "Argument value of option %s is missing.", argv[i] );
            return NULL;
        }

        return argv[++i];
    }

    Status parseCommandLine( int argc, const char * argv[] )
    {
        using namespace GN;

        for( int i = 1; i < argc; ++i )
        {
            const char * a = argv[i];

            if( 0 == str::compareI( "-h", a ) ||
                0 == str::compareI( "/h", a ) ||
                0 == str::compareI( "-?", a ) ||
                0 == str::compareI( "/?", a ) ||
                0 == str::compareI( "--help", a ) )
            {
                return SHOW_HELP;
            }
            else if( '-' == *a
                #if GN_MSWIN
                || '/' == *a
                #endif
                )
            {
                // this is a command line option name

                if( 0 == str::compareI( "dm", a+1 ) )
                {
                    const char * value = getOptionValue( argc, argv, i );
                    if( NULL == value ) return INVALID_COMMAND_LINE;

                    const char * MODES[] = { "f", "b", "w" };
                    rendererOptions.displayMode.mode = (gfx::DisplayMode::Mode)parseStrings(a, value, MODES, _countof(MODES));
                    if( rendererOptions.displayMode.mode < 0)
                        return INVALID_COMMAND_LINE;
                }
                else if( 0 == str::compareI( "mt", a+1 ) )
                {
                    const char * value = getOptionValue( argc, argv, i );
                    if( NULL == value ) return INVALID_COMMAND_LINE;

                    if( !parseBool( useMultiThreadGpu, a, value ) )
                        return INVALID_COMMAND_LINE;
                }
                else if( 0 == str::compareI( "gpu", a+1 ) )
                {
                    const char * value = getOptionValue( argc, argv, i );
                    if( NULL == value ) return INVALID_COMMAND_LINE;

                    if( !parseGpuAPI( rendererOptions.api, value ) )
                        return INVALID_COMMAND_LINE;
                }
                else if( 0 == str::compareI( "vsync", a+1 ) )
                {
                    const char * value = getOptionValue( argc, argv, i );
                    if( NULL == value || !parseBool( rendererOptions.vsync, a, value ) )
                        return INVALID_COMMAND_LINE;
                }
                else
                {
                    // this is an extra option
                    mExtraArgs.append( a );
                }
            }
            else
            {
                // this is an extra argument
                mExtraArgs.append( a );
            }
        }

        // done
        return CONTINUE_EXECUTION;
    }
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_TEST_TESTAPP_H__
