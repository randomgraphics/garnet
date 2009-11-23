#pragma once

#include <d3d9.h>
#include <xnamath.h>

class D3D9OrientationBox
{
public:

    D3D9OrientationBox( float boxSizeInScreenSpace = 32.0f );
    ~D3D9OrientationBox() { OnDeviceDispose(); OnDeviceDelete(); }

    bool OnDeviceCreate( IDirect3DDevice9 * dev );
    bool OnDeviceRestore();
    void OnDeviceDispose();
    void OnDeviceDelete();

    void Draw(
        float x, float y, // box location in viewport space: (0,0) is top left corner of current viewport.
        const XMMATRIX & viewRH // must be a right hand view matrix
        );

private:

    struct BoxVertex
    {
        float x, y, z;   // position
        D3DCOLOR color;  // color
        float u, v;      // texcoord

        void Set( float x_, float y_, float z_, float u_, float v_, D3DCOLOR c_ )
        {
            x = x_; y = y_; z = z_;
            u = u_; v = v_;
            color = c_;
        }
    };

    struct BoxFace
    {
        BoxVertex vertices[4];
    };

    float               m_BoxSize;
    BoxFace             m_BoxFaces[6];

    IDirect3DDevice9       * m_Device;
    IDirect3DVertexShader9 * m_Vs;
    IDirect3DPixelShader9  * m_Ps;
    IDirect3DTexture9      * m_RtTexture;
    IDirect3DSurface9      * m_RtSurface;
    IDirect3DSurface9      * m_ZSurface;
    IDirect3DTexture9      * m_Textures[6];

private:
};
