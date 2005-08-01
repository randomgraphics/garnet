# -*- coding: utf-8 -*-

Import( 'GN_conf GN_targets GN_alias' )

import os, os.path, re, fnmatch

################################################################################
#
# 定义全局变量
#
################################################################################

# 定义编译类型
GN_conf['static'] = ( 'stdbg' == GN_conf['build'] or 'strel' == GN_conf['build'] )
GN_conf['debug'] = ('debug' == GN_conf['build'] or 'stdbg' == GN_conf['build'])

################################################################################
#
# 定义局部变量
#
################################################################################

root_dir = Dir( os.path.join('tmp', 'scons' ) ).abspath
build_dir = os.path.join( root_dir, GN_conf['platform'], GN_conf['compiler'], GN_conf['build'] )
conf_dir  = os.path.join( build_dir, 'conf' )
sig_file = os.path.join( root_dir, GN_conf['platform'], GN_conf['compiler'], '.sconsign.dbm' )
if GN_conf['enable_cache']: cache_dir = os.path.join( root_dir, 'cache' )

# 创建必要的目录
if not os.path.exists( root_dir ) : os.makedirs( root_dir )
if not os.path.exists( build_dir ) : os.makedirs( build_dir )
if not os.path.exists( conf_dir ) : os.makedirs( conf_dir )
if GN_conf['enable_cache'] and not os.path.exists( cache_dir ) : os.makedirs( cache_dir )

################################################################################
#
# 定义全局函数
#
################################################################################

# 输出提示信息
def GN_info( msg ):
    print '===================================================================='
    print 'INFO : %s'%msg
    print '===================================================================='

# 输出警告信息
def GN_warn( msg ):
    print '===================================================================='
    print 'WARNING : %s'%msg
    print '===================================================================='

# 输出错误信息
def GN_error( msg ):
    print '===================================================================='
    print 'ERROR : %s'%msg
    print '===================================================================='

# 生成从target到base的相对路径
def GN_relpath(target, base):
    """
    Return a relative path to the target from either the current dir or an optional base dir.
    Base can be a directory specified either as absolute or relative to current dir.
    """

    if not os.path.exists(target):
        raise OSError, 'Target does not exist: '+target

    if not os.path.isdir(base):
        raise OSError, 'Base is not a directory or does not exist: '+base

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
def GN_glob( patterns, dirs = ['.'],
             recursive = False ):
    patterns = Flatten( [patterns] )
    dirs = Flatten( [dirs] )
    files = []
    for dir in dirs:
        root = Dir(dir).srcnode().abspath;
        for file in os.listdir( root ):
            if recursive \
              and os.path.isdir( os.path.join(root,file) ) \
              and ( not '.svn' == file ) : # ignore subversion directory
                files = files + GN_glob( patterns, os.path.join(dir,file), recursive )
            else:
                # Note: ignore precompiled header
                if not ('pch.cpp' == file or 'stdafx.cpp' == file):
                    for pattern in patterns:
                        if fnmatch.fnmatch(file, pattern):
                            files.append( os.path.join( dir, file ) )
    return files

# setup environment for producing PCH and PDB
def GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb ):
    if pdb:
        #pdb = File(pdb).path
        #e.Append(
        #    CCFLAGS = ['/Zi', '/Fd%s'%pdb],
        #    LINKFLAGS = ['/DEBUG', '/PDB:%s'%pdb] )
        e['PDB'] = pdb
    if 'PCH' in e['BUILDERS'] and pchcpp:
        e['PCH'] = e.PCH(pchcpp)[0]
        e['PCHSTOP'] = pchstop

# 编译 static object
def GN_build_static_object(local_env,source=[],pchstop=0,pchcpp=0,pdb=0):
    e = local_env.Copy()
    GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb )
    return e.Object(source)

# 编译 static object list
def GN_build_static_objects(local_env,sources=[],pchstop=0,pchcpp=0,pdb=0):
    e = local_env.Copy()
    GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb )
    return [e.Object(x) for x in sources]

