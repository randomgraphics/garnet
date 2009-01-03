#include "pch.h"
#include "mtshader.h"

using namespace GN::gfx;

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadGpuProgram::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadGpuProgram, () );

    // Do custom init here

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpuProgram::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from GpuProgram
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::MultiThreadGpuProgram::getNumParameters() const
{
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
const GpuProgramParameterDesc *
GN::gfx::MultiThreadGpuProgram::getParameters() const
{
    GN_UNIMPL();
    return NULL;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpuProgram::setParameter(
    size_t       index,
    const void * value,
    size_t       length )
{
    GN_UNUSED_PARAM( index );
    GN_UNUSED_PARAM( value );
    GN_UNUSED_PARAM( length );
}
