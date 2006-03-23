# -*- coding: utf-8 -*-

import os, os.path, re, fnmatch, SCons.Tool.xenon

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
CONF_platform = Environment()['PLATFORM']

# 辨别Windows的类型
CONF_mswin = None
if 'win32' == CONF_platform:
    if 'AMD64' == getenv('PROCESSOR_ARCHITECTURE') or 'AMD64' == getenv('PROCESSOR_ARCHITEW6432'):
        GN_mswin = 'pcx64'
    else:
        GN_mswin = 'pcx86'

# 定义可用的编译器列表
if 'win32' == CONF_platform:
    ALL_compilers = 'vc71 vc80 vc80-x64 icl icl-em64t'
    if 'pcx64' == GN_mswin:
        default_compiler = 'vc80-x64'
    elif 'pcx86' == GN_mswin:
        default_compiler = 'vc80'
    elif SCons.Tool.xenon.exists( Environment() ):
        ALL_compilers += ' xenon'
else:
    default_compiler = 'gcc'
    ALL_compilers = 'gcc icl'
ALL_variants='debug release stdbg strel'

# 定义缺省的命令行选项
default_cmdargs = {
    'trace'             : getenv('GN_BUILD_TRACE',  '0'),
    'variant'           : getenv('GN_BUILD_VARIANT', 'debug'),
    'compiler'          : getenv('GN_BUILD_COMPILER', default_compiler), # default compiler
    'enable_cg'         : getenv('GN_BUILD_ENABLE_CG', 1), # use Cg by default.
    'enable_profile'    : getenv('GN_BUILD_ENABLE_PROFILE', 0), # disabled by default
    }

# 是否打开trace
CONF_trace = float( ARGUMENTS.get('trace', default_cmdargs['trace']) )

# 定义编译器类型
CONF_compiler = ARGUMENTS.get('compiler', default_cmdargs['compiler'])

# 定义编译类型
CONF_variant = ARGUMENTS.get('variant', default_cmdargs['variant'] )

# 是否支持Cg语言.
CONF_enable_cg  = ARGUMENTS.get('cg', default_cmdargs['enable_cg'] )

# 是否启用profiler.
CONF_enable_profile = ARGUMENTS.get('prof', default_cmdargs['enable_profile'] )

################################################################################
#
# 创建缺省编译环境
#
################################################################################

opts = Options()
opts.Add(
    'trace',
    'Set trace level. Default is 0.',
    default_cmdargs['compiler'] )
opts.Add(
    'compiler',
    'Specify compiler. Could be : one of (%s) or "all". (GN_BUILD_COMPILER)'%ALL_compilers,
    default_cmdargs['compiler'] )
opts.Add(
    'variant',
    'Specify variant. Could be : one of (%s) or "all". (GN_BUILD_VARIANT)'%ALL_variants,
    default_cmdargs['variant'] )
opts.Add(
    'cg',
    'Support Cg language or not. (GN_BUILD_ENABLE_CG)',
    default_cmdargs['enable_cg'] )
opts.Add(
    'prof',
    'Enable performance profiler. (GN_BUILD_ENABLE_PROFILE)',
    default_cmdargs['enable_profile'] )

env = Environment( options = opts )

################################################################################
#
# 定义 global functions
#
################################################################################

# 输出调试信息
def GN_trace( env, level, msg ):
    level = float(level)
    assert( level > 0 )
    if ( CONF_trace > 0 and level <= CONF_trace ) or ( CONF_trace < 0 and level == -CONF_trace ):
        print 'TRACE(%d) : %s'%(level,msg)

# 输出提示信息
def GN_info( env, msg ): print 'INFO : %s'%msg

# 输出警告信息
def GN_warn( env, msg ):
    print '===================================================================='
    print 'WARNING : %s'%msg
    print '===================================================================='

# 输出错误信息
def GN_error( env, msg ):
    print '===================================================================='
    print 'ERROR : %s'%msg
    print '===================================================================='

