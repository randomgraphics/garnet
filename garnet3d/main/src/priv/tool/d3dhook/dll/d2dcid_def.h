// (Script generated header. DO NOT EDIT.)
// Define call ID for all D2D and DXGI methods.
#pragma once

enum D2D_CALL_ID
{

    // CID for ID2D1Resource
    CID_ID2D1Resource_BASE,
    CID_ID2D1Resource_COUNT = 1,
    CID_ID2D1Resource_GetFactory = CID_ID2D1Resource_BASE + 0,

    // CID for ID2D1Image
    CID_ID2D1Image_BASE,
    CID_ID2D1Image_COUNT = 0,

    // CID for ID2D1Bitmap
    CID_ID2D1Bitmap_BASE,
    CID_ID2D1Bitmap_COUNT = 7,
    CID_ID2D1Bitmap_GetSize = CID_ID2D1Bitmap_BASE + 0,
    CID_ID2D1Bitmap_GetPixelSize = CID_ID2D1Bitmap_BASE + 1,
    CID_ID2D1Bitmap_GetPixelFormat = CID_ID2D1Bitmap_BASE + 2,
    CID_ID2D1Bitmap_GetDpi = CID_ID2D1Bitmap_BASE + 3,
    CID_ID2D1Bitmap_CopyFromBitmap = CID_ID2D1Bitmap_BASE + 4,
    CID_ID2D1Bitmap_CopyFromRenderTarget = CID_ID2D1Bitmap_BASE + 5,
    CID_ID2D1Bitmap_CopyFromMemory = CID_ID2D1Bitmap_BASE + 6,

    // CID for ID2D1GradientStopCollection
    CID_ID2D1GradientStopCollection_BASE,
    CID_ID2D1GradientStopCollection_COUNT = 4,
    CID_ID2D1GradientStopCollection_GetGradientStopCount = CID_ID2D1GradientStopCollection_BASE + 0,
    CID_ID2D1GradientStopCollection_GetGradientStops = CID_ID2D1GradientStopCollection_BASE + 1,
    CID_ID2D1GradientStopCollection_GetColorInterpolationGamma = CID_ID2D1GradientStopCollection_BASE + 2,
    CID_ID2D1GradientStopCollection_GetExtendMode = CID_ID2D1GradientStopCollection_BASE + 3,

    // CID for ID2D1Brush
    CID_ID2D1Brush_BASE,
    CID_ID2D1Brush_COUNT = 4,
    CID_ID2D1Brush_SetOpacity = CID_ID2D1Brush_BASE + 0,
    CID_ID2D1Brush_SetTransform = CID_ID2D1Brush_BASE + 1,
    CID_ID2D1Brush_GetOpacity = CID_ID2D1Brush_BASE + 2,
    CID_ID2D1Brush_GetTransform = CID_ID2D1Brush_BASE + 3,

    // CID for ID2D1BitmapBrush
    CID_ID2D1BitmapBrush_BASE,
    CID_ID2D1BitmapBrush_COUNT = 8,
    CID_ID2D1BitmapBrush_SetExtendModeX = CID_ID2D1BitmapBrush_BASE + 0,
    CID_ID2D1BitmapBrush_SetExtendModeY = CID_ID2D1BitmapBrush_BASE + 1,
    CID_ID2D1BitmapBrush_SetInterpolationMode = CID_ID2D1BitmapBrush_BASE + 2,
    CID_ID2D1BitmapBrush_SetBitmap = CID_ID2D1BitmapBrush_BASE + 3,
    CID_ID2D1BitmapBrush_GetExtendModeX = CID_ID2D1BitmapBrush_BASE + 4,
    CID_ID2D1BitmapBrush_GetExtendModeY = CID_ID2D1BitmapBrush_BASE + 5,
    CID_ID2D1BitmapBrush_GetInterpolationMode = CID_ID2D1BitmapBrush_BASE + 6,
    CID_ID2D1BitmapBrush_GetBitmap = CID_ID2D1BitmapBrush_BASE + 7,

    // CID for ID2D1SolidColorBrush
    CID_ID2D1SolidColorBrush_BASE,
    CID_ID2D1SolidColorBrush_COUNT = 2,
    CID_ID2D1SolidColorBrush_SetColor = CID_ID2D1SolidColorBrush_BASE + 0,
    CID_ID2D1SolidColorBrush_GetColor = CID_ID2D1SolidColorBrush_BASE + 1,

