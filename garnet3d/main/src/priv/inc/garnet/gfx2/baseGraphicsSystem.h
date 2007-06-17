#ifndef __GN_GFX2_BASEGRAPHICSSYSTEM_H__
#define __GN_GFX2_BASEGRAPHICSSYSTEM_H__
// *****************************************************************************
//! \file    gfx2/baseGraphicsSystem.h
//! \brief   basic graphics system implementation
//! \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

#include "../GNgfx2.h"

namespace GN { namespace gfx2
{
    ///
    /// structure to hold effect parameter value
    ///
    class EffectParameterValueContainer : public EffectParameterValue
    {
        DynaArray<UInt8> mData;

    public:

        ///
        /// ctor
        ///
        EffectParameterValueContainer( const EffectParameterValue & v )
        {
            *this = v;
        }

        ///
        /// assignment
        ///
        EffectParameterValueContainer & operator=( const EffectParameterValue & v )
        {
            ((EffectParameterValue&)*this) = v;

            if( EFFECT_PARAMETER_VALUE_TYPE_STRING == v.type )
            {
                mData.resize( strLen( v.str ) );
                memcpy( mData.cptr(), v.str, mData.size() );
                str = (const char *)mData.cptr();
            }
            else if( EFFECT_PARAMETER_VALUE_TYPE_RAW == v.type )
            {
                mData.resize( v.raw.bytes );
                memcpy( mData.cptr(), v.raw.ptr, mData.size() );
                raw.ptr = mData.cptr();
            }

            return *this;
        }
    };

    ///
    /// base effect class
    ///
    class BaseEffect : public Effect
    {
    public:

        ///
        /// ctor
        ///
        BaseEffect( GraphicsSystem & gs ) : mGraphicsSystem(gs) {}

        ///
        /// get reference to graphics system
        ///
        GraphicsSystem & gs() const { return mGraphicsSystem; }

        /// \name from Effect
        //@{
        virtual void setParameter( const StrA & name, const EffectParameterValue & value ) = 0;
        virtual void unsetParameter( const StrA & name ) = 0;
        //@}

    protected:

        ///
        /// get parameter value
        ///
        const EffectParameterValue * getParameter( const StrA & name ) const
        {
            GN_GUARD_SLOW;

            UInt32 h = mParameters.name2handle( name );

            if( h )
            {
                return &mParameters[h];
            }
            else
            {
                return mGraphicsSystem.getGlobalEffectParameter( name );
            }

            GN_UNGUARD_SLOW;
        }

    private:

        typedef NamedHandleManager<EffectParameterValueContainer,UInt32> EffectParameterManager;

        GraphicsSystem       & mGraphicsSystem;
        EffectParameterManager mParameters;
    };

    ///
    /// D3D9 graphics system
    ///
    class BaseGraphicsSystem : public GraphicsSystem, public StdClass
    {
        GN_DECLARE_STDCLASS( BaseGraphicsSystem, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BaseGraphicsSystem()          { clear(); }
        virtual ~BaseGraphicsSystem() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // from GraphicsSystem
        // ********************************
    public:

        //@{

        virtual void setGlobalEffectParameter( const StrA & name, const EffectParameterValue & value );
        virtual void unsetGlobalEffectParameter( const StrA & name );
        virtual const EffectParameterValue * getGlobalEffectParameter( const StrA & name );
        virtual void registerEffect( const StrA & name, const EffectFactory & );
        virtual Effect * getEffect( const StrA & name );
        virtual void deleteEffect( const StrA & name );
        virtual void deleteAllEffects();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct EffectItem
        {
            EffectFactory factory;
            Effect *      instance;
        };

        NamedHandleManager<EffectParameterValueContainer,UInt32> mGlobalEffectParameters;
        NamedHandleManager<EffectItem,UInt32>                    mEffects;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of baseGraphicsSystem.h
// *****************************************************************************
#endif // __GN_GFX2_BASEGRAPHICSSYSTEM_H__
