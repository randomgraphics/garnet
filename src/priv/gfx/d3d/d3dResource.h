#ifndef __GN_GFX_D3DRESOURCE_H__
#define __GN_GFX_D3DRESOURCE_H__
// *****************************************************************************
//! \file    d3dResource.h
//! \brief   
//! \author  chenlee (2005.10.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    class D3DRenderer;

    //!
    //! General D3D resource class
    //!
    class D3DResource
    {
    public:

        //!
        //! Resource state
        //!
        enum ResourceState
        {
            UNINITIALIZED, //!< Resource is uninitialized.
            CREATED,       //!< Resource is created, but not restored.
            READY,         //!< Resource is ready to use (created and restored).
        };

        //!
        //! Get the renderer that the resource belongs to.
        //!
        D3DRenderer & getRenderer() const { return mRenderer; }

        //!
        //! Switch resouce to specific state
        //!
        bool switchState( ResourceState );

        //!
        //! Is resource ready to use?
        //!
        bool readyToUse() const { return READY == mState; }

        virtual bool deviceCreate();  //!< Respond to D3D device creation.
        virtual bool deviceRestore(); //!< Respond to D3D device restoration.
        virtual void deviceDispose(); //!< Respond to D3D device dispose(lost).
        virtual void deviceDestroy(); //!< Respond to D3D device deletion.

    protected :

        D3DResource( D3DRenderer & ); //!< ctor
        virtual ~D3DResource();       //!< dtor

    private:

        D3DRenderer & mRenderer;
        ResourceState mState; 
    };
}}

// *****************************************************************************
//                           End of d3dResource.h
// *****************************************************************************
#endif // __GN_GFX_D3DRESOURCE_H__
