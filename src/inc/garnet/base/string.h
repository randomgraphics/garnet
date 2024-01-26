#ifndef __GN_BASE_STRING_H__
#define __GN_BASE_STRING_H__
// *****************************************************************************
/// \file
/// \brief   custom string implementation
/// \author  chenlee (2005.4.17)
// *****************************************************************************

#include <stdarg.h>
#include <ostream>
#include <string.h>
#include <string>
namespace GN {

///
/// Fixed sized string that has no runtime memory allocation.
///
template<size_t N, typename CHAR>
class StackStr {
    typedef CHAR CharType;

    size_t mCount;      ///< How many charecters in the string, not including null end.
    CHAR   mBuf[N + 1]; ///< Pre-allocated string buffer

    static size_t sValidateLength(size_t len) { return len < N ? len : N; }

public:
    /// \name ctor and dtor
    //@{
    StackStr(): mCount(0) { mBuf[0] = 0; }
    StackStr(const CHAR * s, size_t l = 0): mCount(l) {
        if (0 == s) {
            mCount  = 0;
            mBuf[0] = 0;
        } else {
            if (0 == l) l = str::length(s);
            memcpy(mBuf, s, sValidateLength(l) * sizeof(CHAR));
        }
    }
    StackStr(const StackStr & s): mCount(s.mCount) { memcpy(mBuf, s.mBuf, sizeof(CHAR) * s.mCount); }
    StackStr(const std::basic_string<CHAR> & s) { memcpy(mBuf, s.data(), sizeof(CHAR) * sValidateLength(s.size())); }
    //@}
};

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
// TODO: sorted leaf list
//
template<class CHAR, class T>
class StringMap {
    // *****************************
    // public types
    // *****************************

public:
    /// public key-value pair type.
    struct KeyValuePair {
        const CHAR * const key; // Note: key is always an const.
        T                  value;

    protected:
        // default constructor
        KeyValuePair(const char * k, const T & v): key(k), value(v) {}
    };

    // *****************************
    // public methods
    // *****************************

public:
    /// default constructor
    StringMap(bool caseInsensitive = false): mRoot(NULL), mCount(0), mCaseInsensitive(caseInsensitive) {}

    /// copy constructor
    StringMap(const StringMap & sm): mRoot(NULL), mCount(0) { doClone(sm); }

    /// destructor
    ~StringMap() { clear(); }

    /// get first element in the map
    /// \note elements are _NOT_ sorted yet.
    const KeyValuePair * first() const { return doFirst(); }

    /// get first element in the map
    /// \note elements are _NOT_ sorted yet.
    KeyValuePair * first() { return doFirst(); }

    /// clear whole map
    void clear() { doClear(); }

    /// empty
    bool empty() const { return 0 == mCount; }

    /// Get next item
    /// \note elements are _NOT_ sorted yet.
    const KeyValuePair * next(const KeyValuePair * p) const { return doNext(p); }

    /// Get next item
    /// \note elements are _NOT_ sorted yet.
    KeyValuePair * next(const KeyValuePair * p) { return doNext(p); }

    /// erase by key
    void remove(const CHAR * text) { doRemove(text); }

    /// find
    const T * find(const CHAR * text) const { return doFind(text); }

    /// find
    T * find(const CHAR * text) { return doFind(text); }

    /// find
    KeyValuePair * findPair(const CHAR * text) { return doFindPair(text); }

    /// find
    const KeyValuePair * findPair(const CHAR * text) const { return doFindPair(text); }

    /// insert. Return the inserted key value pair or NULL.
    KeyValuePair * insert(const CHAR * text, const T & value) {
        bool           inserted;
        KeyValuePair * p = doFindOrInsert(text, value, inserted);
        return inserted ? p : NULL;
    }

    /// return number of items in map
    size_t size() const { return mCount; }

