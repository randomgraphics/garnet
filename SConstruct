# -*- coding: utf-8 -*-

import os, os.path, re, fnmatch, SCons.Tool.xenon

# enviroment use by local functions
LOCAL_env = Environment()

################################################################################
#
# 定义编译选项
#
################################################################################

# 读取环境变量
def getenv( name, defval = None ):
    if name in os.environ: return os.environ[name]
    else: return defval

# 记录当前的OS
CONF_platform = LOCAL_env['PLATFORM']

# 辨别Windows的类型
CONF_mswin = None
if 'win32' == CONF_platform:
    if 'AMD64' == getenv('PROCESSOR_ARCHITECTURE') or 'AMD64' == getenv('PROCESSOR_ARCHITEW6432'):
        GN_mswin = 'pcx64'
    else:
        GN_mswin = 'pcx86'

# 定义可用的编译器列表
CONF_allCompilers = ''
CONF_defaultCompiler = None
if 'win32' == CONF_platform:
    CONF_allCompilers = 'vc71 vc80 vc80-x64 icl icl-em64t'
    if 'pcx64' == GN_mswin:
        CONF_defaultCompiler = 'vc80-x64'
    elif 'pcx86' == GN_mswin:
        CONF_defaultCompiler = 'vc80'
    elif SCons.Tool.xenon.exists( LOCAL_env ):
        CONF_allCompilers += ' xenon'
else:
    CONF_defaultCompiler = 'gcc'
    CONF_allCompilers = 'gcc icl'

# 定义编译模式
CONF_allVariants = 'debug release stdbg strel'

# 定义缺省的命令行选项
CONF_defaultCmdArgs = {
    'trace'             : getenv('GN_BUILD_TRACE',  '0'),
    'variant'           : getenv('GN_BUILD_VARIANT', 'debug'),
    'compiler'          : getenv('GN_BUILD_COMPILER', CONF_defaultCompiler), # default compiler
    'enableCg'          : getenv('GN_BUILD_ENABLE_CG', 1), # use Cg by default.
    'enableProfile'     : getenv('GN_BUILD_ENABLE_PROFILE', 0), # disabled by default
    }

# 是否打开trace
CONF_trace = float( ARGUMENTS.get('trace', CONF_defaultCmdArgs['trace']) )

# 定义编译器类型
CONF_compiler = ARGUMENTS.get('compiler', CONF_defaultCmdArgs['compiler'])

# 定义编译类型
CONF_variant = ARGUMENTS.get('variant', CONF_defaultCmdArgs['variant'] )

# 是否支持Cg语言.
CONF_enableCg  = ARGUMENTS.get('cg', CONF_defaultCmdArgs['enableCg'] )

# 是否启用profiler.
CONF_enableProfile = ARGUMENTS.get('prof', CONF_defaultCmdArgs['enableProfile'] )

################################################################################
#
# 局部于本文件的工具函数
#
################################################################################

# 是否是静态编译模式
def UTIL_staticBuild( variant ) : return 'stdbg' == variant or 'strel' == variant

# 是否是调试编译模式
def UTIL_debugBuild( variant ) : return 'debug' == variant or 'stdbg' == variant

def UTIL_buildRoot() : return os.path.join( 'bin', 'scons.tmp', CONF_platform )

def UTIL_buildDir( compiler, variant ) : return os.path.join( UTIL_buildRoot(), compiler, variant )

