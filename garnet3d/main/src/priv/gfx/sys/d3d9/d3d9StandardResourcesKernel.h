#ifndef __GN_GFX_D3D9STANDARDRESOURCESKERNEL_H__
#define __GN_GFX_D3D9STANDARDRESOURCESKERNEL_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 standard resources kernel
/// \author  chen@@CHENLI-HOMEPC (2007.8.1)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// standard resources kernel
    ///
    class D3D9StandardResourcesKernel : public D3D9Kernel
    {
        D3D9VtxBufPort       mVtxBuf;
        D3D9IdxBufPort       mIdxBuf;
        D3D9TexturePort      mTexture;
        D3D9RenderTargetPort mTarget;
        D3D9DepthBufferPort  mDepth;

        D3D9StandardResourcesKernel( D3D9GraphicsSystem & gs )
            : D3D9Kernel( KERNEL_NAME(), gs )
            , mVtxBuf( gs, baseref(), "VERTEX_BUFFER", 0 )
            , mIdxBuf( gs, baseref(), "INDEX_BUFFER" )
            , mTexture( gs, baseref(), "TEXTURE", 0 )
            , mTarget( gs, baseref(), "RENDER_TARGET", 0 )
            , mDepth( gs, baseref(), "DEPTH_BUFFER" )
        {
        }

        struct DummyParameterSet : public KernelParameterSet
        {
            DummyParameterSet( Kernel & k ) : KernelParameterSet( k ) {}
            virtual KernelParameter & get( size_t ) { return DummyKernelParameter::sGetInstance(); }
        };

    public:

        ///
        /// return kernel name
        ///
        static const char * KERNEL_NAME() { return "STANDARD_RESOURCES"; }

        ///
        /// kernel factory
        ///
        static Kernel * sFactory( GraphicsSystem & gs )
        {
            PIXPERF_FUNCTION_EVENT();
            return new D3D9StandardResourcesKernel( safeCastRef<D3D9GraphicsSystem>(gs) );
        }

        virtual KernelParameterSet * createParameterSet()
        {
            return new DummyParameterSet( *this );
        }

        virtual void render( const KernelParameterSet & , const KernelPortBinding * )
        {
            // do nothing
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_D3D9STANDARDRESOURCESKERNEL_H__
