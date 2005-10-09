// *****************************************************************************
//! \file    sigslot.h
//! \brief   signal slot implementation
//! \author  chenlee (2005.5.6)
// *****************************************************************************
#ifdef GN_SIGSLOT_TEMPL_N

#ifndef GN_JOIN
#define GN_JOIN(s1, s2)         GN_CONCATNATE_DIRECT(s1, s2)
#define GN_CONCATNATE_DIRECT(s1, s2)  s1##s2
#endif

#define PARAM_TEMPLS_0
#define PARAM_TEMPLS_1 class Param1
#define PARAM_TEMPLS_2 PARAM_TEMPLS_1, class Param2
#define PARAM_TEMPLS_3 PARAM_TEMPLS_2, class Param3
#define PARAM_TEMPLS_4 PARAM_TEMPLS_3, class Param4
#define PARAM_TEMPLS_5 PARAM_TEMPLS_4, class Param5
#define PARAM_TEMPLS_6 PARAM_TEMPLS_5, class Param6
#define PARAM_TEMPLS_7 PARAM_TEMPLS_6, class Param7
#define PARAM_TEMPLS_8 PARAM_TEMPLS_7, class Param8
#define PARAM_TEMPLS_9 PARAM_TEMPLS_8, class Param9
#define PARAM_TEMPLS   GN_JOIN( PARAM_TEMPLS_, GN_SIGSLOT_TEMPL_N)

#define PARAM_TYPES_0
#define PARAM_TYPES_1 Param1
#define PARAM_TYPES_2 PARAM_TYPES_1, Param2
#define PARAM_TYPES_3 PARAM_TYPES_2, Param3
#define PARAM_TYPES_4 PARAM_TYPES_3, Param4
#define PARAM_TYPES_5 PARAM_TYPES_4, Param5
#define PARAM_TYPES_6 PARAM_TYPES_5, Param6
#define PARAM_TYPES_7 PARAM_TYPES_6, Param7
#define PARAM_TYPES_8 PARAM_TYPES_7, Param8
#define PARAM_TYPES_9 PARAM_TYPES_8, Param9
#define PARAM_TYPES   GN_JOIN( PARAM_TYPES_, GN_SIGSLOT_TEMPL_N)

#define PARAM_VALUES_0
#define PARAM_VALUES_1 p1
#define PARAM_VALUES_2 PARAM_VALUES_1, p2
#define PARAM_VALUES_3 PARAM_VALUES_2, p3
#define PARAM_VALUES_4 PARAM_VALUES_3, p4
#define PARAM_VALUES_5 PARAM_VALUES_4, p5
#define PARAM_VALUES_6 PARAM_VALUES_5, p6
#define PARAM_VALUES_7 PARAM_VALUES_6, p7
#define PARAM_VALUES_8 PARAM_VALUES_7, p8
#define PARAM_VALUES_9 PARAM_VALUES_8, p9
#define PARAM_VALUES   GN_JOIN( PARAM_VALUES_, GN_SIGSLOT_TEMPL_N)

#define PARAM_LIST_0
#define PARAM_LIST_1 Param1 p1
#define PARAM_LIST_2 PARAM_LIST_1, Param2 p2
#define PARAM_LIST_3 PARAM_LIST_2, Param3 p3
#define PARAM_LIST_4 PARAM_LIST_3, Param4 p4
#define PARAM_LIST_5 PARAM_LIST_4, Param5 p5
#define PARAM_LIST_6 PARAM_LIST_5, Param6 p6
#define PARAM_LIST_7 PARAM_LIST_6, Param7 p7
#define PARAM_LIST_8 PARAM_LIST_7, Param8 p8
#define PARAM_LIST_9 PARAM_LIST_8, Param9 p9
#define PARAM_LIST   GN_JOIN( PARAM_LIST_, GN_SIGSLOT_TEMPL_N)

#define PARAM_COMMA_0
#define PARAM_COMMA_1 ,
#define PARAM_COMMA_2 ,
#define PARAM_COMMA_3 ,
#define PARAM_COMMA_4 ,
#define PARAM_COMMA_5 ,
#define PARAM_COMMA_6 ,
#define PARAM_COMMA_7 ,
#define PARAM_COMMA_8 ,
#define PARAM_COMMA_9 ,
#define PARAM_COMMA   GN_JOIN( PARAM_COMMA_, GN_SIGSLOT_TEMPL_N)

#define SIGNAL_NAME     GN_JOIN(Signal,GN_SIGSLOT_TEMPL_N)
#define FUNCTOR_NAME    GN_JOIN(Functor,GN_SIGSLOT_TEMPL_N)

