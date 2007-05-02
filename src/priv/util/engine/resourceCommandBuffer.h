#ifndef __GN_ENGINE_RESOURCECOMMANDBUFFER_H__
#define __GN_ENGINE_RESOURCECOMMANDBUFFER_H__
// *****************************************************************************
//! \file    engine/resourceCommandBuffer.h
//! \brief   resource command buffer interface
//! \author  chenli@@FAREAST (2007.4.29)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// basic resource command item class.
    ///
    // TODO: use pooled memory to avoid runtime heap operation
    struct ResourceCommand : public DoubleLinkedItem<ResourceCommand>
    {
        //@{

        bool                                  noerr;                ///< true means this request has no error
        GraphicsResourceOperation             op;                   ///< requested operation.
        GraphicsResourceId                    resourceId;           ///< target resource
        int                                   targetLod;            ///< ignored by GROP_DISPOSE
        AutoRef<GraphicsResourceLoader,Mutex> loader;               ///< ignored by GROP_DISPOSE
        FenceId                               mustAfterThisFence;   ///< the request must happens after this draw fence. For lock/unlock/dispose only.
        FenceId                               submittedAtThisFence; ///< the request is submitted at this fence.
        void *                                data;                 ///< temporary data buffer used by loader
        size_t                                bytes;                ///< size of temporary data buffer.

        //@}

        //@{
        static ResourceCommand * alloc() { return new ResourceCommand; }
        static void free( ResourceCommand * p ) { delete p; }
        //@}

    private:

        ResourceCommand() : data((void*)0xbadbeef), bytes(0xbeefbad) {}
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
        /// get first command in the buffer, and remove it from the buffer.
        /// Block caller until buffer is not empty.
        /// return NULL, if received quit event.
        ///
        ResourceCommand * get();

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
    mMutex.lock();
    bool b = mCommands.empty();
    mMutex.unlock();
    return b;
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
    mMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine::ResourceCommand *
GN::engine::ResourceCommandBuffer::get()
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

    ResourceCommand * cmd = mCommands.head();
    GN_ASSERT( cmd );

    mCommands.remove( cmd );

    if( mCommands.empty() ) mBufferNotEmpty->unsignal();

    mMutex.unlock();

    return cmd;
}

// *****************************************************************************
//                           End of resourceCommandBuffer.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCECOMMANDBUFFER_H__
