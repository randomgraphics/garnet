#ifndef __GN_GFX_RENDERER_H__
#define __GN_GFX_RENDERER_H__
// *****************************************************************************
//! \file    renderer.h
//! \brief   Main renderer interface of GFX module
//! \author  chenlee (2005.9.30)
// *****************************************************************************

//!
//! Global renderer instance
//!
#define gRenderer (::GN::gfx::Renderer::sGetInstance())

//!
//! Pointer of global renderer instance
//!
#define gRendererPtr (::GN::gfx::Renderer::sGetInstancePtr())

//!
//! Implement static renderer data members
//!
#define GN_IMPLEMENT_RENDERER_STATIC_MEMBERS() \
    ::GN::Signal0<bool> GN::gfx::Renderer::sSigInit; \
    ::GN::Signal0<bool> GN::gfx::Renderer::sSigDeviceCreate; \
    ::GN::Signal0<bool> GN::gfx::Renderer::sSigDeviceRestore; \
    ::GN::Signal0<void> GN::gfx::Renderer::sSigDeviceDispose; \
    ::GN::Signal0<void> GN::gfx::Renderer::sSigDeviceDestroy; \
    ::GN::Signal0<void> GN::gfx::Renderer::sSigQuit; \
    ::GN::SharedLib     GN::gfx::Renderer::msSharedLib;

namespace GN { namespace gfx
{
    //!
    //! Renderer option structure.
    //!
    //! \sa Renderer::getOptions()
    //!
    struct RendererOptions
    {
        //!
        //! Display handle. No use on platform other than X Window. Default is zero.
        //!
        //! \note If zero, then default display will be used.
        //!
        HandleType displayHandle;

        //!
        //! Use external render window or not.
        //! 缺省为false.
        //!
        bool useExternalWindow;

        union
        {
            //!
            //! Handle of external render window.
            //! 缺省为0.
            //!
            //! \note Effective only if useExternalWindow is true.
            //!
            HandleType renderWindow;

            //!
            //! Handle of parent window. Can be NULL.
            //! 缺省为0.
            //!
            //! \note Effective only if useExternalWindow is false.
            //!
            void * parentWindow;
        };

        //!
        //! Monitor handle, effective only if useExternalWindow is false.
        //!
        //! - Should be HMONITOR on MS Window or pointer to Screen structure on X Windows.
        //! - 0 means using the monitor where parent window stays in. If monitorHandle
        //!   and parent window are both zero, primary monitor will be used.
        //! - 缺省为0.
        //!
        HandleType monitorHandle;

        //!
        //! 是否监视渲染窗口的大小，并自动对渲染设备作相应的调整
        //! 缺省为true.
        //!
        bool autoBackbufferResizing;

        //!
        //! Display mode for fullscreen mode. Ignored in windowed mode.
        //!
        //! \note For field equals zero, current display setting will be used.
        //!
        DisplayMode displayMode;

        //!
        //! Backbuffer width for windowed mode. Ignored in fullscreen mode.
        //! Zero means using client width of render window. If render window
        //! is also not avaiable, default width 640 will be used.
        //! 缺省为0.
        //!
        uint32_t windowedWidth;

        //!
        //! Backbuffer height for windowed mode. Ignored in fullscreen mode.
        //! Zero means using client height of render window. If render window
        //! is also not avaiable, default height 480 will be used.
        //! 缺省为0.
        //!
        uint32_t windowedHeight;

        //!
        //! Msaa type
        //!
        MsaaType msaa;

        //!
        //! fullscreen or windowed mode.
        //! 缺省为false.
        //!
        bool fullscreen;

        //!
        //! 是否同步刷新. 缺省为false.
        //!
        bool vsync;

        //! \name D3D only parameters
        //@{
        bool software;    //!< use software device. 缺省为false.
        bool reference;   //!< use reference device. 缺省为false.
        bool pure;        //!< use pure device. Default is false.
        bool multithread; //!< use multi-thread safe deivce. Default is false.
        //@}

        //! \name OGL only parameters
        //@{

        //!
        //! Restore display mode while render window is deactivated.
        //!
        //! Note that this is a OGL only parameter. For D3D, you may use
        //! "Enable Multi-mon Debugging" option in DirectX control panel,
        //! and startup your application through a debugger to make that
        //! option effective.
        //! 缺省为true.
        //!
        bool autoRestore;

        //@}

        //!
        //! Construct default render options
        //!
        RendererOptions()
            : displayHandle(0)
            , useExternalWindow(false)
            , parentWindow(0)
            , monitorHandle(0)
            , autoBackbufferResizing(true)
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