namespace GN
{
    //!
    //! template signal class
    //!
    template<typename R PARAM_COMMA PARAM_TEMPLS>
    class SIGNAL_NAME : public detail::SignalBase
    {
        typedef FUNCTOR_NAME<R PARAM_COMMA PARAM_TYPES>  FunctorType;

        struct SlotDesc
        {
            FunctorType      func;
            const SlotBase * slot;
        };

        typedef typename ::std::list<SlotDesc>         SlotContainer;
        typedef typename SlotContainer::iterator       SlotIter;
        typedef typename SlotContainer::const_iterator ConstSlotIter;

        struct EqualSlotClassPtr
        {
            const void * mPtr;

            EqualSlotClassPtr( const void * ptr ) : mPtr(ptr) {}

            bool operator() ( const SlotDesc & desc ) const
            { return mPtr == desc.slot; }
        };

        template<class RetType,class IterType>
        struct Emitter
        {
            RetType emit( IterType begin, const IterType & end PARAM_COMMA PARAM_LIST )
            {
                RetType last;
                for( ; begin != end; ++begin )
                {
                    last = (*begin).func(PARAM_VALUES);
                }
                return last;
            }
        };

        template<class IterType>
        struct Emitter<bool,IterType>
        {
            bool emit( IterType begin, const IterType & end PARAM_COMMA PARAM_LIST )
            {
                for( ; begin != end; ++begin )
                {
                    if( !(*begin).func(PARAM_VALUES) ) return false;
                }
                return true;
            }
        };

        template<class IterType>
        struct Emitter<void,IterType>
        {
            void emit( IterType begin, const IterType & end PARAM_COMMA PARAM_LIST )
            {
                for( ; begin != end; ++begin )
                {
                    (*begin).func(PARAM_VALUES);
                }
            }
        };

        mutable SlotContainer mSlots;

    public:

        ~SIGNAL_NAME()
        {
            // disconnect with all slots
            for( SlotIter i = mSlots.begin(); i != mSlots.end(); ++i )
            {
                if ( (*i).slot ) disconnectToSlot( *(*i).slot );
            }
            mSlots.clear();
        }

        void connect( R (*staticFuncPtr)(PARAM_TYPES) ) const
        {
            if( 0 == staticFuncPtr ) return;
            SlotDesc desc;
            desc.func.bind(staticFuncPtr);
            desc.slot = 0;
            AddSlot( desc );
        }

        template<class X, class Y>
        inline void connect( Y * classPtr, R (X::*memFuncPtr)(PARAM_TYPES) ) const
        {
            GN_ASSERT( !IsConst<Y>::value ); // Y can't be const class
            if( 0 == classPtr || 0 == memFuncPtr ) return;
            SlotDesc desc;
            desc.func.bind( classPtr, memFuncPtr );
            desc.slot = IsBaseAndDerived<SlotBase,Y>::value ? (SlotBase*)classPtr : NULL;
            AddSlot( desc );
        }

        template<class X, class Y>
        inline void connect( const Y * classPtr, R (X::*memFuncPtr)(PARAM_TYPES) const ) const
        {
            if( 0 == classPtr || 0 == memFuncPtr ) return;
            SlotDesc desc;
            desc.func.bind( classPtr, memFuncPtr );
            desc.slot = IsBaseAndDerived<SlotBase,Y>::value ? (SlotBase*)classPtr : NULL;
            AddSlot( desc );
        }

        void disconnect( R (*staticFuncPtr)(PARAM_TYPES) ) const
        {
            if( 0 == staticFuncPtr ) return;
            SlotDesc desc;
            desc.func.bind(staticFuncPtr);
            desc.slot = 0;
            ConstSlotIter i = FindSlot(desc);
            if( i != mSlots.end() ) mSlots.remove(i);
        }

        void disconnect( const SlotBase & slot ) const
        {
            slotDisconnect(slot);   // remove slot from private slot list
            disconnectToSlot(slot); // inform the slot to disconnect with myself.
        }

        R emit( PARAM_LIST ) const
        {
            Emitter<R,ConstSlotIter> e;
            return e.emit( mSlots.begin(), mSlots.end() PARAM_COMMA PARAM_VALUES );
        }

        size_t getNumSlots() const { return mSlots.size(); }

        R operator()( PARAM_LIST ) const { return emit( PARAM_VALUES ); }

    private:

        virtual void slotDisconnect( const SlotBase & slot ) const
        {
            mSlots.remove_if( EqualSlotClassPtr(&slot) );
        }

        void AddSlot( const SlotDesc & desc ) const
        {
            if( mSlots.end() != FindSlot(desc) ) return;
            mSlots.push_back(desc);
            if( desc.slot ) connectToSlot( *desc.slot );
        }

