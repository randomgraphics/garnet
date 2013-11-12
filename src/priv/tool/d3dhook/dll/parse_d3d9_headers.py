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
    def __init__( self, interface_name, method_name, type, name, immediate_array_count = 0, variant_array_count = None, defval_ = '' ) :
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
        self._immediate_array_count = immediate_array_count # 0 means non-array parameter
        self._variant_array_count = variant_array_count # None means not an variant array.

    def IsImmediateArray( self ):
        return self._immediate_array_count > 0

    def IsRef( self ):
        return '&' == self._type[-1:] or \
               self._type.find('REFGUID') >= 0 or \
               self._type.find('REFIID') >= 0

    def IsHookedInterface( self ):
        return self._type.find('IDirect3D') >= 0 or \
               None

    def IsOutput( self ) :
        result = '**' == self._type[-2:]
        return result

class D3D9Method:
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
            p = self._parameter_list[i]
            if p.IsHookedInterface() and p.IsOutput():
                fp.write('    if ( ' + p._name + ' && *' + p._name + ') { *' + p._name + ' = RealToHooked9( *' + p._name + ' ); }\n')
        pass; # end-of-for

    def WriteParameterList(self, fp, writeType, writeName, makeRef = False, newLine = False, convertHookedPtr = None):
        if newLine and len(self._parameter_list):
            fp.write('\n')
        for i, param in enumerate(self._parameter_list):
            if newLine: fp.write('    ')
            if writeType:
                fp.write(param._type)
                if makeRef and not param.IsRef():
                    if param.IsImmediateArray():
                        fp.write(' *')
                    else:
                        fp.write(' &')
            if writeName:
                if writeType: fp.write(' ')
                convert = convertHookedPtr and param.IsHookedInterface() and (not param.IsOutput())
                if convert:
                    #if 'ID3D9DeviceContext' == param._interface_name and 'VSSetConstantBuffers' == param._method_name: print param._type
                    if isinstance(param._variant_array_count, str):
                        fp.write('HookedToReal(' + param._variant_array_count + ', ')
                    else:
                        fp.write('HookedToReal(')
                fp.write(param._name)
                if convert: fp.write(')')
            if writeType and param.IsImmediateArray() and (not makeRef):
                fp.write(' [' + param._immediate_array_count + ']')
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

# ------------------------------------------------------------------------------
# Interface class

class D3D9Interface:

    def __init__( self, interface_name, parent_name, methods ) :
        self._name = interface_name
        self._parent = parent_name
        self._methods = methods

    def FindMethod(self, name, searchParents = True):
        for m in self._methods:
            if m._name == name: return m
        if searchParents and g_interfaces[self._parent]:
            parent = g_interfaces[self._parent]
            if parent: return parent.FindMethod( name, searchParents )
        return None

# ------------------------------------------------------------------------------
# Returns instance of D3D9Method or None
def PARSE_get_interface_method_decl( interface_name, line ):
    m = re.match(r"STDMETHOD\((\w+)\)", line)
    if m is not None:
        return D3D9Method('virtual', 'HRESULT', 'STDMETHODCALLTYPE', m.group(1), interface_name)
    else:
        m = re.match(r"STDMETHOD_\((\w+), (\w+)\)", line)
        if m is not None:
            return D3D9Method('virtual', m.group(1), 'STDMETHODCALLTYPE', m.group(2), interface_name)
    return None

# ------------------------------------------------------------------------------
# Returns FunctionParameter or None
def PARSE_get_func_parameters( interface_name, method_name, line ):
    m = re.match(r".+\(THIS_ (.+)\)|.+\(THIS()\)", line)
    if not m:
        UTIL_error('unrecognized line: ' + line)
        return []
    if not m.group(1):
        # this is expected. Means no arguments
        return []
    result = []
    param_list = m.groups(1)[0].strip()
    for p in param_list.split(','):
        i1 = p.rfind(' ')
        i2 = p.rfind('*')
        i3 = p.rfind('&')
        i = max(i1,max(i2,i3))
        if i < 0 or '&' == p[-1:] or '*' == p[-1:]:
            UTIL_error('parameter name is missing: ' + line)
        ptype = p[:i+1].strip()
        pname = p[i+1:]
        result.append(FunctionParameter(interface_name, method_name, ptype, pname))
        pass
    return result

def PARSE_get_parent_class(text):
    m = re.match(r"DECLARE_INTERFACE_\(\w+, (\w+)", text)
    if m is not None:
        return m.group(1)
    else:
        return None

