#include "pch.h"
#include <pcrecpp.h>

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.test.pcre");

int main( int argc, const char * argv[] )
{
    if( argc < 3 )
    {
        GN_INFO(sLogger)( "usage: %s <pattern> <string>", argv[0] );
        return -1;
    }

    const char * pattern = argv[1];
    const char * buffer = argv[2];

    // compile regex
    pcrecpp::RE re(pattern);
    const std::string & err = re.error();
    if( !err.empty() )
    {
        GN_ERROR(sLogger)( err.c_str() );
        return -1;
    }

    // get substring count
    int n = re.NumberOfCapturingGroups();
    printf( "%d substrings in total\n", n );
    GN_ASSERT( n >= 0 );
    if( 0 == n ) return 0;

    // build argument array
    DynaArray<std::string> results( (size_t)n );
    DynaArray<pcrecpp::Arg*> args( (size_t)n );
    for( int i = 0; i < n; ++i )
    {
        args[i] = new pcrecpp::Arg(&results[i]);
    }

    // do parse
    int consumed;
    while( re.DoMatch( buffer, pcrecpp::RE::UNANCHORED, &consumed, &args[0], n ) )
    {
        printf( "Consume %d characters\n", consumed );
        for( int i = 0; i < n; ++i )
        {
            printf( "[%02d] : %s\n", i, results[i].c_str() );
        }
        buffer += consumed;
    }

    // release argument array
    for( int i = 0; i < n; ++i ) delete args[i];

    return 0;
}
