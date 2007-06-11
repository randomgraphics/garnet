#include "pch.h"
#include "oglSampler.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// local functions
// *****************************************************************************

/*
/// map filter type to opengl constant
// -----------------------------------------------------------------------------
static GN_INLINE GLenum sTexFilter2OGL( GN::gfx::TexFilter f )
{
    switch(f)
    {
    case GN::gfx::TEXFILTER_NEAREST : return GL_NEAREST;
    case GN::gfx::TEXFILTER_LINEAR  : return GL_LINEAR;
    case GN::gfx::TEXFILTER_N_MIP_N : return GL_NEAREST_MIPMAP_NEAREST;
    case GN::gfx::TEXFILTER_N_MIP_L : return GL_NEAREST_MIPMAP_LINEAR;
    case GN::gfx::TEXFILTER_L_MIP_N : return GL_LINEAR_MIPMAP_NEAREST;
    case GN::gfx::TEXFILTER_L_MIP_L : return GL_LINEAR_MIPMAP_LINEAR;
    default : GN_ASSERT_EX( 0, "invaid filter type!" );
        return GL_LINEAR;
    }
}

///
/// map wrap mode to opengl constant
// -----------------------------------------------------------------------------
static GN_INLINE GLenum sTexWrap2OGL( GN::gfx::TexWrap w )
{
    if( GN::gfx::TEXWRAP_REPEAT == w ) return GL_REPEAT;
    else if( GN::gfx::TEXWRAP_CLAMP == w ) return GL_CLAMP;
    else if( GN::gfx::TEXWRAP_CLAMP_TO_EDGE == w )
    {
        if( GLEW_EXT_texture_edge_clamp )
            return GL_CLAMP_TO_EDGE_EXT;
        else if (GLEW_SGIS_texture_edge_clamp )
            return GL_CLAMP_TO_EDGE_SGIS;
        else
        {
            GN_ERROR(sLogger)( "do not support clamp to edge!" );
            return GL_CLAMP;
        }
    }
    else
    {
        GN_ASSERT_EX( 0, "invaid wrap type!" );
        return GL_REPEAT;
    }
}*/

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLSamplerObject::init( const SamplerDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLSamplerObject, () );

    mDesc = desc;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLSamplerObject::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLSamplerObject::bind() const
{
    GN_GUARD_SLOW;

    GN_UNIMPL_WARNING();

    GN_UNGUARD_SLOW;
}


