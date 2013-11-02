// script generated file. Do _NOT_ edit.

#include "pch.h"
#include "d2dhooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1ResourceHook::GetFactory(
    _Outptr_ ID2D1Factory ** factory) const
{
    calltrace::AutoTrace trace(L"D2D1ResourceHook::GetFactory");
    if (_GetFactory_pre_ptr._value) { (this->*_GetFactory_pre_ptr._value)(factory); }
    GetRealObj()->GetFactory(factory);
    if ( factory && *factory) { *factory = RealToHooked_D2D( *factory ); }
    if (_GetFactory_post_ptr._value) { (this->*_GetFactory_post_ptr._value)(factory); }
}

// -----------------------------------------------------------------------------
D2D1_SIZE_F STDMETHODCALLTYPE D2D1BitmapHook::GetSize() const
{
    calltrace::AutoTrace trace(L"D2D1BitmapHook::GetSize");
    if (_GetSize_pre_ptr._value) { (this->*_GetSize_pre_ptr._value)(); }
    D2D1_SIZE_F ret = GetRealObj()->GetSize();
    if (_GetSize_post_ptr._value) { (this->*_GetSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_SIZE_U STDMETHODCALLTYPE D2D1BitmapHook::GetPixelSize() const
{
    calltrace::AutoTrace trace(L"D2D1BitmapHook::GetPixelSize");
    if (_GetPixelSize_pre_ptr._value) { (this->*_GetPixelSize_pre_ptr._value)(); }
    D2D1_SIZE_U ret = GetRealObj()->GetPixelSize();
    if (_GetPixelSize_post_ptr._value) { (this->*_GetPixelSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_PIXEL_FORMAT STDMETHODCALLTYPE D2D1BitmapHook::GetPixelFormat() const
{
    calltrace::AutoTrace trace(L"D2D1BitmapHook::GetPixelFormat");
    if (_GetPixelFormat_pre_ptr._value) { (this->*_GetPixelFormat_pre_ptr._value)(); }
    D2D1_PIXEL_FORMAT ret = GetRealObj()->GetPixelFormat();
    if (_GetPixelFormat_post_ptr._value) { (this->*_GetPixelFormat_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BitmapHook::GetDpi(
    _Out_ FLOAT * dpiX,
    _Out_ FLOAT * dpiY) const
{
    calltrace::AutoTrace trace(L"D2D1BitmapHook::GetDpi");
    if (_GetDpi_pre_ptr._value) { (this->*_GetDpi_pre_ptr._value)(dpiX, dpiY); }
    GetRealObj()->GetDpi(dpiX, dpiY);
    if (_GetDpi_post_ptr._value) { (this->*_GetDpi_post_ptr._value)(dpiX, dpiY); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1BitmapHook::CopyFromBitmap(
    _In_opt_ CONST D2D1_POINT_2U * destPoint,
    _In_ ID2D1Bitmap * bitmap,
    _In_opt_ CONST D2D1_RECT_U * srcRect)
{
    calltrace::AutoTrace trace(L"D2D1BitmapHook::CopyFromBitmap");
    if (_CopyFromBitmap_pre_ptr._value) { (this->*_CopyFromBitmap_pre_ptr._value)(destPoint, bitmap, srcRect); }
    HRESULT ret = GetRealObj()->CopyFromBitmap(destPoint, HookedToReal(bitmap), srcRect);
    if (_CopyFromBitmap_post_ptr._value) { (this->*_CopyFromBitmap_post_ptr._value)(ret, destPoint, bitmap, srcRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1BitmapHook::CopyFromRenderTarget(
    _In_opt_ CONST D2D1_POINT_2U * destPoint,
    _In_ ID2D1RenderTarget * renderTarget,
    _In_opt_ CONST D2D1_RECT_U * srcRect)
{
    calltrace::AutoTrace trace(L"D2D1BitmapHook::CopyFromRenderTarget");
    if (_CopyFromRenderTarget_pre_ptr._value) { (this->*_CopyFromRenderTarget_pre_ptr._value)(destPoint, renderTarget, srcRect); }
    HRESULT ret = GetRealObj()->CopyFromRenderTarget(destPoint, HookedToReal(renderTarget), srcRect);
    if (_CopyFromRenderTarget_post_ptr._value) { (this->*_CopyFromRenderTarget_post_ptr._value)(ret, destPoint, renderTarget, srcRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1BitmapHook::CopyFromMemory(
    _In_opt_ CONST D2D1_RECT_U * dstRect,
    _In_ CONST void * srcData,
    UINT32 pitch)
{
    calltrace::AutoTrace trace(L"D2D1BitmapHook::CopyFromMemory");
    if (_CopyFromMemory_pre_ptr._value) { (this->*_CopyFromMemory_pre_ptr._value)(dstRect, srcData, pitch); }
    HRESULT ret = GetRealObj()->CopyFromMemory(dstRect, srcData, pitch);
    if (_CopyFromMemory_post_ptr._value) { (this->*_CopyFromMemory_post_ptr._value)(ret, dstRect, srcData, pitch); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1GradientStopCollectionHook::GetGradientStopCount() const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollectionHook::GetGradientStopCount");
    if (_GetGradientStopCount_pre_ptr._value) { (this->*_GetGradientStopCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetGradientStopCount();
    if (_GetGradientStopCount_post_ptr._value) { (this->*_GetGradientStopCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1GradientStopCollectionHook::GetGradientStops(
    _Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP * gradientStops,
    UINT32 gradientStopsCount) const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollectionHook::GetGradientStops");
    if (_GetGradientStops_pre_ptr._value) { (this->*_GetGradientStops_pre_ptr._value)(gradientStops, gradientStopsCount); }
    GetRealObj()->GetGradientStops(gradientStops, gradientStopsCount);
    if (_GetGradientStops_post_ptr._value) { (this->*_GetGradientStops_post_ptr._value)(gradientStops, gradientStopsCount); }
}

// -----------------------------------------------------------------------------
D2D1_GAMMA STDMETHODCALLTYPE D2D1GradientStopCollectionHook::GetColorInterpolationGamma() const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollectionHook::GetColorInterpolationGamma");
    if (_GetColorInterpolationGamma_pre_ptr._value) { (this->*_GetColorInterpolationGamma_pre_ptr._value)(); }
    D2D1_GAMMA ret = GetRealObj()->GetColorInterpolationGamma();
    if (_GetColorInterpolationGamma_post_ptr._value) { (this->*_GetColorInterpolationGamma_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_EXTEND_MODE STDMETHODCALLTYPE D2D1GradientStopCollectionHook::GetExtendMode() const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollectionHook::GetExtendMode");
    if (_GetExtendMode_pre_ptr._value) { (this->*_GetExtendMode_pre_ptr._value)(); }
    D2D1_EXTEND_MODE ret = GetRealObj()->GetExtendMode();
    if (_GetExtendMode_post_ptr._value) { (this->*_GetExtendMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BrushHook::SetOpacity(
    FLOAT opacity)
{
    calltrace::AutoTrace trace(L"D2D1BrushHook::SetOpacity");
    if (_SetOpacity_pre_ptr._value) { (this->*_SetOpacity_pre_ptr._value)(opacity); }
    GetRealObj()->SetOpacity(opacity);
    if (_SetOpacity_post_ptr._value) { (this->*_SetOpacity_post_ptr._value)(opacity); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BrushHook::SetTransform(
    _In_ CONST D2D1_MATRIX_3X2_F * transform)
{
    calltrace::AutoTrace trace(L"D2D1BrushHook::SetTransform");
    if (_SetTransform_pre_ptr._value) { (this->*_SetTransform_pre_ptr._value)(transform); }
    GetRealObj()->SetTransform(transform);
    if (_SetTransform_post_ptr._value) { (this->*_SetTransform_post_ptr._value)(transform); }
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE D2D1BrushHook::GetOpacity() const
{
    calltrace::AutoTrace trace(L"D2D1BrushHook::GetOpacity");
    if (_GetOpacity_pre_ptr._value) { (this->*_GetOpacity_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetOpacity();
    if (_GetOpacity_post_ptr._value) { (this->*_GetOpacity_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BrushHook::GetTransform(
    _Out_ D2D1_MATRIX_3X2_F * transform) const
{
    calltrace::AutoTrace trace(L"D2D1BrushHook::GetTransform");
    if (_GetTransform_pre_ptr._value) { (this->*_GetTransform_pre_ptr._value)(transform); }
    GetRealObj()->GetTransform(transform);
    if (_GetTransform_post_ptr._value) { (this->*_GetTransform_post_ptr._value)(transform); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BitmapBrushHook::SetExtendModeX(
    D2D1_EXTEND_MODE extendModeX)
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrushHook::SetExtendModeX");
    if (_SetExtendModeX_pre_ptr._value) { (this->*_SetExtendModeX_pre_ptr._value)(extendModeX); }
    GetRealObj()->SetExtendModeX(extendModeX);
    if (_SetExtendModeX_post_ptr._value) { (this->*_SetExtendModeX_post_ptr._value)(extendModeX); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BitmapBrushHook::SetExtendModeY(
    D2D1_EXTEND_MODE extendModeY)
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrushHook::SetExtendModeY");
    if (_SetExtendModeY_pre_ptr._value) { (this->*_SetExtendModeY_pre_ptr._value)(extendModeY); }
    GetRealObj()->SetExtendModeY(extendModeY);
    if (_SetExtendModeY_post_ptr._value) { (this->*_SetExtendModeY_post_ptr._value)(extendModeY); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BitmapBrushHook::SetInterpolationMode(
    D2D1_BITMAP_INTERPOLATION_MODE interpolationMode)
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrushHook::SetInterpolationMode");
    if (_SetInterpolationMode_pre_ptr._value) { (this->*_SetInterpolationMode_pre_ptr._value)(interpolationMode); }
    GetRealObj()->SetInterpolationMode(interpolationMode);
    if (_SetInterpolationMode_post_ptr._value) { (this->*_SetInterpolationMode_post_ptr._value)(interpolationMode); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BitmapBrushHook::SetBitmap(
    _In_opt_ ID2D1Bitmap * bitmap)
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrushHook::SetBitmap");
    if (_SetBitmap_pre_ptr._value) { (this->*_SetBitmap_pre_ptr._value)(bitmap); }
    GetRealObj()->SetBitmap(HookedToReal(bitmap));
    if (_SetBitmap_post_ptr._value) { (this->*_SetBitmap_post_ptr._value)(bitmap); }
}

// -----------------------------------------------------------------------------
D2D1_EXTEND_MODE STDMETHODCALLTYPE D2D1BitmapBrushHook::GetExtendModeX() const
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrushHook::GetExtendModeX");
    if (_GetExtendModeX_pre_ptr._value) { (this->*_GetExtendModeX_pre_ptr._value)(); }
    D2D1_EXTEND_MODE ret = GetRealObj()->GetExtendModeX();
    if (_GetExtendModeX_post_ptr._value) { (this->*_GetExtendModeX_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_EXTEND_MODE STDMETHODCALLTYPE D2D1BitmapBrushHook::GetExtendModeY() const
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrushHook::GetExtendModeY");
    if (_GetExtendModeY_pre_ptr._value) { (this->*_GetExtendModeY_pre_ptr._value)(); }
    D2D1_EXTEND_MODE ret = GetRealObj()->GetExtendModeY();
    if (_GetExtendModeY_post_ptr._value) { (this->*_GetExtendModeY_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_BITMAP_INTERPOLATION_MODE STDMETHODCALLTYPE D2D1BitmapBrushHook::GetInterpolationMode() const
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrushHook::GetInterpolationMode");
    if (_GetInterpolationMode_pre_ptr._value) { (this->*_GetInterpolationMode_pre_ptr._value)(); }
    D2D1_BITMAP_INTERPOLATION_MODE ret = GetRealObj()->GetInterpolationMode();
    if (_GetInterpolationMode_post_ptr._value) { (this->*_GetInterpolationMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BitmapBrushHook::GetBitmap(
    _Outptr_ ID2D1Bitmap ** bitmap) const
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrushHook::GetBitmap");
    if (_GetBitmap_pre_ptr._value) { (this->*_GetBitmap_pre_ptr._value)(bitmap); }
    GetRealObj()->GetBitmap(bitmap);
    if ( bitmap && *bitmap) { *bitmap = RealToHooked_D2D( *bitmap ); }
    if (_GetBitmap_post_ptr._value) { (this->*_GetBitmap_post_ptr._value)(bitmap); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1SolidColorBrushHook::SetColor(
    _In_ CONST D2D1_COLOR_F * color)
{
    calltrace::AutoTrace trace(L"D2D1SolidColorBrushHook::SetColor");
    if (_SetColor_pre_ptr._value) { (this->*_SetColor_pre_ptr._value)(color); }
    GetRealObj()->SetColor(color);
    if (_SetColor_post_ptr._value) { (this->*_SetColor_post_ptr._value)(color); }
}

// -----------------------------------------------------------------------------
D2D1_COLOR_F STDMETHODCALLTYPE D2D1SolidColorBrushHook::GetColor() const
{
    calltrace::AutoTrace trace(L"D2D1SolidColorBrushHook::GetColor");
    if (_GetColor_pre_ptr._value) { (this->*_GetColor_pre_ptr._value)(); }
    D2D1_COLOR_F ret = GetRealObj()->GetColor();
    if (_GetColor_post_ptr._value) { (this->*_GetColor_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1LinearGradientBrushHook::SetStartPoint(
    D2D1_POINT_2F startPoint)
{
    calltrace::AutoTrace trace(L"D2D1LinearGradientBrushHook::SetStartPoint");
    if (_SetStartPoint_pre_ptr._value) { (this->*_SetStartPoint_pre_ptr._value)(startPoint); }
    GetRealObj()->SetStartPoint(startPoint);
    if (_SetStartPoint_post_ptr._value) { (this->*_SetStartPoint_post_ptr._value)(startPoint); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1LinearGradientBrushHook::SetEndPoint(
    D2D1_POINT_2F endPoint)
{
    calltrace::AutoTrace trace(L"D2D1LinearGradientBrushHook::SetEndPoint");
    if (_SetEndPoint_pre_ptr._value) { (this->*_SetEndPoint_pre_ptr._value)(endPoint); }
    GetRealObj()->SetEndPoint(endPoint);
    if (_SetEndPoint_post_ptr._value) { (this->*_SetEndPoint_post_ptr._value)(endPoint); }
}

// -----------------------------------------------------------------------------
D2D1_POINT_2F STDMETHODCALLTYPE D2D1LinearGradientBrushHook::GetStartPoint() const
{
    calltrace::AutoTrace trace(L"D2D1LinearGradientBrushHook::GetStartPoint");
    if (_GetStartPoint_pre_ptr._value) { (this->*_GetStartPoint_pre_ptr._value)(); }
    D2D1_POINT_2F ret = GetRealObj()->GetStartPoint();
    if (_GetStartPoint_post_ptr._value) { (this->*_GetStartPoint_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_POINT_2F STDMETHODCALLTYPE D2D1LinearGradientBrushHook::GetEndPoint() const
{
    calltrace::AutoTrace trace(L"D2D1LinearGradientBrushHook::GetEndPoint");
    if (_GetEndPoint_pre_ptr._value) { (this->*_GetEndPoint_pre_ptr._value)(); }
    D2D1_POINT_2F ret = GetRealObj()->GetEndPoint();
    if (_GetEndPoint_post_ptr._value) { (this->*_GetEndPoint_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1LinearGradientBrushHook::GetGradientStopCollection(
    _Outptr_ ID2D1GradientStopCollection ** gradientStopCollection) const
{
    calltrace::AutoTrace trace(L"D2D1LinearGradientBrushHook::GetGradientStopCollection");
    if (_GetGradientStopCollection_pre_ptr._value) { (this->*_GetGradientStopCollection_pre_ptr._value)(gradientStopCollection); }
    GetRealObj()->GetGradientStopCollection(gradientStopCollection);
    if ( gradientStopCollection && *gradientStopCollection) { *gradientStopCollection = RealToHooked_D2D( *gradientStopCollection ); }
    if (_GetGradientStopCollection_post_ptr._value) { (this->*_GetGradientStopCollection_post_ptr._value)(gradientStopCollection); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RadialGradientBrushHook::SetCenter(
    D2D1_POINT_2F center)
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::SetCenter");
    if (_SetCenter_pre_ptr._value) { (this->*_SetCenter_pre_ptr._value)(center); }
    GetRealObj()->SetCenter(center);
    if (_SetCenter_post_ptr._value) { (this->*_SetCenter_post_ptr._value)(center); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RadialGradientBrushHook::SetGradientOriginOffset(
    D2D1_POINT_2F gradientOriginOffset)
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::SetGradientOriginOffset");
    if (_SetGradientOriginOffset_pre_ptr._value) { (this->*_SetGradientOriginOffset_pre_ptr._value)(gradientOriginOffset); }
    GetRealObj()->SetGradientOriginOffset(gradientOriginOffset);
    if (_SetGradientOriginOffset_post_ptr._value) { (this->*_SetGradientOriginOffset_post_ptr._value)(gradientOriginOffset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RadialGradientBrushHook::SetRadiusX(
    FLOAT radiusX)
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::SetRadiusX");
    if (_SetRadiusX_pre_ptr._value) { (this->*_SetRadiusX_pre_ptr._value)(radiusX); }
    GetRealObj()->SetRadiusX(radiusX);
    if (_SetRadiusX_post_ptr._value) { (this->*_SetRadiusX_post_ptr._value)(radiusX); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RadialGradientBrushHook::SetRadiusY(
    FLOAT radiusY)
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::SetRadiusY");
    if (_SetRadiusY_pre_ptr._value) { (this->*_SetRadiusY_pre_ptr._value)(radiusY); }
    GetRealObj()->SetRadiusY(radiusY);
    if (_SetRadiusY_post_ptr._value) { (this->*_SetRadiusY_post_ptr._value)(radiusY); }
}

// -----------------------------------------------------------------------------
D2D1_POINT_2F STDMETHODCALLTYPE D2D1RadialGradientBrushHook::GetCenter() const
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::GetCenter");
    if (_GetCenter_pre_ptr._value) { (this->*_GetCenter_pre_ptr._value)(); }
    D2D1_POINT_2F ret = GetRealObj()->GetCenter();
    if (_GetCenter_post_ptr._value) { (this->*_GetCenter_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_POINT_2F STDMETHODCALLTYPE D2D1RadialGradientBrushHook::GetGradientOriginOffset() const
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::GetGradientOriginOffset");
    if (_GetGradientOriginOffset_pre_ptr._value) { (this->*_GetGradientOriginOffset_pre_ptr._value)(); }
    D2D1_POINT_2F ret = GetRealObj()->GetGradientOriginOffset();
    if (_GetGradientOriginOffset_post_ptr._value) { (this->*_GetGradientOriginOffset_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE D2D1RadialGradientBrushHook::GetRadiusX() const
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::GetRadiusX");
    if (_GetRadiusX_pre_ptr._value) { (this->*_GetRadiusX_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetRadiusX();
    if (_GetRadiusX_post_ptr._value) { (this->*_GetRadiusX_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE D2D1RadialGradientBrushHook::GetRadiusY() const
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::GetRadiusY");
    if (_GetRadiusY_pre_ptr._value) { (this->*_GetRadiusY_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetRadiusY();
    if (_GetRadiusY_post_ptr._value) { (this->*_GetRadiusY_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RadialGradientBrushHook::GetGradientStopCollection(
    _Outptr_ ID2D1GradientStopCollection ** gradientStopCollection) const
{
    calltrace::AutoTrace trace(L"D2D1RadialGradientBrushHook::GetGradientStopCollection");
    if (_GetGradientStopCollection_pre_ptr._value) { (this->*_GetGradientStopCollection_pre_ptr._value)(gradientStopCollection); }
    GetRealObj()->GetGradientStopCollection(gradientStopCollection);
    if ( gradientStopCollection && *gradientStopCollection) { *gradientStopCollection = RealToHooked_D2D( *gradientStopCollection ); }
    if (_GetGradientStopCollection_post_ptr._value) { (this->*_GetGradientStopCollection_post_ptr._value)(gradientStopCollection); }
}

// -----------------------------------------------------------------------------
D2D1_CAP_STYLE STDMETHODCALLTYPE D2D1StrokeStyleHook::GetStartCap() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetStartCap");
    if (_GetStartCap_pre_ptr._value) { (this->*_GetStartCap_pre_ptr._value)(); }
    D2D1_CAP_STYLE ret = GetRealObj()->GetStartCap();
    if (_GetStartCap_post_ptr._value) { (this->*_GetStartCap_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_CAP_STYLE STDMETHODCALLTYPE D2D1StrokeStyleHook::GetEndCap() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetEndCap");
    if (_GetEndCap_pre_ptr._value) { (this->*_GetEndCap_pre_ptr._value)(); }
    D2D1_CAP_STYLE ret = GetRealObj()->GetEndCap();
    if (_GetEndCap_post_ptr._value) { (this->*_GetEndCap_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_CAP_STYLE STDMETHODCALLTYPE D2D1StrokeStyleHook::GetDashCap() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetDashCap");
    if (_GetDashCap_pre_ptr._value) { (this->*_GetDashCap_pre_ptr._value)(); }
    D2D1_CAP_STYLE ret = GetRealObj()->GetDashCap();
    if (_GetDashCap_post_ptr._value) { (this->*_GetDashCap_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE D2D1StrokeStyleHook::GetMiterLimit() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetMiterLimit");
    if (_GetMiterLimit_pre_ptr._value) { (this->*_GetMiterLimit_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetMiterLimit();
    if (_GetMiterLimit_post_ptr._value) { (this->*_GetMiterLimit_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_LINE_JOIN STDMETHODCALLTYPE D2D1StrokeStyleHook::GetLineJoin() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetLineJoin");
    if (_GetLineJoin_pre_ptr._value) { (this->*_GetLineJoin_pre_ptr._value)(); }
    D2D1_LINE_JOIN ret = GetRealObj()->GetLineJoin();
    if (_GetLineJoin_post_ptr._value) { (this->*_GetLineJoin_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE D2D1StrokeStyleHook::GetDashOffset() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetDashOffset");
    if (_GetDashOffset_pre_ptr._value) { (this->*_GetDashOffset_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetDashOffset();
    if (_GetDashOffset_post_ptr._value) { (this->*_GetDashOffset_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_DASH_STYLE STDMETHODCALLTYPE D2D1StrokeStyleHook::GetDashStyle() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetDashStyle");
    if (_GetDashStyle_pre_ptr._value) { (this->*_GetDashStyle_pre_ptr._value)(); }
    D2D1_DASH_STYLE ret = GetRealObj()->GetDashStyle();
    if (_GetDashStyle_post_ptr._value) { (this->*_GetDashStyle_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1StrokeStyleHook::GetDashesCount() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetDashesCount");
    if (_GetDashesCount_pre_ptr._value) { (this->*_GetDashesCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetDashesCount();
    if (_GetDashesCount_post_ptr._value) { (this->*_GetDashesCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1StrokeStyleHook::GetDashes(
    _Out_writes_(dashesCount) FLOAT * dashes,
    UINT32 dashesCount) const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyleHook::GetDashes");
    if (_GetDashes_pre_ptr._value) { (this->*_GetDashes_pre_ptr._value)(dashes, dashesCount); }
    GetRealObj()->GetDashes(dashes, dashesCount);
    if (_GetDashes_post_ptr._value) { (this->*_GetDashes_post_ptr._value)(dashes, dashesCount); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::GetBounds(
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    _Out_ D2D1_RECT_F * bounds) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::GetBounds");
    if (_GetBounds_pre_ptr._value) { (this->*_GetBounds_pre_ptr._value)(worldTransform, bounds); }
    HRESULT ret = GetRealObj()->GetBounds(worldTransform, bounds);
    if (_GetBounds_post_ptr._value) { (this->*_GetBounds_post_ptr._value)(ret, worldTransform, bounds); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::GetWidenedBounds(
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _Out_ D2D1_RECT_F * bounds) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::GetWidenedBounds");
    if (_GetWidenedBounds_pre_ptr._value) { (this->*_GetWidenedBounds_pre_ptr._value)(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds); }
    HRESULT ret = GetRealObj()->GetWidenedBounds(strokeWidth, HookedToReal(strokeStyle), worldTransform, flatteningTolerance, bounds);
    if (_GetWidenedBounds_post_ptr._value) { (this->*_GetWidenedBounds_post_ptr._value)(ret, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::StrokeContainsPoint(
    D2D1_POINT_2F point,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _Out_ BOOL * contains) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::StrokeContainsPoint");
    if (_StrokeContainsPoint_pre_ptr._value) { (this->*_StrokeContainsPoint_pre_ptr._value)(point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains); }
    HRESULT ret = GetRealObj()->StrokeContainsPoint(point, strokeWidth, HookedToReal(strokeStyle), worldTransform, flatteningTolerance, contains);
    if (_StrokeContainsPoint_post_ptr._value) { (this->*_StrokeContainsPoint_post_ptr._value)(ret, point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::FillContainsPoint(
    D2D1_POINT_2F point,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _Out_ BOOL * contains) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::FillContainsPoint");
    if (_FillContainsPoint_pre_ptr._value) { (this->*_FillContainsPoint_pre_ptr._value)(point, worldTransform, flatteningTolerance, contains); }
    HRESULT ret = GetRealObj()->FillContainsPoint(point, worldTransform, flatteningTolerance, contains);
    if (_FillContainsPoint_post_ptr._value) { (this->*_FillContainsPoint_post_ptr._value)(ret, point, worldTransform, flatteningTolerance, contains); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::CompareWithGeometry(
    _In_ ID2D1Geometry * inputGeometry,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform,
    FLOAT flatteningTolerance,
    _Out_ D2D1_GEOMETRY_RELATION * relation) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::CompareWithGeometry");
    if (_CompareWithGeometry_pre_ptr._value) { (this->*_CompareWithGeometry_pre_ptr._value)(inputGeometry, inputGeometryTransform, flatteningTolerance, relation); }
    HRESULT ret = GetRealObj()->CompareWithGeometry(HookedToReal(inputGeometry), inputGeometryTransform, flatteningTolerance, relation);
    if (_CompareWithGeometry_post_ptr._value) { (this->*_CompareWithGeometry_post_ptr._value)(ret, inputGeometry, inputGeometryTransform, flatteningTolerance, relation); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::Simplify(
    D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::Simplify");
    if (_Simplify_pre_ptr._value) { (this->*_Simplify_pre_ptr._value)(simplificationOption, worldTransform, flatteningTolerance, geometrySink); }
    HRESULT ret = GetRealObj()->Simplify(simplificationOption, worldTransform, flatteningTolerance, HookedToReal(geometrySink));
    if (_Simplify_post_ptr._value) { (this->*_Simplify_post_ptr._value)(ret, simplificationOption, worldTransform, flatteningTolerance, geometrySink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::Tessellate(
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _In_ ID2D1TessellationSink * tessellationSink) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::Tessellate");
    if (_Tessellate_pre_ptr._value) { (this->*_Tessellate_pre_ptr._value)(worldTransform, flatteningTolerance, tessellationSink); }
    HRESULT ret = GetRealObj()->Tessellate(worldTransform, flatteningTolerance, HookedToReal(tessellationSink));
    if (_Tessellate_post_ptr._value) { (this->*_Tessellate_post_ptr._value)(ret, worldTransform, flatteningTolerance, tessellationSink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::CombineWithGeometry(
    _In_ ID2D1Geometry * inputGeometry,
    D2D1_COMBINE_MODE combineMode,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform,
    FLOAT flatteningTolerance,
    _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::CombineWithGeometry");
    if (_CombineWithGeometry_pre_ptr._value) { (this->*_CombineWithGeometry_pre_ptr._value)(inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink); }
    HRESULT ret = GetRealObj()->CombineWithGeometry(HookedToReal(inputGeometry), combineMode, inputGeometryTransform, flatteningTolerance, HookedToReal(geometrySink));
    if (_CombineWithGeometry_post_ptr._value) { (this->*_CombineWithGeometry_post_ptr._value)(ret, inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::Outline(
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::Outline");
    if (_Outline_pre_ptr._value) { (this->*_Outline_pre_ptr._value)(worldTransform, flatteningTolerance, geometrySink); }
    HRESULT ret = GetRealObj()->Outline(worldTransform, flatteningTolerance, HookedToReal(geometrySink));
    if (_Outline_post_ptr._value) { (this->*_Outline_post_ptr._value)(ret, worldTransform, flatteningTolerance, geometrySink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::ComputeArea(
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _Out_ FLOAT * area) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::ComputeArea");
    if (_ComputeArea_pre_ptr._value) { (this->*_ComputeArea_pre_ptr._value)(worldTransform, flatteningTolerance, area); }
    HRESULT ret = GetRealObj()->ComputeArea(worldTransform, flatteningTolerance, area);
    if (_ComputeArea_post_ptr._value) { (this->*_ComputeArea_post_ptr._value)(ret, worldTransform, flatteningTolerance, area); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::ComputeLength(
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _Out_ FLOAT * length) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::ComputeLength");
    if (_ComputeLength_pre_ptr._value) { (this->*_ComputeLength_pre_ptr._value)(worldTransform, flatteningTolerance, length); }
    HRESULT ret = GetRealObj()->ComputeLength(worldTransform, flatteningTolerance, length);
    if (_ComputeLength_post_ptr._value) { (this->*_ComputeLength_post_ptr._value)(ret, worldTransform, flatteningTolerance, length); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::ComputePointAtLength(
    FLOAT length,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _Out_opt_ D2D1_POINT_2F * point,
    _Out_opt_ D2D1_POINT_2F * unitTangentVector) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::ComputePointAtLength");
    if (_ComputePointAtLength_pre_ptr._value) { (this->*_ComputePointAtLength_pre_ptr._value)(length, worldTransform, flatteningTolerance, point, unitTangentVector); }
    HRESULT ret = GetRealObj()->ComputePointAtLength(length, worldTransform, flatteningTolerance, point, unitTangentVector);
    if (_ComputePointAtLength_post_ptr._value) { (this->*_ComputePointAtLength_post_ptr._value)(ret, length, worldTransform, flatteningTolerance, point, unitTangentVector); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GeometryHook::Widen(
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryHook::Widen");
    if (_Widen_pre_ptr._value) { (this->*_Widen_pre_ptr._value)(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink); }
    HRESULT ret = GetRealObj()->Widen(strokeWidth, HookedToReal(strokeStyle), worldTransform, flatteningTolerance, HookedToReal(geometrySink));
    if (_Widen_post_ptr._value) { (this->*_Widen_post_ptr._value)(ret, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RectangleGeometryHook::GetRect(
    _Out_ D2D1_RECT_F * rect) const
{
    calltrace::AutoTrace trace(L"D2D1RectangleGeometryHook::GetRect");
    if (_GetRect_pre_ptr._value) { (this->*_GetRect_pre_ptr._value)(rect); }
    GetRealObj()->GetRect(rect);
    if (_GetRect_post_ptr._value) { (this->*_GetRect_post_ptr._value)(rect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RoundedRectangleGeometryHook::GetRoundedRect(
    _Out_ D2D1_ROUNDED_RECT * roundedRect) const
{
    calltrace::AutoTrace trace(L"D2D1RoundedRectangleGeometryHook::GetRoundedRect");
    if (_GetRoundedRect_pre_ptr._value) { (this->*_GetRoundedRect_pre_ptr._value)(roundedRect); }
    GetRealObj()->GetRoundedRect(roundedRect);
    if (_GetRoundedRect_post_ptr._value) { (this->*_GetRoundedRect_post_ptr._value)(roundedRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1EllipseGeometryHook::GetEllipse(
    _Out_ D2D1_ELLIPSE * ellipse) const
{
    calltrace::AutoTrace trace(L"D2D1EllipseGeometryHook::GetEllipse");
    if (_GetEllipse_pre_ptr._value) { (this->*_GetEllipse_pre_ptr._value)(ellipse); }
    GetRealObj()->GetEllipse(ellipse);
    if (_GetEllipse_post_ptr._value) { (this->*_GetEllipse_post_ptr._value)(ellipse); }
}

// -----------------------------------------------------------------------------
D2D1_FILL_MODE STDMETHODCALLTYPE D2D1GeometryGroupHook::GetFillMode() const
{
    calltrace::AutoTrace trace(L"D2D1GeometryGroupHook::GetFillMode");
    if (_GetFillMode_pre_ptr._value) { (this->*_GetFillMode_pre_ptr._value)(); }
    D2D1_FILL_MODE ret = GetRealObj()->GetFillMode();
    if (_GetFillMode_post_ptr._value) { (this->*_GetFillMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1GeometryGroupHook::GetSourceGeometryCount() const
{
    calltrace::AutoTrace trace(L"D2D1GeometryGroupHook::GetSourceGeometryCount");
    if (_GetSourceGeometryCount_pre_ptr._value) { (this->*_GetSourceGeometryCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetSourceGeometryCount();
    if (_GetSourceGeometryCount_post_ptr._value) { (this->*_GetSourceGeometryCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1GeometryGroupHook::GetSourceGeometries(
    _Out_writes_(geometriesCount) ID2D1Geometry ** geometries,
    UINT32 geometriesCount) const
{
    calltrace::AutoTrace trace(L"D2D1GeometryGroupHook::GetSourceGeometries");
    if (_GetSourceGeometries_pre_ptr._value) { (this->*_GetSourceGeometries_pre_ptr._value)(geometries, geometriesCount); }
    GetRealObj()->GetSourceGeometries(geometries, geometriesCount);
    if ( geometries && *geometries) { *geometries = RealToHooked_D2D( *geometries ); }
    if (_GetSourceGeometries_post_ptr._value) { (this->*_GetSourceGeometries_post_ptr._value)(geometries, geometriesCount); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1TransformedGeometryHook::GetSourceGeometry(
    _Outptr_ ID2D1Geometry ** sourceGeometry) const
{
    calltrace::AutoTrace trace(L"D2D1TransformedGeometryHook::GetSourceGeometry");
    if (_GetSourceGeometry_pre_ptr._value) { (this->*_GetSourceGeometry_pre_ptr._value)(sourceGeometry); }
    GetRealObj()->GetSourceGeometry(sourceGeometry);
    if ( sourceGeometry && *sourceGeometry) { *sourceGeometry = RealToHooked_D2D( *sourceGeometry ); }
    if (_GetSourceGeometry_post_ptr._value) { (this->*_GetSourceGeometry_post_ptr._value)(sourceGeometry); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1TransformedGeometryHook::GetTransform(
    _Out_ D2D1_MATRIX_3X2_F * transform) const
{
    calltrace::AutoTrace trace(L"D2D1TransformedGeometryHook::GetTransform");
    if (_GetTransform_pre_ptr._value) { (this->*_GetTransform_pre_ptr._value)(transform); }
    GetRealObj()->GetTransform(transform);
    if (_GetTransform_post_ptr._value) { (this->*_GetTransform_post_ptr._value)(transform); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1SimplifiedGeometrySinkHook::SetFillMode(
    D2D1_FILL_MODE fillMode)
{
    calltrace::AutoTrace trace(L"D2D1SimplifiedGeometrySinkHook::SetFillMode");
    if (_SetFillMode_pre_ptr._value) { (this->*_SetFillMode_pre_ptr._value)(fillMode); }
    GetRealObj()->SetFillMode(fillMode);
    if (_SetFillMode_post_ptr._value) { (this->*_SetFillMode_post_ptr._value)(fillMode); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1SimplifiedGeometrySinkHook::SetSegmentFlags(
    D2D1_PATH_SEGMENT vertexFlags)
{
    calltrace::AutoTrace trace(L"D2D1SimplifiedGeometrySinkHook::SetSegmentFlags");
    if (_SetSegmentFlags_pre_ptr._value) { (this->*_SetSegmentFlags_pre_ptr._value)(vertexFlags); }
    GetRealObj()->SetSegmentFlags(vertexFlags);
    if (_SetSegmentFlags_post_ptr._value) { (this->*_SetSegmentFlags_post_ptr._value)(vertexFlags); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1SimplifiedGeometrySinkHook::BeginFigure(
    D2D1_POINT_2F startPoint,
    D2D1_FIGURE_BEGIN figureBegin)
{
    calltrace::AutoTrace trace(L"D2D1SimplifiedGeometrySinkHook::BeginFigure");
    if (_BeginFigure_pre_ptr._value) { (this->*_BeginFigure_pre_ptr._value)(startPoint, figureBegin); }
    GetRealObj()->BeginFigure(startPoint, figureBegin);
    if (_BeginFigure_post_ptr._value) { (this->*_BeginFigure_post_ptr._value)(startPoint, figureBegin); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1SimplifiedGeometrySinkHook::AddLines(
    _In_reads_(pointsCount) CONST D2D1_POINT_2F * points,
    UINT32 pointsCount)
{
    calltrace::AutoTrace trace(L"D2D1SimplifiedGeometrySinkHook::AddLines");
    if (_AddLines_pre_ptr._value) { (this->*_AddLines_pre_ptr._value)(points, pointsCount); }
    GetRealObj()->AddLines(points, pointsCount);
    if (_AddLines_post_ptr._value) { (this->*_AddLines_post_ptr._value)(points, pointsCount); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1SimplifiedGeometrySinkHook::AddBeziers(
    _In_reads_(beziersCount) CONST D2D1_BEZIER_SEGMENT * beziers,
    UINT32 beziersCount)
{
    calltrace::AutoTrace trace(L"D2D1SimplifiedGeometrySinkHook::AddBeziers");
    if (_AddBeziers_pre_ptr._value) { (this->*_AddBeziers_pre_ptr._value)(beziers, beziersCount); }
    GetRealObj()->AddBeziers(beziers, beziersCount);
    if (_AddBeziers_post_ptr._value) { (this->*_AddBeziers_post_ptr._value)(beziers, beziersCount); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1SimplifiedGeometrySinkHook::EndFigure(
    D2D1_FIGURE_END figureEnd)
{
    calltrace::AutoTrace trace(L"D2D1SimplifiedGeometrySinkHook::EndFigure");
    if (_EndFigure_pre_ptr._value) { (this->*_EndFigure_pre_ptr._value)(figureEnd); }
    GetRealObj()->EndFigure(figureEnd);
    if (_EndFigure_post_ptr._value) { (this->*_EndFigure_post_ptr._value)(figureEnd); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1SimplifiedGeometrySinkHook::Close()
{
    calltrace::AutoTrace trace(L"D2D1SimplifiedGeometrySinkHook::Close");
    if (_Close_pre_ptr._value) { (this->*_Close_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Close();
    if (_Close_post_ptr._value) { (this->*_Close_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1GeometrySinkHook::AddLine(
    D2D1_POINT_2F point)
{
    calltrace::AutoTrace trace(L"D2D1GeometrySinkHook::AddLine");
    if (_AddLine_pre_ptr._value) { (this->*_AddLine_pre_ptr._value)(point); }
    GetRealObj()->AddLine(point);
    if (_AddLine_post_ptr._value) { (this->*_AddLine_post_ptr._value)(point); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1GeometrySinkHook::AddBezier(
    _In_ CONST D2D1_BEZIER_SEGMENT * bezier)
{
    calltrace::AutoTrace trace(L"D2D1GeometrySinkHook::AddBezier");
    if (_AddBezier_pre_ptr._value) { (this->*_AddBezier_pre_ptr._value)(bezier); }
    GetRealObj()->AddBezier(bezier);
    if (_AddBezier_post_ptr._value) { (this->*_AddBezier_post_ptr._value)(bezier); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1GeometrySinkHook::AddQuadraticBezier(
    _In_ CONST D2D1_QUADRATIC_BEZIER_SEGMENT * bezier)
{
    calltrace::AutoTrace trace(L"D2D1GeometrySinkHook::AddQuadraticBezier");
    if (_AddQuadraticBezier_pre_ptr._value) { (this->*_AddQuadraticBezier_pre_ptr._value)(bezier); }
    GetRealObj()->AddQuadraticBezier(bezier);
    if (_AddQuadraticBezier_post_ptr._value) { (this->*_AddQuadraticBezier_post_ptr._value)(bezier); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1GeometrySinkHook::AddQuadraticBeziers(
    _In_reads_(beziersCount) CONST D2D1_QUADRATIC_BEZIER_SEGMENT * beziers,
    UINT32 beziersCount)
{
    calltrace::AutoTrace trace(L"D2D1GeometrySinkHook::AddQuadraticBeziers");
    if (_AddQuadraticBeziers_pre_ptr._value) { (this->*_AddQuadraticBeziers_pre_ptr._value)(beziers, beziersCount); }
    GetRealObj()->AddQuadraticBeziers(beziers, beziersCount);
    if (_AddQuadraticBeziers_post_ptr._value) { (this->*_AddQuadraticBeziers_post_ptr._value)(beziers, beziersCount); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1GeometrySinkHook::AddArc(
    _In_ CONST D2D1_ARC_SEGMENT * arc)
{
    calltrace::AutoTrace trace(L"D2D1GeometrySinkHook::AddArc");
    if (_AddArc_pre_ptr._value) { (this->*_AddArc_pre_ptr._value)(arc); }
    GetRealObj()->AddArc(arc);
    if (_AddArc_post_ptr._value) { (this->*_AddArc_post_ptr._value)(arc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1TessellationSinkHook::AddTriangles(
    _In_reads_(trianglesCount) CONST D2D1_TRIANGLE * triangles,
    UINT32 trianglesCount)
{
    calltrace::AutoTrace trace(L"D2D1TessellationSinkHook::AddTriangles");
    if (_AddTriangles_pre_ptr._value) { (this->*_AddTriangles_pre_ptr._value)(triangles, trianglesCount); }
    GetRealObj()->AddTriangles(triangles, trianglesCount);
    if (_AddTriangles_post_ptr._value) { (this->*_AddTriangles_post_ptr._value)(triangles, trianglesCount); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1TessellationSinkHook::Close()
{
    calltrace::AutoTrace trace(L"D2D1TessellationSinkHook::Close");
    if (_Close_pre_ptr._value) { (this->*_Close_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Close();
    if (_Close_post_ptr._value) { (this->*_Close_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PathGeometryHook::Open(
    _Outptr_ ID2D1GeometrySink ** geometrySink)
{
    calltrace::AutoTrace trace(L"D2D1PathGeometryHook::Open");
    if (_Open_pre_ptr._value) { (this->*_Open_pre_ptr._value)(geometrySink); }
    HRESULT ret = GetRealObj()->Open(geometrySink);
    if ( geometrySink && *geometrySink) { *geometrySink = RealToHooked_D2D( *geometrySink ); }
    if (_Open_post_ptr._value) { (this->*_Open_post_ptr._value)(ret, geometrySink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PathGeometryHook::Stream(
    _In_ ID2D1GeometrySink * geometrySink) const
{
    calltrace::AutoTrace trace(L"D2D1PathGeometryHook::Stream");
    if (_Stream_pre_ptr._value) { (this->*_Stream_pre_ptr._value)(geometrySink); }
    HRESULT ret = GetRealObj()->Stream(HookedToReal(geometrySink));
    if (_Stream_post_ptr._value) { (this->*_Stream_post_ptr._value)(ret, geometrySink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PathGeometryHook::GetSegmentCount(
    _Out_ UINT32 * count) const
{
    calltrace::AutoTrace trace(L"D2D1PathGeometryHook::GetSegmentCount");
    if (_GetSegmentCount_pre_ptr._value) { (this->*_GetSegmentCount_pre_ptr._value)(count); }
    HRESULT ret = GetRealObj()->GetSegmentCount(count);
    if (_GetSegmentCount_post_ptr._value) { (this->*_GetSegmentCount_post_ptr._value)(ret, count); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PathGeometryHook::GetFigureCount(
    _Out_ UINT32 * count) const
{
    calltrace::AutoTrace trace(L"D2D1PathGeometryHook::GetFigureCount");
    if (_GetFigureCount_pre_ptr._value) { (this->*_GetFigureCount_pre_ptr._value)(count); }
    HRESULT ret = GetRealObj()->GetFigureCount(count);
    if (_GetFigureCount_post_ptr._value) { (this->*_GetFigureCount_post_ptr._value)(ret, count); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1MeshHook::Open(
    _Outptr_ ID2D1TessellationSink ** tessellationSink)
{
    calltrace::AutoTrace trace(L"D2D1MeshHook::Open");
    if (_Open_pre_ptr._value) { (this->*_Open_pre_ptr._value)(tessellationSink); }
    HRESULT ret = GetRealObj()->Open(tessellationSink);
    if ( tessellationSink && *tessellationSink) { *tessellationSink = RealToHooked_D2D( *tessellationSink ); }
    if (_Open_post_ptr._value) { (this->*_Open_post_ptr._value)(ret, tessellationSink); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_SIZE_F STDMETHODCALLTYPE D2D1LayerHook::GetSize() const
{
    calltrace::AutoTrace trace(L"D2D1LayerHook::GetSize");
    if (_GetSize_pre_ptr._value) { (this->*_GetSize_pre_ptr._value)(); }
    D2D1_SIZE_F ret = GetRealObj()->GetSize();
    if (_GetSize_post_ptr._value) { (this->*_GetSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DrawingStateBlockHook::GetDescription(
    _Out_ D2D1_DRAWING_STATE_DESCRIPTION * stateDescription) const
{
    calltrace::AutoTrace trace(L"D2D1DrawingStateBlockHook::GetDescription");
    if (_GetDescription_pre_ptr._value) { (this->*_GetDescription_pre_ptr._value)(stateDescription); }
    GetRealObj()->GetDescription(stateDescription);
    if (_GetDescription_post_ptr._value) { (this->*_GetDescription_post_ptr._value)(stateDescription); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DrawingStateBlockHook::SetDescription(
    _In_ CONST D2D1_DRAWING_STATE_DESCRIPTION * stateDescription)
{
    calltrace::AutoTrace trace(L"D2D1DrawingStateBlockHook::SetDescription");
    if (_SetDescription_pre_ptr._value) { (this->*_SetDescription_pre_ptr._value)(stateDescription); }
    GetRealObj()->SetDescription(stateDescription);
    if (_SetDescription_post_ptr._value) { (this->*_SetDescription_post_ptr._value)(stateDescription); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DrawingStateBlockHook::SetTextRenderingParams(
    _In_opt_ IDWriteRenderingParams * textRenderingParams)
{
    calltrace::AutoTrace trace(L"D2D1DrawingStateBlockHook::SetTextRenderingParams");
    if (_SetTextRenderingParams_pre_ptr._value) { (this->*_SetTextRenderingParams_pre_ptr._value)(textRenderingParams); }
    GetRealObj()->SetTextRenderingParams(HookedToReal(textRenderingParams));
    if (_SetTextRenderingParams_post_ptr._value) { (this->*_SetTextRenderingParams_post_ptr._value)(textRenderingParams); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DrawingStateBlockHook::GetTextRenderingParams(
    _Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const
{
    calltrace::AutoTrace trace(L"D2D1DrawingStateBlockHook::GetTextRenderingParams");
    if (_GetTextRenderingParams_pre_ptr._value) { (this->*_GetTextRenderingParams_pre_ptr._value)(textRenderingParams); }
    GetRealObj()->GetTextRenderingParams(textRenderingParams);
    if ( textRenderingParams && *textRenderingParams) { *textRenderingParams = RealToHooked_D2D( *textRenderingParams ); }
    if (_GetTextRenderingParams_post_ptr._value) { (this->*_GetTextRenderingParams_post_ptr._value)(textRenderingParams); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateBitmap(
    D2D1_SIZE_U size,
    _In_opt_ CONST void * srcData,
    UINT32 pitch,
    _In_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties,
    _Outptr_ ID2D1Bitmap ** bitmap)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateBitmap");
    if (_CreateBitmap_pre_ptr._value) { (this->*_CreateBitmap_pre_ptr._value)(size, srcData, pitch, bitmapProperties, bitmap); }
    HRESULT ret = GetRealObj()->CreateBitmap(size, srcData, pitch, bitmapProperties, bitmap);
    if ( bitmap && *bitmap) { *bitmap = RealToHooked_D2D( *bitmap ); }
    if (_CreateBitmap_post_ptr._value) { (this->*_CreateBitmap_post_ptr._value)(ret, size, srcData, pitch, bitmapProperties, bitmap); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateBitmapFromWicBitmap(
    _In_ IWICBitmapSource * wicBitmapSource,
    _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties,
    _Outptr_ ID2D1Bitmap ** bitmap)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateBitmapFromWicBitmap");
    if (_CreateBitmapFromWicBitmap_pre_ptr._value) { (this->*_CreateBitmapFromWicBitmap_pre_ptr._value)(wicBitmapSource, bitmapProperties, bitmap); }
    HRESULT ret = GetRealObj()->CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
    if ( bitmap && *bitmap) { *bitmap = RealToHooked_D2D( *bitmap ); }
    if (_CreateBitmapFromWicBitmap_post_ptr._value) { (this->*_CreateBitmapFromWicBitmap_post_ptr._value)(ret, wicBitmapSource, bitmapProperties, bitmap); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateSharedBitmap(
    _In_ REFIID riid,
    _Inout_ void * data,
    _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties,
    _Outptr_ ID2D1Bitmap ** bitmap)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateSharedBitmap");
    if (_CreateSharedBitmap_pre_ptr._value) { (this->*_CreateSharedBitmap_pre_ptr._value)(riid, data, bitmapProperties, bitmap); }
    HRESULT ret = GetRealObj()->CreateSharedBitmap(riid, data, bitmapProperties, bitmap);
    if ( bitmap && *bitmap) { *bitmap = RealToHooked_D2D( *bitmap ); }
    if (_CreateSharedBitmap_post_ptr._value) { (this->*_CreateSharedBitmap_post_ptr._value)(ret, riid, data, bitmapProperties, bitmap); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateBitmapBrush(
    _In_opt_ ID2D1Bitmap * bitmap,
    _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES * bitmapBrushProperties,
    _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties,
    _Outptr_ ID2D1BitmapBrush ** bitmapBrush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateBitmapBrush");
    if (_CreateBitmapBrush_pre_ptr._value) { (this->*_CreateBitmapBrush_pre_ptr._value)(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush); }
    HRESULT ret = GetRealObj()->CreateBitmapBrush(HookedToReal(bitmap), bitmapBrushProperties, brushProperties, bitmapBrush);
    if ( bitmapBrush && *bitmapBrush) { *bitmapBrush = RealToHooked_D2D( *bitmapBrush ); }
    if (_CreateBitmapBrush_post_ptr._value) { (this->*_CreateBitmapBrush_post_ptr._value)(ret, bitmap, bitmapBrushProperties, brushProperties, bitmapBrush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateSolidColorBrush(
    _In_ CONST D2D1_COLOR_F * color,
    _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties,
    _Outptr_ ID2D1SolidColorBrush ** solidColorBrush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateSolidColorBrush");
    if (_CreateSolidColorBrush_pre_ptr._value) { (this->*_CreateSolidColorBrush_pre_ptr._value)(color, brushProperties, solidColorBrush); }
    HRESULT ret = GetRealObj()->CreateSolidColorBrush(color, brushProperties, solidColorBrush);
    if ( solidColorBrush && *solidColorBrush) { *solidColorBrush = RealToHooked_D2D( *solidColorBrush ); }
    if (_CreateSolidColorBrush_post_ptr._value) { (this->*_CreateSolidColorBrush_post_ptr._value)(ret, color, brushProperties, solidColorBrush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateGradientStopCollection(
    _In_reads_(gradientStopsCount) CONST D2D1_GRADIENT_STOP * gradientStops,
    _In_range_(>=,1) UINT32 gradientStopsCount,
    D2D1_GAMMA colorInterpolationGamma,
    D2D1_EXTEND_MODE extendMode,
    _Outptr_ ID2D1GradientStopCollection ** gradientStopCollection)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateGradientStopCollection");
    if (_CreateGradientStopCollection_pre_ptr._value) { (this->*_CreateGradientStopCollection_pre_ptr._value)(gradientStops, gradientStopsCount, colorInterpolationGamma, extendMode, gradientStopCollection); }
    HRESULT ret = GetRealObj()->CreateGradientStopCollection(gradientStops, gradientStopsCount, colorInterpolationGamma, extendMode, gradientStopCollection);
    if ( gradientStopCollection && *gradientStopCollection) { *gradientStopCollection = RealToHooked_D2D( *gradientStopCollection ); }
    if (_CreateGradientStopCollection_post_ptr._value) { (this->*_CreateGradientStopCollection_post_ptr._value)(ret, gradientStops, gradientStopsCount, colorInterpolationGamma, extendMode, gradientStopCollection); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateLinearGradientBrush(
    _In_ CONST D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * linearGradientBrushProperties,
    _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties,
    _In_ ID2D1GradientStopCollection * gradientStopCollection,
    _Outptr_ ID2D1LinearGradientBrush ** linearGradientBrush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateLinearGradientBrush");
    if (_CreateLinearGradientBrush_pre_ptr._value) { (this->*_CreateLinearGradientBrush_pre_ptr._value)(linearGradientBrushProperties, brushProperties, gradientStopCollection, linearGradientBrush); }
    HRESULT ret = GetRealObj()->CreateLinearGradientBrush(linearGradientBrushProperties, brushProperties, HookedToReal(gradientStopCollection), linearGradientBrush);
    if ( linearGradientBrush && *linearGradientBrush) { *linearGradientBrush = RealToHooked_D2D( *linearGradientBrush ); }
    if (_CreateLinearGradientBrush_post_ptr._value) { (this->*_CreateLinearGradientBrush_post_ptr._value)(ret, linearGradientBrushProperties, brushProperties, gradientStopCollection, linearGradientBrush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateRadialGradientBrush(
    _In_ CONST D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * radialGradientBrushProperties,
    _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties,
    _In_ ID2D1GradientStopCollection * gradientStopCollection,
    _Outptr_ ID2D1RadialGradientBrush ** radialGradientBrush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateRadialGradientBrush");
    if (_CreateRadialGradientBrush_pre_ptr._value) { (this->*_CreateRadialGradientBrush_pre_ptr._value)(radialGradientBrushProperties, brushProperties, gradientStopCollection, radialGradientBrush); }
    HRESULT ret = GetRealObj()->CreateRadialGradientBrush(radialGradientBrushProperties, brushProperties, HookedToReal(gradientStopCollection), radialGradientBrush);
    if ( radialGradientBrush && *radialGradientBrush) { *radialGradientBrush = RealToHooked_D2D( *radialGradientBrush ); }
    if (_CreateRadialGradientBrush_post_ptr._value) { (this->*_CreateRadialGradientBrush_post_ptr._value)(ret, radialGradientBrushProperties, brushProperties, gradientStopCollection, radialGradientBrush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateCompatibleRenderTarget(
    _In_opt_ CONST D2D1_SIZE_F * desiredSize,
    _In_opt_ CONST D2D1_SIZE_U * desiredPixelSize,
    _In_opt_ CONST D2D1_PIXEL_FORMAT * desiredFormat,
    D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options,
    _Outptr_ ID2D1BitmapRenderTarget ** bitmapRenderTarget)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateCompatibleRenderTarget");
    if (_CreateCompatibleRenderTarget_pre_ptr._value) { (this->*_CreateCompatibleRenderTarget_pre_ptr._value)(desiredSize, desiredPixelSize, desiredFormat, options, bitmapRenderTarget); }
    HRESULT ret = GetRealObj()->CreateCompatibleRenderTarget(desiredSize, desiredPixelSize, desiredFormat, options, bitmapRenderTarget);
    if ( bitmapRenderTarget && *bitmapRenderTarget) { *bitmapRenderTarget = RealToHooked_D2D( *bitmapRenderTarget ); }
    if (_CreateCompatibleRenderTarget_post_ptr._value) { (this->*_CreateCompatibleRenderTarget_post_ptr._value)(ret, desiredSize, desiredPixelSize, desiredFormat, options, bitmapRenderTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateLayer(
    _In_opt_ CONST D2D1_SIZE_F * size,
    _Outptr_ ID2D1Layer ** layer)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateLayer");
    if (_CreateLayer_pre_ptr._value) { (this->*_CreateLayer_pre_ptr._value)(size, layer); }
    HRESULT ret = GetRealObj()->CreateLayer(size, layer);
    if ( layer && *layer) { *layer = RealToHooked_D2D( *layer ); }
    if (_CreateLayer_post_ptr._value) { (this->*_CreateLayer_post_ptr._value)(ret, size, layer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::CreateMesh(
    _Outptr_ ID2D1Mesh ** mesh)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::CreateMesh");
    if (_CreateMesh_pre_ptr._value) { (this->*_CreateMesh_pre_ptr._value)(mesh); }
    HRESULT ret = GetRealObj()->CreateMesh(mesh);
    if ( mesh && *mesh) { *mesh = RealToHooked_D2D( *mesh ); }
    if (_CreateMesh_post_ptr._value) { (this->*_CreateMesh_post_ptr._value)(ret, mesh); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawLine(
    D2D1_POINT_2F point0,
    D2D1_POINT_2F point1,
    _In_ ID2D1Brush * brush,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawLine");
    if (_DrawLine_pre_ptr._value) { (this->*_DrawLine_pre_ptr._value)(point0, point1, brush, strokeWidth, strokeStyle); }
    GetRealObj()->DrawLine(point0, point1, HookedToReal(brush), strokeWidth, HookedToReal(strokeStyle));
    if (_DrawLine_post_ptr._value) { (this->*_DrawLine_post_ptr._value)(point0, point1, brush, strokeWidth, strokeStyle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawRectangle(
    _In_ CONST D2D1_RECT_F * rect,
    _In_ ID2D1Brush * brush,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawRectangle");
    if (_DrawRectangle_pre_ptr._value) { (this->*_DrawRectangle_pre_ptr._value)(rect, brush, strokeWidth, strokeStyle); }
    GetRealObj()->DrawRectangle(rect, HookedToReal(brush), strokeWidth, HookedToReal(strokeStyle));
    if (_DrawRectangle_post_ptr._value) { (this->*_DrawRectangle_post_ptr._value)(rect, brush, strokeWidth, strokeStyle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::FillRectangle(
    _In_ CONST D2D1_RECT_F * rect,
    _In_ ID2D1Brush * brush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::FillRectangle");
    if (_FillRectangle_pre_ptr._value) { (this->*_FillRectangle_pre_ptr._value)(rect, brush); }
    GetRealObj()->FillRectangle(rect, HookedToReal(brush));
    if (_FillRectangle_post_ptr._value) { (this->*_FillRectangle_post_ptr._value)(rect, brush); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawRoundedRectangle(
    _In_ CONST D2D1_ROUNDED_RECT * roundedRect,
    _In_ ID2D1Brush * brush,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawRoundedRectangle");
    if (_DrawRoundedRectangle_pre_ptr._value) { (this->*_DrawRoundedRectangle_pre_ptr._value)(roundedRect, brush, strokeWidth, strokeStyle); }
    GetRealObj()->DrawRoundedRectangle(roundedRect, HookedToReal(brush), strokeWidth, HookedToReal(strokeStyle));
    if (_DrawRoundedRectangle_post_ptr._value) { (this->*_DrawRoundedRectangle_post_ptr._value)(roundedRect, brush, strokeWidth, strokeStyle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::FillRoundedRectangle(
    _In_ CONST D2D1_ROUNDED_RECT * roundedRect,
    _In_ ID2D1Brush * brush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::FillRoundedRectangle");
    if (_FillRoundedRectangle_pre_ptr._value) { (this->*_FillRoundedRectangle_pre_ptr._value)(roundedRect, brush); }
    GetRealObj()->FillRoundedRectangle(roundedRect, HookedToReal(brush));
    if (_FillRoundedRectangle_post_ptr._value) { (this->*_FillRoundedRectangle_post_ptr._value)(roundedRect, brush); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawEllipse(
    _In_ CONST D2D1_ELLIPSE * ellipse,
    _In_ ID2D1Brush * brush,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawEllipse");
    if (_DrawEllipse_pre_ptr._value) { (this->*_DrawEllipse_pre_ptr._value)(ellipse, brush, strokeWidth, strokeStyle); }
    GetRealObj()->DrawEllipse(ellipse, HookedToReal(brush), strokeWidth, HookedToReal(strokeStyle));
    if (_DrawEllipse_post_ptr._value) { (this->*_DrawEllipse_post_ptr._value)(ellipse, brush, strokeWidth, strokeStyle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::FillEllipse(
    _In_ CONST D2D1_ELLIPSE * ellipse,
    _In_ ID2D1Brush * brush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::FillEllipse");
    if (_FillEllipse_pre_ptr._value) { (this->*_FillEllipse_pre_ptr._value)(ellipse, brush); }
    GetRealObj()->FillEllipse(ellipse, HookedToReal(brush));
    if (_FillEllipse_post_ptr._value) { (this->*_FillEllipse_post_ptr._value)(ellipse, brush); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawGeometry(
    _In_ ID2D1Geometry * geometry,
    _In_ ID2D1Brush * brush,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawGeometry");
    if (_DrawGeometry_pre_ptr._value) { (this->*_DrawGeometry_pre_ptr._value)(geometry, brush, strokeWidth, strokeStyle); }
    GetRealObj()->DrawGeometry(HookedToReal(geometry), HookedToReal(brush), strokeWidth, HookedToReal(strokeStyle));
    if (_DrawGeometry_post_ptr._value) { (this->*_DrawGeometry_post_ptr._value)(geometry, brush, strokeWidth, strokeStyle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::FillGeometry(
    _In_ ID2D1Geometry * geometry,
    _In_ ID2D1Brush * brush,
    _In_opt_ ID2D1Brush * opacityBrush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::FillGeometry");
    if (_FillGeometry_pre_ptr._value) { (this->*_FillGeometry_pre_ptr._value)(geometry, brush, opacityBrush); }
    GetRealObj()->FillGeometry(HookedToReal(geometry), HookedToReal(brush), HookedToReal(opacityBrush));
    if (_FillGeometry_post_ptr._value) { (this->*_FillGeometry_post_ptr._value)(geometry, brush, opacityBrush); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::FillMesh(
    _In_ ID2D1Mesh * mesh,
    _In_ ID2D1Brush * brush)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::FillMesh");
    if (_FillMesh_pre_ptr._value) { (this->*_FillMesh_pre_ptr._value)(mesh, brush); }
    GetRealObj()->FillMesh(HookedToReal(mesh), HookedToReal(brush));
    if (_FillMesh_post_ptr._value) { (this->*_FillMesh_post_ptr._value)(mesh, brush); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::FillOpacityMask(
    _In_ ID2D1Bitmap * opacityMask,
    _In_ ID2D1Brush * brush,
    D2D1_OPACITY_MASK_CONTENT content,
    _In_opt_ CONST D2D1_RECT_F * destinationRectangle,
    _In_opt_ CONST D2D1_RECT_F * sourceRectangle)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::FillOpacityMask");
    if (_FillOpacityMask_pre_ptr._value) { (this->*_FillOpacityMask_pre_ptr._value)(opacityMask, brush, content, destinationRectangle, sourceRectangle); }
    GetRealObj()->FillOpacityMask(HookedToReal(opacityMask), HookedToReal(brush), content, destinationRectangle, sourceRectangle);
    if (_FillOpacityMask_post_ptr._value) { (this->*_FillOpacityMask_post_ptr._value)(opacityMask, brush, content, destinationRectangle, sourceRectangle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawBitmap(
    _In_ ID2D1Bitmap * bitmap,
    _In_opt_ CONST D2D1_RECT_F * destinationRectangle,
    FLOAT opacity,
    D2D1_BITMAP_INTERPOLATION_MODE interpolationMode,
    _In_opt_ CONST D2D1_RECT_F * sourceRectangle)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawBitmap");
    if (_DrawBitmap_pre_ptr._value) { (this->*_DrawBitmap_pre_ptr._value)(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle); }
    GetRealObj()->DrawBitmap(HookedToReal(bitmap), destinationRectangle, opacity, interpolationMode, sourceRectangle);
    if (_DrawBitmap_post_ptr._value) { (this->*_DrawBitmap_post_ptr._value)(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawText(
    _In_reads_(stringLength) CONST WCHAR * string,
    UINT32 stringLength,
    _In_ IDWriteTextFormat * textFormat,
    _In_ CONST D2D1_RECT_F * layoutRect,
    _In_ ID2D1Brush * defaultForegroundBrush,
    D2D1_DRAW_TEXT_OPTIONS options,
    DWRITE_MEASURING_MODE measuringMode)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawText");
    if (_DrawText_pre_ptr._value) { (this->*_DrawText_pre_ptr._value)(string, stringLength, textFormat, layoutRect, defaultForegroundBrush, options, measuringMode); }
    GetRealObj()->DrawText(string, stringLength, HookedToReal(textFormat), layoutRect, HookedToReal(defaultForegroundBrush), options, measuringMode);
    if (_DrawText_post_ptr._value) { (this->*_DrawText_post_ptr._value)(string, stringLength, textFormat, layoutRect, defaultForegroundBrush, options, measuringMode); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawTextLayout(
    D2D1_POINT_2F origin,
    _In_ IDWriteTextLayout * textLayout,
    _In_ ID2D1Brush * defaultForegroundBrush,
    D2D1_DRAW_TEXT_OPTIONS options)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawTextLayout");
    if (_DrawTextLayout_pre_ptr._value) { (this->*_DrawTextLayout_pre_ptr._value)(origin, textLayout, defaultForegroundBrush, options); }
    GetRealObj()->DrawTextLayout(origin, HookedToReal(textLayout), HookedToReal(defaultForegroundBrush), options);
    if (_DrawTextLayout_post_ptr._value) { (this->*_DrawTextLayout_post_ptr._value)(origin, textLayout, defaultForegroundBrush, options); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::DrawGlyphRun(
    D2D1_POINT_2F baselineOrigin,
    _In_ CONST DWRITE_GLYPH_RUN * glyphRun,
    _In_ ID2D1Brush * foregroundBrush,
    DWRITE_MEASURING_MODE measuringMode)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::DrawGlyphRun");
    if (_DrawGlyphRun_pre_ptr._value) { (this->*_DrawGlyphRun_pre_ptr._value)(baselineOrigin, glyphRun, foregroundBrush, measuringMode); }
    GetRealObj()->DrawGlyphRun(baselineOrigin, glyphRun, HookedToReal(foregroundBrush), measuringMode);
    if (_DrawGlyphRun_post_ptr._value) { (this->*_DrawGlyphRun_post_ptr._value)(baselineOrigin, glyphRun, foregroundBrush, measuringMode); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::SetTransform(
    _In_ CONST D2D1_MATRIX_3X2_F * transform)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::SetTransform");
    if (_SetTransform_pre_ptr._value) { (this->*_SetTransform_pre_ptr._value)(transform); }
    GetRealObj()->SetTransform(transform);
    if (_SetTransform_post_ptr._value) { (this->*_SetTransform_post_ptr._value)(transform); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::GetTransform(
    _Out_ D2D1_MATRIX_3X2_F * transform) const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetTransform");
    if (_GetTransform_pre_ptr._value) { (this->*_GetTransform_pre_ptr._value)(transform); }
    GetRealObj()->GetTransform(transform);
    if (_GetTransform_post_ptr._value) { (this->*_GetTransform_post_ptr._value)(transform); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::SetAntialiasMode(
    D2D1_ANTIALIAS_MODE antialiasMode)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::SetAntialiasMode");
    if (_SetAntialiasMode_pre_ptr._value) { (this->*_SetAntialiasMode_pre_ptr._value)(antialiasMode); }
    GetRealObj()->SetAntialiasMode(antialiasMode);
    if (_SetAntialiasMode_post_ptr._value) { (this->*_SetAntialiasMode_post_ptr._value)(antialiasMode); }
}

// -----------------------------------------------------------------------------
D2D1_ANTIALIAS_MODE STDMETHODCALLTYPE D2D1RenderTargetHook::GetAntialiasMode() const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetAntialiasMode");
    if (_GetAntialiasMode_pre_ptr._value) { (this->*_GetAntialiasMode_pre_ptr._value)(); }
    D2D1_ANTIALIAS_MODE ret = GetRealObj()->GetAntialiasMode();
    if (_GetAntialiasMode_post_ptr._value) { (this->*_GetAntialiasMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::SetTextAntialiasMode(
    D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::SetTextAntialiasMode");
    if (_SetTextAntialiasMode_pre_ptr._value) { (this->*_SetTextAntialiasMode_pre_ptr._value)(textAntialiasMode); }
    GetRealObj()->SetTextAntialiasMode(textAntialiasMode);
    if (_SetTextAntialiasMode_post_ptr._value) { (this->*_SetTextAntialiasMode_post_ptr._value)(textAntialiasMode); }
}

// -----------------------------------------------------------------------------
D2D1_TEXT_ANTIALIAS_MODE STDMETHODCALLTYPE D2D1RenderTargetHook::GetTextAntialiasMode() const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetTextAntialiasMode");
    if (_GetTextAntialiasMode_pre_ptr._value) { (this->*_GetTextAntialiasMode_pre_ptr._value)(); }
    D2D1_TEXT_ANTIALIAS_MODE ret = GetRealObj()->GetTextAntialiasMode();
    if (_GetTextAntialiasMode_post_ptr._value) { (this->*_GetTextAntialiasMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::SetTextRenderingParams(
    _In_opt_ IDWriteRenderingParams * textRenderingParams)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::SetTextRenderingParams");
    if (_SetTextRenderingParams_pre_ptr._value) { (this->*_SetTextRenderingParams_pre_ptr._value)(textRenderingParams); }
    GetRealObj()->SetTextRenderingParams(HookedToReal(textRenderingParams));
    if (_SetTextRenderingParams_post_ptr._value) { (this->*_SetTextRenderingParams_post_ptr._value)(textRenderingParams); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::GetTextRenderingParams(
    _Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetTextRenderingParams");
    if (_GetTextRenderingParams_pre_ptr._value) { (this->*_GetTextRenderingParams_pre_ptr._value)(textRenderingParams); }
    GetRealObj()->GetTextRenderingParams(textRenderingParams);
    if ( textRenderingParams && *textRenderingParams) { *textRenderingParams = RealToHooked_D2D( *textRenderingParams ); }
    if (_GetTextRenderingParams_post_ptr._value) { (this->*_GetTextRenderingParams_post_ptr._value)(textRenderingParams); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::SetTags(
    D2D1_TAG tag1,
    D2D1_TAG tag2)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::SetTags");
    if (_SetTags_pre_ptr._value) { (this->*_SetTags_pre_ptr._value)(tag1, tag2); }
    GetRealObj()->SetTags(tag1, tag2);
    if (_SetTags_post_ptr._value) { (this->*_SetTags_post_ptr._value)(tag1, tag2); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::GetTags(
    _Out_opt_ D2D1_TAG * tag1,
    _Out_opt_ D2D1_TAG * tag2) const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetTags");
    if (_GetTags_pre_ptr._value) { (this->*_GetTags_pre_ptr._value)(tag1, tag2); }
    GetRealObj()->GetTags(tag1, tag2);
    if (_GetTags_post_ptr._value) { (this->*_GetTags_post_ptr._value)(tag1, tag2); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::PushLayer(
    _In_ CONST D2D1_LAYER_PARAMETERS * layerParameters,
    _In_opt_ ID2D1Layer * layer)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::PushLayer");
    if (_PushLayer_pre_ptr._value) { (this->*_PushLayer_pre_ptr._value)(layerParameters, layer); }
    GetRealObj()->PushLayer(layerParameters, HookedToReal(layer));
    if (_PushLayer_post_ptr._value) { (this->*_PushLayer_post_ptr._value)(layerParameters, layer); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::PopLayer()
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::PopLayer");
    if (_PopLayer_pre_ptr._value) { (this->*_PopLayer_pre_ptr._value)(); }
    GetRealObj()->PopLayer();
    if (_PopLayer_post_ptr._value) { (this->*_PopLayer_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::Flush(
    _Out_opt_ D2D1_TAG * tag1,
    _Out_opt_ D2D1_TAG * tag2)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::Flush");
    if (_Flush_pre_ptr._value) { (this->*_Flush_pre_ptr._value)(tag1, tag2); }
    HRESULT ret = GetRealObj()->Flush(tag1, tag2);
    if (_Flush_post_ptr._value) { (this->*_Flush_post_ptr._value)(ret, tag1, tag2); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::SaveDrawingState(
    _Inout_ ID2D1DrawingStateBlock * drawingStateBlock) const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::SaveDrawingState");
    if (_SaveDrawingState_pre_ptr._value) { (this->*_SaveDrawingState_pre_ptr._value)(drawingStateBlock); }
    GetRealObj()->SaveDrawingState(HookedToReal(drawingStateBlock));
    if (_SaveDrawingState_post_ptr._value) { (this->*_SaveDrawingState_post_ptr._value)(drawingStateBlock); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::RestoreDrawingState(
    _In_ ID2D1DrawingStateBlock * drawingStateBlock)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::RestoreDrawingState");
    if (_RestoreDrawingState_pre_ptr._value) { (this->*_RestoreDrawingState_pre_ptr._value)(drawingStateBlock); }
    GetRealObj()->RestoreDrawingState(HookedToReal(drawingStateBlock));
    if (_RestoreDrawingState_post_ptr._value) { (this->*_RestoreDrawingState_post_ptr._value)(drawingStateBlock); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::PushAxisAlignedClip(
    _In_ CONST D2D1_RECT_F * clipRect,
    D2D1_ANTIALIAS_MODE antialiasMode)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::PushAxisAlignedClip");
    if (_PushAxisAlignedClip_pre_ptr._value) { (this->*_PushAxisAlignedClip_pre_ptr._value)(clipRect, antialiasMode); }
    GetRealObj()->PushAxisAlignedClip(clipRect, antialiasMode);
    if (_PushAxisAlignedClip_post_ptr._value) { (this->*_PushAxisAlignedClip_post_ptr._value)(clipRect, antialiasMode); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::PopAxisAlignedClip()
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::PopAxisAlignedClip");
    if (_PopAxisAlignedClip_pre_ptr._value) { (this->*_PopAxisAlignedClip_pre_ptr._value)(); }
    GetRealObj()->PopAxisAlignedClip();
    if (_PopAxisAlignedClip_post_ptr._value) { (this->*_PopAxisAlignedClip_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::Clear(
    _In_opt_ CONST D2D1_COLOR_F * clearColor)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::Clear");
    if (_Clear_pre_ptr._value) { (this->*_Clear_pre_ptr._value)(clearColor); }
    GetRealObj()->Clear(clearColor);
    if (_Clear_post_ptr._value) { (this->*_Clear_post_ptr._value)(clearColor); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::BeginDraw()
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::BeginDraw");
    if (_BeginDraw_pre_ptr._value) { (this->*_BeginDraw_pre_ptr._value)(); }
    GetRealObj()->BeginDraw();
    if (_BeginDraw_post_ptr._value) { (this->*_BeginDraw_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1RenderTargetHook::EndDraw(
    _Out_opt_ D2D1_TAG * tag1,
    _Out_opt_ D2D1_TAG * tag2)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::EndDraw");
    if (_EndDraw_pre_ptr._value) { (this->*_EndDraw_pre_ptr._value)(tag1, tag2); }
    HRESULT ret = GetRealObj()->EndDraw(tag1, tag2);
    if (_EndDraw_post_ptr._value) { (this->*_EndDraw_post_ptr._value)(ret, tag1, tag2); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_PIXEL_FORMAT STDMETHODCALLTYPE D2D1RenderTargetHook::GetPixelFormat() const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetPixelFormat");
    if (_GetPixelFormat_pre_ptr._value) { (this->*_GetPixelFormat_pre_ptr._value)(); }
    D2D1_PIXEL_FORMAT ret = GetRealObj()->GetPixelFormat();
    if (_GetPixelFormat_post_ptr._value) { (this->*_GetPixelFormat_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::SetDpi(
    FLOAT dpiX,
    FLOAT dpiY)
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::SetDpi");
    if (_SetDpi_pre_ptr._value) { (this->*_SetDpi_pre_ptr._value)(dpiX, dpiY); }
    GetRealObj()->SetDpi(dpiX, dpiY);
    if (_SetDpi_post_ptr._value) { (this->*_SetDpi_post_ptr._value)(dpiX, dpiY); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1RenderTargetHook::GetDpi(
    _Out_ FLOAT * dpiX,
    _Out_ FLOAT * dpiY) const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetDpi");
    if (_GetDpi_pre_ptr._value) { (this->*_GetDpi_pre_ptr._value)(dpiX, dpiY); }
    GetRealObj()->GetDpi(dpiX, dpiY);
    if (_GetDpi_post_ptr._value) { (this->*_GetDpi_post_ptr._value)(dpiX, dpiY); }
}

// -----------------------------------------------------------------------------
D2D1_SIZE_F STDMETHODCALLTYPE D2D1RenderTargetHook::GetSize() const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetSize");
    if (_GetSize_pre_ptr._value) { (this->*_GetSize_pre_ptr._value)(); }
    D2D1_SIZE_F ret = GetRealObj()->GetSize();
    if (_GetSize_post_ptr._value) { (this->*_GetSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_SIZE_U STDMETHODCALLTYPE D2D1RenderTargetHook::GetPixelSize() const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetPixelSize");
    if (_GetPixelSize_pre_ptr._value) { (this->*_GetPixelSize_pre_ptr._value)(); }
    D2D1_SIZE_U ret = GetRealObj()->GetPixelSize();
    if (_GetPixelSize_post_ptr._value) { (this->*_GetPixelSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1RenderTargetHook::GetMaximumBitmapSize() const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::GetMaximumBitmapSize");
    if (_GetMaximumBitmapSize_pre_ptr._value) { (this->*_GetMaximumBitmapSize_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetMaximumBitmapSize();
    if (_GetMaximumBitmapSize_post_ptr._value) { (this->*_GetMaximumBitmapSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D2D1RenderTargetHook::IsSupported(
    _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties) const
{
    calltrace::AutoTrace trace(L"D2D1RenderTargetHook::IsSupported");
    if (_IsSupported_pre_ptr._value) { (this->*_IsSupported_pre_ptr._value)(renderTargetProperties); }
    BOOL ret = GetRealObj()->IsSupported(renderTargetProperties);
    if (_IsSupported_post_ptr._value) { (this->*_IsSupported_post_ptr._value)(ret, renderTargetProperties); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1BitmapRenderTargetHook::GetBitmap(
    _Outptr_ ID2D1Bitmap ** bitmap)
{
    calltrace::AutoTrace trace(L"D2D1BitmapRenderTargetHook::GetBitmap");
    if (_GetBitmap_pre_ptr._value) { (this->*_GetBitmap_pre_ptr._value)(bitmap); }
    HRESULT ret = GetRealObj()->GetBitmap(bitmap);
    if ( bitmap && *bitmap) { *bitmap = RealToHooked_D2D( *bitmap ); }
    if (_GetBitmap_post_ptr._value) { (this->*_GetBitmap_post_ptr._value)(ret, bitmap); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_WINDOW_STATE STDMETHODCALLTYPE D2D1HwndRenderTargetHook::CheckWindowState()
{
    calltrace::AutoTrace trace(L"D2D1HwndRenderTargetHook::CheckWindowState");
    if (_CheckWindowState_pre_ptr._value) { (this->*_CheckWindowState_pre_ptr._value)(); }
    D2D1_WINDOW_STATE ret = GetRealObj()->CheckWindowState();
    if (_CheckWindowState_post_ptr._value) { (this->*_CheckWindowState_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1HwndRenderTargetHook::Resize(
    _In_ CONST D2D1_SIZE_U * pixelSize)
{
    calltrace::AutoTrace trace(L"D2D1HwndRenderTargetHook::Resize");
    if (_Resize_pre_ptr._value) { (this->*_Resize_pre_ptr._value)(pixelSize); }
    HRESULT ret = GetRealObj()->Resize(pixelSize);
    if (_Resize_post_ptr._value) { (this->*_Resize_post_ptr._value)(ret, pixelSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HWND STDMETHODCALLTYPE D2D1HwndRenderTargetHook::GetHwnd() const
{
    calltrace::AutoTrace trace(L"D2D1HwndRenderTargetHook::GetHwnd");
    if (_GetHwnd_pre_ptr._value) { (this->*_GetHwnd_pre_ptr._value)(); }
    HWND ret = GetRealObj()->GetHwnd();
    if (_GetHwnd_post_ptr._value) { (this->*_GetHwnd_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GdiInteropRenderTargetHook::GetDC(
    D2D1_DC_INITIALIZE_MODE mode,
    _Out_ HDC * hdc)
{
    calltrace::AutoTrace trace(L"D2D1GdiInteropRenderTargetHook::GetDC");
    if (_GetDC_pre_ptr._value) { (this->*_GetDC_pre_ptr._value)(mode, hdc); }
    HRESULT ret = GetRealObj()->GetDC(mode, hdc);
    if (_GetDC_post_ptr._value) { (this->*_GetDC_post_ptr._value)(ret, mode, hdc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GdiInteropRenderTargetHook::ReleaseDC(
    _In_opt_ CONST RECT * update)
{
    calltrace::AutoTrace trace(L"D2D1GdiInteropRenderTargetHook::ReleaseDC");
    if (_ReleaseDC_pre_ptr._value) { (this->*_ReleaseDC_pre_ptr._value)(update); }
    HRESULT ret = GetRealObj()->ReleaseDC(update);
    if (_ReleaseDC_post_ptr._value) { (this->*_ReleaseDC_post_ptr._value)(ret, update); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DCRenderTargetHook::BindDC(
    _In_ CONST HDC hDC,
    _In_ CONST RECT * pSubRect)
{
    calltrace::AutoTrace trace(L"D2D1DCRenderTargetHook::BindDC");
    if (_BindDC_pre_ptr._value) { (this->*_BindDC_pre_ptr._value)(hDC, pSubRect); }
    HRESULT ret = GetRealObj()->BindDC(hDC, pSubRect);
    if (_BindDC_post_ptr._value) { (this->*_BindDC_post_ptr._value)(ret, hDC, pSubRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::ReloadSystemMetrics()
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::ReloadSystemMetrics");
    if (_ReloadSystemMetrics_pre_ptr._value) { (this->*_ReloadSystemMetrics_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->ReloadSystemMetrics();
    if (_ReloadSystemMetrics_post_ptr._value) { (this->*_ReloadSystemMetrics_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1FactoryHook::GetDesktopDpi(
    _Out_ FLOAT * dpiX,
    _Out_ FLOAT * dpiY)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::GetDesktopDpi");
    if (_GetDesktopDpi_pre_ptr._value) { (this->*_GetDesktopDpi_pre_ptr._value)(dpiX, dpiY); }
    GetRealObj()->GetDesktopDpi(dpiX, dpiY);
    if (_GetDesktopDpi_post_ptr._value) { (this->*_GetDesktopDpi_post_ptr._value)(dpiX, dpiY); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateRectangleGeometry(
    _In_ CONST D2D1_RECT_F * rectangle,
    _Outptr_ ID2D1RectangleGeometry ** rectangleGeometry)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateRectangleGeometry");
    if (_CreateRectangleGeometry_pre_ptr._value) { (this->*_CreateRectangleGeometry_pre_ptr._value)(rectangle, rectangleGeometry); }
    HRESULT ret = GetRealObj()->CreateRectangleGeometry(rectangle, rectangleGeometry);
    if ( rectangleGeometry && *rectangleGeometry) { *rectangleGeometry = RealToHooked_D2D( *rectangleGeometry ); }
    if (_CreateRectangleGeometry_post_ptr._value) { (this->*_CreateRectangleGeometry_post_ptr._value)(ret, rectangle, rectangleGeometry); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateRoundedRectangleGeometry(
    _In_ CONST D2D1_ROUNDED_RECT * roundedRectangle,
    _Outptr_ ID2D1RoundedRectangleGeometry ** roundedRectangleGeometry)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateRoundedRectangleGeometry");
    if (_CreateRoundedRectangleGeometry_pre_ptr._value) { (this->*_CreateRoundedRectangleGeometry_pre_ptr._value)(roundedRectangle, roundedRectangleGeometry); }
    HRESULT ret = GetRealObj()->CreateRoundedRectangleGeometry(roundedRectangle, roundedRectangleGeometry);
    if ( roundedRectangleGeometry && *roundedRectangleGeometry) { *roundedRectangleGeometry = RealToHooked_D2D( *roundedRectangleGeometry ); }
    if (_CreateRoundedRectangleGeometry_post_ptr._value) { (this->*_CreateRoundedRectangleGeometry_post_ptr._value)(ret, roundedRectangle, roundedRectangleGeometry); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateEllipseGeometry(
    _In_ CONST D2D1_ELLIPSE * ellipse,
    _Outptr_ ID2D1EllipseGeometry ** ellipseGeometry)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateEllipseGeometry");
    if (_CreateEllipseGeometry_pre_ptr._value) { (this->*_CreateEllipseGeometry_pre_ptr._value)(ellipse, ellipseGeometry); }
    HRESULT ret = GetRealObj()->CreateEllipseGeometry(ellipse, ellipseGeometry);
    if ( ellipseGeometry && *ellipseGeometry) { *ellipseGeometry = RealToHooked_D2D( *ellipseGeometry ); }
    if (_CreateEllipseGeometry_post_ptr._value) { (this->*_CreateEllipseGeometry_post_ptr._value)(ret, ellipse, ellipseGeometry); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateGeometryGroup(
    D2D1_FILL_MODE fillMode,
    _In_reads_(geometriesCount) ID2D1Geometry ** geometries,
    UINT32 geometriesCount,
    _Outptr_ ID2D1GeometryGroup ** geometryGroup)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateGeometryGroup");
    if (_CreateGeometryGroup_pre_ptr._value) { (this->*_CreateGeometryGroup_pre_ptr._value)(fillMode, geometries, geometriesCount, geometryGroup); }
    HRESULT ret = GetRealObj()->CreateGeometryGroup(fillMode, HookedToReal(geometriesCount, geometries), geometriesCount, geometryGroup);
    if ( geometryGroup && *geometryGroup) { *geometryGroup = RealToHooked_D2D( *geometryGroup ); }
    if (_CreateGeometryGroup_post_ptr._value) { (this->*_CreateGeometryGroup_post_ptr._value)(ret, fillMode, geometries, geometriesCount, geometryGroup); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateTransformedGeometry(
    _In_ ID2D1Geometry * sourceGeometry,
    _In_ CONST D2D1_MATRIX_3X2_F * transform,
    _Outptr_ ID2D1TransformedGeometry ** transformedGeometry)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateTransformedGeometry");
    if (_CreateTransformedGeometry_pre_ptr._value) { (this->*_CreateTransformedGeometry_pre_ptr._value)(sourceGeometry, transform, transformedGeometry); }
    HRESULT ret = GetRealObj()->CreateTransformedGeometry(HookedToReal(sourceGeometry), transform, transformedGeometry);
    if ( transformedGeometry && *transformedGeometry) { *transformedGeometry = RealToHooked_D2D( *transformedGeometry ); }
    if (_CreateTransformedGeometry_post_ptr._value) { (this->*_CreateTransformedGeometry_post_ptr._value)(ret, sourceGeometry, transform, transformedGeometry); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreatePathGeometry(
    _Outptr_ ID2D1PathGeometry ** pathGeometry)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreatePathGeometry");
    if (_CreatePathGeometry_pre_ptr._value) { (this->*_CreatePathGeometry_pre_ptr._value)(pathGeometry); }
    HRESULT ret = GetRealObj()->CreatePathGeometry(pathGeometry);
    if ( pathGeometry && *pathGeometry) { *pathGeometry = RealToHooked_D2D( *pathGeometry ); }
    if (_CreatePathGeometry_post_ptr._value) { (this->*_CreatePathGeometry_post_ptr._value)(ret, pathGeometry); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateStrokeStyle(
    _In_ CONST D2D1_STROKE_STYLE_PROPERTIES * strokeStyleProperties,
    _In_reads_opt_(dashesCount) CONST FLOAT * dashes,
    UINT32 dashesCount,
    _Outptr_ ID2D1StrokeStyle ** strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateStrokeStyle");
    if (_CreateStrokeStyle_pre_ptr._value) { (this->*_CreateStrokeStyle_pre_ptr._value)(strokeStyleProperties, dashes, dashesCount, strokeStyle); }
    HRESULT ret = GetRealObj()->CreateStrokeStyle(strokeStyleProperties, dashes, dashesCount, strokeStyle);
    if ( strokeStyle && *strokeStyle) { *strokeStyle = RealToHooked_D2D( *strokeStyle ); }
    if (_CreateStrokeStyle_post_ptr._value) { (this->*_CreateStrokeStyle_post_ptr._value)(ret, strokeStyleProperties, dashes, dashesCount, strokeStyle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateDrawingStateBlock(
    _In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION * drawingStateDescription,
    _In_opt_ IDWriteRenderingParams * textRenderingParams,
    _Outptr_ ID2D1DrawingStateBlock ** drawingStateBlock)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateDrawingStateBlock");
    if (_CreateDrawingStateBlock_pre_ptr._value) { (this->*_CreateDrawingStateBlock_pre_ptr._value)(drawingStateDescription, textRenderingParams, drawingStateBlock); }
    HRESULT ret = GetRealObj()->CreateDrawingStateBlock(drawingStateDescription, HookedToReal(textRenderingParams), drawingStateBlock);
    if ( drawingStateBlock && *drawingStateBlock) { *drawingStateBlock = RealToHooked_D2D( *drawingStateBlock ); }
    if (_CreateDrawingStateBlock_post_ptr._value) { (this->*_CreateDrawingStateBlock_post_ptr._value)(ret, drawingStateDescription, textRenderingParams, drawingStateBlock); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateWicBitmapRenderTarget(
    _In_ IWICBitmap * target,
    _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties,
    _Outptr_ ID2D1RenderTarget ** renderTarget)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateWicBitmapRenderTarget");
    if (_CreateWicBitmapRenderTarget_pre_ptr._value) { (this->*_CreateWicBitmapRenderTarget_pre_ptr._value)(target, renderTargetProperties, renderTarget); }
    HRESULT ret = GetRealObj()->CreateWicBitmapRenderTarget(target, renderTargetProperties, renderTarget);
    if ( renderTarget && *renderTarget) { *renderTarget = RealToHooked_D2D( *renderTarget ); }
    if (_CreateWicBitmapRenderTarget_post_ptr._value) { (this->*_CreateWicBitmapRenderTarget_post_ptr._value)(ret, target, renderTargetProperties, renderTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateHwndRenderTarget(
    _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties,
    _In_ CONST D2D1_HWND_RENDER_TARGET_PROPERTIES * hwndRenderTargetProperties,
    _Outptr_ ID2D1HwndRenderTarget ** hwndRenderTarget)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateHwndRenderTarget");
    if (_CreateHwndRenderTarget_pre_ptr._value) { (this->*_CreateHwndRenderTarget_pre_ptr._value)(renderTargetProperties, hwndRenderTargetProperties, hwndRenderTarget); }
    HRESULT ret = GetRealObj()->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTargetProperties, hwndRenderTarget);
    if ( hwndRenderTarget && *hwndRenderTarget) { *hwndRenderTarget = RealToHooked_D2D( *hwndRenderTarget ); }
    if (_CreateHwndRenderTarget_post_ptr._value) { (this->*_CreateHwndRenderTarget_post_ptr._value)(ret, renderTargetProperties, hwndRenderTargetProperties, hwndRenderTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateDxgiSurfaceRenderTarget(
    _In_ IDXGISurface * dxgiSurface,
    _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties,
    _Outptr_ ID2D1RenderTarget ** renderTarget)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateDxgiSurfaceRenderTarget");
    if (_CreateDxgiSurfaceRenderTarget_pre_ptr._value) { (this->*_CreateDxgiSurfaceRenderTarget_pre_ptr._value)(dxgiSurface, renderTargetProperties, renderTarget); }
    HRESULT ret = GetRealObj()->CreateDxgiSurfaceRenderTarget(HookedToReal(dxgiSurface), renderTargetProperties, renderTarget);
    if ( renderTarget && *renderTarget) { *renderTarget = RealToHooked_D2D( *renderTarget ); }
    if (_CreateDxgiSurfaceRenderTarget_post_ptr._value) { (this->*_CreateDxgiSurfaceRenderTarget_post_ptr._value)(ret, dxgiSurface, renderTargetProperties, renderTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1FactoryHook::CreateDCRenderTarget(
    _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties,
    _Outptr_ ID2D1DCRenderTarget ** dcRenderTarget)
{
    calltrace::AutoTrace trace(L"D2D1FactoryHook::CreateDCRenderTarget");
    if (_CreateDCRenderTarget_pre_ptr._value) { (this->*_CreateDCRenderTarget_pre_ptr._value)(renderTargetProperties, dcRenderTarget); }
    HRESULT ret = GetRealObj()->CreateDCRenderTarget(renderTargetProperties, dcRenderTarget);
    if ( dcRenderTarget && *dcRenderTarget) { *dcRenderTarget = RealToHooked_D2D( *dcRenderTarget ); }
    if (_CreateDCRenderTarget_post_ptr._value) { (this->*_CreateDCRenderTarget_post_ptr._value)(ret, renderTargetProperties, dcRenderTarget); }
    return ret;
}

