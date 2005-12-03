#ifndef __GN_GFX_OGLRENDERER_H__
#define __GN_GFX_OGLRENDERER_H__
// *****************************************************************************
//! \file    oglRenderer.h
//! \brief   OGL renderer class
//! \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicRenderer.h"
#include "oglTypes.h"

#if GN_MSVC
#pragma warning(push)
#pragma warning(disable:4100) // unused parameters
#endif

namespace GN { namespace gfx
{
    //!
    //! OGL renderer class
    //!
    class OGLRenderer : public BasicRenderer
    {
        GN_DECLARE_STDCLASS(OGLRenderer, BasicRenderer);

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        OGLRenderer()          { clear(); }
        virtual ~OGLRenderer() { quit(); }
        //@}

        // ********************************
        // standard init/quit
        // ********************************

        //@{
    public:
        bool init( const RendererOptions & );
        void quit();
        bool ok() const
        {
            return MyParent::ok()
                && dispOK()
                && capsOK()
                && shaderOK()
                && rsbOK()
                && textureOK()
                && bufferOK()
                && paramOK()
                && drawOK();
        }

    private:
        void clear()
        {
            deviceClear();
            dispClear();
            capsClear();
            shaderClear();
            rsbClear();
            textureClear();
            bufferClear();
            paramClear();
            drawClear();
        }
        //@}

    // ************************************************************************
    //
    //! \name                     Device Manager
    //
    // ************************************************************************

        //@{

    public:
        virtual bool changeOptions( RendererOptions ro, bool forceDeviceRecreation );

    protected :
        bool deviceCreate();
        bool deviceRestore();
        void deviceDispose();
        void deviceDestroy();

    private :
        void deviceClear()
        {
            mDeviceChanging = false;
        }

    private:

        //!
        //! if true, then we are inside function changeOptions().
        //!
        bool mDeviceChanging;

        //@}

    // ************************************************************************
    //
    //! \name                     Display Manager
    //
    // ************************************************************************

        //@{

    public :
        virtual void * getD3DDevice() const { return 0; }
        virtual void * getOGLRC() const { return mRenderContext; }

    public:
        bool makeCurrent() const
        {
#if GN_MSWIN
            GN_ASSERT( mRenderContext && mDeviceContext );
            HGLRC hrc = wglGetCurrentContext();
            HDC   hdc = wglGetCurrentDC();
            if( hdc != mDeviceContext || hrc != mRenderContext )
            {
                if( !::wglMakeCurrent(mDeviceContext, mRenderContext) )
                {
                    GNGFX_ERROR( "wglMakeCurrent() failed : %s", getOSErrorInfo() );
                    return false;
                }
            }
            return true;
#elif GN_POSIX
            Display * disp = (Display*)getDispDesc().displayHandle;
            Window win = (Window)getDispDesc().windowHandle;
            GN_ASSERT( disp && win && mRenderContext );
            glXMakeCurrent( disp, win, mRenderContext );
            return true;
#endif
        }

#if GN_MSWIN
    private :
        bool dispInit() { return true; }
        void dispQuit() {}
        bool dispOK() const { return true; }
        void dispClear()
        {
            mDispOK = false;
            mDeviceContext = 0;
            mRenderContext = 0;
            mDisplayModeActivated = false;
            mIgnoreMsgHook = false;
        }

        bool dispDeviceCreate();
        bool dispDeviceRestore();
        void dispDeviceDispose();
        void dispDeviceDestroy();

        bool activateDisplayMode();
        void restoreDisplayMode();
        void msgHook( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp );

    private :

        bool    mDispOK; //!< true between dispDeviceRestore() and dispDeviceDispose()
        HDC     mDeviceContext;
        HGLRC   mRenderContext;
        bool    mDisplayModeActivated;
        bool    mIgnoreMsgHook;
#elif GN_POSIX
    private :
        bool dispInit() { return true; }
        void dispQuit() {}
        bool dispOK() const { return true; }
        void dispClear()
        {
            mRenderContext = 0;
        }

        bool dispDeviceCreate();
        bool dispDeviceRestore();
        void dispDeviceDispose();
        void dispDeviceDestroy();

    private :
        GLXContext mRenderContext;
#endif

        //@}

    // ************************************************************************
    //
    //! \name                 Capability Manager
    //
    // ************************************************************************

        //@{

    public :

        //!
        //! Get pointer to GLEW context
        //!
        GLEWContext * getGLEWContext() const { GN_ASSERT( mGLEWContext ); return mGLEWContext; }

    private :
        bool capsInit() { return true; }
        void capsQuit() {}
        bool capsOK() const { return true; }
        void capsClear()
        {
            mGLEWContext = 0;
#if GN_MSWIN
            mWGLEWContext = 0;
#elif GN_POSIX
            mGLXEWContext = 0;
#endif
        }

        bool capsDeviceCreate();
        bool capsDeviceRestore() { return true; }
        void capsDeviceDispose() {}
        void capsDeviceDestroy();

