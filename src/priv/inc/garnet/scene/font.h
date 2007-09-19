#ifndef __GN_SCENE_FONT_H__
#define __GN_SCENE_FONT_H__
// *****************************************************************************
/// \file
/// \brief   font classes
/// \author  chen@@CHENLI-HOMEPC (2007.3.14)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// Bitmap ASCII font renderer
    ///
    class AsciiFont : public StdClass
    {
        GN_DECLARE_STDCLASS( AsciiFont, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        AsciiFont( engine::RenderEngine & re ) : mRenderEngine(re) { clear(); }
        virtual ~AsciiFont() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() { mKernel = 0; mKernelParam = 0; mKernelPortBinding = 0; mTexture = 0; mContext = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Draw ASCII string. [0,0] is left top corner of the screen.
        ///
        void drawText( const char * text, int x, int y, UInt32 rgba = 0xFFFFFFFF );

        // ********************************
        // private variables
        // ********************************
    private:

        engine::RenderEngine     & mRenderEngine;
        engine::GraphicsResource * mKernel;
        engine::GraphicsResource * mKernelParam;
        engine::GraphicsResource * mKernelPortBinding;
        engine::GraphicsResource * mTexture;
        UIntPtr                     mContext;

        // ********************************
        // private functions
        // ********************************
    private:
    };

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
        /// \note This function will erase previously loaded character (invalidate previous FontImage::buffer)
        ///
        virtual bool loadFontImage(
            FontImage & result,
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
        UInt32          rgba;       ///< text color in R-G-B-A format.
        bool            background; ///< draw background
        bool            kerning;    ///< enable text kerning
    };

    ///
    /// bitmap font renderer
    ///
    class BitmapFont : public StdClass
    {
        GN_DECLARE_STDCLASS( BitmapFont, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BitmapFont( engine::RenderEngine & re ) : mRenderEngine(re) { clear(); }
        virtual ~BitmapFont() { quit(); }
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
            mKernel = 0;
            mKernelParam = 0;
            mNumTextures = 0;
            mFontSlots = 0;
            mFont.clear();
            for( size_t i = 0; i < MAX_TEXTURES; ++i )
            {
                mKernelPortBindings[i] = 0;
                mTextures[i]           = 0;
                mDrawables[i]          = 0;
                mCharList[i]           = 0;
                mNumChars[i]           = 0;
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

        struct QuadVertex
        {
            //@{

            float   x, y, z;    ///< position in normalized screen space, [0,0] is left-top corner, (1,1) is right-bottom corner
            UInt8   r, g, b, a; ///< vertex color
            float   u, v;       ///< texture coordinates
            UInt32  _[2];       ///< padding to 32 bytes

            void set( float x_, float y_, float z_, UInt8 r_, UInt8 g_, UInt8 b_, UInt8 a_, float u_, float v_ )
            {
                x = x_;
                y = y_;
                z = z_;
                r = r_;
                g = g_;
                b = b_;
                a = a_;
                u = u_;
                v = v_;
            }

            //@}
        };
        GN_CASSERT( 32 == sizeof(QuadVertex) );

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
            float u1, v1, u2, v2;

            ///
            /// texture square( in pixel unit )
            ///
            UInt16 x1, y1, x2, y2;
        };

        class FontTextureLoader : public engine::GraphicsResourceLoader
        {
            DynaArray<UInt8>           mFontImage;
            size_t                     mFontWidth, mFontHeight;
            const FontSlot             mSlot;
            engine::GraphicsResource & mTexture;

        public:

            FontTextureLoader( const FontImage & font, const FontSlot & slot, engine::GraphicsResource & tex );

            virtual bool load( const engine::GraphicsResourceDesc & desc, DynaArray<UInt8> & outbuf );
            virtual bool decompress( const engine::GraphicsResourceDesc & desc, DynaArray<UInt8> & outbuf, DynaArray<UInt8> & inbuf );
            virtual bool download( engine::GraphicsResource & res, DynaArray<UInt8> & inbuf );
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
            float            x, y;
        };

        struct FontTexture
        {
            engine::GraphicsResource * texture;
            FontTexture() : texture(0) {}
        };

        engine::RenderEngine      & mRenderEngine;

        engine::GraphicsResource  * mKernel;
        engine::GraphicsResource  * mKernelParam;
        engine::GraphicsResource  * mKernelPortBindings[MAX_TEXTURES];
        engine::GraphicsResource  * mTextures[MAX_TEXTURES];
        engine::Drawable            mDrawables[MAX_TEXTURES];
        size_t                      mNumTextures;

        // font face data
        AutoRef<FontFace>           mFont;

        // texture size
        size_t                      mTexWidth;
        size_t                      mTexHeight;

        // font slot
        size_t                      mNumSlots; // number of used slots
        FontSlot                  * mFontSlots;
        std::map<wchar_t,size_t>    mSlotMap;  // map that convert charcode to slot index

        // texture list
        CharInfo                  * mCharList[MAX_TEXTURES];
        size_t                      mNumChars[MAX_TEXTURES];

        DynaArray<QuadVertex>       mQuadBuffer;

        // ********************************
        // private functions
        // ********************************
    private:

        ///
        /// delete all font slots. Called when texture resources are disposed.
        ///
        void deleteAllSlots( engine::GraphicsResource * );

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
        /// initialize font slots.
        ///
        bool slotInit( UInt16 fontw, UInt16 fonth );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_FONT_H__