#
# Create new build environment
#
def UTIL_newEnv( compiler, variant ):
    # crete new enviroment instance
    tools = ['default']
    msvs_version = '7.1'
    msvs_platform = 'x86'
    icl_version = None
    icl_abi = 'ia32'
    if 'xenon' == compiler:
        tools = ['xenon']
    elif 'icl' == compiler :
        tools += ['intelc']
    elif 'icl-em64t' == compiler :
        tools += ['intelc']
        icl_abi = 'em64t'
        msvs_version = '8.0'
        msvs_platform = 'x64'
    elif 'vc80' == compiler :
        msvs_version = '8.0'
    elif 'vc80-x64' == compiler :
        msvs_version = '8.0'
        msvs_platform = 'x64'
    env = Environment(
        tools = tools,
        MSVS_VERSION = msvs_version,
        MSVS_PLATFORM = msvs_platform,
        ICL_VERSION = icl_version,
        ICL_ABI = icl_abi )

    env.SConsignFile( os.path.join( UTIL_buildRoot(), '.sconsign.dbm' ) )

    # setup builder for gcc precompiled header
    if 'g++' == env['CXX']:

        import SCons.Defaults
        import SCons.Tool

        # attach gch builder
        bld = Builder(
            action = '$CXXCOM',
            suffix = '.h.gch',
            source_scanner = SCons.Tool.SourceFileScanner )
        env.Append( BUILDERS={'PCH':bld} )

        # Sets up the PCH dependencies for an object file
        def pch_emitter( target, source, env, parent_emitter ):
            parent_emitter( target, source, env )
            if env.has_key('PCH') and env['PCH']:
                env.Depends(target, env['PCH'])
            return (target, source)
        def static_pch_emitter(target,source,env):
            return pch_emitter(target,source,env,SCons.Defaults.StaticObjectEmitter)
        def shared_pch_emitter(target,source,env):
            return pch_emitter(target,source,env,SCons.Defaults.SharedObjectEmitter)
        for suffix in Split('.c .C .cc .cxx .cpp .c++'):
            env['BUILDERS']['StaticObject'].add_emitter( suffix, static_pch_emitter );
            env['BUILDERS']['SharedObject'].add_emitter( suffix, shared_pch_emitter );

    # 缺省编译选项
    def generate_empty_options() : return { 'neutral':[], 'common':[],'debug':[],'release':[],'stdbg':[],'strel':[] }
    cppdefines = generate_empty_options()
    cpppath    = generate_empty_options()
    libpath    = generate_empty_options()
    libs       = generate_empty_options()
    ccflags    = generate_empty_options()
    cxxflags   = generate_empty_options()
    linkflags  = generate_empty_options()

    # define profile tag
    if float(CONF_enableProfile): cppdefines['common'] = ['GN_ENABLE_PROFILE=1']

    # 定制不同平台的编译选项
    if 'xenon' == compiler:
        libs['common'] += Split('xboxkrnl xbdm dxerr9')
        libs['stdbg'] += Split('xapilibd d3d9d d3dx9d xgraphicsd xnetd xaudiod xactd vcompd')
        libs['strel'] += Split('xapilib  d3d9  d3dx9  xgraphics  xnet  xaudio  xact  vcomp ')
    elif 'win32' == env['PLATFORM']:
        libs['common'] += Split('kernel32 user32 gdi32 shlwapi advapi32 shell32')
    else:
        cpppath['common'] += Split('/usr/X11R6/include /usr/local/include')
        libpath['common'] += Split('/usr/X11R6/lib /usr/local/lib')
        libs['common'] += Split('GL GLU X11')

    # 定制不同编译模式的编译选项
    cppdefines['debug']   += ['GN_DEBUG=1']
    cppdefines['release'] += ['NDEBUG']
    cppdefines['stdbg']   += ['GN_STATIC=1', 'GN_DEBUG=1']
    cppdefines['strel']   += ['GN_STATIC=1', 'NDEBUG']

    # 定制不同编译器的编译选项
    if 'cl' == env['CC']:
        #
        # To workaround an bug in scons 0.96.1, this variable has to be imported
        # on AMD64 platform, to make msvs link.exe work.
        #
        env['ENV']['USERPROFILE'] = os.environ['USERPROFILE']

        cppdefines['debug']   += ['_DEBUG']
        cppdefines['stdbg']   += ['_DEBUG']

        if float(env['MSVS_VERSION']) >= 8.0:
            cxxflags['common']  += ['/EHa']
            linkflags['common'] += Split( '/NODEFAULTLIB:libcp.lib' )
        else:
            cxxflags['common']  += ['/EHs']

        ccflags['common']  += Split('/W4 /WX')
        ccflags['debug']   += Split('/MDd /GR /RTCscu')
        ccflags['release'] += Split('/MD /O2 /GL')
        ccflags['stdbg']   += Split('/MTd /GR')
        ccflags['strel']   += Split('/MT /O2 /GL')

        # this is for vtune and magellan to do instrumentation
        linkflags['common']  += ['/FIXED:NO', '/DEBUGTYPE:CV,FIXUP']

        linkflags['release'] += ['/OPT:REF']
        linkflags['strel']   += ['/OPT:REF']

    elif 'icl' == env['CC']:
        ccflags['common']  += Split('/W4 /WX /Qpchi- /Zc:forScope')
        ccflags['debug']   += Split('/MDd /GR /Ge /traceback')
        ccflags['release'] += Split('/O2 /MD')
        ccflags['stdbg']   += Split('/MTd /GR /Ge /traceback')
        ccflags['strel']   += Split('/O2 /MT')

        cxxflags['common'] += ['/EHs']

        cppdefines['debug']   += Split('_DEBUG')
        cppdefines['stdbg']   += Split('_DEBUG')

        # this is for vtune to do instrumentation
        linkflags['release'] = Split('/FIXED:NO /OPT:REF')
        linkflags['strel']   = Split('/FIXED:NO /OPT:REF')

    elif 'gcc' == env['CC']:
        ccflags['common']  += Split('-Wall -Werror')
        ccflags['debug']   += ['-g']
        ccflags['release'] += ['-O3']
        ccflags['stdbg']   += ['-g']
        ccflags['strel']   += ['-O3']

    else:
        print 'unknown compiler: ' + env['CC']
        Exit(-1)

    def apply_options( env, variant ):
        env.Append(
            CPPDEFINES = cppdefines[variant],
            CPPPATH = cpppath[variant],
            LIBPATH = libpath[variant],
            CCFLAGS = ccflags[variant],
            CXXFLAGS = cxxflags[variant],
            LINKFLAGS = linkflags[variant] );
        env.Prepend(
            LIBS    = libs[variant] ); # for gcc link

    # apply compile options based on current build
    apply_options( env, variant )
    apply_options( env, 'common' )

    # end of function default_env()
    return env

