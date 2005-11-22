#ifndef __GN_GFX_D3DRENDERER_H__
#define __GN_GFX_D3DRENDERER_H__
// *****************************************************************************
//! \file    d3dRenderer.h
//! \brief   D3D renderer class
//! \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicRenderer.h"

#if GN_MSVC
#pragma warning(push)
#pragma warning(disable:4100) // unused parameters
#endif

//! \def GNGFX_D3DCAPS
//! Define D3D special caps.

namespace GN { namespace gfx
{
    struct D3DResource; // Forward declaration of D3DResource.

    //!
    //! D3D vertex buffer binding descriptor
    //!
    struct D3DVtxBindingDesc
    {
        VtxFmtDesc                              format; //!< vertex format
        AutoComPtr<IDirect3DVertexDeclaration9> decl;   //!< D3D decl
    };

    //!
    //! D3D vertex buffer state
    //!
    struct D3DVtxBufState
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
                int  vtxBufs    : 16; //!< Vertex buffer dirty flags
                bool vtxBinding : 1;  //!< Vertex binding dirty flag
                int  reserved   : 15; //!< Reserved for future use.
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

        VtxBufDesc            vtxBufs[MAX_VERTEX_STREAMS]; //!< front vertex buffers
        uint32_t              vtxBinding;                  //!< front vertex binding handle
        DirtyFlags            dirtyFlags;                  //!< dirty flags

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
        virtual bool changeOptions( const RendererOptions & ro,
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
            mD3D = 0;
            mDevice = 0;
        }

        bool dispDeviceCreate();
        bool dispDeviceRestore();
        void dispDeviceDispose();
        void dispDeviceDestroy();

    private :
        bool                    mDispOK; //!< true between dispDeviceRestore() and dispDeviceDispose()
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
        virtual Shader * createVertexShader( ShadingLanguage, const StrA & ) { GN_UNIMPL(); return 0; }
        virtual Shader * createPixelShader( ShadingLanguage, const StrA & ) { GN_UNIMPL(); return 0; }
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
        virtual Texture *
        createTexture( TexType textype,
                       uint32_t sx, uint32_t sy, uint32_t sz,
                       uint32_t levels,
                       ClrFmt format,
                       uint32_t usages );
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

        void updateTextureFilters( uint32_t stage, const D3DTEXTUREFILTERTYPE * filters );
        void updateTextureWraps( uint32_t stage, const D3DTEXTUREADDRESS * strq );

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

        void updateVtxBufState();
        void updateVtxBufs(); // this is only called by updateVtxBufState().

    private :

        HandleManager<D3DVtxBindingDesc,uint32_t> mVtxBindings;

        D3DVtxBufState mVtxBufState;

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
        virtual void setRenderTarget( size_t index, const Texture * texture, TexFace face );
        virtual void setRenderDepth( const Texture * texture, TexFace face );
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
        virtual void drawTextW( const wchar_t * text, int x, int y, const Vector4f & color );

        // private functions
    private:
        bool drawInit() { return true; }
        void drawQuit() {}
        bool drawOK() const { return true; }
        void drawClear()
        {
            mDrawBegan = false;
            mFont = 0;
            mFontHeight = 0;
            mDefaultRT0 = mDefaultDepth = mAutoDepth = 0;
            mCurrentRTSize.set( 0, 0 );
            mAutoDepthSize.set( 0, 0 );
        }

        bool drawDeviceCreate();
        bool drawDeviceRestore();
        void drawDeviceDispose();
        void drawDeviceDestroy();

        bool createFont();
        bool handleDeviceLost();

    private:

        bool mDrawBegan; // True, if and only if between drawBegin() and drawEnd().

        //
        // Font stuff
        //
        LPD3DXFONT mFont;
        int        mFontHeight;

        //
        // Render target stuff
        //
        LPDIRECT3DSURFACE9
            mDefaultRT0,    // default color buffer
            mDefaultDepth,  // default depth buffer
            mAutoDepth;     // automatic depth buffer
        RenderTargetTextureDesc
            mCurrentRTs[4], // current color buffers.
            mCurrentDepth;  // current depth buffer
        Vector2<uint32_t>
            mCurrentRTSize, // current render target size
            mAutoDepthSize; // size of automatic depth buffer

        //@}
    };
}}

#if GN_MSVC
#pragma warning(pop)
#endif

#if GN_ENABLE_INLINE
#include "d3dRenderStateBlockMgr.inl"
#include "d3dTextureMgr.inl"
#include "d3dBufferMgr.inl"
#endif

// *****************************************************************************
//                           End of d3dRenderer.h
// *****************************************************************************
#endif // __GN_GFX_D3DRENDERER_H__
