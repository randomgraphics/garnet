#ifndef __GN_GFX_GFXRENDERER_H__
#define __GN_GFX_GFXRENDERER_H__
// *****************************************************************************
//! \file    gfxRenderer.h
//! \brief   Main renderer interface of GFX module
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
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

        UserOptions mUserOptions;

    protected:

        //!
        //! Update private user option variable.
        //!
        void setUserOptions( const UserOptions & uo ) { mUserOptions = uo; }

    public:

        //!
        //! Triggered after rendering device is restored to normal stage.
        //!
        //! \note
        //! - Resources that may lost their contents after device reset are:
        //!   - Textures, and
        //!   - Vertex buffers that are NOT type of BUFUSAGE_READBACK.
        //!   - Note that only contents are lost, not resources themselves.
        //! - Index buffers, shaders and rsblocks survive device reset.
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
        //! Change device configuration.
        //!
        //! \param uo
        //!     new device settings
        //! \param forceDeviceRecreation
        //!     force a full device recreation
        //! \note
        //!     This function may trigger sigDeviceRestore.
        //!
        virtual bool changeUserOptions( const UserOptions & uo,
                                        bool forceDeviceRecreation = false ) = 0;

        //!
        //! Get current user options
        //!
        const UserOptions & getUserOptions() const { return mUserOptions; }

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
        void setCaps( Caps, uint32_t );

        //!
        //! reset(clear) all caps. called by child class.
        //!
        void resetAllCaps();

    public:

        //!
        //! Get render device caps
        //!
        uint32_t getCaps( Caps c ) const
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
        //! request a instance of vertex shader
        //!
        virtual AutoRef<Shader>
        createVertexShader( ShadingLanguage lang, const StrA & code ) = 0;

        //!
        //! request a instance of pixel shader
        //!
        virtual AutoRef<Shader>
        createPixelShader( ShadingLanguage lang, const StrA & code ) = 0;

        //!
        //! request a instance of shader
        //!
        //! \param type        shader type
        //! \param lang        shading language
        //! \param code        shader code string
        //! \return            instance of shader
        //!
        //! \note              This is inline function implemented in gfxRenderer.inl
        //!
        AutoRef<Shader>
        createShader( ShaderType      type,
                      ShadingLanguage lang,
                      const StrA &    code );

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
        //! request a rsblock object with specific rsblock structure.
        //! 对于相同的参数，该函数会返回同一个rsblock的实例
        //!
        virtual AutoRef<RenderStateBlock>
        createRenderStateBlock( const RenderStateBlockDesc & ) = 0;

        //!
        //! Bind render state block to rendering device
        //!
        virtual void bindRenderStateBlock( const RenderStateBlock & ) = 0;

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
        //! \param usages      texture usages, one or combination of TexUsage
        //! \note
        //!    - sy/sz will be ignored for 1D/Cube texture,
        //!    - sz will be ignored for 2D texture.
        //!
        virtual AutoRef<Texture>
        createTexture( TexType textype,
                       uint32_t sx, uint32_t sy, uint32_t sz,
                       uint32_t levels = 0,
                       ClrFmt format = FMT_DEFAULT,
                       uint32_t usages = USAGE_NORMAL ) = 0;

        //!
        //! Load texture from file
        //!
        virtual AutoRef<Texture>
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

        //@}

        // ********************************************************************
        //
        //! \name Renderable Buffer Manager
        //
        // ********************************************************************

        //@{

        //!
        //! Create new vertex buffer
        //!
        //! \param format
        //!     vertex format
        //! \param numVtx
        //!     max number of vertices this buffer could hold
        //! \param usage
        //!     buffer type
        //! \param sysCopy
        //!     has system copy or not
        //!
        virtual AutoRef<VtxBuf>
        createVtxBuf( const VtxFmtDesc & format,
                      size_t             numVtx,
                      ResourceUsage      usage,
                      bool               sysCopy ) = 0;

        //!
        //! Create new index buffer
        //!
        //! \param numIdx
        //!     number of indices
        //! \param usage
        //!     buffer type ( see ResourceUsage )
        //! \param sysCopy
        //!     has system copy or not
        //!
        //! \note
        //!     每个索引固定占用16bit
        //!
        virtual AutoRef<IdxBuf>
        createIdxBuf( size_t        numIdx,
                      ResourceUsage usage,
                      bool          sysCopy ) = 0;

        //!
        //! Bind a serias vertex buffers to rendering device.
        //!
        //! \param buffers  Buffer list.
        //! \param start    Stream index of the 1st buffer in buffer list.
        //! \param count    Stream count in buffer list.
        //!
        virtual void
        bindVtxBufs( const VtxBuf * const buffers[],
                     uint32_t start, uint32_t count ) = 0;

        //!
        //! Bind index buffer to rendering device
        //!
        virtual void bindIdxBuf( const IdxBuf * ) = 0;

        //@}

        // ********************************************************************
        //
        //! \name Render Parameter Manager
        //
        // ********************************************************************

        //@{

        // TODO: comment these functions.

        virtual void setParameter( RenderParameter, uint32_t ) = 0;
        virtual void setParameter( RenderParameter, float ) = 0;
        virtual void setParameter( RenderParameter, const double & ) = 0;
        virtual void setParameter( RenderParameter, const Vector4f & ) = 0;
        virtual void setParameter( RenderParameter, const Matrix44f & ) = 0;
        virtual void setParameter( RenderParameter, const Vector4f * ) = 0;
        virtual void setParameter( RenderParameter, const Matrix44f * ) = 0;
        virtual void pushParameter( RenderParameter ) = 0;
        virtual void popParameter( RenderParameter ) = 0;

        virtual Matrix44f &
        computePerspectiveMatrix( Matrix44f & result,
                                  float fovy,
                                  float ratio,
                                  float znear,
                                  float zfar ) const = 0;
        virtual Matrix44f &
        computeOrthoMatrix( Matrix44f & result,
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
        //! 在屏幕上指定的位置绘制2D字符串.
        //!
        //! - 屏幕左上角为坐标原点，X轴向左、Y轴向下。
        //!   被绘制的字符串的坐标位置以第一个字符的左上角为准。
        //! - 函数可以处理中英文的混合字符串，但运行速度较慢，主要为测试而用。
        //!   文字的高度固定为16个象素，宽度为8(English)/16(Chinese)个象素。
        //! - 必须在 drawBegin() 和 drawEnd() 之间调用
        //!
        //! \param text  待绘制度字符串
        //! \param x, y  第一个字符左上角的屏幕坐标
        //! \param color 文字颜色
        //!
        virtual void
        drawTextA( const char * text, int x, int y,
                   const Vector4f & color = Vector4f(1,1,1,1) ) = 0;

        //!
        //!  绘制unicode文字
        //!
        //! \sa draw_texta()
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
    };

    //!
    //! Function prototype to create instance of renderer.
    //!
    typedef Renderer * (*CreateRendererFunc)( const UserOptions & );

#if GN_STATIC
    //!
    //! Create instance of D3D renderer.
    //!
    Renderer * createD3DRenderer( const UserOptions & );

    //!
    //! Create instance of OGL renderer.
    //!
    Renderer * createOGLRenderer( const UserOptions & );
#endif
}}

#include "gfxRenderer.inl"

// *****************************************************************************
//                           End of gfxRenderer.h
// *****************************************************************************
#endif // __GN_GFX_GFXRENDERER_H__
