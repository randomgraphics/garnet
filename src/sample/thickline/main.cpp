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
// #if GN_BUILD_HAS_DIRECTXMATH
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
// #if GN_BUILD_HAS_DIRECTXMATH
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

class ThickLineDemo : public SampleApp {
    using ThickLineVertex = GN::gfx::ThickLineRenderer::ThickLineVertex;

    GN::gfx::ThickLineRenderer rndr;
    // D3D9OrientationBox orientation;

    const float BOX_SIZE = 400.0f;
    float       m_Radius = BOX_SIZE * 1.5f;
    ArcBall     m_Arcball;
    Matrix44f   m_Proj, m_View;

    int  m_ActiveScene            = 0;
    bool m_LineWidthInScreenSpace = false;

    // box scene
    ThickLineVertex m_Box[24];
    uint16          m_BoxIndices[36];

    // view frustum scene
    // D3D9ViewFrustum viewFrustum;

    Rect<uint32_t> getViewport() {
        auto         gpu = engine::getGpu();
        const auto & gc  = gpu->getContext();
        auto         vp  = gc.rs.viewport;
        if (0 == vp.w || 0 == vp.h) {
            uint32_t w, h;
            if (gc.colortargets.size() > 0) {
                const auto & size = gc.colortargets[0].texture->getMipSize(gc.colortargets[0].level);
                w                 = size[0];
                h                 = size[1];
            } else if (gc.depthstencil.texture) {
                const auto & size = gc.depthstencil.texture->getMipSize(gc.depthstencil.level);
                w                 = size[0];
                h                 = size[1];
            } else {
                const auto & dd = gpu->getDispDesc();
                w               = dd.width;
                h               = dd.height;
            }
            if (0 == vp.w) vp.w = w;
            if (0 == vp.h) vp.h = h;
        }
        return vp;
    }

    void updateRadius() {
        // setup transformation matrices
        m_View.lookAt(GN::Vector3f(0, 0, m_Radius), GN::Vector3f(0, 0, 0), GN::Vector3f(0, 1, 0));
        auto vp  = getViewport();
        auto gpu = engine::getGpu();
        m_Proj   = gpu->composePerspectiveMatrix(GN_PI / 3.0f, (float) vp.w / vp.h, m_Radius / 100.0f, m_Radius * 2.0f);

        // setup arcball
        float h = tan(0.5f) * m_Radius * 2.0f;
        m_Arcball.setMouseMoveWindow(0, 0, (int) vp.w, (int) vp.h);
        m_Arcball.setViewMatrix(m_View);
        m_Arcball.setTranslationSpeed(h / vp.h);
    }

public:
    ThickLineDemo() //: orientation(64.0f)
    {
        // create box geometry
        createBox(BOX_SIZE, BOX_SIZE, BOX_SIZE, &m_Box[0].x, sizeof(ThickLineVertex), &m_Box[0].u, sizeof(ThickLineVertex), 0, 0, // normals
                  0, 0,                                                                                                           // tangents
                  0, 0,                                                                                                           // binormals
                  m_BoxIndices,
                  NULL // quad list indices
        );
        for (int i = 0; i < 24; ++i) { m_Box[i].color = 0xFFFFFFFF; }

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

    ~ThickLineDemo() {}

    bool onInit() override {
        auto gpu = engine::getGpu();

        if (!rndr.init(*gpu)) return false;
        // if( !orientation.OnDeviceCreate( &dev ) ) return false;
        // if( !orientation.OnDeviceRestore() ) return false;
        // if( !viewFrustum.OnDeviceCreate( &dev ) ) return false;
        // if( !viewFrustum.OnDeviceRestore() ) return false;

        // setup arcball
        m_Arcball.setHandness(util::RIGHT_HAND);
        m_Arcball.connectToInput();

        updateRadius();

        return true;
    }

    void onQuit() override {
        rndr.quit();
        // orientation.OnDeviceDispose();
        // orientation.OnDeviceDelete();
        // viewFrustum.OnDeviceDispose();
        // viewFrustum.OnDeviceDelete();
    }

    void onKeyPress(input::KeyEvent ke) override {
        SampleApp::onKeyPress(ke);

        if (input::KeyCode::SPACEBAR == ke.code() && ke.status.down) {
            // const int NUM_SCENES = 2;
            // m_ActiveScene = (m_ActiveScene + 1) % NUM_SCENES;
            m_LineWidthInScreenSpace = !m_LineWidthInScreenSpace;
        }
    }

    void onAxisMove(GN::input::Axis a, int d) override {
        if (GN::input::Axis::MOUSE_WHEEL_0 == a) {
            float speed = m_Radius / 100.0f;
            m_Radius -= speed * d;
            if (m_Radius < 0.1f) m_Radius = 0.1f;
            updateRadius();
        }
    }

    void DrawBoxScene(const Matrix44f & world) {
        GN::gfx::ThickLineRenderer::ThickLineParameters p;
        p.worldview          = m_View * world;
        p.proj               = m_Proj;
        p.width              = BOX_SIZE / 100.0f;
        p.widthInScreenSpace = m_LineWidthInScreenSpace;
        if (rndr.drawBegin(p)) {
            // dev.SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

            for (size_t i = 0; i < GN_ARRAY_COUNT(m_BoxIndices) / 3; ++i) {
                rndr.line(m_Box[m_BoxIndices[i * 3 + 0]], m_Box[m_BoxIndices[i * 3 + 1]]);
                rndr.line(m_Box[m_BoxIndices[i * 3 + 1]], m_Box[m_BoxIndices[i * 3 + 2]]);
                rndr.line(m_Box[m_BoxIndices[i * 3 + 2]], m_Box[m_BoxIndices[i * 3 + 0]]);
            }

            rndr.drawEnd();
        }
    }

    void onUpdate() override {}

    void onRender() override {
        auto gpu = engine::getGpu();

        gpu->clearScreen();

        auto r     = m_Arcball.getRotationMatrix44();
        auto t     = Matrix44f::sTranslate(m_Arcball.getTranslation());
        auto world = t * r;

        // draw box frame
        switch (m_ActiveScene) {
        case 0:
            DrawBoxScene(world);
            break;
            // case 1: viewFrustum.DrawRH( world * view, proj ); break;
        }

        // draw orientation box
        // orientation.Draw( 32, 32, XMMatrixTranspose(ToXMMatrix(r)) );
    }
};

int main(int argc, const char * argv[]) {
    ThickLineDemo app;
    return app.run(argc, argv);
}
