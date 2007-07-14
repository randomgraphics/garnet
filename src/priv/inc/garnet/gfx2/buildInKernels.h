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
            : target( "CLEAR_SCREEN", "TARGET" )
            , depth ( "CLEAR_SCREEN", "DEPTH" )
        {
        }
        //@}
    };

    struct ClearScreenKernel : public Kernel
    {
        //@{
        static  const char      * KERNEL_NAME() { return "CLEAR_SCREEN"; }
        virtual const char      * getName() const { return KERNEL_NAME(); }
        //@}
    };

    //@}

    //@{

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
            : target0 ( "HLSL9", "TARGET0" )
            , target1 ( "HLSL9", "TARGET1" )
            , target2 ( "HLSL9", "TARGET2" )
            , target3 ( "HLSL9", "TARGET3" )
            , depth   ( "HLSL9", "DEPTH" )
            , texture0( "HLSL9", "TEXTURE0" )
            , texture1( "HLSL9", "TEXTURE1" )
            , texture2( "HLSL9", "TEXTURE2" )
            , texture3( "HLSL9", "TEXTURE3" )
            , texture4( "HLSL9", "TEXTURE4" )
            , texture5( "HLSL9", "TEXTURE5" )
            , texture6( "HLSL9", "TEXTURE6" )
            , texture7( "HLSL9", "TEXTURE7" )
            , vtxbuf0 ( "HLSL9", "VTXBUF0" )
            , vtxbuf1 ( "HLSL9", "VTXBUF1" )
            , vtxbuf2 ( "HLSL9", "VTXBUF2" )
            , vtxbuf3 ( "HLSL9", "VTXBUF3" )
            , vtxbuf4 ( "HLSL9", "VTXBUF4" )
            , vtxbuf5 ( "HLSL9", "VTXBUF5" )
            , vtxbuf6 ( "HLSL9", "VTXBUF6" )
            , vtxbuf7 ( "HLSL9", "VTXBUF7" )
            , idxbuf  ( "HLSL9", "IDXBUF" )
        {
        }
        //@}
    };

    struct Hlsl9Kernel : public Kernel
    {
        //@{
        static  const char      * KERNEL_NAME() { return "HLSL9"; }
        virtual const char      * getName() const { return KERNEL_NAME(); }
        //@}
    };

    //@}

    //@{

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
            : target ( "QUAD", "TARGET" )
            , depth  ( "QUAD", "DEPTH" )
            , texture( "QUAD", "TEXTURE" )
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

    struct QuadKernel : public Kernel
    {
        //@{
        static  const char             * KERNEL_NAME() { return "QUAD"; }
        virtual const char             * getName() const { return KERNEL_NAME(); }
        virtual void                     pushVertices( const QuadKernelVertex * vertices, size_t vertexcount ) = 0;
        virtual size_t                   getAvailableVertices() const = 0;
        //@}
    };

    //@}

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_BUILDINKERNELS_H__
