#include "pch.h"
#include "cube.h"
 
using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.test.engine.Cube");

bool TestCube::init()
{
    EntityManager & em = entityManager();
    RenderEngine & re = renderEngine();

    // create ascii font
    FontFaceDesc ffd;
    ffd.fontname = "font::/simsun.ttc";
    ffd.width = 32;
    ffd.height = 32;
    if( !font.init( ffd ) ) return false;

    // create cube
    if( !cube.loadFromXmlFile( em, re, "media::/cube/cube_on_cube.drawable.xml" ) ) return false;

    // calculate pvw matrix
    Matrix44f proj,view;
    proj.perspectiveD3D( 1.0f, 4.0f/3.0f, 10.0f, 500.0f );
    view.lookAt( Vector3f(100.0f,100.0f,100.0f), Vector3f(0,0,0), Vector3f(0,1.0f,0) );
    cube.uniforms["pvw"].value = proj * view;

    // create texture entity
    tex = loadTextureEntityFromFile( em, re, "media::/texture/cube1.dds" );
    if( 0 == tex ) return false;
    cube.textures["cube"].texture = tex;

    // success
    return true;
}

void TestCube::quit()
{
    // make sure that all resources are unbinded from render engine.
    renderEngine().clearDrawContext();

    cube.clear();
    deleteEntity( tex );

    font.quit();
}

void TestCube::draw()
{
    // dispose all
    renderEngine().disposeAllResources();

    // draw a quad
    font.drawText( L"render engine cube test", 10, 10 );

    // draw cube
    cube.draw();
}
