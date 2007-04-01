#ifndef __GN_GFX_RENDERER_H__
#define __GN_GFX_RENDERER_H__
// *****************************************************************************
/// \file    renderer.h
/// \brief   Main renderer interface of GFX module
/// \author  chenlee (2005.9.30)
// *****************************************************************************

///
/// Global renderer instance
///
#define gRenderer (::GN::gfx::Renderer::sGetInstance())

///
/// Pointer of global renderer instance
///
#define gRendererPtr (::GN::gfx::Renderer::sGetInstancePtr())

/// \name aliases for global renderer signals
//@{
#define gSigRendererCreate        (GN::gfx::getSigRendererCreate())
#define gSigRendererRestore       (GN::gfx::getSigRendererRestore())
#define gSigRendererDispose       (GN::gfx::getSigRendererDispose())
#define gSigRendererDestroy       (GN::gfx::getSigRendererDestroy())
#define gSigRendererWindowClosing (GN::gfx::getSigRendererWindowClosing())
//@}

namespace GN { namespace gfx
{
    // ********************************************************************
    ///
    /// \name Renderer Signals
    ///
    /// - 信号可以被多次触发，且一定是严格按照如下的顺序：
    ///   <pre>
    ///                         +---------+
    ///                        \|/        |
    ///                         '         |
    ///   (start)-->create-->restore-->dispose-->destroy-->(end)
    ///               .                             |
    ///              /|\                            |
    ///               +-----------------------------+
    ///   </pre>
    /// - create发生后, 必定会紧随着发生一个restore
    /// - 收到create/restore信号说明渲染器ready to use。
    /// - 收到destroy信号后，渲染器的context会被重置为缺省值。
    /// - 这些信号的标准使用方法如下：
    ///   - 收到create信号后, 创建所有图形资源。
    ///   - 收到restore信号后，填充图形资源的内容，如从磁盘读取贴图和模型。
    ///     - 应尽量避免在此创建新的资源，因为这个信号在程序生命期中有可能
    ///       被多次触发。
    ///     - 如必须在这里创建图形资源，则这些资源应在收到dispose信号后释放
    ///   - 收到dispose信号后，应释放在restore信号中创建的资源。
    ///   - 收到destroy信号后，删除所有的图形资源
    ///
    // ********************************************************************

    //@{

    ///
    /// D3D/OGL device creation signal
    ///
    GN_PUBLIC Signal0<bool> & getSigRendererCreate();

    ///
    /// Triggered after renderer is created or restored successfully from
    /// last dispose, and ready to use.
    ///
    /// (Re)load content of graphics resources.
    /// - Lockable resources like texture, vertex buffer and index buffer,
    ///   need content reloading.
    /// - No need to reload shaders, render state blocks and VtxFmtHandle.
    ///
    GN_PUBLIC Signal0<bool> & getSigRendererRestore();

    ///
    /// D3D/OGL device dispose signal
    ///
    GN_PUBLIC Signal0<void> & getSigRendererDispose();

    ///
    /// D3D/OGL device destroy signal
    ///
    GN_PUBLIC Signal0<void> & getSigRendererDestroy();

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
    GN_PUBLIC Signal0<void> & getSigRendererWindowClosing();

    //@}

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

        ///
        /// Use external render window or not.
        /// 缺省为false.
        ///
        bool useExternalWindow;

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
        /// 是否监视渲染窗口的大小，并自动对渲染设备作相应的调整
        /// 缺省为true.
        ///
        bool trackWindowSizeMove;

        ///
        /// Display mode for fullscreen mode. Ignored in windowed mode.
        ///
        /// \note For field equals zero, current display setting will be used.
        ///
        DisplayMode displayMode;

        ///
        /// Backbuffer width for windowed mode. Ignored in fullscreen mode.
        /// Zero means using client width of render window. If render window
        /// is also not avaiable, default width 640 will be used.
        /// 缺省为0.
        ///
        UInt32 windowedWidth;

        ///
        /// Backbuffer height for windowed mode. Ignored in fullscreen mode.
        /// Zero means using client height of render window. If render window
        /// is also not avaiable, default height 480 will be used.
        /// 缺省为0.
        ///
        UInt32 windowedHeight;

