//
//
// -----------------------------------------------------------------------------
inline void GN::gfx2::Effect::setParameter( const StrA & name, const EffectParameter & value )
{
    EffectParameterHandle h = getParameterHandle( name );
    if( h ) setParameter( h, value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx2::Effect::unsetParameter( const StrA & name )
{
    EffectParameterHandle h = getParameterHandle( name );
    if( h ) unsetParameter( h );
}
