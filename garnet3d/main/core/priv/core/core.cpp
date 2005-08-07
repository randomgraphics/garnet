#include "pch.h"
#include "garnet/GnBase.h"

// implement log function
GN_IMPLEMENT_DEFAULT_LOG();

// implement static member of profile manager
GN_IMPLEMENT_SINGLETON( ::GN::ProfilerManager );
    
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