    //!
    //! Display descriptor.
    //!
    //! \sa RendererOptions, Renderer::getDispDesc()
    //!
    struct DispDesc
    {
        HandleType displayHandle;    //!< Display handle. For X Window only.
        HandleType windowHandle;     //!< Render window handle
        HandleType monitorHandle;    //!< Monitor handle.
        uint32_t width;              //!< Back buffer width
        uint32_t height;             //!< Back buffer height
        uint32_t depth;              //!< Back buffer depth
        uint32_t refrate;            //!< Screen refresh rate

        //!
        //! equality operator
        //!
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

    //! \def GNGFX_CAPS
    //! Define renderer caps

    //!
    //! renderer caps
    //!
    enum RendererCaps
    {
        #define GNGFX_CAPS(X) CAPS_##X,
        #include "rendererCapsMeta.h"
        #undef GNGFX_CAPS
        NUM_RENDERER_CAPS,
        CAPS_INVALID
    };

    //!
    //! Vertex shader capability flags
    //!
    enum VSCaps
    {
        VSCAPS_D3D_1_1   = 1<<0, //!< DirectX vs.1.1
        VSCAPS_D3D_2_0   = 1<<1, //!< DirectX vs.2.0
        VSCAPS_D3D_2_X   = 1<<2, //!< DirectX vs.2.x
        VSCAPS_D3D_3_0   = 1<<3, //!< DirectX vs.3.0
        VSCAPS_D3D_XVS   = 1<<4, //!< D3D xvs 3.0 (Xenon only)
        VSCAPS_OGL_ARB1  = 1<<5, //!< OpenGL ARB vertex program 1.0
        VSCAPS_OGL_GLSL  = 1<<6, //!< OpenGL shading language

        //!
        //! alias for all D3D vertex shader caps
        //!
        VSCAPS_D3D_ALL = VSCAPS_D3D_1_1 |
                         VSCAPS_D3D_2_0 |
                         VSCAPS_D3D_2_X |
                         VSCAPS_D3D_3_0 |
                         VSCAPS_D3D_XVS
    };

    //!
    //! Pixel shader capability flags
    //!
    enum PSCaps
    {
        PSCAPS_D3D_1_1   = 1<<0, //!< DirectX ps.1.1
        PSCAPS_D3D_1_2   = 1<<1, //!< DirectX ps.1.2
        PSCAPS_D3D_1_3   = 1<<2, //!< DirectX ps.1.3
        PSCAPS_D3D_1_4   = 1<<3, //!< DirectX ps.1.4
        PSCAPS_D3D_2_0   = 1<<4, //!< DirectX ps.2.0
        PSCAPS_D3D_2_X   = 1<<5, //!< DirectX ps.2.x
        PSCAPS_D3D_3_0   = 1<<6, //!< DirectX ps.3.0
        PSCAPS_D3D_XVS   = 1<<7, //!< D3D XPS 3.0 (Xenon only)
        PSCAPS_OGL_ARB1  = 1<<8, //!< OpenGL ARB pixel program 1.0
        PSCAPS_OGL_GLSL  = 1<<9, //!< OpenGL shading language

        //!
        //! alias for all D3D pixel shader caps
        //!
        PSCAPS_D3D_ALL = PSCAPS_D3D_1_1 |
                         PSCAPS_D3D_1_2 |
                         PSCAPS_D3D_1_3 |
                         PSCAPS_D3D_1_4 |
                         PSCAPS_D3D_2_0 |
                         PSCAPS_D3D_2_X |
                         PSCAPS_D3D_3_0 |
                         PSCAPS_D3D_XVS
    };

    //!
    //! 清屏标志
    //!
    enum ClearFlag
    {
        C_BUFFER       = 1,     //!< clear color buffer
        Z_BUFFER       = 2,     //!< clear z buffer
        S_BUFFER       = 4,     //!< clear stencil buffer
        ALL_BUFFERS    = 7      //!< clear all buffers
    };

    //!
    //! Options for Renderer::drawQuads
    //!
    enum DrawQuadOptions
    {
        //!
        //! 使用当前的渲染状态。
        //!
        //! By default, Renderer::drawQuads() will use a special render state block that
        //! suites for transparent quads:
        //!   - enable blending
        //!   - enable depth testing
        //!   - disable depth writing
        //!
        DQ_USE_CURRENT_RS = 1<<0,

        //!
        //! 使用当前的Vertex Shader。
        //!
        //! - 缺省情况下，Renderer::drawQuads() 会使用一个内置的vertex shader
        //! - 自定义的vertex shader应接受一组2D空间坐标和一组2D贴图坐标。
        //!
        DQ_USE_CURRENT_VS = 1<<1,

        //!
        //! 使用当前的Pixel Shader。
        //!
        //! 缺省情况下，Renderer::drawQuads() 会使用一个内置的Pixel Shader，直接直接输出
        //! 第0层贴图的颜色。
        //!
        DQ_USE_CURRENT_PS = 1<<2,

