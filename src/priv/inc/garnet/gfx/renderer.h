#ifndef __GN_GFX_RENDERER_H__
#define __GN_GFX_RENDERER_H__
// *****************************************************************************
/// \file
/// \brief   Main renderer interface of GFX module
/// \author  chenlee (2005.9.30)
// *****************************************************************************

/// \name aliases for global renderer signals
//@{
#define gSigRendererCreate         (GN::gfx::getSigRendererCreate())
#define gSigRendererDestroy        (GN::gfx::getSigRendererDestroy())
#define gSigRendererWindowSizeMove (GN::gfx::getSigRendererWindowSizeMove())
#define gSigRendererWindowClose    (GN::gfx::getSigRendererWindowClose())
//@}

namespace GN { namespace gfx
{
    struct Renderer;

    // ********************************************************************
    ///
    /// \name Renderer Signals
    ///
    /// - Renderer class may recreate itself when some system event happens,
    ///   like restoring from lock screen, which will trigger renderer create
    ///   and destroy signals.
    /// - Explicty (re)creation of renderer, will also trigger renderer signals.
    /// - 收到destroy信号后，应该删除所有的图形资源
    ///
    // ********************************************************************

    //@{

    ///
    /// D3D/OGL device creation signal
    ///
    GN_PUBLIC Signal1<bool,Renderer&> & getSigRendererCreate();

    ///
    /// D3D/OGL device destroy signal
    ///
    GN_PUBLIC Signal1<void,Renderer&> & getSigRendererDestroy();

    ///
    /// Happens when render windows is moved or resized.
    ///
    /// The 3 parameters are:
    ///  - HandleType monior   : monitor handle that render window stays in
    ///  - UInt32 clientWidth  : width of client area of render window
    ///  - UInt32 clientHeight : height of client area of render window
    ///
    GN_PUBLIC Signal4<void, Renderer&, HandleType, UInt32, UInt32> & getSigRendererWindowSizeMove();

    ///
    /// 当用户试图关闭渲染窗口时被触发，如点击窗口的关闭按钮或者按ALT-F4。
    ///
    /// This signal is useful when you want your application to quit when
    /// user click close button or press ALT-F4, while using internal
    /// render window.
    /// \par
    /// Note that if you igore this sigal, _NOTHING_ will happen. Internal
    /// render window will _NOT_ be closed. You can only close the internal
    /// render window by delete the renderer.
    /// \par
    /// When using external render window, this signall will be triggered
    /// as well, to make renderer behavior consistent. But normally, you
    /// should have external window messages handled already somewhere else
    /// in your code. So you may safely ignore this signal.
    ///
    GN_PUBLIC Signal1<void,Renderer&> & getSigRendererWindowClose();

    //@}

    ///
    /// Display mode structure
    ///
    struct DisplayMode
    {
        UInt32 width;   ///< Screen width. Zero means using current screen width. Default value is zero.
        UInt32 height;  ///< Screen height. Zero means using current screen height. Defualt value is zero.
        UInt32 depth;   ///< Color depth. Zero means using current color depth. Default value is zero.
        UInt32 refrate; ///< Referesh rate. Zero means using adapter default rate. Default value is zero.

        ///
        /// Set display mode parameters
        ///
        void set( UInt32 w, UInt32 h, UInt32 d, UInt32 r )
        {
            width = w; height = h; depth = d; refrate = r;
        }
    };

    ///
    /// Msaa type
    ///
    enum MsaaType
    {
        MSAA_NONE,      ///< No MSAA
        MSAA_LOW,       ///< low quality MSAA
        MSAA_MEDIUM,    ///< medium quality MSAA
        MSAA_HIGH,      ///< high quality MSAA
        MSAA_ULTRA,     ///< ultra quality MSAA
        NUM_MSAA_TYPES, ///< number of MSAA types
    };

