#include "pch.h"
#include "memstress.h"

using namespace GN;
using namespace GN::engine;

class TestLoader : public GN::engine::GraphicsResourceLoader
{
public:

    bool load( const GraphicsResourceDesc &, DynaArray<UInt8> & )
    {
        return true;
    }

    bool decompress( const GraphicsResourceDesc &, DynaArray<UInt8> & , DynaArray<UInt8> & )
    {
        return true;
    }

    bool download( GraphicsResource &, DynaArray<UInt8> & )
    {
        return true;
    }
};

bool TestMemoryStress::init()
{
    const size_t K     = 1024;
    const size_t M     = K * K;
    const size_t COUNT = 64 * K;

    RenderEngine & re = renderEngine();

    StrA name( "test ib" );

    resources.resize( COUNT );
    for( size_t i = 0; i < COUNT; ++i )
    {
        AutoRef<TestLoader> loader( new TestLoader );
        resources[i] = re.createIdxBuf( name, 1*M, loader );
    }

    return true;
}

void TestMemoryStress::quit()
{
    //for( size_t i = 0; i < resources.size(); ++i )
    //{
    //    safeDeleteGraphicsResource( resources[i] );
    //}
}
