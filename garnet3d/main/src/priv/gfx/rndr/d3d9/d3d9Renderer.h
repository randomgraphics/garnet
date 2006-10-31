#ifndef __GN_GFXD3D9_D3D9RENDERER_H__
#define __GN_GFXD3D9_D3D9RENDERER_H__
// *****************************************************************************
//! \file    d3d9Renderer.h
//! \brief   D3D9 renderer class
//! \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicRenderer.h"

//! \def GNGFX_D3D9CAPS
//! Define D3D special caps.

namespace GN { namespace gfx
{
    class D3D9Resource; // Forward declaration of D3D9Resource.

    class D3D9Font;
    class D3D9Quad;
    class D3D9Line;

    //!
    //! D3D9 vertex buffer declaration descriptor
    //!
    struct D3D9VtxDeclDesc
    {
        VtxFmtDesc                              format; //!< vertex format
        AutoComPtr<IDirect3DVertexDeclaration9> decl;   //!< D3D decl
    };

    //!
    //! Default depth surface format
    //!
#if GN_XENON
    static const D3DFORMAT DEFAULT_DEPTH_FORMAT = D3DFMT_D24FS8;
#else
    static const D3DFORMAT DEFAULT_DEPTH_FORMAT = D3DFMT_D24S8;
#endif

    //!
    //! D3D9 renderer class
    //!
    class D3D9Renderer : public BasicRenderer
    {
        GN_DECLARE_STDCLASS(D3D9Renderer, BasicRenderer);

        static Logger * sLogger;

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        D3D9Renderer()          { clear(); }
        virtual ~D3D9Renderer() { quit(); }
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
    //! \name                     Device Manager
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

        bool mDeviceChanging; //! if true, then we are inside function changeOptions().

        //@}

    // ************************************************************************
    //
    //! \name                     Display Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual void * getD3DDevice() const { return getDevice(); }
        virtual void * getOGLRC() const { return 0; }

    public :

        //
        // Access to D3D interfaces
        //
        IDirect3D9                  * getD3D() const { GN_ASSERT(mD3D); return mD3D; }
        IDirect3DDevice9            * getDevice() const { GN_ASSERT(mDevice); return mDevice; }
        UINT                          getAdapter() const { return mAdapter; }
        D3DDEVTYPE                    getDeviceType() const { return mDeviceType; }
        UINT                          getBehavior() const { return mBehavior; }
        const D3DPRESENT_PARAMETERS & getPresentParameters() const { return mPresentParameters; }

        //!
        //! Check resource format compability
        //!
        HRESULT checkD3DDeviceFormat( uint32_t usage, D3DRESOURCETYPE rtype, D3DFORMAT format ) const;

        //!
        //! D3D MSAA descriptor
        //!
        struct D3DMsaaDesc
        {
            D3DMULTISAMPLE_TYPE type;    //!< D3D MSAA type
            DWORD               quality; //!< D3D MSAA quality
        };

        //!
        //! Check render target MSAA compability.
        //!
        //! Return value is a array that has NUM_MSAA_TYPES elements, to descript how MSAA qualities are mapped to D3D.
        //!
        const D3DMsaaDesc * getMsaaDesc( D3DFORMAT format )
        {
            MsaaDescTable::const_iterator i = mMsaaDescTable.find( format );
            if( mMsaaDescTable.end() != i ) return i->second.cptr();
            else return newMsaaDesc( format );
        }

    private :
        bool dispInit();
        void dispQuit();
        void dispClear()
        {
            mDispOK = false;
            mDeviceRecreation = false;
            mD3D = 0;
            mDevice = 0;
        }

        bool dispDeviceCreate();
        bool dispDeviceRestore();
        void dispDeviceDispose();
        void dispDeviceDestroy();

        const D3DMsaaDesc * newMsaaDesc( D3DFORMAT ); // create new MSAA descriptor for specific D3D format.

    private :