        //!
        //! 使用当前的Texture states.
        //!
        //! Effective only when using fixed function pipeline.
        //!
        DQ_USE_CURRENT_TS = 1<<3,

        //!
        //! position in window (post-transformed) space:
        //! (0,0) for left-up corner, (width,height) for right-bottom corner.
        //!
        //! By default, quad positios are in screen space. That is:
        //! (0,0) for left-up of the screen, and (1,1) for right-bottom of the screen)
        //!
        //! \note This option is meaningful only when DQ_USE_CURRENT_VS is _NOT_ set.
        //!
        DQ_WINDOW_SPACE = 1<<4,

        //!
        //! Use 3-D position. Default is 2-D position
        //!
        DQ_3D_POSITION = 1<<5,

        //!
        //! Disable blending. Default is enabled.
        //!
        DQ_OPAQUE = 1<<6,

        //!
        //! Enable depth write. Default is disabled.
        //!
        DQ_UPDATE_DEPTH = 1<<7,

        //!
        //! 上述 DQ_USE_CURRENT_XX 的集合
        //!
        DQ_USE_CURRENT = DQ_USE_CURRENT_RS | DQ_USE_CURRENT_VS | DQ_USE_CURRENT_PS | DQ_USE_CURRENT_TS
    };

    //!
    //! Options for Renderer::drawLines
    //!
    enum DrawLineOptions
    {
        //!
        //! 使用当前的渲染状态。
        //!
        //! By default, Renderer::drawLines() will use a special render state block that
        //! suites for colored lines
        //!
        DL_USE_CURRENT_RS = 1<<0,

        //!
        //! 使用当前的Vertex Shader。
        //!
        //! - 缺省情况下，Renderer::drawLines() 会使用一个内置的vertex shader
        //! - 自定义的vertex shader应接受一组3D空间坐标
        //!
        DL_USE_CURRENT_VS = 1<<1,

        //!
        //! 使用当前的Pixel Shader。
        //!
        //! 缺省情况下，Renderer::drawLines() 会使用一个内置的Pixel Shader
        //!
        DL_USE_CURRENT_PS = 1<<2,

        //!
        //! 使用当前的Texture states.
        //!
        //! Effective only when using fixed function pipeline.
        //!
        DL_USE_CURRENT_TS = 1<<3,

        //!
        //! position in window (post-transformed) space:
        //! (0,0) for left-up corner, (width,height) for right-bottom corner.
        //!
        //! By default, line positions are in object space.
        //!
        //! \note This option is meaningful only when DL_USE_CURRENT_VS is _NOT_ set.
        //!
        DL_WINDOW_SPACE = 1<<4,

        //!
        //! Using line strip. By default input points are treated as line list.
        //!
        DL_LINE_STRIP = 1<<5,

        //!
        //! 上述 DL_USE_CURRENT_XX 的集合
        //!
        DL_USE_CURRENT = DL_USE_CURRENT_RS | DL_USE_CURRENT_VS | DL_USE_CURRENT_PS | DL_USE_CURRENT_TS
    };

    //!
    //! Define rendering API
    //!
    enum RendererAPI
    {
        API_OGL,   //!< OpenGL
        API_D3D9,  //!< D3D9
        API_FAKE,  //!< Fake API
        NUM_RENDERER_API, //!< Number of avaliable API.
        API_AUTO, //!< determine rendering API automatically.
    };

    //!
    //! gfx模块的主接口类
    //!
    //! \nosubgrouping
    //!
    struct Renderer : public Singleton<Renderer>, public NoCopy
    {
        // ********************************************************************
        //!
        //! \name Renderer Signals
        //!
        //! - 信号可以被多次触发，且一定是严格按照如下的顺序：
        //!   <pre>
        //!                                         +---------------+
        //!                                        \|/              |
        //!                                         '               |
        //!   (start)-->init-->deviceCreate-->deviceRestore-->deviceDispose-->deviceDestroy-->quit-->(end)
        //!              .           .                                            |            |
        //!             /|\         /|\                                           |            |
        //!              |           +--------------------------------------------+            |
        //!              |                                                                     |
        //!              +---------------------------------------------------------------------+
        //!   </pre>
        //! - init发生后, 必定发生一个deviceCreate
        //! - deviceCreate发生后, 必定发生一个deviceRestore
        //! - 收到init/deviceCreate/deviceRestore信号说明渲染器ready to use。
        //! - 这些信号的标准使用方法如下：
        //!   - 收到init信号后, 创建所有图形资源。
        //!   - 收到deviceRestore信号后，填充图形资源的内容，如从磁盘读取贴图和模型。
        //!     但应避免创建新的资源，因为这个信号在程序生命期中有可能被多次触发。
        //!   - 收到quit信号后，删除所有的图形资源
        //!   - 通常情况下可以忽略deviceCreate, deviceDispose和deviceDestroy信号。
        //!     TODO: 它们的具体使用方法有待进一步说明。
        // ********************************************************************

