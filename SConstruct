# -*- coding: utf-8 -*-

import copy, os, os.path

################################################################################
#
# 定义编译选项
#
################################################################################

# 定义文件签名
#SourceSignatures( "timestamp" )
#TargetSignatures( "content" )

# 记录当前的OS
conf = {
    'platform' : Environment()['PLATFORM'],
}

# 读取环境变量
def getenv( name, defval = None ):
    if name in os.environ: return os.environ[name]
    else: return defval


# 定义缺省的选项
if 'win32' == conf['platform']:
    if 'AMD64' == getenv('PROCESSOR_ARCHITECTURE') or 'AMD64' == getenv('PROCESSOR_ARCHITEW6432'):
        default_compiler = 'vc80-x64'
    else:
        default_compiler = 'vc80'
    all_compilers = 'vc71 vc80 vc80-x64 icl icl-em64t xenon'
else:
    default_compiler = 'gcc'
    all_compilers = 'gcc icl'
all_variants='debug release stdbg strel'
default_configs = {
    'genconf'           : getenv('GN_BUILD_GENCONF', 0), # force (re)generation of build configuration
    'enable_cache'      : getenv('GN_BUILD_ENABLE_CACHE', 1), # enable build cache
    'variant'           : getenv('GN_BUILD_VARIANT', 'debug'),
    'compiler'          : getenv('GN_BUILD_COMPILER', default_compiler), # default compiler
    'enable_cg'         : getenv('GN_BUILD_ENABLE_CG', 1), # use Cg by default.
    'enable_profile'    : getenv('GN_BUILD_ENABLE_PROFILE', 0), # disabled by default
    'run_unit_tests'    : getenv('GN_BUILD_RUN_UNIT_TESTS', 0), # Do not run unit tests after build by default.
    }


# 是否强制生成配置信息
conf['genconf']  = ARGUMENTS.get('conf', default_configs['genconf'] )

# 是否使用build cache
conf['enable_cache']  = ARGUMENTS.get('cache', default_configs['enable_cache'] )

# 定义编译器类型
conf['compiler'] = ARGUMENTS.get('compiler', default_configs['compiler'])

# 定义编译类型
conf['variant'] = ARGUMENTS.get('variant', default_configs['variant'] )

# 是否支持Cg语言.
conf['enable_cg']  = ARGUMENTS.get('cg', default_configs['enable_cg'] )

# 是否启用profiler.
conf['enable_profile'] = ARGUMENTS.get('prof', default_configs['enable_profile'] )

# 是否自动运行UnitTest.
conf['run_unit_tests'] = ARGUMENTS.get('ut', default_configs['run_unit_tests'] )

# 定义target dict
targets = {}
for v in Split(all_variants):
    targets[v] = {}
    for c in Split(all_compilers):
        targets[v][c] = {}
alias   = []

################################################################################
#
# 创建缺省编译环境
#
################################################################################

opts = Options()
opts.Add(
    'conf',
    'Force (re)generation of build configuration. (GN_BUILD_GENCONF)',
    default_configs['genconf'] )
opts.Add(
    'cache',
    'Use scons build cache. (GN_BUILD_ENABLE_CACHE)',
    default_configs['enable_cache'] )
opts.Add(
    'compiler',
    'Specify compiler. Could be : one of (%s) or "all". (GN_BUILD_COMPILER)'%all_compilers,
    default_configs['compiler'] )
opts.Add(
    'variant',
    'Specify variant. Could be : one of (%s) or "all". (GN_BUILD_VARIANT)'%all_variants,
    default_configs['variant'] )
opts.Add(
    'cg',
    'Support Cg language or not. (GN_BUILD_ENABLE_CG)',
    default_configs['enable_cg'] )
opts.Add(
    'prof',
    'Enable performance profiler. (GN_BUILD_ENABLE_PROFILE)',
    default_configs['enable_profile'] )
opts.Add(
    'ut',
    'Run unit tests after build. (GN_BUILD_RUN_UNIT_TESTS)',
    default_configs['run_unit_tests'] )

env = Environment( options = opts )

################################################################################
#
# Build variants
#
################################################################################

if 'all' in COMMAND_LINE_TARGETS:
    variants = Split(all_variants)
    compilers = Split(all_compilers)
else:
    if 'all' == conf['variant']:
        variants = Split(all_variants)
    else:
        variants = Split(conf['variant'])
    if 'all' == conf['compiler']:
        compilers = Split(all_compilers)
    else:
        compilers = Split(conf['compiler'])

for c in compilers:
    if not c in all_compilers:
        print "ERROR: Ignore invalid compiler '%s'"%c
        continue
    for v in variants:
        if not v in all_variants:
            print "ERROR: Ignore invalid variant '%s'"%v
            continue
        if 'xenon' == c and ('debug'==v or 'release'==v) : continue; # ignore non-static build for xenon
        cc = copy.copy(conf)
        cc['variant'] = v
        cc['compiler'] = c
        SConscript(
            'SConscript',
            exports={
                'GN_conf'    : cc,
                'GN_targets' : targets[v][c],
                'GN_alias'   : alias,
                },
            )

# Collect all targets
all_targets = []
for x in targets:
    for y in targets[x]:
        all_targets += targets[x][y].items()
all_targets.sort()
if 0 == len(all_targets):
    raise "No targets found!"

################################################################################
#
# Build and install manual
#
################################################################################

manual = SConscript( 'src/priv/manual/SConscript' )
if manual: all_targets.append( [ 'GNman', env.Install( os.path.join( 'bin', 'sdk', 'manual' ), manual ) ] )

################################################################################
#
# Build and install media files
#
################################################################################

mediaSourceDir = os.path.join( '#src', 'media' )
mediaTargetDir = os.path.join( 'bin', 'media' )
for x in SConscript( 'src/media/SConscript' ):
    src = os.path.join( mediaSourceDir, x )
    dst = os.path.join( mediaTargetDir, x )
    env.Command( dst, src, Copy('$TARGET', '$SOURCE') )
    env.Alias( "media", dst )

################################################################################
#
# Install public headers
#
################################################################################

# install SDK headers
sdkTargetDir = os.path.join( 'bin','sdk', 'inc' )
sdkSourceDir = os.path.join( '#src','priv','inc' )
for src in env.GN_glob(sdkSourceDir, True):
    dst = os.path.join( sdkTargetDir, env.GN_relpath(src,sdkSourceDir) )
    env.Command( dst, src, Copy('$TARGET', '$SOURCE') )
    env.Alias( 'sdk', dst )

################################################################################
#
# 定义 phony targets
#
################################################################################

env.Alias( 'msvc', '#msvc' )

for x in all_targets:
    env.Alias( x[0], x[1] )
    env.Alias( 'all', x[1] )
    env.Default( x[1] )

env.Default( Split('samples sdk media') )

################################################################################
#
# 定义 help screen
#
################################################################################

targets_text = ''
targets_text += '%16s : %s\n'%( 'all', 'Build and install all targets of all variants for all compilers' )
targets_text += '%16s : %s\n'%( 'samples', 'Build and install samples' )
targets_text += '%16s : %s\n'%( 'sdk', 'Build and install SDK' )
targets_text += '%16s : %s\n'%( 'media', 'Build and install media files' )
targets_text += '%16s : %s\n'%( 'msvc', 'Build MSVC projects' )
for x in all_targets:
    targets_text += '%16s : %s\n'%( x[0], env.File(x[1]) )

help_text = """
Usage:
    scons [options] [target1 target2 ...]

Options:%s

Targets:
%s
""" % (
    opts.GenerateHelpText(env),
    targets_text,
    )
Help( help_text )
