#ifndef __GN_BASE_DICT_H__
#define __GN_BASE_DICT_H__
// *****************************************************************************
/// \file
/// \brief   Dictionary template
/// \author  chenli@@REDMOND (2010.2.27)
// *****************************************************************************

namespace GN
{
    /// Typeless dictionary class.
    class TypelessDict
    {
    public:

        /// Typeless iterator clas
        class Iterator
        {
            uint8 mBuf[32];

        public:

            Iterator(); ///< ctor
            Iterator( const Iterator & ); ///< copy ctor
            ~Iterator(); ///< dtor

            const void * key() const;   ///< get pointer to key
            void       * value() const; ///< get pointer to value.
            void         moveToNext(); ///< move iterator to next item
            bool         equal( const Iterator & rhs ) const; // check equality of 2 iterators

            Iterator & operator=( const Iterator & rhs ); // assign operator
        };

        /// Type traits
        struct TypeTraits
        {
            ///@{
            typedef void (*CtorFunc)(void*);
            typedef void (*CopyCtorFunc)(void*, const void *);
            typedef void (*DtorFunc)(void*);
            typedef void (*AssignFunc)(void*, const void*);
            typedef bool (*EqualFunc)(const void*, const void*);
            typedef bool (*LessFunc)(const void*, const void*);
            //@}

            //@{
            size_t       size;
            CtorFunc     ctor;
            CopyCtorFunc cctor;
            DtorFunc     dtor;
            AssignFunc   assign;
            EqualFunc    equal;
            LessFunc     less;
            //@}
        };

        //@{

        TypelessDict( TypeTraits key, TypeTraits value );
        TypelessDict( const TypelessDict & );
        ~TypelessDict();

        Iterator        begin() const;
        void            clear();
        void            copyFrom( const TypelessDict & );
        bool            empty() const;
        Iterator        end() const;
        void *          find( const void * key ) const;
        void *          findOrInsert( const void * key, TypeTraits::CtorFunc ctor );
        bool            insert( const void * key, const void * value, Iterator * iter );
        void            remove( const void * key );
        size_t          size() const;

        //@}

    private:

        class Impl;

        Impl * mImpl;
    };

    template<typename T>
    struct DictionaryUtil_LessOperator
    {
        bool operator()( const T & a, const T & b )
        {
            return a < b;
        }
    };

    /// Dictionary template
    template<typename KEY_TYPE, typename VALUE_TYPE, typename KEY_LESS_FUNC=DictionaryUtil_LessOperator<KEY_TYPE> >
    class Dictionary
    {
    public:

        class Iterator;
        class ConstIterator;

        class KeyValuePair
        {
            mutable TypelessDict::Iterator mTypelessIter;

            friend class Iterator;
            friend class ConstIterator;

        public:

            KeyValuePair() {}
            KeyValuePair( const KeyValuePair & p ) : mTypelessIter(p.mTypelessIter) {}
            KeyValuePair( const TypelessDict::Iterator & i ) : mTypelessIter(i) {}

            const KEY_TYPE   & key() const { return *(const KEY_TYPE*)mTypelessIter.key(); }

            const VALUE_TYPE & value() const { return *(const VALUE_TYPE*)mTypelessIter.value(); }

            VALUE_TYPE       & value() { return *(VALUE_TYPE*)mTypelessIter.value(); }

            //const Iterator & operator++() const { mTypelessIter.moveToNext(); return *this; }
            //friend Iterator operator++( const Iterator & it, int ) { Iterator ret(it); ret.moveToNext(); return ret; }
            //bool operator==( const ConstIterator & rhs ) const { return mTypelessIter.Equal( rhs.mTypelessIter ); }
            //bool operator==( const Iterator      & rhs ) const { return mTypelessIter.Equal( rhs.mTypelessIter ); }
            //bool operator!=( const ConstIterator & rhs ) const { return !mTypelessIter.Equal( rhs.mTypelessIter ); }
            //bool operator!=( const Iterator      & rhs ) const { return !mTypelessIter.Equal( rhs.mTypelessIter ); }
        };

