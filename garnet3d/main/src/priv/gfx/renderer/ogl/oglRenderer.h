#ifndef __GN_GFX_OGLRENDERER_H__
#define __GN_GFX_OGLRENDERER_H__
// *****************************************************************************
//! \file    oglRenderer.h
//! \brief   OGL renderer class
//! \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicRenderer.h"
#include "oglTypes.h"

namespace GN { namespace gfx
{
    class OGLFont;
    class OGLQuad;

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
        // from StdClass
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
                && ffpOK()
                && renderTargetOK()
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
            ffpClear();
            renderTargetClear();
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

    private :
        bool capsInit() { return true; }
        void capsQuit() {}
        bool capsOK() const { return true; }
        void capsClear() {}

        bool capsDeviceCreate();
        bool capsDeviceRestore() { return true; }
        void capsDeviceDispose() {}
        void capsDeviceDestroy();

        //@}

    // ************************************************************************
    //
    //! \name                      Shader Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual bool supportShader( ShaderType, ShadingLanguage );
        virtual Shader * createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & entry );
        virtual void bindShader( ShaderType type, const Shader * shader ) { mCurrentDrawState.bindShader( type, shader ); }
        virtual void bindShaders( const Shader * const shaders[] ) { mCurrentDrawState.bindShaders(shaders); }

    public:

        //!
        //! Inform OGL renderer that GLSL shader is deleted, to give OGL renderer a chance
        //! to adjust GLSL program map. Only called by GLSL shader class.
        //!
        void removeGLSLShader( ShaderType, Shader * );

    private :
        bool shaderInit() { return true; }
        void shaderQuit() {}
        bool shaderOK() const { return true; }
        void shaderClear() {}

        bool shaderDeviceCreate() { return true; }
        bool shaderDeviceRestore() { return true; }
        void shaderDeviceDispose() {}
        void shaderDeviceDestroy();

        void applyShaderState();

    private:

        union GLSLShaders
        {
            const Shader * shaders[NUM_SHADER_TYPES];
            struct
            {
                const Shader * vs;
                const Shader * ps;
            };

            bool operator < ( const GLSLShaders & rhs ) const
            {
                if( vs != rhs.vs ) return vs < rhs.vs;
                else return ps < rhs.ps;
            }
        };

        typedef std::map<GLSLShaders,void*> GLSLProgramMap;

        GLSLProgramMap mGLSLProgramMap;

        //@}

    // ************************************************************************
    //
    //! \name             Render State Block Manager
    //
    // ************************************************************************

        //@{

    private :
        bool rsbInit() { return true; }
        void rsbQuit() {}
        bool rsbOK() const { return true; }
        void rsbClear() {}

        bool rsbDeviceCreate() { return rebindCurrentRsb(); }
        bool rsbDeviceRestore() { return true; }
        void rsbDeviceDispose() {}
        void rsbDeviceDestroy() { disposeDeviceData(); }

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
                                         uint32_t usage,
                                         const TextureLoader & loader );
        virtual Texture * createTextureFromFile( File & );

    public:

        void chooseClientTextureStage( uint32_t ) const; //!< Choose one stage as client active texture stage.
        void chooseTextureStage( uint32_t ) const; //!< Choose one texture stage as active stage
        void disableTextureStage( uint32_t ) const; //!< Disable one texture stage

    private:
        bool textureInit() { return true; }
        void textureQuit() { clearCurrentTextures(); }
        bool textureOK() const { return true; }
        void textureClear() {}

        bool textureDeviceCreate() { return true; }
        bool textureDeviceRestore() { return true; }
        void textureDeviceDispose() { setAllTextureStagesDirty(); }
        void textureDeviceDestroy() {}

        void applyTexture() const;

        //@}

    // ************************************************************************
    //
    //! \name                 Renderable Buffer Manager
    //
    // ************************************************************************

        //@{

    public :
        virtual uint32_t createVtxBinding( const VtxFmtDesc & );
        virtual VtxBuf * createVtxBuf( size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader );
        virtual IdxBuf * createIdxBuf( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader );
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

        void applyVtxBinding();
        void applyVtxBufState( size_t startVtx );

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
        virtual Matrix44f & composePerspectiveMatrix( Matrix44f &, float, float, float, float ) const;
        virtual Matrix44f & composeOrthoMatrix( Matrix44f &, float, float, float, float, float, float ) const;

    private :
        bool ffpInit() { return true; }
        void ffpQuit() {}
        bool ffpOK() const { return true; }
        void ffpClear() {}

        bool ffpDeviceCreate() { return true; }
        bool ffpDeviceRestore() { reapplyAllFfpStates(); return true; }
        void ffpDeviceDispose() {}
        void ffpDeviceDestroy() {}

        void applyFfpState();

        //@}

    // ************************************************************************
    //
    //! \name              Render Target Manager
    //
    // ************************************************************************

        //@{

    public:
        virtual void setRenderTarget( size_t index, const Texture * texture, uint32_t level, TexFace face );
        virtual void setRenderDepth( const Texture * texture, uint32_t level, TexFace face );

    private:

        bool renderTargetInit() { return true; }
        void renderTargetQuit() {}
        bool renderTargetOK() const { return true; }
        void renderTargetClear()
        {
            mCurrentRTSize.set( 0, 0 );
        }

        bool renderTargetDeviceCreate();
        bool renderTargetDeviceRestore();
        void renderTargetDeviceDispose() {}
        void renderTargetDeviceDestroy() {}

    private:
        RenderTargetTextureDesc
            mCurrentRTs[MAX_RENDER_TARGETS], // current color textures.
            mCurrentDepth;  // current depth texture
        Vector2<uint32_t>
            mCurrentRTSize; // current render target size

        //@}

    // ************************************************************************
    //
    //! \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public:
        virtual bool drawBegin();
        virtual void drawEnd();
        virtual void drawFinish();
        virtual void clearScreen( const Vector4f & c, float z, uint32_t s, uint32_t flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numPrim,
                                  size_t        startVtx,
                                  size_t        minVtxIdx,
                                  size_t        numVtx,
                                  size_t        startIdx );
        virtual void draw( PrimitiveType prim,
                           size_t        numPrim,
                           size_t        startVtx );
        virtual void drawQuads( uint32_t options,
                                const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                size_t count );
        virtual void drawDebugTextW( const wchar_t * text, int x, int y, const Vector4f & color );

    private:
        bool drawInit() { return true; }
        void drawQuit() {}
        bool drawOK() const { return true; }
        void drawClear()
        {
            mDrawBegan = false;
            mCurrentDrawState.clear();
            mLastDrawState.clear();
            mFont = 0;
            mQuad = 0;
        }

        bool drawDeviceCreate();
        bool drawDeviceRestore() { return true; }
        void drawDeviceDispose() { mCurrentDrawState.dirtyFlags.i32 = -1; }
        void drawDeviceDestroy();

    private:
        bool mDrawBegan;

        OGLDrawState mCurrentDrawState;
        OGLDrawState mLastDrawState;

        OGLFont * mFont;
        OGLQuad * mQuad;

    private:
        GN_INLINE void applyDrawState( size_t startVtx );

        //@}

        // ********************************************************************
        //
        //! \name Misc. utilities
        //
        // ********************************************************************

        //@{

    public:

        void dumpCurrentState( StrA & ) const {}

        //@}
    };
}}

#if GN_ENABLE_INLINE
#include "oglTextureMgr.inl"
#endif

// *****************************************************************************
//                           End of oglRenderer.h
// *****************************************************************************
#endif // __GN_GFX_OGLRENDERER_H__
