#include "pch.h"
#include "garnet/GnBase.h"

// implement log function
GN_IMPLEMENT_DEFAULT_LOG();

// Implement global variables
namespace GN
{
    namespace core
    {
        //!
        //! Global plugin manager
        //!
        GN_PUBLIC PluginManager   gPluginMgr;

#if GN_ENABLE_PROFILE
        //!
        //! Global profiler manager
        //!
        GN_PUBLIC ProfilerManager gProfiler;
#endif
    }
}
