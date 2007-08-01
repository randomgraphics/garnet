#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9GraphicsSystem");

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// build-in kernels
// *****************************************************************************

// *****************************************************************************
// kernel registry
// *****************************************************************************

struct KernelRegistry
{
    NamedHandleManager<KernelReflection,size_t> reflections;
    SpinLoop                                    mutex;

    KernelRegistry()
    {
        SurfaceLayoutTemplate rendertarget;
        GN_TODO( "setup render target port surface layout" );

        SurfaceLayoutTemplate depth;
        GN_TODO( "setup depth port surface layout" );

        // texture layout
        SurfaceLayoutTemplate texture;
        texture.flags.u32 = 0;
        texture.flags.attributes = 1; // accept one attribute named "TEXEL"
        texture.attributes = 1;
        texture.requiredAttributes.resize( 1 );
        texture.requiredAttributes[0].semantic.set( "TEXEL" );
        texture.requiredAttributes[0].offset = 0;

        // vertex buffer layout
        SurfaceLayoutTemplate vtxbuf;
        vtxbuf.flags.u32 = 0;
        vtxbuf.flags.dim = 1; // 1D
        vtxbuf.dim = SURFACE_DIMENSION_1D;
        vtxbuf.flags.levels = 1; // no mipmap
        vtxbuf.levels = 1;
        vtxbuf.flags.faces = 1; // single faces
        vtxbuf.faces = 1;

        // index buffer layout
        SurfaceLayoutTemplate idxbuf;
        idxbuf.flags.u32 = 0;
        idxbuf.flags.dim = 1; // 1D
        idxbuf.dim = SURFACE_DIMENSION_1D;
        idxbuf.flags.levels = 1; // no mipmap
        idxbuf.levels = 1;
        idxbuf.flags.faces = 1; // single faces
        idxbuf.faces = 1;
        idxbuf.flags.attributes = 1; // accept 16-bits or 32-bits indices
        idxbuf.attributes = 1;
        idxbuf.requiredAttributes.resize( 1 );
        idxbuf.requiredAttributes[0].semantic.set( "INDEX" );
        idxbuf.requiredAttributes[0].offset = 0;
        idxbuf.requiredAttributes[0].allowedFormats.insert( gfx::FMT_R_16_UINT );
        idxbuf.requiredAttributes[0].allowedFormats.insert( gfx::FMT_R_32_UINT );

        // standard resource kernel
        KernelReflection STANDARD_RESOURCES( "STANDARD_RESOURCES" );
        STANDARD_RESOURCES.addNewPort( "VERTEX_BUFFER", vtxbuf, true, false );
        STANDARD_RESOURCES.addNewPort( "INDEX_BUFFER" , idxbuf, true, false );
        STANDARD_RESOURCES.addNewPort( "TEXTURE" , texture, true, false );
        STANDARD_RESOURCES.addNewPort( "RENDER_TARGET", rendertarget, true, true );
        STANDARD_RESOURCES.addNewPort( "DEPTH_BUFFER" , depth, true, true );
        registerKernelReflection( STANDARD_RESOURCES );

        // clear screen kernel
        KernelReflection CLEAR_SCREEN( "CLEAR_SCREEN" );
        CLEAR_SCREEN.addNewParameter( "CLEAR_COLOR", KERNEL_PARAMETER_TYPE_BOOL, 1 );
        CLEAR_SCREEN.addNewParameter( "CLEAR_DEPTH", KERNEL_PARAMETER_TYPE_BOOL, 1 );
        CLEAR_SCREEN.addNewParameter( "CLEAR_STENCIL", KERNEL_PARAMETER_TYPE_BOOL, 1 );
        CLEAR_SCREEN.addNewParameter( "COLOR", KERNEL_PARAMETER_TYPE_FLOAT, 4 );
        CLEAR_SCREEN.addNewParameter( "DEPTH", KERNEL_PARAMETER_TYPE_FLOAT, 1 );
        CLEAR_SCREEN.addNewParameter( "STENCIL", KERNEL_PARAMETER_TYPE_INT, 1 );
        CLEAR_SCREEN.addNewPort( "TARGET0", rendertarget, true, true );
        CLEAR_SCREEN.addNewPort( "DEPTH", depth, true, true );
        registerKernelReflection( CLEAR_SCREEN );

        // quad kernel
        SurfaceElementFormat streamFormat;
        streamFormat.attribs[0].set( "POSITION",  0, FMT_FLOAT3 );
        streamFormat.attribs[1].set( "COLOR"   , 12, FMT_RGBA32 );
        streamFormat.attribs[2].set( "TEXCOORD", 16, FMT_FLOAT2 );
        streamFormat.count = 3;
        streamFormat.stride = 32;

        KernelReflection QUAD( "QUAD" );
        QUAD.addNewStream( "QUADS", streamFormat );
        QUAD.addNewParameter( "TRANSPARENT", KERNEL_PARAMETER_TYPE_BOOL, 1 );
        QUAD.addNewPort( "TARGET0", rendertarget, true, true );
        QUAD.addNewPort( "DEPTH", depth, true, true );
        QUAD.addNewPort( "TEXTURE0", texture, true, false );
        registerKernelReflection( QUAD );

        // hlsl9 kernel
        KernelReflection HLSL9( "HLSL9" );
        HLSL9.addNewParameter( "FX", KERNEL_PARAMETER_TYPE_STRING, 1 );
        HLSL9.addNewParameter( "VSCF", KERNEL_PARAMETER_TYPE_FLOAT, 256*4 );
        HLSL9.addNewParameter( "PSCF", KERNEL_PARAMETER_TYPE_FLOAT, 256*4 );
        HLSL9.addNewParameter( "PRIM_TYPE", KERNEL_PARAMETER_TYPE_INT, 1 );
        HLSL9.addNewParameter( "PRIM_COUNT", KERNEL_PARAMETER_TYPE_INT, 1 );
        HLSL9.addNewParameter( "BASE_VERTEX", KERNEL_PARAMETER_TYPE_INT, 1 );
        HLSL9.addNewParameter( "BASE_INDEX", KERNEL_PARAMETER_TYPE_INT, 1 );
        HLSL9.addNewParameter( "VERTEX_COUNT", KERNEL_PARAMETER_TYPE_INT, 1 );
        HLSL9.addNewPort( "TARGET0", rendertarget, true, true );
        HLSL9.addNewPort( "TARGET1", rendertarget, true, true );
        HLSL9.addNewPort( "TARGET2", rendertarget, true, true );
        HLSL9.addNewPort( "TARGET3", rendertarget, true, true );
        HLSL9.addNewPort( "DEPTH", depth, true, true );
        HLSL9.addNewPort( "TEXTURE0", texture, true, false );
        HLSL9.addNewPort( "TEXTURE1", texture, true, false );
        HLSL9.addNewPort( "TEXTURE2", texture, true, false );
        HLSL9.addNewPort( "TEXTURE3", texture, true, false );
        HLSL9.addNewPort( "TEXTURE4", texture, true, false );
        HLSL9.addNewPort( "TEXTURE5", texture, true, false );
        HLSL9.addNewPort( "TEXTURE6", texture, true, false );
        HLSL9.addNewPort( "TEXTURE7", texture, true, false );
        HLSL9.addNewPort( "VTXBUF0", vtxbuf, true, false );
        HLSL9.addNewPort( "VTXBUF1", vtxbuf, true, false );
        HLSL9.addNewPort( "VTXBUF2", vtxbuf, true, false );
        HLSL9.addNewPort( "VTXBUF3", vtxbuf, true, false );
        HLSL9.addNewPort( "VTXBUF4", vtxbuf, true, false );
        HLSL9.addNewPort( "VTXBUF5", vtxbuf, true, false );
        HLSL9.addNewPort( "VTXBUF6", vtxbuf, true, false );
        HLSL9.addNewPort( "VTXBUF7", vtxbuf, true, false );
        HLSL9.addNewPort( "IDXBUF", idxbuf, true, false );
        registerKernelReflection( HLSL9 );
    }
};

