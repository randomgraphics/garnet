#include "pch.h"
#include "loader.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;
using namespace GN::scene;
using namespace GN::app;

static GN::Logger * sLogger = GN::getLogger("GN.tool.meshViewer");

class MyApp : public SampleApp
{
    const char                 * filename;
    ArcBall                      arcball; // arcball camera
    float                        radius;  // distance from camera to object
    Matrix44f                    proj, view;
    AutoObjPtr<Scene>            rootScene;
    AutoObjPtr<GeometryNode>     model;

public:

    void updateRadius()
    {
        Renderer & rndr = getRenderer();

        const DispDesc & dd = rndr.getDispDesc();

        view.lookAtRh( Vector3f(0,0,radius), Vector3f(0,0,0), Vector3f(0,1,0) );
        rndr.composePerspectiveMatrixRh( proj, GN_PI/4.0f, (float)dd.width/dd.height, radius / 100.0f, radius * 2.0f );

        float h = tan( 0.5f ) * radius * 2.0f;
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );
        arcball.setViewMatrix( view );
        arcball.setTranslationSpeed( h / dd.height );

        rootScene->setDefaultLight0Position( Vector3f(0,0,radius) ); // head light: same location as camera.

        // calculate move speed
    }

    bool onPostInit()
    {
        Renderer & rndr = getRenderer();

        rndr.getSignals().rendererWindowSizeMove.connect( this, &MyApp::onRenderWindowResize );

        SimpleDiffuseEffect effect;

        // create scene
        rootScene.attach( createScene( rndr ) );
        if( !rootScene ) return false;

        // initialize effect
        if( !effect.init( rndr ) ) return false;

        // load model
        model.attach( loadGeometryFromFile( *rootScene, effect.getEffect(), filename ) );
        if( !model ) return false;

        // update camera stuff
        const Spheref & bs = model->getBoundingSphere();
        radius = bs.radius * 1.5f;
        updateRadius();

        // initialize arcball
        arcball.setHandness( util::RIGHT_HAND );
        arcball.setViewMatrix( view );
        arcball.setTranslation( -bs.center );
        arcball.connectToInput();

        // success
        return true;
    }

    void onQuit()
    {
        model.clear();
        rootScene.clear();
    }

    void onRenderWindowResize( HandleType, UInt32 width, UInt32 height )
    {
        arcball.setMouseMoveWindow( 0, 0, (int)width, (int)height );
    }

    void onAxisMove( Axis a, int d )
    {
        if( Axis::MOUSE_WHEEL_0 == a )
        {
            float speed = radius / 100.0f;
            radius -= speed * d;
            if( radius < 0.1f ) radius = 0.1f;
            updateRadius();
        }
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        Renderer & rndr = getRenderer();

        rndr.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );

        const Vector3f & position = arcball.getTranslation();

        rootScene->setProj( proj );
        rootScene->setView( view );
        model->setPosition( position );
        model->setRotation( arcball.getRotation() );
        rootScene->renderNodeHierarchy( model );

        getFont().drawText(
            strFormat(
                L"position : %f,\n"
                L"           %f,\n"
                L"           %f\n"
                L"radius   : %f",
                position.x, position.y, position.z,
                radius ).cptr(),
            320, 40 );

        drawXYZCoordinateAxes( proj * view * arcball.getRotationMatrix44() );
    }

    bool onCheckExtraCmdlineArguments( int argc, const char * const argv [] )
    {
        if( 0 == argc )
        {
            GN_ERROR(sLogger)( "Mesh file name is missing." );
            return false;
        }
        filename = argv[0];
        return true;
    }

    void onPrintHelpScreen( const char * executableName )
    {
        GN_INFO(sLogger)( "\nUsage: %s [options] meshfile\n", executableName );
        printStandardCommandLineOptions();
    }
};

int main( int argc, const char * argv[] )
{
   printf( "\nGarnet mesh viewer V0.1.\n" );

   MyApp app;

   return app.run( argc, argv );
}
