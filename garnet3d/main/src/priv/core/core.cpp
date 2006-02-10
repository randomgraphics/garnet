#include "pch.h"
#include "garnet/GNcore.h"
#include "garnet/GNinput.h"
#include "garnet/GNgfx.h"
#include "garnet/gfx/effect.h"

// define static data member of singletons
GN_IMPLEMENT_SINGLETON( GN::PluginManager )
GN_IMPLEMENT_SINGLETON( GN::ProfilerManager )
GN_IMPLEMENT_SINGLETON( GN::input::Input )
GN_IMPLEMENT_SINGLETON( GN::gfx::Renderer )
GN_IMPLEMENT_SINGLETON( GN::gfx::TextureDictionary )
GN_IMPLEMENT_SINGLETON( GN::gfx::effect::EffectDictionary )

// Some of singletons can be safely defined as global variable
static GN::PluginManager sPluginManager;
static GN::ProfilerManager sProfilerMgr;

//
// Implement global log function.
// -----------------------------------------------------------------------------
GN_PUBLIC GN::Signal2<void,const GN::LogDesc &, const char *> GN::core::gSigLog;
GN_PUBLIC void
GN::doLog( const LogDesc & desc, const char * msg )
{
    // trigger log signal
    core::gSigLog( desc, msg );

    // do default log
    detail::defaultLogImpl(desc,msg);
}
