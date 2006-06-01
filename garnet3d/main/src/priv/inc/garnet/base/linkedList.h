#ifndef __GN_BASE_LINKEDLIST_H__
#define __GN_BASE_LINKEDLIST_H__
// *****************************************************************************
//! \file    base/linkedList.h
//! \brief   Templates of double-linked list container
//! \author  chenlee (2006.3.24)
// *****************************************************************************

namespace GN
{
    //!
    //! Templates of double-linked list item
    //!
    template<class T>
    struct DoubleLinkedItem
    {
        T * prev; //!< pointer to previous item
        T * next; //!< pointer to next item
        void * owner; //!< pointer to the double-linked-list that this item belongs to.
    };

    //!
    //! Templates of double-linked list container
    //!
    template<class T>
    class DoubleLinkedList
    {
        typedef DoubleLinkedItem<T> ItemType;

        ItemType *mHead, *mTail;
        size_t mCount;

        // no copy
        DoubleLinkedList( const DoubleLinkedList & );
        const DoubleLinkedList & operator = ( const DoubleLinkedList & );

    public:

        //!
        //! constructor
        //!
        DoubleLinkedList() : mHead(0), mTail(0) {}

        //!
        //! Destructor
        //!
        virtual ~DoubleLinkedList() {}

        ItemType * head() const { return mHead; } //!< get head item
        ItemType * tail() const { return mTail; } //!< get tail item
        size_t     size() const { return mCount; } //!< get item count
        bool       empty() const { return 0 == mCount; }; //!< is the list empty?

        //!
        //! append new item.
        //!
        void append( ItemType * newItem ) { insertAfter( mTail, newItem ); }

        //!
        //! insert new item after specified item.
        //!
        void insertAfter( ItemType * where, ItemType * newItem )
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

        //!
        //! insert new item before specified item.
        //!
        void insertBefore( ItemType * where, ItemType * newItem )
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

        //!
        //! remove specific item
        //!
        void remove( ItemType * item )
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

    //!
    //! Templates of single-linked list item
    //!
    template<class T>
    struct SingleLinkedItem
    {
        T * next; //!< pointer to next item
        void * owner; //!< pointer to the single-linked-list that this item belongs to.
    };

    //!
    //! Templates of double-linked list container
    //!
    template<class T>
    class SingleLinkedList
    {
        typedef SingleLinkedList<T> ItemType;

        ItemType *mHead, *mTail;

        // no copy
        SingleLinkedList( const SingleLinkedList & );
        const SingleLinkedList & operator = ( const SingleLinkedList & );

    public:

        //!
        //! constructor
        //!
        SingleLinkedList() : mHead(0), mTail(0) {}

        //!
        //! Destructor
        //!
        virtual ~SingleLinkedList() {}

        ItemType * head() const { return mHead; } //!< get head item
        ItemType * tail() const { return mTail; } //!< get tail item

        //!
        //! append new item
        //!
        void append( ItemType * newItem ) { insertAfter( tail(), newItem ); }

        //!
        //! insert new item after specified item.
        //!
        void insertAfter( ItemType * where, ItemType * newItem )
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

        //!
        //! remove next item of specific one. Set to NULL, if you want to remove head item.
        //!
        void removeNextOf( ItemType * item )
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
