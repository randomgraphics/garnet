#include "pch.h"
#include <pcrecpp.h>

using namespace GN;

int main( int argc, const char * argv[] )
{
    if( argc < 3 )
    {
        GN_INFO( "usage: %s <pattern> <string>", argv[0] );
        return -1;
    }

    const char * pattern = argv[1];
    const char * buffer = argv[2];

    // compile regex
    pcrecpp::RE re(pattern);
    const std::string & err = re.error();
    if( !err.empty() )
    {
        GN_ERROR( err.c_str() );
        return -1;
    }

    // get substring count
    int n = re.NumberOfCapturingGroups();
    printf( "%d substrings in total\n", n );
    GN_ASSERT( n >= 0 );
    if( 0 == n ) return 0;

    // build argument array
    std::vector<std::string> results;
    std::vector<pcrecpp::Arg*> args;
    results.resize( (size_t)n );
    args.resize( n );
    for( int i = 0; i < n; ++i )
    {
        args[i] = new pcrecpp::Arg(&results[i]);
    }

    // do parse
    int consumed;
    if( !re.DoMatch( buffer, pcrecpp::RE::UNANCHORED, &consumed, &args[0], n ) )
    {
        GN_ERROR( "pcrecpp::RE::DoMatch() failed" );
        return -1;
    }

    // release arggument array
    for( int i = 0; i < n; ++i ) delete args[i];

    // print result
    printf( "matched %d substrings\n", consumed );
    for( int i = 0; i < consumed; ++i )
    {
        printf( "[%02d] : %s\n", i, results[i].c_str() );
    }

    return 0;
}
