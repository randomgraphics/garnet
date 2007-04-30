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
    /// basic resource command item class. all resource commands must sub-classed from this.
    ///
    // TODO: use pooled memory to avoid runtime heap operation
    struct ResourceCommandItem : public DoubleLinkedItem<ResourceCommandItem>
    {
        //@{
        GraphicsResourceCommand command;
        //@}

        //@{
        static ResourceCommandItem * alloc() { return new ResourceCommandItem; }
        static void free( ResourceCommandItem * p ) { delete p; }
        //@}

    private:

        ResourceCommandItem()  {}
        ~ResourceCommandItem() {}
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
        /// return command count in the buffer
        ///
        size_t size() const;

        ///
        /// is empty or not
        ///
        bool empty() const;

        ///
        /// submit resource requests.
        ///
        inline void submit( ResourceCommandItem * );

        ///
        /// get first command in the buffer, and remove it from the buffer.
        /// Block caller until buffer is not empty.
        /// return NULL, if received quit event.
        ///
        ResourceCommandItem * get();

        ///
        /// post quit event. wake up any threads that are blocked by get()
        ///
        void postQuitEvent() { mQuit = true; mBufferNotEmpty->signal(); }

        //@}

    private:

        // data to handle resource commands
        DoubleLinkedList<ResourceCommandItem> mCommands;
        mutable Mutex                         mMutex;
        SyncEvent *                           mBufferNotEmpty;
        volatile bool                         mQuit;

    private:

        bool validcmd( GraphicsResourceCommand * cmd ) const
        {
            if( !cmd ) return false;
            return true;
        }

        static inline ResourceCommandItem * cmd2item( GraphicsResourceCommand * cmd )
        {
            return (ResourceCommandItem*)( ((UInt8*)cmd) - GN_FIELD_OFFSET( ResourceCommandItem, command ) );
        }

        static inline GraphicsResourceCommand * item2cmd( ResourceCommandItem * item )
        {
            return &item->command;
        }
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

    ResourceCommandItem * i1 = mCommands.head(), * i2;
    while( i1 )
    {
        i2 = i1->next;

        mCommands.remove( i1 );

        ResourceCommandItem::free( i1 );

        i1 = i2;
    }

    GN_ASSERT( mCommands.empty() );

    mBufferNotEmpty->unsignal();

    mMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::engine::ResourceCommandBuffer::size() const
{
    mMutex.lock();
    size_t n = mCommands.size();
    mMutex.unlock();
    return n;
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
     ResourceCommandItem * item )
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
inline GN::engine::ResourceCommandItem *
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

    ResourceCommandItem * item = mCommands.head();
    GN_ASSERT( item );

    mCommands.remove( item );

    if( mCommands.empty() ) mBufferNotEmpty->unsignal();

    mMutex.unlock();

    return item;
}

// *****************************************************************************
//                           End of resourceCommandBuffer.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCECOMMANDBUFFER_H__
