"""SCons.Tool.xenonlib

Tool-specific initialization for lib (MicroSoft Xbox 360 SDK).

There normally shouldn't be any need to import this module directly.
It will usually be imported through the generic SCons.Tool.Tool()
selection method.

"""

import SCons.Defaults
import SCons.Tool
import SCons.Tool.xenoncl
import SCons.Util

def generate(env):
    """Add Builders and construction variables for lib to an Environment."""
    SCons.Tool.createStaticLibBuilder(env)

    include_path, lib_path, exe_path = SCons.Tool.xenoncl.get_xenon_paths(env)

    # since other tools can set this, we just make sure that the
    # relevant stuff from MSVS is in there somewhere.
    env.PrependENVPath('PATH', exe_path)

    env['AR']          = 'lib'
    env['ARFLAGS']     = SCons.Util.CLVar('/nologo')
    env['ARCOM']       = "${TEMPFILE('$AR $ARFLAGS /OUT:$TARGET $SOURCES')}"
    env['LIBPREFIX']   = ''
    env['LIBSUFFIX']   = '.lib'
