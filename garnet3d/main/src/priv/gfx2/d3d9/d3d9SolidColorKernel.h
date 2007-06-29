#ifndef __GN_GFX2_D3D9SOLIDCOLORKERNEL_H__
#define __GN_GFX2_D3D9SOLIDCOLORKERNEL_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 solid-color kernel
/// \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// solid color kernel
    ///
    class D3D9SolidColorKernel : public D3D9Kernel
    {
        KernelParameterHandle mPvw;   ///< 4x4 matrix: project-view-world transformation
        KernelParameterHandle mColor; ///< 4D float vector: [R,G,B,A]
        D3D9RenderTargetPort  mTarget0;
        D3D9DepthBufferPort   mDepth;
        D3D9VtxBufPort        mVtxBuf;
        D3D9IdxBufPort        mIdxBuf; ///< this is optional.

        static Kernel * sCreator( GraphicsSystem & gs ) { return new D3D9SolidColorKernel(GN_SAFE_CAST<D3D9GraphicsSystem&>(gs)); }

        ///
        /// ctor
        ///
        D3D9SolidColorKernel( D3D9GraphicsSystem & gs )
            : D3D9Kernel( gs )
            , mTarget0( gs )
            , mDepth( gs )
            , mVtxBuf( gs, 0 )
            , mIdxBuf( gs )
        {
            // setup parameters
            KernelParameterDesc p;

            p.type  = KERNEL_PARAMETER_TYPE_FLOAT4X4;
            mPvw    = addParameter( "PVW", p );

            p.type  = KERNEL_PARAMETER_TYPE_FLOAT4;
            mColor  = addParameter( "COLOR", p );

            // setup ports
            addPortRef( "TARGET0", &mTarget0 );
            addPortRef( "DEPTH", &mDepth );
            addPortRef( "VTXBUF", &mVtxBuf );
            addPortRef( "IDXBUF", &mIdxBuf );

            // setup vertx buffer port
            mVtxBuf.addRequiredAttribute( "POS0" );
        }

    public:

        ///
        /// get kernel factory
        ///
        static KernelFactory sGetFactory()
        {
            KernelFactory f;
            f.quality = 0;
            f.creator = &sCreator;
            return f;
        }
 
        virtual void render( const KernelParameterSet & param, KernelBinding binding )
        {
            GN_ASSERT( &param.getKernel() == (Kernel*)this );

            applyBinding( binding );

            const KernelParameter * m = param.getParameter( mPvw );
            const KernelParameter * c = param.getParameter( mColor );

            GN_UNUSED_PARAM( m );
            GN_UNUSED_PARAM( c );
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9SOLIDCOLORKERNEL_H__