    // *****************************
    // public operators
    // *****************************

public:
    /// assignment
    StringMap & operator=(const StringMap & rhs) {
        doClone(rhs);
        return *this;
    }

    /// indexing operator
    T & operator[](const CHAR * text) {
        bool           inserted;
        KeyValuePair * p = doFindOrInsert(text, T(), inserted);
        GN_ASSERT(p);
        return p->value;
    }

    /// indexing operator
    const T & operator[](const CHAR * text) const {
        const KeyValuePair * p = doFindPair(text);
        GN_ASSERT(p);
        return p->value;
    }

    // *****************************
    // private types
    // *****************************

private:
    struct Leaf : public KeyValuePair {
        StringMap & owner;

        // links to other leaves
        DoubleLink link;

        Leaf(StringMap & owner_, const CHAR * text, size_t textlen, const T & v)
            : KeyValuePair((const char *) HeapMemory::alloc(textlen + 1), v), owner(owner_) {
            if (NULL != KeyValuePair::key) { memcpy((char *) KeyValuePair::key, text, textlen + 1); }

            link.context = this;
        }

        ~Leaf() { HeapMemory::dealloc((void *) KeyValuePair::key); }
    };

    struct Node {
        CHAR   splitchar; // 0 means a leaf node
        Node * parent;
        Node * lower;
        Node * equal;
        Node * higher;
        Leaf * leaf; // valid only when splitchar == 0 (leaf node)
    };

    // *****************************
    // private data
    // *****************************

private:
    Node *                              mRoot;
    size_t                              mCount; // number of items in map
    bool                                mCaseInsensitive;
    FixSizedRawMemoryPool<sizeof(Node)> mNodePool;
    ObjectPool<Leaf>                    mLeafPool;
    DoubleLink                          mLeaves;

    // *****************************
    // private methods
    // *****************************

private:
    Node * allocNode() { return (Node *) mNodePool.alloc(); }

    void freeNode(Node * n) { mNodePool.dealloc(n); }

    Leaf * allocLeaf(const CHAR * text, size_t textlen, const T & value) {
        Leaf * p = mLeafPool.allocUnconstructed();
        if (NULL == p) return NULL;

        // call in-place new to construct the leaf
        new (p) Leaf(*this, text, textlen, value);
        if (NULL == p->key) {
            mLeafPool.freeWithoutDeconstruct(p);
            return NULL;
        }

        return p;
    }

    void freeLeaf(Leaf * l) {
        GN_ASSERT(l && this == &l->owner);
        l->link.detach();
        mLeafPool.deconstructAndFree(l);
    }

    /// clear the whole map container
    void doClear() {
        mRoot  = NULL;
        mCount = 0;
        mNodePool.freeAll();
        mLeafPool.deconstructAndFreeAll();

        // All leaves have been destructed already. So there's no
        // need to call detach() at all. Just clear the prev and
        // next pointers.
        mLeaves.prev = NULL;
        mLeaves.next = NULL;
    }

    /// make itself a clone of another map
    void doClone(const StringMap & anotherMap) {
        // shortcut for cloning itself.
        if (this == &anotherMap) return;

        // clear myself
        clear();

        // insert all items in another map to this map.
        for (const KeyValuePair * p = anotherMap.first(); NULL != p; p = anotherMap.next(p)) { insert(p->key, p->value); }
    }

    T * doFind(const CHAR * text) const {
        Leaf * p = doFindPair(text);
        return p ? &p->value : NULL;
    }