    // CID for ID2D1LinearGradientBrush
    CID_ID2D1LinearGradientBrush_BASE,
    CID_ID2D1LinearGradientBrush_COUNT = 5,
    CID_ID2D1LinearGradientBrush_SetStartPoint = CID_ID2D1LinearGradientBrush_BASE + 0,
    CID_ID2D1LinearGradientBrush_SetEndPoint = CID_ID2D1LinearGradientBrush_BASE + 1,
    CID_ID2D1LinearGradientBrush_GetStartPoint = CID_ID2D1LinearGradientBrush_BASE + 2,
    CID_ID2D1LinearGradientBrush_GetEndPoint = CID_ID2D1LinearGradientBrush_BASE + 3,
    CID_ID2D1LinearGradientBrush_GetGradientStopCollection = CID_ID2D1LinearGradientBrush_BASE + 4,

    // CID for ID2D1RadialGradientBrush
    CID_ID2D1RadialGradientBrush_BASE,
    CID_ID2D1RadialGradientBrush_COUNT = 9,
    CID_ID2D1RadialGradientBrush_SetCenter = CID_ID2D1RadialGradientBrush_BASE + 0,
    CID_ID2D1RadialGradientBrush_SetGradientOriginOffset = CID_ID2D1RadialGradientBrush_BASE + 1,
    CID_ID2D1RadialGradientBrush_SetRadiusX = CID_ID2D1RadialGradientBrush_BASE + 2,
    CID_ID2D1RadialGradientBrush_SetRadiusY = CID_ID2D1RadialGradientBrush_BASE + 3,
    CID_ID2D1RadialGradientBrush_GetCenter = CID_ID2D1RadialGradientBrush_BASE + 4,
    CID_ID2D1RadialGradientBrush_GetGradientOriginOffset = CID_ID2D1RadialGradientBrush_BASE + 5,
    CID_ID2D1RadialGradientBrush_GetRadiusX = CID_ID2D1RadialGradientBrush_BASE + 6,
    CID_ID2D1RadialGradientBrush_GetRadiusY = CID_ID2D1RadialGradientBrush_BASE + 7,
    CID_ID2D1RadialGradientBrush_GetGradientStopCollection = CID_ID2D1RadialGradientBrush_BASE + 8,

    // CID for ID2D1StrokeStyle
    CID_ID2D1StrokeStyle_BASE,
    CID_ID2D1StrokeStyle_COUNT = 9,
    CID_ID2D1StrokeStyle_GetStartCap = CID_ID2D1StrokeStyle_BASE + 0,
    CID_ID2D1StrokeStyle_GetEndCap = CID_ID2D1StrokeStyle_BASE + 1,
    CID_ID2D1StrokeStyle_GetDashCap = CID_ID2D1StrokeStyle_BASE + 2,
    CID_ID2D1StrokeStyle_GetMiterLimit = CID_ID2D1StrokeStyle_BASE + 3,
    CID_ID2D1StrokeStyle_GetLineJoin = CID_ID2D1StrokeStyle_BASE + 4,
    CID_ID2D1StrokeStyle_GetDashOffset = CID_ID2D1StrokeStyle_BASE + 5,
    CID_ID2D1StrokeStyle_GetDashStyle = CID_ID2D1StrokeStyle_BASE + 6,
    CID_ID2D1StrokeStyle_GetDashesCount = CID_ID2D1StrokeStyle_BASE + 7,
    CID_ID2D1StrokeStyle_GetDashes = CID_ID2D1StrokeStyle_BASE + 8,

