#include "pch.h"

static GN::Logger * sLogger = GN::getLogger( "GN.gfx.base.UniformValue" );

//
//  <UniformValue type="vector4" count="1">
//      ...
//  </UniformValue>
//

// *****************************************************************************
// local functions
// *****************************************************************************

using namespace GN;

//
// get integer value of specific attribute
// -----------------------------------------------------------------------------
template<typename T>
static T sGetIntAttrib( const XmlElement & node, const char * attribName, T defval )
{
    T result;
    const XmlAttrib * a = node.findAttrib( attribName );
    if ( !a || !str2Int<T>( result, a->value.cptr() ) )
    {
        GN_TRACE(sLogger)( "attribute '%s' is missing!", attribName );
        return defval;
    }
    else
    {
        return result;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sParseFloats( float * buffer, size_t count, const StrA & text )
{
    size_t n = str2Floats( buffer, count, text.cptr() );
    if( n < count )
    {
        GN_ERROR(sLogger)( "invalid floating number at index %d", n );
        return false;
    }

    // success
    return true;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::UniformValue::loadFromXmlNode( const XmlNode & root )
{
    // check root node
    const XmlElement * e = root.toElement();
    if( 0 == e || "uniform" != e->name )
    {
        GN_ERROR(sLogger)( "root node must be \"<uniform>\"." );
        return false;
    }

    // get uniform type
    const XmlAttrib * a = e->findAttrib( "type" );
    if( !a ) return false; // do nothing if uniform type is unspecified.

    if( "BOOL" == a->value ) type = UVT_BOOL;
    else if( "INT" == a->value ) type = UVT_INT;
    else if( "FLOAT" == a->value ) type = UVT_FLOAT;
    else if( "VECTOR4" == a->value ) type = UVT_VECTOR4;
    else if( "MATRIX44" == a->value ) type = UVT_MATRIX44;
    else
    {
        GN_ERROR(sLogger)( "invalid uniform value type: %s", a->value.cptr() );
        return false;
    }

    // get count
    size_t count = sGetIntAttrib( *e, "count", 0 );
    if( 0 == count ) return false;

    // parse uniform value
    if( e->text.empty() )
    {
        GN_TRACE(sLogger)( "uniform value text is missing!" );
        return false;
    }

    switch( type )
    {
        case UVT_BOOL:
        case UVT_INT :
        case UVT_FLOAT:
            break;

        case UVT_VECTOR4:
        {
            Vector4f v;
            if( !sParseFloats( v, 4, e->text ) ) return false;
            setV( v );
            break;
        }

        case UVT_MATRIX44:
        {
            Matrix44f m;
            if( !sParseFloats( m[0], 16, e->text ) ) return false;
            setM( m );
            break;
        }

        default:
            GN_UNEXPECTED();
            return false;
    }

    // success
    return true;
}
