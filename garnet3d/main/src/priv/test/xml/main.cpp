#include "pch.h"
#include "garnet/base/xml.h"

using namespace GN;

bool doParse( XmlProcessor & proc, XmlParseResult & xpr, const char * filename )
{
    AnsiFile fp;

    if( !fp.open( path::toNative(filename), "rt" ) ) return false;

    std::vector<char> buf( fp.size() );

    if( fp.size() != fp.read( &buf[0], fp.size() ) ) return false;

    if( !proc.parseBuffer( xpr, &buf[0], buf.size() ) )
    {
        GN_ERROR( "xml parse error (l:%d,c:%d) : %s", xpr.errLine, xpr.errColumn, xpr.errInfo.cptr() );
        return false;
    }

    // success
    return true;
}

void doPrint( XmlNode * root, int ident )
{
    GN_ASSERT( root );

    for( int i = 0; i < ident; ++i ) printf( "\t" );
    printf( "%s\n", root->name.cptr() );

    if( root->child ) doPrint( root->child, ident + 1 );
    if( root->sibling ) doPrint( root->sibling, ident );
}

int main( int argc, const char * argv[] )
{
    if( argc < 2 )
    {
        GN_INFO( "usage: %s xmlfile", argv[0] );
        return -1;
    }

    XmlProcessor proc;
    XmlParseResult xpr;
    if( doParse( proc, xpr, argv[1] ) ) doPrint( xpr.root, 0 );

    return 0;
}
