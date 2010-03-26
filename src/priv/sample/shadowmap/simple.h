#ifndef __GN_SHADOWMAP_SIMPLE_H__
#define __GN_SHADOWMAP_SIMPLE_H__
// *****************************************************************************
/// \file
/// \brief   the most primitive shadow map implementation.
/// \author  chen@@CHENLI-HOMEPC (2007.4.21)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/GNutil.h"

using namespace GN::gfx;
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
        SimpleShadowMap( SampleApp & app )
            : mApp(app)
            , mCamera(RIGHT_HAND)
            , mScene( app.getEntityManager(), app.getRenderEngine() )
        { Clear(); mCamera.connectToInput(); }
        virtual ~SimpleShadowMap() { Quit(); mCamera.disconnectFromInput(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const StrA & actorName );
        void Quit();
    private:
        void Clear();
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        void Update();
        void Draw();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        SampleApp & mApp;

        GraphicsResource * mColorMap;
        GraphicsResource * mShadowMap;

        FirstPersonCamera mCamera;

        Matrix44f mProj;
        Matrix44f mLightProj;
        Matrix44f mLightView;

        Scene mScene;

        Actor * mShadowProjectors;
        Actor * mShadowReceivers;

        DrawContext mCtx;

        // ********************************
        // private functions
        // ********************************
    private:

        bool loadActor( const StrA & name );
    };
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SHADOWMAP_SIMPLE_H__