    private :

        // for GLEW multi-context support
        GLEWContext * glewGetContext() const { GN_ASSERT( mGLEWContext ); return mGLEWContext; }
        GLEWContext * mGLEWContext;
#if GN_MSWIN
        WGLEWContext * wglewGetContext() const { GN_ASSERT( mWGLEWContext ); return mWGLEWContext; }
        WGLEWContext * mWGLEWContext;
#elif GN_POSIX
        GLXEWContext * glxewGetContext() const { GN_ASSERT( mGLXEWContext ); return mGLXEWContext; }
        GLXEWContext * mGLXEWContext;
#endif

        //@}

    // ************************************************************************
    //
    //! \name                      Shader Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual bool supportShader( ShaderType, ShadingLanguage );
        virtual Shader * createVtxShader( ShadingLanguage, const StrA & ) { GN_WARN("no impl"); return 0; }
        virtual Shader * createPxlShader( ShadingLanguage, const StrA & ) { GN_WARN("no impl"); return 0; }
        virtual void bindVtxShader( const Shader * ) {}
        virtual void bindPxlShader( const Shader * ) {}
        virtual void bindShaders( const Shader *, const Shader * ) {}

    private :
        bool shaderInit() { return true; }
        void shaderQuit() {}
        bool shaderOK() const { return true; }
        void shaderClear() {}

        bool shaderDeviceCreate() { return true; }
        bool shaderDeviceRestore() { return true; }
        void shaderDeviceDispose() {}
        void shaderDeviceDestroy() {}

        void updateShaderState() { GNGFX_WARN( "no impl" ); }

        //@}

    // ************************************************************************
    //
    //! \name                      RSBlock Manager
    //
    // ************************************************************************

        //@{

    private :
        bool rsbInit() { return true; }
        void rsbQuit() {}
        bool rsbOK() const { return true; }
        void rsbClear() {}

        bool rsbDeviceCreate() { return true; }
        bool rsbDeviceRestore() { return true; }
        void rsbDeviceDispose() {}
        void rsbDeviceDestroy() {}

        // from BasicRenderer
        virtual DeviceRenderStateBlock *
        createDeviceRenderStateBlock( const RenderStateBlockDesc & from, const RenderStateBlockDesc & to );

        //@}

    // ************************************************************************
    //
    //! \name                     Texture Manager
    //
    // ************************************************************************

        //@{

    public :
        virtual Texture * createTexture( TexType textype,
                                         uint32_t sx, uint32_t sy, uint32_t sz,
                                         uint32_t levels,
                                         ClrFmt format,
                                         uint32_t usage );
        virtual Texture * createTextureFromFile( File & );
        virtual void bindTextures( const Texture * const texlist[],
                                   uint32_t start, uint32_t numtex );

    public:

        void chooseClientTextureStage( uint32_t ); //!< Choose one stage as client active texture stage.
        void chooseTextureStage( uint32_t ); //!< Choose one texture stage as active stage
        void disableTextureStage( uint32_t ); //!< Disable one texture stage

    private:
        bool textureInit() { return true; }
        void textureQuit() {}
        bool textureOK() const { return true; }
        void textureClear() {}

        bool textureDeviceCreate() { return true; }
        bool textureDeviceRestore() { return true; }
        void textureDeviceDispose() {}
        void textureDeviceDestroy() {}

        //@}

    // ************************************************************************
    //
    //! \name                 Renderable Buffer Manager
    //
    // ************************************************************************

        //@{

    public :
        virtual uint32_t createVtxBinding( const VtxFmtDesc & );
        virtual VtxBuf * createVtxBuf( size_t bytes, ResourceUsage usage, bool sysCopy );
        virtual IdxBuf * createIdxBuf( size_t numIdx, ResourceUsage usage, bool sysCopy );
        virtual void bindVtxBinding( uint32_t );
        virtual void bindVtxBufs( const VtxBuf * const buffers[], size_t start, size_t count );
        virtual void bindVtxBuf( size_t index, const VtxBuf * buffer, size_t stride );
        virtual void bindIdxBuf( const IdxBuf * buf ) { mCurrentIdxBuf.reset( buf ); }

    private :
        bool bufferInit() { return true; }
        void bufferQuit() {}
        bool bufferOK() const { return true; }
        void bufferClear();

        bool bufferDeviceCreate() { return true; }
        bool bufferDeviceRestore() { return true; }
        void bufferDeviceDispose() {}
        void bufferDeviceDestroy() {}

        void updateVtxBinding();
        void updateVtxBufState( size_t baseVtx );

    private :

        typedef HandleManager<void*,uint32_t> VtxBindingManager;

        VtxBindingManager mVtxBindings;

        AutoRef<const IdxBuf> mCurrentIdxBuf;

        //@}

    // ************************************************************************
    //
    //! \name                     Resource Manager
    //
    // ************************************************************************

        //@{

    public :

