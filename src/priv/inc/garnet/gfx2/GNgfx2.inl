//
//
// -----------------------------------------------------------------------------
inline GN::gfx::KernelParameter *
GN::gfx::KernelParameterSet::getParameter( const StrA & name ) const
{
    GN_GUARD_SLOW;

    return getParameter( mKernel.getParameterIndex( name ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    size_t index, const char * value )
{
    KernelParameter * p = getParameter( index );
    if( p ) p->sets( 0, 1, &value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    size_t index, int value )
{
    KernelParameter * p = getParameter( index );
    if( p ) p->seti( 0, 1, &value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    size_t index, float value )
{
    KernelParameter * p = getParameter( index );
    if( p ) p->setf( 0, 1, &value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    size_t index, const Vector4f & value )
{
    KernelParameter * p = getParameter( index );
    if( p ) p->setf( 0, 4, value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::KernelParameterSet::setParameter(
    size_t index, const Matrix44f & value )
{
    KernelParameter * p = getParameter( index );
    if( p ) p->setf( 0, 16, value[0] );
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
inline void GN::gfx::KernelParameterSet::setParameter(
    const StrA & name, const T & value )
{
    size_t index = mKernel.getParameterIndex( name );
    if( (size_t)-1 != index ) setParameter( index, value );
}
