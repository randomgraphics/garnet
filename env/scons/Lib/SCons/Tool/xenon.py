"""engine.SCons.Tool.xenon

Tool-specific initialization for Microsoft Xbox 360

There normally shouldn't be any need to import this module directly.
It will usually be imported through the generic SCons.Tool.Tool()
selection method.

"""

import os

import SCons.Tool.xenoncl
import SCons.Tool.xenonlib
import SCons.Tool.xenonlink

def generate(env):
    SCons.Tool.xenoncl.generate(env)
    SCons.Tool.xenonlink.generate(env)
    SCons.Tool.xenonlib.generate(env)
    SCons.Tool.msvs.generate(env)

def exists(env):
    return 'XEDK' in os.environ;
