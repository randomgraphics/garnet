// *****************************************************************************
// GpuResource class
// *****************************************************************************

//
// Get resource type (GUID)
// -----------------------------------------------------------------------------
inline const GN::Guid & GN::gfx::GpuResource::type() const { return *mDatabase.getResourceType(this); }

//
// Get resource name
// -----------------------------------------------------------------------------
inline const GN::StrA & GN::gfx::GpuResource::name() const { return *mDatabase.getResourceName(this); }

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline T * GN::gfx::GpuResource::castTo(GpuResource * r) {
    GN_ASSERT(0 == r || T::guid() == *r->getGdb().getResourceType(r));
    return (T *) r;
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline T & GN::gfx::GpuResource::castTo(GpuResource & r) {
    GN_ASSERT(T::guid() == *r.getGdb().getResourceType(&r));
    return (T &) r;
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline const T & GN::gfx::GpuResource::castTo(const GpuResource & r) {
    GN_ASSERT(T::guid() == *r.getGdb().getResourceType(&r));
    return (const T &) r;
}

// *****************************************************************************
// GpuResourceDatabase class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline GN::AutoRef<GN::gfx::GpuResource> GN::gfx::GpuResourceDatabase::findOrCreateResource(const Guid & type, const char * name, bool * isExistingResource) {
    AutoRef<GpuResource> res = findResource(type, name);
    if (isExistingResource) *isExistingResource = (NULL != res);
    return res ? res : createResource(type, name);
}

//
//
// -----------------------------------------------------------------------------
template<class T>
inline GN::AutoRef<T> GN::gfx::GpuResourceDatabase::createResource(const char * name) {
    GpuResource * res = createResource(T::guid(), name).detach();
    return attachTo(GpuResource::castTo<T>(res));
}

//
//
// -----------------------------------------------------------------------------
template<class T>
inline GN::AutoRef<T> GN::gfx::GpuResourceDatabase::findResource(const char * name) const {
    GpuResource * res = findResource(T::guid(), name).detach();
    return attachTo(GpuResource::castTo<T>(res));
}

//
//
// -----------------------------------------------------------------------------
template<class T>
inline GN::AutoRef<T> GN::gfx::GpuResourceDatabase::findOrCreateResource(const char * name, bool * isExistingResource) {
    GpuResource * res = findOrCreateResource(T::guid(), name, isExistingResource).detach();
    return attachTo(GpuResource::castTo<T>(res));
}