#
# Check sysem configuration
#
def UTIL_checkConfig( conf, confDir, compiler, variant ):
    env = UTIL_newEnv( compiler, variant )

    # Do NOT treat warning as error
    ccflags = str(env.Dictionary('CCFLAGS'))
    ccflags = ccflags.replace( '/WX', '' )
    ccflags = ccflags.replace( '-Werror', '')
    env.Replace( CCFLAGS = Split( ccflags ) )
    c = env.Configure(
        conf_dir = confDir,
        log_file = os.path.join(confDir,'config.log') )

    # =================
    # 是否支持Cg shader
    # =================
    conf['has_cg'] = c.CheckCHeader('cg/cg.h')

    # ==============
    # 是否支持OpenGL
    # ==============
    conf['has_ogl'] = 'xenon' != compiler

    # ============
    # 是否支持D3D9
    # ============
    conf['has_d3d9'] = c.CheckCXXHeader('xtl.h') or c.CheckCXXHeader( 'd3d9.h' ) and c.CheckCXXHeader( 'd3dx9.h' )

    # =============
    # 是否支持D3D10
    # =============
    conf['has_d3d10'] = c.CheckCXXHeader( 'd3d10.h' )

    # ==============
    # 是否支持XInput
    # ==============
    conf['has_xinput'] = c.CheckCXXHeader( ['windows.h', 'XInput.h'] )

    # =========================
    # 检查是否存在boost library
    # =========================
    conf['has_boost'] = c.CheckCXXHeader( 'boost/any.hpp' )

    # ==========
    # 检查 cegui
    # ==========
    conf['has_cegui'] = c.CheckCXXHeader( 'CEGUI.h' )

    # =============
    # finish config
    # =============
    c.Finish()

################################################################################
#
# 定义 GarnetEnv class
#
################################################################################

