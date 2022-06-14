#include "pch.h"
#include "viewfrustum.h"

using namespace GN;
using namespace GN::d3d9;

// *****************************************************************************
// utilities
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.gfx.thickline");

//
//
// -----------------------------------------------------------------------------
static void LOG_ERROR(const char * format, ...) {
    GN::StrA s;
    va_list  arglist;
    va_start(arglist, format);
    s.formatv(format, arglist);
    va_end(arglist);
    GN_ERROR(sLogger)("%s\n", s.rawptr());
}

#define SAFE_RELEASE(x) \
    if (x) {            \
        (x)->Release(); \
        (x) = NULL;     \
    } else              \
        void(0)

// *****************************************************************************
// local codes
// *****************************************************************************

static const char * vscode = "uniform float4x4 u_wvp    : register(c0); \n"
                             "uniform float4x4 u_inv_wv : register(c4); \n"
                             "struct VSI { \n"
                             "   float4 pos : POSITION0; \n"
                             "   float4 clr : COLOR0;    \n"
                             "   float3 nml : NORMAL0; \n"
                             "}; \n"
                             "struct VSO { \n"
                             "   float4 pos    : POSITION0; \n"
                             "   float4 clr    : COLOR0;    \n"
                             "   float3 normal : TEXCOORD0; \n"
                             "   float3 light  : TEXCOORD1; \n"
                             "}; \n"
                             "VSO main( VSI i ) { \n"
                             "   VSO o; \n"
                             "   o.pos = mul( u_wvp, i.pos ); \n"
                             "   o.clr = i.clr; \n"
                             "   o.normal = i.nml; \n"
                             "   float4 eye = mul( u_inv_wv, float4(0,0,0,1) ); \n"
                             "   o.light = (eye - i.pos).xyz; \n"
                             "   return o; \n"
                             "}";

static const char * pscode = "struct VSO { \n"
                             "   float4 pos    : POSITION0; \n"
                             "   float4 clr    : COLOR0;    \n"
                             "   float3 normal : TEXCOORD0; \n"
                             "   float3 light  : TEXCOORD1; \n"
                             "}; \n"
                             "float4 main( VSO i ) : COLOR0 { \n"
                             "   float3 N = normalize( i.normal ); \n"
                             "   float3 L = normalize( i.light ); \n"
                             "   float  d = dot( N, L ); \n"
                             "   return i.clr * d; \n"
                             "}";

static const D3DCOLOR SIDE_COLORS[4] = {
    // A R G B
    0xFFFF0000, // left   : red
    0xFFFFFF00, // bottom : yellow
    0xFF00FF00, // right  : green
    0xFF0000FF, // top    : blue
};

static const D3DCOLOR FAR_END_COLOR = 0xFFFFFFFF; // white

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
D3D9ViewFrustum::D3D9ViewFrustum(): m_Device(0), m_Vs(0), m_Ps(0), m_VbSolid(0), m_VbTrans(0) {
    memset(&m_Sides, 0, sizeof(m_Sides));
    memset(&m_FarEnd, 0, sizeof(m_FarEnd));
}

//
//
// -----------------------------------------------------------------------------
bool D3D9ViewFrustum::OnDeviceCreate(IDirect3DDevice9 * dev) {
    if (NULL == dev) {
        LOG_ERROR("Null device pointer!");
        return false;
    }

    // initialize shaders
    m_Vs = GN::d3d9::compileAndCreateVS(dev, vscode);
    m_Ps = GN::d3d9::compileAndCreatePS(dev, pscode);
    if (NULL == m_Vs || NULL == m_Ps) return false;

    m_Device = dev;
    return m_LineRenderer.onDeviceCreate(dev);
}

