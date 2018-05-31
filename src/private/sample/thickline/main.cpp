#include "pch.h"
#include "garnet/GNgfx.h"
#include "garnet/util/sampleApp.h"
//#include "orientationBox.h"
//#include "viewfrustum.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.sample.thickline");

// static XMMATRIX ToXMMatrix( const Matrix44f & m )
// {
// #if GN_PLATFORM_HAS_DIRECTXMATH
//     return XMMATRIX(
//         m[0][0], m[0][1], m[0][2], m[0][3],
//         m[1][0], m[1][1], m[1][2], m[1][3],
//         m[2][0], m[2][1], m[2][2], m[2][3],
//         m[3][0], m[3][1], m[3][2], m[3][3] );
// #else
//     return XMMatrixSet(
//         m[0][0], m[0][1], m[0][2], m[0][3],
//         m[1][0], m[1][1], m[1][2], m[1][3],
//         m[2][0], m[2][1], m[2][2], m[2][3],
//         m[3][0], m[3][1], m[3][2], m[3][3] );
// #endif
// }

// Matrix44f ToMatrix44f( CXMMATRIX m )
// {
// #if GN_PLATFORM_HAS_DIRECTXMATH
//     return Matrix44f(
//         XMVectorGetX(m.r[0]), XMVectorGetY(m.r[0]), XMVectorGetZ(m.r[0]), XMVectorGetW(m.r[0]),
//         XMVectorGetX(m.r[1]), XMVectorGetY(m.r[1]), XMVectorGetZ(m.r[1]), XMVectorGetW(m.r[1]),
//         XMVectorGetX(m.r[2]), XMVectorGetY(m.r[2]), XMVectorGetZ(m.r[2]), XMVectorGetW(m.r[2]),
//         XMVectorGetX(m.r[3]), XMVectorGetY(m.r[3]), XMVectorGetZ(m.r[3]), XMVectorGetW(m.r[3]) );
// #else
//     return Matrix44f(
//         m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
//         m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
//         m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
//         m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3] );
// #endif
// }

class ThickLineDemo : public SampleApp
{
    using ThickLineVertex = GN::gfx::ThickLineRenderer::ThickLineVertex;

    GN::gfx::ThickLineRenderer rndr;
    //D3D9OrientationBox orientation;

    float     radius;
    ArcBall   arcball;
    Matrix44f proj, view;

    int activeScene;

    // box scene
    ThickLineVertex m_Box[24];
    uint16          m_BoxIndices[36];

    // view frustum scene
    //D3D9ViewFrustum viewFrustum;

    void updateRadius()
    {
        auto gpu = engine::getGpu();
        const auto & gc = gpu->getContext();
        const auto & vp = gc.rs.viewport;

        // setup transformation matrices
        Matrix44f view, proj;
        view.lookAt( GN::Vector3f(0, 0, radius), GN::Vector3f(0, 0, 0), GN::Vector3f(0, 1, 0) );
        proj = gpu->composePerspectiveMatrix( GN_PI/3.0f, (float)vp.w / vp.h, radius / 100.0f, radius * 2.0f );

        // setup arcball
        float h = tan( 0.5f ) * radius * 2.0f;
        arcball.setMouseMoveWindow( 0, 0, (int)vp.w, (int)vp.h );
        arcball.setViewMatrix( view );
        arcball.setTranslationSpeed( h / vp.h );
    }

public:

    ThickLineDemo() //: orientation(64.0f)
    {
        activeScene = 1;

        // create box geometry
        createBox(
            10.0f, 10.0f, 10.0f,
            &m_Box[0].x, sizeof(ThickLineVertex),
            &m_Box[0].u, sizeof(ThickLineVertex),
            0, 0, // normals
            0, 0, // tangents
            0, 0, // binormals
            m_BoxIndices,
            NULL // quad list indices
            );
        for( int i = 0; i < 24; ++i )
        {
            m_Box[i].color = 0xFFFFFFFF;
        }

        // // create viewfrustum geometry
        // viewFrustum.UpdateViewFrustumRH(
        //     XMVectorSet( 5, 0, 0, 1 ), // eye
        //     XMVectorSet( 0, 0, 0, 1 ), // at
        //     XMVectorSet( 0, 1, 0, 0 ), // up
        //     XM_PI / 3.0f, // fovy
        //     4.0f / 3.0f, // ratio
        //     1.0f, // near
        //     10.0f ); // far
    }

    ~ThickLineDemo()
    {
    }

    bool onInit() override
    {
        auto gpu = engine::getGpu();

        if( !rndr.init( *gpu ) ) return false;
        // if( !orientation.OnDeviceCreate( &dev ) ) return false;
        // if( !orientation.OnDeviceRestore() ) return false;
        // if( !viewFrustum.OnDeviceCreate( &dev ) ) return false;
        // if( !viewFrustum.OnDeviceRestore() ) return false;

        // setup arcball
        arcball.setHandness( util::RIGHT_HAND );
        arcball.connectToInput();

        radius = 15.0f;
        updateRadius();

        return true;
    }

    void onQuit() override
    {
        rndr.quit();
        // orientation.OnDeviceDispose();
        // orientation.OnDeviceDelete();
        // viewFrustum.OnDeviceDispose();
        // viewFrustum.OnDeviceDelete();
    }

    void onKeyPress( input::KeyEvent ke ) override
    {
        SampleApp::onKeyPress(ke);

        if( input::KeyCode::SPACEBAR == ke.code && ke.status.down )
        {
            const int NUM_SCENES = 2;
            activeScene = (activeScene + 1) % NUM_SCENES;
        }
    }

    void onAxisMove( GN::input::Axis a, int d ) override
    {
        if( GN::input::Axis::MOUSE_WHEEL_0 == a )
        {
            float speed = radius / 100.0f;
            radius -= speed * d;
            if( radius < 0.1f ) radius = 0.1f;
            updateRadius();
        }
    }

    void DrawBoxScene( const Matrix44f & world )
    {
        GN::gfx::ThickLineRenderer::ThickLineParameters p;
        p.worldview = world * view;
        p.proj = proj;
        p.width = 0.1f;
        p.widthInScreenSpace = false;
        if( rndr.drawBegin( p ) )
        {
            //dev.SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

            for( size_t i = 0; i < GN_ARRAY_COUNT(m_BoxIndices)/3; ++i )
            {
                rndr.line( m_Box[m_BoxIndices[i*3+0]], m_Box[m_BoxIndices[i*3+1]] );
                rndr.line( m_Box[m_BoxIndices[i*3+1]], m_Box[m_BoxIndices[i*3+2]] );
                rndr.line( m_Box[m_BoxIndices[i*3+2]], m_Box[m_BoxIndices[i*3+0]] );
            }

            rndr.drawEnd();
        }
    }

    void onUpdate() override
    {
    }

    void onRender() override
    {
        auto gpu = engine::getGpu();

        gpu->clearScreen();

        auto r = arcball.getRotationMatrix44();
        auto t = Matrix44f::sTranslate( arcball.getTranslation() );
        auto world = t * r;

        // draw box frame
        switch( activeScene )
        {
            case 0: DrawBoxScene( world ); break;
            // case 1: viewFrustum.DrawRH( world * view, proj ); break;
        }

        // draw orientation box
        //orientation.Draw( 32, 32, XMMatrixTranspose(ToXMMatrix(r)) );
    }
};

int main(int argc, const char * argv[])
{
    ThickLineDemo app;
    return app.run(argc, argv);
}
