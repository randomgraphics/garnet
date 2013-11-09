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
    def __init__( self, interface_name, method_name, type, name, immediate_array_count = 0, variant_array_count = None) :
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
        self._immediate_array_count = immediate_array_count # 0 means non-array parameter
        self._variant_array_count = variant_array_count # None means not an variant array.

    def IsImmediateArray( self ):
        return self._immediate_array_count > 0

    def IsRef( self ):
        return '&' == self._type[-1:] or \
               self._type.find('REFGUID') >= 0 or \
               self._type.find('REFIID') >= 0

    # list parameters that we know we don't need to hook
    def __KnownUninterestedInterfaceParameter( self ):
        result = 'SetPrivateDataInterface' == self._method_name or \
                 'IDXGIFactory2' == self._interface_name and 'CreateSwapChainForCoreWindow' == self._method_name and 'pWindow' == self._name or \
                 None
        return result

    def IsHookedInterface( self ):
        result = self._type.find(' ID3D') >= 0 or \
                 self._type.find(' IDXGI') >= 0 or \
                 'ID3D11TracingDevice' == self._interface_name and 'SetShaderTrackingOptions' == self._method_name and 'pShader' == self._name or \
                 'IDXGIDevice' == self._interface_name and 'QueryResourceResidency' == self._method_name and 'ppResources' == self._name or \
                 'IDXGIOutput' == self._interface_name and 'FindClosestMatchingMode' == self._method_name and 'pConcernedDevice' == self._name or \
                 'IDXGIOutput' == self._interface_name and 'TakeOwnership' == self._method_name and 'pDevice' == self._name or \
                 'IDXGIOutput1' == self._interface_name and 'FindClosestMatchingMode1' == self._method_name and 'pConcernedDevice' == self._name or \
                 'IDXGIOutput1' == self._interface_name and 'DuplicateOutput' == self._method_name and 'pDevice' == self._name or \
                 'IDXGIFactory' == self._interface_name and 'CreateSwapChain' == self._method_name and 'pDevice' == self._name or \
                 'IDXGIFactory2' == self._interface_name and 'CreateSwapChainForHwnd' == self._method_name and 'pDevice' == self._name or \
                 'IDXGIFactory2' == self._interface_name and 'CreateSwapChainForCoreWindow' == self._method_name and 'pDevice' == self._name or \
                 'IDXGIFactory2' == self._interface_name and 'CreateSwapChainForComposition' == self._method_name and 'pDevice' == self._name or \
                 None
        if (not result) and (not self.__KnownUninterestedInterfaceParameter()) and self._type.find('IUnknown') >= 0:
            UTIL_warn('Possible undetected interface parameter: ' + self._interface_name + '::' + self._method_name + '(' + self._name + ')');
        return result

    def IsOutput( self ) :
        result = '_Out' == self._type[:4]
        #if result: print self._interface_name + '::' + self._method_name, self._type, self._name
        return result

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
            p = self._parameter_list[i]
            if p.IsHookedInterface() and p.IsOutput():
                fp.write('    if ( ' + p._name + ' && *' + p._name + ') { *' + p._name + ' = RealToHooked11( *' + p._name + ' ); }\n')
            # special case for IDXGIObject.GetParent()
            elif 'IDXGIObject' == p._interface_name and 'GetParent' == p._method_name and 'ppParent' == p._name or \
                 'IDXGIDeviceSubObject' == p._interface_name and 'GetDevice' == p._method_name and 'ppDevice' == p._name:
                fp.write('    if (SUCCEEDED(ret)) { *' + p._name + ' = RealToHooked11(riid, (IDXGIObject*)*' + p._name + ' ); }\n')
            # special case for OpenSharedResource
            elif 'ID3D11Device' == p._interface_name and 'OpenSharedResource' == p._method_name and 'ppResource' == p._name :
                fp.write('    if (SUCCEEDED(ret)) { *' + p._name + ' = RealToHooked11(ReturnedInterface, (IDXGIResource*)*' + p._name + ' ); }\n')
            elif 'ID3D11Device1' == p._interface_name and 'OpenSharedResource1' == p._method_name and 'ppResource' == p._name or \
                 'ID3D11Device1' == p._interface_name and 'OpenSharedResourceByName' == p._method_name and 'ppResource' == p._name :
                fp.write('    if (SUCCEEDED(ret)) { *' + p._name + ' = RealToHooked11(returnedInterface, (IDXGIResource*)*' + p._name + ' ); }\n')
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
                    #if 'ID3D11DeviceContext' == param._interface_name and 'VSSetConstantBuffers' == param._method_name: print param._type
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

    def WriteImplementationToFile( self, fp, class_name ):
        fp.write('// -----------------------------------------------------------------------------\n')
        fp.write(self._return_type + ' ' + self._decl + ' ' + class_name + '::' + self._name + '(')
        if len(self._parameter_list) > 0:
            fp.write('\n')
            for i in range(len(self._parameter_list)):
                fp.write('    ' + self._parameter_list[i]._type + ' ' + self._parameter_list[i]._name)
                if self._parameter_list[i].IsImmediateArray():
                    fp.write('[' + self._parameter_list[i]._immediate_array_count + ']')
                if i < (len(self._parameter_list) - 1):
                    fp.write(',\n')
                else:
                    fp.write(')\n')
        else:
            fp.write(')\n')
        fp.write('{\n')

        # call CallTrace.enter()
        fp.write('    calltrace::AutoTrace trace(L"' + class_name + '::' + self._name + '");\n');

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
                if self._parameter_list[i].IsImmediateArray():
                    fp.write('    DEFINE_METHOD_ARRAY_PARAMETER(' + self._parameter_list[i]._type + ', ' + self._parameter_list[i]._name + ', ' + self._parameter_list[i]._immediate_array_count + ')')
                else:
                    fp.write('    DEFINE_METHOD_PARAMETER(' + self._parameter_list[i]._type + ', ' + self._parameter_list[i]._name + ')')
                if i < (len(self._parameter_list) - 1):
                    fp.write(',\n')
                else:
                    fp.write('))\n')
        else:
            fp.write('))\n')

    def WritePrototypeToFile( self, fp, class_name ):
        fp.write('    // -----------------------------------------------------------------------------\n'
                 '    ' + self._prefix + ' ' + self._return_type + ' ' + self._decl + ' ' + self._name + '(')
        self.WriteParameterList(fp, writeType=True, writeName=True)
        fp.write(');\n')
        # write prototype for pre method
        fp.write('    NullPtr<void (' + class_name + '::*)(')
        self.WriteParameterTypeList(fp, makeRef=True)
        fp.write(')> _' + self._name + '_pre_ptr;\n')
        # write prototype for post method
        fp.write('    NullPtr<void (' + class_name + '::*)(')
        if 'void' != self._return_type:
            fp.write(self._return_type)
            if len(self._parameter_list) > 0:
                fp.write(', ')
        self.WriteParameterTypeList(fp, makeRef=False)
        fp.write(')> _' + self._name + '_post_ptr;\n\n')

    def WriteCallBaseToFile( self, fp, interface_name, class_name ):
        fp.write('    // -----------------------------------------------------------------------------\n'
                 '    ' + self._return_type + ' ' + self._decl + ' ' + self._name + '(')
        self.WriteParameterList(fp, True, True)
        fp.write(')\n'
                 '    {\n')

        # call base method
        fp.write('        return _' + interface_name[1:] + '.' + self._name + '(')
        self.WriteParameterNameList(fp)
        fp.write(');\n')

        #end of function call
        fp.write('    }\n\n');
        pass; # end-of-func


