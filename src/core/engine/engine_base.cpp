#include "pch.h"

using namespace GN;
using namespace GN::engine;

GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Entity
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity::Entity() { mID = impl::onEntityCtor(this); }

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity::~Entity() {
    // remove all components.
    mComponents.clear();

    impl::onEntityDtor(mID);
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::Entity::getComponent(const EntityType & type) const {
    auto iter = mComponents.find(type);
    if (iter != mComponents.end()) {
        GN_ASSERT(!iter->second.empty());
        return iter->second.data();
    } else {
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::Entity::setComponent(const EntityType & type, Entity * comp) {
    if (comp) {
        mComponents[type].set(comp);
    } else {
        mComponents.erase(type);
    }
}

// *****************************************************************************
// Engine
// *****************************************************************************

struct Engine {
    // Entity manager
    //@{
    typedef HandleManager<engine::Entity *, int> EntityHandleManager;
    std::mutex                                   entityLock;
    EntityHandleManager                          entities;
    //@}

    // Graphics manager
    //@{
    gfx::Gpu *                 gpu;
    gfx::GpuResourceDatabase * gdb;
    gfx::SpriteRenderer *      spriteRenderer;
    gfx::LineRenderer *        lineRenderer;
    gfx::BitmapFont *          fontRenderer;
    //@}

    /// Default constructor
    Engine();
};

static Engine s_engine;

// *****************************************************************************
// Private Engine Functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool sGfxInitInternal(const GfxInitOptions & o) {
    using namespace GN::gfx;

    // initialize GPU
    s_engine.gpu = createGpu(o.gpuOptions, o.useMultithreadGpu ? GPU_CREATION_MULTIPLE_THREADS : 0);
    if (NULL == s_engine.gpu) return false;

    // create sprite renderer
    s_engine.spriteRenderer = new SpriteRenderer();
    if (!s_engine.spriteRenderer->init(*s_engine.gpu)) return false;

    // create line renderer
    s_engine.lineRenderer = new LineRenderer(*s_engine.gpu);
    if (!s_engine.lineRenderer->init()) return false;

    // try load default font face in mInitParam first
    MixedFontCreationDesc mfcd;
    mfcd.font            = o.defaultAsciiFont;
    mfcd.firstChar       = 0;
    mfcd.numChars        = 127;
    AutoRef<FontFace> ff = attachTo(createMixedFontFace(o.defaultNonAsciiFont, &mfcd, 1));
    if (!ff) {
        // if failed, fallback to simple ASCII font face
        ff.attach(createSimpleAsciiFontFace());
        if (!ff) return false;
    }
    s_engine.fontRenderer = new BitmapFont();
    if (!s_engine.fontRenderer->init(s_engine.spriteRenderer, ff)) return false;

    // create GPU resource database
    s_engine.gdb = new GpuResourceDatabase(*s_engine.gpu);

    // attach to input subsystem
    if (gInputPtr) {
        const DispDesc & dd = s_engine.gpu->getDispDesc();
        if (!gInputPtr->attachToWindow(dd.displayHandle, dd.windowHandle)) return false;
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
static void sGfxClearInternal() {
    s_engine.gpu            = NULL;
    s_engine.spriteRenderer = NULL;
    s_engine.lineRenderer   = NULL;
    s_engine.fontRenderer   = NULL;
    s_engine.gdb            = NULL;
}

//
//
// -----------------------------------------------------------------------------
static void sEngineClearInternal() { sGfxClearInternal(); }

//
//
// -----------------------------------------------------------------------------
Engine::Engine() { sEngineClearInternal(); }

// *****************************************************************************
// Public Engine Functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::engine::initialize() { return true; }

//
//
// -----------------------------------------------------------------------------
GN_API void GN::engine::shutdown() {
    // shutdown subsystems.
    gfxShutdown();
    inputShutdown();

    // clear engine structure
    sEngineClearInternal();
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::engine::inputInitialize(input::InputAPI api) {
    using namespace GN::input;

    if (!initializeInputSystem(api)) {
        inputShutdown();
        return false;
    }

    // attach to graphics subsystem
    if (s_engine.gpu) {
        const gfx::DispDesc & dd = s_engine.gpu->getDispDesc();
        if (!gInputPtr->attachToWindow(dd.displayHandle, dd.windowHandle)) {
            inputShutdown();
            return false;
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::engine::inputShutdown() { GN::input::shutdownInputSystem(); }

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::engine::gfxInitialize(const GfxInitOptions & o) {
    bool result = sGfxInitInternal(o);
    if (!result) gfxShutdown();
    return result;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::engine::gfxShutdown() {
    safeDelete(s_engine.gdb);
    safeDelete(s_engine.fontRenderer);
    safeDelete(s_engine.lineRenderer);
    safeDelete(s_engine.spriteRenderer);
    gfx::deleteGpu(s_engine.gpu);
    s_engine.gpu = NULL;

    sGfxClearInternal();
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::gfx::Gpu * GN::engine::getGpu() { return s_engine.gpu; }

//
//
// -----------------------------------------------------------------------------
GN_API GN::gfx::SpriteRenderer * GN::engine::getSpriteRenderer() { return s_engine.spriteRenderer; }

//
//
// -----------------------------------------------------------------------------
GN_API GN::gfx::LineRenderer * GN::engine::getLineRenderer() { return s_engine.lineRenderer; }

//
//
// -----------------------------------------------------------------------------
GN_API GN::gfx::BitmapFont * GN::engine::getDefaultFontRenderer() { return s_engine.fontRenderer; }

//
//
// -----------------------------------------------------------------------------
GN_API GN::gfx::GpuResourceDatabase * GN::engine::getGdb() { return s_engine.gdb; }

//
//
// -----------------------------------------------------------------------------
GN_API GN::engine::Entity * GN::engine::findEntity(int id) {
    std::lock_guard<std::mutex> lock(s_engine.entityLock);

    if (s_engine.entities.validHandle(id)) {
        return s_engine.entities[id];
    } else {
        return NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
GN_API int GN::engine::impl::onEntityCtor(Entity * e) {
    GN_ASSERT(e);

    std::lock_guard<std::mutex> lock(s_engine.entityLock);

    return s_engine.entities.add(e);
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::engine::impl::onEntityDtor(int id) {
    std::lock_guard<std::mutex> lock(s_engine.entityLock);

    GN_VERIFY(s_engine.entities.remove(id));
}
