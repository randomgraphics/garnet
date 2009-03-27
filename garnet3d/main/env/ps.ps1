"********************************************************************************"
"*                                                                              *"
"*                        Garnet Development Console                            *"
"*                                                                              *"
"********************************************************************************"

# ======================
# Define local functions
# ======================

function warn { "WARN : $args" }

function error { "ERROR : $args" }

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

# =======================
# Define global functions
# =======================

function global:ccc {cmd.exe /c $args} # run cmd.exe

# =============================
# TODO: detect current platform
# =============================
$current_cpu="x86"

$global:GARNET_ROOT=split-path -parent $MyInvocation.InvocationName|split-path -parent

# ===================
# setup build variant
# ===================

# setup default build variants
$env:GN_BUILD_COMPILER="vc80"
$env:GN_BUILD_VARIANT="debug"
$env:GN_BUILD_TARGET_OS="mswin"
$env:GN_BUILD_TARGET_CPU="x86"

# TODO: Parse command line (modify build variant according to command line)

# show final build variant
"
GN_BUILD_COMPILER   = $env:GN_BUILD_COMPILER
GN_BUILD_VARIANT    = $env:GN_BUILD_VARIANT
GN_BUILD_TARGET_OS  = $env:GN_BUILD_TARGET_OS
GN_BUILD_TARGET_CPU = $env:GN_BUILD_TARGET_CPU
"

# ===============================
# setup Visual Studio environment
# ===============================

"
=====================================
Setup Visual Studio build environment
=====================================
"

# locate vsvarall.bat
$vcvarbat=$false
if( $env:VS90COMNTOOLS )
{
    $vcvarbat="$env:VS90COMNTOOLS..\..\VC\vcvarsall.bat"
}
else
{
    error "Environment variable VS90COMNTOOLS not found. Please install VS 2008"
    return -1
}


# run vsvarall.bat, catch all environments
if( test-path -path $vcvarbat )
{
    "Run Visual Studio build environment setup script: $vcvarbat"
    if( "x86" -eq $current_cpu )
    {
        if( "x86" -eq $env:GN_BUILD_TARGET_CPU )
        {
            catch_batch_env $vcvarbat "x86"
        }
        elseif( "x64" -eq $env:GN_BUILD_TARGET_CPU )
        {
            catch_batch_env $vcvarbat "x86_amd64"
        }
        else
        {
            error "Unsupport GN_BUILD_TARGET_CPU: $env:GN_BUILD_TARGET_CPU"
            return -1
        }
    }
    else
    {
        error "Unsupport current_cpu: $current_cpu"
        return -1
    }
}
else
{
    error "File $vcvarbat not found."
    return -1
}

# =========================
# setup DirectX environment
# =========================

"
===============================
Setup DirectX build environment
===============================
"
if( $env:DXSDK_DIR )
{
    $batch = "${env:DXSDK_DIR}Utilities\Bin\dx_setenv.cmd"
 
    if( test-path -path $batch )
    {
    	¡°Run DirectX SDK setup script: $batch¡±
    
        if( "x64" -eq $env.GN_BUILD_TARGET_CPU )
        {
            catch_batch_env $batch "amd64"
        }
        else
        {
            catch_batch_env $batch $env.GN_BUILD_TARGET_CPU
        }
    }
    else
    {
        warning "DiretX build environment setup failed: file $batch not found."
    }
}
else
{
    # Note: Just issue a warning, instead of error, since DXSDK is not required to build garnet application.
    warning "Environment variable DXSDK_DIR not found. Please install DirectX SDK."
}

# =================================
# TODO: setup XDK build environment
# =================================

# =============================
# TODO: Setup GREEN environment
# =============================


# ==========
# Setup PATH
# ==========
$MY_BIN_PATH = "$GARNET_ROOT\env\bin\mswin\x86"
if( "x64" -eq $current_cpu )
{
    $MY_BIN_PATH = "$GARNET_ROOT\env\bin\mswin\x64;$MY_BIN_PATH"
}
$env:Path = "$GARNET_ROOT\env\bin\mswin\cmd;$MY_BIN_PATH;$env:Path"

# ===========
# Setup SCONS
# ===========
"
=============================
Setup SCONS build environment
=============================

SCons Directory : $GARNET_ROOT\env\scons\1.2.0
"
$env:Path = "$GARNET_ROOT\env\scons\1.2.0\Scripts;$env:Path"
$env:SCONS_LIB_DIR = "$GARNET_ROOT\env\scons\1.2.0\Lib"

# =============
# setup aliases
# =============

if( Test-Path -path "$GARNET_ROOT\env\alias.txt" )
{
    # create script block for all aliases
    $aliases = ""
    get-content "C:\Users\chenli\gamedev\garnet3d\main\env\alias.txt"|foreach {
        $name, $value = $_.split(' ')
        
        $body = ([System.String]$value).Trim( ' "' ).Replace( "cd /d", "cd" ).Replace( '$*', '$args' )
        $body = $body.Replace( "%GARNET_ROOT%", '$GARNET_ROOT' ).Replace( "%GN_BUILD", '$env:GN_BUILD' ).Replace( "%", "" ) 
        
        $aliases = $aliases +
        "
        function global:$name {$body}
        "
    }
    $aliases = [System.Management.Automation.ScriptBlock]::Create( $aliases )

    # run the script
    &$aliases
}
else
{
	warning "$GARNET_ROOT\env\alias.txt is missing."
}

# ====
# DONE
# ====
"
================================================
Garnet build environment setup done successfully
================================================
"