# ------------------------------------------------------------------------------
# Parse interface definition, generate c++ declarations
#   interface_name : name of the interface that you want to parse
#   include        : include this header file in generated .h file
#   lines          : the source code that you want to parse.
def PARSE_interface( interface_name, lines ):
    UTIL_info( '    Parse ' + interface_name);

    assert not (interface_name in g_interfaces)

    start_line = 'DECLARE_INTERFACE_(' + interface_name + ','
    found = False
    ended = False
    methods = []
    parent_class_name = None
    parent_class = None

    for l in lines:
        if  -1 != l.find(start_line):
            parent_class_name = PARSE_get_parent_class(l)
            if None != parent_class_name:
                if not parent_class_name in g_interfaces:
                    UTIL_warn('    Parent class ' + parent_class_name + ' has not been parsed yet.')
                parent_class = g_interfaces[parent_class_name]
                found = True
        elif found and l == '};': ended = True
        elif found and (not ended):
            func_sig = PARSE_get_interface_method_decl(interface_name, l)
            parent_methods = []
            ancestor = parent_class;
            while ancestor:
                for m in ancestor._methods:
                    parent_methods.append(m._name)
                ancestor = g_interfaces[ancestor._parent] if ancestor._parent else None
            if func_sig and ('QueryInterface' != func_sig._name) and (not func_sig._name in parent_methods):
                func_params = PARSE_get_func_parameters(interface_name, func_sig._name, l)
                for p in func_params: func_sig.AddParameter(p)
                methods.append(func_sig)
        pass # end-of-for

    if found and ended:
        # We have successfully parsed the interface. Put the interface into the global mapping table.
        g_interfaces[interface_name] = D3D9Interface(interface_name, parent_class_name, methods)
    elif not found:
        UTIL_error(interface_name + ' not found!')
    else:
        UTIL_error('The end of ' + interface_name + ' not found!')

    # end of the function
    pass

class D3D9VTable:
    def __init__(self, interface_name, lines):
        self._name = interface_name
        self._code = lines
        self._methods = self._ParseMethodName( interface_name, lines )

    def _ParseMethodName( self, interface_name, lines ):
        names = []
        for l in lines:
            if -1 !=  l.find('STDMETHOD_'):
                m = re.match(r"STDMETHOD_\(\w+,\s*(\w+)\)", l)
                names += [m.group(1)]
            elif -1 != l.find('STDMETHOD'):
                m = re.match(r"STDMETHOD\((\w+)\)", l)
                names += [m.group(1)]
        return names