        bool                    mDispOK; //!< true between dispDeviceRestore() and dispDeviceDispose()
        bool                    mDeviceRecreation; //!< only true between dispDeviceCreate() and dispDeviceRestore()
        UINT                    mAdapter;
        D3DDEVTYPE              mDeviceType;
        UINT                    mBehavior;
        D3DPRESENT_PARAMETERS   mPresentParameters;
        IDirect3D9            * mD3D;
        IDirect3DDevice9      * mDevice;

        typedef std::map<D3DFORMAT,StackArray<D3DMsaaDesc,NUM_MSAA_TYPES> > MsaaDescTable;
        MsaaDescTable mMsaaDescTable;

        //@}

    // ************************************************************************
    //
    //! \name                 Capability Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual bool supportShader( const StrA & );
        virtual bool supportTextureFormat( TexType type, BitField usage, ClrFmt format ) const;

        //!
        //! define API dependent caps
        //!
        enum D3DCaps
        {
            #define GNGFX_D3D9CAPS(X) D3D9CAPS_##X,
            #include "d3d9CapsMeta.h"
            #undef GNGFX_D3D9CAPS
            NUM_D3D9CAPS,
            D3D9CAPS_INVALID,
        };

        //!
        //! get D3D special caps
        //!
        uint32_t getD3DCaps( D3DCaps c ) const { GN_ASSERT( 0 <= c && c < NUM_D3D9CAPS ); return mD3DCaps[c]; }

    private :
        bool capsInit() { return true; }
        void capsQuit() {}
        void capsClear() {}

        bool capsDeviceCreate();
        bool capsDeviceRestore();
        void capsDeviceDispose() {}
        void capsDeviceDestroy() {}

    private :

        uint32_t mD3DCaps[NUM_D3D9CAPS];

        //@}

    // ************************************************************************
    //
    //! \name                     Resource Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual Shader * createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints );
        virtual Texture * createTexture( const TextureDesc & desc, const TextureLoader & loader );
        virtual VtxFmtHandle createVtxFmt( const VtxFmtDesc & );
        virtual VtxBuf * createVtxBuf( size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader );
        virtual IdxBuf * createIdxBuf( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader );

    public :

        //!
        //! Insert resource into resource list. Can be only called by
        //! constructor of D3D9Resource.
        //!
        void insertResource( D3D9Resource * p )
        {
            mResourceList.push_back(p);
        }

        //!
        //! Remove resource from resource list. Can be only called by
        //! destructor of D3D9Resource.
        //!
        void removeResource( D3D9Resource * p )
        {
            mResourceList.remove(p);
        }

    private:

        bool resourceInit() { return true; }
        void resourceQuit() {}
        void resourceClear() {}
        bool resourceDeviceCreate();
        bool resourceDeviceRestore();
        void resourceDeviceDispose();
        void resourceDeviceDestroy();

    private :

        std::list<D3D9Resource*> mResourceList;
        HandleManager<D3D9VtxDeclDesc,VtxFmtHandle> mVtxFmts;

        //@}


    // ********************************************************************
    //
    //! \name Context manager
    //
    // ********************************************************************

        //@{

    public:

        virtual void setContext( const RendererContext & );
        virtual void rebindContext( RendererContext::FieldFlags );
        virtual const RenderStateBlockDesc & getCurrentRenderStateBlock() const;

    public:

        void setD3DRenderState( D3DRENDERSTATETYPE, DWORD );
        DWORD getD3DRenderState( D3DRENDERSTATETYPE );

        void setD3DSamplerState( UINT, D3DSAMPLERSTATETYPE, DWORD );
        DWORD getD3DSamplerState( UINT, D3DSAMPLERSTATETYPE );

#if !GN_XENON
        void setD3DTextureState( UINT, D3DTEXTURESTAGESTATETYPE, DWORD );
        DWORD getD3DTextureState( UINT, D3DTEXTURESTAGESTATETYPE );
#endif

