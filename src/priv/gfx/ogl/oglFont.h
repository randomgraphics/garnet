#ifndef __GN_GFXOGL_OGLFONT_H__
#define __GN_GFXOGL_OGLFONT_H__
// *****************************************************************************
//! \file    ogl/oglFont.h
//! \brief   OGL font renderer
//! \author  chenlee (2005.12.8)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLRenderer;

    //!
    //! OGL Font renderer.
    //!
    class OGLFont : public StdClass
    {
         GN_DECLARE_STDCLASS( OGLFont, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLFont( OGLRenderer & r ) : mRenderer(r) { clear(); }
        virtual ~OGLFont() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mFontMap.clear();
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Draw unicode string
        //!
        //! \sa Renderer::drawTextW()
        //!
        void drawTextW( const wchar_t * s, int x, int y, const Vector4f & c );

        // ********************************
        // private variables
        // ********************************
    private:

        // font parameters
        struct CharDesc
        {
            GLuint displayList; // display list use to display the char
            int    advanceX;    // Advance on X direction.
        };
        typedef std::map<wchar_t,CharDesc> FontMap;

        OGLRenderer & mRenderer;
        FontMap       mFontMap;
        int           mFontHeight;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createFont();
        void deleteFont();
        bool charInit( wchar_t, CharDesc & );
        int  drawChar( wchar_t ); //!< Return x-advance of the character

#if GN_POSIX
        int      getFontBitmapAdvance( char ch );
        uint32_t getFontBitmapHeight();
        void     drawFontBitmap( char ch );
#endif
    };
}}

// *****************************************************************************
//                           End of oglFont.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLFONT_H__
