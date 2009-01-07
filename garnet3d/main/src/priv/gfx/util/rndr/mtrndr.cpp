#include "pch.h"
#include "mtrndr.h"
#include "mtshader.h"
#include "mttexture.h"
#include "mtrndrCmd.h"

#pragma warning( disable : 4100 ) // unused parameters
#pragma warning( disable : 4715 ) // no return value

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.rndr.mtrndr");

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// Local types and data
// *****************************************************************************

struct CommandHeader
{
    UInt16 cid;    ///< command ID ( 2 bytes )
    UInt16 size;   ///< command parameter size. command header is not included.
    UInt32 fence;  ///< command fence
};

// replace an auto-ref pointer without changing its reference counter.
template<typename T>
static inline void sReplaceAutoRefPtr( AutoRef<T> & ref, T * newptr )
{
    ref.detach();
    ref.attach( newptr );
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::init(
    const RendererOptions & ro,
    size_t                  ringBufferSize )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadRenderer, () );

    // initialize ring buffer
    if( !mRingBuffer.init( ringBufferSize ) ) return failure();

    // initialize cross-thread variables
    mRendererCreationStatus = 2;
    mFrontEndFence = 0;
    mBackEndFence = 0;

    // create thread
    ThreadProcedure proc = makeDelegate( this, &GN::gfx::MultiThreadRenderer::threadProc );
    mThread = createThread( proc, (void*)&ro, TP_NORMAL );
    if( NULL == mThread ) return failure();

    // wait for the renderer creation
    while( 2 == mRendererCreationStatus ) sleepCurrentThread(0);
    if( 1 != mRendererCreationStatus ) return failure();

    // initialize front end variables
    postCommand1( CMD_GET_RENDERER_OPTIONS, &mRendererOptions );
    postCommand1( CMD_GET_DISP_DESC, &mDispDesc );
    postCommand1( CMD_GET_D3D_DEVICE, &mD3DDevice );
    postCommand1( CMD_GET_OGL_RC, &mOGLRC );
    postCommand1( CMD_GET_CAPS, &mCaps );
    waitForIdle();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::quit()
{
    GN_GUARD;

    // clear context
    mRendererContext.resetToDefault();

    if( mThread )
    {
        mRingBuffer.postQuitMessage();
        mThread->waitForTermination();
        delete mThread;
        mThread = NULL;
    }

    mRingBuffer.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::waitForIdle()
{
    while( (mFrontEndFence - mBackEndFence) > 0 )
    {
        sleepCurrentThread( 0 );
    }
}

//
//
// -----------------------------------------------------------------------------
UInt8 * GN::gfx::MultiThreadRenderer::beginPostCommand( UInt32 cmd, size_t length )
{
    // align data size to command header size
    GN_ASSERT( isPowerOf2( sizeof(CommandHeader) ) );
    size_t paramsize = ( length + sizeof(CommandHeader) - 1 ) & ~(sizeof(CommandHeader) - 1);

    // check parameter size
    if( paramsize > mRingBuffer.size() )
    {
        GN_ERROR(sLogger)( "command parameter is too large to put into ring buffer." );
        return NULL;
    }

    // push command header
    CommandHeader * header = (CommandHeader *)mRingBuffer.beginProduce( sizeof(CommandHeader) );
    if( NULL == header ) return NULL;
    header->cid   = (UInt16)cmd;
    header->size  = (UInt16)paramsize;
    header->fence = ++mFrontEndFence;
    endPostCommand();

    // return pointer to parameter buffer
    return (UInt8*)mRingBuffer.beginProduce( paramsize );
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
UInt32 GN::gfx::MultiThreadRenderer::threadProc( void * param )
{
    // create the renderer instance
    GN_ASSERT( 2 == mRendererCreationStatus );
    const RendererOptions * ro = (const RendererOptions*)param;
    mRenderer = createSingleThreadRenderer( *ro );
    if( NULL == mRenderer )
    {
        mRendererCreationStatus = 0;
        return 0;
    }
    mRendererCreationStatus = 1;

    // enter command loop
    while(true)
    {
        // get command header
        const void * headerptr = (const CommandHeader*)mRingBuffer.beginConsume( sizeof(CommandHeader) );
        if( NULL == headerptr ) break; // receives quit message
        CommandHeader header;
        memcpy( &header, headerptr, sizeof(header) );
        mRingBuffer.endConsume();

        // Note: after calling of endConsume(), pointer headerptr is not valid any more. Since front end
        //       thread may overwrite its content.

        // get command parameter
        void * param = mRingBuffer.beginConsume( header.size );
        if( NULL == param ) break; // receives quit message

        // execute the command
        g_rndrCommandHandlers[header.cid]( *mRenderer, param, header.size );

        // update fence
        mBackEndFence = header.fence;

        mRingBuffer.endConsume();
    }

    // delete the renderer
    delete mRenderer;
    mRenderer = NULL;

    // success
    return 0;
}

// *****************************************************************************
// Rendering function wrappers (called by front end thread)
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::checkTextureFormatSupport( ColorFormat format, TextureUsages usages ) const
{
    MultiThreadRenderer * nonConstPtr = const_cast<GN::gfx::MultiThreadRenderer*>(this);
    bool result;
    nonConstPtr->postCommand3( CMD_CHECK_TEXTURE_FORMAT_SUPPORT, &result, format, usages );
    nonConstPtr->waitForIdle();
    return result;
}

//
//
// -----------------------------------------------------------------------------
ColorFormat GN::gfx::MultiThreadRenderer::getDefaultTextureFormat( TextureUsages usages ) const
{
    MultiThreadRenderer * nonConstPtr = const_cast<GN::gfx::MultiThreadRenderer*>(this);
    ColorFormat result;
    nonConstPtr->postCommand2( CMD_GET_DEFAULT_TEXTURE_FORMAT, &result, usages );
    nonConstPtr->waitForIdle();
    return result;
}

//
//
// -----------------------------------------------------------------------------
CompiledGpuProgram * GN::gfx::MultiThreadRenderer::compileGpuProgram( const GpuProgramDesc & desc )
{
    CompiledGpuProgram * cgp;
    postCommand2( CMD_COMPILE_GPU_PROGRAM, &cgp, &desc );
    waitForIdle();
    return cgp;
}

//
//
// -----------------------------------------------------------------------------
GpuProgram * GN::gfx::MultiThreadRenderer::createGpuProgram( const void * compiledGpuProgramBinary, size_t length )
{
    GpuProgram * gp;
    postCommand3( CMD_CREATE_GPU_PROGRAM, &gp, compiledGpuProgramBinary, length );
    waitForIdle();

    AutoRef<MultiThreadGpuProgram> mtgp( new MultiThreadGpuProgram(*this) );
    if( !mtgp->init( gp ) ) return NULL;

    return mtgp.detach();
}

//
//
// -----------------------------------------------------------------------------
Texture * GN::gfx::MultiThreadRenderer::createTexture( const TextureDesc & desc )
{
    Texture * tex;
    postCommand2( CMD_CREATE_TEXTURE, &tex, &desc );
    waitForIdle();

    AutoRef<MultiThreadTexture> mtt( new MultiThreadTexture(*this) );
    if( !mtt->init( tex ) ) return NULL;

    return mtt.detach();
}

//
//
// -----------------------------------------------------------------------------
VtxBuf * GN::gfx::MultiThreadRenderer::createVtxBuf( const VtxBufDesc & )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
IdxBuf * GN::gfx::MultiThreadRenderer::createIdxBuf( const IdxBufDesc & desc )
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::bindContext( const RendererContext & inputrc )
{
    RendererContext * rc = (RendererContext*)beginPostCommand( CMD_BIND_CONTEXT, sizeof(inputrc) );
    if( NULL == rc ) return;

    // copy renderer context to command buffer
    memcpy( rc, &inputrc, sizeof(inputrc) );

    // Replace wrapper resource pointers with real resource pointers.

    // GPU program
    MultiThreadGpuProgram * mtgp = (MultiThreadGpuProgram *)rc->gpuProgram.get();
    sReplaceAutoRefPtr( rc->gpuProgram, mtgp ? mtgp->getRealGpuProgram() : NULL );

    // textures
    for( size_t i = 0; i < GN_ARRAY_COUNT(rc->textures); ++i )
    {
        MultiThreadTexture * mtt = (MultiThreadTexture*)rc->textures[i].get();
        sReplaceAutoRefPtr( rc->textures[i], mtt ? mtt->getRealTexture() : NULL );
    }

    // vertex buffers

    // index buffer

    // color render targets
    for( size_t i = 0; i < GN_ARRAY_COUNT(rc->crts); ++i )
    {
        MultiThreadTexture * mtt = (MultiThreadTexture*)rc->crts[i].texture.get();
        sReplaceAutoRefPtr( rc->crts[i].texture, mtt ? mtt->getRealTexture() : NULL );
    }

    // depth-stencil render target
    MultiThreadTexture * ds = (MultiThreadTexture*)rc->dsrt.texture.get();
    sReplaceAutoRefPtr( rc->dsrt.texture, ds ? ds->getRealTexture() : NULL );

    // done
    endPostCommand();
    mRendererContext = inputrc;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::rebindContext()
{
    postCommand0( CMD_BIND_CONTEXT );
}

//
//
// -----------------------------------------------------------------------------
const RendererContext & GN::gfx::MultiThreadRenderer::getContext() const
{
    return mRendererContext;
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::present()
{
    postCommand0( CMD_PRESENT );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::clearScreen(
    const Vector4f & c,
    float            z,
    UInt8            s,
    BitFields        flags )
{
    postCommand4( CMD_CLEAR_SCREEN, c, z, s, flags );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::drawIndexed(
    PrimitiveType prim,
    size_t        numprim,
    size_t        startvtx,
    size_t        minvtxidx,
    size_t        numvtx,
    size_t        startidx )
{
    postCommand6( CMD_DRAW_INDEXED, prim, numprim, startvtx, minvtxidx, numvtx, startidx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::draw(
    PrimitiveType prim,
    size_t        numvtx,
    size_t        startvtx )
{
    postCommand3( CMD_DRAW, prim, numvtx, startvtx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::drawIndexedUp(
    PrimitiveType  prim,
    size_t         numidx,
    size_t         numvtx,
    const void *   vertexData,
    size_t         strideInBytes,
    const UInt16 * indexData )
{
    GN_UNIMPL();
    //postCommand6( CMD_DRAW_INDEXED_UP, prim, numprim, vertexData, strideInBytes, numvtx, indexData );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::drawUp(
    PrimitiveType prim,
    size_t        numvtx,
    const void *  vertexData,
    size_t        strideInBytes )
{
    size_t sz = strideInBytes * numvtx;
    void * vb = heapAlloc( sz );
    memcpy( vb, vertexData, sz );
    postCommand4( CMD_DRAW_UP, prim, numvtx, vb, strideInBytes );
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::MultiThreadRenderer::drawLines(
    BitFields         options,
    const void *      positions,
    size_t            stride,
    size_t            count,
    UInt32            rgba,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::processRenderWindowMessages( bool blockWhileMinimized )
{
    postCommand1( CMD_PROCESS_RENDER_WINDOW_MESSAGES, blockWhileMinimized );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::enableParameterCheck( bool enable )
{
    postCommand1( CMD_ENABLE_PARAMETER_CHECK, enable );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::dumpNextFrame( size_t startBatchIndex, size_t numBatches )
{
    postCommand2( CMD_DUMP_NEXT_FRAME, startBatchIndex, numBatches );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadRenderer::setUserData( const Guid & id, const void * data, size_t length )
{
    mRenderer->setUserData( id, data, length );
}

//
//
// -----------------------------------------------------------------------------
const void * GN::gfx::MultiThreadRenderer::getUserData( const Guid & id, size_t * length ) const
{
    return mRenderer->getUserData( id, length );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadRenderer::hasUserData( const Guid & id ) const
{
    return mRenderer->hasUserData( id );
}

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    void func_GET_RENDERER_OPTIONS( Renderer & r, void * p, size_t )
    {
        RendererOptions ** ro = (RendererOptions **)p;
        memcpy( *ro, &r.getOptions(), sizeof(**ro) );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_DISP_DESC( Renderer & r, void * p, size_t )
    {
        DispDesc ** dd = (DispDesc**)p;
        memcpy( *dd, &r.getDispDesc(), sizeof(**dd) );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_D3D_DEVICE( Renderer & r, void * p, size_t )
    {
        void *** dev = (void***)p;
        **dev = r.getD3DDevice();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_OGL_RC( Renderer & r, void * p, size_t )
    {
        void *** rc = (void***)p;
        **rc = r.getOGLRC();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_CAPS( Renderer & r, void * p, size_t )
    {
        RendererCaps ** caps = (RendererCaps**)p;
        memcpy( *caps, &r.getCaps(), sizeof(**caps) );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CHECK_TEXTURE_FORMAT_SUPPORT( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_DEFAULT_TEXTURE_FORMAT( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_COMPILE_GPU_PROGRAM( Renderer & r, void * p, size_t )
    {
        struct CompileGpuProgramParam
        {
            CompiledGpuProgram  ** cgp;
            const GpuProgramDesc * desc;
        };
        CompileGpuProgramParam * cgpp = (CompileGpuProgramParam*)p;

        *cgpp->cgp = r.compileGpuProgram( *cgpp->desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_GPU_PROGRAM( Renderer & r, void * p, size_t )
    {
        struct CreateGpuProgramParam
        {
            GpuProgram ** gp;
            const void  * bin;
            size_t        length;
        };
        CreateGpuProgramParam * cgpp = (CreateGpuProgramParam*)p;

        *cgpp->gp = r.createGpuProgram( cgpp->bin, cgpp->length );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_TEXTURE( Renderer & r, void * p, size_t )
    {
        struct CreateTextureParam
        {
            Texture          ** tex;
            const TextureDesc * desc;
        };

        CreateTextureParam * ctp = (CreateTextureParam*)p;

        *ctp->tex = r.createTexture( *ctp->desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_VTXBUF( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_IDXBUF( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_BIND_CONTEXT( Renderer & r, void * p, size_t )
    {
        RendererContext * rc = (RendererContext*)p;
        r.bindContext( *rc );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_REBIND_CONTEXT( Renderer & r, void *, size_t )
    {
        r.rebindContext();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_PRESENT( Renderer & r, void *, size_t )
    {
        r.present();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CLEAR_SCREEN( Renderer & r, void * p, size_t )
    {
#pragma pack( push, 1 )
        struct ClearParam
        {
            Vector4f  c;
            float     z;
            UInt8     s;
            BitFields flags;
        };
#pragma pack( pop )

        ClearParam * cp = (ClearParam*)p;

        r.clearScreen( cp->c, cp->z, cp->s, cp->flags );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW_INDEXED( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW_INDEXED_UP( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW_UP( Renderer & r, void * p, size_t )
    {
        struct DrawUpParam
        {
            PrimitiveType prim;
            size_t        numvtx;
            void *        vertexData;
            size_t        strideInBytes;
        };
        DrawUpParam * dup = (DrawUpParam*)p;
        r.drawUp( dup->prim, dup->numvtx, dup->vertexData, dup->strideInBytes );
        heapFree( dup->vertexData );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW_LINES( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_PROCESS_RENDER_WINDOW_MESSAGES( Renderer & r, void * p, size_t )
    {
        bool * blockWhileMinimized = (bool*)p;
        r.processRenderWindowMessages( *blockWhileMinimized );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_ENABLE_PARAMETER_CHECK( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DUMP_NEXT_FRAME( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }
}}