//
//
// -----------------------------------------------------------------------------
static inline KernelRegistry & sGetRegistry()
{
    static KernelRegistry sInstance;

    return sInstance;
}

// *****************************************************************************
// KernelReflection
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::KernelReflection::validate() const
{
    GN_UNIMPL_WARNING();
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::KernelReflection::addNewStream( const StrA & name, const SurfaceElementFormat & format )
{
    StreamSourceReflection r;

    r.kernel = this->name;
    r.name   = name;
    r.index  = streams.size();
    r.format = format;

    streams.add( name, r );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::KernelReflection::addNewParameter( const StrA & name, KernelParameterType type, size_t count )
{
    KernelParameterReflection r;

    r.kernel = this->name;
    r.name   = name;
    r.index  = parameters.size();
    r.type   = type;
    r.count  = count;

    parameters.add( name, r );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::KernelReflection::addNewPort(
    const StrA                  & name,
    const SurfaceLayoutTemplate & layout,
    bool                          input,
    bool                          output )
{
    KernelPortReflection r;

    r.kernel = this->name;
    r.name   = name;
    r.index  = ports.size();
    r.layout = layout;
    r.input  = input;
    r.output = output;

    ports.add( name, r );
}


// *****************************************************************************
// global functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::registerKernelReflection( const KernelReflection & refl )
{
    if( !refl.validate() ) return;

    KernelRegistry & r = sGetRegistry();

    ScopeMutex<SpinLoop> lock( r.mutex );

    sGetRegistry().reflections.add( refl.name, refl );
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::KernelReflection &
GN::gfx::getKernelReflection( const StrA & kernelName, bool * dummy )
{
    KernelRegistry & r = sGetRegistry();

    ScopeMutex<SpinLoop> lock( r.mutex );

    size_t h = r.reflections.name2handle( kernelName );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "invalid kernel name: %s.", kernelName.cptr() );

        static KernelReflection sDummy( "DUMMY" );

        GN_ASSERT( sDummy.validate() );

        if( dummy ) *dummy = true;

        return sDummy;
    }
    else
    {
        if( dummy ) *dummy = false;
        return r.reflections[h];
    }
}