class GarnetEnv :

    conf = {}

    # 输出调试信息
    def trace( self, level, msg ):
        level = float(level)
        assert( level > 0 )
        if ( CONF_trace > 0 and level <= CONF_trace ) or ( CONF_trace < 0 and level == -CONF_trace ):
            print 'TRACE(%d) : %s'%(level,msg)

    # 输出提示信息
    def info( self, msg ): print 'INFO : %s'%msg

    # 输出警告信息
    def warn( self, msg ):
        print '===================================================================='
        print 'WARNING : %s'%msg
        print '===================================================================='

    # 输出错误信息
    def error( self, msg ):
        print '===================================================================='
        print 'ERROR : %s'%msg
        print '===================================================================='

    # 生成从target到base的相对路径
    def relpath( self, target, base ):
        """
        Return a relative path to the target from either the current dir or an optional base dir.
        Base can be a directory specified either as absolute or relative to current dir.
        """

        base_list = (os.path.abspath(base)).split(os.sep)
        target_list = (os.path.abspath(target)).split(os.sep)

        # On the windows platform the target may be on a completely different drive from the base.
        if os.name in ['nt','dos','os2'] and base_list[0] <> target_list[0]:
            raise OSError, 'Target is on a different drive to base. Target: '+target_list[0].upper()+', base: '+base_list[0].upper()

        # Starting from the filepath root, work out how much of the filepath is
        # shared by base and target.
        for i in range(min(len(base_list), len(target_list))):
            if base_list[i] <> target_list[i]: break
        else:
            # If we broke out of the loop, i is pointing to the first differing path elements.
            # If we didn't break out of the loop, i is pointing to identical path elements.
            # Increment i so that in all cases it points to the first differing path elements.
            i+=1

        rel_list = [os.pardir] * (len(base_list)-i) + target_list[i:]
        return os.path.join(*rel_list)

    # 查找指定目录下的文件
    def glob( self, patterns, recursive = False ):
        def do_glob( pattern, dir, recursive ):
            files = []
            root = Dir(dir).srcnode().abspath;
            try:
                for file in os.listdir( root ):
                    if os.path.isdir( os.path.join(root,file) ):
                        if recursive and ( not '.svn' == file ) : # ignore subversion directory
                            files = files + do_glob( pattern, os.path.join(dir,file), recursive )
                    else:
                        # Note: ignore precompiled header
                        if not ('pch.cpp' == file or 'stdafx.cpp' == file):
                            #print 'fnmatch(%s,%s) = %s'%(file,pattern,fnmatch.fnmatch(file, pattern))
                            if fnmatch.fnmatch(file, pattern):
                                files.append( os.path.join( dir, file ) )
            except WindowsError:
                pass
            return files
        #print 'glob %s'%patterns )
        files = []
        if not patterns is list: patterns = [patterns]
        for p in Flatten(patterns):
            if os.path.isdir( GetBuildPath(p) ):
                #print '    do_glob(*.*,%s)'%p
                files += do_glob( '*.*', p, recursive )
            else:
                (dir,pattern) = os.path.split(p);
                if '' == pattern: pattern = '*.*';
                if '' == dir: dir = '.';
                #print '    do_glob(%s,%s)'%(pattern,dir)
                files += do_glob( pattern, dir, recursive )
        return files

    # 创建 source cluster
    def newSourceCluster( self, sources, pchHeader = None, pchSource = None ):
        s = SourceCluster()
        s.sources = [File(x) for x in sources]
        if pchHeader: s.pchHeader = pchHeader
        if pchSource: s.pchSource = File(pchSource)
        return s

    # 创建 custom source cluster
    def newCustomSourceCluster( self, sources, action ):
        s = SourceCluster()
        s.sources = [File(x) for x in sources]
        s.action = action
        return s

    # 创建 Target
    def newTarget( self, type, name, sources, dependencies = [], pdb = None ):
        # create new target instance
        t = Target()
        if UTIL_staticBuild( CURRENT_variant ) and 'shlib' == type: type = 'stlib'
        t.type = type
        t.path = Dir('.')
        t.sources = sources
        t.dependencies = dependencies
        if pdb : t.pdb = File(pdb)
        else   : t.pdb = File("%s.pdb"%(name))
        ALL_targets[CURRENT_compiler][CURRENT_variant][name] = t # insert to global target list
        return t

    # 创建 neutral(compiler insensitive) custom target.
    def newNeutralCustomTarget( self, name, sources ):

        # check for redundant target
        if name in ALL_targets['neutral']['neutral'] : return ALL_targets['neutral']['neutral']

        # create new target instance
        t = Target()
        t.type = 'custom'
        t.path = Dir('.')
        t.sources = sources
        ALL_targets['neutral']['neutral'][name] = t # insert to global target list
        return t

GN = GarnetEnv()

################################################################################
#
# 定义 build target 类
#
################################################################################

class CompileFlags:

    def __init__(self):
        self.cpppath = []
        self.cppdefines = []
        self.ccflags = []
        self.cxxflags = []

class LinkFlags:

    def __init__(self):
        self.libpath = []
        self.linkflags = []