# 编译 shared object
def GN_build_shared_object(local_env,source=[],pchstop=0,pchcpp=0,pdb=0):
    e = local_env.Copy()
    GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb )
    if GN_conf['static']:
        return e.Object(source)
    else:
        return e.SharedObject(source)

# 编译 shared object list
def GN_build_shared_objects(local_env,sources=[],pchstop=0,pchcpp=0,pdb=0):
    e = local_env.Copy()
    GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb )
    if GN_conf['static']:
        return [e.Object(x) for x in sources]
    else:
        return [e.SharedObject(x) for x in sources]

# 编译 static library
def GN_build_static_library(local_env,target=None,sources=[],pchstop=0, pchcpp=0,pdb=0):
    e = local_env.Copy()
    if not pdb and target: pdb = target + '.pdb'
    GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb )
    return e.Library(target,sources)

# 编译 shared library
def GN_build_shared_library(local_env,target=None,sources=[],pchstop=0,pchcpp=0,pdb=0):
    e = local_env.Copy()
    if not pdb and target: pdb = target + '.pdb'
    GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb )
    if GN_conf['static']:
        lib = e.Library(target,sources)
    else:
        extra = [];
        if 'GnCore' != target: extra += [ GN_targets['GnCore'] ]
        extra += [ GN_targets['GnBase'], GN_targets['GnExtern'] ]
        lib = e.SharedLibrary(target,sources+extra)

    """if len(list(lib)) >= 4:
        # 0:dll, 1:pdb, 2:lib, 3:exp
        # note: only msvc may produces more than 2 targets
        assert( 'cl' == local_env['CC'] or 'icl' == local_env['CC'] )
        return [ lib[0], lib[2] ]
    else:
        return lib"""
    return lib

# 编译 dynamic library
def GN_build_dynamic_library(local_env,target=None,sources=[],pchstop=0,pchcpp=0,pdb=0):
    e = local_env.Copy()
    if not pdb and target: pdb = target + '.pdb'
    GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb )
    extra = [ GN_targets['GnBase'], GN_targets['GnExtern'] ]
    return e.SharedLibrary(target,sources+extra)

# 编译可执行文件
def GN_build_program(local_env,target=None,sources=[],pchstop=0,pchcpp=0,pdb=0):
    e = local_env.Copy()
    if not pdb and target: pdb = target + '.pdb'
    GN_setup_PCH_PDB( e, pchstop, pchcpp, pdb )
    extra = [ GN_targets['GnBase'], GN_targets['GnExtern'] ]
    if target:
        prog = e.Program(target,sources+extra)
    else:
        prog = e.Program(sources+extra)
    return prog

################################################################################
#
# 定义局部函数
#
################################################################################