        /// Iterator class
        class Iterator
        {
            mutable KeyValuePair mKeyValuePair;

            friend class ConstIterator;

        public:

            //@{

            typedef std::forward_iterator_tag iterator_category;
            typedef KeyValuePair value_type;
            typedef size_t difference_type;
            typedef difference_type distrance_type;
            typedef KeyValuePair * pointer;
            typedef KeyValuePair & reference;

            Iterator() {}
            Iterator( const Iterator & i ) : mKeyValuePair(i.mKeyValuePair) {}
            Iterator( const TypelessDict::Iterator & i ) : mKeyValuePair(i) {}

            KeyValuePair & operator*() const { return mKeyValuePair; }
            KeyValuePair * operator->() const { return &mKeyValuePair; }

            Iterator & operator=( const Iterator & rhs ) { mKeyValuePair.mTypelessIter = rhs.mKeyValuePair.mTypelessIter; return *this; }

            const Iterator & operator++() const { mKeyValuePair.mTypelessIter.moveToNext(); return *this; }
            friend Iterator operator++( Iterator & it, int ) { Iterator ret(it); it.moveToNext(); return ret; }

            bool operator==( const ConstIterator & rhs ) const { return mKeyValuePair.mTypelessIter.equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator==( const Iterator      & rhs ) const { return mKeyValuePair.mTypelessIter.equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator!=( const ConstIterator & rhs ) const { return !mKeyValuePair.mTypelessIter.equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator!=( const Iterator      & rhs ) const { return !mKeyValuePair.mTypelessIter.equal( rhs.mKeyValuePair.mTypelessIter ); }

            //@}

        };

        /// ConstIterator class
        class ConstIterator
        {
            KeyValuePair mKeyValuePair;

            friend class Iterator;

        public:

            //@{

            typedef std::forward_iterator_tag iterator_category;
            typedef const KeyValuePair value_type;
            typedef size_t difference_type;
            typedef difference_type distrance_type;
            typedef const KeyValuePair * pointer;
            typedef const KeyValuePair & reference;

            ConstIterator() {}
            ConstIterator( const ConstIterator & i ) : mKeyValuePair(i.mKeyValuePair) {}
            ConstIterator( const Iterator      & i ) : mKeyValuePair(i.mKeyValuePair) {}
            ConstIterator( const TypelessDict::Iterator & i ) : mKeyValuePair(i) {}

            const KeyValuePair & operator*() const { return mKeyValuePair; }
            const KeyValuePair * operator->() const { return &mKeyValuePair; }

            ConstIterator & operator=( const ConstIterator & rhs ) { mKeyValuePair.mTypelessIter = rhs.mKeyValuePair.mTypelessIter; return *this; }
            ConstIterator & operator=( const Iterator      & rhs ) { mKeyValuePair.mTypelessIter = rhs.mKeyValuePair.mTypelessIter; return *this; }

            // ++i
            const ConstIterator & operator++() const { mKeyValuePair.mTypelessIter.moveToNext(); return *this; }

            // i++
            friend ConstIterator operator++( ConstIterator & it, int ) { ConstIterator ret(it); it.moveToNext(); return ret; }

            bool operator==( const ConstIterator & rhs ) const { return mKeyValuePair.mTypelessIter.equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator==( const Iterator      & rhs ) const { return mKeyValuePair.mTypelessIter.equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator!=( const ConstIterator & rhs ) const { return !mKeyValuePair.mTypelessIter.equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator!=( const Iterator      & rhs ) const { return !mKeyValuePair.mTypelessIter.equal( rhs.mKeyValuePair.mTypelessIter ); }

            //@}
        };

        // public methods
        //@{

        Dictionary()
            : mTypelessDict( TypeHelper<KEY_TYPE>::sMakeKeyTypeTraits(), TypeHelper<VALUE_TYPE>::sMakeValueTypeTraits() )
        {
        }
        Dictionary( const Dictionary & d )
            : mTypelessDict( d.mTypelessDict )
        {
        }
        ~Dictionary()
        {
        }

        ConstIterator       begin() const { ConstIterator i( mTypelessDict.begin() ); return i; }
        Iterator            begin() { Iterator i( mTypelessDict.begin() ); return i; }
        void                clear() { return mTypelessDict.clear(); }
        bool                empty() const { return mTypelessDict.empty(); }
        ConstIterator       end() const { ConstIterator i( mTypelessDict.end() ); return i; }
        Iterator            end() { Iterator i( mTypelessDict.end() ); return i; }
        const VALUE_TYPE *  find( const KEY_TYPE & key ) const { return (const VALUE_TYPE*)mTypelessDict.find( &key ); }
        VALUE_TYPE *        find( const KEY_TYPE & key ) { return (VALUE_TYPE*)mTypelessDict.find( &key ); }
        bool                insert( const KEY_TYPE & key, const VALUE_TYPE & value, Iterator * iter = NULL ) { return mTypelessDict.insert( &key, &value, (TypelessDict::Iterator*)iter ); }
        void                remove( const KEY_TYPE & key ) { mTypelessDict.remove( &key ); }
        size_t              size() const { return mTypelessDict.size(); }

        //@}

        // operators
        //@{

        Dictionary & operator=( const Dictionary & rhs ) { mTypelessDict.copyFrom( rhs.mTypelessDict ); return *this; }

        VALUE_TYPE & operator[]( const KEY_TYPE & key ) { return findOrInsert( key ); }

        const VALUE_TYPE & operator[]( const KEY_TYPE & key ) const { return *find( key ); }

        //@}

    private:

        VALUE_TYPE & findOrInsert( const KEY_TYPE & key )
        {
            TypelessDict::TypeTraits::CtorFunc ctor = TypeHelper<VALUE_TYPE>::construct;

            return *(VALUE_TYPE*)mTypelessDict.findOrInsert( &key, ctor );
        }

        template<typename T>
        struct TypeHelper
        {
            static TypelessDict::TypeTraits sMakeKeyTypeTraits()
            {
                TypelessDict::TypeTraits tt;

                tt.size = sizeof(T);
                tt.ctor = NULL;//construct;
                tt.cctor = copyConstruct;
                tt.dtor = destruct;
                tt.assign = assign;
                tt.equal = NULL;//equal;
                tt.less = &less;

                return tt;
            }

            static TypelessDict::TypeTraits sMakeValueTypeTraits()
            {
                TypelessDict::TypeTraits tt;

                tt.size = sizeof(T);
                tt.ctor = NULL;//construct; // ctor is required for [] operator only
                tt.cctor = copyConstruct;
                tt.dtor = destruct;

                // value type requires no operator
                tt.assign = NULL;//assign;
                tt.equal = NULL;//equal;
                tt.less = NULL;//&less;

                return tt;
            }

            static void construct( void * ptr )
            {
                GN_ASSERT( ptr );
                new (ptr) T();
            }

            static void copyConstruct( void * ptr, const void * ref )
            {
                GN_ASSERT( ptr && ref );
                new (ptr) T(*(const T*)ref);
            }

            static void destruct( void * ptr )
            {
                GN_ASSERT( ptr );
                T * k = (T*)ptr;
                GN_UNUSED_PARAM(k);
                k->T::~T();
            }

            static void assign( void * a, const void * b )
            {
                (*(T*)a) = (*(const T*)b);
            }

            static bool equal( const void * a, const void * b )
            {
                GN_ASSERT( a && b );
                return (*(const T*)a) == (*(const T*)b);
            }

            static bool less( const void * a, const void * b )
            {
                GN_ASSERT( a && b );
                KEY_LESS_FUNC lessFunc;
                return lessFunc( *(const T*)a, *(const T*)b );
            }
        };

        TypelessDict  mTypelessDict;
    };
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_DICT_H__
