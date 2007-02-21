#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.scene.Drawable");

//
//
// -----------------------------------------------------------------------------
bool GN::scene::Drawable::loadFromXmlNode( const XmlNode &, const StrA & )
{
    //ResourceManager & rm = ResourceManager::sGetInstance;
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::Drawable::loadFromXmlFile( File & fp, const StrA & basedir )
{
    GN_GUARD;

    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp.name().cptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }
    GN_ASSERT( xpr.root );
    return loadFromXmlNode( *xpr.root, basedir );

    GN_UNGUARD;
}
