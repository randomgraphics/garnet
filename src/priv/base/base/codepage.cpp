#include "pch.h"

//
//
// -----------------------------------------------------------------------------
size_t
GN::ConvertCharacterEncoding(
    void            * /*destBuffer*/,
    size_t            /*destBufferSizeInBytes*/,
    CharacterEncoding /*destEncoding*/,
    const void      * /*sourceBuffer*/,
    size_t            /*sourceBufferSizeInBytes*/,
    CharacterEncoding /*sourceEncoding*/
    )
{
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void *
GN::ConvertCharacterEncoding(
    size_t           * /*destBufferSizeInBytes*/,
    CharacterEncoding  /*destEncoding*/,
    const void       * /*sourceBuffer*/,
    size_t             /*sourceBufferSizeInBytes*/,
    CharacterEncoding  /*sourceEncoding*/
    )
{
    GN_UNIMPL();
    return NULL;
}
