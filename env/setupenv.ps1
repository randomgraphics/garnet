"********************************************************************************"
"*                                                                              *"
"*                        Garnet Development Console                            *"
"*                                                                              *"
"********************************************************************************"

# ==============================================================================
# Define local functions
# ==============================================================================

function warn { write-host -ForegroundColor yellow "WARN : $args" }

function error { write-host -ForegroundColor red "ERROR : $args"; "GARNET build environment setup failed."; exit }

function catch_batch_env( $batch, $arg )
{
    $tag = "[[[===start-environment-table===]]]" # cannot have space here.
    $cmd = "`"$batch`" $arg &echo $tag& set"
    $tag_found = $false
    cmd.exe /c $cmd|foreach-object{
        if( $tag_found )
        {
            $p, $v = $_.split('=')
            Set-Item -path env:$p -value $v
        }
        elseif( $_ -eq $tag )
        {
            $tag_found = $true
        }
        else
        {
            $_
        }
    }
}

# ==============================================================================
# Define global functions
# ==============================================================================

# Define your function like this: function global:<name> (...) { .... }
function global:ccc { cmd.exe /c $args }

# redefine prompt function
function global:prompt()
{
	write-host -ForegroundColor Magenta "=== GARNET $env:GN_BUILD_COMPILER-$env:GN_BUILD_TARGET_OS-$env:GN_BUILD_TARGET_CPU $env:GN_BUILD_VARIANT ===="
	write-host -ForegroundColor Magenta "[$(get-location)]"
	return ">"
}

# ==============================================================================
# Get the root directory
# ==============================================================================

# note: $GARNET_ROOT is a global variable that could be used in other places outside of this script.
$global:GARNET_ROOT=split-path -parent $MyInvocation.InvocationName|split-path -parent
$env:GARNET_ROOT=$GARNET_ROOT

# ==============================================================================
# detect current platform
# ==============================================================================

# detect host CPU type
$current_cpu="x86"
if( ("amd64" -ieq $env:PROCESSOR_ARCHITECTURE) -or ("AMD64" -ieq $env:PROCESSOR_ARCHITEWOW64) )
{
    $current_cpu="x64"
}
elseif( "ia64" -ieq $env:PROCESSOR_ARCHITECTURE )
{
    $current_cpu="ia64"
}

# ==============================================================================
# setup build variant
# ==============================================================================

# setup default build variants
$env:GN_BUILD_COMPILER="vc"
$env:GN_BUILD_VARIANT="debug"
$env:GN_BUILD_TARGET_OS="mswin"
$env:GN_BUILD_TARGET_CPU=$current_cpu

# TODO: Parse command line (modify build variant according to command line)
foreach( $a in $args )
{
    if( ("/h" -eq $a ) -or
        ("/?" -eq $a ) -or
        ("-h" -eq $a ) -or
        ("-?" -eq $a ) -or
        ("--help" -eq $a ) )
    {
        $name = $MyInvocation.InvocationName | split-path -leaf

        "Usage: $name [/h|/?] [vc|icl|mingw] [x86|x64|xenon] [debug|profile|retail]"
    }

    elseif( ("vc" -eq $a) -or ("icl" -eq $a) )
    {
        $env:GN_BUILD_COMPILER = $a
    }

    elseif( ("x86" -eq $a) -or ("x64" -eq $a) )
    {
        $env:GN_BUILD_TARGET_CPU = $a
    }

    elseif( ("debug" -eq $a) -or
            ("profile" -eq $a) -or
            ("retail" -eq $a) )
    {
        $env:GN_BUILD_VARIANT = $a
    }

    elseif( "xenon" -eq $a )
    {
        $env:GN_BUILD_COMPILER = "xenon"
        $env:GN_BUILD_TARGET_OS = "xenon"
        $env:GN_BUILD_TARGET_CPU = "ppc"
    }

    else
    {
        warn "Unrecoginized command line argument: $a"
    }
}

# ==============================================================================
# setup Visual Studio environment
# ==============================================================================

if( "vc" -eq $env:GN_BUILD_COMPILER )
{
    ""
    "====================================="
    "Setup Visual Studio build environment"
    "====================================="
    ""

    # locate vsvarall.bat
    $vcvarbat=$false
    if( $env:VS100COMNTOOLS )
    {
        $vcvarbat="$env:VS100COMNTOOLS..\..\VC\vcvarsall.bat"
        $env:GN_BUILD_COMPILER="vc100";
    }
    elseif( $env:VS90COMNTOOLS )
    {
        $vcvarbat="$env:VS90COMNTOOLS..\..\VC\vcvarsall.bat"
        $env:GN_BUILD_COMPILER="vc90";
    }
    else
    {
        error "Neither VS100COMNTOOLS nor VS90COMNTOOLS are found. Please install VS 2010 or VS 2008"
    }

    # run vsvarall.bat, catch all environments
    if( test-path -path $vcvarbat )
    {
        $target = ""
        if( "x86" -eq $current_cpu )
        {
            if( "x86" -eq $env:GN_BUILD_TARGET_CPU )
            {
                $target = "x86"
            }
            elseif( "x64" -eq $env:GN_BUILD_TARGET_CPU )
            {
                $target = "x86_amd64"
            }
            else
            {
                error "Unsupport GN_BUILD_TARGET_CPU: $env:GN_BUILD_TARGET_CPU"
            }
        }
        elseif( "x64" -eq $current_cpu )
        {
            if( "x86" -eq $env:GN_BUILD_TARGET_CPU )
            {
                $target = "x86"
            }
            elseif( "x64" -eq $env:GN_BUILD_TARGET_CPU )
            {
                $target = "amd64"
            }
            else
            {
                error "Unsupport GN_BUILD_TARGET_CPU: $env:GN_BUILD_TARGET_CPU"
            }
        }
        else
        {
            error "Unsupport current_cpu: $current_cpu"
        }

        "Run Visual Studio build environment setup script: $vcvarbat $target"
        ""
        catch_batch_env $vcvarbat $target
    }
    else
    {
        error "File $vcvarbat not found."
    }
}

# ==============================================================================
# setup Intel C++ Compiler environment
# ==============================================================================
if( "icl" -eq $env:GN_BUILD_COMPILER )
{
    ""
    "=========================================="
    "Setup Intel C++ Compiler build environment"
    "=========================================="
    ""

    # TODO: Determine real ICL setup batch file path
    $batch = "C:\Program Files\Intel\Compiler\11.0\072\cpp\Bin\iclvars.bat"

    # call ICL setup batch file
    if( test-path -path $batch )
    {
        "Run ICL setup script: $batch"
        ""
        if( "x86" -eq $current_cpu )
        {
            if( "x86" -eq $env:GN_BUILD_TARGET_CPU )
            {
                catch_batch_env $batch ia32
            }
            elseif( "x64" -eq $env:GN_BUILD_TARGET_CPU )
            {
                catch_batch_env $batch ia32_intel64
            }
            else
            {
                error "Unsupport GN_BUILD_TARGET_CPU: $env:GN_BUILD_TARGET_CPU"
            }
        }
        elseif( "x64" -eq $current_cpu )
        {
            if( "x86" -eq $env:GN_BUILD_TARGET_CPU )
            {
                catch_batch_env $batch ia32
            }
            elseif( "x64" -eq $env:GN_BUILD_TARGET_CPU )
            {
                catch_batch_env $batch intel64
            }
            else
            {
                error "Unsupport GN_BUILD_TARGET_CPU: $env:GN_BUILD_TARGET_CPU"
            }
        }
        else
        {
            error "Unsupport current_cpu: $current_cpu"
        }
    }
    else
    {
        error "File $batch not found."
    }
}

# ==============================================================================
# setup DirectX environment
# ==============================================================================

if( "mswin" -eq $env:GN_BUILD_TARGET_OS )
{
    ""
    "==============================="
    "Setup DirectX build environment"
    "==============================="
    ""

    if( $env:DXSDK_DIR )
    {
        $batch = "${env:DXSDK_DIR}Utilities\Bin\dx_setenv.cmd"

        if( test-path -path $batch )
        {
            $target = ""

            if( "x64" -eq $env:GN_BUILD_TARGET_CPU )
            {
                $target = "amd64"
            }
            else
            {
                $target = $env:GN_BUILD_TARGET_CPU
            }

        	"Run DirectX SDK setup script: $batch $target"
            ""

            catch_batch_env $batch $target
        }
        else
        {
            warn "DiretX build environment setup failed: file $batch not found."
        }
    }
    else
    {
        # Note: Just issue a warning, instead of error, since DXSDK is not required to build garnet application.
        warn "Environment variable DXSDK_DIR not found. Please install DirectX SDK."
    }
}

# ==============================================================================
# setup GREEN path and inclusion
# ==============================================================================

#if( "x86" -eq $env:GN_BUILD_TARGET_CPU )
#{
#    if( $env:GREEN_INC_X86 ) { $env:INCLUDE = "$env:GREEN_INC_X86;$env:INCLUDE" }
#    if( $env:GREEN_LIB_X86 ) { $env:LIB     = "$env:GREEN_LIB_X86;$env:LIB" }
#}
#elseif( "x64" -eq $env:GN_BUILD_TARGET_CPU )
#{
#    if( $env:GREEN_INC_X64 ) { $env:INCLUDE = "$env:GREEN_INC_X64;$env:INCLUDE" }
#    if( $env:GREEN_LIB_X64 ) { $env:LIB     = "$env:GREEN_LIB_X64;$env:LIB" }
#}

# ==============================================================================
# setup XDK build environment
# ==============================================================================

if( "xenon" -eq $env:GN_BUILD_COMPILER )
{
    ""
	"============================="
	"Setup Xenon build environment"
	"============================="
    ""

    if( !$env:XEDK )
    {
        error "Environment XEDK not found. Please install XDK"
    }

    $batch = "$env:XEDK\bin\win32\xdkvars.bat"

    if( test-path $batch )
    {
        catch_batch_env $batch
    }
    else
    {
        error "XDK setup script $batch not found."
    }
}

# ==============================================================================
# Setup SCONS
# ==============================================================================
"
=============================
Setup SCONS build environment
=============================

SCons Directory : $GARNET_ROOT\env\scons\2.1.0.alpha.20101125
"
$env:Path = "$GARNET_ROOT\env\scons\2.1.0.alpha.20101125\script;$env:Path"
$env:SCONS_LIB_DIR = "$GARNET_ROOT\env\scons\2.1.0.alpha.20101125\engine"
$env:SCONSFLAGS="-U"

# ==============================================================================
# setup aliases
# ==============================================================================

if( Test-Path -path "$GARNET_ROOT\env\alias.txt" )
{
    # create script block for all aliases
    $aliases = ""
    get-content "$GARNET_ROOT\env\alias.txt"|foreach {
        $name, $value = $_.split(' ')

        $body = ([System.String]$value).Trim( ' "' ).Replace( "cd /d", "cd" ).Replace( '$*', '$args' )
        $body = $body.Replace( "%GARNET_ROOT%", '$GARNET_ROOT' ).Replace( "%GN_BUILD", '$env:GN_BUILD' ).Replace( "%", "" )

        $aliases = $aliases +
        "
        function global:$name {$body}
        "
    }
    $aliases = $executioncontext.InvokeCommand.NewScriptBlock( $aliases )

    # run the script
    &$aliases
}
else
{
	warn "$GARNET_ROOT\env\alias.txt is missing."
}

# ==============================================================================
# MISC
# ==============================================================================

# Setup PATH
$MY_BIN_PATH = "$GARNET_ROOT\env\bin\mswin\x86"
if( "x64" -eq $current_cpu )
{
    $MY_BIN_PATH = "$GARNET_ROOT\env\bin\mswin\x64;$MY_BIN_PATH"
}
$env:Path = "$GARNET_ROOT\env\bin\mswin\cmd;$MY_BIN_PATH;$env:Path"

# update title
$Host.UI.RawUI.WindowTitle = "garnet3d ( $GARNET_ROOT $env:GN_BUILD_COMPILER-$env:GN_BUILD_TARGET_OS-$env:GN_BUILD_TARGET_CPU $env:GN_BUILD_VARIANT )"

# change current location
cd $GARNET_ROOT

# ==============================================================================
# call user specific setup script
# ==============================================================================

if( Test-Path $GARNET_ROOT\user\$env:USERNAME.ps1 )
{
    .$GARNET_ROOT\user\$env:USERNAME.ps1
}

# ==============================================================================
# DONE
# ==============================================================================

write-host -ForegroundColor green "
================================================
Garnet build environment setup done successfully
================================================
USERNAME            = $env:USERNAME
GARNET_ROOT         = $GARNET_ROOT
GN_BUILD_COMPILER   = $env:GN_BUILD_COMPILER
GN_BUILD_VARIANT    = $env:GN_BUILD_VARIANT
GN_BUILD_TARGET_OS  = $env:GN_BUILD_TARGET_OS
GN_BUILD_TARGET_CPU = $env:GN_BUILD_TARGET_CPU
"
