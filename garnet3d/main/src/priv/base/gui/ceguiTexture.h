#ifndef __GN_GUI_CEGUITEXTURE_H__
#define __GN_GUI_CEGUITEXTURE_H__
// *****************************************************************************
//! \file    gui/ceguiTexture.h
//! \brief   CEGUI texture class
//! \author  chenlee (2006.2.25)
// *****************************************************************************

#ifdef HAS_CEGUI

#include "ceguiHeaders.h    "

namespace CEGUI
{    
    //!
    //! CEGUI Texture adapter 
    //!
    class GarnetTexture : public Texture
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        GarnetTexture( Renderer * r )
            : Texture(r)
            , mTextureHandle(0)
            , mWidth(0), mHeight(0)
            , mMemBuffer(0) {}
        virtual ~GarnetTexture() { destroy(); }
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

        //!
        //! Get internal texture handle
        //!
        uint32_t getHandle() const { return mTextureHandle; }

        // ********************************
        // private variables
        // ********************************
    private:

        static int msNameCounter;

        uint32_t mTextureHandle;
        ushort mWidth;
        ushort mHeight;

        void * mMemBuffer;

        // ********************************
        // private functions
        // ********************************
    private:

        void destroy()
        {
            if( mTextureHandle ) gTexDict.removeResourceHandle(mTextureHandle), mTextureHandle = 0;
            GN::safeMemFree(mMemBuffer);
        }
    };
}

#endif

// *****************************************************************************
//                           End of ceguiTexture.h
// *****************************************************************************
#endif // __GN_GUI_CEGUITEXTURE_H__