    ///
    /// Renderer option structure.
    ///
    /// \sa Renderer::getOptions()
    ///
    struct RendererOptions
    {
        ///
        /// Display handle. No use on platform other than X Window. Default is zero.
        ///
        /// \note If zero, then default display will be used.
        ///
        HandleType displayHandle;

        /// \name these fields are valid only when 'useExternalWindow' is true.
        ///@{

        ///
        /// Handle of external render window.
        /// 缺省为0.
        ///
        /// \note Effective only if useExternalWindow is true.
        ///
        HandleType renderWindow;

        //@}

        /// \name these fields are valid only when 'useExternalWindow' is false.
        ///@{

        ///
        /// Handle of parent window. Default is zero, means a top-level window.
        ///
        HandleType parentWindow;

        ///
        /// Monitor handle.
        ///
        /// - Should be HMONITOR on MS Window or pointer to Screen structure on X Windows.
        /// - 0 means using the monitor where parent and/or render window stays in.
        ///   If monitorHandle and parent window are both zero, primary monitor will be used.
        /// - 缺省为0.
        ///
        HandleType monitorHandle;

        //@}

        ///
        /// Display mode for fullscreen mode. Ignored in windowed mode.
        ///
        /// \note For field equals zero, current display setting will be used.
        ///
        DisplayMode displayMode;

        ///
        /// Backbuffer width for windowed mode. Ignored in fullscreen mode.
        /// Default is zero, which means using client width of render window.
        /// If render window is also not avaiable, 640 will be used.
        ///
        UInt32 windowedWidth;

        ///
        /// Backbuffer height for windowed mode. Ignored in fullscreen mode.
        /// Default value is 0, which means using client height of render window.
        /// If render window is also not avaiable, default height 480 will be used.
        /// 缺省为0.
        ///
        UInt32 windowedHeight;

        ///
        /// Use external render window or not.
        /// 缺省为false.
        ///
        bool useExternalWindow;

        ///
        /// fullscreen or windowed mode.
        /// 缺省为false.
        ///
        bool fullscreen;

        ///
        /// 是否同步刷新. 缺省为false.
        ///
        bool vsync;

        /// \name D3D only parameters
        //@{
        bool reference;   ///< use reference device. 缺省为false.
        //@}

        /// \name OGL only parameters
        //@{

        ///
        /// Restore display mode while render window is deactivated.
        ///
        /// Note that this is a OGL only parameter. For D3D, you may use
        /// "Enable Multi-mon Debugging" option in DirectX control panel,
        /// and startup your application through a debugger to make that
        /// option effective.
        /// 缺省为true.
        ///
        bool autoRestore;

        //@}

        ///
        /// Construct default render options
        ///
        RendererOptions()
            : displayHandle(0)
            , renderWindow(0)
            , parentWindow(0)
            , monitorHandle(0)
            , windowedWidth(0)
            , windowedHeight(0)
            , useExternalWindow(false)
            , fullscreen(false)
            , vsync(false)
            , reference(false)
            , autoRestore(true)
        {
            displayMode.set(0,0,0,0);
        }
    };

    ///
    /// Display descriptor.
    ///
    /// \sa RendererOptions, Renderer::getDispDesc()
    ///
    struct DispDesc
    {
        HandleType displayHandle;    ///< Display handle. For X Window only.
        HandleType monitorHandle;    ///< Monitor handle.
        HandleType windowHandle;     ///< Render window handle
        UInt32     width;            ///< Back buffer width
        UInt32     height;           ///< Back buffer height
        UInt32     depth;            ///< Back buffer color depth in bits
        UInt32     refrate;          ///< Screen refresh rate

        ///
        /// equality operator
        ///
        bool operator!=( const DispDesc & rhs ) const
        {
            if( this == &rhs ) return false;
            return
                displayHandle != rhs.displayHandle ||
                monitorHandle != rhs.monitorHandle ||
                windowHandle != rhs.windowHandle ||
                width != rhs.width ||
                height != rhs.height ||
                depth != rhs.depth ||
                refrate != rhs.refrate;
        }
    };

