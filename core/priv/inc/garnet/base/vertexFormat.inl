//! \cond NEVER

//
//
// ----------------------------------------------------------------------------
GN_INLINE const char *
GN::vertsem2str( VertexSemantic vsem )
{
    static const char * table[] =
    {
    #define GN_VERTEX_SEMANTIC( tag, d3decl, d3dindex, \
                                 glname, glindex, cgname ) #tag,
    #include "vertexSemanticMeta.h"
    #undef GN_VERTEX_SEMANTIC
    };
    if( 0 <= vsem && vsem < NUM_VERTSEMS ) return table[vsem];
    else return "BAD_VERTSEM";
}
//
GN_INLINE bool
GN::vertsem2str( StrA & result, VertexSemantic vsem )
{
    result = vertsem2str( vsem );
    return "BAD_VERTSEM" != result;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE GN::VertexSemantic
GN::str2vertsem( const char * str )
{
    static const char * table[] =
    {
    #define GN_VERTEX_SEMANTIC( tag, d3decl, d3dindex, \
                                glname, glindex, cgname ) #tag,
    #include "vertexSemanticMeta.h"
    #undef GN_VERTEX_SEMANTIC
    };
    if ( str )
    {
        for( size_t i = 0; i < NUM_VERTSEMS; ++i )
        {
            if( 0 == ::strcmp(table[i],str) ) return VertexSemantic(i);
        }
    }
    // failed
    return VERTSEM_INVALID;
}
//
GN_INLINE bool
GN::str2vertsem( VertexSemantic & result, const char * str )
{
    result = str2vertsem( str );
    return VERTSEM_INVALID != result;
}

// ****************************************************************************
// inline function of class VertexFormatDesc
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
GN_INLINE void GN::VertexFormatDesc::reset()
{
    for( int i = 0; i < NUM_VERTSEMS; ++i )
    {
        attribs[i].used = false;
        streams[i].numAttribs = 0;
        streams[i].stride = 0;
    }
    numStreams = 0;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE bool GN::VertexFormatDesc::addAttrib(
    uint8_t         stream,
    uint8_t         offset,
    VertexSemantic  semantic,
    ColorFormat     format )
{
    // check parameters
    if( stream >= NUM_VERTSEMS )
    {
        GN_ERROR( "invalid stream index!" );
        return false;
    }
    if( semantic >= NUM_VERTSEMS )
    {
        GN_ERROR( "invalid attribute semantic!" );
        return false;
    }
    if( attribs[semantic].used )
    {
        GN_ERROR( "semantic '%s' is already occupied!", vertsem2str(semantic) );
        return false;
    }
    if( format >= NUM_COLOR_FORMATS )
    {
        GN_ERROR( "invalid attribute format!" );
        return false;
    }

    AttribDesc & a = attribs[semantic];
    StreamDesc & s = streams[stream];

    // calculate attribute offset and stream stride
    if ( 0 == offset ) offset = s.stride;
    uint8_t new_stride = getColorFormatDesc(format).bits / 8;
    if ( 0 == new_stride )
    {
        GN_ERROR( "pixel size of the format must be larger than 1 byte!" );
        return false;
    }
    if ( long(offset) + long(new_stride) > 256 )
    {
        GN_ERROR( "stride can't exceed 256!" );
        return false;
    }
    new_stride = new_stride + offset;

    // update attribute
    a.used = true;
    a.stream = stream;
    a.offset = offset;
    a.format  = format;

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
// ----------------------------------------------------------------------------
GN_INLINE bool
GN::VertexFormatDesc::operator == ( const VertexFormatDesc & rhs ) const
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
    for( int i = 0; i < NUM_VERTSEMS; ++i )
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
