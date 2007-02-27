#ifndef __GN_GFXD3D10_D3D10RENDERER_H__
#define __GN_GFXD3D10_D3D10RENDERER_H__
// *****************************************************************************
/// \file    d3d10Renderer.h
/// \brief   D3D10 renderer class
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicRenderer.h"

#pragma warning(disable:4100)

namespace GN { namespace gfx
{
    // Forward declarations
    class D3D10Resource;
    class D3D10RTMgr;

    ///
    /// D3D9 vertex buffer layout descriptor
    ///
    struct D3D10VtxLayoutDesc
    {
        VtxFmtDesc                    format; ///< vertex format
        AutoComPtr<ID3D10InputLayout> layout; ///< D3D layout
    };

    ///
    /// D3D10 renderer class
    ///
    class D3D10Renderer : public BasicRenderer
    {
        GN_DECLARE_STDCLASS(D3D10Renderer, BasicRenderer);

        static Logger * sLogger;

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        D3D10Renderer()          { clear(); }
        virtual ~D3D10Renderer() { quit(); }
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
        void deviceDestroy();

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

        virtual void * getD3DDevice() const { return mDevice; }
        virtual void * getOGLRC() const { return 0; }

    public :

        ID3D10Device   * getDevice() const { return mDevice; }
        IDXGISwapChain * getSwapChain() const { return mSwapChain; }

    private :

        bool dispInit() { return true; }
        void dispQuit() {}
        bool dispOK() const { return true; }
        void dispClear()
        {
            mAdapter = 0;
            mDevice = 0;
            mSwapChain = 0;
        }

        bool dispDeviceCreate();
        void dispDeviceDestroy();

    private :

        IDXGIAdapter   * mAdapter;
        ID3D10Device   * mDevice;
        IDXGISwapChain * mSwapChain;

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
        bool capsOK() const { return true; }
        void capsClear() {}

        bool capsDeviceCreate();
        void capsDeviceDestroy() {}

    private :

        //@}

    // ************************************************************************
    //
    /// \name                     Resource Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual Shader * createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints );
        virtual Texture * createTexture( const TextureDesc & desc );
        virtual VtxFmtHandle createVtxFmt( const VtxFmtDesc & );
        virtual VtxBuf * createVtxBuf( const VtxBufDesc & desc );
        virtual IdxBuf * createIdxBuf( const IdxBufDesc & desc );

    public :

        ///
        /// Insert resource into resource list. Can be only called by
        /// constructor of D3D9Resource.
        ///
        void insertResource( D3D10Resource * p )
        {
            mResourceList.push_back(p);
        }

        ///
        /// Remove resource from resource list. Can be only called by
        /// destructor of D3D9Resource.
        ///
        void removeResource( D3D10Resource * p )
        {
            mResourceList.remove(p);
        }

    private:

        bool resourceInit() { return true; }
        void resourceQuit() {}
        bool resourceOK() const { return true; }
        void resourceClear() {}
        bool resourceDeviceCreate();
        void resourceDeviceDestroy();

    private :

        std::list<D3D10Resource*> mResourceList;
        HandleManager<D3D10VtxLayoutDesc,VtxFmtHandle> mVtxFmts;

        //@}


    // ********************************************************************
    //
    /// \name Context manager
    //
    // ********************************************************************

        //@{

    public:

        virtual void setContext( const RendererContext & newContext );
        virtual void rebindContext( RendererContext::FieldFlags );
        virtual const RenderStateBlockDesc & getCurrentRenderStateBlock() const;

    private :

        bool contextInit() { return true; }
        void contextQuit() {}
        bool contextOK() const { return true; }
        void contextClear() { mContext.resetToDefault(); mRTMgr = 0; }
        bool contextDeviceCreate();
        void contextDeviceDestroy();

        GN_INLINE void bindContext(
            const RendererContext & newContext,
            RendererContext::FieldFlags newFlag,
            bool forceRebind );

        GN_INLINE void bindContextState(
            const RendererContext & newContext,
            RendererContext::FieldFlags newFlag,
            bool forceRebind );

        GN_INLINE void bindContextData(
            const RendererContext & newContext,
            RendererContext::FieldFlags newFlag,
            bool forceRebind );

    private:

        RendererContext mContext;

        D3D10RTMgr * mRTMgr;

        //@}

    // ************************************************************************
    //
    /// \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public: // from Renderer
        virtual bool drawBegin();
        virtual void drawEnd();
        virtual void clearScreen( const Vector4f & c, float z, UInt8 s, BitFields flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numprim,
                                  size_t        startvtx,
                                  size_t        minvtxidx,
                                  size_t        numvtx,
                                  size_t        startidx );
        virtual void draw( PrimitiveType prim,
                           size_t        numprim,
                           size_t        startvtx );
        virtual void drawIndexedUp(
                             PrimitiveType    prim,
                             size_t           numprim,
                             size_t           numvtx,
                             const void *     vertexData,
                             size_t           strideInBytes,
                             const UInt16 * indexData );
        virtual void drawUp( PrimitiveType prim,
                             size_t        numprim,
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
        bool drawOK() const { return true; }
        void drawClear() {}

        bool drawDeviceCreate() { return true; }
        void drawDeviceDestroy() {}

    private:

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
        //
        // ********************************************************************

        //@{

        virtual void dumpCurrentState( StrA & ) const {}

        //@}
    };
}}

// *****************************************************************************
//                           End of d3d10Renderer.h
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10RENDERER_H__
