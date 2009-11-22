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
        float x, float y, float z, // box location in screen space
        const XMMATRIX & view );

private:

    struct BoxVertex
    {
        float x, y, z; // position
        float u, v;    // texcoord
        float padding[3];
    };

    struct BoxFace
    {
        BoxVertex vertices[4];
    };

    float               m_BoxSize;
    BoxFace             m_BoxFaces[6];

    IDirect3DDevice9  * m_Device;
    IDirect3DTexture9 * m_Textures[6];

private:
};
