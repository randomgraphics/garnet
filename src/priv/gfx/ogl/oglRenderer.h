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

namespace GN { namespace gfx {

    struct OGLResource; // Forward declaration of OGLResource.

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
        bool init( const UserOptions & );
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
        virtual bool changeUserOptions( const UserOptions & uo,
                                        bool forceDeviceRecreation );

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
        //! if true, then we are inside function changeUserOptions().
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
#else
    private :
        bool dispInit() { return true; }
        void dispQuit() {}
        bool dispOK() const { return true; }
        void dispClear()
        {
            mRenderContext = 0;
        }

        bool dispDeviceCreate() { mRenderContext = (void*)1; return true; }
        bool dispDeviceRestore() { return true; }
        void dispDeviceDispose() {}
        void dispDeviceDestroy() {}

    private :
        void * mRenderContext;
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
        //! get OGL special caps
        //!
        uint32_t getOGLCaps( OGLCaps c ) const
        {
            GN_ASSERT( 0 <= c && c < NUM_OGLCAPS );
            return mOGLCaps[c].get();
        }

    private :
        bool capsInit() { return true; }
        void capsQuit() {}
        bool capsOK() const { return true; }
        void capsClear() {}

        bool capsDeviceCreate();
        bool capsDeviceRestore() { return true; }
        void capsDeviceDispose() {}
        void capsDeviceDestroy();

    private :

        CapsDesc mOGLCaps[NUM_OGLCAPS];

        //@}

    // ************************************************************************
    //
    //! \name                      Shader Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual bool supportShader( ShaderType, ShadingLanguage );
        virtual AutoRef<Shader> createVertexShader( ShadingLanguage, const StrA & ) { GN_UNIMPL(); return AutoRef<Shader>(); }
        virtual AutoRef<Shader> createPixelShader( ShadingLanguage, const StrA & ) { GN_UNIMPL(); return AutoRef<Shader>(); }
        virtual void bindShaders( const Shader *, const Shader * ) { GN_UNIMPL(); }

    private :
        bool shaderInit() { return true; }
        void shaderQuit() {}
        bool shaderOK() const { return true; }
        void shaderClear() {}

        bool shaderDeviceCreate() { return true; }
        bool shaderDeviceRestore() { return true; }
        void shaderDeviceDispose() {}
        void shaderDeviceDestroy() {}

        //@}

    // ************************************************************************
    //
    //! \name                      RSBlock Manager
    //
    // ************************************************************************

        //@{

    public:
        virtual AutoRef<RenderStateBlock> createRenderStateBlock( const RenderStateBlockDesc & )
        { GN_UNIMPL(); return AutoRef<RenderStateBlock>(); }
        virtual void bindRenderStateBlock( const RenderStateBlock & ) { GN_UNIMPL(); }

    private :
        bool rsbInit() { return true; }
        void rsbQuit() {}
        bool rsbOK() const { return true; }
        void rsbClear() {}

        bool rsbDeviceCreate() { return true; }
        bool rsbDeviceRestore() { return true; }
        void rsbDeviceDispose() {}
        void rsbDeviceDestroy() {}

        //@}

    // ************************************************************************
    //
    //! \name                     Texture Manager
    //
    // ************************************************************************

        //@{

    public :
        virtual AutoRef<Texture>
        createTexture( TexType textype,
                       uint32_t sx, uint32_t sy, uint32_t sz,
                       uint32_t levels,
                       ClrFmt format,
                       uint32_t usages )
        {
            GN_UNIMPL();
            return AutoRef<Texture>();
        }
        virtual AutoRef<Texture> createTextureFromFile( File & )
        {
            GN_UNIMPL();
            return AutoRef<Texture>();
        }
        virtual void bindTextures( const Texture * const texlist[],
                                   uint32_t start, uint32_t numtex )
        { GN_UNIMPL(); }

    private :
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
        virtual AutoRef<VtxBuf>
        createVtxBuf( const VtxFmtDesc & format,
                      size_t             numVtx,
                      ResourceUsage      usage,
                      bool               sysCopy )
        {
            GN_UNIMPL();
            return AutoRef<VtxBuf>();
        }
        virtual AutoRef<IdxBuf>
        createIdxBuf( size_t        numIdx,
                      ResourceUsage usage,
                      bool          sysCopy )
        {
            GN_UNIMPL();
            return AutoRef<IdxBuf>();
        }
        virtual void
        bindVtxBufs( const VtxBuf * const buffers[],
                     uint32_t start, uint32_t count ) { GN_UNIMPL(); }
        virtual void
        bindIdxBuf( const IdxBuf * ) { GN_UNIMPL(); }

    private :
        bool bufferInit() { return true; }
        void bufferQuit() {}
        bool bufferOK() const { return true; }
        void bufferClear() {}

        bool bufferDeviceCreate() { return true; }
        bool bufferDeviceRestore() { return true; }
        void bufferDeviceDispose() {}
        void bufferDeviceDestroy() {}

    private :

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
        virtual Matrix44f & computePerspectiveMatrix( Matrix44f &, float, float, float, float ) const;
        virtual Matrix44f & computeOrthoMatrix( Matrix44f &, float, float, float, float, float, float ) const;

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
                                      TexFace face ) { GN_UNIMPL(); }
        virtual void setRenderDepth( const Texture * texture,
                                     TexFace face ) { GN_UNIMPL(); }
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
        virtual void drawTextW( const wchar_t * text, int x, int y,
                                const Vector4f & color ) {}

        // private functions
    private:
        bool drawInit() { return true; }
        void drawQuit() {}
        bool drawOK() const { return true; }
        void drawClear() { mDrawBegan = false; }

        bool drawDeviceCreate() { return true; }
        bool drawDeviceRestore() { return true; }
        void drawDeviceDispose() {}
        void drawDeviceDestroy() {}

        bool handleDeviceLost();

    private:
        bool mDrawBegan;

        //@}
    };
}}

#if GN_MSVC
#pragma warning(pop)
#endif

// *****************************************************************************
//                           End of oglRenderer.h
// *****************************************************************************
#endif // __GN_GFX_OGLRENDERER_H__
