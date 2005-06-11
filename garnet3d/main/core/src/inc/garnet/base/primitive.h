#ifndef __GN_BASE_PRIMITIVE_H__
#define __GN_BASE_PRIMITIVE_H__
// *****************************************************************************
//! \file    primitive.h
//! \brief   define rendering primitives
//! \author  chenlee (2005.6.4)
// *****************************************************************************

namespace GN
{
    //!
    //! ªÊÕº‘≠”Ô
    //!
    enum PrimitiveType
    {
        POINT_LIST,     //!< point list
        LINE_LIST,      //!< line list
        LINE_STRIP,     //!< line strip
        TRIANGLE_LIST,  //!< triangle list
        TRIANGLE_STRIP, //!< triangle strip
        NUM_PRIMITIVES, //!< number of available primitive types
    };

    //!
    //! convert primitive type to string
    //!
    inline StrA prim2str( PrimitiveType pt )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return "POINT_LIST";
            case LINE_LIST      : return "LINE_LIST";
            case LINE_STRIP     : return "LINE_STRIP";
            case TRIANGLE_LIST  : return "TRIANGLE_LIST";
            case TRIANGLE_STRIP : return "TRIANGLE_STRIP";
            default             : return "Invalid primitive type!";
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! convert string to primitive
    //!
    inline PrimitiveType
    str2prim( const StrA & str )
    {
        GN_GUARD_SLOW;
        if( "POINT_LIST" == str ) return POINT_LIST;
        else if( "LINE_LIST" == str ) return LINE_LIST;
        else if( "LINE_STRIP" == str ) return LINE_STRIP;
        else if( "TRIANGLE_LIST" == str ) return TRIANGLE_LIST;
        else if( "TRIANGLE_STRIP" == str ) return TRIANGLE_STRIP;
        else
        {
            // failed
            GN_ERROR( "invalid primitive string : '%s'!", str.c_str() );
            return NUM_PRIMITIVES;
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! calculate primitive count from vertex count
    //!
    //! \param pt       rendering primitive
    //! \param numvert  vertex count
    //! \return         primitive count
    //!
    inline size_t CalcPrimitiveCount( PrimitiveType pt, size_t numvert )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return numvert;
            case LINE_LIST      : return (numvert >= 2) ? numvert / 2 : 0;
            case LINE_STRIP     : return (numvert >= 2) ? numvert - 1 : 0;
            case TRIANGLE_LIST  : return (numvert >= 3) ? numvert / 3 : 0;
            case TRIANGLE_STRIP : return (numvert >= 3) ? numvert - 2 : 0;
            default             :
                GN_ERROR( "Invalid primitive type!" );
                return 0;
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! calculate vertex count from primitive count
    //!
    //! \param pt       rendering primitive
    //! \param numprim  primitive count
    //! \return         vertex count
    //!
    inline size_t CalcVertexCount( PrimitiveType pt, size_t numprim )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return numprim;
            case LINE_LIST      : return numprim * 2;
            case LINE_STRIP     : return numprim > 0 ? numprim + 1 : 0;
            case TRIANGLE_LIST  : return numprim * 3;
            case TRIANGLE_STRIP : return numprim > 0 ? numprim + 2 : 0;
            default :
                GN_ERROR( "Invalid primitive type!" );
                return 0;
        }
        GN_UNGUARD_SLOW;
    }
}

// *****************************************************************************
//                           End of primitive.h
// *****************************************************************************
#endif // __GN_BASE_PRIMITIVE_H__
