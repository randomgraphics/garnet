#ifndef __GN_GFX2_GFX2BASE_H__
#define __GN_GFX2_GFX2BASE_H__
// *****************************************************************************
/// \file
/// \brief   basic graphics system implementation
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

#include "GNgfx2.h"

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

        StrA                  mLogPrefix;
        DynaArray<Item>       mItems;
        std::map<StrA,size_t> mNames;

        const T * getByIndex( size_t index ) const
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s: index is out of range.", mLogPrefix.cptr() );
                return 0;
            }
            return &mItems[index].value;
        }

        T * getByIndex( size_t index )
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s: index is out of range.", mLogPrefix.cptr() );
                return 0;
            }
            return &mItems[index].value;
        }

        const T * getByName( const StrA & name ) const
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s: invalid name '%s'.", mLogPrefix.cptr(), name.cptr() );
                return 0;
            }
            return &mItems[i->second].value;
        }

        T * getByName( const StrA & name )
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s: invalid name '%s'.", mLogPrefix.cptr(), name.cptr() );
                return 0;
            }
            return &mItems[i->second].value;
        }

        const StrA & getNameByIndex( size_t index ) const
        {
            if( index >= mItems.size() )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s: index is out of range.", mLogPrefix.cptr() );
                return StrA::EMPTYSTR;
            }
            return mItems[index].name;
        }

        size_t getIndexByName( const StrA & name ) const
        {
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s: invalid name '%s'.", mLogPrefix.cptr(), name.cptr() );
                return (size_t)-1;
            }
            return i->second;
        }

        size_t doAdd( const StrA & name, const T & value )
        {
            GN_ASSERT( !name.empty() );
            std::map<StrA,size_t>::const_iterator i = mNames.find( name );
            if( mNames.end() != i )
            {
                GN_ERROR(getLogger("GN.gfx2.NamedArray"))( "%s: name '%s' does exist already.", mLogPrefix.cptr(), name.cptr() );
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

        NamedArray( const StrA & logPrefix ) : mLogPrefix(logPrefix)
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
    /// dummy kernel parameter
    ///
    class DummyKernelParameter : public GN::gfx::KernelParameter
    {
        KernelParameterDesc mDesc;

        DummyKernelParameter()
        {
            mDesc.name  = "DUMMY";
            mDesc.type  = KERNEL_PARAMETER_TYPE_BOOL;
            mDesc.count = 0;
        }

    public:

        //@{

        static DummyKernelParameter & sGetInstance() { static DummyKernelParameter sInstance; return sInstance; }

        virtual const KernelParameterDesc & getDesc() const { return mDesc; }
        virtual void setb( size_t, size_t, const bool         * ) {}
        virtual void seti( size_t, size_t, const int          * ) {}
        virtual void setf( size_t, size_t, const float        * ) {}
        virtual void sets( size_t, size_t, const char * const * ) {}
        virtual void unset() {}

        //@}
    };

    ///
    /// simple kernel parameter that contains single typed value.
    ///
    template<typename T>
    class SimpleKernelParameter : public KernelParameter
    {
        KernelParameterDesc mDesc;

    public:

        //@{

        T value;

        operator const T & () const { return value; }

        SimpleKernelParameter( const KernelParameterDesc & desc, const T & initial ) : value(initial) { mDesc.name = desc.name; mDesc.type = desc.type; mDesc.count = desc.count; }
        const KernelParameterDesc & getDesc() const { return mDesc; }
        virtual void                setb( size_t, size_t, const bool         * ) { GN_UNEXPECTED(); }
        virtual void                seti( size_t, size_t, const int          * ) { GN_UNEXPECTED(); }
        virtual void                setf( size_t, size_t, const float        * ) { GN_UNEXPECTED(); }
        virtual void                sets( size_t, size_t, const char * const * ) { GN_UNEXPECTED(); }
        virtual void                unset() {}

        //@}
    };

    ///
    /// kernel parameter that represents boolean value.
    ///
    struct BoolKernelParameter : SimpleKernelParameter<bool>
    {
        //@{

        BoolKernelParameter( const KernelParameterDesc & desc, bool initial )
            : SimpleKernelParameter( desc, initial ) {}

        void setb( size_t offset, size_t count, const bool * values )
        {
            if( 0 != offset || 1 != count || NULL == values )
            {
                GN_ERROR(getLogger("GN.gfx2.BoolKernelParameter"))( "invalid parameter value." );
                return;
            }

            value = *values;
        }

        //@}
    };

    ///
    /// kernel parameter that represents 32bit integer.
    ///
    template<typename T>
    struct IntKernelParameter : SimpleKernelParameter<T>
    {
        //@{

        IntKernelParameter( const KernelParameterDesc & desc, const T & initial )
            : SimpleKernelParameter( desc, initial ) {}

        void seti( size_t offset, size_t count, const int * values )
        {
            if( 0 != offset || 1 != count || NULL == values )
            {
                GN_ERROR(getLogger("GN.gfx2.IntKernelParameter"))( "invalid parameter value." );
                return;
            }

            value = *values;
        }

        //@}
    };

    ///
    /// kernel parameter that represents 32bit float.
    ///
    struct FloatKernelParameter : SimpleKernelParameter<float>
    {
        //@{

        FloatKernelParameter( const KernelParameterDesc & desc, float initial )
            : SimpleKernelParameter( desc, initial ) {}

        void setf( size_t offset, size_t count, const float * values )
        {
            if( 0 != offset || 1 != count || NULL == values )
            {
                GN_ERROR(getLogger("GN.gfx2.FloatKernelParameter"))( "invalid parameter value." );
                return;
            }

            value = *values;
        }

        //@}
    };

    ///
    /// structure to hold kernel parameter value
    ///
    class BaseKernelParameter : public KernelParameter
    {
        const KernelParameterDesc mDesc;
        const size_t              mIndex; ///< index into kenel parameter array
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

        ///
        /// get untyped parameter by index
        ///
        virtual KernelParameter & get( size_t index );

        ///
        /// get typed parameter by index
        ///
        template<typename T>
        T & getT( size_t index ) const { return safeCastRef<T>( get( index ) ); }

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

    class BaseKernelPort : public NoCopy
    {
    public:

        ///
        /// get port descriptor
        ///
        virtual const KernelPortDesc & getDesc() const = 0;

        ///
        /// check if the surface is compatible with the port
        ///
        virtual bool compatible( const Surface * ) const = 0;
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
        BaseKernel( const char * name )
            : mName( name )
            , mStreams( "KERNEL STREAM" )
            , mParameters( "KERNEL PARAMETER" )
            , mPorts( "KEREL PORT" )
        {
            if( strEmpty( name ) )
            {
                GN_UNEXPECTED();
            }
        }

        const char                        * getName() const { GN_ASSERT(!strEmpty(mName)); return mName; }

        //@{

        virtual size_t                      getNumStreams() const { return mStreams.size(); }
        virtual size_t                      getStreamIndex( const StrA & name ) const { return mStreams.getIndex( name ); }
        virtual StreamSource              * getStream( size_t index ) const;

        //@}

        //@{

        virtual size_t                      getNumParameters() const { return mParameters.size(); }
        virtual size_t                      getParameterIndex( const StrA & name ) const { return mParameters.getIndex( name ); }
        virtual const KernelParameterDesc * getParameterDesc( size_t index ) const { return mParameters.get( index ); }
        virtual KernelParameterSet        * createParameterSet();

        //@}

        //@{

        virtual size_t                      getNumPorts() const { return mPorts.size(); }
        virtual size_t                      getPortIndex( const StrA & name ) const { return mPorts.getIndex( name ); }
        virtual const KernelPortDesc      * getPortDesc( size_t index ) const;
        virtual bool                        compatible( const Surface * surf, const StrA & port ) const;

        inline  BaseKernelPort            * getPort( const StrA & name ) const { BaseKernelPort * const * p = mPorts.get(name); return p ? *p : 0; }
        template<typename T>
        inline  T                         * getPortT( const StrA & name ) const { return safeCastPtr<T>( getPort( name ) ); }

        inline  BaseKernelPort            & getPort( size_t index ) const { return *mPorts.at(index); }
        template<typename T>
        inline  T                         & getPortT( size_t index ) const { return safeCastRef<T>( getPort( index ) ); }

        //@}

    protected:

        ///
        /// \return index of the stream. -1 means failed.
        ///
        /// \note Kernel class does _NOT_ hold the ownership of the stream instance.
        ///
        size_t addStreamRef( StreamSource & stream );

        ///
        /// \note add parameter. Return index of newly inserted index. Return -1, if failed.
        ///
        size_t addParameter( const StrA & name, KernelParameterType type, size_t count );

        ///
        /// \return index of the port. -1 means failed.
        ///
        /// \note Kernel class does _NOT_ hold the ownership of the port instance.
        ///
        size_t addPortRef( BaseKernelPort & port );

    private:

        const char *                    mName;
        NamedArray<StreamSource*>       mStreams;
        NamedArray<KernelParameterDesc> mParameters;
        NamedArray<BaseKernelPort*>     mPorts;
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

        virtual void     registerKernelFactory( const StrA & name, KernelFactory factory, int quality );
        virtual Kernel * getKernel( const StrA & name );
        virtual void     deleteAllKernels();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct KernelItem
        {
            KernelFactory factory;
            int           quality;
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
#endif // __GN_GFX2_GFX2BASE_H__