    // CID for ID2D1Geometry
    CID_ID2D1Geometry_BASE,
    CID_ID2D1Geometry_COUNT = 13,
    CID_ID2D1Geometry_GetBounds = CID_ID2D1Geometry_BASE + 0,
    CID_ID2D1Geometry_GetWidenedBounds = CID_ID2D1Geometry_BASE + 1,
    CID_ID2D1Geometry_StrokeContainsPoint = CID_ID2D1Geometry_BASE + 2,
    CID_ID2D1Geometry_FillContainsPoint = CID_ID2D1Geometry_BASE + 3,
    CID_ID2D1Geometry_CompareWithGeometry = CID_ID2D1Geometry_BASE + 4,
    CID_ID2D1Geometry_Simplify = CID_ID2D1Geometry_BASE + 5,
    CID_ID2D1Geometry_Tessellate = CID_ID2D1Geometry_BASE + 6,
    CID_ID2D1Geometry_CombineWithGeometry = CID_ID2D1Geometry_BASE + 7,
    CID_ID2D1Geometry_Outline = CID_ID2D1Geometry_BASE + 8,
    CID_ID2D1Geometry_ComputeArea = CID_ID2D1Geometry_BASE + 9,
    CID_ID2D1Geometry_ComputeLength = CID_ID2D1Geometry_BASE + 10,
    CID_ID2D1Geometry_ComputePointAtLength = CID_ID2D1Geometry_BASE + 11,
    CID_ID2D1Geometry_Widen = CID_ID2D1Geometry_BASE + 12,

    // CID for ID2D1RectangleGeometry
    CID_ID2D1RectangleGeometry_BASE,
    CID_ID2D1RectangleGeometry_COUNT = 1,
    CID_ID2D1RectangleGeometry_GetRect = CID_ID2D1RectangleGeometry_BASE + 0,

    // CID for ID2D1RoundedRectangleGeometry
    CID_ID2D1RoundedRectangleGeometry_BASE,
    CID_ID2D1RoundedRectangleGeometry_COUNT = 1,
    CID_ID2D1RoundedRectangleGeometry_GetRoundedRect = CID_ID2D1RoundedRectangleGeometry_BASE + 0,

    // CID for ID2D1EllipseGeometry
    CID_ID2D1EllipseGeometry_BASE,
    CID_ID2D1EllipseGeometry_COUNT = 1,
    CID_ID2D1EllipseGeometry_GetEllipse = CID_ID2D1EllipseGeometry_BASE + 0,

    // CID for ID2D1GeometryGroup
    CID_ID2D1GeometryGroup_BASE,
    CID_ID2D1GeometryGroup_COUNT = 3,
    CID_ID2D1GeometryGroup_GetFillMode = CID_ID2D1GeometryGroup_BASE + 0,
    CID_ID2D1GeometryGroup_GetSourceGeometryCount = CID_ID2D1GeometryGroup_BASE + 1,
    CID_ID2D1GeometryGroup_GetSourceGeometries = CID_ID2D1GeometryGroup_BASE + 2,

    // CID for ID2D1TransformedGeometry
    CID_ID2D1TransformedGeometry_BASE,
    CID_ID2D1TransformedGeometry_COUNT = 2,
    CID_ID2D1TransformedGeometry_GetSourceGeometry = CID_ID2D1TransformedGeometry_BASE + 0,
    CID_ID2D1TransformedGeometry_GetTransform = CID_ID2D1TransformedGeometry_BASE + 1,

    // CID for ID2D1SimplifiedGeometrySink
    CID_ID2D1SimplifiedGeometrySink_BASE,
    CID_ID2D1SimplifiedGeometrySink_COUNT = 7,
    CID_ID2D1SimplifiedGeometrySink_SetFillMode = CID_ID2D1SimplifiedGeometrySink_BASE + 0,
    CID_ID2D1SimplifiedGeometrySink_SetSegmentFlags = CID_ID2D1SimplifiedGeometrySink_BASE + 1,
    CID_ID2D1SimplifiedGeometrySink_BeginFigure = CID_ID2D1SimplifiedGeometrySink_BASE + 2,
    CID_ID2D1SimplifiedGeometrySink_AddLines = CID_ID2D1SimplifiedGeometrySink_BASE + 3,
    CID_ID2D1SimplifiedGeometrySink_AddBeziers = CID_ID2D1SimplifiedGeometrySink_BASE + 4,
    CID_ID2D1SimplifiedGeometrySink_EndFigure = CID_ID2D1SimplifiedGeometrySink_BASE + 5,
    CID_ID2D1SimplifiedGeometrySink_Close = CID_ID2D1SimplifiedGeometrySink_BASE + 6,