class SourceCluster:

    def __init__(self):
        self.sources = []
        self.pchHeader = None
        self.pchSource = None
        self.extraCompileFlags = CompileFlags()
        self.removedCompileFlags = CompileFlags()
        self.action = None # custom action object, only used for 'custom' type target.
                           # Should accept a enviroment and a list of sources as parameter,
                           # and return list of targets.

    def addCompileFlags( self,
                         CPPPATH = None,
                         CPPDEFINES = None,
                         CCFLAGS = None,
                         CXXFLAGS = None ):
        if CPPPATH :
            for x in CPPPATH:
                p1 = Dir(x).path
                p2 = Dir(x).srcnode().path
                self.extraCompileFlags.cpppath.append( p1 )
                if p2 != p1: self.extraCompileFlags.cpppath.append( p2 )
        if CPPDEFINES : self.extraCompileFlags.cppdefines += CPPDEFINES
        if CCFLAGS : self.extraCompileFlags.ccflags += CCFLAGS
        if CXXFLAGS : self.extraCompileFlags.cxxflags += CXXFLAGS

    def removeCompileFlags( self,
                            CPPPATH = None,
                            CPPDEFINES = None,
                            CCFLAGS = None,
                            CXXFLAGS = None ):
        if CPPPATH : self.removedCompileFlags.cpppath += CPPPATH
        if CPPDEFINES : self.removedCompileFlags.cppdefines += CPPDEFINES
        if CCFLAGS : self.removedCompileFlags.ccflags += CCFLAGS
        if CXXFLAGS : self.removedCompileFlags.cxxflags += CXXFLAGS

class Target:

    def __init__(self):
        self.type = None # could be 'stlib, shlib, prog, custom'
        self.path = None
        self.targets = []
        self.sources = [] # list of source clusters
        self.dependencies = [] # list of denpendencies. item in this list must be valid target name.
        self.externalDependencies = [] # list of external dependencies. Could be any name.
        self.pdb = None
        self.extraLinkFlags = LinkFlags()
        self.removedLinkFlags = LinkFlags()

    def addCluster( self, c ): self.sources.append( c )

    def addLinkFlags( self, LIBPATH = None, LINKFLAGS = None ):
        if LIBPATH : self.extraLinkFlags.libpath += LIBPATH
        if LINKFLAGS : self.extraLinkFlags.linkflags += LINKFLAGS

    def removeLinkFlags( self, LIBPATH = None, LINKFLAGS = None ):
        if LIBPATH : self.removedLinkFlags.libpath += LIBPATH
        if LINKFLAGS : self.removedLinkFlags.linkflags += LINKFLAGS

################################################################################
#
# Collect configurations
#
################################################################################

COLLECT_compilers = []
COLLECT_variants = []
if 'all' in COMMAND_LINE_TARGETS:
    COLLECT_compilers = Split( CONF_allCompilers )
    COLLECT_variants = Split( CONF_allVariants )
else:
    if 'all' == CONF_compiler:
        COLLECT_compilers = Split( CONF_allCompilers )
    else:
        COLLECT_compilers = Split( CONF_compiler )
    if 'all' == CONF_variant:
        COLLECT_variants = Split( CONF_allVariants )
    else:
        COLLECT_variants = Split( CONF_variant )

ALL_configs = []
ALL_targets = { "neutral" : { "neutral" : {} } }
for c in COLLECT_compilers:

    if not c in CONF_allCompilers:
        print "ERROR: Ignore invalid compiler '%s'"%c
        continue

    ALL_targets[c] = {}

    for v in COLLECT_variants:

        if not v in CONF_allVariants:
            print "ERROR: Ignore invalid variant '%s'"%v
            continue

        static = UTIL_staticBuild( v )
        debug  = UTIL_debugBuild( v )

        # ignore non-static build for xenon
        if 'xenon' == c and static : continue

        conf = {
            'platform'   : CONF_platform,
            'mswin'      : CONF_mswin,
            'compiler'   : c,
            'variant'    : v,
            'static'     : static,
            'debug'      : debug
        }

        buildDir = UTIL_buildDir(c,v)

        UTIL_checkConfig( conf, os.path.join(buildDir,'config'), c, v )

        ALL_configs.append( conf )

        ALL_targets[c][v] = {}

################################################################################
#
# Call SConscript to generate target instances
#
################################################################################

