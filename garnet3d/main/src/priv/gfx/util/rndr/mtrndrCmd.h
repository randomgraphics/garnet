//
// DO NOT EDIT. Generated by "gencmd.cmd"
//

namespace GN { namespace gfx
{

///
/// Renderer command enumerations.
///
enum RndrCommand {
CMD_GET_RENDERER_OPTIONS,///< get renderer options
CMD_GET_DISP_DESC,///< get display descriptor
CMD_GET_D3D_DEVICE,
CMD_GET_OGL_RC,
CMD_GET_CAPS,
CMD_CHECK_TEXTURE_FORMAT_SUPPORT,
CMD_GET_DEFAULT_TEXTURE_FORMAT,
CMD_COMPILE_GPU_PROGRAM,
CMD_CREATE_GPU_PROGRAM,
CMD_CREATE_TEXTURE,
CMD_CREATE_VTXBUF,
CMD_CREATE_IDXBUF,
CMD_BIND_CONTEXT,
CMD_REBIND_CONTEXT,
CMD_PRESENT,
CMD_CLEAR_SCREEN,
CMD_DRAW_INDEXED,
CMD_DRAW,
CMD_DRAW_INDEXED_UP,
CMD_DRAW_UP,
CMD_DRAW_LINES,
CMD_PROCESS_RENDER_WINDOW_MESSAGES,
CMD_ENABLE_PARAMETER_CHECK,
CMD_DUMP_NEXT_FRAME,
CMD_TEXTURE_DESTROY,
CMD_TEXTURE_UPDATE_MIPMAP,
CMD_TEXTURE_READ_MIPMAP,
CMD_TEXTURE_BLOB_WRITE,
CMD_TEXTURE_BLOB_READ,
CMD_GPU_PROGRAM_DESTROY,
CMD_GPU_PROGRAM_INIT,
CMD_GPU_PROGRAM_SET_PARAMETER,
};

///
/// Renderer command handler type.
///
typedef void (*RndrCommandHandler)( Renderer & r, void * param, size_t size );

///
/// Renderer command handler table.
///
extern const RndrCommandHandler g_rndrCommandHandlers[];

}} // end of file
