#include "pch.h"

#define ENABLE_IMDEBUG 0

#if ENABLE_IMDEBUG
#include "imdebug.h"
#pragma comment( lib, "imdebug.lib" )
#endif

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.MixedFont");

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// MixedFont class
// *****************************************************************************

struct FontRange
{
    AutoRef<FontFace>   font;
    wchar_t             firstChar;
    size_t              numChars;

    bool contains( wchar_t ch ) const
    {
        return firstChar <= ch && ch <= (firstChar + numChars);
    }
};

///
/// Font class that using freetype2 library
///
class MixedFontFace : public FontFace
{
    DynaArray<FontRange> mFonts;
    FontFaceDesc         mFontDesc;

    bool failure()
    {
        clear();
        return false;
    }

    void clear()
    {
        mFonts.clear();
    }

public:

    MixedFontFace()
    {
    }

    ~MixedFontFace()
    {
        clear();
    }

    bool init(
        const FontFaceCreationDesc & defaultFont,
        const MixedFontCreationDesc    * additionalFonts,
        size_t                           numAdditionalFonts )
    {
        clear();

        if( additionalFonts != NULL || numAdditionalFonts > 0 )
        {
            for( size_t i = 0; i < numAdditionalFonts; ++i )
            {
                const MixedFontCreationDesc & cd = additionalFonts[i];

                AutoRef<FontFace> ff( createFontFace( cd.font ) );

                if( ff )
                {
                    FontRange fr;
                    fr.font = ff;
                    fr.firstChar = cd.firstChar;
                    fr.numChars = cd.numChars;
                    mFonts.append( fr );
                }
            }
        }

        // default font
        AutoRef<FontFace> df( createFontFace( defaultFont ) );
        if( NULL == df ) return failure();
        FontRange fr;
        fr.font = df;
        fr.firstChar = 0;
        fr.numChars = (size_t)-1;
        mFonts.append( fr );

        // update font descriptor
        for( size_t i = 0; i < mFonts.size(); ++i )
        {
            const FontFaceDesc & ffd = mFonts[i].font->getDesc();

            if( 0 == i )
            {
                mFontDesc = ffd;
            }
            else
            {

                mFontDesc.xmin      = math::getmin( mFontDesc.xmin, ffd.xmin );
                mFontDesc.xmax      = math::getmax( mFontDesc.xmax, ffd.xmax );
                mFontDesc.ymin      = math::getmin( mFontDesc.ymin, ffd.ymin );
                mFontDesc.ymax      = math::getmax( mFontDesc.ymax, ffd.ymax );
                mFontDesc.vdistance = math::getmax( mFontDesc.vdistance, ffd.vdistance );
            }
        }
        mFontDesc.fontname = "mixed font";
        mFontDesc.quality  = (FontFaceDesc::Quality)-1; // TODO: enum

        return true;
    }

    virtual const FontFaceDesc & getDesc() const
    {
        return mFontDesc;
    }

    virtual bool loadFontImage( FontImage & result, wchar_t ch )
    {
        for( size_t i = 0; i < mFonts.size(); ++i )
        {
            const FontRange & fr = mFonts[i];

            if( fr.contains( ch ) )
            {
                return fr.font->loadFontImage( result, ch );
            }
        }

        // should not be here
        GN_UNEXPECTED();
        return false;
    }

    virtual void getKerning( int & dx, int & dy, wchar_t ch1, wchar_t ch2 )
    {
        for( size_t i = 0; i < mFonts.size(); ++i )
        {
            const FontRange & fr = mFonts[i];

            if( fr.contains( ch1 ) && fr.contains( ch2 ) )
            {
                return fr.font->getKerning( dx, dy, ch1, ch2 );
            }
        }

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
GN::gfx::FontFace *
GN::gfx::createMixedFontFace(
    const FontFaceCreationDesc  & defaultFont,
    const MixedFontCreationDesc * additionalFonts,
    size_t                        numAdditionalFonts )
{
    GN_GUARD;

    if( NULL == additionalFonts || 0 == numAdditionalFonts )
    {
        return createFontFace( defaultFont );
    }

    AutoRef<MixedFontFace> font( new MixedFontFace );

    if( !font->init( defaultFont, additionalFonts, numAdditionalFonts ) ) return 0;

    // success
    return font.detach();

    GN_UNGUARD;
}