CURRENT_compiler = None
CURRENT_variant = None
for c in ALL_configs :
    GN.conf = c
    CURRENT_compiler = c['compiler']
    CURRENT_variant = c['variant']
    bldDir = UTIL_buildDir( CURRENT_compiler, CURRENT_variant )
    SConscript( 'SConscript', exports=['GN'], build_dir=bldDir, duplicate=0 )

################################################################################
#
# buill targets
#
################################################################################

BUILD_compiler = None
BUILD_variant = None
BUILD_env = None
BUILD_bldDir = None
BUILD_libDir = None
BUILD_binDir = None

#
# Create new compile environment
#
def BUILD_newCompileEnv( cluster ):
    env = BUILD_env.Copy()

    env.Prepend( CPPPATH = ['#src/extern/inc', 'src/priv/inc'] )

    if 'icl' == env['CC']: env.Append( CCFLAGS = ['/Zi', '/debug:full'] )
    elif 'cl' == env['CC']: env.Append( CCFLAGS = ['/Z7', '/Yd'] )

    a = cluster.extraCompileFlags
    env.Append(
        CPPPATH = a.cpppath,
        CPPDEFINES = a.cppdefines,
        CCFLAGS = a.ccflags,
        CXXFLAGS = a.cxxflags )

    def doRemove( env, name, flags ):
        if flags:
            for x in flags:
                if x in env[name]:
                    env[name].remove(x)

    a = cluster.removedCompileFlags
    doRemove( env, 'CPPPATH', a.cpppath )
    doRemove( env, 'CPPDEFINES', a.cppdefines )
    doRemove( env, 'CCFLAGS', a.ccflags )
    doRemove( env, 'CXXFLAGS', a.cxxflags )

    return env

#
# Create new link environment
#
def BUILD_newLinkEnv( target ):
    env = BUILD_env.Copy()

    if target.pdb: env['PDB'] = target.pdb

    env.Prepend( LIBPATH = [BUILD_libDir] )
    if 'pcx64' == CONF_mswin:
        env.Prepend( LIBPATH = ['src/extern/lib/x64'] )
    elif 'pcx86' == CONF_mswin:
        env.Prepend( LIBPATH = ['src/extern/lib/x86'] )

    a = target.extraLinkFlags
    env.Append(
        LIBPATH = a.libpath,
        LINKFLAGS = a.linkflags )

    def doRemove( env, name, flags ):
        if flags:
            for x in flags:
                if x in env[name]:
                    env[name].remove(x)

    a = target.removedLinkFlags
    doRemove( env, 'LIBPATH', a.libpath )
    doRemove( env, 'LINKFLAGS', a.linkflags )

    # success
    return env

#
# setup environment for producing PCH, return PCH object file
#
def BUILD_setupPCH( env, pchHeader, pchSource ):
    if 'PCH' in env['BUILDERS'] and pchSource:
        pch = env.PCH(pchSource)
        env['PCH'] = pch[0]
        env['PCHSTOP'] = pchHeader
        if( len(pch) > 1 ):
            return [pch[1]]
        else:
            return []
    else:
        return []

#
# build static objects
#
def BUILD_staticObjs( cluster ):
    env = BUILD_newCompileEnv( cluster )
    pchObj = BUILD_setupPCH( env, cluster.pchHeader, cluster.pchSource )
    return Flatten( [env.Object(x) for x in cluster.sources] + pchObj )

#
# build shared objects
#
def BUILD_sharedObjs( cluster ):
    env = BUILD_newCompileEnv( cluster )
    pchObj = BUILD_setupPCH( env, cluster.pchHeader, cluster.pchSource )
    return Flatten( [env.SharedObject(x) for x in cluster.sources] + pchObj )

#
# build static library
#
def BUILD_staticLib( name, target ):
    objs = []
    for s in target.sources: objs += BUILD_staticObjs( s )
    env = BUILD_newLinkEnv( target )
    libName = '%s%s%s'%(env['LIBPREFIX'],name,env['LIBSUFFIX'])
    target.targets = env.Install( BUILD_libDir, env.Library( os.path.join(str(target.path),libName), objs ) )
    Alias( name, target.targets )
    Default( target.targets )

#
# handle dependencies
#
def BUILD_toList( x ):
    if x : return x
    else : return []

