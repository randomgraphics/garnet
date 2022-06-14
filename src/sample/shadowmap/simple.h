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

namespace GN {
///
/// The most primitive shadow map implementation.
///
class SimpleShadowMap : public StdClass {
    GN_DECLARE_STDCLASS(SimpleShadowMap, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SimpleShadowMap(SampleApp & app): mApp(app), mCamera(RIGHT_HAND), mScene(app.getEntityManager(), app.getRenderEngine()) {
        clear();
        mCamera.connectToInput();
    }
    virtual ~SimpleShadowMap() {
        quit();
        mCamera.disconnectFromInput();
    }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(const StrA & actorName);
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
    bool loadActor(const StrA & name);
};
}; // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SHADOWMAP_SIMPLE_H__
