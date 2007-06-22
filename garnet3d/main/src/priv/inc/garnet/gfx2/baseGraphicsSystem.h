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
    class EffectParameterWrapper
    {
        EffectParameter  mParam;
        DynaArray<UInt8> mData;

    public:

        ///
        /// ctor
        ///
        EffectParameterWrapper()
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
    /// base effect parameter set
    ///
    class BaseEffectParameterSet : public EffectParameterSet
    {
        DynaArray<EffectParameterWrapper> mParameters;

    public:

        ///
        /// ctor
        ///
        BaseEffectParameterSet( Effect & e, size_t count )
            : EffectParameterSet( e )
            , mParameters( count )
        {
        }

        /// \name from parent class
        //@{
        virtual const EffectParameter * getParameter( EffectParameterHandle handle ) const;
        virtual void                    setParameter( EffectParameterHandle handle, const EffectParameter & value );
        virtual void                    unsetParameter( EffectParameterHandle handle );
        //@}
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

        ///
        /// convert parameter handle to parameter index.
        ///
        bool getParameterIndex( size_t & result, EffectParameterHandle handle ) const
        {
            if( !mParameterHandles.validHandle( handle ) )
            {
                GN_ERROR(getLogger("GN.gfx2.base.BaseEffect"))( "invalid arameter handle: %d", handle );
                return false;
            }
            result = mParameterHandles[handle];
            GN_ASSERT( result < mParameters.size() );
            return true;
        }

        ///
        /// get global value of the parameter
        ///
        const EffectParameter * getGlobalParameterByIndex( size_t index ) const
        {
            GN_ASSERT( index < mParameters.size() );
            return mGraphicsSystem.getGlobalEffectParameter( mParameters[index].global );
        }

        /// \name from Effect
        //@{
        virtual const EffectParameterDesc * getParameterDesc( const StrA & name ) const;
        virtual EffectParameterHandle       getParameterHandle( const StrA & name ) const;
        virtual EffectParameterSet        * createParameterSet();
        virtual const EffectProperty      * getProperity( const StrA & ) const
        {
            GN_ERROR(getLogger("GN.gfx2.base.BaseEffect"))( "effect has no property." );
            return 0;
        }
        //@}

    protected:

        ///
        /// \note add parameter. Normallly called in constructor
        ///
        EffectParameterHandle addParameter( const StrA & name, const EffectParameterDesc & param );

    private:

        struct ParameterItem
        {
            EffectParameterDesc    desc;
            EffectParameterHandle  global;
        };

        GraphicsSystem                                 & mGraphicsSystem;
        DynaArray<ParameterItem>                         mParameters;
        NamedHandleManager<size_t,EffectParameterHandle> mParameterHandles; ///< convert handle and name to array index.
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

        NamedHandleManager<EffectParameterWrapper,EffectParameterHandle> mGlobalEffectParameters;
        NamedHandleManager<EffectItem,UInt32>                            mEffects;

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
