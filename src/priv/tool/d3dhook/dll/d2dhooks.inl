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

