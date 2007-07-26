#ifndef __GN_GFX2_D3D9QUADKERNEL_H__
#define __GN_GFX2_D3D9QUADKERNEL_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 quad kernel
/// \author  chenli@@FAREAST (2007.6.29)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// quad vertex stream
    ///
    class D3D9QuadStream : public StreamSource, public D3D9UnstableResource, public StdClass
    {
        GN_DECLARE_STDCLASS( D3D9QuadStream, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9QuadStream( D3D9GraphicsSystem & gs, BaseKernel & k, const StrA & name );
        virtual ~D3D9QuadStream() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public methods
        // ********************************
    public:

        //@{

        enum
        {
            NUM_VTXBUFS = 128,
            MAX_QUADS   = 256,
        };

        // from parents
        virtual void                     push( const void * data, size_t bytes );
        virtual size_t                   freeBytes() const { return (MAX_QUADS - mNumQuads) * sizeof(QuadVertex); }
        virtual bool                     onRestore() { return true; }
        virtual void                     onDispose() {}

        inline void                      draw();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct QuadVertex
        {
            GN::Vector3f pos;  // position in [0,1] space. [0,0] is left-top corner; [1,1] is right-bottom corner.
            UInt32       clr;  // color in R-G-B-A format
            GN::Vector2f tex;
            float        _[2]; // padding to 32 bytes
        };
        GN_CASSERT( sizeof(QuadVertex) == 32 );

        AutoComPtr<IDirect3DVertexBuffer9> mVtxBufs[NUM_VTXBUFS];
        size_t                             mActiveVB;
        size_t                             mNumQuads;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// rendering 2D quads
    ///
    class D3D9QuadKernel : public D3D9Kernel, public StdClass
    {
        GN_DECLARE_STDCLASS( D3D9QuadKernel, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    private:
        D3D9QuadKernel( D3D9GraphicsSystem & gs );
    public:
        virtual ~D3D9QuadKernel() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        ///
        /// kernel name
        ///
        static const char * KERNEL_NAME() { return "QUAD"; }

        ///
        /// kernel factory
        ///
        static Kernel * sFactory( GraphicsSystem & gs )
        {
            AutoObjPtr<D3D9QuadKernel> p( new D3D9QuadKernel( safeCastRef<D3D9GraphicsSystem>(gs) ) );
            if( !p->init() ) return 0;
            return p.detach();
        }

        virtual KernelParameterSet * createParameterSet();
        virtual void                 render( const KernelParameterSet &, KernelPortBinding );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        D3D9RenderTargetPort                    mTarget0;
        D3D9DepthBufferPort                     mDepth;
        D3D9TexturePort                         mTexture;
        D3D9QuadStream                          mQuads;

        AutoComPtr<IDirect3DVertexShader9>      mVs;
        AutoComPtr<IDirect3DPixelShader9>       mPs;
        AutoComPtr<IDirect3DVertexDeclaration9> mDecl;
        AutoComPtr<IDirect3DIndexBuffer9>       mIdxBuf;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}
// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9QUADKERNEL_H__
