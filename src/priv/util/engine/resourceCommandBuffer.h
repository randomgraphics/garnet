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
        inline void submit( const GraphicsResourceCommand & );

        ///
        /// get first command in the buffer, and remove it from the buffer.
        /// return false, if there's no such command in buffer.
        ///
        bool get( GraphicsResourceCommand & cmd );

        ///
        /// post quit event. wake up any threads that are blocked by get()
        ///
        void postQuitEvent() { mQuit = true; mBufferNotEmpty->signal(); }

        //@}

    private:

        // TODO: use pooled memory to avoid runtime heap operation
        struct ResourceCommandItem : public DoubleLinkedItem<ResourceCommandItem>
        {
            GraphicsResourceCommand command;
        };

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

        delete i1;

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
     const GraphicsResourceCommand & command )
{
    // allocate new resource command item
    AutoObjPtr<ResourceCommandItem> item( new ResourceCommandItem );
    item->command = command;

    // append to resource command list
    mMutex.lock();
    mCommands.append( item.detach() );
    mBufferNotEmpty->signal();
    mMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline bool
GN::engine::ResourceCommandBuffer::get( GraphicsResourceCommand & cmd )
{
    mBufferNotEmpty->wait();
    if( mQuit ) return false;
    mMutex.lock();

    // handle multiple simutaneously consumers
    while( mCommands.empty() )
    {
        mMutex.unlock();
        if( mQuit ) return false;
        mBufferNotEmpty->wait();
        mMutex.lock();
    }

    ResourceCommandItem * item = mCommands.head();
    GN_ASSERT( item );

    mCommands.remove( item );

    cmd = item->command;

    // TODO: use memory pool
    delete item;

    if( mCommands.empty() ) mBufferNotEmpty->unsignal();

    mMutex.unlock();

    return true;
}

// *****************************************************************************
//                           End of resourceCommandBuffer.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCECOMMANDBUFFER_H__
