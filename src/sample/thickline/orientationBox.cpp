#include "pch.h"
#include "orientationBox.h"
#include "garnet/GNd3d.h"

using namespace GN;
using namespace GN::d3d9;

// *****************************************************************************
// utilities
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.gfx.orientationbox");

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
// local code
// *****************************************************************************

struct SpriteVertex {
    float x, y, z, w;
    float u, v;
};

static const char * vscode = "float4x4 wvp : register(c0); \n"
                             "struct VSIO { \n"
                             "   float4 pos : POSITION0; \n"
                             "   float2 tex : TEXCOORD0; \n"
                             "   float4 clr : COLOR0; \n"
                             "}; \n"
                             "VSIO main( VSIO i ) { \n"
                             "   VSIO o; \n"
                             "   o.pos = mul( i.pos, wvp ); \n"
                             "   o.tex = i.tex; \n"
                             "   o.clr = i.clr; \n"
                             "   return o; \n"
                             "}";

static const char * pscode = "sampler s0: register(s0); \n"
                             "struct VSIO { \n"
                             "   float4 pos : POSITION0; \n"
                             "   float2 tex : TEXCOORD0; \n"
                             "   float4 clr : COLOR0; \n"
                             "}; \n"
                             "float4 main( VSIO i ) : COLOR0 { \n"
                             "   return tex2D( s0, i.tex ); \n"
                             "}";