# 生成从target到base的相对路径
def GN_relpath( env, target, base ):
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
def GN_glob( env, patterns, recursive = False ):
    def do_glob( env, pattern, dir, recursive ):
        files = []
        root = Dir(dir).srcnode().abspath;
        try:
            for file in os.listdir( root ):
                if os.path.isdir( os.path.join(root,file) ):
                    if recursive and ( not '.svn' == file ) : # ignore subversion directory
                        files = files + do_glob( env, pattern, os.path.join(dir,file), recursive )
                else:
                    # Note: ignore precompiled header
                    if not ('pch.cpp' == file or 'stdafx.cpp' == file):
                        #print 'fnmatch(%s,%s) = %s'%(file,pattern,fnmatch.fnmatch(file, pattern))
                        if fnmatch.fnmatch(file, pattern):
                            files.append( os.path.join( dir, file ) )
        except WindowsError:
            pass
        return files
    #GN_info( env, 'GN_glob %s'%patterns )
    files = []
    if not patterns is list: patterns = [patterns]
    for p in Flatten(patterns):
        if os.path.isdir( env.GetBuildPath(p) ):
            #print '    do_glob(*.*,%s)'%p
            files += do_glob( env, '*.*', p, recursive )
        else:
            (dir,pattern) = os.path.split(p);
            if '' == pattern: pattern = '*.*';
            if '' == dir: dir = '.';
            #print '    do_glob(%s,%s)'%(pattern,dir)
            files += do_glob( env, pattern, dir, recursive )
    return files

# 创建 source cluster
def GN_newBuildFlags( env ): return BuildFlags()

# 创建 source cluster
def GN_newSourceCluster( env, sources, pchHeader = None, pchSource = None ):
    s = SourceCluster()
    s.sources = [File(x) for x in sources]
    if pchHeader: s.pchHeader = pchHeader
    if pchSource: s.pchSource = File(pchSource)
    return s

# 创建 Target
def GN_newTarget( env, type, name, sources = None, pdb = None, dependencies = None ):

    # check for redundant target
    if ALL_targets[CURRENT_compiler][CURRENT_variant].has_key( name ) :
        env.GN_info( "Ingore redundant target: %s"%(name) )
        return

    # create new target instance
    t = Target()
    if ( 'stdbg' == CURRENT_variant or 'strel' == CURRENT_variant ) and 'shlib' == type: type = 'stlib'
    t.type = type
    t.path = Dir('.')
    t.sources = sources
    t.dependencies = dependencies
    if pdb  : t.pdb = File(pdb)
    else    : t.pdb = File( "%s.pdb"%(name) )

    # insert to global target list
    ALL_targets[CURRENT_compiler][CURRENT_variant][name] = t

    return t

# register global functions to scons.
from SCons.Script.SConscript import SConsEnvironment
SConsEnvironment.GN_trace = GN_trace;
SConsEnvironment.GN_info = GN_info;
SConsEnvironment.GN_warn = GN_warn;
SConsEnvironment.GN_error = GN_error;
SConsEnvironment.GN_relpath = GN_relpath;
SConsEnvironment.GN_glob = GN_glob
SConsEnvironment.GN_newSourceCluster = GN_newSourceCluster
SConsEnvironment.GN_newTarget = GN_newTarget

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

    def addCompileFlags( self,
                         CPPPATH = None,
                         CPPDEFINES = None,
                         CCFLAGS = None,
                         CXXFLAGS = None ):
        if CPPPATH : self.extraCompileFlags.cpppath += CPPPATH
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
        self.type = None
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
# Local functions
#
################################################################################

def genBuildRoot() : return os.path.join( 'bin', 'tmp', CONF_platform )
def genBuildDir( compiler, variant ) : return os.path.join( genBuildRoot(), compiler, variant )

#
# Create new build environment
#
___DEFAULT_envs = {}
def newDefaultEnv( compiler, variant ):
    def createDefaultEnv( compiler, variant ):
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

        env.SConsignFile( os.path.join( genBuildRoot(), '.sconsign.dbm' ) )

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
        def generate_empty_options() : return { 'common':[],'debug':[],'release':[],'stdbg':[],'strel':[] }
        cppdefines = generate_empty_options()
        cpppath    = generate_empty_options()
        libpath    = generate_empty_options()
        libs       = generate_empty_options()
        ccflags    = generate_empty_options()
        cxxflags   = generate_empty_options()
        linkflags  = generate_empty_options()

        # define profile tag
        if float(CONF_enable_profile): cppdefines['common'] = ['GN_ENABLE_PROFILE=1']

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

            cppdefines['debug']   += Split('_DEBUG')
            cppdefines['stdbg']   += Split('_DEBUG')

            if float(env['MSVS_VERSION']) >= 8.0:
                cxxflags['common']  += ['/EHa']
                linkflags['common'] += Split( '/NODEFAULTLIB:libcp.lib' )
            else:
                cxxflags['common']  += ['/EHs']

            ccflags['common']  += Split('/W4 /WX')
            ccflags['debug']   += Split('/MDd /GR')
            ccflags['release'] += Split('/MD /O2')
            ccflags['stdbg']   += Split('/MTd /GR')
            ccflags['strel']   += Split('/MT /O2')

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

    if not compiler in ___DEFAULT_envs:
        ___DEFAULT_envs[compiler] = {}
    if not variant  in ___DEFAULT_envs[compiler]:
        ___DEFAULT_envs[compiler][variant] = createDefaultEnv( compiler, variant )
    env = ___DEFAULT_envs[compiler][variant].Copy()
    return env