        //@{

        //!
        //! Triggered right after renderer is created successfully, and ready to use.
        //!
        static GN_PUBLIC Signal0<bool> sSigInit;

        //!
        //! D3D/OGL device creation signal
        //!
        static GN_PUBLIC Signal0<bool> sSigDeviceCreate;

        //!
        //! Triggered after renderer is created or restored from last dispose successfully, and ready to use.
        //!
        //! (Re)load content of graphics resources. But do _NOT_ create new graphics
        //! resources here.
        //! - Only lockable resources (texture, vertex/index buffer) that have
        //!   neither system-copy no content loader need contexnt reloading.
        //! - No need to restore shaders, RSBs and VtxFmtHandle.
        //!
        static GN_PUBLIC Signal0<bool> sSigDeviceRestore;

        //!
        //! D3D/OGL device dispose signal
        //!
        static GN_PUBLIC Signal0<void> sSigDeviceDispose;

        //!
        //! D3D/OGL device destroy signal
        //!
        static GN_PUBLIC Signal0<void> sSigDeviceDestroy;

        //!
        //! Triggered right before renderer is about to be destroied.
        //!
        //! You must release _ALL_ graphics resourcesafter received this signal.
        //!
        static GN_PUBLIC Signal0<void> sSigQuit;

        //@}

        // ********************************************************************
        //
        //! \name Device Manager
        //
        // ********************************************************************

        //@{

    private:

        RendererOptions mOptions;

    protected:

        //!
        //! Update private renderer option variable.
        //!
        void setOptions( const RendererOptions & ro ) { mOptions = ro; }

    public:

        //!
        //! Change renderer options.
        //!
        //! \param ro
        //!     new renderer options
        //! \param forceDeviceRecreation
        //!     force a full device recreation
        //! \note
        //!     This function may trigger sSigDeviceRestore and/or sSigDeviceDispose.
        //!
        virtual bool changeOptions( const RendererOptions & ro, bool forceDeviceRecreation = false ) = 0;

        //!
        //! Get renderer options
        //!
        const RendererOptions & getOptions() const { return mOptions; }

        //@}

        // ********************************************************************
        //
        //! \name Display Manager
        //! \note
        //!     We provide two functions to get API specific rendering context:
        //!     - Sometime, you want to run some API specific codes,
        //!       for debug, test or any other purpose.
        //!     - You may use these functions to detect the current API
        //
        // ********************************************************************

        //@{

    private:

        DispDesc mDispDesc;

    protected:

        //!
        //! Update private diplay decriptor
        //!
        void setDispDesc( const DispDesc & desc )
        {
            mDispDesc = desc;
        }

    public:

        //!
        //! 获得当前的渲染窗口句柄
        //!
        const DispDesc & getDispDesc() const { return mDispDesc; }

        //!
        //! For D3D, return pointer to current D3D device; for OGL, return NULL.
        //!
        virtual void * getD3DDevice() const = 0;

        //!
        //! For OGL, return current rendering context; for D3D, return NULL.
        //!
        virtual void * getOGLRC() const = 0;

        //@}

        // ********************************************************************
        //
        //! \name Capabilities Manager
        //
        // ********************************************************************

        //@{

    protected:

        uint32_t mCaps[NUM_RENDERER_CAPS];

    public:

        //!
        //! Get render device caps
        //!
        virtual uint32_t getCaps( RendererCaps c ) const { GN_ASSERT( 0 <= c && c < NUM_RENDERER_CAPS ); return mCaps[c]; }

        //!
        //! Does specific shading language supported by hardware?
        //!
        virtual bool supportShader( ShaderType, ShadingLanguage ) = 0;

        //!
        //! Test compability of specific texture format
        //!
        virtual bool supportTextureFormat( TexType type, BitField usage, ClrFmt format ) const = 0;

        //@}

        // ********************************************************************
        //
        //! \name Resource Manager
        //
        // ********************************************************************

        //@{

        //!
        //! Create shader. Parameter 'entry' will be ignored for low-level shading language.
        //!
        virtual Shader *
        createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & entry = "main" ) = 0;

        //!
        //! Create vetex shader. Parameter 'entry' will be ignored for low-level shading language.
        //!
        Shader *
        createVtxShader( ShadingLanguage lang, const StrA & code, const StrA & entry = "main" );

