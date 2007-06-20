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
    class EffectParameterContainer
    {
        EffectParameter  mParam;
        DynaArray<UInt8> mData;

    public:

        ///
        /// ctor
        ///
        EffectParameterContainer()
        {
            unset();
        }

        ///
        /// check if parameter has valid value
        ///
        bool empty() const { return EFFECT_PARAMETER_TYPE_UNKNOWN == mParam.type; }

        ///
        /// get parameter
        ///
        const EffectParameter & get() const { GN_ASSERT(!empty()); return mParam; }

        ///
        /// set parameter
        ///
        void set( const EffectParameter & v )
        {
            mParam = v;

            if( EFFECT_PARAMETER_TYPE_STRING == v.type )
            {
                mData.resize( strLen( v.str ) );
                memcpy( mData.cptr(), v.str, mData.size() );
                mParam.str = (const char *)mData.cptr();
            }
            else if( EFFECT_PARAMETER_TYPE_RAW == v.type )
            {
                mData.resize( v.raw.bytes );
                memcpy( mData.cptr(), v.raw.ptr, mData.size() );
                mParam.raw.ptr = mData.cptr();
            }
        }

        ///
        /// clear to empty
        ///
        void unset() { mParam.type = EFFECT_PARAMETER_TYPE_UNKNOWN; }
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
        virtual EffectParameterHandle getParameterHandle( const StrA & name ) const;
        virtual void                  setParameter( EffectParameterHandle handle, const EffectParameter & value );
        virtual void                  unsetParameter( EffectParameterHandle handle );
        //@}

    protected:

        ///
        /// get parameter value
        ///
        const EffectParameter * getParameter( EffectParameterHandle handle ) const;

    private:

        struct ParameterItem
        {
            EffectParameterContainer param;
            EffectParameterHandle    global;
        };

        GraphicsSystem                                        & mGraphicsSystem;
        NamedHandleManager<ParameterItem,EffectParameterHandle> mParameters;
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

        virtual EffectParameterHandle   getGlobalEffectParameterHandle( const StrA & name );
        virtual void                    setGlobalEffectParameter( EffectParameterHandle handle, const EffectParameter & value );
        virtual void                    unsetGlobalEffectParameter( EffectParameterHandle handle );
        virtual const EffectParameter * getGlobalEffectParameter( EffectParameterHandle handle );

        virtual void     registerEffect( const StrA & name, const EffectFactory & );
        virtual Effect * getEffect( const StrA & name );
        virtual void     deleteEffect( const StrA & name );
        virtual void     deleteAllEffects();

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

        NamedHandleManager<EffectParameterContainer,EffectParameterHandle> mGlobalEffectParameters;
        NamedHandleManager<EffectItem,UInt32>                              mEffects;

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