#
# Check sysem configuration
#
def checkConfig( conf, confDir, compiler, variant ):
    env = newDefaultEnv( compiler, variant )

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
# Collect targets
#
################################################################################

if 'all' in COMMAND_LINE_TARGETS:
    variants = Split( ALL_variants )
    compilers = Split( ALL_compilers )
else:
    if 'all' == CONF_variant:
        variants = Split( ALL_variants )
    else:
        variants = Split( CONF_variant )
    if 'all' == CONF_compiler:
        compilers = Split( ALL_compilers )
    else:
        compilers = Split( CONF_compiler )
#
# collect configurations
#
ALL_configs = []
ALL_targets = {}
for c in compilers:

    if not c in ALL_compilers:
        print "ERROR: Ignore invalid compiler '%s'"%c
        continue

    ALL_targets[c] = {}

    for v in variants:

        if not v in ALL_variants:
            print "ERROR: Ignore invalid variant '%s'"%v
            continue

        static = 'debug'==v or 'release'==v
        debug  = 'debug'==v or 'stdbg'==v

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

        buildDir = genBuildDir(c,v)

        checkConfig( conf, os.path.join(buildDir,'config'), c, v )

        ALL_configs.append( conf )

        ALL_targets[c][v] = {}

#
# Call sub-script files for all configurations
#
for c in ALL_configs :
    CURRENT_conf = c
    CURRENT_compiler = c['compiler']
    CURRENT_variant = c['variant']
    bldDir = genBuildDir( CURRENT_compiler, CURRENT_variant )
    SConscript( 'SConscript', exports={'conf':c}, build_dir=bldDir, duplicate=0 )

################################################################################
#
# buill targets
#
################################################################################

BUILD_compiler = None
BUILD_variant = None
BUILD_bldDir = None
BUILD_libDir = None
BUILD_binDir = None

#
# Create new compile environment
#
def newCompileEnv( cluster ):
    env = newDefaultEnv( BUILD_compiler, BUILD_variant )

    env.Prepend( CPPPATH = ['#src/extern/inc', 'src/priv/inc'] )

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
def newLinkEnv( target ):
    env = newDefaultEnv( BUILD_compiler, BUILD_variant )

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

def isStatic( variant ) : return 'stdbg' == variant or 'strel' == variant

#
# setup environment for producing PCH, return PCH object file
#
def setup_PCH( env, pchHeader, pchSource ):
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
def buildStaticObjs( cluster ):
    env = newCompileEnv( cluster )
    pchObj = setup_PCH( env, cluster.pchHeader, cluster.pchSource )
    return Flatten( [env.Object(x) for x in cluster.sources] + pchObj )

#
# build shared objects
#
def buildStaticObjs( cluster ):
    env = newCompileEnv( cluster )
    pchObj = setup_PCH( env, cluster.pchHeader, cluster.pchSource )
    return Flatten( [env.SharedObject(x) for x in cluster.sources] + pchObj )

#
# build static library
#
def buildStaticLib( name, target ):
    objs = []
    for s in target.sources: objs += buildStaticObjs( s )
    env = newLinkEnv( target )
    libName = '%s%s%s'%(env['LIBPREFIX'],name,env['LIBSUFFIX'])
    target.targets = env.Install( BUILD_libDir, env.Library( os.path.join(str(target.path),libName), objs ) )
    Alias( name, target.targets )
    Default( target.targets )

#
# handle dependencies
#
def toList( x ):
    if x : return x
    else : return []

