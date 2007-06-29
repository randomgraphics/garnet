//
//
// -----------------------------------------------------------------------------
inline GN::gfx::KernelParameter *
GN::gfx::KernelParameterSet::getParameter( const StrA & name ) const
{
    KernelParameterHandle h = mKernel.getParameterHandle( name );
    return h ? getParameter( h ) : 0;
}
