//
// DO NOT EDIT. Generated by "gencmd.cmd"
//

#include "pch.h"
#include "mtgpuCmd.h"

namespace GN { namespace gfx
{

void func_SHUTDOWN( Gpu &, void *, uint32 );
void func_FENCE( Gpu &, void *, uint32 );
void func_GET_GPU_OPTIONS( Gpu &, void *, uint32 );
void func_GET_DISP_DESC( Gpu &, void *, uint32 );
void func_GET_D3D_DEVICE( Gpu &, void *, uint32 );
void func_GET_OGL_RC( Gpu &, void *, uint32 );
void func_GET_CAPS( Gpu &, void *, uint32 );
void func_CHECK_TEXTURE_FORMAT_SUPPORT( Gpu &, void *, uint32 );
void func_GET_DEFAULT_TEXTURE_FORMAT( Gpu &, void *, uint32 );
void func_CREATE_GPU_PROGRAM( Gpu &, void *, uint32 );
void func_CREATE_UNIFORM( Gpu &, void *, uint32 );
void func_CREATE_TEXTURE( Gpu &, void *, uint32 );
void func_CREATE_VTXBUF( Gpu &, void *, uint32 );
void func_CREATE_IDXBUF( Gpu &, void *, uint32 );
void func_BIND_CONTEXT( Gpu &, void *, uint32 );
void func_REBIND_CONTEXT( Gpu &, void *, uint32 );
void func_PRESENT( Gpu &, void *, uint32 );
void func_CLEAR_SCREEN( Gpu &, void *, uint32 );
void func_DRAW_INDEXED( Gpu &, void *, uint32 );
void func_DRAW( Gpu &, void *, uint32 );
void func_DRAW_INDEXED_UP( Gpu &, void *, uint32 );
void func_DRAW_UP( Gpu &, void *, uint32 );
void func_DRAW_LINES( Gpu &, void *, uint32 );
void func_GET_BACK_BUFFER_CONTENT( Gpu &, void *, uint32 );
void func_PROCESS_RENDER_WINDOW_MESSAGES( Gpu &, void *, uint32 );
void func_DEBUG_ENABLE_PARAMETER_CHECK( Gpu &, void *, uint32 );
void func_DEBUG_DUMP_NEXT_FRAME( Gpu &, void *, uint32 );
void func_DEBUG_MARK_BEGIN( Gpu &, void *, uint32 );
void func_DEBUG_MARK_END( Gpu &, void *, uint32 );
void func_DEBUG_MARK_SET( Gpu &, void *, uint32 );
void func_GPU_PROGRAM_DESTROY( Gpu &, void *, uint32 );
void func_GPU_PROGRAM_INIT( Gpu &, void *, uint32 );
void func_UNIFORM_DESTROY( Gpu &, void *, uint32 );
void func_UNIFORM_UPDATE( Gpu &, void *, uint32 );
void func_TEXTURE_DESTROY( Gpu &, void *, uint32 );
void func_TEXTURE_UPDATE_MIPMAP( Gpu &, void *, uint32 );
void func_TEXTURE_READ_MIPMAP( Gpu &, void *, uint32 );
void func_TEXTURE_BLOB_WRITE( Gpu &, void *, uint32 );
void func_TEXTURE_BLOB_READ( Gpu &, void *, uint32 );
void func_VTXBUF_DESTROY( Gpu &, void *, uint32 );
void func_VTXBUF_UPDATE( Gpu &, void *, uint32 );
void func_VTXBUF_READBACK( Gpu &, void *, uint32 );
void func_IDXBUF_DESTROY( Gpu &, void *, uint32 );
void func_IDXBUF_UPDATE( Gpu &, void *, uint32 );
void func_IDXBUF_READBACK( Gpu &, void *, uint32 );
void func_GET_SIGNALS( Gpu &, void *, uint32 );

const GpuCommandHandler g_gpuCommandHandlers[] = {
&func_SHUTDOWN,
&func_FENCE,
&func_GET_GPU_OPTIONS,
&func_GET_DISP_DESC,
&func_GET_D3D_DEVICE,
&func_GET_OGL_RC,
&func_GET_CAPS,
&func_CHECK_TEXTURE_FORMAT_SUPPORT,
&func_GET_DEFAULT_TEXTURE_FORMAT,
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
&func_DEBUG_ENABLE_PARAMETER_CHECK,
&func_DEBUG_DUMP_NEXT_FRAME,
&func_DEBUG_MARK_BEGIN,
&func_DEBUG_MARK_END,
&func_DEBUG_MARK_SET,
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
