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

        NamedHandleManager<EffectParameterValue,UInt32> mGlobalEffectParameters;
        NamedHandleManager<EffectItem,UInt32>           mEffects;
        

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