def BUILD_addLib( env, name, lib ):
    if not lib in env['LIBS']:
        env.Prepend( LIBS = [lib] )
        GN.trace( 1, 'Add depends of %s : %s'%(name,lib) )

def BUILD_addExternalDependencies( env, name, deps ):
    for x in reversed(deps):
        BUILD_addLib( env, name, x )

def BUILD_addDependencies( env, name, deps ):
    targets = ALL_targets[BUILD_compiler][BUILD_variant]
    for x in reversed(deps):
        if x  in targets:
            BUILD_addExternalDependencies( env, name, BUILD_toList(targets[x].externalDependencies) )
            BUILD_addDependencies( env, name, BUILD_toList(targets[x].dependencies) )
            if 'GNcore' == x or 'stlib' == targets[x].type : # here we ignore shared libraried other then GNcore.
                BUILD_addLib( env, name, x )
        else:
            GN.warn( "Ingore non-exist dependency for target %s: %s"%(name,x) )

#
# does compiler produce manifest file?
#
def BUILD_handleManifest( env, target ):
    if float(env.get('MSVS_VERSION',0)) >= 8.0 and not UTIL_staticBuild( BUILD_variant ):
        manifest = File( '%s.manifest'%target[0] )
        env.SideEffect( manifest, target )
        target += [manifest]

#
# build static library
#
def BUILD_sharedLib( name, target ):
    objs = []
    for s in target.sources: objs += BUILD_staticObjs( s )

    env = BUILD_newLinkEnv( target )

    stdlibs = []
    if 'GNcore' != name : stdlibs.append( 'GNcore' )
    stdlibs += Split('GNbase GNextern')

    BUILD_addDependencies( env, name, BUILD_toList(target.dependencies) + stdlibs )
    BUILD_addExternalDependencies( env, name, BUILD_toList(target.externalDependencies) )
    GN.trace( 1, "Depends of %s : %s"%(name,env['LIBS']) )

    libName = '%s%s%s'%(env['SHLIBPREFIX'],name,env['SHLIBSUFFIX'])
    shlib = env.SharedLibrary( os.path.join(str(target.path),libName), objs )
    BUILD_handleManifest( env, shlib )

    def extname( path ):
        p,e = os.path.splitext( str(path) )
        return e

    target.targets = []
    for x in shlib:
        e = extname( x )
        if '.lib' == e or '.exp' == e  or '.a' == e :
            d = BUILD_libDir
        else:
            d = BUILD_binDir
        target.targets.append( env.Install( d, x )[0] )

    Alias( name, target.targets )
    Default( target.targets )

#
# build executable
#
def BUILD_program( name, target ):
    objs = []
    for s in target.sources: objs += BUILD_staticObjs( s )

    env = BUILD_newLinkEnv( target )

    stdlibs = Split('GNgfxD3D9 GNgfxD3D10 GNgfxOGL GNcore GNbase GNextern')

    BUILD_addDependencies( env, name, BUILD_toList(target.dependencies) + stdlibs )
    BUILD_addExternalDependencies( env, name, BUILD_toList(target.externalDependencies) )
    GN.trace( 1, "Depends of %s : %s"%(name,env['LIBS']) )

    if 'gcc' == env['CC']: env.Prepend( LIBS=['GNcore','GNbase'] )

    exeName = '%s%s%s'%(env['PROGPREFIX'],name,env['PROGSUFFIX'])
    prog = env.Program( os.path.join(str(target.path),exeName), objs )
    BUILD_handleManifest( env, prog )
    target.targets = env.Install( BUILD_binDir, prog )

    Alias( name, target.targets )
    Default( target.targets )

#
# build custom target
#
def BUILD_custom( name, target ):
    assert( 'custom' == target.type )
    env = BUILD_env.Copy()
    target.targets = []
    for s in target.sources:
        target.targets += s.action( env, s.sources )
    Alias( name, target.targets )
    Default( target.targets )

