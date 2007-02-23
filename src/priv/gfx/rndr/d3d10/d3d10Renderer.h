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

        ID3D10Device * getDevice() const { return mDevice; }

    private :

        bool dispInit() { return true; }
        void dispQuit() {}
        bool dispOK() const { return true; }
        void dispClear()
        {
            mAdapter = 0;
            mDevice = 0;
            mSwapChain = 0;
            mRTView = 0;
        }

        bool dispDeviceCreate();
        void dispDeviceDestroy();

    private :

        IDXGIAdapter   * mAdapter;
        ID3D10Device   * mDevice;
        IDXGISwapChain * mSwapChain;
        ID3D10RenderTargetView * mRTView;

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
        virtual Texture * createTexture( const TextureDesc & desc, const TextureLoader & loader );
        virtual VtxFmtHandle createVtxFmt( const VtxFmtDesc & );
        virtual VtxBuf * createVtxBuf( size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader );
        virtual IdxBuf * createIdxBuf( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader );

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

        //@}


    // ********************************************************************
    //
    /// \name Context manager
    //
    // ********************************************************************

        //@{

    public:

        virtual void setContext( const RendererContext & newContext ) {}
        virtual void rebindContext( RendererContext::FieldFlags ) {}
        virtual const RenderStateBlockDesc & getCurrentRenderStateBlock() const { return mContext.rsb; }

    private :

        bool contextInit() { return true; }
        void contextQuit() {}
        bool contextOK() const { return true; }
        void contextClear() {}
        bool contextDeviceCreate() { return true; }
        void contextDeviceDestroy() {}

    private:

        RendererContext mContext;

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
        virtual void clearScreen( const Vector4f & c, float z, UInt32 s, BitFields flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numPrims,
                                  size_t        startVtx,
                                  size_t        minVtxIdx,
                                  size_t        numVtx,
                                  size_t        startIdx ) {}
        virtual void draw( PrimitiveType prim,
                           size_t        numPrims,
                           size_t        startVtx ) {}
        virtual void drawIndexedUp(
                             PrimitiveType    prim,
                             size_t           numPrims,
                             size_t           numVertices,
                             const void *     vertexData,
                             size_t           strideInBytes,
                             const UInt16 * indexData ) {}
        virtual void drawUp( PrimitiveType prim,
                             size_t        numPrims,
                             const void *  vertexData,
                             size_t        strideInBytes ) {}
        virtual void drawQuads( BitFields options,
                                const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                const void * colors, size_t clrStride,
                                size_t count ) {}
        virtual void drawLines( BitFields options,
                                const void * positions,
                                size_t stride,
                                size_t count,
                                UInt32 color,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj ) {}
        virtual void drawDebugText( const char * text, int x, int y, const Vector4f & color ) {}

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
