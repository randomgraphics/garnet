#include "pch.h"
#include "garnet/GNd3d9.h"
#include "thickline.h"

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

static Matrix44f ToMatrix44f( const XMMATRIX & m )
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

    float    radius;
    ArcBall  arcball;
    XMMATRIX proj, view;

    ThickLineVertex m_Box[24];
    UInt16          m_BoxIndices[36];

    void updateRadius()
    {
        IDirect3DDevice9 & dev = d3d9dev();
        D3DVIEWPORT9 vp;
        dev.GetViewport( &vp );

        // setup transformation matrices
        view = XMMatrixLookAtLH( XMVectorSet(0,0,radius,1), XMVectorSet(0,0,0,1), XMVectorSet(0,1,0,0) );
        proj = XMMatrixPerspectiveFovLH( GN_PI/3.0f, (float)vp.Width/vp.Height, radius / 100.0f, radius * 2.0f );

        // setup arcball
        float h = tan( 0.5f ) * radius * 2.0f;
        arcball.setMouseMoveWindow( 0, 0, (int)vp.Width, (int)vp.Height );
        arcball.setViewMatrix( ToMatrix44f(XMMatrixTranspose(view)) );
        arcball.setTranslationSpeed( h / vp.Height );
    }

public:

    ThickLineDemo()
    {
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
    }

    ~ThickLineDemo()
    {
    }

    bool onCreate()
    {
        IDirect3DDevice9 & dev = d3d9dev();

        if( !rndr.OnDeviceCreate( &dev ) ) return false;

        // setup arcball
        arcball.setHandness( util::LEFT_HAND );
        arcball.connectToInput();

        return true;
    }

    bool onRestore()
    {
        if( !rndr.OnDeviceRestore() ) return false;

        IDirect3DDevice9 & dev = d3d9dev();
        D3DVIEWPORT9 vp;
        dev.GetViewport( &vp );

        radius = 15.0f;
        updateRadius();

        return true;
    }

    void onDispose()
    {
        rndr.OnDeviceDispose();
    }

    void onDestroy()
    {
        rndr.OnDeviceDelete();
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

    void onDraw()
    {
        IDirect3DDevice9 & dev = d3d9dev();

        dev.Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );

        if( SUCCEEDED( dev.BeginScene() ) )
        {
            Matrix44f r = arcball.getRotationMatrix44();
            Matrix44f t = Matrix44f::sTranslate( arcball.getTranslation() );
            Matrix44f world = t * r;

            ThickLineParameters p;
            p.transformation = XMMatrixTranspose( ToXMMatrix(world) ) * view * proj;
            p.width = 4.0f;
            if( rndr.DrawBegin( p ) )
            {
                dev.SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

                for( size_t i = 0; i < GN_ARRAY_COUNT(m_BoxIndices)/3; ++i )
                {
                    rndr.Line( m_Box[m_BoxIndices[i*3+0]], m_Box[m_BoxIndices[i*3+1]] );
                    rndr.Line( m_Box[m_BoxIndices[i*3+1]], m_Box[m_BoxIndices[i*3+2]] );
                    rndr.Line( m_Box[m_BoxIndices[i*3+2]], m_Box[m_BoxIndices[i*3+0]] );
                }

                rndr.DrawEnd();
            }

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