    private :

        bool contextInit() { return true; }
        void contextQuit() {}
        void contextClear() { mContext.resetToDefault(); }
        bool contextDeviceCreate() { return true; }
        bool contextDeviceRestore();
        void contextDeviceDispose();
        void contextDeviceDestroy();

        GN_INLINE void bindContext(
            const RendererContext & newContext,
            RendererContext::FieldFlags newFlag,
            bool forceRebind );

        GN_INLINE void bindContextState(
            const RendererContext & newContext,
            RendererContext::FieldFlags newFlag,
            bool forceRebind );

        GN_INLINE void bindContextRenderTargetsAndViewport( // called _ONLY_ by bindContextState()
            const RendererContext & newContext,
            RendererContext::FieldFlags newFlag,
            bool forceRebind );

        GN_INLINE void bindContextFfp(
            const RendererContext & newContext,
            RendererContext::FieldFlags newFlag,
            bool forceRebind );

        GN_INLINE void bindContextData(
            const RendererContext & newContext,
            RendererContext::FieldFlags newFlag,
            bool forceRebind );

    private:

        template<typename T>
        struct StateValue
        {
            T    value;
            bool initialized;
            void clear() { initialized = false; }
        };

        enum
        {
#if GN_XENON
            // Note: Xenon has no texture states
            MAX_D3D_RENDER_STATES  = D3DRS_MAX,
            MAX_D3D_SAMPLER_STATES = D3DSAMP_MAX,
#else
            MAX_D3D_RENDER_STATES  = 256,
            MAX_D3D_SAMPLER_STATES = 16,
            MAX_D3D_TEXTURE_STATES = 32,
#endif
        };

        StateValue<DWORD> mRenderStates[MAX_D3D_RENDER_STATES];
        StateValue<DWORD> mSamplerStates[MAX_TEXTURE_STAGES][MAX_D3D_SAMPLER_STATES];
#if !GN_XENON
        StateValue<DWORD> mTextureStates[MAX_TEXTURE_STAGES][MAX_D3D_TEXTURE_STATES];
#endif

        RendererContext mContext;

        AutoComPtr<IDirect3DSurface9> mAutoColor0, mAutoDepth;

        AutoComPtr<IDirect3DSurface9> mRenderTargets[MAX_RENDER_TARGETS];

        //@}

    // ************************************************************************
    //
    //! \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public: // from Renderer
        virtual bool drawBegin();
        virtual void drawEnd();
        virtual void clearScreen( const Vector4f & c, float z, uint32_t s, BitField flags );
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
                             const uint16_t * indexData );
        virtual void drawUp( PrimitiveType prim,
                             size_t        numPrims,
                             const void *  vertexData,
                             size_t        strideInBytes );
        virtual void drawQuads( BitField options,
                                const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                const void * colors, size_t clrStride,
                                size_t count );
        virtual void drawLines( BitField options,
                                const void * positions,
                                size_t stride,
                                size_t count,
                                uint32_t color,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj );
        virtual void drawDebugText( const char * text, int x, int y, const Vector4f & color );

        // private functions
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

        bool drawDeviceCreate() { return true; }
        bool drawDeviceRestore() { return true; }
        void drawDeviceDispose() {}
        void drawDeviceDestroy() {}

        bool handleDeviceLost();

    private:

        bool mDrawBegan; // True, if and only if between drawBegin() and drawEnd().
        D3D9Font * mFont; // Font renderer
        D3D9Quad * mQuad; // Quad renderer
        D3D9Line * mLine; // Line renderer

        //@}

        // ********************************************************************
        //
        //! \name Misc. utilities
        //
        // ********************************************************************

        //@{

        virtual void dumpCurrentState( StrA & ) const {}

        //@}
    };
}}

#if GN_ENABLE_INLINE
#include "d3d9ContextMgr.inl"
#endif

// *****************************************************************************
//                           End of d3d9Renderer.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9RENDERER_H__
