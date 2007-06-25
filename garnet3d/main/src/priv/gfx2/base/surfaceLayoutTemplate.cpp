#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.SurfaceLayoutTemplate");

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::SurfaceLayoutTemplate::check() const
{
    GN_UNIMPL_WARNING();
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::SurfaceLayoutTemplate::compatible( const SurfaceLayout & layout ) const
{
    if( flags.dim && layout.dim != dim )
    {
        GN_ERROR(sLogger)( "incompatible dim: expected(%s), input(%s)",
            surfaceDimension2String(dim), 
            surfaceDimension2String(layout.dim) );
        return false;
    }

    if( flags.levels && layout.levels != levels )
    {
        GN_ERROR(sLogger)( "incompatible level count: expected(%d), input(%d)", levels, layout.levels );
        return false;
    }

    GN_UNUSED_PARAM( layout );
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::SurfaceLayoutTemplate::apply( SurfaceLayout & layout ) const
{
    GN_UNUSED_PARAM( layout );
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::SurfaceLayoutTemplate::mergeWith( const SurfaceLayoutTemplate & templ )
{
    GN_UNUSED_PARAM( templ );
    GN_UNIMPL_WARNING();
    return true;
}
