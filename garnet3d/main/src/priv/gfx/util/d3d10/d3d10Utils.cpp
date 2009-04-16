#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.d3d10.utils");

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
DXGI_SAMPLE_DESC GN::d3d10::constructSampleDesc(
    ID3D10Device       & device,
    MultiSampleAntiAlias msaa,
    DXGI_FORMAT          format )
{
    DXGI_SAMPLE_DESC sd;

    if( msaa )
    {
        static UINT counts[] = { 32, 16, 8, 4, 2 };

        UINT quality;

        for( size_t i = 0; i < GN_ARRAY_COUNT(counts); ++i )
        {
            if( S_OK == device.CheckMultisampleQualityLevels( format, counts[i], &quality ) && quality > 0 )
            {
                sd.Count = counts[i];
                sd.Quality = quality - 1;
                return sd;
            }
        }

        GN_WARN(sLogger)( "current device does not support MSAA" );
    }

    sd.Count = 1;
    sd.Quality = 0;
    return sd;
}