# ------------------------------------------------------------------------------
# Interface class

class InterfaceSigature:

    def __init__( self, interface_name, hooked_class_name, methods ) :
        self._name = interface_name
        self._hookedClassName = hooked_class_name
        self._methods = methods

    def FindMethod(self, name, searchParents = True):
        for m in self._methods:
            if m._name == name: return m
        if searchParents and g_parents[self._name]:
            parent = g_interfaces[g_parents[self._name]]
            if parent: return parent.FindMethod( name, searchParents = True )
        return None

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
        type = m.group(1)
        name = m.group(2)
        count = None
        if type.find('_In_reads') >= 0:
            # the parameter is an input array, like: __In_reads_opt_(xxxx) ....
            m = re.match(r"_In_reads\w+\(\s*(\w+)\s*\)", line)
            if m is not None:
                count = m.group(1)
                #print type
                #print interface_name + '::' + method_name, name, m.group(1)
        return FunctionParameter(interface_name, method_name, type, name, variant_array_count=count)

    # __in  const UINT Value) = 0;
    m = re.match(r"(.+[^\w])(\w+)\) = 0;$", line)
    if m is not None:
        type = m.group(1)
        name = m.group(2)
        count = None
        #if 'ID3D11DeviceContext' == interface_name and 'VSSetConstantBuffers' == method_name: print m
        if type.find('_In_reads') >= 0:
            # the parameter is an input array, like: __In_reads_opt_(xxxx) ....
            m = re.match(r"_In_reads\w+\(\s*(\w+)\s*\)", line)
            count = m.group(1)
            #print type
            #print interface_name + '::' + method_name, name, m.group(1)
        return FunctionParameter(interface_name, method_name, type, name, variant_array_count=count)

    # __in_opt  const FLOAT BlendFactor[ 4 ],
    m = re.match(r"(.+[^\w])(\w+)\[\s*(\w+)\s*\],$", line)
    if m is not None:
        type = m.group(1)
        name = m.group(2)
        count = m.group(3)
        #print interface_name + '::' + method_name, type, name, count
        return FunctionParameter(interface_name, method_name, type, name, immediate_array_count=count)

    #  __in  const FLOAT ColorRGBA[ 4 ]) = 0;
    m = re.match(r"(.+[^\w])(\w+)\[\s*(\w+)\s*\]\) = 0;$", line)
    if m is not None:
        #print 'm[1] = "',m.group(1),'" m[2] = "',m.group(2),'" m[3] = "',m.group(3),'"'
        return FunctionParameter(interface_name, method_name, m.group(1), m.group(2), immediate_array_count=m.group(3))

    # comment line
    m = re.match(r"/\*.*", line)
    if m is not None:
        return None

    UTIL_error('Unrecognized function parameter line: ' + line)
    return None