def addLib( env, name, lib ):
    if not lib in env['LIBS']:
        env.Prepend( LIBS = [lib] )
        env.GN_trace( 1, 'Add depends of %s : %s'%(name,lib) )

def addExternalDependencies( env, name, deps ):
    for x in reversed(deps):
        addLib( env, name, x )

def addDependencies( env, name, deps ):
    targets = ALL_targets[BUILD_compiler][BUILD_variant]
    for x in reversed(deps):
        if x  in targets:
            addExternalDependencies( env, name, toList(targets[x].externalDependencies) )
            addDependencies( env, name, toList(targets[x].dependencies) )
            if 'GNcore' == x or 'stlib' == targets[x].type : # here we ignore shared libraried other then GNcore.
                addLib( env, name, x )
        else:
            env.GN_warn( "Ingore non-exist dependency for target %s: %s"%(name,x) )

#
# does compiler produce manifest file?
#
def handleManifest( env, targets ):
    if float(env.get('MSVS_VERSION',0)) >= 8.0 and not isStatic( BUILD_variant ):
        # TODO : handle manifest file.
        #manifest = File( os.path.join( os.path.dirname(targets[0].abspath), '%s.dll.manifest'%target ) )
        #env.SideEffect( manifest, result )
        #result += [manifest]
        pass

#
# build static library
#
def buildSharedLib( name, target ):
    objs = []
    for s in target.sources: objs += buildStaticObjs( s )

    env = newLinkEnv( target )

    stdlibs = []
    if 'GNcore' != name : stdlibs.append( 'GNcore' )
    stdlibs += Split('GNbase GNextern')

    addDependencies( env, name, toList(target.dependencies) + stdlibs )
    addExternalDependencies( env, name, toList(target.externalDependencies) )
    env.GN_trace( 1, "Depends of %s : %s"%(name,env['LIBS']) )

    libName = '%s%s%s'%(env['SHLIBPREFIX'],name,env['SHLIBSUFFIX'])
    shlib = env.SharedLibrary( os.path.join(str(target.path),libName), objs )
    handleManifest( env, shlib )
    target.targets = env.Install( BUILD_binDir, env.Install( BUILD_libDir, shlib ) )

    Alias( name, target.targets )
    Default( target.targets )

#
# build executable
#
def buildProgram( name, target ):
    objs = []
    for s in target.sources: objs += buildStaticObjs( s )

    env = newLinkEnv( target )

    stdlibs = Split('GNgfxD3D9 GNgfxD3D10 GNgfxOGL GNcore GNbase GNextern')

    addDependencies( env, name, toList(target.dependencies) + stdlibs )
    addExternalDependencies( env, name, toList(target.externalDependencies) )
    env.GN_trace( 1, "Depends of %s : %s"%(name,env['LIBS']) )

    exeName = '%s%s%s'%(env['PROGPREFIX'],name,env['PROGSUFFIX'])
    prog = env.Program( os.path.join(str(target.path),exeName), objs )
    handleManifest( env, prog )
    target.targets = env.Install( BUILD_binDir, prog )

    Alias( name, target.targets )
    Default( target.targets )

#
# build all targets
#
for compiler, c in ALL_targets.iteritems() :
    BUILD_compiler = compiler
    for variant, v in c.iteritems():
        BUILD_variant = variant
        BUILD_bldDir = genBuildDir( compiler, variant )
        BUILD_libDir = os.path.join( BUILD_bldDir, 'lib' )
        BUILD_binDir = os.path.join( BUILD_bldDir, 'bin' )
        for name, x in v.iteritems():

            if 'stlib' == x.type   : buildStaticLib( name, x )
            elif 'shlib' == x.type : buildSharedLib( name, x )
            elif 'prog' == x.type  : buildProgram( name, x )
            else: env.GN_error( 'Unknown target type for target %s: %s'%(name,x.type) )

            env.GN_trace( 1, "%s : compiler(%s), variant(%s), type(%s), path(%s), targets(%s)"%(
                name, compiler, variant, x.type, x.path, [str(t) for t in x.targets] ) )

#
# special case for build documents
#

################################################################################
#
# 定义 help screen
#
################################################################################

help_text = """
Usage:
    scons [options] [target1 target2 ...]

Options:%s

""" % (
    opts.GenerateHelpText(env),
    )
Help( help_text )