    ///
    /// renderer caps
    ///
    struct RendererCaps
    {
        UInt32 maxTex1DSize[2];  ///< width, array
        UInt32 maxTex2DSize[3];  ///< width, height, array
        UInt32 maxTex3DSize[4];  ///< width, height, array
        UInt32 maxTextures;      ///< max number of simutaneous textures
        UInt32 maxRenderTargets; ///< max number of simutaneous render targets
        bool   vsProfiles[NUM_SHADER_PROFILES];
        bool   gsProfiles[NUM_SHADER_PROFILES];
        bool   psProfiles[NUM_SHADER_PROFILES];
    };

    ///
    /// enumerations used by renderer context structure
    ///
    enum RendererContextEnums
    {
        RC_FILL_SOLID = 0,
        RC_FILL_WIREFRAME,

        RC_CULL_NONE = 0,
        RC_CULL_FRONT,
        RC_CULL_BACK,

        RC_CMP_LT = 0,
        RC_CMP_LE,
        RC_CMP_EQ,
        RC_CMP_GE,
        RC_CMP_GT,
        RC_CMP_NE,

        RC_STENCIL_KEEP = 0,
        RC_STENCIL_ZERO,
        RC_STENCIL_REPLACE,
        RC_STENCIL_INC_SAT,
        RC_STENCIL_DEC_SAT,
        RC_STENCIL_INVERT,
        RC_STENCIL_INC,
        RC_STENCIL_DEC,

        RC_BLEND_ZERO = 0,
        RC_BLEND_ONE,
        RC_BLEND_SRC_COLOR,
        RC_BLEND_INV_SRC_COLOR,
        RC_BLEND_SRC_ALPHA,
        RC_BLEND_INV_SRC_ALPHA,
        RC_BLEND_DEST_ALPHA,
        RC_BLEND_INV_DEST_ALPHA,
        RC_BLEND_DEST_COLOR,
        RC_BLEND_INV_DEST_COLOR,
        RC_BLEND_BLEND_FACTOR,
        RC_BLEND_INV_BLEND_FACTOR,

        RC_BLEND_OP_ADD = 0,
        RC_BLEND_OP_SUB,
        RC_BLEND_OP_REV_SUB,
        RC_BLEND_OP_MIN,
        RC_BLEND_OP_MAX,
    };

    ///
    /// definition of single vertex element
    ///
    struct VertexElement
    {
        char        name[32];    ///< name of the element (must be unique among vertex format)
        ColorFormat format;      ///< format of the element
        UInt16      bufferIndex; ///< vertex buffer index
        UInt16      offset;      ///< offset of the element
    };

    ///
    /// define input vertex format
    ///
    struct VertexFormat
    {
        VertexElement elements[32]; ///< vertex element array
        UInt32        numElements;  ///< number of elements
    };

    ///
    /// renderer context
    ///
    struct RendererContext
    {
        // DWORD 0
        UInt32 fillMode       : 2;
        UInt32 cullMode       : 2;
        UInt32 scissorEnabled : 1;
        UInt32 msaaEnabled    : 1;
        UInt32 depthTest      : 1;
        UInt32 depthWrite     : 1;
        UInt32 depthFunc      : 3;
        UInt32 stencilEnable  : 1;
        UInt32 stencilPassOp  : 3; ///< pass both stencil and Z
        UInt32 stencilFailOp  : 3; ///< fail stencil (no z test at all)
        UInt32 stencilZFailOp : 3; ///< pass stencil but fail Z
        UInt32 nouse_0        : 11;

        // DWORD 1
        UInt32 blendSrc       : 4;
        UInt32 blendDst       : 4;
        UInt32 blendOp        : 3;
        UInt32 blendSrcAlpha  : 4;
        UInt32 blendDstAlpha  : 4;
        UInt32 blendOpAlpha   : 3;
        UInt32 nouse_1        : 10;

        // DWORD 2
        UInt32 colorWriteMask; ///< 4 bits x 8 render targets.

