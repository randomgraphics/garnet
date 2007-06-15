#ifndef __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
#define __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
// *****************************************************************************
//! \file    gfx2/d3d9GraphicsSystem.h
//! \brief   d3d9 graphics system interface
//! \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// D3D9 graphics system descriptor
    ///
    struct D3D9GraphicsSystemDesc : public GraphicSystemDesc
    {
    };

    ///
    /// D3D9 graphics system
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9GraphicsSystem : public BaseGraphicsSystem
    {
        GN_DECLARE_STDCLASS( D3D9GraphicsSystem, BaseGraphicsSystem );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9GraphicsSystem()          { clear(); }
        virtual ~D3D9GraphicsSystem() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GraphicsSystemCreationParameter & );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // from GraphicsSystem
        // ********************************
    public:

        //@{

        virtual const GraphicSystemDesc & getDesc() const { return mDesc; }
        virtual Surface * createSurface( const SurfaceCreationParameter & scp ) { GN_UNUSED_PARAM(scp); return 0; }

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        D3D9GraphicsSystemDesc mDesc;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of d3d9GraphicsSystem.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
