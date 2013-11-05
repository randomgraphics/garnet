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

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GdiMetafileSinkHook::ProcessRecord(
    DWORD recordType,
    _In_opt_ CONST void * recordData,
    DWORD recordDataSize)
{
    calltrace::AutoTrace trace(L"D2D1GdiMetafileSinkHook::ProcessRecord");
    if (_ProcessRecord_pre_ptr._value) { (this->*_ProcessRecord_pre_ptr._value)(recordType, recordData, recordDataSize); }
    HRESULT ret = GetRealObj()->ProcessRecord(recordType, recordData, recordDataSize);
    if (_ProcessRecord_post_ptr._value) { (this->*_ProcessRecord_post_ptr._value)(ret, recordType, recordData, recordDataSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GdiMetafileHook::Stream(
    _In_ ID2D1GdiMetafileSink * sink)
{
    calltrace::AutoTrace trace(L"D2D1GdiMetafileHook::Stream");
    if (_Stream_pre_ptr._value) { (this->*_Stream_pre_ptr._value)(sink); }
    HRESULT ret = GetRealObj()->Stream(HookedToReal(sink));
    if (_Stream_post_ptr._value) { (this->*_Stream_post_ptr._value)(ret, sink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1GdiMetafileHook::GetBounds(
    _Out_ D2D1_RECT_F * bounds)
{
    calltrace::AutoTrace trace(L"D2D1GdiMetafileHook::GetBounds");
    if (_GetBounds_pre_ptr._value) { (this->*_GetBounds_pre_ptr._value)(bounds); }
    HRESULT ret = GetRealObj()->GetBounds(bounds);
    if (_GetBounds_post_ptr._value) { (this->*_GetBounds_post_ptr._value)(ret, bounds); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::BeginDraw()
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::BeginDraw");
    if (_BeginDraw_pre_ptr._value) { (this->*_BeginDraw_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->BeginDraw();
    if (_BeginDraw_post_ptr._value) { (this->*_BeginDraw_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::EndDraw()
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::EndDraw");
    if (_EndDraw_pre_ptr._value) { (this->*_EndDraw_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->EndDraw();
    if (_EndDraw_post_ptr._value) { (this->*_EndDraw_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::SetAntialiasMode(
    D2D1_ANTIALIAS_MODE antialiasMode)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::SetAntialiasMode");
    if (_SetAntialiasMode_pre_ptr._value) { (this->*_SetAntialiasMode_pre_ptr._value)(antialiasMode); }
    HRESULT ret = GetRealObj()->SetAntialiasMode(antialiasMode);
    if (_SetAntialiasMode_post_ptr._value) { (this->*_SetAntialiasMode_post_ptr._value)(ret, antialiasMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::SetTags(
    D2D1_TAG tag1,
    D2D1_TAG tag2)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::SetTags");
    if (_SetTags_pre_ptr._value) { (this->*_SetTags_pre_ptr._value)(tag1, tag2); }
    HRESULT ret = GetRealObj()->SetTags(tag1, tag2);
    if (_SetTags_post_ptr._value) { (this->*_SetTags_post_ptr._value)(ret, tag1, tag2); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::SetTextAntialiasMode(
    D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::SetTextAntialiasMode");
    if (_SetTextAntialiasMode_pre_ptr._value) { (this->*_SetTextAntialiasMode_pre_ptr._value)(textAntialiasMode); }
    HRESULT ret = GetRealObj()->SetTextAntialiasMode(textAntialiasMode);
    if (_SetTextAntialiasMode_post_ptr._value) { (this->*_SetTextAntialiasMode_post_ptr._value)(ret, textAntialiasMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::SetTextRenderingParams(
    _In_opt_ IDWriteRenderingParams * textRenderingParams)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::SetTextRenderingParams");
    if (_SetTextRenderingParams_pre_ptr._value) { (this->*_SetTextRenderingParams_pre_ptr._value)(textRenderingParams); }
    HRESULT ret = GetRealObj()->SetTextRenderingParams(HookedToReal(textRenderingParams));
    if (_SetTextRenderingParams_post_ptr._value) { (this->*_SetTextRenderingParams_post_ptr._value)(ret, textRenderingParams); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::SetTransform(
    _In_ CONST D2D1_MATRIX_3X2_F * transform)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::SetTransform");
    if (_SetTransform_pre_ptr._value) { (this->*_SetTransform_pre_ptr._value)(transform); }
    HRESULT ret = GetRealObj()->SetTransform(transform);
    if (_SetTransform_post_ptr._value) { (this->*_SetTransform_post_ptr._value)(ret, transform); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::SetPrimitiveBlend(
    D2D1_PRIMITIVE_BLEND primitiveBlend)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::SetPrimitiveBlend");
    if (_SetPrimitiveBlend_pre_ptr._value) { (this->*_SetPrimitiveBlend_pre_ptr._value)(primitiveBlend); }
    HRESULT ret = GetRealObj()->SetPrimitiveBlend(primitiveBlend);
    if (_SetPrimitiveBlend_post_ptr._value) { (this->*_SetPrimitiveBlend_post_ptr._value)(ret, primitiveBlend); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::SetUnitMode(
    D2D1_UNIT_MODE unitMode)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::SetUnitMode");
    if (_SetUnitMode_pre_ptr._value) { (this->*_SetUnitMode_pre_ptr._value)(unitMode); }
    HRESULT ret = GetRealObj()->SetUnitMode(unitMode);
    if (_SetUnitMode_post_ptr._value) { (this->*_SetUnitMode_post_ptr._value)(ret, unitMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::Clear(
    _In_opt_ CONST D2D1_COLOR_F * color)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::Clear");
    if (_Clear_pre_ptr._value) { (this->*_Clear_pre_ptr._value)(color); }
    HRESULT ret = GetRealObj()->Clear(color);
    if (_Clear_post_ptr._value) { (this->*_Clear_post_ptr._value)(ret, color); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::DrawGlyphRun(
    D2D1_POINT_2F baselineOrigin,
    _In_ CONST DWRITE_GLYPH_RUN * glyphRun,
    _In_opt_ CONST DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription,
    _In_ ID2D1Brush * foregroundBrush,
    DWRITE_MEASURING_MODE measuringMode)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::DrawGlyphRun");
    if (_DrawGlyphRun_pre_ptr._value) { (this->*_DrawGlyphRun_pre_ptr._value)(baselineOrigin, glyphRun, glyphRunDescription, foregroundBrush, measuringMode); }
    HRESULT ret = GetRealObj()->DrawGlyphRun(baselineOrigin, glyphRun, glyphRunDescription, HookedToReal(foregroundBrush), measuringMode);
    if (_DrawGlyphRun_post_ptr._value) { (this->*_DrawGlyphRun_post_ptr._value)(ret, baselineOrigin, glyphRun, glyphRunDescription, foregroundBrush, measuringMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::DrawLine(
    D2D1_POINT_2F point0,
    D2D1_POINT_2F point1,
    _In_ ID2D1Brush * brush,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::DrawLine");
    if (_DrawLine_pre_ptr._value) { (this->*_DrawLine_pre_ptr._value)(point0, point1, brush, strokeWidth, strokeStyle); }
    HRESULT ret = GetRealObj()->DrawLine(point0, point1, HookedToReal(brush), strokeWidth, HookedToReal(strokeStyle));
    if (_DrawLine_post_ptr._value) { (this->*_DrawLine_post_ptr._value)(ret, point0, point1, brush, strokeWidth, strokeStyle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::DrawGeometry(
    _In_ ID2D1Geometry * geometry,
    _In_ ID2D1Brush * brush,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::DrawGeometry");
    if (_DrawGeometry_pre_ptr._value) { (this->*_DrawGeometry_pre_ptr._value)(geometry, brush, strokeWidth, strokeStyle); }
    HRESULT ret = GetRealObj()->DrawGeometry(HookedToReal(geometry), HookedToReal(brush), strokeWidth, HookedToReal(strokeStyle));
    if (_DrawGeometry_post_ptr._value) { (this->*_DrawGeometry_post_ptr._value)(ret, geometry, brush, strokeWidth, strokeStyle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::DrawRectangle(
    _In_ CONST D2D1_RECT_F * rect,
    _In_ ID2D1Brush * brush,
    FLOAT strokeWidth,
    _In_opt_ ID2D1StrokeStyle * strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::DrawRectangle");
    if (_DrawRectangle_pre_ptr._value) { (this->*_DrawRectangle_pre_ptr._value)(rect, brush, strokeWidth, strokeStyle); }
    HRESULT ret = GetRealObj()->DrawRectangle(rect, HookedToReal(brush), strokeWidth, HookedToReal(strokeStyle));
    if (_DrawRectangle_post_ptr._value) { (this->*_DrawRectangle_post_ptr._value)(ret, rect, brush, strokeWidth, strokeStyle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::DrawBitmap(
    _In_ ID2D1Bitmap * bitmap,
    _In_opt_ CONST D2D1_RECT_F * destinationRectangle,
    FLOAT opacity,
    D2D1_INTERPOLATION_MODE interpolationMode,
    _In_opt_ CONST D2D1_RECT_F * sourceRectangle,
    _In_opt_ CONST D2D1_MATRIX_4X4_F * perspectiveTransform)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::DrawBitmap");
    if (_DrawBitmap_pre_ptr._value) { (this->*_DrawBitmap_pre_ptr._value)(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle, perspectiveTransform); }
    HRESULT ret = GetRealObj()->DrawBitmap(HookedToReal(bitmap), destinationRectangle, opacity, interpolationMode, sourceRectangle, perspectiveTransform);
    if (_DrawBitmap_post_ptr._value) { (this->*_DrawBitmap_post_ptr._value)(ret, bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle, perspectiveTransform); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::DrawImage(
    _In_ ID2D1Image * image,
    _In_opt_ CONST D2D1_POINT_2F * targetOffset,
    _In_opt_ CONST D2D1_RECT_F * imageRectangle,
    D2D1_INTERPOLATION_MODE interpolationMode,
    D2D1_COMPOSITE_MODE compositeMode)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::DrawImage");
    if (_DrawImage_pre_ptr._value) { (this->*_DrawImage_pre_ptr._value)(image, targetOffset, imageRectangle, interpolationMode, compositeMode); }
    HRESULT ret = GetRealObj()->DrawImage(HookedToReal(image), targetOffset, imageRectangle, interpolationMode, compositeMode);
    if (_DrawImage_post_ptr._value) { (this->*_DrawImage_post_ptr._value)(ret, image, targetOffset, imageRectangle, interpolationMode, compositeMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::DrawGdiMetafile(
    _In_ ID2D1GdiMetafile * gdiMetafile,
    _In_opt_ CONST D2D1_POINT_2F * targetOffset)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::DrawGdiMetafile");
    if (_DrawGdiMetafile_pre_ptr._value) { (this->*_DrawGdiMetafile_pre_ptr._value)(gdiMetafile, targetOffset); }
    HRESULT ret = GetRealObj()->DrawGdiMetafile(HookedToReal(gdiMetafile), targetOffset);
    if (_DrawGdiMetafile_post_ptr._value) { (this->*_DrawGdiMetafile_post_ptr._value)(ret, gdiMetafile, targetOffset); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::FillMesh(
    _In_ ID2D1Mesh * mesh,
    _In_ ID2D1Brush * brush)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::FillMesh");
    if (_FillMesh_pre_ptr._value) { (this->*_FillMesh_pre_ptr._value)(mesh, brush); }
    HRESULT ret = GetRealObj()->FillMesh(HookedToReal(mesh), HookedToReal(brush));
    if (_FillMesh_post_ptr._value) { (this->*_FillMesh_post_ptr._value)(ret, mesh, brush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::FillOpacityMask(
    _In_ ID2D1Bitmap * opacityMask,
    _In_ ID2D1Brush * brush,
    _In_opt_ CONST D2D1_RECT_F * destinationRectangle,
    _In_opt_ CONST D2D1_RECT_F * sourceRectangle)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::FillOpacityMask");
    if (_FillOpacityMask_pre_ptr._value) { (this->*_FillOpacityMask_pre_ptr._value)(opacityMask, brush, destinationRectangle, sourceRectangle); }
    HRESULT ret = GetRealObj()->FillOpacityMask(HookedToReal(opacityMask), HookedToReal(brush), destinationRectangle, sourceRectangle);
    if (_FillOpacityMask_post_ptr._value) { (this->*_FillOpacityMask_post_ptr._value)(ret, opacityMask, brush, destinationRectangle, sourceRectangle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::FillGeometry(
    _In_ ID2D1Geometry * geometry,
    _In_ ID2D1Brush * brush,
    _In_opt_ ID2D1Brush * opacityBrush)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::FillGeometry");
    if (_FillGeometry_pre_ptr._value) { (this->*_FillGeometry_pre_ptr._value)(geometry, brush, opacityBrush); }
    HRESULT ret = GetRealObj()->FillGeometry(HookedToReal(geometry), HookedToReal(brush), HookedToReal(opacityBrush));
    if (_FillGeometry_post_ptr._value) { (this->*_FillGeometry_post_ptr._value)(ret, geometry, brush, opacityBrush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::FillRectangle(
    _In_ CONST D2D1_RECT_F * rect,
    _In_ ID2D1Brush * brush)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::FillRectangle");
    if (_FillRectangle_pre_ptr._value) { (this->*_FillRectangle_pre_ptr._value)(rect, brush); }
    HRESULT ret = GetRealObj()->FillRectangle(rect, HookedToReal(brush));
    if (_FillRectangle_post_ptr._value) { (this->*_FillRectangle_post_ptr._value)(ret, rect, brush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::PushAxisAlignedClip(
    _In_ CONST D2D1_RECT_F * clipRect,
    D2D1_ANTIALIAS_MODE antialiasMode)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::PushAxisAlignedClip");
    if (_PushAxisAlignedClip_pre_ptr._value) { (this->*_PushAxisAlignedClip_pre_ptr._value)(clipRect, antialiasMode); }
    HRESULT ret = GetRealObj()->PushAxisAlignedClip(clipRect, antialiasMode);
    if (_PushAxisAlignedClip_post_ptr._value) { (this->*_PushAxisAlignedClip_post_ptr._value)(ret, clipRect, antialiasMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::PushLayer(
    _In_ CONST D2D1_LAYER_PARAMETERS1 * layerParameters1,
    _In_opt_ ID2D1Layer * layer)
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::PushLayer");
    if (_PushLayer_pre_ptr._value) { (this->*_PushLayer_pre_ptr._value)(layerParameters1, layer); }
    HRESULT ret = GetRealObj()->PushLayer(layerParameters1, HookedToReal(layer));
    if (_PushLayer_post_ptr._value) { (this->*_PushLayer_post_ptr._value)(ret, layerParameters1, layer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::PopAxisAlignedClip()
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::PopAxisAlignedClip");
    if (_PopAxisAlignedClip_pre_ptr._value) { (this->*_PopAxisAlignedClip_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->PopAxisAlignedClip();
    if (_PopAxisAlignedClip_post_ptr._value) { (this->*_PopAxisAlignedClip_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandSinkHook::PopLayer()
{
    calltrace::AutoTrace trace(L"D2D1CommandSinkHook::PopLayer");
    if (_PopLayer_pre_ptr._value) { (this->*_PopLayer_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->PopLayer();
    if (_PopLayer_post_ptr._value) { (this->*_PopLayer_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandListHook::Stream(
    _In_ ID2D1CommandSink * sink)
{
    calltrace::AutoTrace trace(L"D2D1CommandListHook::Stream");
    if (_Stream_pre_ptr._value) { (this->*_Stream_pre_ptr._value)(sink); }
    HRESULT ret = GetRealObj()->Stream(HookedToReal(sink));
    if (_Stream_post_ptr._value) { (this->*_Stream_post_ptr._value)(ret, sink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1CommandListHook::Close()
{
    calltrace::AutoTrace trace(L"D2D1CommandListHook::Close");
    if (_Close_pre_ptr._value) { (this->*_Close_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Close();
    if (_Close_post_ptr._value) { (this->*_Close_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PrintControlHook::AddPage(
    _In_ ID2D1CommandList * commandList,
    D2D_SIZE_F pageSize,
    _In_opt_ IStream * pagePrintTicketStream,
    _Out_opt_ D2D1_TAG * tag1,
    _Out_opt_ D2D1_TAG * tag2)
{
    calltrace::AutoTrace trace(L"D2D1PrintControlHook::AddPage");
    if (_AddPage_pre_ptr._value) { (this->*_AddPage_pre_ptr._value)(commandList, pageSize, pagePrintTicketStream, tag1, tag2); }
    HRESULT ret = GetRealObj()->AddPage(HookedToReal(commandList), pageSize, pagePrintTicketStream, tag1, tag2);
    if (_AddPage_post_ptr._value) { (this->*_AddPage_post_ptr._value)(ret, commandList, pageSize, pagePrintTicketStream, tag1, tag2); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PrintControlHook::Close()
{
    calltrace::AutoTrace trace(L"D2D1PrintControlHook::Close");
    if (_Close_pre_ptr._value) { (this->*_Close_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Close();
    if (_Close_post_ptr._value) { (this->*_Close_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1ImageBrushHook::SetImage(
    _In_opt_ ID2D1Image * image)
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::SetImage");
    if (_SetImage_pre_ptr._value) { (this->*_SetImage_pre_ptr._value)(image); }
    GetRealObj()->SetImage(HookedToReal(image));
    if (_SetImage_post_ptr._value) { (this->*_SetImage_post_ptr._value)(image); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1ImageBrushHook::SetExtendModeX(
    D2D1_EXTEND_MODE extendModeX)
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::SetExtendModeX");
    if (_SetExtendModeX_pre_ptr._value) { (this->*_SetExtendModeX_pre_ptr._value)(extendModeX); }
    GetRealObj()->SetExtendModeX(extendModeX);
    if (_SetExtendModeX_post_ptr._value) { (this->*_SetExtendModeX_post_ptr._value)(extendModeX); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1ImageBrushHook::SetExtendModeY(
    D2D1_EXTEND_MODE extendModeY)
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::SetExtendModeY");
    if (_SetExtendModeY_pre_ptr._value) { (this->*_SetExtendModeY_pre_ptr._value)(extendModeY); }
    GetRealObj()->SetExtendModeY(extendModeY);
    if (_SetExtendModeY_post_ptr._value) { (this->*_SetExtendModeY_post_ptr._value)(extendModeY); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1ImageBrushHook::SetInterpolationMode(
    D2D1_INTERPOLATION_MODE interpolationMode)
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::SetInterpolationMode");
    if (_SetInterpolationMode_pre_ptr._value) { (this->*_SetInterpolationMode_pre_ptr._value)(interpolationMode); }
    GetRealObj()->SetInterpolationMode(interpolationMode);
    if (_SetInterpolationMode_post_ptr._value) { (this->*_SetInterpolationMode_post_ptr._value)(interpolationMode); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1ImageBrushHook::SetSourceRectangle(
    _In_ CONST D2D1_RECT_F * sourceRectangle)
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::SetSourceRectangle");
    if (_SetSourceRectangle_pre_ptr._value) { (this->*_SetSourceRectangle_pre_ptr._value)(sourceRectangle); }
    GetRealObj()->SetSourceRectangle(sourceRectangle);
    if (_SetSourceRectangle_post_ptr._value) { (this->*_SetSourceRectangle_post_ptr._value)(sourceRectangle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1ImageBrushHook::GetImage(
    _Outptr_ ID2D1Image ** image) const
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::GetImage");
    if (_GetImage_pre_ptr._value) { (this->*_GetImage_pre_ptr._value)(image); }
    GetRealObj()->GetImage(image);
    if ( image && *image) { *image = RealToHooked_D2D( *image ); }
    if (_GetImage_post_ptr._value) { (this->*_GetImage_post_ptr._value)(image); }
}

// -----------------------------------------------------------------------------
D2D1_EXTEND_MODE STDMETHODCALLTYPE D2D1ImageBrushHook::GetExtendModeX() const
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::GetExtendModeX");
    if (_GetExtendModeX_pre_ptr._value) { (this->*_GetExtendModeX_pre_ptr._value)(); }
    D2D1_EXTEND_MODE ret = GetRealObj()->GetExtendModeX();
    if (_GetExtendModeX_post_ptr._value) { (this->*_GetExtendModeX_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_EXTEND_MODE STDMETHODCALLTYPE D2D1ImageBrushHook::GetExtendModeY() const
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::GetExtendModeY");
    if (_GetExtendModeY_pre_ptr._value) { (this->*_GetExtendModeY_pre_ptr._value)(); }
    D2D1_EXTEND_MODE ret = GetRealObj()->GetExtendModeY();
    if (_GetExtendModeY_post_ptr._value) { (this->*_GetExtendModeY_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_INTERPOLATION_MODE STDMETHODCALLTYPE D2D1ImageBrushHook::GetInterpolationMode() const
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::GetInterpolationMode");
    if (_GetInterpolationMode_pre_ptr._value) { (this->*_GetInterpolationMode_pre_ptr._value)(); }
    D2D1_INTERPOLATION_MODE ret = GetRealObj()->GetInterpolationMode();
    if (_GetInterpolationMode_post_ptr._value) { (this->*_GetInterpolationMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1ImageBrushHook::GetSourceRectangle(
    _Out_ D2D1_RECT_F * sourceRectangle) const
{
    calltrace::AutoTrace trace(L"D2D1ImageBrushHook::GetSourceRectangle");
    if (_GetSourceRectangle_pre_ptr._value) { (this->*_GetSourceRectangle_pre_ptr._value)(sourceRectangle); }
    GetRealObj()->GetSourceRectangle(sourceRectangle);
    if (_GetSourceRectangle_post_ptr._value) { (this->*_GetSourceRectangle_post_ptr._value)(sourceRectangle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1BitmapBrush1Hook::SetInterpolationMode1(
    D2D1_INTERPOLATION_MODE interpolationMode)
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrush1Hook::SetInterpolationMode1");
    if (_SetInterpolationMode1_pre_ptr._value) { (this->*_SetInterpolationMode1_pre_ptr._value)(interpolationMode); }
    GetRealObj()->SetInterpolationMode1(interpolationMode);
    if (_SetInterpolationMode1_post_ptr._value) { (this->*_SetInterpolationMode1_post_ptr._value)(interpolationMode); }
}

// -----------------------------------------------------------------------------
D2D1_INTERPOLATION_MODE STDMETHODCALLTYPE D2D1BitmapBrush1Hook::GetInterpolationMode1() const
{
    calltrace::AutoTrace trace(L"D2D1BitmapBrush1Hook::GetInterpolationMode1");
    if (_GetInterpolationMode1_pre_ptr._value) { (this->*_GetInterpolationMode1_pre_ptr._value)(); }
    D2D1_INTERPOLATION_MODE ret = GetRealObj()->GetInterpolationMode1();
    if (_GetInterpolationMode1_post_ptr._value) { (this->*_GetInterpolationMode1_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_STROKE_TRANSFORM_TYPE STDMETHODCALLTYPE D2D1StrokeStyle1Hook::GetStrokeTransformType() const
{
    calltrace::AutoTrace trace(L"D2D1StrokeStyle1Hook::GetStrokeTransformType");
    if (_GetStrokeTransformType_pre_ptr._value) { (this->*_GetStrokeTransformType_pre_ptr._value)(); }
    D2D1_STROKE_TRANSFORM_TYPE ret = GetRealObj()->GetStrokeTransformType();
    if (_GetStrokeTransformType_post_ptr._value) { (this->*_GetStrokeTransformType_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PathGeometry1Hook::ComputePointAndSegmentAtLength(
    FLOAT length,
    UINT32 startSegment,
    _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform,
    FLOAT flatteningTolerance,
    _Out_ D2D1_POINT_DESCRIPTION * pointDescription) const
{
    calltrace::AutoTrace trace(L"D2D1PathGeometry1Hook::ComputePointAndSegmentAtLength");
    if (_ComputePointAndSegmentAtLength_pre_ptr._value) { (this->*_ComputePointAndSegmentAtLength_pre_ptr._value)(length, startSegment, worldTransform, flatteningTolerance, pointDescription); }
    HRESULT ret = GetRealObj()->ComputePointAndSegmentAtLength(length, startSegment, worldTransform, flatteningTolerance, pointDescription);
    if (_ComputePointAndSegmentAtLength_post_ptr._value) { (this->*_ComputePointAndSegmentAtLength_post_ptr._value)(ret, length, startSegment, worldTransform, flatteningTolerance, pointDescription); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1PropertiesHook::GetPropertyCount() const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetPropertyCount");
    if (_GetPropertyCount_pre_ptr._value) { (this->*_GetPropertyCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetPropertyCount();
    if (_GetPropertyCount_post_ptr._value) { (this->*_GetPropertyCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PropertiesHook::GetPropertyName(
    UINT32 index,
    _Out_writes_(nameCount) PWSTR name,
    UINT32 nameCount) const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetPropertyName");
    if (_GetPropertyName_pre_ptr._value) { (this->*_GetPropertyName_pre_ptr._value)(index, name, nameCount); }
    HRESULT ret = GetRealObj()->GetPropertyName(index, name, nameCount);
    if (_GetPropertyName_post_ptr._value) { (this->*_GetPropertyName_post_ptr._value)(ret, index, name, nameCount); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1PropertiesHook::GetPropertyNameLength(
    UINT32 index) const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetPropertyNameLength");
    if (_GetPropertyNameLength_pre_ptr._value) { (this->*_GetPropertyNameLength_pre_ptr._value)(index); }
    UINT32 ret = GetRealObj()->GetPropertyNameLength(index);
    if (_GetPropertyNameLength_post_ptr._value) { (this->*_GetPropertyNameLength_post_ptr._value)(ret, index); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_PROPERTY_TYPE STDMETHODCALLTYPE D2D1PropertiesHook::GetType(
    UINT32 index) const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetType");
    if (_GetType_pre_ptr._value) { (this->*_GetType_pre_ptr._value)(index); }
    D2D1_PROPERTY_TYPE ret = GetRealObj()->GetType(index);
    if (_GetType_post_ptr._value) { (this->*_GetType_post_ptr._value)(ret, index); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1PropertiesHook::GetPropertyIndex(
    _In_ PCWSTR name) const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetPropertyIndex");
    if (_GetPropertyIndex_pre_ptr._value) { (this->*_GetPropertyIndex_pre_ptr._value)(name); }
    UINT32 ret = GetRealObj()->GetPropertyIndex(name);
    if (_GetPropertyIndex_post_ptr._value) { (this->*_GetPropertyIndex_post_ptr._value)(ret, name); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PropertiesHook::SetValueByName(
    _In_ PCWSTR name,
    D2D1_PROPERTY_TYPE type,
    _In_reads_(dataSize) CONST BYTE * data,
    UINT32 dataSize)
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::SetValueByName");
    if (_SetValueByName_pre_ptr._value) { (this->*_SetValueByName_pre_ptr._value)(name, type, data, dataSize); }
    HRESULT ret = GetRealObj()->SetValueByName(name, type, data, dataSize);
    if (_SetValueByName_post_ptr._value) { (this->*_SetValueByName_post_ptr._value)(ret, name, type, data, dataSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PropertiesHook::SetValue(
    UINT32 index,
    D2D1_PROPERTY_TYPE type,
    _In_reads_(dataSize) CONST BYTE * data,
    UINT32 dataSize)
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::SetValue");
    if (_SetValue_pre_ptr._value) { (this->*_SetValue_pre_ptr._value)(index, type, data, dataSize); }
    HRESULT ret = GetRealObj()->SetValue(index, type, data, dataSize);
    if (_SetValue_post_ptr._value) { (this->*_SetValue_post_ptr._value)(ret, index, type, data, dataSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PropertiesHook::GetValueByName(
    _In_ PCWSTR name,
    D2D1_PROPERTY_TYPE type,
    _Out_writes_(dataSize) BYTE * data,
    UINT32 dataSize) const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetValueByName");
    if (_GetValueByName_pre_ptr._value) { (this->*_GetValueByName_pre_ptr._value)(name, type, data, dataSize); }
    HRESULT ret = GetRealObj()->GetValueByName(name, type, data, dataSize);
    if (_GetValueByName_post_ptr._value) { (this->*_GetValueByName_post_ptr._value)(ret, name, type, data, dataSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PropertiesHook::GetValue(
    UINT32 index,
    D2D1_PROPERTY_TYPE type,
    _Out_writes_(dataSize) BYTE * data,
    UINT32 dataSize) const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetValue");
    if (_GetValue_pre_ptr._value) { (this->*_GetValue_pre_ptr._value)(index, type, data, dataSize); }
    HRESULT ret = GetRealObj()->GetValue(index, type, data, dataSize);
    if (_GetValue_post_ptr._value) { (this->*_GetValue_post_ptr._value)(ret, index, type, data, dataSize); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1PropertiesHook::GetValueSize(
    UINT32 index) const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetValueSize");
    if (_GetValueSize_pre_ptr._value) { (this->*_GetValueSize_pre_ptr._value)(index); }
    UINT32 ret = GetRealObj()->GetValueSize(index);
    if (_GetValueSize_post_ptr._value) { (this->*_GetValueSize_post_ptr._value)(ret, index); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1PropertiesHook::GetSubProperties(
    UINT32 index,
    _Outptr_opt_ ID2D1Properties ** subProperties) const
{
    calltrace::AutoTrace trace(L"D2D1PropertiesHook::GetSubProperties");
    if (_GetSubProperties_pre_ptr._value) { (this->*_GetSubProperties_pre_ptr._value)(index, subProperties); }
    HRESULT ret = GetRealObj()->GetSubProperties(index, subProperties);
    if ( subProperties && *subProperties) { *subProperties = RealToHooked_D2D( *subProperties ); }
    if (_GetSubProperties_post_ptr._value) { (this->*_GetSubProperties_post_ptr._value)(ret, index, subProperties); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1EffectHook::SetInput(
    UINT32 index,
    _In_opt_ ID2D1Image * input,
    BOOL invalidate)
{
    calltrace::AutoTrace trace(L"D2D1EffectHook::SetInput");
    if (_SetInput_pre_ptr._value) { (this->*_SetInput_pre_ptr._value)(index, input, invalidate); }
    GetRealObj()->SetInput(index, HookedToReal(input), invalidate);
    if (_SetInput_post_ptr._value) { (this->*_SetInput_post_ptr._value)(index, input, invalidate); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1EffectHook::SetInputCount(
    UINT32 inputCount)
{
    calltrace::AutoTrace trace(L"D2D1EffectHook::SetInputCount");
    if (_SetInputCount_pre_ptr._value) { (this->*_SetInputCount_pre_ptr._value)(inputCount); }
    HRESULT ret = GetRealObj()->SetInputCount(inputCount);
    if (_SetInputCount_post_ptr._value) { (this->*_SetInputCount_post_ptr._value)(ret, inputCount); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1EffectHook::GetInput(
    UINT32 index,
    _Outptr_opt_ ID2D1Image ** input) const
{
    calltrace::AutoTrace trace(L"D2D1EffectHook::GetInput");
    if (_GetInput_pre_ptr._value) { (this->*_GetInput_pre_ptr._value)(index, input); }
    GetRealObj()->GetInput(index, input);
    if ( input && *input) { *input = RealToHooked_D2D( *input ); }
    if (_GetInput_post_ptr._value) { (this->*_GetInput_post_ptr._value)(index, input); }
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1EffectHook::GetInputCount() const
{
    calltrace::AutoTrace trace(L"D2D1EffectHook::GetInputCount");
    if (_GetInputCount_pre_ptr._value) { (this->*_GetInputCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetInputCount();
    if (_GetInputCount_post_ptr._value) { (this->*_GetInputCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1EffectHook::GetOutput(
    _Outptr_ ID2D1Image ** outputImage) const
{
    calltrace::AutoTrace trace(L"D2D1EffectHook::GetOutput");
    if (_GetOutput_pre_ptr._value) { (this->*_GetOutput_pre_ptr._value)(outputImage); }
    GetRealObj()->GetOutput(outputImage);
    if ( outputImage && *outputImage) { *outputImage = RealToHooked_D2D( *outputImage ); }
    if (_GetOutput_post_ptr._value) { (this->*_GetOutput_post_ptr._value)(outputImage); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1Bitmap1Hook::GetColorContext(
    _Outptr_result_maybenull_ ID2D1ColorContext ** colorContext) const
{
    calltrace::AutoTrace trace(L"D2D1Bitmap1Hook::GetColorContext");
    if (_GetColorContext_pre_ptr._value) { (this->*_GetColorContext_pre_ptr._value)(colorContext); }
    GetRealObj()->GetColorContext(colorContext);
    if ( colorContext && *colorContext) { *colorContext = RealToHooked_D2D( *colorContext ); }
    if (_GetColorContext_post_ptr._value) { (this->*_GetColorContext_post_ptr._value)(colorContext); }
}

// -----------------------------------------------------------------------------
D2D1_BITMAP_OPTIONS STDMETHODCALLTYPE D2D1Bitmap1Hook::GetOptions() const
{
    calltrace::AutoTrace trace(L"D2D1Bitmap1Hook::GetOptions");
    if (_GetOptions_pre_ptr._value) { (this->*_GetOptions_pre_ptr._value)(); }
    D2D1_BITMAP_OPTIONS ret = GetRealObj()->GetOptions();
    if (_GetOptions_post_ptr._value) { (this->*_GetOptions_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Bitmap1Hook::GetSurface(
    _Outptr_result_maybenull_ IDXGISurface ** dxgiSurface) const
{
    calltrace::AutoTrace trace(L"D2D1Bitmap1Hook::GetSurface");
    if (_GetSurface_pre_ptr._value) { (this->*_GetSurface_pre_ptr._value)(dxgiSurface); }
    HRESULT ret = GetRealObj()->GetSurface(dxgiSurface);
    if ( dxgiSurface && *dxgiSurface) { *dxgiSurface = RealToHooked_D2D( *dxgiSurface ); }
    if (_GetSurface_post_ptr._value) { (this->*_GetSurface_post_ptr._value)(ret, dxgiSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Bitmap1Hook::Map(
    D2D1_MAP_OPTIONS options,
    _Out_ D2D1_MAPPED_RECT * mappedRect)
{
    calltrace::AutoTrace trace(L"D2D1Bitmap1Hook::Map");
    if (_Map_pre_ptr._value) { (this->*_Map_pre_ptr._value)(options, mappedRect); }
    HRESULT ret = GetRealObj()->Map(options, mappedRect);
    if (_Map_post_ptr._value) { (this->*_Map_post_ptr._value)(ret, options, mappedRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Bitmap1Hook::Unmap()
{
    calltrace::AutoTrace trace(L"D2D1Bitmap1Hook::Unmap");
    if (_Unmap_pre_ptr._value) { (this->*_Unmap_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Unmap();
    if (_Unmap_post_ptr._value) { (this->*_Unmap_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_COLOR_SPACE STDMETHODCALLTYPE D2D1ColorContextHook::GetColorSpace() const
{
    calltrace::AutoTrace trace(L"D2D1ColorContextHook::GetColorSpace");
    if (_GetColorSpace_pre_ptr._value) { (this->*_GetColorSpace_pre_ptr._value)(); }
    D2D1_COLOR_SPACE ret = GetRealObj()->GetColorSpace();
    if (_GetColorSpace_post_ptr._value) { (this->*_GetColorSpace_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE D2D1ColorContextHook::GetProfileSize() const
{
    calltrace::AutoTrace trace(L"D2D1ColorContextHook::GetProfileSize");
    if (_GetProfileSize_pre_ptr._value) { (this->*_GetProfileSize_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetProfileSize();
    if (_GetProfileSize_post_ptr._value) { (this->*_GetProfileSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1ColorContextHook::GetProfile(
    _Out_writes_(profileSize) BYTE * profile,
    UINT32 profileSize) const
{
    calltrace::AutoTrace trace(L"D2D1ColorContextHook::GetProfile");
    if (_GetProfile_pre_ptr._value) { (this->*_GetProfile_pre_ptr._value)(profile, profileSize); }
    HRESULT ret = GetRealObj()->GetProfile(profile, profileSize);
    if (_GetProfile_post_ptr._value) { (this->*_GetProfile_post_ptr._value)(ret, profile, profileSize); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1GradientStopCollection1Hook::GetGradientStops1(
    _Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP * gradientStops,
    UINT32 gradientStopsCount) const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollection1Hook::GetGradientStops1");
    if (_GetGradientStops1_pre_ptr._value) { (this->*_GetGradientStops1_pre_ptr._value)(gradientStops, gradientStopsCount); }
    GetRealObj()->GetGradientStops1(gradientStops, gradientStopsCount);
    if (_GetGradientStops1_post_ptr._value) { (this->*_GetGradientStops1_post_ptr._value)(gradientStops, gradientStopsCount); }
}

// -----------------------------------------------------------------------------
D2D1_COLOR_SPACE STDMETHODCALLTYPE D2D1GradientStopCollection1Hook::GetPreInterpolationSpace() const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollection1Hook::GetPreInterpolationSpace");
    if (_GetPreInterpolationSpace_pre_ptr._value) { (this->*_GetPreInterpolationSpace_pre_ptr._value)(); }
    D2D1_COLOR_SPACE ret = GetRealObj()->GetPreInterpolationSpace();
    if (_GetPreInterpolationSpace_post_ptr._value) { (this->*_GetPreInterpolationSpace_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_COLOR_SPACE STDMETHODCALLTYPE D2D1GradientStopCollection1Hook::GetPostInterpolationSpace() const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollection1Hook::GetPostInterpolationSpace");
    if (_GetPostInterpolationSpace_pre_ptr._value) { (this->*_GetPostInterpolationSpace_pre_ptr._value)(); }
    D2D1_COLOR_SPACE ret = GetRealObj()->GetPostInterpolationSpace();
    if (_GetPostInterpolationSpace_post_ptr._value) { (this->*_GetPostInterpolationSpace_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_BUFFER_PRECISION STDMETHODCALLTYPE D2D1GradientStopCollection1Hook::GetBufferPrecision() const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollection1Hook::GetBufferPrecision");
    if (_GetBufferPrecision_pre_ptr._value) { (this->*_GetBufferPrecision_pre_ptr._value)(); }
    D2D1_BUFFER_PRECISION ret = GetRealObj()->GetBufferPrecision();
    if (_GetBufferPrecision_post_ptr._value) { (this->*_GetBufferPrecision_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
D2D1_COLOR_INTERPOLATION_MODE STDMETHODCALLTYPE D2D1GradientStopCollection1Hook::GetColorInterpolationMode() const
{
    calltrace::AutoTrace trace(L"D2D1GradientStopCollection1Hook::GetColorInterpolationMode");
    if (_GetColorInterpolationMode_pre_ptr._value) { (this->*_GetColorInterpolationMode_pre_ptr._value)(); }
    D2D1_COLOR_INTERPOLATION_MODE ret = GetRealObj()->GetColorInterpolationMode();
    if (_GetColorInterpolationMode_post_ptr._value) { (this->*_GetColorInterpolationMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DrawingStateBlock1Hook::GetDescription(
    _Out_ D2D1_DRAWING_STATE_DESCRIPTION1 * stateDescription) const
{
    calltrace::AutoTrace trace(L"D2D1DrawingStateBlock1Hook::GetDescription");
    if (_GetDescription_pre_ptr._value) { (this->*_GetDescription_pre_ptr._value)(stateDescription); }
    GetRealObj()->GetDescription(stateDescription);
    if (_GetDescription_post_ptr._value) { (this->*_GetDescription_post_ptr._value)(stateDescription); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DrawingStateBlock1Hook::SetDescription(
    _In_ CONST D2D1_DRAWING_STATE_DESCRIPTION1 * stateDescription)
{
    calltrace::AutoTrace trace(L"D2D1DrawingStateBlock1Hook::SetDescription");
    if (_SetDescription_pre_ptr._value) { (this->*_SetDescription_pre_ptr._value)(stateDescription); }
    GetRealObj()->SetDescription(stateDescription);
    if (_SetDescription_post_ptr._value) { (this->*_SetDescription_post_ptr._value)(stateDescription); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateBitmap(
    D2D1_SIZE_U size,
    _In_opt_ CONST void * sourceData,
    UINT32 pitch,
    _In_ CONST D2D1_BITMAP_PROPERTIES1 * bitmapProperties,
    _Outptr_ ID2D1Bitmap1 ** bitmap)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateBitmap");
    if (_CreateBitmap_pre_ptr._value) { (this->*_CreateBitmap_pre_ptr._value)(size, sourceData, pitch, bitmapProperties, bitmap); }
    HRESULT ret = GetRealObj()->CreateBitmap(size, sourceData, pitch, bitmapProperties, bitmap);
    if ( bitmap && *bitmap) { *bitmap = RealToHooked_D2D( *bitmap ); }
    if (_CreateBitmap_post_ptr._value) { (this->*_CreateBitmap_post_ptr._value)(ret, size, sourceData, pitch, bitmapProperties, bitmap); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateBitmapFromWicBitmap(
    _In_ IWICBitmapSource * wicBitmapSource,
    _In_opt_ CONST D2D1_BITMAP_PROPERTIES1 * bitmapProperties,
    _Outptr_ ID2D1Bitmap1 ** bitmap)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateBitmapFromWicBitmap");
    if (_CreateBitmapFromWicBitmap_pre_ptr._value) { (this->*_CreateBitmapFromWicBitmap_pre_ptr._value)(wicBitmapSource, bitmapProperties, bitmap); }
    HRESULT ret = GetRealObj()->CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
    if ( bitmap && *bitmap) { *bitmap = RealToHooked_D2D( *bitmap ); }
    if (_CreateBitmapFromWicBitmap_post_ptr._value) { (this->*_CreateBitmapFromWicBitmap_post_ptr._value)(ret, wicBitmapSource, bitmapProperties, bitmap); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateColorContext(
    D2D1_COLOR_SPACE space,
    _In_reads_opt_(profileSize) CONST BYTE * profile,
    UINT32 profileSize,
    _Outptr_ ID2D1ColorContext ** colorContext)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateColorContext");
    if (_CreateColorContext_pre_ptr._value) { (this->*_CreateColorContext_pre_ptr._value)(space, profile, profileSize, colorContext); }
    HRESULT ret = GetRealObj()->CreateColorContext(space, profile, profileSize, colorContext);
    if ( colorContext && *colorContext) { *colorContext = RealToHooked_D2D( *colorContext ); }
    if (_CreateColorContext_post_ptr._value) { (this->*_CreateColorContext_post_ptr._value)(ret, space, profile, profileSize, colorContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateColorContextFromFilename(
    _In_ PCWSTR filename,
    _Outptr_ ID2D1ColorContext ** colorContext)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateColorContextFromFilename");
    if (_CreateColorContextFromFilename_pre_ptr._value) { (this->*_CreateColorContextFromFilename_pre_ptr._value)(filename, colorContext); }
    HRESULT ret = GetRealObj()->CreateColorContextFromFilename(filename, colorContext);
    if ( colorContext && *colorContext) { *colorContext = RealToHooked_D2D( *colorContext ); }
    if (_CreateColorContextFromFilename_post_ptr._value) { (this->*_CreateColorContextFromFilename_post_ptr._value)(ret, filename, colorContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateColorContextFromWicColorContext(
    _In_ IWICColorContext * wicColorContext,
    _Outptr_ ID2D1ColorContext ** colorContext)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateColorContextFromWicColorContext");
    if (_CreateColorContextFromWicColorContext_pre_ptr._value) { (this->*_CreateColorContextFromWicColorContext_pre_ptr._value)(wicColorContext, colorContext); }
    HRESULT ret = GetRealObj()->CreateColorContextFromWicColorContext(wicColorContext, colorContext);
    if ( colorContext && *colorContext) { *colorContext = RealToHooked_D2D( *colorContext ); }
    if (_CreateColorContextFromWicColorContext_post_ptr._value) { (this->*_CreateColorContextFromWicColorContext_post_ptr._value)(ret, wicColorContext, colorContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateBitmapFromDxgiSurface(
    _In_ IDXGISurface * surface,
    _In_opt_ CONST D2D1_BITMAP_PROPERTIES1 * bitmapProperties,
    _Outptr_ ID2D1Bitmap1 ** bitmap)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateBitmapFromDxgiSurface");
    if (_CreateBitmapFromDxgiSurface_pre_ptr._value) { (this->*_CreateBitmapFromDxgiSurface_pre_ptr._value)(surface, bitmapProperties, bitmap); }
    HRESULT ret = GetRealObj()->CreateBitmapFromDxgiSurface(HookedToReal(surface), bitmapProperties, bitmap);
    if ( bitmap && *bitmap) { *bitmap = RealToHooked_D2D( *bitmap ); }
    if (_CreateBitmapFromDxgiSurface_post_ptr._value) { (this->*_CreateBitmapFromDxgiSurface_post_ptr._value)(ret, surface, bitmapProperties, bitmap); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateEffect(
    _In_ REFCLSID effectId,
    _Outptr_ ID2D1Effect ** effect)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateEffect");
    if (_CreateEffect_pre_ptr._value) { (this->*_CreateEffect_pre_ptr._value)(effectId, effect); }
    HRESULT ret = GetRealObj()->CreateEffect(effectId, effect);
    if ( effect && *effect) { *effect = RealToHooked_D2D( *effect ); }
    if (_CreateEffect_post_ptr._value) { (this->*_CreateEffect_post_ptr._value)(ret, effectId, effect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateGradientStopCollection(
    _In_reads_(straightAlphaGradientStopsCount) CONST D2D1_GRADIENT_STOP * straightAlphaGradientStops,
    _In_range_(>=,1) UINT32 straightAlphaGradientStopsCount,
    D2D1_COLOR_SPACE preInterpolationSpace,
    D2D1_COLOR_SPACE postInterpolationSpace,
    D2D1_BUFFER_PRECISION bufferPrecision,
    D2D1_EXTEND_MODE extendMode,
    D2D1_COLOR_INTERPOLATION_MODE colorInterpolationMode,
    _Outptr_ ID2D1GradientStopCollection1 ** gradientStopCollection1)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateGradientStopCollection");
    if (_CreateGradientStopCollection_pre_ptr._value) { (this->*_CreateGradientStopCollection_pre_ptr._value)(straightAlphaGradientStops, straightAlphaGradientStopsCount, preInterpolationSpace, postInterpolationSpace, bufferPrecision, extendMode, colorInterpolationMode, gradientStopCollection1); }
    HRESULT ret = GetRealObj()->CreateGradientStopCollection(straightAlphaGradientStops, straightAlphaGradientStopsCount, preInterpolationSpace, postInterpolationSpace, bufferPrecision, extendMode, colorInterpolationMode, gradientStopCollection1);
    if ( gradientStopCollection1 && *gradientStopCollection1) { *gradientStopCollection1 = RealToHooked_D2D( *gradientStopCollection1 ); }
    if (_CreateGradientStopCollection_post_ptr._value) { (this->*_CreateGradientStopCollection_post_ptr._value)(ret, straightAlphaGradientStops, straightAlphaGradientStopsCount, preInterpolationSpace, postInterpolationSpace, bufferPrecision, extendMode, colorInterpolationMode, gradientStopCollection1); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateImageBrush(
    _In_opt_ ID2D1Image * image,
    _In_ CONST D2D1_IMAGE_BRUSH_PROPERTIES * imageBrushProperties,
    _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties,
    _Outptr_ ID2D1ImageBrush ** imageBrush)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateImageBrush");
    if (_CreateImageBrush_pre_ptr._value) { (this->*_CreateImageBrush_pre_ptr._value)(image, imageBrushProperties, brushProperties, imageBrush); }
    HRESULT ret = GetRealObj()->CreateImageBrush(HookedToReal(image), imageBrushProperties, brushProperties, imageBrush);
    if ( imageBrush && *imageBrush) { *imageBrush = RealToHooked_D2D( *imageBrush ); }
    if (_CreateImageBrush_post_ptr._value) { (this->*_CreateImageBrush_post_ptr._value)(ret, image, imageBrushProperties, brushProperties, imageBrush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateBitmapBrush(
    _In_opt_ ID2D1Bitmap * bitmap,
    _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES1 * bitmapBrushProperties,
    _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties,
    _Outptr_ ID2D1BitmapBrush1 ** bitmapBrush)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateBitmapBrush");
    if (_CreateBitmapBrush_pre_ptr._value) { (this->*_CreateBitmapBrush_pre_ptr._value)(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush); }
    HRESULT ret = GetRealObj()->CreateBitmapBrush(HookedToReal(bitmap), bitmapBrushProperties, brushProperties, bitmapBrush);
    if ( bitmapBrush && *bitmapBrush) { *bitmapBrush = RealToHooked_D2D( *bitmapBrush ); }
    if (_CreateBitmapBrush_post_ptr._value) { (this->*_CreateBitmapBrush_post_ptr._value)(ret, bitmap, bitmapBrushProperties, brushProperties, bitmapBrush); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::CreateCommandList(
    _Outptr_ ID2D1CommandList ** commandList)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::CreateCommandList");
    if (_CreateCommandList_pre_ptr._value) { (this->*_CreateCommandList_pre_ptr._value)(commandList); }
    HRESULT ret = GetRealObj()->CreateCommandList(commandList);
    if ( commandList && *commandList) { *commandList = RealToHooked_D2D( *commandList ); }
    if (_CreateCommandList_post_ptr._value) { (this->*_CreateCommandList_post_ptr._value)(ret, commandList); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D2D1DeviceContextHook::IsDxgiFormatSupported(
    DXGI_FORMAT format) const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::IsDxgiFormatSupported");
    if (_IsDxgiFormatSupported_pre_ptr._value) { (this->*_IsDxgiFormatSupported_pre_ptr._value)(format); }
    BOOL ret = GetRealObj()->IsDxgiFormatSupported(format);
    if (_IsDxgiFormatSupported_post_ptr._value) { (this->*_IsDxgiFormatSupported_post_ptr._value)(ret, format); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D2D1DeviceContextHook::IsBufferPrecisionSupported(
    D2D1_BUFFER_PRECISION bufferPrecision) const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::IsBufferPrecisionSupported");
    if (_IsBufferPrecisionSupported_pre_ptr._value) { (this->*_IsBufferPrecisionSupported_pre_ptr._value)(bufferPrecision); }
    BOOL ret = GetRealObj()->IsBufferPrecisionSupported(bufferPrecision);
    if (_IsBufferPrecisionSupported_post_ptr._value) { (this->*_IsBufferPrecisionSupported_post_ptr._value)(ret, bufferPrecision); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::GetImageLocalBounds(
    _In_ ID2D1Image * image,
    _Out_ D2D1_RECT_F * localBounds) const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetImageLocalBounds");
    if (_GetImageLocalBounds_pre_ptr._value) { (this->*_GetImageLocalBounds_pre_ptr._value)(image, localBounds); }
    HRESULT ret = GetRealObj()->GetImageLocalBounds(HookedToReal(image), localBounds);
    if (_GetImageLocalBounds_post_ptr._value) { (this->*_GetImageLocalBounds_post_ptr._value)(ret, image, localBounds); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::GetImageWorldBounds(
    _In_ ID2D1Image * image,
    _Out_ D2D1_RECT_F * worldBounds) const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetImageWorldBounds");
    if (_GetImageWorldBounds_pre_ptr._value) { (this->*_GetImageWorldBounds_pre_ptr._value)(image, worldBounds); }
    HRESULT ret = GetRealObj()->GetImageWorldBounds(HookedToReal(image), worldBounds);
    if (_GetImageWorldBounds_post_ptr._value) { (this->*_GetImageWorldBounds_post_ptr._value)(ret, image, worldBounds); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::GetGlyphRunWorldBounds(
    D2D1_POINT_2F baselineOrigin,
    _In_ CONST DWRITE_GLYPH_RUN * glyphRun,
    DWRITE_MEASURING_MODE measuringMode,
    _Out_ D2D1_RECT_F * bounds) const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetGlyphRunWorldBounds");
    if (_GetGlyphRunWorldBounds_pre_ptr._value) { (this->*_GetGlyphRunWorldBounds_pre_ptr._value)(baselineOrigin, glyphRun, measuringMode, bounds); }
    HRESULT ret = GetRealObj()->GetGlyphRunWorldBounds(baselineOrigin, glyphRun, measuringMode, bounds);
    if (_GetGlyphRunWorldBounds_post_ptr._value) { (this->*_GetGlyphRunWorldBounds_post_ptr._value)(ret, baselineOrigin, glyphRun, measuringMode, bounds); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::GetDevice(
    _Outptr_ ID2D1Device ** device) const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(device); }
    GetRealObj()->GetDevice(device);
    if ( device && *device) { *device = RealToHooked_D2D( *device ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(device); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::SetTarget(
    _In_opt_ ID2D1Image * image)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::SetTarget");
    if (_SetTarget_pre_ptr._value) { (this->*_SetTarget_pre_ptr._value)(image); }
    GetRealObj()->SetTarget(HookedToReal(image));
    if (_SetTarget_post_ptr._value) { (this->*_SetTarget_post_ptr._value)(image); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::GetTarget(
    _Outptr_result_maybenull_ ID2D1Image ** image) const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetTarget");
    if (_GetTarget_pre_ptr._value) { (this->*_GetTarget_pre_ptr._value)(image); }
    GetRealObj()->GetTarget(image);
    if ( image && *image) { *image = RealToHooked_D2D( *image ); }
    if (_GetTarget_post_ptr._value) { (this->*_GetTarget_post_ptr._value)(image); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::SetRenderingControls(
    _In_ CONST D2D1_RENDERING_CONTROLS * renderingControls)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::SetRenderingControls");
    if (_SetRenderingControls_pre_ptr._value) { (this->*_SetRenderingControls_pre_ptr._value)(renderingControls); }
    GetRealObj()->SetRenderingControls(renderingControls);
    if (_SetRenderingControls_post_ptr._value) { (this->*_SetRenderingControls_post_ptr._value)(renderingControls); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::GetRenderingControls(
    _Out_ D2D1_RENDERING_CONTROLS * renderingControls) const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetRenderingControls");
    if (_GetRenderingControls_pre_ptr._value) { (this->*_GetRenderingControls_pre_ptr._value)(renderingControls); }
    GetRealObj()->GetRenderingControls(renderingControls);
    if (_GetRenderingControls_post_ptr._value) { (this->*_GetRenderingControls_post_ptr._value)(renderingControls); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::SetPrimitiveBlend(
    D2D1_PRIMITIVE_BLEND primitiveBlend)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::SetPrimitiveBlend");
    if (_SetPrimitiveBlend_pre_ptr._value) { (this->*_SetPrimitiveBlend_pre_ptr._value)(primitiveBlend); }
    GetRealObj()->SetPrimitiveBlend(primitiveBlend);
    if (_SetPrimitiveBlend_post_ptr._value) { (this->*_SetPrimitiveBlend_post_ptr._value)(primitiveBlend); }
}

// -----------------------------------------------------------------------------
D2D1_PRIMITIVE_BLEND STDMETHODCALLTYPE D2D1DeviceContextHook::GetPrimitiveBlend() const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetPrimitiveBlend");
    if (_GetPrimitiveBlend_pre_ptr._value) { (this->*_GetPrimitiveBlend_pre_ptr._value)(); }
    D2D1_PRIMITIVE_BLEND ret = GetRealObj()->GetPrimitiveBlend();
    if (_GetPrimitiveBlend_post_ptr._value) { (this->*_GetPrimitiveBlend_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::SetUnitMode(
    D2D1_UNIT_MODE unitMode)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::SetUnitMode");
    if (_SetUnitMode_pre_ptr._value) { (this->*_SetUnitMode_pre_ptr._value)(unitMode); }
    GetRealObj()->SetUnitMode(unitMode);
    if (_SetUnitMode_post_ptr._value) { (this->*_SetUnitMode_post_ptr._value)(unitMode); }
}

// -----------------------------------------------------------------------------
D2D1_UNIT_MODE STDMETHODCALLTYPE D2D1DeviceContextHook::GetUnitMode() const
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetUnitMode");
    if (_GetUnitMode_pre_ptr._value) { (this->*_GetUnitMode_pre_ptr._value)(); }
    D2D1_UNIT_MODE ret = GetRealObj()->GetUnitMode();
    if (_GetUnitMode_post_ptr._value) { (this->*_GetUnitMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::DrawGlyphRun(
    D2D1_POINT_2F baselineOrigin,
    _In_ CONST DWRITE_GLYPH_RUN * glyphRun,
    _In_opt_ CONST DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription,
    _In_ ID2D1Brush * foregroundBrush,
    DWRITE_MEASURING_MODE measuringMode)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::DrawGlyphRun");
    if (_DrawGlyphRun_pre_ptr._value) { (this->*_DrawGlyphRun_pre_ptr._value)(baselineOrigin, glyphRun, glyphRunDescription, foregroundBrush, measuringMode); }
    GetRealObj()->DrawGlyphRun(baselineOrigin, glyphRun, glyphRunDescription, HookedToReal(foregroundBrush), measuringMode);
    if (_DrawGlyphRun_post_ptr._value) { (this->*_DrawGlyphRun_post_ptr._value)(baselineOrigin, glyphRun, glyphRunDescription, foregroundBrush, measuringMode); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::DrawImage(
    _In_ ID2D1Image * image,
    _In_opt_ CONST D2D1_POINT_2F * targetOffset,
    _In_opt_ CONST D2D1_RECT_F * imageRectangle,
    D2D1_INTERPOLATION_MODE interpolationMode,
    D2D1_COMPOSITE_MODE compositeMode)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::DrawImage");
    if (_DrawImage_pre_ptr._value) { (this->*_DrawImage_pre_ptr._value)(image, targetOffset, imageRectangle, interpolationMode, compositeMode); }
    GetRealObj()->DrawImage(HookedToReal(image), targetOffset, imageRectangle, interpolationMode, compositeMode);
    if (_DrawImage_post_ptr._value) { (this->*_DrawImage_post_ptr._value)(image, targetOffset, imageRectangle, interpolationMode, compositeMode); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::DrawGdiMetafile(
    _In_ ID2D1GdiMetafile * gdiMetafile,
    _In_opt_ CONST D2D1_POINT_2F * targetOffset)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::DrawGdiMetafile");
    if (_DrawGdiMetafile_pre_ptr._value) { (this->*_DrawGdiMetafile_pre_ptr._value)(gdiMetafile, targetOffset); }
    GetRealObj()->DrawGdiMetafile(HookedToReal(gdiMetafile), targetOffset);
    if (_DrawGdiMetafile_post_ptr._value) { (this->*_DrawGdiMetafile_post_ptr._value)(gdiMetafile, targetOffset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::DrawBitmap(
    _In_ ID2D1Bitmap * bitmap,
    _In_opt_ CONST D2D1_RECT_F * destinationRectangle,
    FLOAT opacity,
    D2D1_INTERPOLATION_MODE interpolationMode,
    _In_opt_ CONST D2D1_RECT_F * sourceRectangle,
    _In_opt_ CONST D2D1_MATRIX_4X4_F * perspectiveTransform)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::DrawBitmap");
    if (_DrawBitmap_pre_ptr._value) { (this->*_DrawBitmap_pre_ptr._value)(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle, perspectiveTransform); }
    GetRealObj()->DrawBitmap(HookedToReal(bitmap), destinationRectangle, opacity, interpolationMode, sourceRectangle, perspectiveTransform);
    if (_DrawBitmap_post_ptr._value) { (this->*_DrawBitmap_post_ptr._value)(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle, perspectiveTransform); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::PushLayer(
    _In_ CONST D2D1_LAYER_PARAMETERS1 * layerParameters,
    _In_opt_ ID2D1Layer * layer)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::PushLayer");
    if (_PushLayer_pre_ptr._value) { (this->*_PushLayer_pre_ptr._value)(layerParameters, layer); }
    GetRealObj()->PushLayer(layerParameters, HookedToReal(layer));
    if (_PushLayer_post_ptr._value) { (this->*_PushLayer_post_ptr._value)(layerParameters, layer); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::InvalidateEffectInputRectangle(
    _In_ ID2D1Effect * effect,
    UINT32 input,
    _In_ CONST D2D1_RECT_F * inputRectangle)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::InvalidateEffectInputRectangle");
    if (_InvalidateEffectInputRectangle_pre_ptr._value) { (this->*_InvalidateEffectInputRectangle_pre_ptr._value)(effect, input, inputRectangle); }
    HRESULT ret = GetRealObj()->InvalidateEffectInputRectangle(HookedToReal(effect), input, inputRectangle);
    if (_InvalidateEffectInputRectangle_post_ptr._value) { (this->*_InvalidateEffectInputRectangle_post_ptr._value)(ret, effect, input, inputRectangle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::GetEffectInvalidRectangleCount(
    _In_ ID2D1Effect * effect,
    _Out_ UINT32 * rectangleCount)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetEffectInvalidRectangleCount");
    if (_GetEffectInvalidRectangleCount_pre_ptr._value) { (this->*_GetEffectInvalidRectangleCount_pre_ptr._value)(effect, rectangleCount); }
    HRESULT ret = GetRealObj()->GetEffectInvalidRectangleCount(HookedToReal(effect), rectangleCount);
    if (_GetEffectInvalidRectangleCount_post_ptr._value) { (this->*_GetEffectInvalidRectangleCount_post_ptr._value)(ret, effect, rectangleCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::GetEffectInvalidRectangles(
    _In_ ID2D1Effect * effect,
    _Out_writes_(rectanglesCount) D2D1_RECT_F * rectangles,
    UINT32 rectanglesCount)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetEffectInvalidRectangles");
    if (_GetEffectInvalidRectangles_pre_ptr._value) { (this->*_GetEffectInvalidRectangles_pre_ptr._value)(effect, rectangles, rectanglesCount); }
    HRESULT ret = GetRealObj()->GetEffectInvalidRectangles(HookedToReal(effect), rectangles, rectanglesCount);
    if (_GetEffectInvalidRectangles_post_ptr._value) { (this->*_GetEffectInvalidRectangles_post_ptr._value)(ret, effect, rectangles, rectanglesCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceContextHook::GetEffectRequiredInputRectangles(
    _In_ ID2D1Effect * renderEffect,
    _In_opt_ CONST D2D1_RECT_F * renderImageRectangle,
    _In_reads_(inputCount) CONST D2D1_EFFECT_INPUT_DESCRIPTION * inputDescriptions,
    _Out_writes_(inputCount) D2D1_RECT_F * requiredInputRects,
    UINT32 inputCount)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::GetEffectRequiredInputRectangles");
    if (_GetEffectRequiredInputRectangles_pre_ptr._value) { (this->*_GetEffectRequiredInputRectangles_pre_ptr._value)(renderEffect, renderImageRectangle, inputDescriptions, requiredInputRects, inputCount); }
    HRESULT ret = GetRealObj()->GetEffectRequiredInputRectangles(HookedToReal(renderEffect), renderImageRectangle, inputDescriptions, requiredInputRects, inputCount);
    if (_GetEffectRequiredInputRectangles_post_ptr._value) { (this->*_GetEffectRequiredInputRectangles_post_ptr._value)(ret, renderEffect, renderImageRectangle, inputDescriptions, requiredInputRects, inputCount); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceContextHook::FillOpacityMask(
    _In_ ID2D1Bitmap * opacityMask,
    _In_ ID2D1Brush * brush,
    _In_opt_ CONST D2D1_RECT_F * destinationRectangle,
    _In_opt_ CONST D2D1_RECT_F * sourceRectangle)
{
    calltrace::AutoTrace trace(L"D2D1DeviceContextHook::FillOpacityMask");
    if (_FillOpacityMask_pre_ptr._value) { (this->*_FillOpacityMask_pre_ptr._value)(opacityMask, brush, destinationRectangle, sourceRectangle); }
    GetRealObj()->FillOpacityMask(HookedToReal(opacityMask), HookedToReal(brush), destinationRectangle, sourceRectangle);
    if (_FillOpacityMask_post_ptr._value) { (this->*_FillOpacityMask_post_ptr._value)(opacityMask, brush, destinationRectangle, sourceRectangle); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceHook::CreateDeviceContext(
    D2D1_DEVICE_CONTEXT_OPTIONS options,
    _Outptr_ ID2D1DeviceContext ** deviceContext)
{
    calltrace::AutoTrace trace(L"D2D1DeviceHook::CreateDeviceContext");
    if (_CreateDeviceContext_pre_ptr._value) { (this->*_CreateDeviceContext_pre_ptr._value)(options, deviceContext); }
    HRESULT ret = GetRealObj()->CreateDeviceContext(options, deviceContext);
    if ( deviceContext && *deviceContext) { *deviceContext = RealToHooked_D2D( *deviceContext ); }
    if (_CreateDeviceContext_post_ptr._value) { (this->*_CreateDeviceContext_post_ptr._value)(ret, options, deviceContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1DeviceHook::CreatePrintControl(
    _In_ IWICImagingFactory * wicFactory,
    _In_ IPrintDocumentPackageTarget * documentTarget,
    _In_opt_ CONST D2D1_PRINT_CONTROL_PROPERTIES * printControlProperties,
    _Outptr_ ID2D1PrintControl ** printControl)
{
    calltrace::AutoTrace trace(L"D2D1DeviceHook::CreatePrintControl");
    if (_CreatePrintControl_pre_ptr._value) { (this->*_CreatePrintControl_pre_ptr._value)(wicFactory, documentTarget, printControlProperties, printControl); }
    HRESULT ret = GetRealObj()->CreatePrintControl(wicFactory, documentTarget, printControlProperties, printControl);
    if ( printControl && *printControl) { *printControl = RealToHooked_D2D( *printControl ); }
    if (_CreatePrintControl_post_ptr._value) { (this->*_CreatePrintControl_post_ptr._value)(ret, wicFactory, documentTarget, printControlProperties, printControl); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceHook::SetMaximumTextureMemory(
    UINT64 maximumInBytes)
{
    calltrace::AutoTrace trace(L"D2D1DeviceHook::SetMaximumTextureMemory");
    if (_SetMaximumTextureMemory_pre_ptr._value) { (this->*_SetMaximumTextureMemory_pre_ptr._value)(maximumInBytes); }
    GetRealObj()->SetMaximumTextureMemory(maximumInBytes);
    if (_SetMaximumTextureMemory_post_ptr._value) { (this->*_SetMaximumTextureMemory_post_ptr._value)(maximumInBytes); }
}

// -----------------------------------------------------------------------------
UINT64 STDMETHODCALLTYPE D2D1DeviceHook::GetMaximumTextureMemory() const
{
    calltrace::AutoTrace trace(L"D2D1DeviceHook::GetMaximumTextureMemory");
    if (_GetMaximumTextureMemory_pre_ptr._value) { (this->*_GetMaximumTextureMemory_pre_ptr._value)(); }
    UINT64 ret = GetRealObj()->GetMaximumTextureMemory();
    if (_GetMaximumTextureMemory_post_ptr._value) { (this->*_GetMaximumTextureMemory_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1DeviceHook::ClearResources(
    UINT32 millisecondsSinceUse)
{
    calltrace::AutoTrace trace(L"D2D1DeviceHook::ClearResources");
    if (_ClearResources_pre_ptr._value) { (this->*_ClearResources_pre_ptr._value)(millisecondsSinceUse); }
    GetRealObj()->ClearResources(millisecondsSinceUse);
    if (_ClearResources_post_ptr._value) { (this->*_ClearResources_post_ptr._value)(millisecondsSinceUse); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::CreateDevice(
    _In_ IDXGIDevice * dxgiDevice,
    _Outptr_ ID2D1Device ** d2dDevice)
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::CreateDevice");
    if (_CreateDevice_pre_ptr._value) { (this->*_CreateDevice_pre_ptr._value)(dxgiDevice, d2dDevice); }
    HRESULT ret = GetRealObj()->CreateDevice(HookedToReal(dxgiDevice), d2dDevice);
    if ( d2dDevice && *d2dDevice) { *d2dDevice = RealToHooked_D2D( *d2dDevice ); }
    if (_CreateDevice_post_ptr._value) { (this->*_CreateDevice_post_ptr._value)(ret, dxgiDevice, d2dDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::CreateStrokeStyle(
    _In_ CONST D2D1_STROKE_STYLE_PROPERTIES1 * strokeStyleProperties,
    _In_reads_opt_(dashesCount) CONST FLOAT * dashes,
    UINT32 dashesCount,
    _Outptr_ ID2D1StrokeStyle1 ** strokeStyle)
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::CreateStrokeStyle");
    if (_CreateStrokeStyle_pre_ptr._value) { (this->*_CreateStrokeStyle_pre_ptr._value)(strokeStyleProperties, dashes, dashesCount, strokeStyle); }
    HRESULT ret = GetRealObj()->CreateStrokeStyle(strokeStyleProperties, dashes, dashesCount, strokeStyle);
    if ( strokeStyle && *strokeStyle) { *strokeStyle = RealToHooked_D2D( *strokeStyle ); }
    if (_CreateStrokeStyle_post_ptr._value) { (this->*_CreateStrokeStyle_post_ptr._value)(ret, strokeStyleProperties, dashes, dashesCount, strokeStyle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::CreatePathGeometry(
    _Outptr_ ID2D1PathGeometry1 ** pathGeometry)
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::CreatePathGeometry");
    if (_CreatePathGeometry_pre_ptr._value) { (this->*_CreatePathGeometry_pre_ptr._value)(pathGeometry); }
    HRESULT ret = GetRealObj()->CreatePathGeometry(pathGeometry);
    if ( pathGeometry && *pathGeometry) { *pathGeometry = RealToHooked_D2D( *pathGeometry ); }
    if (_CreatePathGeometry_post_ptr._value) { (this->*_CreatePathGeometry_post_ptr._value)(ret, pathGeometry); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::CreateDrawingStateBlock(
    _In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION1 * drawingStateDescription,
    _In_opt_ IDWriteRenderingParams * textRenderingParams,
    _Outptr_ ID2D1DrawingStateBlock1 ** drawingStateBlock)
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::CreateDrawingStateBlock");
    if (_CreateDrawingStateBlock_pre_ptr._value) { (this->*_CreateDrawingStateBlock_pre_ptr._value)(drawingStateDescription, textRenderingParams, drawingStateBlock); }
    HRESULT ret = GetRealObj()->CreateDrawingStateBlock(drawingStateDescription, HookedToReal(textRenderingParams), drawingStateBlock);
    if ( drawingStateBlock && *drawingStateBlock) { *drawingStateBlock = RealToHooked_D2D( *drawingStateBlock ); }
    if (_CreateDrawingStateBlock_post_ptr._value) { (this->*_CreateDrawingStateBlock_post_ptr._value)(ret, drawingStateDescription, textRenderingParams, drawingStateBlock); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::CreateGdiMetafile(
    _In_ IStream * metafileStream,
    _Outptr_ ID2D1GdiMetafile ** metafile)
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::CreateGdiMetafile");
    if (_CreateGdiMetafile_pre_ptr._value) { (this->*_CreateGdiMetafile_pre_ptr._value)(metafileStream, metafile); }
    HRESULT ret = GetRealObj()->CreateGdiMetafile(metafileStream, metafile);
    if ( metafile && *metafile) { *metafile = RealToHooked_D2D( *metafile ); }
    if (_CreateGdiMetafile_post_ptr._value) { (this->*_CreateGdiMetafile_post_ptr._value)(ret, metafileStream, metafile); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::RegisterEffectFromStream(
    _In_ REFCLSID classId,
    _In_ IStream * propertyXml,
    _In_reads_opt_(bindingsCount) CONST D2D1_PROPERTY_BINDING * bindings,
    UINT32 bindingsCount,
    _In_ CONST PD2D1_EFFECT_FACTORY effectFactory)
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::RegisterEffectFromStream");
    if (_RegisterEffectFromStream_pre_ptr._value) { (this->*_RegisterEffectFromStream_pre_ptr._value)(classId, propertyXml, bindings, bindingsCount, effectFactory); }
    HRESULT ret = GetRealObj()->RegisterEffectFromStream(classId, propertyXml, bindings, bindingsCount, effectFactory);
    if (_RegisterEffectFromStream_post_ptr._value) { (this->*_RegisterEffectFromStream_post_ptr._value)(ret, classId, propertyXml, bindings, bindingsCount, effectFactory); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::RegisterEffectFromString(
    _In_ REFCLSID classId,
    _In_ PCWSTR propertyXml,
    _In_reads_opt_(bindingsCount) CONST D2D1_PROPERTY_BINDING * bindings,
    UINT32 bindingsCount,
    _In_ CONST PD2D1_EFFECT_FACTORY effectFactory)
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::RegisterEffectFromString");
    if (_RegisterEffectFromString_pre_ptr._value) { (this->*_RegisterEffectFromString_pre_ptr._value)(classId, propertyXml, bindings, bindingsCount, effectFactory); }
    HRESULT ret = GetRealObj()->RegisterEffectFromString(classId, propertyXml, bindings, bindingsCount, effectFactory);
    if (_RegisterEffectFromString_post_ptr._value) { (this->*_RegisterEffectFromString_post_ptr._value)(ret, classId, propertyXml, bindings, bindingsCount, effectFactory); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::UnregisterEffect(
    _In_ REFCLSID classId)
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::UnregisterEffect");
    if (_UnregisterEffect_pre_ptr._value) { (this->*_UnregisterEffect_pre_ptr._value)(classId); }
    HRESULT ret = GetRealObj()->UnregisterEffect(classId);
    if (_UnregisterEffect_post_ptr._value) { (this->*_UnregisterEffect_post_ptr._value)(ret, classId); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::GetRegisteredEffects(
    _Out_writes_to_opt_(effectsCount, *effectsReturned) CLSID * effects,
    UINT32 effectsCount,
    _Out_opt_ UINT32 * effectsReturned,
    _Out_opt_ UINT32 * effectsRegistered) const
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::GetRegisteredEffects");
    if (_GetRegisteredEffects_pre_ptr._value) { (this->*_GetRegisteredEffects_pre_ptr._value)(effects, effectsCount, effectsReturned, effectsRegistered); }
    HRESULT ret = GetRealObj()->GetRegisteredEffects(effects, effectsCount, effectsReturned, effectsRegistered);
    if (_GetRegisteredEffects_post_ptr._value) { (this->*_GetRegisteredEffects_post_ptr._value)(ret, effects, effectsCount, effectsReturned, effectsRegistered); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D2D1Factory1Hook::GetEffectProperties(
    _In_ REFCLSID effectId,
    _Outptr_ ID2D1Properties ** properties) const
{
    calltrace::AutoTrace trace(L"D2D1Factory1Hook::GetEffectProperties");
    if (_GetEffectProperties_pre_ptr._value) { (this->*_GetEffectProperties_pre_ptr._value)(effectId, properties); }
    HRESULT ret = GetRealObj()->GetEffectProperties(effectId, properties);
    if ( properties && *properties) { *properties = RealToHooked_D2D( *properties ); }
    if (_GetEffectProperties_post_ptr._value) { (this->*_GetEffectProperties_post_ptr._value)(ret, effectId, properties); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D2D1MultithreadHook::GetMultithreadProtected() const
{
    calltrace::AutoTrace trace(L"D2D1MultithreadHook::GetMultithreadProtected");
    if (_GetMultithreadProtected_pre_ptr._value) { (this->*_GetMultithreadProtected_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->GetMultithreadProtected();
    if (_GetMultithreadProtected_post_ptr._value) { (this->*_GetMultithreadProtected_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1MultithreadHook::Enter()
{
    calltrace::AutoTrace trace(L"D2D1MultithreadHook::Enter");
    if (_Enter_pre_ptr._value) { (this->*_Enter_pre_ptr._value)(); }
    GetRealObj()->Enter();
    if (_Enter_post_ptr._value) { (this->*_Enter_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D2D1MultithreadHook::Leave()
{
    calltrace::AutoTrace trace(L"D2D1MultithreadHook::Leave");
    if (_Leave_pre_ptr._value) { (this->*_Leave_pre_ptr._value)(); }
    GetRealObj()->Leave();
    if (_Leave_post_ptr._value) { (this->*_Leave_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileLoaderHook::CreateStreamFromKey(
    _In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey,
    UINT32 fontFileReferenceKeySize,
    _Out_ IDWriteFontFileStream** fontFileStream)
{
    calltrace::AutoTrace trace(L"DWriteFontFileLoaderHook::CreateStreamFromKey");
    if (_CreateStreamFromKey_pre_ptr._value) { (this->*_CreateStreamFromKey_pre_ptr._value)(fontFileReferenceKey, fontFileReferenceKeySize, fontFileStream); }
    HRESULT ret = GetRealObj()->CreateStreamFromKey(fontFileReferenceKey, fontFileReferenceKeySize, fontFileStream);
    if ( fontFileStream && *fontFileStream) { *fontFileStream = RealToHooked_D2D( *fontFileStream ); }
    if (_CreateStreamFromKey_post_ptr._value) { (this->*_CreateStreamFromKey_post_ptr._value)(ret, fontFileReferenceKey, fontFileReferenceKeySize, fontFileStream); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteLocalFontFileLoaderHook::GetFilePathLengthFromKey(
    _In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey,
    UINT32 fontFileReferenceKeySize,
    _Out_ UINT32* filePathLength)
{
    calltrace::AutoTrace trace(L"DWriteLocalFontFileLoaderHook::GetFilePathLengthFromKey");
    if (_GetFilePathLengthFromKey_pre_ptr._value) { (this->*_GetFilePathLengthFromKey_pre_ptr._value)(fontFileReferenceKey, fontFileReferenceKeySize, filePathLength); }
    HRESULT ret = GetRealObj()->GetFilePathLengthFromKey(fontFileReferenceKey, fontFileReferenceKeySize, filePathLength);
    if (_GetFilePathLengthFromKey_post_ptr._value) { (this->*_GetFilePathLengthFromKey_post_ptr._value)(ret, fontFileReferenceKey, fontFileReferenceKeySize, filePathLength); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteLocalFontFileLoaderHook::GetFilePathFromKey(
    _In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey,
    UINT32 fontFileReferenceKeySize,
    _Out_writes_z_(filePathSize) WCHAR* filePath,
    UINT32 filePathSize)
{
    calltrace::AutoTrace trace(L"DWriteLocalFontFileLoaderHook::GetFilePathFromKey");
    if (_GetFilePathFromKey_pre_ptr._value) { (this->*_GetFilePathFromKey_pre_ptr._value)(fontFileReferenceKey, fontFileReferenceKeySize, filePath, filePathSize); }
    HRESULT ret = GetRealObj()->GetFilePathFromKey(fontFileReferenceKey, fontFileReferenceKeySize, filePath, filePathSize);
    if (_GetFilePathFromKey_post_ptr._value) { (this->*_GetFilePathFromKey_post_ptr._value)(ret, fontFileReferenceKey, fontFileReferenceKeySize, filePath, filePathSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteLocalFontFileLoaderHook::GetLastWriteTimeFromKey(
    _In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey,
    UINT32 fontFileReferenceKeySize,
    _Out_ FILETIME* lastWriteTime)
{
    calltrace::AutoTrace trace(L"DWriteLocalFontFileLoaderHook::GetLastWriteTimeFromKey");
    if (_GetLastWriteTimeFromKey_pre_ptr._value) { (this->*_GetLastWriteTimeFromKey_pre_ptr._value)(fontFileReferenceKey, fontFileReferenceKeySize, lastWriteTime); }
    HRESULT ret = GetRealObj()->GetLastWriteTimeFromKey(fontFileReferenceKey, fontFileReferenceKeySize, lastWriteTime);
    if (_GetLastWriteTimeFromKey_post_ptr._value) { (this->*_GetLastWriteTimeFromKey_post_ptr._value)(ret, fontFileReferenceKey, fontFileReferenceKeySize, lastWriteTime); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileStreamHook::ReadFileFragment(
    _Outptr_result_bytebuffer_(fragmentSize) void const** fragmentStart,
    UINT64 fileOffset,
    UINT64 fragmentSize,
    _Out_ void** fragmentContext)
{
    calltrace::AutoTrace trace(L"DWriteFontFileStreamHook::ReadFileFragment");
    if (_ReadFileFragment_pre_ptr._value) { (this->*_ReadFileFragment_pre_ptr._value)(fragmentStart, fileOffset, fragmentSize, fragmentContext); }
    HRESULT ret = GetRealObj()->ReadFileFragment(fragmentStart, fileOffset, fragmentSize, fragmentContext);
    if (_ReadFileFragment_post_ptr._value) { (this->*_ReadFileFragment_post_ptr._value)(ret, fragmentStart, fileOffset, fragmentSize, fragmentContext); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DWriteFontFileStreamHook::ReleaseFileFragment(
    void* fragmentContext)
{
    calltrace::AutoTrace trace(L"DWriteFontFileStreamHook::ReleaseFileFragment");
    if (_ReleaseFileFragment_pre_ptr._value) { (this->*_ReleaseFileFragment_pre_ptr._value)(fragmentContext); }
    GetRealObj()->ReleaseFileFragment(fragmentContext);
    if (_ReleaseFileFragment_post_ptr._value) { (this->*_ReleaseFileFragment_post_ptr._value)(fragmentContext); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileStreamHook::GetFileSize(
    _Out_ UINT64* fileSize)
{
    calltrace::AutoTrace trace(L"DWriteFontFileStreamHook::GetFileSize");
    if (_GetFileSize_pre_ptr._value) { (this->*_GetFileSize_pre_ptr._value)(fileSize); }
    HRESULT ret = GetRealObj()->GetFileSize(fileSize);
    if (_GetFileSize_post_ptr._value) { (this->*_GetFileSize_post_ptr._value)(ret, fileSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileStreamHook::GetLastWriteTime(
    _Out_ UINT64* lastWriteTime)
{
    calltrace::AutoTrace trace(L"DWriteFontFileStreamHook::GetLastWriteTime");
    if (_GetLastWriteTime_pre_ptr._value) { (this->*_GetLastWriteTime_pre_ptr._value)(lastWriteTime); }
    HRESULT ret = GetRealObj()->GetLastWriteTime(lastWriteTime);
    if (_GetLastWriteTime_post_ptr._value) { (this->*_GetLastWriteTime_post_ptr._value)(ret, lastWriteTime); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileHook::GetReferenceKey(
    _Outptr_result_bytebuffer_(*fontFileReferenceKeySize) void const** fontFileReferenceKey,
    _Out_ UINT32* fontFileReferenceKeySize)
{
    calltrace::AutoTrace trace(L"DWriteFontFileHook::GetReferenceKey");
    if (_GetReferenceKey_pre_ptr._value) { (this->*_GetReferenceKey_pre_ptr._value)(fontFileReferenceKey, fontFileReferenceKeySize); }
    HRESULT ret = GetRealObj()->GetReferenceKey(fontFileReferenceKey, fontFileReferenceKeySize);
    if (_GetReferenceKey_post_ptr._value) { (this->*_GetReferenceKey_post_ptr._value)(ret, fontFileReferenceKey, fontFileReferenceKeySize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileHook::GetLoader(
    _Out_ IDWriteFontFileLoader** fontFileLoader)
{
    calltrace::AutoTrace trace(L"DWriteFontFileHook::GetLoader");
    if (_GetLoader_pre_ptr._value) { (this->*_GetLoader_pre_ptr._value)(fontFileLoader); }
    HRESULT ret = GetRealObj()->GetLoader(fontFileLoader);
    if ( fontFileLoader && *fontFileLoader) { *fontFileLoader = RealToHooked_D2D( *fontFileLoader ); }
    if (_GetLoader_post_ptr._value) { (this->*_GetLoader_post_ptr._value)(ret, fontFileLoader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileHook::Analyze(
    _Out_ BOOL* isSupportedFontType,
    _Out_ DWRITE_FONT_FILE_TYPE* fontFileType,
    _Out_opt_ DWRITE_FONT_FACE_TYPE* fontFaceType,
    _Out_ UINT32* numberOfFaces)
{
    calltrace::AutoTrace trace(L"DWriteFontFileHook::Analyze");
    if (_Analyze_pre_ptr._value) { (this->*_Analyze_pre_ptr._value)(isSupportedFontType, fontFileType, fontFaceType, numberOfFaces); }
    HRESULT ret = GetRealObj()->Analyze(isSupportedFontType, fontFileType, fontFaceType, numberOfFaces);
    if (_Analyze_post_ptr._value) { (this->*_Analyze_post_ptr._value)(ret, isSupportedFontType, fontFileType, fontFaceType, numberOfFaces); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE DWriteRenderingParamsHook::GetGamma()
{
    calltrace::AutoTrace trace(L"DWriteRenderingParamsHook::GetGamma");
    if (_GetGamma_pre_ptr._value) { (this->*_GetGamma_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetGamma();
    if (_GetGamma_post_ptr._value) { (this->*_GetGamma_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE DWriteRenderingParamsHook::GetEnhancedContrast()
{
    calltrace::AutoTrace trace(L"DWriteRenderingParamsHook::GetEnhancedContrast");
    if (_GetEnhancedContrast_pre_ptr._value) { (this->*_GetEnhancedContrast_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetEnhancedContrast();
    if (_GetEnhancedContrast_post_ptr._value) { (this->*_GetEnhancedContrast_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE DWriteRenderingParamsHook::GetClearTypeLevel()
{
    calltrace::AutoTrace trace(L"DWriteRenderingParamsHook::GetClearTypeLevel");
    if (_GetClearTypeLevel_pre_ptr._value) { (this->*_GetClearTypeLevel_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetClearTypeLevel();
    if (_GetClearTypeLevel_post_ptr._value) { (this->*_GetClearTypeLevel_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_PIXEL_GEOMETRY STDMETHODCALLTYPE DWriteRenderingParamsHook::GetPixelGeometry()
{
    calltrace::AutoTrace trace(L"DWriteRenderingParamsHook::GetPixelGeometry");
    if (_GetPixelGeometry_pre_ptr._value) { (this->*_GetPixelGeometry_pre_ptr._value)(); }
    DWRITE_PIXEL_GEOMETRY ret = GetRealObj()->GetPixelGeometry();
    if (_GetPixelGeometry_post_ptr._value) { (this->*_GetPixelGeometry_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_RENDERING_MODE STDMETHODCALLTYPE DWriteRenderingParamsHook::GetRenderingMode()
{
    calltrace::AutoTrace trace(L"DWriteRenderingParamsHook::GetRenderingMode");
    if (_GetRenderingMode_pre_ptr._value) { (this->*_GetRenderingMode_pre_ptr._value)(); }
    DWRITE_RENDERING_MODE ret = GetRealObj()->GetRenderingMode();
    if (_GetRenderingMode_post_ptr._value) { (this->*_GetRenderingMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_FACE_TYPE STDMETHODCALLTYPE DWriteFontFaceHook::GetType()
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetType");
    if (_GetType_pre_ptr._value) { (this->*_GetType_pre_ptr._value)(); }
    DWRITE_FONT_FACE_TYPE ret = GetRealObj()->GetType();
    if (_GetType_post_ptr._value) { (this->*_GetType_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFaceHook::GetFiles(
    _Inout_ UINT32* numberOfFiles,
    _Out_writes_opt_(*numberOfFiles) IDWriteFontFile** fontFiles)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetFiles");
    if (_GetFiles_pre_ptr._value) { (this->*_GetFiles_pre_ptr._value)(numberOfFiles, fontFiles); }
    HRESULT ret = GetRealObj()->GetFiles(numberOfFiles, fontFiles);
    if ( fontFiles && *fontFiles) { *fontFiles = RealToHooked_D2D( *fontFiles ); }
    if (_GetFiles_post_ptr._value) { (this->*_GetFiles_post_ptr._value)(ret, numberOfFiles, fontFiles); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE DWriteFontFaceHook::GetIndex()
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetIndex");
    if (_GetIndex_pre_ptr._value) { (this->*_GetIndex_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetIndex();
    if (_GetIndex_post_ptr._value) { (this->*_GetIndex_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_SIMULATIONS STDMETHODCALLTYPE DWriteFontFaceHook::GetSimulations()
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetSimulations");
    if (_GetSimulations_pre_ptr._value) { (this->*_GetSimulations_pre_ptr._value)(); }
    DWRITE_FONT_SIMULATIONS ret = GetRealObj()->GetSimulations();
    if (_GetSimulations_post_ptr._value) { (this->*_GetSimulations_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DWriteFontFaceHook::IsSymbolFont()
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::IsSymbolFont");
    if (_IsSymbolFont_pre_ptr._value) { (this->*_IsSymbolFont_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsSymbolFont();
    if (_IsSymbolFont_post_ptr._value) { (this->*_IsSymbolFont_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DWriteFontFaceHook::GetMetrics(
    _Out_ DWRITE_FONT_METRICS* fontFaceMetrics)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetMetrics");
    if (_GetMetrics_pre_ptr._value) { (this->*_GetMetrics_pre_ptr._value)(fontFaceMetrics); }
    GetRealObj()->GetMetrics(fontFaceMetrics);
    if (_GetMetrics_post_ptr._value) { (this->*_GetMetrics_post_ptr._value)(fontFaceMetrics); }
}

// -----------------------------------------------------------------------------
UINT16 STDMETHODCALLTYPE DWriteFontFaceHook::GetGlyphCount()
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetGlyphCount");
    if (_GetGlyphCount_pre_ptr._value) { (this->*_GetGlyphCount_pre_ptr._value)(); }
    UINT16 ret = GetRealObj()->GetGlyphCount();
    if (_GetGlyphCount_post_ptr._value) { (this->*_GetGlyphCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFaceHook::GetDesignGlyphMetrics(
    _In_reads_(glyphCount) UINT16 const* glyphIndices,
    UINT32 glyphCount,
    _Out_writes_(glyphCount) DWRITE_GLYPH_METRICS* glyphMetrics,
    BOOL isSideways)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetDesignGlyphMetrics");
    if (_GetDesignGlyphMetrics_pre_ptr._value) { (this->*_GetDesignGlyphMetrics_pre_ptr._value)(glyphIndices, glyphCount, glyphMetrics, isSideways); }
    HRESULT ret = GetRealObj()->GetDesignGlyphMetrics(glyphIndices, glyphCount, glyphMetrics, isSideways);
    if (_GetDesignGlyphMetrics_post_ptr._value) { (this->*_GetDesignGlyphMetrics_post_ptr._value)(ret, glyphIndices, glyphCount, glyphMetrics, isSideways); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFaceHook::GetGlyphIndices(
    _In_reads_(codePointCount) UINT32 const* codePoints,
    UINT32 codePointCount,
    _Out_writes_(codePointCount) UINT16* glyphIndices)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetGlyphIndices");
    if (_GetGlyphIndices_pre_ptr._value) { (this->*_GetGlyphIndices_pre_ptr._value)(codePoints, codePointCount, glyphIndices); }
    HRESULT ret = GetRealObj()->GetGlyphIndices(codePoints, codePointCount, glyphIndices);
    if (_GetGlyphIndices_post_ptr._value) { (this->*_GetGlyphIndices_post_ptr._value)(ret, codePoints, codePointCount, glyphIndices); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFaceHook::TryGetFontTable(
    _In_ UINT32 openTypeTableTag,
    _Outptr_result_bytebuffer_(*tableSize) const void** tableData,
    _Out_ UINT32* tableSize,
    _Out_ void** tableContext,
    _Out_ BOOL* exists)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::TryGetFontTable");
    if (_TryGetFontTable_pre_ptr._value) { (this->*_TryGetFontTable_pre_ptr._value)(openTypeTableTag, tableData, tableSize, tableContext, exists); }
    HRESULT ret = GetRealObj()->TryGetFontTable(openTypeTableTag, tableData, tableSize, tableContext, exists);
    if (_TryGetFontTable_post_ptr._value) { (this->*_TryGetFontTable_post_ptr._value)(ret, openTypeTableTag, tableData, tableSize, tableContext, exists); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DWriteFontFaceHook::ReleaseFontTable(
    _In_ void* tableContext)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::ReleaseFontTable");
    if (_ReleaseFontTable_pre_ptr._value) { (this->*_ReleaseFontTable_pre_ptr._value)(tableContext); }
    GetRealObj()->ReleaseFontTable(tableContext);
    if (_ReleaseFontTable_post_ptr._value) { (this->*_ReleaseFontTable_post_ptr._value)(tableContext); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFaceHook::GetGlyphRunOutline(
    FLOAT emSize,
    _In_reads_(glyphCount) UINT16 const* glyphIndices,
    _In_reads_opt_(glyphCount) FLOAT const* glyphAdvances,
    _In_reads_opt_(glyphCount) DWRITE_GLYPH_OFFSET const* glyphOffsets,
    UINT32 glyphCount,
    BOOL isSideways,
    BOOL isRightToLeft,
    IDWriteGeometrySink* geometrySink)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetGlyphRunOutline");
    if (_GetGlyphRunOutline_pre_ptr._value) { (this->*_GetGlyphRunOutline_pre_ptr._value)(emSize, glyphIndices, glyphAdvances, glyphOffsets, glyphCount, isSideways, isRightToLeft, geometrySink); }
    HRESULT ret = GetRealObj()->GetGlyphRunOutline(emSize, glyphIndices, glyphAdvances, glyphOffsets, glyphCount, isSideways, isRightToLeft, geometrySink);
    if (_GetGlyphRunOutline_post_ptr._value) { (this->*_GetGlyphRunOutline_post_ptr._value)(ret, emSize, glyphIndices, glyphAdvances, glyphOffsets, glyphCount, isSideways, isRightToLeft, geometrySink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFaceHook::GetRecommendedRenderingMode(
    FLOAT emSize,
    FLOAT pixelsPerDip,
    DWRITE_MEASURING_MODE measuringMode,
    IDWriteRenderingParams* renderingParams,
    _Out_ DWRITE_RENDERING_MODE* renderingMode)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetRecommendedRenderingMode");
    if (_GetRecommendedRenderingMode_pre_ptr._value) { (this->*_GetRecommendedRenderingMode_pre_ptr._value)(emSize, pixelsPerDip, measuringMode, renderingParams, renderingMode); }
    HRESULT ret = GetRealObj()->GetRecommendedRenderingMode(emSize, pixelsPerDip, measuringMode, renderingParams, renderingMode);
    if (_GetRecommendedRenderingMode_post_ptr._value) { (this->*_GetRecommendedRenderingMode_post_ptr._value)(ret, emSize, pixelsPerDip, measuringMode, renderingParams, renderingMode); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFaceHook::GetGdiCompatibleMetrics(
    FLOAT emSize,
    FLOAT pixelsPerDip,
    _In_opt_ DWRITE_MATRIX const* transform,
    _Out_ DWRITE_FONT_METRICS* fontFaceMetrics)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetGdiCompatibleMetrics");
    if (_GetGdiCompatibleMetrics_pre_ptr._value) { (this->*_GetGdiCompatibleMetrics_pre_ptr._value)(emSize, pixelsPerDip, transform, fontFaceMetrics); }
    HRESULT ret = GetRealObj()->GetGdiCompatibleMetrics(emSize, pixelsPerDip, transform, fontFaceMetrics);
    if (_GetGdiCompatibleMetrics_post_ptr._value) { (this->*_GetGdiCompatibleMetrics_post_ptr._value)(ret, emSize, pixelsPerDip, transform, fontFaceMetrics); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFaceHook::GetGdiCompatibleGlyphMetrics(
    FLOAT emSize,
    FLOAT pixelsPerDip,
    _In_opt_ DWRITE_MATRIX const* transform,
    BOOL useGdiNatural,
    _In_reads_(glyphCount) UINT16 const* glyphIndices,
    UINT32 glyphCount,
    _Out_writes_(glyphCount) DWRITE_GLYPH_METRICS* glyphMetrics,
    BOOL isSideways)
{
    calltrace::AutoTrace trace(L"DWriteFontFaceHook::GetGdiCompatibleGlyphMetrics");
    if (_GetGdiCompatibleGlyphMetrics_pre_ptr._value) { (this->*_GetGdiCompatibleGlyphMetrics_pre_ptr._value)(emSize, pixelsPerDip, transform, useGdiNatural, glyphIndices, glyphCount, glyphMetrics, isSideways); }
    HRESULT ret = GetRealObj()->GetGdiCompatibleGlyphMetrics(emSize, pixelsPerDip, transform, useGdiNatural, glyphIndices, glyphCount, glyphMetrics, isSideways);
    if (_GetGdiCompatibleGlyphMetrics_post_ptr._value) { (this->*_GetGdiCompatibleGlyphMetrics_post_ptr._value)(ret, emSize, pixelsPerDip, transform, useGdiNatural, glyphIndices, glyphCount, glyphMetrics, isSideways); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontCollectionLoaderHook::CreateEnumeratorFromKey(
    IDWriteFactory* factory,
    _In_reads_bytes_(collectionKeySize) void const* collectionKey,
    UINT32 collectionKeySize,
    _Out_ IDWriteFontFileEnumerator** fontFileEnumerator)
{
    calltrace::AutoTrace trace(L"DWriteFontCollectionLoaderHook::CreateEnumeratorFromKey");
    if (_CreateEnumeratorFromKey_pre_ptr._value) { (this->*_CreateEnumeratorFromKey_pre_ptr._value)(factory, collectionKey, collectionKeySize, fontFileEnumerator); }
    HRESULT ret = GetRealObj()->CreateEnumeratorFromKey(factory, collectionKey, collectionKeySize, fontFileEnumerator);
    if ( fontFileEnumerator && *fontFileEnumerator) { *fontFileEnumerator = RealToHooked_D2D( *fontFileEnumerator ); }
    if (_CreateEnumeratorFromKey_post_ptr._value) { (this->*_CreateEnumeratorFromKey_post_ptr._value)(ret, factory, collectionKey, collectionKeySize, fontFileEnumerator); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileEnumeratorHook::MoveNext(
    _Out_ BOOL* hasCurrentFile)
{
    calltrace::AutoTrace trace(L"DWriteFontFileEnumeratorHook::MoveNext");
    if (_MoveNext_pre_ptr._value) { (this->*_MoveNext_pre_ptr._value)(hasCurrentFile); }
    HRESULT ret = GetRealObj()->MoveNext(hasCurrentFile);
    if (_MoveNext_post_ptr._value) { (this->*_MoveNext_post_ptr._value)(ret, hasCurrentFile); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFileEnumeratorHook::GetCurrentFontFile(
    _Out_ IDWriteFontFile** fontFile)
{
    calltrace::AutoTrace trace(L"DWriteFontFileEnumeratorHook::GetCurrentFontFile");
    if (_GetCurrentFontFile_pre_ptr._value) { (this->*_GetCurrentFontFile_pre_ptr._value)(fontFile); }
    HRESULT ret = GetRealObj()->GetCurrentFontFile(fontFile);
    if ( fontFile && *fontFile) { *fontFile = RealToHooked_D2D( *fontFile ); }
    if (_GetCurrentFontFile_post_ptr._value) { (this->*_GetCurrentFontFile_post_ptr._value)(ret, fontFile); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE DWriteLocalizedStringsHook::GetCount()
{
    calltrace::AutoTrace trace(L"DWriteLocalizedStringsHook::GetCount");
    if (_GetCount_pre_ptr._value) { (this->*_GetCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetCount();
    if (_GetCount_post_ptr._value) { (this->*_GetCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteLocalizedStringsHook::FindLocaleName(
    _In_z_ WCHAR const* localeName,
    _Out_ UINT32* index,
    _Out_ BOOL* exists)
{
    calltrace::AutoTrace trace(L"DWriteLocalizedStringsHook::FindLocaleName");
    if (_FindLocaleName_pre_ptr._value) { (this->*_FindLocaleName_pre_ptr._value)(localeName, index, exists); }
    HRESULT ret = GetRealObj()->FindLocaleName(localeName, index, exists);
    if (_FindLocaleName_post_ptr._value) { (this->*_FindLocaleName_post_ptr._value)(ret, localeName, index, exists); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteLocalizedStringsHook::GetLocaleNameLength(
    UINT32 index,
    _Out_ UINT32* length)
{
    calltrace::AutoTrace trace(L"DWriteLocalizedStringsHook::GetLocaleNameLength");
    if (_GetLocaleNameLength_pre_ptr._value) { (this->*_GetLocaleNameLength_pre_ptr._value)(index, length); }
    HRESULT ret = GetRealObj()->GetLocaleNameLength(index, length);
    if (_GetLocaleNameLength_post_ptr._value) { (this->*_GetLocaleNameLength_post_ptr._value)(ret, index, length); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteLocalizedStringsHook::GetLocaleName(
    UINT32 index,
    _Out_writes_z_(size) WCHAR* localeName,
    UINT32 size)
{
    calltrace::AutoTrace trace(L"DWriteLocalizedStringsHook::GetLocaleName");
    if (_GetLocaleName_pre_ptr._value) { (this->*_GetLocaleName_pre_ptr._value)(index, localeName, size); }
    HRESULT ret = GetRealObj()->GetLocaleName(index, localeName, size);
    if (_GetLocaleName_post_ptr._value) { (this->*_GetLocaleName_post_ptr._value)(ret, index, localeName, size); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteLocalizedStringsHook::GetStringLength(
    UINT32 index,
    _Out_ UINT32* length)
{
    calltrace::AutoTrace trace(L"DWriteLocalizedStringsHook::GetStringLength");
    if (_GetStringLength_pre_ptr._value) { (this->*_GetStringLength_pre_ptr._value)(index, length); }
    HRESULT ret = GetRealObj()->GetStringLength(index, length);
    if (_GetStringLength_post_ptr._value) { (this->*_GetStringLength_post_ptr._value)(ret, index, length); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteLocalizedStringsHook::GetString(
    UINT32 index,
    _Out_writes_z_(size) WCHAR* stringBuffer,
    UINT32 size)
{
    calltrace::AutoTrace trace(L"DWriteLocalizedStringsHook::GetString");
    if (_GetString_pre_ptr._value) { (this->*_GetString_pre_ptr._value)(index, stringBuffer, size); }
    HRESULT ret = GetRealObj()->GetString(index, stringBuffer, size);
    if (_GetString_post_ptr._value) { (this->*_GetString_post_ptr._value)(ret, index, stringBuffer, size); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE DWriteFontCollectionHook::GetFontFamilyCount()
{
    calltrace::AutoTrace trace(L"DWriteFontCollectionHook::GetFontFamilyCount");
    if (_GetFontFamilyCount_pre_ptr._value) { (this->*_GetFontFamilyCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetFontFamilyCount();
    if (_GetFontFamilyCount_post_ptr._value) { (this->*_GetFontFamilyCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontCollectionHook::GetFontFamily(
    UINT32 index,
    _Out_ IDWriteFontFamily** fontFamily)
{
    calltrace::AutoTrace trace(L"DWriteFontCollectionHook::GetFontFamily");
    if (_GetFontFamily_pre_ptr._value) { (this->*_GetFontFamily_pre_ptr._value)(index, fontFamily); }
    HRESULT ret = GetRealObj()->GetFontFamily(index, fontFamily);
    if ( fontFamily && *fontFamily) { *fontFamily = RealToHooked_D2D( *fontFamily ); }
    if (_GetFontFamily_post_ptr._value) { (this->*_GetFontFamily_post_ptr._value)(ret, index, fontFamily); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontCollectionHook::FindFamilyName(
    _In_z_ WCHAR const* familyName,
    _Out_ UINT32* index,
    _Out_ BOOL* exists)
{
    calltrace::AutoTrace trace(L"DWriteFontCollectionHook::FindFamilyName");
    if (_FindFamilyName_pre_ptr._value) { (this->*_FindFamilyName_pre_ptr._value)(familyName, index, exists); }
    HRESULT ret = GetRealObj()->FindFamilyName(familyName, index, exists);
    if (_FindFamilyName_post_ptr._value) { (this->*_FindFamilyName_post_ptr._value)(ret, familyName, index, exists); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontCollectionHook::GetFontFromFontFace(
    IDWriteFontFace* fontFace,
    _Out_ IDWriteFont** font)
{
    calltrace::AutoTrace trace(L"DWriteFontCollectionHook::GetFontFromFontFace");
    if (_GetFontFromFontFace_pre_ptr._value) { (this->*_GetFontFromFontFace_pre_ptr._value)(fontFace, font); }
    HRESULT ret = GetRealObj()->GetFontFromFontFace(fontFace, font);
    if ( font && *font) { *font = RealToHooked_D2D( *font ); }
    if (_GetFontFromFontFace_post_ptr._value) { (this->*_GetFontFromFontFace_post_ptr._value)(ret, fontFace, font); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontListHook::GetFontCollection(
    _Out_ IDWriteFontCollection** fontCollection)
{
    calltrace::AutoTrace trace(L"DWriteFontListHook::GetFontCollection");
    if (_GetFontCollection_pre_ptr._value) { (this->*_GetFontCollection_pre_ptr._value)(fontCollection); }
    HRESULT ret = GetRealObj()->GetFontCollection(fontCollection);
    if ( fontCollection && *fontCollection) { *fontCollection = RealToHooked_D2D( *fontCollection ); }
    if (_GetFontCollection_post_ptr._value) { (this->*_GetFontCollection_post_ptr._value)(ret, fontCollection); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE DWriteFontListHook::GetFontCount()
{
    calltrace::AutoTrace trace(L"DWriteFontListHook::GetFontCount");
    if (_GetFontCount_pre_ptr._value) { (this->*_GetFontCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetFontCount();
    if (_GetFontCount_post_ptr._value) { (this->*_GetFontCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontListHook::GetFont(
    UINT32 index,
    _Out_ IDWriteFont** font)
{
    calltrace::AutoTrace trace(L"DWriteFontListHook::GetFont");
    if (_GetFont_pre_ptr._value) { (this->*_GetFont_pre_ptr._value)(index, font); }
    HRESULT ret = GetRealObj()->GetFont(index, font);
    if ( font && *font) { *font = RealToHooked_D2D( *font ); }
    if (_GetFont_post_ptr._value) { (this->*_GetFont_post_ptr._value)(ret, index, font); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFamilyHook::GetFamilyNames(
    _Out_ IDWriteLocalizedStrings** names)
{
    calltrace::AutoTrace trace(L"DWriteFontFamilyHook::GetFamilyNames");
    if (_GetFamilyNames_pre_ptr._value) { (this->*_GetFamilyNames_pre_ptr._value)(names); }
    HRESULT ret = GetRealObj()->GetFamilyNames(names);
    if ( names && *names) { *names = RealToHooked_D2D( *names ); }
    if (_GetFamilyNames_post_ptr._value) { (this->*_GetFamilyNames_post_ptr._value)(ret, names); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFamilyHook::GetFirstMatchingFont(
    DWRITE_FONT_WEIGHT weight,
    DWRITE_FONT_STRETCH stretch,
    DWRITE_FONT_STYLE style,
    _Out_ IDWriteFont** matchingFont)
{
    calltrace::AutoTrace trace(L"DWriteFontFamilyHook::GetFirstMatchingFont");
    if (_GetFirstMatchingFont_pre_ptr._value) { (this->*_GetFirstMatchingFont_pre_ptr._value)(weight, stretch, style, matchingFont); }
    HRESULT ret = GetRealObj()->GetFirstMatchingFont(weight, stretch, style, matchingFont);
    if ( matchingFont && *matchingFont) { *matchingFont = RealToHooked_D2D( *matchingFont ); }
    if (_GetFirstMatchingFont_post_ptr._value) { (this->*_GetFirstMatchingFont_post_ptr._value)(ret, weight, stretch, style, matchingFont); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontFamilyHook::GetMatchingFonts(
    DWRITE_FONT_WEIGHT weight,
    DWRITE_FONT_STRETCH stretch,
    DWRITE_FONT_STYLE style,
    _Out_ IDWriteFontList** matchingFonts)
{
    calltrace::AutoTrace trace(L"DWriteFontFamilyHook::GetMatchingFonts");
    if (_GetMatchingFonts_pre_ptr._value) { (this->*_GetMatchingFonts_pre_ptr._value)(weight, stretch, style, matchingFonts); }
    HRESULT ret = GetRealObj()->GetMatchingFonts(weight, stretch, style, matchingFonts);
    if ( matchingFonts && *matchingFonts) { *matchingFonts = RealToHooked_D2D( *matchingFonts ); }
    if (_GetMatchingFonts_post_ptr._value) { (this->*_GetMatchingFonts_post_ptr._value)(ret, weight, stretch, style, matchingFonts); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontHook::GetFontFamily(
    _Out_ IDWriteFontFamily** fontFamily)
{
    calltrace::AutoTrace trace(L"DWriteFontHook::GetFontFamily");
    if (_GetFontFamily_pre_ptr._value) { (this->*_GetFontFamily_pre_ptr._value)(fontFamily); }
    HRESULT ret = GetRealObj()->GetFontFamily(fontFamily);
    if ( fontFamily && *fontFamily) { *fontFamily = RealToHooked_D2D( *fontFamily ); }
    if (_GetFontFamily_post_ptr._value) { (this->*_GetFontFamily_post_ptr._value)(ret, fontFamily); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_WEIGHT STDMETHODCALLTYPE DWriteFontHook::GetWeight()
{
    calltrace::AutoTrace trace(L"DWriteFontHook::GetWeight");
    if (_GetWeight_pre_ptr._value) { (this->*_GetWeight_pre_ptr._value)(); }
    DWRITE_FONT_WEIGHT ret = GetRealObj()->GetWeight();
    if (_GetWeight_post_ptr._value) { (this->*_GetWeight_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_STRETCH STDMETHODCALLTYPE DWriteFontHook::GetStretch()
{
    calltrace::AutoTrace trace(L"DWriteFontHook::GetStretch");
    if (_GetStretch_pre_ptr._value) { (this->*_GetStretch_pre_ptr._value)(); }
    DWRITE_FONT_STRETCH ret = GetRealObj()->GetStretch();
    if (_GetStretch_post_ptr._value) { (this->*_GetStretch_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_STYLE STDMETHODCALLTYPE DWriteFontHook::GetStyle()
{
    calltrace::AutoTrace trace(L"DWriteFontHook::GetStyle");
    if (_GetStyle_pre_ptr._value) { (this->*_GetStyle_pre_ptr._value)(); }
    DWRITE_FONT_STYLE ret = GetRealObj()->GetStyle();
    if (_GetStyle_post_ptr._value) { (this->*_GetStyle_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DWriteFontHook::IsSymbolFont()
{
    calltrace::AutoTrace trace(L"DWriteFontHook::IsSymbolFont");
    if (_IsSymbolFont_pre_ptr._value) { (this->*_IsSymbolFont_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsSymbolFont();
    if (_IsSymbolFont_post_ptr._value) { (this->*_IsSymbolFont_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontHook::GetFaceNames(
    _Out_ IDWriteLocalizedStrings** names)
{
    calltrace::AutoTrace trace(L"DWriteFontHook::GetFaceNames");
    if (_GetFaceNames_pre_ptr._value) { (this->*_GetFaceNames_pre_ptr._value)(names); }
    HRESULT ret = GetRealObj()->GetFaceNames(names);
    if ( names && *names) { *names = RealToHooked_D2D( *names ); }
    if (_GetFaceNames_post_ptr._value) { (this->*_GetFaceNames_post_ptr._value)(ret, names); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontHook::GetInformationalStrings(
    DWRITE_INFORMATIONAL_STRING_ID informationalStringID,
    _Out_ IDWriteLocalizedStrings** informationalStrings,
    _Out_ BOOL* exists)
{
    calltrace::AutoTrace trace(L"DWriteFontHook::GetInformationalStrings");
    if (_GetInformationalStrings_pre_ptr._value) { (this->*_GetInformationalStrings_pre_ptr._value)(informationalStringID, informationalStrings, exists); }
    HRESULT ret = GetRealObj()->GetInformationalStrings(informationalStringID, informationalStrings, exists);
    if ( informationalStrings && *informationalStrings) { *informationalStrings = RealToHooked_D2D( *informationalStrings ); }
    if (_GetInformationalStrings_post_ptr._value) { (this->*_GetInformationalStrings_post_ptr._value)(ret, informationalStringID, informationalStrings, exists); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_SIMULATIONS STDMETHODCALLTYPE DWriteFontHook::GetSimulations()
{
    calltrace::AutoTrace trace(L"DWriteFontHook::GetSimulations");
    if (_GetSimulations_pre_ptr._value) { (this->*_GetSimulations_pre_ptr._value)(); }
    DWRITE_FONT_SIMULATIONS ret = GetRealObj()->GetSimulations();
    if (_GetSimulations_post_ptr._value) { (this->*_GetSimulations_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DWriteFontHook::GetMetrics(
    _Out_ DWRITE_FONT_METRICS* fontMetrics)
{
    calltrace::AutoTrace trace(L"DWriteFontHook::GetMetrics");
    if (_GetMetrics_pre_ptr._value) { (this->*_GetMetrics_pre_ptr._value)(fontMetrics); }
    GetRealObj()->GetMetrics(fontMetrics);
    if (_GetMetrics_post_ptr._value) { (this->*_GetMetrics_post_ptr._value)(fontMetrics); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontHook::HasCharacter(
    UINT32 unicodeValue,
    _Out_ BOOL* exists)
{
    calltrace::AutoTrace trace(L"DWriteFontHook::HasCharacter");
    if (_HasCharacter_pre_ptr._value) { (this->*_HasCharacter_pre_ptr._value)(unicodeValue, exists); }
    HRESULT ret = GetRealObj()->HasCharacter(unicodeValue, exists);
    if (_HasCharacter_post_ptr._value) { (this->*_HasCharacter_post_ptr._value)(ret, unicodeValue, exists); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFontHook::CreateFontFace(
    _Out_ IDWriteFontFace** fontFace)
{
    calltrace::AutoTrace trace(L"DWriteFontHook::CreateFontFace");
    if (_CreateFontFace_pre_ptr._value) { (this->*_CreateFontFace_pre_ptr._value)(fontFace); }
    HRESULT ret = GetRealObj()->CreateFontFace(fontFace);
    if ( fontFace && *fontFace) { *fontFace = RealToHooked_D2D( *fontFace ); }
    if (_CreateFontFace_post_ptr._value) { (this->*_CreateFontFace_post_ptr._value)(ret, fontFace); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::SetTextAlignment(
    DWRITE_TEXT_ALIGNMENT textAlignment)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::SetTextAlignment");
    if (_SetTextAlignment_pre_ptr._value) { (this->*_SetTextAlignment_pre_ptr._value)(textAlignment); }
    HRESULT ret = GetRealObj()->SetTextAlignment(textAlignment);
    if (_SetTextAlignment_post_ptr._value) { (this->*_SetTextAlignment_post_ptr._value)(ret, textAlignment); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::SetParagraphAlignment(
    DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::SetParagraphAlignment");
    if (_SetParagraphAlignment_pre_ptr._value) { (this->*_SetParagraphAlignment_pre_ptr._value)(paragraphAlignment); }
    HRESULT ret = GetRealObj()->SetParagraphAlignment(paragraphAlignment);
    if (_SetParagraphAlignment_post_ptr._value) { (this->*_SetParagraphAlignment_post_ptr._value)(ret, paragraphAlignment); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::SetWordWrapping(
    DWRITE_WORD_WRAPPING wordWrapping)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::SetWordWrapping");
    if (_SetWordWrapping_pre_ptr._value) { (this->*_SetWordWrapping_pre_ptr._value)(wordWrapping); }
    HRESULT ret = GetRealObj()->SetWordWrapping(wordWrapping);
    if (_SetWordWrapping_post_ptr._value) { (this->*_SetWordWrapping_post_ptr._value)(ret, wordWrapping); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::SetReadingDirection(
    DWRITE_READING_DIRECTION readingDirection)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::SetReadingDirection");
    if (_SetReadingDirection_pre_ptr._value) { (this->*_SetReadingDirection_pre_ptr._value)(readingDirection); }
    HRESULT ret = GetRealObj()->SetReadingDirection(readingDirection);
    if (_SetReadingDirection_post_ptr._value) { (this->*_SetReadingDirection_post_ptr._value)(ret, readingDirection); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::SetFlowDirection(
    DWRITE_FLOW_DIRECTION flowDirection)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::SetFlowDirection");
    if (_SetFlowDirection_pre_ptr._value) { (this->*_SetFlowDirection_pre_ptr._value)(flowDirection); }
    HRESULT ret = GetRealObj()->SetFlowDirection(flowDirection);
    if (_SetFlowDirection_post_ptr._value) { (this->*_SetFlowDirection_post_ptr._value)(ret, flowDirection); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::SetIncrementalTabStop(
    FLOAT incrementalTabStop)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::SetIncrementalTabStop");
    if (_SetIncrementalTabStop_pre_ptr._value) { (this->*_SetIncrementalTabStop_pre_ptr._value)(incrementalTabStop); }
    HRESULT ret = GetRealObj()->SetIncrementalTabStop(incrementalTabStop);
    if (_SetIncrementalTabStop_post_ptr._value) { (this->*_SetIncrementalTabStop_post_ptr._value)(ret, incrementalTabStop); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::SetTrimming(
    _In_ DWRITE_TRIMMING const* trimmingOptions,
    IDWriteInlineObject* trimmingSign)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::SetTrimming");
    if (_SetTrimming_pre_ptr._value) { (this->*_SetTrimming_pre_ptr._value)(trimmingOptions, trimmingSign); }
    HRESULT ret = GetRealObj()->SetTrimming(trimmingOptions, trimmingSign);
    if (_SetTrimming_post_ptr._value) { (this->*_SetTrimming_post_ptr._value)(ret, trimmingOptions, trimmingSign); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::SetLineSpacing(
    DWRITE_LINE_SPACING_METHOD lineSpacingMethod,
    FLOAT lineSpacing,
    FLOAT baseline)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::SetLineSpacing");
    if (_SetLineSpacing_pre_ptr._value) { (this->*_SetLineSpacing_pre_ptr._value)(lineSpacingMethod, lineSpacing, baseline); }
    HRESULT ret = GetRealObj()->SetLineSpacing(lineSpacingMethod, lineSpacing, baseline);
    if (_SetLineSpacing_post_ptr._value) { (this->*_SetLineSpacing_post_ptr._value)(ret, lineSpacingMethod, lineSpacing, baseline); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_TEXT_ALIGNMENT STDMETHODCALLTYPE DWriteTextFormatHook::GetTextAlignment()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetTextAlignment");
    if (_GetTextAlignment_pre_ptr._value) { (this->*_GetTextAlignment_pre_ptr._value)(); }
    DWRITE_TEXT_ALIGNMENT ret = GetRealObj()->GetTextAlignment();
    if (_GetTextAlignment_post_ptr._value) { (this->*_GetTextAlignment_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_PARAGRAPH_ALIGNMENT STDMETHODCALLTYPE DWriteTextFormatHook::GetParagraphAlignment()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetParagraphAlignment");
    if (_GetParagraphAlignment_pre_ptr._value) { (this->*_GetParagraphAlignment_pre_ptr._value)(); }
    DWRITE_PARAGRAPH_ALIGNMENT ret = GetRealObj()->GetParagraphAlignment();
    if (_GetParagraphAlignment_post_ptr._value) { (this->*_GetParagraphAlignment_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_WORD_WRAPPING STDMETHODCALLTYPE DWriteTextFormatHook::GetWordWrapping()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetWordWrapping");
    if (_GetWordWrapping_pre_ptr._value) { (this->*_GetWordWrapping_pre_ptr._value)(); }
    DWRITE_WORD_WRAPPING ret = GetRealObj()->GetWordWrapping();
    if (_GetWordWrapping_post_ptr._value) { (this->*_GetWordWrapping_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_READING_DIRECTION STDMETHODCALLTYPE DWriteTextFormatHook::GetReadingDirection()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetReadingDirection");
    if (_GetReadingDirection_pre_ptr._value) { (this->*_GetReadingDirection_pre_ptr._value)(); }
    DWRITE_READING_DIRECTION ret = GetRealObj()->GetReadingDirection();
    if (_GetReadingDirection_post_ptr._value) { (this->*_GetReadingDirection_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FLOW_DIRECTION STDMETHODCALLTYPE DWriteTextFormatHook::GetFlowDirection()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetFlowDirection");
    if (_GetFlowDirection_pre_ptr._value) { (this->*_GetFlowDirection_pre_ptr._value)(); }
    DWRITE_FLOW_DIRECTION ret = GetRealObj()->GetFlowDirection();
    if (_GetFlowDirection_post_ptr._value) { (this->*_GetFlowDirection_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE DWriteTextFormatHook::GetIncrementalTabStop()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetIncrementalTabStop");
    if (_GetIncrementalTabStop_pre_ptr._value) { (this->*_GetIncrementalTabStop_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetIncrementalTabStop();
    if (_GetIncrementalTabStop_post_ptr._value) { (this->*_GetIncrementalTabStop_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::GetTrimming(
    _Out_ DWRITE_TRIMMING* trimmingOptions,
    _Out_ IDWriteInlineObject** trimmingSign)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetTrimming");
    if (_GetTrimming_pre_ptr._value) { (this->*_GetTrimming_pre_ptr._value)(trimmingOptions, trimmingSign); }
    HRESULT ret = GetRealObj()->GetTrimming(trimmingOptions, trimmingSign);
    if ( trimmingSign && *trimmingSign) { *trimmingSign = RealToHooked_D2D( *trimmingSign ); }
    if (_GetTrimming_post_ptr._value) { (this->*_GetTrimming_post_ptr._value)(ret, trimmingOptions, trimmingSign); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::GetLineSpacing(
    _Out_ DWRITE_LINE_SPACING_METHOD* lineSpacingMethod,
    _Out_ FLOAT* lineSpacing,
    _Out_ FLOAT* baseline)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetLineSpacing");
    if (_GetLineSpacing_pre_ptr._value) { (this->*_GetLineSpacing_pre_ptr._value)(lineSpacingMethod, lineSpacing, baseline); }
    HRESULT ret = GetRealObj()->GetLineSpacing(lineSpacingMethod, lineSpacing, baseline);
    if (_GetLineSpacing_post_ptr._value) { (this->*_GetLineSpacing_post_ptr._value)(ret, lineSpacingMethod, lineSpacing, baseline); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::GetFontCollection(
    _Out_ IDWriteFontCollection** fontCollection)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetFontCollection");
    if (_GetFontCollection_pre_ptr._value) { (this->*_GetFontCollection_pre_ptr._value)(fontCollection); }
    HRESULT ret = GetRealObj()->GetFontCollection(fontCollection);
    if ( fontCollection && *fontCollection) { *fontCollection = RealToHooked_D2D( *fontCollection ); }
    if (_GetFontCollection_post_ptr._value) { (this->*_GetFontCollection_post_ptr._value)(ret, fontCollection); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE DWriteTextFormatHook::GetFontFamilyNameLength()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetFontFamilyNameLength");
    if (_GetFontFamilyNameLength_pre_ptr._value) { (this->*_GetFontFamilyNameLength_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetFontFamilyNameLength();
    if (_GetFontFamilyNameLength_post_ptr._value) { (this->*_GetFontFamilyNameLength_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::GetFontFamilyName(
    _Out_writes_z_(nameSize) WCHAR* fontFamilyName,
    UINT32 nameSize)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetFontFamilyName");
    if (_GetFontFamilyName_pre_ptr._value) { (this->*_GetFontFamilyName_pre_ptr._value)(fontFamilyName, nameSize); }
    HRESULT ret = GetRealObj()->GetFontFamilyName(fontFamilyName, nameSize);
    if (_GetFontFamilyName_post_ptr._value) { (this->*_GetFontFamilyName_post_ptr._value)(ret, fontFamilyName, nameSize); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_WEIGHT STDMETHODCALLTYPE DWriteTextFormatHook::GetFontWeight()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetFontWeight");
    if (_GetFontWeight_pre_ptr._value) { (this->*_GetFontWeight_pre_ptr._value)(); }
    DWRITE_FONT_WEIGHT ret = GetRealObj()->GetFontWeight();
    if (_GetFontWeight_post_ptr._value) { (this->*_GetFontWeight_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_STYLE STDMETHODCALLTYPE DWriteTextFormatHook::GetFontStyle()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetFontStyle");
    if (_GetFontStyle_pre_ptr._value) { (this->*_GetFontStyle_pre_ptr._value)(); }
    DWRITE_FONT_STYLE ret = GetRealObj()->GetFontStyle();
    if (_GetFontStyle_post_ptr._value) { (this->*_GetFontStyle_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_FONT_STRETCH STDMETHODCALLTYPE DWriteTextFormatHook::GetFontStretch()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetFontStretch");
    if (_GetFontStretch_pre_ptr._value) { (this->*_GetFontStretch_pre_ptr._value)(); }
    DWRITE_FONT_STRETCH ret = GetRealObj()->GetFontStretch();
    if (_GetFontStretch_post_ptr._value) { (this->*_GetFontStretch_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE DWriteTextFormatHook::GetFontSize()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetFontSize");
    if (_GetFontSize_pre_ptr._value) { (this->*_GetFontSize_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetFontSize();
    if (_GetFontSize_post_ptr._value) { (this->*_GetFontSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE DWriteTextFormatHook::GetLocaleNameLength()
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetLocaleNameLength");
    if (_GetLocaleNameLength_pre_ptr._value) { (this->*_GetLocaleNameLength_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetLocaleNameLength();
    if (_GetLocaleNameLength_post_ptr._value) { (this->*_GetLocaleNameLength_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextFormatHook::GetLocaleName(
    _Out_writes_z_(nameSize) WCHAR* localeName,
    UINT32 nameSize)
{
    calltrace::AutoTrace trace(L"DWriteTextFormatHook::GetLocaleName");
    if (_GetLocaleName_pre_ptr._value) { (this->*_GetLocaleName_pre_ptr._value)(localeName, nameSize); }
    HRESULT ret = GetRealObj()->GetLocaleName(localeName, nameSize);
    if (_GetLocaleName_post_ptr._value) { (this->*_GetLocaleName_post_ptr._value)(ret, localeName, nameSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTypographyHook::AddFontFeature(
    DWRITE_FONT_FEATURE fontFeature)
{
    calltrace::AutoTrace trace(L"DWriteTypographyHook::AddFontFeature");
    if (_AddFontFeature_pre_ptr._value) { (this->*_AddFontFeature_pre_ptr._value)(fontFeature); }
    HRESULT ret = GetRealObj()->AddFontFeature(fontFeature);
    if (_AddFontFeature_post_ptr._value) { (this->*_AddFontFeature_post_ptr._value)(ret, fontFeature); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT32 STDMETHODCALLTYPE DWriteTypographyHook::GetFontFeatureCount()
{
    calltrace::AutoTrace trace(L"DWriteTypographyHook::GetFontFeatureCount");
    if (_GetFontFeatureCount_pre_ptr._value) { (this->*_GetFontFeatureCount_pre_ptr._value)(); }
    UINT32 ret = GetRealObj()->GetFontFeatureCount();
    if (_GetFontFeatureCount_post_ptr._value) { (this->*_GetFontFeatureCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTypographyHook::GetFontFeature(
    UINT32 fontFeatureIndex,
    _Out_ DWRITE_FONT_FEATURE* fontFeature)
{
    calltrace::AutoTrace trace(L"DWriteTypographyHook::GetFontFeature");
    if (_GetFontFeature_pre_ptr._value) { (this->*_GetFontFeature_pre_ptr._value)(fontFeatureIndex, fontFeature); }
    HRESULT ret = GetRealObj()->GetFontFeature(fontFeatureIndex, fontFeature);
    if (_GetFontFeature_post_ptr._value) { (this->*_GetFontFeature_post_ptr._value)(ret, fontFeatureIndex, fontFeature); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalysisSourceHook::GetTextAtPosition(
    UINT32 textPosition,
    _Outptr_result_buffer_(*textLength) WCHAR const** textString,
    _Out_ UINT32* textLength)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSourceHook::GetTextAtPosition");
    if (_GetTextAtPosition_pre_ptr._value) { (this->*_GetTextAtPosition_pre_ptr._value)(textPosition, textString, textLength); }
    HRESULT ret = GetRealObj()->GetTextAtPosition(textPosition, textString, textLength);
    if (_GetTextAtPosition_post_ptr._value) { (this->*_GetTextAtPosition_post_ptr._value)(ret, textPosition, textString, textLength); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalysisSourceHook::GetTextBeforePosition(
    UINT32 textPosition,
    _Outptr_result_buffer_(*textLength) WCHAR const** textString,
    _Out_ UINT32* textLength)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSourceHook::GetTextBeforePosition");
    if (_GetTextBeforePosition_pre_ptr._value) { (this->*_GetTextBeforePosition_pre_ptr._value)(textPosition, textString, textLength); }
    HRESULT ret = GetRealObj()->GetTextBeforePosition(textPosition, textString, textLength);
    if (_GetTextBeforePosition_post_ptr._value) { (this->*_GetTextBeforePosition_post_ptr._value)(ret, textPosition, textString, textLength); }
    return ret;
}

// -----------------------------------------------------------------------------
DWRITE_READING_DIRECTION STDMETHODCALLTYPE DWriteTextAnalysisSourceHook::GetParagraphReadingDirection()
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSourceHook::GetParagraphReadingDirection");
    if (_GetParagraphReadingDirection_pre_ptr._value) { (this->*_GetParagraphReadingDirection_pre_ptr._value)(); }
    DWRITE_READING_DIRECTION ret = GetRealObj()->GetParagraphReadingDirection();
    if (_GetParagraphReadingDirection_post_ptr._value) { (this->*_GetParagraphReadingDirection_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalysisSourceHook::GetLocaleName(
    UINT32 textPosition,
    _Out_ UINT32* textLength,
    _Outptr_result_z_ WCHAR const** localeName)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSourceHook::GetLocaleName");
    if (_GetLocaleName_pre_ptr._value) { (this->*_GetLocaleName_pre_ptr._value)(textPosition, textLength, localeName); }
    HRESULT ret = GetRealObj()->GetLocaleName(textPosition, textLength, localeName);
    if (_GetLocaleName_post_ptr._value) { (this->*_GetLocaleName_post_ptr._value)(ret, textPosition, textLength, localeName); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalysisSourceHook::GetNumberSubstitution(
    UINT32 textPosition,
    _Out_ UINT32* textLength,
    _Outptr_ IDWriteNumberSubstitution** numberSubstitution)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSourceHook::GetNumberSubstitution");
    if (_GetNumberSubstitution_pre_ptr._value) { (this->*_GetNumberSubstitution_pre_ptr._value)(textPosition, textLength, numberSubstitution); }
    HRESULT ret = GetRealObj()->GetNumberSubstitution(textPosition, textLength, numberSubstitution);
    if ( numberSubstitution && *numberSubstitution) { *numberSubstitution = RealToHooked_D2D( *numberSubstitution ); }
    if (_GetNumberSubstitution_post_ptr._value) { (this->*_GetNumberSubstitution_post_ptr._value)(ret, textPosition, textLength, numberSubstitution); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalysisSinkHook::SetScriptAnalysis(
    UINT32 textPosition,
    UINT32 textLength,
    _In_ DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSinkHook::SetScriptAnalysis");
    if (_SetScriptAnalysis_pre_ptr._value) { (this->*_SetScriptAnalysis_pre_ptr._value)(textPosition, textLength, scriptAnalysis); }
    HRESULT ret = GetRealObj()->SetScriptAnalysis(textPosition, textLength, scriptAnalysis);
    if (_SetScriptAnalysis_post_ptr._value) { (this->*_SetScriptAnalysis_post_ptr._value)(ret, textPosition, textLength, scriptAnalysis); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalysisSinkHook::SetLineBreakpoints(
    UINT32 textPosition,
    UINT32 textLength,
    _In_reads_(textLength) DWRITE_LINE_BREAKPOINT const* lineBreakpoints)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSinkHook::SetLineBreakpoints");
    if (_SetLineBreakpoints_pre_ptr._value) { (this->*_SetLineBreakpoints_pre_ptr._value)(textPosition, textLength, lineBreakpoints); }
    HRESULT ret = GetRealObj()->SetLineBreakpoints(textPosition, textLength, lineBreakpoints);
    if (_SetLineBreakpoints_post_ptr._value) { (this->*_SetLineBreakpoints_post_ptr._value)(ret, textPosition, textLength, lineBreakpoints); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalysisSinkHook::SetBidiLevel(
    UINT32 textPosition,
    UINT32 textLength,
    UINT8 explicitLevel,
    UINT8 resolvedLevel)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSinkHook::SetBidiLevel");
    if (_SetBidiLevel_pre_ptr._value) { (this->*_SetBidiLevel_pre_ptr._value)(textPosition, textLength, explicitLevel, resolvedLevel); }
    HRESULT ret = GetRealObj()->SetBidiLevel(textPosition, textLength, explicitLevel, resolvedLevel);
    if (_SetBidiLevel_post_ptr._value) { (this->*_SetBidiLevel_post_ptr._value)(ret, textPosition, textLength, explicitLevel, resolvedLevel); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalysisSinkHook::SetNumberSubstitution(
    UINT32 textPosition,
    UINT32 textLength,
    _In_ IDWriteNumberSubstitution* numberSubstitution)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalysisSinkHook::SetNumberSubstitution");
    if (_SetNumberSubstitution_pre_ptr._value) { (this->*_SetNumberSubstitution_pre_ptr._value)(textPosition, textLength, numberSubstitution); }
    HRESULT ret = GetRealObj()->SetNumberSubstitution(textPosition, textLength, HookedToReal(numberSubstitution));
    if (_SetNumberSubstitution_post_ptr._value) { (this->*_SetNumberSubstitution_post_ptr._value)(ret, textPosition, textLength, numberSubstitution); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalyzerHook::AnalyzeScript(
    IDWriteTextAnalysisSource* analysisSource,
    UINT32 textPosition,
    UINT32 textLength,
    IDWriteTextAnalysisSink* analysisSink)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalyzerHook::AnalyzeScript");
    if (_AnalyzeScript_pre_ptr._value) { (this->*_AnalyzeScript_pre_ptr._value)(analysisSource, textPosition, textLength, analysisSink); }
    HRESULT ret = GetRealObj()->AnalyzeScript(analysisSource, textPosition, textLength, analysisSink);
    if (_AnalyzeScript_post_ptr._value) { (this->*_AnalyzeScript_post_ptr._value)(ret, analysisSource, textPosition, textLength, analysisSink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalyzerHook::AnalyzeBidi(
    IDWriteTextAnalysisSource* analysisSource,
    UINT32 textPosition,
    UINT32 textLength,
    IDWriteTextAnalysisSink* analysisSink)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalyzerHook::AnalyzeBidi");
    if (_AnalyzeBidi_pre_ptr._value) { (this->*_AnalyzeBidi_pre_ptr._value)(analysisSource, textPosition, textLength, analysisSink); }
    HRESULT ret = GetRealObj()->AnalyzeBidi(analysisSource, textPosition, textLength, analysisSink);
    if (_AnalyzeBidi_post_ptr._value) { (this->*_AnalyzeBidi_post_ptr._value)(ret, analysisSource, textPosition, textLength, analysisSink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalyzerHook::AnalyzeNumberSubstitution(
    IDWriteTextAnalysisSource* analysisSource,
    UINT32 textPosition,
    UINT32 textLength,
    IDWriteTextAnalysisSink* analysisSink)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalyzerHook::AnalyzeNumberSubstitution");
    if (_AnalyzeNumberSubstitution_pre_ptr._value) { (this->*_AnalyzeNumberSubstitution_pre_ptr._value)(analysisSource, textPosition, textLength, analysisSink); }
    HRESULT ret = GetRealObj()->AnalyzeNumberSubstitution(analysisSource, textPosition, textLength, analysisSink);
    if (_AnalyzeNumberSubstitution_post_ptr._value) { (this->*_AnalyzeNumberSubstitution_post_ptr._value)(ret, analysisSource, textPosition, textLength, analysisSink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalyzerHook::AnalyzeLineBreakpoints(
    IDWriteTextAnalysisSource* analysisSource,
    UINT32 textPosition,
    UINT32 textLength,
    IDWriteTextAnalysisSink* analysisSink)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalyzerHook::AnalyzeLineBreakpoints");
    if (_AnalyzeLineBreakpoints_pre_ptr._value) { (this->*_AnalyzeLineBreakpoints_pre_ptr._value)(analysisSource, textPosition, textLength, analysisSink); }
    HRESULT ret = GetRealObj()->AnalyzeLineBreakpoints(analysisSource, textPosition, textLength, analysisSink);
    if (_AnalyzeLineBreakpoints_post_ptr._value) { (this->*_AnalyzeLineBreakpoints_post_ptr._value)(ret, analysisSource, textPosition, textLength, analysisSink); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalyzerHook::GetGlyphs(
    _In_reads_(textLength) WCHAR const* textString,
    UINT32 textLength,
    IDWriteFontFace* fontFace,
    BOOL isSideways,
    BOOL isRightToLeft,
    _In_ DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis,
    _In_opt_z_ WCHAR const* localeName,
    _In_opt_ IDWriteNumberSubstitution* numberSubstitution,
    _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features,
    _In_reads_opt_(featureRanges) UINT32 const* featureRangeLengths,
    UINT32 featureRanges,
    UINT32 maxGlyphCount,
    _Out_writes_(textLength) UINT16* clusterMap,
    _Out_writes_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps,
    _Out_writes_(maxGlyphCount) UINT16* glyphIndices,
    _Out_writes_(maxGlyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES* glyphProps,
    _Out_ UINT32* actualGlyphCount)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalyzerHook::GetGlyphs");
    if (_GetGlyphs_pre_ptr._value) { (this->*_GetGlyphs_pre_ptr._value)(textString, textLength, fontFace, isSideways, isRightToLeft, scriptAnalysis, localeName, numberSubstitution, features, featureRangeLengths, featureRanges, maxGlyphCount, clusterMap, textProps, glyphIndices, glyphProps, actualGlyphCount); }
    HRESULT ret = GetRealObj()->GetGlyphs(textString, textLength, fontFace, isSideways, isRightToLeft, scriptAnalysis, localeName, HookedToReal(numberSubstitution), features, featureRangeLengths, featureRanges, maxGlyphCount, clusterMap, textProps, glyphIndices, glyphProps, actualGlyphCount);
    if (_GetGlyphs_post_ptr._value) { (this->*_GetGlyphs_post_ptr._value)(ret, textString, textLength, fontFace, isSideways, isRightToLeft, scriptAnalysis, localeName, numberSubstitution, features, featureRangeLengths, featureRanges, maxGlyphCount, clusterMap, textProps, glyphIndices, glyphProps, actualGlyphCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalyzerHook::GetGlyphPlacements(
    _In_reads_(textLength) WCHAR const* textString,
    _In_reads_(textLength) UINT16 const* clusterMap,
    _In_reads_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps,
    UINT32 textLength,
    _In_reads_(glyphCount) UINT16 const* glyphIndices,
    _In_reads_(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const* glyphProps,
    UINT32 glyphCount,
    IDWriteFontFace * fontFace,
    FLOAT fontEmSize,
    BOOL isSideways,
    BOOL isRightToLeft,
    _In_ DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis,
    _In_opt_z_ WCHAR const* localeName,
    _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features,
    _In_reads_opt_(featureRanges) UINT32 const* featureRangeLengths,
    UINT32 featureRanges,
    _Out_writes_(glyphCount) FLOAT* glyphAdvances,
    _Out_writes_(glyphCount) DWRITE_GLYPH_OFFSET* glyphOffsets)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalyzerHook::GetGlyphPlacements");
    if (_GetGlyphPlacements_pre_ptr._value) { (this->*_GetGlyphPlacements_pre_ptr._value)(textString, clusterMap, textProps, textLength, glyphIndices, glyphProps, glyphCount, fontFace, fontEmSize, isSideways, isRightToLeft, scriptAnalysis, localeName, features, featureRangeLengths, featureRanges, glyphAdvances, glyphOffsets); }
    HRESULT ret = GetRealObj()->GetGlyphPlacements(textString, clusterMap, textProps, textLength, glyphIndices, glyphProps, glyphCount, fontFace, fontEmSize, isSideways, isRightToLeft, scriptAnalysis, localeName, features, featureRangeLengths, featureRanges, glyphAdvances, glyphOffsets);
    if (_GetGlyphPlacements_post_ptr._value) { (this->*_GetGlyphPlacements_post_ptr._value)(ret, textString, clusterMap, textProps, textLength, glyphIndices, glyphProps, glyphCount, fontFace, fontEmSize, isSideways, isRightToLeft, scriptAnalysis, localeName, features, featureRangeLengths, featureRanges, glyphAdvances, glyphOffsets); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextAnalyzerHook::GetGdiCompatibleGlyphPlacements(
    _In_reads_(textLength) WCHAR const* textString,
    _In_reads_(textLength) UINT16 const* clusterMap,
    _In_reads_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps,
    UINT32 textLength,
    _In_reads_(glyphCount) UINT16 const* glyphIndices,
    _In_reads_(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const* glyphProps,
    UINT32 glyphCount,
    IDWriteFontFace * fontFace,
    FLOAT fontEmSize,
    FLOAT pixelsPerDip,
    _In_opt_ DWRITE_MATRIX const* transform,
    BOOL useGdiNatural,
    BOOL isSideways,
    BOOL isRightToLeft,
    _In_ DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis,
    _In_opt_z_ WCHAR const* localeName,
    _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features,
    _In_reads_opt_(featureRanges) UINT32 const* featureRangeLengths,
    UINT32 featureRanges,
    _Out_writes_(glyphCount) FLOAT* glyphAdvances,
    _Out_writes_(glyphCount) DWRITE_GLYPH_OFFSET* glyphOffsets)
{
    calltrace::AutoTrace trace(L"DWriteTextAnalyzerHook::GetGdiCompatibleGlyphPlacements");
    if (_GetGdiCompatibleGlyphPlacements_pre_ptr._value) { (this->*_GetGdiCompatibleGlyphPlacements_pre_ptr._value)(textString, clusterMap, textProps, textLength, glyphIndices, glyphProps, glyphCount, fontFace, fontEmSize, pixelsPerDip, transform, useGdiNatural, isSideways, isRightToLeft, scriptAnalysis, localeName, features, featureRangeLengths, featureRanges, glyphAdvances, glyphOffsets); }
    HRESULT ret = GetRealObj()->GetGdiCompatibleGlyphPlacements(textString, clusterMap, textProps, textLength, glyphIndices, glyphProps, glyphCount, fontFace, fontEmSize, pixelsPerDip, transform, useGdiNatural, isSideways, isRightToLeft, scriptAnalysis, localeName, features, featureRangeLengths, featureRanges, glyphAdvances, glyphOffsets);
    if (_GetGdiCompatibleGlyphPlacements_post_ptr._value) { (this->*_GetGdiCompatibleGlyphPlacements_post_ptr._value)(ret, textString, clusterMap, textProps, textLength, glyphIndices, glyphProps, glyphCount, fontFace, fontEmSize, pixelsPerDip, transform, useGdiNatural, isSideways, isRightToLeft, scriptAnalysis, localeName, features, featureRangeLengths, featureRanges, glyphAdvances, glyphOffsets); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteInlineObjectHook::Draw(
    _In_opt_ void* clientDrawingContext,
    IDWriteTextRenderer* renderer,
    FLOAT originX,
    FLOAT originY,
    BOOL isSideways,
    BOOL isRightToLeft,
    _In_opt_ IUnknown* clientDrawingEffect)
{
    calltrace::AutoTrace trace(L"DWriteInlineObjectHook::Draw");
    if (_Draw_pre_ptr._value) { (this->*_Draw_pre_ptr._value)(clientDrawingContext, renderer, originX, originY, isSideways, isRightToLeft, clientDrawingEffect); }
    HRESULT ret = GetRealObj()->Draw(clientDrawingContext, renderer, originX, originY, isSideways, isRightToLeft, clientDrawingEffect);
    if (_Draw_post_ptr._value) { (this->*_Draw_post_ptr._value)(ret, clientDrawingContext, renderer, originX, originY, isSideways, isRightToLeft, clientDrawingEffect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteInlineObjectHook::GetMetrics(
    _Out_ DWRITE_INLINE_OBJECT_METRICS* metrics)
{
    calltrace::AutoTrace trace(L"DWriteInlineObjectHook::GetMetrics");
    if (_GetMetrics_pre_ptr._value) { (this->*_GetMetrics_pre_ptr._value)(metrics); }
    HRESULT ret = GetRealObj()->GetMetrics(metrics);
    if (_GetMetrics_post_ptr._value) { (this->*_GetMetrics_post_ptr._value)(ret, metrics); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteInlineObjectHook::GetOverhangMetrics(
    _Out_ DWRITE_OVERHANG_METRICS* overhangs)
{
    calltrace::AutoTrace trace(L"DWriteInlineObjectHook::GetOverhangMetrics");
    if (_GetOverhangMetrics_pre_ptr._value) { (this->*_GetOverhangMetrics_pre_ptr._value)(overhangs); }
    HRESULT ret = GetRealObj()->GetOverhangMetrics(overhangs);
    if (_GetOverhangMetrics_post_ptr._value) { (this->*_GetOverhangMetrics_post_ptr._value)(ret, overhangs); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteInlineObjectHook::GetBreakConditions(
    _Out_ DWRITE_BREAK_CONDITION* breakConditionBefore,
    _Out_ DWRITE_BREAK_CONDITION* breakConditionAfter)
{
    calltrace::AutoTrace trace(L"DWriteInlineObjectHook::GetBreakConditions");
    if (_GetBreakConditions_pre_ptr._value) { (this->*_GetBreakConditions_pre_ptr._value)(breakConditionBefore, breakConditionAfter); }
    HRESULT ret = GetRealObj()->GetBreakConditions(breakConditionBefore, breakConditionAfter);
    if (_GetBreakConditions_post_ptr._value) { (this->*_GetBreakConditions_post_ptr._value)(ret, breakConditionBefore, breakConditionAfter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWritePixelSnappingHook::IsPixelSnappingDisabled(
    _In_opt_ void* clientDrawingContext,
    _Out_ BOOL* isDisabled)
{
    calltrace::AutoTrace trace(L"DWritePixelSnappingHook::IsPixelSnappingDisabled");
    if (_IsPixelSnappingDisabled_pre_ptr._value) { (this->*_IsPixelSnappingDisabled_pre_ptr._value)(clientDrawingContext, isDisabled); }
    HRESULT ret = GetRealObj()->IsPixelSnappingDisabled(clientDrawingContext, isDisabled);
    if (_IsPixelSnappingDisabled_post_ptr._value) { (this->*_IsPixelSnappingDisabled_post_ptr._value)(ret, clientDrawingContext, isDisabled); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWritePixelSnappingHook::GetCurrentTransform(
    _In_opt_ void* clientDrawingContext,
    _Out_ DWRITE_MATRIX* transform)
{
    calltrace::AutoTrace trace(L"DWritePixelSnappingHook::GetCurrentTransform");
    if (_GetCurrentTransform_pre_ptr._value) { (this->*_GetCurrentTransform_pre_ptr._value)(clientDrawingContext, transform); }
    HRESULT ret = GetRealObj()->GetCurrentTransform(clientDrawingContext, transform);
    if (_GetCurrentTransform_post_ptr._value) { (this->*_GetCurrentTransform_post_ptr._value)(ret, clientDrawingContext, transform); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWritePixelSnappingHook::GetPixelsPerDip(
    _In_opt_ void* clientDrawingContext,
    _Out_ FLOAT* pixelsPerDip)
{
    calltrace::AutoTrace trace(L"DWritePixelSnappingHook::GetPixelsPerDip");
    if (_GetPixelsPerDip_pre_ptr._value) { (this->*_GetPixelsPerDip_pre_ptr._value)(clientDrawingContext, pixelsPerDip); }
    HRESULT ret = GetRealObj()->GetPixelsPerDip(clientDrawingContext, pixelsPerDip);
    if (_GetPixelsPerDip_post_ptr._value) { (this->*_GetPixelsPerDip_post_ptr._value)(ret, clientDrawingContext, pixelsPerDip); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextRendererHook::DrawGlyphRun(
    _In_opt_ void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    DWRITE_MEASURING_MODE measuringMode,
    _In_ DWRITE_GLYPH_RUN const* glyphRun,
    _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
    _In_opt_ IUnknown* clientDrawingEffect)
{
    calltrace::AutoTrace trace(L"DWriteTextRendererHook::DrawGlyphRun");
    if (_DrawGlyphRun_pre_ptr._value) { (this->*_DrawGlyphRun_pre_ptr._value)(clientDrawingContext, baselineOriginX, baselineOriginY, measuringMode, glyphRun, glyphRunDescription, clientDrawingEffect); }
    HRESULT ret = GetRealObj()->DrawGlyphRun(clientDrawingContext, baselineOriginX, baselineOriginY, measuringMode, glyphRun, glyphRunDescription, clientDrawingEffect);
    if (_DrawGlyphRun_post_ptr._value) { (this->*_DrawGlyphRun_post_ptr._value)(ret, clientDrawingContext, baselineOriginX, baselineOriginY, measuringMode, glyphRun, glyphRunDescription, clientDrawingEffect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextRendererHook::DrawUnderline(
    _In_opt_ void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    _In_ DWRITE_UNDERLINE const* underline,
    _In_opt_ IUnknown* clientDrawingEffect)
{
    calltrace::AutoTrace trace(L"DWriteTextRendererHook::DrawUnderline");
    if (_DrawUnderline_pre_ptr._value) { (this->*_DrawUnderline_pre_ptr._value)(clientDrawingContext, baselineOriginX, baselineOriginY, underline, clientDrawingEffect); }
    HRESULT ret = GetRealObj()->DrawUnderline(clientDrawingContext, baselineOriginX, baselineOriginY, underline, clientDrawingEffect);
    if (_DrawUnderline_post_ptr._value) { (this->*_DrawUnderline_post_ptr._value)(ret, clientDrawingContext, baselineOriginX, baselineOriginY, underline, clientDrawingEffect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextRendererHook::DrawStrikethrough(
    _In_opt_ void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    _In_ DWRITE_STRIKETHROUGH const* strikethrough,
    _In_opt_ IUnknown* clientDrawingEffect)
{
    calltrace::AutoTrace trace(L"DWriteTextRendererHook::DrawStrikethrough");
    if (_DrawStrikethrough_pre_ptr._value) { (this->*_DrawStrikethrough_pre_ptr._value)(clientDrawingContext, baselineOriginX, baselineOriginY, strikethrough, clientDrawingEffect); }
    HRESULT ret = GetRealObj()->DrawStrikethrough(clientDrawingContext, baselineOriginX, baselineOriginY, strikethrough, clientDrawingEffect);
    if (_DrawStrikethrough_post_ptr._value) { (this->*_DrawStrikethrough_post_ptr._value)(ret, clientDrawingContext, baselineOriginX, baselineOriginY, strikethrough, clientDrawingEffect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextRendererHook::DrawInlineObject(
    _In_opt_ void* clientDrawingContext,
    FLOAT originX,
    FLOAT originY,
    IDWriteInlineObject* inlineObject,
    BOOL isSideways,
    BOOL isRightToLeft,
    _In_opt_ IUnknown* clientDrawingEffect)
{
    calltrace::AutoTrace trace(L"DWriteTextRendererHook::DrawInlineObject");
    if (_DrawInlineObject_pre_ptr._value) { (this->*_DrawInlineObject_pre_ptr._value)(clientDrawingContext, originX, originY, inlineObject, isSideways, isRightToLeft, clientDrawingEffect); }
    HRESULT ret = GetRealObj()->DrawInlineObject(clientDrawingContext, originX, originY, inlineObject, isSideways, isRightToLeft, clientDrawingEffect);
    if (_DrawInlineObject_post_ptr._value) { (this->*_DrawInlineObject_post_ptr._value)(ret, clientDrawingContext, originX, originY, inlineObject, isSideways, isRightToLeft, clientDrawingEffect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetMaxWidth(
    FLOAT maxWidth)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetMaxWidth");
    if (_SetMaxWidth_pre_ptr._value) { (this->*_SetMaxWidth_pre_ptr._value)(maxWidth); }
    HRESULT ret = GetRealObj()->SetMaxWidth(maxWidth);
    if (_SetMaxWidth_post_ptr._value) { (this->*_SetMaxWidth_post_ptr._value)(ret, maxWidth); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetMaxHeight(
    FLOAT maxHeight)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetMaxHeight");
    if (_SetMaxHeight_pre_ptr._value) { (this->*_SetMaxHeight_pre_ptr._value)(maxHeight); }
    HRESULT ret = GetRealObj()->SetMaxHeight(maxHeight);
    if (_SetMaxHeight_post_ptr._value) { (this->*_SetMaxHeight_post_ptr._value)(ret, maxHeight); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetFontCollection(
    IDWriteFontCollection* fontCollection,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetFontCollection");
    if (_SetFontCollection_pre_ptr._value) { (this->*_SetFontCollection_pre_ptr._value)(fontCollection, textRange); }
    HRESULT ret = GetRealObj()->SetFontCollection(fontCollection, textRange);
    if (_SetFontCollection_post_ptr._value) { (this->*_SetFontCollection_post_ptr._value)(ret, fontCollection, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetFontFamilyName(
    _In_z_ WCHAR const* fontFamilyName,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetFontFamilyName");
    if (_SetFontFamilyName_pre_ptr._value) { (this->*_SetFontFamilyName_pre_ptr._value)(fontFamilyName, textRange); }
    HRESULT ret = GetRealObj()->SetFontFamilyName(fontFamilyName, textRange);
    if (_SetFontFamilyName_post_ptr._value) { (this->*_SetFontFamilyName_post_ptr._value)(ret, fontFamilyName, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetFontWeight(
    DWRITE_FONT_WEIGHT fontWeight,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetFontWeight");
    if (_SetFontWeight_pre_ptr._value) { (this->*_SetFontWeight_pre_ptr._value)(fontWeight, textRange); }
    HRESULT ret = GetRealObj()->SetFontWeight(fontWeight, textRange);
    if (_SetFontWeight_post_ptr._value) { (this->*_SetFontWeight_post_ptr._value)(ret, fontWeight, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetFontStyle(
    DWRITE_FONT_STYLE fontStyle,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetFontStyle");
    if (_SetFontStyle_pre_ptr._value) { (this->*_SetFontStyle_pre_ptr._value)(fontStyle, textRange); }
    HRESULT ret = GetRealObj()->SetFontStyle(fontStyle, textRange);
    if (_SetFontStyle_post_ptr._value) { (this->*_SetFontStyle_post_ptr._value)(ret, fontStyle, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetFontStretch(
    DWRITE_FONT_STRETCH fontStretch,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetFontStretch");
    if (_SetFontStretch_pre_ptr._value) { (this->*_SetFontStretch_pre_ptr._value)(fontStretch, textRange); }
    HRESULT ret = GetRealObj()->SetFontStretch(fontStretch, textRange);
    if (_SetFontStretch_post_ptr._value) { (this->*_SetFontStretch_post_ptr._value)(ret, fontStretch, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetFontSize(
    FLOAT fontSize,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetFontSize");
    if (_SetFontSize_pre_ptr._value) { (this->*_SetFontSize_pre_ptr._value)(fontSize, textRange); }
    HRESULT ret = GetRealObj()->SetFontSize(fontSize, textRange);
    if (_SetFontSize_post_ptr._value) { (this->*_SetFontSize_post_ptr._value)(ret, fontSize, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetUnderline(
    BOOL hasUnderline,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetUnderline");
    if (_SetUnderline_pre_ptr._value) { (this->*_SetUnderline_pre_ptr._value)(hasUnderline, textRange); }
    HRESULT ret = GetRealObj()->SetUnderline(hasUnderline, textRange);
    if (_SetUnderline_post_ptr._value) { (this->*_SetUnderline_post_ptr._value)(ret, hasUnderline, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetStrikethrough(
    BOOL hasStrikethrough,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetStrikethrough");
    if (_SetStrikethrough_pre_ptr._value) { (this->*_SetStrikethrough_pre_ptr._value)(hasStrikethrough, textRange); }
    HRESULT ret = GetRealObj()->SetStrikethrough(hasStrikethrough, textRange);
    if (_SetStrikethrough_post_ptr._value) { (this->*_SetStrikethrough_post_ptr._value)(ret, hasStrikethrough, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetDrawingEffect(
    IUnknown* drawingEffect,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetDrawingEffect");
    if (_SetDrawingEffect_pre_ptr._value) { (this->*_SetDrawingEffect_pre_ptr._value)(drawingEffect, textRange); }
    HRESULT ret = GetRealObj()->SetDrawingEffect(drawingEffect, textRange);
    if (_SetDrawingEffect_post_ptr._value) { (this->*_SetDrawingEffect_post_ptr._value)(ret, drawingEffect, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetInlineObject(
    IDWriteInlineObject* inlineObject,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetInlineObject");
    if (_SetInlineObject_pre_ptr._value) { (this->*_SetInlineObject_pre_ptr._value)(inlineObject, textRange); }
    HRESULT ret = GetRealObj()->SetInlineObject(inlineObject, textRange);
    if (_SetInlineObject_post_ptr._value) { (this->*_SetInlineObject_post_ptr._value)(ret, inlineObject, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetTypography(
    IDWriteTypography* typography,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetTypography");
    if (_SetTypography_pre_ptr._value) { (this->*_SetTypography_pre_ptr._value)(typography, textRange); }
    HRESULT ret = GetRealObj()->SetTypography(typography, textRange);
    if (_SetTypography_post_ptr._value) { (this->*_SetTypography_post_ptr._value)(ret, typography, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::SetLocaleName(
    _In_z_ WCHAR const* localeName,
    DWRITE_TEXT_RANGE textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::SetLocaleName");
    if (_SetLocaleName_pre_ptr._value) { (this->*_SetLocaleName_pre_ptr._value)(localeName, textRange); }
    HRESULT ret = GetRealObj()->SetLocaleName(localeName, textRange);
    if (_SetLocaleName_post_ptr._value) { (this->*_SetLocaleName_post_ptr._value)(ret, localeName, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE DWriteTextLayoutHook::GetMaxWidth()
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetMaxWidth");
    if (_GetMaxWidth_pre_ptr._value) { (this->*_GetMaxWidth_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetMaxWidth();
    if (_GetMaxWidth_post_ptr._value) { (this->*_GetMaxWidth_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE DWriteTextLayoutHook::GetMaxHeight()
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetMaxHeight");
    if (_GetMaxHeight_pre_ptr._value) { (this->*_GetMaxHeight_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetMaxHeight();
    if (_GetMaxHeight_post_ptr._value) { (this->*_GetMaxHeight_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetFontCollection(
    UINT32 currentPosition,
    _Out_ IDWriteFontCollection** fontCollection,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetFontCollection");
    if (_GetFontCollection_pre_ptr._value) { (this->*_GetFontCollection_pre_ptr._value)(currentPosition, fontCollection, textRange); }
    HRESULT ret = GetRealObj()->GetFontCollection(currentPosition, fontCollection, textRange);
    if ( fontCollection && *fontCollection) { *fontCollection = RealToHooked_D2D( *fontCollection ); }
    if (_GetFontCollection_post_ptr._value) { (this->*_GetFontCollection_post_ptr._value)(ret, currentPosition, fontCollection, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetFontFamilyNameLength(
    UINT32 currentPosition,
    _Out_ UINT32* nameLength,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetFontFamilyNameLength");
    if (_GetFontFamilyNameLength_pre_ptr._value) { (this->*_GetFontFamilyNameLength_pre_ptr._value)(currentPosition, nameLength, textRange); }
    HRESULT ret = GetRealObj()->GetFontFamilyNameLength(currentPosition, nameLength, textRange);
    if (_GetFontFamilyNameLength_post_ptr._value) { (this->*_GetFontFamilyNameLength_post_ptr._value)(ret, currentPosition, nameLength, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetFontFamilyName(
    UINT32 currentPosition,
    _Out_writes_z_(nameSize) WCHAR* fontFamilyName,
    UINT32 nameSize,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetFontFamilyName");
    if (_GetFontFamilyName_pre_ptr._value) { (this->*_GetFontFamilyName_pre_ptr._value)(currentPosition, fontFamilyName, nameSize, textRange); }
    HRESULT ret = GetRealObj()->GetFontFamilyName(currentPosition, fontFamilyName, nameSize, textRange);
    if (_GetFontFamilyName_post_ptr._value) { (this->*_GetFontFamilyName_post_ptr._value)(ret, currentPosition, fontFamilyName, nameSize, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetFontWeight(
    UINT32 currentPosition,
    _Out_ DWRITE_FONT_WEIGHT* fontWeight,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetFontWeight");
    if (_GetFontWeight_pre_ptr._value) { (this->*_GetFontWeight_pre_ptr._value)(currentPosition, fontWeight, textRange); }
    HRESULT ret = GetRealObj()->GetFontWeight(currentPosition, fontWeight, textRange);
    if (_GetFontWeight_post_ptr._value) { (this->*_GetFontWeight_post_ptr._value)(ret, currentPosition, fontWeight, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetFontStyle(
    UINT32 currentPosition,
    _Out_ DWRITE_FONT_STYLE* fontStyle,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetFontStyle");
    if (_GetFontStyle_pre_ptr._value) { (this->*_GetFontStyle_pre_ptr._value)(currentPosition, fontStyle, textRange); }
    HRESULT ret = GetRealObj()->GetFontStyle(currentPosition, fontStyle, textRange);
    if (_GetFontStyle_post_ptr._value) { (this->*_GetFontStyle_post_ptr._value)(ret, currentPosition, fontStyle, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetFontStretch(
    UINT32 currentPosition,
    _Out_ DWRITE_FONT_STRETCH* fontStretch,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetFontStretch");
    if (_GetFontStretch_pre_ptr._value) { (this->*_GetFontStretch_pre_ptr._value)(currentPosition, fontStretch, textRange); }
    HRESULT ret = GetRealObj()->GetFontStretch(currentPosition, fontStretch, textRange);
    if (_GetFontStretch_post_ptr._value) { (this->*_GetFontStretch_post_ptr._value)(ret, currentPosition, fontStretch, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetFontSize(
    UINT32 currentPosition,
    _Out_ FLOAT* fontSize,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetFontSize");
    if (_GetFontSize_pre_ptr._value) { (this->*_GetFontSize_pre_ptr._value)(currentPosition, fontSize, textRange); }
    HRESULT ret = GetRealObj()->GetFontSize(currentPosition, fontSize, textRange);
    if (_GetFontSize_post_ptr._value) { (this->*_GetFontSize_post_ptr._value)(ret, currentPosition, fontSize, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetUnderline(
    UINT32 currentPosition,
    _Out_ BOOL* hasUnderline,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetUnderline");
    if (_GetUnderline_pre_ptr._value) { (this->*_GetUnderline_pre_ptr._value)(currentPosition, hasUnderline, textRange); }
    HRESULT ret = GetRealObj()->GetUnderline(currentPosition, hasUnderline, textRange);
    if (_GetUnderline_post_ptr._value) { (this->*_GetUnderline_post_ptr._value)(ret, currentPosition, hasUnderline, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetStrikethrough(
    UINT32 currentPosition,
    _Out_ BOOL* hasStrikethrough,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetStrikethrough");
    if (_GetStrikethrough_pre_ptr._value) { (this->*_GetStrikethrough_pre_ptr._value)(currentPosition, hasStrikethrough, textRange); }
    HRESULT ret = GetRealObj()->GetStrikethrough(currentPosition, hasStrikethrough, textRange);
    if (_GetStrikethrough_post_ptr._value) { (this->*_GetStrikethrough_post_ptr._value)(ret, currentPosition, hasStrikethrough, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetDrawingEffect(
    UINT32 currentPosition,
    _Out_ IUnknown** drawingEffect,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetDrawingEffect");
    if (_GetDrawingEffect_pre_ptr._value) { (this->*_GetDrawingEffect_pre_ptr._value)(currentPosition, drawingEffect, textRange); }
    HRESULT ret = GetRealObj()->GetDrawingEffect(currentPosition, drawingEffect, textRange);
    if (_GetDrawingEffect_post_ptr._value) { (this->*_GetDrawingEffect_post_ptr._value)(ret, currentPosition, drawingEffect, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetInlineObject(
    UINT32 currentPosition,
    _Out_ IDWriteInlineObject** inlineObject,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetInlineObject");
    if (_GetInlineObject_pre_ptr._value) { (this->*_GetInlineObject_pre_ptr._value)(currentPosition, inlineObject, textRange); }
    HRESULT ret = GetRealObj()->GetInlineObject(currentPosition, inlineObject, textRange);
    if ( inlineObject && *inlineObject) { *inlineObject = RealToHooked_D2D( *inlineObject ); }
    if (_GetInlineObject_post_ptr._value) { (this->*_GetInlineObject_post_ptr._value)(ret, currentPosition, inlineObject, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetTypography(
    UINT32 currentPosition,
    _Out_ IDWriteTypography** typography,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetTypography");
    if (_GetTypography_pre_ptr._value) { (this->*_GetTypography_pre_ptr._value)(currentPosition, typography, textRange); }
    HRESULT ret = GetRealObj()->GetTypography(currentPosition, typography, textRange);
    if ( typography && *typography) { *typography = RealToHooked_D2D( *typography ); }
    if (_GetTypography_post_ptr._value) { (this->*_GetTypography_post_ptr._value)(ret, currentPosition, typography, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetLocaleNameLength(
    UINT32 currentPosition,
    _Out_ UINT32* nameLength,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetLocaleNameLength");
    if (_GetLocaleNameLength_pre_ptr._value) { (this->*_GetLocaleNameLength_pre_ptr._value)(currentPosition, nameLength, textRange); }
    HRESULT ret = GetRealObj()->GetLocaleNameLength(currentPosition, nameLength, textRange);
    if (_GetLocaleNameLength_post_ptr._value) { (this->*_GetLocaleNameLength_post_ptr._value)(ret, currentPosition, nameLength, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetLocaleName(
    UINT32 currentPosition,
    _Out_writes_z_(nameSize) WCHAR* localeName,
    UINT32 nameSize,
    _Out_opt_ DWRITE_TEXT_RANGE* textRange)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetLocaleName");
    if (_GetLocaleName_pre_ptr._value) { (this->*_GetLocaleName_pre_ptr._value)(currentPosition, localeName, nameSize, textRange); }
    HRESULT ret = GetRealObj()->GetLocaleName(currentPosition, localeName, nameSize, textRange);
    if (_GetLocaleName_post_ptr._value) { (this->*_GetLocaleName_post_ptr._value)(ret, currentPosition, localeName, nameSize, textRange); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::Draw(
    _In_opt_ void* clientDrawingContext,
    IDWriteTextRenderer* renderer,
    FLOAT originX,
    FLOAT originY)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::Draw");
    if (_Draw_pre_ptr._value) { (this->*_Draw_pre_ptr._value)(clientDrawingContext, renderer, originX, originY); }
    HRESULT ret = GetRealObj()->Draw(clientDrawingContext, renderer, originX, originY);
    if (_Draw_post_ptr._value) { (this->*_Draw_post_ptr._value)(ret, clientDrawingContext, renderer, originX, originY); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetLineMetrics(
    _Out_writes_opt_(maxLineCount) DWRITE_LINE_METRICS* lineMetrics,
    UINT32 maxLineCount,
    _Out_ UINT32* actualLineCount)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetLineMetrics");
    if (_GetLineMetrics_pre_ptr._value) { (this->*_GetLineMetrics_pre_ptr._value)(lineMetrics, maxLineCount, actualLineCount); }
    HRESULT ret = GetRealObj()->GetLineMetrics(lineMetrics, maxLineCount, actualLineCount);
    if (_GetLineMetrics_post_ptr._value) { (this->*_GetLineMetrics_post_ptr._value)(ret, lineMetrics, maxLineCount, actualLineCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetMetrics(
    _Out_ DWRITE_TEXT_METRICS* textMetrics)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetMetrics");
    if (_GetMetrics_pre_ptr._value) { (this->*_GetMetrics_pre_ptr._value)(textMetrics); }
    HRESULT ret = GetRealObj()->GetMetrics(textMetrics);
    if (_GetMetrics_post_ptr._value) { (this->*_GetMetrics_post_ptr._value)(ret, textMetrics); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetOverhangMetrics(
    _Out_ DWRITE_OVERHANG_METRICS* overhangs)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetOverhangMetrics");
    if (_GetOverhangMetrics_pre_ptr._value) { (this->*_GetOverhangMetrics_pre_ptr._value)(overhangs); }
    HRESULT ret = GetRealObj()->GetOverhangMetrics(overhangs);
    if (_GetOverhangMetrics_post_ptr._value) { (this->*_GetOverhangMetrics_post_ptr._value)(ret, overhangs); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::GetClusterMetrics(
    _Out_writes_opt_(maxClusterCount) DWRITE_CLUSTER_METRICS* clusterMetrics,
    UINT32 maxClusterCount,
    _Out_ UINT32* actualClusterCount)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::GetClusterMetrics");
    if (_GetClusterMetrics_pre_ptr._value) { (this->*_GetClusterMetrics_pre_ptr._value)(clusterMetrics, maxClusterCount, actualClusterCount); }
    HRESULT ret = GetRealObj()->GetClusterMetrics(clusterMetrics, maxClusterCount, actualClusterCount);
    if (_GetClusterMetrics_post_ptr._value) { (this->*_GetClusterMetrics_post_ptr._value)(ret, clusterMetrics, maxClusterCount, actualClusterCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::DetermineMinWidth(
    _Out_ FLOAT* minWidth)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::DetermineMinWidth");
    if (_DetermineMinWidth_pre_ptr._value) { (this->*_DetermineMinWidth_pre_ptr._value)(minWidth); }
    HRESULT ret = GetRealObj()->DetermineMinWidth(minWidth);
    if (_DetermineMinWidth_post_ptr._value) { (this->*_DetermineMinWidth_post_ptr._value)(ret, minWidth); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::HitTestPoint(
    FLOAT pointX,
    FLOAT pointY,
    _Out_ BOOL* isTrailingHit,
    _Out_ BOOL* isInside,
    _Out_ DWRITE_HIT_TEST_METRICS* hitTestMetrics)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::HitTestPoint");
    if (_HitTestPoint_pre_ptr._value) { (this->*_HitTestPoint_pre_ptr._value)(pointX, pointY, isTrailingHit, isInside, hitTestMetrics); }
    HRESULT ret = GetRealObj()->HitTestPoint(pointX, pointY, isTrailingHit, isInside, hitTestMetrics);
    if (_HitTestPoint_post_ptr._value) { (this->*_HitTestPoint_post_ptr._value)(ret, pointX, pointY, isTrailingHit, isInside, hitTestMetrics); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::HitTestTextPosition(
    UINT32 textPosition,
    BOOL isTrailingHit,
    _Out_ FLOAT* pointX,
    _Out_ FLOAT* pointY,
    _Out_ DWRITE_HIT_TEST_METRICS* hitTestMetrics)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::HitTestTextPosition");
    if (_HitTestTextPosition_pre_ptr._value) { (this->*_HitTestTextPosition_pre_ptr._value)(textPosition, isTrailingHit, pointX, pointY, hitTestMetrics); }
    HRESULT ret = GetRealObj()->HitTestTextPosition(textPosition, isTrailingHit, pointX, pointY, hitTestMetrics);
    if (_HitTestTextPosition_post_ptr._value) { (this->*_HitTestTextPosition_post_ptr._value)(ret, textPosition, isTrailingHit, pointX, pointY, hitTestMetrics); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteTextLayoutHook::HitTestTextRange(
    UINT32 textPosition,
    UINT32 textLength,
    FLOAT originX,
    FLOAT originY,
    _Out_writes_opt_(maxHitTestMetricsCount) DWRITE_HIT_TEST_METRICS* hitTestMetrics,
    UINT32 maxHitTestMetricsCount,
    _Out_ UINT32* actualHitTestMetricsCount)
{
    calltrace::AutoTrace trace(L"DWriteTextLayoutHook::HitTestTextRange");
    if (_HitTestTextRange_pre_ptr._value) { (this->*_HitTestTextRange_pre_ptr._value)(textPosition, textLength, originX, originY, hitTestMetrics, maxHitTestMetricsCount, actualHitTestMetricsCount); }
    HRESULT ret = GetRealObj()->HitTestTextRange(textPosition, textLength, originX, originY, hitTestMetrics, maxHitTestMetricsCount, actualHitTestMetricsCount);
    if (_HitTestTextRange_post_ptr._value) { (this->*_HitTestTextRange_post_ptr._value)(ret, textPosition, textLength, originX, originY, hitTestMetrics, maxHitTestMetricsCount, actualHitTestMetricsCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteBitmapRenderTargetHook::DrawGlyphRun(
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    DWRITE_MEASURING_MODE measuringMode,
    _In_ DWRITE_GLYPH_RUN const* glyphRun,
    IDWriteRenderingParams* renderingParams,
    COLORREF textColor,
    _Out_opt_ RECT* blackBoxRect)
{
    calltrace::AutoTrace trace(L"DWriteBitmapRenderTargetHook::DrawGlyphRun");
    if (_DrawGlyphRun_pre_ptr._value) { (this->*_DrawGlyphRun_pre_ptr._value)(baselineOriginX, baselineOriginY, measuringMode, glyphRun, renderingParams, textColor, blackBoxRect); }
    HRESULT ret = GetRealObj()->DrawGlyphRun(baselineOriginX, baselineOriginY, measuringMode, glyphRun, renderingParams, textColor, blackBoxRect);
    if (_DrawGlyphRun_post_ptr._value) { (this->*_DrawGlyphRun_post_ptr._value)(ret, baselineOriginX, baselineOriginY, measuringMode, glyphRun, renderingParams, textColor, blackBoxRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HDC STDMETHODCALLTYPE DWriteBitmapRenderTargetHook::GetMemoryDC()
{
    calltrace::AutoTrace trace(L"DWriteBitmapRenderTargetHook::GetMemoryDC");
    if (_GetMemoryDC_pre_ptr._value) { (this->*_GetMemoryDC_pre_ptr._value)(); }
    HDC ret = GetRealObj()->GetMemoryDC();
    if (_GetMemoryDC_post_ptr._value) { (this->*_GetMemoryDC_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE DWriteBitmapRenderTargetHook::GetPixelsPerDip()
{
    calltrace::AutoTrace trace(L"DWriteBitmapRenderTargetHook::GetPixelsPerDip");
    if (_GetPixelsPerDip_pre_ptr._value) { (this->*_GetPixelsPerDip_pre_ptr._value)(); }
    FLOAT ret = GetRealObj()->GetPixelsPerDip();
    if (_GetPixelsPerDip_post_ptr._value) { (this->*_GetPixelsPerDip_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteBitmapRenderTargetHook::SetPixelsPerDip(
    FLOAT pixelsPerDip)
{
    calltrace::AutoTrace trace(L"DWriteBitmapRenderTargetHook::SetPixelsPerDip");
    if (_SetPixelsPerDip_pre_ptr._value) { (this->*_SetPixelsPerDip_pre_ptr._value)(pixelsPerDip); }
    HRESULT ret = GetRealObj()->SetPixelsPerDip(pixelsPerDip);
    if (_SetPixelsPerDip_post_ptr._value) { (this->*_SetPixelsPerDip_post_ptr._value)(ret, pixelsPerDip); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteBitmapRenderTargetHook::GetCurrentTransform(
    _Out_ DWRITE_MATRIX* transform)
{
    calltrace::AutoTrace trace(L"DWriteBitmapRenderTargetHook::GetCurrentTransform");
    if (_GetCurrentTransform_pre_ptr._value) { (this->*_GetCurrentTransform_pre_ptr._value)(transform); }
    HRESULT ret = GetRealObj()->GetCurrentTransform(transform);
    if (_GetCurrentTransform_post_ptr._value) { (this->*_GetCurrentTransform_post_ptr._value)(ret, transform); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteBitmapRenderTargetHook::SetCurrentTransform(
    _In_opt_ DWRITE_MATRIX const* transform)
{
    calltrace::AutoTrace trace(L"DWriteBitmapRenderTargetHook::SetCurrentTransform");
    if (_SetCurrentTransform_pre_ptr._value) { (this->*_SetCurrentTransform_pre_ptr._value)(transform); }
    HRESULT ret = GetRealObj()->SetCurrentTransform(transform);
    if (_SetCurrentTransform_post_ptr._value) { (this->*_SetCurrentTransform_post_ptr._value)(ret, transform); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteBitmapRenderTargetHook::GetSize(
    _Out_ SIZE* size)
{
    calltrace::AutoTrace trace(L"DWriteBitmapRenderTargetHook::GetSize");
    if (_GetSize_pre_ptr._value) { (this->*_GetSize_pre_ptr._value)(size); }
    HRESULT ret = GetRealObj()->GetSize(size);
    if (_GetSize_post_ptr._value) { (this->*_GetSize_post_ptr._value)(ret, size); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteBitmapRenderTargetHook::Resize(
    UINT32 width,
    UINT32 height)
{
    calltrace::AutoTrace trace(L"DWriteBitmapRenderTargetHook::Resize");
    if (_Resize_pre_ptr._value) { (this->*_Resize_pre_ptr._value)(width, height); }
    HRESULT ret = GetRealObj()->Resize(width, height);
    if (_Resize_post_ptr._value) { (this->*_Resize_post_ptr._value)(ret, width, height); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteGdiInteropHook::CreateFontFromLOGFONT(
    _In_ LOGFONTW const* logFont,
    _Out_ IDWriteFont** font)
{
    calltrace::AutoTrace trace(L"DWriteGdiInteropHook::CreateFontFromLOGFONT");
    if (_CreateFontFromLOGFONT_pre_ptr._value) { (this->*_CreateFontFromLOGFONT_pre_ptr._value)(logFont, font); }
    HRESULT ret = GetRealObj()->CreateFontFromLOGFONT(logFont, font);
    if ( font && *font) { *font = RealToHooked_D2D( *font ); }
    if (_CreateFontFromLOGFONT_post_ptr._value) { (this->*_CreateFontFromLOGFONT_post_ptr._value)(ret, logFont, font); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteGdiInteropHook::ConvertFontToLOGFONT(
    IDWriteFont* font,
    _Out_ LOGFONTW* logFont,
    _Out_ BOOL* isSystemFont)
{
    calltrace::AutoTrace trace(L"DWriteGdiInteropHook::ConvertFontToLOGFONT");
    if (_ConvertFontToLOGFONT_pre_ptr._value) { (this->*_ConvertFontToLOGFONT_pre_ptr._value)(font, logFont, isSystemFont); }
    HRESULT ret = GetRealObj()->ConvertFontToLOGFONT(font, logFont, isSystemFont);
    if (_ConvertFontToLOGFONT_post_ptr._value) { (this->*_ConvertFontToLOGFONT_post_ptr._value)(ret, font, logFont, isSystemFont); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteGdiInteropHook::ConvertFontFaceToLOGFONT(
    IDWriteFontFace* font,
    _Out_ LOGFONTW* logFont)
{
    calltrace::AutoTrace trace(L"DWriteGdiInteropHook::ConvertFontFaceToLOGFONT");
    if (_ConvertFontFaceToLOGFONT_pre_ptr._value) { (this->*_ConvertFontFaceToLOGFONT_pre_ptr._value)(font, logFont); }
    HRESULT ret = GetRealObj()->ConvertFontFaceToLOGFONT(font, logFont);
    if (_ConvertFontFaceToLOGFONT_post_ptr._value) { (this->*_ConvertFontFaceToLOGFONT_post_ptr._value)(ret, font, logFont); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteGdiInteropHook::CreateFontFaceFromHdc(
    HDC hdc,
    _Out_ IDWriteFontFace** fontFace)
{
    calltrace::AutoTrace trace(L"DWriteGdiInteropHook::CreateFontFaceFromHdc");
    if (_CreateFontFaceFromHdc_pre_ptr._value) { (this->*_CreateFontFaceFromHdc_pre_ptr._value)(hdc, fontFace); }
    HRESULT ret = GetRealObj()->CreateFontFaceFromHdc(hdc, fontFace);
    if ( fontFace && *fontFace) { *fontFace = RealToHooked_D2D( *fontFace ); }
    if (_CreateFontFaceFromHdc_post_ptr._value) { (this->*_CreateFontFaceFromHdc_post_ptr._value)(ret, hdc, fontFace); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteGdiInteropHook::CreateBitmapRenderTarget(
    _In_opt_ HDC hdc,
    UINT32 width,
    UINT32 height,
    _Out_ IDWriteBitmapRenderTarget** renderTarget)
{
    calltrace::AutoTrace trace(L"DWriteGdiInteropHook::CreateBitmapRenderTarget");
    if (_CreateBitmapRenderTarget_pre_ptr._value) { (this->*_CreateBitmapRenderTarget_pre_ptr._value)(hdc, width, height, renderTarget); }
    HRESULT ret = GetRealObj()->CreateBitmapRenderTarget(hdc, width, height, renderTarget);
    if ( renderTarget && *renderTarget) { *renderTarget = RealToHooked_D2D( *renderTarget ); }
    if (_CreateBitmapRenderTarget_post_ptr._value) { (this->*_CreateBitmapRenderTarget_post_ptr._value)(ret, hdc, width, height, renderTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteGlyphRunAnalysisHook::GetAlphaTextureBounds(
    DWRITE_TEXTURE_TYPE textureType,
    _Out_ RECT* textureBounds)
{
    calltrace::AutoTrace trace(L"DWriteGlyphRunAnalysisHook::GetAlphaTextureBounds");
    if (_GetAlphaTextureBounds_pre_ptr._value) { (this->*_GetAlphaTextureBounds_pre_ptr._value)(textureType, textureBounds); }
    HRESULT ret = GetRealObj()->GetAlphaTextureBounds(textureType, textureBounds);
    if (_GetAlphaTextureBounds_post_ptr._value) { (this->*_GetAlphaTextureBounds_post_ptr._value)(ret, textureType, textureBounds); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteGlyphRunAnalysisHook::CreateAlphaTexture(
    DWRITE_TEXTURE_TYPE textureType,
    _In_ RECT const* textureBounds,
    _Out_writes_bytes_(bufferSize) BYTE* alphaValues,
    UINT32 bufferSize)
{
    calltrace::AutoTrace trace(L"DWriteGlyphRunAnalysisHook::CreateAlphaTexture");
    if (_CreateAlphaTexture_pre_ptr._value) { (this->*_CreateAlphaTexture_pre_ptr._value)(textureType, textureBounds, alphaValues, bufferSize); }
    HRESULT ret = GetRealObj()->CreateAlphaTexture(textureType, textureBounds, alphaValues, bufferSize);
    if (_CreateAlphaTexture_post_ptr._value) { (this->*_CreateAlphaTexture_post_ptr._value)(ret, textureType, textureBounds, alphaValues, bufferSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteGlyphRunAnalysisHook::GetAlphaBlendParams(
    IDWriteRenderingParams* renderingParams,
    _Out_ FLOAT* blendGamma,
    _Out_ FLOAT* blendEnhancedContrast,
    _Out_ FLOAT* blendClearTypeLevel)
{
    calltrace::AutoTrace trace(L"DWriteGlyphRunAnalysisHook::GetAlphaBlendParams");
    if (_GetAlphaBlendParams_pre_ptr._value) { (this->*_GetAlphaBlendParams_pre_ptr._value)(renderingParams, blendGamma, blendEnhancedContrast, blendClearTypeLevel); }
    HRESULT ret = GetRealObj()->GetAlphaBlendParams(renderingParams, blendGamma, blendEnhancedContrast, blendClearTypeLevel);
    if (_GetAlphaBlendParams_post_ptr._value) { (this->*_GetAlphaBlendParams_post_ptr._value)(ret, renderingParams, blendGamma, blendEnhancedContrast, blendClearTypeLevel); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::GetSystemFontCollection(
    _Out_ IDWriteFontCollection** fontCollection,
    BOOL checkForUpdates)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::GetSystemFontCollection");
    if (_GetSystemFontCollection_pre_ptr._value) { (this->*_GetSystemFontCollection_pre_ptr._value)(fontCollection, checkForUpdates); }
    HRESULT ret = GetRealObj()->GetSystemFontCollection(fontCollection, checkForUpdates);
    if ( fontCollection && *fontCollection) { *fontCollection = RealToHooked_D2D( *fontCollection ); }
    if (_GetSystemFontCollection_post_ptr._value) { (this->*_GetSystemFontCollection_post_ptr._value)(ret, fontCollection, checkForUpdates); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateCustomFontCollection(
    IDWriteFontCollectionLoader* collectionLoader,
    _In_reads_bytes_(collectionKeySize) void const* collectionKey,
    UINT32 collectionKeySize,
    _Out_ IDWriteFontCollection** fontCollection)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateCustomFontCollection");
    if (_CreateCustomFontCollection_pre_ptr._value) { (this->*_CreateCustomFontCollection_pre_ptr._value)(collectionLoader, collectionKey, collectionKeySize, fontCollection); }
    HRESULT ret = GetRealObj()->CreateCustomFontCollection(collectionLoader, collectionKey, collectionKeySize, fontCollection);
    if ( fontCollection && *fontCollection) { *fontCollection = RealToHooked_D2D( *fontCollection ); }
    if (_CreateCustomFontCollection_post_ptr._value) { (this->*_CreateCustomFontCollection_post_ptr._value)(ret, collectionLoader, collectionKey, collectionKeySize, fontCollection); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::RegisterFontCollectionLoader(
    IDWriteFontCollectionLoader* fontCollectionLoader)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::RegisterFontCollectionLoader");
    if (_RegisterFontCollectionLoader_pre_ptr._value) { (this->*_RegisterFontCollectionLoader_pre_ptr._value)(fontCollectionLoader); }
    HRESULT ret = GetRealObj()->RegisterFontCollectionLoader(fontCollectionLoader);
    if (_RegisterFontCollectionLoader_post_ptr._value) { (this->*_RegisterFontCollectionLoader_post_ptr._value)(ret, fontCollectionLoader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::UnregisterFontCollectionLoader(
    IDWriteFontCollectionLoader* fontCollectionLoader)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::UnregisterFontCollectionLoader");
    if (_UnregisterFontCollectionLoader_pre_ptr._value) { (this->*_UnregisterFontCollectionLoader_pre_ptr._value)(fontCollectionLoader); }
    HRESULT ret = GetRealObj()->UnregisterFontCollectionLoader(fontCollectionLoader);
    if (_UnregisterFontCollectionLoader_post_ptr._value) { (this->*_UnregisterFontCollectionLoader_post_ptr._value)(ret, fontCollectionLoader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateFontFileReference(
    _In_z_ WCHAR const* filePath,
    _In_opt_ FILETIME const* lastWriteTime,
    _Out_ IDWriteFontFile** fontFile)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateFontFileReference");
    if (_CreateFontFileReference_pre_ptr._value) { (this->*_CreateFontFileReference_pre_ptr._value)(filePath, lastWriteTime, fontFile); }
    HRESULT ret = GetRealObj()->CreateFontFileReference(filePath, lastWriteTime, fontFile);
    if ( fontFile && *fontFile) { *fontFile = RealToHooked_D2D( *fontFile ); }
    if (_CreateFontFileReference_post_ptr._value) { (this->*_CreateFontFileReference_post_ptr._value)(ret, filePath, lastWriteTime, fontFile); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateCustomFontFileReference(
    _In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey,
    UINT32 fontFileReferenceKeySize,
    IDWriteFontFileLoader* fontFileLoader,
    _Out_ IDWriteFontFile** fontFile)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateCustomFontFileReference");
    if (_CreateCustomFontFileReference_pre_ptr._value) { (this->*_CreateCustomFontFileReference_pre_ptr._value)(fontFileReferenceKey, fontFileReferenceKeySize, fontFileLoader, fontFile); }
    HRESULT ret = GetRealObj()->CreateCustomFontFileReference(fontFileReferenceKey, fontFileReferenceKeySize, fontFileLoader, fontFile);
    if ( fontFile && *fontFile) { *fontFile = RealToHooked_D2D( *fontFile ); }
    if (_CreateCustomFontFileReference_post_ptr._value) { (this->*_CreateCustomFontFileReference_post_ptr._value)(ret, fontFileReferenceKey, fontFileReferenceKeySize, fontFileLoader, fontFile); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateFontFace(
    DWRITE_FONT_FACE_TYPE fontFaceType,
    UINT32 numberOfFiles,
    _In_reads_(numberOfFiles) IDWriteFontFile* const* fontFiles,
    UINT32 faceIndex,
    DWRITE_FONT_SIMULATIONS fontFaceSimulationFlags,
    _Out_ IDWriteFontFace** fontFace)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateFontFace");
    if (_CreateFontFace_pre_ptr._value) { (this->*_CreateFontFace_pre_ptr._value)(fontFaceType, numberOfFiles, fontFiles, faceIndex, fontFaceSimulationFlags, fontFace); }
    HRESULT ret = GetRealObj()->CreateFontFace(fontFaceType, numberOfFiles, HookedToReal(numberOfFiles, fontFiles), faceIndex, fontFaceSimulationFlags, fontFace);
    if ( fontFace && *fontFace) { *fontFace = RealToHooked_D2D( *fontFace ); }
    if (_CreateFontFace_post_ptr._value) { (this->*_CreateFontFace_post_ptr._value)(ret, fontFaceType, numberOfFiles, fontFiles, faceIndex, fontFaceSimulationFlags, fontFace); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateRenderingParams(
    _Out_ IDWriteRenderingParams** renderingParams)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateRenderingParams");
    if (_CreateRenderingParams_pre_ptr._value) { (this->*_CreateRenderingParams_pre_ptr._value)(renderingParams); }
    HRESULT ret = GetRealObj()->CreateRenderingParams(renderingParams);
    if ( renderingParams && *renderingParams) { *renderingParams = RealToHooked_D2D( *renderingParams ); }
    if (_CreateRenderingParams_post_ptr._value) { (this->*_CreateRenderingParams_post_ptr._value)(ret, renderingParams); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateMonitorRenderingParams(
    HMONITOR monitor,
    _Out_ IDWriteRenderingParams** renderingParams)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateMonitorRenderingParams");
    if (_CreateMonitorRenderingParams_pre_ptr._value) { (this->*_CreateMonitorRenderingParams_pre_ptr._value)(monitor, renderingParams); }
    HRESULT ret = GetRealObj()->CreateMonitorRenderingParams(monitor, renderingParams);
    if ( renderingParams && *renderingParams) { *renderingParams = RealToHooked_D2D( *renderingParams ); }
    if (_CreateMonitorRenderingParams_post_ptr._value) { (this->*_CreateMonitorRenderingParams_post_ptr._value)(ret, monitor, renderingParams); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateCustomRenderingParams(
    FLOAT gamma,
    FLOAT enhancedContrast,
    FLOAT clearTypeLevel,
    DWRITE_PIXEL_GEOMETRY pixelGeometry,
    DWRITE_RENDERING_MODE renderingMode,
    _Out_ IDWriteRenderingParams** renderingParams)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateCustomRenderingParams");
    if (_CreateCustomRenderingParams_pre_ptr._value) { (this->*_CreateCustomRenderingParams_pre_ptr._value)(gamma, enhancedContrast, clearTypeLevel, pixelGeometry, renderingMode, renderingParams); }
    HRESULT ret = GetRealObj()->CreateCustomRenderingParams(gamma, enhancedContrast, clearTypeLevel, pixelGeometry, renderingMode, renderingParams);
    if ( renderingParams && *renderingParams) { *renderingParams = RealToHooked_D2D( *renderingParams ); }
    if (_CreateCustomRenderingParams_post_ptr._value) { (this->*_CreateCustomRenderingParams_post_ptr._value)(ret, gamma, enhancedContrast, clearTypeLevel, pixelGeometry, renderingMode, renderingParams); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::RegisterFontFileLoader(
    IDWriteFontFileLoader* fontFileLoader)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::RegisterFontFileLoader");
    if (_RegisterFontFileLoader_pre_ptr._value) { (this->*_RegisterFontFileLoader_pre_ptr._value)(fontFileLoader); }
    HRESULT ret = GetRealObj()->RegisterFontFileLoader(fontFileLoader);
    if (_RegisterFontFileLoader_post_ptr._value) { (this->*_RegisterFontFileLoader_post_ptr._value)(ret, fontFileLoader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::UnregisterFontFileLoader(
    IDWriteFontFileLoader* fontFileLoader)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::UnregisterFontFileLoader");
    if (_UnregisterFontFileLoader_pre_ptr._value) { (this->*_UnregisterFontFileLoader_pre_ptr._value)(fontFileLoader); }
    HRESULT ret = GetRealObj()->UnregisterFontFileLoader(fontFileLoader);
    if (_UnregisterFontFileLoader_post_ptr._value) { (this->*_UnregisterFontFileLoader_post_ptr._value)(ret, fontFileLoader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateTextFormat(
    _In_z_ WCHAR const* fontFamilyName,
    _In_opt_ IDWriteFontCollection* fontCollection,
    DWRITE_FONT_WEIGHT fontWeight,
    DWRITE_FONT_STYLE fontStyle,
    DWRITE_FONT_STRETCH fontStretch,
    FLOAT fontSize,
    _In_z_ WCHAR const* localeName,
    _Out_ IDWriteTextFormat** textFormat)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateTextFormat");
    if (_CreateTextFormat_pre_ptr._value) { (this->*_CreateTextFormat_pre_ptr._value)(fontFamilyName, fontCollection, fontWeight, fontStyle, fontStretch, fontSize, localeName, textFormat); }
    HRESULT ret = GetRealObj()->CreateTextFormat(fontFamilyName, HookedToReal(fontCollection), fontWeight, fontStyle, fontStretch, fontSize, localeName, textFormat);
    if ( textFormat && *textFormat) { *textFormat = RealToHooked_D2D( *textFormat ); }
    if (_CreateTextFormat_post_ptr._value) { (this->*_CreateTextFormat_post_ptr._value)(ret, fontFamilyName, fontCollection, fontWeight, fontStyle, fontStretch, fontSize, localeName, textFormat); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateTypography(
    _Out_ IDWriteTypography** typography)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateTypography");
    if (_CreateTypography_pre_ptr._value) { (this->*_CreateTypography_pre_ptr._value)(typography); }
    HRESULT ret = GetRealObj()->CreateTypography(typography);
    if ( typography && *typography) { *typography = RealToHooked_D2D( *typography ); }
    if (_CreateTypography_post_ptr._value) { (this->*_CreateTypography_post_ptr._value)(ret, typography); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::GetGdiInterop(
    _Out_ IDWriteGdiInterop** gdiInterop)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::GetGdiInterop");
    if (_GetGdiInterop_pre_ptr._value) { (this->*_GetGdiInterop_pre_ptr._value)(gdiInterop); }
    HRESULT ret = GetRealObj()->GetGdiInterop(gdiInterop);
    if ( gdiInterop && *gdiInterop) { *gdiInterop = RealToHooked_D2D( *gdiInterop ); }
    if (_GetGdiInterop_post_ptr._value) { (this->*_GetGdiInterop_post_ptr._value)(ret, gdiInterop); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateTextLayout(
    _In_reads_(stringLength) WCHAR const* string,
    UINT32 stringLength,
    IDWriteTextFormat* textFormat,
    FLOAT maxWidth,
    FLOAT maxHeight,
    _Out_ IDWriteTextLayout** textLayout)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateTextLayout");
    if (_CreateTextLayout_pre_ptr._value) { (this->*_CreateTextLayout_pre_ptr._value)(string, stringLength, textFormat, maxWidth, maxHeight, textLayout); }
    HRESULT ret = GetRealObj()->CreateTextLayout(string, stringLength, textFormat, maxWidth, maxHeight, textLayout);
    if ( textLayout && *textLayout) { *textLayout = RealToHooked_D2D( *textLayout ); }
    if (_CreateTextLayout_post_ptr._value) { (this->*_CreateTextLayout_post_ptr._value)(ret, string, stringLength, textFormat, maxWidth, maxHeight, textLayout); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateGdiCompatibleTextLayout(
    _In_reads_(stringLength) WCHAR const* string,
    UINT32 stringLength,
    IDWriteTextFormat* textFormat,
    FLOAT layoutWidth,
    FLOAT layoutHeight,
    FLOAT pixelsPerDip,
    _In_opt_ DWRITE_MATRIX const* transform,
    BOOL useGdiNatural,
    _Out_ IDWriteTextLayout** textLayout)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateGdiCompatibleTextLayout");
    if (_CreateGdiCompatibleTextLayout_pre_ptr._value) { (this->*_CreateGdiCompatibleTextLayout_pre_ptr._value)(string, stringLength, textFormat, layoutWidth, layoutHeight, pixelsPerDip, transform, useGdiNatural, textLayout); }
    HRESULT ret = GetRealObj()->CreateGdiCompatibleTextLayout(string, stringLength, textFormat, layoutWidth, layoutHeight, pixelsPerDip, transform, useGdiNatural, textLayout);
    if ( textLayout && *textLayout) { *textLayout = RealToHooked_D2D( *textLayout ); }
    if (_CreateGdiCompatibleTextLayout_post_ptr._value) { (this->*_CreateGdiCompatibleTextLayout_post_ptr._value)(ret, string, stringLength, textFormat, layoutWidth, layoutHeight, pixelsPerDip, transform, useGdiNatural, textLayout); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateEllipsisTrimmingSign(
    IDWriteTextFormat* textFormat,
    _Out_ IDWriteInlineObject** trimmingSign)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateEllipsisTrimmingSign");
    if (_CreateEllipsisTrimmingSign_pre_ptr._value) { (this->*_CreateEllipsisTrimmingSign_pre_ptr._value)(textFormat, trimmingSign); }
    HRESULT ret = GetRealObj()->CreateEllipsisTrimmingSign(textFormat, trimmingSign);
    if ( trimmingSign && *trimmingSign) { *trimmingSign = RealToHooked_D2D( *trimmingSign ); }
    if (_CreateEllipsisTrimmingSign_post_ptr._value) { (this->*_CreateEllipsisTrimmingSign_post_ptr._value)(ret, textFormat, trimmingSign); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateTextAnalyzer(
    _Out_ IDWriteTextAnalyzer** textAnalyzer)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateTextAnalyzer");
    if (_CreateTextAnalyzer_pre_ptr._value) { (this->*_CreateTextAnalyzer_pre_ptr._value)(textAnalyzer); }
    HRESULT ret = GetRealObj()->CreateTextAnalyzer(textAnalyzer);
    if ( textAnalyzer && *textAnalyzer) { *textAnalyzer = RealToHooked_D2D( *textAnalyzer ); }
    if (_CreateTextAnalyzer_post_ptr._value) { (this->*_CreateTextAnalyzer_post_ptr._value)(ret, textAnalyzer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateNumberSubstitution(
    _In_ DWRITE_NUMBER_SUBSTITUTION_METHOD substitutionMethod,
    _In_z_ WCHAR const* localeName,
    _In_ BOOL ignoreUserOverride,
    _Out_ IDWriteNumberSubstitution** numberSubstitution)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateNumberSubstitution");
    if (_CreateNumberSubstitution_pre_ptr._value) { (this->*_CreateNumberSubstitution_pre_ptr._value)(substitutionMethod, localeName, ignoreUserOverride, numberSubstitution); }
    HRESULT ret = GetRealObj()->CreateNumberSubstitution(substitutionMethod, localeName, ignoreUserOverride, numberSubstitution);
    if ( numberSubstitution && *numberSubstitution) { *numberSubstitution = RealToHooked_D2D( *numberSubstitution ); }
    if (_CreateNumberSubstitution_post_ptr._value) { (this->*_CreateNumberSubstitution_post_ptr._value)(ret, substitutionMethod, localeName, ignoreUserOverride, numberSubstitution); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DWriteFactoryHook::CreateGlyphRunAnalysis(
    _In_ DWRITE_GLYPH_RUN const* glyphRun,
    FLOAT pixelsPerDip,
    _In_opt_ DWRITE_MATRIX const* transform,
    DWRITE_RENDERING_MODE renderingMode,
    DWRITE_MEASURING_MODE measuringMode,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    _Out_ IDWriteGlyphRunAnalysis** glyphRunAnalysis)
{
    calltrace::AutoTrace trace(L"DWriteFactoryHook::CreateGlyphRunAnalysis");
    if (_CreateGlyphRunAnalysis_pre_ptr._value) { (this->*_CreateGlyphRunAnalysis_pre_ptr._value)(glyphRun, pixelsPerDip, transform, renderingMode, measuringMode, baselineOriginX, baselineOriginY, glyphRunAnalysis); }
    HRESULT ret = GetRealObj()->CreateGlyphRunAnalysis(glyphRun, pixelsPerDip, transform, renderingMode, measuringMode, baselineOriginX, baselineOriginY, glyphRunAnalysis);
    if ( glyphRunAnalysis && *glyphRunAnalysis) { *glyphRunAnalysis = RealToHooked_D2D( *glyphRunAnalysis ); }
    if (_CreateGlyphRunAnalysis_post_ptr._value) { (this->*_CreateGlyphRunAnalysis_post_ptr._value)(ret, glyphRun, pixelsPerDip, transform, renderingMode, measuringMode, baselineOriginX, baselineOriginY, glyphRunAnalysis); }
    return ret;
}

