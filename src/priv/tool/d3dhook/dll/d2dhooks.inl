// script generated file. Do _NOT_ edit.

// ==============================================================================
// ID2D1Resource
// ==============================================================================
class D2D1ResourceHook : public HookBase<ID2D1Resource>
{

protected:

    D2D1ResourceHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1ResourceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1ResourceHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * typedPtr = (D2D1ResourceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const;
    NullPtr<void (D2D1ResourceHook::*)(_Outptr_ ID2D1Factory ** &) const> _GetFactory_pre_ptr;
    NullPtr<void (D2D1ResourceHook::*)(_Outptr_ ID2D1Factory **) const> _GetFactory_post_ptr;

};

// ==============================================================================
// ID2D1Image
// ==============================================================================
class D2D1ImageHook : public HookBase<ID2D1Image>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1ImageHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1ImageHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1ImageHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ImageHook * typedPtr = (D2D1ImageHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

};

// ==============================================================================
// ID2D1Bitmap
// ==============================================================================
class D2D1BitmapHook : public HookBase<ID2D1Bitmap>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1ImageHook & _D2D1Image;

protected:

    D2D1BitmapHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1ImageHook & D2D1Image, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Image(D2D1Image)
    {
    }

    ~D2D1BitmapHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1ImageHook * D2D1Image = (D2D1ImageHook *)unknown.GetHookedObj(__uuidof(ID2D1Image));
        if (nullptr == D2D1Image) return nullptr;

        try
        {
            return new D2D1BitmapHook(unknown, *D2D1Resource, *D2D1Image, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1BitmapHook * typedPtr = (D2D1BitmapHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual D2D1_SIZE_F STDMETHODCALLTYPE GetSize() const;
    NullPtr<void (D2D1BitmapHook::*)() const> _GetSize_pre_ptr;
    NullPtr<void (D2D1BitmapHook::*)(D2D1_SIZE_F) const> _GetSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_SIZE_U STDMETHODCALLTYPE GetPixelSize() const;
    NullPtr<void (D2D1BitmapHook::*)() const> _GetPixelSize_pre_ptr;
    NullPtr<void (D2D1BitmapHook::*)(D2D1_SIZE_U) const> _GetPixelSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_PIXEL_FORMAT STDMETHODCALLTYPE GetPixelFormat() const;
    NullPtr<void (D2D1BitmapHook::*)() const> _GetPixelFormat_pre_ptr;
    NullPtr<void (D2D1BitmapHook::*)(D2D1_PIXEL_FORMAT) const> _GetPixelFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY) const;
    NullPtr<void (D2D1BitmapHook::*)(_Out_ FLOAT * &, _Out_ FLOAT * &) const> _GetDpi_pre_ptr;
    NullPtr<void (D2D1BitmapHook::*)(_Out_ FLOAT *, _Out_ FLOAT *) const> _GetDpi_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CopyFromBitmap(_In_opt_ CONST D2D1_POINT_2U * destPoint, _In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_U * srcRect);
    NullPtr<void (D2D1BitmapHook::*)(_In_opt_ CONST D2D1_POINT_2U * &, _In_ ID2D1Bitmap * &, _In_opt_ CONST D2D1_RECT_U * &)> _CopyFromBitmap_pre_ptr;
    NullPtr<void (D2D1BitmapHook::*)(HRESULT, _In_opt_ CONST D2D1_POINT_2U *, _In_ ID2D1Bitmap *, _In_opt_ CONST D2D1_RECT_U *)> _CopyFromBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CopyFromRenderTarget(_In_opt_ CONST D2D1_POINT_2U * destPoint, _In_ ID2D1RenderTarget * renderTarget, _In_opt_ CONST D2D1_RECT_U * srcRect);
    NullPtr<void (D2D1BitmapHook::*)(_In_opt_ CONST D2D1_POINT_2U * &, _In_ ID2D1RenderTarget * &, _In_opt_ CONST D2D1_RECT_U * &)> _CopyFromRenderTarget_pre_ptr;
    NullPtr<void (D2D1BitmapHook::*)(HRESULT, _In_opt_ CONST D2D1_POINT_2U *, _In_ ID2D1RenderTarget *, _In_opt_ CONST D2D1_RECT_U *)> _CopyFromRenderTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CopyFromMemory(_In_opt_ CONST D2D1_RECT_U * dstRect, _In_ CONST void * srcData, UINT32 pitch);
    NullPtr<void (D2D1BitmapHook::*)(_In_opt_ CONST D2D1_RECT_U * &, _In_ CONST void * &, UINT32 &)> _CopyFromMemory_pre_ptr;
    NullPtr<void (D2D1BitmapHook::*)(HRESULT, _In_opt_ CONST D2D1_RECT_U *, _In_ CONST void *, UINT32)> _CopyFromMemory_post_ptr;

};

// ==============================================================================
// ID2D1GradientStopCollection
// ==============================================================================
class D2D1GradientStopCollectionHook : public HookBase<ID2D1GradientStopCollection>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1GradientStopCollectionHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1GradientStopCollectionHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1GradientStopCollectionHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1GradientStopCollectionHook * typedPtr = (D2D1GradientStopCollectionHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetGradientStopCount() const;
    NullPtr<void (D2D1GradientStopCollectionHook::*)() const> _GetGradientStopCount_pre_ptr;
    NullPtr<void (D2D1GradientStopCollectionHook::*)(UINT32) const> _GetGradientStopCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetGradientStops(_Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP * gradientStops, UINT32 gradientStopsCount) const;
    NullPtr<void (D2D1GradientStopCollectionHook::*)(_Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP * &, UINT32 &) const> _GetGradientStops_pre_ptr;
    NullPtr<void (D2D1GradientStopCollectionHook::*)(_Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP *, UINT32) const> _GetGradientStops_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_GAMMA STDMETHODCALLTYPE GetColorInterpolationGamma() const;
    NullPtr<void (D2D1GradientStopCollectionHook::*)() const> _GetColorInterpolationGamma_pre_ptr;
    NullPtr<void (D2D1GradientStopCollectionHook::*)(D2D1_GAMMA) const> _GetColorInterpolationGamma_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_EXTEND_MODE STDMETHODCALLTYPE GetExtendMode() const;
    NullPtr<void (D2D1GradientStopCollectionHook::*)() const> _GetExtendMode_pre_ptr;
    NullPtr<void (D2D1GradientStopCollectionHook::*)(D2D1_EXTEND_MODE) const> _GetExtendMode_post_ptr;

};

// ==============================================================================
// ID2D1Brush
// ==============================================================================
class D2D1BrushHook : public HookBase<ID2D1Brush>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1BrushHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1BrushHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1BrushHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1BrushHook * typedPtr = (D2D1BrushHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetOpacity(FLOAT opacity);
    NullPtr<void (D2D1BrushHook::*)(FLOAT &)> _SetOpacity_pre_ptr;
    NullPtr<void (D2D1BrushHook::*)(FLOAT)> _SetOpacity_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform);
    NullPtr<void (D2D1BrushHook::*)(_In_ CONST D2D1_MATRIX_3X2_F * &)> _SetTransform_pre_ptr;
    NullPtr<void (D2D1BrushHook::*)(_In_ CONST D2D1_MATRIX_3X2_F *)> _SetTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetOpacity() const;
    NullPtr<void (D2D1BrushHook::*)() const> _GetOpacity_pre_ptr;
    NullPtr<void (D2D1BrushHook::*)(FLOAT) const> _GetOpacity_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const;
    NullPtr<void (D2D1BrushHook::*)(_Out_ D2D1_MATRIX_3X2_F * &) const> _GetTransform_pre_ptr;
    NullPtr<void (D2D1BrushHook::*)(_Out_ D2D1_MATRIX_3X2_F *) const> _GetTransform_post_ptr;

};

// ==============================================================================
// ID2D1BitmapBrush
// ==============================================================================
class D2D1BitmapBrushHook : public HookBase<ID2D1BitmapBrush>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1BrushHook & _D2D1Brush;

protected:

    D2D1BitmapBrushHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1BrushHook & D2D1Brush, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Brush(D2D1Brush)
    {
    }

    ~D2D1BitmapBrushHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1BrushHook * D2D1Brush = (D2D1BrushHook *)unknown.GetHookedObj(__uuidof(ID2D1Brush));
        if (nullptr == D2D1Brush) return nullptr;

        try
        {
            return new D2D1BitmapBrushHook(unknown, *D2D1Resource, *D2D1Brush, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1BitmapBrushHook * typedPtr = (D2D1BitmapBrushHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetOpacity(FLOAT opacity)
    {
        return _D2D1Brush.SetOpacity(opacity);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1Brush.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetOpacity() const
    {
        return _D2D1Brush.GetOpacity();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1Brush.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetExtendModeX(D2D1_EXTEND_MODE extendModeX);
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_EXTEND_MODE &)> _SetExtendModeX_pre_ptr;
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_EXTEND_MODE)> _SetExtendModeX_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetExtendModeY(D2D1_EXTEND_MODE extendModeY);
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_EXTEND_MODE &)> _SetExtendModeY_pre_ptr;
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_EXTEND_MODE)> _SetExtendModeY_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetInterpolationMode(D2D1_BITMAP_INTERPOLATION_MODE interpolationMode);
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_BITMAP_INTERPOLATION_MODE &)> _SetInterpolationMode_pre_ptr;
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_BITMAP_INTERPOLATION_MODE)> _SetInterpolationMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetBitmap(_In_opt_ ID2D1Bitmap * bitmap);
    NullPtr<void (D2D1BitmapBrushHook::*)(_In_opt_ ID2D1Bitmap * &)> _SetBitmap_pre_ptr;
    NullPtr<void (D2D1BitmapBrushHook::*)(_In_opt_ ID2D1Bitmap *)> _SetBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_EXTEND_MODE STDMETHODCALLTYPE GetExtendModeX() const;
    NullPtr<void (D2D1BitmapBrushHook::*)() const> _GetExtendModeX_pre_ptr;
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_EXTEND_MODE) const> _GetExtendModeX_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_EXTEND_MODE STDMETHODCALLTYPE GetExtendModeY() const;
    NullPtr<void (D2D1BitmapBrushHook::*)() const> _GetExtendModeY_pre_ptr;
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_EXTEND_MODE) const> _GetExtendModeY_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_BITMAP_INTERPOLATION_MODE STDMETHODCALLTYPE GetInterpolationMode() const;
    NullPtr<void (D2D1BitmapBrushHook::*)() const> _GetInterpolationMode_pre_ptr;
    NullPtr<void (D2D1BitmapBrushHook::*)(D2D1_BITMAP_INTERPOLATION_MODE) const> _GetInterpolationMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetBitmap(_Outptr_ ID2D1Bitmap ** bitmap) const;
    NullPtr<void (D2D1BitmapBrushHook::*)(_Outptr_ ID2D1Bitmap ** &) const> _GetBitmap_pre_ptr;
    NullPtr<void (D2D1BitmapBrushHook::*)(_Outptr_ ID2D1Bitmap **) const> _GetBitmap_post_ptr;

};

// ==============================================================================
// ID2D1SolidColorBrush
// ==============================================================================
class D2D1SolidColorBrushHook : public HookBase<ID2D1SolidColorBrush>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1BrushHook & _D2D1Brush;