        ///
        /// Msaa type
        ///
        MsaaType msaa;

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
        bool software;    ///< use software device. 缺省为false.
        bool reference;   ///< use reference device. 缺省为false.
        bool pure;        ///< use pure device. Default is false.
        bool multithread; ///< use multi-thread safe deivce. Default is false.
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
            , useExternalWindow(false)
            , renderWindow(0)
            , parentWindow(0)
            , monitorHandle(0)
            , trackWindowSizeMove(true)
            , windowedWidth(0)
            , windowedHeight(0)
            , msaa(MSAA_NONE)
            , fullscreen(false)
            , vsync(false)
            , software(false)
            , reference(false)
            , pure(false)
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
        HandleType windowHandle;     ///< Render window handle
        HandleType monitorHandle;    ///< Monitor handle.
        UInt32 width;              ///< Back buffer width
        UInt32 height;             ///< Back buffer height
        UInt32 depth;              ///< Back buffer depth
        UInt32 refrate;            ///< Screen refresh rate

        ///
        /// equality operator
        ///
        bool operator!=( const DispDesc & rhs ) const
        {
            if( this == &rhs ) return false;
            return
                displayHandle != rhs.displayHandle ||
                windowHandle != rhs.windowHandle ||
                monitorHandle != rhs.monitorHandle ||
                width != rhs.width ||
                height != rhs.height ||
                depth != rhs.depth ||
                refrate != rhs.refrate;
        }
    };

    /// \def GNGFX_CAPS
    /// Define renderer caps

