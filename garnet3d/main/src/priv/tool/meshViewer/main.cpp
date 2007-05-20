#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::scene;
using namespace GN::input;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.tool.meshViewer");

class MeshViewerApp : public app::SampleApp
{
    StrA      mFileName;
    StrA      mObjName;
    Scene     mScene;
    Actor *   mActor;
    ArcBall   mArcBall;

    float     mRadius; // distance from camera to object

    Matrix44f mProj, mView;

    void updateRadius()
    {
        Renderer & r = gRenderer;

        const DispDesc & dd = r.getDispDesc();

        mView.lookAtRh( Vector3f(0,0,mRadius), Vector3f(0,0,0), Vector3f(0,1,0) );
        r.composePerspectiveMatrixRh( mProj, GN_PI/4.0f, (float)dd.width/dd.height, mRadius / 100.0f, mRadius * 2.0f );
        mScene.setView( mView );
        mScene.setProj( mProj );
        mScene.light(0).position.set( 0, 0, mRadius ); // head light: same location as camera.
        mArcBall.setViewMatrix( mView );

        // calculate move speed
        float h = tan( 0.5f ) * mRadius * 2.0f;
        mArcBall.setTranslationSpeed( h / dd.height );
    }

public:

    MeshViewerApp()
        : mScene( getEntityManager(), getRenderEngine() )
        , mActor(0)
    {}

    bool onCheckCmdLine( int argc, const char * const argv[] )
    {
        bool isFileName = true;
        for( int i = 1; i < argc; ++i )
        {
            const char * a = argv[i];
            if( '-' != *a )
            {
                if( isFileName ) mFileName = a, isFileName = false;
                else mObjName = a;
            }
            else GN_WARN(sLogger)( "unknown command line argument: %s", a );
        }
        if( mFileName.empty() )
        {
            mFileName = "media::/cube/cube.actor.xml";
            GN_INFO(sLogger)( "no object specified in comment line. Using default one: %s", mFileName.cptr() );
        }
        if( mObjName.empty() ) mObjName = "root";
        return true;
    }

    bool onInit()
    {
        if( !mActor && !mFileName.empty() )
        {
            // (re)load actor
            releaseActorHiearacy( mActor );
            mActor = mScene.loadActorHiearachyFromXmlFile( mFileName, mObjName );
            if( 0 == mActor ) return false;

            // update camera stuff
            mRadius = mActor->getBoundingSphere().radius * 2.0f;
            updateRadius();

            // initialize mArcBall
            mArcBall.setHandness( util::RIGHT_HAND );
            mArcBall.setViewMatrix( mView );
            mArcBall.connectToInput();
            mArcBall.setTranslation( -mActor->getBoundingSphere().center );
        }

        // update arcball window
        const DispDesc & dd = gRenderer.getDispDesc();
        mArcBall.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );

        return true;
    }

    void onQuit()
    {
        releaseActorHiearacy( mActor );
        mActor = 0;
    }

    void onAxisMove( Axis a, int d )
    {
        app::SampleApp::onAxisMove( a, d );

        if( AXIS_MOUSE_WHEEL_0 == a )
        {
            float speed = mRadius / 100.0f;
            mRadius -= speed * d;
            if( mRadius < 0.1f ) mRadius = 0.1f;
            updateRadius();
        }
    }

    void onUpdate()
    {
        const int * axises = gInput.getAxisStatus();
        mArcBall.rotate(
            (float)axises[input::AXIS_XB360_THUMB_LX] /  2000.0f,
            (float)axises[input::AXIS_XB360_THUMB_LY] / -2000.0f );

        if( mActor )
        {
            Vector3f pos = mArcBall.getTranslation();
            mActor->setPosition( pos );
            mActor->setPivot( -pos );
            mActor->setRotation( mArcBall.getRotation() );
        }
    }

    void onRender()
    {
        RenderEngine & e = getRenderEngine();

        e.clearScreen();

        static const float X[] = { 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f, 0.0f };
        static const float Y[] = { 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f };
        static const float Z[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f };

        const Matrix44f & world = mArcBall.getRotationMatrix44();
        e.drawLines( 0, X, 3*sizeof(float), 1, GN_RGBA32(255,0,0,255), world, mView, mProj );
        e.drawLines( 0, Y, 3*sizeof(float), 1, GN_RGBA32(0,255,0,255), world, mView, mProj );
        e.drawLines( 0, Z, 3*sizeof(float), 1, GN_RGBA32(0,0,255,255), world, mView, mProj );

        if( mActor ) mActor->draw();

#if 0
        // draw matrices onto screen
        {
            StrW text;
            text.format(
                L"world :\n%S\n"
                L"view  :\n%S\n"
                L"proj  :\n%S\n",
                mWorld.print().cptr(),
                mView.print().cptr(),
                mProj.print().cptr() );
            getFont().drawText( text.cptr(), 0, 100 );
        }
#endif
    }
};

int main( int argc, const char * argv[] )
{
    MeshViewerApp app;
    return app.run( argc, argv );
}