        // DWORD 3-6
        float  blendFactors[4];

        // DWORD 7-10
        Rect<UInt32> viewport;

        // DWORD 11-14
        Rect<UInt32> scissorRect; ///< scissor rects

        // TODO: depth bias

        /// vertex format
        VertexFormat vtxfmt;

        /// shader
        WeakRef<Shader> shader;

        // Resources
        WeakRef<VtxBuf>  vtxbufs[32];      ///< vertex buffers
        UInt32           strides[32];      ///< strides for each vertex buffer
        WeakRef<IdxBuf>  idxbuf;           ///< index buffer
        WeakRef<Texture> textures[32];     ///< textures
        WeakRef<Texture> renderTargets[8]; ///< render targets

        // TODO: sampler
    };

    // make sure bit-wise flags occupy only 2 DWORDs.
    GN_CASSERT( GN_FIELD_OFFSET(RendererContext,colorWriteMask) == 2*sizeof(UInt32) );

    ///
    /// 清屏标志
    ///
    enum ClearFlag
    {
        CLEAR_C      = 1,     ///< clear color buffer
        CLEAR_Z      = 2,     ///< clear z buffer
        CLEAR_S      = 4,     ///< clear stencil buffer
        CLEAR_ALL    = 7      ///< clear all buffers
    };

    ///
    /// Options for Renderer::drawLines
    ///
    enum DrawLineOptions
    {
        ///
        /// 使用当前的渲染状态。
        ///
        /// By default, Renderer::drawLines() will use a special render state block that
        /// suites for colored lines
        ///
        DL_USE_CURRENT_RS = 1<<0,

        ///
        /// 使用当前的Vertex Shader。
        ///
        /// - 缺省情况下，Renderer::drawLines() 会使用一个内置的vertex shader
        /// - 自定义的vertex shader应接受一组3D空间坐标
        ///
        DL_USE_CURRENT_VS = 1<<1,

        ///
        /// 使用当前的Pixel Shader。
        ///
        /// 缺省情况下，Renderer::drawLines() 会使用一个内置的Pixel Shader
        ///
        DL_USE_CURRENT_PS = 1<<2,

        ///
        /// 使用当前的Texture states.
        ///
        /// Effective only when using fixed function pipeline.
        ///
        DL_USE_CURRENT_TS = 1<<3,

        ///
        /// position in window (post-transformed) space:
        /// (0,0) for left-up corner, (width,height) for right-bottom corner.
        ///
        /// By default, line positions are in object space.
        ///
        /// \note This option is meaningful only when DL_USE_CURRENT_VS is _NOT_ set.
        ///
        DL_WINDOW_SPACE = 1<<4,

        ///
        /// Using line strip. By default input points are treated as line list.
        ///
        DL_LINE_STRIP = 1<<5,

        ///
        /// 上述 DL_USE_CURRENT_XX 的集合
        ///
        DL_USE_CURRENT = DL_USE_CURRENT_RS | DL_USE_CURRENT_VS | DL_USE_CURRENT_PS | DL_USE_CURRENT_TS
    };

    ///
    /// Define rendering API
    ///
    enum RendererAPI
    {
        API_OGL,          ///< OpenGL
        API_D3D9,         ///< D3D9
        API_D3D10,        ///< D3D10
        API_FAKE,         ///< Fake API
        NUM_RENDERER_API, ///< Number of avaliable API.
        API_AUTO,         ///< determine rendering API automatically.
    };

    ///
    /// 渲染器模块的主接口类
    ///
    /// \nosubgrouping
    ///
    struct Renderer : public NoCopy
    {
        // ********************************************************************
        //
        /// \name Display Manager
        //
        // ********************************************************************

        //@{

        ///
        /// Reset renderer with new options.
        ///
        /// \param ro
        ///     new renderer options
        /// \note
        ///     - You must call this function at least once, to make renderer usable.
        ///     - This function may trigger gSigRendererCreate and gSigRendererDestroy.
        ///
        virtual bool reset( const RendererOptions & ro ) = 0;

