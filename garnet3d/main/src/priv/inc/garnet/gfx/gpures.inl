// *****************************************************************************
// GpuResource class
// *****************************************************************************

//
// Get resource type (GUID)
// -----------------------------------------------------------------------------
inline const GN::Guid &
GN::gfx::GpuResource::Type() const
{
    return mDatabase.GetResourceType(this);
}

//
// Get resource name
// -----------------------------------------------------------------------------
inline const char *
GN::gfx::GpuResource::Name() const
{
    return mDatabase.GetResourceName(this);
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline T * GN::gfx::GpuResource::CastTo( GpuResource * r )
{
    GN_ASSERT( 0 == r || T::GetGuid() == r->GetGdb().GetResourceType( r ) );
    return (T*)r;
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline T & GN::gfx::GpuResource::CastTo( GpuResource & r )
{
    GN_ASSERT( T::GetGuid() == r.GetGdb().GetResourceType( &r ) );
    return (T&)r;
}

//
// Cast GPU resource pointer with type check.
// -----------------------------------------------------------------------------
template<typename T>
inline const T & GN::gfx::GpuResource::CastTo( const GpuResource & r )
{
    GN_ASSERT( T::GetGuid() == r.GetGdb().GetResourceType( this ) );
    return (const T&)r;
}

// *****************************************************************************
// GpuResourceDatabase class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline GN::AutoRef<GN::gfx::GpuResource>
GN::gfx::GpuResourceDatabase::FindOrCreateResource( const Guid & type, const char * name )
{
    AutoRef<GpuResource> res = FindResource( type, name );
    return res ? res : CreateResource( type, name );
}

//
//
// -----------------------------------------------------------------------------
template<class T> inline GN::AutoRef<T>
GN::gfx::GpuResourceDatabase::CreateResource( const char * name )
{
    GpuResource * res = CreateResource( T::GetGuid(), name ).Detach();
    return AutoRef<T>( GpuResource::CastTo<T>(res) );
}

//
//
// -----------------------------------------------------------------------------
template<class T> inline GN::AutoRef<T>
GN::gfx::GpuResourceDatabase::FindResource( const char * name ) const
{
    GpuResource * res = FindResource( T::GetGuid(), name ).Detach();
    return AutoRef<T>( GpuResource::CastTo<T>(res) );
}

//
//
// -----------------------------------------------------------------------------
template<class T> inline GN::AutoRef<T>
GN::gfx::GpuResourceDatabase::FindOrCreateResource( const char * name )
{
    GpuResource * res = FindOrCreateResource( T::GetGuid(), name ).Detach();
    return AutoRef<T>( GpuResource::CastTo<T>(res) );
}