    Leaf * doFindPair(const CHAR * text) const {
        // check for NULL text pointer
        if (NULL == text) {
            static Logger * sLocalLogger = getLogger("GN.base.StringMap");
            GN_WARN(sLocalLogger)("StringMap finding warning: NULL text!");
            return NULL;
        }

        Node * p = mRoot;
        while (p) {
            int d;

            if (mCaseInsensitive) {
                // conver both to upper case
                CHAR t = *text;
                CHAR s = p->splitchar;
                if ('a' <= t && t <= 'z') t += 'A' - 'a';
                if ('a' <= s && s <= 'z') s += 'A' - 'a';
                d = t - s;
            } else {
                d = *text - p->splitchar;
            }

            if (0 == d) {
                if (0 == *text) {
                    // found!
                    GN_ASSERT(p->leaf);
                    return p->leaf;
                } else {
                    p = p->equal;
                }

                ++text;
            } else if (d < 0) {
                p = p->lower;
            } else {
                p = p->higher;
            }
        }

        // not found
        return NULL;
    }

    Leaf * doFindOrInsert(const CHAR * text, const T & value, bool & inserted) {
        inserted = false;

        // check for NULL text pointer
        if (NULL == text) {
            static Logger * sLocalLogger = getLogger("GN.base.StringMap");
            GN_WARN(sLocalLogger)("Null text is not allowed!");
            return NULL;
        }

        // store input text pointer
        const char * inputText = text;

        // search in existing nodes
        Node *  parentNode = NULL;
        Node ** pp         = &mRoot;
        while (NULL != *pp) {
            int d;

            if (mCaseInsensitive) {
                // conver both to upper case
                CHAR t = *text;
                CHAR s = (*pp)->splitchar;
                if ('a' <= t && t <= 'z') t += 'A' - 'a';
                if ('a' <= s && s <= 'z') s += 'A' - 'a';
                d = t - s;
            } else {
                d = *text - (*pp)->splitchar;
            }

            if (0 == d) {
                if (0 == *text) {
                    // The text exists already. Insertion failed.
                    GN_ASSERT((*pp)->leaf);
                    inserted = false;
                    return (*pp)->leaf;
                } else {
                    parentNode = *pp;
                    pp         = &((*pp)->equal);
                }

                ++text;
            } else if (d < 0) {
                parentNode = *pp;
                pp         = &((*pp)->lower);
            } else {
                parentNode = *pp;
                pp         = &((*pp)->higher);
            }
        }

        // No existing text found. Now insert new nodes for
        // each unmatched characters in input text.
        for (;;) {
            // create new node
            Node * newNode = allocNode();
            if (NULL == newNode) {
                static Logger * sLocalLogger = getLogger("GN.base.StringMap");
                GN_ERROR(sLocalLogger)("out of memory!");
                return NULL;
            }

            // create new leaf if reaching the end of the text
            if (0 == *text) {
                // we reach the end of the text. Now create a new leaf.
                newNode->leaf = allocLeaf(inputText, text - inputText, value);
                if (NULL == newNode->leaf) {
                    static Logger * sLocalLogger = getLogger("GN.base.StringMap");
                    GN_ERROR(sLocalLogger)("out of memory!");
                    return NULL;
                }

                // TODO: sort leaves

                // Insert the new leaf to the end of the list.
                newNode->leaf->link.linkBefore(&mLeaves);
                if (NULL == mLeaves.next) {
                    // This is the first leaf. Make a link loop. So that mLeaves.next
                    // points to the first item and mLeaves.prev points to the last
                    // item.
                    newNode->leaf->link.linkAfter(&mLeaves);
                }

                ++mCount;
                inserted = true;
            } else {
                newNode->leaf = 0;
            }

            // link new node into node tree
            newNode->parent    = parentNode;
            newNode->splitchar = *text;
            newNode->lower = newNode->higher = newNode->equal = 0;
            *pp                                               = newNode;

            // continue with next character or exit
            if (0 == *text) {
                GN_ASSERT((*pp)->leaf);
                return (*pp)->leaf;
            } else {
                parentNode = *pp;
                pp         = &((*pp)->equal);
                ++text;
            }
        }
    }

    Leaf * doFirst() const { return mLeaves.next ? (Leaf *) mLeaves.next->context : NULL; }