        ///
        /// Get renderer options
        ///
        virtual const RendererOptions & getOptions() const = 0;

        ///
        /// Get Display Description
        ///
        virtual const DispDesc & getDispDesc() const = 0;

        ///
        /// Return the rendering API
        ///
        virtual RendererAPI getApi() const = 0;

        ///
        /// For D3D, return pointer to current D3D device; for OGL, return NULL.
        ///
        virtual void * getD3DDevice() const = 0;

        ///
        /// For OGL, return current rendering context; for D3D, return NULL.
        ///
        virtual void * getOGLRC() const = 0;

        //@}

        // ********************************************************************
        //
        /// \name Capabilities Manager
        //
        // ********************************************************************

        //@{

        ///
        /// Get render device caps
        ///
        virtual const RendererCaps & getCaps() const = 0;

        ///
        /// Check texture format support
        ///
        /// \param format       The texture format.
        /// \param usages       Combination of TextureUsage
        ///
        virtual bool checkTextureFormatSupport( ColorFormat format, TextureUsages usages ) const = 0;

        ///
        /// Get default texture format.
        ///
        /// \param usage        Combination of TextureUsage
        ///
        /// \return             Return COLOR_FORMAT_UNKNOWN, if the usage is not supported by current renderer.
        ///
        virtual ColorFormat getDefaultTextureFormat( TextureUsages usages ) const = 0;

        //@}

        // ********************************************************************
        //
        /// \name Resource Manager
        //
        // ********************************************************************

        //@{

        ///
        /// Compile shader into platform dependant format
        ///
        virtual CompiledShaderBlob *
        compileShader( const ShaderDesc & desc ) = 0;

        ///
        /// create shader
        ///
        virtual Shader *
        createShader( const CompiledShaderBlob * ) = 0;

        ///
        /// Create new texture
        /// See TextureDesc for detail explaination of each fields in descriptor.
        ///
        virtual Texture *
        createTexture( const TextureDesc & desc ) = 0;

        ///
        /// Create new texture, with individual creation parameters.
        ///
        Texture *
        createTexture( size_t        sx,
                       size_t        sy,
                       size_t        sz,
                       size_t        faces  = 1,
                       size_t        levels = 0, // 0 means full mipmap chain
                       ColorFormat   format = COLOR_FORMAT_UNKNOWN,
                       TextureUsages usages = TextureUsages::DEFAULT() )
        {
            TextureDesc desc =
            {
                (UInt32)sx, (UInt32)sy, (UInt32)sz,
                (UInt32)faces, (UInt32)levels,
                COLOR_FORMAT_UNKNOWN == format ? getDefaultTextureFormat( usages ) : format,
                usages,
            };
            return createTexture( desc );
        }

        ///
        /// Create 1D texture
        ///
        Texture *
        create1DTexture( size_t        sx,
                         size_t        levels = 0,
                         ColorFormat   format = COLOR_FORMAT_UNKNOWN,
                         TextureUsages usages = TextureUsages::DEFAULT() )
        {
            return createTexture( sx, 1, 1, 1, levels, format, usages );
        }

        ///
        /// Create 2D texture
        ///
        Texture *
        create2DTexture( size_t        sx,
                         size_t        sy,
                         size_t        levels = 0,
                         ColorFormat   format = COLOR_FORMAT_UNKNOWN,
                         TextureUsages usages = TextureUsages::DEFAULT() )
        {
            return createTexture( sx, sy, 1, 1, levels, format, usages );
        }

        ///
        /// Create 3D texture
        ///
        Texture *
        create3DTexture( size_t        sx,
                         size_t        sy,
                         size_t        sz,
                         size_t        levels = 0,
                         ColorFormat   format = COLOR_FORMAT_UNKNOWN,
                         TextureUsages usages = TextureUsages::DEFAULT() )
        {
            return createTexture( sx, sy, sz, 1, levels, format, usages );
        }