    // CID for ID2D1GeometrySink
    CID_ID2D1GeometrySink_BASE,
    CID_ID2D1GeometrySink_COUNT = 5,
    CID_ID2D1GeometrySink_AddLine = CID_ID2D1GeometrySink_BASE + 0,
    CID_ID2D1GeometrySink_AddBezier = CID_ID2D1GeometrySink_BASE + 1,
    CID_ID2D1GeometrySink_AddQuadraticBezier = CID_ID2D1GeometrySink_BASE + 2,
    CID_ID2D1GeometrySink_AddQuadraticBeziers = CID_ID2D1GeometrySink_BASE + 3,
    CID_ID2D1GeometrySink_AddArc = CID_ID2D1GeometrySink_BASE + 4,

    // CID for ID2D1TessellationSink
    CID_ID2D1TessellationSink_BASE,
    CID_ID2D1TessellationSink_COUNT = 2,
    CID_ID2D1TessellationSink_AddTriangles = CID_ID2D1TessellationSink_BASE + 0,
    CID_ID2D1TessellationSink_Close = CID_ID2D1TessellationSink_BASE + 1,

    // CID for ID2D1PathGeometry
    CID_ID2D1PathGeometry_BASE,
    CID_ID2D1PathGeometry_COUNT = 4,
    CID_ID2D1PathGeometry_Open = CID_ID2D1PathGeometry_BASE + 0,
    CID_ID2D1PathGeometry_Stream = CID_ID2D1PathGeometry_BASE + 1,
    CID_ID2D1PathGeometry_GetSegmentCount = CID_ID2D1PathGeometry_BASE + 2,
    CID_ID2D1PathGeometry_GetFigureCount = CID_ID2D1PathGeometry_BASE + 3,

    // CID for ID2D1Mesh
    CID_ID2D1Mesh_BASE,
    CID_ID2D1Mesh_COUNT = 1,
    CID_ID2D1Mesh_Open = CID_ID2D1Mesh_BASE + 0,

    // CID for ID2D1Layer
    CID_ID2D1Layer_BASE,
    CID_ID2D1Layer_COUNT = 1,
    CID_ID2D1Layer_GetSize = CID_ID2D1Layer_BASE + 0,

    // CID for ID2D1DrawingStateBlock
    CID_ID2D1DrawingStateBlock_BASE,
    CID_ID2D1DrawingStateBlock_COUNT = 4,
    CID_ID2D1DrawingStateBlock_GetDescription = CID_ID2D1DrawingStateBlock_BASE + 0,
    CID_ID2D1DrawingStateBlock_SetDescription = CID_ID2D1DrawingStateBlock_BASE + 1,
    CID_ID2D1DrawingStateBlock_SetTextRenderingParams = CID_ID2D1DrawingStateBlock_BASE + 2,
    CID_ID2D1DrawingStateBlock_GetTextRenderingParams = CID_ID2D1DrawingStateBlock_BASE + 3,

