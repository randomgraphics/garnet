#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include <png.h>

static UInt8              gBuf[10000];
static GN::MemFile<UInt8> gFile(gBuf,10000,"a.png");

class ImageTest : public CxxTest::TestSuite
{
public:
    void testInvalidPNG()
    {
        png_byte png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
        ::memcpy( gBuf, png_signature, 8 );

        GN::gfx::ImageDesc id;
        GN::gfx::ImageReader ir;

        TS_ASSERT( !ir.readHeader( id ) );
        TS_ASSERT( !ir.readImage( gBuf ) );

        ir.reset( gFile );
        TS_ASSERT( !ir.readHeader( id ) );
        TS_ASSERT( !ir.readImage( gBuf ) );
    }

    void testInvalidJPG()
    {
        gBuf[6] = 'J';
        gBuf[7] = 'F';
        gBuf[8] = 'I';
        gBuf[9] = 'F';

        GN::gfx::ImageDesc id;
        GN::gfx::ImageReader ir;

        gFile.seek( 0, GN::FileSeek::SET );

        ir.reset( gFile );
        TS_ASSERT( !ir.readHeader( id ) );
        TS_ASSERT( !ir.readImage( gBuf ) );
    }
};
