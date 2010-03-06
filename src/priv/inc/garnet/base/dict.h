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
            UInt8 mBuf[32];

        public:

            Iterator(); ///< ctor
            Iterator( const Iterator & ); ///< copy ctor
            ~Iterator(); ///< dtor

            const void * Key() const;   ///< get pointer to key
            void       * Value() const; ///< get pointer to value.
            void         GoToNext(); ///< move iterator to next item
            bool         Equal( const Iterator & rhs ) const; // check equality of 2 iterators

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

        Iterator        Begin() const;
        void            Clear();
        void            CopyFrom( const TypelessDict & );
        bool            Empty() const;
        Iterator        End() const;
        void *          Find( const void * key ) const;
        void *          FindOrInsert( const void * key, TypeTraits::CtorFunc ctor );
        bool            Insert( const void * key, const void * value, Iterator * iter );
        void            Remove( const void * key );
        size_t          Size() const;

        //@}

    private:

        class Impl;

        Impl * mImpl;
    };

    /// Dictionary template
    template<typename KEY_TYPE, typename VALUE_TYPE>
    class Dictionary
    {
    public:

        class KeyValuePair
        {
            mutable TypelessDict::Iterator mTypelessIter;

            friend class Iterator;
            friend class ConstIterator;

        public:

            KeyValuePair() {}
            KeyValuePair( const KeyValuePair & p ) : mTypelessIter(p.mTypelessIter) {}
            KeyValuePair( const TypelessDict::Iterator & i ) : mTypelessIter(i) {}

            const KEY_TYPE   & Key() const { return *(const KEY_TYPE*)mTypelessIter.Key(); }

            const VALUE_TYPE & Value() const { return *(const VALUE_TYPE*)mTypelessIter.Value(); }

            VALUE_TYPE       & Value() { return *(VALUE_TYPE*)mTypelessIter.Value(); }

            //const Iterator & operator++() const { mTypelessIter.GoToNext(); return *this; }
            //friend Iterator operator++( const Iterator & it, int ) { Iterator ret(it); ret.GotoNext(); return ret; }
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

            const Iterator & operator++() const { mKeyValuePair.mTypelessIter.GoToNext(); return *this; }
            friend Iterator operator++( Iterator & it, int ) { Iterator ret(it); it.GotoNext(); return ret; }

            bool operator==( const ConstIterator & rhs ) const { return mKeyValuePair.mTypelessIter.Equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator==( const Iterator      & rhs ) const { return mKeyValuePair.mTypelessIter.Equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator!=( const ConstIterator & rhs ) const { return !mKeyValuePair.mTypelessIter.Equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator!=( const Iterator      & rhs ) const { return !mKeyValuePair.mTypelessIter.Equal( rhs.mKeyValuePair.mTypelessIter ); }

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
            const ConstIterator & operator++() const { mKeyValuePair.mTypelessIter.GoToNext(); return *this; }

            // i++
            friend ConstIterator operator++( ConstIterator & it, int ) { ConstIterator ret(it); it.GotoNext(); return ret; }

            bool operator==( const ConstIterator & rhs ) const { return mKeyValuePair.mTypelessIter.Equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator==( const Iterator      & rhs ) const { return mKeyValuePair.mTypelessIter.Equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator!=( const ConstIterator & rhs ) const { return !mKeyValuePair.mTypelessIter.Equal( rhs.mKeyValuePair.mTypelessIter ); }
            bool operator!=( const Iterator      & rhs ) const { return !mKeyValuePair.mTypelessIter.Equal( rhs.mKeyValuePair.mTypelessIter ); }

            //@}
        };

        // public methods
        //@{

        Dictionary() : mTypelessDict( TypeHelper<KEY_TYPE>::sMakeKeyTypeTraits(), TypeHelper<VALUE_TYPE>::sMakeValueTypeTraits() ) {}
        Dictionary( const Dictionary & d ) : mTypelessDict( d.mTypelessDict ) {}
        ~Dictionary() {}

        ConstIterator       Begin() const { ConstIterator i( mTypelessDict.Begin() ); return i; }
        Iterator            Begin() { Iterator i( mTypelessDict.Begin() ); return i; }
        void                Clear() { return mTypelessDict.Clear(); }
        bool                Empty() const { return mTypelessDict.Empty(); }
        ConstIterator       End() const { ConstIterator i( mTypelessDict.End() ); return i; }
        Iterator            End() { Iterator i( mTypelessDict.End() ); return i; }
        const VALUE_TYPE *  Find( const KEY_TYPE & key ) const { return (const VALUE_TYPE*)mTypelessDict.Find( &key ); }
        VALUE_TYPE *        Find( const KEY_TYPE & key ) { return (VALUE_TYPE*)mTypelessDict.Find( &key ); }
        bool                Insert( const KEY_TYPE & key, const VALUE_TYPE & value, Iterator * iter = NULL ) { return mTypelessDict.Insert( &key, &value, (TypelessDict::Iterator*)iter ); }
        void                Remove( const KEY_TYPE & key ) { mTypelessDict.Remove( &key ); }
        size_t              Size() const { return mTypelessDict.Size(); }

        //@}

        // operators
        //@{

        Dictionary & operator=( const Dictionary & rhs ) { mTypelessDict.CopyFrom( rhs.mTypelessDict ); return *this; }

        VALUE_TYPE & operator[]( const KEY_TYPE & key ) { return FindOrCreate( key ); }

        const VALUE_TYPE & operator[]( const KEY_TYPE & key ) const { return *Find( key ); }

        //@}

    private:

        VALUE_TYPE & FindOrCreate( const KEY_TYPE & key )
        {
            TypelessDict::TypeTraits::CtorFunc ctor = TypeHelper<VALUE_TYPE>::Construct;

            return *(VALUE_TYPE*)mTypelessDict.FindOrInsert( &key, ctor );
        }

        template<typename T>
        struct TypeHelper
        {
            static TypelessDict::TypeTraits sMakeKeyTypeTraits()
            {
                TypelessDict::TypeTraits tt;

                tt.size = sizeof(T);
                tt.ctor = NULL;//Construct;
                tt.cctor = CopyConstruct;
                tt.dtor = Destruct;
                tt.assign = Assign;
                tt.equal = NULL;//Equal;
                tt.less = &Less;

                return tt;
            }

            static TypelessDict::TypeTraits sMakeValueTypeTraits()
            {
                TypelessDict::TypeTraits tt;

                tt.size = sizeof(T);
                tt.ctor = NULL;//Construct; // ctor is required for [] operator only
                tt.cctor = CopyConstruct;
                tt.dtor = Destruct;

                // value type requires no operator
                tt.assign = NULL;//Assign;
                tt.equal = NULL;//Equal;
                tt.less = NULL;//&Less;

                return tt;
            }

            static void Construct( void * ptr )
            {
                GN_ASSERT( ptr );
                new (ptr) T();
            }

            static void CopyConstruct( void * ptr, const void * ref )
            {
                GN_ASSERT( ptr && ref );
                new (ptr) T(*(const T*)ref);
            }

            static void Destruct( void * ptr )
            {
                GN_ASSERT( ptr );
                T * k = (T*)ptr;
                k->T::~T();
            }

            static void Assign( void * a, const void * b )
            {
                (*(T*)a) = (*(const T*)b);
            }

            static bool Equal( const void * a, const void * b )
            {
                GN_ASSERT( a && b );
                return (*(const T*)a) == (*(const T*)b);
            }

            static bool Less( const void * a, const void * b )
            {
                GN_ASSERT( a && b );
                return (*(const T*)a) < (*(const T*)b);
            }
        };

        TypelessDict mTypelessDict;
    };
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_DICT_H__
