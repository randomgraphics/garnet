#include "pch.h"
#include "garnet/GNutil.h"
#include "charBitmap.h"

using namespace GN;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.util.AsciiFont");

// *****************************************************************************
// ascii font face class
// *****************************************************************************

class AsciiFontFace : public FontFace
{
    FontFaceDesc mDesc;
    UInt8        mImage[8*16];

public:

    /// ctor
    AsciiFontFace()
    {
        mDesc.fontname = "ascii_8x16";
        mDesc.quality  = FFQ_MONOCHROM;
        mDesc.width    = 8;
        mDesc.height   = 16;
    }

    virtual const FontFaceDesc & getDesc() const { return mDesc; }

    virtual bool loadFontImage( FontImage & result, wchar_t ch )
    {
        if( ch > 255 )
        {
            GN_ERROR(sLogger)( "this function supports ASCII characters only." );
            return false;
        }

        const BitmapCharDesc * bcd = gBitmapChars8x13[ch];

        std::fill( mImage, mImage+sizeof(mImage), 0 );

        for( size_t y = 0; y < bcd->height; ++y )
        {
            for( size_t x = 0; x < 8; ++x )
            {
                mImage[y*8+x] = 255 * !!( bcd->bitmap[bcd->height-y-1] & (1L<<(7-x)) );
            }
        }

        result.width  = 8;
        result.height = bcd->height;
        result.buffer = mImage;
        result.offx   = -bcd->xorig;
        result.offy   = bcd->yorig;
        result.advx   = bcd->advance;
        result.advy   = 16;

        return true;
    }

    virtual void getKerning( int & dx, int & dy, wchar_t, wchar_t )
    {
        // this is a fixed size font, no kerning.
        dx = 0;
        dy = 0;
    }
};

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::FontFace *
GN::util::createSimpleAsciiFontFace()
{
    return new AsciiFontFace;
}
