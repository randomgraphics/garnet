# -*- coding: GB18030 -*-

import os, os.path, re, fnmatch

# try import xenon module
try:
	import SCons.Tool.xenon
	CONF_has_xenon_extension = True
except ImportError:
	CONF_has_xenon_extension = False


# enviroment use by local functions
LOCAL_env = Environment( tools=[] )

################################################################################
#
# 定义编译选项
#
################################################################################

# 读取环境变量
def getenv( name, defval = None ):
	if name in os.environ: return os.environ[name]
	else: return defval

# 检测当前的OS和CPU类型
CONF_os = None
CONF_cpu = None
if 'win32' == LOCAL_env['PLATFORM']:
	CONF_os = 'mswin'
	if 'AMD64' == getenv('PROCESSOR_ARCHITECTURE') or 'AMD64' == getenv('PROCESSOR_ARCHITEW6432'):
		CONF_cpu = 'x64'
	else:
		CONF_cpu = 'x86'
elif 'cygwin' == LOCAL_env['PLATFORM']:
	CONF_os = 'cygwin'
	CONF_cpu = 'x86'
elif 'posix' == LOCAL_env['PLATFORM']:
	CONF_os = 'posix'
	CONF_cpu = 'x86'
else:
	print 'FATAL: Unknown platform:', LOCAL_env['PLATFORM']
	Exit(-1)

# 定义可用的编译器列表
class Compiler:
	def __init__( self, name, os, cpu ):
		self.name = name; # compiler name
		self.os = os; # target OS
		self.cpu = cpu; # target CPU

	def __repr__( self ) : return '%s-%s-%s'%(self.name,self.os,self.cpu)
	def __str__( self ) : return '%s-%s-%s'%(self.name,self.os,self.cpu)

CONF_allCompilers = []
if 'mswin' == CONF_os:
	CONF_allCompilers.append( Compiler('vc71','mswin','x86') )
	CONF_allCompilers.append( Compiler('vc80','mswin','x86') )
	CONF_allCompilers.append( Compiler('vc80','mswin','x64') )
	CONF_allCompilers.append( Compiler('icl','mswin','x86') )
	CONF_allCompilers.append( Compiler('icl','mswin','x64') )
	CONF_allCompilers.append( Compiler('icl','mswin','ia64') )
	CONF_allCompilers.append( Compiler('mingw','mswin','x86') )
	if CONF_has_xenon_extension:
		if SCons.Tool.xenon.exists( LOCAL_env ):
			CONF_allCompilers.append( Compiler('xenon','xenon','ppc') )
elif 'cygwin' == CONF_os:
	CONF_allCompilers.append( Compiler('gcc','cygwin','x86') )
else:
	assert( 'posix' == CONF_os )
	CONF_allCompilers.append( Compiler('gcc','posix','x86') )

# Get a compiler for specific target OS and CPU type.
def CONF_getCompiler( os, cpu ):
	for c in CONF_allCompilers:
		if c.os == os and c.cpu == cpu : return c;
	return None;

# 定义编译模式
CONF_allVariants = 'debug profile retail stdbg stprof stret'

# 定义缺省的命令行选项
CONF_defaultCmdArgs = {
	'trace'     : getenv('GN_BUILD_TRACE',  '0'),
	'variant'   : getenv('GN_BUILD_VARIANT', 'debug'),
	'compiler'  : getenv('GN_BUILD_COMPILER', CONF_getCompiler(CONF_os,CONF_cpu).name ),
	'os'        : getenv('GN_BUILD_TARGET_OS', CONF_os ),
	'cpu'       : getenv('GN_BUILD_TARGET_CPU', CONF_cpu ),
	'cg'        : getenv('GN_BUILD_ENABLE_CG', 1), # use Cg by default.
	'xedeploy'  : getenv('GN_BUILD_XEDEPLOY', 1), # copy to devkit, default is true.
	'locale'    : getenv('GN_BUILD_LOCALE', 'CHS'),
	'sdkroot'   : getenv('GN_BUILD_SDK_ROOT', '<svnroot>\\bin\\sdk') # root directory of SDK installation
	}

# 是否打开trace
CONF_trace = float( ARGUMENTS.get('trace', CONF_defaultCmdArgs['trace']) )

# 定义编译类型
CONF_variant = ARGUMENTS.get('variant', CONF_defaultCmdArgs['variant'] )

# 定义编译器类型
CONF_compiler = Compiler( ARGUMENTS.get('compiler', CONF_defaultCmdArgs['compiler'] ),
						  ARGUMENTS.get('os', CONF_defaultCmdArgs['os'] ),
						  ARGUMENTS.get('cpu', CONF_defaultCmdArgs['cpu'] ) )

