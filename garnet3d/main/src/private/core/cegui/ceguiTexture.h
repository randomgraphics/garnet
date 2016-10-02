#ifndef __GN_GUI_CEGUITEXTURE_H__
#define __GN_GUI_CEGUITEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   CEGUI texture class
/// \author  chenlee (2006.2.25)
// *****************************************************************************

#ifdef HAS_CEGUI

#include "ceguiHeaders.h"

namespace CEGUI
{
    ///
    /// CEGUI Texture adapter
    ///
    class GarnetTexture : public Texture
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        GarnetTexture( Renderer * r )
            : Texture(r)
            , mWidth(0), mHeight(0)
            , mMemBuffer(0) {}
        virtual ~GarnetTexture() { dispose(); GN::SafeHeapDealloc(mMemBuffer); }
        //@}

        // ********************************
        // from CECUI::Texture
        // ********************************
    public:

    	virtual	ushort  getWidth(void) const { return mWidth; }
    	virtual	ushort  getHeight(void) const { return mHeight; }
    	virtual void    loadFromFile(const String& filename, const String& resourceGroup);
    	virtual void    loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight);

        // ********************************
        // public functions
        // ********************************

   public:

        ///
        /// reload the content
        ///
        bool reload();

        ///
        /// dispose the content
        ///
        void dispose() { mGarnetTexture.clear(); }

        ///
        /// Get internal texture handle
        ///
        GN::gfx::Texture * getGarnetTexture() const { return mGarnetTexture; }

        // ********************************
        // private variables
        // ********************************
    private:

        GN::AutoRef<GN::gfx::Texture> mGarnetTexture;
        ushort mWidth;
        ushort mHeight;

        String mFileName;
        String mGroup;
        void * mMemBuffer;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GUI_CEGUITEXTURE_H__
