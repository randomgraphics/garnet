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

    //!
    //! D3D vertex buffer binding descriptor
    //!
    struct D3DVtxBindingDesc
    {
        VtxFmtDesc                              format; //!< vertex format
        AutoComPtr<IDirect3DVertexDeclaration9> decl;   //!< D3D decl
    };

    //!
    //! D3D draw state
    //!
    struct D3DDrawState
    {
        //!
        //! Renderer state dirty flags
        //!
        union DirtyFlags
        {
            uint32_t u32; //!< Dirty flags as unsigned integer
            int32_t  i32; //!< Dirty flags as signed integer

            struct
            {
                int vtxBufs    : 16; //!< Vertex buffer dirty flags
                int vtxBinding : 1;  //!< Vertex binding dirty flag
                int vtxShader  : 1;  //!< Vertex shader
                int pxlShader  : 1;  //!< Pixel shader
                int reserved   : 13; //!< Reserved for future use.
            };
        };

        //!
        //! Vertex buffer state
        //!
        struct VtxBufDesc
        {
            AutoRef<const VtxBuf> buf;    //!< the buffer pointer
            size_t                stride; //!< vertex stride
        };

        VtxBufDesc            vtxBufs[MAX_VERTEX_STREAMS]; //!< vertex buffers
        uint32_t              vtxBinding;                  //!< vertex binding handle
        AutoRef<const Shader> vtxShader;                   //!< vertex shader
        AutoRef<const Shader> pxlShader;                   //!< pixel shader
        DirtyFlags            dirtyFlags;                  //!< dirty flags

        //!
        //! clear buffer states
        //!
        void clear()
        {
            GN_CASSERT( sizeof(DirtyFlags) == 4 );
            for( size_t i = 0; i < MAX_VERTEX_STREAMS; ++i )
            {
                vtxBufs[i].buf.reset();
                vtxBufs[i].stride = 0;
            }
            vtxBinding = 0;
            dirtyFlags.u32 = 0;
        }

        //!
        //! bind vertex buffer
        //!
        void bindVtxBuf( size_t index, const VtxBuf * buf, size_t stride )
        {
            GN_ASSERT( index < MAX_VERTEX_STREAMS );
            dirtyFlags.vtxBufs |= 1 << index;
            vtxBufs[index].buf.reset( buf );
            vtxBufs[index].stride = stride;
        }

        //!
        //! bind vertex binding
        //!
        void bindVtxBinding( uint32_t handle )
        {
            dirtyFlags.vtxBinding = true;
            vtxBinding = handle;
        }

        //!
        //! bind vertex shader
        //!
        void bindVtxShader( const Shader * shader )
        {
            if( shader == vtxShader.get() ) return;
            dirtyFlags.vtxShader |= true;
            vtxShader.reset( shader );
        }

        //!
        //! bind pixel shader
        //!
        void bindPxlShader( const Shader * shader )
        {
            if( shader == pxlShader.get() ) return;
            dirtyFlags.pxlShader |= true;
            pxlShader.reset( shader );
        }
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
                && shaderOK()
                && rsbOK()
                && textureOK()
                && bufferOK()
                && paramOK()
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
            paramClear();
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

    private :

        void deviceClear()
        {
            mDeviceChanging = false;
        }

        bool deviceCreate();
        bool deviceRestore();
        void deviceDispose();
        void deviceDestroy();

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
        uint32_t getD3DCaps( D3DCaps c ) const
        {
            GN_ASSERT( 0 <= c && c < NUM_D3DCAPS );
            return mD3DCaps[c].get();
        }

    private :
        bool capsInit() { return true; }
        void capsQuit() {}
        bool capsOK() const { return true; }
        void capsClear() {}

        bool capsDeviceCreate();
        bool capsDeviceRestore();
        void capsDeviceDispose() {}
        void capsDeviceDestroy();

    private :

        CapsDesc mD3DCaps[NUM_D3DCAPS];

        //@}

    // ************************************************************************
    //
    //! \name                      Shader Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual bool supportShader( ShaderType, ShadingLanguage );
        virtual Shader * createVtxShader( ShadingLanguage, const StrA &, const StrA & );
        virtual Shader * createPxlShader( ShadingLanguage, const StrA &, const StrA & );
        virtual void bindVtxShader( const Shader * s ) { mDrawState.bindVtxShader(s); }
        virtual void bindPxlShader( const Shader * s ) { mDrawState.bindPxlShader(s); }
        virtual void bindShaders( const Shader *, const Shader * );

    private :
        bool shaderInit() { return true; }
        void shaderQuit() {}
        bool shaderOK() const { return true; }
        void shaderClear() {}

        bool shaderDeviceCreate() { return true; }
        bool shaderDeviceRestore() { return true; }
        void shaderDeviceDispose() {}
        void shaderDeviceDestroy() {}

        void applyShader(
                const Shader * vtxShader, bool vtxShaderDirty,
                const Shader * pxlShader, bool pxlShaderDirty ) const;

        //@}

    // ************************************************************************
    //
    //! \name                      RSBlock Manager
    //
    // ************************************************************************

        //@{

    public :

        void setD3DRenderState( D3DRENDERSTATETYPE, DWORD );
        void setD3DTextureState( UINT, D3DTEXTURESTAGESTATETYPE, DWORD );
        void setD3DSamplerState( UINT, D3DSAMPLERSTATETYPE, DWORD );

    private :
        bool rsbInit() { return true; }
        void rsbQuit() {}
        bool rsbOK() const { return true; }
        void rsbClear() {}

        bool rsbDeviceCreate() { return true; }
        bool rsbDeviceRestore();
        void rsbDeviceDispose() { disposeDeviceData(); }
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
        virtual Texture *
        createTexture( TexType textype,
                       uint32_t sx, uint32_t sy, uint32_t sz,
                       uint32_t levels,
                       ClrFmt format,
                       uint32_t usage,
                       const TextureLoader & loader );
        virtual Texture * createTextureFromFile( File & );
        virtual void bindTextures( const Texture * const texlist[],
                                   uint32_t start, uint32_t numtex );

    private :
        bool textureInit() { return true; }
        void textureQuit() {}
        bool textureOK() const { return true; }
        void textureClear() {}

        bool textureDeviceCreate() { return true; }
        bool textureDeviceRestore();
        void textureDeviceDispose() {}
        void textureDeviceDestroy() {}

    private:

        struct TexParameters
        {
            D3DTEXTUREFILTERTYPE min, mag, mip;
            D3DTEXTUREADDRESS s, q, r, t;
        };

        TexParameters mTexParameters[MAX_TEXTURE_STAGES];

    private:

        GN_INLINE void updateTextureFilters( uint32_t stage, const D3DTEXTUREFILTERTYPE * filters );
        GN_INLINE void updateTextureWraps( uint32_t stage, const D3DTEXTUREADDRESS * strq );

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
        virtual void bindIdxBuf( const IdxBuf * );

    private :
        bool bufferInit() { return true; }
        void bufferQuit() {}
        bool bufferOK() const { return true; }
        void bufferClear() { mVtxBindings.clear(); }

        bool bufferDeviceCreate() { return true; }
        bool bufferDeviceRestore();
        void bufferDeviceDispose();
        void bufferDeviceDestroy() {}

        void applyVtxBinding( uint32_t );
        void applyVtxBuffers();

    private :

        HandleManager<D3DVtxBindingDesc,uint32_t> mVtxBindings;

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

        bool resourceDeviceCreate();
        bool resourceDeviceRestore();
        void resourceDeviceDispose();
        void resourceDeviceDestroy();

    private :

        std::list<D3DResource*> mResourceList;

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
        bool paramInit() { return true; }
        void paramQuit() {}
        bool paramOK() const { return true; }
        void paramClear() {}

        bool paramDeviceCreate() { return true; }
        bool paramDeviceRestore();
        void paramDeviceDispose() {}
        void paramDeviceDestroy() {}

        void applyRenderParameters();

        //@}

    // ************************************************************************
    //
    //! \name                  Render Target Manager
    //
    // ************************************************************************

        //@{

    public:
        virtual void setRenderTarget( size_t index, const Texture * texture, TexFace face );
        virtual void setRenderDepth( const Texture * texture, TexFace face );

    private:
        bool renderTargetInit() { return true; }
        void renderTargetQuit() {}
        bool renderTargetOK() const { return true; }
        void renderTargetClear()
        {
            mDefaultRT0 = mAutoDepth = 0;
            mCurrentRTSize.set( 0, 0 );
            mAutoDepthSize.set( 0, 0 );
        }

        bool renderTargetDeviceCreate();
        bool renderTargetDeviceRestore();
        void renderTargetDeviceDispose();
        void renderTargetDeviceDestroy() {}

        GN_INLINE void resizeAutoDepthBuffer( const Vector2<uint32_t> & );

    private:
        LPDIRECT3DSURFACE9
            mDefaultRT0,    // default color buffer
            mAutoDepth;     // automatic depth buffer
        RenderTargetTextureDesc
            mCurrentRTs[4], // current color textures.
            mCurrentDepth;  // current depth texture
        Vector2<uint32_t>
            mCurrentRTSize, // current render target size
            mAutoDepthSize; // size of automatic depth buffer

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
        virtual void drawQuads( uint32_t options,
                                const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                size_t count );
        virtual void drawTextW( const wchar_t * text, int x, int y, const Vector4f & color );

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
            mDrawState.clear();
        }

        bool drawDeviceCreate() { return true; }
        bool drawDeviceRestore() { return true; }
        void drawDeviceDispose() { mDrawState.dirtyFlags.i32 = -1; }
        void drawDeviceDestroy() {}

        bool handleDeviceLost();

        GN_INLINE void applyDrawState();

    private:

        bool mDrawBegan; // True, if and only if between drawBegin() and drawEnd().

        D3DFont * mFont; // Font class

        D3DQuad * mQuad; // Quad renderer class

        D3DDrawState mDrawState;

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
        void dumpCurrentState( StrA & );

        //@}
    };
}}

#if GN_ENABLE_INLINE
#include "d3dRenderStateBlockMgr.inl"
#include "d3dBufferMgr.inl"
#endif

// *****************************************************************************
//                           End of d3dRenderer.h
// *****************************************************************************
#endif // __GN_GFX_D3DRENDERER_H__