        //!
        //! Create pixel shader. Parameter 'entry' will be ignored for low-level shading language.
        //!
        Shader *
        createPxlShader( ShadingLanguage lang, const StrA & code, const StrA & entry = "main" );

        //!
        //! Create new texture
        //! See TextureDesc for detail explaination of each fields in descriptor.
        //!
        virtual Texture *
        createTexture( const TextureDesc & desc, const TextureLoader & loader = TextureLoader() ) = 0;

        //!
        //! Load texture from file
        //!
        virtual Texture *
        createTextureFromFile( File & file ) = 0;

        //!
        //! Create new texture, with individual creation parameters.
        //!
        Texture *
        createTexture( TexType  type,
                       size_t   sx, size_t sy, size_t sz,
                       size_t   faces = 0,
                       size_t   levels = 0,
                       ClrFmt   format = FMT_DEFAULT,
                       BitField usage = 0,
                       const TextureLoader & loader = TextureLoader() )
        {
            TextureDesc desc = { type, (uint32_t)sx, (uint32_t)sy, (uint32_t)sz, (uint32_t)faces, (uint32_t)levels, format, usage };
            return createTexture( desc, loader );
        }

        //!
        //! Create 1D texture
        //!
        Texture *
        create1DTexture( size_t   sx,
                         size_t   levels = 0,
                         ClrFmt   format = FMT_DEFAULT,
                         BitField usage = 0,
                         const TextureLoader & loader = TextureLoader() )
        {
            return createTexture( TEXTYPE_1D, sx, 0, 0, 1, levels, format, usage, loader );
        }

        //!
        //! Create 2D texture
        //!
        Texture *
        create2DTexture( size_t   sx, size_t sy,
                         size_t   levels = 0,
                         ClrFmt   format = FMT_DEFAULT,
                         BitField usage = 0,
                         const TextureLoader & loader = TextureLoader() )
        {
            return createTexture( TEXTYPE_2D, sx, sy, 0, 1, levels, format, usage, loader );
        }

        //!
        //! Create 3D texture
        //!
        Texture *
        create3DTexture( size_t   sx, size_t sy, size_t sz,
                         size_t   levels = 0,
                         ClrFmt   format = FMT_DEFAULT,
                         BitField usage = 0,
                         const TextureLoader & loader = TextureLoader() )
        {
            return createTexture( TEXTYPE_3D, sx, sy, sz, 1, levels, format, usage, loader );
        }

        //!
        //! Create CUBE texture
        //!
        Texture *
        createCubeTexture( size_t   sx,
                           size_t   levels = 0,
                           ClrFmt   format = FMT_DEFAULT,
                           BitField usage = 0,
                           const TextureLoader & loader = TextureLoader() )
        {
            return createTexture( TEXTYPE_CUBE, sx, 0, 0, 6, levels, format, usage, loader );
        }

        //!
        //! Create vertex format handle.
        //!
        virtual VtxFmtHandle createVtxFmt( const VtxFmtDesc & ) = 0;

        //!
        //! Create new vertex buffer
        //!
        //! \param bytes
        //!     Size of vertex buffer in bytes.
        //! \param dynamic
        //!     Dynamic or static vertex buffer.
        //! \param sysCopy
        //!     has system copy or not
        //! \param loader
        //!     Optional content loader.
        //!
        virtual VtxBuf *
        createVtxBuf( size_t bytes,
                      bool   dynamic = false,
                      bool   sysCopy = true,
                      const  VtxBufLoader & loader = VtxBufLoader() ) = 0;

        //!
        //! Create new index buffer
        //!
        //! \param numIdx
        //!     number of indices
        //! \param dynamic
        //!     Dynamic or static vertex buffer.
        //! \param sysCopy
        //!     has system copy or not
        //! \param loader
        //!     Optional content loader.
        //!
        //! \note
        //!     每个索引固定占用16bit
        //!
        virtual IdxBuf *
        createIdxBuf( size_t numIdx,
                      bool   dynamic = false,
                      bool   sysCopy = true,
                      const  IdxBufLoader & loader = IdxBufLoader() ) = 0;

        //@}

        // ********************************************************************
        //
        //! \name Context Manager
        //
        // ********************************************************************

        //@{

    public:

        //!
        //! Set rendering state context
        //!
        virtual void setContextState( const ContextState & ) = 0;

        //!
        //! Set rendering data context
        //!
        virtual void setContextData( const ContextData & ) = 0;

        //!
        //! Rebind current rendering context to rendering device.
        //!
        //! \par
        //!     This function will "reset" some of states of low-level rendering device,
        //!     based on input field flags, to sync-up with current context stored in
        //!     in renderer.
        //! \par
        //!     Call this function to restore state of low-level rendering device, when
        //!     state is modified (by calling OpenGL functions or IDirect3DDevice methods,
        //!     for example), and you want to restore deivce to its previous states.
        //!
        virtual void rebindContextState( ContextState::FieldFlags ) = 0;

