#ifndef __GN_UTIL_FONT_H__
#define __GN_UTIL_FONT_H__
// *****************************************************************************
/// \file
/// \brief   font utilities
/// \author  chen@@CHENLI-HOMEPC (2007.3.14)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// bitmap image of single character
    ///
    struct FontImage
    {
        /// Font image pixel format
        enum PixelFormat
        {
            GRAYSCALE,  //< gray scale image
            RGBA,       //< RGBA_8_8_8_8 image
        };

        uint32           width;  ///< bitmap width in pixel
        uint32           height; ///< bitmap height in pixel
        const uint8    * buffer; ///< bitmap data
        PixelFormat      format; ///< pixel format

        /// \name per character metrics in unit of pixels,
        ///
        /// X coordinate points RIGHT; Y coordinate points DOWN.
        ///
        /// For Glyph metrics detail, check out http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
        //@{
        float horiBearingX;
        float horiBearingY; // usually negative for horizontal font
        float horiAdvance;

        float vertAdvance;
        //@}
    };

    ///
    /// Font face descriptor
    ///
    struct FontFaceDesc
    {
        ///
        /// font quality
        ///
        enum Quality
        {
            MONOCHROM,          ///< default quality, suitable for small size font.
            ANTIALIASED,        ///< suitable for larger size font.
            NUM_FONT_QUALITIES, ///< number of font quality enumerations
        };

        ///
        /// font file name. Normally would be something like "font::/xxxx"
        ///
        StrA fontname;

        ///
        /// font quality
        ///
        Quality quality;

        /// Metrics that are defined for all glyphs in a given font.
        //@{

        // Bounding box in pixles. X points RIGHT; Y points DOWN.
        //
        // Note that due to glyph hinting, the bounding box might not be exact for certain fonts.
        // Thus they must be treated as unreliable with an error margin of at least one pixel!
        // Indeed, the only way to get the exact metrics is to render all glyphs. As this would
        // be a definite performance hit, it is up to client applications to perform such computations.
        float xmin; //< bounding box left
        float xmax; //< bounding box right
        float ymin; //< bounding box top
        float ymax; //< bounding box bottom

        /// The vertical distance between two consecutive baselines
        float vdistance;

        /// max width in pixels of all the glyphs.
        uint16 maxGlyphWidth() const { return (uint16)ceil(xmax - xmin); }

        /// max height in pixels of all the glyphs.
        uint16 maxGlyphHeight() const { return (uint16)ceil(ymax - ymin); }

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
        virtual const FontFaceDesc & getDesc() const = 0;

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
        /// Font file name. Usually would be something like "font::/xxxx"
        ///
        StrA fontname;

        ///
        /// Character width in pixel.
        ///
        uint16 width;

        ///
        /// Character height in pixel
        ///
        uint16 height;

        ///
        /// font quality
        ///
        FontFaceDesc::Quality quality;
    };

    ///
    /// create font face from a font file
    ///
    GN_API FontFace * createFontFace( const FontFaceCreationDesc & cd );

    ///
    /// Mixed font face creation descriptor
    ///
    struct MixedFontCreationDesc
    {
        wchar_t              firstChar;
        size_t               numChars;
        FontFaceCreationDesc font;
    };

    ///
    /// Create font face with mixed font face
    ///
    GN_API FontFace * createMixedFontFace(
        const FontFaceCreationDesc  & defaultFont,
        const MixedFontCreationDesc * additionalFonts,
        size_t                        numAdditionalFonts );

    ///
    /// create simple ASCII only font with size of 8x16, without external font file dependency.
    ///
    GN_API FontFace * createSimpleAsciiFontFace();

    ///
    /// text descriptor
    ///
    struct TextDesc
    {
        ///
        /// font alignment flags
        ///
        enum TextAlignment
        {
            HORI_LEFT      = 0,

            VERT_TOP       = 0,
            VERT_CENTER    = 1,
            VERT_BASELINE  = 2,
            VERT_BOTTOM    = 3,
        };

        const wchar_t * text;       ///< text string
        size_t          len;        ///< text lengh. Set to 0 for NULL-terminated string.
        float           x;          ///< pen position of the first character
        float           y;          ///< pen position of the first character
        float           z;          ///< pen position of the first character
        uint32          rgba;       ///< text color in R-G-B-A format.
        uint32          alignment;  ///< combination of TextAlignment flags
        bool            background; ///< draw background
        bool            kerning;    ///< enable text kerning
    };

    ///
    /// bitmap font renderer.
    ///
    class GN_API BitmapFont : public StdClass
    {
        GN_DECLARE_STDCLASS( BitmapFont, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BitmapFont() { clear(); }
        virtual ~BitmapFont() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        /// Initialize BitmapFont class
        /// \param sr           Pointer to sprite renderer
        /// \param ff           Pointer to the font face object. Its reference count will be increased by one after this function call.
        /// \param maxchars     Maxinum different characters allowed.
        bool init( gfx::SpriteRenderer * sr, FontFace * ff, size_t maxchars = 4096 );
        void quit();
    private:
        void clear()
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
        /// Get default font pointer. The refcounter of the fount is _NOT_ increased.
        ///
        FontFace * getDefaultFontFace() const { return mFont; }

        ///
        /// draw UNICODE text
        ///
        void drawText( const TextDesc & td );

        ///
        /// draw UNICODE text
        ///
        void drawText( const wchar_t * text, float x, float y )
        {
            TextDesc td;
            td.text = text;
            td.len = 0;
            td.x = x;
            td.y = y;
            td.z = 0;
            td.rgba = 0xFFFFFFFF;
            td.alignment = TextDesc::HORI_LEFT | TextDesc::VERT_TOP;
            td.background = true;
            td.kerning = false;
            drawText( td );
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
            uint8 texidx;

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
            uint16 x, y, w, h;
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

        typedef Dictionary<wchar_t,size_t> SlotMap;

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

        ///
        /// get slot of specific character
        ///
        /// \return Return null, if failed.
        ///
        inline const FontSlot * getSlot( wchar_t ch );

        ///
        /// create slot of specific character
        ///
        /// \return Return null, if failed.
        ///
        const FontSlot * createSlot( wchar_t ch );

        ///
        /// initialize font slots and font textures
        ///
        bool slotInit(
            gfx::Gpu & gpu,
            uint16     fontw,
            uint16     fonth,
            size_t     maxchars );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_FONT_H__
