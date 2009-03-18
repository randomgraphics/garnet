#include "../testCommon.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>

namespace GN
{
    //
    // Implmement algorithm described in paper "Fast Algorithms for Sorting and Searching Strings"
    //
    //      http://www.cs.princeton.edu/~rs/strings/)
    //
    // Here are some reference implementations:
    //
    //      http://www.codeproject.com/KB/recipes/tst.aspx
    //      http://meshula.net/wordpress/?p=183
    //
    template<class CHAR, class T>
    class StringMap
    {
        struct Leaf
        {
            const CHAR * key;
            T            value;

            Leaf( const CHAR * k, const T & v )
                : key(k), value(v)
            {
            }
        };

        struct Node
        {
            CHAR   splitchar;
            Node * lokid;
            Node * eqkid;
            Node * hikid;
            Leaf * leaf;  // valid only when splitchar == 0 (leaf node)
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
            Node * mNode;

            friend typename class StringMap<CHAR,T>;

        public:

            Iterator() : mNode(NULL) {}

            Iterator( const Iterator & iter ) : mNode( iter.mNode ) {}

            PairType & operator*() const
            {
                GN_ASSERT( mNode && mNode->leaf );
                return *(PairType*)mNode->leaf;
            }

            PairType * operator->() const
            {
                GN_ASSERT( mNode && mNode->leaf );
                return (PairType*)mNode->leaf;
            }

            ///
            /// prefix plus operator
            ///
            Iterator & operator++()
            {
                GN_ASSERT( mNode && mNode->leaf );
                GN_UNIMPL();
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

        /// default constructor
        StringMap() : mRoot(NULL), mCount(0)
        {
        }

        /// copy constructor
        StringMap( const StringMap & sm ) : mRoot(NULL), mCount(0)
        {
            doCopy( sm );
        }

        /// destructor
        ~StringMap()
        {
            clear();
        }

        /// begin
        Iterator begin();

        /// clear whole map
        void clear() { return doClear(); }

        /// empty
        bool empty() const { return 0 == mCount; }

        /// end
        Iterator end();

        /// erase by key
        void erase( const CHAR * text );

        /// find
        Iterator find( const CHAR * text ) const { return doFind( text ); }

        /// insert
        void insert( const CHAR * text, const T & value, Iterator * iter, bool * insertSuccess ) { doInsert( text, value, iter, insertSuccess ); }

        /// return number of items in map
        size_t size() const { return mCount; }

        // *****************************
        // public operators
        // *****************************

    public:

        /// assignment
        StringMap & operator=( const StringMap & rhs ) { doClone( rhs ); return *this; }

        /// indexing operator
        Iterator operator[]( const CHAR * text ) { Iterator iter; doInsert( text, T(), &iter, NULL ); return iter; }

        /// less operator (NOT IMPLEMENTED)
        bool operator<( const StringMap & ) { GN_UNIMPL(); return false; }

        // *****************************
        // private data
        // *****************************

    private:

        Node * mRoot;
        size_t mCount; // number of items in map
        FixSizedRawMemoryPool<sizeof(Node)> mNodePool;
        ObjectPool<Leaf>                    mLeafPool;

        // *****************************
        // private methods
        // *****************************

    private:

        Node * allocNode()
        {
            return (Node*)mNodePool.alloc();
        }

        Leaf * allocLeaf( const CHAR * text, const T & value )
        {
            Leaf * p = mLeafPool.allocUnconstructed();
            if( p )
            {
                // call in-place new to construct the leaf
                new (p) ( text, value );
            }
            return p;
        }

        /// clear the whole map container
        void doClear()
        {
            mRoot = NULL;
            mCount = 0;
            mNodePool.freeAll();
            mLeafPool.freeAll();
        }

        /// make itself a clone of another map
        void doClone( const StringMap & anotherMap )
        {
            // shortcut for cloning itself.
            if( this == &anotherMap ) return;

            GN_UNIMPL();
        }

        Iterator doFind( const CHAR * text ) const
        {
            Iterator iter;

            Node * p = mRoot;
            while( p )
            {
                int d = *text - p->splitchar;

                if( 0 == d )
                {
                    if( 0 == *text )
                    {
                        // found!
                        iter->mNode = p;
                        return iter;
                    }
                    else
                    {
                        p = p->eqkid;
                    }

                    ++text;
                }
                else if( d < 0 )
                {
                    p = p->lokid;
                }
                else
                {
                    p = p->hikid;
                }
            }

            // not found
            return iter;
        }

        void doInsert( const CHAR * text, const T & value, Iterator * iter, bool * insertSuccess )
        {
            // store input text pointer
            const char * inputText = text;

            // search in existing nodes
            Node * p = mRoot;
            while( p )
            {
                int d = *text - p->splitchar;

                if( 0 == d )
                {
                    if( 0 == *text )
                    {
                        // the text does exist. insertion failed.
                        if( *iter ) iter->mNode = p;
                        if( insertSuccess ) *insertSuccess = false;
                        return;
                    }
                    else
                    {
                        p = p->eqkid;
                    }

                    ++text;
                }
                else if( d < 0 )
                {
                    p = p->lokid;
                }
                else
                {
                    p = p->hikid;
                }
            }

            // No existing text found. Now insert new nodes for
            // each unmatched characters in input text.
            for(;;)
            {
                // create new node
                p = allocNode();
                p->splitchar = *text;
                p->lokid = p->hikid = p->eqkid = 0;

                if( 0 == *text )
                {
                    // we reach the end of the node, insertion done.
                    p->leaf = allocLeaf( inputText, value );
                    ++mCount;
                    if( *iter ) iter->mNode = p;
                    if( insertSuccess ) *insertSuccess = true;
                    return;
                }
                else
                {
                    p->leaf = 0;
                }

                // continue with next character
                ++text;
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
