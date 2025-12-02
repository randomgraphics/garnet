#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include <png.h>

class ImageTest : public CxxTest::TestSuite {
public:
    void testEmptyImage() {
        using namespace GN;
        using namespace GN::gfx;
        // empty image descriptor is not valid
        TS_ASSERT(!img::PlaneDesc().valid());
    }

    void testImagePitch() {
        using namespace GN;
        using namespace GN::gfx;

        auto id = img::ImageDesc().reset(img::PlaneDesc::make(img::PixelFormat::RGBA8(), {0x4000, 0x4000}));

        TS_ASSERT(id.pitch() == 0x10000);
        TS_ASSERT(id.slice() == 0x40000000);
        TS_ASSERT(id.size == 0x40000000);
    }

    void testConstructMipmap() {
        using namespace GN;
        using namespace GN::gfx;
        auto base = img::PlaneDesc::make(img::PixelFormat::RGBA8(), {5, 7, 9});
        TS_ASSERT(base.valid());
        auto desc = img::ImageDesc().reset(base, 1, 3, 0); // construct full mipmap chain with 3 layers from the base map above.
        TS_ASSERT_EQUALS(5, desc.width());
        TS_ASSERT_EQUALS(7, desc.height());
        TS_ASSERT_EQUALS(9, desc.depth());
        TS_ASSERT_EQUALS(4, desc.levels);
        TS_ASSERT_EQUALS(3, desc.faces);
        TS_ASSERT_EQUALS(12, desc.planes.size());
    }

    void testInvalidPNG() {
        using namespace GN;
        using namespace GN::gfx;
        auto     buf              = std::vector<uint8_t>(10000);
        auto     file             = GN::MemFile(buf.data(), buf.size(), "a.png");
        png_byte png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
        ::memcpy(buf.data(), png_signature, 8);
        auto image = img::Image::load(file.input());
        TS_ASSERT(image.empty());
    }

    void testInvalidJPG() {
        using namespace GN;
        using namespace GN::gfx;
        auto buf   = std::vector<uint8_t>(10000);
        auto file  = GN::MemFile(buf.data(), buf.size(), "a.jpg");
        buf[6]     = 'J';
        buf[7]     = 'F';
        buf[8]     = 'I';
        buf[9]     = 'F';
        auto image = img::Image::load(file.input());
        TS_ASSERT(image.empty());
    }
};
