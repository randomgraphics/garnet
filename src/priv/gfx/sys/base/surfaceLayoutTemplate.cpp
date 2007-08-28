#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.SurfaceLayoutTemplate");

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SurfaceLayoutTemplate::check() const
{
    GN_UNIMPL_WARNING();
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SurfaceLayoutTemplate::compatible( const SurfaceLayout & layout ) const
{
    #define CHECK_FIELD4( name, flag, expected, actual ) \
        if( flag && expected != actual ) \
        { \
            GN_ERROR(sLogger)( "incompatible " name ": expected(%d), input(%d)", expected, actual ); \
            return false; \
        }
    #define CHECK_FIELD3( flag, expected, actual ) CHECK_FIELD4( #actual, flag, expected, actual )
    #define CHECK_FIELD2( flag, field ) CHECK_FIELD3( flag, field, layout.field )

    if( flags.dim && layout.dim != dim )
    {
        GN_ERROR(sLogger)( "incompatible layout.dim: expected(%s), input(%s)",
            surfaceDimension2String(dim), 
            surfaceDimension2String(layout.dim) );
        return false;
    }

    CHECK_FIELD2( flags.levels, levels );
    CHECK_FIELD2( flags.faces, levels );
    CHECK_FIELD2( flags.width, basemap.width );
    CHECK_FIELD2( flags.height, basemap.height );
    CHECK_FIELD2( flags.depth, basemap.depth );
    CHECK_FIELD2( flags.rowBytes, basemap.rowBytes );
    CHECK_FIELD2( flags.sliceBytes, basemap.sliceBytes );
    CHECK_FIELD3( flags.attributes, attributes, layout.format.count );
    CHECK_FIELD3( flags.stride, stride, layout.format.stride );

    // check required attributes
    for( size_t i = 0; i < requiredAttributes.size(); ++i )
    {
        const SurfaceAttributeTemplate & sat = requiredAttributes[i];

        bool found = false;
        for( size_t i = 0; i < layout.format.count; ++i )
        {
            const SurfaceAttribute & sa = layout.format.attribs[i];
            if( sa.semantic == sat.semantic )
            {
                if( (size_t)-1 != sat.offset && sat.offset != sa.offset )
                {
                    GN_ERROR(sLogger)("offset of attribute #%d is incompatible with template.", i );
                    return false;
                }
                if( !sat.allowedFormats.empty() && sat.allowedFormats.end() == sat.allowedFormats.find(sa.format) )
                {
                    GN_ERROR(sLogger)("format of attribute #%d is incompatible with template.", i );
                    return false;
                }

                found = true;
                break;
            }
        }

        if( !found )
        {
            GN_ERROR(sLogger)( "required attribute with semantic '%s' is missing.", sat.semantic.str() );
            return false;
        }
    }

    #undef CHECK_FIELD2
    #undef CHECK_FIELD3
    #undef CHECK_FIELD4

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SurfaceLayoutTemplate::apply( SurfaceLayout & layout ) const
{
    GN_UNUSED_PARAM( layout );
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SurfaceLayoutTemplate::mergeWith( const SurfaceLayoutTemplate & templ )
{
    GN_UNUSED_PARAM( templ );
    GN_UNIMPL_WARNING();
    return true;
}
