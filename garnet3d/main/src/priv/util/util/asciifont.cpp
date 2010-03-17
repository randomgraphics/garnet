#include "pch.h"
#include "garnet/GNutil.h"
#include "charBitmap.h"

using namespace GN;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util.AsciiFont");

// *****************************************************************************
// ascii font face class
// *****************************************************************************

class AsciiFontFace : public FontFace
{
    FontFaceDesc mDesc;
    UInt8        mImage[8*13];

public:

    /// ctor
    AsciiFontFace()
    {
        mDesc.fontname       = "ascii_8x13";
        mDesc.quality        = FFQ_MONOCHROM;
        mDesc.xmin           = 100;
        mDesc.xmax           = -100;
        mDesc.ymin           = 100;
        mDesc.ymax           = -100;
        mDesc.linegap        = 3;

        for( size_t i = 0; i < 256; ++i )
        {
            const BitmapCharDesc * bcd = gBitmapChars8x13[i];

            float xmin = (float)bcd->xorig;
            float xmax = (float)xmin + bcd->width;
            float ymin = (float)bcd->yorig - bcd->height;
            float ymax = (float)bcd->yorig;

            if( xmin < mDesc.xmin ) mDesc.xmin = xmin;
            if( xmax > mDesc.xmax ) mDesc.xmax = xmax;
            if( ymin < mDesc.ymin ) mDesc.ymin = ymin;
            if( ymax > mDesc.ymax ) mDesc.ymax = ymax;
        }
    }

    virtual const FontFaceDesc & getDesc() const { return mDesc; }

    virtual bool loadFontImage( FontImage & result, wchar_t ch )
    {
        if( (unsigned int)ch > 255 )
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
                mImage[(bcd->height-y-1)*8+x] = 255 * !!( bcd->bitmap[y] & (1L<<(7-x)) );
            }
        }

        result.width        = 8;
        result.height       = bcd->height;
        result.buffer       = mImage;
        result.horiBearingX = (float)bcd->xorig;
        result.horiBearingY = (float)bcd->yorig - bcd->height;
        result.horiAdvance  = (float)bcd->advance;
        result.vertAdvance  = mDesc.maxGlyphHeight() + mDesc.linegap;

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
GN::util::CreateSimpleAsciiFontFace()
{
    return new AsciiFontFace;
}
