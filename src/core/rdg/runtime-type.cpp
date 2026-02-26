#include "pch.h"
#include <garnet/GNrdg.h>
#include "runtime-type.h"

namespace GN::rdg {

// Define the type id for the public types defined in rdg public headers

GN_API const uint64_t Texture::TYPE_ID              = getNextUniqueTypeId();
GN_API const uint64_t GpuContext::TYPE_ID           = getNextUniqueTypeId();
GN_API const uint64_t Backbuffer::TYPE_ID           = getNextUniqueTypeId();
GN_API const uint64_t GpuDraw::TYPE_ID              = getNextUniqueTypeId();
GN_API const uint64_t GpuDraw::A::TYPE_ID           = getNextUniqueTypeId();
GN_API const uint64_t ClearRenderTarget::A::TYPE_ID = getNextUniqueTypeId();
GN_API const uint64_t ClearRenderTarget::TYPE_ID    = getNextUniqueTypeId();
GN_API const uint64_t PrepareBackbuffer::A::TYPE_ID = getNextUniqueTypeId();
GN_API const uint64_t PrepareBackbuffer::TYPE_ID    = getNextUniqueTypeId();
GN_API const uint64_t PresentBackbuffer::A::TYPE_ID = getNextUniqueTypeId();
GN_API const uint64_t PresentBackbuffer::TYPE_ID    = getNextUniqueTypeId();

} // namespace GN::rdg