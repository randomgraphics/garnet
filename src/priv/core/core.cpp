#include "pch.h"
#include "garnet/GNcore.h"
#include "garnet/GNinput.h"
#include "coreResourceDict.h"

// implement static members for string class
template<> GN_PUBLIC GN::FixedSizedObjectAllocator< GN::Str<char> > GN::Str<char>::msAllocator;
template<> GN_PUBLIC GN::FixedSizedObjectAllocator< GN::Str<wchar_t> > GN::Str<wchar_t>::msAllocator;

// implement global singletons
GN_IMPLEMENT_SINGLETON( GN::PluginManager )
GN_IMPLEMENT_SINGLETON( GN::ProfilerManager )
GN_IMPLEMENT_SINGLETON( GN::input::Input )
GN_IMPLEMENT_SINGLETON( GN::gfx::Renderer )
GN_IMPLEMENT_SINGLETON( GN::gfx::ShaderDictionary )
GN_IMPLEMENT_SINGLETON( GN::gfx::TextureDictionary )
GN_IMPLEMENT_SINGLETON( GN::gfx::effect::EffectDictionary )
GN_IMPLEMENT_SINGLETON( GN::core::RawResourceDictionary )

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

// *****************************************************************************
// Garnet core
// *****************************************************************************

#if GN_MSVC
#include <crtdbg.h>
#endif
static void sEnableCRTMemoryCheck()
{
#if GN_MSVC
    int tmpDbgFlag;
    tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
#endif
}

namespace GN { namespace core
{
    
    
    class GarnetCore
    {
        PluginManager mPluginManager;
        ProfilerManager mProfilerManager;
        CoreRawResourceDict mRawResDict;
        CoreShaderDict mShaderDict;
        CoreTextureDict mTextureDict;
        CoreEffectDict mEffectDict;

    public:

        GarnetCore() {}

        ~GarnetCore() { shutdown(); }

        bool initialize()
        {
            GN_GUARD_ALWAYS;

            // enable MSVC CRT memory leak check
            sEnableCRTMemoryCheck();

            GN_INFO( "Garnet core startup ..." );

            if( !mRawResDict.init() ) return false;
            if( !mShaderDict.init() ) return false;
            if( !mTextureDict.init() ) return false;
            if( !mEffectDict.init() ) return false;

            // success
            GN_INFO( "Garnet core startup ... Done." );
            return true;

            // failed
            GN_UNGUARD_ALWAYS_NO_THROW;
            return false;
        }

        void shutdown()
        {
            GN_GUARD_ALWAYS;

            GN_INFO( "Garnet core shutdown ..." );

            mEffectDict.quit();
            mTextureDict.quit();
            mShaderDict.quit();
            mRawResDict.quit();

            GN_INFO( "Garnet core shutdown ... Done." );

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
