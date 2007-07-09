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

    struct ClearScreenParameterSet : public KernelParameterSet
    {
        //@{
        virtual void setClearColor( bool enabled, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f ) = 0;
        virtual void setClearDepth( bool enabled, float z = 1.0f ) = 0;
        virtual void setClearStencil( bool enabled, int s = 0 ) = 0;
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
        virtual const char              * getName() const { return "CLEAR_SCREEN"; }
        virtual ClearScreenParameterSet * createParameterSet() = 0;
        virtual KernelPortBinding         createPortBinding( const ClearScreenPortBinding & ) = 0;
        //@}
    };

    struct Hlsl9ParameterSet : public NoCopy
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

    struct Hlsl9PortBinding
    {
        //@{
        SurfaceView renderTargets[4];
        SurfaceView depth;
        SurfaceView textures[8];
        SurfaceView vtxbufs[8];
        SurfaceView idxbuf;
        //@}
    };

    struct Hlsl9Kernel : public Kernel
    {
        //@{
        virtual const char        * getName() const { return "HLSL9"; }
        virtual Hlsl9ParameterSet * createParameterSet() = 0;
        virtual KernelPortBinding   createPortBinding( const Hlsl9PortBinding & ) = 0;
        //@}
    };

    //@}

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_BUILDINKERNELS_H__
