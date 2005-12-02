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

        virtual bool deviceCreate() = 0;  //!< Respond to D3D device creation.
        virtual bool deviceRestore() = 0; //!< Respond to D3D device restoration.
        virtual void deviceDispose() = 0; //!< Respond to D3D device dispose(lost).
        virtual void deviceDestroy() = 0; //!< Respond to D3D device deletion.

        //!
        //! Get the renderer that the resource belongs to.
        //!
        D3DRenderer & getRenderer() const { return mRenderer; }

    protected :
        //@{
        D3DResource( D3DRenderer & );
        virtual ~D3DResource();
        //@}

    private:
        D3DRenderer & mRenderer;
    };
}}

// *****************************************************************************
//                           End of d3dResource.h
// *****************************************************************************
#endif // __GN_GFX_D3DRESOURCE_H__
