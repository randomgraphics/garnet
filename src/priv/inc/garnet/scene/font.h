#ifndef __GN_SCENE_FONT_H__
#define __GN_SCENE_FONT_H__
// *****************************************************************************
//! \file    scene/font.h
//! \brief   font classes
//! \author  chen@@CHENLI-HOMEPC (2007.3.14)
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
}}

// *****************************************************************************
//                           End of font.h
// *****************************************************************************
#endif // __GN_SCENE_FONT_H__
