// *****************************************************************************
// GpuResource class
// *****************************************************************************

//
// protected constructor
// -----------------------------------------------------------------------------
inline GN::gfx::GpuResource::GpuResource( GpuResourceDatabase & db, GpuResourceHandle h )
    : mDatabase(db), mHandle(h)
{
}

//
// protected destructor
// -----------------------------------------------------------------------------
inline GN::gfx::GpuResource::~GpuResource()
{
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline T * GN::gfx::GpuResource::castTo( GpuResource * r )
{
    GN_ASSERT( 0 == r || T::guid() == *r->database().getResourceType( r->handle() ) );
    return (T*)r;
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline T & GN::gfx::GpuResource::castTo( GpuResource & r )
{
    GN_ASSERT( T::guid() == *r.database().getResourceType( r.handle() ) );
    return (T&)r;
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline T & GN::gfx::GpuResource::castTo()
{
    GN_ASSERT( T::guid() == *database().getResourceType( handle() ) );
    return (T&)*this;
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline const T & GN::gfx::GpuResource::castTo() const
{
    GN_ASSERT( T::guid() == *database().getResourceType( handle() ) );
    return (const T&)*this;
}