//
//
// -----------------------------------------------------------------------------
bool D3D9ViewFrustum::OnDeviceRestore() {
    if (NULL == m_Device) {
        LOG_ERROR("Null device pointer!");
        return false;
    }

    // create vertex buffer
    if (FAILED(m_Device->CreateVertexBuffer(VIEW_FRUSTUM_VB_SIZE, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_VbSolid, NULL)) ||
        FAILED(m_Device->CreateVertexBuffer(VIEW_FRUSTUM_VB_SIZE, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_VbTrans, NULL))) {
        LOG_ERROR("Fail to create vertex buffer.");
        return false;
    }

    return m_LineRenderer.onDeviceRestore();
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::OnDeviceDispose() {
    m_LineRenderer.onDeviceDispose();
    SAFE_RELEASE(m_VbSolid);
    SAFE_RELEASE(m_VbTrans);
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::OnDeviceDelete() {
    m_LineRenderer.onDeviceDelete();
    SAFE_RELEASE(m_Vs);
    SAFE_RELEASE(m_Ps);
    m_Device = 0;
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::UpdateViewFrustumRH(const XMVECTOR & eye, const XMVECTOR & at, const XMVECTOR & unalignedUp, float fovy, float ratio, float near,
                                          float far_) {
    XMVECTOR forward = XMVector3Normalize(at - eye);
    XMVECTOR right   = XMVector3Normalize(XMVector3Cross(forward, unalignedUp));
    XMVECTOR up      = XMVector3Normalize(XMVector3Cross(right, forward));

    float    farend_H  = tan(fovy / 2.0f) * far_;
    float    farend_W  = farend_H * ratio;
    XMVECTOR farend_C  = eye + (forward * far_);
    XMVECTOR farend_R  = right * farend_W;
    XMVECTOR farend_U  = up * farend_H;
    XMVECTOR farend_TL = farend_C - farend_R + farend_U;
    XMVECTOR farend_TR = farend_C + farend_R + farend_U;
    XMVECTOR farend_BL = farend_C - farend_R - farend_U;
    XMVECTOR farend_BR = farend_C + farend_R - farend_U;

    m_Sides[0].Set(eye, farend_TL, farend_BL); // left
    m_Sides[1].Set(eye, farend_BL, farend_BR); // bottom
    m_Sides[2].Set(eye, farend_BR, farend_TR); // right
    m_Sides[3].Set(eye, farend_TR, farend_TL); // top
    m_FarEnd.Set(farend_TL, farend_TR, farend_BR, farend_BL);
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::DrawRH(const XMMATRIX & worldViewRH, const XMMATRIX & projRH) {
    if (NULL == m_Device || NULL == m_VbSolid || NULL == m_VbTrans) return;

    // get eye position in world space
    XMVECTOR det;
    XMMATRIX invWV = XMMatrixInverse(&det, worldViewRH);
    XMVECTOR eye   = XMVector3Transform(XMVectorSet(0, 0, 0, 1), invWV);

    UINT     numSolidVertices = 0;
    Vertex * solidVertices;
    if (FAILED(m_VbSolid->Lock(0, 0, (void **) &solidVertices, D3DLOCK_DISCARD))) { return; }

    UINT     numTransVertices = 0;
    Vertex * transVertices;
    if (FAILED(m_VbTrans->Lock(0, 0, (void **) &transVertices, D3DLOCK_DISCARD))) {
        m_VbSolid->Unlock();
        return;
    }

    // for each side faces:
    for (size_t i = 0; i < ARRAYSIZE(m_Sides); ++i) {
        XMVECTOR lookAt = m_Sides[i].vertices[0] - eye;
        XMVECTOR dot    = XMVector3Dot(m_Sides[i].outterNormal, lookAt);
        if (XMVectorGetX(dot) > 0) {
            // show solid color
            D3DCOLOR c = SIDE_COLORS[i];

            numSolidVertices += BuildTriangleFaceVertices(solidVertices + numSolidVertices, m_Sides[i],
                                                          true, // flip the face, since we are looking from its back side
                                                          c);
        } else {
            // show transparency
            D3DCOLOR c = (SIDE_COLORS[i] & 0x00FFFFFF) | 0x40000000;
            numTransVertices += BuildTriangleFaceVertices(transVertices + numTransVertices, m_Sides[i],
                                                          false, // do not flip the face
                                                          c);
        }
    }

    // for end faces:
    if (XMVectorGetX(XMVector3Dot(m_FarEnd.outterNormal, m_FarEnd.vertices[0] - eye)) > 0) {
        // show solid color
        D3DCOLOR c = FAR_END_COLOR;
        numSolidVertices += BuildQuadFaceVertices(solidVertices + numSolidVertices, m_FarEnd,
                                                  true, // flip the face, since we are looking from its back side
                                                  c);
    } else {
        // show transparency
        D3DCOLOR c = (FAR_END_COLOR & 0x00FFFFFF) | 0x40000000;
        numTransVertices += BuildQuadFaceVertices(transVertices + numTransVertices, m_FarEnd,
                                                  false, // do not flip the face
                                                  c);
    }

    GN_ASSERT(numSolidVertices <= NUM_VIEW_FRUSTUM_VERTICES);
    GN_ASSERT(numTransVertices <= NUM_VIEW_FRUSTUM_VERTICES);

    // unlock the vertex buffers
    m_VbSolid->Unlock();
    m_VbTrans->Unlock();

    // store render states
    D3D9RenderStateSaver rss(m_Device);

    // setup common states
    XMMATRIX wvp = worldViewRH * projRH;
    rss.SetRS(D3DRS_CULLMODE, D3DCULL_NONE);
    m_Device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE);
    m_Device->SetVertexShaderConstantF(0, (const float *) &wvp, 4);
    m_Device->SetVertexShaderConstantF(4, (const float *) &invWV, 4);
    m_Device->SetVertexShader(m_Vs);
    m_Device->SetPixelShader(m_Ps);

    // draw solid faces
    if (numSolidVertices > 0) {
        m_Device->SetStreamSource(0, m_VbSolid, 0, sizeof(Vertex));
        m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, numSolidVertices / 3);
    }

    // draw transparent faces
    if (numTransVertices > 0) {
        rss.SetRS(D3DRS_ALPHABLENDENABLE, TRUE);
        rss.SetRS(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        rss.SetRS(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        rss.SetRS(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        rss.SetRS(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
        rss.SetRS(D3DRS_ZWRITEENABLE, FALSE);
        m_Device->SetStreamSource(0, m_VbTrans, 0, sizeof(Vertex));
        m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, numTransVertices / 3);
    }
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::TriangleFace::Set(const XMVECTOR & v0, const XMVECTOR & v1, const XMVECTOR & v2) {
    outterNormal = XMVector3Normalize(XMVector3Cross((v2 - v1), (v0 - v1)));
    vertices[0]  = v0;
    vertices[1]  = v1;
    vertices[2]  = v2;
}

//
//
// -----------------------------------------------------------------------------
void D3D9ViewFrustum::QuadFace::Set(const XMVECTOR & v0, const XMVECTOR & v1, const XMVECTOR & v2, const XMVECTOR & v3) {
    outterNormal = XMVector3Normalize(XMVector3Cross((v2 - v1), (v0 - v1)));
    vertices[0]  = v0;
    vertices[1]  = v1;
    vertices[2]  = v2;
    vertices[3]  = v3;
}

//
//
// -----------------------------------------------------------------------------
UINT D3D9ViewFrustum::BuildTriangleFaceVertices(Vertex * vertices, const TriangleFace & face, bool flip, D3DCOLOR c) {
    if (flip) {
        XMVECTOR n = -face.outterNormal;
        vertices[0].Set(face.vertices[2], n, c);
        vertices[1].Set(face.vertices[1], n, c);
        vertices[2].Set(face.vertices[0], n, c);
    } else {
        vertices[0].Set(face.vertices[0], face.outterNormal, c);
        vertices[1].Set(face.vertices[1], face.outterNormal, c);
        vertices[2].Set(face.vertices[2], face.outterNormal, c);
    }

    return 3;
}

//
//
// -----------------------------------------------------------------------------
UINT D3D9ViewFrustum::BuildQuadFaceVertices(Vertex * vertices, const QuadFace & face, bool flip, D3DCOLOR c) {
    if (flip) {
        XMVECTOR n = -face.outterNormal;

        vertices[0].Set(face.vertices[2], n, c);
        vertices[1].Set(face.vertices[1], n, c);
        vertices[2].Set(face.vertices[0], n, c);

        vertices[3].Set(face.vertices[3], n, c);
        vertices[4].Set(face.vertices[2], n, c);
        vertices[5].Set(face.vertices[0], n, c);
    } else {
        vertices[0].Set(face.vertices[0], face.outterNormal, c);
        vertices[1].Set(face.vertices[1], face.outterNormal, c);
        vertices[2].Set(face.vertices[2], face.outterNormal, c);

        vertices[3].Set(face.vertices[0], face.outterNormal, c);
        vertices[4].Set(face.vertices[2], face.outterNormal, c);
        vertices[5].Set(face.vertices[3], face.outterNormal, c);
    }

    return 6;
}
