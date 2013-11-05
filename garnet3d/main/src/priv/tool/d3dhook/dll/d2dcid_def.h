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

    // CID for ID2D1GdiMetafileSink
    CID_ID2D1GdiMetafileSink_BASE,
    CID_ID2D1GdiMetafileSink_COUNT = 1,
    CID_ID2D1GdiMetafileSink_ProcessRecord = CID_ID2D1GdiMetafileSink_BASE + 0,

    // CID for ID2D1GdiMetafile
    CID_ID2D1GdiMetafile_BASE,
    CID_ID2D1GdiMetafile_COUNT = 2,
    CID_ID2D1GdiMetafile_Stream = CID_ID2D1GdiMetafile_BASE + 0,
    CID_ID2D1GdiMetafile_GetBounds = CID_ID2D1GdiMetafile_BASE + 1,

    // CID for ID2D1CommandSink
    CID_ID2D1CommandSink_BASE,
    CID_ID2D1CommandSink_COUNT = 25,
    CID_ID2D1CommandSink_BeginDraw = CID_ID2D1CommandSink_BASE + 0,
    CID_ID2D1CommandSink_EndDraw = CID_ID2D1CommandSink_BASE + 1,
    CID_ID2D1CommandSink_SetAntialiasMode = CID_ID2D1CommandSink_BASE + 2,
    CID_ID2D1CommandSink_SetTags = CID_ID2D1CommandSink_BASE + 3,
    CID_ID2D1CommandSink_SetTextAntialiasMode = CID_ID2D1CommandSink_BASE + 4,
    CID_ID2D1CommandSink_SetTextRenderingParams = CID_ID2D1CommandSink_BASE + 5,
    CID_ID2D1CommandSink_SetTransform = CID_ID2D1CommandSink_BASE + 6,
    CID_ID2D1CommandSink_SetPrimitiveBlend = CID_ID2D1CommandSink_BASE + 7,
    CID_ID2D1CommandSink_SetUnitMode = CID_ID2D1CommandSink_BASE + 8,
    CID_ID2D1CommandSink_Clear = CID_ID2D1CommandSink_BASE + 9,
    CID_ID2D1CommandSink_DrawGlyphRun = CID_ID2D1CommandSink_BASE + 10,
    CID_ID2D1CommandSink_DrawLine = CID_ID2D1CommandSink_BASE + 11,
    CID_ID2D1CommandSink_DrawGeometry = CID_ID2D1CommandSink_BASE + 12,
    CID_ID2D1CommandSink_DrawRectangle = CID_ID2D1CommandSink_BASE + 13,
    CID_ID2D1CommandSink_DrawBitmap = CID_ID2D1CommandSink_BASE + 14,
    CID_ID2D1CommandSink_DrawImage = CID_ID2D1CommandSink_BASE + 15,
    CID_ID2D1CommandSink_DrawGdiMetafile = CID_ID2D1CommandSink_BASE + 16,
    CID_ID2D1CommandSink_FillMesh = CID_ID2D1CommandSink_BASE + 17,
    CID_ID2D1CommandSink_FillOpacityMask = CID_ID2D1CommandSink_BASE + 18,
    CID_ID2D1CommandSink_FillGeometry = CID_ID2D1CommandSink_BASE + 19,
    CID_ID2D1CommandSink_FillRectangle = CID_ID2D1CommandSink_BASE + 20,
    CID_ID2D1CommandSink_PushAxisAlignedClip = CID_ID2D1CommandSink_BASE + 21,
    CID_ID2D1CommandSink_PushLayer = CID_ID2D1CommandSink_BASE + 22,
    CID_ID2D1CommandSink_PopAxisAlignedClip = CID_ID2D1CommandSink_BASE + 23,
    CID_ID2D1CommandSink_PopLayer = CID_ID2D1CommandSink_BASE + 24,

    // CID for ID2D1CommandList
    CID_ID2D1CommandList_BASE,
    CID_ID2D1CommandList_COUNT = 2,
    CID_ID2D1CommandList_Stream = CID_ID2D1CommandList_BASE + 0,
    CID_ID2D1CommandList_Close = CID_ID2D1CommandList_BASE + 1,

    // CID for ID2D1PrintControl
    CID_ID2D1PrintControl_BASE,
    CID_ID2D1PrintControl_COUNT = 2,
    CID_ID2D1PrintControl_AddPage = CID_ID2D1PrintControl_BASE + 0,
    CID_ID2D1PrintControl_Close = CID_ID2D1PrintControl_BASE + 1,

    // CID for ID2D1ImageBrush
    CID_ID2D1ImageBrush_BASE,
    CID_ID2D1ImageBrush_COUNT = 10,
    CID_ID2D1ImageBrush_SetImage = CID_ID2D1ImageBrush_BASE + 0,
    CID_ID2D1ImageBrush_SetExtendModeX = CID_ID2D1ImageBrush_BASE + 1,
    CID_ID2D1ImageBrush_SetExtendModeY = CID_ID2D1ImageBrush_BASE + 2,
    CID_ID2D1ImageBrush_SetInterpolationMode = CID_ID2D1ImageBrush_BASE + 3,
    CID_ID2D1ImageBrush_SetSourceRectangle = CID_ID2D1ImageBrush_BASE + 4,
    CID_ID2D1ImageBrush_GetImage = CID_ID2D1ImageBrush_BASE + 5,
    CID_ID2D1ImageBrush_GetExtendModeX = CID_ID2D1ImageBrush_BASE + 6,
    CID_ID2D1ImageBrush_GetExtendModeY = CID_ID2D1ImageBrush_BASE + 7,
    CID_ID2D1ImageBrush_GetInterpolationMode = CID_ID2D1ImageBrush_BASE + 8,
    CID_ID2D1ImageBrush_GetSourceRectangle = CID_ID2D1ImageBrush_BASE + 9,

    // CID for ID2D1BitmapBrush1
    CID_ID2D1BitmapBrush1_BASE,
    CID_ID2D1BitmapBrush1_COUNT = 2,
    CID_ID2D1BitmapBrush1_SetInterpolationMode1 = CID_ID2D1BitmapBrush1_BASE + 0,
    CID_ID2D1BitmapBrush1_GetInterpolationMode1 = CID_ID2D1BitmapBrush1_BASE + 1,

    // CID for ID2D1StrokeStyle1
    CID_ID2D1StrokeStyle1_BASE,
    CID_ID2D1StrokeStyle1_COUNT = 1,
    CID_ID2D1StrokeStyle1_GetStrokeTransformType = CID_ID2D1StrokeStyle1_BASE + 0,

    // CID for ID2D1PathGeometry1
    CID_ID2D1PathGeometry1_BASE,
    CID_ID2D1PathGeometry1_COUNT = 1,
    CID_ID2D1PathGeometry1_ComputePointAndSegmentAtLength = CID_ID2D1PathGeometry1_BASE + 0,

    // CID for ID2D1Properties
    CID_ID2D1Properties_BASE,
    CID_ID2D1Properties_COUNT = 11,
    CID_ID2D1Properties_GetPropertyCount = CID_ID2D1Properties_BASE + 0,
    CID_ID2D1Properties_GetPropertyName = CID_ID2D1Properties_BASE + 1,
    CID_ID2D1Properties_GetPropertyNameLength = CID_ID2D1Properties_BASE + 2,
    CID_ID2D1Properties_GetType = CID_ID2D1Properties_BASE + 3,
    CID_ID2D1Properties_GetPropertyIndex = CID_ID2D1Properties_BASE + 4,
    CID_ID2D1Properties_SetValueByName = CID_ID2D1Properties_BASE + 5,
    CID_ID2D1Properties_SetValue = CID_ID2D1Properties_BASE + 6,
    CID_ID2D1Properties_GetValueByName = CID_ID2D1Properties_BASE + 7,
    CID_ID2D1Properties_GetValue = CID_ID2D1Properties_BASE + 8,
    CID_ID2D1Properties_GetValueSize = CID_ID2D1Properties_BASE + 9,
    CID_ID2D1Properties_GetSubProperties = CID_ID2D1Properties_BASE + 10,

    // CID for ID2D1Effect
    CID_ID2D1Effect_BASE,
    CID_ID2D1Effect_COUNT = 5,
    CID_ID2D1Effect_SetInput = CID_ID2D1Effect_BASE + 0,
    CID_ID2D1Effect_SetInputCount = CID_ID2D1Effect_BASE + 1,
    CID_ID2D1Effect_GetInput = CID_ID2D1Effect_BASE + 2,
    CID_ID2D1Effect_GetInputCount = CID_ID2D1Effect_BASE + 3,
    CID_ID2D1Effect_GetOutput = CID_ID2D1Effect_BASE + 4,

    // CID for ID2D1Bitmap1
    CID_ID2D1Bitmap1_BASE,
    CID_ID2D1Bitmap1_COUNT = 5,
    CID_ID2D1Bitmap1_GetColorContext = CID_ID2D1Bitmap1_BASE + 0,
    CID_ID2D1Bitmap1_GetOptions = CID_ID2D1Bitmap1_BASE + 1,
    CID_ID2D1Bitmap1_GetSurface = CID_ID2D1Bitmap1_BASE + 2,
    CID_ID2D1Bitmap1_Map = CID_ID2D1Bitmap1_BASE + 3,
    CID_ID2D1Bitmap1_Unmap = CID_ID2D1Bitmap1_BASE + 4,

    // CID for ID2D1ColorContext
    CID_ID2D1ColorContext_BASE,
    CID_ID2D1ColorContext_COUNT = 3,
    CID_ID2D1ColorContext_GetColorSpace = CID_ID2D1ColorContext_BASE + 0,
    CID_ID2D1ColorContext_GetProfileSize = CID_ID2D1ColorContext_BASE + 1,
    CID_ID2D1ColorContext_GetProfile = CID_ID2D1ColorContext_BASE + 2,

    // CID for ID2D1GradientStopCollection1
    CID_ID2D1GradientStopCollection1_BASE,
    CID_ID2D1GradientStopCollection1_COUNT = 5,
    CID_ID2D1GradientStopCollection1_GetGradientStops1 = CID_ID2D1GradientStopCollection1_BASE + 0,
    CID_ID2D1GradientStopCollection1_GetPreInterpolationSpace = CID_ID2D1GradientStopCollection1_BASE + 1,
    CID_ID2D1GradientStopCollection1_GetPostInterpolationSpace = CID_ID2D1GradientStopCollection1_BASE + 2,
    CID_ID2D1GradientStopCollection1_GetBufferPrecision = CID_ID2D1GradientStopCollection1_BASE + 3,
    CID_ID2D1GradientStopCollection1_GetColorInterpolationMode = CID_ID2D1GradientStopCollection1_BASE + 4,

    // CID for ID2D1DrawingStateBlock1
    CID_ID2D1DrawingStateBlock1_BASE,
    CID_ID2D1DrawingStateBlock1_COUNT = 2,
    CID_ID2D1DrawingStateBlock1_GetDescription = CID_ID2D1DrawingStateBlock1_BASE + 0,
    CID_ID2D1DrawingStateBlock1_SetDescription = CID_ID2D1DrawingStateBlock1_BASE + 1,

    // CID for ID2D1DeviceContext
    CID_ID2D1DeviceContext_BASE,
    CID_ID2D1DeviceContext_COUNT = 35,
    CID_ID2D1DeviceContext_CreateBitmap = CID_ID2D1DeviceContext_BASE + 0,
    CID_ID2D1DeviceContext_CreateBitmapFromWicBitmap = CID_ID2D1DeviceContext_BASE + 1,
    CID_ID2D1DeviceContext_CreateColorContext = CID_ID2D1DeviceContext_BASE + 2,
    CID_ID2D1DeviceContext_CreateColorContextFromFilename = CID_ID2D1DeviceContext_BASE + 3,
    CID_ID2D1DeviceContext_CreateColorContextFromWicColorContext = CID_ID2D1DeviceContext_BASE + 4,
    CID_ID2D1DeviceContext_CreateBitmapFromDxgiSurface = CID_ID2D1DeviceContext_BASE + 5,
    CID_ID2D1DeviceContext_CreateEffect = CID_ID2D1DeviceContext_BASE + 6,
    CID_ID2D1DeviceContext_CreateGradientStopCollection = CID_ID2D1DeviceContext_BASE + 7,
    CID_ID2D1DeviceContext_CreateImageBrush = CID_ID2D1DeviceContext_BASE + 8,
    CID_ID2D1DeviceContext_CreateBitmapBrush = CID_ID2D1DeviceContext_BASE + 9,
    CID_ID2D1DeviceContext_CreateCommandList = CID_ID2D1DeviceContext_BASE + 10,
    CID_ID2D1DeviceContext_IsDxgiFormatSupported = CID_ID2D1DeviceContext_BASE + 11,
    CID_ID2D1DeviceContext_IsBufferPrecisionSupported = CID_ID2D1DeviceContext_BASE + 12,
    CID_ID2D1DeviceContext_GetImageLocalBounds = CID_ID2D1DeviceContext_BASE + 13,
    CID_ID2D1DeviceContext_GetImageWorldBounds = CID_ID2D1DeviceContext_BASE + 14,
    CID_ID2D1DeviceContext_GetGlyphRunWorldBounds = CID_ID2D1DeviceContext_BASE + 15,
    CID_ID2D1DeviceContext_GetDevice = CID_ID2D1DeviceContext_BASE + 16,
    CID_ID2D1DeviceContext_SetTarget = CID_ID2D1DeviceContext_BASE + 17,
    CID_ID2D1DeviceContext_GetTarget = CID_ID2D1DeviceContext_BASE + 18,
    CID_ID2D1DeviceContext_SetRenderingControls = CID_ID2D1DeviceContext_BASE + 19,
    CID_ID2D1DeviceContext_GetRenderingControls = CID_ID2D1DeviceContext_BASE + 20,
    CID_ID2D1DeviceContext_SetPrimitiveBlend = CID_ID2D1DeviceContext_BASE + 21,
    CID_ID2D1DeviceContext_GetPrimitiveBlend = CID_ID2D1DeviceContext_BASE + 22,
    CID_ID2D1DeviceContext_SetUnitMode = CID_ID2D1DeviceContext_BASE + 23,
    CID_ID2D1DeviceContext_GetUnitMode = CID_ID2D1DeviceContext_BASE + 24,
    CID_ID2D1DeviceContext_DrawGlyphRun = CID_ID2D1DeviceContext_BASE + 25,
    CID_ID2D1DeviceContext_DrawImage = CID_ID2D1DeviceContext_BASE + 26,
    CID_ID2D1DeviceContext_DrawGdiMetafile = CID_ID2D1DeviceContext_BASE + 27,
    CID_ID2D1DeviceContext_DrawBitmap = CID_ID2D1DeviceContext_BASE + 28,
    CID_ID2D1DeviceContext_PushLayer = CID_ID2D1DeviceContext_BASE + 29,
    CID_ID2D1DeviceContext_InvalidateEffectInputRectangle = CID_ID2D1DeviceContext_BASE + 30,
    CID_ID2D1DeviceContext_GetEffectInvalidRectangleCount = CID_ID2D1DeviceContext_BASE + 31,
    CID_ID2D1DeviceContext_GetEffectInvalidRectangles = CID_ID2D1DeviceContext_BASE + 32,
    CID_ID2D1DeviceContext_GetEffectRequiredInputRectangles = CID_ID2D1DeviceContext_BASE + 33,
    CID_ID2D1DeviceContext_FillOpacityMask = CID_ID2D1DeviceContext_BASE + 34,

    // CID for ID2D1Device
    CID_ID2D1Device_BASE,
    CID_ID2D1Device_COUNT = 5,
    CID_ID2D1Device_CreateDeviceContext = CID_ID2D1Device_BASE + 0,
    CID_ID2D1Device_CreatePrintControl = CID_ID2D1Device_BASE + 1,
    CID_ID2D1Device_SetMaximumTextureMemory = CID_ID2D1Device_BASE + 2,
    CID_ID2D1Device_GetMaximumTextureMemory = CID_ID2D1Device_BASE + 3,
    CID_ID2D1Device_ClearResources = CID_ID2D1Device_BASE + 4,

    // CID for ID2D1Factory1
    CID_ID2D1Factory1_BASE,
    CID_ID2D1Factory1_COUNT = 10,
    CID_ID2D1Factory1_CreateDevice = CID_ID2D1Factory1_BASE + 0,
    CID_ID2D1Factory1_CreateStrokeStyle = CID_ID2D1Factory1_BASE + 1,
    CID_ID2D1Factory1_CreatePathGeometry = CID_ID2D1Factory1_BASE + 2,
    CID_ID2D1Factory1_CreateDrawingStateBlock = CID_ID2D1Factory1_BASE + 3,
    CID_ID2D1Factory1_CreateGdiMetafile = CID_ID2D1Factory1_BASE + 4,
    CID_ID2D1Factory1_RegisterEffectFromStream = CID_ID2D1Factory1_BASE + 5,
    CID_ID2D1Factory1_RegisterEffectFromString = CID_ID2D1Factory1_BASE + 6,
    CID_ID2D1Factory1_UnregisterEffect = CID_ID2D1Factory1_BASE + 7,
    CID_ID2D1Factory1_GetRegisteredEffects = CID_ID2D1Factory1_BASE + 8,
    CID_ID2D1Factory1_GetEffectProperties = CID_ID2D1Factory1_BASE + 9,

    // CID for ID2D1Multithread
    CID_ID2D1Multithread_BASE,
    CID_ID2D1Multithread_COUNT = 3,
    CID_ID2D1Multithread_GetMultithreadProtected = CID_ID2D1Multithread_BASE + 0,
    CID_ID2D1Multithread_Enter = CID_ID2D1Multithread_BASE + 1,
    CID_ID2D1Multithread_Leave = CID_ID2D1Multithread_BASE + 2,

    // CID for IDWriteFontFileLoader
    CID_IDWriteFontFileLoader_BASE,
    CID_IDWriteFontFileLoader_COUNT = 1,
    CID_IDWriteFontFileLoader_CreateStreamFromKey = CID_IDWriteFontFileLoader_BASE + 0,

    // CID for IDWriteLocalFontFileLoader
    CID_IDWriteLocalFontFileLoader_BASE,
    CID_IDWriteLocalFontFileLoader_COUNT = 3,
    CID_IDWriteLocalFontFileLoader_GetFilePathLengthFromKey = CID_IDWriteLocalFontFileLoader_BASE + 0,
    CID_IDWriteLocalFontFileLoader_GetFilePathFromKey = CID_IDWriteLocalFontFileLoader_BASE + 1,
    CID_IDWriteLocalFontFileLoader_GetLastWriteTimeFromKey = CID_IDWriteLocalFontFileLoader_BASE + 2,

    // CID for IDWriteFontFileStream
    CID_IDWriteFontFileStream_BASE,
    CID_IDWriteFontFileStream_COUNT = 4,
    CID_IDWriteFontFileStream_ReadFileFragment = CID_IDWriteFontFileStream_BASE + 0,
    CID_IDWriteFontFileStream_ReleaseFileFragment = CID_IDWriteFontFileStream_BASE + 1,
    CID_IDWriteFontFileStream_GetFileSize = CID_IDWriteFontFileStream_BASE + 2,
    CID_IDWriteFontFileStream_GetLastWriteTime = CID_IDWriteFontFileStream_BASE + 3,

    // CID for IDWriteFontFile
    CID_IDWriteFontFile_BASE,
    CID_IDWriteFontFile_COUNT = 3,
    CID_IDWriteFontFile_GetReferenceKey = CID_IDWriteFontFile_BASE + 0,
    CID_IDWriteFontFile_GetLoader = CID_IDWriteFontFile_BASE + 1,
    CID_IDWriteFontFile_Analyze = CID_IDWriteFontFile_BASE + 2,

    // CID for IDWriteRenderingParams
    CID_IDWriteRenderingParams_BASE,
    CID_IDWriteRenderingParams_COUNT = 5,
    CID_IDWriteRenderingParams_GetGamma = CID_IDWriteRenderingParams_BASE + 0,
    CID_IDWriteRenderingParams_GetEnhancedContrast = CID_IDWriteRenderingParams_BASE + 1,
    CID_IDWriteRenderingParams_GetClearTypeLevel = CID_IDWriteRenderingParams_BASE + 2,
    CID_IDWriteRenderingParams_GetPixelGeometry = CID_IDWriteRenderingParams_BASE + 3,
    CID_IDWriteRenderingParams_GetRenderingMode = CID_IDWriteRenderingParams_BASE + 4,

    // CID for IDWriteFontFace
    CID_IDWriteFontFace_BASE,
    CID_IDWriteFontFace_COUNT = 15,
    CID_IDWriteFontFace_GetType = CID_IDWriteFontFace_BASE + 0,
    CID_IDWriteFontFace_GetFiles = CID_IDWriteFontFace_BASE + 1,
    CID_IDWriteFontFace_GetIndex = CID_IDWriteFontFace_BASE + 2,
    CID_IDWriteFontFace_GetSimulations = CID_IDWriteFontFace_BASE + 3,
    CID_IDWriteFontFace_IsSymbolFont = CID_IDWriteFontFace_BASE + 4,
    CID_IDWriteFontFace_GetMetrics = CID_IDWriteFontFace_BASE + 5,
    CID_IDWriteFontFace_GetGlyphCount = CID_IDWriteFontFace_BASE + 6,
    CID_IDWriteFontFace_GetDesignGlyphMetrics = CID_IDWriteFontFace_BASE + 7,
    CID_IDWriteFontFace_GetGlyphIndices = CID_IDWriteFontFace_BASE + 8,
    CID_IDWriteFontFace_TryGetFontTable = CID_IDWriteFontFace_BASE + 9,
    CID_IDWriteFontFace_ReleaseFontTable = CID_IDWriteFontFace_BASE + 10,
    CID_IDWriteFontFace_GetGlyphRunOutline = CID_IDWriteFontFace_BASE + 11,
    CID_IDWriteFontFace_GetRecommendedRenderingMode = CID_IDWriteFontFace_BASE + 12,
    CID_IDWriteFontFace_GetGdiCompatibleMetrics = CID_IDWriteFontFace_BASE + 13,
    CID_IDWriteFontFace_GetGdiCompatibleGlyphMetrics = CID_IDWriteFontFace_BASE + 14,

    // CID for IDWriteFontCollectionLoader
    CID_IDWriteFontCollectionLoader_BASE,
    CID_IDWriteFontCollectionLoader_COUNT = 1,
    CID_IDWriteFontCollectionLoader_CreateEnumeratorFromKey = CID_IDWriteFontCollectionLoader_BASE + 0,

    // CID for IDWriteFontFileEnumerator
    CID_IDWriteFontFileEnumerator_BASE,
    CID_IDWriteFontFileEnumerator_COUNT = 2,
    CID_IDWriteFontFileEnumerator_MoveNext = CID_IDWriteFontFileEnumerator_BASE + 0,
    CID_IDWriteFontFileEnumerator_GetCurrentFontFile = CID_IDWriteFontFileEnumerator_BASE + 1,

    // CID for IDWriteLocalizedStrings
    CID_IDWriteLocalizedStrings_BASE,
    CID_IDWriteLocalizedStrings_COUNT = 6,
    CID_IDWriteLocalizedStrings_GetCount = CID_IDWriteLocalizedStrings_BASE + 0,
    CID_IDWriteLocalizedStrings_FindLocaleName = CID_IDWriteLocalizedStrings_BASE + 1,
    CID_IDWriteLocalizedStrings_GetLocaleNameLength = CID_IDWriteLocalizedStrings_BASE + 2,
    CID_IDWriteLocalizedStrings_GetLocaleName = CID_IDWriteLocalizedStrings_BASE + 3,
    CID_IDWriteLocalizedStrings_GetStringLength = CID_IDWriteLocalizedStrings_BASE + 4,
    CID_IDWriteLocalizedStrings_GetString = CID_IDWriteLocalizedStrings_BASE + 5,

    // CID for IDWriteFontCollection
    CID_IDWriteFontCollection_BASE,
    CID_IDWriteFontCollection_COUNT = 4,
    CID_IDWriteFontCollection_GetFontFamilyCount = CID_IDWriteFontCollection_BASE + 0,
    CID_IDWriteFontCollection_GetFontFamily = CID_IDWriteFontCollection_BASE + 1,
    CID_IDWriteFontCollection_FindFamilyName = CID_IDWriteFontCollection_BASE + 2,
    CID_IDWriteFontCollection_GetFontFromFontFace = CID_IDWriteFontCollection_BASE + 3,

    // CID for IDWriteFontList
    CID_IDWriteFontList_BASE,
    CID_IDWriteFontList_COUNT = 3,
    CID_IDWriteFontList_GetFontCollection = CID_IDWriteFontList_BASE + 0,
    CID_IDWriteFontList_GetFontCount = CID_IDWriteFontList_BASE + 1,
    CID_IDWriteFontList_GetFont = CID_IDWriteFontList_BASE + 2,

    // CID for IDWriteFontFamily
    CID_IDWriteFontFamily_BASE,
    CID_IDWriteFontFamily_COUNT = 3,
    CID_IDWriteFontFamily_GetFamilyNames = CID_IDWriteFontFamily_BASE + 0,
    CID_IDWriteFontFamily_GetFirstMatchingFont = CID_IDWriteFontFamily_BASE + 1,
    CID_IDWriteFontFamily_GetMatchingFonts = CID_IDWriteFontFamily_BASE + 2,

    // CID for IDWriteFont
    CID_IDWriteFont_BASE,
    CID_IDWriteFont_COUNT = 11,
    CID_IDWriteFont_GetFontFamily = CID_IDWriteFont_BASE + 0,
    CID_IDWriteFont_GetWeight = CID_IDWriteFont_BASE + 1,
    CID_IDWriteFont_GetStretch = CID_IDWriteFont_BASE + 2,
    CID_IDWriteFont_GetStyle = CID_IDWriteFont_BASE + 3,
    CID_IDWriteFont_IsSymbolFont = CID_IDWriteFont_BASE + 4,
    CID_IDWriteFont_GetFaceNames = CID_IDWriteFont_BASE + 5,
    CID_IDWriteFont_GetInformationalStrings = CID_IDWriteFont_BASE + 6,
    CID_IDWriteFont_GetSimulations = CID_IDWriteFont_BASE + 7,
    CID_IDWriteFont_GetMetrics = CID_IDWriteFont_BASE + 8,
    CID_IDWriteFont_HasCharacter = CID_IDWriteFont_BASE + 9,
    CID_IDWriteFont_CreateFontFace = CID_IDWriteFont_BASE + 10,

    // CID for IDWriteTextFormat
    CID_IDWriteTextFormat_BASE,
    CID_IDWriteTextFormat_COUNT = 25,
    CID_IDWriteTextFormat_SetTextAlignment = CID_IDWriteTextFormat_BASE + 0,
    CID_IDWriteTextFormat_SetParagraphAlignment = CID_IDWriteTextFormat_BASE + 1,
    CID_IDWriteTextFormat_SetWordWrapping = CID_IDWriteTextFormat_BASE + 2,
    CID_IDWriteTextFormat_SetReadingDirection = CID_IDWriteTextFormat_BASE + 3,
    CID_IDWriteTextFormat_SetFlowDirection = CID_IDWriteTextFormat_BASE + 4,
    CID_IDWriteTextFormat_SetIncrementalTabStop = CID_IDWriteTextFormat_BASE + 5,
    CID_IDWriteTextFormat_SetTrimming = CID_IDWriteTextFormat_BASE + 6,
    CID_IDWriteTextFormat_SetLineSpacing = CID_IDWriteTextFormat_BASE + 7,
    CID_IDWriteTextFormat_GetTextAlignment = CID_IDWriteTextFormat_BASE + 8,
    CID_IDWriteTextFormat_GetParagraphAlignment = CID_IDWriteTextFormat_BASE + 9,
    CID_IDWriteTextFormat_GetWordWrapping = CID_IDWriteTextFormat_BASE + 10,
    CID_IDWriteTextFormat_GetReadingDirection = CID_IDWriteTextFormat_BASE + 11,
    CID_IDWriteTextFormat_GetFlowDirection = CID_IDWriteTextFormat_BASE + 12,
    CID_IDWriteTextFormat_GetIncrementalTabStop = CID_IDWriteTextFormat_BASE + 13,
    CID_IDWriteTextFormat_GetTrimming = CID_IDWriteTextFormat_BASE + 14,
    CID_IDWriteTextFormat_GetLineSpacing = CID_IDWriteTextFormat_BASE + 15,
    CID_IDWriteTextFormat_GetFontCollection = CID_IDWriteTextFormat_BASE + 16,
    CID_IDWriteTextFormat_GetFontFamilyNameLength = CID_IDWriteTextFormat_BASE + 17,
    CID_IDWriteTextFormat_GetFontFamilyName = CID_IDWriteTextFormat_BASE + 18,
    CID_IDWriteTextFormat_GetFontWeight = CID_IDWriteTextFormat_BASE + 19,
    CID_IDWriteTextFormat_GetFontStyle = CID_IDWriteTextFormat_BASE + 20,
    CID_IDWriteTextFormat_GetFontStretch = CID_IDWriteTextFormat_BASE + 21,
    CID_IDWriteTextFormat_GetFontSize = CID_IDWriteTextFormat_BASE + 22,
    CID_IDWriteTextFormat_GetLocaleNameLength = CID_IDWriteTextFormat_BASE + 23,
    CID_IDWriteTextFormat_GetLocaleName = CID_IDWriteTextFormat_BASE + 24,

    // CID for IDWriteTypography
    CID_IDWriteTypography_BASE,
    CID_IDWriteTypography_COUNT = 3,
    CID_IDWriteTypography_AddFontFeature = CID_IDWriteTypography_BASE + 0,
    CID_IDWriteTypography_GetFontFeatureCount = CID_IDWriteTypography_BASE + 1,
    CID_IDWriteTypography_GetFontFeature = CID_IDWriteTypography_BASE + 2,

    // CID for IDWriteNumberSubstitution
    CID_IDWriteNumberSubstitution_BASE,
    CID_IDWriteNumberSubstitution_COUNT = 0,

    // CID for IDWriteTextAnalysisSource
    CID_IDWriteTextAnalysisSource_BASE,
    CID_IDWriteTextAnalysisSource_COUNT = 5,
    CID_IDWriteTextAnalysisSource_GetTextAtPosition = CID_IDWriteTextAnalysisSource_BASE + 0,
    CID_IDWriteTextAnalysisSource_GetTextBeforePosition = CID_IDWriteTextAnalysisSource_BASE + 1,
    CID_IDWriteTextAnalysisSource_GetParagraphReadingDirection = CID_IDWriteTextAnalysisSource_BASE + 2,
    CID_IDWriteTextAnalysisSource_GetLocaleName = CID_IDWriteTextAnalysisSource_BASE + 3,
    CID_IDWriteTextAnalysisSource_GetNumberSubstitution = CID_IDWriteTextAnalysisSource_BASE + 4,

    // CID for IDWriteTextAnalysisSink
    CID_IDWriteTextAnalysisSink_BASE,
    CID_IDWriteTextAnalysisSink_COUNT = 4,
    CID_IDWriteTextAnalysisSink_SetScriptAnalysis = CID_IDWriteTextAnalysisSink_BASE + 0,
    CID_IDWriteTextAnalysisSink_SetLineBreakpoints = CID_IDWriteTextAnalysisSink_BASE + 1,
    CID_IDWriteTextAnalysisSink_SetBidiLevel = CID_IDWriteTextAnalysisSink_BASE + 2,
    CID_IDWriteTextAnalysisSink_SetNumberSubstitution = CID_IDWriteTextAnalysisSink_BASE + 3,

    // CID for IDWriteTextAnalyzer
    CID_IDWriteTextAnalyzer_BASE,
    CID_IDWriteTextAnalyzer_COUNT = 7,
    CID_IDWriteTextAnalyzer_AnalyzeScript = CID_IDWriteTextAnalyzer_BASE + 0,
    CID_IDWriteTextAnalyzer_AnalyzeBidi = CID_IDWriteTextAnalyzer_BASE + 1,
    CID_IDWriteTextAnalyzer_AnalyzeNumberSubstitution = CID_IDWriteTextAnalyzer_BASE + 2,
    CID_IDWriteTextAnalyzer_AnalyzeLineBreakpoints = CID_IDWriteTextAnalyzer_BASE + 3,
    CID_IDWriteTextAnalyzer_GetGlyphs = CID_IDWriteTextAnalyzer_BASE + 4,
    CID_IDWriteTextAnalyzer_GetGlyphPlacements = CID_IDWriteTextAnalyzer_BASE + 5,
    CID_IDWriteTextAnalyzer_GetGdiCompatibleGlyphPlacements = CID_IDWriteTextAnalyzer_BASE + 6,

    // CID for IDWriteInlineObject
    CID_IDWriteInlineObject_BASE,
    CID_IDWriteInlineObject_COUNT = 4,
    CID_IDWriteInlineObject_Draw = CID_IDWriteInlineObject_BASE + 0,
    CID_IDWriteInlineObject_GetMetrics = CID_IDWriteInlineObject_BASE + 1,
    CID_IDWriteInlineObject_GetOverhangMetrics = CID_IDWriteInlineObject_BASE + 2,
    CID_IDWriteInlineObject_GetBreakConditions = CID_IDWriteInlineObject_BASE + 3,

    // CID for IDWritePixelSnapping
    CID_IDWritePixelSnapping_BASE,
    CID_IDWritePixelSnapping_COUNT = 3,
    CID_IDWritePixelSnapping_IsPixelSnappingDisabled = CID_IDWritePixelSnapping_BASE + 0,
    CID_IDWritePixelSnapping_GetCurrentTransform = CID_IDWritePixelSnapping_BASE + 1,
    CID_IDWritePixelSnapping_GetPixelsPerDip = CID_IDWritePixelSnapping_BASE + 2,

    // CID for IDWriteTextRenderer
    CID_IDWriteTextRenderer_BASE,
    CID_IDWriteTextRenderer_COUNT = 4,
    CID_IDWriteTextRenderer_DrawGlyphRun = CID_IDWriteTextRenderer_BASE + 0,
    CID_IDWriteTextRenderer_DrawUnderline = CID_IDWriteTextRenderer_BASE + 1,
    CID_IDWriteTextRenderer_DrawStrikethrough = CID_IDWriteTextRenderer_BASE + 2,
    CID_IDWriteTextRenderer_DrawInlineObject = CID_IDWriteTextRenderer_BASE + 3,

    // CID for IDWriteTextLayout
    CID_IDWriteTextLayout_BASE,
    CID_IDWriteTextLayout_COUNT = 39,
    CID_IDWriteTextLayout_SetMaxWidth = CID_IDWriteTextLayout_BASE + 0,
    CID_IDWriteTextLayout_SetMaxHeight = CID_IDWriteTextLayout_BASE + 1,
    CID_IDWriteTextLayout_SetFontCollection = CID_IDWriteTextLayout_BASE + 2,
    CID_IDWriteTextLayout_SetFontFamilyName = CID_IDWriteTextLayout_BASE + 3,
    CID_IDWriteTextLayout_SetFontWeight = CID_IDWriteTextLayout_BASE + 4,
    CID_IDWriteTextLayout_SetFontStyle = CID_IDWriteTextLayout_BASE + 5,
    CID_IDWriteTextLayout_SetFontStretch = CID_IDWriteTextLayout_BASE + 6,
    CID_IDWriteTextLayout_SetFontSize = CID_IDWriteTextLayout_BASE + 7,
    CID_IDWriteTextLayout_SetUnderline = CID_IDWriteTextLayout_BASE + 8,
    CID_IDWriteTextLayout_SetStrikethrough = CID_IDWriteTextLayout_BASE + 9,
    CID_IDWriteTextLayout_SetDrawingEffect = CID_IDWriteTextLayout_BASE + 10,
    CID_IDWriteTextLayout_SetInlineObject = CID_IDWriteTextLayout_BASE + 11,
    CID_IDWriteTextLayout_SetTypography = CID_IDWriteTextLayout_BASE + 12,
    CID_IDWriteTextLayout_SetLocaleName = CID_IDWriteTextLayout_BASE + 13,
    CID_IDWriteTextLayout_GetMaxWidth = CID_IDWriteTextLayout_BASE + 14,
    CID_IDWriteTextLayout_GetMaxHeight = CID_IDWriteTextLayout_BASE + 15,
    CID_IDWriteTextLayout_GetFontCollection = CID_IDWriteTextLayout_BASE + 16,
    CID_IDWriteTextLayout_GetFontFamilyNameLength = CID_IDWriteTextLayout_BASE + 17,
    CID_IDWriteTextLayout_GetFontFamilyName = CID_IDWriteTextLayout_BASE + 18,
    CID_IDWriteTextLayout_GetFontWeight = CID_IDWriteTextLayout_BASE + 19,
    CID_IDWriteTextLayout_GetFontStyle = CID_IDWriteTextLayout_BASE + 20,
    CID_IDWriteTextLayout_GetFontStretch = CID_IDWriteTextLayout_BASE + 21,
    CID_IDWriteTextLayout_GetFontSize = CID_IDWriteTextLayout_BASE + 22,
    CID_IDWriteTextLayout_GetUnderline = CID_IDWriteTextLayout_BASE + 23,
    CID_IDWriteTextLayout_GetStrikethrough = CID_IDWriteTextLayout_BASE + 24,
    CID_IDWriteTextLayout_GetDrawingEffect = CID_IDWriteTextLayout_BASE + 25,
    CID_IDWriteTextLayout_GetInlineObject = CID_IDWriteTextLayout_BASE + 26,
    CID_IDWriteTextLayout_GetTypography = CID_IDWriteTextLayout_BASE + 27,
    CID_IDWriteTextLayout_GetLocaleNameLength = CID_IDWriteTextLayout_BASE + 28,
    CID_IDWriteTextLayout_GetLocaleName = CID_IDWriteTextLayout_BASE + 29,
    CID_IDWriteTextLayout_Draw = CID_IDWriteTextLayout_BASE + 30,
    CID_IDWriteTextLayout_GetLineMetrics = CID_IDWriteTextLayout_BASE + 31,
    CID_IDWriteTextLayout_GetMetrics = CID_IDWriteTextLayout_BASE + 32,
    CID_IDWriteTextLayout_GetOverhangMetrics = CID_IDWriteTextLayout_BASE + 33,
    CID_IDWriteTextLayout_GetClusterMetrics = CID_IDWriteTextLayout_BASE + 34,
    CID_IDWriteTextLayout_DetermineMinWidth = CID_IDWriteTextLayout_BASE + 35,
    CID_IDWriteTextLayout_HitTestPoint = CID_IDWriteTextLayout_BASE + 36,
    CID_IDWriteTextLayout_HitTestTextPosition = CID_IDWriteTextLayout_BASE + 37,
    CID_IDWriteTextLayout_HitTestTextRange = CID_IDWriteTextLayout_BASE + 38,

    // CID for IDWriteBitmapRenderTarget
    CID_IDWriteBitmapRenderTarget_BASE,
    CID_IDWriteBitmapRenderTarget_COUNT = 8,
    CID_IDWriteBitmapRenderTarget_DrawGlyphRun = CID_IDWriteBitmapRenderTarget_BASE + 0,
    CID_IDWriteBitmapRenderTarget_GetMemoryDC = CID_IDWriteBitmapRenderTarget_BASE + 1,
    CID_IDWriteBitmapRenderTarget_GetPixelsPerDip = CID_IDWriteBitmapRenderTarget_BASE + 2,
    CID_IDWriteBitmapRenderTarget_SetPixelsPerDip = CID_IDWriteBitmapRenderTarget_BASE + 3,
    CID_IDWriteBitmapRenderTarget_GetCurrentTransform = CID_IDWriteBitmapRenderTarget_BASE + 4,
    CID_IDWriteBitmapRenderTarget_SetCurrentTransform = CID_IDWriteBitmapRenderTarget_BASE + 5,
    CID_IDWriteBitmapRenderTarget_GetSize = CID_IDWriteBitmapRenderTarget_BASE + 6,
    CID_IDWriteBitmapRenderTarget_Resize = CID_IDWriteBitmapRenderTarget_BASE + 7,

    // CID for IDWriteGdiInterop
    CID_IDWriteGdiInterop_BASE,
    CID_IDWriteGdiInterop_COUNT = 5,
    CID_IDWriteGdiInterop_CreateFontFromLOGFONT = CID_IDWriteGdiInterop_BASE + 0,
    CID_IDWriteGdiInterop_ConvertFontToLOGFONT = CID_IDWriteGdiInterop_BASE + 1,
    CID_IDWriteGdiInterop_ConvertFontFaceToLOGFONT = CID_IDWriteGdiInterop_BASE + 2,
    CID_IDWriteGdiInterop_CreateFontFaceFromHdc = CID_IDWriteGdiInterop_BASE + 3,
    CID_IDWriteGdiInterop_CreateBitmapRenderTarget = CID_IDWriteGdiInterop_BASE + 4,

    // CID for IDWriteGlyphRunAnalysis
    CID_IDWriteGlyphRunAnalysis_BASE,
    CID_IDWriteGlyphRunAnalysis_COUNT = 3,
    CID_IDWriteGlyphRunAnalysis_GetAlphaTextureBounds = CID_IDWriteGlyphRunAnalysis_BASE + 0,
    CID_IDWriteGlyphRunAnalysis_CreateAlphaTexture = CID_IDWriteGlyphRunAnalysis_BASE + 1,
    CID_IDWriteGlyphRunAnalysis_GetAlphaBlendParams = CID_IDWriteGlyphRunAnalysis_BASE + 2,

    // CID for IDWriteFactory
    CID_IDWriteFactory_BASE,
    CID_IDWriteFactory_COUNT = 21,
    CID_IDWriteFactory_GetSystemFontCollection = CID_IDWriteFactory_BASE + 0,
    CID_IDWriteFactory_CreateCustomFontCollection = CID_IDWriteFactory_BASE + 1,
    CID_IDWriteFactory_RegisterFontCollectionLoader = CID_IDWriteFactory_BASE + 2,
    CID_IDWriteFactory_UnregisterFontCollectionLoader = CID_IDWriteFactory_BASE + 3,
    CID_IDWriteFactory_CreateFontFileReference = CID_IDWriteFactory_BASE + 4,
    CID_IDWriteFactory_CreateCustomFontFileReference = CID_IDWriteFactory_BASE + 5,
    CID_IDWriteFactory_CreateFontFace = CID_IDWriteFactory_BASE + 6,
    CID_IDWriteFactory_CreateRenderingParams = CID_IDWriteFactory_BASE + 7,
    CID_IDWriteFactory_CreateMonitorRenderingParams = CID_IDWriteFactory_BASE + 8,
    CID_IDWriteFactory_CreateCustomRenderingParams = CID_IDWriteFactory_BASE + 9,
    CID_IDWriteFactory_RegisterFontFileLoader = CID_IDWriteFactory_BASE + 10,
    CID_IDWriteFactory_UnregisterFontFileLoader = CID_IDWriteFactory_BASE + 11,
    CID_IDWriteFactory_CreateTextFormat = CID_IDWriteFactory_BASE + 12,
    CID_IDWriteFactory_CreateTypography = CID_IDWriteFactory_BASE + 13,
    CID_IDWriteFactory_GetGdiInterop = CID_IDWriteFactory_BASE + 14,
    CID_IDWriteFactory_CreateTextLayout = CID_IDWriteFactory_BASE + 15,
    CID_IDWriteFactory_CreateGdiCompatibleTextLayout = CID_IDWriteFactory_BASE + 16,
    CID_IDWriteFactory_CreateEllipsisTrimmingSign = CID_IDWriteFactory_BASE + 17,
    CID_IDWriteFactory_CreateTextAnalyzer = CID_IDWriteFactory_BASE + 18,
    CID_IDWriteFactory_CreateNumberSubstitution = CID_IDWriteFactory_BASE + 19,
    CID_IDWriteFactory_CreateGlyphRunAnalysis = CID_IDWriteFactory_BASE + 20,

    CID_TOTAL_COUNT,
    CID_INVALID = 0xFFFFFFFF,
}; // end of enum definition

extern const char * const g_D2DCallIDText;
