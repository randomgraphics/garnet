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
# Function classes

class FunctionParameter:
    def __init__( self, type, name, defval_ = '' ) :
        t = type.strip()
        m = re.match(r"(__\w+\(.+\))\s+(.+$)",t)
        if m is None:
            m = re.match(r"(__\w+)\s+(.+$)", t)
        if m is not None:
            #print 't=', t, 'sal=', m.group(1), 'type=', m.group(2)
            self._sal = m.group(1)
            self._type = m.group(2)
        else:
            self._sal = ''
            self._type = t
        self._name = name.strip()
        self._defval = defval_

class FunctionSignature:
    def __init__( self, prefix, return_type, decl, name ) :
        self._prefix = prefix
        self._return_type = return_type
        self._decl = decl
        self._name = name
        self._parameter_list = []

    def AddParameter( self, param ):
        #print param
        if isinstance(param, FunctionParameter):
            self._parameter_list.append(param)
        else:
            UTIL_warn('"param" is not a FunctionParameter')

    def WriteCID( self, interface_name ):
        cid_file.write('    CID_' + interface_name + '_' + self._name + ',\n')

    def WriteParameterList(self, fp, writeType, writeName, makeRef = False, newLine = False):
        for i in range(len(self._parameter_list)):
            if newLine: fp.write('    ')
            if writeType:
                fp.write(self._parameter_list[i]._type)
                if makeRef:
                    # TODO: fp.write(' &') wont' work, if the paramter is already a ref type.
                    pass
            if writeName:
                if writeType: fp.write(' ')
                fp.write(self._parameter_list[i]._name)
            if i < (len(self._parameter_list) - 1):
                if newLine:
                    fp.write(',\n')
                else:
                    fp.write(', ')
        pass; # end-of-for

    def WriteParameterNameList(self, fp):
        self.WriteParameterList(fp, writeType=False, writeName=True)

    def WriteParameterTypeList(self, fp, makeRef = False):
        self.WriteParameterList(fp, writeType=True, writeName=False, makeRef=makeRef)

    def WriteImplementationToFile( self, fp, class_name ):
        fp.write('// -----------------------------------------------------------------------------\n')
        fp.write(self._return_type + ' ' + self._decl + ' ' + class_name + '::' + self._name + '(')
        if len(self._parameter_list) > 0:
            fp.write('\n')
            for i in range(len(self._parameter_list)):
                fp.write('    ' + self._parameter_list[i]._type + ' ' + self._parameter_list[i]._name)
                if i < (len(self._parameter_list) - 1):
                    fp.write(',\n')
                else:
                    fp.write(')\n')
        else:
            fp.write(')\n')
        fp.write('{\n')

        # call _xxx_pre_ptr(...)
        fp.write('    if (_' + self._name + '_pre_ptr._value) { (this->*_' + self._name + '_pre_ptr._value)(')
        self.WriteParameterNameList(fp)
        fp.write('); }\n')

        # call the real function
        if 'void' == self._return_type:
            fp.write('    GetRealObj()->' + self._name + '(')
        else:
            fp.write('    ' + self._return_type + ' ret = GetRealObj()->' + self._name + '(')
        self.WriteParameterNameList(fp)
        fp.write(');\n')

        # call xxx_POST(...)
        fp.write('    if (_' + self._name + '_post_ptr._value) { (this->*_' + self._name + '_post_ptr._value)(')
        if 'void' != self._return_type:
            fp.write('ret')
            if len(self._parameter_list) > 0:
                fp.write(', ')
        self.WriteParameterNameList(fp)
        fp.write('); }\n')

        if 'void' != self._return_type:
            fp.write('    return ret;\n')

        fp.write('}\n\n')
        pass; # end-of-func

    def WriteMetaDataToFile( self, fp ):
        fp.write('// -----------------------------------------------------------------------------\n')
        fp.write('DEFINE_INTERFACE_METHOD(' + self._prefix + ', ' + self._return_type + ', ' + self._decl + ', ' + self._name + ', PARAMETER_LIST_' + str(len(self._parameter_list)) + '(')
        if len(self._parameter_list) > 0:
            fp.write('\n')
            for i in range(len(self._parameter_list)):
                fp.write('    DEFINE_METHOD_PARAMETER(' + self._parameter_list[i]._type + ', ' + self._parameter_list[i]._name + ')')
                if i < (len(self._parameter_list) - 1):
                    fp.write(',\n')
                else:
                    fp.write('))\n')
        else:
            fp.write('))\n')

    def WritePrototypeToFile( self, fp, class_name ):
        fp.write('// -----------------------------------------------------------------------------\n')
        fp.write(self._prefix + ' ' + self._return_type + ' ' + self._decl + ' ' + self._name + '(')
        if len(self._parameter_list) > 0:
            fp.write('\n')
            for i in range(len(self._parameter_list)):
                fp.write('    ' + self._parameter_list[i]._type + ' ' + self._parameter_list[i]._name)
                if i < (len(self._parameter_list) - 1):
                    fp.write(',\n')
                else:
                    fp.write(');\n')
        else:
            fp.write(');\n')
        # write prototype for pre method
        fp.write('NullPtr<void (' + class_name + '::*)(')
        self.WriteParameterTypeList(fp, makeRef=True)
        fp.write(')> _' + self._name + '_pre_ptr;\n')
        # write prototype for post method
        fp.write('NullPtr<void (' + class_name + '::*)(')
        if 'void' != self._return_type:
            fp.write(self._return_type)
            if len(self._parameter_list) > 0:
                fp.write(', ')
        self.WriteParameterTypeList(fp)
        fp.write(')> _' + self._name + '_post_ptr;\n')


