#ifndef __GN_GFXD3D9_D3D9RESOURCE_H__
#define __GN_GFXD3D9_D3D9RESOURCE_H__
// *****************************************************************************
//! \file    d3d9Resource.h
//! \brief   
//! \author  chenlee (2005.10.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    class D3D9Renderer;

    //!
    //! General D3D9 resource class
    //!
    class D3D9Resource
    {
    public:

        //!
        //! Get the renderer that the resource belongs to.
        //!
        D3D9Renderer & getRenderer() const { return mRenderer; }

        virtual bool deviceCreate() = 0;  //!< Respond to D3D device creation.
        virtual bool deviceRestore() = 0; //!< Respond to D3D device restoration.
        virtual void deviceDispose() = 0; //!< Respond to D3D device dispose(lost).
        virtual void deviceDestroy() = 0; //!< Respond to D3D device deletion.

    protected :

        D3D9Resource( D3D9Renderer & ); //!< ctor
        virtual ~D3D9Resource();       //!< dtor

    private:

        D3D9Renderer & mRenderer;
    };
}}

// *****************************************************************************
//                           End of d3d9Resource.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9RESOURCE_H__
