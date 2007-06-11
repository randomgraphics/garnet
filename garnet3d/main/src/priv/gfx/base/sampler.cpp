#include "pch.h"

static GN::gfx::SamplerDesc sMakeDefaultSampler()
{
    using namespace GN;
    using namespace GN::gfx;

    SamplerDesc desc;

    GN_TODO( "setup default sampler" );
    memset( &desc, 0, sizeof(desc) );

    return desc;
}

const GN::gfx::SamplerDesc GN::gfx::SamplerDesc::DEFAULT = sMakeDefaultSampler();
