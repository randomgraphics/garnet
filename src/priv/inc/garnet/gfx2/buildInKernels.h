#ifndef __GN_GFX2_BUILDINKERNELS_H__
#define __GN_GFX2_BUILDINKERNELS_H__
// *****************************************************************************
/// \file
/// \brief   buildin kernels
/// \author  chenli@@FAREAST (2007.7.9)
// *****************************************************************************

namespace GN { namespace gfx
{
    //@{

    struct ClearScreenParameterSet : public NoCopy
    {
        //@{
        virtual void setClearColor( bool enabled, float r, float g, float b, float a ) = 0;
        virtual void setClearDepth( bool enabled, float z ) = 0;
        virtual void setClearStencil( bool enabled, int s ) = 0;
        //@}
    };

    struct ClearScreenPortBinding
    {
        //@{
        SurfaceView colorSurface;
        SurfaceView depthSurface;
        //@}
    };

    struct ClearScreenKernel : public Kernel
    {
        //@{
        virtual ClearScreenParameterSet * createParameterSet() = 0;
        virtual KernelPortBinding         createPortBinding( const ClearScreenPortBinding & ) = 0;
        virtual void                      render( const ClearScreenParameterSet &, KernelPortBinding ) = 0;
        //@}
    };

    struct D3D9FxParameterSet : public NoCopy
    {
        //@{
        virtual void setFx( const StrA & ) = 0;
        virtual void setVSConstF( size_t fistRegister, size_t numRegisters, const float * data ) = 0;
        virtual void setPSConstF( size_t fistRegister, size_t numRegisters, const float * data ) = 0;
        virtual void setPrimType( PrimitiveType ) = 0;
        virtual void setPrimCount( size_t ) = 0;
        virtual void setBaseVertex( size_t ) = 0;
        virtual void setBaseIndex( size_t ) = 0;
        virtual void setVertexCount( size_t ) = 0;
        //@}
    };

    struct D3D9FxPortBinding
    {
        //@{
        SurfaceView renderTargets[4];
        SurfaceView depth;
        SurfaceView textures[8];
        SurfaceView vtxbufs[8];
        SurfaceView idxbuf;
        //@}
    };

    struct D3D9FxKernel : public Kernel
    {
        //@{
        virtual D3D9FxParameterSet * createParameterSet() = 0;
        virtual KernelPortBinding    createPortBinding( const D3D9FxPortBinding & ) = 0;
        virtual void                 render( const D3D9FxParameterSet &, KernelPortBinding ) = 0;
        //@}
    };

    //@}

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_BUILDINKERNELS_H__
