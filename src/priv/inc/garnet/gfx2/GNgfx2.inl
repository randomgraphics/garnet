//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter( const StrA & name, const KernelParameter & value )
{
    KernelParameterHandle h = mKernel.getParameterHandle( name );
    if( h ) setParameter( h, value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setRawParameter(
    const StrA & name, size_t offset, size_t bytes, const void * data )
{
    KernelParameterHandle h = mKernel.getParameterHandle( name );
    if( h ) setRawParameter( h, offset, bytes, data );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::unsetParameter( const StrA & name )
{
    KernelParameterHandle h = mKernel.getParameterHandle( name );
    if( h ) unsetParameter( h );
}
