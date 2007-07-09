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
    /// array of items with unique name
    ///
    template<typename T>
    class NamedArray
    {
        struct Item
        {
            StrA   name;
            size_t index;
            T      value;
        };

        StrA                  mTypeName;
        DynaArray<Item>       mItems;
        std::map<StrA,size_t> mNames;

        const T * getByIndex( size_t index ) const
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s index is out of range.", mTypeName.cptr() );
                return 0;
            }
            return &mItems[index].value;
        }

        T * getByIndex( size_t index )
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s index is out of range.", mTypeName.cptr() );
                return 0;
            }
            return &mItems[index].value;
        }

        const T * getByName( const StrA & name ) const
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "invalid %s name '%s'.", mTypeName.cptr(), name.cptr() );
                return 0;
            }
            return &mItems[i->second].value;
        }

        T * getByName( const StrA & name )
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "invalid %s name '%s'.", mTypeName.cptr(), name.cptr() );
                return 0;
            }
            return &mItems[i->second].value;
        }

        const StrA & getNameByIndex( size_t index ) const
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s index is out of range.", mTypeName.cptr() );
                return StrA::EMPTYSTR;
            }
            return mItems[index].name;
        }

        size_t getIndexByName( const StrA & name ) const
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "invalid %s name '%s'.", mTypeName.cptr(), name.cptr() );
                return (size_t)-1;
            }
            return i->second;
        }

        size_t doAdd( const StrA & name, const T & value )
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() != i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s named '%s' does exist already.", mTypeName.cptr() );
                return (size_t)-1;
            }

            Item item;
            item.name  = name;
            item.index = mItems.size();
            item.value = value;

            mNames[name] = mItems.size();
            mItems.append( item );

            return item.index;
        }

    public:

        //@{

        NamedArray( const char * typeName = "item" ) : mTypeName(typeName)
        {
        }

        ~NamedArray()
        {
        }

        bool         empty() const { return mItems.empty(); }
        size_t       size() const { GN_ASSERT( mItems.size() == mNames.size() ); return mItems.size(); }
        const T    * get( size_t index ) const { return getByIndex( index ); }
        const T    * get( const StrA & name ) const { return getByName( name ); }
        const StrA & getName( size_t index ) const { return getNameByIndex( index ); }
        size_t       getIndex( const StrA & name ) const { return getIndexByName( name ); }
        size_t       add( const StrA & name, const T & value ) { return doAdd( name, value ); }

        ///
        /// this function does not allow invalid index
        ///
        const T    & at( size_t index ) const { GN_ASSERT( index < size() ); return mItems[index].value; }

        ///
        /// this function does not allow invalid name
        ///
        const T    & at( const StrA & name ) const { std::map<StrA,size_t>::const_iterator i = mNames.find(name); GN_ASSERT( mNames.end() != i ); return mItems[i->second].value; }

        //@}
    };

    ///
    /// structure to hold kernel parameter value
    ///
    class BaseKernelParameter : public KernelParameter
    {
        const KernelParameterDesc mDesc;
        const size_t              mIndex;
        DynaArray<UInt8*>         mData;
        DynaArray<StrA>           mStr;
        bool                      mEmpty;

        const void * getData() const { return empty() ? 0 : mData.cptr(); }
        const StrA * getString() const { return empty() ? 0 : mStr.cptr(); }

    public:

        ///
        /// triggered right after the value is set
        ///
        Signal3< void, size_t /*index*/, size_t /*offset*/, size_t /*count*/ > sigValueSet;

        ///
        /// triggered right after the value is unset
        ///
        Signal1< void, size_t /*index*/ > sigValueUnset;

        ///
        /// ctor
        ///
        BaseKernelParameter( const KernelParameterDesc & desc, size_t index )
            : mDesc( desc )
            , mIndex( index )
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
        virtual void                        unset();
        //@}
    };

    ///
    /// base kernel parameter set
    ///    
    class BaseKernelParameterSet : public KernelParameterSet
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BaseKernelParameterSet( BaseKernel & e );
        virtual ~BaseKernelParameterSet();
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// \name from parent class
        //@{
        virtual KernelParameter * getParameter( size_t index ) const;
        //@}

        ///
        /// get parameter instance by name
        ///
        BaseKernelParameter * getBaseParameterByIndex( size_t index ) const
        {
            return GN_SAFE_CAST<BaseKernelParameter*>( getParameter( index ) );
        }

        ///
        /// get parameter instance by name
        ///
        BaseKernelParameter * getBaseParameterByName( const StrA & name ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        DynaArray<BaseKernelParameter*> mParameters;

        // ********************************
        // private functions
        // ********************************
    private:
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
        BaseKernel( GraphicsSystem & gs )
            : mGraphicsSystem(gs)
            , mParameters( "PARAMETER" )
            , mStreams( "STREAM" )
        {}

        ///
        /// get reference to graphics system
        ///
        GraphicsSystem & gs() const { return mGraphicsSystem; }

        /// \name from Kernel
        //@{

        virtual size_t               getNumStreams() const { return mStreams.size(); }
        virtual const StrA         & getStreamName( size_t index ) const { return mStreams.getName( index ); }
        virtual size_t               getStreamIndex( const StrA & name ) const { return mStreams.getIndex( name ); }
        virtual StreamSource       * getStream( size_t index ) const;
        virtual StreamSource       * getStream( const StrA & name ) const;

        virtual size_t                      getNumParameters() const { return mParameters.size(); }
        virtual const StrA                & getParameterName( size_t index ) const { return mParameters.getName( index ); }
        virtual size_t                      getParameterIndex( const StrA & name ) const { return mParameters.getIndex( name ); }
        virtual const KernelParameterDesc * getParameterDesc( size_t index ) const { return mParameters.get( index ); }
        virtual const KernelParameterDesc * getParameterDesc( const StrA & name ) const { return mParameters.get( name ); }
        virtual KernelParameterSet        * createParameterSet();
        //@}

    protected:

        ///
        /// \return index of the stream. -1 means failed.
        ///
        /// \note Kernel class does _NOT_ hold the ownership of the stream instance.
        ///
        size_t addStreamRef( const StrA & name, StreamSource * stream );

        ///
        /// \note add parameter. Return index of newly inserted index. Return -1, if failed.
        ///
        size_t addParameter(
            const StrA        & name,
            KernelParameterType type,
            size_t              count );

    private:

        GraphicsSystem                & mGraphicsSystem;
        NamedArray<KernelParameterDesc> mParameters;
        NamedArray<StreamSource*>       mStreams;
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

        virtual void     registerKernel( const StrA & name, KernelCreator creator );
        virtual Kernel * getKernel( const StrA & name );
        virtual void     unregisterKernel( const StrA & name );
        virtual void     unregisterAllKernels();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct KernelItem
        {
            KernelCreator creator;
            Kernel *      instance;
        };

        NamedHandleManager<KernelItem,UInt32> mKernels;

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