class D3D9VTableFile:
    def __init__(self):
        self._header = open('d3d9vtable.inl', 'w')
        self._header.write('// script generated file. Do _NOT_ edit.\n\n')
        self._cpp = open('d3d9vtable.cpp', 'w')
        self._cpp.write('// script generated file. Do _NOT_ edit.\n\n'
                        '#include "pch.h"\n'
                        '#include "d3d9vtable.h"\n\n'
                        'D3D9VTables g_D3D9OriginVTables;\n'
                        'D3D9VTables g_D3D9HookedVTables;\n'
                        '\n')
        self._vtables = []

    def _WriteRealToHookFunc(self, fp, vtable ):
        interface_name = vtable._name
        methods = vtable._methods
        vtable_name = interface_name + 'Vtbl'
        fp.write('// -----------------------------------------------------------------------------\n'
                 'inline void RealToHooked9_' + interface_name + '(' + interface_name + ' * p)\n'
                 '{\n'
                 '    if (p) RealToHooked_General(**(' + vtable_name + '**)p, g_D3D9OriginVTables._' + interface_name + ', g_D3D9HookedVTables._' + interface_name + ', "' + interface_name + '");\n'
                 '}\n'
                 'template <> inline void RealToHooked9<' + interface_name + '>(' + interface_name + ' * p)\n'
                 '{\n'
                 '    return RealToHooked9_' + interface_name + '( p );\n'
                 '}\n'
                 '\n')
        pass # end of function

    def _WriteAddRef( self, fp, interface_name ):
        fp.write('// -----------------------------------------------------------------------------\n'
                 'template<UINT INDEX> static ULONG STDMETHODCALLTYPE ' + interface_name + '_AddRef_Hooked(' + interface_name + ' * ptr)\n'
                 '{\n'
                 '    calltrace::AutoTrace trace("' + interface_name + '::AddRef");\n'
                 '    return g_D3D9OriginVTables._' + interface_name + '.tables[INDEX].AddRef(ptr);\n'
                 '}\n\n');
        pass

    def _WriteRelease( self, fp, interface_name ):
        fp.write('// -----------------------------------------------------------------------------\n'
                 'template<UINT INDEX> static ULONG STDMETHODCALLTYPE ' + interface_name + '_Release_Hooked(' + interface_name + ' * ptr)\n'
                 '{\n'
                 '    calltrace::AutoTrace trace("' + interface_name + '::Release");\n'
                 '    return g_D3D9OriginVTables._' + interface_name + '.tables[INDEX].Release(ptr);\n'
                 '}\n\n');

    def _WriteQI( self, fp, interface_name ):
        fp.write('// -----------------------------------------------------------------------------\n'
                 'template<UINT INDEX> static HRESULT STDMETHODCALLTYPE ' + interface_name + '_QueryInterface_Hooked(' + interface_name + ' * ptr, const IID & iid, void ** pp)\n'
                 '{\n'
                 '    calltrace::AutoTrace trace("' + interface_name + '::QueryInterface");\n'
                 '    return g_D3D9OriginVTables._' + interface_name + '.tables[INDEX].QueryInterface(ptr, iid, pp);\n'
                 '}\n\n');

    def _WriteHookMethod( self, fp, interface_name, method_name ):
        if 'AddRef' == method_name:
            self._WriteAddRef(fp, interface_name)
        elif 'Release' == method_name:
            self._WriteRelease(fp, interface_name)
        elif 'QueryInterface' == method_name:
            self._WriteQI(fp, interface_name)
        else:
            m = g_interfaces[interface_name].FindMethod(method_name)
            fp.write('// -----------------------------------------------------------------------------\n'
                     'template<UINT INDEX> static ' + m._return_type + ' ' + m._decl + ' ' + interface_name + '_' + m._name + '_Hooked(' + interface_name + ' * ptr')
            if len(m._parameter_list) > 0: fp.write(', ')
            m.WriteParameterList(fp, writeType=True, writeName=True)
            decl_return = '' if 'void' == m._return_type else (m._return_type + ' result = ')
            func_return = '' if 'void' == m._return_type else '    return result;\n'
            fp.write(')\n'
                     '{\n'
                     '    calltrace::AutoTrace trace("' + interface_name + '::' + m._name + '");\n'
                     '    ' + decl_return + 'g_D3D9OriginVTables._' + interface_name + '.tables[INDEX].' + m._name + '(ptr')
            if len(m._parameter_list) > 0: fp.write(', ')
            m.WriteParameterNameList(fp)
            fp.write(');\n')
            for p in m._parameter_list:
                if p.IsHookedInterface() and p.IsOutput():
                    fp.write('    if (' + p._name + ' && *' + p._name + ') { RealToHooked9( *' + p._name + ' ); }\n')
            fp.write(func_return +
                     '}\n'
                     '\n');


    def Close(self):
        # write to header file
        self._header.write('// -----------------------------------------------------------------------------\n'
                           '// Global vtables for all D3D9/DXGI classes\n'
                           '// -----------------------------------------------------------------------------\n'
                           '\n'
                           'struct D3D9VTables\n'
                           '{\n')
        for vt in self._vtables:
            self._header.write('    VTable<' + vt._name + 'Vtbl> _' + vt._name + ';\n')
        self._header.write('};\n\n'
                           'extern D3D9VTables g_D3D9OriginVTables;\n'
                           'extern D3D9VTables g_D3D9HookedVTables;\n'
                           '\n'
                           '// -----------------------------------------------------------------------------\n'
                           '// Real -> Hook Functions\n'
                           '// -----------------------------------------------------------------------------\n'
                           '\n')
        for vt in self._vtables:
            self._WriteRealToHookFunc(self._header, vt)
        self._header.close()
        self._header = None

        # write to cpp file
        self._cpp.write('// -----------------------------------------------------------------------------\n'
                        'void RealToHooked9(const IID & iid, void * p)\n'
                        '{\n'
                        '    if (false) {}\n')
        for vt in self._vtables:
            self._cpp.write('    else if (__uuidof(' + vt._name + ') == iid) RealToHooked9_' + vt._name + '((' + vt._name + '*)p);\n')
        self._cpp.write('    else\n'
                        '    {\n'
                        '        HOOK_WARN_LOG("unrecognized interface UUID: <xxxx-xxxx-xxxxx...>");\n'
                        '    }\n'
                        '}\n'
                        '\n'
                        '// -----------------------------------------------------------------------------\n'
                        'template<UINT INDEX> static void SetupD3D9HookedVTables()\n'
                        '{\n');
        for vt in self._vtables:
            for m in vt._methods:
                self._cpp.write('    g_D3D9HookedVTables._' + vt._name + '.tables[INDEX].' + m + ' = ' + vt._name + '_' + m + '_Hooked<INDEX>;\n')
        self._cpp.write('}\n\n'
                        '// -----------------------------------------------------------------------------\n'
                        'void SetupD3D9HookedVTables()\n'
                        '{\n')
        for i in range(16):
            self._cpp.write('    SetupD3D9HookedVTables<' + str(i) + '>();\n')
        self._cpp.write('}\n')
        self._cpp.close()
        self._cpp = None
        pass

    def WriteVtable( self, interface_name, lines ):

        # parse vtable definition
        vtable = D3D9VTable(interface_name, lines)
        methods = vtable._methods
        self._vtables.append( vtable )

        # generate vtable decl
        self._header.write('// -----------------------------------------------------------------------------\n'
                           '// ' + interface_name + '\n'
                           '// -----------------------------------------------------------------------------\n'
                           'struct ' + interface_name + 'Vtbl\n'
                           '{\n'
                           '    BEGIN_INTERFACE\n')
        for l in lines:
            if -1 != l.find('STDMETHOD_'):
                m = re.match(r"STDMETHOD_\((\w+),\s*(\w+)\)", l)
                l = m.group(1) + ' (STDMETHODCALLTYPE *' + m.group(2) + ')' + l[m.end(0):]
            elif -1 != l.find('STDMETHOD'):
                m = re.match(r"STDMETHOD\((\w+)\)", l)
                l = 'HRESULT (STDMETHODCALLTYPE *' + m.group(1) + ')' + l[m.end(0):]
            else:
                continue
            l = l.replace('THIS_', interface_name + ' * this_, ')
            l = l.replace('THIS', interface_name + ' * this_')
            l = l.replace('PURE;', ';')
            l = l.strip()
            self._header.write('    ' + l + '\n')
        self._header.write('    END_INTERFACE\n'
                           '};\n\n')

        # individula hook methods
        self._cpp.write('// -----------------------------------------------------------------------------\n'
                        '// ' + interface_name + '\n'
                        '// -----------------------------------------------------------------------------\n'
                        '\n')
        for m in methods:
            self._WriteHookMethod(self._cpp, interface_name, m)

        # end of function
        pass

