#include "pch.h"
#include "garnet/GNcore.h"

// instance of plugin manager
static GN::PluginManager sPluginManager;
GN_IMPLEMENT_SINGLETON( GN::PluginManager )

// instance of profile manager
static GN::ProfilerManager sProfilerMgr;
GN_IMPLEMENT_SINGLETON( GN::ProfilerManager )

// Implement global log function.
GN_PUBLIC GN::Signal2<void,const GN::LogDesc &, const char *> GN::core::gSigLog;
GN_PUBLIC void
GN::doLog( const LogDesc & desc, const char * msg )
{
    // trigger log signal
    core::gSigLog( desc, msg );

    // do default log
    detail::defaultLogImpl(desc,msg);
}
