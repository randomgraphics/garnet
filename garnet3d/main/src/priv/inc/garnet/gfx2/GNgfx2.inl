//
//
// -----------------------------------------------------------------------------
inline GN::gfx::KernelParameter *
GN::gfx::KernelParameterSet::getParameter( const StrA & name ) const
{
    KernelParameterHandle h = mKernel.getParameterHandle( name );
    return h ? getParameter( h ) : 0;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    KernelParameterHandle handle, const char * value )
{
    KernelParameter * p = getParameter( handle );
    if( p ) p->sets( 0, 1, &value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    KernelParameterHandle handle, int value )
{
    KernelParameter * p = getParameter( handle );
    if( p ) p->seti( 0, 1, &value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    KernelParameterHandle handle, float value )
{
    KernelParameter * p = getParameter( handle );
    if( p ) p->setf( 0, 1, &value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    KernelParameterHandle handle, const Vector4f & value )
{
    KernelParameter * p = getParameter( handle );
    if( p ) p->setf( 0, 4, value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    KernelParameterHandle handle, const Matrix44f & value )
{
    KernelParameter * p = getParameter( handle );
    if( p ) p->setf( 0, 16, value[0] );
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
inline void GN::gfx::KernelParameterSet::setParameter(
    const StrA & name, const T & value )
{
    KernelParameterHandle h = mKernel.getParameterHandle( name );
    if( h ) setParameter( h, value );
}
