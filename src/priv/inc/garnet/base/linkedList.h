#ifndef __GN_BASE_LINKEDLIST_H__
#define __GN_BASE_LINKEDLIST_H__
// *****************************************************************************
/// \file
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
#if GN_BUILD_ENABLE_ASSERT
        void * owner; ///< pointer to the double-linked-list that this item belongs to.
#endif
    };

    ///
    /// Templates of double-linked list container
    ///
    template<class T>
    class DoubleLinkedList
    {
    public:

        typedef T ItemType; ///< type of linked item

        /// \name ctor and dtor
        //@{
        DoubleLinkedList() : mHead(0), mTail(0) {}
        virtual ~DoubleLinkedList() {}
        //@}

        /// \name list operations
        //@{
        void       Append( ItemType * newItem ) { InsertAfter( mTail, newItem ); }
        bool       Empty() const { return 0 == mHead; };
        ItemType * GetHead() const { return mHead; }
        void       InsertAfter( ItemType * where, ItemType * newItem ) { DoInsertAfter( where, newItem ); }
        void       InsertBefore( ItemType * where, ItemType * newItem ) { DoInsertBefore( where, newItem ); }
        void       Remove( ItemType * item ) { DoRemove( item ); }
        ItemType * GetTail() const { return mTail; }
        //@}

    private:

        ItemType *mHead, *mTail;

    private:

        // no copy
        DoubleLinkedList( const DoubleLinkedList & );
        const DoubleLinkedList & operator = ( const DoubleLinkedList & );

        void DoInsertAfter( ItemType * where, ItemType * newItem )
        {
            GN_ASSERT( newItem && where != newItem );

            if( NULL == mHead )
            {
                // this is the first item
                GN_ASSERT( NULL == where );
                mHead = newItem;
                mTail = newItem;
                newItem->prev = NULL;
                newItem->next = NULL;
            }
            else
            {
                GN_ASSERT( where );
                GN_ASSERT( where->owner == this );

                ItemType * prev = where;
                ItemType * next = where->next;

                newItem->prev = prev;
                newItem->next = next;

                prev->next = newItem;
                if( next )
                {
                    GN_ASSERT( mTail != where );
                    next->prev = newItem;
                }
                else
                {
                    GN_ASSERT( mTail == where );
                    mTail = newItem;
                }
            }

#if GN_BUILD_ENABLE_ASSERT
            newItem->owner = this;
#endif
        }

        void DoInsertBefore( ItemType * where, ItemType * newItem )
        {
            GN_ASSERT( newItem && where != newItem );

            if( NULL == mHead )
            {
                // this is the first item
                GN_ASSERT( 0 == where );
                mHead = newItem;
                mTail = newItem;
                newItem->prev = NULL;
                newItem->next = NULL;
            }
            else
            {
                GN_ASSERT( where );
                GN_ASSERT( where->owner == this );

                ItemType * prev = where->prev;
                ItemType * next = where;

                newItem->prev = prev;
                newItem->next = next;

                if( prev )
                {
                    GN_ASSERT( mHead != where );
                    prev->next = newItem;
                }
                else
                {
                    GN_ASSERT( mHead == where );
                    mHead = newItem;
                }

                next->prev = newItem;
            }

#if GN_BUILD_ENABLE_ASSERT
            newItem->owner = this;
#endif
        }

        void DoRemove( ItemType * item )
        {
            GN_ASSERT( item && this == item->owner );

            ItemType * prev = item->prev;
            ItemType * next = item->next;
            if( prev ) prev->next = next;
            if( next ) next->prev = prev;
#if GN_BUILD_ENABLE_ASSERT
            item->owner = NULL;
#endif
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
#if GN_BUILD_ENABLE_ASSERT
        void * owner; ///< pointer to the single-linked-list that this item belongs to.
#endif
    };

    ///
    /// Templates of double-linked list container
    ///
    template<class T>
    class SingleLinkedList
    {
    public:

        typedef T ItemType; ///< type of linked item

        /// \name ctor and dtor
        //@{
        SingleLinkedList() : mHead(0), mTail(0) {}
        virtual ~SingleLinkedList() {}
        //@}

        /// \name list operations
        //@{
        void       Append( ItemType * newItem ) { InsertAfter( GetTail(), newItem ); }
        ItemType * GetHead() const { return mHead; }
        void       InsertAfter( ItemType * where, ItemType * newItem ) { DoInsertAfter( where, newItem ); }
        void       RemoveNextOf( ItemType * item ) { DoRemoveNextOf( item ); }
        ItemType * GetTail() const { return mTail; }
        //@}

    private:

        ItemType *mHead, *mTail;

    private:

        // no copy
        SingleLinkedList( const SingleLinkedList & );
        const SingleLinkedList & operator = ( const SingleLinkedList & );

        void DoInsertAfter( ItemType * where, ItemType * newItem )
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
#if GN_BUILD_ENABLE_ASSERT
            newItem->owner = this;
#endif
            if( mTail == where ) mTail = newItem;
        }

        void DoRemoveNextOf( ItemType * item )
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
                // remove GetHead item
                mHead = mHead->next;
            }
        }

    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_LINKEDLIST_H__