# ================
# 创建缺省编译环境
# ================
def default_env( options = None ):

    # create environment instance
    tools = ['default']
    msvs_version = '7.1'
    if 'icl' == GN_conf['compiler'] :
        tools += ['intelc']
    elif 'vs8' == GN_conf['compiler'] :
        msvs_version = '8.0'
    env = Environment(
        tools = tools,
        MSVS_VERSION = msvs_version,
        options = options,
        )

    # attach gch builder
    if 'g++' == env['CXX']:
        bld = Builder(
            #action = GN_build_pch
            action = '$CXXCOM',
            #action = '$CXX $CXXFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS -x c++ -c -o $TARGET $SOURCES',
            suffix = '.h.gch' )
        env.Append( BUILDERS={'PCH':bld} )

    # 定义sconsign文件
    env.SConsignFile( sig_file )

    # 缺省编译选项
    def generate_empty_options() : return { 'common':[],'debug':[],'release':[],'stdbg':[],'strel':[] }
    cppdefines = generate_empty_options()
    cpppath    = generate_empty_options()
    libpath    = generate_empty_options()
    libs       = generate_empty_options()
    ccflags    = generate_empty_options()
    cxxflags   = generate_empty_options()
    linkflags  = generate_empty_options()

    cpppath['common']  = ['#core/pub/sdk/inc/common']
    cpppath['debug']   = ['#core/pub/sdk/inc/debug']
    cpppath['release'] = ['#core/pub/sdk/inc/release']
    cpppath['stdbg']   = ['#core/pub/sdk/inc/stdbg']
    cpppath['strel']   = ['#core/pub/sdk/inc/strel']

    libpath['common']  = ['#core/pub/sdk/lib/%s/common'%env['PLATFORM']]
    libpath['debug']   = ['#core/pub/sdk/lib/%s/debug'%env['PLATFORM']]
    libpath['release'] = ['#core/pub/sdk/lib/%s/release'%env['PLATFORM']]
    libpath['stdbg']   = ['#core/pub/sdk/lib/%s/stdbg'%env['PLATFORM']]
    libpath['strel']   = ['#core/pub/sdk/lib/%s/strel'%env['PLATFORM']]

    # 定制不同平台的编译选项
    if 'win32' == env['PLATFORM']:
        libs['common'] += Split('kernel32 user32 gdi32')
    else:
        cpppath['common'] += Split('/usr/X11R6/include /usr/local/include')
        libpath['common'] += Split('/usr/X11R6/lib /usr/local/lib')
        libs['common']    += Split('X11 glut GL GLU')

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
            # Note: disable CRT deprecate warnings by now.
            ccflags['common']   += Split('/W4 /WX /D_CRT_SECURE_NO_DEPRECATE')
            cxxflags['common']  += ['/EHa']
            linkflags['common'] += Split( '/NODEFAULTLIB:libcp.lib' )
        else:
            ccflags['common']   += Split('/W4 /WX')
            cxxflags['common']  += ['/EHs']

        ccflags['debug']   += Split('/MDd /GR')
        ccflags['release'] += Split('/O2 /MD')
        ccflags['stdbg']   += Split('/MTd /GR')
        ccflags['strel']   += Split('/O2 /MT')

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

        linkflags['common'] += ['/FORCE']

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

    def apply_options( env, build ):
        env.Append(
            CPPDEFINES = cppdefines[build],
            CPPPATH = cpppath[build],
            LIBPATH = libpath[build],
            CCFLAGS = ccflags[build],
            CXXFLAGS = cxxflags[build],
            LINKFLAGS = linkflags[build] );
        env.Prepend(
            LIBS    = libs[build] ); # for gcc link

    # apply compile options based on current build
    apply_options( env, GN_conf['build'] )
    apply_options( env, 'common' )

    # end of function default_env()
    return env

# ============
# 读取配置信息
# ============
def read_config( conf, conf_file ):
    try:
        fp = file( conf_file, 'rt' )
        lines = fp.readlines()
        parser = re.compile( '^\s*(\w*)\s*=\s*(.*)$' )
        for x in lines:
            m = parser.match(x)
            if not None == m:
                if 'False' == m.group(2) or '0' == m.group(2):
                    conf[m.group(1)] = False
                elif 'True' == m.group(2) or '1' == m.group(2):
                    conf[m.group(1)] = True
                else:
                    conf[m.group(1)] = m.group(2)
        return True;
    except:
        return False;

# ============
# 存储配置信息
# ============
def write_config( conf, conf_file ):
    try:
        fp = file( conf_file, 'wt' )
        items = conf.items();
        items.sort()
        for x in items:
            fp.write( '%s = %s\n'%(x[0],x[1]) )
        fp.close()
    except:
        GN_error( 'fail to generate config file!' )
        Exit(-1);

