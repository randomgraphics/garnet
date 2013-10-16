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
def UTIL_info( msg ): print '[INFO] %s'%msg

# ------------------------------------------------------------------------------
def UTIL_warn( msg ):
    print '===================================================================='
    print '[WARN] %s'%msg
    print '===================================================================='

# ------------------------------------------------------------------------------
def UTIL_error( msg ):
    print '===================================================================='
    print '[ERROR] %s'%msg
    print '===================================================================='

# ------------------------------------------------------------------------------
# Fatal Error. Need to halt.
def UTIL_fatal( msg ):
    print '===================================================================='
    print '[FATAL] %s'%msg
    print '===================================================================='
    Exit(-1)

# ------------------------------------------------------------------------------
# Function classes

class FunctionParameter:
    def __init__( self, interface_name, method_name, type, name, array_count = 0, defval_ = '' ) :
        t = type.strip()
        m = re.match(r"(__\w+\(.+\))\s+(.+$)", t)
        if m is None:
            m = re.match(r"(__\w+)\s+(.+$)", t)
        if m is not None:
            #print 't=', t, 'sal=', m.group(1), 'type=', m.group(2)
            self._sal = m.group(1)
            self._type = m.group(2)
        else:
            self._sal = ''
            self._type = t
        self._interface_name = interface_name
        self._method_name = method_name
        self._name = name.strip()
        self._defval = defval_
        self._array_count = array_count # 0 means non-array parameter

    def IsArray( self ):
        return self._array_count > 0

    def IsRef( self ):
        return '&' == self._type[-1:] or \
               self._type.find('REFGUID') >= 0 or \
               self._type.find('REFIID') >= 0

    def IsHookedInterface( self ):
        return self._type.find(' ID3D11') >= 0 or \
               self._type.find(' IDXGI') >= 0 or \
               'IDXGIOutput' == self._interface_name and 'FindClosestMatchingMode' == self._method_name and 'pConcernedDevice' == self._name or \
               'IDXGIOutput' == self._interface_name and 'TakeOwnership' == self._method_name and 'pDevice' == self._name or \
               None

    def IsOutput( self ) :
        return '_Out_' == self._type[:5] or \
               '&&' == self._type[-2:] >= 0

