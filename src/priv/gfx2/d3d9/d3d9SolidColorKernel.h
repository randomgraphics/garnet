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
        size_t                mPvw;   ///< 4x4 matrix: project-view-world transformation
        size_t                mColor; ///< 4D float vector: [R,G,B,A]
        D3D9RenderTargetPort  mTarget0;
        D3D9DepthBufferPort   mDepth;
        D3D9VtxBufPort        mVtxBuf;
        D3D9IdxBufPort        mIdxBuf; ///< this is optional.


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
            mPvw   = addParameter( "PVW", KERNEL_PARAMETER_TYPE_FLOAT, 16 );
            mColor = addParameter( "COLOR", KERNEL_PARAMETER_TYPE_FLOAT, 4 );

            // setup ports
            addPortRef( "TARGET0", &mTarget0 );
            addPortRef( "DEPTH", &mDepth );
            addPortRef( "VTXBUF", &mVtxBuf );
            addPortRef( "IDXBUF", &mIdxBuf );

            // setup vertx buffer port
            //mVtxBuf.setLayoutTemplate( ... );
        }

    public:

        ///
        /// kernel creator
        ///
        static Kernel * sCreator( GraphicsSystem & gs )
        {
            return new D3D9SolidColorKernel(safeCastRef<D3D9GraphicsSystem>(gs));
        }
 
        virtual void render( const KernelParameterSet & param, KernelBinding binding )
        {
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9SOLIDCOLORKERNEL_H__
