#ifndef __GN_UTIL_FONT_H__
#define __GN_UTIL_FONT_H__
// *****************************************************************************
/// \file
/// \brief   font utilities
/// \author  chen@@CHENLI-HOMEPC (2007.3.14)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace util
{
    ///
    /// bitmap image of single character
    ///
    struct FontImage
    {
        size_t        width;  ///< bitmap width in pixel
        size_t        height; ///< bitmap height in pixel
        const UInt8 * buffer; ///< bitmap data (8 bits gray image)

        /// \name per character metrics in unit of pixels,
        ///       X coordinate points left; Y coordinate points down.
        //@{
        float horiBearingX;
        float horiBearingY;
        float horiAdvance;

        float vertAdvance;
        //@}
    };

    ///
    /// font quality
    ///
    enum FontFaceQuality
    {
        FFQ_MONOCHROM,      ///< default quality, suitable for small size font.
        FFQ_ANTIALIASED,    ///< suitable for larger size font.
        NUM_FONT_QUALITIES, ///< number of font quality enumerations
    };

    ///
    /// Font face descriptor
    ///
    struct FontFaceDesc
    {
        ///
        /// font file name. Normally would be something like "font::/xxxx"
        ///
        StrA fontname;

        ///
        /// font quality
        ///
        FontFaceQuality quality;

        /// Metrics that are defined for all glyphs in a given font.
        //@{

        // bounding box in pixles. X is left; Y is down
        float xmin;
        float xmax;
        float ymin;
        float ymax;

        /// The distance that must be placed between two lines of text
        /// The baseline-to-baseline distance should be computed as
        /// maxGlyphHeight + linegap
        float linegap;

        /// max height in pixels of all the glyphs.
        UInt16 maxGlyphWidth() const { return (UInt16)ceil(xmax - xmin); }

        /// max height in pixels of all the glyphs.
        UInt16 maxGlyphHeight() const { return (UInt16)ceil(ymax - ymin); }

        /// baseline-to-baseline distance of 2 rows of text
        float baseLineDistance() const { return ymax - ymin + linegap; }

        //@}
    };

    ///
    /// abstract font interface
    ///
    struct FontFace : public RefCounter
    {
        ///
        /// get descriptor
        ///
        virtual const FontFaceDesc & GetDesc() const = 0;

        ///
        /// load font data of specific unicode character.
        ///
        /// \note This function will erase previously loaded character (invalidate previous FontImage::buffer)
        ///
        virtual bool loadFontImage( FontImage & result, wchar_t ch ) = 0;

        ///
        /// get font kerning data in pixel
        ///
        virtual void getKerning( int & dx, int & dy, wchar_t ch1, wchar_t ch2 ) = 0;
    };

    ///
    /// Font face creation descriptor
    ///
    struct FontFaceCreationDesc
    {
        ///
        /// font file name. Normally would be something like "font::/xxxx"
        ///
        StrA fontname;

        ///
        /// Character width in pixel.
        ///
        UInt16 width;

        ///
        /// get character height in pixel
        ///
        UInt16 height;

        ///
        /// font quality
        ///
        FontFaceQuality quality;
    };

    ///
    /// create font face (usually loading from a TTF file)
    ///
    FontFace * CreateFontFace( const FontFaceCreationDesc & cd );

    ///
    /// create simple ASCII only font with size of 8x16, without external font file dependency.
    ///
    FontFace * CreateSimpleAsciiFontFace();

    ///
    /// font alignment flags
    ///
    struct TextAlignment
    {
        enum ENUM
        {
            HORI_LEFT      = 0,

            VERT_TOP       = 0,
            VERT_CENTER    = 1,
            VERT_BASELINE  = 2,
            VERT_BOTTOM    = 3,
        };
    };

    ///
    /// text descriptor
    ///
    struct TextDesc
    {
        const wchar_t * text;       ///< text string
        size_t          len;        ///< text lengh. Set to 0 for NULL-terminated string.
        float           x;          ///< pen position of the first character
        float           y;          ///< pen position of the first character
        float           z;          ///< pen position of the first character
        UInt32          rgba;       ///< text color in R-G-B-A format.
        UInt32          alignment;  ///< combination of TextAlignment flags
        bool            background; ///< draw background
        bool            kerning;    ///< enable text kerning
    };

    ///
    /// bitmap font renderer.
    ///
    class BitmapFont : public StdClass
    {
        GN_DECLARE_STDCLASS( BitmapFont, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BitmapFont() { Clear(); }
        virtual ~BitmapFont() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        /// \param sr           Pointer to sprite renderer
        /// \param ff           Pointer to font face object. Its reference count will be increased by one after this function call.
        /// \param maxchars     Maxinum different characters allowed.
        bool Init( gfx::SpriteRenderer * sr, FontFace * ff, size_t maxchars = 4096 );
        void Quit();
    private:
        void Clear()
        {
            mFontSlots = NULL;
            for( size_t i = 0; i < MAX_TEXTURES; ++i )
            {
                mCharList[i] = NULL;
            }
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Get internal font pointer
        ///
        FontFace * GetFontFace() const { return mFont.Get(); }

        ///
        /// draw UNICODE text
        ///
        void DrawText( const TextDesc & td );

        ///
        /// draw UNICODE text
        ///
        void DrawText( const wchar_t * text, float x, float y )
        {
            TextDesc td;
            td.text = text;
            td.len = 0;
            td.x = x;
            td.y = y;
            td.z = 0;
            td.rgba = 0xFFFFFFFF;
            td.alignment = TextAlignment::HORI_LEFT | TextAlignment::VERT_TOP;
            td.background = true;
            td.kerning = false;
            DrawText( td );
        }

        // ********************************
        // private variables
        // ********************************
    private:

        ///
        /// font slot structure.
        ///
        /// "font slot" is a square area inside the font texture that
        /// representing a single character's image.
        ///
        struct FontSlot
        {
            ///
            /// character code
            ///
            wchar_t ch;

            ///
            /// texture index
            ///
            UInt8 texidx;

            ///
            /// offset value ( in pixel unit )
            ///
            float offx, offy;

            ///
            /// advance value ( in pixel unit )
            ///
            float advx, advy;

            ///
            /// texture coord square ( in texture unit )
            ///
            float u, v, tw, th;

            ///
            /// texture square( in pixel unit )
            ///
            UInt16 x, y, w, h;
        };

        // private constants
        enum
        {
            MAX_TEXTURES = gfx::GpuContext::MAX_TEXTURES,
        };

        // character information
        struct CharInfo
        {
            const FontSlot * fs;   // font slot of the character
            float            x, y; // screen position of the character
        };

        typedef StackArray<AutoRef<gfx::Texture>,MAX_TEXTURES> TextureArray;

        typedef HashMap<wchar_t,size_t> SlotMap;

        // graphics resources
        gfx::SpriteRenderer       * mSpriteRenderer;

        // font face data
        AutoRef<FontFace>           mFont;

        // font slot
        FontSlot                  * mFontSlots;
        size_t                      mMaxSlots;
        size_t                      mNumUsedSlots;  // number of used slots

        SlotMap                     mSlotMap;       // map that convert charcode to slot index

        // font textures
        TextureArray                mTextures;

        // character list
        CharInfo                  * mCharList[MAX_TEXTURES];
        size_t                      mNumChars[MAX_TEXTURES];

        // ********************************
        // private functions
        // ********************************
    private:

        static inline UInt64 sWcharHash( const wchar_t & ch ) { return (UInt64)ch; }
        static inline bool sWcharEqual( const wchar_t & a, const wchar_t & b ) { return a == b; }

        ///
        /// get slot of specific character
        ///
        /// \return Return null, if failed.
        ///
        inline const FontSlot * GetSlot( wchar_t ch );

        ///
        /// create slot of specific character
        ///
        /// \return Return null, if failed.
        ///
        const FontSlot * CreateSlot( wchar_t ch );

        ///
        /// initialize font slots and font textures
        ///
        bool SlotInit(
            gfx::Gpu & gpu,
            UInt16     fontw,
            UInt16     fonth,
            size_t     maxchars );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_FONT_H__
