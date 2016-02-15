#ifndef __GN_BASE_LINK_H__
#define __GN_BASE_LINK_H__
// *****************************************************************************
/// \file
/// \brief   Linked list classes and utilities
/// \author  chen@@chenli-homepc (2011.4.9)
// *****************************************************************************

namespace GN
{
    struct DoubleLink
    {
        DoubleLink * prev;
        DoubleLink * next;
        void       * context; //< A user defined pointer that is not used by the DoubeLink class.

        DoubleLink() : prev(NULL), next(NULL)
        {
        }

        // Link itself as next item of "where".
        bool linkAfter( DoubleLink * where )
        {
            // should never self-linked.
            GN_ASSERT( prev != this && next != this );

            // I must be either alone or head of another list.
            // Either way, I can't have prev.
            if( NULL != this->prev ) return false;

            // Sorry, where?
            if( NULL == where ) return false;

            // Can't really link to myself.
            if( this == where ) return false;

            if( this->next )
            {
                // If I have next, which means I'm the head of a list,
                // then "where" must be the end of another list.
                if( where->next ) return false;

                // Linking 2 lists together
                this->prev = where;
                where->next = this;
            }
            else
            {
                // I have neither next nor prev. I'm alone. I'm gonna
                // insert between "where" and "where->next".

                this->next = where->next;
                this->prev = where;

                if( where->next ) where->next->prev = this;
                where->next = this;
            }

            return true;
        }

        bool linkBefore( DoubleLink * where )
        {
            // should never self-linked.
            GN_ASSERT( prev != this && next != this );

            // I must be alone or the tail of a list. Either way,
            // I can't have next.
            if( NULL != this->next ) return false;

            // Sorry, where?
            if( NULL == where ) return false;

            // Can't really link to myself.
            if( this == where ) return false;

            if( this->prev )
            {
                // I'm tail of a list, which means "where" must be head of a list, or alone.
                if( where->prev ) return false;

                // Link 2 lists together
                this->next = where;
                where->prev = this;
            }
            else
            {
                // I'm alone. I'm goanna insert myself between "where->prev" and "where".

                this->prev = where->prev;
                this->next = where;

                if( where->prev ) where->prev->next = this;
                where->prev = this;
            }

            return true;
        }

        // Detach itself from any possible lists
        void detach()
        {
            // should never self-linked.
            GN_ASSERT( prev != this && next != this );

            DoubleLink * p = this->prev;
            DoubleLink * n = this->next;

            if( p == n )
            {
                if( p )
                {
                    GN_ASSERT( p->next == this && p->prev == this );
                    p->prev = NULL;
                    p->next = NULL;
                }
            }
            else
            {
                if( p ) p->next = n;
                if( n ) n->prev = p;
            }

            this->prev = NULL;
            this->next = NULL;
        }
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_LINK_H__
