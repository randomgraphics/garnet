#ifndef __GN_SCENE_FONT_H__
#define __GN_SCENE_FONT_H__
// *****************************************************************************
/// \file    scene/font.h
/// \brief   font classes
/// \author  chen@@CHENLI-HOMEPC (2007.3.14)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// Bitmap ASCII font renderer
    ///
    class AsciiFont
    {
    public:

        //@{

        AsciiFont();
        ~AsciiFont();

        //@}

        ///
        /// Draw ASCII string. [0,0] is left top corner of the screen.
        ///
        void drawText( const char * text, int x, int y, UInt32 bgra = 0xFFFFFFFF );

        // ********************************
        // private variables
        // ********************************
    private:

        AutoRef<gfx::Texture> mTexture;

        // ********************************
        // private functions
        // ********************************
    private:

        bool rendererRestore();
        void rendererDispose() { mTexture.clear(); }
    };

    ///
    /// global ascii font instance
    ///
    extern AsciiFont gAsciiFont;

    ///
    /// bitmap image of single character
    ///
    struct FontBitmap
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
        UInt32 width;

        ///
        /// get character height (minimal line step) in pixel
        ///
        UInt32 height;
    };

    ///
    /// font quality
    ///
    enum FontFaceQuality
    {
        FFQ_MONOCHROM,   // default quality, suitable for small size font.
        FFQ_ANTIALIASED, // suitable for larger size font.
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
        /// \note This function will erase previously loaded character (invalidate previous FontBitmap::buffer)
        ///
        virtual bool loadFontBitmap(
            FontBitmap & result,
            wchar_t ch,
            FontFaceQuality quality = FFQ_MONOCHROM ) = 0;

        ///
        /// get font kerning data in pixel
        ///
        virtual void getKerning( int & dx, int & dy, wchar_t ch1, wchar_t ch2 ) = 0;
    };

    ///
    /// create font face
    ///
    FontFace * createFont( const FontFaceDesc & desc );

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
        UInt32          bgra;       ///< text color in B-G-R-A format.
        bool            background; ///< draw background
        bool            kerning;    ///< enable text kerning
    };

    ///
    /// Bitmap font renderer
    ///
    class BitmapFontRenderer : public StdClass
    {
        GN_DECLARE_STDCLASS( BitmapFontRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BitmapFontRenderer()          { clear(); }
        virtual ~BitmapFontRenderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const FontFaceDesc & );
        void quit();
    private:
        void clear()
        {
            mNumTextures = 0;
            mFontSlots = 0;
            mFont.clear();
            for ( size_t i = 0; i < MAX_TEXTURES; ++i )
            {
                mNumChars[i] = 0;
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
            td.bgra = 0xFFFFFFFF;
            td.background = true;
            drawText( td );
        }

        ///
        /// purge font cache (release all textures, clear slot map)
        ///
        void purge();

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
            /// padding fields, make structure 32-bit aligned
            ///
            UInt8 reserved[2];

            ///
            /// texture index
            ///
            size_t texidx;

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
            float u1, v1, u2, v2;

            ///
            /// texture square( in pixel unit )
            ///
            int x1, y1, x2, y2;
        };

        // private constants
        enum
        {
            MAX_TEXTURES    = 16,
            MAX_SLOTS       = 16 * 16 * MAX_TEXTURES,
            MAX_TEXT_LENGTH = MAX_SLOTS,
        };

        // character information
        struct CharInfo
        {
            const FontSlot * fs;
            float          x, y;
        };

        // font face data
        AutoRef<FontFace>       mFont;

        // font texture list
        AutoRef<gfx::Texture>   mTextures[MAX_TEXTURES];
        size_t                  mNumTextures;

        // texture size
        size_t                  mTexWidth;
        size_t                  mTexHeight;

        // font slot
        size_t                  mNumSlots; // number of used slots
        FontSlot              * mFontSlots;
        std::map<wchar_t,int>   mSlotMap;  // map that convert charcode to slot index

        // texture list
        CharInfo                mCharList[MAX_TEXTURES][MAX_TEXT_LENGTH];
        size_t                  mNumChars[MAX_TEXTURES];

        // ********************************
        // private functions
        // ********************************
    private:

        ///
        /// get slot of specific character
        ///
        /// \return Return null, if failed.
        ///
        const FontSlot * getSlot( wchar_t ch )
        {
            // find font slot in slotmap
            std::map<wchar_t,int>::iterator iter = mSlotMap.find(ch);
            if( iter != mSlotMap.end() )
            {
                // found!
                return &mFontSlots[iter->second];
            }

            return createSlot( ch );
        }

        ///
        /// create slot of specific character
        ///
        /// \return Return null, if failed.
        ///
        const FontSlot * createSlot( wchar_t ch );

        ///
        /// initialize font slots.
        ///
        bool slotInit( size_t fontw, size_t fonth );
    };
}}

// *****************************************************************************
//                           End of font.h
// *****************************************************************************
#endif // __GN_SCENE_FONT_H__
