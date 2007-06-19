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
    class EffectParameterContainer : public EffectParameter
    {
        DynaArray<UInt8> mData;

    public:

        ///
        /// ctor
        ///
        EffectParameterContainer( const EffectParameter & v )
        {
            *this = v;
        }

        ///
        /// assignment
        ///
        EffectParameterContainer & operator=( const EffectParameter & v )
        {
            ((EffectParameter&)*this) = v;

            if( EFFECT_PARAMETER_TYPE_STRING == v.type )
            {
                mData.resize( strLen( v.str ) );
                memcpy( mData.cptr(), v.str, mData.size() );
                str = (const char *)mData.cptr();
            }
            else if( EFFECT_PARAMETER_TYPE_RAW == v.type )
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
        virtual void setParameter( const StrA & name, const EffectParameter & value );
        virtual void unsetParameter( const StrA & name );
        //@}

    protected:

        ///
        /// get parameter value
        ///
        const EffectParameter * getParameter( const StrA & name ) const
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

        typedef NamedHandleManager<EffectParameterContainer,UInt32> EffectParameterManager;

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

        virtual void setGlobalEffectParameter( const StrA & name, const EffectParameter & value );
        virtual void unsetGlobalEffectParameter( const StrA & name );
        virtual const EffectParameter * getGlobalEffectParameter( const StrA & name );
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

        NamedHandleManager<EffectParameterContainer,UInt32> mGlobalEffectParameters;
        NamedHandleManager<EffectItem,UInt32>               mEffects;

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
