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

        /// \name character position metrics in pixel
        //@{
        int offx;
        int offy;
        int advx;
        int advy;
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
        /// Maximum single character width in pixel.
        ///
        /// This value can be used to position characters in fixed-width mannter.
        ///
        UInt16 width;

        ///
        /// get character height (minimal line step) in pixel
        ///
        UInt16 height;

        ///
        /// font quality
        ///
        FontFaceQuality quality;
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
    /// create font face (usually loading from a TTF file)
    ///
    FontFace * createFontFace( const FontFaceDesc & desc );

    ///
    /// create simple ASCII only font with size of 8x16, without external font file dependency.
    ///
    FontFace * createSimpleAsciiFontFace();

    ///
    /// text descriptor
    ///
    struct TextDesc
    {
        const wchar_t * text;       ///< text string
        size_t          len;        ///< text lengh. Set to 0 for NULL-terminated string.
        float           x;          ///< position of top-left corner of the first character
        float           y;          ///< position of top-left corner of the first character
        float           z;          ///< position of top-left corner of the first character
        UInt32          rgba;       ///< text color in R-G-B-A format.
        bool            background; ///< draw background
        bool            kerning;    ///< enable text kerning
    };

    ///
    /// bitmap font renderer. Support unicode characters like CJK.
    ///
    class BitmapFont : public StdClass
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
        /// \param sr           Pointer to sprite renderer
        /// \param ff           Pointer to font face object. Its reference count will be increased by one after this function call.
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
        /// Get internal font pointer
        ///
        FontFace * getFontFace() const { return mFont.get(); }

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
            td.background = true;
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
            UInt8 texidx;

            ///
            /// offset value ( in pixel unit )
            ///
            int offx, offy;

            ///
            /// advance value ( in pixel unit )
            ///
            int advx, advy;

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
            MAX_TEXTURES = gfx::RendererContext::MAX_TEXTURES,
        };

        // character information
        struct CharInfo
        {
            const FontSlot * fs;   // font slot of the character
            float            x, y; // screen position of the character
        };

        typedef StackArray<AutoRef<gfx::Texture>,MAX_TEXTURES> TextureArray;

        // graphics resources
        gfx::SpriteRenderer       * mSpriteRenderer;

        // font face data
        AutoRef<FontFace>           mFont;

        // font slot
        FontSlot                  * mFontSlots;
        size_t                      mMaxSlots;
        size_t                      mNumUsedSlots;  // number of used slots
        std::map<wchar_t,size_t>    mSlotMap;       // map that convert charcode to slot index

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
            gfx::Renderer & rndr,
            UInt16          fontw,
            UInt16          fonth,
            size_t          maxchars );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_FONT_H__
