#ifndef __GN_SHADOWMAP_SIMPLE_H__
#define __GN_SHADOWMAP_SIMPLE_H__
// *****************************************************************************
//! \file    shadowmap/simple.h
//! \brief   the most primitive shadow map implementation.
//! \author  chen@@CHENLI-HOMEPC (2007.4.21)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/GNscene.h"
#include "garnet/GNutil.h"

using namespace GN::gfx;
using namespace GN::scene;
using namespace GN::util;

namespace GN
{    
    ///
    /// The most primitive shadow map implementation.
    ///
    class SimpleShadowMap : public StdClass
    {
        GN_DECLARE_STDCLASS( SimpleShadowMap, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SimpleShadowMap() : mCamera(RIGHT_HAND) { clear(); mCamera.connectToInput(); }
        virtual ~SimpleShadowMap() { quit(); mCamera.disconnectFromInput(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & actorName );
        void quit();
    private:
        void clear();
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        void update();
        void draw();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        Vector2<UInt32> mShadowMapSize;

        AutoRef<Texture> mShadowMap; // store distance to light

        FirstPersonCamera mCamera;

        Scene mScene;

        Actor * mShadowProjectors;
        Actor * mShadowReceivers;

        // ********************************
        // private functions
        // ********************************
    private:

        bool loadActor( const StrA & name );
    };
};

// *****************************************************************************
//                           End of simple.h
// *****************************************************************************
#endif // __GN_SHADOWMAP_SIMPLE_H__
