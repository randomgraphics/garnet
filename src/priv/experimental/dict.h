#ifndef __GN_BASE_DICT_H__
#define __GN_BASE_DICT_H__
// *****************************************************************************
/// \file
/// \brief   Dictionary template
/// \author  chenli@@REDMOND (2010.2.27)
// *****************************************************************************

namespace GN
{
    class TypelessDict
    {
    public:

        class Iterator
        {
            TypelessDict * mDict;

        public:

            Iterator();
            Iterator( const Iterator & );
            ~Iterator();

            const void * Key() const;
            const void * Value() const;
            void       * Value();
        };

        typedef const Iterator ConstIterator;

        struct TypeTraits
        {
            typedef void (*CtorFunc)(void*);
            typedef void (*CopyCtorFunc)(void*, const void *);
            typedef void (*DtorFunc)(void*);
            typedef void (*AssignFunc)(void*, const void*);
            typedef void (*EqualFunc)(const void*, const void*);
            typedef void (*LessFunc)(const void*, const void*);

            size_t       size;
            CtorFunc     ctor;
            CopyCtorFunc cctor;
            DtorFunc     dtor;
            AssignFunc   assign;
            EqualFunc    equal;
            LessFunc     less;
        };

        TypelessDict( TypeTraits key, TypeTraits value );
        TypelessDict( const TypelessDict & );

        ConstIterator   Begin() const;
        Iterator        Begin();
        bool            Empty() const;
        ConstIterator   End() const;
        Iterator        End();
        VALUE_TYPE *    Find( const KEY_TYPE & key );
        bool            Insert( const KEY_TYPE & key, const VALUE_TYPE & value );
        void            RemoveKey( const KEY_TYPE & key );
        size_t          Size() const;

    private:

        class Impl;

        Impl * mImpl;
    };

    template<typename KEY_TYPE, typename VALUE_TYPE>
    class Dictionary
    {
    public:

        /// Iterator class
        class Iterator
        {
            TypelessDict::Iterator mTypelessIter;

        public:

            Iterator() {}
            Iterator( const Iterator & i ) : mTypelessIter(i.mIter) {}
            Iterator( TypelessDict::Iterator & i ) : mTypelessIter(i) {}
            ~Iterator();

            const KEY_TYPE   * Key() const { return (const KEY_TYPE*)mTypelessIter->Key(); }
            const VALUE_TYPE * Value() const { return (const VALUE_TYPE*)mTypelessIter->Value(); }
            VALUE_TYPE       * Value() { return (VALUE_TYPE*)mTypelessIter->Value(); }
        };

        typedef const Iterator ConstIterator;

        // public methods
        //@{

        Dictionary() : mTypelessDict( TypeHelper<KEY_TYPE>::sMakeTypeTraits(), TypeHelper<VALUE_TYPE>::sMakeTypeTraits ) {}
        Dictionary( const Dictionary & d ) : mTypelessDict( d.mTyplessDict ) {}
        ~Dictionary() {}

        ConstIterator   Begin() const { ConstIterator i( mTypelessDict.Begin() ); return i; }
        Iterator        Begin() { Iterator i( mTypelessDict.Begin() ); return i; }
        bool            Empty() const { return mTypelessDict.Empty(); }
        ConstIterator   End() const { ConstIterator i( mTypelessDict.End() ); return i; }
        Iterator        End() { Iterator i( mTypelessDict.End() ); return i; }
        VALUE_TYPE *    Find( const KEY_TYPE & key ) { return (VALUE_TYPE*)mTypelessDict.Find( &key ); }
        bool            Insert( const KEY_TYPE & key, const VALUE_TYPE & value ) { return mTypelessDict.Insert( &key, &value ); }
        void            RemoveKey( const KEY_TYPE & key ) { mTypelessDict.RemoveKey( &key ); }
        size_t          Size() const { return mTypelessDict.Size(); }

        //@}

        // operators
        //@{

        Dictionary & operator=( const Dictionary & rhs );

        VALUE_TYPE & operator[]( const KEY_TYPE & key );

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
                (*(const T*)a) = (*(const T*)b)
            }

            static void Equal( const void * a, const void * b )
            {
                GN_ASSERT( a && b );
                return (*(const T*)a) == (*(const T*)b);
            }

            static void Less( const void * a, const void * b )
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
