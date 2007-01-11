#ifndef __GN_GFX_OGLRENDERER_H__
#define __GN_GFX_OGLRENDERER_H__
// *****************************************************************************
/// \file    oglRenderer.h
/// \brief   OGL renderer class
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicRenderer.h"

namespace GN { namespace gfx
{
    struct OGLResource;
    class OGLVtxFmt;
    class OGLFont;
    class OGLQuad;
    class OGLLine;

    ///
    /// OGL renderer class
    ///
    class OGLRenderer : public BasicRenderer
    {
        GN_DECLARE_STDCLASS(OGLRenderer, BasicRenderer);

        static Logger * sLogger;

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
        bool init();
        void quit();
    private:
        void clear()
        {
            deviceClear();
            dispClear();
            capsClear();
            resourceClear();
            contextClear();
            drawClear();
        }
        //@}

    // ************************************************************************
    //
    /// \name                     Device Manager
    //
    // ************************************************************************

        //@{

    public:
        virtual bool changeOptions( const RendererOptions & ro, bool forceDeviceRecreation );

    private :

        void deviceClear() { mDeviceChanging = false; }
        bool deviceCreate();
        bool deviceRestore();
        void deviceDispose();
        void deviceDestroy();

    private:

        ///
        /// if true, then we are inside function changeOptions().
        ///
        bool mDeviceChanging;

        //@}

    // ************************************************************************
    //
    /// \name                     Display Manager
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

        bool    mDispOK; ///< true between dispDeviceRestore() and dispDeviceDispose()
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
    /// \name                 Capability Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual bool supportShader( const StrA & );
        virtual bool supportTextureFormat( TexType type, BitFields usage, ClrFmt format ) const;

    private :
        bool capsInit() { return true; }
        void capsQuit() {}
        void capsClear() {}

        bool capsDeviceCreate();
        bool capsDeviceRestore() { return true; }
        void capsDeviceDispose() {}
        void capsDeviceDestroy() {}

        //@}

    // ************************************************************************
    //
    /// \name                     Resource Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual Shader * createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints );
        virtual Texture * createTexture( const TextureDesc & desc, const TextureLoader & loader );
        virtual VtxFmtHandle createVtxFmt( const VtxFmtDesc & );
        virtual VtxBuf * createVtxBuf( size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader );
        virtual IdxBuf * createIdxBuf( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader );

    public:

        ///
        /// Insert resource into resource list. Can be only called by
        /// constructor of OGLResource.
        ///
        void insertResource( OGLResource * p ) { mResourceList.push_back(p); }

        ///
        /// Remove resource from resource list. Can be only called by
        /// destructor of OGLResource.
        ///
        void removeResource( OGLResource * p ) { mResourceList.remove(p); }

        ///
        /// Inform OGL renderer that GLSL shader is deleted, to give OGL renderer a chance
        /// to adjust GLSL program map. Only called by GLSL shader class.
        ///
        void removeGLSLShader( ShaderType, Shader * );

    private :

        bool resourceInit() { return true; }
        void resourceQuit() {}
        void resourceClear() {}
        bool resourceDeviceCreate();
        bool resourceDeviceRestore() { return true; }
        void resourceDeviceDispose() {}
        void resourceDeviceDestroy();

    private:

        union GLSLShaders
        {
            const Shader * shaders[NUM_SHADER_TYPES];
            struct
            {
                const Shader * vs;
                const Shader * ps;
                const Shader * gs;
            };

            bool operator < ( const GLSLShaders & rhs ) const
            {
                if( vs != rhs.vs ) return vs < rhs.vs;
                else return ps < rhs.ps;
            }
        };

        typedef std::map<GLSLShaders,void*> GLSLProgramMap;

        typedef HandleManager<OGLVtxFmt*,VtxFmtHandle> VtxFmtManager;

        std::list<OGLResource*> mResourceList;
        GLSLProgramMap          mGLSLProgramMap;
        VtxFmtManager           mVtxFmts;

