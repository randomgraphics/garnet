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
    struct GN_GFX2_D3D9_PUBLIC D3D9KernelPortDesc : public KernelPortDesc
    {
        ///
        /// the port type
        ///
        D3D9KernelPortType portType;

        ///
        /// Surface type that this port expects.
        ///
        D3D9SurfaceType surfaceType;
    };

    ///
    /// base D3D9 kernel port class
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9KernelPort
    {
        D3D9GraphicsSystem & mGraphicsSystem;

    protected:

        D3D9KernelPortDesc mDesc; ///< port descriptor

    public:

        ///
        /// ctor
        ///
        D3D9KernelPort( D3D9GraphicsSystem & gs ) : mGraphicsSystem(gs) {}

        ///
        /// get graphics system
        ///
        D3D9GraphicsSystem & gs() const { return mGraphicsSystem; }

        ///
        /// get descriptor
        ///
        const D3D9KernelPortDesc & getDesc() const { return mDesc; }

        ///
        /// check surface compatility
        ///
        virtual bool compatible( const Surface * surf ) const = 0;

        ///
        /// bind surface to device
        ///
        virtual void bind( const KernelBindingTarget & ) const = 0;
    };

    ///
    /// D3D9 render target port (accept render target texture)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9RenderTargetPort : public D3D9KernelPort
    {
    public:

        //@{
        D3D9RenderTargetPort( D3D9GraphicsSystem & gs );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const KernelBindingTarget & ) const;
        //@}
    };

    ///
    /// D3D9 render target port (accept depth surface)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9DepthBufferPort : public D3D9KernelPort
    {
    public:

        //@{
        D3D9DepthBufferPort( D3D9GraphicsSystem & gs );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const KernelBindingTarget & ) const;
        //@}
    };

    ///
    /// D3D9 render target port (accepts texture)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9TexturePort : public D3D9KernelPort
    {
    public:

        //@{
        D3D9TexturePort( D3D9GraphicsSystem & gs );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const KernelBindingTarget & ) const;
        //@}
    };

    ///
    /// D3D9 render target port (accepts vertex buffer)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9VtxBufPort : public D3D9KernelPort
    {
        UInt32 mStage;

    public:

        ///
        /// ctor
        ///
        D3D9VtxBufPort( D3D9GraphicsSystem & gs, UInt32 stage );

        //@{
        void setStride( UInt32 );      // call this only if this port requires fixed stride.
        void setVertexCount( UInt32 ); // call this only if this port requires a fixed vertex count.
        void addRequiredAttribute( const SurfaceAttributeTemplate & );
        void addRequiredAttribute( const char * s, UInt32 offset = -1 );
        void addOptionalAttribute( const SurfaceAttributeTemplate & );
        //@}

        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const KernelBindingTarget & ) const;
        //@}
    };

    ///
    /// D3D9 render target port (accept index buffer)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9IdxBufPort : public D3D9KernelPort
    {
    public:

        //@{
        D3D9IdxBufPort( D3D9GraphicsSystem & gs );
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const KernelBindingTarget & ) const;
        //@}
    };

    ///
    /// base D3D9 kernel binding
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9KernelBinding
    {
        D3D9Kernel & mKernel;

        struct BindItem
        {
            UInt32              port;   ///< port handle
            KernelBindingTarget target; ///< binding target
        };

        DynaArray<BindItem>           mBindItems;
        IDirect3DVertexDeclaration9 * mVtxDecl;
        bool                          mHasZBuf;
        bool                          mHasIdxBuf;

    public:

        ///
        /// ctor
        ///
        D3D9KernelBinding( D3D9Kernel & e );

        ///
        /// dtor
        ///
        virtual ~D3D9KernelBinding();

        ///
        /// binding setup
        ///
        bool setup( const KernelBindingDesc & ebd );

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
        D3D9Kernel( GraphicsSystem & gs ) : BaseKernel(gs), mDefaultBinding(0) {}

        ///
        /// dtor
        ///
        ~D3D9Kernel() { if( mDefaultBinding ) deleteBinding( mDefaultBinding ); }

        ///
        /// get D3D9 graphic system
        ///
        D3D9GraphicsSystem & d3d9gs() const { return (D3D9GraphicsSystem&)gs(); }

        UInt32                 getFirstPortHandle() const { return mPorts.first(); }
        UInt32                 getNextPortHandle( UInt32 current ) const { return mPorts.next( current ); }
        const StrA           & getPortName( UInt32 h ){ GN_ASSERT(mPorts.validHandle(h)); return mPorts.handle2name(h); }
        const D3D9KernelPort & getPort( UInt32 h ) const { GN_ASSERT(mPorts.validHandle(h)); return *mPorts[h]; }
        const D3D9KernelPort * getPort( const StrA & name ) const; ///< return NULL for invalid name
        D3D9KernelPort       * getPort( const StrA & name ); ///< return NULL for invalid name

        //@}

        /// \name from Kernel
        //@{
        virtual const KernelPortDesc * getPortDesc( const StrA & name ) const;
        virtual bool                   compatible( const Surface * surf, const StrA & port );
        virtual KernelBinding          createBinding( const KernelBindingDesc & );
        virtual void                   deleteBinding( KernelBinding );
        //@}

    protected:

        //@{

        ///
        /// \note D3D9Kernel class does _NOT_ hold the ownership of the port instance.
        ///
        void addPortRef( const StrA & name, D3D9KernelPort * port );

        ///
        /// get port binding by handle
        ///
        D3D9KernelBinding & getPortBinding( KernelBinding b )
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

        ///
        /// apply port binding
        ///
        void applyBinding( KernelBinding b )
        {
            GN_GUARD_SLOW;

            if( 0 == b )
            {
                if( 0 == mDefaultBinding )
                {
                    mDefaultBinding = createDefaultBinding();
                    if( 0 == mDefaultBinding ) return;
                }

                b = mDefaultBinding;
            }

            GN_ASSERT( mBindings.validHandle( b ) );
            GN_ASSERT( mBindings[b] );

            mBindings[b]->apply();

            GN_UNGUARD_SLOW;
        }

        //@}

    private:

        typedef NamedHandleManager<D3D9KernelPort*,UInt32>      PortContainer;
        typedef HandleManager<D3D9KernelBinding*,KernelBinding> KernelBindingContainer;

        PortContainer          mPorts;
        KernelBindingContainer mBindings;
        KernelBinding          mDefaultBinding;

    private:

        virtual KernelBinding createDefaultBinding();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9KERNEL_H__
