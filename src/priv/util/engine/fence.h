#ifndef __GN_ENGINE_FENCE_H__
#define __GN_ENGINE_FENCE_H__
// *****************************************************************************
//! \file    engine/fence.h
//! \brief   fence definition
//! \author  chen@@CHENLI-HP (2007.5.2)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// Fence ID type.
    ///
    /// \note 0 is reserved as initial fence, which is smaller then any other fences.
    ///
    class FenceId
    {
        int value;

        static inline int sCompare( int v1, int v2 )
        {
            if( 0 == v1 )
            {
                if( 0 == v2 ) return 0;
                else return -1;
            }
            else
            {
                if( 0 == v2 ) return 1;
                else
                {
                    int d = v2 - v1;
                    if( d > 0 ) return 1;
                    if( d < 0 ) return -1;
                    return 0;
                }
            }
        }

    public:

        ///
        /// ctor
        ///
        FenceId() {}

        ///
        /// ctor
        ///
        FenceId( const FenceId & f ) : value( f.value )
        {
        }

        ///
        /// ctor
        ///
        explicit FenceId( int value_ ) : value( value_ )
        {
        }

        //@{
        bool operator <  ( const FenceId & rhs ) const { return -1 == sCompare( value, rhs.value ); }
        bool operator <= ( const FenceId & rhs ) const { return  1 != sCompare( value, rhs.value ); }
        bool operator >  ( const FenceId & rhs ) const { return  1 == sCompare( value, rhs.value ); }
        bool operator >= ( const FenceId & rhs ) const { return -1 != sCompare( value, rhs.value ); }
        bool operator == ( const FenceId & rhs ) const { return value == rhs.value; }
        bool operator != ( const FenceId & rhs ) const { return value != rhs.value; }

        bool operator <  ( volatile const FenceId & rhs ) volatile const { return -1 == sCompare( value, rhs.value ); }
        bool operator <= ( volatile const FenceId & rhs ) volatile const { return  1 != sCompare( value, rhs.value ); }
        bool operator >  ( volatile const FenceId & rhs ) volatile const { return  1 == sCompare( value, rhs.value ); }
        bool operator >= ( volatile const FenceId & rhs ) volatile const { return -1 != sCompare( value, rhs.value ); }
        bool operator == ( volatile const FenceId & rhs ) volatile const { return value == rhs.value; }
        bool operator != ( volatile const FenceId & rhs ) volatile const { return value != rhs.value; }

        //FenceId & operator = ( const FenceId & rhs ) { value = rhs.value; return *this; }
        //FenceId & operator = ( int value_ ) { value = value_; return *this; }

        volatile FenceId & operator = ( const FenceId & rhs ) volatile { value = rhs.value; return *this; }
        volatile FenceId & operator = ( int value_ ) volatile { value = value_; return *this; }

        ///
        /// prefix increment
        ///
        FenceId & operator++()
        {
            ++value;
            if( 0 == value ) ++value;
            return *this;
        }

        ///
        /// postfie increament
        ///
        FenceId operator++(int)
        {
            FenceId r(*this);
            ++value;
            if( 0 == value ) ++value;
            return r;
        }

        //@}
    };
}}

// *****************************************************************************
//                           End of fence.h
// *****************************************************************************
#endif // __GN_ENGINE_FENCE_H__