# 是否支持Cg语言.
CONF_enableCg  = float( ARGUMENTS.get( 'cg', CONF_defaultCmdArgs['cg'] ) )

# copy to devkit
CONF_xedeploy = float( ARGUMENTS.get( 'xedeploy', CONF_defaultCmdArgs['xedeploy'] ) )

# installation directory
CONF_sdkroot = ARGUMENTS.get( 'sdkroot', os.path.join( '#bin', 'sdk' ) )

################################################################################
#
# 局部于本文件的工具函数
#
################################################################################

# 输出调试信息
def UTIL_trace( level, msg ):
	level = float(level)
	assert( 0 != level )
	if ( CONF_trace > 0 and level <= CONF_trace ) or ( CONF_trace < 0 and level == -CONF_trace ):
		print 'TRACE(%d) : %s'%(level,msg)

# 输出提示信息
def UTIL_info( msg ): print 'INFO : %s'%msg

# 输出警告信息
def UTIL_warn( msg ):
	#print '===================================================================='
	print 'WARNING : %s'%msg
	#print '===================================================================='

# 输出错误信息
def UTIL_error( msg ):
	print '===================================================================='
	print 'ERROR : %s'%msg
	print '===================================================================='

#
def UTIL_staticBuild( v ): return 'stdbg' == v or 'stprof' == v or 'stret' == v

# get sub directory of a specific compiler and build variant
def UTIL_bldsubdir( compiler, variant ):
	if not isinstance( compiler, Compiler ):
		assert( isinstance(compiler,int) )
		assert( isinstance(variant,int) )
		return ''
	else:
		return compiler.os + '.' + compiler.cpu + '.' + compiler.name + '.' + variant

# get root directory of build directory
def UTIL_buildRoot() :
	return os.path.join( '#build.tmp' )

# get build directory of specific compiler and build variant
def UTIL_buildDir( compiler, variant ) :
	return os.path.join( UTIL_buildRoot(), UTIL_bldsubdir( compiler, variant ) )