class FunctionSignature:
    def __init__( self, prefix, return_type, decl, name, interface_name ) :
        self._prefix = prefix
        self._return_type = return_type
        self._decl = decl
        self._name = name
        self._interface_name = interface_name
        self._parameter_list = []

    def AddParameter( self, param ):
        #print param
        if isinstance(param, FunctionParameter):
            self._parameter_list.append(param)
        else:
            UTIL_warn('"param" is not a FunctionParameter')

    # Convert output D3D pointer to hooked object pointer
    def ConvertOuputParameters(self, fp):
        for i in range(len(self._parameter_list)):
            if self._parameter_list[i].IsHookedInterface() and self._parameter_list[i].IsOutput():
                fp.write('    if (' + self._parameter_list[i]._name + ') *' + self._parameter_list[i]._name + ' = RealToHooked( *' + self._parameter_list[i]._name + ' );\n')
        pass; # end-of-for

    def WriteParameterList(self, fp, writeType, writeName, makeRef = False, newLine = False, convertHookedPtr = None):
        if newLine and len(self._parameter_list):
            fp.write('\n')
        for i in range(len(self._parameter_list)):
            if newLine: fp.write('    ')
            if writeType:
                fp.write(self._parameter_list[i]._type)
                if makeRef and not self._parameter_list[i].IsRef():
                    if self._parameter_list[i].IsArray():
                        fp.write(' *')
                    else:
                        fp.write(' &')
            if writeName:
                if writeType: fp.write(' ')
                convert = convertHookedPtr and self._parameter_list[i].IsHookedInterface() and (not self._parameter_list[i].IsOutput())
                if convert: fp.write('HookedToReal(')
                fp.write(self._parameter_list[i]._name)
                if convert: fp.write(')')
            if writeType and self._parameter_list[i].IsArray() and (not makeRef):
                fp.write(' [' + self._parameter_list[i]._array_count + ']')
            if i < (len(self._parameter_list) - 1):
                if newLine:
                    fp.write(',\n')
                else:
                    fp.write(', ')
        pass; # end-of-for

    def WriteParameterNameList(self, fp, convertHookedPtr = None):
        self.WriteParameterList(fp, writeType=False, writeName=True, convertHookedPtr = convertHookedPtr)

    def WriteParameterTypeList(self, fp, makeRef = False):
        self.WriteParameterList(fp, writeType=True, writeName=False, makeRef=makeRef)

    def WriteImplementationToFile( self, fp, class_name ):
        fp.write('// -----------------------------------------------------------------------------\n')
        fp.write(self._return_type + ' ' + self._decl + ' ' + class_name + '::' + self._name + '(')
        if len(self._parameter_list) > 0:
            fp.write('\n')
            for i in range(len(self._parameter_list)):
                fp.write('    ' + self._parameter_list[i]._type + ' ' + self._parameter_list[i]._name)
                if self._parameter_list[i].IsArray():
                    fp.write('[' + self._parameter_list[i]._array_count + ']')
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
        self.WriteParameterNameList(fp, convertHookedPtr = True)
        fp.write(');\n')

        # handle output parameters
        self.ConvertOuputParameters(fp);

        # call xxx_post_ptr(...)
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
                if self._parameter_list[i].IsArray():
                    fp.write('    DEFINE_METHOD_ARRAY_PARAMETER(' + self._parameter_list[i]._type + ', ' + self._parameter_list[i]._name + ', ' + self._parameter_list[i]._array_count + ')')
                else:
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
        self.WriteParameterList(fp, writeType=True, writeName=True, newLine=True)
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
        self.WriteParameterTypeList(fp, makeRef=False)
        fp.write(')> _' + self._name + '_post_ptr;\n')

    def WriteCallBaseToFile( self, fp, interface_name, class_name ):
        fp.write('// -----------------------------------------------------------------------------\n')
        fp.write(self._return_type + ' ' + self._decl + ' ' + self._name + '(')
        if len(self._parameter_list) > 0:
            fp.write('\n')
            for i in range(len(self._parameter_list)):
                fp.write('    ' + self._parameter_list[i]._type + ' ' + self._parameter_list[i]._name)
                if self._parameter_list[i].IsArray():
                    fp.write('[' + self._parameter_list[i]._array_count + ']')
                if i < (len(self._parameter_list) - 1):
                    fp.write(',\n')
                else:
                    fp.write(')\n')
        else:
            fp.write(')\n')
        fp.write('{\n')

        # call base method
        fp.write('    _' + interface_name[1:] + '.' + self._name + '(')
        self.WriteParameterNameList(fp)
        fp.write(');\n')

        #end of function call
        fp.write('}\n');
        pass; # end-of-func


# ------------------------------------------------------------------------------
# Interface class

class InterfaceSigature:

    def __init__( self, interface_name, hooked_class_name, methods ) :
        self._name = interface_name
        self._hookedClassName = hooked_class_name
        self._methods = methods

# ------------------------------------------------------------------------------
# Returns instance of FunctionSignature or None
def PARSE_get_interface_method_decl( interface_name, line ):
    m = re.match(r"virtual (\w+) STDMETHODCALLTYPE (\w+)", line)
    if m is not None:
        #print 'm = ', m.group(0)
        return FunctionSignature('virtual', m.group(1), 'STDMETHODCALLTYPE', m.group(2), interface_name)
    else:
        return None

# ------------------------------------------------------------------------------
# Returns FunctionParameter or None
def PARSE_get_func_parameter( interface_name, method_name, line ):
    # __in_opt  const FLOAT parameter,
    m = re.match(r"(.+[^\w])(\w+),$", line)
    if m is not None:
        #print 'm[1] = "',m.group(1),'" m[2] = "',m.group(2),'"'
        return FunctionParameter(interface_name, method_name, m.group(1), m.group(2))

    # __in  const UINT Value) = 0;
    m = re.match(r"(.+[^\w])(\w+)\) = 0;$", line)
    if m is not None:
        #print 'm[1] = "',m.group(1),'" m[2] = "',m.group(2),'"'
        return FunctionParameter(interface_name, method_name, m.group(1), m.group(2))

    # __in_opt  const FLOAT BlendFactor[ 4 ],
    m = re.match(r"(.+[^\w])(\w+)\[\s*(\w+)\s*\],$", line)
    if m is not None:
        #print 'm[1] = "',m.group(1),'" m[2] = "',m.group(2),'" m[3] = "',m.group(3),'"'
        return FunctionParameter(interface_name, method_name, m.group(1), m.group(2), array_count=m.group(3))

    #  __in  const FLOAT ColorRGBA[ 4 ]) = 0;
    m = re.match(r"(.+[^\w])(\w+)\[\s*(\w+)\s*\]\) = 0;$", line)
    if m is not None:
        #print 'm[1] = "',m.group(1),'" m[2] = "',m.group(2),'" m[3] = "',m.group(3),'"'
        return FunctionParameter(interface_name, method_name, m.group(1), m.group(2), array_count=m.group(3))

    # comment line
    m = re.match(r"/\*.*", line)
    if m is not None:
        return None

    UTIL_error('Unrecognized function parameter line: ' + line)
    return None