    // CID for ID2D1RenderTarget
    CID_ID2D1RenderTarget_BASE,
    CID_ID2D1RenderTarget_COUNT = 53,
    CID_ID2D1RenderTarget_CreateBitmap = CID_ID2D1RenderTarget_BASE + 0,
    CID_ID2D1RenderTarget_CreateBitmapFromWicBitmap = CID_ID2D1RenderTarget_BASE + 1,
    CID_ID2D1RenderTarget_CreateSharedBitmap = CID_ID2D1RenderTarget_BASE + 2,
    CID_ID2D1RenderTarget_CreateBitmapBrush = CID_ID2D1RenderTarget_BASE + 3,
    CID_ID2D1RenderTarget_CreateSolidColorBrush = CID_ID2D1RenderTarget_BASE + 4,
    CID_ID2D1RenderTarget_CreateGradientStopCollection = CID_ID2D1RenderTarget_BASE + 5,
    CID_ID2D1RenderTarget_CreateLinearGradientBrush = CID_ID2D1RenderTarget_BASE + 6,
    CID_ID2D1RenderTarget_CreateRadialGradientBrush = CID_ID2D1RenderTarget_BASE + 7,
    CID_ID2D1RenderTarget_CreateCompatibleRenderTarget = CID_ID2D1RenderTarget_BASE + 8,
    CID_ID2D1RenderTarget_CreateLayer = CID_ID2D1RenderTarget_BASE + 9,
    CID_ID2D1RenderTarget_CreateMesh = CID_ID2D1RenderTarget_BASE + 10,
    CID_ID2D1RenderTarget_DrawLine = CID_ID2D1RenderTarget_BASE + 11,
    CID_ID2D1RenderTarget_DrawRectangle = CID_ID2D1RenderTarget_BASE + 12,
    CID_ID2D1RenderTarget_FillRectangle = CID_ID2D1RenderTarget_BASE + 13,
    CID_ID2D1RenderTarget_DrawRoundedRectangle = CID_ID2D1RenderTarget_BASE + 14,
    CID_ID2D1RenderTarget_FillRoundedRectangle = CID_ID2D1RenderTarget_BASE + 15,
    CID_ID2D1RenderTarget_DrawEllipse = CID_ID2D1RenderTarget_BASE + 16,
    CID_ID2D1RenderTarget_FillEllipse = CID_ID2D1RenderTarget_BASE + 17,
    CID_ID2D1RenderTarget_DrawGeometry = CID_ID2D1RenderTarget_BASE + 18,
    CID_ID2D1RenderTarget_FillGeometry = CID_ID2D1RenderTarget_BASE + 19,
    CID_ID2D1RenderTarget_FillMesh = CID_ID2D1RenderTarget_BASE + 20,
    CID_ID2D1RenderTarget_FillOpacityMask = CID_ID2D1RenderTarget_BASE + 21,
    CID_ID2D1RenderTarget_DrawBitmap = CID_ID2D1RenderTarget_BASE + 22,
    CID_ID2D1RenderTarget_DrawText = CID_ID2D1RenderTarget_BASE + 23,
    CID_ID2D1RenderTarget_DrawTextLayout = CID_ID2D1RenderTarget_BASE + 24,
    CID_ID2D1RenderTarget_DrawGlyphRun = CID_ID2D1RenderTarget_BASE + 25,
    CID_ID2D1RenderTarget_SetTransform = CID_ID2D1RenderTarget_BASE + 26,
    CID_ID2D1RenderTarget_GetTransform = CID_ID2D1RenderTarget_BASE + 27,
    CID_ID2D1RenderTarget_SetAntialiasMode = CID_ID2D1RenderTarget_BASE + 28,
    CID_ID2D1RenderTarget_GetAntialiasMode = CID_ID2D1RenderTarget_BASE + 29,
    CID_ID2D1RenderTarget_SetTextAntialiasMode = CID_ID2D1RenderTarget_BASE + 30,
    CID_ID2D1RenderTarget_GetTextAntialiasMode = CID_ID2D1RenderTarget_BASE + 31,
    CID_ID2D1RenderTarget_SetTextRenderingParams = CID_ID2D1RenderTarget_BASE + 32,
    CID_ID2D1RenderTarget_GetTextRenderingParams = CID_ID2D1RenderTarget_BASE + 33,
    CID_ID2D1RenderTarget_SetTags = CID_ID2D1RenderTarget_BASE + 34,
    CID_ID2D1RenderTarget_GetTags = CID_ID2D1RenderTarget_BASE + 35,
    CID_ID2D1RenderTarget_PushLayer = CID_ID2D1RenderTarget_BASE + 36,
    CID_ID2D1RenderTarget_PopLayer = CID_ID2D1RenderTarget_BASE + 37,
    CID_ID2D1RenderTarget_Flush = CID_ID2D1RenderTarget_BASE + 38,
    CID_ID2D1RenderTarget_SaveDrawingState = CID_ID2D1RenderTarget_BASE + 39,
    CID_ID2D1RenderTarget_RestoreDrawingState = CID_ID2D1RenderTarget_BASE + 40,
    CID_ID2D1RenderTarget_PushAxisAlignedClip = CID_ID2D1RenderTarget_BASE + 41,
    CID_ID2D1RenderTarget_PopAxisAlignedClip = CID_ID2D1RenderTarget_BASE + 42,
    CID_ID2D1RenderTarget_Clear = CID_ID2D1RenderTarget_BASE + 43,
    CID_ID2D1RenderTarget_BeginDraw = CID_ID2D1RenderTarget_BASE + 44,
    CID_ID2D1RenderTarget_EndDraw = CID_ID2D1RenderTarget_BASE + 45,
    CID_ID2D1RenderTarget_GetPixelFormat = CID_ID2D1RenderTarget_BASE + 46,
    CID_ID2D1RenderTarget_SetDpi = CID_ID2D1RenderTarget_BASE + 47,
    CID_ID2D1RenderTarget_GetDpi = CID_ID2D1RenderTarget_BASE + 48,
    CID_ID2D1RenderTarget_GetSize = CID_ID2D1RenderTarget_BASE + 49,
    CID_ID2D1RenderTarget_GetPixelSize = CID_ID2D1RenderTarget_BASE + 50,
    CID_ID2D1RenderTarget_GetMaximumBitmapSize = CID_ID2D1RenderTarget_BASE + 51,
    CID_ID2D1RenderTarget_IsSupported = CID_ID2D1RenderTarget_BASE + 52,