#
# Create new build environment
#
def UTIL_newEnv( compiler, variant ):

	if not isinstance( compiler, Compiler ):
		assert( isinstance(compiler,int) )
		assert( isinstance(variant,int) )
		return Environment( tools=[] )

	if 'mswin' == CONF_os: tools = ['msvc','mslink','mslib','msvs']
	else: tools = ['default']

	msvs_version = '8.0'
	msvs_platform = 'x86'
	icl_version = None
	icl_abi = 'ia32'
	if 'xenon' == compiler.name:
		tools = ['xenon']
		msvs_platform = 'Xbox 360'
	elif 'icl' == compiler.name :
		tools += ['intelc']
		if 'x64' == compiler.cpu :
			icl_abi = 'em64t'
			msvs_platform = 'x64'
	elif 'x64' == compiler.cpu :
		msvs_platform = 'x64'
	elif 'mingw' == compiler.name :
		tools = ['mingw']
	env = Environment(
		tools          = tools,
		MSVS_VERSION   = msvs_version,
		MSVS8_PLATFORM = msvs_platform,
		ICL_VERSION    = icl_version,
		ICL_ABI        = icl_abi,
		ENV            = {
		                 	'PATH'     : getenv('PATH'),
		                 	'LANG'     : getenv('LANG'),
		                 	'LANGUAGE' : getenv('LANGUAGE'),
		                 	'INCLUDE'  : getenv('INCLUDE'),
		                 	'LIB'      : getenv('LIB'),
		                 }
		)
	env.SConsignFile( File( os.path.join( UTIL_buildRoot(), '.sconsign.dbm' ) ).path )

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
	def generate_empty_options() : return { 'neutral':[], 'common':[],'debug':[],'profile':[],'retail':[],'stdbg':[],'stprof':[],'stret':[] }
	cppdefines = generate_empty_options()
	cpppath    = generate_empty_options()
	libpath    = generate_empty_options()
	libs       = generate_empty_options()
	ccflags    = generate_empty_options()
	cxxflags   = generate_empty_options()
	linkflags  = generate_empty_options()

	# 定制不同编译模式的编译选项
	cppdefines['common']  += ['UNICODE','_UNICODE']
	cppdefines['retail']  += ['GN_BUILD_VARIANT=0','NDEBUG']
	cppdefines['profile'] += ['GN_BUILD_VARIANT=1','NDEBUG']
	cppdefines['debug']   += ['GN_BUILD_VARIANT=2']
	cppdefines['stret']   += ['GN_BUILD_VARIANT=3','NDEBUG']
	cppdefines['stprof']  += ['GN_BUILD_VARIANT=4','NDEBUG']
	cppdefines['stdbg']   += ['GN_BUILD_VARIANT=5']

	# 定制不同平台的编译选项
	if 'xenon' == compiler.os:
		ccflags['common'] += ['/QVMX128']
		cppdefines['profile'] += ['PROFILE']
		cppdefines['stprof']  += ['PROFILE']
		linkflags['profile'] += ['/NODEFAULTLIB:xapilib.lib']
		linkflags['stprof'] += ['/NODEFAULTLIB:xapilib.lib']
		libs['common']  += Split('xboxkrnl xbdm dxerr9')
		libs['debug']   += Split('xapilibd d3d9d d3dx9d xgraphicsd xnetd xaudiod xactd vcompd libcmtd')
		libs['profile'] += Split('xapilibi d3d9i d3dx9  xgraphics  xnet  xaudioi xacti vcomp  libcmt')
		libs['retail']  += Split('xapilib  d3d9  d3dx9  xgraphics  xnet  xaudio  xact  vcomp  libcmt')
		libs['stdbg']   += Split('xapilibd d3d9d d3dx9d xgraphicsd xnetd xaudiod xactd vcompd libcmtd')
		libs['stprof']  += Split('xapilibi d3d9i d3dx9  xgraphics  xnet  xaudioi xacti vcomp  libcmt')
		libs['stret']   += Split('xapilib  d3d9  d3dx9  xgraphics  xnet  xaudio  xact  vcomp  libcmt')
	elif 'mswin' == compiler.os:
		libs['common'] += Split('kernel32 user32 gdi32 shlwapi advapi32 shell32')
	else:
		cpppath['common'] += Split('/usr/X11R6/include /usr/local/include')
		libpath['common'] += Split('/usr/X11R6/lib /usr/local/lib')

	# 定制不同编译器的编译选项
	if 'cl' == env['CC']:
		#
		# To workaround an bug in scons 0.96.1, this variable has to be imported
		# or else, link.exe will report error: cannot open file 'TEMPFILE'
		#
		env['ENV']['USERPROFILE'] = os.environ['USERPROFILE']

		cppdefines['debug']   += ['_DEBUG']
		cppdefines['stdbg']   += ['_DEBUG']

		cxxflags['common']  += ['/EHa']

		ccflags['common']  += ['/W4', '/WX', '/Ot', '/Oi'] # favor speed, enable intrinsic functions.
		ccflags['debug']   += ['/GR', '/RTCscu']
		ccflags['profile'] += ['/Ox'] # maximum optimization.
		ccflags['retail']  += ['/Ox', '/GL']
		ccflags['stdbg']   += ['/MTd', '/GR']
		ccflags['stprof']  += ['/MT', '/Ox']
		ccflags['stret']   += ['/MT', '/Ox', '/GL']
		if 'xenon' == compiler.os:
			ccflags['debug']   += ['/MTd']
			ccflags['profile'] += ['/MT']
			ccflags['retail']  += ['/MT']
		else:
			ccflags['debug']   += ['/MDd']
			ccflags['profile'] += ['/MD']
			ccflags['retail']  += ['/MD']

		if float(env['MSVS_VERSION']) >= 8.0:
			linkflags['common'] += ['/NODEFAULTLIB:libcp.lib']
		linkflags['common']  += ['/FIXED:NO', '/DEBUGTYPE:CV,FIXUP'] # this is for vtune and magellan to do instrumentation
		linkflags['profile'] += ['/OPT:REF']
		linkflags['stprof']  += ['/OPT:REF']
		linkflags['retail']  += ['/OPT:REF','/LTCG:STATUS']
		linkflags['stret']   += ['/OPT:REF','/LTCG:STATUS']

	elif 'icl' == env['CC']:
		ccflags['common']  += ['/W3','/WX','/Wcheck','/Qpchi-','/Zc:forScope']
		ccflags['debug']   += ['/MDd','/GR','/Ge','/traceback']
		ccflags['profile'] += ['/O2','/MD']
		ccflags['retail']  += ['/O2','/MD']
		ccflags['stdbg']   += ['/MTd','/GR','/Ge','/traceback']
		ccflags['stret']   += ['/O2','/MT']

		cxxflags['common'] += ['/EHs']

		cppdefines['debug'] += ['_DEBUG']
		cppdefines['stdbg'] += ['_DEBUG']

		linkflags['common']  += ['/FIXED:NO', '/DEBUGTYPE:CV,FIXUP'] # this is for vtune and magellan to do instrumentation
		linkflags['profile'] += ['/OPT:REF']
		linkflags['retail']  += ['/OPT:REF']
		linkflags['stprof']  += ['/OPT:REF']
		linkflags['stret']   += ['/OPT:REF']

	elif 'gcc' == env['CC']:
		ccflags['common']  += ['-Wall','-Werror']
		ccflags['debug']   += ['-g']
		ccflags['profile'] += ['-O3']
		ccflags['retail']  += ['-O3']
		ccflags['stdbg']   += ['-g']
		ccflags['stprof']  += ['-O3']
		ccflags['stret']   += ['-O3']
		if 'mingw' == compiler.name:
			cppdefines['common'] += ['WINVER=0x500']

	else:
		UTIL_error( 'unknown compiler: %s'%env['CC'] )
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
	env.Append( CPPPATH = ['src/extern/inc'] )
	c = env.Configure(
		conf_dir = confDir,
		log_file = os.path.join(confDir,'config.log') )

	# =================
	# 是否支持Cg shader
	# =================
	if CONF_enableCg :
		conf['has_cg']      = c.CheckLibWithHeader( 'cg', 'cg/cg.h', 'C', 'cgCreateContext();' )
		conf['has_cg_d3d9'] = c.CheckLibWithHeader( 'cgD3D9', ['cg/cg.h','cg/cgD3D9.h'], 'C', "cgD3D9SetDevice(0);" )
		conf['has_cg_ogl']  = c.CheckLibWithHeader( 'cgGL', ['cg/cg.h','cg/cgGL.h'], 'C', "cgGLRegisterStates(0);" )
	else:
		conf['has_cg']      = 0
		conf['has_cg_d3d9'] = 0
		conf['has_cg_ogl']  = 0

	# ==============
	# 是否支持OpenGL
	# ==============
	if c.CheckLibWithHeader( 'opengl32', ['glew.h'], 'C', 'glVertex3f(0,0,0);' ) and \
		c.CheckLibWithHeader( 'glu32', ['glew.h','GL/glu.h'], 'C', 'gluOrtho2D(0,0,0,0);' ) :
		conf['has_ogl'] = True
	elif c.CheckLibWithHeader( 'GL', ['glew.h'], 'C', 'glVertex3f(0,0,0);' ) and \
		c.CheckLibWithHeader( 'GLU', ['glew.h','GL/glu.h'], 'C', 'gluOrtho2D(0,0,0,0);' ) :
		conf['has_ogl'] = True
	else :
		conf['has_ogl'] = False

	# ============
	# 是否支持XTL
	# ============
	conf['has_xtl'] = c.CheckCHeader( 'xtl.h' )

	# ============
	# 是否支持D3D9
	# ============
	conf['has_d3d9'] = conf['has_xtl'] or c.CheckCHeader( 'd3d9.h' ) and c.CheckCHeader( 'd3dx9.h' )

	# =============
	# 是否支持D3D10
	# =============

	# Detect Windows Vista
	def isVista( env ):
		if 'win32' != env['PLATFORM']: return False
		stdout = os.popen( "ver" )
		str = stdout.read()
		stdout.close()
		import string
		return string.find( str, "6.0" ) >= 0
	conf['has_d3d10'] = c.CheckCXXHeader( 'd3d10.h' ) and ( isVista(env) or not UTIL_staticBuild( variant ) )

	# ===================
	# 是否支持DirectInput
	# ===================
	conf['has_dinput'] = c.CheckCHeader( ['windows.h', 'dinput.h'] )

	# ==============
	# 是否支持XInput
	# ==============
	conf['has_xinput'] = c.CheckCHeader( ['windows.h', 'XInput.h'] )

	# =========================
	# 检查是否存在boost library
	# =========================
	conf['has_boost'] = c.CheckCXXHeader( 'boost/any.hpp' )

	# ==========
	# 检查 cegui
	# ==========
	conf['has_cegui'] = c.CheckCXXHeader( 'CEGUI.h' )

	# ===========
	# 检查 maxsdk
	# ===========
	conf['has_maxsdk'] = c.CheckCHeader( ['max.h','maxapi.h','maxversion.h'] )

	# =========
	# 检查 GLUT
	# =========
	if c.CheckLibWithHeader( 'glut', 'GL/glut.h', 'C', 'glutInit(0,0);' ) :
	    conf['has_glut'] = True
	    conf['glut'] = 'glut'
	elif c.CheckLibWithHeader( 'glut32', 'GL/glut.h', 'C', 'glutInit(0,0);' ) :
	    conf['has_glut'] = True
	    conf['glut'] = 'glut32'
	else :
	    conf['has_glut'] = None

	# =====================
	# Copy to devkit or not
	# =====================
	conf['xedeploy'] = CONF_xedeploy;

	# =============
	# finish config
	# =============
	c.Finish()

