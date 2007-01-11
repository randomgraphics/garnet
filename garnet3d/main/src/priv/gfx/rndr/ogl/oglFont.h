#ifndef __GN_GFXOGL_OGLFONT_H__
#define __GN_GFXOGL_OGLFONT_H__
// *****************************************************************************
/// \file    ogl/oglFont.h
/// \brief   OGL font renderer
/// \author  chenlee (2005.12.8)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLRenderer;

    ///
    /// OGL Font renderer.
    ///
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
    private:
        void clear()
        {
            mDisplayLists = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Draw unicode string
        ///
        /// \sa Renderer::drawDebugText()
        ///
        void drawText( const char * s, int x, int y, const Vector4f & c );

        // ********************************
        // private variables
        // ********************************
    private:

        OGLRenderer & mRenderer;
        GLuint        mDisplayLists; // display list for all characters (256 items)

        // ********************************
        // private functions
        // ********************************
    private:

        bool createFont();
        void deleteFont();
    };
}}

// *****************************************************************************
//                           End of oglFont.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLFONT_H__
