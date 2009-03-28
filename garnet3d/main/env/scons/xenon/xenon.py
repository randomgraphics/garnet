"""Scons Xenon Extension

Tool-specific initialization for Microsoft Xbox 360

There normally shouldn't be any need to import this module directly.
It will usually be imported through the generic SCons.Tool.Tool()
selection method.

"""

import os
import xenoncl
import xenonlib
import xenonlink
import SCons.Tool.msvs

def generate(env):
    xenoncl.generate(env)
    xenonlink.generate(env)
    xenonlib.generate(env)
    SCons.Tool.msvs.generate(env)

def exists(env):
    return 'XEDK' in os.environ;
