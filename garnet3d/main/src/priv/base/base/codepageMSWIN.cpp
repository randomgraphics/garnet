#include "pch.h"
#include "codepageMSWIN.h"

#if GN_MSWIN

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::MBCEImplMSWIN::init( MultiByteCharacterEncoding::Enum )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::MBCEImplMSWIN, () );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::MBCEImplMSWIN::quit()
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
size_t
GN::MBCEImplMSWIN::toUTF16_LE(
    void            * /*destBuffer*/,
    size_t            /*destBufferSizeInBytes*/,
    const void      * /*sourceBuffer*/,
    size_t            /*sourceBufferSizeInBytes*/ )
{
    GN_UNIMPL();
    return 0;
}

size_t
GN::MBCEImplMSWIN::fromUTF16_LE(
    void            * /*destBuffer*/,
    size_t            /*destBufferSizeInBytes*/,
    const void      * /*sourceBuffer*/,
    size_t            /*sourceBufferSizeInBytes*/ )
{
    GN_UNIMPL();
    return 0;
}

#endif
