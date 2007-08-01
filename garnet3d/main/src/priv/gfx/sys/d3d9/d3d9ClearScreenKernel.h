#ifndef __GN_GFX_D3D9CLEARSCREENKERNEL_H__
#define __GN_GFX_D3D9CLEARSCREENKERNEL_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 clear screen kernel
/// \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// clear screen kernel
    ///
    class D3D9ClearScreenKernel : public D3D9Kernel
    {
        D3D9RenderTargetPort mTarget0;
        D3D9DepthBufferPort  mDepth;

        D3D9ClearScreenKernel( D3D9GraphicsSystem & gs );

    public:

        ///
        /// return kernel name
        ///
        static const char * KERNEL_NAME() { return "CLEAR_SCREEN"; }

        ///
        /// kernel factory
        ///
        static Kernel * sFactory( GraphicsSystem & gs )
        {
            PIXPERF_FUNCTION_EVENT();
            return new D3D9ClearScreenKernel( safeCastRef<D3D9GraphicsSystem>(gs) );
        }

        virtual KernelParameterSet * createParameterSet();
        virtual void                 render( const KernelParameterSet & paramset, const KernelPortBinding * binding );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_D3D9CLEARSCREENKERNEL_H__
