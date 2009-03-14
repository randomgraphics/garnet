#include "../testCommon.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>

namespace GN
{
    template<class CHAR, class T, class ALLOCATOR=StlAllocator<std::pair<CHAR,T> > >
    class StringMap
    {
        struct LeafItem
        {
            const CHAR * text;
            T            value;
            LeafItem   * prev;
            LeafItem   * next;
        };

        struct Node
        {
            Node     * parent;
            Node     * left;
            Node     * right;
            LeafItem * leaf;
            UInt32     bit : 1;
            bool isLeafNode() const
            {
                return NULL != leaf;
            }
        };

        // *****************************
        // public data type
        // *****************************

    public:

        ///
        /// key and value pair type
        ///
        typedef std::pair<const CHAR *, T> PairType;
        typedef PairType       & PairTypeRef;
        typedef const PairType & ConstPairTypeRef;

        ///
        /// Iterator type
        ///
        class Iterator
        {
            LeafItem * mLeaf;

        public:

            Iterator() : mLeaf(NULL) {}

            Iterator( const Iterator & iter ) : mLeaf( iter.mLeaf ) {}

            PairType & operator*() const
            {
                GN_ASSERT( mLeaf );
                return *(PairType*)&mLeaf;
            }

            PairType * operator->() const
            {
                GN_ASSERT( mLeaf );
                return (PairType*)&mLeaf;
            }

            ///
            /// prefix plus operator
            ///
            Iterator & operator++()
            {
                GN_ASSERT( mLeaf );
                mLeaf = mLeaf->next;
                return *this;
            }

            ///
            /// suffix plus operator
            ///
            friend Iterator operator++( const Iterator & it, int )
            {
                Iterator ret( it );
                ++it;
                return ret;
            }
        };

        // *****************************
        // public methods
        // *****************************

    public:

        /// constructor
        StringMap()
        {
        }

        /// copy constructor
        StringMap( const StringMap & )
        {
        }

        /// destructor
        ~StringMap()
        {
        }

        /// begin
        Iterator begin();

        /// end
        Iterator end();

        /// insert
        void insert( const CHAR * text, const T & value, Iterator * iter, bool * insertSuccess ) { doInsert( text, value, iter, insertSuccess ); }

        /// find
        Iterator find( const CHAR * text ) const;

        /// empty
        bool empty() const;

        /// return number of items in map
        size_t size() const;

        // *****************************
        // public operators
        // *****************************

    public:

        /// assignment
        StringMap & operator=( const StringMap & );

        /// indexing operator
        Iterator operator[]( const CHAR * );

        /// less operator
        bool operator<( const StringMap & );

        // *****************************
        // private data
        // *****************************

    private:

        Node * mRoot;

        // *****************************
        // private methods
        // *****************************

    public:

        struct NodeSearchResult
        {
            Node * node;
            size_t bits;
            bool   found;
        };

        void findNode( NodeSearchResult & result, const CHAR * text )
        {
            result.node = NULL;
            result.bits = 0;
            result.found = false;

            if( 0 == text )
            {
                // not found
                return;
            }

            const size_t NUM_BITS_PER_CHAR = sizeof(CHAR)*8;

            Node * n = mRoot;
            while( 0 != *text )
            {
                for( size_t i = 0; i < NUM_BITS_PER_CHAR; ++i )
                {
                    if( NULL == n )
                    {
                        // not found
                        return;
                    }

                    UInt32 bit = (*text)>>i;

                    if( n->bit != bit )
                    {
                        // not found
                        return;
                    }

                    // we found a matching bit
                    result.node = n;
                    ++result.bits;

                    // continue with next node
                    if( 0 == bit ) n = n->left;
                    else           n = n->right;
                }

                // here means that we found a matching charactor.

                // continue with next charactor
                ++text;
            }

            // Here means that the whole text matches.

            if( result.node->isLeafNode() )
            {
                // found it!
                result.found = true;
            }
        }

        void doInsert( const CHAR * text, const T & value, Iterator * iter, bool * insertSuccess )
        {
            NodeSearchResult nsr;

            findNode( nsr, text );

            if( nsr.found )
            {
                GN_ASSERT( nsr.node && nsr.node->isLeafNode() );
                if( iter ) iter->mNode = nsr.node->leaf;
                if( insertSuccess ) *insertSuccess = false;
            }
        }
    };
}

class StringMapTest : public CxxTest::TestSuite
{
public:

    void testSmoke1()
    {
        using namespace GN;

        StringMap<char,int> m;
    }
};
