#ifndef __GN_TESTGFX2_FONT_H__
#define __GN_TESTGFX2_FONT_H__
// *****************************************************************************
/// \file
/// \brief   font class using quad kernel
/// \author  chen@@CHENLI-HOMEPC (2007.7.7)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/GNscene.h"

using namespace GN::gfx;
using namespace GN::scene;

namespace GN { namespace test
{
    ///
    /// font renderer using quad kernel
    ///
    class QuadKernelFont : public StdClass
    {
        GN_DECLARE_STDCLASS( QuadKernelFont, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        QuadKernelFont( GraphicsSystem & gs ) : mGfxSys(gs) { clear(); }
        virtual ~QuadKernelFont() { quit(); }
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
                mCharList[i]       = 0;
                mNumChars[i]       = 0;
                mKernelPortBindings[i] = 0;
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

        struct FontTexture
        {
            Surface        * texture;
            DynaArray<UInt8> syscopy;

            FontTexture() : texture(0) {}
        };

        GraphicsSystem            & mGfxSys;

        Kernel                    * mKernel;
        StreamSource              * mStream;
        KernelParameterSet        * mKernelParam;
        KernelPortBinding         * mKernelPortBindings[MAX_TEXTURES];

        // font face data
        AutoRef<FontFace>           mFont;

        // font texture list
        FontTexture                 mTextures[MAX_TEXTURES];
        size_t                      mNumTextures;

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
#endif // __GN_TESTGFX2_FONT_H__