# ------------------------------------------------------------------------------
# Parse interface vtable, generate c++ declarations
#   interface_name : name of the interface that you want to parse
#   include        : include this header file in generated .h file
#   lines          : the source code that you want to parse.
def PARSE_d3d9_vtable( interface_name, lines ):
    start_line = 'DECLARE_INTERFACE_(' + interface_name;
    end_line = '};'
    found = None
    ended = None

    definition = []

    for l in lines:
        if not found:
            if  -1 != l.find(start_line):
                found = True
                definition += [l]
        else:
            definition += [l]
            if -1 != l.find(end_line):
                ended = True
                break
        pass # end-of-for

    # write vtable to file
    if found and ended:
        g_d3d9vtables.WriteVtable(interface_name, definition)
    elif not found:
        UTIL_error(interface_name + ' not found!')
    else:
        UTIL_error('The end of ' + interface_name + ' not found: ' + end_line)

    # end of the function
    pass

# ------------------------------------------------------------------------------
# Parse a list of interfaces in an opened file
def PARSE_interfaces_from_opened_file(file, interfaces):
    UTIL_info( 'Parse ' + file.name);

    lines = [line.strip() for line in file.readlines()]

    all = GatherAllInterfaces(lines, file.name)

    if 0 == len(interfaces): interfaces = all

    for interface_name in interfaces:
        PARSE_interface(interface_name, lines)
        PARSE_d3d9_vtable(interface_name, lines)
    pass

# ------------------------------------------------------------------------------
# Gather all interfaces in a file
def GatherAllInterfaces(lines, sourceFileName):
    interfaces = []
    for l in lines:
        m = re.match(r"DECLARE_INTERFACE_\((\w+)\s*,\s*(\w+)\)", l)
        if m is None: continue
        interfaces += [m.group(1)]
    return interfaces

# ------------------------------------------------------------------------------
# Start of main procedure
g_d3d9vtables = D3D9VTableFile()

# interface->hook list
g_interfaces = dict()
g_interfaces['IUnknown'] = D3D9Interface('IUnknown', None, [])

# parse d3d9.h
with open( 'd3d/d3d9.h' ) as f:
    PARSE_interfaces_from_opened_file(f, [])

# close opened files
g_d3d9vtables.Close()
