"********************************************************************************"
"*                                                                              *"
"*                        Garnet Development Console                            *"
"*                                                                              *"
"********************************************************************************"

# ==============================================================================
# Define local functions
# ==============================================================================

function warn { write-host -ForegroundColor yellow "WARN : $args" }

function error {
    write-host -ForegroundColor red "GARNET build environment setup failed : $args"
    stop-process $PID # kill current power shell process
}

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
# Setup Vulkan SDK
# ==============================================================================

if (-not $(test-path env:VULKAN_SDK)) {
    write-host -ForegroundColor red -NoNewline "Environment variable VULKAN_SDK is not set. Please run "
    write-host -ForegroundColor yellow  -NoNewline "install-vulkan-sdk.ps1 "
    write-host -ForegroundColor red "to download and install Vulkan SDK."
} elseif (-not $(test-path $env:VULKAN_SDK)) {
    fatal "Environment variable VULKAN_SDK is not pointing to valid folder."
}

# ==============================================================================
# Check Python support
# ==============================================================================

$py = [System.Version]$(python.exe -V).Substring(7)
if ([System.Version]"3.8.0" -gt $py) {
    warn "Python is not installed or current version ($py) is too low. Please upgrade to 3.8.0+ for best script compatibility."
}

# ==============================================================================
# Define global functions
# ==============================================================================

# Define your function like this: function global:<name> (...) { .... }
function global:ccc { cmd.exe /c $args }

# A helper function to retrieve current git branch
function global:get-git-branch {
    $branch = $(git rev-parse --abbrev-ref HEAD 2>&1)
    if ($lastExitCode -ne 0) {
        $branch = "!!!GIT ERROR: {$lastExitCode}!!!"
    }
    "$branch"
}

# redefine prompt function
function global:prompt {
    write-host -ForegroundColor Green "==== GARNET - " -NoNewline
    write-host -ForegroundColor Blue "$GARNET_ROOT" -NoNewline
    write-host -ForegroundColor Green " - " -NoNewline
    write-host -ForegroundColor Yellow "$(get-git-branch)" -NoNewline
    write-host -ForegroundColor Green " ===="
    write-host -ForegroundColor Green "[$(get-location)]"
    return ">"
}

# ==============================================================================
# Get the root directory
# ==============================================================================

# note: $GARNET_ROOT is a global variable that could be used in other places outside of this script.
$global:GARNET_ROOT=split-path -parent $PSScriptRoot
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
$env:GN_BUILD_CMAKE_GENERATOR=""
$env:GN_BUILD_TARGET_CPU=$current_cpu

# Parse command line
foreach( $a in $args )
{
    if( ("/h" -eq $a ) -or
        ("/?" -eq $a ) -or
        ("-h" -eq $a ) -or
        ("-?" -eq $a ) -or
        ("--help" -eq $a ) )
    {
        $name = $MyInvocation.InvocationName | split-path -leaf

        "Usage: $name [/h|/?] [ninja] [x86|x64]"
    }

    elseif( "ninja" -eq $a )
    {
        $env:GN_BUILD_CMAKE_GENERATOR = "Ninja"
    }

    elseif( ("x86" -eq $a) -or ("x64" -eq $a) )
    {
        $env:GN_BUILD_TARGET_CPU = $a
    }

    else
    {
        warn "Unrecoginized command line argument: $a"
    }
}

if ( "" -eq "$env:GN_BUILD_CMAKE_GENERATOR" )
{
    $env:GN_BUILD_DIR="build.msbuild.$env:GN_BUILD_TARGET_CPU"
}
else
{
    $env:GN_BUILD_DIR="build.$env:GN_BUILD_CMAKE_GENERATOR.$env:GN_BUILD_TARGET_CPU"
}

# ==============================================================================
# setup Visual Studio environment
# ==============================================================================

# Setup VS environment only when generating ninja build.
if( "Ninja" -eq $env:GN_BUILD_CMAKE_GENERATOR )
{
    ""
    "====================================="
    "Setup Visual Studio build environment"
    "====================================="
    ""

    # locate vsvarall.bat
    $vcvarbat=$false
    if( test-path "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" )
    {
        $vcvarbat="C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
        $env:GN_BUILD_COMPILER="vc170";
    }
    elseif( test-path "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" )
    {
        $vcvarbat="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
        $env:GN_BUILD_COMPILER="vc170";
    }
    else
    {
        error "Visual Studio 2022 Community/Professional is required."
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

<# not needed for cmake to work

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
# setup git
# ==============================================================================

git config --local include.path ${GARNET_ROOT}/.gitconfig

# ==============================================================================
# MISC
# ==============================================================================

# Setup PATH
$MY_BIN_PATH = "$GARNET_ROOT\env\bin\mswin\x86"
if( "x64" -eq $current_cpu )
{
    $MY_BIN_PATH = "$GARNET_ROOT\env\bin\mswin\x64;$MY_BIN_PATH"
}
$env:Path = "$GARNET_ROOT\env\bin;$env:Path"

# update title
$Host.UI.RawUI.WindowTitle = "garnet3d ( $GARNET_ROOT )"

# change current location
set-location $GARNET_ROOT

#reset some command line color
Set-PSReadlineOption -Colors @{
        "Parameter" = [System.ConsoleColor]::White
        "Operator" = [System.ConsoleColor]::White
}

# ==============================================================================
# DONE
# ==============================================================================

write-host -ForegroundColor green "

Garnet build environment ready to use. Happy coding!

GARNET_ROOT              = $env:GARNET_ROOT
GN_BUILD_CMAKE_GENERATOR = $env:GN_BUILD_CMAKE_GENERATOR
GN_BUILD_TARGET_CPU      = $env:GN_BUILD_TARGET_CPU
GN_BUILD_DIR             = $env:GN_BUILD_DIR
VULKAN_SDK               = $env:VULKAN_SDK
"
