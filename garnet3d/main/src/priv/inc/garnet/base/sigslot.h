// *****************************************************************************
//! \file    sigslot.h
//! \brief   signal slot implementation
//! \author  chenlee (2005.5.6)
// *****************************************************************************
#ifdef GN_SIGSLOT_TEMPL_N

#ifndef GN_JOIN
#define GN_JOIN(s1, s2)               GN_CONCATNATE_DIRECT(s1, s2)
#define GN_JOIN3(s1, s2, s3)          GN_JOIN( GN_JOIN( s1, s2 ), s3 )
#define GN_JOIN4(s1, s2, s3, s4)      GN_JOIN( GN_JOIN3( s1, s2, s3 ), s4 )
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
#define DELEGATE_NAME    GN_JOIN(Delegate,GN_SIGSLOT_TEMPL_N)

namespace GN
{
    //!
    //! template signal class
    //!
    template<typename R PARAM_COMMA PARAM_TEMPLS>
    class SIGNAL_NAME : public detail::SignalBase
    {
        typedef DELEGATE_NAME<R PARAM_COMMA PARAM_TYPES>  FunctorType;

        struct SlotDesc
        {
            FunctorType      func;
            const void *     classPtr;
            const SlotBase * basePtr;
        };

        typedef typename ::std::list<SlotDesc>         SlotContainer;
        typedef typename SlotContainer::iterator       SlotIter;
        typedef typename SlotContainer::const_iterator ConstSlotIter;

        template<class RetType,class ContainerType>
        struct Emitter
        {
            RetType emit( const ContainerType & slots PARAM_COMMA PARAM_LIST )
            {
                RetType last;
                typename ContainerType::const_iterator i = slots.begin();
                while( i != slots.end() )
                {
                    last = (*i).func(PARAM_VALUES);
                    ++i;
                }
                return last;
            }
        };

        template<class ContainerType>
        struct Emitter<bool,ContainerType>
        {
            bool emit( const ContainerType & slots PARAM_COMMA PARAM_LIST )
            {
                typename ContainerType::const_iterator i = slots.begin();
                while( i != slots.end() )
                {
                    if( !(*i).func(PARAM_VALUES) ) return false;
                    ++i;
                }
                return true;
            }
        };

        template<class ContainerType>
        struct Emitter<void,ContainerType>
        {
            void emit( const ContainerType & slots PARAM_COMMA PARAM_LIST )
            {
                typename ContainerType::const_iterator i = slots.begin();
                while( i != slots.end() )
                {
                    (*i).func(PARAM_VALUES);
                    ++i;
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
                if( (*i).basePtr ) disconnectFromSlotClass( *(*i).basePtr );
            }
            mSlots.clear();
        }

        void connect( R (*staticFuncPtr)(PARAM_TYPES) ) const
        {
            if( 0 == staticFuncPtr ) return;
            SlotDesc desc;
            desc.func.bind(staticFuncPtr);
            desc.classPtr = 0;
            desc.basePtr = 0;
            addSlotItem( desc );
        }

        template<class X, class Y>
        inline void connect( Y * classPtr, R (X::*memFuncPtr)(PARAM_TYPES) ) const
        {
            GN_ASSERT( !IsConst<Y>::value ); // Y can't be const class
            if( 0 == classPtr || 0 == memFuncPtr ) { GN_ERROR( "Can't connect to NULL method pointer!" ); return; }
            SlotDesc desc;
            desc.func.bind( classPtr, memFuncPtr );
            desc.classPtr = classPtr;
            desc.basePtr = IsBaseAndDerived<SlotBase,Y>::value ? (const SlotBase*)classPtr : 0;
            addSlotItem( desc );
        }

        template<class X, class Y>
        inline void connect( const Y * classPtr, R (X::*memFuncPtr)(PARAM_TYPES) const ) const
        {
            if( 0 == classPtr || 0 == memFuncPtr ) { GN_ERROR( "Can't connect to NULL method pointer!" ); return; }
            SlotDesc desc;
            desc.func.bind( classPtr, memFuncPtr );
            desc.classPtr = classPtr;
            desc.basePtr = IsBaseAndDerived<SlotBase,Y>::value ? (const SlotBase*)classPtr : 0;
            addSlotItem( desc );
        }

        void disconnect( R (*staticFuncPtr)(PARAM_TYPES) ) const
        {
            if( 0 == staticFuncPtr ) return;
            SlotDesc desc;
            desc.func.bind(staticFuncPtr);
            desc.classPtr = 0;
            desc.basePtr = 0;
            SlotIter i = findSlotItem(desc);
            if( i != mSlots.end() ) mSlots.erase(i);
        }

        template<class X>
        void disconnect( const X * slot ) const
        {
            if( 0 == slot ) return;

            // remove the class from private slot list that has same class ptr
            typename SlotContainer::iterator i, t, e = mSlots.end();
            for( i = mSlots.begin(); i != e; )
            {
                t = i; ++i;
                if( slot == t->classPtr )
                    mSlots.erase(t);
            }

            if( IsBaseAndDerived<SlotBase,X>::value )
            {
                // remove itself from target slot's singal array.
                disconnectFromSlotClass( (const SlotBase &)*slot );
            }
        }

        R emit( PARAM_LIST ) const
        {
            Emitter<R,SlotContainer> e;
            return e.emit( mSlots PARAM_COMMA PARAM_VALUES );
        }

        size_t getNumSlots() const { return mSlots.size(); }

        R operator()( PARAM_LIST ) const { return emit( PARAM_VALUES ); }

    private:

        virtual void removeBaseSlotClass( const SlotBase & base ) const
        {
            // Remove slots that has same special base class
            typename SlotContainer::iterator i, t, e = mSlots.end();
            for( i = mSlots.begin(); i != e; )
            {
                t = i; ++i;
                if( &base == t->basePtr )
                    mSlots.erase(t);
            }
        }

        void addSlotItem( const SlotDesc & desc ) const
        {
            if( mSlots.end() != findSlotItem(desc) ) return;
            mSlots.push_back(desc);
            if( desc.basePtr ) connectToSlotClass( *desc.basePtr );
        }

        SlotIter
        findSlotItem( const SlotDesc & desc ) const
        {
            SlotIter i;
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
#undef DELEGATE_NAME

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
            /** remove slot from signal's private slot list */
            virtual void removeBaseSlotClass( const SlotBase & ) const {}
        protected:
            /** add itself to target slot's signal list */
            void connectToSlotClass( const GN::SlotBase & slot ) const;
            /** remove itself from target slot's signal list */
            void disconnectFromSlotClass( const GN::SlotBase & slot ) const;
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
                (*i)->removeBaseSlotClass( *this );
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

    inline void detail::SignalBase::connectToSlotClass( const SlotBase & slot ) const
    {
        slot.mSignals.push_back( this );
    }
    inline void detail::SignalBase::disconnectFromSlotClass( const SlotBase & slot ) const
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
