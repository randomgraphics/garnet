#include "pch.h"
#include <garnet/GNrdg.h>
#include "runtime-type.h"

namespace GN::rdg {

// Define the type id for the public types defined in rdg public headers

GN_API const uint64_t ClearRenderTarget::A::TYPE = getNextUniqueTypeId();
GN_API const uint64_t PrepareBackbuffer::TYPE    = getNextUniqueTypeId();
GN_API const uint64_t PrepareBackbuffer::A::TYPE = getNextUniqueTypeId();
GN_API const uint64_t Texture::TYPE              = getNextUniqueTypeId();
GN_API const uint64_t GpuContext::TYPE           = getNextUniqueTypeId();
GN_API const uint64_t PresentBackbuffer::A::TYPE = getNextUniqueTypeId();
GN_API const uint64_t PresentBackbuffer::TYPE    = getNextUniqueTypeId();
GN_API const uint64_t Backbuffer::TYPE           = getNextUniqueTypeId();
GN_API const uint64_t ClearRenderTarget::TYPE    = getNextUniqueTypeId();
GN_API const uint64_t GenericDraw::TYPE         = getNextUniqueTypeId();
GN_API const uint64_t GenericDraw::A::TYPE       = getNextUniqueTypeId();

} // namespace GN::rdg