#
# Copy to Xenon devkit
#
class UTIL_copy_to_devkit:

	def __init__( self, targetDir ):
		self.targetDir = targetDir

	def mkdir( self, path, env ):
		import popen2
		parent = os.path.dirname( path )
		if len(parent) > 0 :
			self.mkdir( parent, env )
		popen2.popen3( 'xbmkdir %s'%path )

	def __call__( self, target, source, env ):
		for x in target:
			self.mkdir( self.targetDir, env )
			env.Execute( 'xbcp /Q /D /Y %s %s'%(x.abspath,self.targetDir) )
			t = '%s\\%s'%(self.targetDir,os.path.basename(x.path))
		return 0

################################################################################
#
# 定义 GarnetEnv class
#
################################################################################

class GarnetEnv :

	def __init__( self ) :
		self.compiler = None
		self.variant = None
		self.conf = {}

	# UTIL functions
	def buildDir( self, compiler, variant ) : return UTIL_buildDir( compiler, variant )
	def newEnv( self, compiler, variant ) : return UTIL_newEnv( compiler, variant )
	def trace( self, level, msg ): UTIL_trace( level, msg )
	def info( self, msg ): UTIL_info( msg )
	def warn( self, msg ): UTIL_warn( msg )
	def error( self, msg ): UTIL_error( msg )
	def copy_to_devkit( self, targetDir ) : return UTIL_copy_to_devkit( targetDir )

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
		s.sources = [File(x) for x in Flatten(sources)]
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
	def newTarget( self, type, name, sources, dependencies = [], ignoreDefaultDependencies = False, pdb = None ):
		# create new target instance
		t = Target()
		if 'shlib' == type:
			if UTIL_staticBuild( self.variant ) : type = 'stlib'
			else : type = 'dylib'
		t.compiler = self.compiler
		t.variant = self.variant
		t.type = type
		t.path = Dir('.')
		t.sources = sources
		t.dependencies = dependencies
		t.ignoreDefaultDependencies = ignoreDefaultDependencies
		if pdb : t.pdb = File(pdb)
		else   : t.pdb = File("%s.pdb"%(name))
		ALL_targets[self.compiler][self.variant][name] = t # insert to global target list
		return t

	# 创建 custom compiler and platform _DEPENDANT_ target
	def newCustomTarget( self, name, sources ):

		# create new target instance
		t = Target()
		t.compiler = self.compiler
		t.variant = self.variant
		t.type = 'custom'
		t.path = Dir('.')
		t.sources = sources
		ALL_targets[self.compiler][self.variant][name] = t # insert to global target list
		return t

	# 创建 compiler _INDEPENDANT_ target.
	def newNeutralCustomTarget( self, name, sources ):

		# check for redundant target
		if name in ALL_targets[0][0] : return ALL_targets[0][0]

		# create new target instance
		t = Target()
		t.compiler = self.compiler
		t.variant = self.variant
		t.type = 'custom'
		t.path = Dir('.')
		t.sources = sources
		ALL_targets[0][0][name] = t # insert to global target list
		return t

