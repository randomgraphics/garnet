#include "pch.h"
#include "garnet/GNcore.h"
#include "garnet/GNinput.h"
#include "coreResourceDict.h"

// implement global singletons
GN_IMPLEMENT_SINGLETON( GN::PluginManager )
GN_IMPLEMENT_SINGLETON( GN::ProfilerManager )
GN_IMPLEMENT_SINGLETON( GN::input::Input )
GN_IMPLEMENT_SINGLETON( GN::gfx::Renderer )
GN_IMPLEMENT_SINGLETON( GN::gfx::TextureDictionary )
GN_IMPLEMENT_SINGLETON( GN::gfx::effect::EffectDictionary )

// implement static renderer data members
GN_IMPLEMENT_RENDERER_STATIC_MEMBERS()

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

// *****************************************************************************
// Garnet core
// *****************************************************************************

namespace GN { namespace core
{
    class GarnetCore
    {
        PluginManager mPluginManager;
        ProfilerManager mProfilerManager;
        CoreTextureDict mTextureDict;
        CoreEffectDict mEffectDict;

    public:

        GarnetCore() {}

        ~GarnetCore() { shutdown(); }

        bool initialize()
        {
            GN_GUARD_ALWAYS;

            GN_INFO( "Initialize garnet core ..." );

            if( !mTextureDict.init() ) return false;
            if( !mEffectDict.init() ) return false;

            // success
            return true;

            // failed
            GN_UNGUARD_ALWAYS_NO_THROW;
            return false;
        }

        void shutdown()
        {
            GN_GUARD_ALWAYS;

            GN_INFO( "Shutdown garnet core ..." );

            mEffectDict.quit();
            mTextureDict.quit();

            GN_UNGUARD_ALWAYS_NO_THROW;
        }
    };
}}

// *****************************************************************************
// Garnet core initializer
// *****************************************************************************

struct CoreInitializer
{
    GN::AutoObjPtr<GN::core::GarnetCore> mCore;

    CoreInitializer()
    {
        GN_GUARD_ALWAYS;
        mCore.attach( new GN::core::GarnetCore );
        if( !mCore->initialize() ) GN_THROW( "Garnet core initialization failed!" );
        GN_UNGUARD_ALWAYS_DO( mCore.clear(); );
    };
};
static CoreInitializer sCoreInitializor;
