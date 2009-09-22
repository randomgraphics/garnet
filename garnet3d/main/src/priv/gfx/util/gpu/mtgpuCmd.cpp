//
// DO NOT EDIT. Generated by "gencmd.cmd"
//

#include "pch.h"
#include "mtgpuCmd.h"

namespace GN { namespace gfx
{

void func_GET_GPU_OPTIONS( Gpu &, void *, size_t );
void func_GET_DISP_DESC( Gpu &, void *, size_t );
void func_GET_D3D_DEVICE( Gpu &, void *, size_t );
void func_GET_OGL_RC( Gpu &, void *, size_t );
void func_GET_CAPS( Gpu &, void *, size_t );
void func_CHECK_TEXTURE_FORMAT_SUPPORT( Gpu &, void *, size_t );
void func_GET_DEFAULT_TEXTURE_FORMAT( Gpu &, void *, size_t );
void func_COMPILE_GPU_PROGRAM( Gpu &, void *, size_t );
void func_CREATE_GPU_PROGRAM( Gpu &, void *, size_t );
void func_CREATE_UNIFORM( Gpu &, void *, size_t );
void func_CREATE_TEXTURE( Gpu &, void *, size_t );
void func_CREATE_VTXBUF( Gpu &, void *, size_t );
void func_CREATE_IDXBUF( Gpu &, void *, size_t );
void func_BIND_CONTEXT( Gpu &, void *, size_t );
void func_REBIND_CONTEXT( Gpu &, void *, size_t );
void func_PRESENT( Gpu &, void *, size_t );
void func_CLEAR_SCREEN( Gpu &, void *, size_t );
void func_DRAW_INDEXED( Gpu &, void *, size_t );
void func_DRAW( Gpu &, void *, size_t );
void func_DRAW_INDEXED_UP( Gpu &, void *, size_t );
void func_DRAW_UP( Gpu &, void *, size_t );
void func_DRAW_LINES( Gpu &, void *, size_t );
void func_GET_BACK_BUFFER_CONTENT( Gpu &, void *, size_t );
void func_PROCESS_RENDER_WINDOW_MESSAGES( Gpu &, void *, size_t );
void func_ENABLE_PARAMETER_CHECK( Gpu &, void *, size_t );
void func_DUMP_NEXT_FRAME( Gpu &, void *, size_t );
void func_GPU_PROGRAM_DESTROY( Gpu &, void *, size_t );
void func_GPU_PROGRAM_INIT( Gpu &, void *, size_t );
void func_UNIFORM_DESTROY( Gpu &, void *, size_t );
void func_UNIFORM_UPDATE( Gpu &, void *, size_t );
void func_TEXTURE_DESTROY( Gpu &, void *, size_t );
void func_TEXTURE_UPDATE_MIPMAP( Gpu &, void *, size_t );
void func_TEXTURE_READ_MIPMAP( Gpu &, void *, size_t );
void func_TEXTURE_BLOB_WRITE( Gpu &, void *, size_t );
void func_TEXTURE_BLOB_READ( Gpu &, void *, size_t );
void func_VTXBUF_DESTROY( Gpu &, void *, size_t );
void func_VTXBUF_UPDATE( Gpu &, void *, size_t );
void func_VTXBUF_READBACK( Gpu &, void *, size_t );
void func_IDXBUF_DESTROY( Gpu &, void *, size_t );
void func_IDXBUF_UPDATE( Gpu &, void *, size_t );
void func_IDXBUF_READBACK( Gpu &, void *, size_t );
void func_GET_SIGNALS( Gpu &, void *, size_t );

const GpuCommandHandler g_gpuCommandHandlers[] = {
&func_GET_GPU_OPTIONS,
&func_GET_DISP_DESC,
&func_GET_D3D_DEVICE,
&func_GET_OGL_RC,
&func_GET_CAPS,
&func_CHECK_TEXTURE_FORMAT_SUPPORT,
&func_GET_DEFAULT_TEXTURE_FORMAT,
&func_COMPILE_GPU_PROGRAM,
&func_CREATE_GPU_PROGRAM,
&func_CREATE_UNIFORM,
&func_CREATE_TEXTURE,
&func_CREATE_VTXBUF,
&func_CREATE_IDXBUF,
&func_BIND_CONTEXT,
&func_REBIND_CONTEXT,
&func_PRESENT,
&func_CLEAR_SCREEN,
&func_DRAW_INDEXED,
&func_DRAW,
&func_DRAW_INDEXED_UP,
&func_DRAW_UP,
&func_DRAW_LINES,
&func_GET_BACK_BUFFER_CONTENT,
&func_PROCESS_RENDER_WINDOW_MESSAGES,
&func_ENABLE_PARAMETER_CHECK,
&func_DUMP_NEXT_FRAME,
&func_GPU_PROGRAM_DESTROY,
&func_GPU_PROGRAM_INIT,
&func_UNIFORM_DESTROY,
&func_UNIFORM_UPDATE,
&func_TEXTURE_DESTROY,
&func_TEXTURE_UPDATE_MIPMAP,
&func_TEXTURE_READ_MIPMAP,
&func_TEXTURE_BLOB_WRITE,
&func_TEXTURE_BLOB_READ,
&func_VTXBUF_DESTROY,
&func_VTXBUF_UPDATE,
&func_VTXBUF_READBACK,
&func_IDXBUF_DESTROY,
&func_IDXBUF_UPDATE,
&func_IDXBUF_READBACK,
&func_GET_SIGNALS,
};

}} // end of file