        //!
        //! Rebind current vertex-pixel data to rendering device.
        //!
        //! This function has analogy to rebindContextState().
        //!
        virtual void rebindContextData( ContextData::FieldFlags ) = 0;

        //!
        //! Get current render state block descriptor
        //!
        virtual const RenderStateBlockDesc & getCurrentRenderStateBlock() const = 0;

        // ********************************************************************
        //
        //! \name Helper functions to update rendering context.
        //!
        //! - See corresponding methods in ContextState and ContextData for usage
        //!   of each method.
        //! - Recommended way of call sequence is:
        //!   <code>
        //!     contextUpdateBegin();
        //!     ... // call context update methods here.
        //!     contextUpdateEnd();
        //!   </code>
        //! - Call single update method out side of contextUpdateBegin()
        //
        // ********************************************************************

        //@{

    private:

        ContextState         mHelperContextState;
        ContextData          mHelperContextData;
        AutoInit<bool,false> mHelperUpdateBegun;

    public:

        //!
        //! start context and VP data update
        //!
        inline void contextUpdateBegin();

        //!
        //! end context and VP data update, flush modified context and data to renderer.
        //!
        inline void contextUpdateEnd();

        inline void setShader( ShaderType type, const Shader * shader );
        inline void setShaders( const Shader * const shaders[] );
        inline void setShaders( const Shader * vtxShader, const Shader * pxlShader );
        inline void setShaderHandles( ShaderDictionary::HandleType vtxShader, ShaderDictionary::HandleType pxlShader );
        inline void setVtxShader( const Shader * s );
        inline void setVtxShaderHandle( ShaderDictionary::HandleType h );
        inline void setPxlShader( const Shader * s );
        inline void setPxlShaderHandle( ShaderDictionary::HandleType h );
        inline void setRenderStateBlock( const RenderStateBlockDesc & );
        inline void setRenderState( RenderState state, int32_t value );
        inline void setColorBuffer( size_t index, const Texture * texture, size_t face = 0, size_t level = 0, size_t slice = 0 );
        inline void setDepthBuffer( const Texture * texture, size_t face = 0, size_t level = 0, size_t slice = 0 );
        inline void setMsaa( MsaaType );
        inline void setViewport( const Rectf & );
        inline void setViewport( float left, float top, float width, float height );
        inline void setTextureStateBlock( const TextureStateBlockDesc & );
        inline void setTextureState( size_t stage, TextureState state, TextureStateValue value );

        inline void setTexture( size_t stage, const Texture * tex );
        inline void setTextureHandle( size_t stage, TextureDictionary::HandleType tex );
        inline void setTextures( const Texture * const texlist[], size_t start, size_t count );
        inline void setTextureHandles( const TextureDictionary::HandleType texlist[], size_t start, size_t count );
        inline void setVtxFmt( VtxFmtHandle );
        inline void setVtxBuf( size_t index, const VtxBuf * buffer, size_t stride );
        inline void setIdxBuf( const IdxBuf * );

        //@}

        // ********************************************************************
        //
        //! \name Drawing Manager
        //
        // ********************************************************************

        //@{

    protected:

        AutoInit<size_t,0> mNumPrims; //!< Number of primitives per frame.
        AutoInit<size_t,0> mNumDraws; //!< Number of draws per frame.

    public :

        //!
        //! 开始绘图操作.
        //!
        //! 所有的绘图操作都必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        virtual bool drawBegin() = 0;

        //!
        //! 结束绘图操作. Similar as EndScene() followed by Present()
        //!
        //! call drawEnd() <b>if and only if</b> drawBegin() returns true.
        //!
        virtual void drawEnd() = 0;

        //!
        //! 本函数的功能类似于OpenGL中的glFinish()
        //!
        //! \note 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        virtual void drawFinish() = 0;

        //!
        //! 清屏操作
        //!
        //! \param flags 清屏标志, see ClearFlag
        //! \param c     背景色
        //! \param z     深度值
        //! \param s     模板值
        //!
        //! \note Can be called outside of drawBegin()/drawEnd() scope.
        //!
        virtual void
        clearScreen( const Vector4f & c = Vector4f(0,0,0,1),
                     float z = 1.0f, uint32_t s = 0,
                     BitField flags = C_BUFFER | Z_BUFFER ) = 0;

        //!
        //! Draw indexed primitives.
        //!
        //! \param prim
        //!     primititive type
        //! \param numPrims
        //!     number of primitives
        //! \param startVtx
        //!     vertex index into vertex buffer that index "0" will be refering to.
        //! \param minVtxIdx, numVtx
        //!     define effective range in vertex buffer, starting from startVtx.
        //! \param startIdx
        //!     index into index buffer of the first index
        //!
        //! \note 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numPrims,
                                  size_t        startVtx,
                                  size_t        minVtxIdx,
                                  size_t        numVtx,
                                  size_t        startIdx ) = 0;

