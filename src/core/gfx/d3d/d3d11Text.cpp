#include "pch.h"

#if GN_BUILD_HAS_D3D11

#include <dwrite.h>
#include <d2d1.h>

using namespace GN;

#pragma warning(disable:4100)  // unreferenced formal parameter

static Logger * sLogger = GN::getLogger("GN.d3d11.TextRenderer");

class GN::d3d11::TextRenderer::Impl : public IDWriteTextRenderer
{
    AutoComPtr<IDWriteFactory>        _dWriteFactory;
    AutoComPtr<IDWriteTextFormat>     _textFormat;
    AutoComPtr<ID2D1Factory>          _d2dFactory;
    AutoComPtr<IUnknown>              _d3drt0;
    AutoComPtr<ID2D1RenderTarget>     _rt;
    AutoComPtr<ID2D1SolidColorBrush>  _brush;
    DWORD                             _textColor;

    // -----------------------------------------------------------------------------
    ID2D1RenderTarget * GetD2DRenderTarget(ID3D11DeviceContext & cxt)
    {
        AutoComPtr<ID3D11RenderTargetView> rtv0;
        cxt.OMGetRenderTargets(1, &rtv0, nullptr);
        if(rtv0.empty()) return nullptr;

        AutoComPtr<ID3D11Resource> rt0;
        rtv0->GetResource(&rt0);

        AutoComPtr<IUnknown> unknwon0;
        rt0->QueryInterface(&unknwon0);
        GN_ASSERT(nullptr != unknwon0);
        if(unknwon0 == _d3drt0)
        {
            GN_ASSERT(_rt);
            return _rt;
        }

        AutoComPtr<IDXGISurface> dxgisurface;
        GN_RETURN_NULL_ON_HR_FAILED(rt0->QueryInterface(&dxgisurface));

        //FLOAT dpiX, dpiY;
        //_d2dFactory->GetDesktopDpi(&dpiX, &dpiY);
        auto dpi = (float)GetDpiForSystem();

        auto properties = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
            dpi,
            dpi);

        AutoComPtr<ID2D1RenderTarget> rt;
        GN_RETURN_NULL_ON_HR_FAILED(_d2dFactory->CreateDxgiSurfaceRenderTarget(
            dxgisurface,
            properties,
            &rt));

        // success
        _d3drt0 = unknwon0;
        _rt = rt;
        return _rt;
    }

    ID2D1Brush * GetD2DBrush(DWORD color)
    {
        if (_textColor == color && nullptr != _brush)
        {
            return _brush;
        }

        D2D1_COLOR_F c = {
            (float)((color>> 0)&0xFF) / 255.0f,
            (float)((color>> 8)&0xFF) / 255.0f,
            (float)((color>>16)&0xFF) / 255.0f,
            (float)((color>>24)&0xFF) / 255.0f,
        };

        AutoComPtr<ID2D1SolidColorBrush> brush;
        GN_RETURN_NULL_ON_HR_FAILED(_rt->CreateSolidColorBrush(
            c,
            &brush));

        _textColor = color;
        _brush = brush;
        return _brush;
    }