# Create garnet build environment (singleton)
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
		self.compiler = None
		self.variant = None
		self.type = None # could be 'stlib, dylib, shlib, prog, custom'
		self.path = None
		self.targets = []
		self.sources = [] # list of source clusters
		self.dependencies = [] # list of denpendencies. item in this list must be valid target name.
		self.externalDependencies = [] # list of external dependencies. Could be any name.
		self.ignoreDefaultDependencies = False
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

def COLLECT_getCompilers( candidate ):
	result = []
	for c in CONF_allCompilers:
		if ( 'all' == candidate.name or c.name == candidate.name ) and \
		   ( 'all' == candidate.os or c.os == candidate.os ) and \
		   ( 'all' == candidate.cpu or c.cpu == candidate.cpu ) : result.append( c )
	if 0 == len(result): print 'ERROR : invalid compiler : %s'%candidate
	return result

COLLECT_compilers = []
COLLECT_variants = []
if 'all' in COMMAND_LINE_TARGETS:
	COLLECT_compilers = CONF_allCompilers
	COLLECT_variants = Split( CONF_allVariants )
else:
	COLLECT_compilers = COLLECT_getCompilers( CONF_compiler )
	if 'all' == CONF_variant:
		COLLECT_variants = Split( CONF_allVariants )
	else:
		COLLECT_variants = Split( CONF_variant )

ALL_conf = []
ALL_targets = { 0 : { 0 : {} } }
for c in COLLECT_compilers:

	ALL_targets[c] = {}

	for v in COLLECT_variants:

		if not v in CONF_allVariants:
			print "ERROR: Ignore invalid variant '%s'"%v
			continue

		conf = {}
		UTIL_checkConfig( conf, os.path.join(UTIL_buildDir(c,v),'config'), c, v )

		ALL_conf.append( [c,v,conf] )

		ALL_targets[c][v] = {}

################################################################################
#
# Call SConscript to generate target instances
#
################################################################################

for c in ALL_conf:
	GN.compiler = c[0]
	GN.variant = c[1]
	GN.conf = c[2]
	SConscript( 'SConscript', exports=['GN'], variant_dir=UTIL_buildDir( GN.compiler, GN.variant ), duplicate=1 )

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