//
//
// -----------------------------------------------------------------------------
IDirect3DTexture9 * CreateAxisTexture(IDirect3DDevice9 * dev, D3DCOLOR background, const char * text) {
    const UINT TEXW = 32;
    const UINT TEXH = 32;

    AutoComPtr<IDirect3DTexture9> tex;
    if (FAILED(dev->CreateTexture(TEXW, TEXH, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &tex, NULL))) { return NULL; }

    // clear texture to specific color
    D3DLOCKED_RECT lrc;
    if (FAILED(tex->LockRect(0, &lrc, NULL, 0))) {
        LOG_ERROR("Fail to lock D3D texture.");
        return NULL;
    }
    for (UINT y = 0; y < TEXH; ++y) {
        for (UINT x = 0; x < TEXW; ++x) {
            D3DCOLOR * p = (D3DCOLOR *) (((BYTE *) lrc.pBits) + y * lrc.Pitch + x * 4);
            *p           = background;
        }
    }
    tex->UnlockRect(0);

    // write text to texture
#if GN_XBOX2
    GN_UNIMPL_WARNING();
#else
    AutoComPtr<IDirect3DSurface9> surf;
    tex->GetSurfaceLevel(0, &surf);
    HDC dc;
    if (FAILED(surf->GetDC(&dc))) {
        LOG_ERROR("Fail to get surface DC.");
        return NULL;
    }
    RECT rc = {0, 0, (INT) TEXW, (INT) TEXH};
    DrawTextA(dc, text, -1, &rc, DT_CENTER | DT_VCENTER);
    surf->ReleaseDC(dc);
#endif

    // done
    return tex.detach();
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
D3D9OrientationBox::D3D9OrientationBox(float boxSizeInScreenSpace)
    : m_BoxSize(boxSizeInScreenSpace), m_Device(0), m_Vs(0), m_Ps(0), m_RtTexture(0), m_RtSurface(0), m_ZSurface(0) {
    memset(m_Textures, 0, sizeof(m_Textures));

    // build box vertices (right handed)

    // +x
    m_BoxFaces[0].vertices[0].Set(+1, -1, +1, 0, 0, 0xFFFF0000);
    m_BoxFaces[0].vertices[1].Set(+1, -1, -1, 0, 1, 0xFFFF0000);
    m_BoxFaces[0].vertices[2].Set(+1, +1, +1, 1, 0, 0xFFFF0000);
    m_BoxFaces[0].vertices[3].Set(+1, +1, -1, 1, 1, 0xFFFF0000);

    // -x
    m_BoxFaces[1].vertices[0].Set(-1, +1, +1, 0, 0, 0xFF800000);
    m_BoxFaces[1].vertices[1].Set(-1, +1, -1, 0, 1, 0xFF800000);
    m_BoxFaces[1].vertices[2].Set(-1, -1, +1, 1, 0, 0xFF800000);
    m_BoxFaces[1].vertices[3].Set(-1, -1, -1, 1, 1, 0xFF800000);

    // +y
    m_BoxFaces[2].vertices[0].Set(+1, +1, -1, 0, 0, 0xFF00FF00);
    m_BoxFaces[2].vertices[1].Set(-1, +1, -1, 0, 1, 0xFF00FF00);
    m_BoxFaces[2].vertices[2].Set(+1, +1, +1, 1, 0, 0xFF00FF00);
    m_BoxFaces[2].vertices[3].Set(-1, +1, +1, 1, 1, 0xFF00FF00);

    // -y
    m_BoxFaces[3].vertices[0].Set(+1, -1, +1, 0, 0, 0xFF008000);
    m_BoxFaces[3].vertices[1].Set(-1, -1, +1, 0, 1, 0xFF008000);
    m_BoxFaces[3].vertices[2].Set(+1, -1, -1, 1, 0, 0xFF008000);
    m_BoxFaces[3].vertices[3].Set(-1, -1, -1, 1, 1, 0xFF008000);

    // +z
    m_BoxFaces[4].vertices[0].Set(-1, +1, +1, 0, 0, 0xFF0000FF);
    m_BoxFaces[4].vertices[1].Set(-1, -1, +1, 0, 1, 0xFF0000FF);
    m_BoxFaces[4].vertices[2].Set(+1, +1, +1, 1, 0, 0xFF0000FF);
    m_BoxFaces[4].vertices[3].Set(+1, -1, +1, 1, 1, 0xFF0000FF);

    // -z
    m_BoxFaces[5].vertices[0].Set(+1, +1, -1, 0, 0, 0xFF000080);
    m_BoxFaces[5].vertices[1].Set(+1, -1, -1, 0, 1, 0xFF000080);
    m_BoxFaces[5].vertices[2].Set(-1, +1, -1, 1, 0, 0xFF000080);
    m_BoxFaces[5].vertices[3].Set(-1, -1, -1, 1, 1, 0xFF000080);
}

//
//
// -----------------------------------------------------------------------------
bool D3D9OrientationBox::OnDeviceCreate(IDirect3DDevice9 * dev) {
    if (NULL == dev) {
        LOG_ERROR("Null device pointer!");
        return false;
    }

    // initialize shaders
    UINT flag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
    m_Vs      = GN::d3d9::compileAndCreateVS(dev, vscode, 0, flag);
    m_Ps      = GN::d3d9::compileAndCreatePS(dev, pscode);
    if (NULL == m_Vs || NULL == m_Ps) return false;

    // create textures
    m_Textures[0] = CreateAxisTexture(dev, 0xFFFF0000, "+X");
    m_Textures[1] = CreateAxisTexture(dev, 0xFF800000, "-X");
    m_Textures[2] = CreateAxisTexture(dev, 0xFF00FF00, "+Y");
    m_Textures[3] = CreateAxisTexture(dev, 0xFF008000, "-Y");
    m_Textures[4] = CreateAxisTexture(dev, 0xFF0000FF, "+Z");
    m_Textures[5] = CreateAxisTexture(dev, 0xFF000080, "-Z");

    m_Device = dev;
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool D3D9OrientationBox::OnDeviceRestore() {
    if (NULL == m_Device) {
        LOG_ERROR("Null device pointer!");
        return false;
    }

    if (FAILED(m_Device->CreateTexture((UINT) m_BoxSize, (UINT) m_BoxSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RtTexture, NULL))) {
        LOG_ERROR("Fail to create render target texture.");
        return false;
    }

    m_RtTexture->GetSurfaceLevel(0, &m_RtSurface);

    if (FAILED(m_Device->CreateDepthStencilSurface((UINT) m_BoxSize, (UINT) m_BoxSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_ZSurface, NULL))) {
        LOG_ERROR("Fail to create depth stencil surface.");
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void D3D9OrientationBox::OnDeviceDispose() {
    SAFE_RELEASE(m_ZSurface);
    SAFE_RELEASE(m_RtSurface);
    SAFE_RELEASE(m_RtTexture);
}

//
//
// -----------------------------------------------------------------------------
void D3D9OrientationBox::OnDeviceDelete() {
    for (int i = 0; i < 6; ++i) { SAFE_RELEASE(m_Textures[i]); }
    SAFE_RELEASE(m_Vs);
    SAFE_RELEASE(m_Ps);
    m_Device = 0;
}

Matrix44f ToMatrix44f(CXMMATRIX m);

//
//
// -----------------------------------------------------------------------------
void D3D9OrientationBox::Draw(float x, float y, const XMMATRIX & viewRH) {
    GN_UNUSED_PARAM(x);
    GN_UNUSED_PARAM(y);

    if (NULL == m_Device) {
        LOG_ERROR("Null D3D device.");
        return;
    }

    // store device states
    D3DVIEWPORT9                  oldvp;
    AutoComPtr<IDirect3DSurface9> oldrt, oldz;
    m_Device->GetViewport(&oldvp);
    m_Device->GetRenderTarget(0, &oldrt);
    m_Device->GetDepthStencilSurface(&oldz);

    D3D9RenderStateSaver stateSaver(m_Device);

    // calculate transformation matrix for the box
    float    distance = 10.0f;
    XMVECTOR scale, rotation, translation;
    XMMatrixDecompose(&scale, &rotation, &translation, viewRH);
    XMMATRIX  world     = XMMatrixRotationQuaternion(rotation);
    XMMATRIX  view      = XMMatrixTranslation(0, 0, -distance);
    XMMATRIX  proj      = XMMatrixPerspectiveRH(2.0f, 2.0f, distance - 4.0f, distance + 4.0f);
    Matrix44f transform = ToMatrix44f(world * view * proj);

    // draw the box to private render target
    stateSaver.SetRS(D3DRS_CULLMODE, D3DCULL_NONE);
    stateSaver.SetRS(D3DRS_ZENABLE, TRUE);
    stateSaver.SetRS(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    m_Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    m_Device->SetVertexShaderConstantF(0, (const float *) transform.rows, 4);
    m_Device->SetVertexShader(m_Vs);

    m_Device->SetPixelShader(m_Ps);
    m_Device->SetTexture(0, 0);

    D3DVIEWPORT9 newvp = {0, 0, (UINT) m_BoxSize, (UINT) m_BoxSize, 0.0f, 1.0f};
    m_Device->SetRenderTarget(0, m_RtSurface);
    m_Device->SetDepthStencilSurface(m_ZSurface);
    m_Device->SetViewport(&newvp);

    m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    for (size_t i = 0; i < 6; ++i) {
        m_Device->SetTexture(0, m_Textures[i]);
        m_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
                                  2, // primitive count
                                  m_BoxFaces[i].vertices, sizeof(BoxVertex));
    }

    // draw the render target texture to original render target
    m_Device->SetRenderTarget(0, oldrt);
    m_Device->SetDepthStencilSurface(oldz);
    m_Device->SetViewport(&oldvp);

    stateSaver.SetRS(D3DRS_ZENABLE, FALSE);
    stateSaver.SetRS(D3DRS_ALPHABLENDENABLE, TRUE);
    stateSaver.SetRS(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    stateSaver.SetRS(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    stateSaver.SetRS(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

#if GN_XBOX2
    GN_UNIMPL_WARNING();
#else
    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
    m_Device->SetVertexShader(NULL);

    m_Device->SetTexture(0, m_RtTexture);
    stateSaver.SetSS(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    stateSaver.SetSS(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    stateSaver.SetSS(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    stateSaver.SetTSS(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    stateSaver.SetTSS(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_Device->SetPixelShader(NULL);

    // compose sprite vertex buffer
    float        centerX     = x;
    float        centerY     = y;
    float        halfSpriteW = m_BoxSize / 2.0f;
    float        halfSpriteH = m_BoxSize / 2.0f;
    SpriteVertex sprite[]    = {
        // x                            y                             z     w     u     v
        {centerX - halfSpriteW - 0.5f, centerY - halfSpriteH + 0.5f, 0.0f, 1.0f, 0.0f, 0.0f}, // left top
        {centerX - halfSpriteW - 0.5f, centerY + halfSpriteH + 0.5f, 0.0f, 1.0f, 0.0f, 1.0f}, // left bottoom
        {centerX + halfSpriteW - 0.5f, centerY - halfSpriteH + 0.5f, 0.0f, 1.0f, 1.0f, 0.0f}, // right top
        {centerX + halfSpriteW - 0.5f, centerY + halfSpriteH + 0.5f, 0.0f, 1.0f, 1.0f, 1.0f}, // right bottoom
    };

    m_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
                              2, // primitive count
                              sprite, sizeof(SpriteVertex));
#endif
}

// *****************************************************************************
// private method
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
