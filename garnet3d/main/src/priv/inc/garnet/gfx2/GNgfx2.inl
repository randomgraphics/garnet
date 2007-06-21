//
//
// -----------------------------------------------------------------------------
inline void GN::gfx2::EffectParameterSet::setParameter( const StrA & name, const EffectParameter & value )
{
    EffectParameterHandle h = mEffect.getParameterHandle( name );
    if( h ) setParameter( h, value );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx2::EffectParameterSet::unsetParameter( const StrA & name )
{
    EffectParameterHandle h = mEffect.getParameterHandle( name );
    if( h ) unsetParameter( h );
}