    // CID for ID2D1BitmapRenderTarget
    CID_ID2D1BitmapRenderTarget_BASE,
    CID_ID2D1BitmapRenderTarget_COUNT = 1,
    CID_ID2D1BitmapRenderTarget_GetBitmap = CID_ID2D1BitmapRenderTarget_BASE + 0,

    // CID for ID2D1HwndRenderTarget
    CID_ID2D1HwndRenderTarget_BASE,
    CID_ID2D1HwndRenderTarget_COUNT = 3,
    CID_ID2D1HwndRenderTarget_CheckWindowState = CID_ID2D1HwndRenderTarget_BASE + 0,
    CID_ID2D1HwndRenderTarget_Resize = CID_ID2D1HwndRenderTarget_BASE + 1,
    CID_ID2D1HwndRenderTarget_GetHwnd = CID_ID2D1HwndRenderTarget_BASE + 2,

    // CID for ID2D1GdiInteropRenderTarget
    CID_ID2D1GdiInteropRenderTarget_BASE,
    CID_ID2D1GdiInteropRenderTarget_COUNT = 2,
    CID_ID2D1GdiInteropRenderTarget_GetDC = CID_ID2D1GdiInteropRenderTarget_BASE + 0,
    CID_ID2D1GdiInteropRenderTarget_ReleaseDC = CID_ID2D1GdiInteropRenderTarget_BASE + 1,

    // CID for ID2D1DCRenderTarget
    CID_ID2D1DCRenderTarget_BASE,
    CID_ID2D1DCRenderTarget_COUNT = 1,
    CID_ID2D1DCRenderTarget_BindDC = CID_ID2D1DCRenderTarget_BASE + 0,

    // CID for ID2D1Factory
    CID_ID2D1Factory_BASE,
    CID_ID2D1Factory_COUNT = 14,
    CID_ID2D1Factory_ReloadSystemMetrics = CID_ID2D1Factory_BASE + 0,
    CID_ID2D1Factory_GetDesktopDpi = CID_ID2D1Factory_BASE + 1,
    CID_ID2D1Factory_CreateRectangleGeometry = CID_ID2D1Factory_BASE + 2,
    CID_ID2D1Factory_CreateRoundedRectangleGeometry = CID_ID2D1Factory_BASE + 3,
    CID_ID2D1Factory_CreateEllipseGeometry = CID_ID2D1Factory_BASE + 4,
    CID_ID2D1Factory_CreateGeometryGroup = CID_ID2D1Factory_BASE + 5,
    CID_ID2D1Factory_CreateTransformedGeometry = CID_ID2D1Factory_BASE + 6,
    CID_ID2D1Factory_CreatePathGeometry = CID_ID2D1Factory_BASE + 7,
    CID_ID2D1Factory_CreateStrokeStyle = CID_ID2D1Factory_BASE + 8,
    CID_ID2D1Factory_CreateDrawingStateBlock = CID_ID2D1Factory_BASE + 9,
    CID_ID2D1Factory_CreateWicBitmapRenderTarget = CID_ID2D1Factory_BASE + 10,
    CID_ID2D1Factory_CreateHwndRenderTarget = CID_ID2D1Factory_BASE + 11,
    CID_ID2D1Factory_CreateDxgiSurfaceRenderTarget = CID_ID2D1Factory_BASE + 12,
    CID_ID2D1Factory_CreateDCRenderTarget = CID_ID2D1Factory_BASE + 13,

    CID_TOTAL_COUNT,
    CID_INVALID = 0xFFFFFFFF,
}; // end of enum definition

extern const char * const g_D2DCallIDText;
