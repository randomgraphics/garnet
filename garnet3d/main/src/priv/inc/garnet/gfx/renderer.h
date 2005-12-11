#ifndef __GN_GFX_RENDERER_H__
#define __GN_GFX_RENDERER_H__
// *****************************************************************************
//! \file    renderer.h
//! \brief   Main renderer interface of GFX module
//! \author  chenlee (2005.9.30)
// *****************************************************************************

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
        bool software;   //!< using software device. 缺省为false.
        bool reference;  //!< using reference device. 缺省为false.
        //@}

        //! \name OGL only parameters
        //@{

        //!
        //! Restore display mode while render window is deactivated.
        //!
        //! Note that this is a OGL only parameter. For D3D, you may use
        //! "Enable Multi-mon Debugging" option in DirectX control panel,
        //! and startup your application through an debugger to make that
        //! option effective.
        //! 缺省为true.
        //!
        bool autoRestore;

        //@}

        //!
        //! Construct a default device settings
        //!
        RendererOptions()
            : displayHandle(0)
            , useExternalWindow(false)
            , parentWindow(0)
            , monitorHandle(0)
            , autoBackbufferResizing(true)
            , windowedWidth(0)
            , windowedHeight(0)
            , fullscreen(false)
            , vsync(false)
            , software(false)
            , reference(false)
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
        NUM_CAPS,
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
    //! Render parameter type
    //!
    enum RenderParameterType
    {
        RPT_LIGHT0_POSITION,     //!< Light 0 position
        RPT_LIGHT0_DIFFUSE,      //!< Light 0 diffuse color
        RPT_LIGHT_FIRST = RPT_LIGHT0_POSITION, //!< first light property
        RPT_LIGHT_LAST = RPT_LIGHT0_DIFFUSE,   //!< last light property

        RPT_MATERIAL_DIFFUSE,    //!< Diffuse material color
        RPT_MATERIAL_SPECULAR,   //!< Specular material color 
        RPT_MATERIAL_FIRST = RPT_MATERIAL_DIFFUSE, //!< first material property
        RPT_MATERIAL_LAST = RPT_MATERIAL_SPECULAR, //!< last material property

        RPT_TRANSFORM_WORLD,     //!< World transformation
        RPT_TRANSFORM_VIEW,      //!< Camera transformation
        RPT_TRANSFORM_PROJ,      //!< Projection transformation
        RPT_TRANSFORM_VIEWPORT,  //!< 4 floats: left, top, width, height; ranging from 0.0 to 1.0.
        RPT_TRANSFORM_FIRST = RPT_TRANSFORM_WORLD,   //!< first transform property
        RPT_TRANSFORM_LAST = RPT_TRANSFORM_VIEWPORT, //!< last transform property

        NUM_RENDER_PARAMETER_TYPES,   //!< Number of avaiable parameters

        RPT_INVALID,             //!< Indicates invalid state.

        // Below are parameter masks that are purely for push/popRenderParameter()

        _RPM_MASK      = 0x80000000,         //!< Mask bit

        RPM_LIGHT     = (1<<0) | _RPM_MASK, //!< all light parameters
        RPM_MATERIAL  = (1<<1) | _RPM_MASK, //!< all material parameters
        RPM_TRANSFORM = (1<<2) | _RPM_MASK, //!< all transform parameters
        RPM_ALL       = 0xFFFFFFFF,         //!< all parameters
    };

    //!
    //! Convert render parameter type to string.
    //! If failed, return "BAD_RENDER_PARAMETER_TYPE".
    //!
    const char * rpt2Str( RenderParameterType );

    //!
    //! Convert render parameter type to string. Return false, if failed.
    //!
    bool rpt2Str( StrA & result, RenderParameterType type );

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
        //! position in window space: (0,0) for left-up corner, (width,height) for right-bottom corner.
        //!
        //! By default, quad positios are in screen space. That is:
        //! (0,0) for left-up of the screen, and (1,1) for right-bottom of the screen)
        //!
        //! \note This option is meaningful only when DQ_USE_CURRENT_VS is _NOT_ set.
        //!
        DQ_WINDOW_SPACE = 1<<0,

        //!
        //! 使用当前的渲染状态。
        //!
        //! By default, Renderer::drawQuad() will use a special render state block that
        //! suites for transparent quads:
        //!   - enable blending
        //!   - enable depth testing
        //!   - disable depth writing
        //!
        DQ_USE_CURRENT_RS = 1<<1,

        //!
        //! 使用当前的Vertex Shader。
        //!
        //! - 缺省情况下，Renderer::drawQuad() 会使用一个内置的vertex shader
        //! - 自定义的vertex shader应接受一组2D空间坐标和一组2D贴图坐标。
        //!
        DQ_USE_CURRENT_VS = 1<<2,

        //!
        //! 使用当前的Pixel Shader。
        //!
        //! 缺省情况下，Renderer::drawQuad() 会使用一个内置的Pixel Shader，直接直接输出
        //! 第0层贴图的颜色。
        //!
        DQ_USE_CURRENT_PS = 1<<3,

        //!
        //! 上述 DQ_USE_CURRENT_XX 的集合
        //!
        DQ_USE_CURRENT = DQ_USE_CURRENT_RS | DQ_USE_CURRENT_VS | DQ_USE_CURRENT_PS
    };

    //!
    //! gfx模块的主接口类
    //!
    //! \nosubgrouping
    //!
    struct Renderer : public NoCopy
    {
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
        //! Triggered after rendering device is restored to normal stage.
        //!
        //! - Resources that has neither system-copy nor content loader will lost
        //!   contents after device reset/lost. You have to reload them manually.
        //!   Note that only contents are lost, not resources themselves.
        //! - Shaders and render-state-blocks will survive device reset/lost.
        //!
        Signal0<bool> sigDeviceRestore;

        //!
        //! Triggered right before invalidating of rendering device.
        //!
        //! \note
        //! - After receiving this signal, no rendering function should be called,
        //!   before you receive sigDeviceRestore.
        //! - This signal will also be triggered, before rendering device is
        //!   destroyed.
        //!
        Signal0<void> sigDeviceDispose;

        //!
        //! Change renderer options.
        //!
        //! \param ro
        //!     new rendeer options
        //! \param forceDeviceRecreation
        //!     force a full device recreation
        //! \note
        //!     This function may trigger sigDeviceRestore.
        //!
        virtual bool changeOptions( RendererOptions ro, bool forceDeviceRecreation = false ) = 0;

        //!
        //! Get current renderer options
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

        //!
        //! caps descriptor
        //!
        class CapsDesc
        {
            uint32_t value; //!< caps value
#if GN_DEBUG
            bool     valid; //!< caps is initialized or not.
#endif

        public:
            CapsDesc()
#if GN_DEBUG
                : valid(false)
#endif
            {}

            //!
            //! get caps value
            //!
            uint32_t get() const
            {
                GN_ASSERT( valid );
                return value;
            }

            //!
            //! set caps value
            //!
            void set( uint32_t value );

            //!
            //! reset caps value (to invalid state)
            //!
            void reset();
        };

    private:

        CapsDesc mCaps[NUM_CAPS];

    protected:

        //!
        //! update caps. called by child class.
        //!
        void setCaps( RendererCaps, uint32_t );

        //!
        //! reset(clear) all caps. called by child class.
        //!
        void resetAllCaps();

    public:

        //!
        //! Get render device caps
        //!
        uint32_t getCaps( RendererCaps c ) const
        {
            GN_ASSERT( 0 <= c && c < NUM_CAPS );
            return mCaps[c].get();
        }

        //@}

        // ********************************************************************
        //
        //! \name Shader Manager
        //
        // ********************************************************************

        //@{

        //!
        //! Does specific shading language supported by hardware?
        //!
        virtual bool supportShader( ShaderType, ShadingLanguage ) = 0;

        //!
        //! Create vetex shader. Parameter 'entry' will be ignored for low-level shading language.
        //!
        virtual Shader *
        createVtxShader( ShadingLanguage lang, const StrA & code, const StrA & entry = "main" ) = 0;

        //!
        //! Create pixel shader. Parameter 'entry' will be ignored for low-level shading language.
        //!
        virtual Shader *
        createPxlShader( ShadingLanguage lang, const StrA & code, const StrA & entry = "main" ) = 0;

        //!
        //! Create shader. Parameter 'entry' will be ignored for low-level shading language.
        //!
        //! \note Inlined function implemented in renderer.inl
        //!
        Shader *
        createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & entry = "main" );

        //!
        //! Bind programmable vertex shader to rendering device. Set to NULL to use
        //! fixed pipeline.
        //!
        virtual void bindVtxShader( const Shader * ) = 0;

        //!
        //! Bind programmable pixel to rendering device. Set to NULL to use
        //! fixed pipeline.
        //!
        virtual void bindPxlShader( const Shader * ) = 0;

        //!
        //! Bind programmable shader to rendering device. Set to NULL to use
        //! fixed pipeline.
        //!
        virtual void
        bindShaders( const Shader * vtxShader, const Shader * pxlShader ) = 0;

        //@}

        // ********************************************************************
        //
        //! \name Render State Block Manager
        //
        // ********************************************************************

        //@{

        //!
        //! request a render state block object with specific render state block structure.
        //! Return 0, if failed.
        //!
        virtual uint32_t
        createRenderStateBlock( const RenderStateBlockDesc & ) = 0;

        //!
        //! Bind render state block to rendering device
        //!
        virtual void bindRenderStateBlock( uint32_t ) = 0;

        //!
        //! Get current render state.
        //!
        //! \note Better not use this functio in performance critical code.
        //!
        virtual void getCurrentRenderStateBlock( RenderStateBlockDesc & ) const = 0;

        //!
        //! Another style of getting current render state.
        //!
        //! \note This is even slower because of extra data copy.
        //!
        RenderStateBlockDesc getCurrentRenderStateBlock() const
        {
            RenderStateBlockDesc result;
            getCurrentRenderStateBlock(result);
            return result;
        }

        //!
        //! Update individual render state.
        //!
        //! \return
        //!     Return the render state block handler that represents current render state.
        //!     Return 0, if failed.
        //!
        //! \note
        //!     This function is purely for coding convenience.
        //!     Please use render state block at performance critical section.
        //!
        virtual uint32_t setRenderState( RenderState state, RenderStateValue value ) = 0;

        //!
        //! Update individual texture state.
        //!
        //! \return
        //!     Return the render state block handler that represents current render state.
        //!     Return 0, if failed.
        //!
        //! \note
        //!     - This function is purely for coding convenience.
        //!       Please use render state block at performance critical section.
        //!     - Also, Texture states are only used for fixed function pipeline.
        //!
        virtual uint32_t setTextureState( uint32_t stage, TextureState state, TextureStateValue value ) = 0;

        //@}

        // ********************************************************************
        //
        //! \name Texture Manager
        //
        // ********************************************************************

        //@{

        //!
        //! Create new texture.
        //!
        //! \param textype     texture type
        //! \param sx, sy, sz  texture size
        //! \param levels      how many mipmap levels?
        //!                    "0" means generate full mipmap levels down to 1x1
        //! \param format      texture format, FMT_DEFAULT means
        //!                    using default/appropriating format of current
        //!                    rendering hardware.
        //! \param usage       texture usage, one or combination of TexUsage
        //! \param loader      Optional content loader
        //! \note
        //!    - sy/sz will be ignored for 1D/Cube texture,
        //!    - sz will be ignored for 2D texture.
        //!
        virtual Texture *
        createTexture( TexType textype,
                       uint32_t sx, uint32_t sy, uint32_t sz,
                       uint32_t levels = 0,
                       ClrFmt format = FMT_DEFAULT,
                       uint32_t usage = 0,
                       const TextureLoader & loader = TextureLoader() ) = 0;

        //!
        //! Create 1D texture
        //!
        Texture *
        create1DTexture( uint32_t sx,
                         uint32_t levels = 0,
                         ClrFmt format = FMT_DEFAULT,
                         uint32_t usage = 0,
                         const TextureLoader & loader = TextureLoader() )
        {
            return createTexture( TEXTYPE_1D, sx, 0, 0, levels, format, usage, loader );
        }

        //!
        //! Create 2D texture
        //!
        Texture *
        create2DTexture( uint32_t sx, uint32_t sy,
                         uint32_t levels = 0,
                         ClrFmt format = FMT_DEFAULT,
                         uint32_t usage = 0,
                         const TextureLoader & loader = TextureLoader() )
        {
            return createTexture( TEXTYPE_2D, sx, sy, 0, levels, format, usage, loader );
        }

        //!
        //! Create 3D texture
        //!
        Texture *
        create3DTexture( uint32_t sx, uint32_t sy, uint32_t sz,
                         uint32_t levels = 0,
                         ClrFmt format = FMT_DEFAULT,
                         uint32_t usage = 0,
                         const TextureLoader & loader = TextureLoader() )
        {
            return createTexture( TEXTYPE_3D, sx, sy, sz, levels, format, usage, loader );
        }

        //!
        //! Create CUBE texture
        //!
        Texture *
        createCubeTexture( uint32_t sx,
                           uint32_t levels = 0,
                           ClrFmt format = FMT_DEFAULT,
                           uint32_t usage = 0,
                           const TextureLoader & loader = TextureLoader() )
        {
            return createTexture( TEXTYPE_CUBE, sx, 0, 0, levels, format, usage, loader );
        }

        //!
        //! Load texture from file
        //!
        virtual Texture *
        createTextureFromFile( File & file ) = 0;

        //!
        //! bind textures ( from stage[start] to stage[start+numtex-1] )
        //!
        //! \param texlist texture list
        //! \param start   start stage
        //! \param numtex  number of textures
        //!
        virtual void
        bindTextures( const Texture * const texlist[],
                      uint32_t start, uint32_t numtex ) = 0;


        //!
        //! bind one texture
        //!
        void bindTexture( uint32_t stage, const Texture * tex )
        {
            bindTextures( &tex, stage, 1 );
        }

        //@}

        // ********************************************************************
        //
        //! \name Renderable Buffer Manager
        //
        // ********************************************************************

        //@{

        //!
        //! Create vertex bindings.
        //!
        virtual uint32_t createVtxBinding( const VtxFmtDesc & ) = 0;

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

        //!
        //! Bind vertex bindings
        //!
        virtual void bindVtxBinding( uint32_t ) = 0;

        //!
        //! Bind a serias vertex buffers to rendering device.
        //!
        //! \param buffers  Buffer list.
        //! \param start    Stream index of the 1st buffer in buffer list.
        //! \param count    Stream count in buffer list.
        //!
        virtual void
        bindVtxBufs( const VtxBuf * const buffers[], size_t start, size_t count ) = 0;

        //!
        //! Bind a vertex buffers to rendering device, with user-specified stride.
        //!
        //! \note
        //! - By default, vertex buffer stride will be determined by current vertex binding.
        //!   Use this function only when your want to use custom stride for your vertex buffer.
        //! - To bind a single vertex buffer with default stride, please use
        //!   bindVtxBufs( &yourBuffer, yourIndex, 1 ) instead.
        //!
        virtual void
        bindVtxBuf( size_t index, const VtxBuf * buffer, size_t stride ) = 0;

        //!
        //! Bind index buffer to rendering device
        //!
        virtual void bindIdxBuf( const IdxBuf * ) = 0;

        //@}

        // ********************************************************************
        //
        //! \name Misc. Render Parameter Manager.
        //
        // ********************************************************************

        //@{

    protected:

        //!
        //! Render parameter value type
        //!
        enum RenderParameterValueType
        {
            RPVT_FLOAT, //!< float array
            NUM_OF_RENDER_PARAMETER_VALUE_TYPES, //!< number of avaliable types.
            RPVT_INVALID, //!< invalid type
        };

        //!
        //! Render parameter value
        //!
        struct RenderParameterValue
        {
            RenderParameterValueType type;            //!< value type
            float                    valueFloats[16]; //!< float values
            size_t                   count;           //!< float count

#if GN_DEBUG
            //!
            //! ctor
            //!
            RenderParameterValue() : type(RPVT_INVALID), count(0) {}
#endif
        };

    protected:

        //!
        //! Get render parameter value
        //!
        const RenderParameterValue & getRenderParameter( RenderParameterType type ) const 
        {
            GN_ASSERT( 0 <= type && type < NUM_RENDER_PARAMETER_TYPES );
            return mRenderParameters[type].top();
        }

        //!
        //! Get render parameter dirty set
        //!
        const std::set<RenderParameterType> & getRpDirtySet() const { return mRenderParameterDirtySet; }

        //!
        //! Clear render parameter dirty set
        //!
        void clearRpDirtySet() const { mRenderParameterDirtySet.clear(); }

    private:

        //!
        //! Fixed-size stack container that do not perform any runtime
        //! memory allocation/deallocation.
        //!
        template< class T, size_t MAX_DEPTH = 256 >
        class FixStack
        {
            T      mTop;              //!< top element
            T      mStack[MAX_DEPTH]; //!< element stack
            size_t mDepth;            //!< current depth
        public :

            //!
            //! default constructor
            //!
            FixStack() : mDepth(0) {}

            //!
            //! get current depth
            //!
            size_t depth() const { return mDepth; }

            //!
            //! push the top element into stack
            //!
            void push()
            {
                GN_ASSERT( mDepth < MAX_DEPTH );
                mStack[mDepth] = mTop;
                ++mDepth;
            }

            //!
            //! pop out the stack to top element
            //!
            T & pop()
            {
                GN_ASSERT( mDepth > 0 );
                mTop = mStack[--mDepth];
                return mTop;
            }

            //!
            //! return top element
            //!
            T & top() { return mTop; }

            //!
            //! return constant top element
            //!
            const T & top() const { return mTop; }
        };

        //!
        //! Render parameter structure
        //!
        typedef FixStack<RenderParameterValue> RenderParameter;

        RenderParameter mRenderParameters[NUM_RENDER_PARAMETER_TYPES];

        // Render parameter dirty set
        mutable std::set<RenderParameterType> mRenderParameterDirtySet;

    public:

        //!
        //! Set parameter value. Can't use RPM_XXX here.
        //!
        void setRenderParameter( RenderParameterType, const float *, size_t );

        //!
        //! Push parameter value. Can use any value of RPT_XXX and RPM_XXX.
        //!
        void pushRenderParameter( RenderParameterType );

        //!
        //! Pop parameter value. Can use any value of RPT_XXX and RPM_XXX.
        //!
        void popRenderParameter( RenderParameterType );

        //!
        //! setup viewport
        //!
        void setViewport( float left, float top, float width, float height );

        //!
        //! This function is provided because different API has different ways
        //! to compose projection matrix.
        //!
        virtual Matrix44f &
        composePerspectiveMatrix( Matrix44f & result,
                                  float fovy,
                                  float ratio,
                                  float znear,
                                  float zfar ) const = 0;
        //!
        //! This function is provided because different API has different ways
        //! to compose projection matrix.
        //!
        virtual Matrix44f &
        composeOrthoMatrix( Matrix44f & result,
                            float left,
                            float bottom,
                            float width,
                            float height,
                            float znear,
                            float zfar ) const = 0;

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
        //! set render target texture
        //!
        //! \param index
        //!     render target index, starting from 0
        //! \param texture
        //!     target texture, must be created with flag TEXUSAGE_RENDERTARGET. Set
        //!     this parameter to NULL will reset to default target (back buffer
        //!     for RT0 and null for others.
        //! \param face
        //!     Ignored if target_texture is not cubemap.
        //!
        virtual void setRenderTarget( size_t index,
                                      const Texture * texture,
                                      TexFace face = TEXFACE_PX ) = 0;

        //!
        //! set render target texture
        //!
        //! \param texture
        //!     Target texture, must be created with flag TEXUSAGE_DEPTH. Set this
        //!     parameter to NULL will reset to default depth buffer.
        //! \param face
        //!     Ignored if target_texture is not cubemap.
        //!
        virtual void setRenderDepth( const Texture * texture,
                                     TexFace face = TEXFACE_PX ) = 0;

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
                     uint32_t flags = C_BUFFER | Z_BUFFER ) = 0;

        //!
        //! Draw indexed primitives.
        //!
        //! \param prim
        //!     primititive type
        //! \param numPrim
        //!     number of primitives
        //! \param baseVtx
        //!     vertex index into vertex buffer that index "0" will be refering to.
        //! \param minVtxIdx, numVtx
        //!     define effective range in vertex buffer, starting from baseVtx.
        //! \param startIdx
        //!     index into index buffer of the first index
        //!
        //! \note 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numPrim,
                                  size_t        baseVtx,
                                  size_t        minVtxIdx,
                                  size_t        numVtx,
                                  size_t        startIdx ) = 0;

        //!
        //! Draw non-indexed primitives.
        //!
        //! \param prim
        //!     primititive type
        //! \param numPrim
        //!     number of primitives
        //! \param startVtx
        //!     index into vertex buffer of the first vertex.
        //!
        //! \note 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        virtual void draw( PrimitiveType prim,
                           size_t        numPrim,
                           size_t        startVtx ) = 0;

        //!
        //! Draw quads
        //!
        //! \param options
        //!     渲染选项，详见 DrawQuadOptions。Set to 0 to use default options
        //! \param positions, posStride
        //!     顶点坐标数据，由一系列的2D顶点组成。4个顶点表示一个矩形。
        //!     选项 DQ_WINDOW_SPACE 会影响坐标的含义。
        //! \param texcoords, texStride
        //!     贴图坐标数组，由一系列的2D顶点组成。4个顶点表示一个矩形。
        //! \param count
        //!     Number of quads.
        //!
        virtual void drawQuads(
            uint32_t options,
            const void * positions, size_t posStride,
            const void * texcoords, size_t texStride,
            size_t count ) = 0;

        //!
        //! Draw quads, with same position and texture stride.
        //!
        void drawQuads(
            uint32_t options,
            const void * positions, const void * texcoords, size_t stride,
            size_t count )
        {
            drawQuads( options, positions, stride, texcoords, stride, count );
        }

        //!
        //! Draw single quad.
        //!
        //! \note This function may not very effecient.
        //!
        void drawQuad(
            uint32_t options = 0,
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

            drawQuads( options, pos, sizeof(Vector2f), tex, sizeof(Vector2f), 1 );
        }

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
        drawTextA( const char * text, int x, int y,
                   const Vector4f & color = Vector4f(1,1,1,1) ) = 0;

        //!
        //!  绘制unicode文字
        //!
        //! \sa drawTextA()
        //!
        //! \note 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        virtual void
        drawTextW( const wchar_t * text, int x, int y,
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
        void dumpCurrentState( StrA & );

        //@}
    };

    //!
    //! Function prototype to create instance of renderer.
    //!
    typedef Renderer * (*CreateRendererFunc)( const RendererOptions & );

#if GN_STATIC

    //!
    //! Create instance of D3D renderer.
    //!
#if GN_MSWIN
    Renderer * createD3DRenderer( const RendererOptions & );
#else
    inline Renderer * createD3DRenderer( const RendererOptions & )
    { GN_ERROR( "No D3D support on platform other then MS Windows." ); return 0; }
#endif

    //!
    //! Create instance of OGL renderer.
    //!
    Renderer * createOGLRenderer( const RendererOptions & );
#endif
}}

#include "renderer.inl"

// *****************************************************************************
//                           End of renderer.h
// *****************************************************************************
#endif // __GN_GFX_RENDERER_H__
