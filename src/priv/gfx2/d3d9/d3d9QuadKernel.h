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
    /// rendering 2D quads
    ///
    class D3D9QuadKernel : public QuadKernel, public D3D9KernelBase, public StdClass
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
        /// kernel factory
        ///
        static Kernel * sFactory( GraphicsSystem & gs )
        {
            AutoObjPtr<D3D9QuadKernel> p( new D3D9QuadKernel( safeCastRef<D3D9GraphicsSystem>(gs) ) );
            if( !p->init() ) return 0;
            return p.detach();
        }

        virtual QuadKernelParameterSet * createParameterSet();
        virtual KernelPortBinding        createPortBinding( const QuadKernelPortBinding & b ) { return D3D9KernelBase::createPortBinding( &b.target ); }
        virtual void                     deletePortBinding( KernelPortBinding b ) { D3D9KernelBase::deletePortBinding( b ); }
        virtual bool                     compatible( const Surface * surf, const StrA & port ) const { return D3D9KernelBase::compatible( surf, port ); }
        virtual void                     pushVertices( const QuadKernelVertex * vertices, size_t count );
        virtual size_t                   getAvailableVertices() const;
        virtual void                     render( const KernelParameterSet &, KernelPortBinding );

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        enum
        {
            NUM_VTXBUFS = 128,
            MAX_QUADS   = 256,
        };

        D3D9RenderTargetPort                    mTarget0;
        D3D9DepthBufferPort                     mDepth;
        D3D9TexturePort                         mTexture;

        AutoComPtr<IDirect3DVertexShader9>      mVs;
        AutoComPtr<IDirect3DPixelShader9>       mPs;
        AutoComPtr<IDirect3DVertexDeclaration9> mDecl;
        AutoComPtr<IDirect3DIndexBuffer9>       mIdxBuf;
        AutoComPtr<IDirect3DVertexBuffer9>      mVtxBufs[NUM_VTXBUFS];
        size_t                                  mActiveVB;
        size_t                                  mNumQuads;

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
