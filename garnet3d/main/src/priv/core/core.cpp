#include "pch.h"
#include "garnet/GNcore.h"
#include "garnet/GNgfx.h"
#include "garnet/GNinput.h"

// implement static members for string class
template<> GN_PUBLIC GN::FixedSizedObjectAllocator< GN::Str<char> > GN::Str<char>::msAllocator;
template<> GN_PUBLIC GN::FixedSizedObjectAllocator< GN::Str<wchar_t> > GN::Str<wchar_t>::msAllocator;

// implement global singletons
GN_IMPLEMENT_SINGLETON( GN::PluginManager )
GN_IMPLEMENT_SINGLETON( GN::ProfilerManager )
GN_IMPLEMENT_SINGLETON( GN::input::Input )
GN_IMPLEMENT_SINGLETON( GN::gfx::Renderer )

// implement static renderer data members
GN_IMPLEMENT_RENDERER_STATIC_MEMBERS()

namespace GN
{
    // Global runtime assert behavior flag.
    GN_PUBLIC RuntimeAssertBehavior gRuntimeAssertBehavior = RAB_ASK_USER;

    //
    // Implement global log function.
    // -------------------------------------------------------------------------
    GN_PUBLIC Signal2<void,const GN::LogDesc &, const char *> core::gSigLog;
    GN_PUBLIC void
    doLog( const LogDesc & desc, const char * msg )
    {
        // trigger log signal
        core::gSigLog( desc, msg );

        // do default log
        detail::defaultLogImpl(desc,msg);
    }
}