# ------------------------------------------------------------------------------
# Returns instance of FunctionSignature or None
def PARSE_get_interface_method_decl( line ):
    m = re.match(r"virtual (\w+) STDMETHODCALLTYPE (\w+)", line)
    if m is not None:
        #print 'm = ', m.group(0)
        return FunctionSignature('virtual', m.group(1), 'STDMETHODCALLTYPE', m.group(2))
    else:
        return None

# ------------------------------------------------------------------------------
# Returns FunctionParameter or None
def PARSE_get_func_parameter( line ):
    m = re.match(r"(.+[^\w])(\w+),$", line)
    if m is None:
        m = re.match(r"(.+[^\w])(\w+)\) = 0;$", line)
    if m is not None:
        #print 'm[1] = "',m.group(1),'" m[2] = "',m.group(2),'"'
        return FunctionParameter(m.group(1), m.group(2))
    else:
        return None

# ------------------------------------------------------------------------------
# Parse interface definition, generate c++ declarations
def PARSE_interface( class_name, include, lines ):
    interface_name = 'I' + class_name
    UTIL_info( "Parsing " + interface_name + "...");
    start_line = interface_name + " : public IUnknown"

    found = False
    ended = False
    methods = []
    func_sig = None

    for l in lines:
        if  l == start_line: found = True
        elif found and l == '};': ended = True
        elif found and (not ended):
            temp_func_sig = PARSE_get_interface_method_decl(l)
            if temp_func_sig is not None:
                # write function parameter, if it is not written yet.
                if func_sig is not None: methods.append(func_sig)
                func_sig = temp_func_sig
            elif (func_sig is not None) and len(l) > 0:
                func_param = PARSE_get_func_parameter(l)
                if func_param is not None:
                    func_sig.AddParameter(func_param)
            elif (func_sig is not None):
                methods.append(func_sig)
                func_sig = None
            pass
        pass # end-of-for

    # Handle the last methd.
    if func_sig is not None:
        methods.append(func_sig)
        func_sig = None

    # write methods to file
    if found and ended:
        with open(interface_name + "_meta.h", "w") as f:
            for m in methods: m.WriteMetaDataToFile( f )
        with open(interface_name + ".h", "w") as f:
            for m in methods: m.WritePrototypeToFile( f, class_name )
        with open(interface_name + ".cpp", "w") as f:
            f.write('#include "pch.h"\n')
            f.write('#include "' + include + '"\n')
            for m in methods: m.WriteImplementationToFile( f, class_name )
        cid_file.write('\n')
        cid_file.write('    // CID for ' + interface_name + '\n')
        cid_file.write('    CID_ID3D11DEVICE_BASE,\n'),
        cid_file.write('    CID_ID3D11DEVICE_COUNT = ' + str(len(methods)) + ',\n'),
        cid_file.write('    CID_ID3D11DEVICE_AddRef = CID_ID3D11DEVICE_BASE,\n'),
        cid_file.write('    CID_ID3D11DEVICE_Release,\n'),
        cid_file.write('    CID_ID3D11DEVICE_QueryInterface,\n'),
        for m in methods: m.WriteCID(interface_name)

    # end of the function
    pass

# ------------------------------------------------------------------------------
# Parse d3d11.h
def PARSE_d3d11_h( f ):
    lines = [line.strip() for line in f.readlines()]
    PARSE_interface('D3D11Device', 'device.h', lines)


# ------------------------------------------------------------------------------
# Start of main procedure

DXSDK_ROOT_PATH = UTIL_getenv("DXSDK_DIR")
if '' == DXSDK_ROOT_PATH: UTIL_fatal('Environment variable DXSDK_DIR not found. Make sure that DXSDK is properly installed.')
DXSDK_INC_PATH = os.path.join( DXSDK_ROOT_PATH, 'include')

# open global CID file
cid_file = open("d3d11cid.h", "w")
cid_file.write(
"""// Define call ID for all D3D11 and DXGI methods.
#pragma once

enum D3D11_CALL_ID
{""")

with open(os.path.join( DXSDK_INC_PATH, "d3d11.h" )) as f: PARSE_d3d11_h(f)


# close global CID file
cid_file.write('};\n')
cid_file.close();
