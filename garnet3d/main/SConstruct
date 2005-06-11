# -*- coding: utf-8 -*-

################################################################################
#
# 定义编译选项
#
################################################################################

conf = {}

# 定义缺省的选项
default_configs = {
    'build'             : 'debug',
    'use_cg'            : 1,    # use Cg by default.
    'enable_profile'    : 0,    # disabled by default
    }

# 定义编译类型
# can be 'debug', 'release', 'stdbg', 'strel', 'static', 'all'
conf['build'] = ARGUMENTS.get('build', default_configs['build'] )
if not conf['build'] in Split('debug release stdbg strel all'):
    print 'Invalid build type! Must be one of (debug releae stdbg strel all)';
    Exit(-1)

# 是否支持Cg语言.
conf['use_cg']  = ARGUMENTS.get('cg', default_configs['use_cg'] )

# 是否启用profiler.
conf['enable_profile'] = ARGUMENTS.get('prof', default_configs['enable_profile'] )

# 定义target dict
targets = [{}, {}, {}, {}]
alias   = []

################################################################################
#
# 创建缺省编译环境
#
################################################################################

opts = Options()
opts.Add(
    'build',
    'Specify build variant. Could be : debug, release, stdbg, strel or all',
    default_configs['build'] )
opts.Add(
    'cg',
    'Support Cg language or not.',
    default_configs['use_cg'] )
opts.Add(
    'prof',
    'Enable performance profiler.',
    default_configs['enable_profile'] )

env = Environment( options = opts )

################################################################################
#
# call SConscript
#
################################################################################

if 'all' in COMMAND_LINE_TARGETS: conf['build'] = 'all'

import copy

if 'all' == conf['build'] or 'debug' == conf['build'] or 'debug' in COMMAND_LINE_TARGETS:
    c = copy.copy(conf);
    c['build'] = 'debug'
    env.SConscript(
        'SConscript',
        exports={
            'GN_conf'    : c,
            'GN_targets' : targets[0],
            'GN_alias'   : alias,
            },
        )

if 'all' == conf['build'] or 'release' == conf['build'] or 'release' in COMMAND_LINE_TARGETS:
    c = copy.copy(conf);
    c['build'] = 'release'
    env.SConscript(
        'SConscript',
        exports={
            'GN_conf'    : c,
            'GN_targets' : targets[1],
            'GN_alias'   : alias,
            },
        )

if 'all' == conf['build'] or 'stdbg' == conf['build'] or 'stdbg' in COMMAND_LINE_TARGETS:
    c = copy.copy(conf);
    c['build'] = 'stdbg'
    env.SConscript(
        'SConscript',
        exports={
            'GN_conf'    : c,
            'GN_targets' : targets[2],
            'GN_alias'   : alias,
            },
        )

if 'all' == conf['build'] or 'strel' == conf['build'] or 'strel' in COMMAND_LINE_TARGETS:
    c = copy.copy(conf);
    c['build'] = 'strel'
    env.SConscript(
        'SConscript',
        exports={
            'GN_conf'    : c,
            'GN_targets' : targets[3],
            'GN_alias'   : alias,
            },
        )

################################################################################
#
# 定义 phony targets
#
################################################################################

for x in targets[0].items(): env.Alias( 'debug', x[1] )
for x in targets[1].items(): env.Alias( 'release', x[1] )
for x in targets[2].items(): env.Alias( 'stdbg', x[1] )
for x in targets[3].items(): env.Alias( 'strel', x[1] )

all_targets = targets[0].items() + targets[1].items() + targets[2].items() + targets[3].items()
all_targets.sort()
for x in all_targets:
    env.Alias( x[0], x[1] )
    env.Alias( 'all', x[1] )
    env.Default( x[1] )

################################################################################
#
# 定义 help screen
#
################################################################################

targets_text = ''
for x in Split('all debug release stdbg strel'):
    targets_text += '%25s : %s\n'%( x, 'Build %s build(s)'%x )
for x in all_targets:
    targets_text += '%25s : %s\n'%( x[0], env.File(x[1]) )

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
