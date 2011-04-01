#include "pch.h"
#include <garnet/gfx/fatModel.h>

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::engine;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.tool.meshViewer");

#define USE_STATIC_MESH 1

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
    bool         showbbox;

#if USE_STATIC_MESH
    AutoObjPtr<Entity> entity;
#else
    SampleWorld  world;
#endif

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
        engine::getGdb()->setStandardUniform( StandardUniform::Index::LIGHT0_POSITION, lightPos );
    }

    bool onInit()
    {
        // load mesh from file
#if USE_STATIC_MESH
        FatModel fm;
        if( !fm.loadFromFile( filename ) ) return false;
        if( fm.skeletons.empty() )
        {
            StaticMesh * mesh = new StaticMesh;
            entity.attach( mesh );
            if( !mesh->loadFromFatModel( fm ) ) return false;
        }
        else
        {
            SkinnedMesh * mesh = new SkinnedMesh;
            entity.attach( mesh );
            if( !mesh->loadFromFatModel( fm ) ) return false;
        }
        const Boxf & bbox = entity->getComponent<SpacialComponent>()->getUberBoundingBox();
#else
        ModelHierarchyDesc swd;
        if( !swd.loadFromFile( filename ) ) return false;
        if( !world.createEntites( swd ) ) return false;
        world.showBoundingBoxes( true );
        const Boxf & bbox = world.getRootEntity()->getComponent<SpacialComponent>()->getUberBoundingBox();
#endif
        showbbox = true;

        // update scene radius
        Spheref bs;
        calculateBoundingSphereFromBoundingBox( bs, bbox );
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
#if USE_STATIC_MESH
        entity.clear();
#else
        world.clear();
#endif
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
#if USE_STATIC_MESH
        SpacialComponent * spacial = entity->getComponent<SpacialComponent>();
#else
        SpacialComponent * spacial = world.getRootEntity()->getComponent<engine::SpacialComponent>();
#endif
        const Vector3f & position = arcball.getTranslation();
        spacial->setPosition( position );
        spacial->setRotation( arcball.getRotation() );
    }

    void onRender()
    {
        Gpu * gpu = engine::getGpu();

        gpu->clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );

#if USE_STATIC_MESH
        entity->getComponent<VisualComponent>()->draw( camera.proj, camera.view );
        if( showbbox ) entity->getComponent<SpacialComponent>()->drawBoundingBox( camera.proj, camera.view, 0xFF000000 );
#else
        world.showBoundingBoxes( showbbox );
        world.draw( camera.proj, camera.view );
#endif
        const Vector3f & position = arcball.getTranslation();

        drawXYZCoordinateAxes( camera.proj * camera.view * arcball.getRotationMatrix44() );

        engine::getDefaultFontRenderer()->drawText(
            stringFormat(
                L"position : %f,\n"
                L"           %f,\n"
                L"           %f\n"
                L"radius   : %f",
                position.x, position.y, position.z,
                radius ).cptr(),
            320, 40 );
    }

    void onKeyPress( input::KeyEvent ke )
    {
        SampleApp::onKeyPress( ke );

        if( input::KeyCode::B == ke.code && ke.status.down )
        {
            showbbox = !showbbox;
        }
    }

    bool onCheckExtraCmdlineArguments( const char * exename, int argc, const char * const argv [] )
    {
        GN_UNUSED_PARAM( exename );

        if( 0 == argc )
        {
            GN_ERROR(sLogger)( "Mesh file name is missing." );
            return false;
        }

        if( argc >= 2 && ( 0 == stringCompare( argv[0], "-print" ) || 0 == stringCompare( argv[0], "--print" ) ) )
        {
            StrA s;
            printModelFileNodeHierarchy( s, argv[1] );
            GN_INFO(sLogger)( "%s", s );
            return false;
        }
        else
        {
            filename = argv[0];
            return true;
        }
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
