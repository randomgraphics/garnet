#ifndef __GN_GFX2_BASEGRAPHICSSYSTEM_H__
#define __GN_GFX2_BASEGRAPHICSSYSTEM_H__
// *****************************************************************************
/// \file
/// \brief   basic graphics system implementation
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

#include "../GNgfx2.h"

namespace GN { namespace gfx
{
    ///
    /// structure to hold kernel parameter value
    ///
    class KernelParameterWrapper
    {
        KernelParameter  mParam;
        DynaArray<UInt8> mData;

    public:

        ///
        /// ctor
        ///
        KernelParameterWrapper()
        {
            unset();
        }

        ///
        /// check if parameter has valid value
        ///
        bool empty() const { return KERNEL_PARAMETER_TYPE_UNKNOWN == mParam.type; }

        ///
        /// get parameter
        ///
        const KernelParameter & get() const { GN_ASSERT(!empty()); return mParam; }

        ///
        /// set parameter
        ///
        void set( const KernelParameter & v )
        {
            mParam = v;

            if( KERNEL_PARAMETER_TYPE_STRING == v.type )
            {
                mData.resize( strLen( v.str ) );
                memcpy( mData.cptr(), v.str, mData.size() );
                mParam.str = (const char *)mData.cptr();
            }
            else if( KERNEL_PARAMETER_TYPE_RAW == v.type )
            {
                mData.resize( v.raw.bytes );
                memcpy( mData.cptr(), v.raw.ptr, mData.size() );
                mParam.raw.ptr = mData.cptr();
            }
        }

        ///
        /// set raw data
        ///
        void setRaw( size_t offset, size_t bytes, const void * data )
        {
            mParam.type = KERNEL_PARAMETER_TYPE_RAW;
            if( mData.size() < (offset+bytes) )
            {
                mData.resize( offset + bytes );
            }
            memcpy( mData.cptr() + offset, data, bytes );
            mParam.raw.ptr   = mData.cptr();
            mParam.raw.bytes = mData.size();
        }

        ///
        /// clear to empty
        ///
        void unset() { mParam.type = KERNEL_PARAMETER_TYPE_UNKNOWN; }
    };

    ///
    /// base kernel parameter set
    ///
    class BaseKernelParameterSet : public KernelParameterSet
    {
        DynaArray<KernelParameterWrapper> mParameters;

    public:

        ///
        /// ctor
        ///
        BaseKernelParameterSet( Kernel & e, size_t count )
            : KernelParameterSet( e )
            , mParameters( count )
        {
        }

        /// \name from parent class
        //@{
        virtual const KernelParameter * getParameter( KernelParameterHandle handle ) const;
        virtual void                    setParameter( KernelParameterHandle handle, const KernelParameter & value );
        virtual void                    setRawParameter( KernelParameterHandle handle, size_t offset, size_t bytes, const void * data );
        virtual void                    unsetParameter( KernelParameterHandle handle );
        //@}
    };

    ///
    /// base kernel class
    ///
    class BaseKernel : public Kernel
    {
    public:

        ///
        /// ctor
        ///
        BaseKernel( GraphicsSystem & gs ) : mGraphicsSystem(gs) {}

        ///
        /// get reference to graphics system
        ///
        GraphicsSystem & gs() const { return mGraphicsSystem; }

        ///
        /// get number of parameters
        ///
        size_t getParameterCount() const
        {
            return mParameters.size();
        }

        ///
        /// convert parameter handle to parameter index.
        ///
        bool getParameterIndex( size_t & result, KernelParameterHandle handle ) const
        {
            if( !mParameterHandles.validHandle( handle ) )
            {
                GN_ERROR(getLogger("GN.gfx2.base.BaseKernel"))( "invalid arameter handle: %d", handle );
                return false;
            }
            result = mParameterHandles[handle];
            GN_ASSERT( result < mParameters.size() );
            return true;
        }

        ///
        /// get global value of the parameter
        ///
        const KernelParameter * getGlobalParameterByIndex( size_t index ) const
        {
            GN_ASSERT( index < mParameters.size() );
            return mGraphicsSystem.getGlobalKernelParameter( mParameters[index].global );
        }

        /// \name from Kernel
        //@{
        virtual const KernelParameterDesc * getParameterDesc( const StrA & name ) const;
        virtual KernelParameterHandle       getParameterHandle( const StrA & name ) const;
        virtual KernelParameterSet        * createParameterSet();
        virtual bool                        hasProperity( const StrA & name ) const;
        virtual const KernelProperty      * getProperity( const StrA & name ) const;
        //@}

    protected:

        ///
        /// \note add parameter. Normallly called in constructor
        ///
        KernelParameterHandle addParameter( const StrA & name, const KernelParameterDesc & param );

        /// \name property management
        void setProperty( const StrA & name, const KernelProperty & property );
        void unsetProperty( const StrA & name );
        //@}

    private:

        struct ParameterItem
        {
            KernelParameterDesc    desc;
            KernelParameterHandle  global;
        };

        GraphicsSystem                                  & mGraphicsSystem;
        DynaArray<ParameterItem>                          mParameters;
        NamedHandleManager<size_t,KernelParameterHandle>  mParameterHandles; ///< convert handle and name to array index.
        NamedHandleManager<KernelParameterWrapper,UInt32> mProperties;
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

        virtual KernelParameterHandle   getGlobalKernelParameterHandle( const StrA & name );
        virtual void                    setGlobalKernelParameter( KernelParameterHandle handle, const KernelParameter & value );
        virtual void                    unsetGlobalKernelParameter( KernelParameterHandle handle );
        virtual const KernelParameter * getGlobalKernelParameter( KernelParameterHandle handle );

        virtual void     registerKernel( const StrA & name, const KernelFactory & );
        virtual Kernel * getKernel( const StrA & name );
        virtual void     deleteKernel( const StrA & name );
        virtual void     deleteAllKernels();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct KernelItem
        {
            KernelFactory factory;
            Kernel *      instance;
        };

        NamedHandleManager<KernelParameterWrapper,KernelParameterHandle> mGlobalKernelParameters;
        NamedHandleManager<KernelItem,UInt32>                            mKernels;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_BASEGRAPHICSSYSTEM_H__
