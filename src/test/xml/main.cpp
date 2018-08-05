#include "pch.h"
#include "garnet/base/xml.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.test.xml");

bool doParse( XmlDocument & doc, XmlParseResult & xpr, const char * filename )
{
    DiskFile fp;

    if( !fp.open( fs::toNativeDiskFilePath(filename), "rt" ) ) return false;

    if( !doc.parse( xpr, fp ) )
    {
        GN_ERROR(sLogger)( "xml parse error (l:%d,c:%d) : %s", xpr.errLine, xpr.errColumn, xpr.errInfo.rawptr() );
        return false;
    }

    // success
    return true;
}

int main( int argc, const char * argv[] )
{
    if( argc < 2 )
    {
        GN_INFO(sLogger)( "usage: %s xmlfile", argv[0] );
        return -1;
    }

    XmlDocument doc;
    XmlParseResult xpr;
    if( doParse( doc, xpr, argv[1] ) )
    {
        StdFile fp(stdout);
        doc.writeToFile( fp, *xpr.root, 0 );
    }

    return 0;
}
