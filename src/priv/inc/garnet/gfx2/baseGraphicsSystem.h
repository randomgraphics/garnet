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
    class BaseKernel;
    class BaseGraphicsSystem;

    ///
    /// structure to hold kernel parameter value
    ///
    class BaseKernelParameter : public KernelParameter
    {
        KernelParameterDesc   mDesc;
        DynaArray<UInt8*>     mData;
        DynaArray<StrA>       mStr;
        bool                  mEmpty;

        const void * getData() const { return empty() ? 0 : mData.cptr(); }
        const StrA * getString() const { return empty() ? 0 : mStr.cptr(); }

    public:

        ///
        /// triggered right after the value is set
        ///
        Signal2< void, size_t /*offset*/, size_t /*count*/ > sigValueSet;

        ///
        /// triggered right after the value is unset
        ///
        Signal0< void > sigValueUnset;

        ///
        /// ctor
        ///
        BaseKernelParameter( const KernelParameterDesc & desc )
            : mDesc( desc )
            , mEmpty( true )
        {
            if( KERNEL_PARAMETER_TYPE_STRING == mDesc.type )
            {
                mStr.resize( mDesc.count );
            }
            else
            {
                mData.resize( mDesc.count );
            }
        }

        ///
        /// check if parameter has valid value
        ///
        bool empty() const { return mEmpty; }

        /// \name get parameter value
        //@{
        const bool         * toBool()   const { GN_ASSERT( KERNEL_PARAMETER_TYPE_BOOL == mDesc.type );   return (const bool*)getData(); }
        const int          * toInt()    const { GN_ASSERT( KERNEL_PARAMETER_TYPE_INT == mDesc.type );    return (const int*)getData(); }
        const unsigned int * toUInt()   const { GN_ASSERT( KERNEL_PARAMETER_TYPE_INT == mDesc.type );    return (const unsigned int*)getData(); }
        const float        * toFloat()  const { GN_ASSERT( KERNEL_PARAMETER_TYPE_FLOAT == mDesc.type );  return (const float*)getData(); }
        const StrA         * toString() const { GN_ASSERT( KERNEL_PARAMETER_TYPE_STRING == mDesc.type ); return getString(); }
        //@}

        // from parent class
        //@{

        virtual const KernelParameterDesc & getDesc() const { return mDesc; }
        virtual void                        setb( size_t offset, size_t count, const bool         * values );
        virtual void                        seti( size_t offset, size_t count, const int          * values );
        virtual void                        setf( size_t offset, size_t count, const float        * values );
        virtual void                        sets( size_t offset, size_t count, const char * const * values );
        virtual void                        unset() { mEmpty = true; }
        //@}
    };

    ///
    /// base kernel parameter set
    ///    
    class BaseKernelParameterSet : public KernelParameterSet, public StdClass
    {
        GN_DECLARE_STDCLASS( BaseKernelParameterSet, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BaseKernelParameterSet( BaseKernel & e );
        virtual ~BaseKernelParameterSet() { quit(); }
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
        // public functions
        // ********************************
    public:

        /// \name from parent class
        //@{
        virtual KernelParameter * getParameter( KernelParameterHandle handle ) const;
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        DynaArray<BaseKernelParameter*> mParameters;

        // ********************************
        // private functions
        // ********************************
    private:

        ///
        /// override this method to create your own parameter instance
        ///
        virtual BaseKernelParameter * createParameter( const KernelParameterDesc & desc ) const
        {
            return new BaseKernelParameter( desc );
        }
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
        /// get parameter descriptor by index
        ///
        const KernelParameterDesc & getParameterDescByIndex( size_t index ) const
        {
            GN_ASSERT( index < mParameters.size() );
            return mParameters[index].desc;
        }

        /// \name from Kernel
        //@{
        virtual const KernelParameterDesc * getParameterDesc( const StrA & name ) const;
        virtual KernelParameterHandle       getParameterHandle( const StrA & name ) const;
        virtual KernelParameterSet        * createParameterSet();
        //@}

    protected:

        ///
        /// \note add parameter. Normallly called in constructor
        ///
        //KernelParameterHandle addParameter( const StrA & name, const KernelParameterDesc & param );
        KernelParameterHandle addParameter(
            const StrA        & name,
            KernelParameterType type,
            size_t              count );

    private:

        struct ParameterItem
        {
            KernelParameterDesc desc;
        };

        GraphicsSystem                                  & mGraphicsSystem;
        DynaArray<ParameterItem>                          mParameters;
        NamedHandleManager<size_t,KernelParameterHandle>  mParameterHandles; ///< convert handle and name to array index.
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
        virtual KernelParameterHandle createGlobalKernelParameter( const StrA & name, const KernelParameterDesc & desc );
        virtual KernelParameterHandle getGlobalKernelParameterHandle( const StrA & name ) const;
        virtual KernelParameter     * getGlobalKernelParameter( KernelParameterHandle handle ) const;

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

        NamedHandleManager<BaseKernelParameter*,KernelParameterHandle> mGlobalKernelParameters;
        NamedHandleManager<KernelItem,UInt32>                          mKernels;

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
