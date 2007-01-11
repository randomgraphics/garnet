#ifndef __GN_BASE_LINKEDLIST_H__
#define __GN_BASE_LINKEDLIST_H__
// *****************************************************************************
/// \file    base/linkedList.h
/// \brief   Templates of double-linked list container
/// \author  chenlee (2006.3.24)
// *****************************************************************************

namespace GN
{
    ///
    /// Templates of double-linked list item
    ///
    template<class T>
    struct DoubleLinkedItem
    {
        T * prev; ///< pointer to previous item
        T * next; ///< pointer to next item
        void * owner; ///< pointer to the double-linked-list that this item belongs to.
    };

    ///
    /// Templates of double-linked list container
    ///
    template<class T>
    class DoubleLinkedList
    {
    public:

        typedef DoubleLinkedItem<T> ItemType; ///< type of linked item

        /// \name ctor and dtor
        //@{
        DoubleLinkedList() : mHead(0), mTail(0) {}
        virtual ~DoubleLinkedList() {}
        //@}

        /// \name list operations
        //@{
        void       append( ItemType * newItem ) { insertAfter( mTail, newItem ); }
        bool       empty() const { return 0 == mCount; };
        ItemType * head() const { return mHead; }
        void       insertAfter( ItemType * where, ItemType * newItem ) { doInsertAfter( where, newItem ); }
        void       insertBefore( ItemType * where, ItemType * newItem ) { doInsertBefore( where, newItem ); }
        void       remove( ItemType * item ) { doRemove( item ); }
        size_t     size() const { return mCount; }
        ItemType * tail() const { return mTail; }
        //@}

    private:

        ItemType *mHead, *mTail;
        size_t mCount;

    private:

        // no copy
        DoubleLinkedList( const DoubleLinkedList & );
        const DoubleLinkedList & operator = ( const DoubleLinkedList & );

        void doInsertAfter( ItemType * where, ItemType * newItem )
        {
            GN_ASSERT( newItem && where != newItem );

            ItemType * prev = where;
            ItemType * next = where ? where->next : NULL;

            newItem->prev = prev;
            newItem->next = next;
            newItem->owner = this;
            if( prev ) prev->next = newItem;
            if( next ) next->prev = newItem;

            if( mTail == where ) mTail = newItem;
        }

        void doInsertBefore( ItemType * where, ItemType * newItem )
        {
            GN_ASSERT( newItem && where != newItem );

            ItemType * prev = where ? where->prev : NULL;
            ItemType * next = where;

            newItem->prev = prev;
            newItem->next = next;
            newItem->owner = this;
            if( prev ) prev->next = newItem;
            if( next ) next->prev = newItem;

            if( mHead == where ) mHead = newItem;
        }

        void doRemove( ItemType * item )
        {
            GN_ASSERT( item && this == item->owner );
            ItemType * prev = item->prev;
            ItemType * next = item->next;
            if( prev ) prev->next = next;
            if( next ) next->prev = prev;
            item->owner = NULL;
            if( item == mHead ) mHead = item->next;
            if( item == mTail ) mTail = item->prev;
        }
    };

    ///
    /// Templates of single-linked list item
    ///
    template<class T>
    struct SingleLinkedItem
    {
        T * next; ///< pointer to next item
        void * owner; ///< pointer to the single-linked-list that this item belongs to.
    };

    ///
    /// Templates of double-linked list container
    ///
    template<class T>
    class SingleLinkedList
    {
    public:

        typedef SingleLinkedList<T> ItemType; ///< type of linked item

        /// \name ctor and dtor
        //@{
        SingleLinkedList() : mHead(0), mTail(0) {}
        virtual ~SingleLinkedList() {}
        //@}

        /// \name list operations
        //@{
        void       append( ItemType * newItem ) { insertAfter( tail(), newItem ); }
        ItemType * head() const { return mHead; }
        void       insertAfter( ItemType * where, ItemType * newItem ) { doInsertAfter( where, newItem ); }
        void       removeNextOf( ItemType * item ) { doRemoveNextOf( item ); }
        ItemType * tail() const { return mTail; }
        //@}

    private:

        ItemType *mHead, *mTail;

    private:

        // no copy
        SingleLinkedList( const SingleLinkedList & );
        const SingleLinkedList & operator = ( const SingleLinkedList & );

        void doInsertAfter( ItemType * where, ItemType * newItem )
        {
            GN_ASSERT( newItem && where != newItem );
            if( where )
            {
                newItem->next = where->next;
                where->next = newItem;
            }
            else
            {
                newItem->next = NULL;
            }
            newItem->owner = this;
            if( mTail == where ) mTail = newItem;
        }

        void doRemoveNextOf( ItemType * item )
        {
            if( item )
            {
                GN_ASSERT( this == item->owner );
                ItemType * next = item->next;
                if( next )
                {
                    item->next = next->next;
                    if( next == mTail ) mTail = item;
                }
            }
            else if( mHead )
            {
                // remove head item
                mHead = mHead->next;
            }
        }

    };
}

// *****************************************************************************
//                           End of linkedList.h
// *****************************************************************************
#endif // __GN_BASE_LINKEDLIST_H__
