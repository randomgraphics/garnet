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
    /// fence ID type
    ///
    class FenceId
    {
        int value;

    public:

        ///
        /// ctor
        ///
        FenceId() {}

        ///
        /// ctor
        ///
        explicit FenceId( int value_ ) : value(value_) {}

        //@{
        bool operator <  ( const FenceId & rhs ) const { return rhs.value - value >  0; }
        bool operator <= ( const FenceId & rhs ) const { return rhs.value - value >= 0; }
        bool operator >  ( const FenceId & rhs ) const { return value - rhs.value >  0; }
        bool operator >= ( const FenceId & rhs ) const { return value - rhs.value >= 0; }
        bool operator == ( const FenceId & rhs ) const { return value == rhs.value; }
        bool operator != ( const FenceId & rhs ) const { return value != rhs.value; }

        bool operator <  ( volatile const FenceId & rhs ) volatile const { return rhs.value - value >  0; }
        bool operator <= ( volatile const FenceId & rhs ) volatile const { return rhs.value - value >= 0; }
        bool operator >  ( volatile const FenceId & rhs ) volatile const { return value - rhs.value >  0; }
        bool operator >= ( volatile const FenceId & rhs ) volatile const { return value - rhs.value >= 0; }
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
            return *this;
        }

        ///
        /// postfie increament
        ///
        FenceId operator++(int)
        {
            FenceId r(*this);
            ++value;
            return r;
        }

        //@}
    };
}}

// *****************************************************************************
//                           End of fence.h
// *****************************************************************************
#endif // __GN_ENGINE_FENCE_H__
