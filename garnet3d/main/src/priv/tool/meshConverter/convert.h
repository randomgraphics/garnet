#ifndef __GN_MESHCONVERTER_CONVERT_H__
#define __GN_MESHCONVERTER_CONVERT_H__
// *****************************************************************************
/// \file
/// \brief   Common header of mesh converter
/// \author  chen@@CHENLI-HOMEPC (2007.3.24)
// *****************************************************************************

namespace GN
{
    ///
    /// mesh convert options
    ///
    struct ConvertOptions
    {
        StrA inputFileName;  ///< input file name (full path)
        StrA outputFileName; ///< output file name (full path)
    };

    ///
    /// convert from wavefront obj file
    ///
    bool obj2mesh( const ConvertOptions & );

    ///
    /// convert from 3dsmax ASE file
    ///
    bool ase2mesh( const ConvertOptions & );
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_MESHCONVERTER_CONVERT_H__