        //!
        //! Draw non-indexed primitives.
        //!
        //! \param prim
        //!     primititive type
        //! \param numPrims
        //!     number of primitives
        //! \param startVtx
        //!     index into vertex buffer of the first vertex.
        //!
        //! \note 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        virtual void draw( PrimitiveType prim,
                           size_t        numPrims,
                           size_t        startVtx ) = 0;

        //!
        //! draw on-indexed primitives with user-defined data array
        //!
        virtual void drawIndexedUp(
                             PrimitiveType    prim,
                             size_t           numPrims,
                             size_t           numVertices,
                             const void *     vertexData,
                             size_t           strideInBytes,
                             const uint16_t * indexData ) = 0;

        //!
        //! draw on-indexed primitives with user-defined data array
        //!
        virtual void drawUp( PrimitiveType prim,
                             size_t        numPrims,
                             const void *  vertexData,
                             size_t        strideInBytes ) = 0;

        //!
        //! Draw quads
        //!
        //! \param options
        //!     渲染选项，详见 DrawQuadOptions。Set to 0 to use default options
        //! \param positions, posStride
        //!     顶点坐标数据，由一系列的顶点组成。4个顶点表示一个矩形。
        //!     选项 DQ_WINDOW_SPACE 和 DQ_3D_POSITION 会影响坐标的含义。
        //!     Note "posStride" is stride of one vertex.
        //! \param texcoords, texStride
        //!     贴图坐标数组，由一系列的2D顶点组成。4个顶点表示一个矩形。
        //!     Note "texStride" is stride of one vertex.
        //!     Specify texcoords to NULL, if you want non-textured quad.
        //!     texStride is be ignored in this case.
        //! \param colors, clrStride
        //!     顶点颜色数组，由一系列的BGRA32颜色值组成。4个顶点表示一个矩形。
        //!     Note "clrStride" is stride of one vertex.
        //!     Set colors to NULL, to specify pure white for all vertices.
        //! \param count
        //!     Number of quads.
        //!
        virtual void drawQuads(
            BitField options,
            const void * positions, size_t posStride,
            const void * texcoords, size_t texStride,
            const void * colors, size_t clrStride,
            size_t count ) = 0;

        //!
        //! Draw quads, with same stride for positions, texcoords and colors
        //!
        void drawQuads(
            BitField options,
            const void * positions, const void * texcoords, const void * colors, size_t stride,
            size_t count )
        {
            drawQuads( options, positions, stride, texcoords, stride, colors, stride, count );
        }

        //!
        //! Draw single 2D textured quad.
        //!
        //! \note This function may not very effecient.
        //!
        void draw2DTexturedQuad(
            BitField options,
            double left = 0.0, double top = 0.0, double right = 1.0, double bottom = 1.0,
            double leftU = 0.0, double topV = 0.0, double rightU = 1.0, double bottomV = 1.0 )
        {
            float x1 = (float)left;
            float y1 = (float)top;
            float x2 = (float)right;
            float y2 = (float)bottom;
            float u1 = (float)leftU;
            float v1 = (float)topV;
            float u2 = (float)rightU;
            float v2 = (float)bottomV;

            Vector2f pos[4];
            Vector2f tex[4];

            pos[0].set( x1, y1 );
            pos[1].set( x2, y1 );
            pos[2].set( x2, y2 );
            pos[3].set( x1, y2 );

            tex[0].set( u1, v1 );
            tex[1].set( u2, v1 );
            tex[2].set( u2, v2 );
            tex[3].set( u1, v2 );

            drawQuads( options&(~DQ_3D_POSITION), pos, sizeof(Vector2f), tex, sizeof(Vector2f), 0, 0, 1 );
        }

        //!
        //! Draw single 2D solid quad.
        //!
        //! \note This function may not very effecient.
        //!
        void draw2DSolidQuad(
            BitField options,
            double left = 0.0, double top = 0.0, double right = 1.0, double bottom = 1.0,
            uint32_t color = 0xFFFFFFFF )
        {
            float x1 = (float)left;
            float y1 = (float)top;
            float x2 = (float)right;
            float y2 = (float)bottom;

            Vector2f pos[4];
            pos[0].set( x1, y1 );
            pos[1].set( x2, y1 );
            pos[2].set( x2, y2 );
            pos[3].set( x1, y2 );

            uint32_t colors[] = { color, color, color, color };

            drawQuads( options&(~DQ_3D_POSITION), pos, sizeof(Vector2f), 0, 0, colors, sizeof(uint32_t), 1 );
        }