#define all targets

TARGET_headers= [
	'GNinc',
	]

TARGET_stlibs = [
	'GNextern',
	'GNbase',
	'GNgfxUtil',
	'GNrndrCommon',
	'GNutil',
	]

TARGET_shlibs = [
    'GNcore',
    'GNrndrOGL',
    ]

TARGET_tests = [
    'GNtestCegui',
    'GNtestD3D10',
    'GNtestDrawable',
    'GNtestEngine',
    'GNtestFt2',
    'GNtestGfx',
    'GNtestGfx2',
    'GNtestInput',
    'GNtestOGL',
    'GNtestPcre',
    'GNtestSprite',
    'GNtestRndr',
    'GNtestXenonNegativeZRange',
    'GNtestXenonStackTexture',
    'GNtestXenonVertexEndian',
    'GNtestXml',
    'GNut',
    ]

TARGET_samples = [
    'GNsampleDepthTexture',
    'GNsampleDolphin',
    'GNsampleRenderToCube',
    'GNsampleRenderToTexture',
    'GNsampleShadowMap',
    ]

TARGET_tools = [
    'GNtoolD3D10DumpViewer',
    'GNtoolD3D9DumpViewer',
    'GNtoolD3D9Wrapper',
    'GNtoolGPUBenchmark',
    'GNtoolMeshConverter',
    'GNtoolMeshViewer',
    'GNtoolOGLInfo',
    ]

TARGET_misc = [
	'GNmedia',
	]

ALL_aliases = {} # empty map

#
# define new build alias
#
def ALIAS_add_default( alias, targets ):
	Alias( alias, targets )
	Default( alias )
	ALL_aliases[alias] = 1

#
# define non-default build alias
#
def ALIAS_add_non_default( alias, targets ):
	Alias( alias, targets )
	ALL_aliases[alias] = 0

#
# Get libarary suffix. Currently, none.
#
def BUILD_getSuffix(): return ""

#
# Create new compile environment
#
def BUILD_newCompileEnv( cluster ):
	env = BUILD_env.Clone()

	env.Prepend( CPPPATH = [
		'#src/extern/inc',
	    BUILD_bldDir + '/src/priv/inc',
		'src/priv/inc',
		])

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
	env = BUILD_env.Clone()

	def extname( path ):
		p,e = os.path.splitext( str(path) )
		return e

	if target.pdb:
		p,e = os.path.splitext( target.pdb.path )
		env['PDB'] = p+BUILD_getSuffix()+e

	if 'gcc' == env['CC']:
		env.Prepend( LIBPATH = [BUILD_binDir,BUILD_libDir] )
	else:
		env.Prepend( LIBPATH = [BUILD_libDir] )

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
	libName = '%s%s%s%s'%( env['LIBPREFIX'], name, BUILD_getSuffix(), env['LIBSUFFIX'] )
	target.targets = env.Library( os.path.join(BUILD_libDir,libName), objs )
	ALIAS_add_default( name, target.targets )

#
# handle dependencies
#
def BUILD_toList( x ):
	if x : return Flatten(x)
	else : return []

def BUILD_addLib( env, name, lib, addSuffix ):
	if 'gcc' != env['CC'] and lib in env['LIBS']:
		return # ignore redundant libraries, if not GCC
	if addSuffix:
		env.Prepend( LIBS = [lib+BUILD_getSuffix()] )
	else:
		env.Prepend( LIBS = [lib] )
	GN.trace( 2, 'Add depends of %s : %s'%(name,lib) )

class ReverseIteratorProxy:
	def __init__(self, sequence):
		self.sequence = sequence
	def __iter__(self):
		length = len(self.sequence)
		i = length
		while i > 0:
			i = i - 1
			yield self.sequence[i]

def BUILD_addExternalDependencies( env, name, deps ):
	for x in ReverseIteratorProxy(deps):
		BUILD_addLib( env, name, x, False )

def BUILD_addDependencies( env, name, deps ):
	targets = ALL_targets[BUILD_compiler][BUILD_variant]
	for x in ReverseIteratorProxy(deps):
		if x in targets:
			BUILD_addExternalDependencies( env, name, BUILD_toList(targets[x].externalDependencies) )
			BUILD_addDependencies( env, name, BUILD_toList(targets[x].dependencies) )
			if 'GNcore' == x or 'stlib' == targets[x].type : # here we ignore shared libraried other then GNcore.
				BUILD_addLib( env, name, x, True )
		else:
			GN.warn( "Ingore non-exist dependency for target %s: %s"%(name,x) )

