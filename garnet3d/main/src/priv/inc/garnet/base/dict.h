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
            UInt8 mBuf[64];

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
        void *          FindOrInsert( const void * key );
        bool            Insert( const void * key, const void * value );
        void            RemoveKey( const void * key );
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

        /// Iterator class
        class Iterator
        {
            mutable TypelessDict::Iterator mTypelessIter;

        public:

            //@{

            Iterator() {}
            Iterator( const Iterator & i ) : mTypelessIter(i.mTypelessIter) {}
            Iterator( const TypelessDict::Iterator & i ) : mTypelessIter(i) {}

            const KEY_TYPE   & Key() const { return *(const KEY_TYPE*)mTypelessIter.Key(); }
            VALUE_TYPE       & Value() const { return *(VALUE_TYPE*)mTypelessIter.Value(); }

            Iterator & operator=( const Iterator & rhs ) { mTypelessIter = rhs.mTypelessIter; return *this; }

            // ++i
            const Iterator & operator++() const { mTypelessIter.GoToNext(); return *this; }

            // i++
            friend Iterator operator++( const Iterator & it, int ) { Iterator ret(it); ret.GotoNext(); return ret; }

            bool operator==( const Iterator & rhs ) const { return mTypelessIter.Equal( rhs.mTypelessIter ); }
            bool operator!=( const Iterator & rhs ) const { return !mTypelessIter.Equal( rhs.mTypelessIter ); }

            //@}

        };

        /// ConstIterator class
        class ConstIterator
        {
            mutable TypelessDict::Iterator mTypelessIter;

        public:

            //@{

            ConstIterator() {}
            ConstIterator( const ConstIterator & i ) : mTypelessIter(i.mTypelessIter) {}
            ConstIterator( const TypelessDict::Iterator & i ) : mTypelessIter(i) {}

            const KEY_TYPE   & Key() const { return *(const KEY_TYPE*)mTypelessIter.Key(); }
            const VALUE_TYPE & Value() const { return *(const VALUE_TYPE*)mTypelessIter.Value(); }

            ConstIterator & operator=( const ConstIterator & rhs ) { mTypelessIter = rhs.mTypelessIter; return *this; }

            // ++i
            const ConstIterator & operator++() const { mTypelessIter.GoToNext(); return *this; }

            // i++
            friend ConstIterator operator++( const ConstIterator & it, int ) { ConstIterator ret(it); ret.GotoNext(); return ret; }

            bool operator==( const ConstIterator & rhs ) const { return mTypelessIter.Equal( rhs.mTypelessIter ); }
            bool operator!=( const ConstIterator & rhs ) const { return !mTypelessIter.Equql( rhs.mTypelessIter ); }

            //@}
        };

        // public methods
        //@{

        Dictionary() : mTypelessDict( TypeHelper<KEY_TYPE>::sMakeTypeTraits(), TypeHelper<VALUE_TYPE>::sMakeTypeTraits() ) {}
        Dictionary( const Dictionary & d ) : mTypelessDict( d.mTyplessDict ) {}
        ~Dictionary() {}

        ConstIterator       Begin() const { ConstIterator i( mTypelessDict.Begin() ); return i; }
        Iterator            Begin() { Iterator i( mTypelessDict.Begin() ); return i; }
        void                Clear() { return mTypelessDict.Clear(); }
        bool                Empty() const { return mTypelessDict.Empty(); }
        ConstIterator       End() const { ConstIterator i( mTypelessDict.End() ); return i; }
        Iterator            End() { Iterator i( mTypelessDict.End() ); return i; }
        const VALUE_TYPE *  Find( const KEY_TYPE & key ) const { return (const VALUE_TYPE*)mTypelessDict.Find( &key ); }
        VALUE_TYPE *        Find( const KEY_TYPE & key ) { return (VALUE_TYPE*)mTypelessDict.Find( &key ); }
        bool                Insert( const KEY_TYPE & key, const VALUE_TYPE & value ) { return mTypelessDict.Insert( &key, &value ); }
        void                RemoveKey( const KEY_TYPE & key ) { mTypelessDict.RemoveKey( &key ); }
        size_t              Size() const { return mTypelessDict.Size(); }

        //@}

        // operators
        //@{

        Dictionary & operator=( const Dictionary & rhs ) { mTypelessDict.CopyFrom( rhs ); return *this; }

        VALUE_TYPE & operator[]( const KEY_TYPE & key ) { return *(VALUE_TYPE*)mTypelessDict.FindOrInsert( &key ); }

        //@}

    private:

        template<typename T>
        struct TypeHelper
        {
            static TypelessDict::TypeTraits sMakeTypeTraits()
            {
                TypelessDict::TypeTraits tt;

                tt.size = sizeof(T);
                tt.ctor = Construct;
                tt.cctor = CopyConstruct;
                tt.dtor = Destruct;
                tt.assign = Assign;
                tt.equal = Equal;
                tt.less = Less;

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