def PARSE_get_parent_class(text):
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

class D3D11HooksFile:
    def __init__(self):
        self._header = open('d3d11hooks.inl', 'w')
        self._header.write('// script generated file. Do _NOT_ edit.\n\n'
                           '\n')
        self._cpp = open('d3d11hooks.cpp', 'w')
        self._cpp.write('// script generated file. Do _NOT_ edit.\n\n'
                        '#include "pch.h"\n'
                        '#include "d3d11hooks.h"\n\n')

    def WriteHookDecl(self, interface_name, class_name, methods):
        f = self._header
        f.write('// ==============================================================================\n'
                '// ' + interface_name + '\n'
                '// ==============================================================================\n'
                'class ' + class_name + ' : public HookBase<' + interface_name + '>\n'
                '{\n')
        self.WriteStandardHookMethodsToFile(f, interface_name, class_name)
        self.WriteCallBaseToFile(f, interface_name, class_name)
        for m in methods: m.WritePrototypeToFile( f, class_name )
        f.write('};\n\n')
        pass # end of function

    def WriteHookImpl(self, interface_name, class_name, methods):
        for m in methods: m.WriteImplementationToFile( self._cpp, class_name )
        pass # end of function

    # ------------------------------------------------------------------------------
    # Write standard hook methods to file
    #   interface_name : name of the interface that you want to parse
    #   class_name     : name of the hook class
    def WriteStandardHookMethodsToFile(self, f, interface_name, class_name):
        parents = GetParentInterfaceList(interface_name)
        for p in parents:
            f.write('    ' + g_interfaces[p]._hookedClassName + ' & _' + p[1:] + ';\n')
        f.write('\n'
                'protected:\n\n'
                '    ' + class_name + '(UnknownBase & unknown, ');
        for p in parents:
            f.write(g_interfaces[p]._hookedClassName + ' & ' + p[1:] + ', ')
        f.write('    IUnknown * realobj)\n' \
                '        : BASE_CLASS(unknown, realobj)\n')
        for p in parents:
            f.write('        , _' + p[1:] + '(' + p[1:] + ')\n')
        f.write('    {\n' \
                '    }\n\n' \
                '    ~' + class_name + '() {}\n\n'
                'public:\n\n'
                '    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)\n'
                '    {\n'
                '        UNREFERENCED_PARAMETER(context);\n')
        for p in parents:
            ii = g_interfaces[p]
            hookedType = ii._hookedClassName
            interfaceType = ii._name
            objectName = ii._name[1:]
            f.write('        ' + hookedType + ' * ' + objectName + ' = (' + hookedType + ' *)unknown.GetHookedObj(__uuidof(' + interfaceType + '));\n'
                    '        if (nullptr == ' + objectName + ') return nullptr;\n\n')
        f.write('        try\n'
                '        {\n'
                '            return new ' + class_name + '(unknown')
        for p in parents:
            ii = g_interfaces[p]
            objectName = ii._name[1:]
            f.write(', *' + objectName)
        f.write(', realobj);\n'
                '        }\n'
                '        catch(std::bad_alloc&)\n'
                '        {\n'
                '            HOOK_ERROR_LOG("Out of memory.");\n'
                '            return nullptr;\n'
                '        }\n'
                '    }\n\n'
                '    static void sDeleteInstance(void * context, void * ptr)\n'
                '    {\n'
                '        UNREFERENCED_PARAMETER(context);\n'
                '        ' + class_name + ' * typedPtr = (' + class_name + ' *)ptr;\n'
                '        delete typedPtr;\n'
                '    }\n\n'
                )
        pass

    # ------------------------------------------------------------------------------
    # Parse interface definition, generate c++ declarations
    #   interface_name : name of the interface that you want to parse
    #   class_name     : name of the hook class
    def WriteCallBaseToFile(self, f, interface_name, class_name):
        for p in GetParentInterfaceList(interface_name):
            for m in g_interfaces[p]._methods: m.WriteCallBaseToFile( f, p, class_name )

