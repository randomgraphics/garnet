#!/usr/bin/python

import sys, os, os.path, re, fnmatch

# ------------------------------------------------------------------------------
def UTIL_getenv( name, defval = '' ):
    if name in os.environ: return os.environ[name]
    else: return defval

# ------------------------------------------------------------------------------
def UTIL_trace( level, msg ):
    level = float(level)
    assert( 0 != level )
    if ( CONF_trace > 0 and level <= CONF_trace ) or ( CONF_trace < 0 and level == -CONF_trace ):
        print 'TRACE(%d) : %s'%(level,msg)

# ------------------------------------------------------------------------------
def UTIL_info( msg ): print 'INFO : %s'%msg

# ------------------------------------------------------------------------------
def UTIL_warn( msg ):
    #print '===================================================================='
    print 'WARNING : %s'%msg
    #print '===================================================================='

# ------------------------------------------------------------------------------
def UTIL_error( msg ):
    print '===================================================================='
    print 'ERROR : %s'%msg
    print '===================================================================='

# ------------------------------------------------------------------------------
# Fatal Error. Need to halt.
def UTIL_fatal( msg ):
    print '===================================================================='
    print 'FATAL : %s'%msg
    print '===================================================================='
    Exit(-1)

# ------------------------------------------------------------------------------
# Start of main procedure

DXSDK_ROOT_PATH = UTIL_getenv("DXSDK_DIR")
if '' == DXSDK_ROOT_PATH: UTIL_fatal('Environment variable DXSDK_DIR not found. Make sure that DXSDK is properly installed.')
DXSDK_INC_PATH = os.path.join( DXSDK_ROOT_PATH, 'include')
D3D11_H_PATH = os.path.join( DXSDK_INC_PATH, "d3d11.h" )