        //@}

    // ************************************************************************
    //
    /// \name                   Context Manager
    //
    // ************************************************************************

        //@{

    public:

        virtual void setContext( const RendererContext & newContext );
        virtual void rebindContext( RendererContext::FieldFlags );
        virtual const RenderStateBlockDesc & getCurrentRenderStateBlock() const;

    public:

        void chooseClientTextureStage( size_t ) const; ///< Choose one stage as client active texture stage.
        void chooseTextureStage( size_t ) const; ///< Choose one texture stage as active stage
        void disableTextureStage( size_t ) const; ///< Disable one texture stage

    private:

        bool contextInit() { return true; }
        void contextQuit() {}
        void contextClear();
        bool contextDeviceCreate();
        bool contextDeviceRestore();
        void contextDeviceDispose();
        void contextDeviceDestroy();

        GN_INLINE void bindContext( const RendererContext & newContext, RendererContext::FieldFlags newFlag, bool forceRebind );
        GN_INLINE void bindContextShaders( const RendererContext & newContext, RendererContext::FieldFlags newFlag, bool forceRebind );
        GN_INLINE void bindContextRenderStates( const RendererContext & newContext, RendererContext::FieldFlags newFlag, bool forceRebind );
        GN_INLINE void bindContextRenderTargetsAndViewport( const RendererContext & newContext, RendererContext::FieldFlags newFlag, bool forceRebind );
        GN_INLINE void bindContextFfp( const RendererContext & newContext, RendererContext::FieldFlags newFlag, bool forceRebind );
        GN_INLINE void bindContextData( const RendererContext & newContext, RendererContext::FieldFlags newFlag, bool forceRebind );

    private:

        RendererContext mContext;
        UInt32 mColorBufferWidth, mColorBufferHeight; // current render target size

        //@}

    // ************************************************************************
    //
    /// \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public:

        virtual bool drawBegin();
        virtual void drawEnd();
        virtual void clearScreen( const Vector4f & c, float z, UInt32 s, BitFields flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numPrims,
                                  size_t        startVtx,
                                  size_t        minVtxIdx,
                                  size_t        numVtx,
                                  size_t        startIdx );
        virtual void draw( PrimitiveType prim,
                           size_t        numPrims,
                           size_t        startVtx );
        virtual void drawIndexedUp(
                             PrimitiveType    prim,
                             size_t           numPrims,
                             size_t           numVertices,
                             const void *     vertexData,
                             size_t           strideInBytes,
                             const UInt16 * indexData );
        virtual void drawUp( PrimitiveType prim,
                             size_t        numPrims,
                             const void *  vertexData,
                             size_t        strideInBytes );
        virtual void drawQuads( BitFields options,
                                const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                const void * colors, size_t clrStride,
                                size_t count );
        virtual void drawLines( BitFields options,
                                const void * positions,
                                size_t stride,
                                size_t count,
                                UInt32 color,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj );
        virtual void drawDebugText( const char * text, int x, int y, const Vector4f & color );

    private:

        bool drawInit() { return true; }
        void drawQuit() {}
        void drawClear()
        {
            mDrawBegan = false;
            mFont = 0;
            mQuad = 0;
            mLine = 0;
        }

        bool drawDeviceCreate();
        bool drawDeviceRestore() { return true; }
        void drawDeviceDispose() {}
        void drawDeviceDestroy();

    private:

        bool mDrawBegan;
        OGLFont * mFont;
        OGLQuad * mQuad;
        OGLLine * mLine;

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
        //
        // ********************************************************************

        //@{

    public:

        void dumpCurrentState( StrA & ) const {}

        //@}
    };
}}

#if GN_ENABLE_INLINE
#include "oglContextMgr.inl"
#endif

// *****************************************************************************
//                           End of oglRenderer.h
// *****************************************************************************
#endif // __GN_GFX_OGLRENDERER_H__