# ============
# 检查系统配置
# ============
def check_config( conf, conf_dir ):
    env = default_env()
    # Do NOT treat warning as error
    ccflags = str(env.Dictionary('CCFLAGS'))
    ccflags = ccflags.replace( '/WX', '' )
    ccflags = ccflags.replace( '-Werror', '')
    env.Replace( CCFLAGS = Split( ccflags ) )
    c = env.Configure(
        conf_dir = conf_dir,
        log_file = os.path.join(conf_dir,'config.log'),
        );

    # =================
    # 是否支持Cg shader
    # =================
    conf['has_cg'] = c.CheckCHeader('cg/cg.h')

    # ===========
    # 是否支持D3D
    # ===========
    conf['has_d3d'] = c.CheckCXXHeader( 'd3d9.h' ) and c.CheckCXXHeader( 'd3dx9.h' )

    # =================
    # 是否支持freetype2
    # =================
    has_ft2 = 0
    ft2_inc_path = ''
    ft2_libs = ''
    if 'win32' == env['PLATFORM']:
        if c.CheckCHeader( Split('ft2build.h freetype/freetype.h') ):
            if 'debug' == GN_conf['build']:
                if c.CheckLib('ft2MDd','FT_Init_FreeType', autoadd=0): ft2_libs = 'ft2MDd'
            elif 'release' == GN_conf['build']:
                if c.CheckLib('ft2MD','FT_Init_FreeType', autoadd=0): ft2_libs = 'ft2MD'
            elif 'stdbg' == GN_conf['build']:
                if c.CheckLib('ft2MTd','FT_Init_FreeType', autoadd=0): ft2_libs = 'ft2MTd'
            elif 'strel' == GN_conf['build']:
                if c.CheckLib('ft2MT','FT_Init_FreeType', autoadd=0): ft2_libs = 'ft2MT'
            if ft2_libs: has_ft2 = True
    else:
        # config freetype is easy for linux-like system
        e = env.Copy( CPPPATH = [], LIBS = [] )
        e.ParseConfig( 'freetype-config --cflags' )
        e.ParseConfig( 'freetype-config --libs' )
        def list2str( l ):
            s = ''
            for x in l : s += '%s '%str(x)
            return s
        ft2_inc_path = list2str( e['CPPPATH'] )
        ft2_libs = list2str( e['LIBS'] )
        has_ft2 = ( None == os.popen( 'freetype-config --cflags' ).close() )
    conf['has_ft2'] = has_ft2
    conf['ft2_inc_path'] = ft2_inc_path
    conf['ft2_libs'] = ft2_libs

    # =====================
    # 检查 RenderMonkey SDK
    # =====================
    """has_rendermonkey = os.environ.has_key('RENDER_MONKEY_ROOT_PATH')
    if has_rendermonkey:
        rendermonkey_root_path = os.environ['RENDER_MONKEY_ROOT_PATH']
        GN_info( 'Using RenderMonkey installation at "%s".'%rendermonkey_root_path )
    else:
        rendermonkey_root_path = ''
        GN_warn( 'Please specify RenderMonkey location using environment RENDERMONKEY_ROOT_PATH.' )
    conf['has_rendermonkey'] = has_rendermonkey
    conf['rendermonkey_root_path'] = rendermonkey_root_path"""

    # =============
    # finish config
    # =============
    c.Finish();

################################################################################
#
# 配置编译环境
#
################################################################################


conf_file = os.path.join( conf_dir, 'build.conf' )
conf  = {}
if not read_config( conf, conf_file ) or GN_conf['genconf']:
    check_config( conf, conf_dir )
    write_config( conf, conf_file )
GN_conf.update(conf)

################################################################################
#
# 初始化编译环境
#
################################################################################

env = default_env()

env.Prepend(
    CPPPATH = Split('%(root)s/extern/inc %(root)s/priv/inc'%{'root':build_dir})
    )

env.BuildDir( build_dir, 'core' )

if GN_conf['enable_cache']: env.CacheDir( cache_dir )

################################################################################
#
# export全局变量
#
################################################################################

GN_func = {
    'info'                  : GN_info,
    'warn'                  : GN_warn,
    'error'                 : GN_error,
    'glob'                  : GN_glob,
    'relpath'               : GN_relpath,
    #'build_setup_PCH_PDB'   : GN_setup_PCH_PDB,
    'build_static_object'   : GN_build_static_object,
    'build_static_objects'  : GN_build_static_objects,
    'build_shared_object'   : GN_build_shared_object,
    'build_shared_objects'  : GN_build_shared_objects,
    'build_static_library'  : GN_build_static_library,
    'build_shared_library'  : GN_build_shared_library,
    'build_dynamic_library' : GN_build_dynamic_library,
    'build_program'         : GN_build_program,
    }

env.Export(
    'env',
    'GN_conf',
    'GN_alias',
    'GN_targets',
    'GN_func',
    )

################################################################################
#
# build sub-directories
#
################################################################################

SConscript( dirs = [build_dir] )