        ///
        /// Create CUBE texture
        ///
        Texture *
        createCubeTexture( size_t        sx,
                           size_t        levels = 0,
                           ColorFormat   format = COLOR_FORMAT_UNKNOWN,
                           TextureUsages usages = TextureUsages::DEFAULT() )
        {
            return createTexture( sx, sx, 1, 6, levels, format, usages );
        }

        ///
        /// Create new vertex buffer
        ///
        virtual VtxBuf *
        createVtxBuf( const VtxBufDesc & ) = 0;

        ///
        /// Create new index buffer
        ///
        virtual IdxBuf *
        createIdxBuf( const IdxBufDesc & desc ) = 0;

        ///
        /// Create new index buffer
        ///
        IdxBuf *
        createIdxBuf( size_t numidx, bool dynamic = false, bool readback = false )
        {
            IdxBufDesc desc;
            desc.numidx   = (UInt32)numidx;
            desc.bits32   = false;
            desc.dynamic  = dynamic;
            desc.readback = readback;
            return createIdxBuf( desc );
        }

        //@}

        // ********************************************************************
        //
        /// \name Context Manager
        //
        // ********************************************************************

        //@{

        ///
        /// Bind rendering context to rendering device.
        ///
        virtual void bindContext( const RendererContext & ) = 0;

        ///
        /// Rebind current rendering context to rendering device.
        ///
        /// Renderer usually has internal cache mechanism to avoid
        /// redunant state changing. So if you call D3D/OGL functions
        /// directly in your code that changes D3D/OGL states, this cache
        /// mechanism will be broken. One way to avoid this situation, is
        /// to store/restore D3D/OGL states by yourself. Another way is to
        /// call this function to force rebinding of current renderer
        /// context, which is much easier and less error prone.
        ///
        virtual void rebindContext() = 0;

        ///
        /// Get current render context
        ///
        virtual const RendererContext & getContext() const = 0;

        //@}

        // ********************************************************************
        //
        /// \name Drawing Manager
        //
        // ********************************************************************

        //@{

        ///
        /// 结束一帧的绘图操作
        ///
        virtual void present() = 0;

        ///
        /// 清屏操作
        ///
        /// \param flags 清屏标志, see ClearFlag
        /// \param c     背景色
        /// \param z     深度值
        /// \param s     模板值
        ///
        /// \note Must be called btween drawBegin() and drawEnd().
        ///
        virtual void
        clearScreen( const Vector4f & c = Vector4f(0,0,0,1),
                     float z = 1.0f, UInt8 s = 0,
                     BitFields flags = CLEAR_ALL ) = 0;

        ///
        /// Draw indexed primitives.
        ///
        /// \param prim
        ///     primititive type
        /// \param numprim
        ///     number of primitives
        /// \param startvtx
        ///     vertex index into vertex buffer that index "0" refers to.
        /// \param minvtxidx, numvtx
        ///     define effective range in vertex buffer, starting from startvtx.
        /// \param startidx
        ///     index into index buffer of the first index
        ///
        /// \note 必须在 drawBegin() 和 drawEnd() 之间调用
        ///
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numprim,
                                  size_t        startvtx,
                                  size_t        minvtxidx,
                                  size_t        numvtx,
                                  size_t        startidx ) = 0;

        ///
        /// Draw non-indexed primitives.
        ///
        /// \param prim
        ///     primititive type
        /// \param numprim
        ///     number of primitives
        /// \param startvtx
        ///     index into vertex buffer of the first vertex.
        ///
        /// \note 必须在 drawBegin() 和 drawEnd() 之间调用
        ///
        virtual void draw( PrimitiveType prim,
                           size_t        numprim,
                           size_t        startvtx ) = 0;

        ///
        /// draw on-indexed primitives with user-defined data array
        ///
        virtual void drawIndexedUp(
                             PrimitiveType  prim,
                             size_t         numprim,
                             size_t         numvtx,
                             const void *   vertexData,
                             size_t         strideInBytes,
                             const UInt16 * indexData ) = 0;

