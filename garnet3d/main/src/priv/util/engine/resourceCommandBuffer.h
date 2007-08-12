#ifndef __GN_ENGINE2_RESOURCECOMMANDBUFFER_H__
#define __GN_ENGINE2_RESOURCECOMMANDBUFFER_H__
// *****************************************************************************
/// \file
/// \brief   resource command buffer interface
/// \author  chenli@@FAREAST (2007.4.29)
// *****************************************************************************

#include "fence.h"
#include "resourceItem.h"

namespace GN { namespace engine
{
    ///
    /// resource operations will happens in strict order as the enum itself.
    ///
    enum GraphicsResourceOperation
    {
        GROP_CREATE,     ///< create resource instance, leave resource content uninitialized. Handled by draw thread.

        GROP_LOAD,       ///< load from external/slow/remote storage. Handled by IO tread.

        GROP_DECOMPRESS, ///< prepare data for download. Handled by process thread.

        GROP_DOWNLOAD,   ///< copy data to graphics resource. Handled by draw thread.

        GROP_DISPOSE,    ///< dispose resource. Handled by draw thread.
    };

    ///
    /// basic resource command item class.
    ///
    // TODO: use pooled memory to avoid runtime heap operation
    struct ResourceCommand : public DoubleLinkedItem<ResourceCommand>
    {
        //@{

        bool                                  noerr;                      ///< true means this request has no error
        GraphicsResourceOperation             op;                         ///< requested operation.
        GraphicsResourceItem *                resource;                   ///< target resource
        AutoRef<GraphicsResourceLoader>       loader;                     ///< ignored by GROP_DELETE
        FenceId                               mustAfterThisDrawFence;     ///< the request must happens after this draw fence. For copy/dispose only.
        FenceId                               mustAfterThisResourceFence; ///< the request must happens after this resource fence. For copy/dispose only
        FenceId                               submittedAtThisFence;       ///< the request is submitted at this fence.
        DynaArray<UInt8>                      tmpbuf;                     ///< temporary data buffer used by loader

        //@}

        //@{
        static ResourceCommand * alloc() { return new ResourceCommand; }
        static void              free( ResourceCommand * p ) { delete p; }
        //@}

    private:

        ResourceCommand() {}
        ~ResourceCommand() {}
    };

    ///
    /// resource command buffer class1, used by resource threads
    ///
    /// - submit() could be called any time, any where.
    ///
    /// - remove() and removeAll() may invalid pointer returned by first() and next().
    ///
    class ResourceCommandBuffer
    {
    public:

        //@{

        ///
        /// ctor
        ///
        ResourceCommandBuffer();

        ///
        /// dtor
        ///
        ~ResourceCommandBuffer();

        ///
        /// clear the buffer
        ///
        void clear();

        ///
        /// is empty or not
        ///
        bool empty() const;

        ///
        /// submit resource requests.
        ///
        inline void submit( ResourceCommand * );

        ///
        /// begin of consuming first command in the buffer.
        /// Block caller until buffer is not empty.
        /// return NULL, if received quit event.
        ///
        /// Must be called consumeEnd() if return value is not NULL.
        ///
        ResourceCommand * consumeBegin();

        ///
        /// end of consuming, must be called pair with consumeBegin()
        ///
        void consumeEnd();

        ///
        /// post quit event. wake up any threads that are blocked by get()
        ///
        void postQuitEvent() { mQuit = true; mBufferNotEmpty->signal(); }

        //@}

    private:

        // data to handle resource commands
        DoubleLinkedList<ResourceCommand> mCommands;
        mutable Mutex                     mMutex;
        SyncEvent *                       mBufferNotEmpty;
        volatile bool                     mEmpty;
        volatile bool                     mQuit;
    };
}}

// *****************************************************************************
// inline function implementations
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline GN::engine::ResourceCommandBuffer::ResourceCommandBuffer()
    : mBufferNotEmpty( createSyncEvent( false, false ) ) // initial unsignaled, manual reset
    , mEmpty( true )
    , mQuit( false )
{
    if( 0 == mBufferNotEmpty )
    {
        GN_UNEXPECTED();
    }
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine::ResourceCommandBuffer::~ResourceCommandBuffer()
{
    clear();
    safeDelete( mBufferNotEmpty );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::ResourceCommandBuffer::clear()
{
    mMutex.lock();

    ResourceCommand * i1 = mCommands.head(), * i2;
    while( i1 )
    {
        i2 = i1->next;

        mCommands.remove( i1 );

        ResourceCommand::free( i1 );

        i1 = i2;
    }

    GN_ASSERT( mCommands.empty() );

    mBufferNotEmpty->unsignal();

    mMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::engine::ResourceCommandBuffer::empty() const
{
    return mEmpty;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::ResourceCommandBuffer::submit(
     ResourceCommand * item )
{
    GN_ASSERT( item );

    // append to resource command list
    mMutex.lock();
    mCommands.append( item );
    mBufferNotEmpty->signal();
    mEmpty = false;
    mMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine::ResourceCommand *
GN::engine::ResourceCommandBuffer::consumeBegin()
{
    mBufferNotEmpty->wait();
    if( mQuit ) return NULL;
    mMutex.lock();

    // handle multiple simutaneously consumers
    while( mCommands.empty() )
    {
        mMutex.unlock();
        if( mQuit ) return NULL;
        mBufferNotEmpty->wait();
        mMutex.lock();
    }

    GN_ASSERT( !mEmpty );

    ResourceCommand * cmd = mCommands.head();
    GN_ASSERT( cmd );

    mCommands.remove( cmd );

    if( mCommands.empty() )
    {
        mBufferNotEmpty->unsignal();
    }

    mMutex.unlock();

    return cmd;
}

//
//
// -----------------------------------------------------------------------------
inline void
GN::engine::ResourceCommandBuffer::consumeEnd()
{
    mMutex.lock();
    if( mCommands.empty() )
    {
        mEmpty = true;
    }
    else
    {
        GN_ASSERT( !mEmpty );
    }
    mMutex.unlock();
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_RESOURCECOMMANDBUFFER_H__