# ------------------------------------------------------------------------------
# Parse interface definition, generate c++ declarations
#   interface_name : name of the interface that you want to parse
#   include        : include this header file in generated .h file
#   lines          : the source code that you want to parse.
def PARSE_interface( interface_name, lines ):
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
            parent_class = PARSE_get_parent_class(l)
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
        """generate meta file
        with open(interface_name + "_meta.h", "w") as f:
            f.write('// script generated file. DO NOT edit.\n\n')
            for m in methods: m.WriteMetaDataToFile( f )"""
        g_d3d11hooks.WriteHookDecl(interface_name, class_name, methods)
        g_d3d11hooks.WriteHookImpl(interface_name, class_name, methods)
        g_cid.BeginInterface(interface_name, len(methods))
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

class D3D11VTable:
    def __init__(self, interface_name, lines):
        self._name = interface_name
        self._code = lines
        self._methods = self._ParseMethodName( interface_name, lines )

    def _ParseMethodName( self, interface_name, lines ):
        names = []
        for l in lines:
            if -1 != l.find('STDMETHODCALLTYPE'):
                m = re.match(r".+STDMETHODCALLTYPE \*(\w+)", l)
                names += [m.group(1)]
        return names

class D3D11VTableFile:
    def __init__(self):
        self._header = open('d3d11vtable.inl', 'w')
        self._header.write('// script generated file. Do _NOT_ edit.\n\n')
        self._cpp = open('d3d11vtable.cpp', 'w')
        self._cpp.write('// script generated file. Do _NOT_ edit.\n\n'
                        '#include "pch.h"\n'
                        '#include "d3d11vtable.h"\n\n'
                        'D3D11VTables g_D3D11OriginVTables;\n'
                        'D3D11VTables g_D3D11HookedVTables;\n'
                        '\n')
        self._vtables = []

    def _WriteRealToHookFunc(self, fp, vtable ):
        interface_name = vtable._name
        methods = vtable._methods
        vtable_name = interface_name + 'Vtbl'
        fp.write('// -----------------------------------------------------------------------------\n'
                 'inline void RealToHooked11_' + interface_name + '(' + interface_name + ' * p)\n'
                 '{\n'
                 '    if (p) RealToHooked_General(**(' + vtable_name + '**)p, g_D3D11OriginVTables._' + interface_name + ', g_D3D11HookedVTables._' + interface_name + ', "' + interface_name + '");\n'
                 '}\n'
                 'template <> inline void RealToHooked11<' + interface_name + '>(' + interface_name + ' * p)\n'
                 '{\n'
                 '    return RealToHooked11_' + interface_name + '( p );\n'
                 '}\n'
                 '\n')
        pass # end of function

    def _WriteAddRef( self, fp, interface_name ):
        fp.write('// -----------------------------------------------------------------------------\n'
                 'template<UINT INDEX> static ULONG STDMETHODCALLTYPE ' + interface_name + '_AddRef_Hooked(' + interface_name + ' * ptr)\n'
                 '{\n'
                 '    calltrace::AutoTrace trace("' + interface_name + '::AddRef");\n'
                 '    return g_D3D11OriginVTables._' + interface_name + '.tables[INDEX].AddRef(ptr);\n'
                 '}\n\n');
        pass

    def _WriteRelease( self, fp, interface_name ):
        fp.write('// -----------------------------------------------------------------------------\n'
                 'template<UINT INDEX> static ULONG STDMETHODCALLTYPE ' + interface_name + '_Release_Hooked(' + interface_name + ' * ptr)\n'
                 '{\n'
                 '    calltrace::AutoTrace trace("' + interface_name + '::Release");\n'
                 '    return g_D3D11OriginVTables._' + interface_name + '.tables[INDEX].Release(ptr);\n'
                 '}\n\n');

    def _WriteQI( self, fp, interface_name ):
        fp.write('// -----------------------------------------------------------------------------\n'
                 'template<UINT INDEX> static HRESULT STDMETHODCALLTYPE ' + interface_name + '_QueryInterface_Hooked(' + interface_name + ' * ptr, const IID & iid, void ** pp)\n'
                 '{\n'
                 '    calltrace::AutoTrace trace("' + interface_name + '::QueryInterface");\n'
                 '    return g_D3D11OriginVTables._' + interface_name + '.tables[INDEX].QueryInterface(ptr, iid, pp);\n'
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
            fp.write(')\n'
                     '{\n'
                     '    calltrace::AutoTrace trace("' + interface_name + '::' + m._name + '");\n'
                     '    return g_D3D11OriginVTables._' + interface_name + '.tables[INDEX].' + m._name + '(ptr')
            if len(m._parameter_list) > 0: fp.write(', ')
            m.WriteParameterNameList(fp)
            fp.write(');\n'
                               '}\n'
                               '\n');


    def Close(self):
        # write to header file
        self._header.write('// -----------------------------------------------------------------------------\n'
                           '// Global vtables for all D3D11/DXGI classes\n'
                           '// -----------------------------------------------------------------------------\n'
                           '\n'
                           'struct D3D11VTables\n'
                           '{\n')
        for vt in self._vtables:
            self._header.write('    VTable<' + vt._name + 'Vtbl> _' + vt._name + ';\n')
        self._header.write('};\n\n'
                           'extern D3D11VTables g_D3D11OriginVTables;\n'
                           'extern D3D11VTables g_D3D11HookedVTables;\n'
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
                        'void RealToHooked11(const IID & iid, void * p)\n'
                        '{\n'
                        '    if (false) {}\n')
        for vt in self._vtables:
            self._cpp.write('    else if (__uuidof(' + vt._name + ') == iid) RealToHooked11_' + vt._name + '((' + vt._name + '*)p);\n')
        self._cpp.write('    else\n'
                        '    {\n'
                        '        HOOK_WARN_LOG("unrecognized interface UUID: <xxxx-xxxx-xxxxx...>");\n'
                        '    }\n'
                        '}\n'
                        '\n'
                        '// -----------------------------------------------------------------------------\n'
                        'template<UINT INDEX> static void SetupD3D11HookedVTables()\n'
                        '{\n');
        for vt in self._vtables:
            for m in vt._methods:
                self._cpp.write('    g_D3D11HookedVTables._' + vt._name + '.tables[INDEX].' + m + ' = ' + vt._name + '_' + m + '_Hooked<INDEX>;\n')
        self._cpp.write('}\n\n'
                        '// -----------------------------------------------------------------------------\n'
                        'void SetupD3D11HookedVTables()\n'
                        '{\n')
        for i in range(16):
            self._cpp.write('    SetupD3D11HookedVTables<' + str(i) + '>();\n')
        self._cpp.write('}\n')
        self._cpp.close()
        self._cpp = None
        pass

    def WriteVtable( self, interface_name, lines ):

        # parse vtable definition
        vtable = D3D11VTable(interface_name, lines)
        methods = vtable._methods
        self._vtables.append( vtable )

        # generate vtable decl
        self._header.write('// -----------------------------------------------------------------------------\n'
                           '// ' + interface_name + '\n'
                           '// -----------------------------------------------------------------------------\n')
        for i, l in enumerate(lines):
            if (0 == len(l)) or (0 == i) or (1 == i) or (i == (len(lines)-1)):
                ident = ''
            elif 'BEGIN_INTERFACE' == l or 'END_INTERFACE' == l or -1 != l.find('STDMETHODCALLTYPE'):
                ident = '    '
            else:
                ident = '        '
            self._header.write(ident + l + '\n')
        self._header.write('\n')

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
def PARSE_d3d11_vtable( interface_name, lines ):
    start_line = 'typedef struct ' + interface_name + 'Vtbl'
    end_line = '} ' + interface_name + 'Vtbl;'
    found = None
    ended = None

    vtable = []

    for l in lines:
        if not found:
            if  -1 != l.find(start_line):
                found = True
                vtable += [l]
        else:
            vtable += [l]
            if -1 != l.find(end_line):
                ended = True
                break
        pass # end-of-for

    # write vtable to file
    if found and ended:
        g_d3d11vtables.WriteVtable(interface_name, vtable)
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

    all = g_interfaceNameFile.Gather(lines, file.name)

    if 0 == len(interfaces): interfaces = all

    for interface_name in interfaces:
        PARSE_interface(interface_name, lines)
        PARSE_d3d11_vtable(interface_name, lines)
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
# Gather all interfaces defined in an opened file.
class InterfaceNameFile :
    def __init__( self ) :
        self._file = open("d3d11interfaces.inl", "w")
        self._file.write('// Script generated. DO NOT EDIT.)\n')

    def Close(self):
        self._file.close();
        self._file = None;

    def Gather(self, lines, sourceFileName):
        interfaces = []
        for l in lines:
            m = re.match(r"(\w+) : public (\w+)", l)
            if m is None: continue
            interfaces += [m.group(1)]
        self._file.write('\n// ' + sourceFileName + '\n')
        for i in interfaces:
            self._file.write('DECLARE_D3D11_INTERFACE( ' + i + ' )\n')
        return interfaces

# ------------------------------------------------------------------------------
# Start of main procedure

g_d3d11hooks = D3D11HooksFile()

g_d3d11vtables = D3D11VTableFile()

# open global CID files
g_cid = CallIDCodeGen()

# interface->hook list
g_interfaces = dict()
g_interfaces['IUnknown'] = InterfaceSigature('IUnknown', 'UnknownHook', [])

# interface parent list
g_parents = dict()
g_parents['IUnknown'] = None

g_interfaceNameFile = InterfaceNameFile()

# parse d3d11.h
with open( 'd3d/d3d11.h' ) as f:
    PARSE_interfaces_from_opened_file(f, [])

# parse d3d11_1.h
with open( 'd3d/d3d11_1.h' ) as f:
    PARSE_interfaces_from_opened_file(f, [])

# parse d3d11sdklayer.h
with open( 'd3d/d3d11sdklayers.h' ) as f:
    PARSE_interfaces_from_opened_file(f, [])

# parse dxgi.h
with open( 'd3d/dxgi.h' ) as f:
    PARSE_interfaces_from_opened_file(f, [])

# parse dxgi1_2.h
with open( 'd3d/dxgi1_2.h' ) as f:
    PARSE_interfaces_from_opened_file(f, [])

# parse dxgidebug.h
with open( 'd3d/dxgidebug.h' ) as f:
    PARSE_interfaces_from_opened_file(f, [])

# Register all factories
with open("d3d11factories.inl", "w") as f:
    f.write('// script generated file. DO NOT edit.\n\n')
    for interfaceName, v in g_interfaces.iteritems():
        if ('IUnknown' != interfaceName):
            f.write('registerFactory<' + interfaceName + '>(' + v._hookedClassName + '::sNewInstance, ' + v._hookedClassName + '::sDeleteInstance, nullptr);\n')

# close opened files
g_interfaceNameFile.Close()
g_d3d11vtables.Close();
g_cid.Close()
