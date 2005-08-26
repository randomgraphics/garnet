#include "pch.h"
#include "garnet/GNbase.h"

// implement log function
GN_IMPLEMENT_DEFAULT_LOG();

// instance of plugin manager
static GN::PluginManager sPluginManager;
GN_IMPLEMENT_SINGLETON( GN::PluginManager )

// instance of profile manager
static GN::ProfilerManager sProfilerMgr;
GN_IMPLEMENT_SINGLETON( GN::ProfilerManager )
