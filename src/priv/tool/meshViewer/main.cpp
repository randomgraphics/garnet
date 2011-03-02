#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.tool.meshViewer");

class MyApp : public SampleApp
{
    struct Camera
    {
        Matrix44f proj;
        Matrix44f view;
    };

    const char * filename;
    ArcBall      arcball; // arcball camera
    float        radius;  // distance from camera to object
    Camera       camera;
    SampleWorld  world;

public:

    void updateRadius()
    {
        Gpu * gpu = engine::getGpu();

        const DispDesc & dd = gpu->getDispDesc();

        // setup camera
        camera.view.lookAtRh( Vector3f(0,0,radius), Vector3f(0,0,0), Vector3f(0,1,0) );
        gpu->composePerspectiveMatrixRh( camera.proj, GN_PI/4.0f, (float)dd.width/dd.height, radius / 100.0f, radius * 2.0f );

        // setup arcball
        float h = tan( 0.5f ) * radius * 2.0f;
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );
        arcball.setViewMatrix( camera.view );
        arcball.setTranslationSpeed( h / dd.height );

        // setup light as a head light that is in same location as camera.
        Vector4f lightPos(0,0,radius,1);
        engine::getStandardUniformManager()->setGlobalUniform( engine::StandardUniformType::LIGHT0_POSITION, lightPos );
    }

    bool onInit()
    {
        // load scene from file
        SampleWorldDesc swd;
        if( !swd.loadFromFile( filename ) ) return false;
        if( !world.createEntites( swd ) ) return false;
        world.showBoundingBoxes( true );

        // update scene radius
        Spheref bs;
        calculateBoundingSphereFromBoundingBox( bs, swd.bbox );
        radius = bs.radius * 2.0f;
        if( 0.0f == radius ) radius = 1.0f;
        updateRadius();

        // initialize arcball
        arcball.setHandness( util::RIGHT_HAND );
        arcball.setTranslation( -bs.center );
        arcball.connectToInput();

        // success
        return true;
    }

    void onQuit()
    {
        world.clear();
    }

    void onRenderWindowResize( intptr_t, uint32 width, uint32 height )
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
        engine::SpacialComponent * spacial = world.getRootEntity()->getComponent<engine::SpacialComponent>();
        const Vector3f & position = arcball.getTranslation();
        spacial->setPosition( position );
        spacial->setRotation( arcball.getRotation() );
    }

    void onRender()
    {
        Gpu * gpu = engine::getGpu();

        gpu->clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );

        world.draw( camera.proj, camera.view );

        const Vector3f & position = arcball.getTranslation();

        engine::getDefaultFontRenderer()->drawText(
            stringFormat(
                L"position : %f,\n"
                L"           %f,\n"
                L"           %f\n"
                L"radius   : %f",
                position.x, position.y, position.z,
                radius ).cptr(),
            320, 40 );

        drawXYZCoordinateAxes( camera.proj * camera.view * arcball.getRotationMatrix44() );
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
