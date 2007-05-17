#include "pch.h"
#include "cube.h"
#include "garnet/GNscene.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.test.engine.Cube");

bool TestCube::init()
{
    EntityManager & em = entityManager();
    RenderEngine & re = renderEngine();

    // create effect
    engine::EffectDesc ed;
    if( !scene::loadFromXmlFile( ed, "media::/cube/cube_on_cube.effect.xml" ) ) return false;
    if( !eff.init( ed ) ) return false;

    // create mesh entity
    mesh = loadMeshEntityFromFile( em, re, "media::/cube/cube.mesh.xml" );
    if( 0 == mesh ) return false;

    // create texture entity
    tex = loadTextureEntityFromFile( em, re, "media::/texture/cube1.dds" );
    if( 0 == tex ) return false;

    // success
    return true;
}

void TestCube::quit()
{
    // make sure that all resources are unbinded from render engine.
    clearDrawContext( renderEngine() );

    deleteEntity( mesh );
    deleteEntity( tex );

    eff.quit();
}

void TestCube::draw()
{
    RenderEngine & re = renderEngine();

    // dispose all
    re.disposeAllResources();

    // calculate pvw matrix
    Matrix44f proj,view;
    proj.perspectiveD3D( 1.0f, 4.0f/3.0f, 10.0f, 500.0f );
    view.lookAt( Vector3f(100.0f,100.0f,100.0f), Vector3f(0,0,0), Vector3f(0,1.0f,0) );

    // load mesh
    engine::Mesh * m = entity2Object<engine::Mesh*>(mesh,0);

    // bind context
    DrawContext ctx;
    ctx.resetToDefault();
    eff.setUniformByName( "pvw", proj*view );
    eff.setTextureByName( "cube", tex );
    eff.passBegin( ctx, 0 );
    m->updateContext( ctx );
    eff.commitChanges();
    eff.passEnd();
    re.setContext( ctx );

    // do draw
    m->draw();
}