def ParseParentClass(text):
    m = re.match(r"\w+ : public (\w+)", text)
    if m is not None:
        return m.group(1)
    else:
        return None

# ------------------------------------------------------------------------------
# Get list of parent classes (not including IUnknown)
#   interface_name : name of the interface that you want to parse
def GetParentInterfaceList(interface_name):
    parents = []
    p = g_parents[interface_name]
    while (p and ('IUnknown' != p)):
        parents = [p] + parents
        p = g_parents[p]
    return parents

# ------------------------------------------------------------------------------
# Write standard hook methods to file
#   interface_name : name of the interface that you want to parse
#   class_name     : name of the hook class
def WriteStandardHookMethodsToFile(f, interface_name, class_name):
    f.write('// ==============================================================================\n' \
            '// Constructor / Destructor\n' \
            '// ==============================================================================\n' \
            'private:\n\n');
    parents = GetParentInterfaceList(interface_name)
    for p in parents:
        f.write(g_interfaces[p]._hookedClassName + ' & _' + p[1:] + ';\n')
    f.write('\nprotected:\n\n' +
            class_name + '(UnknownBase & unknown, ');
    for p in parents:
        f.write(g_interfaces[p]._hookedClassName + ' & ' + p[1:] + ', ')
    f.write('IUnknown * realobj)\n' \
            '    : BASE_CLASS(unknown, realobj)\n')
    for p in parents:
        f.write('    , _' + p[1:] + '(' + p[1:] + ')\n')
    f.write('{\n' \
            '    unknown.AddInterface<' + interface_name + '>(this, realobj);\n' \
            '    Construct(); \n'
            '}\n\n' \
            '~' + class_name + '() {}\n\n'
            );
    pass

# ------------------------------------------------------------------------------
# Parse interface definition, generate c++ declarations
#   interface_name : name of the interface that you want to parse
#   class_name     : name of the hook class
def WriteCallBaseToFile(f, interface_name, class_name):
    f.write('// ==============================================================================\n' \
            '// Calling to base interfaces\n' \
            '// ==============================================================================\n' \
            'public:\n\n')
    for p in GetParentInterfaceList(interface_name):
        for m in g_interfaces[p]._methods: m.WriteCallBaseToFile( f, p, class_name )