protected:

    D2D1SolidColorBrushHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1BrushHook & D2D1Brush, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Brush(D2D1Brush)
    {
    }

    ~D2D1SolidColorBrushHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1BrushHook * D2D1Brush = (D2D1BrushHook *)unknown.GetHookedObj(__uuidof(ID2D1Brush));
        if (nullptr == D2D1Brush) return nullptr;

        try
        {
            return new D2D1SolidColorBrushHook(unknown, *D2D1Resource, *D2D1Brush, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1SolidColorBrushHook * typedPtr = (D2D1SolidColorBrushHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetOpacity(FLOAT opacity)
    {
        return _D2D1Brush.SetOpacity(opacity);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1Brush.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetOpacity() const
    {
        return _D2D1Brush.GetOpacity();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1Brush.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetColor(_In_ CONST D2D1_COLOR_F * color);
    NullPtr<void (D2D1SolidColorBrushHook::*)(_In_ CONST D2D1_COLOR_F * &)> _SetColor_pre_ptr;
    NullPtr<void (D2D1SolidColorBrushHook::*)(_In_ CONST D2D1_COLOR_F *)> _SetColor_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_COLOR_F STDMETHODCALLTYPE GetColor() const;
    NullPtr<void (D2D1SolidColorBrushHook::*)() const> _GetColor_pre_ptr;
    NullPtr<void (D2D1SolidColorBrushHook::*)(D2D1_COLOR_F) const> _GetColor_post_ptr;

};

// ==============================================================================
// ID2D1LinearGradientBrush
// ==============================================================================
class D2D1LinearGradientBrushHook : public HookBase<ID2D1LinearGradientBrush>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1BrushHook & _D2D1Brush;

protected:

    D2D1LinearGradientBrushHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1BrushHook & D2D1Brush, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Brush(D2D1Brush)
    {
    }

    ~D2D1LinearGradientBrushHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1BrushHook * D2D1Brush = (D2D1BrushHook *)unknown.GetHookedObj(__uuidof(ID2D1Brush));
        if (nullptr == D2D1Brush) return nullptr;

        try
        {
            return new D2D1LinearGradientBrushHook(unknown, *D2D1Resource, *D2D1Brush, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1LinearGradientBrushHook * typedPtr = (D2D1LinearGradientBrushHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetOpacity(FLOAT opacity)
    {
        return _D2D1Brush.SetOpacity(opacity);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1Brush.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetOpacity() const
    {
        return _D2D1Brush.GetOpacity();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1Brush.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetStartPoint(D2D1_POINT_2F startPoint);
    NullPtr<void (D2D1LinearGradientBrushHook::*)(D2D1_POINT_2F &)> _SetStartPoint_pre_ptr;
    NullPtr<void (D2D1LinearGradientBrushHook::*)(D2D1_POINT_2F)> _SetStartPoint_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetEndPoint(D2D1_POINT_2F endPoint);
    NullPtr<void (D2D1LinearGradientBrushHook::*)(D2D1_POINT_2F &)> _SetEndPoint_pre_ptr;
    NullPtr<void (D2D1LinearGradientBrushHook::*)(D2D1_POINT_2F)> _SetEndPoint_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_POINT_2F STDMETHODCALLTYPE GetStartPoint() const;
    NullPtr<void (D2D1LinearGradientBrushHook::*)() const> _GetStartPoint_pre_ptr;
    NullPtr<void (D2D1LinearGradientBrushHook::*)(D2D1_POINT_2F) const> _GetStartPoint_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_POINT_2F STDMETHODCALLTYPE GetEndPoint() const;
    NullPtr<void (D2D1LinearGradientBrushHook::*)() const> _GetEndPoint_pre_ptr;
    NullPtr<void (D2D1LinearGradientBrushHook::*)(D2D1_POINT_2F) const> _GetEndPoint_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetGradientStopCollection(_Outptr_ ID2D1GradientStopCollection ** gradientStopCollection) const;
    NullPtr<void (D2D1LinearGradientBrushHook::*)(_Outptr_ ID2D1GradientStopCollection ** &) const> _GetGradientStopCollection_pre_ptr;
    NullPtr<void (D2D1LinearGradientBrushHook::*)(_Outptr_ ID2D1GradientStopCollection **) const> _GetGradientStopCollection_post_ptr;

};

// ==============================================================================
// ID2D1RadialGradientBrush
// ==============================================================================
class D2D1RadialGradientBrushHook : public HookBase<ID2D1RadialGradientBrush>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1BrushHook & _D2D1Brush;

protected:

    D2D1RadialGradientBrushHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1BrushHook & D2D1Brush, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Brush(D2D1Brush)
    {
    }

    ~D2D1RadialGradientBrushHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1BrushHook * D2D1Brush = (D2D1BrushHook *)unknown.GetHookedObj(__uuidof(ID2D1Brush));
        if (nullptr == D2D1Brush) return nullptr;

        try
        {
            return new D2D1RadialGradientBrushHook(unknown, *D2D1Resource, *D2D1Brush, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1RadialGradientBrushHook * typedPtr = (D2D1RadialGradientBrushHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetOpacity(FLOAT opacity)
    {
        return _D2D1Brush.SetOpacity(opacity);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1Brush.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetOpacity() const
    {
        return _D2D1Brush.GetOpacity();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1Brush.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetCenter(D2D1_POINT_2F center);
    NullPtr<void (D2D1RadialGradientBrushHook::*)(D2D1_POINT_2F &)> _SetCenter_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(D2D1_POINT_2F)> _SetCenter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetGradientOriginOffset(D2D1_POINT_2F gradientOriginOffset);
    NullPtr<void (D2D1RadialGradientBrushHook::*)(D2D1_POINT_2F &)> _SetGradientOriginOffset_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(D2D1_POINT_2F)> _SetGradientOriginOffset_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetRadiusX(FLOAT radiusX);
    NullPtr<void (D2D1RadialGradientBrushHook::*)(FLOAT &)> _SetRadiusX_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(FLOAT)> _SetRadiusX_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetRadiusY(FLOAT radiusY);
    NullPtr<void (D2D1RadialGradientBrushHook::*)(FLOAT &)> _SetRadiusY_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(FLOAT)> _SetRadiusY_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_POINT_2F STDMETHODCALLTYPE GetCenter() const;
    NullPtr<void (D2D1RadialGradientBrushHook::*)() const> _GetCenter_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(D2D1_POINT_2F) const> _GetCenter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_POINT_2F STDMETHODCALLTYPE GetGradientOriginOffset() const;
    NullPtr<void (D2D1RadialGradientBrushHook::*)() const> _GetGradientOriginOffset_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(D2D1_POINT_2F) const> _GetGradientOriginOffset_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetRadiusX() const;
    NullPtr<void (D2D1RadialGradientBrushHook::*)() const> _GetRadiusX_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(FLOAT) const> _GetRadiusX_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetRadiusY() const;
    NullPtr<void (D2D1RadialGradientBrushHook::*)() const> _GetRadiusY_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(FLOAT) const> _GetRadiusY_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetGradientStopCollection(_Outptr_ ID2D1GradientStopCollection ** gradientStopCollection) const;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(_Outptr_ ID2D1GradientStopCollection ** &) const> _GetGradientStopCollection_pre_ptr;
    NullPtr<void (D2D1RadialGradientBrushHook::*)(_Outptr_ ID2D1GradientStopCollection **) const> _GetGradientStopCollection_post_ptr;

};

// ==============================================================================
// ID2D1StrokeStyle
// ==============================================================================
class D2D1StrokeStyleHook : public HookBase<ID2D1StrokeStyle>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1StrokeStyleHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1StrokeStyleHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1StrokeStyleHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1StrokeStyleHook * typedPtr = (D2D1StrokeStyleHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual D2D1_CAP_STYLE STDMETHODCALLTYPE GetStartCap() const;
    NullPtr<void (D2D1StrokeStyleHook::*)() const> _GetStartCap_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(D2D1_CAP_STYLE) const> _GetStartCap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_CAP_STYLE STDMETHODCALLTYPE GetEndCap() const;
    NullPtr<void (D2D1StrokeStyleHook::*)() const> _GetEndCap_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(D2D1_CAP_STYLE) const> _GetEndCap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_CAP_STYLE STDMETHODCALLTYPE GetDashCap() const;
    NullPtr<void (D2D1StrokeStyleHook::*)() const> _GetDashCap_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(D2D1_CAP_STYLE) const> _GetDashCap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetMiterLimit() const;
    NullPtr<void (D2D1StrokeStyleHook::*)() const> _GetMiterLimit_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(FLOAT) const> _GetMiterLimit_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_LINE_JOIN STDMETHODCALLTYPE GetLineJoin() const;
    NullPtr<void (D2D1StrokeStyleHook::*)() const> _GetLineJoin_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(D2D1_LINE_JOIN) const> _GetLineJoin_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetDashOffset() const;
    NullPtr<void (D2D1StrokeStyleHook::*)() const> _GetDashOffset_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(FLOAT) const> _GetDashOffset_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_DASH_STYLE STDMETHODCALLTYPE GetDashStyle() const;
    NullPtr<void (D2D1StrokeStyleHook::*)() const> _GetDashStyle_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(D2D1_DASH_STYLE) const> _GetDashStyle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetDashesCount() const;
    NullPtr<void (D2D1StrokeStyleHook::*)() const> _GetDashesCount_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(UINT32) const> _GetDashesCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDashes(_Out_writes_(dashesCount) FLOAT * dashes, UINT32 dashesCount) const;
    NullPtr<void (D2D1StrokeStyleHook::*)(_Out_writes_(dashesCount) FLOAT * &, UINT32 &) const> _GetDashes_pre_ptr;
    NullPtr<void (D2D1StrokeStyleHook::*)(_Out_writes_(dashesCount) FLOAT *, UINT32) const> _GetDashes_post_ptr;

};

// ==============================================================================
// ID2D1Geometry
// ==============================================================================
class D2D1GeometryHook : public HookBase<ID2D1Geometry>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1GeometryHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1GeometryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1GeometryHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1GeometryHook * typedPtr = (D2D1GeometryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetBounds(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, _Out_ D2D1_RECT_F * bounds) const;
    NullPtr<void (D2D1GeometryHook::*)(_In_opt_ CONST D2D1_MATRIX_3X2_F * &, _Out_ D2D1_RECT_F * &) const> _GetBounds_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, _In_opt_ CONST D2D1_MATRIX_3X2_F *, _Out_ D2D1_RECT_F *) const> _GetBounds_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetWidenedBounds(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_RECT_F * bounds) const;
    NullPtr<void (D2D1GeometryHook::*)(FLOAT &, _In_opt_ ID2D1StrokeStyle * &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _Out_ D2D1_RECT_F * &) const> _GetWidenedBounds_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, FLOAT, _In_opt_ ID2D1StrokeStyle *, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _Out_ D2D1_RECT_F *) const> _GetWidenedBounds_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE StrokeContainsPoint(D2D1_POINT_2F point, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const;
    NullPtr<void (D2D1GeometryHook::*)(D2D1_POINT_2F &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _Out_ BOOL * &) const> _StrokeContainsPoint_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, D2D1_POINT_2F, FLOAT, _In_opt_ ID2D1StrokeStyle *, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _Out_ BOOL *) const> _StrokeContainsPoint_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FillContainsPoint(D2D1_POINT_2F point, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const;
    NullPtr<void (D2D1GeometryHook::*)(D2D1_POINT_2F &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _Out_ BOOL * &) const> _FillContainsPoint_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, D2D1_POINT_2F, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _Out_ BOOL *) const> _FillContainsPoint_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CompareWithGeometry(_In_ ID2D1Geometry * inputGeometry, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _Out_ D2D1_GEOMETRY_RELATION * relation) const;
    NullPtr<void (D2D1GeometryHook::*)(_In_ ID2D1Geometry * &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _Out_ D2D1_GEOMETRY_RELATION * &) const> _CompareWithGeometry_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, _In_ ID2D1Geometry *, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _Out_ D2D1_GEOMETRY_RELATION *) const> _CompareWithGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const;
    NullPtr<void (D2D1GeometryHook::*)(D2D1_GEOMETRY_SIMPLIFICATION_OPTION &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _In_ ID2D1SimplifiedGeometrySink * &) const> _Simplify_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, D2D1_GEOMETRY_SIMPLIFICATION_OPTION, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _In_ ID2D1SimplifiedGeometrySink *) const> _Simplify_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Tessellate(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1TessellationSink * tessellationSink) const;
    NullPtr<void (D2D1GeometryHook::*)(_In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _In_ ID2D1TessellationSink * &) const> _Tessellate_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _In_ ID2D1TessellationSink *) const> _Tessellate_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CombineWithGeometry(_In_ ID2D1Geometry * inputGeometry, D2D1_COMBINE_MODE combineMode, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const;
    NullPtr<void (D2D1GeometryHook::*)(_In_ ID2D1Geometry * &, D2D1_COMBINE_MODE &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _In_ ID2D1SimplifiedGeometrySink * &) const> _CombineWithGeometry_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, _In_ ID2D1Geometry *, D2D1_COMBINE_MODE, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _In_ ID2D1SimplifiedGeometrySink *) const> _CombineWithGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Outline(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const;
    NullPtr<void (D2D1GeometryHook::*)(_In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _In_ ID2D1SimplifiedGeometrySink * &) const> _Outline_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _In_ ID2D1SimplifiedGeometrySink *) const> _Outline_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ComputeArea(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * area) const;
    NullPtr<void (D2D1GeometryHook::*)(_In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _Out_ FLOAT * &) const> _ComputeArea_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _Out_ FLOAT *) const> _ComputeArea_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ComputeLength(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * length) const;
    NullPtr<void (D2D1GeometryHook::*)(_In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _Out_ FLOAT * &) const> _ComputeLength_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _Out_ FLOAT *) const> _ComputeLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ComputePointAtLength(FLOAT length, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_opt_ D2D1_POINT_2F * point, _Out_opt_ D2D1_POINT_2F * unitTangentVector) const;
    NullPtr<void (D2D1GeometryHook::*)(FLOAT &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _Out_opt_ D2D1_POINT_2F * &, _Out_opt_ D2D1_POINT_2F * &) const> _ComputePointAtLength_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, FLOAT, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _Out_opt_ D2D1_POINT_2F *, _Out_opt_ D2D1_POINT_2F *) const> _ComputePointAtLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Widen(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const;
    NullPtr<void (D2D1GeometryHook::*)(FLOAT &, _In_opt_ ID2D1StrokeStyle * &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _In_ ID2D1SimplifiedGeometrySink * &) const> _Widen_pre_ptr;
    NullPtr<void (D2D1GeometryHook::*)(HRESULT, FLOAT, _In_opt_ ID2D1StrokeStyle *, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _In_ ID2D1SimplifiedGeometrySink *) const> _Widen_post_ptr;

};

// ==============================================================================
// ID2D1RectangleGeometry
// ==============================================================================
class D2D1RectangleGeometryHook : public HookBase<ID2D1RectangleGeometry>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1GeometryHook & _D2D1Geometry;

protected:

    D2D1RectangleGeometryHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1GeometryHook & D2D1Geometry, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Geometry(D2D1Geometry)
    {
    }

    ~D2D1RectangleGeometryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1GeometryHook * D2D1Geometry = (D2D1GeometryHook *)unknown.GetHookedObj(__uuidof(ID2D1Geometry));
        if (nullptr == D2D1Geometry) return nullptr;

        try
        {
            return new D2D1RectangleGeometryHook(unknown, *D2D1Resource, *D2D1Geometry, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1RectangleGeometryHook * typedPtr = (D2D1RectangleGeometryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBounds(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetBounds(worldTransform, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWidenedBounds(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetWidenedBounds(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE StrokeContainsPoint(D2D1_POINT_2F point, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.StrokeContainsPoint(point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FillContainsPoint(D2D1_POINT_2F point, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.FillContainsPoint(point, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CompareWithGeometry(_In_ ID2D1Geometry * inputGeometry, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _Out_ D2D1_GEOMETRY_RELATION * relation) const
    {
        return _D2D1Geometry.CompareWithGeometry(inputGeometry, inputGeometryTransform, flatteningTolerance, relation);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Simplify(simplificationOption, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Tessellate(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1TessellationSink * tessellationSink) const
    {
        return _D2D1Geometry.Tessellate(worldTransform, flatteningTolerance, tessellationSink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CombineWithGeometry(_In_ ID2D1Geometry * inputGeometry, D2D1_COMBINE_MODE combineMode, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.CombineWithGeometry(inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Outline(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Outline(worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeArea(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * area) const
    {
        return _D2D1Geometry.ComputeArea(worldTransform, flatteningTolerance, area);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeLength(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * length) const
    {
        return _D2D1Geometry.ComputeLength(worldTransform, flatteningTolerance, length);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputePointAtLength(FLOAT length, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_opt_ D2D1_POINT_2F * point, _Out_opt_ D2D1_POINT_2F * unitTangentVector) const
    {
        return _D2D1Geometry.ComputePointAtLength(length, worldTransform, flatteningTolerance, point, unitTangentVector);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Widen(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Widen(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetRect(_Out_ D2D1_RECT_F * rect) const;
    NullPtr<void (D2D1RectangleGeometryHook::*)(_Out_ D2D1_RECT_F * &) const> _GetRect_pre_ptr;
    NullPtr<void (D2D1RectangleGeometryHook::*)(_Out_ D2D1_RECT_F *) const> _GetRect_post_ptr;

};

// ==============================================================================
// ID2D1RoundedRectangleGeometry
// ==============================================================================
class D2D1RoundedRectangleGeometryHook : public HookBase<ID2D1RoundedRectangleGeometry>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1GeometryHook & _D2D1Geometry;

protected:

    D2D1RoundedRectangleGeometryHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1GeometryHook & D2D1Geometry, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Geometry(D2D1Geometry)
    {
    }

    ~D2D1RoundedRectangleGeometryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1GeometryHook * D2D1Geometry = (D2D1GeometryHook *)unknown.GetHookedObj(__uuidof(ID2D1Geometry));
        if (nullptr == D2D1Geometry) return nullptr;

        try
        {
            return new D2D1RoundedRectangleGeometryHook(unknown, *D2D1Resource, *D2D1Geometry, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1RoundedRectangleGeometryHook * typedPtr = (D2D1RoundedRectangleGeometryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBounds(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetBounds(worldTransform, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWidenedBounds(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetWidenedBounds(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE StrokeContainsPoint(D2D1_POINT_2F point, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.StrokeContainsPoint(point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FillContainsPoint(D2D1_POINT_2F point, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.FillContainsPoint(point, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CompareWithGeometry(_In_ ID2D1Geometry * inputGeometry, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _Out_ D2D1_GEOMETRY_RELATION * relation) const
    {
        return _D2D1Geometry.CompareWithGeometry(inputGeometry, inputGeometryTransform, flatteningTolerance, relation);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Simplify(simplificationOption, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Tessellate(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1TessellationSink * tessellationSink) const
    {
        return _D2D1Geometry.Tessellate(worldTransform, flatteningTolerance, tessellationSink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CombineWithGeometry(_In_ ID2D1Geometry * inputGeometry, D2D1_COMBINE_MODE combineMode, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.CombineWithGeometry(inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Outline(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Outline(worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeArea(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * area) const
    {
        return _D2D1Geometry.ComputeArea(worldTransform, flatteningTolerance, area);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeLength(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * length) const
    {
        return _D2D1Geometry.ComputeLength(worldTransform, flatteningTolerance, length);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputePointAtLength(FLOAT length, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_opt_ D2D1_POINT_2F * point, _Out_opt_ D2D1_POINT_2F * unitTangentVector) const
    {
        return _D2D1Geometry.ComputePointAtLength(length, worldTransform, flatteningTolerance, point, unitTangentVector);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Widen(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Widen(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetRoundedRect(_Out_ D2D1_ROUNDED_RECT * roundedRect) const;
    NullPtr<void (D2D1RoundedRectangleGeometryHook::*)(_Out_ D2D1_ROUNDED_RECT * &) const> _GetRoundedRect_pre_ptr;
    NullPtr<void (D2D1RoundedRectangleGeometryHook::*)(_Out_ D2D1_ROUNDED_RECT *) const> _GetRoundedRect_post_ptr;

};

// ==============================================================================
// ID2D1EllipseGeometry
// ==============================================================================
class D2D1EllipseGeometryHook : public HookBase<ID2D1EllipseGeometry>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1GeometryHook & _D2D1Geometry;

protected:

    D2D1EllipseGeometryHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1GeometryHook & D2D1Geometry, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Geometry(D2D1Geometry)
    {
    }

    ~D2D1EllipseGeometryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1GeometryHook * D2D1Geometry = (D2D1GeometryHook *)unknown.GetHookedObj(__uuidof(ID2D1Geometry));
        if (nullptr == D2D1Geometry) return nullptr;

        try
        {
            return new D2D1EllipseGeometryHook(unknown, *D2D1Resource, *D2D1Geometry, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1EllipseGeometryHook * typedPtr = (D2D1EllipseGeometryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBounds(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetBounds(worldTransform, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWidenedBounds(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetWidenedBounds(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE StrokeContainsPoint(D2D1_POINT_2F point, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.StrokeContainsPoint(point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FillContainsPoint(D2D1_POINT_2F point, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.FillContainsPoint(point, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CompareWithGeometry(_In_ ID2D1Geometry * inputGeometry, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _Out_ D2D1_GEOMETRY_RELATION * relation) const
    {
        return _D2D1Geometry.CompareWithGeometry(inputGeometry, inputGeometryTransform, flatteningTolerance, relation);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Simplify(simplificationOption, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Tessellate(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1TessellationSink * tessellationSink) const
    {
        return _D2D1Geometry.Tessellate(worldTransform, flatteningTolerance, tessellationSink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CombineWithGeometry(_In_ ID2D1Geometry * inputGeometry, D2D1_COMBINE_MODE combineMode, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.CombineWithGeometry(inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Outline(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Outline(worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeArea(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * area) const
    {
        return _D2D1Geometry.ComputeArea(worldTransform, flatteningTolerance, area);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeLength(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * length) const
    {
        return _D2D1Geometry.ComputeLength(worldTransform, flatteningTolerance, length);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputePointAtLength(FLOAT length, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_opt_ D2D1_POINT_2F * point, _Out_opt_ D2D1_POINT_2F * unitTangentVector) const
    {
        return _D2D1Geometry.ComputePointAtLength(length, worldTransform, flatteningTolerance, point, unitTangentVector);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Widen(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Widen(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetEllipse(_Out_ D2D1_ELLIPSE * ellipse) const;
    NullPtr<void (D2D1EllipseGeometryHook::*)(_Out_ D2D1_ELLIPSE * &) const> _GetEllipse_pre_ptr;
    NullPtr<void (D2D1EllipseGeometryHook::*)(_Out_ D2D1_ELLIPSE *) const> _GetEllipse_post_ptr;

};

// ==============================================================================
// ID2D1GeometryGroup
// ==============================================================================
class D2D1GeometryGroupHook : public HookBase<ID2D1GeometryGroup>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1GeometryHook & _D2D1Geometry;

protected:

    D2D1GeometryGroupHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1GeometryHook & D2D1Geometry, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Geometry(D2D1Geometry)
    {
    }

    ~D2D1GeometryGroupHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1GeometryHook * D2D1Geometry = (D2D1GeometryHook *)unknown.GetHookedObj(__uuidof(ID2D1Geometry));
        if (nullptr == D2D1Geometry) return nullptr;

        try
        {
            return new D2D1GeometryGroupHook(unknown, *D2D1Resource, *D2D1Geometry, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1GeometryGroupHook * typedPtr = (D2D1GeometryGroupHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBounds(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetBounds(worldTransform, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWidenedBounds(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetWidenedBounds(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE StrokeContainsPoint(D2D1_POINT_2F point, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.StrokeContainsPoint(point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FillContainsPoint(D2D1_POINT_2F point, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.FillContainsPoint(point, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CompareWithGeometry(_In_ ID2D1Geometry * inputGeometry, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _Out_ D2D1_GEOMETRY_RELATION * relation) const
    {
        return _D2D1Geometry.CompareWithGeometry(inputGeometry, inputGeometryTransform, flatteningTolerance, relation);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Simplify(simplificationOption, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Tessellate(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1TessellationSink * tessellationSink) const
    {
        return _D2D1Geometry.Tessellate(worldTransform, flatteningTolerance, tessellationSink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CombineWithGeometry(_In_ ID2D1Geometry * inputGeometry, D2D1_COMBINE_MODE combineMode, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.CombineWithGeometry(inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Outline(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Outline(worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeArea(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * area) const
    {
        return _D2D1Geometry.ComputeArea(worldTransform, flatteningTolerance, area);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeLength(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * length) const
    {
        return _D2D1Geometry.ComputeLength(worldTransform, flatteningTolerance, length);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputePointAtLength(FLOAT length, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_opt_ D2D1_POINT_2F * point, _Out_opt_ D2D1_POINT_2F * unitTangentVector) const
    {
        return _D2D1Geometry.ComputePointAtLength(length, worldTransform, flatteningTolerance, point, unitTangentVector);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Widen(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Widen(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    virtual D2D1_FILL_MODE STDMETHODCALLTYPE GetFillMode() const;
    NullPtr<void (D2D1GeometryGroupHook::*)() const> _GetFillMode_pre_ptr;
    NullPtr<void (D2D1GeometryGroupHook::*)(D2D1_FILL_MODE) const> _GetFillMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetSourceGeometryCount() const;
    NullPtr<void (D2D1GeometryGroupHook::*)() const> _GetSourceGeometryCount_pre_ptr;
    NullPtr<void (D2D1GeometryGroupHook::*)(UINT32) const> _GetSourceGeometryCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetSourceGeometries(_Out_writes_(geometriesCount) ID2D1Geometry ** geometries, UINT32 geometriesCount) const;
    NullPtr<void (D2D1GeometryGroupHook::*)(_Out_writes_(geometriesCount) ID2D1Geometry ** &, UINT32 &) const> _GetSourceGeometries_pre_ptr;
    NullPtr<void (D2D1GeometryGroupHook::*)(_Out_writes_(geometriesCount) ID2D1Geometry **, UINT32) const> _GetSourceGeometries_post_ptr;

};

// ==============================================================================
// ID2D1TransformedGeometry
// ==============================================================================
class D2D1TransformedGeometryHook : public HookBase<ID2D1TransformedGeometry>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1GeometryHook & _D2D1Geometry;

protected:

    D2D1TransformedGeometryHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1GeometryHook & D2D1Geometry, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Geometry(D2D1Geometry)
    {
    }

    ~D2D1TransformedGeometryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1GeometryHook * D2D1Geometry = (D2D1GeometryHook *)unknown.GetHookedObj(__uuidof(ID2D1Geometry));
        if (nullptr == D2D1Geometry) return nullptr;

        try
        {
            return new D2D1TransformedGeometryHook(unknown, *D2D1Resource, *D2D1Geometry, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1TransformedGeometryHook * typedPtr = (D2D1TransformedGeometryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBounds(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetBounds(worldTransform, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWidenedBounds(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetWidenedBounds(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE StrokeContainsPoint(D2D1_POINT_2F point, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.StrokeContainsPoint(point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FillContainsPoint(D2D1_POINT_2F point, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.FillContainsPoint(point, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CompareWithGeometry(_In_ ID2D1Geometry * inputGeometry, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _Out_ D2D1_GEOMETRY_RELATION * relation) const
    {
        return _D2D1Geometry.CompareWithGeometry(inputGeometry, inputGeometryTransform, flatteningTolerance, relation);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Simplify(simplificationOption, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Tessellate(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1TessellationSink * tessellationSink) const
    {
        return _D2D1Geometry.Tessellate(worldTransform, flatteningTolerance, tessellationSink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CombineWithGeometry(_In_ ID2D1Geometry * inputGeometry, D2D1_COMBINE_MODE combineMode, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.CombineWithGeometry(inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Outline(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Outline(worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeArea(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * area) const
    {
        return _D2D1Geometry.ComputeArea(worldTransform, flatteningTolerance, area);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeLength(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * length) const
    {
        return _D2D1Geometry.ComputeLength(worldTransform, flatteningTolerance, length);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputePointAtLength(FLOAT length, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_opt_ D2D1_POINT_2F * point, _Out_opt_ D2D1_POINT_2F * unitTangentVector) const
    {
        return _D2D1Geometry.ComputePointAtLength(length, worldTransform, flatteningTolerance, point, unitTangentVector);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Widen(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Widen(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetSourceGeometry(_Outptr_ ID2D1Geometry ** sourceGeometry) const;
    NullPtr<void (D2D1TransformedGeometryHook::*)(_Outptr_ ID2D1Geometry ** &) const> _GetSourceGeometry_pre_ptr;
    NullPtr<void (D2D1TransformedGeometryHook::*)(_Outptr_ ID2D1Geometry **) const> _GetSourceGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const;
    NullPtr<void (D2D1TransformedGeometryHook::*)(_Out_ D2D1_MATRIX_3X2_F * &) const> _GetTransform_pre_ptr;
    NullPtr<void (D2D1TransformedGeometryHook::*)(_Out_ D2D1_MATRIX_3X2_F *) const> _GetTransform_post_ptr;

};

// ==============================================================================
// ID2D1SimplifiedGeometrySink
// ==============================================================================
class D2D1SimplifiedGeometrySinkHook : public HookBase<ID2D1SimplifiedGeometrySink>
{

protected:

    D2D1SimplifiedGeometrySinkHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1SimplifiedGeometrySinkHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1SimplifiedGeometrySinkHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1SimplifiedGeometrySinkHook * typedPtr = (D2D1SimplifiedGeometrySinkHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE fillMode);
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(D2D1_FILL_MODE &)> _SetFillMode_pre_ptr;
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(D2D1_FILL_MODE)> _SetFillMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT vertexFlags);
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(D2D1_PATH_SEGMENT &)> _SetSegmentFlags_pre_ptr;
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(D2D1_PATH_SEGMENT)> _SetSegmentFlags_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN figureBegin);
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(D2D1_POINT_2F &, D2D1_FIGURE_BEGIN &)> _BeginFigure_pre_ptr;
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(D2D1_POINT_2F, D2D1_FIGURE_BEGIN)> _BeginFigure_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE AddLines(_In_reads_(pointsCount) CONST D2D1_POINT_2F * points, UINT32 pointsCount);
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(_In_reads_(pointsCount) CONST D2D1_POINT_2F * &, UINT32 &)> _AddLines_pre_ptr;
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(_In_reads_(pointsCount) CONST D2D1_POINT_2F *, UINT32)> _AddLines_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE AddBeziers(_In_reads_(beziersCount) CONST D2D1_BEZIER_SEGMENT * beziers, UINT32 beziersCount);
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(_In_reads_(beziersCount) CONST D2D1_BEZIER_SEGMENT * &, UINT32 &)> _AddBeziers_pre_ptr;
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(_In_reads_(beziersCount) CONST D2D1_BEZIER_SEGMENT *, UINT32)> _AddBeziers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END figureEnd);
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(D2D1_FIGURE_END &)> _EndFigure_pre_ptr;
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(D2D1_FIGURE_END)> _EndFigure_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Close();
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)()> _Close_pre_ptr;
    NullPtr<void (D2D1SimplifiedGeometrySinkHook::*)(HRESULT)> _Close_post_ptr;

};

// ==============================================================================
// ID2D1GeometrySink
// ==============================================================================
class D2D1GeometrySinkHook : public HookBase<ID2D1GeometrySink>
{
    D2D1SimplifiedGeometrySinkHook & _D2D1SimplifiedGeometrySink;

protected:

    D2D1GeometrySinkHook(UnknownBase & unknown, D2D1SimplifiedGeometrySinkHook & D2D1SimplifiedGeometrySink, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1SimplifiedGeometrySink(D2D1SimplifiedGeometrySink)
    {
    }

    ~D2D1GeometrySinkHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1SimplifiedGeometrySinkHook * D2D1SimplifiedGeometrySink = (D2D1SimplifiedGeometrySinkHook *)unknown.GetHookedObj(__uuidof(ID2D1SimplifiedGeometrySink));
        if (nullptr == D2D1SimplifiedGeometrySink) return nullptr;

        try
        {
            return new D2D1GeometrySinkHook(unknown, *D2D1SimplifiedGeometrySink, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1GeometrySinkHook * typedPtr = (D2D1GeometrySinkHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE fillMode)
    {
        return _D2D1SimplifiedGeometrySink.SetFillMode(fillMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT vertexFlags)
    {
        return _D2D1SimplifiedGeometrySink.SetSegmentFlags(vertexFlags);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN figureBegin)
    {
        return _D2D1SimplifiedGeometrySink.BeginFigure(startPoint, figureBegin);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE AddLines(_In_reads_(pointsCount) CONST D2D1_POINT_2F * points, UINT32 pointsCount)
    {
        return _D2D1SimplifiedGeometrySink.AddLines(points, pointsCount);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE AddBeziers(_In_reads_(beziersCount) CONST D2D1_BEZIER_SEGMENT * beziers, UINT32 beziersCount)
    {
        return _D2D1SimplifiedGeometrySink.AddBeziers(beziers, beziersCount);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END figureEnd)
    {
        return _D2D1SimplifiedGeometrySink.EndFigure(figureEnd);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Close()
    {
        return _D2D1SimplifiedGeometrySink.Close();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE AddLine(D2D1_POINT_2F point);
    NullPtr<void (D2D1GeometrySinkHook::*)(D2D1_POINT_2F &)> _AddLine_pre_ptr;
    NullPtr<void (D2D1GeometrySinkHook::*)(D2D1_POINT_2F)> _AddLine_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE AddBezier(_In_ CONST D2D1_BEZIER_SEGMENT * bezier);
    NullPtr<void (D2D1GeometrySinkHook::*)(_In_ CONST D2D1_BEZIER_SEGMENT * &)> _AddBezier_pre_ptr;
    NullPtr<void (D2D1GeometrySinkHook::*)(_In_ CONST D2D1_BEZIER_SEGMENT *)> _AddBezier_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE AddQuadraticBezier(_In_ CONST D2D1_QUADRATIC_BEZIER_SEGMENT * bezier);
    NullPtr<void (D2D1GeometrySinkHook::*)(_In_ CONST D2D1_QUADRATIC_BEZIER_SEGMENT * &)> _AddQuadraticBezier_pre_ptr;
    NullPtr<void (D2D1GeometrySinkHook::*)(_In_ CONST D2D1_QUADRATIC_BEZIER_SEGMENT *)> _AddQuadraticBezier_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE AddQuadraticBeziers(_In_reads_(beziersCount) CONST D2D1_QUADRATIC_BEZIER_SEGMENT * beziers, UINT32 beziersCount);
    NullPtr<void (D2D1GeometrySinkHook::*)(_In_reads_(beziersCount) CONST D2D1_QUADRATIC_BEZIER_SEGMENT * &, UINT32 &)> _AddQuadraticBeziers_pre_ptr;
    NullPtr<void (D2D1GeometrySinkHook::*)(_In_reads_(beziersCount) CONST D2D1_QUADRATIC_BEZIER_SEGMENT *, UINT32)> _AddQuadraticBeziers_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE AddArc(_In_ CONST D2D1_ARC_SEGMENT * arc);
    NullPtr<void (D2D1GeometrySinkHook::*)(_In_ CONST D2D1_ARC_SEGMENT * &)> _AddArc_pre_ptr;
    NullPtr<void (D2D1GeometrySinkHook::*)(_In_ CONST D2D1_ARC_SEGMENT *)> _AddArc_post_ptr;

};

// ==============================================================================
// ID2D1TessellationSink
// ==============================================================================
class D2D1TessellationSinkHook : public HookBase<ID2D1TessellationSink>
{

protected:

    D2D1TessellationSinkHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1TessellationSinkHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1TessellationSinkHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1TessellationSinkHook * typedPtr = (D2D1TessellationSinkHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE AddTriangles(_In_reads_(trianglesCount) CONST D2D1_TRIANGLE * triangles, UINT32 trianglesCount);
    NullPtr<void (D2D1TessellationSinkHook::*)(_In_reads_(trianglesCount) CONST D2D1_TRIANGLE * &, UINT32 &)> _AddTriangles_pre_ptr;
    NullPtr<void (D2D1TessellationSinkHook::*)(_In_reads_(trianglesCount) CONST D2D1_TRIANGLE *, UINT32)> _AddTriangles_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Close();
    NullPtr<void (D2D1TessellationSinkHook::*)()> _Close_pre_ptr;
    NullPtr<void (D2D1TessellationSinkHook::*)(HRESULT)> _Close_post_ptr;

};

// ==============================================================================
// ID2D1PathGeometry
// ==============================================================================
class D2D1PathGeometryHook : public HookBase<ID2D1PathGeometry>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1GeometryHook & _D2D1Geometry;

protected:

    D2D1PathGeometryHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1GeometryHook & D2D1Geometry, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Geometry(D2D1Geometry)
    {
    }

    ~D2D1PathGeometryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1GeometryHook * D2D1Geometry = (D2D1GeometryHook *)unknown.GetHookedObj(__uuidof(ID2D1Geometry));
        if (nullptr == D2D1Geometry) return nullptr;

        try
        {
            return new D2D1PathGeometryHook(unknown, *D2D1Resource, *D2D1Geometry, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1PathGeometryHook * typedPtr = (D2D1PathGeometryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBounds(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetBounds(worldTransform, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWidenedBounds(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetWidenedBounds(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE StrokeContainsPoint(D2D1_POINT_2F point, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.StrokeContainsPoint(point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FillContainsPoint(D2D1_POINT_2F point, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.FillContainsPoint(point, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CompareWithGeometry(_In_ ID2D1Geometry * inputGeometry, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _Out_ D2D1_GEOMETRY_RELATION * relation) const
    {
        return _D2D1Geometry.CompareWithGeometry(inputGeometry, inputGeometryTransform, flatteningTolerance, relation);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Simplify(simplificationOption, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Tessellate(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1TessellationSink * tessellationSink) const
    {
        return _D2D1Geometry.Tessellate(worldTransform, flatteningTolerance, tessellationSink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CombineWithGeometry(_In_ ID2D1Geometry * inputGeometry, D2D1_COMBINE_MODE combineMode, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.CombineWithGeometry(inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Outline(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Outline(worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeArea(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * area) const
    {
        return _D2D1Geometry.ComputeArea(worldTransform, flatteningTolerance, area);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeLength(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * length) const
    {
        return _D2D1Geometry.ComputeLength(worldTransform, flatteningTolerance, length);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputePointAtLength(FLOAT length, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_opt_ D2D1_POINT_2F * point, _Out_opt_ D2D1_POINT_2F * unitTangentVector) const
    {
        return _D2D1Geometry.ComputePointAtLength(length, worldTransform, flatteningTolerance, point, unitTangentVector);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Widen(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Widen(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Open(_Outptr_ ID2D1GeometrySink ** geometrySink);
    NullPtr<void (D2D1PathGeometryHook::*)(_Outptr_ ID2D1GeometrySink ** &)> _Open_pre_ptr;
    NullPtr<void (D2D1PathGeometryHook::*)(HRESULT, _Outptr_ ID2D1GeometrySink **)> _Open_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Stream(_In_ ID2D1GeometrySink * geometrySink) const;
    NullPtr<void (D2D1PathGeometryHook::*)(_In_ ID2D1GeometrySink * &) const> _Stream_pre_ptr;
    NullPtr<void (D2D1PathGeometryHook::*)(HRESULT, _In_ ID2D1GeometrySink *) const> _Stream_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSegmentCount(_Out_ UINT32 * count) const;
    NullPtr<void (D2D1PathGeometryHook::*)(_Out_ UINT32 * &) const> _GetSegmentCount_pre_ptr;
    NullPtr<void (D2D1PathGeometryHook::*)(HRESULT, _Out_ UINT32 *) const> _GetSegmentCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFigureCount(_Out_ UINT32 * count) const;
    NullPtr<void (D2D1PathGeometryHook::*)(_Out_ UINT32 * &) const> _GetFigureCount_pre_ptr;
    NullPtr<void (D2D1PathGeometryHook::*)(HRESULT, _Out_ UINT32 *) const> _GetFigureCount_post_ptr;

};

// ==============================================================================
// ID2D1Mesh
// ==============================================================================
class D2D1MeshHook : public HookBase<ID2D1Mesh>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1MeshHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1MeshHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1MeshHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1MeshHook * typedPtr = (D2D1MeshHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Open(_Outptr_ ID2D1TessellationSink ** tessellationSink);
    NullPtr<void (D2D1MeshHook::*)(_Outptr_ ID2D1TessellationSink ** &)> _Open_pre_ptr;
    NullPtr<void (D2D1MeshHook::*)(HRESULT, _Outptr_ ID2D1TessellationSink **)> _Open_post_ptr;

};

// ==============================================================================
// ID2D1Layer
// ==============================================================================
class D2D1LayerHook : public HookBase<ID2D1Layer>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1LayerHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1LayerHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1LayerHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1LayerHook * typedPtr = (D2D1LayerHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual D2D1_SIZE_F STDMETHODCALLTYPE GetSize() const;
    NullPtr<void (D2D1LayerHook::*)() const> _GetSize_pre_ptr;
    NullPtr<void (D2D1LayerHook::*)(D2D1_SIZE_F) const> _GetSize_post_ptr;

};

// ==============================================================================
// ID2D1DrawingStateBlock
// ==============================================================================
class D2D1DrawingStateBlockHook : public HookBase<ID2D1DrawingStateBlock>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1DrawingStateBlockHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1DrawingStateBlockHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1DrawingStateBlockHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1DrawingStateBlockHook * typedPtr = (D2D1DrawingStateBlockHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDescription(_Out_ D2D1_DRAWING_STATE_DESCRIPTION * stateDescription) const;
    NullPtr<void (D2D1DrawingStateBlockHook::*)(_Out_ D2D1_DRAWING_STATE_DESCRIPTION * &) const> _GetDescription_pre_ptr;
    NullPtr<void (D2D1DrawingStateBlockHook::*)(_Out_ D2D1_DRAWING_STATE_DESCRIPTION *) const> _GetDescription_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetDescription(_In_ CONST D2D1_DRAWING_STATE_DESCRIPTION * stateDescription);
    NullPtr<void (D2D1DrawingStateBlockHook::*)(_In_ CONST D2D1_DRAWING_STATE_DESCRIPTION * &)> _SetDescription_pre_ptr;
    NullPtr<void (D2D1DrawingStateBlockHook::*)(_In_ CONST D2D1_DRAWING_STATE_DESCRIPTION *)> _SetDescription_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetTextRenderingParams(_In_opt_ IDWriteRenderingParams * textRenderingParams = NULL);
    NullPtr<void (D2D1DrawingStateBlockHook::*)(_In_opt_ IDWriteRenderingParams * &)> _SetTextRenderingParams_pre_ptr;
    NullPtr<void (D2D1DrawingStateBlockHook::*)(_In_opt_ IDWriteRenderingParams *)> _SetTextRenderingParams_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetTextRenderingParams(_Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const;
    NullPtr<void (D2D1DrawingStateBlockHook::*)(_Outptr_result_maybenull_ IDWriteRenderingParams ** &) const> _GetTextRenderingParams_pre_ptr;
    NullPtr<void (D2D1DrawingStateBlockHook::*)(_Outptr_result_maybenull_ IDWriteRenderingParams **) const> _GetTextRenderingParams_post_ptr;

};

// ==============================================================================
// ID2D1RenderTarget
// ==============================================================================
class D2D1RenderTargetHook : public HookBase<ID2D1RenderTarget>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1RenderTargetHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1RenderTargetHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1RenderTargetHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1RenderTargetHook * typedPtr = (D2D1RenderTargetHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBitmap(D2D1_SIZE_U size, _In_opt_ CONST void * srcData, UINT32 pitch, _In_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap);
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_SIZE_U &, _In_opt_ CONST void * &, UINT32 &, _In_ CONST D2D1_BITMAP_PROPERTIES * &, _Outptr_ ID2D1Bitmap ** &)> _CreateBitmap_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, D2D1_SIZE_U, _In_opt_ CONST void *, UINT32, _In_ CONST D2D1_BITMAP_PROPERTIES *, _Outptr_ ID2D1Bitmap **)> _CreateBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBitmapFromWicBitmap(_In_ IWICBitmapSource * wicBitmapSource, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ IWICBitmapSource * &, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * &, _Outptr_ ID2D1Bitmap ** &)> _CreateBitmapFromWicBitmap_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_ IWICBitmapSource *, _In_opt_ CONST D2D1_BITMAP_PROPERTIES *, _Outptr_ ID2D1Bitmap **)> _CreateBitmapFromWicBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSharedBitmap(_In_ REFIID riid, _Inout_ void * data, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ REFIID, _Inout_ void * &, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * &, _Outptr_ ID2D1Bitmap ** &)> _CreateSharedBitmap_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_ REFIID, _Inout_ void *, _In_opt_ CONST D2D1_BITMAP_PROPERTIES *, _Outptr_ ID2D1Bitmap **)> _CreateSharedBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBitmapBrush(_In_opt_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES * bitmapBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1BitmapBrush ** bitmapBrush);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_opt_ ID2D1Bitmap * &, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES * &, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * &, _Outptr_ ID2D1BitmapBrush ** &)> _CreateBitmapBrush_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_opt_ ID2D1Bitmap *, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES *, _In_opt_ CONST D2D1_BRUSH_PROPERTIES *, _Outptr_ ID2D1BitmapBrush **)> _CreateBitmapBrush_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateSolidColorBrush(_In_ CONST D2D1_COLOR_F * color, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1SolidColorBrush ** solidColorBrush);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_COLOR_F * &, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * &, _Outptr_ ID2D1SolidColorBrush ** &)> _CreateSolidColorBrush_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_ CONST D2D1_COLOR_F *, _In_opt_ CONST D2D1_BRUSH_PROPERTIES *, _Outptr_ ID2D1SolidColorBrush **)> _CreateSolidColorBrush_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateGradientStopCollection(_In_reads_(gradientStopsCount) CONST D2D1_GRADIENT_STOP * gradientStops, _In_range_(>=,1) UINT32 gradientStopsCount, D2D1_GAMMA colorInterpolationGamma, D2D1_EXTEND_MODE extendMode, _Outptr_ ID2D1GradientStopCollection ** gradientStopCollection);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_reads_(gradientStopsCount) CONST D2D1_GRADIENT_STOP * &, _In_range_(>=,1) UINT32 &, D2D1_GAMMA &, D2D1_EXTEND_MODE &, _Outptr_ ID2D1GradientStopCollection ** &)> _CreateGradientStopCollection_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_reads_(gradientStopsCount) CONST D2D1_GRADIENT_STOP *, _In_range_(>=,1) UINT32, D2D1_GAMMA, D2D1_EXTEND_MODE, _Outptr_ ID2D1GradientStopCollection **)> _CreateGradientStopCollection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateLinearGradientBrush(_In_ CONST D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * linearGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1LinearGradientBrush ** linearGradientBrush);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * &, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * &, _In_ ID2D1GradientStopCollection * &, _Outptr_ ID2D1LinearGradientBrush ** &)> _CreateLinearGradientBrush_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_ CONST D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES *, _In_opt_ CONST D2D1_BRUSH_PROPERTIES *, _In_ ID2D1GradientStopCollection *, _Outptr_ ID2D1LinearGradientBrush **)> _CreateLinearGradientBrush_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRadialGradientBrush(_In_ CONST D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * radialGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1RadialGradientBrush ** radialGradientBrush);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * &, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * &, _In_ ID2D1GradientStopCollection * &, _Outptr_ ID2D1RadialGradientBrush ** &)> _CreateRadialGradientBrush_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_ CONST D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES *, _In_opt_ CONST D2D1_BRUSH_PROPERTIES *, _In_ ID2D1GradientStopCollection *, _Outptr_ ID2D1RadialGradientBrush **)> _CreateRadialGradientBrush_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCompatibleRenderTarget(_In_opt_ CONST D2D1_SIZE_F * desiredSize, _In_opt_ CONST D2D1_SIZE_U * desiredPixelSize, _In_opt_ CONST D2D1_PIXEL_FORMAT * desiredFormat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options, _Outptr_ ID2D1BitmapRenderTarget ** bitmapRenderTarget);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_opt_ CONST D2D1_SIZE_F * &, _In_opt_ CONST D2D1_SIZE_U * &, _In_opt_ CONST D2D1_PIXEL_FORMAT * &, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS &, _Outptr_ ID2D1BitmapRenderTarget ** &)> _CreateCompatibleRenderTarget_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_opt_ CONST D2D1_SIZE_F *, _In_opt_ CONST D2D1_SIZE_U *, _In_opt_ CONST D2D1_PIXEL_FORMAT *, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS, _Outptr_ ID2D1BitmapRenderTarget **)> _CreateCompatibleRenderTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateLayer(_In_opt_ CONST D2D1_SIZE_F * size, _Outptr_ ID2D1Layer ** layer);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_opt_ CONST D2D1_SIZE_F * &, _Outptr_ ID2D1Layer ** &)> _CreateLayer_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _In_opt_ CONST D2D1_SIZE_F *, _Outptr_ ID2D1Layer **)> _CreateLayer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateMesh(_Outptr_ ID2D1Mesh ** mesh);
    NullPtr<void (D2D1RenderTargetHook::*)(_Outptr_ ID2D1Mesh ** &)> _CreateMesh_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _Outptr_ ID2D1Mesh **)> _CreateMesh_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawLine(D2D1_POINT_2F point0, D2D1_POINT_2F point1, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_POINT_2F &, D2D1_POINT_2F &, _In_ ID2D1Brush * &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &)> _DrawLine_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_POINT_2F, D2D1_POINT_2F, _In_ ID2D1Brush *, FLOAT, _In_opt_ ID2D1StrokeStyle *)> _DrawLine_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_RECT_F * &, _In_ ID2D1Brush * &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &)> _DrawRectangle_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_RECT_F *, _In_ ID2D1Brush *, FLOAT, _In_opt_ ID2D1StrokeStyle *)> _DrawRectangle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE FillRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_RECT_F * &, _In_ ID2D1Brush * &)> _FillRectangle_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_RECT_F *, _In_ ID2D1Brush *)> _FillRectangle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_ROUNDED_RECT * &, _In_ ID2D1Brush * &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &)> _DrawRoundedRectangle_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_ROUNDED_RECT *, _In_ ID2D1Brush *, FLOAT, _In_opt_ ID2D1StrokeStyle *)> _DrawRoundedRectangle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE FillRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_ROUNDED_RECT * &, _In_ ID2D1Brush * &)> _FillRoundedRectangle_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_ROUNDED_RECT *, _In_ ID2D1Brush *)> _FillRoundedRectangle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_ELLIPSE * &, _In_ ID2D1Brush * &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &)> _DrawEllipse_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_ELLIPSE *, _In_ ID2D1Brush *, FLOAT, _In_opt_ ID2D1StrokeStyle *)> _DrawEllipse_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE FillEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_ELLIPSE * &, _In_ ID2D1Brush * &)> _FillEllipse_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_ELLIPSE *, _In_ ID2D1Brush *)> _FillEllipse_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Geometry * &, _In_ ID2D1Brush * &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &)> _DrawGeometry_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Geometry *, _In_ ID2D1Brush *, FLOAT, _In_opt_ ID2D1StrokeStyle *)> _DrawGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE FillGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, _In_opt_ ID2D1Brush * opacityBrush = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Geometry * &, _In_ ID2D1Brush * &, _In_opt_ ID2D1Brush * &)> _FillGeometry_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Geometry *, _In_ ID2D1Brush *, _In_opt_ ID2D1Brush *)> _FillGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE FillMesh(_In_ ID2D1Mesh * mesh, _In_ ID2D1Brush * brush);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Mesh * &, _In_ ID2D1Brush * &)> _FillMesh_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Mesh *, _In_ ID2D1Brush *)> _FillMesh_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE FillOpacityMask(_In_ ID2D1Bitmap * opacityMask, _In_ ID2D1Brush * brush, D2D1_OPACITY_MASK_CONTENT content, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Bitmap * &, _In_ ID2D1Brush * &, D2D1_OPACITY_MASK_CONTENT &, _In_opt_ CONST D2D1_RECT_F * &, _In_opt_ CONST D2D1_RECT_F * &)> _FillOpacityMask_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Bitmap *, _In_ ID2D1Brush *, D2D1_OPACITY_MASK_CONTENT, _In_opt_ CONST D2D1_RECT_F *, _In_opt_ CONST D2D1_RECT_F *)> _FillOpacityMask_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawBitmap(_In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, FLOAT opacity = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Bitmap * &, _In_opt_ CONST D2D1_RECT_F * &, FLOAT &, D2D1_BITMAP_INTERPOLATION_MODE &, _In_opt_ CONST D2D1_RECT_F * &)> _DrawBitmap_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1Bitmap *, _In_opt_ CONST D2D1_RECT_F *, FLOAT, D2D1_BITMAP_INTERPOLATION_MODE, _In_opt_ CONST D2D1_RECT_F *)> _DrawBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawText(_In_reads_(stringLength) CONST WCHAR * string, UINT32 stringLength, _In_ IDWriteTextFormat * textFormat, _In_ CONST D2D1_RECT_F * layoutRect, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_reads_(stringLength) CONST WCHAR * &, UINT32 &, _In_ IDWriteTextFormat * &, _In_ CONST D2D1_RECT_F * &, _In_ ID2D1Brush * &, D2D1_DRAW_TEXT_OPTIONS &, DWRITE_MEASURING_MODE &)> _DrawText_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_reads_(stringLength) CONST WCHAR *, UINT32, _In_ IDWriteTextFormat *, _In_ CONST D2D1_RECT_F *, _In_ ID2D1Brush *, D2D1_DRAW_TEXT_OPTIONS, DWRITE_MEASURING_MODE)> _DrawText_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawTextLayout(D2D1_POINT_2F origin, _In_ IDWriteTextLayout * textLayout, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE);
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_POINT_2F &, _In_ IDWriteTextLayout * &, _In_ ID2D1Brush * &, D2D1_DRAW_TEXT_OPTIONS &)> _DrawTextLayout_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_POINT_2F, _In_ IDWriteTextLayout *, _In_ ID2D1Brush *, D2D1_DRAW_TEXT_OPTIONS)> _DrawTextLayout_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawGlyphRun(D2D1_POINT_2F baselineOrigin, _In_ CONST DWRITE_GLYPH_RUN * glyphRun, _In_ ID2D1Brush * foregroundBrush, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL);
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_POINT_2F &, _In_ CONST DWRITE_GLYPH_RUN * &, _In_ ID2D1Brush * &, DWRITE_MEASURING_MODE &)> _DrawGlyphRun_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_POINT_2F, _In_ CONST DWRITE_GLYPH_RUN *, _In_ ID2D1Brush *, DWRITE_MEASURING_MODE)> _DrawGlyphRun_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_MATRIX_3X2_F * &)> _SetTransform_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_MATRIX_3X2_F *)> _SetTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const;
    NullPtr<void (D2D1RenderTargetHook::*)(_Out_ D2D1_MATRIX_3X2_F * &) const> _GetTransform_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_Out_ D2D1_MATRIX_3X2_F *) const> _GetTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode);
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_ANTIALIAS_MODE &)> _SetAntialiasMode_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_ANTIALIAS_MODE)> _SetAntialiasMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_ANTIALIAS_MODE STDMETHODCALLTYPE GetAntialiasMode() const;
    NullPtr<void (D2D1RenderTargetHook::*)() const> _GetAntialiasMode_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_ANTIALIAS_MODE) const> _GetAntialiasMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode);
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_TEXT_ANTIALIAS_MODE &)> _SetTextAntialiasMode_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_TEXT_ANTIALIAS_MODE)> _SetTextAntialiasMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_TEXT_ANTIALIAS_MODE STDMETHODCALLTYPE GetTextAntialiasMode() const;
    NullPtr<void (D2D1RenderTargetHook::*)() const> _GetTextAntialiasMode_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_TEXT_ANTIALIAS_MODE) const> _GetTextAntialiasMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetTextRenderingParams(_In_opt_ IDWriteRenderingParams * textRenderingParams = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_opt_ IDWriteRenderingParams * &)> _SetTextRenderingParams_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_opt_ IDWriteRenderingParams *)> _SetTextRenderingParams_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetTextRenderingParams(_Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const;
    NullPtr<void (D2D1RenderTargetHook::*)(_Outptr_result_maybenull_ IDWriteRenderingParams ** &) const> _GetTextRenderingParams_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_Outptr_result_maybenull_ IDWriteRenderingParams **) const> _GetTextRenderingParams_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetTags(D2D1_TAG tag1, D2D1_TAG tag2);
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_TAG &, D2D1_TAG &)> _SetTags_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_TAG, D2D1_TAG)> _SetTags_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetTags(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL) const;
    NullPtr<void (D2D1RenderTargetHook::*)(_Out_opt_ D2D1_TAG * &, _Out_opt_ D2D1_TAG * &) const> _GetTags_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_Out_opt_ D2D1_TAG *, _Out_opt_ D2D1_TAG *) const> _GetTags_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PushLayer(_In_ CONST D2D1_LAYER_PARAMETERS * layerParameters, _In_opt_ ID2D1Layer * layer);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_LAYER_PARAMETERS * &, _In_opt_ ID2D1Layer * &)> _PushLayer_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_LAYER_PARAMETERS *, _In_opt_ ID2D1Layer *)> _PushLayer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PopLayer();
    NullPtr<void (D2D1RenderTargetHook::*)()> _PopLayer_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)()> _PopLayer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Flush(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_Out_opt_ D2D1_TAG * &, _Out_opt_ D2D1_TAG * &)> _Flush_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _Out_opt_ D2D1_TAG *, _Out_opt_ D2D1_TAG *)> _Flush_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SaveDrawingState(_Inout_ ID2D1DrawingStateBlock * drawingStateBlock) const;
    NullPtr<void (D2D1RenderTargetHook::*)(_Inout_ ID2D1DrawingStateBlock * &) const> _SaveDrawingState_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_Inout_ ID2D1DrawingStateBlock *) const> _SaveDrawingState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE RestoreDrawingState(_In_ ID2D1DrawingStateBlock * drawingStateBlock);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1DrawingStateBlock * &)> _RestoreDrawingState_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ ID2D1DrawingStateBlock *)> _RestoreDrawingState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PushAxisAlignedClip(_In_ CONST D2D1_RECT_F * clipRect, D2D1_ANTIALIAS_MODE antialiasMode);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_RECT_F * &, D2D1_ANTIALIAS_MODE &)> _PushAxisAlignedClip_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_RECT_F *, D2D1_ANTIALIAS_MODE)> _PushAxisAlignedClip_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PopAxisAlignedClip();
    NullPtr<void (D2D1RenderTargetHook::*)()> _PopAxisAlignedClip_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)()> _PopAxisAlignedClip_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE Clear(_In_opt_ CONST D2D1_COLOR_F * clearColor = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_In_opt_ CONST D2D1_COLOR_F * &)> _Clear_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_opt_ CONST D2D1_COLOR_F *)> _Clear_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE BeginDraw();
    NullPtr<void (D2D1RenderTargetHook::*)()> _BeginDraw_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)()> _BeginDraw_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EndDraw(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL);
    NullPtr<void (D2D1RenderTargetHook::*)(_Out_opt_ D2D1_TAG * &, _Out_opt_ D2D1_TAG * &)> _EndDraw_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(HRESULT, _Out_opt_ D2D1_TAG *, _Out_opt_ D2D1_TAG *)> _EndDraw_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_PIXEL_FORMAT STDMETHODCALLTYPE GetPixelFormat() const;
    NullPtr<void (D2D1RenderTargetHook::*)() const> _GetPixelFormat_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_PIXEL_FORMAT) const> _GetPixelFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetDpi(FLOAT dpiX, FLOAT dpiY);
    NullPtr<void (D2D1RenderTargetHook::*)(FLOAT &, FLOAT &)> _SetDpi_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(FLOAT, FLOAT)> _SetDpi_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY) const;
    NullPtr<void (D2D1RenderTargetHook::*)(_Out_ FLOAT * &, _Out_ FLOAT * &) const> _GetDpi_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(_Out_ FLOAT *, _Out_ FLOAT *) const> _GetDpi_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_SIZE_F STDMETHODCALLTYPE GetSize() const;
    NullPtr<void (D2D1RenderTargetHook::*)() const> _GetSize_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_SIZE_F) const> _GetSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_SIZE_U STDMETHODCALLTYPE GetPixelSize() const;
    NullPtr<void (D2D1RenderTargetHook::*)() const> _GetPixelSize_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(D2D1_SIZE_U) const> _GetPixelSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetMaximumBitmapSize() const;
    NullPtr<void (D2D1RenderTargetHook::*)() const> _GetMaximumBitmapSize_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(UINT32) const> _GetMaximumBitmapSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsSupported(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties) const;
    NullPtr<void (D2D1RenderTargetHook::*)(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * &) const> _IsSupported_pre_ptr;
    NullPtr<void (D2D1RenderTargetHook::*)(BOOL, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES *) const> _IsSupported_post_ptr;

};

// ==============================================================================
// ID2D1BitmapRenderTarget
// ==============================================================================
class D2D1BitmapRenderTargetHook : public HookBase<ID2D1BitmapRenderTarget>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1RenderTargetHook & _D2D1RenderTarget;

protected:

    D2D1BitmapRenderTargetHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1RenderTargetHook & D2D1RenderTarget, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1RenderTarget(D2D1RenderTarget)
    {
    }

    ~D2D1BitmapRenderTargetHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1RenderTargetHook * D2D1RenderTarget = (D2D1RenderTargetHook *)unknown.GetHookedObj(__uuidof(ID2D1RenderTarget));
        if (nullptr == D2D1RenderTarget) return nullptr;

        try
        {
            return new D2D1BitmapRenderTargetHook(unknown, *D2D1Resource, *D2D1RenderTarget, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1BitmapRenderTargetHook * typedPtr = (D2D1BitmapRenderTargetHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmap(D2D1_SIZE_U size, _In_opt_ CONST void * srcData, UINT32 pitch, _In_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateBitmap(size, srcData, pitch, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmapFromWicBitmap(_In_ IWICBitmapSource * wicBitmapSource, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSharedBitmap(_In_ REFIID riid, _Inout_ void * data, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateSharedBitmap(riid, data, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmapBrush(_In_opt_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES * bitmapBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1BitmapBrush ** bitmapBrush)
    {
        return _D2D1RenderTarget.CreateBitmapBrush(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSolidColorBrush(_In_ CONST D2D1_COLOR_F * color, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1SolidColorBrush ** solidColorBrush)
    {
        return _D2D1RenderTarget.CreateSolidColorBrush(color, brushProperties, solidColorBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateGradientStopCollection(_In_reads_(gradientStopsCount) CONST D2D1_GRADIENT_STOP * gradientStops, _In_range_(>=,1) UINT32 gradientStopsCount, D2D1_GAMMA colorInterpolationGamma, D2D1_EXTEND_MODE extendMode, _Outptr_ ID2D1GradientStopCollection ** gradientStopCollection)
    {
        return _D2D1RenderTarget.CreateGradientStopCollection(gradientStops, gradientStopsCount, colorInterpolationGamma, extendMode, gradientStopCollection);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateLinearGradientBrush(_In_ CONST D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * linearGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1LinearGradientBrush ** linearGradientBrush)
    {
        return _D2D1RenderTarget.CreateLinearGradientBrush(linearGradientBrushProperties, brushProperties, gradientStopCollection, linearGradientBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRadialGradientBrush(_In_ CONST D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * radialGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1RadialGradientBrush ** radialGradientBrush)
    {
        return _D2D1RenderTarget.CreateRadialGradientBrush(radialGradientBrushProperties, brushProperties, gradientStopCollection, radialGradientBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateCompatibleRenderTarget(_In_opt_ CONST D2D1_SIZE_F * desiredSize, _In_opt_ CONST D2D1_SIZE_U * desiredPixelSize, _In_opt_ CONST D2D1_PIXEL_FORMAT * desiredFormat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options, _Outptr_ ID2D1BitmapRenderTarget ** bitmapRenderTarget)
    {
        return _D2D1RenderTarget.CreateCompatibleRenderTarget(desiredSize, desiredPixelSize, desiredFormat, options, bitmapRenderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateLayer(_In_opt_ CONST D2D1_SIZE_F * size, _Outptr_ ID2D1Layer ** layer)
    {
        return _D2D1RenderTarget.CreateLayer(size, layer);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateMesh(_Outptr_ ID2D1Mesh ** mesh)
    {
        return _D2D1RenderTarget.CreateMesh(mesh);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawLine(D2D1_POINT_2F point0, D2D1_POINT_2F point1, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawLine(point0, point1, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawRectangle(rect, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillRectangle(rect, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawRoundedRectangle(roundedRect, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillRoundedRectangle(roundedRect, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawEllipse(ellipse, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillEllipse(ellipse, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawGeometry(geometry, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, _In_opt_ ID2D1Brush * opacityBrush = NULL)
    {
        return _D2D1RenderTarget.FillGeometry(geometry, brush, opacityBrush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillMesh(_In_ ID2D1Mesh * mesh, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillMesh(mesh, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillOpacityMask(_In_ ID2D1Bitmap * opacityMask, _In_ ID2D1Brush * brush, D2D1_OPACITY_MASK_CONTENT content, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL)
    {
        return _D2D1RenderTarget.FillOpacityMask(opacityMask, brush, content, destinationRectangle, sourceRectangle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawBitmap(_In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, FLOAT opacity = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL)
    {
        return _D2D1RenderTarget.DrawBitmap(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawText(_In_reads_(stringLength) CONST WCHAR * string, UINT32 stringLength, _In_ IDWriteTextFormat * textFormat, _In_ CONST D2D1_RECT_F * layoutRect, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL)
    {
        return _D2D1RenderTarget.DrawText(string, stringLength, textFormat, layoutRect, defaultForegroundBrush, options, measuringMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawTextLayout(D2D1_POINT_2F origin, _In_ IDWriteTextLayout * textLayout, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE)
    {
        return _D2D1RenderTarget.DrawTextLayout(origin, textLayout, defaultForegroundBrush, options);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawGlyphRun(D2D1_POINT_2F baselineOrigin, _In_ CONST DWRITE_GLYPH_RUN * glyphRun, _In_ ID2D1Brush * foregroundBrush, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL)
    {
        return _D2D1RenderTarget.DrawGlyphRun(baselineOrigin, glyphRun, foregroundBrush, measuringMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1RenderTarget.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1RenderTarget.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode)
    {
        return _D2D1RenderTarget.SetAntialiasMode(antialiasMode);
    }

    // -----------------------------------------------------------------------------
    D2D1_ANTIALIAS_MODE STDMETHODCALLTYPE GetAntialiasMode() const
    {
        return _D2D1RenderTarget.GetAntialiasMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode)
    {
        return _D2D1RenderTarget.SetTextAntialiasMode(textAntialiasMode);
    }

    // -----------------------------------------------------------------------------
    D2D1_TEXT_ANTIALIAS_MODE STDMETHODCALLTYPE GetTextAntialiasMode() const
    {
        return _D2D1RenderTarget.GetTextAntialiasMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextRenderingParams(_In_opt_ IDWriteRenderingParams * textRenderingParams = NULL)
    {
        return _D2D1RenderTarget.SetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTextRenderingParams(_Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const
    {
        return _D2D1RenderTarget.GetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTags(D2D1_TAG tag1, D2D1_TAG tag2)
    {
        return _D2D1RenderTarget.SetTags(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTags(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL) const
    {
        return _D2D1RenderTarget.GetTags(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PushLayer(_In_ CONST D2D1_LAYER_PARAMETERS * layerParameters, _In_opt_ ID2D1Layer * layer)
    {
        return _D2D1RenderTarget.PushLayer(layerParameters, layer);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PopLayer()
    {
        return _D2D1RenderTarget.PopLayer();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Flush(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL)
    {
        return _D2D1RenderTarget.Flush(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SaveDrawingState(_Inout_ ID2D1DrawingStateBlock * drawingStateBlock) const
    {
        return _D2D1RenderTarget.SaveDrawingState(drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RestoreDrawingState(_In_ ID2D1DrawingStateBlock * drawingStateBlock)
    {
        return _D2D1RenderTarget.RestoreDrawingState(drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PushAxisAlignedClip(_In_ CONST D2D1_RECT_F * clipRect, D2D1_ANTIALIAS_MODE antialiasMode)
    {
        return _D2D1RenderTarget.PushAxisAlignedClip(clipRect, antialiasMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PopAxisAlignedClip()
    {
        return _D2D1RenderTarget.PopAxisAlignedClip();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Clear(_In_opt_ CONST D2D1_COLOR_F * clearColor = NULL)
    {
        return _D2D1RenderTarget.Clear(clearColor);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE BeginDraw()
    {
        return _D2D1RenderTarget.BeginDraw();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EndDraw(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL)
    {
        return _D2D1RenderTarget.EndDraw(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    D2D1_PIXEL_FORMAT STDMETHODCALLTYPE GetPixelFormat() const
    {
        return _D2D1RenderTarget.GetPixelFormat();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetDpi(FLOAT dpiX, FLOAT dpiY)
    {
        return _D2D1RenderTarget.SetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY) const
    {
        return _D2D1RenderTarget.GetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_F STDMETHODCALLTYPE GetSize() const
    {
        return _D2D1RenderTarget.GetSize();
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_U STDMETHODCALLTYPE GetPixelSize() const
    {
        return _D2D1RenderTarget.GetPixelSize();
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetMaximumBitmapSize() const
    {
        return _D2D1RenderTarget.GetMaximumBitmapSize();
    }

    // -----------------------------------------------------------------------------
    BOOL STDMETHODCALLTYPE IsSupported(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties) const
    {
        return _D2D1RenderTarget.IsSupported(renderTargetProperties);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetBitmap(_Outptr_ ID2D1Bitmap ** bitmap);
    NullPtr<void (D2D1BitmapRenderTargetHook::*)(_Outptr_ ID2D1Bitmap ** &)> _GetBitmap_pre_ptr;
    NullPtr<void (D2D1BitmapRenderTargetHook::*)(HRESULT, _Outptr_ ID2D1Bitmap **)> _GetBitmap_post_ptr;

};

// ==============================================================================
// ID2D1HwndRenderTarget
// ==============================================================================
class D2D1HwndRenderTargetHook : public HookBase<ID2D1HwndRenderTarget>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1RenderTargetHook & _D2D1RenderTarget;

protected:

    D2D1HwndRenderTargetHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1RenderTargetHook & D2D1RenderTarget, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1RenderTarget(D2D1RenderTarget)
    {
    }

    ~D2D1HwndRenderTargetHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1RenderTargetHook * D2D1RenderTarget = (D2D1RenderTargetHook *)unknown.GetHookedObj(__uuidof(ID2D1RenderTarget));
        if (nullptr == D2D1RenderTarget) return nullptr;

        try
        {
            return new D2D1HwndRenderTargetHook(unknown, *D2D1Resource, *D2D1RenderTarget, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1HwndRenderTargetHook * typedPtr = (D2D1HwndRenderTargetHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmap(D2D1_SIZE_U size, _In_opt_ CONST void * srcData, UINT32 pitch, _In_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateBitmap(size, srcData, pitch, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmapFromWicBitmap(_In_ IWICBitmapSource * wicBitmapSource, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSharedBitmap(_In_ REFIID riid, _Inout_ void * data, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateSharedBitmap(riid, data, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmapBrush(_In_opt_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES * bitmapBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1BitmapBrush ** bitmapBrush)
    {
        return _D2D1RenderTarget.CreateBitmapBrush(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSolidColorBrush(_In_ CONST D2D1_COLOR_F * color, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1SolidColorBrush ** solidColorBrush)
    {
        return _D2D1RenderTarget.CreateSolidColorBrush(color, brushProperties, solidColorBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateGradientStopCollection(_In_reads_(gradientStopsCount) CONST D2D1_GRADIENT_STOP * gradientStops, _In_range_(>=,1) UINT32 gradientStopsCount, D2D1_GAMMA colorInterpolationGamma, D2D1_EXTEND_MODE extendMode, _Outptr_ ID2D1GradientStopCollection ** gradientStopCollection)
    {
        return _D2D1RenderTarget.CreateGradientStopCollection(gradientStops, gradientStopsCount, colorInterpolationGamma, extendMode, gradientStopCollection);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateLinearGradientBrush(_In_ CONST D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * linearGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1LinearGradientBrush ** linearGradientBrush)
    {
        return _D2D1RenderTarget.CreateLinearGradientBrush(linearGradientBrushProperties, brushProperties, gradientStopCollection, linearGradientBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRadialGradientBrush(_In_ CONST D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * radialGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1RadialGradientBrush ** radialGradientBrush)
    {
        return _D2D1RenderTarget.CreateRadialGradientBrush(radialGradientBrushProperties, brushProperties, gradientStopCollection, radialGradientBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateCompatibleRenderTarget(_In_opt_ CONST D2D1_SIZE_F * desiredSize, _In_opt_ CONST D2D1_SIZE_U * desiredPixelSize, _In_opt_ CONST D2D1_PIXEL_FORMAT * desiredFormat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options, _Outptr_ ID2D1BitmapRenderTarget ** bitmapRenderTarget)
    {
        return _D2D1RenderTarget.CreateCompatibleRenderTarget(desiredSize, desiredPixelSize, desiredFormat, options, bitmapRenderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateLayer(_In_opt_ CONST D2D1_SIZE_F * size, _Outptr_ ID2D1Layer ** layer)
    {
        return _D2D1RenderTarget.CreateLayer(size, layer);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateMesh(_Outptr_ ID2D1Mesh ** mesh)
    {
        return _D2D1RenderTarget.CreateMesh(mesh);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawLine(D2D1_POINT_2F point0, D2D1_POINT_2F point1, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawLine(point0, point1, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawRectangle(rect, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillRectangle(rect, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawRoundedRectangle(roundedRect, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillRoundedRectangle(roundedRect, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawEllipse(ellipse, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillEllipse(ellipse, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawGeometry(geometry, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, _In_opt_ ID2D1Brush * opacityBrush = NULL)
    {
        return _D2D1RenderTarget.FillGeometry(geometry, brush, opacityBrush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillMesh(_In_ ID2D1Mesh * mesh, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillMesh(mesh, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillOpacityMask(_In_ ID2D1Bitmap * opacityMask, _In_ ID2D1Brush * brush, D2D1_OPACITY_MASK_CONTENT content, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL)
    {
        return _D2D1RenderTarget.FillOpacityMask(opacityMask, brush, content, destinationRectangle, sourceRectangle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawBitmap(_In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, FLOAT opacity = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL)
    {
        return _D2D1RenderTarget.DrawBitmap(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawText(_In_reads_(stringLength) CONST WCHAR * string, UINT32 stringLength, _In_ IDWriteTextFormat * textFormat, _In_ CONST D2D1_RECT_F * layoutRect, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL)
    {
        return _D2D1RenderTarget.DrawText(string, stringLength, textFormat, layoutRect, defaultForegroundBrush, options, measuringMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawTextLayout(D2D1_POINT_2F origin, _In_ IDWriteTextLayout * textLayout, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE)
    {
        return _D2D1RenderTarget.DrawTextLayout(origin, textLayout, defaultForegroundBrush, options);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawGlyphRun(D2D1_POINT_2F baselineOrigin, _In_ CONST DWRITE_GLYPH_RUN * glyphRun, _In_ ID2D1Brush * foregroundBrush, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL)
    {
        return _D2D1RenderTarget.DrawGlyphRun(baselineOrigin, glyphRun, foregroundBrush, measuringMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1RenderTarget.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1RenderTarget.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode)
    {
        return _D2D1RenderTarget.SetAntialiasMode(antialiasMode);
    }

    // -----------------------------------------------------------------------------
    D2D1_ANTIALIAS_MODE STDMETHODCALLTYPE GetAntialiasMode() const
    {
        return _D2D1RenderTarget.GetAntialiasMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode)
    {
        return _D2D1RenderTarget.SetTextAntialiasMode(textAntialiasMode);
    }

    // -----------------------------------------------------------------------------
    D2D1_TEXT_ANTIALIAS_MODE STDMETHODCALLTYPE GetTextAntialiasMode() const
    {
        return _D2D1RenderTarget.GetTextAntialiasMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextRenderingParams(_In_opt_ IDWriteRenderingParams * textRenderingParams = NULL)
    {
        return _D2D1RenderTarget.SetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTextRenderingParams(_Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const
    {
        return _D2D1RenderTarget.GetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTags(D2D1_TAG tag1, D2D1_TAG tag2)
    {
        return _D2D1RenderTarget.SetTags(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTags(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL) const
    {
        return _D2D1RenderTarget.GetTags(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PushLayer(_In_ CONST D2D1_LAYER_PARAMETERS * layerParameters, _In_opt_ ID2D1Layer * layer)
    {
        return _D2D1RenderTarget.PushLayer(layerParameters, layer);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PopLayer()
    {
        return _D2D1RenderTarget.PopLayer();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Flush(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL)
    {
        return _D2D1RenderTarget.Flush(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SaveDrawingState(_Inout_ ID2D1DrawingStateBlock * drawingStateBlock) const
    {
        return _D2D1RenderTarget.SaveDrawingState(drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RestoreDrawingState(_In_ ID2D1DrawingStateBlock * drawingStateBlock)
    {
        return _D2D1RenderTarget.RestoreDrawingState(drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PushAxisAlignedClip(_In_ CONST D2D1_RECT_F * clipRect, D2D1_ANTIALIAS_MODE antialiasMode)
    {
        return _D2D1RenderTarget.PushAxisAlignedClip(clipRect, antialiasMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PopAxisAlignedClip()
    {
        return _D2D1RenderTarget.PopAxisAlignedClip();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Clear(_In_opt_ CONST D2D1_COLOR_F * clearColor = NULL)
    {
        return _D2D1RenderTarget.Clear(clearColor);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE BeginDraw()
    {
        return _D2D1RenderTarget.BeginDraw();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EndDraw(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL)
    {
        return _D2D1RenderTarget.EndDraw(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    D2D1_PIXEL_FORMAT STDMETHODCALLTYPE GetPixelFormat() const
    {
        return _D2D1RenderTarget.GetPixelFormat();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetDpi(FLOAT dpiX, FLOAT dpiY)
    {
        return _D2D1RenderTarget.SetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY) const
    {
        return _D2D1RenderTarget.GetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_F STDMETHODCALLTYPE GetSize() const
    {
        return _D2D1RenderTarget.GetSize();
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_U STDMETHODCALLTYPE GetPixelSize() const
    {
        return _D2D1RenderTarget.GetPixelSize();
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetMaximumBitmapSize() const
    {
        return _D2D1RenderTarget.GetMaximumBitmapSize();
    }

    // -----------------------------------------------------------------------------
    BOOL STDMETHODCALLTYPE IsSupported(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties) const
    {
        return _D2D1RenderTarget.IsSupported(renderTargetProperties);
    }

    // -----------------------------------------------------------------------------
    virtual D2D1_WINDOW_STATE STDMETHODCALLTYPE CheckWindowState();
    NullPtr<void (D2D1HwndRenderTargetHook::*)()> _CheckWindowState_pre_ptr;
    NullPtr<void (D2D1HwndRenderTargetHook::*)(D2D1_WINDOW_STATE)> _CheckWindowState_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Resize(_In_ CONST D2D1_SIZE_U * pixelSize);
    NullPtr<void (D2D1HwndRenderTargetHook::*)(_In_ CONST D2D1_SIZE_U * &)> _Resize_pre_ptr;
    NullPtr<void (D2D1HwndRenderTargetHook::*)(HRESULT, _In_ CONST D2D1_SIZE_U *)> _Resize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HWND STDMETHODCALLTYPE GetHwnd() const;
    NullPtr<void (D2D1HwndRenderTargetHook::*)() const> _GetHwnd_pre_ptr;
    NullPtr<void (D2D1HwndRenderTargetHook::*)(HWND) const> _GetHwnd_post_ptr;

};

// ==============================================================================
// ID2D1GdiInteropRenderTarget
// ==============================================================================
class D2D1GdiInteropRenderTargetHook : public HookBase<ID2D1GdiInteropRenderTarget>
{

protected:

    D2D1GdiInteropRenderTargetHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1GdiInteropRenderTargetHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1GdiInteropRenderTargetHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1GdiInteropRenderTargetHook * typedPtr = (D2D1GdiInteropRenderTargetHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDC(D2D1_DC_INITIALIZE_MODE mode, _Out_ HDC * hdc);
    NullPtr<void (D2D1GdiInteropRenderTargetHook::*)(D2D1_DC_INITIALIZE_MODE &, _Out_ HDC * &)> _GetDC_pre_ptr;
    NullPtr<void (D2D1GdiInteropRenderTargetHook::*)(HRESULT, D2D1_DC_INITIALIZE_MODE, _Out_ HDC *)> _GetDC_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReleaseDC(_In_opt_ CONST RECT * update);
    NullPtr<void (D2D1GdiInteropRenderTargetHook::*)(_In_opt_ CONST RECT * &)> _ReleaseDC_pre_ptr;
    NullPtr<void (D2D1GdiInteropRenderTargetHook::*)(HRESULT, _In_opt_ CONST RECT *)> _ReleaseDC_post_ptr;

};

// ==============================================================================
// ID2D1DCRenderTarget
// ==============================================================================
class D2D1DCRenderTargetHook : public HookBase<ID2D1DCRenderTarget>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1RenderTargetHook & _D2D1RenderTarget;

protected:

    D2D1DCRenderTargetHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1RenderTargetHook & D2D1RenderTarget, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1RenderTarget(D2D1RenderTarget)
    {
    }

    ~D2D1DCRenderTargetHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1RenderTargetHook * D2D1RenderTarget = (D2D1RenderTargetHook *)unknown.GetHookedObj(__uuidof(ID2D1RenderTarget));
        if (nullptr == D2D1RenderTarget) return nullptr;

        try
        {
            return new D2D1DCRenderTargetHook(unknown, *D2D1Resource, *D2D1RenderTarget, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1DCRenderTargetHook * typedPtr = (D2D1DCRenderTargetHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmap(D2D1_SIZE_U size, _In_opt_ CONST void * srcData, UINT32 pitch, _In_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateBitmap(size, srcData, pitch, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmapFromWicBitmap(_In_ IWICBitmapSource * wicBitmapSource, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSharedBitmap(_In_ REFIID riid, _Inout_ void * data, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateSharedBitmap(riid, data, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmapBrush(_In_opt_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES * bitmapBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1BitmapBrush ** bitmapBrush)
    {
        return _D2D1RenderTarget.CreateBitmapBrush(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSolidColorBrush(_In_ CONST D2D1_COLOR_F * color, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1SolidColorBrush ** solidColorBrush)
    {
        return _D2D1RenderTarget.CreateSolidColorBrush(color, brushProperties, solidColorBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateGradientStopCollection(_In_reads_(gradientStopsCount) CONST D2D1_GRADIENT_STOP * gradientStops, _In_range_(>=,1) UINT32 gradientStopsCount, D2D1_GAMMA colorInterpolationGamma, D2D1_EXTEND_MODE extendMode, _Outptr_ ID2D1GradientStopCollection ** gradientStopCollection)
    {
        return _D2D1RenderTarget.CreateGradientStopCollection(gradientStops, gradientStopsCount, colorInterpolationGamma, extendMode, gradientStopCollection);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateLinearGradientBrush(_In_ CONST D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * linearGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1LinearGradientBrush ** linearGradientBrush)
    {
        return _D2D1RenderTarget.CreateLinearGradientBrush(linearGradientBrushProperties, brushProperties, gradientStopCollection, linearGradientBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRadialGradientBrush(_In_ CONST D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * radialGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1RadialGradientBrush ** radialGradientBrush)
    {
        return _D2D1RenderTarget.CreateRadialGradientBrush(radialGradientBrushProperties, brushProperties, gradientStopCollection, radialGradientBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateCompatibleRenderTarget(_In_opt_ CONST D2D1_SIZE_F * desiredSize, _In_opt_ CONST D2D1_SIZE_U * desiredPixelSize, _In_opt_ CONST D2D1_PIXEL_FORMAT * desiredFormat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options, _Outptr_ ID2D1BitmapRenderTarget ** bitmapRenderTarget)
    {
        return _D2D1RenderTarget.CreateCompatibleRenderTarget(desiredSize, desiredPixelSize, desiredFormat, options, bitmapRenderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateLayer(_In_opt_ CONST D2D1_SIZE_F * size, _Outptr_ ID2D1Layer ** layer)
    {
        return _D2D1RenderTarget.CreateLayer(size, layer);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateMesh(_Outptr_ ID2D1Mesh ** mesh)
    {
        return _D2D1RenderTarget.CreateMesh(mesh);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawLine(D2D1_POINT_2F point0, D2D1_POINT_2F point1, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawLine(point0, point1, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawRectangle(rect, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillRectangle(rect, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawRoundedRectangle(roundedRect, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillRoundedRectangle(roundedRect, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawEllipse(ellipse, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillEllipse(ellipse, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawGeometry(geometry, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, _In_opt_ ID2D1Brush * opacityBrush = NULL)
    {
        return _D2D1RenderTarget.FillGeometry(geometry, brush, opacityBrush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillMesh(_In_ ID2D1Mesh * mesh, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillMesh(mesh, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillOpacityMask(_In_ ID2D1Bitmap * opacityMask, _In_ ID2D1Brush * brush, D2D1_OPACITY_MASK_CONTENT content, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL)
    {
        return _D2D1RenderTarget.FillOpacityMask(opacityMask, brush, content, destinationRectangle, sourceRectangle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawBitmap(_In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, FLOAT opacity = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL)
    {
        return _D2D1RenderTarget.DrawBitmap(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawText(_In_reads_(stringLength) CONST WCHAR * string, UINT32 stringLength, _In_ IDWriteTextFormat * textFormat, _In_ CONST D2D1_RECT_F * layoutRect, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL)
    {
        return _D2D1RenderTarget.DrawText(string, stringLength, textFormat, layoutRect, defaultForegroundBrush, options, measuringMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawTextLayout(D2D1_POINT_2F origin, _In_ IDWriteTextLayout * textLayout, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE)
    {
        return _D2D1RenderTarget.DrawTextLayout(origin, textLayout, defaultForegroundBrush, options);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawGlyphRun(D2D1_POINT_2F baselineOrigin, _In_ CONST DWRITE_GLYPH_RUN * glyphRun, _In_ ID2D1Brush * foregroundBrush, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL)
    {
        return _D2D1RenderTarget.DrawGlyphRun(baselineOrigin, glyphRun, foregroundBrush, measuringMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1RenderTarget.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1RenderTarget.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode)
    {
        return _D2D1RenderTarget.SetAntialiasMode(antialiasMode);
    }

    // -----------------------------------------------------------------------------
    D2D1_ANTIALIAS_MODE STDMETHODCALLTYPE GetAntialiasMode() const
    {
        return _D2D1RenderTarget.GetAntialiasMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode)
    {
        return _D2D1RenderTarget.SetTextAntialiasMode(textAntialiasMode);
    }

    // -----------------------------------------------------------------------------
    D2D1_TEXT_ANTIALIAS_MODE STDMETHODCALLTYPE GetTextAntialiasMode() const
    {
        return _D2D1RenderTarget.GetTextAntialiasMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextRenderingParams(_In_opt_ IDWriteRenderingParams * textRenderingParams = NULL)
    {
        return _D2D1RenderTarget.SetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTextRenderingParams(_Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const
    {
        return _D2D1RenderTarget.GetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTags(D2D1_TAG tag1, D2D1_TAG tag2)
    {
        return _D2D1RenderTarget.SetTags(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTags(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL) const
    {
        return _D2D1RenderTarget.GetTags(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PushLayer(_In_ CONST D2D1_LAYER_PARAMETERS * layerParameters, _In_opt_ ID2D1Layer * layer)
    {
        return _D2D1RenderTarget.PushLayer(layerParameters, layer);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PopLayer()
    {
        return _D2D1RenderTarget.PopLayer();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Flush(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL)
    {
        return _D2D1RenderTarget.Flush(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SaveDrawingState(_Inout_ ID2D1DrawingStateBlock * drawingStateBlock) const
    {
        return _D2D1RenderTarget.SaveDrawingState(drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RestoreDrawingState(_In_ ID2D1DrawingStateBlock * drawingStateBlock)
    {
        return _D2D1RenderTarget.RestoreDrawingState(drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PushAxisAlignedClip(_In_ CONST D2D1_RECT_F * clipRect, D2D1_ANTIALIAS_MODE antialiasMode)
    {
        return _D2D1RenderTarget.PushAxisAlignedClip(clipRect, antialiasMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PopAxisAlignedClip()
    {
        return _D2D1RenderTarget.PopAxisAlignedClip();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Clear(_In_opt_ CONST D2D1_COLOR_F * clearColor = NULL)
    {
        return _D2D1RenderTarget.Clear(clearColor);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE BeginDraw()
    {
        return _D2D1RenderTarget.BeginDraw();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EndDraw(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL)
    {
        return _D2D1RenderTarget.EndDraw(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    D2D1_PIXEL_FORMAT STDMETHODCALLTYPE GetPixelFormat() const
    {
        return _D2D1RenderTarget.GetPixelFormat();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetDpi(FLOAT dpiX, FLOAT dpiY)
    {
        return _D2D1RenderTarget.SetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY) const
    {
        return _D2D1RenderTarget.GetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_F STDMETHODCALLTYPE GetSize() const
    {
        return _D2D1RenderTarget.GetSize();
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_U STDMETHODCALLTYPE GetPixelSize() const
    {
        return _D2D1RenderTarget.GetPixelSize();
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetMaximumBitmapSize() const
    {
        return _D2D1RenderTarget.GetMaximumBitmapSize();
    }

    // -----------------------------------------------------------------------------
    BOOL STDMETHODCALLTYPE IsSupported(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties) const
    {
        return _D2D1RenderTarget.IsSupported(renderTargetProperties);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE BindDC(_In_ CONST HDC hDC, _In_ CONST RECT * pSubRect);
    NullPtr<void (D2D1DCRenderTargetHook::*)(_In_ CONST HDC &, _In_ CONST RECT * &)> _BindDC_pre_ptr;
    NullPtr<void (D2D1DCRenderTargetHook::*)(HRESULT, _In_ CONST HDC, _In_ CONST RECT *)> _BindDC_post_ptr;

};

// ==============================================================================
// ID2D1Factory
// ==============================================================================
class D2D1FactoryHook : public HookBase<ID2D1Factory>
{

protected:

    D2D1FactoryHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1FactoryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1FactoryHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1FactoryHook * typedPtr = (D2D1FactoryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReloadSystemMetrics();
    NullPtr<void (D2D1FactoryHook::*)()> _ReloadSystemMetrics_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT)> _ReloadSystemMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDesktopDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY);
    NullPtr<void (D2D1FactoryHook::*)(_Out_ FLOAT * &, _Out_ FLOAT * &)> _GetDesktopDpi_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(_Out_ FLOAT *, _Out_ FLOAT *)> _GetDesktopDpi_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRectangleGeometry(_In_ CONST D2D1_RECT_F * rectangle, _Outptr_ ID2D1RectangleGeometry ** rectangleGeometry);
    NullPtr<void (D2D1FactoryHook::*)(_In_ CONST D2D1_RECT_F * &, _Outptr_ ID2D1RectangleGeometry ** &)> _CreateRectangleGeometry_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ CONST D2D1_RECT_F *, _Outptr_ ID2D1RectangleGeometry **)> _CreateRectangleGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRoundedRectangleGeometry(_In_ CONST D2D1_ROUNDED_RECT * roundedRectangle, _Outptr_ ID2D1RoundedRectangleGeometry ** roundedRectangleGeometry);
    NullPtr<void (D2D1FactoryHook::*)(_In_ CONST D2D1_ROUNDED_RECT * &, _Outptr_ ID2D1RoundedRectangleGeometry ** &)> _CreateRoundedRectangleGeometry_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ CONST D2D1_ROUNDED_RECT *, _Outptr_ ID2D1RoundedRectangleGeometry **)> _CreateRoundedRectangleGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateEllipseGeometry(_In_ CONST D2D1_ELLIPSE * ellipse, _Outptr_ ID2D1EllipseGeometry ** ellipseGeometry);
    NullPtr<void (D2D1FactoryHook::*)(_In_ CONST D2D1_ELLIPSE * &, _Outptr_ ID2D1EllipseGeometry ** &)> _CreateEllipseGeometry_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ CONST D2D1_ELLIPSE *, _Outptr_ ID2D1EllipseGeometry **)> _CreateEllipseGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateGeometryGroup(D2D1_FILL_MODE fillMode, _In_reads_(geometriesCount) ID2D1Geometry ** geometries, UINT32 geometriesCount, _Outptr_ ID2D1GeometryGroup ** geometryGroup);
    NullPtr<void (D2D1FactoryHook::*)(D2D1_FILL_MODE &, _In_reads_(geometriesCount) ID2D1Geometry ** &, UINT32 &, _Outptr_ ID2D1GeometryGroup ** &)> _CreateGeometryGroup_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, D2D1_FILL_MODE, _In_reads_(geometriesCount) ID2D1Geometry **, UINT32, _Outptr_ ID2D1GeometryGroup **)> _CreateGeometryGroup_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTransformedGeometry(_In_ ID2D1Geometry * sourceGeometry, _In_ CONST D2D1_MATRIX_3X2_F * transform, _Outptr_ ID2D1TransformedGeometry ** transformedGeometry);
    NullPtr<void (D2D1FactoryHook::*)(_In_ ID2D1Geometry * &, _In_ CONST D2D1_MATRIX_3X2_F * &, _Outptr_ ID2D1TransformedGeometry ** &)> _CreateTransformedGeometry_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ ID2D1Geometry *, _In_ CONST D2D1_MATRIX_3X2_F *, _Outptr_ ID2D1TransformedGeometry **)> _CreateTransformedGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreatePathGeometry(_Outptr_ ID2D1PathGeometry ** pathGeometry);
    NullPtr<void (D2D1FactoryHook::*)(_Outptr_ ID2D1PathGeometry ** &)> _CreatePathGeometry_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _Outptr_ ID2D1PathGeometry **)> _CreatePathGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateStrokeStyle(_In_ CONST D2D1_STROKE_STYLE_PROPERTIES * strokeStyleProperties, _In_reads_opt_(dashesCount) CONST FLOAT * dashes, UINT32 dashesCount, _Outptr_ ID2D1StrokeStyle ** strokeStyle);
    NullPtr<void (D2D1FactoryHook::*)(_In_ CONST D2D1_STROKE_STYLE_PROPERTIES * &, _In_reads_opt_(dashesCount) CONST FLOAT * &, UINT32 &, _Outptr_ ID2D1StrokeStyle ** &)> _CreateStrokeStyle_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ CONST D2D1_STROKE_STYLE_PROPERTIES *, _In_reads_opt_(dashesCount) CONST FLOAT *, UINT32, _Outptr_ ID2D1StrokeStyle **)> _CreateStrokeStyle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDrawingStateBlock(_In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION * drawingStateDescription, _In_opt_ IDWriteRenderingParams * textRenderingParams, _Outptr_ ID2D1DrawingStateBlock ** drawingStateBlock);
    NullPtr<void (D2D1FactoryHook::*)(_In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION * &, _In_opt_ IDWriteRenderingParams * &, _Outptr_ ID2D1DrawingStateBlock ** &)> _CreateDrawingStateBlock_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION *, _In_opt_ IDWriteRenderingParams *, _Outptr_ ID2D1DrawingStateBlock **)> _CreateDrawingStateBlock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateWicBitmapRenderTarget(_In_ IWICBitmap * target, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties, _Outptr_ ID2D1RenderTarget ** renderTarget);
    NullPtr<void (D2D1FactoryHook::*)(_In_ IWICBitmap * &, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * &, _Outptr_ ID2D1RenderTarget ** &)> _CreateWicBitmapRenderTarget_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ IWICBitmap *, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES *, _Outptr_ ID2D1RenderTarget **)> _CreateWicBitmapRenderTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateHwndRenderTarget(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties, _In_ CONST D2D1_HWND_RENDER_TARGET_PROPERTIES * hwndRenderTargetProperties, _Outptr_ ID2D1HwndRenderTarget ** hwndRenderTarget);
    NullPtr<void (D2D1FactoryHook::*)(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * &, _In_ CONST D2D1_HWND_RENDER_TARGET_PROPERTIES * &, _Outptr_ ID2D1HwndRenderTarget ** &)> _CreateHwndRenderTarget_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES *, _In_ CONST D2D1_HWND_RENDER_TARGET_PROPERTIES *, _Outptr_ ID2D1HwndRenderTarget **)> _CreateHwndRenderTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDxgiSurfaceRenderTarget(_In_ IDXGISurface * dxgiSurface, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties, _Outptr_ ID2D1RenderTarget ** renderTarget);
    NullPtr<void (D2D1FactoryHook::*)(_In_ IDXGISurface * &, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * &, _Outptr_ ID2D1RenderTarget ** &)> _CreateDxgiSurfaceRenderTarget_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ IDXGISurface *, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES *, _Outptr_ ID2D1RenderTarget **)> _CreateDxgiSurfaceRenderTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDCRenderTarget(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties, _Outptr_ ID2D1DCRenderTarget ** dcRenderTarget);
    NullPtr<void (D2D1FactoryHook::*)(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * &, _Outptr_ ID2D1DCRenderTarget ** &)> _CreateDCRenderTarget_pre_ptr;
    NullPtr<void (D2D1FactoryHook::*)(HRESULT, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES *, _Outptr_ ID2D1DCRenderTarget **)> _CreateDCRenderTarget_post_ptr;

};

// ==============================================================================
// ID2D1GdiMetafileSink
// ==============================================================================
class D2D1GdiMetafileSinkHook : public HookBase<ID2D1GdiMetafileSink>
{

protected:

    D2D1GdiMetafileSinkHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1GdiMetafileSinkHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1GdiMetafileSinkHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1GdiMetafileSinkHook * typedPtr = (D2D1GdiMetafileSinkHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ProcessRecord(DWORD recordType, _In_opt_ CONST void * recordData, DWORD recordDataSize);
    NullPtr<void (D2D1GdiMetafileSinkHook::*)(DWORD &, _In_opt_ CONST void * &, DWORD &)> _ProcessRecord_pre_ptr;
    NullPtr<void (D2D1GdiMetafileSinkHook::*)(HRESULT, DWORD, _In_opt_ CONST void *, DWORD)> _ProcessRecord_post_ptr;

};

// ==============================================================================
// ID2D1GdiMetafile
// ==============================================================================
class D2D1GdiMetafileHook : public HookBase<ID2D1GdiMetafile>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1GdiMetafileHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1GdiMetafileHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1GdiMetafileHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1GdiMetafileHook * typedPtr = (D2D1GdiMetafileHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Stream(_In_ ID2D1GdiMetafileSink * sink);
    NullPtr<void (D2D1GdiMetafileHook::*)(_In_ ID2D1GdiMetafileSink * &)> _Stream_pre_ptr;
    NullPtr<void (D2D1GdiMetafileHook::*)(HRESULT, _In_ ID2D1GdiMetafileSink *)> _Stream_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetBounds(_Out_ D2D1_RECT_F * bounds);
    NullPtr<void (D2D1GdiMetafileHook::*)(_Out_ D2D1_RECT_F * &)> _GetBounds_pre_ptr;
    NullPtr<void (D2D1GdiMetafileHook::*)(HRESULT, _Out_ D2D1_RECT_F *)> _GetBounds_post_ptr;

};

// ==============================================================================
// ID2D1CommandSink
// ==============================================================================
class D2D1CommandSinkHook : public HookBase<ID2D1CommandSink>
{

protected:

    D2D1CommandSinkHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1CommandSinkHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1CommandSinkHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1CommandSinkHook * typedPtr = (D2D1CommandSinkHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE BeginDraw();
    NullPtr<void (D2D1CommandSinkHook::*)()> _BeginDraw_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT)> _BeginDraw_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE EndDraw();
    NullPtr<void (D2D1CommandSinkHook::*)()> _EndDraw_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT)> _EndDraw_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode);
    NullPtr<void (D2D1CommandSinkHook::*)(D2D1_ANTIALIAS_MODE &)> _SetAntialiasMode_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, D2D1_ANTIALIAS_MODE)> _SetAntialiasMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTags(D2D1_TAG tag1, D2D1_TAG tag2);
    NullPtr<void (D2D1CommandSinkHook::*)(D2D1_TAG &, D2D1_TAG &)> _SetTags_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, D2D1_TAG, D2D1_TAG)> _SetTags_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode);
    NullPtr<void (D2D1CommandSinkHook::*)(D2D1_TEXT_ANTIALIAS_MODE &)> _SetTextAntialiasMode_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, D2D1_TEXT_ANTIALIAS_MODE)> _SetTextAntialiasMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTextRenderingParams(_In_opt_ IDWriteRenderingParams * textRenderingParams);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_opt_ IDWriteRenderingParams * &)> _SetTextRenderingParams_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_opt_ IDWriteRenderingParams *)> _SetTextRenderingParams_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ CONST D2D1_MATRIX_3X2_F * &)> _SetTransform_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ CONST D2D1_MATRIX_3X2_F *)> _SetTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND primitiveBlend);
    NullPtr<void (D2D1CommandSinkHook::*)(D2D1_PRIMITIVE_BLEND &)> _SetPrimitiveBlend_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, D2D1_PRIMITIVE_BLEND)> _SetPrimitiveBlend_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetUnitMode(D2D1_UNIT_MODE unitMode);
    NullPtr<void (D2D1CommandSinkHook::*)(D2D1_UNIT_MODE &)> _SetUnitMode_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, D2D1_UNIT_MODE)> _SetUnitMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Clear(_In_opt_ CONST D2D1_COLOR_F * color);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_opt_ CONST D2D1_COLOR_F * &)> _Clear_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_opt_ CONST D2D1_COLOR_F *)> _Clear_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawGlyphRun(D2D1_POINT_2F baselineOrigin, _In_ CONST DWRITE_GLYPH_RUN * glyphRun, _In_opt_ CONST DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription, _In_ ID2D1Brush * foregroundBrush, DWRITE_MEASURING_MODE measuringMode);
    NullPtr<void (D2D1CommandSinkHook::*)(D2D1_POINT_2F &, _In_ CONST DWRITE_GLYPH_RUN * &, _In_opt_ CONST DWRITE_GLYPH_RUN_DESCRIPTION * &, _In_ ID2D1Brush * &, DWRITE_MEASURING_MODE &)> _DrawGlyphRun_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, D2D1_POINT_2F, _In_ CONST DWRITE_GLYPH_RUN *, _In_opt_ CONST DWRITE_GLYPH_RUN_DESCRIPTION *, _In_ ID2D1Brush *, DWRITE_MEASURING_MODE)> _DrawGlyphRun_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawLine(D2D1_POINT_2F point0, D2D1_POINT_2F point1, _In_ ID2D1Brush * brush, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle);
    NullPtr<void (D2D1CommandSinkHook::*)(D2D1_POINT_2F &, D2D1_POINT_2F &, _In_ ID2D1Brush * &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &)> _DrawLine_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, D2D1_POINT_2F, D2D1_POINT_2F, _In_ ID2D1Brush *, FLOAT, _In_opt_ ID2D1StrokeStyle *)> _DrawLine_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ ID2D1Geometry * &, _In_ ID2D1Brush * &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &)> _DrawGeometry_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ ID2D1Geometry *, _In_ ID2D1Brush *, FLOAT, _In_opt_ ID2D1StrokeStyle *)> _DrawGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ CONST D2D1_RECT_F * &, _In_ ID2D1Brush * &, FLOAT &, _In_opt_ ID2D1StrokeStyle * &)> _DrawRectangle_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ CONST D2D1_RECT_F *, _In_ ID2D1Brush *, FLOAT, _In_opt_ ID2D1StrokeStyle *)> _DrawRectangle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawBitmap(_In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_F * destinationRectangle, FLOAT opacity, D2D1_INTERPOLATION_MODE interpolationMode, _In_opt_ CONST D2D1_RECT_F * sourceRectangle, _In_opt_ CONST D2D1_MATRIX_4X4_F * perspectiveTransform);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ ID2D1Bitmap * &, _In_opt_ CONST D2D1_RECT_F * &, FLOAT &, D2D1_INTERPOLATION_MODE &, _In_opt_ CONST D2D1_RECT_F * &, _In_opt_ CONST D2D1_MATRIX_4X4_F * &)> _DrawBitmap_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ ID2D1Bitmap *, _In_opt_ CONST D2D1_RECT_F *, FLOAT, D2D1_INTERPOLATION_MODE, _In_opt_ CONST D2D1_RECT_F *, _In_opt_ CONST D2D1_MATRIX_4X4_F *)> _DrawBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawImage(_In_ ID2D1Image * image, _In_opt_ CONST D2D1_POINT_2F * targetOffset, _In_opt_ CONST D2D1_RECT_F * imageRectangle, D2D1_INTERPOLATION_MODE interpolationMode, D2D1_COMPOSITE_MODE compositeMode);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ ID2D1Image * &, _In_opt_ CONST D2D1_POINT_2F * &, _In_opt_ CONST D2D1_RECT_F * &, D2D1_INTERPOLATION_MODE &, D2D1_COMPOSITE_MODE &)> _DrawImage_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ ID2D1Image *, _In_opt_ CONST D2D1_POINT_2F *, _In_opt_ CONST D2D1_RECT_F *, D2D1_INTERPOLATION_MODE, D2D1_COMPOSITE_MODE)> _DrawImage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawGdiMetafile(_In_ ID2D1GdiMetafile * gdiMetafile, _In_opt_ CONST D2D1_POINT_2F * targetOffset);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ ID2D1GdiMetafile * &, _In_opt_ CONST D2D1_POINT_2F * &)> _DrawGdiMetafile_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ ID2D1GdiMetafile *, _In_opt_ CONST D2D1_POINT_2F *)> _DrawGdiMetafile_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FillMesh(_In_ ID2D1Mesh * mesh, _In_ ID2D1Brush * brush);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ ID2D1Mesh * &, _In_ ID2D1Brush * &)> _FillMesh_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ ID2D1Mesh *, _In_ ID2D1Brush *)> _FillMesh_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FillOpacityMask(_In_ ID2D1Bitmap * opacityMask, _In_ ID2D1Brush * brush, _In_opt_ CONST D2D1_RECT_F * destinationRectangle, _In_opt_ CONST D2D1_RECT_F * sourceRectangle);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ ID2D1Bitmap * &, _In_ ID2D1Brush * &, _In_opt_ CONST D2D1_RECT_F * &, _In_opt_ CONST D2D1_RECT_F * &)> _FillOpacityMask_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ ID2D1Bitmap *, _In_ ID2D1Brush *, _In_opt_ CONST D2D1_RECT_F *, _In_opt_ CONST D2D1_RECT_F *)> _FillOpacityMask_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FillGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, _In_opt_ ID2D1Brush * opacityBrush);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ ID2D1Geometry * &, _In_ ID2D1Brush * &, _In_opt_ ID2D1Brush * &)> _FillGeometry_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ ID2D1Geometry *, _In_ ID2D1Brush *, _In_opt_ ID2D1Brush *)> _FillGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FillRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ CONST D2D1_RECT_F * &, _In_ ID2D1Brush * &)> _FillRectangle_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ CONST D2D1_RECT_F *, _In_ ID2D1Brush *)> _FillRectangle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PushAxisAlignedClip(_In_ CONST D2D1_RECT_F * clipRect, D2D1_ANTIALIAS_MODE antialiasMode);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ CONST D2D1_RECT_F * &, D2D1_ANTIALIAS_MODE &)> _PushAxisAlignedClip_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ CONST D2D1_RECT_F *, D2D1_ANTIALIAS_MODE)> _PushAxisAlignedClip_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PushLayer(_In_ CONST D2D1_LAYER_PARAMETERS1 * layerParameters1, _In_opt_ ID2D1Layer * layer);
    NullPtr<void (D2D1CommandSinkHook::*)(_In_ CONST D2D1_LAYER_PARAMETERS1 * &, _In_opt_ ID2D1Layer * &)> _PushLayer_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT, _In_ CONST D2D1_LAYER_PARAMETERS1 *, _In_opt_ ID2D1Layer *)> _PushLayer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PopAxisAlignedClip();
    NullPtr<void (D2D1CommandSinkHook::*)()> _PopAxisAlignedClip_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT)> _PopAxisAlignedClip_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE PopLayer();
    NullPtr<void (D2D1CommandSinkHook::*)()> _PopLayer_pre_ptr;
    NullPtr<void (D2D1CommandSinkHook::*)(HRESULT)> _PopLayer_post_ptr;

};

// ==============================================================================
// ID2D1CommandList
// ==============================================================================
class D2D1CommandListHook : public HookBase<ID2D1CommandList>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1ImageHook & _D2D1Image;

protected:

    D2D1CommandListHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1ImageHook & D2D1Image, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Image(D2D1Image)
    {
    }

    ~D2D1CommandListHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1ImageHook * D2D1Image = (D2D1ImageHook *)unknown.GetHookedObj(__uuidof(ID2D1Image));
        if (nullptr == D2D1Image) return nullptr;

        try
        {
            return new D2D1CommandListHook(unknown, *D2D1Resource, *D2D1Image, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1CommandListHook * typedPtr = (D2D1CommandListHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Stream(_In_ ID2D1CommandSink * sink);
    NullPtr<void (D2D1CommandListHook::*)(_In_ ID2D1CommandSink * &)> _Stream_pre_ptr;
    NullPtr<void (D2D1CommandListHook::*)(HRESULT, _In_ ID2D1CommandSink *)> _Stream_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Close();
    NullPtr<void (D2D1CommandListHook::*)()> _Close_pre_ptr;
    NullPtr<void (D2D1CommandListHook::*)(HRESULT)> _Close_post_ptr;

};

// ==============================================================================
// ID2D1PrintControl
// ==============================================================================
class D2D1PrintControlHook : public HookBase<ID2D1PrintControl>
{

protected:

    D2D1PrintControlHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1PrintControlHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1PrintControlHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1PrintControlHook * typedPtr = (D2D1PrintControlHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddPage(_In_ ID2D1CommandList * commandList, D2D_SIZE_F pageSize, _In_opt_ IStream * pagePrintTicketStream, _Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL);
    NullPtr<void (D2D1PrintControlHook::*)(_In_ ID2D1CommandList * &, D2D_SIZE_F &, _In_opt_ IStream * &, _Out_opt_ D2D1_TAG * &, _Out_opt_ D2D1_TAG * &)> _AddPage_pre_ptr;
    NullPtr<void (D2D1PrintControlHook::*)(HRESULT, _In_ ID2D1CommandList *, D2D_SIZE_F, _In_opt_ IStream *, _Out_opt_ D2D1_TAG *, _Out_opt_ D2D1_TAG *)> _AddPage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Close();
    NullPtr<void (D2D1PrintControlHook::*)()> _Close_pre_ptr;
    NullPtr<void (D2D1PrintControlHook::*)(HRESULT)> _Close_post_ptr;

};

// ==============================================================================
// ID2D1ImageBrush
// ==============================================================================
class D2D1ImageBrushHook : public HookBase<ID2D1ImageBrush>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1BrushHook & _D2D1Brush;

protected:

    D2D1ImageBrushHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1BrushHook & D2D1Brush, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Brush(D2D1Brush)
    {
    }

    ~D2D1ImageBrushHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1BrushHook * D2D1Brush = (D2D1BrushHook *)unknown.GetHookedObj(__uuidof(ID2D1Brush));
        if (nullptr == D2D1Brush) return nullptr;

        try
        {
            return new D2D1ImageBrushHook(unknown, *D2D1Resource, *D2D1Brush, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ImageBrushHook * typedPtr = (D2D1ImageBrushHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetOpacity(FLOAT opacity)
    {
        return _D2D1Brush.SetOpacity(opacity);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1Brush.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetOpacity() const
    {
        return _D2D1Brush.GetOpacity();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1Brush.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetImage(_In_opt_ ID2D1Image * image);
    NullPtr<void (D2D1ImageBrushHook::*)(_In_opt_ ID2D1Image * &)> _SetImage_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(_In_opt_ ID2D1Image *)> _SetImage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetExtendModeX(D2D1_EXTEND_MODE extendModeX);
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_EXTEND_MODE &)> _SetExtendModeX_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_EXTEND_MODE)> _SetExtendModeX_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetExtendModeY(D2D1_EXTEND_MODE extendModeY);
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_EXTEND_MODE &)> _SetExtendModeY_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_EXTEND_MODE)> _SetExtendModeY_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetInterpolationMode(D2D1_INTERPOLATION_MODE interpolationMode);
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_INTERPOLATION_MODE &)> _SetInterpolationMode_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_INTERPOLATION_MODE)> _SetInterpolationMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetSourceRectangle(_In_ CONST D2D1_RECT_F * sourceRectangle);
    NullPtr<void (D2D1ImageBrushHook::*)(_In_ CONST D2D1_RECT_F * &)> _SetSourceRectangle_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(_In_ CONST D2D1_RECT_F *)> _SetSourceRectangle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetImage(_Outptr_ ID2D1Image ** image) const;
    NullPtr<void (D2D1ImageBrushHook::*)(_Outptr_ ID2D1Image ** &) const> _GetImage_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(_Outptr_ ID2D1Image **) const> _GetImage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_EXTEND_MODE STDMETHODCALLTYPE GetExtendModeX() const;
    NullPtr<void (D2D1ImageBrushHook::*)() const> _GetExtendModeX_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_EXTEND_MODE) const> _GetExtendModeX_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_EXTEND_MODE STDMETHODCALLTYPE GetExtendModeY() const;
    NullPtr<void (D2D1ImageBrushHook::*)() const> _GetExtendModeY_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_EXTEND_MODE) const> _GetExtendModeY_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_INTERPOLATION_MODE STDMETHODCALLTYPE GetInterpolationMode() const;
    NullPtr<void (D2D1ImageBrushHook::*)() const> _GetInterpolationMode_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(D2D1_INTERPOLATION_MODE) const> _GetInterpolationMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetSourceRectangle(_Out_ D2D1_RECT_F * sourceRectangle) const;
    NullPtr<void (D2D1ImageBrushHook::*)(_Out_ D2D1_RECT_F * &) const> _GetSourceRectangle_pre_ptr;
    NullPtr<void (D2D1ImageBrushHook::*)(_Out_ D2D1_RECT_F *) const> _GetSourceRectangle_post_ptr;

};

// ==============================================================================
// ID2D1BitmapBrush1
// ==============================================================================
class D2D1BitmapBrush1Hook : public HookBase<ID2D1BitmapBrush1>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1BrushHook & _D2D1Brush;
    D2D1BitmapBrushHook & _D2D1BitmapBrush;

protected:

    D2D1BitmapBrush1Hook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1BrushHook & D2D1Brush, D2D1BitmapBrushHook & D2D1BitmapBrush, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Brush(D2D1Brush)
        , _D2D1BitmapBrush(D2D1BitmapBrush)
    {
    }

    ~D2D1BitmapBrush1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1BrushHook * D2D1Brush = (D2D1BrushHook *)unknown.GetHookedObj(__uuidof(ID2D1Brush));
        if (nullptr == D2D1Brush) return nullptr;

        D2D1BitmapBrushHook * D2D1BitmapBrush = (D2D1BitmapBrushHook *)unknown.GetHookedObj(__uuidof(ID2D1BitmapBrush));
        if (nullptr == D2D1BitmapBrush) return nullptr;

        try
        {
            return new D2D1BitmapBrush1Hook(unknown, *D2D1Resource, *D2D1Brush, *D2D1BitmapBrush, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1BitmapBrush1Hook * typedPtr = (D2D1BitmapBrush1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetOpacity(FLOAT opacity)
    {
        return _D2D1Brush.SetOpacity(opacity);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1Brush.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetOpacity() const
    {
        return _D2D1Brush.GetOpacity();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1Brush.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetExtendModeX(D2D1_EXTEND_MODE extendModeX)
    {
        return _D2D1BitmapBrush.SetExtendModeX(extendModeX);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetExtendModeY(D2D1_EXTEND_MODE extendModeY)
    {
        return _D2D1BitmapBrush.SetExtendModeY(extendModeY);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetInterpolationMode(D2D1_BITMAP_INTERPOLATION_MODE interpolationMode)
    {
        return _D2D1BitmapBrush.SetInterpolationMode(interpolationMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetBitmap(_In_opt_ ID2D1Bitmap * bitmap)
    {
        return _D2D1BitmapBrush.SetBitmap(bitmap);
    }

    // -----------------------------------------------------------------------------
    D2D1_EXTEND_MODE STDMETHODCALLTYPE GetExtendModeX() const
    {
        return _D2D1BitmapBrush.GetExtendModeX();
    }

    // -----------------------------------------------------------------------------
    D2D1_EXTEND_MODE STDMETHODCALLTYPE GetExtendModeY() const
    {
        return _D2D1BitmapBrush.GetExtendModeY();
    }

    // -----------------------------------------------------------------------------
    D2D1_BITMAP_INTERPOLATION_MODE STDMETHODCALLTYPE GetInterpolationMode() const
    {
        return _D2D1BitmapBrush.GetInterpolationMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetBitmap(_Outptr_ ID2D1Bitmap ** bitmap) const
    {
        return _D2D1BitmapBrush.GetBitmap(bitmap);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetInterpolationMode1(D2D1_INTERPOLATION_MODE interpolationMode);
    NullPtr<void (D2D1BitmapBrush1Hook::*)(D2D1_INTERPOLATION_MODE &)> _SetInterpolationMode1_pre_ptr;
    NullPtr<void (D2D1BitmapBrush1Hook::*)(D2D1_INTERPOLATION_MODE)> _SetInterpolationMode1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_INTERPOLATION_MODE STDMETHODCALLTYPE GetInterpolationMode1() const;
    NullPtr<void (D2D1BitmapBrush1Hook::*)() const> _GetInterpolationMode1_pre_ptr;
    NullPtr<void (D2D1BitmapBrush1Hook::*)(D2D1_INTERPOLATION_MODE) const> _GetInterpolationMode1_post_ptr;

};

// ==============================================================================
// ID2D1StrokeStyle1
// ==============================================================================
class D2D1StrokeStyle1Hook : public HookBase<ID2D1StrokeStyle1>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1StrokeStyleHook & _D2D1StrokeStyle;

protected:

    D2D1StrokeStyle1Hook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1StrokeStyleHook & D2D1StrokeStyle, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1StrokeStyle(D2D1StrokeStyle)
    {
    }

    ~D2D1StrokeStyle1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1StrokeStyleHook * D2D1StrokeStyle = (D2D1StrokeStyleHook *)unknown.GetHookedObj(__uuidof(ID2D1StrokeStyle));
        if (nullptr == D2D1StrokeStyle) return nullptr;

        try
        {
            return new D2D1StrokeStyle1Hook(unknown, *D2D1Resource, *D2D1StrokeStyle, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1StrokeStyle1Hook * typedPtr = (D2D1StrokeStyle1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    D2D1_CAP_STYLE STDMETHODCALLTYPE GetStartCap() const
    {
        return _D2D1StrokeStyle.GetStartCap();
    }

    // -----------------------------------------------------------------------------
    D2D1_CAP_STYLE STDMETHODCALLTYPE GetEndCap() const
    {
        return _D2D1StrokeStyle.GetEndCap();
    }

    // -----------------------------------------------------------------------------
    D2D1_CAP_STYLE STDMETHODCALLTYPE GetDashCap() const
    {
        return _D2D1StrokeStyle.GetDashCap();
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetMiterLimit() const
    {
        return _D2D1StrokeStyle.GetMiterLimit();
    }

    // -----------------------------------------------------------------------------
    D2D1_LINE_JOIN STDMETHODCALLTYPE GetLineJoin() const
    {
        return _D2D1StrokeStyle.GetLineJoin();
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetDashOffset() const
    {
        return _D2D1StrokeStyle.GetDashOffset();
    }

    // -----------------------------------------------------------------------------
    D2D1_DASH_STYLE STDMETHODCALLTYPE GetDashStyle() const
    {
        return _D2D1StrokeStyle.GetDashStyle();
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetDashesCount() const
    {
        return _D2D1StrokeStyle.GetDashesCount();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDashes(_Out_writes_(dashesCount) FLOAT * dashes, UINT32 dashesCount) const
    {
        return _D2D1StrokeStyle.GetDashes(dashes, dashesCount);
    }

    // -----------------------------------------------------------------------------
    virtual D2D1_STROKE_TRANSFORM_TYPE STDMETHODCALLTYPE GetStrokeTransformType() const;
    NullPtr<void (D2D1StrokeStyle1Hook::*)() const> _GetStrokeTransformType_pre_ptr;
    NullPtr<void (D2D1StrokeStyle1Hook::*)(D2D1_STROKE_TRANSFORM_TYPE) const> _GetStrokeTransformType_post_ptr;

};

// ==============================================================================
// ID2D1PathGeometry1
// ==============================================================================
class D2D1PathGeometry1Hook : public HookBase<ID2D1PathGeometry1>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1GeometryHook & _D2D1Geometry;
    D2D1PathGeometryHook & _D2D1PathGeometry;

protected:

    D2D1PathGeometry1Hook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1GeometryHook & D2D1Geometry, D2D1PathGeometryHook & D2D1PathGeometry, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Geometry(D2D1Geometry)
        , _D2D1PathGeometry(D2D1PathGeometry)
    {
    }

    ~D2D1PathGeometry1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1GeometryHook * D2D1Geometry = (D2D1GeometryHook *)unknown.GetHookedObj(__uuidof(ID2D1Geometry));
        if (nullptr == D2D1Geometry) return nullptr;

        D2D1PathGeometryHook * D2D1PathGeometry = (D2D1PathGeometryHook *)unknown.GetHookedObj(__uuidof(ID2D1PathGeometry));
        if (nullptr == D2D1PathGeometry) return nullptr;

        try
        {
            return new D2D1PathGeometry1Hook(unknown, *D2D1Resource, *D2D1Geometry, *D2D1PathGeometry, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1PathGeometry1Hook * typedPtr = (D2D1PathGeometry1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetBounds(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetBounds(worldTransform, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetWidenedBounds(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_RECT_F * bounds) const
    {
        return _D2D1Geometry.GetWidenedBounds(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, bounds);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE StrokeContainsPoint(D2D1_POINT_2F point, FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.StrokeContainsPoint(point, strokeWidth, strokeStyle, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE FillContainsPoint(D2D1_POINT_2F point, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ BOOL * contains) const
    {
        return _D2D1Geometry.FillContainsPoint(point, worldTransform, flatteningTolerance, contains);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CompareWithGeometry(_In_ ID2D1Geometry * inputGeometry, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _Out_ D2D1_GEOMETRY_RELATION * relation) const
    {
        return _D2D1Geometry.CompareWithGeometry(inputGeometry, inputGeometryTransform, flatteningTolerance, relation);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Simplify(simplificationOption, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Tessellate(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1TessellationSink * tessellationSink) const
    {
        return _D2D1Geometry.Tessellate(worldTransform, flatteningTolerance, tessellationSink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CombineWithGeometry(_In_ ID2D1Geometry * inputGeometry, D2D1_COMBINE_MODE combineMode, _In_opt_ CONST D2D1_MATRIX_3X2_F * inputGeometryTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.CombineWithGeometry(inputGeometry, combineMode, inputGeometryTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Outline(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Outline(worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeArea(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * area) const
    {
        return _D2D1Geometry.ComputeArea(worldTransform, flatteningTolerance, area);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputeLength(_In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ FLOAT * length) const
    {
        return _D2D1Geometry.ComputeLength(worldTransform, flatteningTolerance, length);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ComputePointAtLength(FLOAT length, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_opt_ D2D1_POINT_2F * point, _Out_opt_ D2D1_POINT_2F * unitTangentVector) const
    {
        return _D2D1Geometry.ComputePointAtLength(length, worldTransform, flatteningTolerance, point, unitTangentVector);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Widen(FLOAT strokeWidth, _In_opt_ ID2D1StrokeStyle * strokeStyle, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _In_ ID2D1SimplifiedGeometrySink * geometrySink) const
    {
        return _D2D1Geometry.Widen(strokeWidth, strokeStyle, worldTransform, flatteningTolerance, geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Open(_Outptr_ ID2D1GeometrySink ** geometrySink)
    {
        return _D2D1PathGeometry.Open(geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Stream(_In_ ID2D1GeometrySink * geometrySink) const
    {
        return _D2D1PathGeometry.Stream(geometrySink);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetSegmentCount(_Out_ UINT32 * count) const
    {
        return _D2D1PathGeometry.GetSegmentCount(count);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFigureCount(_Out_ UINT32 * count) const
    {
        return _D2D1PathGeometry.GetFigureCount(count);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ComputePointAndSegmentAtLength(FLOAT length, UINT32 startSegment, _In_opt_ CONST D2D1_MATRIX_3X2_F * worldTransform, FLOAT flatteningTolerance, _Out_ D2D1_POINT_DESCRIPTION * pointDescription) const;
    NullPtr<void (D2D1PathGeometry1Hook::*)(FLOAT &, UINT32 &, _In_opt_ CONST D2D1_MATRIX_3X2_F * &, FLOAT &, _Out_ D2D1_POINT_DESCRIPTION * &) const> _ComputePointAndSegmentAtLength_pre_ptr;
    NullPtr<void (D2D1PathGeometry1Hook::*)(HRESULT, FLOAT, UINT32, _In_opt_ CONST D2D1_MATRIX_3X2_F *, FLOAT, _Out_ D2D1_POINT_DESCRIPTION *) const> _ComputePointAndSegmentAtLength_post_ptr;

};

// ==============================================================================
// ID2D1Properties
// ==============================================================================
class D2D1PropertiesHook : public HookBase<ID2D1Properties>
{

protected:

    D2D1PropertiesHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1PropertiesHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1PropertiesHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1PropertiesHook * typedPtr = (D2D1PropertiesHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetPropertyCount() const;
    NullPtr<void (D2D1PropertiesHook::*)() const> _GetPropertyCount_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32) const> _GetPropertyCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPropertyName(UINT32 index, _Out_writes_(nameCount) PWSTR name, UINT32 nameCount) const;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32 &, _Out_writes_(nameCount) PWSTR &, UINT32 &) const> _GetPropertyName_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(HRESULT, UINT32, _Out_writes_(nameCount) PWSTR, UINT32) const> _GetPropertyName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetPropertyNameLength(UINT32 index) const;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32 &) const> _GetPropertyNameLength_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32, UINT32) const> _GetPropertyNameLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_PROPERTY_TYPE STDMETHODCALLTYPE GetType(UINT32 index) const;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32 &) const> _GetType_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(D2D1_PROPERTY_TYPE, UINT32) const> _GetType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetPropertyIndex(_In_ PCWSTR name) const;
    NullPtr<void (D2D1PropertiesHook::*)(_In_ PCWSTR &) const> _GetPropertyIndex_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32, _In_ PCWSTR) const> _GetPropertyIndex_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetValueByName(_In_ PCWSTR name, D2D1_PROPERTY_TYPE type, _In_reads_(dataSize) CONST BYTE * data, UINT32 dataSize);
    NullPtr<void (D2D1PropertiesHook::*)(_In_ PCWSTR &, D2D1_PROPERTY_TYPE &, _In_reads_(dataSize) CONST BYTE * &, UINT32 &)> _SetValueByName_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(HRESULT, _In_ PCWSTR, D2D1_PROPERTY_TYPE, _In_reads_(dataSize) CONST BYTE *, UINT32)> _SetValueByName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetValue(UINT32 index, D2D1_PROPERTY_TYPE type, _In_reads_(dataSize) CONST BYTE * data, UINT32 dataSize);
    NullPtr<void (D2D1PropertiesHook::*)(UINT32 &, D2D1_PROPERTY_TYPE &, _In_reads_(dataSize) CONST BYTE * &, UINT32 &)> _SetValue_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(HRESULT, UINT32, D2D1_PROPERTY_TYPE, _In_reads_(dataSize) CONST BYTE *, UINT32)> _SetValue_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetValueByName(_In_ PCWSTR name, D2D1_PROPERTY_TYPE type, _Out_writes_(dataSize) BYTE * data, UINT32 dataSize) const;
    NullPtr<void (D2D1PropertiesHook::*)(_In_ PCWSTR &, D2D1_PROPERTY_TYPE &, _Out_writes_(dataSize) BYTE * &, UINT32 &) const> _GetValueByName_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(HRESULT, _In_ PCWSTR, D2D1_PROPERTY_TYPE, _Out_writes_(dataSize) BYTE *, UINT32) const> _GetValueByName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetValue(UINT32 index, D2D1_PROPERTY_TYPE type, _Out_writes_(dataSize) BYTE * data, UINT32 dataSize) const;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32 &, D2D1_PROPERTY_TYPE &, _Out_writes_(dataSize) BYTE * &, UINT32 &) const> _GetValue_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(HRESULT, UINT32, D2D1_PROPERTY_TYPE, _Out_writes_(dataSize) BYTE *, UINT32) const> _GetValue_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetValueSize(UINT32 index) const;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32 &) const> _GetValueSize_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32, UINT32) const> _GetValueSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSubProperties(UINT32 index, _Outptr_opt_ ID2D1Properties ** subProperties) const;
    NullPtr<void (D2D1PropertiesHook::*)(UINT32 &, _Outptr_opt_ ID2D1Properties ** &) const> _GetSubProperties_pre_ptr;
    NullPtr<void (D2D1PropertiesHook::*)(HRESULT, UINT32, _Outptr_opt_ ID2D1Properties **) const> _GetSubProperties_post_ptr;

};

// ==============================================================================
// ID2D1Effect
// ==============================================================================
class D2D1EffectHook : public HookBase<ID2D1Effect>
{
    D2D1PropertiesHook & _D2D1Properties;

protected:

    D2D1EffectHook(UnknownBase & unknown, D2D1PropertiesHook & D2D1Properties, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Properties(D2D1Properties)
    {
    }

    ~D2D1EffectHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1PropertiesHook * D2D1Properties = (D2D1PropertiesHook *)unknown.GetHookedObj(__uuidof(ID2D1Properties));
        if (nullptr == D2D1Properties) return nullptr;

        try
        {
            return new D2D1EffectHook(unknown, *D2D1Properties, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1EffectHook * typedPtr = (D2D1EffectHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetPropertyCount() const
    {
        return _D2D1Properties.GetPropertyCount();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPropertyName(UINT32 index, _Out_writes_(nameCount) PWSTR name, UINT32 nameCount) const
    {
        return _D2D1Properties.GetPropertyName(index, name, nameCount);
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetPropertyNameLength(UINT32 index) const
    {
        return _D2D1Properties.GetPropertyNameLength(index);
    }

    // -----------------------------------------------------------------------------
    D2D1_PROPERTY_TYPE STDMETHODCALLTYPE GetType(UINT32 index) const
    {
        return _D2D1Properties.GetType(index);
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetPropertyIndex(_In_ PCWSTR name) const
    {
        return _D2D1Properties.GetPropertyIndex(name);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetValueByName(_In_ PCWSTR name, D2D1_PROPERTY_TYPE type, _In_reads_(dataSize) CONST BYTE * data, UINT32 dataSize)
    {
        return _D2D1Properties.SetValueByName(name, type, data, dataSize);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetValue(UINT32 index, D2D1_PROPERTY_TYPE type, _In_reads_(dataSize) CONST BYTE * data, UINT32 dataSize)
    {
        return _D2D1Properties.SetValue(index, type, data, dataSize);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetValueByName(_In_ PCWSTR name, D2D1_PROPERTY_TYPE type, _Out_writes_(dataSize) BYTE * data, UINT32 dataSize) const
    {
        return _D2D1Properties.GetValueByName(name, type, data, dataSize);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetValue(UINT32 index, D2D1_PROPERTY_TYPE type, _Out_writes_(dataSize) BYTE * data, UINT32 dataSize) const
    {
        return _D2D1Properties.GetValue(index, type, data, dataSize);
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetValueSize(UINT32 index) const
    {
        return _D2D1Properties.GetValueSize(index);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetSubProperties(UINT32 index, _Outptr_opt_ ID2D1Properties ** subProperties) const
    {
        return _D2D1Properties.GetSubProperties(index, subProperties);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetInput(UINT32 index, _In_opt_ ID2D1Image * input, BOOL invalidate = TRUE);
    NullPtr<void (D2D1EffectHook::*)(UINT32 &, _In_opt_ ID2D1Image * &, BOOL &)> _SetInput_pre_ptr;
    NullPtr<void (D2D1EffectHook::*)(UINT32, _In_opt_ ID2D1Image *, BOOL)> _SetInput_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetInputCount(UINT32 inputCount);
    NullPtr<void (D2D1EffectHook::*)(UINT32 &)> _SetInputCount_pre_ptr;
    NullPtr<void (D2D1EffectHook::*)(HRESULT, UINT32)> _SetInputCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetInput(UINT32 index, _Outptr_opt_ ID2D1Image ** input) const;
    NullPtr<void (D2D1EffectHook::*)(UINT32 &, _Outptr_opt_ ID2D1Image ** &) const> _GetInput_pre_ptr;
    NullPtr<void (D2D1EffectHook::*)(UINT32, _Outptr_opt_ ID2D1Image **) const> _GetInput_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetInputCount() const;
    NullPtr<void (D2D1EffectHook::*)() const> _GetInputCount_pre_ptr;
    NullPtr<void (D2D1EffectHook::*)(UINT32) const> _GetInputCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetOutput(_Outptr_ ID2D1Image ** outputImage) const;
    NullPtr<void (D2D1EffectHook::*)(_Outptr_ ID2D1Image ** &) const> _GetOutput_pre_ptr;
    NullPtr<void (D2D1EffectHook::*)(_Outptr_ ID2D1Image **) const> _GetOutput_post_ptr;

};

// ==============================================================================
// ID2D1Bitmap1
// ==============================================================================
class D2D1Bitmap1Hook : public HookBase<ID2D1Bitmap1>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1ImageHook & _D2D1Image;
    D2D1BitmapHook & _D2D1Bitmap;

protected:

    D2D1Bitmap1Hook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1ImageHook & D2D1Image, D2D1BitmapHook & D2D1Bitmap, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1Image(D2D1Image)
        , _D2D1Bitmap(D2D1Bitmap)
    {
    }

    ~D2D1Bitmap1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1ImageHook * D2D1Image = (D2D1ImageHook *)unknown.GetHookedObj(__uuidof(ID2D1Image));
        if (nullptr == D2D1Image) return nullptr;

        D2D1BitmapHook * D2D1Bitmap = (D2D1BitmapHook *)unknown.GetHookedObj(__uuidof(ID2D1Bitmap));
        if (nullptr == D2D1Bitmap) return nullptr;

        try
        {
            return new D2D1Bitmap1Hook(unknown, *D2D1Resource, *D2D1Image, *D2D1Bitmap, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1Bitmap1Hook * typedPtr = (D2D1Bitmap1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_F STDMETHODCALLTYPE GetSize() const
    {
        return _D2D1Bitmap.GetSize();
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_U STDMETHODCALLTYPE GetPixelSize() const
    {
        return _D2D1Bitmap.GetPixelSize();
    }

    // -----------------------------------------------------------------------------
    D2D1_PIXEL_FORMAT STDMETHODCALLTYPE GetPixelFormat() const
    {
        return _D2D1Bitmap.GetPixelFormat();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY) const
    {
        return _D2D1Bitmap.GetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CopyFromBitmap(_In_opt_ CONST D2D1_POINT_2U * destPoint, _In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_U * srcRect)
    {
        return _D2D1Bitmap.CopyFromBitmap(destPoint, bitmap, srcRect);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CopyFromRenderTarget(_In_opt_ CONST D2D1_POINT_2U * destPoint, _In_ ID2D1RenderTarget * renderTarget, _In_opt_ CONST D2D1_RECT_U * srcRect)
    {
        return _D2D1Bitmap.CopyFromRenderTarget(destPoint, renderTarget, srcRect);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CopyFromMemory(_In_opt_ CONST D2D1_RECT_U * dstRect, _In_ CONST void * srcData, UINT32 pitch)
    {
        return _D2D1Bitmap.CopyFromMemory(dstRect, srcData, pitch);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetColorContext(_Outptr_result_maybenull_ ID2D1ColorContext ** colorContext) const;
    NullPtr<void (D2D1Bitmap1Hook::*)(_Outptr_result_maybenull_ ID2D1ColorContext ** &) const> _GetColorContext_pre_ptr;
    NullPtr<void (D2D1Bitmap1Hook::*)(_Outptr_result_maybenull_ ID2D1ColorContext **) const> _GetColorContext_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_BITMAP_OPTIONS STDMETHODCALLTYPE GetOptions() const;
    NullPtr<void (D2D1Bitmap1Hook::*)() const> _GetOptions_pre_ptr;
    NullPtr<void (D2D1Bitmap1Hook::*)(D2D1_BITMAP_OPTIONS) const> _GetOptions_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSurface(_Outptr_result_maybenull_ IDXGISurface ** dxgiSurface) const;
    NullPtr<void (D2D1Bitmap1Hook::*)(_Outptr_result_maybenull_ IDXGISurface ** &) const> _GetSurface_pre_ptr;
    NullPtr<void (D2D1Bitmap1Hook::*)(HRESULT, _Outptr_result_maybenull_ IDXGISurface **) const> _GetSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Map(D2D1_MAP_OPTIONS options, _Out_ D2D1_MAPPED_RECT * mappedRect);
    NullPtr<void (D2D1Bitmap1Hook::*)(D2D1_MAP_OPTIONS &, _Out_ D2D1_MAPPED_RECT * &)> _Map_pre_ptr;
    NullPtr<void (D2D1Bitmap1Hook::*)(HRESULT, D2D1_MAP_OPTIONS, _Out_ D2D1_MAPPED_RECT *)> _Map_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Unmap();
    NullPtr<void (D2D1Bitmap1Hook::*)()> _Unmap_pre_ptr;
    NullPtr<void (D2D1Bitmap1Hook::*)(HRESULT)> _Unmap_post_ptr;

};

// ==============================================================================
// ID2D1ColorContext
// ==============================================================================
class D2D1ColorContextHook : public HookBase<ID2D1ColorContext>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1ColorContextHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1ColorContextHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1ColorContextHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ColorContextHook * typedPtr = (D2D1ColorContextHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual D2D1_COLOR_SPACE STDMETHODCALLTYPE GetColorSpace() const;
    NullPtr<void (D2D1ColorContextHook::*)() const> _GetColorSpace_pre_ptr;
    NullPtr<void (D2D1ColorContextHook::*)(D2D1_COLOR_SPACE) const> _GetColorSpace_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetProfileSize() const;
    NullPtr<void (D2D1ColorContextHook::*)() const> _GetProfileSize_pre_ptr;
    NullPtr<void (D2D1ColorContextHook::*)(UINT32) const> _GetProfileSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetProfile(_Out_writes_(profileSize) BYTE * profile, UINT32 profileSize) const;
    NullPtr<void (D2D1ColorContextHook::*)(_Out_writes_(profileSize) BYTE * &, UINT32 &) const> _GetProfile_pre_ptr;
    NullPtr<void (D2D1ColorContextHook::*)(HRESULT, _Out_writes_(profileSize) BYTE *, UINT32) const> _GetProfile_post_ptr;

};

// ==============================================================================
// ID2D1GradientStopCollection1
// ==============================================================================
class D2D1GradientStopCollection1Hook : public HookBase<ID2D1GradientStopCollection1>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1GradientStopCollectionHook & _D2D1GradientStopCollection;

protected:

    D2D1GradientStopCollection1Hook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1GradientStopCollectionHook & D2D1GradientStopCollection, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1GradientStopCollection(D2D1GradientStopCollection)
    {
    }

    ~D2D1GradientStopCollection1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1GradientStopCollectionHook * D2D1GradientStopCollection = (D2D1GradientStopCollectionHook *)unknown.GetHookedObj(__uuidof(ID2D1GradientStopCollection));
        if (nullptr == D2D1GradientStopCollection) return nullptr;

        try
        {
            return new D2D1GradientStopCollection1Hook(unknown, *D2D1Resource, *D2D1GradientStopCollection, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1GradientStopCollection1Hook * typedPtr = (D2D1GradientStopCollection1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetGradientStopCount() const
    {
        return _D2D1GradientStopCollection.GetGradientStopCount();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetGradientStops(_Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP * gradientStops, UINT32 gradientStopsCount) const
    {
        return _D2D1GradientStopCollection.GetGradientStops(gradientStops, gradientStopsCount);
    }

    // -----------------------------------------------------------------------------
    D2D1_GAMMA STDMETHODCALLTYPE GetColorInterpolationGamma() const
    {
        return _D2D1GradientStopCollection.GetColorInterpolationGamma();
    }

    // -----------------------------------------------------------------------------
    D2D1_EXTEND_MODE STDMETHODCALLTYPE GetExtendMode() const
    {
        return _D2D1GradientStopCollection.GetExtendMode();
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetGradientStops1(_Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP * gradientStops, UINT32 gradientStopsCount) const;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)(_Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP * &, UINT32 &) const> _GetGradientStops1_pre_ptr;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)(_Out_writes_to_(gradientStopsCount, _Inexpressible_("Retrieved through GetGradientStopCount") ) D2D1_GRADIENT_STOP *, UINT32) const> _GetGradientStops1_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_COLOR_SPACE STDMETHODCALLTYPE GetPreInterpolationSpace() const;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)() const> _GetPreInterpolationSpace_pre_ptr;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)(D2D1_COLOR_SPACE) const> _GetPreInterpolationSpace_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_COLOR_SPACE STDMETHODCALLTYPE GetPostInterpolationSpace() const;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)() const> _GetPostInterpolationSpace_pre_ptr;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)(D2D1_COLOR_SPACE) const> _GetPostInterpolationSpace_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_BUFFER_PRECISION STDMETHODCALLTYPE GetBufferPrecision() const;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)() const> _GetBufferPrecision_pre_ptr;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)(D2D1_BUFFER_PRECISION) const> _GetBufferPrecision_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_COLOR_INTERPOLATION_MODE STDMETHODCALLTYPE GetColorInterpolationMode() const;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)() const> _GetColorInterpolationMode_pre_ptr;
    NullPtr<void (D2D1GradientStopCollection1Hook::*)(D2D1_COLOR_INTERPOLATION_MODE) const> _GetColorInterpolationMode_post_ptr;

};

// ==============================================================================
// ID2D1DrawingStateBlock1
// ==============================================================================
class D2D1DrawingStateBlock1Hook : public HookBase<ID2D1DrawingStateBlock1>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1DrawingStateBlockHook & _D2D1DrawingStateBlock;

protected:

    D2D1DrawingStateBlock1Hook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1DrawingStateBlockHook & D2D1DrawingStateBlock, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1DrawingStateBlock(D2D1DrawingStateBlock)
    {
    }

    ~D2D1DrawingStateBlock1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1DrawingStateBlockHook * D2D1DrawingStateBlock = (D2D1DrawingStateBlockHook *)unknown.GetHookedObj(__uuidof(ID2D1DrawingStateBlock));
        if (nullptr == D2D1DrawingStateBlock) return nullptr;

        try
        {
            return new D2D1DrawingStateBlock1Hook(unknown, *D2D1Resource, *D2D1DrawingStateBlock, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1DrawingStateBlock1Hook * typedPtr = (D2D1DrawingStateBlock1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDescription(_Out_ D2D1_DRAWING_STATE_DESCRIPTION * stateDescription) const
    {
        return _D2D1DrawingStateBlock.GetDescription(stateDescription);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetDescription(_In_ CONST D2D1_DRAWING_STATE_DESCRIPTION * stateDescription)
    {
        return _D2D1DrawingStateBlock.SetDescription(stateDescription);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextRenderingParams(_In_opt_ IDWriteRenderingParams * textRenderingParams = NULL)
    {
        return _D2D1DrawingStateBlock.SetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTextRenderingParams(_Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const
    {
        return _D2D1DrawingStateBlock.GetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDescription(_Out_ D2D1_DRAWING_STATE_DESCRIPTION1 * stateDescription) const;
    NullPtr<void (D2D1DrawingStateBlock1Hook::*)(_Out_ D2D1_DRAWING_STATE_DESCRIPTION1 * &) const> _GetDescription_pre_ptr;
    NullPtr<void (D2D1DrawingStateBlock1Hook::*)(_Out_ D2D1_DRAWING_STATE_DESCRIPTION1 *) const> _GetDescription_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetDescription(_In_ CONST D2D1_DRAWING_STATE_DESCRIPTION1 * stateDescription);
    NullPtr<void (D2D1DrawingStateBlock1Hook::*)(_In_ CONST D2D1_DRAWING_STATE_DESCRIPTION1 * &)> _SetDescription_pre_ptr;
    NullPtr<void (D2D1DrawingStateBlock1Hook::*)(_In_ CONST D2D1_DRAWING_STATE_DESCRIPTION1 *)> _SetDescription_post_ptr;

};

// ==============================================================================
// ID2D1DeviceContext
// ==============================================================================
class D2D1DeviceContextHook : public HookBase<ID2D1DeviceContext>
{
    D2D1ResourceHook & _D2D1Resource;
    D2D1RenderTargetHook & _D2D1RenderTarget;

protected:

    D2D1DeviceContextHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, D2D1RenderTargetHook & D2D1RenderTarget, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
        , _D2D1RenderTarget(D2D1RenderTarget)
    {
    }

    ~D2D1DeviceContextHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        D2D1RenderTargetHook * D2D1RenderTarget = (D2D1RenderTargetHook *)unknown.GetHookedObj(__uuidof(ID2D1RenderTarget));
        if (nullptr == D2D1RenderTarget) return nullptr;

        try
        {
            return new D2D1DeviceContextHook(unknown, *D2D1Resource, *D2D1RenderTarget, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1DeviceContextHook * typedPtr = (D2D1DeviceContextHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmap(D2D1_SIZE_U size, _In_opt_ CONST void * srcData, UINT32 pitch, _In_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateBitmap(size, srcData, pitch, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmapFromWicBitmap(_In_ IWICBitmapSource * wicBitmapSource, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSharedBitmap(_In_ REFIID riid, _Inout_ void * data, _In_opt_ CONST D2D1_BITMAP_PROPERTIES * bitmapProperties, _Outptr_ ID2D1Bitmap ** bitmap)
    {
        return _D2D1RenderTarget.CreateSharedBitmap(riid, data, bitmapProperties, bitmap);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateBitmapBrush(_In_opt_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES * bitmapBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1BitmapBrush ** bitmapBrush)
    {
        return _D2D1RenderTarget.CreateBitmapBrush(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateSolidColorBrush(_In_ CONST D2D1_COLOR_F * color, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1SolidColorBrush ** solidColorBrush)
    {
        return _D2D1RenderTarget.CreateSolidColorBrush(color, brushProperties, solidColorBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateGradientStopCollection(_In_reads_(gradientStopsCount) CONST D2D1_GRADIENT_STOP * gradientStops, _In_range_(>=,1) UINT32 gradientStopsCount, D2D1_GAMMA colorInterpolationGamma, D2D1_EXTEND_MODE extendMode, _Outptr_ ID2D1GradientStopCollection ** gradientStopCollection)
    {
        return _D2D1RenderTarget.CreateGradientStopCollection(gradientStops, gradientStopsCount, colorInterpolationGamma, extendMode, gradientStopCollection);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateLinearGradientBrush(_In_ CONST D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * linearGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1LinearGradientBrush ** linearGradientBrush)
    {
        return _D2D1RenderTarget.CreateLinearGradientBrush(linearGradientBrushProperties, brushProperties, gradientStopCollection, linearGradientBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRadialGradientBrush(_In_ CONST D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * radialGradientBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _In_ ID2D1GradientStopCollection * gradientStopCollection, _Outptr_ ID2D1RadialGradientBrush ** radialGradientBrush)
    {
        return _D2D1RenderTarget.CreateRadialGradientBrush(radialGradientBrushProperties, brushProperties, gradientStopCollection, radialGradientBrush);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateCompatibleRenderTarget(_In_opt_ CONST D2D1_SIZE_F * desiredSize, _In_opt_ CONST D2D1_SIZE_U * desiredPixelSize, _In_opt_ CONST D2D1_PIXEL_FORMAT * desiredFormat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options, _Outptr_ ID2D1BitmapRenderTarget ** bitmapRenderTarget)
    {
        return _D2D1RenderTarget.CreateCompatibleRenderTarget(desiredSize, desiredPixelSize, desiredFormat, options, bitmapRenderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateLayer(_In_opt_ CONST D2D1_SIZE_F * size, _Outptr_ ID2D1Layer ** layer)
    {
        return _D2D1RenderTarget.CreateLayer(size, layer);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateMesh(_Outptr_ ID2D1Mesh ** mesh)
    {
        return _D2D1RenderTarget.CreateMesh(mesh);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawLine(D2D1_POINT_2F point0, D2D1_POINT_2F point1, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawLine(point0, point1, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawRectangle(rect, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillRectangle(_In_ CONST D2D1_RECT_F * rect, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillRectangle(rect, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawRoundedRectangle(roundedRect, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillRoundedRectangle(_In_ CONST D2D1_ROUNDED_RECT * roundedRect, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillRoundedRectangle(roundedRect, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawEllipse(ellipse, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillEllipse(_In_ CONST D2D1_ELLIPSE * ellipse, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillEllipse(ellipse, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, FLOAT strokeWidth = 1.0f, _In_opt_ ID2D1StrokeStyle * strokeStyle = NULL)
    {
        return _D2D1RenderTarget.DrawGeometry(geometry, brush, strokeWidth, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillGeometry(_In_ ID2D1Geometry * geometry, _In_ ID2D1Brush * brush, _In_opt_ ID2D1Brush * opacityBrush = NULL)
    {
        return _D2D1RenderTarget.FillGeometry(geometry, brush, opacityBrush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillMesh(_In_ ID2D1Mesh * mesh, _In_ ID2D1Brush * brush)
    {
        return _D2D1RenderTarget.FillMesh(mesh, brush);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE FillOpacityMask(_In_ ID2D1Bitmap * opacityMask, _In_ ID2D1Brush * brush, D2D1_OPACITY_MASK_CONTENT content, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL)
    {
        return _D2D1RenderTarget.FillOpacityMask(opacityMask, brush, content, destinationRectangle, sourceRectangle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawBitmap(_In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, FLOAT opacity = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL)
    {
        return _D2D1RenderTarget.DrawBitmap(bitmap, destinationRectangle, opacity, interpolationMode, sourceRectangle);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawText(_In_reads_(stringLength) CONST WCHAR * string, UINT32 stringLength, _In_ IDWriteTextFormat * textFormat, _In_ CONST D2D1_RECT_F * layoutRect, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL)
    {
        return _D2D1RenderTarget.DrawText(string, stringLength, textFormat, layoutRect, defaultForegroundBrush, options, measuringMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawTextLayout(D2D1_POINT_2F origin, _In_ IDWriteTextLayout * textLayout, _In_ ID2D1Brush * defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE)
    {
        return _D2D1RenderTarget.DrawTextLayout(origin, textLayout, defaultForegroundBrush, options);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE DrawGlyphRun(D2D1_POINT_2F baselineOrigin, _In_ CONST DWRITE_GLYPH_RUN * glyphRun, _In_ ID2D1Brush * foregroundBrush, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL)
    {
        return _D2D1RenderTarget.DrawGlyphRun(baselineOrigin, glyphRun, foregroundBrush, measuringMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTransform(_In_ CONST D2D1_MATRIX_3X2_F * transform)
    {
        return _D2D1RenderTarget.SetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTransform(_Out_ D2D1_MATRIX_3X2_F * transform) const
    {
        return _D2D1RenderTarget.GetTransform(transform);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode)
    {
        return _D2D1RenderTarget.SetAntialiasMode(antialiasMode);
    }

    // -----------------------------------------------------------------------------
    D2D1_ANTIALIAS_MODE STDMETHODCALLTYPE GetAntialiasMode() const
    {
        return _D2D1RenderTarget.GetAntialiasMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode)
    {
        return _D2D1RenderTarget.SetTextAntialiasMode(textAntialiasMode);
    }

    // -----------------------------------------------------------------------------
    D2D1_TEXT_ANTIALIAS_MODE STDMETHODCALLTYPE GetTextAntialiasMode() const
    {
        return _D2D1RenderTarget.GetTextAntialiasMode();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTextRenderingParams(_In_opt_ IDWriteRenderingParams * textRenderingParams = NULL)
    {
        return _D2D1RenderTarget.SetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTextRenderingParams(_Outptr_result_maybenull_ IDWriteRenderingParams ** textRenderingParams) const
    {
        return _D2D1RenderTarget.GetTextRenderingParams(textRenderingParams);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetTags(D2D1_TAG tag1, D2D1_TAG tag2)
    {
        return _D2D1RenderTarget.SetTags(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetTags(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL) const
    {
        return _D2D1RenderTarget.GetTags(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PushLayer(_In_ CONST D2D1_LAYER_PARAMETERS * layerParameters, _In_opt_ ID2D1Layer * layer)
    {
        return _D2D1RenderTarget.PushLayer(layerParameters, layer);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PopLayer()
    {
        return _D2D1RenderTarget.PopLayer();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE Flush(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL)
    {
        return _D2D1RenderTarget.Flush(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SaveDrawingState(_Inout_ ID2D1DrawingStateBlock * drawingStateBlock) const
    {
        return _D2D1RenderTarget.SaveDrawingState(drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE RestoreDrawingState(_In_ ID2D1DrawingStateBlock * drawingStateBlock)
    {
        return _D2D1RenderTarget.RestoreDrawingState(drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PushAxisAlignedClip(_In_ CONST D2D1_RECT_F * clipRect, D2D1_ANTIALIAS_MODE antialiasMode)
    {
        return _D2D1RenderTarget.PushAxisAlignedClip(clipRect, antialiasMode);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE PopAxisAlignedClip()
    {
        return _D2D1RenderTarget.PopAxisAlignedClip();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE Clear(_In_opt_ CONST D2D1_COLOR_F * clearColor = NULL)
    {
        return _D2D1RenderTarget.Clear(clearColor);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE BeginDraw()
    {
        return _D2D1RenderTarget.BeginDraw();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE EndDraw(_Out_opt_ D2D1_TAG * tag1 = NULL, _Out_opt_ D2D1_TAG * tag2 = NULL)
    {
        return _D2D1RenderTarget.EndDraw(tag1, tag2);
    }

    // -----------------------------------------------------------------------------
    D2D1_PIXEL_FORMAT STDMETHODCALLTYPE GetPixelFormat() const
    {
        return _D2D1RenderTarget.GetPixelFormat();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE SetDpi(FLOAT dpiX, FLOAT dpiY)
    {
        return _D2D1RenderTarget.SetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY) const
    {
        return _D2D1RenderTarget.GetDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_F STDMETHODCALLTYPE GetSize() const
    {
        return _D2D1RenderTarget.GetSize();
    }

    // -----------------------------------------------------------------------------
    D2D1_SIZE_U STDMETHODCALLTYPE GetPixelSize() const
    {
        return _D2D1RenderTarget.GetPixelSize();
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetMaximumBitmapSize() const
    {
        return _D2D1RenderTarget.GetMaximumBitmapSize();
    }

    // -----------------------------------------------------------------------------
    BOOL STDMETHODCALLTYPE IsSupported(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties) const
    {
        return _D2D1RenderTarget.IsSupported(renderTargetProperties);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBitmap(D2D1_SIZE_U size, _In_opt_ CONST void * sourceData, UINT32 pitch, _In_ CONST D2D1_BITMAP_PROPERTIES1 * bitmapProperties, _Outptr_ ID2D1Bitmap1 ** bitmap);
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_SIZE_U &, _In_opt_ CONST void * &, UINT32 &, _In_ CONST D2D1_BITMAP_PROPERTIES1 * &, _Outptr_ ID2D1Bitmap1 ** &)> _CreateBitmap_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, D2D1_SIZE_U, _In_opt_ CONST void *, UINT32, _In_ CONST D2D1_BITMAP_PROPERTIES1 *, _Outptr_ ID2D1Bitmap1 **)> _CreateBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBitmapFromWicBitmap(_In_ IWICBitmapSource * wicBitmapSource, _In_opt_ CONST D2D1_BITMAP_PROPERTIES1 * bitmapProperties, _Outptr_ ID2D1Bitmap1 ** bitmap);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ IWICBitmapSource * &, _In_opt_ CONST D2D1_BITMAP_PROPERTIES1 * &, _Outptr_ ID2D1Bitmap1 ** &)> _CreateBitmapFromWicBitmap_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ IWICBitmapSource *, _In_opt_ CONST D2D1_BITMAP_PROPERTIES1 *, _Outptr_ ID2D1Bitmap1 **)> _CreateBitmapFromWicBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateColorContext(D2D1_COLOR_SPACE space, _In_reads_opt_(profileSize) CONST BYTE * profile, UINT32 profileSize, _Outptr_ ID2D1ColorContext ** colorContext);
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_COLOR_SPACE &, _In_reads_opt_(profileSize) CONST BYTE * &, UINT32 &, _Outptr_ ID2D1ColorContext ** &)> _CreateColorContext_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, D2D1_COLOR_SPACE, _In_reads_opt_(profileSize) CONST BYTE *, UINT32, _Outptr_ ID2D1ColorContext **)> _CreateColorContext_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateColorContextFromFilename(_In_ PCWSTR filename, _Outptr_ ID2D1ColorContext ** colorContext);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ PCWSTR &, _Outptr_ ID2D1ColorContext ** &)> _CreateColorContextFromFilename_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ PCWSTR, _Outptr_ ID2D1ColorContext **)> _CreateColorContextFromFilename_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateColorContextFromWicColorContext(_In_ IWICColorContext * wicColorContext, _Outptr_ ID2D1ColorContext ** colorContext);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ IWICColorContext * &, _Outptr_ ID2D1ColorContext ** &)> _CreateColorContextFromWicColorContext_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ IWICColorContext *, _Outptr_ ID2D1ColorContext **)> _CreateColorContextFromWicColorContext_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBitmapFromDxgiSurface(_In_ IDXGISurface * surface, _In_opt_ CONST D2D1_BITMAP_PROPERTIES1 * bitmapProperties, _Outptr_ ID2D1Bitmap1 ** bitmap);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ IDXGISurface * &, _In_opt_ CONST D2D1_BITMAP_PROPERTIES1 * &, _Outptr_ ID2D1Bitmap1 ** &)> _CreateBitmapFromDxgiSurface_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ IDXGISurface *, _In_opt_ CONST D2D1_BITMAP_PROPERTIES1 *, _Outptr_ ID2D1Bitmap1 **)> _CreateBitmapFromDxgiSurface_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateEffect(_In_ REFCLSID effectId, _Outptr_ ID2D1Effect ** effect);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ REFCLSID, _Outptr_ ID2D1Effect ** &)> _CreateEffect_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ REFCLSID, _Outptr_ ID2D1Effect **)> _CreateEffect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateGradientStopCollection(_In_reads_(straightAlphaGradientStopsCount) CONST D2D1_GRADIENT_STOP * straightAlphaGradientStops, _In_range_(>=,1) UINT32 straightAlphaGradientStopsCount, D2D1_COLOR_SPACE preInterpolationSpace, D2D1_COLOR_SPACE postInterpolationSpace, D2D1_BUFFER_PRECISION bufferPrecision, D2D1_EXTEND_MODE extendMode, D2D1_COLOR_INTERPOLATION_MODE colorInterpolationMode, _Outptr_ ID2D1GradientStopCollection1 ** gradientStopCollection1);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_reads_(straightAlphaGradientStopsCount) CONST D2D1_GRADIENT_STOP * &, _In_range_(>=,1) UINT32 &, D2D1_COLOR_SPACE &, D2D1_COLOR_SPACE &, D2D1_BUFFER_PRECISION &, D2D1_EXTEND_MODE &, D2D1_COLOR_INTERPOLATION_MODE &, _Outptr_ ID2D1GradientStopCollection1 ** &)> _CreateGradientStopCollection_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_reads_(straightAlphaGradientStopsCount) CONST D2D1_GRADIENT_STOP *, _In_range_(>=,1) UINT32, D2D1_COLOR_SPACE, D2D1_COLOR_SPACE, D2D1_BUFFER_PRECISION, D2D1_EXTEND_MODE, D2D1_COLOR_INTERPOLATION_MODE, _Outptr_ ID2D1GradientStopCollection1 **)> _CreateGradientStopCollection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateImageBrush(_In_opt_ ID2D1Image * image, _In_ CONST D2D1_IMAGE_BRUSH_PROPERTIES * imageBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1ImageBrush ** imageBrush);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_opt_ ID2D1Image * &, _In_ CONST D2D1_IMAGE_BRUSH_PROPERTIES * &, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * &, _Outptr_ ID2D1ImageBrush ** &)> _CreateImageBrush_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_opt_ ID2D1Image *, _In_ CONST D2D1_IMAGE_BRUSH_PROPERTIES *, _In_opt_ CONST D2D1_BRUSH_PROPERTIES *, _Outptr_ ID2D1ImageBrush **)> _CreateImageBrush_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBitmapBrush(_In_opt_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES1 * bitmapBrushProperties, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * brushProperties, _Outptr_ ID2D1BitmapBrush1 ** bitmapBrush);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_opt_ ID2D1Bitmap * &, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES1 * &, _In_opt_ CONST D2D1_BRUSH_PROPERTIES * &, _Outptr_ ID2D1BitmapBrush1 ** &)> _CreateBitmapBrush_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_opt_ ID2D1Bitmap *, _In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES1 *, _In_opt_ CONST D2D1_BRUSH_PROPERTIES *, _Outptr_ ID2D1BitmapBrush1 **)> _CreateBitmapBrush_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCommandList(_Outptr_ ID2D1CommandList ** commandList);
    NullPtr<void (D2D1DeviceContextHook::*)(_Outptr_ ID2D1CommandList ** &)> _CreateCommandList_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _Outptr_ ID2D1CommandList **)> _CreateCommandList_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsDxgiFormatSupported(DXGI_FORMAT format) const;
    NullPtr<void (D2D1DeviceContextHook::*)(DXGI_FORMAT &) const> _IsDxgiFormatSupported_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(BOOL, DXGI_FORMAT) const> _IsDxgiFormatSupported_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsBufferPrecisionSupported(D2D1_BUFFER_PRECISION bufferPrecision) const;
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_BUFFER_PRECISION &) const> _IsBufferPrecisionSupported_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(BOOL, D2D1_BUFFER_PRECISION) const> _IsBufferPrecisionSupported_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetImageLocalBounds(_In_ ID2D1Image * image, _Out_ D2D1_RECT_F * localBounds) const;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Image * &, _Out_ D2D1_RECT_F * &) const> _GetImageLocalBounds_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ ID2D1Image *, _Out_ D2D1_RECT_F *) const> _GetImageLocalBounds_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetImageWorldBounds(_In_ ID2D1Image * image, _Out_ D2D1_RECT_F * worldBounds) const;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Image * &, _Out_ D2D1_RECT_F * &) const> _GetImageWorldBounds_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ ID2D1Image *, _Out_ D2D1_RECT_F *) const> _GetImageWorldBounds_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGlyphRunWorldBounds(D2D1_POINT_2F baselineOrigin, _In_ CONST DWRITE_GLYPH_RUN * glyphRun, DWRITE_MEASURING_MODE measuringMode, _Out_ D2D1_RECT_F * bounds) const;
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_POINT_2F &, _In_ CONST DWRITE_GLYPH_RUN * &, DWRITE_MEASURING_MODE &, _Out_ D2D1_RECT_F * &) const> _GetGlyphRunWorldBounds_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, D2D1_POINT_2F, _In_ CONST DWRITE_GLYPH_RUN *, DWRITE_MEASURING_MODE, _Out_ D2D1_RECT_F *) const> _GetGlyphRunWorldBounds_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetDevice(_Outptr_ ID2D1Device ** device) const;
    NullPtr<void (D2D1DeviceContextHook::*)(_Outptr_ ID2D1Device ** &) const> _GetDevice_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_Outptr_ ID2D1Device **) const> _GetDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetTarget(_In_opt_ ID2D1Image * image);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_opt_ ID2D1Image * &)> _SetTarget_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_opt_ ID2D1Image *)> _SetTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetTarget(_Outptr_result_maybenull_ ID2D1Image ** image) const;
    NullPtr<void (D2D1DeviceContextHook::*)(_Outptr_result_maybenull_ ID2D1Image ** &) const> _GetTarget_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_Outptr_result_maybenull_ ID2D1Image **) const> _GetTarget_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetRenderingControls(_In_ CONST D2D1_RENDERING_CONTROLS * renderingControls);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ CONST D2D1_RENDERING_CONTROLS * &)> _SetRenderingControls_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ CONST D2D1_RENDERING_CONTROLS *)> _SetRenderingControls_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetRenderingControls(_Out_ D2D1_RENDERING_CONTROLS * renderingControls) const;
    NullPtr<void (D2D1DeviceContextHook::*)(_Out_ D2D1_RENDERING_CONTROLS * &) const> _GetRenderingControls_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_Out_ D2D1_RENDERING_CONTROLS *) const> _GetRenderingControls_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND primitiveBlend);
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_PRIMITIVE_BLEND &)> _SetPrimitiveBlend_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_PRIMITIVE_BLEND)> _SetPrimitiveBlend_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_PRIMITIVE_BLEND STDMETHODCALLTYPE GetPrimitiveBlend() const;
    NullPtr<void (D2D1DeviceContextHook::*)() const> _GetPrimitiveBlend_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_PRIMITIVE_BLEND) const> _GetPrimitiveBlend_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetUnitMode(D2D1_UNIT_MODE unitMode);
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_UNIT_MODE &)> _SetUnitMode_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_UNIT_MODE)> _SetUnitMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual D2D1_UNIT_MODE STDMETHODCALLTYPE GetUnitMode() const;
    NullPtr<void (D2D1DeviceContextHook::*)() const> _GetUnitMode_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_UNIT_MODE) const> _GetUnitMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawGlyphRun(D2D1_POINT_2F baselineOrigin, _In_ CONST DWRITE_GLYPH_RUN * glyphRun, _In_opt_ CONST DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription, _In_ ID2D1Brush * foregroundBrush, DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL);
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_POINT_2F &, _In_ CONST DWRITE_GLYPH_RUN * &, _In_opt_ CONST DWRITE_GLYPH_RUN_DESCRIPTION * &, _In_ ID2D1Brush * &, DWRITE_MEASURING_MODE &)> _DrawGlyphRun_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(D2D1_POINT_2F, _In_ CONST DWRITE_GLYPH_RUN *, _In_opt_ CONST DWRITE_GLYPH_RUN_DESCRIPTION *, _In_ ID2D1Brush *, DWRITE_MEASURING_MODE)> _DrawGlyphRun_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawImage(_In_ ID2D1Image * image, _In_opt_ CONST D2D1_POINT_2F * targetOffset = NULL, _In_opt_ CONST D2D1_RECT_F * imageRectangle = NULL, D2D1_INTERPOLATION_MODE interpolationMode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE compositeMode = D2D1_COMPOSITE_MODE_SOURCE_OVER);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Image * &, _In_opt_ CONST D2D1_POINT_2F * &, _In_opt_ CONST D2D1_RECT_F * &, D2D1_INTERPOLATION_MODE &, D2D1_COMPOSITE_MODE &)> _DrawImage_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Image *, _In_opt_ CONST D2D1_POINT_2F *, _In_opt_ CONST D2D1_RECT_F *, D2D1_INTERPOLATION_MODE, D2D1_COMPOSITE_MODE)> _DrawImage_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawGdiMetafile(_In_ ID2D1GdiMetafile * gdiMetafile, _In_opt_ CONST D2D1_POINT_2F * targetOffset = NULL);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1GdiMetafile * &, _In_opt_ CONST D2D1_POINT_2F * &)> _DrawGdiMetafile_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1GdiMetafile *, _In_opt_ CONST D2D1_POINT_2F *)> _DrawGdiMetafile_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE DrawBitmap(_In_ ID2D1Bitmap * bitmap, _In_opt_ CONST D2D1_RECT_F * destinationRectangle, FLOAT opacity, D2D1_INTERPOLATION_MODE interpolationMode, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL, _In_opt_ CONST D2D1_MATRIX_4X4_F * perspectiveTransform = NULL);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Bitmap * &, _In_opt_ CONST D2D1_RECT_F * &, FLOAT &, D2D1_INTERPOLATION_MODE &, _In_opt_ CONST D2D1_RECT_F * &, _In_opt_ CONST D2D1_MATRIX_4X4_F * &)> _DrawBitmap_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Bitmap *, _In_opt_ CONST D2D1_RECT_F *, FLOAT, D2D1_INTERPOLATION_MODE, _In_opt_ CONST D2D1_RECT_F *, _In_opt_ CONST D2D1_MATRIX_4X4_F *)> _DrawBitmap_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE PushLayer(_In_ CONST D2D1_LAYER_PARAMETERS1 * layerParameters, _In_opt_ ID2D1Layer * layer);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ CONST D2D1_LAYER_PARAMETERS1 * &, _In_opt_ ID2D1Layer * &)> _PushLayer_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ CONST D2D1_LAYER_PARAMETERS1 *, _In_opt_ ID2D1Layer *)> _PushLayer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE InvalidateEffectInputRectangle(_In_ ID2D1Effect * effect, UINT32 input, _In_ CONST D2D1_RECT_F * inputRectangle);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Effect * &, UINT32 &, _In_ CONST D2D1_RECT_F * &)> _InvalidateEffectInputRectangle_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ ID2D1Effect *, UINT32, _In_ CONST D2D1_RECT_F *)> _InvalidateEffectInputRectangle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetEffectInvalidRectangleCount(_In_ ID2D1Effect * effect, _Out_ UINT32 * rectangleCount);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Effect * &, _Out_ UINT32 * &)> _GetEffectInvalidRectangleCount_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ ID2D1Effect *, _Out_ UINT32 *)> _GetEffectInvalidRectangleCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetEffectInvalidRectangles(_In_ ID2D1Effect * effect, _Out_writes_(rectanglesCount) D2D1_RECT_F * rectangles, UINT32 rectanglesCount);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Effect * &, _Out_writes_(rectanglesCount) D2D1_RECT_F * &, UINT32 &)> _GetEffectInvalidRectangles_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ ID2D1Effect *, _Out_writes_(rectanglesCount) D2D1_RECT_F *, UINT32)> _GetEffectInvalidRectangles_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetEffectRequiredInputRectangles(_In_ ID2D1Effect * renderEffect, _In_opt_ CONST D2D1_RECT_F * renderImageRectangle, _In_reads_(inputCount) CONST D2D1_EFFECT_INPUT_DESCRIPTION * inputDescriptions, _Out_writes_(inputCount) D2D1_RECT_F * requiredInputRects, UINT32 inputCount);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Effect * &, _In_opt_ CONST D2D1_RECT_F * &, _In_reads_(inputCount) CONST D2D1_EFFECT_INPUT_DESCRIPTION * &, _Out_writes_(inputCount) D2D1_RECT_F * &, UINT32 &)> _GetEffectRequiredInputRectangles_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(HRESULT, _In_ ID2D1Effect *, _In_opt_ CONST D2D1_RECT_F *, _In_reads_(inputCount) CONST D2D1_EFFECT_INPUT_DESCRIPTION *, _Out_writes_(inputCount) D2D1_RECT_F *, UINT32)> _GetEffectRequiredInputRectangles_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE FillOpacityMask(_In_ ID2D1Bitmap * opacityMask, _In_ ID2D1Brush * brush, _In_opt_ CONST D2D1_RECT_F * destinationRectangle = NULL, _In_opt_ CONST D2D1_RECT_F * sourceRectangle = NULL);
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Bitmap * &, _In_ ID2D1Brush * &, _In_opt_ CONST D2D1_RECT_F * &, _In_opt_ CONST D2D1_RECT_F * &)> _FillOpacityMask_pre_ptr;
    NullPtr<void (D2D1DeviceContextHook::*)(_In_ ID2D1Bitmap *, _In_ ID2D1Brush *, _In_opt_ CONST D2D1_RECT_F *, _In_opt_ CONST D2D1_RECT_F *)> _FillOpacityMask_post_ptr;

};

// ==============================================================================
// ID2D1Device
// ==============================================================================
class D2D1DeviceHook : public HookBase<ID2D1Device>
{
    D2D1ResourceHook & _D2D1Resource;

protected:

    D2D1DeviceHook(UnknownBase & unknown, D2D1ResourceHook & D2D1Resource, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Resource(D2D1Resource)
    {
    }

    ~D2D1DeviceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1ResourceHook * D2D1Resource = (D2D1ResourceHook *)unknown.GetHookedObj(__uuidof(ID2D1Resource));
        if (nullptr == D2D1Resource) return nullptr;

        try
        {
            return new D2D1DeviceHook(unknown, *D2D1Resource, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1DeviceHook * typedPtr = (D2D1DeviceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetFactory(_Outptr_ ID2D1Factory ** factory) const
    {
        return _D2D1Resource.GetFactory(factory);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS options, _Outptr_ ID2D1DeviceContext ** deviceContext);
    NullPtr<void (D2D1DeviceHook::*)(D2D1_DEVICE_CONTEXT_OPTIONS &, _Outptr_ ID2D1DeviceContext ** &)> _CreateDeviceContext_pre_ptr;
    NullPtr<void (D2D1DeviceHook::*)(HRESULT, D2D1_DEVICE_CONTEXT_OPTIONS, _Outptr_ ID2D1DeviceContext **)> _CreateDeviceContext_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreatePrintControl(_In_ IWICImagingFactory * wicFactory, _In_ IPrintDocumentPackageTarget * documentTarget, _In_opt_ CONST D2D1_PRINT_CONTROL_PROPERTIES * printControlProperties, _Outptr_ ID2D1PrintControl ** printControl);
    NullPtr<void (D2D1DeviceHook::*)(_In_ IWICImagingFactory * &, _In_ IPrintDocumentPackageTarget * &, _In_opt_ CONST D2D1_PRINT_CONTROL_PROPERTIES * &, _Outptr_ ID2D1PrintControl ** &)> _CreatePrintControl_pre_ptr;
    NullPtr<void (D2D1DeviceHook::*)(HRESULT, _In_ IWICImagingFactory *, _In_ IPrintDocumentPackageTarget *, _In_opt_ CONST D2D1_PRINT_CONTROL_PROPERTIES *, _Outptr_ ID2D1PrintControl **)> _CreatePrintControl_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE SetMaximumTextureMemory(UINT64 maximumInBytes);
    NullPtr<void (D2D1DeviceHook::*)(UINT64 &)> _SetMaximumTextureMemory_pre_ptr;
    NullPtr<void (D2D1DeviceHook::*)(UINT64)> _SetMaximumTextureMemory_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT64 STDMETHODCALLTYPE GetMaximumTextureMemory() const;
    NullPtr<void (D2D1DeviceHook::*)() const> _GetMaximumTextureMemory_pre_ptr;
    NullPtr<void (D2D1DeviceHook::*)(UINT64) const> _GetMaximumTextureMemory_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ClearResources(UINT32 millisecondsSinceUse = 0);
    NullPtr<void (D2D1DeviceHook::*)(UINT32 &)> _ClearResources_pre_ptr;
    NullPtr<void (D2D1DeviceHook::*)(UINT32)> _ClearResources_post_ptr;

};

// ==============================================================================
// ID2D1Factory1
// ==============================================================================
class D2D1Factory1Hook : public HookBase<ID2D1Factory1>
{
    D2D1FactoryHook & _D2D1Factory;

protected:

    D2D1Factory1Hook(UnknownBase & unknown, D2D1FactoryHook & D2D1Factory, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _D2D1Factory(D2D1Factory)
    {
    }

    ~D2D1Factory1Hook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1FactoryHook * D2D1Factory = (D2D1FactoryHook *)unknown.GetHookedObj(__uuidof(ID2D1Factory));
        if (nullptr == D2D1Factory) return nullptr;

        try
        {
            return new D2D1Factory1Hook(unknown, *D2D1Factory, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1Factory1Hook * typedPtr = (D2D1Factory1Hook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE ReloadSystemMetrics()
    {
        return _D2D1Factory.ReloadSystemMetrics();
    }

    // -----------------------------------------------------------------------------
    void STDMETHODCALLTYPE GetDesktopDpi(_Out_ FLOAT * dpiX, _Out_ FLOAT * dpiY)
    {
        return _D2D1Factory.GetDesktopDpi(dpiX, dpiY);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRectangleGeometry(_In_ CONST D2D1_RECT_F * rectangle, _Outptr_ ID2D1RectangleGeometry ** rectangleGeometry)
    {
        return _D2D1Factory.CreateRectangleGeometry(rectangle, rectangleGeometry);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateRoundedRectangleGeometry(_In_ CONST D2D1_ROUNDED_RECT * roundedRectangle, _Outptr_ ID2D1RoundedRectangleGeometry ** roundedRectangleGeometry)
    {
        return _D2D1Factory.CreateRoundedRectangleGeometry(roundedRectangle, roundedRectangleGeometry);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateEllipseGeometry(_In_ CONST D2D1_ELLIPSE * ellipse, _Outptr_ ID2D1EllipseGeometry ** ellipseGeometry)
    {
        return _D2D1Factory.CreateEllipseGeometry(ellipse, ellipseGeometry);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateGeometryGroup(D2D1_FILL_MODE fillMode, _In_reads_(geometriesCount) ID2D1Geometry ** geometries, UINT32 geometriesCount, _Outptr_ ID2D1GeometryGroup ** geometryGroup)
    {
        return _D2D1Factory.CreateGeometryGroup(fillMode, geometries, geometriesCount, geometryGroup);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateTransformedGeometry(_In_ ID2D1Geometry * sourceGeometry, _In_ CONST D2D1_MATRIX_3X2_F * transform, _Outptr_ ID2D1TransformedGeometry ** transformedGeometry)
    {
        return _D2D1Factory.CreateTransformedGeometry(sourceGeometry, transform, transformedGeometry);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreatePathGeometry(_Outptr_ ID2D1PathGeometry ** pathGeometry)
    {
        return _D2D1Factory.CreatePathGeometry(pathGeometry);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateStrokeStyle(_In_ CONST D2D1_STROKE_STYLE_PROPERTIES * strokeStyleProperties, _In_reads_opt_(dashesCount) CONST FLOAT * dashes, UINT32 dashesCount, _Outptr_ ID2D1StrokeStyle ** strokeStyle)
    {
        return _D2D1Factory.CreateStrokeStyle(strokeStyleProperties, dashes, dashesCount, strokeStyle);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDrawingStateBlock(_In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION * drawingStateDescription, _In_opt_ IDWriteRenderingParams * textRenderingParams, _Outptr_ ID2D1DrawingStateBlock ** drawingStateBlock)
    {
        return _D2D1Factory.CreateDrawingStateBlock(drawingStateDescription, textRenderingParams, drawingStateBlock);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateWicBitmapRenderTarget(_In_ IWICBitmap * target, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties, _Outptr_ ID2D1RenderTarget ** renderTarget)
    {
        return _D2D1Factory.CreateWicBitmapRenderTarget(target, renderTargetProperties, renderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateHwndRenderTarget(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties, _In_ CONST D2D1_HWND_RENDER_TARGET_PROPERTIES * hwndRenderTargetProperties, _Outptr_ ID2D1HwndRenderTarget ** hwndRenderTarget)
    {
        return _D2D1Factory.CreateHwndRenderTarget(renderTargetProperties, hwndRenderTargetProperties, hwndRenderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDxgiSurfaceRenderTarget(_In_ IDXGISurface * dxgiSurface, _In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties, _Outptr_ ID2D1RenderTarget ** renderTarget)
    {
        return _D2D1Factory.CreateDxgiSurfaceRenderTarget(dxgiSurface, renderTargetProperties, renderTarget);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateDCRenderTarget(_In_ CONST D2D1_RENDER_TARGET_PROPERTIES * renderTargetProperties, _Outptr_ ID2D1DCRenderTarget ** dcRenderTarget)
    {
        return _D2D1Factory.CreateDCRenderTarget(renderTargetProperties, dcRenderTarget);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDevice(_In_ IDXGIDevice * dxgiDevice, _Outptr_ ID2D1Device ** d2dDevice);
    NullPtr<void (D2D1Factory1Hook::*)(_In_ IDXGIDevice * &, _Outptr_ ID2D1Device ** &)> _CreateDevice_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _In_ IDXGIDevice *, _Outptr_ ID2D1Device **)> _CreateDevice_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateStrokeStyle(_In_ CONST D2D1_STROKE_STYLE_PROPERTIES1 * strokeStyleProperties, _In_reads_opt_(dashesCount) CONST FLOAT * dashes, UINT32 dashesCount, _Outptr_ ID2D1StrokeStyle1 ** strokeStyle);
    NullPtr<void (D2D1Factory1Hook::*)(_In_ CONST D2D1_STROKE_STYLE_PROPERTIES1 * &, _In_reads_opt_(dashesCount) CONST FLOAT * &, UINT32 &, _Outptr_ ID2D1StrokeStyle1 ** &)> _CreateStrokeStyle_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _In_ CONST D2D1_STROKE_STYLE_PROPERTIES1 *, _In_reads_opt_(dashesCount) CONST FLOAT *, UINT32, _Outptr_ ID2D1StrokeStyle1 **)> _CreateStrokeStyle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreatePathGeometry(_Outptr_ ID2D1PathGeometry1 ** pathGeometry);
    NullPtr<void (D2D1Factory1Hook::*)(_Outptr_ ID2D1PathGeometry1 ** &)> _CreatePathGeometry_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _Outptr_ ID2D1PathGeometry1 **)> _CreatePathGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateDrawingStateBlock(_In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION1 * drawingStateDescription, _In_opt_ IDWriteRenderingParams * textRenderingParams, _Outptr_ ID2D1DrawingStateBlock1 ** drawingStateBlock);
    NullPtr<void (D2D1Factory1Hook::*)(_In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION1 * &, _In_opt_ IDWriteRenderingParams * &, _Outptr_ ID2D1DrawingStateBlock1 ** &)> _CreateDrawingStateBlock_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _In_opt_ CONST D2D1_DRAWING_STATE_DESCRIPTION1 *, _In_opt_ IDWriteRenderingParams *, _Outptr_ ID2D1DrawingStateBlock1 **)> _CreateDrawingStateBlock_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateGdiMetafile(_In_ IStream * metafileStream, _Outptr_ ID2D1GdiMetafile ** metafile);
    NullPtr<void (D2D1Factory1Hook::*)(_In_ IStream * &, _Outptr_ ID2D1GdiMetafile ** &)> _CreateGdiMetafile_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _In_ IStream *, _Outptr_ ID2D1GdiMetafile **)> _CreateGdiMetafile_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterEffectFromStream(_In_ REFCLSID classId, _In_ IStream * propertyXml, _In_reads_opt_(bindingsCount) CONST D2D1_PROPERTY_BINDING * bindings, UINT32 bindingsCount, _In_ CONST PD2D1_EFFECT_FACTORY effectFactory);
    NullPtr<void (D2D1Factory1Hook::*)(_In_ REFCLSID, _In_ IStream * &, _In_reads_opt_(bindingsCount) CONST D2D1_PROPERTY_BINDING * &, UINT32 &, _In_ CONST PD2D1_EFFECT_FACTORY &)> _RegisterEffectFromStream_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _In_ REFCLSID, _In_ IStream *, _In_reads_opt_(bindingsCount) CONST D2D1_PROPERTY_BINDING *, UINT32, _In_ CONST PD2D1_EFFECT_FACTORY)> _RegisterEffectFromStream_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterEffectFromString(_In_ REFCLSID classId, _In_ PCWSTR propertyXml, _In_reads_opt_(bindingsCount) CONST D2D1_PROPERTY_BINDING * bindings, UINT32 bindingsCount, _In_ CONST PD2D1_EFFECT_FACTORY effectFactory);
    NullPtr<void (D2D1Factory1Hook::*)(_In_ REFCLSID, _In_ PCWSTR &, _In_reads_opt_(bindingsCount) CONST D2D1_PROPERTY_BINDING * &, UINT32 &, _In_ CONST PD2D1_EFFECT_FACTORY &)> _RegisterEffectFromString_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _In_ REFCLSID, _In_ PCWSTR, _In_reads_opt_(bindingsCount) CONST D2D1_PROPERTY_BINDING *, UINT32, _In_ CONST PD2D1_EFFECT_FACTORY)> _RegisterEffectFromString_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnregisterEffect(_In_ REFCLSID classId);
    NullPtr<void (D2D1Factory1Hook::*)(_In_ REFCLSID)> _UnregisterEffect_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _In_ REFCLSID)> _UnregisterEffect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRegisteredEffects(_Out_writes_to_opt_(effectsCount, *effectsReturned) CLSID * effects, UINT32 effectsCount, _Out_opt_ UINT32 * effectsReturned, _Out_opt_ UINT32 * effectsRegistered) const;
    NullPtr<void (D2D1Factory1Hook::*)(_Out_writes_to_opt_(effectsCount, *effectsReturned) CLSID * &, UINT32 &, _Out_opt_ UINT32 * &, _Out_opt_ UINT32 * &) const> _GetRegisteredEffects_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _Out_writes_to_opt_(effectsCount, *effectsReturned) CLSID *, UINT32, _Out_opt_ UINT32 *, _Out_opt_ UINT32 *) const> _GetRegisteredEffects_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetEffectProperties(_In_ REFCLSID effectId, _Outptr_ ID2D1Properties ** properties) const;
    NullPtr<void (D2D1Factory1Hook::*)(_In_ REFCLSID, _Outptr_ ID2D1Properties ** &) const> _GetEffectProperties_pre_ptr;
    NullPtr<void (D2D1Factory1Hook::*)(HRESULT, _In_ REFCLSID, _Outptr_ ID2D1Properties **) const> _GetEffectProperties_post_ptr;

};

// ==============================================================================
// ID2D1Multithread
// ==============================================================================
class D2D1MultithreadHook : public HookBase<ID2D1Multithread>
{

protected:

    D2D1MultithreadHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~D2D1MultithreadHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new D2D1MultithreadHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        D2D1MultithreadHook * typedPtr = (D2D1MultithreadHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE GetMultithreadProtected() const;
    NullPtr<void (D2D1MultithreadHook::*)() const> _GetMultithreadProtected_pre_ptr;
    NullPtr<void (D2D1MultithreadHook::*)(BOOL) const> _GetMultithreadProtected_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE Enter();
    NullPtr<void (D2D1MultithreadHook::*)()> _Enter_pre_ptr;
    NullPtr<void (D2D1MultithreadHook::*)()> _Enter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE Leave();
    NullPtr<void (D2D1MultithreadHook::*)()> _Leave_pre_ptr;
    NullPtr<void (D2D1MultithreadHook::*)()> _Leave_post_ptr;

};

// ==============================================================================
// IDWriteFontFileLoader
// ==============================================================================
class DWriteFontFileLoaderHook : public HookBase<IDWriteFontFileLoader>
{

protected:

    DWriteFontFileLoaderHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontFileLoaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontFileLoaderHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontFileLoaderHook * typedPtr = (DWriteFontFileLoaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateStreamFromKey(_In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey, UINT32 fontFileReferenceKeySize, _Out_ IDWriteFontFileStream** fontFileStream);
    NullPtr<void (DWriteFontFileLoaderHook::*)(_In_reads_bytes_(fontFileReferenceKeySize) void const* &, UINT32 &, _Out_ IDWriteFontFileStream** &)> _CreateStreamFromKey_pre_ptr;
    NullPtr<void (DWriteFontFileLoaderHook::*)(HRESULT, _In_reads_bytes_(fontFileReferenceKeySize) void const*, UINT32, _Out_ IDWriteFontFileStream**)> _CreateStreamFromKey_post_ptr;

};

// ==============================================================================
// IDWriteLocalFontFileLoader
// ==============================================================================
class DWriteLocalFontFileLoaderHook : public HookBase<IDWriteLocalFontFileLoader>
{
    DWriteFontFileLoaderHook & _DWriteFontFileLoader;

protected:

    DWriteLocalFontFileLoaderHook(UnknownBase & unknown, DWriteFontFileLoaderHook & DWriteFontFileLoader, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DWriteFontFileLoader(DWriteFontFileLoader)
    {
    }

    ~DWriteLocalFontFileLoaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontFileLoaderHook * DWriteFontFileLoader = (DWriteFontFileLoaderHook *)unknown.GetHookedObj(__uuidof(IDWriteFontFileLoader));
        if (nullptr == DWriteFontFileLoader) return nullptr;

        try
        {
            return new DWriteLocalFontFileLoaderHook(unknown, *DWriteFontFileLoader, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteLocalFontFileLoaderHook * typedPtr = (DWriteLocalFontFileLoaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE CreateStreamFromKey(_In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey, UINT32 fontFileReferenceKeySize, _Out_ IDWriteFontFileStream** fontFileStream)
    {
        return _DWriteFontFileLoader.CreateStreamFromKey(fontFileReferenceKey, fontFileReferenceKeySize, fontFileStream);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFilePathLengthFromKey(_In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey, UINT32 fontFileReferenceKeySize, _Out_ UINT32* filePathLength);
    NullPtr<void (DWriteLocalFontFileLoaderHook::*)(_In_reads_bytes_(fontFileReferenceKeySize) void const* &, UINT32 &, _Out_ UINT32* &)> _GetFilePathLengthFromKey_pre_ptr;
    NullPtr<void (DWriteLocalFontFileLoaderHook::*)(HRESULT, _In_reads_bytes_(fontFileReferenceKeySize) void const*, UINT32, _Out_ UINT32*)> _GetFilePathLengthFromKey_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFilePathFromKey(_In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey, UINT32 fontFileReferenceKeySize, _Out_writes_z_(filePathSize) WCHAR* filePath, UINT32 filePathSize);
    NullPtr<void (DWriteLocalFontFileLoaderHook::*)(_In_reads_bytes_(fontFileReferenceKeySize) void const* &, UINT32 &, _Out_writes_z_(filePathSize) WCHAR* &, UINT32 &)> _GetFilePathFromKey_pre_ptr;
    NullPtr<void (DWriteLocalFontFileLoaderHook::*)(HRESULT, _In_reads_bytes_(fontFileReferenceKeySize) void const*, UINT32, _Out_writes_z_(filePathSize) WCHAR*, UINT32)> _GetFilePathFromKey_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLastWriteTimeFromKey(_In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey, UINT32 fontFileReferenceKeySize, _Out_ FILETIME* lastWriteTime);
    NullPtr<void (DWriteLocalFontFileLoaderHook::*)(_In_reads_bytes_(fontFileReferenceKeySize) void const* &, UINT32 &, _Out_ FILETIME* &)> _GetLastWriteTimeFromKey_pre_ptr;
    NullPtr<void (DWriteLocalFontFileLoaderHook::*)(HRESULT, _In_reads_bytes_(fontFileReferenceKeySize) void const*, UINT32, _Out_ FILETIME*)> _GetLastWriteTimeFromKey_post_ptr;

};

// ==============================================================================
// IDWriteFontFileStream
// ==============================================================================
class DWriteFontFileStreamHook : public HookBase<IDWriteFontFileStream>
{

protected:

    DWriteFontFileStreamHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontFileStreamHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontFileStreamHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontFileStreamHook * typedPtr = (DWriteFontFileStreamHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ReadFileFragment(_Outptr_result_bytebuffer_(fragmentSize) void const** fragmentStart, UINT64 fileOffset, UINT64 fragmentSize, _Out_ void** fragmentContext);
    NullPtr<void (DWriteFontFileStreamHook::*)(_Outptr_result_bytebuffer_(fragmentSize) void const** &, UINT64 &, UINT64 &, _Out_ void** &)> _ReadFileFragment_pre_ptr;
    NullPtr<void (DWriteFontFileStreamHook::*)(HRESULT, _Outptr_result_bytebuffer_(fragmentSize) void const**, UINT64, UINT64, _Out_ void**)> _ReadFileFragment_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ReleaseFileFragment(void* fragmentContext);
    NullPtr<void (DWriteFontFileStreamHook::*)(void* &)> _ReleaseFileFragment_pre_ptr;
    NullPtr<void (DWriteFontFileStreamHook::*)(void*)> _ReleaseFileFragment_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFileSize(_Out_ UINT64* fileSize);
    NullPtr<void (DWriteFontFileStreamHook::*)(_Out_ UINT64* &)> _GetFileSize_pre_ptr;
    NullPtr<void (DWriteFontFileStreamHook::*)(HRESULT, _Out_ UINT64*)> _GetFileSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLastWriteTime(_Out_ UINT64* lastWriteTime);
    NullPtr<void (DWriteFontFileStreamHook::*)(_Out_ UINT64* &)> _GetLastWriteTime_pre_ptr;
    NullPtr<void (DWriteFontFileStreamHook::*)(HRESULT, _Out_ UINT64*)> _GetLastWriteTime_post_ptr;

};

// ==============================================================================
// IDWriteFontFile
// ==============================================================================
class DWriteFontFileHook : public HookBase<IDWriteFontFile>
{

protected:

    DWriteFontFileHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontFileHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontFileHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontFileHook * typedPtr = (DWriteFontFileHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetReferenceKey(_Outptr_result_bytebuffer_(*fontFileReferenceKeySize) void const** fontFileReferenceKey, _Out_ UINT32* fontFileReferenceKeySize);
    NullPtr<void (DWriteFontFileHook::*)(_Outptr_result_bytebuffer_(*fontFileReferenceKeySize) void const** &, _Out_ UINT32* &)> _GetReferenceKey_pre_ptr;
    NullPtr<void (DWriteFontFileHook::*)(HRESULT, _Outptr_result_bytebuffer_(*fontFileReferenceKeySize) void const**, _Out_ UINT32*)> _GetReferenceKey_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLoader(_Out_ IDWriteFontFileLoader** fontFileLoader);
    NullPtr<void (DWriteFontFileHook::*)(_Out_ IDWriteFontFileLoader** &)> _GetLoader_pre_ptr;
    NullPtr<void (DWriteFontFileHook::*)(HRESULT, _Out_ IDWriteFontFileLoader**)> _GetLoader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Analyze(_Out_ BOOL* isSupportedFontType, _Out_ DWRITE_FONT_FILE_TYPE* fontFileType, _Out_opt_ DWRITE_FONT_FACE_TYPE* fontFaceType, _Out_ UINT32* numberOfFaces);
    NullPtr<void (DWriteFontFileHook::*)(_Out_ BOOL* &, _Out_ DWRITE_FONT_FILE_TYPE* &, _Out_opt_ DWRITE_FONT_FACE_TYPE* &, _Out_ UINT32* &)> _Analyze_pre_ptr;
    NullPtr<void (DWriteFontFileHook::*)(HRESULT, _Out_ BOOL*, _Out_ DWRITE_FONT_FILE_TYPE*, _Out_opt_ DWRITE_FONT_FACE_TYPE*, _Out_ UINT32*)> _Analyze_post_ptr;

};

// ==============================================================================
// IDWriteRenderingParams
// ==============================================================================
class DWriteRenderingParamsHook : public HookBase<IDWriteRenderingParams>
{

protected:

    DWriteRenderingParamsHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteRenderingParamsHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteRenderingParamsHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteRenderingParamsHook * typedPtr = (DWriteRenderingParamsHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetGamma();
    NullPtr<void (DWriteRenderingParamsHook::*)()> _GetGamma_pre_ptr;
    NullPtr<void (DWriteRenderingParamsHook::*)(FLOAT)> _GetGamma_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetEnhancedContrast();
    NullPtr<void (DWriteRenderingParamsHook::*)()> _GetEnhancedContrast_pre_ptr;
    NullPtr<void (DWriteRenderingParamsHook::*)(FLOAT)> _GetEnhancedContrast_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetClearTypeLevel();
    NullPtr<void (DWriteRenderingParamsHook::*)()> _GetClearTypeLevel_pre_ptr;
    NullPtr<void (DWriteRenderingParamsHook::*)(FLOAT)> _GetClearTypeLevel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_PIXEL_GEOMETRY STDMETHODCALLTYPE GetPixelGeometry();
    NullPtr<void (DWriteRenderingParamsHook::*)()> _GetPixelGeometry_pre_ptr;
    NullPtr<void (DWriteRenderingParamsHook::*)(DWRITE_PIXEL_GEOMETRY)> _GetPixelGeometry_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_RENDERING_MODE STDMETHODCALLTYPE GetRenderingMode();
    NullPtr<void (DWriteRenderingParamsHook::*)()> _GetRenderingMode_pre_ptr;
    NullPtr<void (DWriteRenderingParamsHook::*)(DWRITE_RENDERING_MODE)> _GetRenderingMode_post_ptr;

};

// ==============================================================================
// IDWriteFontFace
// ==============================================================================
class DWriteFontFaceHook : public HookBase<IDWriteFontFace>
{

protected:

    DWriteFontFaceHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontFaceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontFaceHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontFaceHook * typedPtr = (DWriteFontFaceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_FACE_TYPE STDMETHODCALLTYPE GetType();
    NullPtr<void (DWriteFontFaceHook::*)()> _GetType_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(DWRITE_FONT_FACE_TYPE)> _GetType_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFiles(_Inout_ UINT32* numberOfFiles, _Out_writes_opt_(*numberOfFiles) IDWriteFontFile** fontFiles);
    NullPtr<void (DWriteFontFaceHook::*)(_Inout_ UINT32* &, _Out_writes_opt_(*numberOfFiles) IDWriteFontFile** &)> _GetFiles_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(HRESULT, _Inout_ UINT32*, _Out_writes_opt_(*numberOfFiles) IDWriteFontFile**)> _GetFiles_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetIndex();
    NullPtr<void (DWriteFontFaceHook::*)()> _GetIndex_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(UINT32)> _GetIndex_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_SIMULATIONS STDMETHODCALLTYPE GetSimulations();
    NullPtr<void (DWriteFontFaceHook::*)()> _GetSimulations_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(DWRITE_FONT_SIMULATIONS)> _GetSimulations_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsSymbolFont();
    NullPtr<void (DWriteFontFaceHook::*)()> _IsSymbolFont_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(BOOL)> _IsSymbolFont_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetMetrics(_Out_ DWRITE_FONT_METRICS* fontFaceMetrics);
    NullPtr<void (DWriteFontFaceHook::*)(_Out_ DWRITE_FONT_METRICS* &)> _GetMetrics_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(_Out_ DWRITE_FONT_METRICS*)> _GetMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT16 STDMETHODCALLTYPE GetGlyphCount();
    NullPtr<void (DWriteFontFaceHook::*)()> _GetGlyphCount_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(UINT16)> _GetGlyphCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDesignGlyphMetrics(_In_reads_(glyphCount) UINT16 const* glyphIndices, UINT32 glyphCount, _Out_writes_(glyphCount) DWRITE_GLYPH_METRICS* glyphMetrics, BOOL isSideways = FALSE);
    NullPtr<void (DWriteFontFaceHook::*)(_In_reads_(glyphCount) UINT16 const* &, UINT32 &, _Out_writes_(glyphCount) DWRITE_GLYPH_METRICS* &, BOOL &)> _GetDesignGlyphMetrics_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(HRESULT, _In_reads_(glyphCount) UINT16 const*, UINT32, _Out_writes_(glyphCount) DWRITE_GLYPH_METRICS*, BOOL)> _GetDesignGlyphMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGlyphIndices(_In_reads_(codePointCount) UINT32 const* codePoints, UINT32 codePointCount, _Out_writes_(codePointCount) UINT16* glyphIndices);
    NullPtr<void (DWriteFontFaceHook::*)(_In_reads_(codePointCount) UINT32 const* &, UINT32 &, _Out_writes_(codePointCount) UINT16* &)> _GetGlyphIndices_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(HRESULT, _In_reads_(codePointCount) UINT32 const*, UINT32, _Out_writes_(codePointCount) UINT16*)> _GetGlyphIndices_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE TryGetFontTable(_In_ UINT32 openTypeTableTag, _Outptr_result_bytebuffer_(*tableSize) const void** tableData, _Out_ UINT32* tableSize, _Out_ void** tableContext, _Out_ BOOL* exists);
    NullPtr<void (DWriteFontFaceHook::*)(_In_ UINT32 &, _Outptr_result_bytebuffer_(*tableSize) const void** &, _Out_ UINT32* &, _Out_ void** &, _Out_ BOOL* &)> _TryGetFontTable_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(HRESULT, _In_ UINT32, _Outptr_result_bytebuffer_(*tableSize) const void**, _Out_ UINT32*, _Out_ void**, _Out_ BOOL*)> _TryGetFontTable_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE ReleaseFontTable(_In_ void* tableContext);
    NullPtr<void (DWriteFontFaceHook::*)(_In_ void* &)> _ReleaseFontTable_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(_In_ void*)> _ReleaseFontTable_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGlyphRunOutline(FLOAT emSize, _In_reads_(glyphCount) UINT16 const* glyphIndices, _In_reads_opt_(glyphCount) FLOAT const* glyphAdvances, _In_reads_opt_(glyphCount) DWRITE_GLYPH_OFFSET const* glyphOffsets, UINT32 glyphCount, BOOL isSideways, BOOL isRightToLeft, IDWriteGeometrySink* geometrySink);
    NullPtr<void (DWriteFontFaceHook::*)(FLOAT &, _In_reads_(glyphCount) UINT16 const* &, _In_reads_opt_(glyphCount) FLOAT const* &, _In_reads_opt_(glyphCount) DWRITE_GLYPH_OFFSET const* &, UINT32 &, BOOL &, BOOL &, IDWriteGeometrySink* &)> _GetGlyphRunOutline_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(HRESULT, FLOAT, _In_reads_(glyphCount) UINT16 const*, _In_reads_opt_(glyphCount) FLOAT const*, _In_reads_opt_(glyphCount) DWRITE_GLYPH_OFFSET const*, UINT32, BOOL, BOOL, IDWriteGeometrySink*)> _GetGlyphRunOutline_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetRecommendedRenderingMode(FLOAT emSize, FLOAT pixelsPerDip, DWRITE_MEASURING_MODE measuringMode, IDWriteRenderingParams* renderingParams, _Out_ DWRITE_RENDERING_MODE* renderingMode);
    NullPtr<void (DWriteFontFaceHook::*)(FLOAT &, FLOAT &, DWRITE_MEASURING_MODE &, IDWriteRenderingParams* &, _Out_ DWRITE_RENDERING_MODE* &)> _GetRecommendedRenderingMode_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(HRESULT, FLOAT, FLOAT, DWRITE_MEASURING_MODE, IDWriteRenderingParams*, _Out_ DWRITE_RENDERING_MODE*)> _GetRecommendedRenderingMode_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGdiCompatibleMetrics(FLOAT emSize, FLOAT pixelsPerDip, _In_opt_ DWRITE_MATRIX const* transform, _Out_ DWRITE_FONT_METRICS* fontFaceMetrics);
    NullPtr<void (DWriteFontFaceHook::*)(FLOAT &, FLOAT &, _In_opt_ DWRITE_MATRIX const* &, _Out_ DWRITE_FONT_METRICS* &)> _GetGdiCompatibleMetrics_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(HRESULT, FLOAT, FLOAT, _In_opt_ DWRITE_MATRIX const*, _Out_ DWRITE_FONT_METRICS*)> _GetGdiCompatibleMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGdiCompatibleGlyphMetrics(FLOAT emSize, FLOAT pixelsPerDip, _In_opt_ DWRITE_MATRIX const* transform, BOOL useGdiNatural, _In_reads_(glyphCount) UINT16 const* glyphIndices, UINT32 glyphCount, _Out_writes_(glyphCount) DWRITE_GLYPH_METRICS* glyphMetrics, BOOL isSideways = FALSE);
    NullPtr<void (DWriteFontFaceHook::*)(FLOAT &, FLOAT &, _In_opt_ DWRITE_MATRIX const* &, BOOL &, _In_reads_(glyphCount) UINT16 const* &, UINT32 &, _Out_writes_(glyphCount) DWRITE_GLYPH_METRICS* &, BOOL &)> _GetGdiCompatibleGlyphMetrics_pre_ptr;
    NullPtr<void (DWriteFontFaceHook::*)(HRESULT, FLOAT, FLOAT, _In_opt_ DWRITE_MATRIX const*, BOOL, _In_reads_(glyphCount) UINT16 const*, UINT32, _Out_writes_(glyphCount) DWRITE_GLYPH_METRICS*, BOOL)> _GetGdiCompatibleGlyphMetrics_post_ptr;

};

// ==============================================================================
// IDWriteFontCollectionLoader
// ==============================================================================
class DWriteFontCollectionLoaderHook : public HookBase<IDWriteFontCollectionLoader>
{

protected:

    DWriteFontCollectionLoaderHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontCollectionLoaderHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontCollectionLoaderHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontCollectionLoaderHook * typedPtr = (DWriteFontCollectionLoaderHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(IDWriteFactory* factory, _In_reads_bytes_(collectionKeySize) void const* collectionKey, UINT32 collectionKeySize, _Out_ IDWriteFontFileEnumerator** fontFileEnumerator);
    NullPtr<void (DWriteFontCollectionLoaderHook::*)(IDWriteFactory* &, _In_reads_bytes_(collectionKeySize) void const* &, UINT32 &, _Out_ IDWriteFontFileEnumerator** &)> _CreateEnumeratorFromKey_pre_ptr;
    NullPtr<void (DWriteFontCollectionLoaderHook::*)(HRESULT, IDWriteFactory*, _In_reads_bytes_(collectionKeySize) void const*, UINT32, _Out_ IDWriteFontFileEnumerator**)> _CreateEnumeratorFromKey_post_ptr;

};

// ==============================================================================
// IDWriteFontFileEnumerator
// ==============================================================================
class DWriteFontFileEnumeratorHook : public HookBase<IDWriteFontFileEnumerator>
{

protected:

    DWriteFontFileEnumeratorHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontFileEnumeratorHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontFileEnumeratorHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontFileEnumeratorHook * typedPtr = (DWriteFontFileEnumeratorHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ BOOL* hasCurrentFile);
    NullPtr<void (DWriteFontFileEnumeratorHook::*)(_Out_ BOOL* &)> _MoveNext_pre_ptr;
    NullPtr<void (DWriteFontFileEnumeratorHook::*)(HRESULT, _Out_ BOOL*)> _MoveNext_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile);
    NullPtr<void (DWriteFontFileEnumeratorHook::*)(_Out_ IDWriteFontFile** &)> _GetCurrentFontFile_pre_ptr;
    NullPtr<void (DWriteFontFileEnumeratorHook::*)(HRESULT, _Out_ IDWriteFontFile**)> _GetCurrentFontFile_post_ptr;

};

// ==============================================================================
// IDWriteLocalizedStrings
// ==============================================================================
class DWriteLocalizedStringsHook : public HookBase<IDWriteLocalizedStrings>
{

protected:

    DWriteLocalizedStringsHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteLocalizedStringsHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteLocalizedStringsHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteLocalizedStringsHook * typedPtr = (DWriteLocalizedStringsHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetCount();
    NullPtr<void (DWriteLocalizedStringsHook::*)()> _GetCount_pre_ptr;
    NullPtr<void (DWriteLocalizedStringsHook::*)(UINT32)> _GetCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FindLocaleName(_In_z_ WCHAR const* localeName, _Out_ UINT32* index, _Out_ BOOL* exists);
    NullPtr<void (DWriteLocalizedStringsHook::*)(_In_z_ WCHAR const* &, _Out_ UINT32* &, _Out_ BOOL* &)> _FindLocaleName_pre_ptr;
    NullPtr<void (DWriteLocalizedStringsHook::*)(HRESULT, _In_z_ WCHAR const*, _Out_ UINT32*, _Out_ BOOL*)> _FindLocaleName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLocaleNameLength(UINT32 index, _Out_ UINT32* length);
    NullPtr<void (DWriteLocalizedStringsHook::*)(UINT32 &, _Out_ UINT32* &)> _GetLocaleNameLength_pre_ptr;
    NullPtr<void (DWriteLocalizedStringsHook::*)(HRESULT, UINT32, _Out_ UINT32*)> _GetLocaleNameLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLocaleName(UINT32 index, _Out_writes_z_(size) WCHAR* localeName, UINT32 size);
    NullPtr<void (DWriteLocalizedStringsHook::*)(UINT32 &, _Out_writes_z_(size) WCHAR* &, UINT32 &)> _GetLocaleName_pre_ptr;
    NullPtr<void (DWriteLocalizedStringsHook::*)(HRESULT, UINT32, _Out_writes_z_(size) WCHAR*, UINT32)> _GetLocaleName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetStringLength(UINT32 index, _Out_ UINT32* length);
    NullPtr<void (DWriteLocalizedStringsHook::*)(UINT32 &, _Out_ UINT32* &)> _GetStringLength_pre_ptr;
    NullPtr<void (DWriteLocalizedStringsHook::*)(HRESULT, UINT32, _Out_ UINT32*)> _GetStringLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetString(UINT32 index, _Out_writes_z_(size) WCHAR* stringBuffer, UINT32 size);
    NullPtr<void (DWriteLocalizedStringsHook::*)(UINT32 &, _Out_writes_z_(size) WCHAR* &, UINT32 &)> _GetString_pre_ptr;
    NullPtr<void (DWriteLocalizedStringsHook::*)(HRESULT, UINT32, _Out_writes_z_(size) WCHAR*, UINT32)> _GetString_post_ptr;

};

// ==============================================================================
// IDWriteFontCollection
// ==============================================================================
class DWriteFontCollectionHook : public HookBase<IDWriteFontCollection>
{

protected:

    DWriteFontCollectionHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontCollectionHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontCollectionHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontCollectionHook * typedPtr = (DWriteFontCollectionHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetFontFamilyCount();
    NullPtr<void (DWriteFontCollectionHook::*)()> _GetFontFamilyCount_pre_ptr;
    NullPtr<void (DWriteFontCollectionHook::*)(UINT32)> _GetFontFamilyCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontFamily(UINT32 index, _Out_ IDWriteFontFamily** fontFamily);
    NullPtr<void (DWriteFontCollectionHook::*)(UINT32 &, _Out_ IDWriteFontFamily** &)> _GetFontFamily_pre_ptr;
    NullPtr<void (DWriteFontCollectionHook::*)(HRESULT, UINT32, _Out_ IDWriteFontFamily**)> _GetFontFamily_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE FindFamilyName(_In_z_ WCHAR const* familyName, _Out_ UINT32* index, _Out_ BOOL* exists);
    NullPtr<void (DWriteFontCollectionHook::*)(_In_z_ WCHAR const* &, _Out_ UINT32* &, _Out_ BOOL* &)> _FindFamilyName_pre_ptr;
    NullPtr<void (DWriteFontCollectionHook::*)(HRESULT, _In_z_ WCHAR const*, _Out_ UINT32*, _Out_ BOOL*)> _FindFamilyName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontFromFontFace(IDWriteFontFace* fontFace, _Out_ IDWriteFont** font);
    NullPtr<void (DWriteFontCollectionHook::*)(IDWriteFontFace* &, _Out_ IDWriteFont** &)> _GetFontFromFontFace_pre_ptr;
    NullPtr<void (DWriteFontCollectionHook::*)(HRESULT, IDWriteFontFace*, _Out_ IDWriteFont**)> _GetFontFromFontFace_post_ptr;

};

// ==============================================================================
// IDWriteFontList
// ==============================================================================
class DWriteFontListHook : public HookBase<IDWriteFontList>
{

protected:

    DWriteFontListHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontListHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontListHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontListHook * typedPtr = (DWriteFontListHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontCollection(_Out_ IDWriteFontCollection** fontCollection);
    NullPtr<void (DWriteFontListHook::*)(_Out_ IDWriteFontCollection** &)> _GetFontCollection_pre_ptr;
    NullPtr<void (DWriteFontListHook::*)(HRESULT, _Out_ IDWriteFontCollection**)> _GetFontCollection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetFontCount();
    NullPtr<void (DWriteFontListHook::*)()> _GetFontCount_pre_ptr;
    NullPtr<void (DWriteFontListHook::*)(UINT32)> _GetFontCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFont(UINT32 index, _Out_ IDWriteFont** font);
    NullPtr<void (DWriteFontListHook::*)(UINT32 &, _Out_ IDWriteFont** &)> _GetFont_pre_ptr;
    NullPtr<void (DWriteFontListHook::*)(HRESULT, UINT32, _Out_ IDWriteFont**)> _GetFont_post_ptr;

};

// ==============================================================================
// IDWriteFontFamily
// ==============================================================================
class DWriteFontFamilyHook : public HookBase<IDWriteFontFamily>
{
    DWriteFontListHook & _DWriteFontList;

protected:

    DWriteFontFamilyHook(UnknownBase & unknown, DWriteFontListHook & DWriteFontList, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DWriteFontList(DWriteFontList)
    {
    }

    ~DWriteFontFamilyHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontListHook * DWriteFontList = (DWriteFontListHook *)unknown.GetHookedObj(__uuidof(IDWriteFontList));
        if (nullptr == DWriteFontList) return nullptr;

        try
        {
            return new DWriteFontFamilyHook(unknown, *DWriteFontList, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontFamilyHook * typedPtr = (DWriteFontFamilyHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFontCollection(_Out_ IDWriteFontCollection** fontCollection)
    {
        return _DWriteFontList.GetFontCollection(fontCollection);
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetFontCount()
    {
        return _DWriteFontList.GetFontCount();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFont(UINT32 index, _Out_ IDWriteFont** font)
    {
        return _DWriteFontList.GetFont(index, font);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFamilyNames(_Out_ IDWriteLocalizedStrings** names);
    NullPtr<void (DWriteFontFamilyHook::*)(_Out_ IDWriteLocalizedStrings** &)> _GetFamilyNames_pre_ptr;
    NullPtr<void (DWriteFontFamilyHook::*)(HRESULT, _Out_ IDWriteLocalizedStrings**)> _GetFamilyNames_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFirstMatchingFont(DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STRETCH stretch, DWRITE_FONT_STYLE style, _Out_ IDWriteFont** matchingFont);
    NullPtr<void (DWriteFontFamilyHook::*)(DWRITE_FONT_WEIGHT &, DWRITE_FONT_STRETCH &, DWRITE_FONT_STYLE &, _Out_ IDWriteFont** &)> _GetFirstMatchingFont_pre_ptr;
    NullPtr<void (DWriteFontFamilyHook::*)(HRESULT, DWRITE_FONT_WEIGHT, DWRITE_FONT_STRETCH, DWRITE_FONT_STYLE, _Out_ IDWriteFont**)> _GetFirstMatchingFont_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetMatchingFonts(DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STRETCH stretch, DWRITE_FONT_STYLE style, _Out_ IDWriteFontList** matchingFonts);
    NullPtr<void (DWriteFontFamilyHook::*)(DWRITE_FONT_WEIGHT &, DWRITE_FONT_STRETCH &, DWRITE_FONT_STYLE &, _Out_ IDWriteFontList** &)> _GetMatchingFonts_pre_ptr;
    NullPtr<void (DWriteFontFamilyHook::*)(HRESULT, DWRITE_FONT_WEIGHT, DWRITE_FONT_STRETCH, DWRITE_FONT_STYLE, _Out_ IDWriteFontList**)> _GetMatchingFonts_post_ptr;

};

// ==============================================================================
// IDWriteFont
// ==============================================================================
class DWriteFontHook : public HookBase<IDWriteFont>
{

protected:

    DWriteFontHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFontHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFontHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFontHook * typedPtr = (DWriteFontHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontFamily(_Out_ IDWriteFontFamily** fontFamily);
    NullPtr<void (DWriteFontHook::*)(_Out_ IDWriteFontFamily** &)> _GetFontFamily_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(HRESULT, _Out_ IDWriteFontFamily**)> _GetFontFamily_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_WEIGHT STDMETHODCALLTYPE GetWeight();
    NullPtr<void (DWriteFontHook::*)()> _GetWeight_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(DWRITE_FONT_WEIGHT)> _GetWeight_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_STRETCH STDMETHODCALLTYPE GetStretch();
    NullPtr<void (DWriteFontHook::*)()> _GetStretch_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(DWRITE_FONT_STRETCH)> _GetStretch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_STYLE STDMETHODCALLTYPE GetStyle();
    NullPtr<void (DWriteFontHook::*)()> _GetStyle_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(DWRITE_FONT_STYLE)> _GetStyle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual BOOL STDMETHODCALLTYPE IsSymbolFont();
    NullPtr<void (DWriteFontHook::*)()> _IsSymbolFont_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(BOOL)> _IsSymbolFont_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFaceNames(_Out_ IDWriteLocalizedStrings** names);
    NullPtr<void (DWriteFontHook::*)(_Out_ IDWriteLocalizedStrings** &)> _GetFaceNames_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(HRESULT, _Out_ IDWriteLocalizedStrings**)> _GetFaceNames_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetInformationalStrings(DWRITE_INFORMATIONAL_STRING_ID informationalStringID, _Out_ IDWriteLocalizedStrings** informationalStrings, _Out_ BOOL* exists);
    NullPtr<void (DWriteFontHook::*)(DWRITE_INFORMATIONAL_STRING_ID &, _Out_ IDWriteLocalizedStrings** &, _Out_ BOOL* &)> _GetInformationalStrings_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(HRESULT, DWRITE_INFORMATIONAL_STRING_ID, _Out_ IDWriteLocalizedStrings**, _Out_ BOOL*)> _GetInformationalStrings_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_SIMULATIONS STDMETHODCALLTYPE GetSimulations();
    NullPtr<void (DWriteFontHook::*)()> _GetSimulations_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(DWRITE_FONT_SIMULATIONS)> _GetSimulations_post_ptr;

    // -----------------------------------------------------------------------------
    virtual void STDMETHODCALLTYPE GetMetrics(_Out_ DWRITE_FONT_METRICS* fontMetrics);
    NullPtr<void (DWriteFontHook::*)(_Out_ DWRITE_FONT_METRICS* &)> _GetMetrics_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(_Out_ DWRITE_FONT_METRICS*)> _GetMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE HasCharacter(UINT32 unicodeValue, _Out_ BOOL* exists);
    NullPtr<void (DWriteFontHook::*)(UINT32 &, _Out_ BOOL* &)> _HasCharacter_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(HRESULT, UINT32, _Out_ BOOL*)> _HasCharacter_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateFontFace(_Out_ IDWriteFontFace** fontFace);
    NullPtr<void (DWriteFontHook::*)(_Out_ IDWriteFontFace** &)> _CreateFontFace_pre_ptr;
    NullPtr<void (DWriteFontHook::*)(HRESULT, _Out_ IDWriteFontFace**)> _CreateFontFace_post_ptr;

};

// ==============================================================================
// IDWriteTextFormat
// ==============================================================================
class DWriteTextFormatHook : public HookBase<IDWriteTextFormat>
{

protected:

    DWriteTextFormatHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteTextFormatHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteTextFormatHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteTextFormatHook * typedPtr = (DWriteTextFormatHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment);
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_TEXT_ALIGNMENT &)> _SetTextAlignment_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, DWRITE_TEXT_ALIGNMENT)> _SetTextAlignment_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment);
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_PARAGRAPH_ALIGNMENT &)> _SetParagraphAlignment_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, DWRITE_PARAGRAPH_ALIGNMENT)> _SetParagraphAlignment_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetWordWrapping(DWRITE_WORD_WRAPPING wordWrapping);
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_WORD_WRAPPING &)> _SetWordWrapping_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, DWRITE_WORD_WRAPPING)> _SetWordWrapping_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetReadingDirection(DWRITE_READING_DIRECTION readingDirection);
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_READING_DIRECTION &)> _SetReadingDirection_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, DWRITE_READING_DIRECTION)> _SetReadingDirection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFlowDirection(DWRITE_FLOW_DIRECTION flowDirection);
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_FLOW_DIRECTION &)> _SetFlowDirection_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, DWRITE_FLOW_DIRECTION)> _SetFlowDirection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetIncrementalTabStop(FLOAT incrementalTabStop);
    NullPtr<void (DWriteTextFormatHook::*)(FLOAT &)> _SetIncrementalTabStop_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, FLOAT)> _SetIncrementalTabStop_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTrimming(_In_ DWRITE_TRIMMING const* trimmingOptions, IDWriteInlineObject* trimmingSign);
    NullPtr<void (DWriteTextFormatHook::*)(_In_ DWRITE_TRIMMING const* &, IDWriteInlineObject* &)> _SetTrimming_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, _In_ DWRITE_TRIMMING const*, IDWriteInlineObject*)> _SetTrimming_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetLineSpacing(DWRITE_LINE_SPACING_METHOD lineSpacingMethod, FLOAT lineSpacing, FLOAT baseline);
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_LINE_SPACING_METHOD &, FLOAT &, FLOAT &)> _SetLineSpacing_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, DWRITE_LINE_SPACING_METHOD, FLOAT, FLOAT)> _SetLineSpacing_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_TEXT_ALIGNMENT STDMETHODCALLTYPE GetTextAlignment();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetTextAlignment_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_TEXT_ALIGNMENT)> _GetTextAlignment_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_PARAGRAPH_ALIGNMENT STDMETHODCALLTYPE GetParagraphAlignment();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetParagraphAlignment_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_PARAGRAPH_ALIGNMENT)> _GetParagraphAlignment_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_WORD_WRAPPING STDMETHODCALLTYPE GetWordWrapping();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetWordWrapping_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_WORD_WRAPPING)> _GetWordWrapping_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_READING_DIRECTION STDMETHODCALLTYPE GetReadingDirection();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetReadingDirection_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_READING_DIRECTION)> _GetReadingDirection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FLOW_DIRECTION STDMETHODCALLTYPE GetFlowDirection();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetFlowDirection_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_FLOW_DIRECTION)> _GetFlowDirection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetIncrementalTabStop();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetIncrementalTabStop_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(FLOAT)> _GetIncrementalTabStop_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetTrimming(_Out_ DWRITE_TRIMMING* trimmingOptions, _Out_ IDWriteInlineObject** trimmingSign);
    NullPtr<void (DWriteTextFormatHook::*)(_Out_ DWRITE_TRIMMING* &, _Out_ IDWriteInlineObject** &)> _GetTrimming_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, _Out_ DWRITE_TRIMMING*, _Out_ IDWriteInlineObject**)> _GetTrimming_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLineSpacing(_Out_ DWRITE_LINE_SPACING_METHOD* lineSpacingMethod, _Out_ FLOAT* lineSpacing, _Out_ FLOAT* baseline);
    NullPtr<void (DWriteTextFormatHook::*)(_Out_ DWRITE_LINE_SPACING_METHOD* &, _Out_ FLOAT* &, _Out_ FLOAT* &)> _GetLineSpacing_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, _Out_ DWRITE_LINE_SPACING_METHOD*, _Out_ FLOAT*, _Out_ FLOAT*)> _GetLineSpacing_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontCollection(_Out_ IDWriteFontCollection** fontCollection);
    NullPtr<void (DWriteTextFormatHook::*)(_Out_ IDWriteFontCollection** &)> _GetFontCollection_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, _Out_ IDWriteFontCollection**)> _GetFontCollection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetFontFamilyNameLength();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetFontFamilyNameLength_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(UINT32)> _GetFontFamilyNameLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontFamilyName(_Out_writes_z_(nameSize) WCHAR* fontFamilyName, UINT32 nameSize);
    NullPtr<void (DWriteTextFormatHook::*)(_Out_writes_z_(nameSize) WCHAR* &, UINT32 &)> _GetFontFamilyName_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, _Out_writes_z_(nameSize) WCHAR*, UINT32)> _GetFontFamilyName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_WEIGHT STDMETHODCALLTYPE GetFontWeight();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetFontWeight_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_FONT_WEIGHT)> _GetFontWeight_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_STYLE STDMETHODCALLTYPE GetFontStyle();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetFontStyle_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_FONT_STYLE)> _GetFontStyle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_FONT_STRETCH STDMETHODCALLTYPE GetFontStretch();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetFontStretch_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(DWRITE_FONT_STRETCH)> _GetFontStretch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetFontSize();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetFontSize_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(FLOAT)> _GetFontSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetLocaleNameLength();
    NullPtr<void (DWriteTextFormatHook::*)()> _GetLocaleNameLength_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(UINT32)> _GetLocaleNameLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLocaleName(_Out_writes_z_(nameSize) WCHAR* localeName, UINT32 nameSize);
    NullPtr<void (DWriteTextFormatHook::*)(_Out_writes_z_(nameSize) WCHAR* &, UINT32 &)> _GetLocaleName_pre_ptr;
    NullPtr<void (DWriteTextFormatHook::*)(HRESULT, _Out_writes_z_(nameSize) WCHAR*, UINT32)> _GetLocaleName_post_ptr;

};

// ==============================================================================
// IDWriteTypography
// ==============================================================================
class DWriteTypographyHook : public HookBase<IDWriteTypography>
{

protected:

    DWriteTypographyHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteTypographyHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteTypographyHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteTypographyHook * typedPtr = (DWriteTypographyHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AddFontFeature(DWRITE_FONT_FEATURE fontFeature);
    NullPtr<void (DWriteTypographyHook::*)(DWRITE_FONT_FEATURE &)> _AddFontFeature_pre_ptr;
    NullPtr<void (DWriteTypographyHook::*)(HRESULT, DWRITE_FONT_FEATURE)> _AddFontFeature_post_ptr;

    // -----------------------------------------------------------------------------
    virtual UINT32 STDMETHODCALLTYPE GetFontFeatureCount();
    NullPtr<void (DWriteTypographyHook::*)()> _GetFontFeatureCount_pre_ptr;
    NullPtr<void (DWriteTypographyHook::*)(UINT32)> _GetFontFeatureCount_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontFeature(UINT32 fontFeatureIndex, _Out_ DWRITE_FONT_FEATURE* fontFeature);
    NullPtr<void (DWriteTypographyHook::*)(UINT32 &, _Out_ DWRITE_FONT_FEATURE* &)> _GetFontFeature_pre_ptr;
    NullPtr<void (DWriteTypographyHook::*)(HRESULT, UINT32, _Out_ DWRITE_FONT_FEATURE*)> _GetFontFeature_post_ptr;

};

// ==============================================================================
// IDWriteNumberSubstitution
// ==============================================================================
class DWriteNumberSubstitutionHook : public HookBase<IDWriteNumberSubstitution>
{

protected:

    DWriteNumberSubstitutionHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteNumberSubstitutionHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteNumberSubstitutionHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteNumberSubstitutionHook * typedPtr = (DWriteNumberSubstitutionHook *)ptr;
        delete typedPtr;
    }

};

// ==============================================================================
// IDWriteTextAnalysisSource
// ==============================================================================
class DWriteTextAnalysisSourceHook : public HookBase<IDWriteTextAnalysisSource>
{

protected:

    DWriteTextAnalysisSourceHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteTextAnalysisSourceHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteTextAnalysisSourceHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteTextAnalysisSourceHook * typedPtr = (DWriteTextAnalysisSourceHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetTextAtPosition(UINT32 textPosition, _Outptr_result_buffer_(*textLength) WCHAR const** textString, _Out_ UINT32* textLength);
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(UINT32 &, _Outptr_result_buffer_(*textLength) WCHAR const** &, _Out_ UINT32* &)> _GetTextAtPosition_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(HRESULT, UINT32, _Outptr_result_buffer_(*textLength) WCHAR const**, _Out_ UINT32*)> _GetTextAtPosition_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetTextBeforePosition(UINT32 textPosition, _Outptr_result_buffer_(*textLength) WCHAR const** textString, _Out_ UINT32* textLength);
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(UINT32 &, _Outptr_result_buffer_(*textLength) WCHAR const** &, _Out_ UINT32* &)> _GetTextBeforePosition_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(HRESULT, UINT32, _Outptr_result_buffer_(*textLength) WCHAR const**, _Out_ UINT32*)> _GetTextBeforePosition_post_ptr;

    // -----------------------------------------------------------------------------
    virtual DWRITE_READING_DIRECTION STDMETHODCALLTYPE GetParagraphReadingDirection();
    NullPtr<void (DWriteTextAnalysisSourceHook::*)()> _GetParagraphReadingDirection_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(DWRITE_READING_DIRECTION)> _GetParagraphReadingDirection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLocaleName(UINT32 textPosition, _Out_ UINT32* textLength, _Outptr_result_z_ WCHAR const** localeName);
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(UINT32 &, _Out_ UINT32* &, _Outptr_result_z_ WCHAR const** &)> _GetLocaleName_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(HRESULT, UINT32, _Out_ UINT32*, _Outptr_result_z_ WCHAR const**)> _GetLocaleName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetNumberSubstitution(UINT32 textPosition, _Out_ UINT32* textLength, _Outptr_ IDWriteNumberSubstitution** numberSubstitution);
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(UINT32 &, _Out_ UINT32* &, _Outptr_ IDWriteNumberSubstitution** &)> _GetNumberSubstitution_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSourceHook::*)(HRESULT, UINT32, _Out_ UINT32*, _Outptr_ IDWriteNumberSubstitution**)> _GetNumberSubstitution_post_ptr;

};

// ==============================================================================
// IDWriteTextAnalysisSink
// ==============================================================================
class DWriteTextAnalysisSinkHook : public HookBase<IDWriteTextAnalysisSink>
{

protected:

    DWriteTextAnalysisSinkHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteTextAnalysisSinkHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteTextAnalysisSinkHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteTextAnalysisSinkHook * typedPtr = (DWriteTextAnalysisSinkHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetScriptAnalysis(UINT32 textPosition, UINT32 textLength, _In_ DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis);
    NullPtr<void (DWriteTextAnalysisSinkHook::*)(UINT32 &, UINT32 &, _In_ DWRITE_SCRIPT_ANALYSIS const* &)> _SetScriptAnalysis_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSinkHook::*)(HRESULT, UINT32, UINT32, _In_ DWRITE_SCRIPT_ANALYSIS const*)> _SetScriptAnalysis_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetLineBreakpoints(UINT32 textPosition, UINT32 textLength, _In_reads_(textLength) DWRITE_LINE_BREAKPOINT const* lineBreakpoints);
    NullPtr<void (DWriteTextAnalysisSinkHook::*)(UINT32 &, UINT32 &, _In_reads_(textLength) DWRITE_LINE_BREAKPOINT const* &)> _SetLineBreakpoints_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSinkHook::*)(HRESULT, UINT32, UINT32, _In_reads_(textLength) DWRITE_LINE_BREAKPOINT const*)> _SetLineBreakpoints_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetBidiLevel(UINT32 textPosition, UINT32 textLength, UINT8 explicitLevel, UINT8 resolvedLevel);
    NullPtr<void (DWriteTextAnalysisSinkHook::*)(UINT32 &, UINT32 &, UINT8 &, UINT8 &)> _SetBidiLevel_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSinkHook::*)(HRESULT, UINT32, UINT32, UINT8, UINT8)> _SetBidiLevel_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetNumberSubstitution(UINT32 textPosition, UINT32 textLength, _In_ IDWriteNumberSubstitution* numberSubstitution);
    NullPtr<void (DWriteTextAnalysisSinkHook::*)(UINT32 &, UINT32 &, _In_ IDWriteNumberSubstitution* &)> _SetNumberSubstitution_pre_ptr;
    NullPtr<void (DWriteTextAnalysisSinkHook::*)(HRESULT, UINT32, UINT32, _In_ IDWriteNumberSubstitution*)> _SetNumberSubstitution_post_ptr;

};

// ==============================================================================
// IDWriteTextAnalyzer
// ==============================================================================
class DWriteTextAnalyzerHook : public HookBase<IDWriteTextAnalyzer>
{

protected:

    DWriteTextAnalyzerHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteTextAnalyzerHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteTextAnalyzerHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteTextAnalyzerHook * typedPtr = (DWriteTextAnalyzerHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AnalyzeScript(IDWriteTextAnalysisSource* analysisSource, UINT32 textPosition, UINT32 textLength, IDWriteTextAnalysisSink* analysisSink);
    NullPtr<void (DWriteTextAnalyzerHook::*)(IDWriteTextAnalysisSource* &, UINT32 &, UINT32 &, IDWriteTextAnalysisSink* &)> _AnalyzeScript_pre_ptr;
    NullPtr<void (DWriteTextAnalyzerHook::*)(HRESULT, IDWriteTextAnalysisSource*, UINT32, UINT32, IDWriteTextAnalysisSink*)> _AnalyzeScript_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AnalyzeBidi(IDWriteTextAnalysisSource* analysisSource, UINT32 textPosition, UINT32 textLength, IDWriteTextAnalysisSink* analysisSink);
    NullPtr<void (DWriteTextAnalyzerHook::*)(IDWriteTextAnalysisSource* &, UINT32 &, UINT32 &, IDWriteTextAnalysisSink* &)> _AnalyzeBidi_pre_ptr;
    NullPtr<void (DWriteTextAnalyzerHook::*)(HRESULT, IDWriteTextAnalysisSource*, UINT32, UINT32, IDWriteTextAnalysisSink*)> _AnalyzeBidi_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AnalyzeNumberSubstitution(IDWriteTextAnalysisSource* analysisSource, UINT32 textPosition, UINT32 textLength, IDWriteTextAnalysisSink* analysisSink);
    NullPtr<void (DWriteTextAnalyzerHook::*)(IDWriteTextAnalysisSource* &, UINT32 &, UINT32 &, IDWriteTextAnalysisSink* &)> _AnalyzeNumberSubstitution_pre_ptr;
    NullPtr<void (DWriteTextAnalyzerHook::*)(HRESULT, IDWriteTextAnalysisSource*, UINT32, UINT32, IDWriteTextAnalysisSink*)> _AnalyzeNumberSubstitution_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE AnalyzeLineBreakpoints(IDWriteTextAnalysisSource* analysisSource, UINT32 textPosition, UINT32 textLength, IDWriteTextAnalysisSink* analysisSink);
    NullPtr<void (DWriteTextAnalyzerHook::*)(IDWriteTextAnalysisSource* &, UINT32 &, UINT32 &, IDWriteTextAnalysisSink* &)> _AnalyzeLineBreakpoints_pre_ptr;
    NullPtr<void (DWriteTextAnalyzerHook::*)(HRESULT, IDWriteTextAnalysisSource*, UINT32, UINT32, IDWriteTextAnalysisSink*)> _AnalyzeLineBreakpoints_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGlyphs(_In_reads_(textLength) WCHAR const* textString, UINT32 textLength, IDWriteFontFace* fontFace, BOOL isSideways, BOOL isRightToLeft, _In_ DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis, _In_opt_z_ WCHAR const* localeName, _In_opt_ IDWriteNumberSubstitution* numberSubstitution, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features, _In_reads_opt_(featureRanges) UINT32 const* featureRangeLengths, UINT32 featureRanges, UINT32 maxGlyphCount, _Out_writes_(textLength) UINT16* clusterMap, _Out_writes_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps, _Out_writes_(maxGlyphCount) UINT16* glyphIndices, _Out_writes_(maxGlyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES* glyphProps, _Out_ UINT32* actualGlyphCount);
    NullPtr<void (DWriteTextAnalyzerHook::*)(_In_reads_(textLength) WCHAR const* &, UINT32 &, IDWriteFontFace* &, BOOL &, BOOL &, _In_ DWRITE_SCRIPT_ANALYSIS const* &, _In_opt_z_ WCHAR const* &, _In_opt_ IDWriteNumberSubstitution* &, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** &, _In_reads_opt_(featureRanges) UINT32 const* &, UINT32 &, UINT32 &, _Out_writes_(textLength) UINT16* &, _Out_writes_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* &, _Out_writes_(maxGlyphCount) UINT16* &, _Out_writes_(maxGlyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES* &, _Out_ UINT32* &)> _GetGlyphs_pre_ptr;
    NullPtr<void (DWriteTextAnalyzerHook::*)(HRESULT, _In_reads_(textLength) WCHAR const*, UINT32, IDWriteFontFace*, BOOL, BOOL, _In_ DWRITE_SCRIPT_ANALYSIS const*, _In_opt_z_ WCHAR const*, _In_opt_ IDWriteNumberSubstitution*, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const**, _In_reads_opt_(featureRanges) UINT32 const*, UINT32, UINT32, _Out_writes_(textLength) UINT16*, _Out_writes_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES*, _Out_writes_(maxGlyphCount) UINT16*, _Out_writes_(maxGlyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES*, _Out_ UINT32*)> _GetGlyphs_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGlyphPlacements(_In_reads_(textLength) WCHAR const* textString, _In_reads_(textLength) UINT16 const* clusterMap, _In_reads_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps, UINT32 textLength, _In_reads_(glyphCount) UINT16 const* glyphIndices, _In_reads_(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const* glyphProps, UINT32 glyphCount, IDWriteFontFace * fontFace, FLOAT fontEmSize, BOOL isSideways, BOOL isRightToLeft, _In_ DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis, _In_opt_z_ WCHAR const* localeName, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features, _In_reads_opt_(featureRanges) UINT32 const* featureRangeLengths, UINT32 featureRanges, _Out_writes_(glyphCount) FLOAT* glyphAdvances, _Out_writes_(glyphCount) DWRITE_GLYPH_OFFSET* glyphOffsets);
    NullPtr<void (DWriteTextAnalyzerHook::*)(_In_reads_(textLength) WCHAR const* &, _In_reads_(textLength) UINT16 const* &, _In_reads_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* &, UINT32 &, _In_reads_(glyphCount) UINT16 const* &, _In_reads_(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const* &, UINT32 &, IDWriteFontFace * &, FLOAT &, BOOL &, BOOL &, _In_ DWRITE_SCRIPT_ANALYSIS const* &, _In_opt_z_ WCHAR const* &, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** &, _In_reads_opt_(featureRanges) UINT32 const* &, UINT32 &, _Out_writes_(glyphCount) FLOAT* &, _Out_writes_(glyphCount) DWRITE_GLYPH_OFFSET* &)> _GetGlyphPlacements_pre_ptr;
    NullPtr<void (DWriteTextAnalyzerHook::*)(HRESULT, _In_reads_(textLength) WCHAR const*, _In_reads_(textLength) UINT16 const*, _In_reads_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES*, UINT32, _In_reads_(glyphCount) UINT16 const*, _In_reads_(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const*, UINT32, IDWriteFontFace *, FLOAT, BOOL, BOOL, _In_ DWRITE_SCRIPT_ANALYSIS const*, _In_opt_z_ WCHAR const*, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const**, _In_reads_opt_(featureRanges) UINT32 const*, UINT32, _Out_writes_(glyphCount) FLOAT*, _Out_writes_(glyphCount) DWRITE_GLYPH_OFFSET*)> _GetGlyphPlacements_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGdiCompatibleGlyphPlacements(_In_reads_(textLength) WCHAR const* textString, _In_reads_(textLength) UINT16 const* clusterMap, _In_reads_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps, UINT32 textLength, _In_reads_(glyphCount) UINT16 const* glyphIndices, _In_reads_(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const* glyphProps, UINT32 glyphCount, IDWriteFontFace * fontFace, FLOAT fontEmSize, FLOAT pixelsPerDip, _In_opt_ DWRITE_MATRIX const* transform, BOOL useGdiNatural, BOOL isSideways, BOOL isRightToLeft, _In_ DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis, _In_opt_z_ WCHAR const* localeName, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features, _In_reads_opt_(featureRanges) UINT32 const* featureRangeLengths, UINT32 featureRanges, _Out_writes_(glyphCount) FLOAT* glyphAdvances, _Out_writes_(glyphCount) DWRITE_GLYPH_OFFSET* glyphOffsets);
    NullPtr<void (DWriteTextAnalyzerHook::*)(_In_reads_(textLength) WCHAR const* &, _In_reads_(textLength) UINT16 const* &, _In_reads_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* &, UINT32 &, _In_reads_(glyphCount) UINT16 const* &, _In_reads_(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const* &, UINT32 &, IDWriteFontFace * &, FLOAT &, FLOAT &, _In_opt_ DWRITE_MATRIX const* &, BOOL &, BOOL &, BOOL &, _In_ DWRITE_SCRIPT_ANALYSIS const* &, _In_opt_z_ WCHAR const* &, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** &, _In_reads_opt_(featureRanges) UINT32 const* &, UINT32 &, _Out_writes_(glyphCount) FLOAT* &, _Out_writes_(glyphCount) DWRITE_GLYPH_OFFSET* &)> _GetGdiCompatibleGlyphPlacements_pre_ptr;
    NullPtr<void (DWriteTextAnalyzerHook::*)(HRESULT, _In_reads_(textLength) WCHAR const*, _In_reads_(textLength) UINT16 const*, _In_reads_(textLength) DWRITE_SHAPING_TEXT_PROPERTIES*, UINT32, _In_reads_(glyphCount) UINT16 const*, _In_reads_(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const*, UINT32, IDWriteFontFace *, FLOAT, FLOAT, _In_opt_ DWRITE_MATRIX const*, BOOL, BOOL, BOOL, _In_ DWRITE_SCRIPT_ANALYSIS const*, _In_opt_z_ WCHAR const*, _In_reads_opt_(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const**, _In_reads_opt_(featureRanges) UINT32 const*, UINT32, _Out_writes_(glyphCount) FLOAT*, _Out_writes_(glyphCount) DWRITE_GLYPH_OFFSET*)> _GetGdiCompatibleGlyphPlacements_post_ptr;

};

// ==============================================================================
// IDWriteInlineObject
// ==============================================================================
class DWriteInlineObjectHook : public HookBase<IDWriteInlineObject>
{

protected:

    DWriteInlineObjectHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteInlineObjectHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteInlineObjectHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteInlineObjectHook * typedPtr = (DWriteInlineObjectHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Draw(_In_opt_ void* clientDrawingContext, IDWriteTextRenderer* renderer, FLOAT originX, FLOAT originY, BOOL isSideways, BOOL isRightToLeft, _In_opt_ IUnknown* clientDrawingEffect);
    NullPtr<void (DWriteInlineObjectHook::*)(_In_opt_ void* &, IDWriteTextRenderer* &, FLOAT &, FLOAT &, BOOL &, BOOL &, _In_opt_ IUnknown* &)> _Draw_pre_ptr;
    NullPtr<void (DWriteInlineObjectHook::*)(HRESULT, _In_opt_ void*, IDWriteTextRenderer*, FLOAT, FLOAT, BOOL, BOOL, _In_opt_ IUnknown*)> _Draw_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetMetrics(_Out_ DWRITE_INLINE_OBJECT_METRICS* metrics);
    NullPtr<void (DWriteInlineObjectHook::*)(_Out_ DWRITE_INLINE_OBJECT_METRICS* &)> _GetMetrics_pre_ptr;
    NullPtr<void (DWriteInlineObjectHook::*)(HRESULT, _Out_ DWRITE_INLINE_OBJECT_METRICS*)> _GetMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetOverhangMetrics(_Out_ DWRITE_OVERHANG_METRICS* overhangs);
    NullPtr<void (DWriteInlineObjectHook::*)(_Out_ DWRITE_OVERHANG_METRICS* &)> _GetOverhangMetrics_pre_ptr;
    NullPtr<void (DWriteInlineObjectHook::*)(HRESULT, _Out_ DWRITE_OVERHANG_METRICS*)> _GetOverhangMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetBreakConditions(_Out_ DWRITE_BREAK_CONDITION* breakConditionBefore, _Out_ DWRITE_BREAK_CONDITION* breakConditionAfter);
    NullPtr<void (DWriteInlineObjectHook::*)(_Out_ DWRITE_BREAK_CONDITION* &, _Out_ DWRITE_BREAK_CONDITION* &)> _GetBreakConditions_pre_ptr;
    NullPtr<void (DWriteInlineObjectHook::*)(HRESULT, _Out_ DWRITE_BREAK_CONDITION*, _Out_ DWRITE_BREAK_CONDITION*)> _GetBreakConditions_post_ptr;

};

// ==============================================================================
// IDWritePixelSnapping
// ==============================================================================
class DWritePixelSnappingHook : public HookBase<IDWritePixelSnapping>
{

protected:

    DWritePixelSnappingHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWritePixelSnappingHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWritePixelSnappingHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWritePixelSnappingHook * typedPtr = (DWritePixelSnappingHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE IsPixelSnappingDisabled(_In_opt_ void* clientDrawingContext, _Out_ BOOL* isDisabled);
    NullPtr<void (DWritePixelSnappingHook::*)(_In_opt_ void* &, _Out_ BOOL* &)> _IsPixelSnappingDisabled_pre_ptr;
    NullPtr<void (DWritePixelSnappingHook::*)(HRESULT, _In_opt_ void*, _Out_ BOOL*)> _IsPixelSnappingDisabled_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCurrentTransform(_In_opt_ void* clientDrawingContext, _Out_ DWRITE_MATRIX* transform);
    NullPtr<void (DWritePixelSnappingHook::*)(_In_opt_ void* &, _Out_ DWRITE_MATRIX* &)> _GetCurrentTransform_pre_ptr;
    NullPtr<void (DWritePixelSnappingHook::*)(HRESULT, _In_opt_ void*, _Out_ DWRITE_MATRIX*)> _GetCurrentTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetPixelsPerDip(_In_opt_ void* clientDrawingContext, _Out_ FLOAT* pixelsPerDip);
    NullPtr<void (DWritePixelSnappingHook::*)(_In_opt_ void* &, _Out_ FLOAT* &)> _GetPixelsPerDip_pre_ptr;
    NullPtr<void (DWritePixelSnappingHook::*)(HRESULT, _In_opt_ void*, _Out_ FLOAT*)> _GetPixelsPerDip_post_ptr;

};

// ==============================================================================
// IDWriteTextRenderer
// ==============================================================================
class DWriteTextRendererHook : public HookBase<IDWriteTextRenderer>
{
    DWritePixelSnappingHook & _DWritePixelSnapping;

protected:

    DWriteTextRendererHook(UnknownBase & unknown, DWritePixelSnappingHook & DWritePixelSnapping, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DWritePixelSnapping(DWritePixelSnapping)
    {
    }

    ~DWriteTextRendererHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DWritePixelSnappingHook * DWritePixelSnapping = (DWritePixelSnappingHook *)unknown.GetHookedObj(__uuidof(IDWritePixelSnapping));
        if (nullptr == DWritePixelSnapping) return nullptr;

        try
        {
            return new DWriteTextRendererHook(unknown, *DWritePixelSnapping, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteTextRendererHook * typedPtr = (DWriteTextRendererHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE IsPixelSnappingDisabled(_In_opt_ void* clientDrawingContext, _Out_ BOOL* isDisabled)
    {
        return _DWritePixelSnapping.IsPixelSnappingDisabled(clientDrawingContext, isDisabled);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetCurrentTransform(_In_opt_ void* clientDrawingContext, _Out_ DWRITE_MATRIX* transform)
    {
        return _DWritePixelSnapping.GetCurrentTransform(clientDrawingContext, transform);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetPixelsPerDip(_In_opt_ void* clientDrawingContext, _Out_ FLOAT* pixelsPerDip)
    {
        return _DWritePixelSnapping.GetPixelsPerDip(clientDrawingContext, pixelsPerDip);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawGlyphRun(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, _In_ DWRITE_GLYPH_RUN const* glyphRun, _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, _In_opt_ IUnknown* clientDrawingEffect);
    NullPtr<void (DWriteTextRendererHook::*)(_In_opt_ void* &, FLOAT &, FLOAT &, DWRITE_MEASURING_MODE &, _In_ DWRITE_GLYPH_RUN const* &, _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* &, _In_opt_ IUnknown* &)> _DrawGlyphRun_pre_ptr;
    NullPtr<void (DWriteTextRendererHook::*)(HRESULT, _In_opt_ void*, FLOAT, FLOAT, DWRITE_MEASURING_MODE, _In_ DWRITE_GLYPH_RUN const*, _In_ DWRITE_GLYPH_RUN_DESCRIPTION const*, _In_opt_ IUnknown*)> _DrawGlyphRun_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawUnderline(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, _In_ DWRITE_UNDERLINE const* underline, _In_opt_ IUnknown* clientDrawingEffect);
    NullPtr<void (DWriteTextRendererHook::*)(_In_opt_ void* &, FLOAT &, FLOAT &, _In_ DWRITE_UNDERLINE const* &, _In_opt_ IUnknown* &)> _DrawUnderline_pre_ptr;
    NullPtr<void (DWriteTextRendererHook::*)(HRESULT, _In_opt_ void*, FLOAT, FLOAT, _In_ DWRITE_UNDERLINE const*, _In_opt_ IUnknown*)> _DrawUnderline_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawStrikethrough(_In_opt_ void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, _In_ DWRITE_STRIKETHROUGH const* strikethrough, _In_opt_ IUnknown* clientDrawingEffect);
    NullPtr<void (DWriteTextRendererHook::*)(_In_opt_ void* &, FLOAT &, FLOAT &, _In_ DWRITE_STRIKETHROUGH const* &, _In_opt_ IUnknown* &)> _DrawStrikethrough_pre_ptr;
    NullPtr<void (DWriteTextRendererHook::*)(HRESULT, _In_opt_ void*, FLOAT, FLOAT, _In_ DWRITE_STRIKETHROUGH const*, _In_opt_ IUnknown*)> _DrawStrikethrough_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawInlineObject(_In_opt_ void* clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, _In_opt_ IUnknown* clientDrawingEffect);
    NullPtr<void (DWriteTextRendererHook::*)(_In_opt_ void* &, FLOAT &, FLOAT &, IDWriteInlineObject* &, BOOL &, BOOL &, _In_opt_ IUnknown* &)> _DrawInlineObject_pre_ptr;
    NullPtr<void (DWriteTextRendererHook::*)(HRESULT, _In_opt_ void*, FLOAT, FLOAT, IDWriteInlineObject*, BOOL, BOOL, _In_opt_ IUnknown*)> _DrawInlineObject_post_ptr;

};

// ==============================================================================
// IDWriteTextLayout
// ==============================================================================
class DWriteTextLayoutHook : public HookBase<IDWriteTextLayout>
{
    DWriteTextFormatHook & _DWriteTextFormat;

protected:

    DWriteTextLayoutHook(UnknownBase & unknown, DWriteTextFormatHook & DWriteTextFormat, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
        , _DWriteTextFormat(DWriteTextFormat)
    {
    }

    ~DWriteTextLayoutHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteTextFormatHook * DWriteTextFormat = (DWriteTextFormatHook *)unknown.GetHookedObj(__uuidof(IDWriteTextFormat));
        if (nullptr == DWriteTextFormat) return nullptr;

        try
        {
            return new DWriteTextLayoutHook(unknown, *DWriteTextFormat, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteTextLayoutHook * typedPtr = (DWriteTextLayoutHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment)
    {
        return _DWriteTextFormat.SetTextAlignment(textAlignment);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment)
    {
        return _DWriteTextFormat.SetParagraphAlignment(paragraphAlignment);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetWordWrapping(DWRITE_WORD_WRAPPING wordWrapping)
    {
        return _DWriteTextFormat.SetWordWrapping(wordWrapping);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetReadingDirection(DWRITE_READING_DIRECTION readingDirection)
    {
        return _DWriteTextFormat.SetReadingDirection(readingDirection);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetFlowDirection(DWRITE_FLOW_DIRECTION flowDirection)
    {
        return _DWriteTextFormat.SetFlowDirection(flowDirection);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetIncrementalTabStop(FLOAT incrementalTabStop)
    {
        return _DWriteTextFormat.SetIncrementalTabStop(incrementalTabStop);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetTrimming(_In_ DWRITE_TRIMMING const* trimmingOptions, IDWriteInlineObject* trimmingSign)
    {
        return _DWriteTextFormat.SetTrimming(trimmingOptions, trimmingSign);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE SetLineSpacing(DWRITE_LINE_SPACING_METHOD lineSpacingMethod, FLOAT lineSpacing, FLOAT baseline)
    {
        return _DWriteTextFormat.SetLineSpacing(lineSpacingMethod, lineSpacing, baseline);
    }

    // -----------------------------------------------------------------------------
    DWRITE_TEXT_ALIGNMENT STDMETHODCALLTYPE GetTextAlignment()
    {
        return _DWriteTextFormat.GetTextAlignment();
    }

    // -----------------------------------------------------------------------------
    DWRITE_PARAGRAPH_ALIGNMENT STDMETHODCALLTYPE GetParagraphAlignment()
    {
        return _DWriteTextFormat.GetParagraphAlignment();
    }

    // -----------------------------------------------------------------------------
    DWRITE_WORD_WRAPPING STDMETHODCALLTYPE GetWordWrapping()
    {
        return _DWriteTextFormat.GetWordWrapping();
    }

    // -----------------------------------------------------------------------------
    DWRITE_READING_DIRECTION STDMETHODCALLTYPE GetReadingDirection()
    {
        return _DWriteTextFormat.GetReadingDirection();
    }

    // -----------------------------------------------------------------------------
    DWRITE_FLOW_DIRECTION STDMETHODCALLTYPE GetFlowDirection()
    {
        return _DWriteTextFormat.GetFlowDirection();
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetIncrementalTabStop()
    {
        return _DWriteTextFormat.GetIncrementalTabStop();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetTrimming(_Out_ DWRITE_TRIMMING* trimmingOptions, _Out_ IDWriteInlineObject** trimmingSign)
    {
        return _DWriteTextFormat.GetTrimming(trimmingOptions, trimmingSign);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetLineSpacing(_Out_ DWRITE_LINE_SPACING_METHOD* lineSpacingMethod, _Out_ FLOAT* lineSpacing, _Out_ FLOAT* baseline)
    {
        return _DWriteTextFormat.GetLineSpacing(lineSpacingMethod, lineSpacing, baseline);
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFontCollection(_Out_ IDWriteFontCollection** fontCollection)
    {
        return _DWriteTextFormat.GetFontCollection(fontCollection);
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetFontFamilyNameLength()
    {
        return _DWriteTextFormat.GetFontFamilyNameLength();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetFontFamilyName(_Out_writes_z_(nameSize) WCHAR* fontFamilyName, UINT32 nameSize)
    {
        return _DWriteTextFormat.GetFontFamilyName(fontFamilyName, nameSize);
    }

    // -----------------------------------------------------------------------------
    DWRITE_FONT_WEIGHT STDMETHODCALLTYPE GetFontWeight()
    {
        return _DWriteTextFormat.GetFontWeight();
    }

    // -----------------------------------------------------------------------------
    DWRITE_FONT_STYLE STDMETHODCALLTYPE GetFontStyle()
    {
        return _DWriteTextFormat.GetFontStyle();
    }

    // -----------------------------------------------------------------------------
    DWRITE_FONT_STRETCH STDMETHODCALLTYPE GetFontStretch()
    {
        return _DWriteTextFormat.GetFontStretch();
    }

    // -----------------------------------------------------------------------------
    FLOAT STDMETHODCALLTYPE GetFontSize()
    {
        return _DWriteTextFormat.GetFontSize();
    }

    // -----------------------------------------------------------------------------
    UINT32 STDMETHODCALLTYPE GetLocaleNameLength()
    {
        return _DWriteTextFormat.GetLocaleNameLength();
    }

    // -----------------------------------------------------------------------------
    HRESULT STDMETHODCALLTYPE GetLocaleName(_Out_writes_z_(nameSize) WCHAR* localeName, UINT32 nameSize)
    {
        return _DWriteTextFormat.GetLocaleName(localeName, nameSize);
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetMaxWidth(FLOAT maxWidth);
    NullPtr<void (DWriteTextLayoutHook::*)(FLOAT &)> _SetMaxWidth_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, FLOAT)> _SetMaxWidth_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetMaxHeight(FLOAT maxHeight);
    NullPtr<void (DWriteTextLayoutHook::*)(FLOAT &)> _SetMaxHeight_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, FLOAT)> _SetMaxHeight_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFontCollection(IDWriteFontCollection* fontCollection, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(IDWriteFontCollection* &, DWRITE_TEXT_RANGE &)> _SetFontCollection_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, IDWriteFontCollection*, DWRITE_TEXT_RANGE)> _SetFontCollection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFontFamilyName(_In_z_ WCHAR const* fontFamilyName, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(_In_z_ WCHAR const* &, DWRITE_TEXT_RANGE &)> _SetFontFamilyName_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, _In_z_ WCHAR const*, DWRITE_TEXT_RANGE)> _SetFontFamilyName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFontWeight(DWRITE_FONT_WEIGHT fontWeight, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(DWRITE_FONT_WEIGHT &, DWRITE_TEXT_RANGE &)> _SetFontWeight_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, DWRITE_FONT_WEIGHT, DWRITE_TEXT_RANGE)> _SetFontWeight_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFontStyle(DWRITE_FONT_STYLE fontStyle, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(DWRITE_FONT_STYLE &, DWRITE_TEXT_RANGE &)> _SetFontStyle_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, DWRITE_FONT_STYLE, DWRITE_TEXT_RANGE)> _SetFontStyle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFontStretch(DWRITE_FONT_STRETCH fontStretch, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(DWRITE_FONT_STRETCH &, DWRITE_TEXT_RANGE &)> _SetFontStretch_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, DWRITE_FONT_STRETCH, DWRITE_TEXT_RANGE)> _SetFontStretch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetFontSize(FLOAT fontSize, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(FLOAT &, DWRITE_TEXT_RANGE &)> _SetFontSize_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, FLOAT, DWRITE_TEXT_RANGE)> _SetFontSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetUnderline(BOOL hasUnderline, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(BOOL &, DWRITE_TEXT_RANGE &)> _SetUnderline_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, BOOL, DWRITE_TEXT_RANGE)> _SetUnderline_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetStrikethrough(BOOL hasStrikethrough, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(BOOL &, DWRITE_TEXT_RANGE &)> _SetStrikethrough_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, BOOL, DWRITE_TEXT_RANGE)> _SetStrikethrough_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetDrawingEffect(IUnknown* drawingEffect, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(IUnknown* &, DWRITE_TEXT_RANGE &)> _SetDrawingEffect_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, IUnknown*, DWRITE_TEXT_RANGE)> _SetDrawingEffect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetInlineObject(IDWriteInlineObject* inlineObject, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(IDWriteInlineObject* &, DWRITE_TEXT_RANGE &)> _SetInlineObject_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, IDWriteInlineObject*, DWRITE_TEXT_RANGE)> _SetInlineObject_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetTypography(IDWriteTypography* typography, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(IDWriteTypography* &, DWRITE_TEXT_RANGE &)> _SetTypography_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, IDWriteTypography*, DWRITE_TEXT_RANGE)> _SetTypography_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetLocaleName(_In_z_ WCHAR const* localeName, DWRITE_TEXT_RANGE textRange);
    NullPtr<void (DWriteTextLayoutHook::*)(_In_z_ WCHAR const* &, DWRITE_TEXT_RANGE &)> _SetLocaleName_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, _In_z_ WCHAR const*, DWRITE_TEXT_RANGE)> _SetLocaleName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetMaxWidth();
    NullPtr<void (DWriteTextLayoutHook::*)()> _GetMaxWidth_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(FLOAT)> _GetMaxWidth_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetMaxHeight();
    NullPtr<void (DWriteTextLayoutHook::*)()> _GetMaxHeight_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(FLOAT)> _GetMaxHeight_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontCollection(UINT32 currentPosition, _Out_ IDWriteFontCollection** fontCollection, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ IDWriteFontCollection** &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetFontCollection_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ IDWriteFontCollection**, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetFontCollection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontFamilyNameLength(UINT32 currentPosition, _Out_ UINT32* nameLength, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ UINT32* &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetFontFamilyNameLength_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ UINT32*, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetFontFamilyNameLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontFamilyName(UINT32 currentPosition, _Out_writes_z_(nameSize) WCHAR* fontFamilyName, UINT32 nameSize, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_writes_z_(nameSize) WCHAR* &, UINT32 &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetFontFamilyName_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_writes_z_(nameSize) WCHAR*, UINT32, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetFontFamilyName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontWeight(UINT32 currentPosition, _Out_ DWRITE_FONT_WEIGHT* fontWeight, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ DWRITE_FONT_WEIGHT* &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetFontWeight_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ DWRITE_FONT_WEIGHT*, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetFontWeight_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontStyle(UINT32 currentPosition, _Out_ DWRITE_FONT_STYLE* fontStyle, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ DWRITE_FONT_STYLE* &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetFontStyle_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ DWRITE_FONT_STYLE*, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetFontStyle_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontStretch(UINT32 currentPosition, _Out_ DWRITE_FONT_STRETCH* fontStretch, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ DWRITE_FONT_STRETCH* &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetFontStretch_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ DWRITE_FONT_STRETCH*, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetFontStretch_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetFontSize(UINT32 currentPosition, _Out_ FLOAT* fontSize, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ FLOAT* &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetFontSize_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ FLOAT*, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetFontSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetUnderline(UINT32 currentPosition, _Out_ BOOL* hasUnderline, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ BOOL* &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetUnderline_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ BOOL*, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetUnderline_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetStrikethrough(UINT32 currentPosition, _Out_ BOOL* hasStrikethrough, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ BOOL* &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetStrikethrough_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ BOOL*, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetStrikethrough_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetDrawingEffect(UINT32 currentPosition, _Out_ IUnknown** drawingEffect, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ IUnknown** &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetDrawingEffect_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ IUnknown**, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetDrawingEffect_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetInlineObject(UINT32 currentPosition, _Out_ IDWriteInlineObject** inlineObject, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ IDWriteInlineObject** &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetInlineObject_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ IDWriteInlineObject**, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetInlineObject_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetTypography(UINT32 currentPosition, _Out_ IDWriteTypography** typography, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ IDWriteTypography** &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetTypography_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ IDWriteTypography**, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetTypography_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLocaleNameLength(UINT32 currentPosition, _Out_ UINT32* nameLength, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_ UINT32* &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetLocaleNameLength_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_ UINT32*, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetLocaleNameLength_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLocaleName(UINT32 currentPosition, _Out_writes_z_(nameSize) WCHAR* localeName, UINT32 nameSize, _Out_opt_ DWRITE_TEXT_RANGE* textRange = NULL);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, _Out_writes_z_(nameSize) WCHAR* &, UINT32 &, _Out_opt_ DWRITE_TEXT_RANGE* &)> _GetLocaleName_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, _Out_writes_z_(nameSize) WCHAR*, UINT32, _Out_opt_ DWRITE_TEXT_RANGE*)> _GetLocaleName_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Draw(_In_opt_ void* clientDrawingContext, IDWriteTextRenderer* renderer, FLOAT originX, FLOAT originY);
    NullPtr<void (DWriteTextLayoutHook::*)(_In_opt_ void* &, IDWriteTextRenderer* &, FLOAT &, FLOAT &)> _Draw_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, _In_opt_ void*, IDWriteTextRenderer*, FLOAT, FLOAT)> _Draw_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetLineMetrics(_Out_writes_opt_(maxLineCount) DWRITE_LINE_METRICS* lineMetrics, UINT32 maxLineCount, _Out_ UINT32* actualLineCount);
    NullPtr<void (DWriteTextLayoutHook::*)(_Out_writes_opt_(maxLineCount) DWRITE_LINE_METRICS* &, UINT32 &, _Out_ UINT32* &)> _GetLineMetrics_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, _Out_writes_opt_(maxLineCount) DWRITE_LINE_METRICS*, UINT32, _Out_ UINT32*)> _GetLineMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetMetrics(_Out_ DWRITE_TEXT_METRICS* textMetrics);
    NullPtr<void (DWriteTextLayoutHook::*)(_Out_ DWRITE_TEXT_METRICS* &)> _GetMetrics_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, _Out_ DWRITE_TEXT_METRICS*)> _GetMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetOverhangMetrics(_Out_ DWRITE_OVERHANG_METRICS* overhangs);
    NullPtr<void (DWriteTextLayoutHook::*)(_Out_ DWRITE_OVERHANG_METRICS* &)> _GetOverhangMetrics_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, _Out_ DWRITE_OVERHANG_METRICS*)> _GetOverhangMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetClusterMetrics(_Out_writes_opt_(maxClusterCount) DWRITE_CLUSTER_METRICS* clusterMetrics, UINT32 maxClusterCount, _Out_ UINT32* actualClusterCount);
    NullPtr<void (DWriteTextLayoutHook::*)(_Out_writes_opt_(maxClusterCount) DWRITE_CLUSTER_METRICS* &, UINT32 &, _Out_ UINT32* &)> _GetClusterMetrics_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, _Out_writes_opt_(maxClusterCount) DWRITE_CLUSTER_METRICS*, UINT32, _Out_ UINT32*)> _GetClusterMetrics_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DetermineMinWidth(_Out_ FLOAT* minWidth);
    NullPtr<void (DWriteTextLayoutHook::*)(_Out_ FLOAT* &)> _DetermineMinWidth_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, _Out_ FLOAT*)> _DetermineMinWidth_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE HitTestPoint(FLOAT pointX, FLOAT pointY, _Out_ BOOL* isTrailingHit, _Out_ BOOL* isInside, _Out_ DWRITE_HIT_TEST_METRICS* hitTestMetrics);
    NullPtr<void (DWriteTextLayoutHook::*)(FLOAT &, FLOAT &, _Out_ BOOL* &, _Out_ BOOL* &, _Out_ DWRITE_HIT_TEST_METRICS* &)> _HitTestPoint_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, FLOAT, FLOAT, _Out_ BOOL*, _Out_ BOOL*, _Out_ DWRITE_HIT_TEST_METRICS*)> _HitTestPoint_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE HitTestTextPosition(UINT32 textPosition, BOOL isTrailingHit, _Out_ FLOAT* pointX, _Out_ FLOAT* pointY, _Out_ DWRITE_HIT_TEST_METRICS* hitTestMetrics);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, BOOL &, _Out_ FLOAT* &, _Out_ FLOAT* &, _Out_ DWRITE_HIT_TEST_METRICS* &)> _HitTestTextPosition_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, BOOL, _Out_ FLOAT*, _Out_ FLOAT*, _Out_ DWRITE_HIT_TEST_METRICS*)> _HitTestTextPosition_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE HitTestTextRange(UINT32 textPosition, UINT32 textLength, FLOAT originX, FLOAT originY, _Out_writes_opt_(maxHitTestMetricsCount) DWRITE_HIT_TEST_METRICS* hitTestMetrics, UINT32 maxHitTestMetricsCount, _Out_ UINT32* actualHitTestMetricsCount);
    NullPtr<void (DWriteTextLayoutHook::*)(UINT32 &, UINT32 &, FLOAT &, FLOAT &, _Out_writes_opt_(maxHitTestMetricsCount) DWRITE_HIT_TEST_METRICS* &, UINT32 &, _Out_ UINT32* &)> _HitTestTextRange_pre_ptr;
    NullPtr<void (DWriteTextLayoutHook::*)(HRESULT, UINT32, UINT32, FLOAT, FLOAT, _Out_writes_opt_(maxHitTestMetricsCount) DWRITE_HIT_TEST_METRICS*, UINT32, _Out_ UINT32*)> _HitTestTextRange_post_ptr;

};

// ==============================================================================
// IDWriteBitmapRenderTarget
// ==============================================================================
class DWriteBitmapRenderTargetHook : public HookBase<IDWriteBitmapRenderTarget>
{

protected:

    DWriteBitmapRenderTargetHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteBitmapRenderTargetHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteBitmapRenderTargetHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteBitmapRenderTargetHook * typedPtr = (DWriteBitmapRenderTargetHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE DrawGlyphRun(FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, _In_ DWRITE_GLYPH_RUN const* glyphRun, IDWriteRenderingParams* renderingParams, COLORREF textColor, _Out_opt_ RECT* blackBoxRect = NULL);
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(FLOAT &, FLOAT &, DWRITE_MEASURING_MODE &, _In_ DWRITE_GLYPH_RUN const* &, IDWriteRenderingParams* &, COLORREF &, _Out_opt_ RECT* &)> _DrawGlyphRun_pre_ptr;
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(HRESULT, FLOAT, FLOAT, DWRITE_MEASURING_MODE, _In_ DWRITE_GLYPH_RUN const*, IDWriteRenderingParams*, COLORREF, _Out_opt_ RECT*)> _DrawGlyphRun_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HDC STDMETHODCALLTYPE GetMemoryDC();
    NullPtr<void (DWriteBitmapRenderTargetHook::*)()> _GetMemoryDC_pre_ptr;
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(HDC)> _GetMemoryDC_post_ptr;

    // -----------------------------------------------------------------------------
    virtual FLOAT STDMETHODCALLTYPE GetPixelsPerDip();
    NullPtr<void (DWriteBitmapRenderTargetHook::*)()> _GetPixelsPerDip_pre_ptr;
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(FLOAT)> _GetPixelsPerDip_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetPixelsPerDip(FLOAT pixelsPerDip);
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(FLOAT &)> _SetPixelsPerDip_pre_ptr;
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(HRESULT, FLOAT)> _SetPixelsPerDip_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetCurrentTransform(_Out_ DWRITE_MATRIX* transform);
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(_Out_ DWRITE_MATRIX* &)> _GetCurrentTransform_pre_ptr;
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(HRESULT, _Out_ DWRITE_MATRIX*)> _GetCurrentTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE SetCurrentTransform(_In_opt_ DWRITE_MATRIX const* transform);
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(_In_opt_ DWRITE_MATRIX const* &)> _SetCurrentTransform_pre_ptr;
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(HRESULT, _In_opt_ DWRITE_MATRIX const*)> _SetCurrentTransform_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSize(_Out_ SIZE* size);
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(_Out_ SIZE* &)> _GetSize_pre_ptr;
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(HRESULT, _Out_ SIZE*)> _GetSize_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE Resize(UINT32 width, UINT32 height);
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(UINT32 &, UINT32 &)> _Resize_pre_ptr;
    NullPtr<void (DWriteBitmapRenderTargetHook::*)(HRESULT, UINT32, UINT32)> _Resize_post_ptr;

};

// ==============================================================================
// IDWriteGdiInterop
// ==============================================================================
class DWriteGdiInteropHook : public HookBase<IDWriteGdiInterop>
{

protected:

    DWriteGdiInteropHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteGdiInteropHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteGdiInteropHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteGdiInteropHook * typedPtr = (DWriteGdiInteropHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateFontFromLOGFONT(_In_ LOGFONTW const* logFont, _Out_ IDWriteFont** font);
    NullPtr<void (DWriteGdiInteropHook::*)(_In_ LOGFONTW const* &, _Out_ IDWriteFont** &)> _CreateFontFromLOGFONT_pre_ptr;
    NullPtr<void (DWriteGdiInteropHook::*)(HRESULT, _In_ LOGFONTW const*, _Out_ IDWriteFont**)> _CreateFontFromLOGFONT_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ConvertFontToLOGFONT(IDWriteFont* font, _Out_ LOGFONTW* logFont, _Out_ BOOL* isSystemFont);
    NullPtr<void (DWriteGdiInteropHook::*)(IDWriteFont* &, _Out_ LOGFONTW* &, _Out_ BOOL* &)> _ConvertFontToLOGFONT_pre_ptr;
    NullPtr<void (DWriteGdiInteropHook::*)(HRESULT, IDWriteFont*, _Out_ LOGFONTW*, _Out_ BOOL*)> _ConvertFontToLOGFONT_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE ConvertFontFaceToLOGFONT(IDWriteFontFace* font, _Out_ LOGFONTW* logFont);
    NullPtr<void (DWriteGdiInteropHook::*)(IDWriteFontFace* &, _Out_ LOGFONTW* &)> _ConvertFontFaceToLOGFONT_pre_ptr;
    NullPtr<void (DWriteGdiInteropHook::*)(HRESULT, IDWriteFontFace*, _Out_ LOGFONTW*)> _ConvertFontFaceToLOGFONT_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateFontFaceFromHdc(HDC hdc, _Out_ IDWriteFontFace** fontFace);
    NullPtr<void (DWriteGdiInteropHook::*)(HDC &, _Out_ IDWriteFontFace** &)> _CreateFontFaceFromHdc_pre_ptr;
    NullPtr<void (DWriteGdiInteropHook::*)(HRESULT, HDC, _Out_ IDWriteFontFace**)> _CreateFontFaceFromHdc_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateBitmapRenderTarget(_In_opt_ HDC hdc, UINT32 width, UINT32 height, _Out_ IDWriteBitmapRenderTarget** renderTarget);
    NullPtr<void (DWriteGdiInteropHook::*)(_In_opt_ HDC &, UINT32 &, UINT32 &, _Out_ IDWriteBitmapRenderTarget** &)> _CreateBitmapRenderTarget_pre_ptr;
    NullPtr<void (DWriteGdiInteropHook::*)(HRESULT, _In_opt_ HDC, UINT32, UINT32, _Out_ IDWriteBitmapRenderTarget**)> _CreateBitmapRenderTarget_post_ptr;

};

// ==============================================================================
// IDWriteGlyphRunAnalysis
// ==============================================================================
class DWriteGlyphRunAnalysisHook : public HookBase<IDWriteGlyphRunAnalysis>
{

protected:

    DWriteGlyphRunAnalysisHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteGlyphRunAnalysisHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteGlyphRunAnalysisHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteGlyphRunAnalysisHook * typedPtr = (DWriteGlyphRunAnalysisHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetAlphaTextureBounds(DWRITE_TEXTURE_TYPE textureType, _Out_ RECT* textureBounds);
    NullPtr<void (DWriteGlyphRunAnalysisHook::*)(DWRITE_TEXTURE_TYPE &, _Out_ RECT* &)> _GetAlphaTextureBounds_pre_ptr;
    NullPtr<void (DWriteGlyphRunAnalysisHook::*)(HRESULT, DWRITE_TEXTURE_TYPE, _Out_ RECT*)> _GetAlphaTextureBounds_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateAlphaTexture(DWRITE_TEXTURE_TYPE textureType, _In_ RECT const* textureBounds, _Out_writes_bytes_(bufferSize) BYTE* alphaValues, UINT32 bufferSize);
    NullPtr<void (DWriteGlyphRunAnalysisHook::*)(DWRITE_TEXTURE_TYPE &, _In_ RECT const* &, _Out_writes_bytes_(bufferSize) BYTE* &, UINT32 &)> _CreateAlphaTexture_pre_ptr;
    NullPtr<void (DWriteGlyphRunAnalysisHook::*)(HRESULT, DWRITE_TEXTURE_TYPE, _In_ RECT const*, _Out_writes_bytes_(bufferSize) BYTE*, UINT32)> _CreateAlphaTexture_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetAlphaBlendParams(IDWriteRenderingParams* renderingParams, _Out_ FLOAT* blendGamma, _Out_ FLOAT* blendEnhancedContrast, _Out_ FLOAT* blendClearTypeLevel);
    NullPtr<void (DWriteGlyphRunAnalysisHook::*)(IDWriteRenderingParams* &, _Out_ FLOAT* &, _Out_ FLOAT* &, _Out_ FLOAT* &)> _GetAlphaBlendParams_pre_ptr;
    NullPtr<void (DWriteGlyphRunAnalysisHook::*)(HRESULT, IDWriteRenderingParams*, _Out_ FLOAT*, _Out_ FLOAT*, _Out_ FLOAT*)> _GetAlphaBlendParams_post_ptr;

};

// ==============================================================================
// IDWriteFactory
// ==============================================================================
class DWriteFactoryHook : public HookBase<IDWriteFactory>
{

protected:

    DWriteFactoryHook(UnknownBase & unknown, IUnknown * realobj)
        : BASE_CLASS(unknown, realobj)
    {
    }

    ~DWriteFactoryHook() {}

public:

    static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
    {
        UNREFERENCED_PARAMETER(context);
        try
        {
            return new DWriteFactoryHook(unknown, realobj);
        }
        catch(std::bad_alloc&)
        {
            HOOK_ERROR_LOG("Out of memory.");
            return nullptr;
        }
    }

    static void sDeleteInstance(void * context, void * ptr)
    {
        UNREFERENCED_PARAMETER(context);
        DWriteFactoryHook * typedPtr = (DWriteFactoryHook *)ptr;
        delete typedPtr;
    }

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetSystemFontCollection(_Out_ IDWriteFontCollection** fontCollection, BOOL checkForUpdates = FALSE);
    NullPtr<void (DWriteFactoryHook::*)(_Out_ IDWriteFontCollection** &, BOOL &)> _GetSystemFontCollection_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _Out_ IDWriteFontCollection**, BOOL)> _GetSystemFontCollection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCustomFontCollection(IDWriteFontCollectionLoader* collectionLoader, _In_reads_bytes_(collectionKeySize) void const* collectionKey, UINT32 collectionKeySize, _Out_ IDWriteFontCollection** fontCollection);
    NullPtr<void (DWriteFactoryHook::*)(IDWriteFontCollectionLoader* &, _In_reads_bytes_(collectionKeySize) void const* &, UINT32 &, _Out_ IDWriteFontCollection** &)> _CreateCustomFontCollection_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, IDWriteFontCollectionLoader*, _In_reads_bytes_(collectionKeySize) void const*, UINT32, _Out_ IDWriteFontCollection**)> _CreateCustomFontCollection_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterFontCollectionLoader(IDWriteFontCollectionLoader* fontCollectionLoader);
    NullPtr<void (DWriteFactoryHook::*)(IDWriteFontCollectionLoader* &)> _RegisterFontCollectionLoader_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, IDWriteFontCollectionLoader*)> _RegisterFontCollectionLoader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnregisterFontCollectionLoader(IDWriteFontCollectionLoader* fontCollectionLoader);
    NullPtr<void (DWriteFactoryHook::*)(IDWriteFontCollectionLoader* &)> _UnregisterFontCollectionLoader_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, IDWriteFontCollectionLoader*)> _UnregisterFontCollectionLoader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateFontFileReference(_In_z_ WCHAR const* filePath, _In_opt_ FILETIME const* lastWriteTime, _Out_ IDWriteFontFile** fontFile);
    NullPtr<void (DWriteFactoryHook::*)(_In_z_ WCHAR const* &, _In_opt_ FILETIME const* &, _Out_ IDWriteFontFile** &)> _CreateFontFileReference_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _In_z_ WCHAR const*, _In_opt_ FILETIME const*, _Out_ IDWriteFontFile**)> _CreateFontFileReference_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCustomFontFileReference(_In_reads_bytes_(fontFileReferenceKeySize) void const* fontFileReferenceKey, UINT32 fontFileReferenceKeySize, IDWriteFontFileLoader* fontFileLoader, _Out_ IDWriteFontFile** fontFile);
    NullPtr<void (DWriteFactoryHook::*)(_In_reads_bytes_(fontFileReferenceKeySize) void const* &, UINT32 &, IDWriteFontFileLoader* &, _Out_ IDWriteFontFile** &)> _CreateCustomFontFileReference_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _In_reads_bytes_(fontFileReferenceKeySize) void const*, UINT32, IDWriteFontFileLoader*, _Out_ IDWriteFontFile**)> _CreateCustomFontFileReference_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateFontFace(DWRITE_FONT_FACE_TYPE fontFaceType, UINT32 numberOfFiles, _In_reads_(numberOfFiles) IDWriteFontFile* const* fontFiles, UINT32 faceIndex, DWRITE_FONT_SIMULATIONS fontFaceSimulationFlags, _Out_ IDWriteFontFace** fontFace);
    NullPtr<void (DWriteFactoryHook::*)(DWRITE_FONT_FACE_TYPE &, UINT32 &, _In_reads_(numberOfFiles) IDWriteFontFile* const* &, UINT32 &, DWRITE_FONT_SIMULATIONS &, _Out_ IDWriteFontFace** &)> _CreateFontFace_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, DWRITE_FONT_FACE_TYPE, UINT32, _In_reads_(numberOfFiles) IDWriteFontFile* const*, UINT32, DWRITE_FONT_SIMULATIONS, _Out_ IDWriteFontFace**)> _CreateFontFace_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateRenderingParams(_Out_ IDWriteRenderingParams** renderingParams);
    NullPtr<void (DWriteFactoryHook::*)(_Out_ IDWriteRenderingParams** &)> _CreateRenderingParams_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _Out_ IDWriteRenderingParams**)> _CreateRenderingParams_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateMonitorRenderingParams(HMONITOR monitor, _Out_ IDWriteRenderingParams** renderingParams);
    NullPtr<void (DWriteFactoryHook::*)(HMONITOR &, _Out_ IDWriteRenderingParams** &)> _CreateMonitorRenderingParams_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, HMONITOR, _Out_ IDWriteRenderingParams**)> _CreateMonitorRenderingParams_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateCustomRenderingParams(FLOAT gamma, FLOAT enhancedContrast, FLOAT clearTypeLevel, DWRITE_PIXEL_GEOMETRY pixelGeometry, DWRITE_RENDERING_MODE renderingMode, _Out_ IDWriteRenderingParams** renderingParams);
    NullPtr<void (DWriteFactoryHook::*)(FLOAT &, FLOAT &, FLOAT &, DWRITE_PIXEL_GEOMETRY &, DWRITE_RENDERING_MODE &, _Out_ IDWriteRenderingParams** &)> _CreateCustomRenderingParams_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, FLOAT, FLOAT, FLOAT, DWRITE_PIXEL_GEOMETRY, DWRITE_RENDERING_MODE, _Out_ IDWriteRenderingParams**)> _CreateCustomRenderingParams_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE RegisterFontFileLoader(IDWriteFontFileLoader* fontFileLoader);
    NullPtr<void (DWriteFactoryHook::*)(IDWriteFontFileLoader* &)> _RegisterFontFileLoader_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, IDWriteFontFileLoader*)> _RegisterFontFileLoader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE UnregisterFontFileLoader(IDWriteFontFileLoader* fontFileLoader);
    NullPtr<void (DWriteFactoryHook::*)(IDWriteFontFileLoader* &)> _UnregisterFontFileLoader_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, IDWriteFontFileLoader*)> _UnregisterFontFileLoader_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTextFormat(_In_z_ WCHAR const* fontFamilyName, _In_opt_ IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, FLOAT fontSize, _In_z_ WCHAR const* localeName, _Out_ IDWriteTextFormat** textFormat);
    NullPtr<void (DWriteFactoryHook::*)(_In_z_ WCHAR const* &, _In_opt_ IDWriteFontCollection* &, DWRITE_FONT_WEIGHT &, DWRITE_FONT_STYLE &, DWRITE_FONT_STRETCH &, FLOAT &, _In_z_ WCHAR const* &, _Out_ IDWriteTextFormat** &)> _CreateTextFormat_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _In_z_ WCHAR const*, _In_opt_ IDWriteFontCollection*, DWRITE_FONT_WEIGHT, DWRITE_FONT_STYLE, DWRITE_FONT_STRETCH, FLOAT, _In_z_ WCHAR const*, _Out_ IDWriteTextFormat**)> _CreateTextFormat_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTypography(_Out_ IDWriteTypography** typography);
    NullPtr<void (DWriteFactoryHook::*)(_Out_ IDWriteTypography** &)> _CreateTypography_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _Out_ IDWriteTypography**)> _CreateTypography_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE GetGdiInterop(_Out_ IDWriteGdiInterop** gdiInterop);
    NullPtr<void (DWriteFactoryHook::*)(_Out_ IDWriteGdiInterop** &)> _GetGdiInterop_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _Out_ IDWriteGdiInterop**)> _GetGdiInterop_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTextLayout(_In_reads_(stringLength) WCHAR const* string, UINT32 stringLength, IDWriteTextFormat* textFormat, FLOAT maxWidth, FLOAT maxHeight, _Out_ IDWriteTextLayout** textLayout);
    NullPtr<void (DWriteFactoryHook::*)(_In_reads_(stringLength) WCHAR const* &, UINT32 &, IDWriteTextFormat* &, FLOAT &, FLOAT &, _Out_ IDWriteTextLayout** &)> _CreateTextLayout_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _In_reads_(stringLength) WCHAR const*, UINT32, IDWriteTextFormat*, FLOAT, FLOAT, _Out_ IDWriteTextLayout**)> _CreateTextLayout_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateGdiCompatibleTextLayout(_In_reads_(stringLength) WCHAR const* string, UINT32 stringLength, IDWriteTextFormat* textFormat, FLOAT layoutWidth, FLOAT layoutHeight, FLOAT pixelsPerDip, _In_opt_ DWRITE_MATRIX const* transform, BOOL useGdiNatural, _Out_ IDWriteTextLayout** textLayout);
    NullPtr<void (DWriteFactoryHook::*)(_In_reads_(stringLength) WCHAR const* &, UINT32 &, IDWriteTextFormat* &, FLOAT &, FLOAT &, FLOAT &, _In_opt_ DWRITE_MATRIX const* &, BOOL &, _Out_ IDWriteTextLayout** &)> _CreateGdiCompatibleTextLayout_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _In_reads_(stringLength) WCHAR const*, UINT32, IDWriteTextFormat*, FLOAT, FLOAT, FLOAT, _In_opt_ DWRITE_MATRIX const*, BOOL, _Out_ IDWriteTextLayout**)> _CreateGdiCompatibleTextLayout_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateEllipsisTrimmingSign(IDWriteTextFormat* textFormat, _Out_ IDWriteInlineObject** trimmingSign);
    NullPtr<void (DWriteFactoryHook::*)(IDWriteTextFormat* &, _Out_ IDWriteInlineObject** &)> _CreateEllipsisTrimmingSign_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, IDWriteTextFormat*, _Out_ IDWriteInlineObject**)> _CreateEllipsisTrimmingSign_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateTextAnalyzer(_Out_ IDWriteTextAnalyzer** textAnalyzer);
    NullPtr<void (DWriteFactoryHook::*)(_Out_ IDWriteTextAnalyzer** &)> _CreateTextAnalyzer_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _Out_ IDWriteTextAnalyzer**)> _CreateTextAnalyzer_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateNumberSubstitution(_In_ DWRITE_NUMBER_SUBSTITUTION_METHOD substitutionMethod, _In_z_ WCHAR const* localeName, _In_ BOOL ignoreUserOverride, _Out_ IDWriteNumberSubstitution** numberSubstitution);
    NullPtr<void (DWriteFactoryHook::*)(_In_ DWRITE_NUMBER_SUBSTITUTION_METHOD &, _In_z_ WCHAR const* &, _In_ BOOL &, _Out_ IDWriteNumberSubstitution** &)> _CreateNumberSubstitution_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _In_ DWRITE_NUMBER_SUBSTITUTION_METHOD, _In_z_ WCHAR const*, _In_ BOOL, _Out_ IDWriteNumberSubstitution**)> _CreateNumberSubstitution_post_ptr;

    // -----------------------------------------------------------------------------
    virtual HRESULT STDMETHODCALLTYPE CreateGlyphRunAnalysis(_In_ DWRITE_GLYPH_RUN const* glyphRun, FLOAT pixelsPerDip, _In_opt_ DWRITE_MATRIX const* transform, DWRITE_RENDERING_MODE renderingMode, DWRITE_MEASURING_MODE measuringMode, FLOAT baselineOriginX, FLOAT baselineOriginY, _Out_ IDWriteGlyphRunAnalysis** glyphRunAnalysis);
    NullPtr<void (DWriteFactoryHook::*)(_In_ DWRITE_GLYPH_RUN const* &, FLOAT &, _In_opt_ DWRITE_MATRIX const* &, DWRITE_RENDERING_MODE &, DWRITE_MEASURING_MODE &, FLOAT &, FLOAT &, _Out_ IDWriteGlyphRunAnalysis** &)> _CreateGlyphRunAnalysis_pre_ptr;
    NullPtr<void (DWriteFactoryHook::*)(HRESULT, _In_ DWRITE_GLYPH_RUN const*, FLOAT, _In_opt_ DWRITE_MATRIX const*, DWRITE_RENDERING_MODE, DWRITE_MEASURING_MODE, FLOAT, FLOAT, _Out_ IDWriteGlyphRunAnalysis**)> _CreateGlyphRunAnalysis_post_ptr;

};

