#include "pch.h"
#include "mtgpu.h"
#include "mtshader.h"
#include "mttexture.h"
#include "mtvtxbuf.h"
#include "mtidxbuf.h"
#include "mtgpuCmd.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.gpu.mtgpu");

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// Local types and data
// *****************************************************************************

struct CommandHeader
{
    uint16 cid;    ///< command ID ( 2 bytes )
    uint16 size;   ///< command parameter size. command header is not included.
    uint32 fence;  ///< command fence
};

struct DrawLineParams
{
    uint32 options;
    void    * positions;
    uint32    stride;
    uint32    numpoints;
    uint32    rgba;
    Matrix44f model;
    Matrix44f view;
    Matrix44f proj;
};

template<typename T>
static inline void sReplaceAutoRefPtr( AutoRef<T> & ref, T * newptr )
{
    ref.set( newptr );
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadGpu::init(
    const GpuOptions            & ro,
    const MultiThreadGpuOptions & mo,
    CreateSingleThreadFunc        func,
    void *                        context )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    // check parameters
    if( NULL == func )
    {
        GN_ERROR(sLogger)( "Null creator!" );
        return failure();
    }
    mCreator = func;
    mCreationContext = context;

    // initialize ring buffer
    if( !mCommandBuffer.init( mo.commandBufferSize ) ) return failure();

    // initialize sync objects
    mGpuCreationStatus = 2; // 2 is "not done yet";
    if( !mWaitForIdleFence.create( SyncEvent::UNSIGNALED, SyncEvent::AUTO_RESET ) ||
        !mPresentFence.create( SyncEvent::SIGNALED, SyncEvent::AUTO_RESET ) )
    {
        return failure();
    }

    // create thread
    Thread::Procedure proc = makeDelegate( this, &GN::gfx::MultiThreadGpu::threadProc );
    mThread = Thread::sCreate( proc, (void*)&ro );
    if( 0 == mThread ) return failure();

    // wait for the GPU creation
    while( 2 == mGpuCreationStatus ) Thread::sSleepCurrentThread(0);
    if( 1 != mGpuCreationStatus ) return failure();

    // initialize front end variables
    mMultithreadOptions = mo;
    mCommandBuffer.postCommand1( CMD_GET_GPU_OPTIONS, &mGpuOptions );
    mCommandBuffer.postCommand1( CMD_GET_DISP_DESC, &mDispDesc );
    mCommandBuffer.postCommand1( CMD_GET_D3D_DEVICE, &mD3DDevice );
    mCommandBuffer.postCommand1( CMD_GET_OGL_RC, &mOGLRC );
    mCommandBuffer.postCommand1( CMD_GET_CAPS, &mCaps );
    mCommandBuffer.postCommand1( CMD_GET_SIGNALS, &mSignals );
    waitForIdle();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::quit()
{
    GN_GUARD;

    // clear context
    mGpuContext.clear();

    if( mThread )
    {
        mCommandBuffer.postCommand0( CMD_SHUTDOWN );
        Thread::sJoin( mThread );
        mThread = 0;
    }

    mCommandBuffer.quit();

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
void GN::gfx::MultiThreadGpu::waitForIdle()
{
    mCommandBuffer.postCommand0( CMD_FENCE, &mWaitForIdleFence );
    mWaitForIdleFence.wait();
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::threadProc( void * param )
{
    // create the GPU instance
    GN_ASSERT( 2 == mGpuCreationStatus );
    const GpuOptions * ro = (const GpuOptions*)param;
    mGpu = mCreator( *ro, mCreationContext );
    if( NULL == mGpu )
    {
        mGpuCreationStatus = 0;
        return;
    }
    mGpuCreationStatus = 1;

    // command loop
    for(;;)
    {
        CommandBuffer::Token token;
        if( CommandBuffer::OPERATION_SUCCEEDED != mCommandBuffer.beginConsume( &token ) )
        {
            GN_UNEXPECTED_EX( "Command consumption failed unexpectedly." );
            break;
        }

        g_gpuCommandHandlers[token.commandID]( *mGpu, token.pParameterBuffer, token.parameterSize );

        mCommandBuffer.endConsume();

        if( CMD_SHUTDOWN == token.commandID ) break;
    }

    // delete the GPU
    deleteGpu( mGpu );
    mGpu = NULL;
}

// *****************************************************************************
// Rendering function wrappers (called by front end thread)
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadGpu::checkTextureFormatSupport( ColorFormat format, TextureUsage usages ) const
{
    MultiThreadGpu * nonConstPtr = const_cast<GN::gfx::MultiThreadGpu*>(this);
    bool result;
    nonConstPtr->mCommandBuffer.postCommand3( CMD_CHECK_TEXTURE_FORMAT_SUPPORT, &result, format, usages );
    nonConstPtr->waitForIdle();
    return result;
}

//
//
// -----------------------------------------------------------------------------
GpuProgram * GN::gfx::MultiThreadGpu::createGpuProgram( const GpuProgramDesc & desc )
{
    GpuProgram * gp = NULL;
    mCommandBuffer.postCommand3( CMD_CREATE_GPU_PROGRAM, &gp, &desc );
    waitForIdle();
    if( NULL == gp ) return NULL;

    AutoRef<MultiThreadGpuProgram> mtgp = referenceTo( new MultiThreadGpuProgram(*this) );
    if( !mtgp->init( gp ) ) return NULL;

    return mtgp.detach();
}

//
//
// -----------------------------------------------------------------------------
Uniform * GN::gfx::MultiThreadGpu::createUniform( uint32 size )
{
    Uniform * uni = NULL;
    mCommandBuffer.postCommand2( CMD_CREATE_UNIFORM, &uni, size );
    waitForIdle();
    if( NULL == uni ) return NULL;

    AutoRef<MultiThreadUniform> mu = referenceTo( new MultiThreadUniform(*this) );
    if( !mu->init( uni ) ) return NULL;

    return mu.detach();
}

//
//
// -----------------------------------------------------------------------------
Texture * GN::gfx::MultiThreadGpu::createTexture( const TextureDesc & desc )
{
    Texture * tex = NULL;
    mCommandBuffer.postCommand2( CMD_CREATE_TEXTURE, &tex, &desc );
    waitForIdle();
    if( NULL == tex ) return NULL;

    AutoRef<MultiThreadTexture> mtt = referenceTo( new MultiThreadTexture(*this) );
    if( !mtt->init( tex ) ) return NULL;

    return mtt.detach();
}

//
//
// -----------------------------------------------------------------------------
VtxBuf * GN::gfx::MultiThreadGpu::createVtxBuf( const VtxBufDesc & desc )
{
    VtxBuf * vb = NULL;
    mCommandBuffer.postCommand2( CMD_CREATE_VTXBUF, &vb, &desc );
    waitForIdle();
    if( NULL == vb ) return NULL;

    AutoRef<MultiThreadVtxBuf> mtvb = referenceTo( new MultiThreadVtxBuf(*this) );
    if( !mtvb->init( vb ) ) return NULL;

    return mtvb.detach();
}

//
//
// -----------------------------------------------------------------------------
IdxBuf * GN::gfx::MultiThreadGpu::createIdxBuf( const IdxBufDesc & desc )
{
    IdxBuf * ib = NULL;
    mCommandBuffer.postCommand2( CMD_CREATE_IDXBUF, &ib, &desc );
    waitForIdle();
    if( NULL == ib ) return NULL;

    AutoRef<MultiThreadIdxBuf> mtib = referenceTo( new MultiThreadIdxBuf(*this) );
    if( !mtib->init( ib ) ) return NULL;

    return mtib.detach();
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::bindContext( const GpuContext & inputrc )
{
    CommandBuffer::Token token;
    if( CommandBuffer::OPERATION_SUCCEEDED != mCommandBuffer.beginProduce( CMD_BIND_CONTEXT, sizeof(inputrc), &token ) )
    {
        return;
    }

    // copy GPU context to command buffer by inplace new operator
    GpuContext * rc = (GpuContext*)token.pParameterBuffer;
    new (rc) GpuContext(inputrc);

    // Replace wrapper resource pointers with real resource pointers.

    // GPU program
    MultiThreadGpuProgram * mtgp = (MultiThreadGpuProgram *)rc->gpuProgram.rawptr();
    sReplaceAutoRefPtr( rc->gpuProgram, mtgp ? mtgp->getRealGpuProgram() : NULL );

    // uniforms
    for( uint32 i = 0; i < rc->uniforms.size(); ++i )
    {
        MultiThreadUniform * mtu = (MultiThreadUniform*)rc->uniforms[i].rawptr();
        sReplaceAutoRefPtr( rc->uniforms[i], mtu ? mtu->getRealUniform() : NULL );
    }

    // textures
    for( uint32 i = 0; i < GN_ARRAY_COUNT(rc->textures); ++i )
    {
        MultiThreadTexture * mtt = (MultiThreadTexture*)rc->textures[i].texture.rawptr();
        sReplaceAutoRefPtr( rc->textures[i].texture, mtt ? mtt->getRealTexture() : NULL );
    }

    // vertex buffers
    for( uint32 i = 0; i < GN_ARRAY_COUNT(rc->vtxbufs); ++i )
    {
        MultiThreadVtxBuf * mtvb = (MultiThreadVtxBuf *)rc->vtxbufs[i].vtxbuf.rawptr();
        sReplaceAutoRefPtr( rc->vtxbufs[i].vtxbuf, mtvb ? mtvb->getRealVtxBuf() : NULL );
    }

    // index buffer
    MultiThreadIdxBuf * mtib = (MultiThreadIdxBuf *)rc->idxbuf.rawptr();
    sReplaceAutoRefPtr( rc->idxbuf, mtib ? mtib->getRealIdxBuf() : NULL );

    // color render targets
    for( uint32 i = 0; i < rc->colortargets.size(); ++i )
    {
        MultiThreadTexture * mtt = (MultiThreadTexture*)rc->colortargets[i].texture.rawptr();
        sReplaceAutoRefPtr( rc->colortargets[i].texture, mtt ? mtt->getRealTexture() : NULL );
    }

    // depth-stencil render target
    MultiThreadTexture * ds = (MultiThreadTexture*)rc->depthstencil.texture.rawptr();
    sReplaceAutoRefPtr( rc->depthstencil.texture, ds ? ds->getRealTexture() : NULL );

    // done
    mCommandBuffer.endProduce();
    mGpuContext = inputrc;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::rebindContext()
{
    mCommandBuffer.postCommand0( CMD_BIND_CONTEXT );
}

//
//
// -----------------------------------------------------------------------------
const GpuContext & GN::gfx::MultiThreadGpu::getContext() const
{
    return mGpuContext;
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::present()
{
    SyncEvent * fence;
    if( mMultithreadOptions.cacheOneFrameAtMost )
    {
        // wait for the previous present to complete, to ensure that
        // we cache only one full frame in the command buffer.
        mPresentFence.wait();
        fence = &mPresentFence;
    }
    else
    {
        fence = NULL;
    }

    mCommandBuffer.postCommand0( CMD_PRESENT, fence );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::clearScreen(
    const Vector4f & c,
    float            z,
    uint8            s,
    uint32        flags )
{
    mCommandBuffer.postCommand4( CMD_CLEAR_SCREEN, c, z, s, flags );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::drawIndexed(
    PrimitiveType prim,
    uint32        numidx,
    uint32        basevtx,
    uint32        startvtx,
    uint32        numvtx,
    uint32        startidx )
{
    mCommandBuffer.postCommand6( CMD_DRAW_INDEXED, prim, numidx, basevtx, startvtx, numvtx, startidx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::draw(
    PrimitiveType prim,
    uint32        numvtx,
    uint32        startvtx )
{
    mCommandBuffer.postCommand3( CMD_DRAW, prim, numvtx, startvtx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::drawIndexedUp(
    PrimitiveType  prim,
    uint32         numidx,
    uint32         numvtx,
    const void   * vertexData,
    uint32         strideInBytes,
    const uint16 * indexData )
{
    uint32 vbsize = numvtx * strideInBytes;
    uint32 ibsize = numidx * 2;

    void * tmpvb = HeapMemory::alloc( vbsize );
    if( NULL == tmpvb )
    {
        GN_ERROR(sLogger)( "Fail to allocate temporary vertex buffer." );
        return;
    }
    memcpy( tmpvb, vertexData, vbsize );

    void * tmpib = HeapMemory::alloc( ibsize );
    if( NULL == tmpib )
    {
        GN_ERROR(sLogger)( "Fail to allocate temporary index buffer." );
        HeapMemory::dealloc( tmpvb );
        return;
    }
    memcpy( tmpib, indexData, ibsize );

    mCommandBuffer.postCommand6( CMD_DRAW_INDEXED_UP, prim, numidx, numvtx, tmpvb, strideInBytes, tmpib );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::drawUp(
    PrimitiveType prim,
    uint32        numvtx,
    const void *  vertexData,
    uint32        strideInBytes )
{
    uint32 sz = strideInBytes * numvtx;
    void * vb = HeapMemory::alloc( sz );
    if( NULL == vb )
    {
        GN_ERROR(sLogger)( "fail to allocate temporary vertex buffer." );
        return;
    }
    memcpy( vb, vertexData, sz );
    mCommandBuffer.postCommand4( CMD_DRAW_UP, prim, numvtx, vb, strideInBytes );
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::MultiThreadGpu::drawLines(
    uint32         options,
    const void *      positions,
    uint32            stride,
    uint32            numpoints,
    uint32            rgba,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    uint32 length = stride * numpoints;

    void * tmpbuf = HeapMemory::alloc( length );
    if( NULL == tmpbuf )
    {
        GN_ERROR(sLogger)( "fail to allocate temporary buffer." );
        return;
    }
    memcpy( tmpbuf, positions, length );

    CommandBuffer::Token token;
    if( CommandBuffer::OPERATION_SUCCEEDED == mCommandBuffer.beginProduce( CMD_DRAW_LINES, sizeof(DrawLineParams), &token ) )
    {
        DrawLineParams * dlp = (DrawLineParams*)token.pParameterBuffer;

        dlp->options   = options;
        dlp->positions = tmpbuf;
        dlp->stride    = stride;
        dlp->numpoints = numpoints;
        dlp->rgba      = rgba;
        dlp->model     = model;
        dlp->view      = view;
        dlp->proj      = proj;

        mCommandBuffer.endProduce();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::getBackBufferContent( BackBufferContent & result )
{
    mCommandBuffer.postCommand1( CMD_GET_BACK_BUFFER_CONTENT, &result );
    waitForIdle();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::processRenderWindowMessages( bool blockWhileMinimized )
{
    mCommandBuffer.postCommand1( CMD_PROCESS_RENDER_WINDOW_MESSAGES, blockWhileMinimized );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::setUserData( const Guid & id, const void * data, uint32 length )
{
    mGpu->setUserData( id, data, length );
}

//
//
// -----------------------------------------------------------------------------
const void * GN::gfx::MultiThreadGpu::getUserData( const Guid & id, uint32 * length ) const
{
    return mGpu->getUserData( id, length );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadGpu::hasUserData( const Guid & id ) const
{
    return mGpu->hasUserData( id );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::debugEnableParameterCheck( bool enable )
{
    mCommandBuffer.postCommand1( CMD_DEBUG_ENABLE_PARAMETER_CHECK, enable );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::debugDumpNextFrame( uint32 startBatchIndex, uint32 numBatches )
{
    mCommandBuffer.postCommand2( CMD_DEBUG_DUMP_NEXT_FRAME, startBatchIndex, numBatches );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::debugMarkBegin( const char * markerName )
{
    uint32 len = (uint32)str::length( markerName ) + 1;
    if( len < 1 || len > 1024 ) return;

    CommandBuffer::Token token;
    CommandBuffer::OperationResult hr = mCommandBuffer.beginProduce( CMD_DEBUG_MARK_BEGIN, (uint16)len, &token );
    if( CommandBuffer::OPERATION_SUCCEEDED == hr )
    {
        GN_ASSERT( token.commandID == CMD_DEBUG_MARK_BEGIN );
        GN_ASSERT( token.parameterSize >= len );
        memcpy( token.pParameterBuffer, markerName, len );
        mCommandBuffer.endProduce();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::debugMarkEnd()
{
    mCommandBuffer.postCommand0( CMD_DEBUG_MARK_END );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadGpu::debugMarkSet( const char * markerName )
{
    uint32 len = (uint32)str::length( markerName ) + 1;
    if( len < 1 || len > 1024 ) return;

    CommandBuffer::Token token;
    CommandBuffer::OperationResult hr = mCommandBuffer.beginProduce( CMD_DEBUG_MARK_SET, (uint16)len, &token );
    if( CommandBuffer::OPERATION_SUCCEEDED == hr )
    {
        GN_ASSERT( token.commandID == CMD_DEBUG_MARK_SET );
        GN_ASSERT( token.parameterSize >= len );
        memcpy( token.pParameterBuffer, markerName, len );
        mCommandBuffer.endProduce();
    }
}

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    void func_SHUTDOWN( Gpu &, void *, uint32 )
    {
        // do nothing
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_FENCE( Gpu &, void *, uint32 )
    {
        // do nothing
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_GPU_OPTIONS( Gpu & r, void * p, uint32 )
    {
        GpuOptions ** ro = (GpuOptions **)p;
        memcpy( *ro, &r.getOptions(), sizeof(**ro) );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_DISP_DESC( Gpu & r, void * p, uint32 )
    {
        DispDesc ** dd = (DispDesc**)p;
        memcpy( *dd, &r.getDispDesc(), sizeof(**dd) );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_D3D_DEVICE( Gpu & r, void * p, uint32 )
    {
        void *** dev = (void***)p;
        **dev = r.getD3DDevice();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_OGL_RC( Gpu & r, void * p, uint32 )
    {
        void *** rc = (void***)p;
        **rc = r.getOGLRC();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_CAPS( Gpu & r, void * p, uint32 )
    {
        GpuCaps ** caps = (GpuCaps**)p;
        memcpy( *caps, &r.caps(), sizeof(**caps) );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CHECK_TEXTURE_FORMAT_SUPPORT( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct CheckTextureFormatSupportParam
        {
            bool        * result;
            ColorFormat   format;
            TextureUsage usages;
        };
#pragma pack( pop )
        CheckTextureFormatSupportParam * param = (CheckTextureFormatSupportParam*)p;

        *param->result = r.checkTextureFormatSupport( param->format, param->usages );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_GPU_PROGRAM( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct CreateGpuProgramParam
        {
            GpuProgram ** gp;
            const GpuProgramDesc * desc;
        };
#pragma pack( pop )
        CreateGpuProgramParam * cgpp = (CreateGpuProgramParam*)p;

        *cgpp->gp = r.createGpuProgram( *cgpp->desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_UNIFORM( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct CreateUniformParam
        {
            Uniform ** result;
            uint32     length;
        };
#pragma pack( pop )

        CreateUniformParam * cup = (CreateUniformParam*)p;

        *cup->result = r.createUniform( cup->length );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_TEXTURE( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct CreateTextureParam
        {
            Texture          ** result;
            const TextureDesc * desc;
        };
#pragma pack( pop )

        CreateTextureParam * ctp = (CreateTextureParam*)p;

        *ctp->result = r.createTexture( *ctp->desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_VTXBUF( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct CreateVtxBufParam
        {
            VtxBuf          ** result;
            const VtxBufDesc * desc;
        };
#pragma pack( pop )

        CreateVtxBufParam * param = (CreateVtxBufParam*)p;

        *param->result = r.createVtxBuf( *param->desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CREATE_IDXBUF( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct CreateIdxBufParam
        {
            IdxBuf          ** result;
            const IdxBufDesc * desc;
        };
#pragma pack( pop )

        CreateIdxBufParam * param = (CreateIdxBufParam*)p;

        *param->result = r.createIdxBuf( *param->desc );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_BIND_CONTEXT( Gpu & r, void * p, uint32 )
    {
        GpuContext * rc = (GpuContext*)p;

        r.bindContext( *rc );

        // destruct the GPU context (release all ref counted resources)
        rc->GpuContext::~GpuContext();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_REBIND_CONTEXT( Gpu & r, void *, uint32 )
    {
        r.rebindContext();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_PRESENT( Gpu & r, void *, uint32 )
    {
        r.present();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_CLEAR_SCREEN( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct ClearParam
        {
            Vector4f  c;
            float     z;
            uint8     s;
            uint32 flags;
        };
#pragma pack( pop )

        ClearParam * cp = (ClearParam*)p;

        r.clearScreen( cp->c, cp->z, cp->s, cp->flags );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW_INDEXED( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct DrawIndexedParam
        {
            PrimitiveType prim;
            uint32        numidx;
            uint32        basevtx;
            uint32        startvtx;
            uint32        numvtx;
            uint32        startidx;
        };
#pragma pack( pop )

        DrawIndexedParam * dip = (DrawIndexedParam*)p;

        r.drawIndexed( dip->prim, dip->numidx, dip->basevtx, dip->startvtx, dip->numvtx, dip->startidx );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct DrawParam
        {
            PrimitiveType prim;
            uint32        numvtx;
            uint32        startvtx;
        };
#pragma pack( pop )

        DrawParam * dp = (DrawParam*)p;

        r.draw( dp->prim, dp->numvtx, dp->startvtx );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW_INDEXED_UP( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct DrawIndexedUpParam
        {
            PrimitiveType  prim;
            uint32         numidx;
            uint32         numvtx;
            void         * vertexData;
            uint32         strideInBytes;
            uint16       * indexData;
        };
#pragma pack( pop )

        DrawIndexedUpParam * diup = (DrawIndexedUpParam*)p;

        r.drawIndexedUp( diup->prim, diup->numidx, diup->numvtx, diup->vertexData, diup->strideInBytes, diup->indexData );

        HeapMemory::dealloc( diup->vertexData );
        HeapMemory::dealloc( diup->indexData );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW_UP( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct DrawUpParam
        {
            PrimitiveType prim;
            uint32        numvtx;
            void *        vertexData;
            uint32        strideInBytes;
        };
#pragma pack( pop )
        DrawUpParam * dup = (DrawUpParam*)p;
        r.drawUp( dup->prim, dup->numvtx, dup->vertexData, dup->strideInBytes );
        HeapMemory::dealloc( dup->vertexData );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DRAW_LINES( Gpu & r, void * p, uint32 )
    {
        DrawLineParams * dlp = (DrawLineParams*)p;

        r.drawLines(
            dlp->options,
            dlp->positions,
            dlp->stride,
            dlp->numpoints,
            dlp->rgba,
            dlp->model,
            dlp->view,
            dlp->proj );

        HeapMemory::dealloc( dlp->positions );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_SIGNALS( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct GetSignalsParam
        {
            GpuSignals ** ppSignals;
        };
#pragma pack( pop )
        GetSignalsParam * param = (GetSignalsParam *)p;
        *(param->ppSignals) = &r.getSignals();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_GET_BACK_BUFFER_CONTENT( Gpu & r, void * p, uint32 )
    {
        Gpu::BackBufferContent ** param = (Gpu::BackBufferContent **)p;
        r.getBackBufferContent( **param );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_PROCESS_RENDER_WINDOW_MESSAGES( Gpu & r, void * p, uint32 )
    {
        bool * blockWhileMinimized = (bool*)p;
        r.processRenderWindowMessages( *blockWhileMinimized );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DEBUG_ENABLE_PARAMETER_CHECK( Gpu & r, void * p, uint32 )
    {
        bool * enable = (bool*)p;
        r.debugEnableParameterCheck( *enable );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DEBUG_DUMP_NEXT_FRAME( Gpu & r, void * p, uint32 )
    {
#pragma pack( push, 1 )
        struct DumpNextFrameParam
        {
            uint32 startBatchIndex;
            uint32 numBatches;
        };
#pragma pack( pop )

        DumpNextFrameParam * param = (DumpNextFrameParam*)p;
        r.debugDumpNextFrame( param->startBatchIndex, param->numBatches );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DEBUG_MARK_BEGIN( Gpu & r, void * p, uint32 )
    {
        r.debugMarkBegin( (const char*)p );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DEBUG_MARK_END( Gpu & r, void *, uint32 )
    {
        r.debugMarkEnd();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_DEBUG_MARK_SET( Gpu & r, void * p, uint32 )
    {
        r.debugMarkSet( (const char*)p );
    }
}}