        ConstSlotIter
        FindSlot( const SlotDesc & desc ) const
        {
            ConstSlotIter i;
            for( i = mSlots.begin(); i != mSlots.end(); ++i )
            {
                if( (*i).func == desc.func ) return i;
            }
            return mSlots.end();
        }
    };
}

#undef PARAM_TEMPLS_0
#undef PARAM_TEMPLS_1
#undef PARAM_TEMPLS_2
#undef PARAM_TEMPLS_3
#undef PARAM_TEMPLS_4
#undef PARAM_TEMPLS_5
#undef PARAM_TEMPLS_6
#undef PARAM_TEMPLS_7
#undef PARAM_TEMPLS_8
#undef PARAM_TEMPLS_9
#undef PARAM_TEMPLS

#undef PARAM_TYPES_0
#undef PARAM_TYPES_1
#undef PARAM_TYPES_2
#undef PARAM_TYPES_3
#undef PARAM_TYPES_4
#undef PARAM_TYPES_5
#undef PARAM_TYPES_6
#undef PARAM_TYPES_7
#undef PARAM_TYPES_8
#undef PARAM_TYPES_9
#undef PARAM_TYPES

#undef PARAM_VALUES_0
#undef PARAM_VALUES_1
#undef PARAM_VALUES_2
#undef PARAM_VALUES_3
#undef PARAM_VALUES_4
#undef PARAM_VALUES_5
#undef PARAM_VALUES_6
#undef PARAM_VALUES_7
#undef PARAM_VALUES_8
#undef PARAM_VALUES_9
#undef PARAM_VALUES

#undef PARAM_LIST_0
#undef PARAM_LIST_1
#undef PARAM_LIST_2
#undef PARAM_LIST_3
#undef PARAM_LIST_4
#undef PARAM_LIST_5
#undef PARAM_LIST_6
#undef PARAM_LIST_7
#undef PARAM_LIST_8
#undef PARAM_LIST_9
#undef PARAM_LIST

#undef PARAM_COMMA_0
#undef PARAM_COMMA_1
#undef PARAM_COMMA_2
#undef PARAM_COMMA_3
#undef PARAM_COMMA_4
#undef PARAM_COMMA_5
#undef PARAM_COMMA_6
#undef PARAM_COMMA_7
#undef PARAM_COMMA_8
#undef PARAM_COMMA_9
#undef PARAM_COMMA

#undef SIGNAL_NAME
#undef FUNCTOR_NAME

#undef GN_SIGSLOT_TEMPL_N

#elif !defined(__GN_BASE_SIGSLOT_H__)
#define __GN_BASE_SIGSLOT_H__ //!< Include protector

#include <list>
#include <algorithm>

namespace GN
{
    class SlotBase;

    namespace detail
    {
        //!
        //! Base signal class
        //!
        class SignalBase
        {
            friend class GN::SlotBase;
            virtual void slotDisconnect( const GN::SlotBase & ) const {}
        protected:
            /** add itself to target slot's signal-array */
            void connectToSlot( const GN::SlotBase & slot ) const;
            /** remove itself from target slot's signal-array */
            void disconnectToSlot( const GN::SlotBase & slot ) const;
        public:
            virtual ~SignalBase() {}
        };
    }

    //!
    //! Base slot class. Derive your class from this, if you want automatic
    //! management of connections between signal and slot.
    //!
    class SlotBase
    {
    protected:

        SlotBase() {}

        virtual ~SlotBase()
        {
            // disconnect with all signals
            for( SignalContainer::iterator i = mSignals.begin(); i != mSignals.end(); ++i )
            {
                (*i)->slotDisconnect( *this );
            }
            mSignals.clear();
        }

    public:

        /** 返回与当前slot连接的信号数 */
        size_t getNumSignals() const { return mSignals.size(); }

    private:
        friend class detail::SignalBase;
        typedef std::list<const detail::SignalBase*> SignalContainer;
        mutable SignalContainer mSignals;
    };

    inline void detail::SignalBase::connectToSlot( const SlotBase & slot ) const
    {
        slot.mSignals.push_back( this );
    }
    inline void detail::SignalBase::disconnectToSlot( const SlotBase & slot ) const
    {
        slot.mSignals.remove( this );
    }
}

//!
//! Signal/Slot parameter count
//!
#define GN_SIGSLOT_TEMPL_N 0
#include "sigslot.h"

#define GN_SIGSLOT_TEMPL_N 1
#include "sigslot.h"

#define GN_SIGSLOT_TEMPL_N 2
#include "sigslot.h"

#define GN_SIGSLOT_TEMPL_N 3
#include "sigslot.h"

#define GN_SIGSLOT_TEMPL_N 4
#include "sigslot.h"

// *****************************************************************************
//                           End of sigslot.h
// *****************************************************************************
#endif // __GN_BASE_SIGSLOT_H__
