#ifndef __GN_GFX_PRIMITIVE_H__
#define __GN_GFX_PRIMITIVE_H__
// *****************************************************************************
/// \file
/// \brief   primitive types and related utilities.
/// \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// ªÊÕº‘≠”Ô
    ///
    enum PrimitiveType
    {
        POINT_LIST,     ///< point list
        LINE_LIST,      ///< line list
        LINE_STRIP,     ///< line strip
        TRIANGLE_LIST,  ///< triangle list
        TRIANGLE_STRIP, ///< triangle strip
        QUAD_LIST     , ///< quad list. D3D9 does not support this primitive.
        RECT_LIST     , ///< rect list. This is xenon special primitive.
        NUM_PRIMITIVES  ///< number of available primitive types
    };

    /// \name Primitive utils
    //@{

    ///
    /// convert primitive type to string
    ///
    inline StrA primitiveType2Str( PrimitiveType pt )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return "POINT_LIST";
            case LINE_LIST      : return "LINE_LIST";
            case LINE_STRIP     : return "LINE_STRIP";
            case TRIANGLE_LIST  : return "TRIANGLE_LIST";
            case TRIANGLE_STRIP : return "TRIANGLE_STRIP";
            case QUAD_LIST      : return "QUAD_LIST";
            case RECT_LIST      : return "RECT_LIST";
            default             : return "Invalid primitive type!";
        }
        GN_UNGUARD_SLOW;
    }

    ///
    /// convert string to primitive
    ///
    inline PrimitiveType str2PrimitiveType( const StrA & str )
    {
        GN_GUARD_SLOW;
        if( "POINT_LIST" == str ) return POINT_LIST;
        else if( "LINE_LIST" == str ) return LINE_LIST;
        else if( "LINE_STRIP" == str ) return LINE_STRIP;
        else if( "TRIANGLE_LIST" == str ) return TRIANGLE_LIST;
        else if( "TRIANGLE_STRIP" == str ) return TRIANGLE_STRIP;
        else if( "QUAD_LIST" == str ) return QUAD_LIST;
        else if( "RECT_LIST" == str ) return RECT_LIST;
        else
        {
            // failed
            static Logger * sLogger = getLogger("GN.gfx.misc");
            GN_ERROR(sLogger)( "invalid primitive string : '%s'!", str.cptr() );
            return NUM_PRIMITIVES;
        }
        GN_UNGUARD_SLOW;
    }

    ///
    /// calculate primitive count from vertex count
    ///
    /// \param pt       rendering primitive
    /// \param numvert  vertex count
    /// \return         primitive count
    ///
    inline size_t calcPrimitiveCount( PrimitiveType pt, size_t numvert )
    {
        GN_GUARD_SLOW;

        switch( pt )
        {
            case POINT_LIST     : return numvert;
            case LINE_LIST      : return (numvert >= 2) ? numvert / 2 : 0;
            case LINE_STRIP     : return (numvert >= 2) ? numvert - 1 : 0;
            case TRIANGLE_LIST  : return (numvert >= 3) ? numvert / 3 : 0;
            case TRIANGLE_STRIP : return (numvert >= 3) ? numvert - 2 : 0;
            case QUAD_LIST      : return (numvert >= 4) ? numvert / 4 : 0;
            case RECT_LIST      : return (numvert >= 3) ? numvert / 3 : 0;
            default             :
                {
                    static Logger * sLogger = getLogger("GN.gfx.misc");
                    GN_ERROR(sLogger)( "Invalid primitive type!" );
                }
                return 0;
        }
        GN_UNGUARD_SLOW;
    }

    ///
    /// calculate vertex count from primitive count
    ///
    /// \param pt       rendering primitive
    /// \param numprim  primitive count
    /// \return         vertex count
    ///
    inline size_t calcVertexCount( PrimitiveType pt, size_t numprim )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return numprim;
            case LINE_LIST      : return numprim * 2;
            case LINE_STRIP     : return numprim > 0 ? numprim + 1 : 0;
            case TRIANGLE_LIST  : return numprim * 3;
            case TRIANGLE_STRIP : return numprim > 0 ? numprim + 2 : 0;
            case QUAD_LIST      : return numprim * 4;
            case RECT_LIST      : return numprim * 3;
            default :
                {
                    static Logger * sLogger = getLogger("GN.gfx.misc");
                    GN_ERROR(sLogger)( "Invalid primitive type!" );
                }
                return 0;
        }
        GN_UNGUARD_SLOW;
    }

    //@}

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_PRIMITIVE_H__
