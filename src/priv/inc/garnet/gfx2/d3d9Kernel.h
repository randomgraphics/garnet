#ifndef __GN_GFX2_D3D9KERNEL_H__
#define __GN_GFX2_D3D9KERNEL_H__
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
    /// D3D9 kernel port descriptor
    ///
    struct D3D9KernelPortDesc : public KernelPortDesc
    {
        //@{
        StrA                  name;
        D3D9KernelPortType    portType;
        D3D9SurfaceType       surfaceType;
        //@}
    };

    ///
    /// base D3D9 kernel port class
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9KernelPort : public BaseKernelPort
    {
        D3D9GraphicsSystem & mGraphicsSystem;

    protected:

        D3D9KernelPortDesc mDesc; ///< port descriptor

    public:

        ///
        /// ctor
        ///
        D3D9KernelPort( D3D9GraphicsSystem & gs, const char * name ) : mGraphicsSystem(gs)
        {
            mDesc.name = name;
        }

        ///
        /// get graphics system
        ///
        D3D9GraphicsSystem & gfxsys() const { return mGraphicsSystem; }

        ///
        /// get descriptor
        ///
        const D3D9KernelPortDesc & getDesc() const { return mDesc; }

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
        D3D9RenderTargetPort( D3D9GraphicsSystem & gs, const char * name, UInt32 stage );
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
        D3D9DepthBufferPort( D3D9GraphicsSystem & gs, const char * name );
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
        D3D9TexturePort( D3D9GraphicsSystem & gs, const char * name , UInt32 stage );
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
        D3D9VtxBufPort( D3D9GraphicsSystem & gs, const char * name , UInt32 stage );

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
        D3D9IdxBufPort( D3D9GraphicsSystem & gs, const char * name );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const SurfaceView & ) const;
        //@}
    };

    ///
    /// base D3D9 kernel binding
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9KernelPortBinding
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
        D3D9Kernel( D3D9GraphicsSystem & gs );

        ///
        /// dtor
        ///
        ~D3D9Kernel();

        D3D9GraphicsSystem      & gfxsys() const { return mGraphicsSystem; }
        KernelPortBinding         createPortBinding( const KernelPortBindingDesc & );
        void                      deletePortBinding( KernelPortBinding );

        ///
        /// get port binding by handle
        ///
        D3D9KernelPortBinding & getPortBinding( KernelPortBinding b )
        {
            GN_GUARD_SLOW;

            if( 0 == b )
            {
                if( 0 == mDefaultBinding )
                {
                    mDefaultBinding = createDefaultBinding();
                    if( 0 == mDefaultBinding )
                    {
                        GN_UNEXPECTED();
                    }
                }

                b = mDefaultBinding;
            }

            GN_ASSERT( mBindings.validHandle( b ) );
            GN_ASSERT( mBindings[b] );

            return *mBindings[b];

            GN_UNGUARD_SLOW;
        }

        //@}

    private:

        typedef HandleManager<D3D9KernelPortBinding*,KernelPortBinding> KernelBindingContainer;

        D3D9GraphicsSystem        & mGraphicsSystem;
        KernelBindingContainer      mBindings;
        KernelPortBinding           mDefaultBinding;

    private:

        virtual KernelPortBinding createDefaultBinding();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9KERNEL_H__
