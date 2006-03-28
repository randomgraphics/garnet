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
    };

    //!
    //! Templates of double-linked list container
    //!
    template<class T>
    class DoubleLinkedList : public NoCopy
    {
        typedef DoubleLinkedItem<T> ItemType;

        ItemType *mHead, *mTail;

    public:

        //!
        //! constructor
        //!
        DoubleLinkedList() : mHead(0), mTail(0) {}

        //!
        //! Destructor
        //!
        virtual ~DoubleLinkedList() {}
    };
}

// *****************************************************************************
//                           End of linkedList.h
// *****************************************************************************
#endif // __GN_BASE_LINKEDLIST_H__