        //!
        //! Insert resource into resource list. Can be only called by
        //! constructor of OGLResource.
        //!
        void insertResource( OGLResource * p )
        {
            mResourceList.push_back(p);
        }

        //!
        //! Remove resource from resource list. Can be only called by
        //! destructor of OGLResource.
        //!
        void removeResource( OGLResource * p )
        {
            mResourceList.remove(p);
        }

    private:

        bool resourceDeviceCreate();
        bool resourceDeviceRestore();
        void resourceDeviceDispose();
        void resourceDeviceDestroy();

    private :

        std::list<OGLResource*> mResourceList;

        //@}

    // ************************************************************************
    //
    //! \name                  Render Parameters Manager
    //
    // ************************************************************************

        //@{

    public:
        virtual void setParameter( RenderParameter, uint32_t ) { GN_UNIMPL(); }
        virtual void setParameter( RenderParameter, float ) { GN_UNIMPL(); }
        virtual void setParameter( RenderParameter, const double & ) { GN_UNIMPL(); }
        virtual void setParameter( RenderParameter, const Vector4f & ) { GN_UNIMPL(); }
        virtual void setParameter( RenderParameter, const Matrix44f & ) { GN_UNIMPL(); }
        virtual void setParameter( RenderParameter, const Vector4f * ) { GN_UNIMPL(); }
        virtual void setParameter( RenderParameter, const Matrix44f * ) { GN_UNIMPL(); }
        virtual void pushParameter( RenderParameter ) { GN_UNIMPL(); }
        virtual void popParameter( RenderParameter ) { GN_UNIMPL(); }
        virtual Matrix44f & composePerspectiveMatrix( Matrix44f &, float, float, float, float ) const;
        virtual Matrix44f & composeOrthoMatrix( Matrix44f &, float, float, float, float, float, float ) const;

    private :
        bool paramInit() { return true; }
        void paramQuit() {}
        bool paramOK() const { return true; }
        void paramClear() {}

        bool paramDeviceCreate() { return true; }
        bool paramDeviceRestore() { return true; }
        void paramDeviceDispose() {}
        void paramDeviceDestroy() {}

        //@}

    // ************************************************************************
    //
    //! \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public:
        virtual void setRenderTarget( size_t index,
                                      const Texture * texture,
                                      TexFace face ) { GN_WARN( "no impl" ); }
        virtual void setRenderDepth( const Texture * texture,
                                     TexFace face ) { GN_WARN( "no impl" ); }
        virtual bool drawBegin();
        virtual void drawEnd();
        virtual void drawFinish();
        virtual void clearScreen( const Vector4f & c, float z, uint32_t s, uint32_t flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numPrim,
                                  size_t        baseVtx,
                                  size_t        minVtxIdx,
                                  size_t        numVtx,
                                  size_t        startIdx );
        virtual void draw( PrimitiveType prim,
                           size_t        numPrim,
                           size_t        startVtx );
        virtual void drawQuads( const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                size_t count, uint32_t options ) {}
        virtual void drawTextW( const wchar_t * text, int x, int y, const Vector4f & color );

        // private functions
    private:
        bool drawInit() { return true; }
        void drawQuit() {}
        bool drawOK() const { return true; }
        void drawClear() { mDrawBegan = false; mFontMap.clear(); mCurrentDrawState.clear(); mLastDrawState.clear(); }

        bool drawDeviceCreate() { return true; }
        bool drawDeviceRestore();
        void drawDeviceDispose();
        void drawDeviceDestroy() {}

    private:
        bool mDrawBegan;

        // font parameters
        struct CharDesc
        {
            GLuint displayList; // display list use to display the char
            int    advanceX;    // Advance on X direction.
        };
        typedef std::map<wchar_t,CharDesc> FontMap;
        FontMap mFontMap;
        int     mFontHeight;

        OGLDrawState mCurrentDrawState;
        OGLDrawState mLastDrawState;

    private:
        bool fontInit();
        void fontQuit();
        bool charInit( wchar_t, CharDesc & );
        int  drawChar( wchar_t ); //!< Return x-advance of the character
        void updateDrawState( size_t baseVtx );

#if GN_POSIX
        int      getFontBitmapAdvance( char ch );
        uint32_t getFontBitmapHeight();
        void     drawFontBitmap( char ch );
#endif
        //@}

        // ********************************************************************
        //
        //! \name Misc. utilities
        //
        // ********************************************************************

        //@{

        //!
        //! Dump current renderer state to string. For debug purpose only.
        //!
        void dumpCurrentState( StrA & ) {}

        //@}
    };
}}

#if GN_MSVC
#pragma warning(pop)
#endif

#if GN_ENABLE_INLINE
#include "oglTextureMgr.inl"
#include "oglDrawMgr.inl"
#endif

// *****************************************************************************
//                           End of oglRenderer.h
// *****************************************************************************
#endif // __GN_GFX_OGLRENDERER_H__
