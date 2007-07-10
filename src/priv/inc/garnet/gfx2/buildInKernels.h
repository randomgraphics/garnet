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

    struct ClearScreenKernelParameterSet;
    struct ClearScreenKernelPortBinding;
    struct ClearScreenKernel : public Kernel
    {
        //@{
        static  const char                    * KERNEL_NAME() { return "CLEAR_SCREEN"; }
        virtual const char                    * getName() const { return KERNEL_NAME(); }
        virtual ClearScreenKernelParameterSet * createParameterSet() = 0;
        virtual KernelPortBinding               createPortBinding( const ClearScreenKernelPortBinding & ) = 0;
        //@}
    };

    struct ClearScreenKernelParameterSet : public KernelParameterSet
    {
        //@{
        virtual void setClearColor( bool enabled, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f ) = 0;
        virtual void setClearDepth( bool enabled, float z = 1.0f ) = 0;
        virtual void setClearStencil( bool enabled, int s = 0 ) = 0;
        //@}
    };

    struct ClearScreenKernelPortBinding
    {
        //@{
        KernelPort target;
        KernelPort depth;
        ClearScreenKernelPortBinding()
            : target( ClearScreenKernel::KERNEL_NAME(), "TARGET" )
            , depth( ClearScreenKernel::KERNEL_NAME(), "DEPTH" )
        {
        }
        //@}
    };

    //@}

    //@{

    struct Hlsl9KernelParameterSet;
    struct Hlsl9KernelPortBinding;
    struct Hlsl9Kernel : public Kernel
    {
        //@{
        static  const char              * KERNEL_NAME() { return "HLSL9"; }
        virtual const char              * getName() const { return KERNEL_NAME(); }
        virtual Hlsl9KernelParameterSet * createParameterSet() = 0;
        virtual KernelPortBinding         createPortBinding( const Hlsl9KernelPortBinding & ) = 0;
        //@}
    };

    struct Hlsl9KernelParameterSet : public KernelParameterSet
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

    struct Hlsl9KernelPortBinding
    {
        //@{
        KernelPort  target0;
        KernelPort  target1;
        KernelPort  target2;
        KernelPort  target3;
        KernelPort  depth;
        KernelPort  texture0;
        KernelPort  texture1;
        KernelPort  texture2;
        KernelPort  texture3;
        KernelPort  texture4;
        KernelPort  texture5;
        KernelPort  texture6;
        KernelPort  texture7;
        KernelPort  vtxbuf0;
        KernelPort  vtxbuf1;
        KernelPort  vtxbuf2;
        KernelPort  vtxbuf3;
        KernelPort  vtxbuf4;
        KernelPort  vtxbuf5;
        KernelPort  vtxbuf6;
        KernelPort  vtxbuf7;
        KernelPort  idxbuf;

        Hlsl9KernelPortBinding()
            : target0( Hlsl9Kernel::KERNEL_NAME(), "TARGET0" )
            , target1( Hlsl9Kernel::KERNEL_NAME(), "TARGET1" )
            , target2( Hlsl9Kernel::KERNEL_NAME(), "TARGET2" )
            , target3( Hlsl9Kernel::KERNEL_NAME(), "TARGET3" )
            , depth( Hlsl9Kernel::KERNEL_NAME(), "DEPTH" )
            , texture0( Hlsl9Kernel::KERNEL_NAME(), "TEXTURE0" )
            , texture1( Hlsl9Kernel::KERNEL_NAME(), "TEXTURE1" )
            , texture2( Hlsl9Kernel::KERNEL_NAME(), "TEXTURE2" )
            , texture3( Hlsl9Kernel::KERNEL_NAME(), "TEXTURE3" )
            , texture4( Hlsl9Kernel::KERNEL_NAME(), "TEXTURE4" )
            , texture5( Hlsl9Kernel::KERNEL_NAME(), "TEXTURE5" )
            , texture6( Hlsl9Kernel::KERNEL_NAME(), "TEXTURE6" )
            , texture7( Hlsl9Kernel::KERNEL_NAME(), "TEXTURE7" )
            , vtxbuf0( Hlsl9Kernel::KERNEL_NAME(), "VTXBUF0" )
            , vtxbuf1( Hlsl9Kernel::KERNEL_NAME(), "VTXBUF1" )
            , vtxbuf2( Hlsl9Kernel::KERNEL_NAME(), "VTXBUF2" )
            , vtxbuf3( Hlsl9Kernel::KERNEL_NAME(), "VTXBUF3" )
            , vtxbuf4( Hlsl9Kernel::KERNEL_NAME(), "VTXBUF4" )
            , vtxbuf5( Hlsl9Kernel::KERNEL_NAME(), "VTXBUF5" )
            , vtxbuf6( Hlsl9Kernel::KERNEL_NAME(), "VTXBUF6" )
            , vtxbuf7( Hlsl9Kernel::KERNEL_NAME(), "VTXBUF7" )
            , idxbuf( Hlsl9Kernel::KERNEL_NAME(), "IDXBUF" )
        {
        }
        //@}
    };

    //@}

    //@{

    struct QuadKernelParameterSet;
    struct QuadKernelPortBinding;
    struct QuadKernelVertex;
    struct QuadKernel : public Kernel
    {
        //@{
        static  const char             * KERNEL_NAME() { return "QUAD"; }
        virtual const char             * getName() const { return KERNEL_NAME(); }
        virtual QuadKernelParameterSet * createParameterSet() = 0;
        virtual KernelPortBinding        createPortBinding( const QuadKernelPortBinding & ) = 0;
        virtual void                     pushVertices( const QuadKernelVertex * vertices, size_t vertexcount ) = 0;
        virtual size_t                   getAvailableVertices() const = 0;
        //@}
    };

    struct QuadKernelParameterSet : public KernelParameterSet
    {
        //@{
        virtual void setBlend( bool enabled ) = 0;
        //@}
    };

    struct QuadKernelPortBinding
    {
        //@{
        KernelPort target;
        KernelPort depth;
        KernelPort texture;

        QuadKernelPortBinding()
            : target( QuadKernel::KERNEL_NAME(), "TARGET" )
            , depth( QuadKernel::KERNEL_NAME(), "DEPTH" )
            , texture( QuadKernel::KERNEL_NAME(), "TEXTURE" )
        {
        }

        //@}
    };

    struct QuadKernelVertex
    {
        //@{

        float   x, y, z;    ///< position in normalized screen space, [0,0] is left-top corner, (1,1) is right-bottom corner
        UInt8   r, g, b, a; ///< vertex color
        float   u, v;       ///< texture coordinates
        UInt32  _[2];       ///< padding to 32 bytes

        void set( float x_, float y_, float z_, UInt8 r_, UInt8 g_, UInt8 b_, UInt8 a_, float u_, float v_ )
        {
            x = x_;
            y = y_;
            z = z_;
            r = r_;
            g = g_;
            b = b_;
            a = a_;
            u = u_;
            v = v_;
        }

        //@}
    };
    GN_CASSERT( 32 == sizeof(QuadKernelVertex) );

    //@}

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_BUILDINKERNELS_H__