# ------------------------------------------------------------------------------
# Parse interface definition, generate c++ declarations
#   interface_name : name of the interface that you want to parse
#   include        : include this header file in generated .h file
#   lines          : the source code that you want to parse.
def PARSE_interface( interface_name, include, lines ):
    UTIL_info( '    Parse ' + interface_name);

    assert not (interface_name in g_interfaces)

    class_name = interface_name[1:] + 'Hook' # name of the hook class

    start_line = interface_name + ' : public'
    found = False
    ended = False
    methods = []
    func_sig = None
    parent_class = None

    for l in lines:
        if  -1 != l.find(start_line):
            parent_class = ParseParentClass(l)
            if None != parent_class:
                if not parent_class in g_interfaces:
                    UTIL_warn('    Parent class ' + parent_class + ' has not been parsed yet.')
                found = True
        elif found and l == '};': ended = True
        elif found and (not ended):
            temp_func_sig = PARSE_get_interface_method_decl(interface_name, l)
            if temp_func_sig is not None:
                # write function parameter, if it is not written yet.
                if func_sig is not None: methods.append(func_sig)
                func_sig = temp_func_sig
            elif (func_sig is not None) and len(l) > 0:
                func_param = PARSE_get_func_parameter(interface_name, func_sig._name, l)
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
        g_parents[interface_name] = parent_class;
        with open(interface_name + "_meta.h", "w") as f:
            f.write('// script generated file. DO NOT edit.\n\n')
            for m in methods: m.WriteMetaDataToFile( f )
        with open(interface_name + ".h", "w") as f:
            f.write('// script generated file. DO NOT edit.\n\n')
            WriteStandardHookMethodsToFile(f, interface_name, class_name)
            WriteCallBaseToFile(f, interface_name, class_name)
            f.write('// ==============================================================================\n' \
                    '// Method Prototypes\n'
                    '// ==============================================================================\n' \
                    'public:\n\n')
            for m in methods: m.WritePrototypeToFile( f, class_name )
            f.write('\n' \
                    '// ==============================================================================\n' \
                    '// The End\n'
                    '// ==============================================================================\n' \
                    'private:')
        with open(interface_name + ".cpp", "w") as f:
            f.write('// script generated file. DO NOT edit.\n\n')
            f.write('#include "pch.h"\n')
            f.write('#include "' + include + '"\n\n')
            for m in methods: m.WriteImplementationToFile( f, class_name )
        g_cid.BeginInterface(interface_name, len(methods))
        """
        TODO: parent class?
        g_cid._header.write('    CID_' + interface_name + '_AddRef = CID_' + interface_name + '_BASE,\n')
        g_cid._header.write('    CID_' + interface_name + '_Release,\n')
        g_cid._header.write('    CID_' + interface_name + '_QueryInterface,\n')
        if 'ID3D11DeviceChild' == parent_class:
            UTIL_info('    Write out extra methods of ' + parent_class)
            g_cid._header.write('    CID_' + interface_name + '_GetDevice,\n')
            g_cid._header.write('    CID_' + interface_name + '_GetPrivateData,\n')
            g_cid._header.write('    CID_' + interface_name + '_SetPrivateData,\n')
            g_cid._header.write('    CID_' + interface_name + '_SetPrivateDataInterface,\n')
        elif 'IDXGIObject' == parent_class:
            UTIL_info('    Write out extra methods of ' + parent_class)
            g_cid._header.write('    CID_' + interface_name + '_SetPrivateData,\n')
            g_cid._header.write('    CID_' + interface_name + '_SetPrivateDataInterface,\n')
            g_cid._header.write('    CID_' + interface_name + '_GetPrivateData,\n')
            g_cid._header.write('    CID_' + interface_name + '_GetParent,\n')
        elif 'IUnknown' != parent_class:
            UTIL_warn('    Unrecoginized parent class: ' + parent_class)
        """
        for idx, m in enumerate(methods): g_cid.WriteMethod(interface_name, idx, interface_name + '_' + m._name)

		# We have successfully parsed the interface. Put the interface -> wrapp mapping
        # into the global mapping table.
        g_interfaces[interface_name] = InterfaceSigature(interface_name, class_name, methods)
    elif not found:
        UTIL_error(interface_name + ' not found!')
    else:
        UTIL_error('The end of ' + interface_name + ' not found!')

    # end of the function
    pass

# ------------------------------------------------------------------------------
# Parse a list of interfaces in an opened file
def PARSE_interfaces_from_opened_file(file, common_include_header, interfaces):
    UTIL_info( 'Parse ' + file.name);
    lines = [line.strip() for line in file.readlines()]
    for interface_name in interfaces:
        PARSE_interface(interface_name, common_include_header, lines)
    pass

# ------------------------------------------------------------------------------
# Get list of interface that an interface can QI (not including parent interfaces)
def GetQIList(interface_name):
    for l in g_qi :
        for i in l :
            if i == interface_name:
                return l
    return [interface_name]

