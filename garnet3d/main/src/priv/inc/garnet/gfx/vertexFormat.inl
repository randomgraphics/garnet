//! \cond NEVER

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char *
GN::gfx::vtxSem2Str( VtxSem vsem )
{
    static const char * table[] =
    {
    #define GNGFX_DEFINE_VTXSEM( tag, d3decl, d3dindex, glname, glindex, cgname ) #tag,
    #include "vertexSemanticMeta.h"
    #undef GNGFX_DEFINE_VTXSEM
    };
    if( 0 <= vsem && vsem < NUM_VTXSEMS ) return table[vsem];
    else return "BAD_VTXSEM";
}
//
GN_INLINE bool
GN::gfx::vtxSem2Str( StrA & result, VtxSem vsem )
{
    result = vtxSem2Str( vsem );
    return "BAD_VTXSEM" != result;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::VtxSem
GN::gfx::str2VtxSem( const char * str )
{
    static const char * table[] =
    {
    #define GNGFX_DEFINE_VTXSEM( tag, d3decl, d3dindex, glname, glindex, cgname ) #tag,
    #include "vertexSemanticMeta.h"
    #undef GNGFX_DEFINE_VTXSEM
    };
    if ( str )
    {
        for( size_t i = 0; i < NUM_VTXSEMS; ++i )
        {
            if( 0 == ::strcmp(table[i],str) ) return VtxSem(i);
        }
    }
    // failed
    return VTXSEM_INVALID;
}
//
GN_INLINE bool
GN::gfx::str2VtxSem( VtxSem & result, const char * str )
{
    result = str2VtxSem( str );
    return VTXSEM_INVALID != result;
}

// *****************************************************************************
// inline function of class VtxFmtDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::VtxFmtDesc::clear()
{
    for( int i = 0; i < NUM_VTXSEMS; ++i )
    {
        attribs[i].used = false;
        streams[i].numAttribs = 0;
        streams[i].stride = 0;
    }
    numStreams = 0;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool GN::gfx::VtxFmtDesc::addAttrib(
    uint8_t  stream,
    uint16_t offset,
    VtxSem   semantic,
    ClrFmt   format )
{
    // check parameters
    if( stream >= NUM_VTXSEMS )
    {
        GN_ERROR( "invalid stream index!" );
        return false;
    }
    if( semantic >= NUM_VTXSEMS )
    {
        GN_ERROR( "invalid attribute semantic!" );
        return false;
    }
    if( attribs[semantic].used )
    {
        GN_ERROR( "semantic '%s' is already occupied!", vtxSem2Str(semantic) );
        return false;
    }
    if( format >= NUM_CLRFMTS )
    {
        GN_ERROR( "invalid attribute format!" );
        return false;
    }

    AttribDesc & a = attribs[semantic];
    StreamDesc & s = streams[stream];

    // calculate attribute offset and stream stride
    if ( 0 == offset ) offset = s.stride;
    uint16_t new_stride = getClrFmtDesc(format).bits / 8;
    if ( 0 == new_stride )
    {
        GN_ERROR( "pixel size of the format must be larger than 1 byte!" );
        return false;
    }
    new_stride = new_stride + offset;

    // update attribute
    a.used = true;
    a.stream = stream;
    a.offset = offset;
    a.format = format;

    // update stream
    if( numStreams <= stream ) numStreams = stream + 1;
    if( s.stride < new_stride ) s.stride = new_stride;
    s.attribs[s.numAttribs] = semantic;
    ++s.numAttribs;

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool
GN::gfx::VtxFmtDesc::operator == ( const VtxFmtDesc & rhs ) const
{
    // check streams
    if( numStreams != rhs.numStreams ) return false;
    for( uint8_t i = 0; i < numStreams; ++i )
    {
        const StreamDesc & s1 = streams[i];
        const StreamDesc & s2 = rhs.streams[i];

        if( s1.stride != s2.stride || s1.numAttribs != s2.numAttribs )
            return false;

        for( uint8_t j = 0; j < s1.numAttribs; ++j )
        {
            if( s1.attribs[j] != s2.attribs[j] ) return false;
        }
    }

    // check attributes
    for( int i = 0; i < NUM_VTXSEMS; ++i )
    {
        const AttribDesc & a1 = attribs[i];
        const AttribDesc & a2 = rhs.attribs[i];

        if( a1.used != a2.used ) return false;

        if( a1.used )
        {
            if( a1.format != a2.format ||
                a1.offset != a2.offset ||
                a1.stream != a2.stream ) return false;
        }
    }

    // program goes here means equal decl.
    return true;
}

//! \endcond
