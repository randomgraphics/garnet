#include "pch.h"
#include "garnet/GNd3d.h"
#include "orientationBox.h"
#include "viewfrustum.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;
using namespace GN::d3d9;

static GN::Logger * sLogger = GN::getLogger("GN.sample.thickline");

static XMMATRIX ToXMMatrix( const Matrix44f & m )
{
    return XMMatrixSet(
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3] );
}

static Matrix44f ToMatrix44f( CXMMATRIX m )
{
    return Matrix44f(
        m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
        m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
        m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
        m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3] );
}

class ThickLineDemo : public D3D9Application
{
    D3D9ThickLineRenderer rndr;
    D3D9OrientationBox orientation;

    float    radius;
    ArcBall  arcball;
    XMMATRIX proj, view;

    int activeScene;

    // box scene
    ThickLineVertex m_Box[24];
    UInt16          m_BoxIndices[36];

    // view frustum scene
    D3D9ViewFrustum viewFrustum;

    void updateRadius()
    {
        IDirect3DDevice9 & dev = d3d9dev();
        D3DVIEWPORT9 vp;
        dev.GetViewport( &vp );

        // setup transformation matrices
        view = XMMatrixLookAtRH( XMVectorSet(0,0,radius,1), XMVectorSet(0,0,0,1), XMVectorSet(0,1,0,0) );
        proj = XMMatrixPerspectiveFovRH( GN_PI/3.0f, (float)vp.Width/vp.Height, radius / 100.0f, radius * 2.0f );

        // setup arcball
        float h = tan( 0.5f ) * radius * 2.0f;
        arcball.setMouseMoveWindow( 0, 0, (int)vp.Width, (int)vp.Height );
        arcball.setViewMatrix( ToMatrix44f(XMMatrixTranspose(view)) );
        arcball.setTranslationSpeed( h / vp.Height );
    }

public:

    ThickLineDemo() : orientation(64.0f)
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

        // create viewfrustum geometry
        viewFrustum.UpdateViewFrustumRH(
            XMVectorSet( 5, 0, 0, 1 ), // eye
            XMVectorSet( 0, 0, 0, 1 ), // at
            XMVectorSet( 0, 1, 0, 0 ), // up
            XM_PI / 3.0f, // fovy
            4.0f / 3.0f, // ratio
            1.0f, // near
            10.0f ); // far
    }

    ~ThickLineDemo()
    {
    }

    bool onCreate()
    {
        IDirect3DDevice9 & dev = d3d9dev();

        if( !rndr.onDeviceCreate( &dev ) ) return false;
        if( !orientation.OnDeviceCreate( &dev ) ) return false;
        if( !viewFrustum.OnDeviceCreate( &dev ) ) return false;

        // setup arcball
        arcball.setHandness( util::RIGHT_HAND );
        arcball.connectToInput();

        return true;
    }

    bool onRestore()
    {
        if( !rndr.onDeviceRestore() ) return false;
        if( !orientation.OnDeviceRestore() ) return false;
        if( !viewFrustum.OnDeviceRestore() ) return false;

        IDirect3DDevice9 & dev = d3d9dev();
        D3DVIEWPORT9 vp;
        dev.GetViewport( &vp );

        radius = 15.0f;
        updateRadius();

        return true;
    }

    void onDispose()
    {
        rndr.onDeviceDispose();
        orientation.OnDeviceDispose();
        viewFrustum.OnDeviceDispose();
    }

    void onDestroy()
    {
        rndr.onDeviceDelete();
        orientation.OnDeviceDelete();
        viewFrustum.OnDeviceDelete();
    }

    void onKeyPress( input::KeyEvent ke )
    {
        if( input::KeyCode::SPACEBAR == ke.code && ke.status.down )
        {
            const int NUM_SCENES = 2;
            activeScene = (activeScene + 1) % NUM_SCENES;
        }
    }

    void onAxisMove( GN::input::Axis a, int d )
    {
        if( GN::input::Axis::MOUSE_WHEEL_0 == a )
        {
            float speed = radius / 100.0f;
            radius -= speed * d;
            if( radius < 0.1f ) radius = 0.1f;
            updateRadius();
        }
    }

    void DrawBoxScene( CXMMATRIX world )
    {
        IDirect3DDevice9 & dev = d3d9dev();

        ThickLineParameters p;
        p.worldview = world * view;
        p.proj = proj;
        p.width = 0.1f;
        p.widthInScreenSpace = false;
        if( rndr.drawBegin( p ) )
        {
            dev.SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

            for( size_t i = 0; i < GN_ARRAY_COUNT(m_BoxIndices)/3; ++i )
            {
                rndr.line( m_Box[m_BoxIndices[i*3+0]], m_Box[m_BoxIndices[i*3+1]] );
                rndr.line( m_Box[m_BoxIndices[i*3+1]], m_Box[m_BoxIndices[i*3+2]] );
                rndr.line( m_Box[m_BoxIndices[i*3+2]], m_Box[m_BoxIndices[i*3+0]] );
            }

            rndr.drawEnd();
        }
    }

    void DrawViewFrustumScene( CXMMATRIX world )
    {
        viewFrustum.DrawRH( world * view, proj );
    }

    void onDraw()
    {
        IDirect3DDevice9 & dev = d3d9dev();

        dev.Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );

        if( SUCCEEDED( dev.BeginScene() ) )
        {
            Matrix44f r = arcball.getRotationMatrix44();
            Matrix44f t = Matrix44f::sTranslate( arcball.getTranslation() );
            XMMATRIX world = XMMatrixTranspose( ToXMMatrix(t * r) );

            // draw box frame
            switch( activeScene )
            {
                case 0: DrawBoxScene( world ); break;
                case 1: DrawViewFrustumScene( world ); break;
            }

            // draw orientation box
            orientation.Draw( 32, 32, XMMatrixTranspose(ToXMMatrix(r)) );

            dev.EndScene();
        }

        dev.Present( 0, 0, 0, 0 );
    }
};

int main()
{
    D3D9AppOption opt;
    //opt.refdev = true;

    ThickLineDemo app;
    return app.run( &opt );
}