#
# build all targets
#
for compiler, variants in ALL_targets.iteritems() :
    BUILD_compiler = compiler
    for variant, targets in variants.iteritems():

        BUILD_env = UTIL_newEnv( compiler, variant )
        BUILD_variant = variant
        BUILD_bldDir = UTIL_buildDir( compiler, variant )
        BUILD_libDir = os.path.join( BUILD_bldDir, 'lib' )
        BUILD_binDir = os.path.join( BUILD_bldDir, 'bin' )

        # do build
        for name, x in targets.iteritems():
            if 'stlib' == x.type :
                BUILD_staticLib( name, x )
            elif 'shlib' == x.type :
                BUILD_sharedLib( name, x )
            elif 'prog' == x.type :
                BUILD_program( name, x )
            elif 'custom' == x.type :
                BUILD_custom( name, x )
            # TODO: special case for build documents
            else: GN.error( 'Unknown target type for target %s: %s'%(name,x.type) )
            GN.trace( 1, "%s : compiler(%s), variant(%s), type(%s), path(%s), targets(%s)"%(
                name, compiler, variant, x.type, x.path, [str(t) for t in x.targets] ) )

        # build additional dependencies:
        # - Make binaries depend on their by-products, such as manifest and PDB, to make sure
        #   those files are copied to binary directory, before execution of the binaries.
        # - Make executables depend on shared libraries.
        stlibs = Split('GNextern GNbase GNcore')
        shlibs = Split('GNcore GNgfxD3D9 GNgfxD3D10 GNgfxOGL')
        tests = Split('GNtestD3D9 GNtestD3D10 GNtestFt2 GNtestGfx GNtestGui GNtestInput GNut')
        samples = Split('GNsampleRenderToTexture GNsampleDepthTexture')
        tools = Split('GNtoolOglInfo')
        progs = tests + samples + tools
        def getTargets( n ):
            if n in targets : return targets[n].targets
            else : return []
        for n in ( shlibs + progs ):
            t = getTargets(n)
            for x in t[1:] :
                Depends( t[0], x )
        for pn in progs:
            for pt in getTargets(pn):
                for sn in shlibs:
                    for st in getTargets(sn):
                        Depends( pt, st )

################################################################################
#
# TODO: 生成bin/dist目录
#
################################################################################

################################################################################
#
# 生成msvc工程文件
#
################################################################################

if 'MSVSProject' in LOCAL_env['BUILDERS']:
    for compiler, variants in ALL_targets.iteritems() :
        for variant, targets in variants.iteritems():
            for name, x in targets.iteritems():
                if 'stlib' == x.type or 'shlib' == x.type or 'prog' == x.type :
                    SConscript( 
                    'msvc/SConscript',
                        exports={
                            'GN' : GN,
                            'env' : LOCAL_env,
                            'compiler' : compiler,
                            'variant' : variant,
                            'name' : name,
                            'target' : x,
                            },
                        build_dir=os.path.join( UTIL_buildDir( compiler, variant ), "msvc" )
                        )

################################################################################
#
# 定义 help screen
#
################################################################################

def HELP_generateTargetList():
    names = set()
    maxlen = 0;
    for v in ALL_targets.itervalues():
        for t in v.itervalues():
            for n in t.keys():
                names.add( n )
                l = len( n )
                if l > maxlen: maxlen = l
    names = [x for x in names]
    names.sort()
    s = ''
    #for n in names: s += ('\n%' + str(maxlen+4) + 's')%n
    for n in names: s += '\n    %s'%n
    return s

HELP_opts = Options()
HELP_opts.Add(
    'trace',
    'Set trace level. (GN_BUILD_TRACE)',
    CONF_defaultCmdArgs['trace'] )
HELP_opts.Add(
    'compiler',
    'Specify compiler. Could be : one of (%s) or "all". (GN_BUILD_COMPILER)'%CONF_allCompilers,
    CONF_defaultCmdArgs['compiler'] )
HELP_opts.Add(
    'variant',
    'Specify variant. Could be : one of (%s) or "all". (GN_BUILD_VARIANT)'%CONF_allVariants,
    CONF_defaultCmdArgs['variant'] )
HELP_opts.Add(
    'cg',
    'Support Cg language or not. (GN_BUILD_ENABLE_CG)',
    CONF_defaultCmdArgs['enableCg'] )
HELP_opts.Add(
    'prof',
    'Enable performance profiler. (GN_BUILD_ENABLE_PROFILE)',
    CONF_defaultCmdArgs['enableProfile'] )

HELP_text = """
Usage:
    scons [options] [target1 target2 ...]

Options:%s

Targets:%s
""" % (
    HELP_opts.GenerateHelpText( Environment( options = HELP_opts ) ),
    HELP_generateTargetList()
    )
Help( HELP_text )