public:

    // -----------------------------------------------------------------------------
    Impl()
    {
    }

    // -----------------------------------------------------------------------------
    ~Impl()
    {
    }

    // -----------------------------------------------------------------------------
    bool Initialize(ID3D11Device &, const FontDesc & fontdesc)
    {
        // dwrite stuff
        GN_RETURN_FALSE_ON_HR_FAILED(DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&_dWriteFactory)));
        GN_RETURN_FALSE_ON_HR_FAILED(_dWriteFactory->CreateTextFormat(
                fontdesc.face,                // Font family name.
                NULL,                         // Font collection (NULL sets it to use the system font collection).
                DWRITE_FONT_WEIGHT_REGULAR,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                fontdesc.height,
                L"en-us",
                &_textFormat));

        // Left align (horizontally) the text.
        GN_RETURN_FALSE_ON_HR_FAILED(_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
        GN_RETURN_FALSE_ON_HR_FAILED(_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

        // Create D2D factory
        D2D1_FACTORY_OPTIONS d2dfo;
        d2dfo.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
        GN_RETURN_FALSE_ON_HR_FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            d2dfo,
            &_d2dFactory));

        // done
        return true;
    }

    // -----------------------------------------------------------------------------
    void Draw2DText(ID3D11DeviceContext & cxt, const TextDesc & td)
    {
        // skip empty text.
        if (str::isEmpty(td.text)) return;

        auto rt = GetD2DRenderTarget(cxt);
        if (nullptr == rt) return;

        auto brush = GetD2DBrush(td.color);
        if (nullptr == brush) return;

        D2D1_RECT_F rc = {td.x, td.y, 1920, 1080};

        rt->BeginDraw();
        rt->DrawText(
            td.text,
            (0 == td.len)? (UINT)wcslen(td.text): (UINT)td.len,
            _textFormat,
            &rc,
            brush);
        rt->EndDraw();
    }

    // -----------------------------------------------------------------------------
    // IUnknown

    virtual ULONG STDMETHODCALLTYPE AddRef() { return 1; }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject)
    {
        if (nullptr == ppvObject)
        {
            return E_POINTER;
        }
        else if (riid == __uuidof(IUnknown) ||
                 riid == __uuidof(IDWriteTextRenderer) ||
                 riid == __uuidof(IDWritePixelSnapping))
        {
            *ppvObject = this;
            return S_OK;
        }
        else
        {
            *ppvObject = nullptr;
            return E_NOINTERFACE;
        }
    }

    virtual ULONG STDMETHODCALLTYPE Release() { return 1; }

    // -----------------------------------------------------------------------------
    // IDWriteTextRenderer

    virtual HRESULT STDMETHODCALLTYPE DrawGlyphRun(
        void * clientDrawingContext,
        FLOAT  baselineOriginX,
        FLOAT  baselineOriginY,
        DWRITE_MEASURING_MODE  measuringMode,
        const DWRITE_GLYPH_RUN * glyphRun,
        const DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription,
        IUnknown * clientDrawingEffect
        )
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE DrawInlineObject(
        void * clientDrawingContext,
        FLOAT  originX,
        FLOAT  originY,
        IDWriteInlineObject * inlineObject,
        BOOL  isSideways,
        BOOL  isRightToLeft,
        IUnknown * clientDrawingEffect
        )
    {
        // not implemented yet.
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE DrawStrikethrough(
        void * clientDrawingContext,
        FLOAT  baselineOriginX,
        FLOAT  baselineOriginY,
        const DWRITE_STRIKETHROUGH * strikethrough,
        IUnknown * clientDrawingEffect
        )
    {
        // not implemented yet.
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE DrawUnderline(
        void * clientDrawingContext,
        FLOAT  baselineOriginX,
        FLOAT  baselineOriginY,
        const DWRITE_UNDERLINE * underline,
        IUnknown * clientDrawingEffect
    )
    {
        // not implemented yet.
        return S_OK;
    }

    // -----------------------------------------------------------------------------
    // IDWritePixelSnapping

    /// <summary>
    /// Determines whether pixel snapping is disabled. The recommended default is FALSE,
    /// unless doing animation that requires subpixel vertical placement.
    /// </summary>
    /// <param name="clientDrawingContext">The context passed to IDWriteTextLayout::Draw.</param>
    /// <param name="isDisabled">Receives TRUE if pixel snapping is disabled or FALSE if it not.</param>
    /// <returns>
    /// Standard HRESULT error code.
    /// </returns>
    STDMETHOD(IsPixelSnappingDisabled)(
        _In_opt_ void* clientDrawingContext,
        _Out_ BOOL* isDisabled
        )
    {
        *isDisabled = FALSE; // Means enabled.
        return S_OK;
    }

    /// <summary>
    /// Gets the current transform that maps abstract coordinates to DIPs,
    /// which may disable pixel snapping upon any rotation or shear.
    /// </summary>
    /// <param name="clientDrawingContext">The context passed to IDWriteTextLayout::Draw.</param>
    /// <param name="transform">Receives the transform.</param>
    /// <returns>
    /// Standard HRESULT error code.
    /// </returns>
    STDMETHOD(GetCurrentTransform)(
        _In_opt_ void* clientDrawingContext,
        _Out_ DWRITE_MATRIX* transform
        )
    {
        transform->m11 = 1.0f;
        transform->m12 = 0.0f;
        transform->m21 = 0.0f;
        transform->m22 = 1.0f;
        transform->dx = 0.0f;
        transform->dy = 0.0f;
        return S_OK;
    }

    /// <summary>
    /// Gets the number of physical pixels per DIP. A DIP (device-independent pixel) is 1/96 inch,
    /// so the pixelsPerDip value is the number of logical pixels per inch divided by 96 (yielding
    /// a value of 1 for 96 DPI and 1.25 for 120).
    /// </summary>
    /// <param name="clientDrawingContext">The context passed to IDWriteTextLayout::Draw.</param>
    /// <param name="pixelsPerDip">Receives the number of physical pixels per DIP.</param>
    /// <returns>
    /// Standard HRESULT error code.
    /// </returns>
    STDMETHOD(GetPixelsPerDip)(
        _In_opt_ void* clientDrawingContext,
        _Out_ FLOAT* pixelsPerDip
        )
    {
        *pixelsPerDip = 1;
        return S_OK;
    }
};

// -----------------------------------------------------------------------------
GN::d3d11::TextRenderer::TextRenderer() : _impl(nullptr)
{
}

// -----------------------------------------------------------------------------
GN::d3d11::TextRenderer::~TextRenderer()
{
    shutdown();
}

// -----------------------------------------------------------------------------
bool GN::d3d11::TextRenderer::initialize(ID3D11Device & dev, FontDesc & font)
{
    std::unique_ptr<Impl> impl(new Impl);
    if (nullptr == impl) { GN_ERROR(sLogger)("Out of memory."); return false; }
    if (!impl->Initialize(dev, font)) return false;
    _impl = impl.release();
    return true;
}

// -----------------------------------------------------------------------------
void GN::d3d11::TextRenderer::shutdown()
{
    delete _impl;
    _impl = nullptr;
}

// -----------------------------------------------------------------------------
void GN::d3d11::TextRenderer::draw2DText(ID3D11DeviceContext & cxt, const TextDesc & text) const
{
    if(nullptr != _impl)
    {
        _impl->Draw2DText(cxt, text);
    }
}

#endif // GN_BUILD_HAS_D3D11
