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
    struct PrimitiveType
    {
        enum Enum
        {
            POINT_LIST,     ///< point list
            LINE_LIST,      ///< line list
            LINE_STRIP,     ///< line strip
            TRIANGLE_LIST,  ///< triangle list
            TRIANGLE_STRIP, ///< triangle strip
            QUAD_LIST     , ///< quad list. D3D9 does not support this primitive.
            RECT_LIST     , ///< rect list. This is xenon special primitive.
            NUM_PRIMITIVES, ///< number of available primitive types
            INVALID  = NUM_PRIMITIVES, // use to indicate invalid primitive type.
        };

        /// conver to string
        const char * ToString() const
        {
            static const char * TABLE[]=
            {
                "POINT_LIST",
                "LINE_LIST",
                "LINE_STRIP",
                "TRIANGLE_LIST",
                "TRIANGLE_STRIP",
                "QUAD_LIST",
                "RECT_LIST",
            };
            GN_CASSERT( GN_ARRAY_COUNT(TABLE) == (size_t)NUM_PRIMITIVES );

            unsigned int e = *this;
            if( e < GN_ARRAY_COUNT(TABLE) ) return TABLE[e];
            else return "INVALID_PRIMITIVE_TYPE";
        }

        /// convert from string
        static PrimitiveType sFromString( const char * s )
        {
            if( 0 == StringCompareI( "POINT_LIST", s ) ) return POINT_LIST;
            else if( 0 == StringCompareI( "LINE_LIST", s ) ) return LINE_LIST;
            else if( 0 == StringCompareI( "LINE_STRIP", s ) ) return LINE_STRIP;
            else if( 0 == StringCompareI( "TRIANGLE_LIST", s ) ) return TRIANGLE_LIST;
            else if( 0 == StringCompareI( "TRIANGLE_STRIP", s ) ) return TRIANGLE_STRIP;
            else if( 0 == StringCompareI( "QUAD_LIST", s ) ) return QUAD_LIST;
            else if( 0 == StringCompareI( "RECT_LIST", s ) ) return RECT_LIST;
            else return INVALID;
        }

        GN_DEFINE_ENUM_CLASS_HELPERS(PrimitiveType, Enum)
    };

    /// \name Primitive utils
    //@{

    ///
    /// calculate primitive count from vertex count
    ///
    /// \param pt       rendering primitive
    /// \param numvert  vertex count
    /// \return         primitive count
    ///
    inline size_t CalcPrimitiveCount( PrimitiveType pt, size_t numvert )
    {
        GN_GUARD_SLOW;

        switch( pt )
        {
            case PrimitiveType::POINT_LIST     : return numvert;
            case PrimitiveType::LINE_LIST      : return (numvert >= 2) ? numvert / 2 : 0;
            case PrimitiveType::LINE_STRIP     : return (numvert >= 2) ? numvert - 1 : 0;
            case PrimitiveType::TRIANGLE_LIST  : return (numvert >= 3) ? numvert / 3 : 0;
            case PrimitiveType::TRIANGLE_STRIP : return (numvert >= 3) ? numvert - 2 : 0;
            case PrimitiveType::QUAD_LIST      : return (numvert >= 4) ? numvert / 4 : 0;
            case PrimitiveType::RECT_LIST      : return (numvert >= 3) ? numvert / 3 : 0;
            default             :
                {
                    static Logger * sLogger = GetLogger("GN.gfx.misc");
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
    inline size_t CalcVertexCount( PrimitiveType pt, size_t numprim )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case PrimitiveType::POINT_LIST     : return numprim;
            case PrimitiveType::LINE_LIST      : return numprim * 2;
            case PrimitiveType::LINE_STRIP     : return numprim > 0 ? numprim + 1 : 0;
            case PrimitiveType::TRIANGLE_LIST  : return numprim * 3;
            case PrimitiveType::TRIANGLE_STRIP : return numprim > 0 ? numprim + 2 : 0;
            case PrimitiveType::QUAD_LIST      : return numprim * 4;
            case PrimitiveType::RECT_LIST      : return numprim * 3;
            default :
                {
                    static Logger * sLogger = GetLogger("GN.gfx.misc");
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