#
# does compiler produce manifest file?
#
def BUILD_handleManifest( env, target ):
	if ( float(env.get('MSVS_VERSION',0)) >= 8.0
	   and ('cl' == env['CC'] or 'icl' == env['CC'] )
	   and not UTIL_staticBuild( BUILD_variant ) ) :
		manifest = File( '%s.manifest'%target[0] )
		env.SideEffect( manifest, target )
		target += [manifest]

#
# build dynamic linked library
#
def BUILD_dynamicLib( name, target ):
	objs = []
	for s in target.sources: objs += BUILD_sharedObjs( s )

	env = BUILD_newLinkEnv( target )

	stdlibs = []
	if not target.ignoreDefaultDependencies:
		if 'GNcore' != name : stdlibs += ['GNcore']
		stdlibs += TARGET_stlibs

	BUILD_addExternalDependencies( env, name, BUILD_toList(target.externalDependencies) )
	BUILD_addDependencies( env, name, BUILD_toList(target.dependencies) + stdlibs )
	GN.trace( 2, "Depends of %s : %s"%(name,env['LIBS']) )

	libName = '%s%s%s%s'%(env['SHLIBPREFIX'],name,BUILD_getSuffix(),env['SHLIBSUFFIX'])
	lib = env.SharedLibrary( os.path.join(str(target.path),libName), objs )
	BUILD_handleManifest( env, lib )

	def extname( path ):
		p,e = os.path.splitext( str(path) )
		return e

	target.targets = []
	for x in lib:
		e = extname( x )
		if '.lib' == e or '.exp' == e  or '.a' == e :
			d = BUILD_libDir
		else:
			d = BUILD_binDir
		target.targets.append( env.Install( d, x )[0] )

	ALIAS_add_default( name, target.targets )

#
# build executable
#
def BUILD_program( name, target ):
	objs = []
	for s in target.sources: objs += BUILD_staticObjs( s )

	env = BUILD_newLinkEnv( target )

	stdlibs = []
	if not target.ignoreDefaultDependencies:
		stdlibs = TARGET_shlibs + TARGET_stlibs + ['GNcore'] # Need 2 GNcore instances to workaround gcc link ordering issue.

	BUILD_addDependencies( env, name, BUILD_toList(target.dependencies) + stdlibs )
	BUILD_addExternalDependencies( env, name, BUILD_toList(target.externalDependencies) )
	GN.trace( 2, "Depends of %s : %s"%(name,env['LIBS']) )

	exeName = '%s%s%s%s'%(env['PROGPREFIX'],name,BUILD_getSuffix(),env['PROGSUFFIX'])
	prog = env.Program( os.path.join(str(target.path),exeName), objs )
	BUILD_handleManifest( env, prog )

	target.targets = env.Install( BUILD_binDir, prog )

	if 'xenon' == BUILD_compiler.name and CONF_xedeploy:
		env.AddPostAction( target.targets[0], UTIL_copy_to_devkit('xe:\\garnet3d\\%s'%BUILD_variant) )

	ALIAS_add_default( name, target.targets )

#
# build custom target
#
def BUILD_custom( name, target ):
	assert( 'custom' == target.type )
	env = BUILD_env.Clone()
	target.targets = []
	for s in target.sources:
		target.targets += s.action( env, s.sources, BUILD_bldDir )
	ALIAS_add_default( name, target.targets )

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
			elif 'dylib' == x.type :
				BUILD_dynamicLib( name, x )
			elif 'prog' == x.type :
				BUILD_program( name, x )
			elif 'custom' == x.type :
				BUILD_custom( name, x )
			else: GN.error( 'Unknown target type for target %s: %s'%(name,x.type) )
			GN.trace( 1, "%s : compiler(%s), variant(%s), type(%s), path(%s), targets(%s)"%(
				name, compiler, variant, x.type, x.path, [str(t) for t in x.targets] ) )

		# build additional dependencies:
		def getTargets( n ):
			if n in targets : return targets[n].targets
			else : return []

		progs = TARGET_tests + TARGET_samples + TARGET_tools

		# Everything should depend on the header target
		for n in ( TARGET_stlibs + TARGET_shlibs + progs ):
			if 'GNinc' != n:
				for t in getTargets(n):
					Depends( t, 'GNinc' )

		# Make binaries depend on their by-products, such as manifest and PDB, to make sure
		# those files are copied to binary directory, before execution of the binaries.
		for n in ( TARGET_shlibs + progs ):
			t = getTargets(n)
			for x in t[1:] :
				Depends( t[0], x )

		# Make executables depend on shared libraries and media files.
		for pn in progs:
			for pt in getTargets(pn):
				Depends( pt, 'GNmedia' )
				for sn in TARGET_shlibs:
					for st in getTargets(sn):
						Depends( pt, st )

		################################################################################
		#
		# INSTALL
		#
		################################################################################

		#define installation root directory
		INSTALL_root = os.path.join( CONF_sdkroot, UTIL_bldsubdir( compiler, variant ) )

		#define installation alias
		ALIAS_add_default( "install", INSTALL_root )

		def installTargets( dir, files ):
			dstdir = os.path.join( INSTALL_root, dir )
			for f in files:
				Install( dstdir, getTargets(f) )

		def installMedia( dstroot, srcroot ):
			srcroot = Dir(srcroot).path
			files = getTargets('GNmedia')
			for src in files:
				relpath = GN.relpath( src[0].path, srcroot )
				dst     = os.path.join( INSTALL_root, dstroot, relpath )
				InstallAs( dst, src )

		def installHeaders( dstroot, srcroot ):
			if compiler and variant:
				headers = GN.glob( srcroot, True )
				for src in headers:
					relpath = GN.relpath( src, srcroot )
					dst     = os.path.join( INSTALL_root, dstroot, relpath )
					InstallAs( dst, src )

		installTargets( 'bin',        TARGET_shlibs + TARGET_tools )
		installTargets( 'lib',        TARGET_stlibs + TARGET_shlibs )
		installTargets( 'doc',        ['GNdoc'] )
		installMedia  ( 'media',      os.path.join( UTIL_buildDir( compiler, variant ), 'media' ) )

		installTargets( 'inc/garnet/base', ['GNinc'] )
		installHeaders( 'inc/garnet',      '#src/priv/inc/garnet' )
		installHeaders( 'inc/extern',      '#src/extern/inc' )

