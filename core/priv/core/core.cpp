#include "pch.h"
#include "garnet/GnBase.h"

// implement log function
GN_IMPLEMENT_DEFAULT_LOG();

// implement static member of profile manager
GN_IMPLEMENT_SINGLETON( ::GN::ProfilerManager );

// instance of profile manager
static ::GN::ProfilerManager sProfilerMgr;

// Implement global variables
namespace GN
{
    namespace core
    {
        //!
        //! Global plugin manager
        //!
        GN_PUBLIC PluginManager   gPluginMgr;
    }
}
