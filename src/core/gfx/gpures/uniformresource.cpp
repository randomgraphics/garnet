#include "pch.h"
#include "uniformresource.h"

using namespace GN;
using namespace GN::gfx;

// static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// GN::gfx::UniformResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::UniformResource::guid() {
    static const Guid UNIFORM_GUID = {0xd140dbea, 0x9ce3, 0x4bb9, {0x9c, 0xa7, 0xb8, 0x14, 0xf3, 0x83, 0xab, 0x95}};
    return UNIFORM_GUID;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::UniformResource::reset(uint32 length, const void * initialData) {
    AutoRef<Uniform> u = attachTo(getGdb().getGpu().createUniform(length));
    if (!u) return false;

    if (initialData) u->update(0, length, initialData);

    setUniform(u);

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::UniformResource::setUniform(const AutoRef<Uniform> & newUniform) {
    if (mUniform == newUniform) return;

    mUniform = newUniform;

    sigUniformChanged(*this);
}

// *****************************************************************************
// GN::gfx::UniformResourceInternal
// *****************************************************************************

class UniformResourceInternal : public UniformResource {
    //
    //
    // -----------------------------------------------------------------------------
    UniformResourceInternal(GpuResourceDatabase & db): UniformResource(db) {}

    //
    //
    // -----------------------------------------------------------------------------
    ~UniformResourceInternal() {}

    //
    //
    // -----------------------------------------------------------------------------
    static GpuResource * sCreateInstance(GpuResourceDatabase & db) {
        GpuResource * res = new UniformResourceInternal(db);
        res->incref();
        return res;
    }

public:
    //
    //
    // -----------------------------------------------------------------------------
    static bool sRegisterFactory(GpuResourceDatabase & db) {
        GpuResourceFactory factory = {&sCreateInstance};

        if (db.hasResourceFactory(UniformResource::guid())) return true;

        return db.registerResourceFactory(UniformResource::guid(), "Uniform Resource", factory);
    }
};

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::registerUniformResourceFactory(GpuResourceDatabase & db) { return UniformResourceInternal::sRegisterFactory(db); }
