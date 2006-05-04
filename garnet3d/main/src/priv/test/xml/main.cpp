#include "pch.h"
#include "garnet/base/xml.h"

using namespace GN;

bool doParse( XmlDocument & doc, XmlParseResult & xpr, const char * filename )
{
    AnsiFile fp;

    if( !fp.open( path::toNative(filename), "rt" ) ) return false;

    std::vector<char> buf( fp.size() );

    size_t sz = fp.read( &buf[0], fp.size() );
    if( !doc.parseBuffer( xpr, &buf[0], sz ) )
    {
        GN_ERROR( "xml parse error (l:%d,c:%d) : %s", xpr.errLine, xpr.errColumn, xpr.errInfo.cptr() );
        return false;
    }

    // success
    return true;
}

int main( int argc, const char * argv[] )
{
    if( argc < 2 )
    {
        GN_INFO( "usage: %s xmlfile", argv[0] );
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
