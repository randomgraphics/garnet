#ifndef __GN_BASE_DICT_H__
#define __GN_BASE_DICT_H__
// *****************************************************************************
/// \file
/// \brief   Dictionary template
/// \author  chenli@@REDMOND (2010.2.27)
// *****************************************************************************

namespace GN
{
    template<typename KEY_TYPE, typename VALUE_TYPE>
    class Dictionary
    {
        template<typename T>
        struct TypeConsturctor
        {
            static void Construct( void * ptr )
            {
                GN_ASSERT( ptr );
                new (ptr) T();
            }

            static void CopyConstruct( void * ptr, void * ref )
            {
                GN_ASSERT( ptr && ref );
                new (ptr) T(*(const T*)ref);
            }

            static void Destructor( void * ptr )
            {
                GN_ASSERT( ptr );
                T * k = (T*)ptr;
                k->T::~T();
            }
        };

    public:

        /// Iterator class
        class Iterator
        {
        public:

            Iterator();
            Iterator( const Iterator & );
            ~Iterator();

            const KEY_TYPE   * Key() const;
            const VALUE_TYPE * Value() const;
            VALUE_TYPE       * Value();
        };

        typedef const Iterator ConstIterator;

        // public methods
        //@{

        Dictionary();
        Dictionary( const Dictionary & );
        ~Dictionary();

        ConstIterator   Begin() const;
        Iterator        Begin();
        ConstIterator   End() const;
        Iterator        End();
        VALUE_TYPE *    Find( const KEY_TYPE & key );
        bool            Insert( const KEY_TYPE & key, const VALUE_TYPE & value );
        void            RemoveKey( const KEY_TYPE & key );

        //@}

        // operators
        //@{

        Dictionary & operator=( const Dictionary & rhs );

        VALUE_TYPE & operator[]( const KEY_TYPE & key );

        //@}
    };
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_DICT_H__