        //!
        //! Draw line segments
        //!
        //! \param options
        //!     渲染选项，详见 DrawLineOptions。Set to 0 to use default options
        //! \param positions
        //!     顶点坐标数据，由一系列的顶点组成。2个顶点表示一个矩形。
        //!     选项 DQ_WINDOW_SPACE 和 DQ_3D_POSITION 会影响坐标的含义。
        //! \param stride
        //!     stride of one vertex.
        //! \param count
        //!     Number of line segments (note: _NOT_ number of points)
        //! \param color
        //!     line color. in FMT_BGRA32 format.
        //! \param model, view, proj
        //!     Transformation matrices. Ignored when using DL_WINDOW_SPACE.
        //!
        virtual void drawLines(
            BitField options,
            const void * positions,
            size_t stride,
            size_t count,
            uint32_t color,
            const Matrix44f & model,
            const Matrix44f & view,
            const Matrix44f & proj ) = 0;

        //!
        //! 在屏幕上指定的位置绘制2D字符串.
        //!
        //! - 函数可以处理中英文的混合字符串，但运行速度较慢，主要为测试而用。
        //!   文字的高度固定为16个象素，宽度为8(English)/16(Chinese)个象素。
        //! - 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        //! \param text  待绘制度字符串
        //! \param x, y  第一个字符左上角的窗口坐标
        //!              - 屏幕左上角为(0,0)，右下角为(width,height)
        //!              - 被绘制的字符串的坐标位置以第一个字符的左上角为准。
        //! \param color 文字颜色
        //!
        virtual void
        drawDebugTextA( const char * text, int x, int y,
                        const Vector4f & color = Vector4f(1,1,1,1) ) = 0;

        //!
        //!  绘制unicode文字
        //!
        //! \sa drawDebugTextA()
        //!
        //! \note 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        virtual void
        drawDebugTextW( const wchar_t * text, int x, int y,
                        const Vector4f & color = Vector4f(1,1,1,1) ) = 0;

        //!
        //! 返回上一次 drawEnd() 到现在所绘制的原语的个数
        //!
        size_t getNumPrimitives() const { return mNumPrims; }

        //!
        //! 返回上一次 drawEnd() 到现在 draw() / drawindexed() 的次数
        //!
        size_t getNumDraws() const { return mNumDraws; }

        //!
        //! 返回上一次 drawEnd() 到现在平均每次 draw()/drawIndexed() 的原语数
        //!
        size_t getNumPrimsPerDraw() const
        {
            return 0 == mNumDraws ? 0 : mNumPrims / mNumDraws;
        }

        //@}

        // ********************************************************************
        //
        //! \name Misc. utilities
        //
        // ********************************************************************

        //@{

    private:

#if GN_DEBUG
        AutoInit<bool,true> mEnableParameterCheck;
#else
        AutoInit<bool,false> mEnableParameterCheck;
#endif

    public:

        //!
        //! This function is provided because different API has different ways
        //! to compose projection matrix.
        //!
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

        //!
        //! This function is provided because different API has different ways
        //! to compose projection matrix.
        //!
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

        //!
        //! Enable/Disable parameter check for performance critical functions.
        //!
        //! Enabled by default for debug build; disabled by default for release build.
        //!
        void enableParameterCheck( bool enable ) { mEnableParameterCheck = enable; }

        //!
        //! Get parameter check flag.
        //!
        bool isParameterCheckEnabled() const { return mEnableParameterCheck; }

        //!
        //! Dump current renderer state to string. For debug purpose only.
        //!
        virtual void dumpCurrentState( StrA & ) const = 0;

        //@}

        // ********************************************************************
        //
        //! \name Ctor / dtor
        //
        // ********************************************************************

        //@{

    protected:

        //!
        //! ctor
        //!
        Renderer() {}

        //!
        //! Dtor
        //!
        virtual ~Renderer() {}

        //@}

        // ********************************************************************
        //
        //! \name Instance Manager
        //
        // ********************************************************************

        //@{

    private:

        static GN_PUBLIC SharedLib msSharedLib;
        friend Renderer * createRenderer( const RendererOptions &, RendererAPI );
        friend void deleteRenderer();

        //@}
    };

    //!
    //! (Re)Create a renderer.
    //!
    //! This function will release old renderer, then create a new one with new settings.
    //!
    Renderer * createRenderer( const RendererOptions &, RendererAPI = API_AUTO );

    //!
    //! Delete renderer
    //!
    void deleteRenderer();
}}

#include "renderer.inl"

// *****************************************************************************
//                           End of renderer.h
// *****************************************************************************
#endif // __GN_GFX_RENDERER_H__
