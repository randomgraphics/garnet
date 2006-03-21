#ifndef __GN_GFX_D3DRENDERER_H__
#define __GN_GFX_D3DRENDERER_H__
// *****************************************************************************
//! \file    d3dRenderer.h
//! \brief   D3D renderer class
//! \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicRenderer.h"

//! \def GNGFX_D3DCAPS
//! Define D3D special caps.

namespace GN { namespace gfx
{
    class D3DResource; // Forward declaration of D3DResource.

    class D3DFont;
    class D3DQuad;
    class D3DLine;

    //!
    //! D3D vertex buffer declaration descriptor
    //!
    struct D3DVtxDeclDesc
    {
        VtxFmtDesc                              format; //!< vertex format
        AutoComPtr<IDirect3DVertexDeclaration9> decl;   //!< D3D decl
    };

    //!
    //! D3D renderer class
    //!
    class D3DRenderer : public BasicRenderer
    {
        GN_DECLARE_STDCLASS(D3DRenderer, BasicRenderer);

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        D3DRenderer()          { clear(); }
        virtual ~D3DRenderer() { quit(); }
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
                && resourceOK()
                && contextOK()
                && drawOK();
        }

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

        enum OptionChangingType
        {
            OCT_AUTO,
            OCT_CREATE,
            OCT_INIT
        };

        bool doOptionChange( RendererOptions, OptionChangingType );

        void deviceClear() { mDeviceChanging = false; }
        bool deviceCreate( bool triggerInitSignal );
        bool deviceRestore();
        void deviceDispose();
        void deviceDestroy( bool triggerQuitSignal );

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
        HRESULT checkD3DDeviceFormat( uint32_t usage, D3DRESOURCETYPE rtype, D3DFORMAT format ) const
        {
            return mD3D->CheckDeviceFormat(
                mAdapter,
                mDeviceType,
                mPresentParameters.BackBufferFormat,
                usage, rtype, format );
        }

        //!
        //! Check render target MSAA compability
        //!
        bool checkD3DDeviceMsaa( D3DFORMAT format ) const
        {
            if( D3DMULTISAMPLE_NONE == mPresentParameters.MultiSampleType ) return true;
            DWORD maxQuality;
            HRESULT hr = mD3D->CheckDeviceMultiSampleType(
                mAdapter,
                mDeviceType,
                format,
                mPresentParameters.Windowed,
                mPresentParameters.MultiSampleType, &maxQuality );
            return D3D_OK == hr && mPresentParameters.MultiSampleQuality < maxQuality;
        }

    private :
        bool dispInit();
        void dispQuit();
        bool dispOK() const { return 0 != mD3D; }
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

    private :
        bool                    mDispOK; //!< true between dispDeviceRestore() and dispDeviceDispose()
        bool                    mDeviceRecreation; //!< only true between dispDeviceCreate() and dispDeviceRestore()
        UINT                    mAdapter;
        D3DDEVTYPE              mDeviceType;
        UINT                    mBehavior;
        D3DPRESENT_PARAMETERS   mPresentParameters;
        IDirect3D9            * mD3D;
        IDirect3DDevice9      * mDevice;

        //@}

    // ************************************************************************
    //
    //! \name                 Capability Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual bool supportShader( ShaderType, ShadingLanguage );
        virtual bool supportTextureFormat( TexType type, BitField usage, ClrFmt format ) const;

        //!
        //! define API dependent caps
        //!
        enum D3DCaps
        {
            #define GNGFX_D3DCAPS(X) D3DCAPS_##X,
            #include "d3dCapsMeta.h"
            #undef GNGFX_D3DCAPS
            NUM_D3DCAPS,
            D3DCAPS_INVALID,
        };

        //!
        //! get D3D special caps
        //!
        uint32_t getD3DCaps( D3DCaps c ) const { GN_ASSERT( 0 <= c && c < NUM_D3DCAPS ); return mD3DCaps[c]; }

    private :
        bool capsInit() { return true; }
        void capsQuit() {}
        bool capsOK() const { return true; }
        void capsClear() {}

        bool capsDeviceCreate();
        bool capsDeviceRestore();
        void capsDeviceDispose() {}
        void capsDeviceDestroy() {}

    private :

        uint32_t mD3DCaps[NUM_D3DCAPS];

        //@}

    // ************************************************************************
    //
    //! \name                     Resource Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual Shader * createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & entry );
        virtual Texture * createTexture( const TextureDesc & desc, const TextureLoader & loader );
        virtual VtxFmtHandle createVtxFmt( const VtxFmtDesc & );
        virtual VtxBuf * createVtxBuf( size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader );
        virtual IdxBuf * createIdxBuf( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader );

    public :

        //!
        //! Insert resource into resource list. Can be only called by
        //! constructor of D3DResource.
        //!
        void insertResource( D3DResource * p )
        {
            mResourceList.push_back(p);
        }

        //!
        //! Remove resource from resource list. Can be only called by
        //! destructor of D3DResource.
        //!
        void removeResource( D3DResource * p )
        {
            mResourceList.remove(p);
        }

    private:

        bool resourceInit() { return true; }
        void resourceQuit() {}
        bool resourceOK() const { return true; }
        void resourceClear() {}
        bool resourceDeviceCreate();
        bool resourceDeviceRestore();
        void resourceDeviceDispose();
        void resourceDeviceDestroy();

    private :

        std::list<D3DResource*> mResourceList;
        HandleManager<D3DVtxDeclDesc,VtxFmtHandle> mVtxFmts;

        //@}


    // ********************************************************************
    //
    //! \name Context manager
    //
    // ********************************************************************

        //@{

    public:

        virtual void setContextState( const ContextState & newContext );
        virtual void setContextData( const ContextData & );
        virtual void rebindContextState( ContextState::FieldFlags );
        virtual void rebindContextData( ContextData::FieldFlags );
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
        bool contextOK() const { return true; }
        void contextClear();
        bool contextDeviceCreate() { return true; }
        bool contextDeviceRestore();
        void contextDeviceDispose() {}
        void contextDeviceDestroy() {}

        GN_INLINE void bindContextState(
            const ContextState & newContext,
            ContextState::FieldFlags newFlag,
            bool forceRebind );

        GN_INLINE void bindContextData(
            const ContextData & newData,
            ContextData::FieldFlags newFlag,
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

        ContextState mContextState;
        ContextData  mContextData;

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
        virtual void drawFinish();
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
        virtual void drawDebugTextW( const wchar_t * text, int x, int y, const Vector4f & color );

        // private functions
    private:
        bool drawInit();
        void drawQuit();
        bool drawOK() const { return true; }
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
        D3DFont * mFont; // Font renderer
        D3DQuad * mQuad; // Quad renderer
        D3DLine * mLine; // Line renderer

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
#include "d3dContextMgr.inl"
#endif

// *****************************************************************************
//                           End of d3dRenderer.h
// *****************************************************************************
#endif // __GN_GFX_D3DRENDERER_H__