        ///
        /// draw on-indexed primitives with user-defined data array
        ///
        virtual void drawUp( PrimitiveType prim,
                             size_t        numprim,
                             const void *  vertexData,
                             size_t        strideInBytes ) = 0;

        ///
        /// Draw line segments
        ///
        /// \param options
        ///     渲染选项，详见 DrawLineOptions。Set to 0 to use default options
        /// \param positions
        ///     顶点坐标数据，由一系列的3D顶点组成。2个顶点表示一条线段。
        ///     选项 DL_WINDOW_SPACE会影响坐标的含义。
        /// \param stride
        ///     stride of one vertex.
        /// \param count
        ///     Number of line segments (note: _NOT_ number of points)
        /// \param rgba
        ///     line color, in R-G-B-A format.
        /// \param model, view, proj
        ///     Transformation matrices. Ignored when using DL_WINDOW_SPACE.
        ///
        virtual void drawLines( BitFields         options,
                                const void *      positions,
                                size_t            stride,
                                size_t            count,
                                UInt32            rgba,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj ) = 0;

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
        //
        // ********************************************************************

        //@{

        ///
        /// This function is provided because different API has different ways
        /// to compose projection matrix.
        ///
        Matrix44f &
        composePerspectiveMatrixLh( Matrix44f & result,
                                    float fovy,
                                    float ratio,
                                    float znear,
                                    float zfar ) const
        {
            return getD3DDevice()
                ? result.perspectiveD3DLh( fovy, ratio, znear, zfar )
                : result.perspectiveOGLLh( fovy, ratio, znear, zfar );
        }

        ///
        /// This function is provided because different API has different ways
        /// to compose projection matrix.
        ///
        Matrix44f &
        composePerspectiveMatrixRh( Matrix44f & result,
                                    float fovy,
                                    float ratio,
                                    float znear,
                                    float zfar ) const
        {
            return getD3DDevice()
                ? result.perspectiveD3DRh( fovy, ratio, znear, zfar )
                : result.perspectiveOGLRh( fovy, ratio, znear, zfar );
        }

        ///
        /// This function is provided because different API has different ways
        /// to compose projection matrix.
        ///
        Matrix44f &
        composePerspectiveMatrix( Matrix44f & result,
                                  float fovy,
                                  float ratio,
                                  float znear,
                                  float zfar ) const
        {
            return getD3DDevice()
                ? result.perspectiveD3D( fovy, ratio, znear, zfar )
                : result.perspectiveOGL( fovy, ratio, znear, zfar );
        }

        ///
        /// This function is provided because different API has different ways
        /// to compose projection matrix.
        ///
        Matrix44f &
        composeOrthoMatrix( Matrix44f & result,
                            float left,
                            float bottom,
                            float width,
                            float height,
                            float znear,
                            float zfar ) const
        {
            return getD3DDevice()
                ? result.orthoD3D( left, left+width, bottom, bottom+height, znear, zfar )
                : result.orthoOGL( left, left+width, bottom, bottom+height, znear, zfar );
        }

        ///
        /// Enable/Disable parameter check for performance critical functions.
        ///
        /// Enabled by default for debug build; disabled by default for release build.
        ///
        virtual void enableParameterCheck( bool enable ) = 0;

        ///
        /// dump device states of the next frame.
        ///
        /// \param startBatchIndex, numBatches
        ///     Specify range of the dump. Set (0,0) to dump the whole frame.
        ///
        /// \note
        ///     Dump data format is renderer dependent.
        ///     Check document of specific renderer implementation for details.
        ///
        virtual void dumpNextFrame( size_t startBatchIndex = 0, size_t numBatches = 0 ) = 0;

        //@}
    };

    ///
    /// Create a new renderer.
    ///
    Renderer * createRenderer( RendererAPI = API_AUTO );

    ///
    /// Delete renderer
    ///
    void deleteRenderer( Renderer * );
}}

#include "renderer.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_RENDERER_H__
