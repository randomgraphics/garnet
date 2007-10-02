#ifndef __GN_GFX_D3D9KERNEL_H__
#define __GN_GFX_D3D9KERNEL_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 base kernel interfaces
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx
{
    class D3D9GraphicsSystem;
    class D3D9Kernel;

    ///
    /// D3D9 kernel port type
    ///
    enum D3D9KernelPortType
    {
        //@{
        D3D9_KERNEL_PORT_RENDER_TARGET,
        D3D9_KERNEL_PORT_DEPTH_BUFFER,
        D3D9_KERNEL_PORT_TEXTURE,
        D3D9_KERNEL_PORT_VTXBUF,
        D3D9_KERNEL_PORT_IDXBUF,
        //@}
    };

    ///
    /// base D3D9 kernel port class
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9KernelPort : public BaseKernelPort
    {
        D3D9GraphicsSystem     & mGraphicsSystem;
        const D3D9KernelPortType mPortType;

    public:

        ///
        /// ctor
        ///
        D3D9KernelPort(
            D3D9GraphicsSystem & gs,
            D3D9KernelPortType   type,
            BaseKernel         & k,
            const StrA         & name,
            const Guid         & allowedSurfaceType )
            : BaseKernelPort( k, name, &allowedSurfaceType, 1 )
            , mGraphicsSystem(gs)
            , mPortType(type)
        {
        }

        ///
        /// get graphics system
        ///
        D3D9GraphicsSystem & d3d9gs() const { return mGraphicsSystem; }

        ///
        /// get descriptor
        ///
        const D3D9KernelPortType & getPortType() const { return mPortType; }

        ///
        /// bind surface to device
        ///
        virtual void bind( const SurfaceView & ) const = 0;
    };

    ///
    /// D3D9 render target port (accept render target texture)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9RenderTargetPort : public D3D9KernelPort
    {
        UInt32 mStage;
    public:

        //@{
        D3D9RenderTargetPort( D3D9GraphicsSystem & gs, BaseKernel & k, const StrA & name, UInt32 stage );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const SurfaceView & ) const;
        //@}
    };

    ///
    /// D3D9 depth buffer port (accept depth surface)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9DepthBufferPort : public D3D9KernelPort
    {
    public:

        //@{
        D3D9DepthBufferPort( D3D9GraphicsSystem & gs, BaseKernel & k, const StrA & name );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const SurfaceView & ) const;
        //@}
    };

    ///
    /// D3D9 texture port (accepts texture)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9TexturePort : public D3D9KernelPort
    {
        UInt32 mStage;

    public:

        //@{
        D3D9TexturePort( D3D9GraphicsSystem & gs, BaseKernel & k, const StrA & name, UInt32 stage );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const SurfaceView & ) const;
        //@}
    };

    ///
    /// D3D9 vertex buffer port (accepts vertex buffer)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9VtxBufPort : public D3D9KernelPort
    {
        UInt32 mStage;

    public:

        ///
        /// ctor
        ///
        D3D9VtxBufPort( D3D9GraphicsSystem & gs, BaseKernel & k, const StrA & name, UInt32 stage );

        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const SurfaceView & ) const;
        //@}
    };

    ///
    /// D3D9 index buffer port (accept index buffer)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9IdxBufPort : public D3D9KernelPort
    {
    public:

        //@{
        D3D9IdxBufPort( D3D9GraphicsSystem & gs, BaseKernel & k, const StrA & name );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const SurfaceView & ) const;
        //@}
    };

    ///
    /// base D3D9 kernel binding
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9KernelPortBinding : public KernelPortBinding
    {
        D3D9Kernel & mKernel;

        struct BindItem
        {
            size_t      port;   ///< port index
            SurfaceView target; ///< binding target
        };

        DynaArray<BindItem>           mBindItems;
        IDirect3DVertexDeclaration9 * mVtxDecl;
        bool                          mHasZBuf;
        bool                          mHasIdxBuf;

    public:

        ///
        /// ctor
        ///
        D3D9KernelPortBinding( D3D9Kernel & e );

        ///
        /// dtor
        ///
        virtual ~D3D9KernelPortBinding();

        ///
        /// binding setup
        ///
        bool setup( const KernelPortBindingDesc & );

        /// \name properties
        //@{
        bool hasZBuf() const { return mHasZBuf; }
        bool hasIdxBuf() const { return mHasIdxBuf; }
        //@}

        ///
        /// apply binding to rendering device
        ///
        void apply() const;
    };

    ///
    /// base D3D9 kernel
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9Kernel : public BaseKernel
    {
    public:

        //@{

        ///
        /// ctor
        ///
        D3D9Kernel( const char * name, D3D9GraphicsSystem & gs );

        ///
        /// dtor
        ///
        ~D3D9Kernel();

        D3D9GraphicsSystem      & d3d9gs() const { return mGraphicsSystem; }

        ///
        /// create port binding (inherited from Kernel)
        ///
        KernelPortBinding       * createPortBinding( const KernelPortBindingDesc & );

        ///
        /// get port binding by handle
        ///
        D3D9KernelPortBinding   * getDefaultPortBinding()
        {
            GN_GUARD_SLOW;

            if( 0 == mDefaultPortBinding )
            {
                KernelPortBindingDesc desc;
                mDefaultPortBinding = safeCastPtr<D3D9KernelPortBinding>( createPortBinding( desc ) );
                if( 0 == mDefaultPortBinding )
                {
                    GN_UNEXPECTED();
                }
            }

            return mDefaultPortBinding;

            GN_UNGUARD_SLOW;
        }

        //@}

    private:

        D3D9GraphicsSystem        & mGraphicsSystem;
        D3D9KernelPortBinding     * mDefaultPortBinding;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_D3D9KERNEL_H__