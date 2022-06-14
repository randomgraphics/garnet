#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include <png.h>

static uint8_t            gBuf[10000];
static GN::MemFile<uint8> gFile(gBuf, 10000, "a.png");

class ImageTest : public CxxTest::TestSuite {
public:
    void testEmptyImage() {
        using namespace GN;
        using namespace GN::gfx;
        // empty image descriptor is not valid
        TS_ASSERT(!ImagePlaneDesc().valid());
    }

    void testImagePitch() {
        using namespace GN;
        using namespace GN::gfx;

        auto id = ImageDesc(ImagePlaneDesc::make(ColorFormat::RGBA8, 0x4000, 0x4000));

        TS_ASSERT(id.pitch() == 0x10000);
        TS_ASSERT(id.slice() == 0x40000000);
        TS_ASSERT(id.size == 0x40000000);
    }

    void testConstructMipmap() {
        using namespace GN;
        using namespace GN::gfx;
        auto base = ImagePlaneDesc::make(ColorFormat::RGBA8, 5, 7, 9);
        TS_ASSERT(base.valid());
        auto desc = ImageDesc(base, 3, 0); // construct full mipmap chain with 3 layers from the base map above.
        TS_ASSERT(5 == desc.width());
        TS_ASSERT(7 == desc.height());
        TS_ASSERT(9 == desc.depth());
        TS_ASSERT(4 == desc.levels);
        TS_ASSERT(3 == desc.layers);
        TS_ASSERT(12 == desc.planes.size());
    }

    void testInvalidPNG() {
        using namespace GN;
        using namespace GN::gfx;
        png_byte png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
        ::memcpy(gBuf, png_signature, 8);
        auto image = RawImage::load(gFile);
        TS_ASSERT(image.empty());
    }

    void testInvalidJPG() {
        using namespace GN;
        using namespace GN::gfx;
        gBuf[6]    = 'J';
        gBuf[7]    = 'F';
        gBuf[8]    = 'I';
        gBuf[9]    = 'F';
        auto image = RawImage::load(gFile);
        TS_ASSERT(image.empty());
    }
};
