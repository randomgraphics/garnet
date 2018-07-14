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

<# not needed for cmake to work

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

        "Usage: $name [/h|/?] [vc|icl|mingw] [x86|x64] [debug|profile|retail]"
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

    elseif( ("durango" -eq $a) -or ("xbox3" -eq $a) )
    {
        $env:GN_BUILD_COMPILER = "vc"
        $env:GN_BUILD_TARGET_OS = "xbox3"
        $env:GN_BUILD_TARGET_CPU = "x64"
    }

    else
    {
        warn "Unrecoginized command line argument: $a"
    }
}

# ==============================================================================
# setup Visual Studio environment
# ==============================================================================

# skip VS setup on Xbox One platform.
if( ("vc" -eq $env:GN_BUILD_COMPILER) -and ("xbox3" -ne $env:GN_BUILD_TARGET_OS) )
{
    ""
    "====================================="
    "Setup Visual Studio build environment"
    "====================================="
    ""

    # locate vsvarall.bat
    $vcvarbat=$false
    if( test-path "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" )
    {
        $vcvarbat="C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"
        $env:GN_BUILD_COMPILER="vc150";
    }
    elseif( test-path "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" )
    {
        $vcvarbat="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
        $env:GN_BUILD_COMPILER="vc150";
    }
    else
    {
        error "Visual Studio 2017 Community/Professional is required."
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
    $batch = "C:\Program Files\Intel\ComposerXE-2011\bin\iclvars.bat"

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
# setup Windows SDK environment
# ==============================================================================

function LookForWindowsSDK($version)
{
    $result = ""

    $program = ""
    if( "" -ne "${env:ProgramFiles(x86)}" )
    {
        $program = "${env:ProgramFiles(x86)}"
    }
    else
    {
        $program = "${env:ProgramFiles}"
    }

    if( test-path "$program\Windows Kits\${version}\Include" )
    {
        $result = "$program\Windows Kits\${version}"
    }
    elseif( test-path "$program\Microsoft SDKs\Windows\${version}\Include" )
    {
        $result = "$program\Microsoft SDKs\Windows\${version}"
    }

    return $result
}

if( "mswin" -eq $env:GN_BUILD_TARGET_OS )
{
    ""
    "============================="
    "Setup Windows SDK environment"
    "============================="
    ""
    $winsdk_root = LookForWindowsSDK("8.1");
    if( "" -eq $winsdk_root )
    {
        $winsdk_root = LookForWindowsSDK("v7.1A");
    }

    if( "" -ne $winsdk_root )
    {
        "Windows SDK found: $winsdk_root"
        ""
        ${env:INCLUDE} = "${env:INCLUDE};$winsdk_root\Include"
        if( "x86" -eq $env:GN_BUILD_TARGET_CPU )
        {
            ${env:LIB} = "${env:LIB};$winsdk_root\Lib"
        }
        elseif( "x64" -eq $env:GN_BUILD_TARGET_CPU )
        {
            ${env:LIB} = "${env:LIB};$winsdk_root\Lib\x64"
        }
    }
    else
    {
        "Windows SDK is not insalled."
        ""
    }
}

# ==============================================================================
# setup Vulkan SDK
# ==============================================================================

if( "mswin" -eq $env:GN_BUILD_TARGET_OS )
{
    ""
    "=============================="
    "Setup Vulkan build environment"
    "=============================="
    ""

    if( $env:VULKAN_SDK -and ( test-path $env:VULKAN_SDK ) )
    {
        "Vulkan SDK found: $env:VULKAN_SDK"
        ${env:INCLUDE} = "${env:INCLUDE};${env:VULKAN_SDK}\Include"
        if( "x86" -eq $env:GN_BUILD_TARGET_CPU )
        {
            ${env:LIB} = "${env:LIB};${env:VULKAN_SDK}\Lib32"
        }
        elseif( "x64" -eq $env:GN_BUILD_TARGET_CPU )
        {
            ${env:LIB} = "${env:LIB};${env:VULKAN_SDK}\Lib"
        }
    }
    else
    {
        warn "Environment variable VULKAN_SDK is not defined or pointing to invalid directory."
    }
}

# ==============================================================================
# setup Qt build environment
# ==============================================================================

if( "mswin" -eq $env:GN_BUILD_TARGET_OS )
{
    ""
    "=========================="
    "Setup Qt build environment"
    "=========================="
    ""
    # TODO: search for Qt
    $qt=""
    if( (test-path "C:\Qt\5.10.0\msvc2017_64") -and ("x64" -eq $env:GN_BUILD_TARGET_CPU ) )
    {
        "Qt runtime found: C:\Qt\5.10.0\msvc2017_64"
        $qt = "C:\Qt\5.10.0\msvc2017_64"
    }
    else
    {
        warn "No Qt runtime found."
    }

    if( "" -ne $qt )
    {
        ${env:INCLUDE} = "${env:INCLUDE};$qt\Include"
        ${env:LIB} = "${env:LIB};$qt\Lib"
        ${env:Path} = "${env:Path};$qt\Bin"
    }
}

# ==============================================================================
# setup Xbox One build environment
# ==============================================================================

if( "xbox3" -eq $env:GN_BUILD_TARGET_OS )
{
    ""
	"================================"
	"Setup Xbox One build environment"
	"================================"
    ""

    if( !$env:DurangoXDK )
    {
        error "Environment DurangoXDK not found. Please install Durango XDK"
    }

    $batch = "$env:DurangoXDK\xdk\DurangoVars.cmd"

    if( test-path $batch )
    {
        catch_batch_env $batch XDK
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
"

$SCONS_DIR= "$GARNET_ROOT\env\scons\3.0.0"
$env:SCONS_LIB_DIR    = "$SCONS_DIR\engine"
$env:SCONS_SCRIPT_DIR = "$SCONS_DIR\script\"
$env:SCONSFLAGS="-U"

"SCons Directory : $SCONS_DIR"

#>

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
$Host.UI.RawUI.WindowTitle = "garnet3d ( $GARNET_ROOT )"

# change current location
set-location $GARNET_ROOT

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
"