    ///
    /// renderer caps
    ///
    enum RendererCaps
    {
        #define GNGFX_CAPS(X) CAPS_##X,
        #include "rendererCapsMeta.h"
        #undef GNGFX_CAPS
        NUM_RENDERER_CAPS,
        CAPS_INVALID
    };

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
        API_OGL,   ///< OpenGL
        API_D3D9,  ///< D3D9
        API_D3D10, ///< D3D10
        API_FAKE,  ///< Fake API
        NUM_RENDERER_API, ///< Number of avaliable API.
        API_AUTO, ///< determine rendering API automatically.
    };

    ///
    /// gfx模块的主接口类
    ///
    /// \nosubgrouping
    ///
    struct Renderer : public CrossDllSingleton<Renderer>, public NoCopy
    {
        // ********************************************************************
        //
        /// \name Device Manager
        //
        // ********************************************************************

        //@{

    private:

        RendererOptions mOptions;

    protected:

        ///
        /// Update private renderer option variable.
        ///
        void setOptions( const RendererOptions & ro ) { mOptions = ro; }

    public:

        ///
        /// Change renderer options.
        ///
        /// \param ro
        ///     new renderer options
        /// \param forceDeviceRecreation
        ///     force a full device recreation
        /// \note
        ///     - You must call this function at least once, to make renderer usable.
        ///     - This function may trigger gSigRendererRestore and/or gSigRendererDispose.
        ///
        virtual bool changeOptions( const RendererOptions & ro, bool forceDeviceRecreation = false ) = 0;

        ///
        /// Get renderer options
        ///
        const RendererOptions & getOptions() const { return mOptions; }

        //@}

        // ********************************************************************
        //
        /// \name Display Manager
        /// \note
        ///     We provide two functions to get API specific rendering context:
        ///     - Sometime, you want to run some API specific codes,
        ///       for debug, test or any other purpose.
        ///     - You may use these functions to detect the current API
        //
        // ********************************************************************

        //@{

    private:

        DispDesc mDispDesc;

    protected:

        ///
        /// Update private diplay decriptor
        ///
        void setDispDesc( const DispDesc & desc )
        {
            mDispDesc = desc;
        }

    public:

        ///
        /// 获得当前的渲染窗口句柄
        ///
        const DispDesc & getDispDesc() const { return mDispDesc; }

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

    protected:

        UInt32 mCaps[NUM_RENDERER_CAPS];

    public:

        ///
        /// Get render device caps
        ///
        UInt32 getCaps( SInt32 c ) const { GN_ASSERT( 0 <= c && c < NUM_RENDERER_CAPS ); return mCaps[c]; }

        ///
        /// Check renderer support to specific shader profile. Profile tag could be:
        ///     vs_1_1 vs_2_0 vs_2_x vs_3_0 xvs
        ///     ps_1_1 ps_1_2 ps_1_3 ps_1_4 ps_2_0 ps_2_x ps_3_0
        ///     ps_4_0 vs_4_0
        ///     xps
        ///     arbvp1, arbfp1,
        ///     glslvs, glslps,
        ///     cgvs, cgps
        ///
        /// \note Profile tag is case sensitive.
        ///
        virtual bool supportShader( const StrA & ) = 0;

        ///
        /// Test compability of specific texture format
        ///
        virtual bool supportTextureFormat( TexDim type, BitFields usage, ClrFmt format ) const = 0;

        //@}

        // ********************************************************************
        //
        /// \name Resource Manager
        //
        // ********************************************************************

        //@{

        ///
        /// Create shader. Parameter 'entry' will be ignored for low-level shading language.
        ///
        /// \param type
        ///     Shader type
        /// \param lang
        ///     Shading language
        /// \param code
        ///     Shader code
        /// \param hints
        ///     Shader compilation hints. Hints string must be in format that can be imported
        ///     into a registry object. See Registry::importFromStr() for details.
        ///     \par
        ///     For D3D shader, several hints are supported:
        ///     - "entry": specify entry function name, default is "main"
        ///     - "target": specify HLSL compile target, default is empty, means using the highest possible target.
        ///     - "sm30": favor shader model 3.0 or not, default is yes.
        ///       - In DirectX, VS 3.0 and PS 3.0 can _ONLY_ be used with each other. So sometimes, you may want
        ///         your HLSL shader to be compiled into VS/PS 2.x. So it can be used with other non-SM3 shaders.
        ///       - This hints has higher priority then "target". So if you set "target" to "vs_3_0", while setting
        ///         this hint to "true". The shader will be compiled to "vs_2_a".
        ///
        virtual Shader *
        createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints = "" ) = 0;

        ///
        /// create shader from file
        ///
        Shader *
        createShaderFromFile( ShaderType type, ShadingLanguage lang, const StrA & filename, const StrA & hints = "" );

        ///
        /// Create vetex shader.
        ///
        Shader *
        createVS( ShadingLanguage lang, const StrA & code, const StrA & hints = "" );

        ///
        /// Create pixel shader.
        ///
        Shader *
        createPS( ShadingLanguage lang, const StrA & code, const StrA & hints = "" );

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
        createTexture( TexDim    dim,
                       size_t    sx, size_t sy, size_t sz,
                       size_t    faces = 0,
                       size_t    levels = 0,
                       ClrFmt    format = FMT_DEFAULT,
                       BitFields usage = 0 )
        {
            TextureDesc desc =
            {
                dim,
                (UInt32)sx, (UInt32)sy, (UInt32)sz,
                (UInt32)faces, (UInt32)levels,
                format,
                { usage }
            };
            return createTexture( desc );
        }

        ///
        /// Create 1D texture
        ///
        Texture *
        create1DTexture( size_t    sx,
                         size_t    levels = 0,
                         ClrFmt    format = FMT_DEFAULT,
                         BitFields usage = 0 )
        {
            return createTexture( TEXDIM_1D, sx, 0, 0, 1, levels, format, usage );
        }

        ///
        /// Create 2D texture
        ///
        Texture *
        create2DTexture( size_t    sx, size_t sy,
                         size_t    levels = 0,
                         ClrFmt    format = FMT_DEFAULT,
                         BitFields usage = 0 )
        {
            return createTexture( TEXDIM_2D, sx, sy, 0, 1, levels, format, usage );
        }

        ///
        /// Create 3D texture
        ///
        Texture *
        create3DTexture( size_t    sx, size_t sy, size_t sz,
                         size_t    levels = 0,
                         ClrFmt    format = FMT_DEFAULT,
                         BitFields usage = 0 )
        {
            return createTexture( TEXDIM_3D, sx, sy, sz, 1, levels, format, usage );
        }

        ///
        /// Create CUBE texture
        ///
        Texture *
        createCubeTexture( size_t    sx,
                           size_t    levels = 0,
                           ClrFmt    format = FMT_DEFAULT,
                           BitFields usage = 0 )
        {
            return createTexture( TEXDIM_CUBE, sx, 0, 0, 6, levels, format, usage );
        }

        ///
        /// Create vertex format handle.
        ///
        virtual VtxFmtHandle createVtxFmt( const VtxFmtDesc & ) = 0;

        ///
        /// Create new vertex buffer
        ///
        virtual VtxBuf *
        createVtxBuf( const VtxBufDesc & ) = 0;

        ///
        /// Create new vertex buffer
        ///
        virtual VtxBuf *
        createVtxBuf( size_t bytes, bool dynamic = false, bool readback = false )
        {
            VtxBufDesc desc;
            desc.bytes = (UInt32)bytes;
            desc.dynamic = dynamic;
            desc.readback = readback;
            return createVtxBuf( desc );
        }

        ///
        /// Create new dynamic vertex buffer
        ///
        VtxBuf *
        createDynamicVtxBuf( size_t bytes ) { return createVtxBuf( bytes, true, false ); }

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
            desc.numidx = (UInt32)numidx;
            desc.dynamic = dynamic;
            desc.readback = readback;
            return createIdxBuf( desc );
        }

        ///
        /// Create new dynamic index buffer
        ///
        IdxBuf *
        createDynamicIdxBuf( size_t numidx ) { return createIdxBuf( numidx, true, false ); }

        //@}

        // ********************************************************************
        //
        /// \name Context Manager
        //
        // ********************************************************************

        //@{

    public:

        ///
        /// Set rendering context.
        ///
        /// This function, with pre-initialized renderer context structure, is
        /// always prefered over context update helper functions below, for
        /// better performance.
        ///
        virtual void setContext( const RendererContext & ) = 0;

        ///
        /// Rebind current rendering context to rendering device.
        ///
        /// \par
        ///     This function will rebind current rendering context to renderer.
        /// \par
        ///     Renderer class have internal cache mechanism to avoid
        ///     redunant state changing. But if you call D3D/OGL functions
        ///     directly in your code that changes D3D/OGL states, this cache
        ///     mechanism will be broken. One way to avoid this situation, is
        ///     to store/restore D3D/OGL states by yourself. Another way is to
        ///     call this function to force rebinding of current renderer
        ///     context, which is much easier and less error prone.
        ///
        virtual void rebindContext( RendererContext::FieldFlags ) = 0;

        ///
        /// Get current render state block descriptor
        ///
        virtual const RenderStateBlockDesc & getCurrentRenderStateBlock() const = 0;

        // ********************************************************************
        ///
        /// \name Helper functions to update rendering context.
        ///
        /// - See corresponding methods in RendererContext for detail usage.
        /// - Recommended call sequence is:
        /// <pre>
        ///     contextUpdateBegin();
        ///     ... // call context update methods here.
        ///     contextUpdateEnd();
        /// </pre>
        /// - Calling update method outside of contextUpdateBegin() and
        ///   contextUpdateEnd() will take effect immediatly, but is not
        ///   recommented for performance reason.
        ///
        // ********************************************************************

        //@{

    private:

        RendererContext      mHelperContext;
        AutoInit<bool,false> mHelperContextUpdateBegun;

    public:

        ///
        /// start context update
        ///
        inline void contextUpdateBegin();

        ///
        /// end context update, flush modified context to renderer.
        ///
        inline void contextUpdateEnd();

        inline void setShader( ShaderType type, const Shader * shader );
        inline void setShaders( const Shader * vs, const Shader * ps, const Shader * gs );
        inline void setVS( const Shader * s );
        inline void setPS( const Shader * s );
        inline void setGS( const Shader * s );
        inline void setRenderStateBlock( const RenderStateBlockDesc & );
        inline void setRenderState( RenderState state, SInt32 value );
        inline void setRenderTargets( const RenderTargetDesc & );
        inline void setDrawToBackBuf();
        inline void setDrawToTextures( UInt32 count, const Texture * rt0, const Texture * rt1=0, const Texture * rt2=0, const Texture * rt3=0, const Texture * z=0, MsaaType aa=MSAA_NONE );
        inline void setDrawToTextureWithoutDepth( const Texture * tex, UInt32 level = 0, UInt32 face = 0, UInt32 slice = 0, MsaaType aa_ = MSAA_NONE );
        inline void setViewport( const Rectf & );
        inline void setViewport( float left, float top, float width, float height );

        inline void setWorld( const Matrix44f & );
        inline void setView( const Matrix44f & );
        inline void setProj( const Matrix44f & );
        inline void setTextureStateBlock( const TextureStateBlockDesc & );
        inline void setTextureState( size_t stage, TextureState state, TextureStateValue value );

        inline void setTexture( size_t stage, const Texture * tex );
        inline void setTextures( const Texture * const texlist[], size_t start, size_t count );
        inline void setVtxFmt( VtxFmtHandle );
        inline void setVtxBuf( size_t index, const VtxBuf * buffer, size_t offset, size_t stride );
        inline void setIdxBuf( const IdxBuf * );

        //@}

        // ********************************************************************
        //
        /// \name Drawing Manager
        //
        // ********************************************************************

        //@{

    protected:

        AutoInit<size_t,0> mNumPrims;   ///< Number of primitives per frame.
        AutoInit<size_t,0> mNumBatches; ///< Number of draws per frame.

    public :

        ///
        /// 开始绘图操作.
        ///
        /// 所有的绘图操作都必须在 drawBegin() 和 drawEnd() 之间调用
        ///
        virtual bool drawBegin() = 0;

        ///
        /// 结束绘图操作. Similar as EndScene() followed by Present()
        ///
        /// call drawEnd() <b>if and only if</b> drawBegin() returns true.
        ///
        virtual void drawEnd() = 0;

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
        /// \param color
        ///     line color. in FMT_BGRA32 format.
        /// \param model, view, proj
        ///     Transformation matrices. Ignored when using DL_WINDOW_SPACE.
        ///
        virtual void drawLines(
            BitFields options,
            const void * positions,
            size_t stride,
            size_t count,
            UInt32 color,
            const Matrix44f & model,
            const Matrix44f & view,
            const Matrix44f & proj ) = 0;

        ///
        /// 返回上一次 drawEnd() 到现在所绘制的原语的个数
        ///
        size_t getNumPrimitives() const { return mNumPrims; }

        ///
        /// 返回上一次 drawEnd() 到现在 draw() / drawindexed() 的次数
        ///
        size_t getNumBatches() const { return mNumBatches; }

        ///
        /// 返回上一次 drawEnd() 到现在平均每次 draw()/drawIndexed() 的原语数
        ///
        size_t getNumPrimsPerBatch() const
        {
            return 0 == mNumBatches ? 0 : mNumPrims / mNumBatches;
        }

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
        //
        // ********************************************************************

        //@{

    private:

#if GN_DEBUG_BUILD
        AutoInit<bool,true> mEnableParameterCheck;
#else
        AutoInit<bool,false> mEnableParameterCheck;
#endif

    public:

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
        void enableParameterCheck( bool enable ) { mEnableParameterCheck = enable; }

        ///
        /// Get parameter check flag.
        ///
        bool parameterCheckEnabled() const { return mEnableParameterCheck; }

        ///
        /// Dump current renderer state to string. For debug purpose only.
        ///
        virtual void dumpCurrentState( StrA & ) const = 0;

        //@}

        // ********************************************************************
        //
        /// \name Ctor / dtor
        //
        // ********************************************************************

        //@{

    protected:

        ///
        /// ctor
        ///
        Renderer() : mSharedLib(0) {}

        ///
        /// Dtor
        ///
        virtual ~Renderer() {}

        //@}

        // ********************************************************************
        //
        /// \name Instance Manager
        //
        // ********************************************************************

        //@{

    private:

        SharedLib * mSharedLib;
        friend Renderer * createRenderer( RendererAPI );
        friend void deleteRenderer();

        //@}
    };

    ///
    /// (Re)Create a renderer.
    ///
    /// This function will release old renderer, then create a new one with new settings.
    ///
    Renderer * createRenderer( RendererAPI = API_AUTO );

    ///
    /// Delete renderer
    ///
    void deleteRenderer();
}}

#include "renderer.inl"

// *****************************************************************************
//                           End of renderer.h
// *****************************************************************************
#endif // __GN_GFX_RENDERER_H__