# ------------------------------------------------------------------------------
# Generate implementation class for a interface
#  f         : file
#  interface : the interface
def WriteImplClass(f, interface):
    if 'IUnknown' == interface._name : return

    interface_names = []
    qi      = GetQIList(interface._name)
    for q in qi:
        parents = GetParentInterfaceList(q)
        for p in parents:
            if 0 == interface_names.count(p):
                interface_names.append(p)
        if 0 == interface_names.count(q) : interface_names.append(q)
    implClassName = interface._name[1:] + 'Impl'
    f.write('// -----------------------------------------------------------------------------\n' \
            'class ' + implClassName + '\n'
            '    : public UnknownBase\n')
    for p in interface_names:
        f.write('    , public ' + g_interfaces[p]._hookedClassName + '\n')
    f.write('{\n'
            'public:\n'
            '    ' + implClassName + '(IUnknown * realobj)\n')
    for index, name in enumerate(interface_names):
        f.write('        ' + (':' if 0 == index else ',') + ' ' + g_interfaces[name]._hookedClassName + '(*(UnknownBase*)this')
        parents = GetParentInterfaceList(name);
        for p in parents:
            f.write( ', *(' + g_interfaces[p]._hookedClassName + '*)this')
        f.write(', realobj)\n')
    f.write('    {\n'
            '    }\n'
            '};\n\n'
            )
    pass

# ------------------------------------------------------------------------------
# Call ID code generator
class CallIDCodeGen:
    def __init__( self ) :
        self._header = open("d3d11cid_def.h", "w")
        self._header.write(
"""// (Script generated header. DO NOT EDIT.)
// Define call ID for all D3D11 and DXGI methods.
#pragma once

enum D3D11_CALL_ID
{
""")
        self._source = open("d3d11cid_def.cpp", "w")
        self._source.write(
"""// (Script generated header. DO NOT EDIT.)
#include "pch.h"
#include "d3d11cid_def.h"
const char * g_D3D11CallIDText[] =
{
""")
        self._method = []
        pass

    def BeginInterface(self, interface, count):
        self._header.write('\n')
        self._header.write('    // CID for ' + interface + '\n')
        self._header.write('    CID_' + interface + '_BASE,\n'),
        self._header.write('    CID_' + interface + '_COUNT = ' + str(count) + ',\n'),
        self._source.write('\n')
        self._source.write('    // CID for ' + interface + '\n')

    def WriteMethod(self, interface, index, method):
        self._header.write('    CID_' + method + ' = CID_' + interface + '_BASE + ' + str(index) + ',\n')
        self._source.write('    "CID_' + method + '",\n')

    def Close(self) :
        self._header.write("""
    CID_TOTAL_COUNT,
    CID_INVALID = 0xFFFFFFFF,
}; // end of enum definition

extern const char * const g_D3D11CallIDText;
""")
        self._header.close();
        self._source.write('};\n')
        self._source.close();
        pass


# ------------------------------------------------------------------------------
# Start of main procedure

# open global CID files
g_cid = CallIDCodeGen()

# interface->hook list
g_interfaces = dict()
g_interfaces['IUnknown'] = InterfaceSigature('IUnknown', 'UnknownHook', [])

# interface parent list
g_parents = dict()
g_parents['IUnknown'] = None

# list of interfaces that could QI among them
g_qi = [
    ['IDXGIDevice','ID3D11Device','ID3D11Debug','ID3D11InfoQueue'],
]

# define the

# parse d3d11.h
with open( 'd3d/d3d11.h' ) as f:
    PARSE_interfaces_from_opened_file(f, 'hooks.h', [
        'ID3D11Device',
        'ID3D11DeviceChild',
        'ID3D11DeviceContext',
    ])

# parse d3d11sdklayer.h
with open( 'd3d/d3d11sdklayers.h' ) as f:
    PARSE_interfaces_from_opened_file(f, 'hooks.h', [
        'ID3D11Debug',
        'ID3D11InfoQueue',
    ])

# parse dxgi.h
with open( 'd3d/dxgi.h' ) as f:
    PARSE_interfaces_from_opened_file(f, 'hooks.h', [
        'IDXGIObject',
        'IDXGIFactory',
        'IDXGIFactory1',
        'IDXGIDeviceSubObject',
        'IDXGISurface',
        'IDXGIOutput',
        'IDXGIAdapter',
        'IDXGIAdapter1',
        'IDXGIDevice',
        'IDXGISwapChain',
    ])

# Generate implementation classes
with open("implementations.h", "w") as f:
    f.write('// script generated file. DO NOT edit.\n\n'
            '#include "pch.h"\n'
            '#include "hooks.h"\n'
            '#pragma warning(disable: 4355) // \'this\' : used in base member initializer list\n\n'
            )

    for k,v in g_interfaces.iteritems(): WriteImplClass(f, v)

# close Call ID code gen
g_cid.Close()