    Leaf * doNext(const KeyValuePair * p) const {
        if (NULL == p) return NULL;

        Leaf * leaf = (Leaf *) p;

        if (&leaf->owner != this) {
            static Logger * sLocalLogger = getLogger("GN.base.StringMap");
            GN_ERROR(sLocalLogger)("Input pointer does not belong to this string map.");
            return NULL;
        }

        DoubleLink * n = leaf->link.next;

        if (NULL == n || &mLeaves == n) return NULL;

        return (Leaf *) n->context;
    }

    Node * doRecursiveErase(Node * n, const CHAR * text) {
        GN_ASSERT(text);

        if (NULL == n) return NULL;

        int d = *text - n->splitchar;

        if (d < 0) {
            n->lower = doRecursiveErase(n->lower, text);
        } else if (d > 0) {
            n->higher = doRecursiveErase(n->higher, text);
        } else if (0 == *text) {
            // We reaches the end of the string. It must contain a leaf node.
            GN_ASSERT(n->leaf);

            // delete leaf node
            freeLeaf(n->leaf);
            n->leaf = NULL;

            --mCount;
        } else {
            n->equal = doRecursiveErase(n->equal, text + 1);
        }

        // free the node if it contains neither children or leaf
        if (0 == n->lower && 0 == n->higher && 0 == n->equal && 0 == n->leaf) {
            freeNode(n);
            return NULL;
        } else {
            return n;
        }
    }

    void doRemove(const CHAR * text) {
        // check for NULL text pointer
        if (NULL == text) {
            static Logger * sLocalLogger = getLogger("GN.base.StringMap");
            GN_WARN(sLocalLogger)("StringMap erasing warning: NULL text!");
            return;
        }

        mRoot = doRecursiveErase(mRoot, text);
    }
}; // End of StringMap class

namespace str {

/// \name string -> number conversion
///
///  Returns number of characters that are sucessfully converted. Return 0 for failure.
//@{

GN_API size_t toSignedInteger(sint64 & result, int bits, int base, const char * s);
GN_API size_t toUnsignedInteger(uint64 & result, int bits, int base, const char * s);

template<typename T>
inline size_t toInetger(T & i, const char * s, int base = 10) {
    size_t n;

    if (SignedType<T>::value) {
        sint64 s64;
        n = toSignedInteger(s64, sizeof(T) * 8, base, s);
        if (n > 0) i = (T) s64;
    } else {
        uint64 u64;
        n = toUnsignedInteger(u64, sizeof(T) * 8, base, s);
        if (n > 0) i = (T) u64;
    }

    return n;
}

template<typename T>
T toInetger(const char * s, T defaultValue, int base = 10) {
    T result;
    if (0 == toInetger<T>(result, s, base)) {
        return defaultValue;
    } else {
        return result;
    }
}

GN_API size_t toFloat(float & i, const char * s);

GN_API size_t toDouble(double & i, const char * s);

template<typename T>
inline size_t toNumber(T & i, const char * s) {
    return toInetger<T>(i, s, 10);
}
template<>
inline size_t toNumber<float>(float & i, const char * s) {
    return toFloat(i, s);
}
template<>
inline size_t toNumber<double>(double & i, const char * s) {
    return toDouble(i, s);
}
template<typename T>
T toNumber(const char * s, T defaultValue) {
    T result;
    if (toNumber<T>(result, s))
        return result;
    else
        return defaultValue;
}

///
/// Convert string to float array. String should be in format like:
///    float1, float2, float3, ...
/// or:
///    float1 float2 float3 ...
///
/// \return
///     Return count of floating filled into target buffer.
///
///     REVIEW: this function returns number of floats, while other function returns number of characters.
///
GN_API size_t toFloatArray(float * buffer, size_t maxCount, const char * string, size_t stringLength = 0);

//@}
} // namespace str
} // namespace GN

// *****************************************************************************
//                 End of string.h
// *****************************************************************************
#endif // __GN_BASE_STRING_H__
