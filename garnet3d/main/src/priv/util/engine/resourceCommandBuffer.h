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
    /// resource command buffer class.
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
        /// get first command in the buffer that meets fence requirement, and remove it from the buffer.
        /// return false, if there's no such command in buffer.
        ///
        bool get( GraphicsResourceCommand & cmd, FenceId currentDrawFence );

        //@}

    private:

        // TODO: use pooled memory to avoid runtime heap operation
        struct ResourceCommandItem : public DoubleLinkedItem<ResourceCommandItem>
        {
            GraphicsResourceCommand command;
        };

        // data to handle resource commands
        DoubleLinkedList<ResourceCommandItem> mResourceCommands;
        mutable Mutex                         mResourceMutex;

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
inline void GN::engine::ResourceCommandBuffer::clear()
{
    mResourceMutex.lock();

    ResourceCommandItem * i1 = mResourceCommands.head(), * i2;
    while( i1 )
    {
        i2 = i1->next;

        mResourceCommands.remove( i1 );

        delete i1;

        i1 = i2;
    }

    GN_ASSERT( mResourceCommands.empty() );

    mResourceMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline size_t GN::engine::ResourceCommandBuffer::size() const
{
    mResourceMutex.lock();
    size_t n = mResourceCommands.size();
    mResourceMutex.unlock();
    return n;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::engine::ResourceCommandBuffer::empty() const
{
    mResourceMutex.lock();
    ResourceCommandItem * item = mResourceCommands.head();
    mResourceMutex.unlock();
    return 0 != item;
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
    mResourceMutex.lock();
    mResourceCommands.append( item.detach() );
    mResourceMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline bool
GN::engine::ResourceCommandBuffer::get( GraphicsResourceCommand & cmd, FenceId currentDrawFence )
{
    mResourceMutex.lock();

    ResourceCommandItem * item = mResourceCommands.head();

    while( item && ( GROP_DISPOSE == item->command.op || item->command.waitForDrawFence > currentDrawFence ) ) item = item->next;

    if( item )
    {
        cmd = item->command;

        mResourceCommands.remove( item );

        // TODO: use memory pool
        delete item;

        mResourceMutex.unlock();

        return true;
    }
    else
    {
        mResourceMutex.unlock();
        return false;
    }
}

// *****************************************************************************
//                           End of resourceCommandBuffer.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCECOMMANDBUFFER_H__
