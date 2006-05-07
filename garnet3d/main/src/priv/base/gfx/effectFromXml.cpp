#include "pch.h"
#include "garnet/gfx/effect.h"
#include "garnet/base/xml.h"

// *****************************************************************************
// local functions
// *****************************************************************************

using namespace GN;
using namespace GN::gfx;

//
//
// -----------------------------------------------------------------------------
static bool sDoParse( EffectDesc & desc, XmlNode * root )
{
    desc.clear();

    if( 0 == root ) return true; // empty effect

    XmlElement * e = root->toElement();

    if( 0 == e ||e->name != "effect" )
    {
        GN_ERROR( "root node must be \"<effect>\"." );
        return false;
    }

    return true;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectDesc::fromXml( const char * str, size_t size )
{
    GN_GUARD;

    XmlDocument doc;

    XmlParseResult xpr;

    if( !doc.parse( xpr, str, size ) )
    {
        GN_ERROR(
            "Fail to read XML string:\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }

    return sDoParse( *this, xpr.root );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectDesc::fromXml( File & fp )
{
    GN_GUARD;

    XmlDocument doc;

    XmlParseResult xpr;

    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(
            "Fail to read XML file %s:\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp.name().cptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }

    return sDoParse( *this, xpr.root );

    GN_UNGUARD;
}