################################################################################
#
# 生成msvc工程文件
#
################################################################################

for compiler, variants in ALL_targets.iteritems():
	for variant, targets in variants.iteritems():
		MSVS_env = UTIL_newEnv( compiler, variant )
		if 'MSVSProject' in MSVS_env['BUILDERS'] :
			for name, x in targets.iteritems():
				if 'stlib' == x.type or 'dylib' == x.type or 'prog' == x.type :
					SConscript(
					'msvc/SConscript',
						exports={
							'GN' : GN,
							'env' : MSVS_env,
							'compiler' : compiler,
							'variant' : variant,
							'name' : name,
							'target' : x,
							},
						build_dir=os.path.join( UTIL_buildDir( compiler, variant ), "msvc" )
						)
ALIAS_add_non_default( 'msvc', '#msvc' )

################################################################################
#
# 定义 help screen
#
################################################################################

def HELP_generateAliasList():
	s = ''
	for a in ALL_aliases.iteritems():
		if a[1]: d = '(default)'
		else   : d = '(non defualt)'
 		s += '\n    ' + a[0] + ('%' + str(24-len(a[0])) +'s')%' ' + d
	return s

HELP_opts = Options()
HELP_opts.Add(
	'trace',
	'Set trace level (GN_BUILD_TRACE).\n'
	'           Specify level  N to enable levels [1..N],\n'
	'           Specify level -N to enable level N only.'
	,
	CONF_defaultCmdArgs['trace'] )
HELP_opts.Add(
	'compiler',
	'Specify compiler. Could be : one of (%s) or "all". (GN_BUILD_COMPILER)'%'...',
	CONF_defaultCmdArgs['compiler'] )
HELP_opts.Add(
	'os',
	'Specify build target OS. Could be : one of (%s) or "all". (GN_BUILD_TARGET_OS)'%'...',
	CONF_defaultCmdArgs['os'] )
HELP_opts.Add(
	'cpu',
	'Specify build target CPU type. Could be : one of (%s) or "all". (GN_BUILD_TARGET_CPU)'%'...',
	CONF_defaultCmdArgs['cpu'] )
HELP_opts.Add(
	'variant',
	'Specify variant. Could be : one of (%s) or "all". (GN_BUILD_VARIANT)'%CONF_allVariants,
	CONF_defaultCmdArgs['variant'] )
HELP_opts.Add(
	'cg',
	'Support Cg language or not. This flag has no effect, if Cg library is not found. (GN_BUILD_ENABLE_CG)',
	CONF_defaultCmdArgs['cg'] )
HELP_opts.Add(
	'xedeploy',
	'Copy to Xenon devkit. Only effective when building Xenon binaries. (GN_BUILD_XEDEPLOY)',
	CONF_defaultCmdArgs['xedeploy'] )
HELP_opts.Add(
	'sdkroot',
	'Specify SDK installation directory. (GN_BUILD_SDK_ROOT)',
	CONF_defaultCmdArgs['sdkroot'] )

HELP_text = """
Usage:
	scons [options] [target1 target2 ...]

Options:%s

Targets:%s
""" % (
	HELP_opts.GenerateHelpText( Environment( options = HELP_opts, tools=[] ) ),
	#HELP_generateTargetList()
	HELP_generateAliasList()
	)
Help( HELP_text )
