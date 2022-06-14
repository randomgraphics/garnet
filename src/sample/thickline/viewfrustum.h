#pragma once

#include "garnet/GNd3d.h"

class D3D9ViewFrustum {
public:
    D3D9ViewFrustum();
    ~D3D9ViewFrustum() {
        OnDeviceDispose();
        OnDeviceDelete();
    }

    bool OnDeviceCreate(IDirect3DDevice9 * dev);
    bool OnDeviceRestore();
    void OnDeviceDispose();
    void OnDeviceDelete();

    void UpdateViewFrustumRH(const XMVECTOR & eye, const XMVECTOR & at, const XMVECTOR & up, float fovy, float ratio, float near, float far);

    void DrawRH(const XMMATRIX & worldViewRH, const XMMATRIX & projRH);

private:
    struct TriangleFace {
        XMVECTOR vertices[3];  // vertices should be in CCW order if when looking from outside of the view frustum
        XMVECTOR outterNormal; // face normal pointing to outside
        void     Set(const XMVECTOR & v0, const XMVECTOR & v1, const XMVECTOR & v2);
    };

    struct QuadFace {
        XMVECTOR vertices[4];
        XMVECTOR outterNormal; // face normal pointing to outside
        void     Set(const XMVECTOR & v0, const XMVECTOR & v1, const XMVECTOR & v2, const XMVECTOR & v3);
    };

    struct Vertex {
        float    x, y, z;    // position
        float    nx, ny, nz; // normal
        D3DCOLOR color;

        void Set(const XMVECTOR & pos, const XMVECTOR & normal, D3DCOLOR c) {
            x     = XMVectorGetX(pos);
            y     = XMVectorGetY(pos);
            z     = XMVectorGetZ(pos);
            nx    = XMVectorGetX(normal);
            ny    = XMVectorGetY(normal);
            nz    = XMVectorGetZ(normal);
            color = c;
        }
    };

    static const UINT NUM_VIEW_FRUSTUM_VERTICES = 6 * 3; // One view frustum has 4 side triangles and 2 far-end triangles.
    static const UINT VIEW_FRUSTUM_VB_SIZE      = (UINT) (sizeof(Vertex) * NUM_VIEW_FRUSTUM_VERTICES);

private:
    TriangleFace m_Sides[4]; // left, bottom, right, top
    QuadFace     m_FarEnd;
    Vertex       m_Vertices[NUM_VIEW_FRUSTUM_VERTICES];

    IDirect3DDevice9 *       m_Device;
    IDirect3DVertexShader9 * m_Vs;
    IDirect3DPixelShader9 *  m_Ps;
    IDirect3DVertexBuffer9 * m_VbSolid, *m_VbTrans;

    GN::d3d9::D3D9ThickLineRenderer m_LineRenderer;

private:
    // return number of vertices pushed into vertex array
    UINT BuildTriangleFaceVertices(Vertex * vertices, const TriangleFace & face, bool flipFace, D3DCOLOR c);
    UINT BuildQuadFaceVertices(Vertex * vertices, const QuadFace & face, bool flipFace, D3DCOLOR c);
};
