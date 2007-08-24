#ifndef __GN_GFX_GFX2BASE_H__
#define __GN_GFX_GFX2BASE_H__
// *****************************************************************************
/// \file
/// \brief   basic graphics system implementation (for graphics system development only)
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

#include "GNgfx.h"

namespace GN { namespace gfx
{
    class BaseKernel;
    class BaseGraphicsSystem;

    ///
    /// dummy kernel parameter
    ///
    class DummyKernelParameter : public GN::gfx::KernelParameter
    {
        DummyKernelParameter() {}

    public:

        //@{

        static DummyKernelParameter & sGetInstance() { static DummyKernelParameter sInstance; return sInstance; }

        virtual size_t size() const { return 0; }
        virtual void   get( size_t, size_t, void * ) const {}
        virtual bool   set( size_t, size_t, const void * ) { return true; }
        virtual void   unset() {}

        //@}
    };

    ///
    /// base kernel parameter.
    ///
    class BaseKernelParameter : public KernelParameter
    {
        const KernelParameterReflection & mRefl;
        DynaArray<UInt8>                  mData;

    protected:

        //@{
        template<typename T>
        const T & getRef() const { return *(const T*)mData.cptr(); }

        template<typename T>
        T & getRef() { return *(T*)mData.cptr(); }

        template<typename T>
        const T * getPtr() const { return (const T*)mData.cptr(); }

        template<typename T>
        T * getPtr() { return (T*)mData.cptr(); }
        //@}

        void error( const char * msg ) const
        {
            GN_ERROR(getLogger("GN.gfx2.BaseKernelParameter"))(
                "kernel(%s), parameter(%s): %s",
                mRefl.kernel.cptr(),
                mRefl.name.cptr(),
                msg );
        }

    public:

        //@{

        BaseKernelParameter( const KernelParameterReflection & refl )
            : mRefl(refl), mData( refl.calcSizeInBytes() )
        {
        }

        virtual size_t size() const
        {
            return mData.size();
        }

        virtual void get( size_t offset, size_t bytes, void * values ) const
        {
            if( offset >= mData.size() || (offset+bytes) > mData.size() )
            {
                error( "out of range." );
                return;
            }
            if( NULL == values )
            {
                error( "NULL values." );
                return;
            }
            memcpy( values, &mData[offset], bytes );
        }

        virtual bool set( size_t offset, size_t bytes, const void * values )
        {
            if( NULL == values )
            {
                error( "NULL values." );
                return false;
            }
            if( 0 == mRefl.count )
            {
                size_t newsize = offset + bytes;
                if( mData.size() < newsize ) mData.resize( newsize );
            }
            else
            {
                if( offset >= mData.size() || (offset+bytes) > mData.size() )
                {
                    error( "out of range." );
                    return false;
                }
            }

            memcpy( &mData[offset], values, bytes );

            return true;
        }

        virtual void unset()
        {
            if( 0 == mRefl.count )
            {
                mData.clear();
            }
        }

        //@}
    };

    ///
    /// kernel parameter that contains typed value
    ///
    template<typename T>
    struct TypedKernelParameter : public BaseKernelParameter
    {
        //@{

        operator const T & () const { return getRef<T>(); }

        TypedKernelParameter( const KernelParameterReflection & refl, const T & initial )
            : BaseKernelParameter( refl )
        {
            getRef<T>() = initial;
        }

        //@}
    };

    ///
    /// kernel parameter that represents boolean value.
    ///
    struct BoolKernelParameter : TypedKernelParameter<bool>
    {
        //@{

        BoolKernelParameter( const KernelParameterReflection & refl, bool initial )
            : TypedKernelParameter( refl, initial ) {}

        //@}
    };

    ///
    /// kernel parameter that represents 32bit integer.
    ///
    template<typename T>
    struct IntKernelParameter : TypedKernelParameter<T>
    {
        //@{

        IntKernelParameter( const KernelParameterReflection & refl, const T & initial )
            : TypedKernelParameter( refl, initial ) {}

        //@}
    };

    ///
    /// kernel parameter that represents 32bit float.
    ///
    struct FloatKernelParameter : TypedKernelParameter<float>
    {
        //@{

        FloatKernelParameter( const KernelParameterReflection & refl, float initial )
            : TypedKernelParameter( refl, initial ) {}

        //@}
    };

    class BaseKernelPort;

    ///
    /// base kernel class
    ///
    class BaseKernel : public Kernel
    {
    public:

        ///
        /// ctor
        ///
        BaseKernel( const char * name );

        ///
        /// get the reflection
        ///
        const KernelReflection & getRefl() const { return mRefl; }

        // from parent class
        //@{

        const StrA             & getName() const { GN_ASSERT(!mRefl.name.empty()); return mRefl.name; }
        virtual StreamSource   * getStream( size_t index ) const;
        virtual bool             compatible( const Surface * surf, const StrA & port ) const;

        //@}

        /// \name port utilities.
        //@{

        inline  BaseKernelPort & getPort( size_t index ) const { GN_ASSERT( mPorts[index] ); return *mPorts[index]; }

        template<typename T>
        inline  T              & getPortT( size_t index ) const { return safeCastRef<T>( getPort( index ) ); }

        inline  BaseKernelPort * getPort( const StrA & name ) const
        {
            size_t i = mRefl.ports.name2idx( name );
            if( (size_t)-1 == i ) return 0;
            GN_ASSERT( mPorts[i] );
            return mPorts[i];
        }

        template<typename T>
        inline  T              * getPortT( const StrA & name ) const { return safeCastPtr<T>( getPort( name ) ); }

        //@}

        //@{

        void setStreamRef( size_t index, StreamSource & stream );
        void setPortRef( size_t index, BaseKernelPort & port );

        //@}

    protected:

        ///
        /// get reference to itself.
        ///
        /// \note: this is used in subclasses' constructor, to initialize some data member.
        ///
        BaseKernel & baseref() { return *this; }

        ///
        /// check validity of itself
        ///
        bool validate() const;

    private:

        const KernelReflection   & mRefl;
        DynaArray<StreamSource*>   mStreams;
        DynaArray<BaseKernelPort*> mPorts;
    };

    ///
    /// base kernel port class
    ///
    class BaseKernelPort : public NoCopy
    {
        const KernelPortReflection & mRefl;
        DynaArray<Guid>              mAllowedSurfaceTypes; ///< surface types that accepeted by this port

    protected:

        void addSurfaceType( const Guid & g ) { mAllowedSurfaceTypes.append( g ); }

    public:

        ///
        /// ctor
        ///
        BaseKernelPort( BaseKernel & k, const StrA & name, const Guid * allowedSurfaceTypes, size_t count )
            : mRefl( k.getRefl().ports[name] )
            , mAllowedSurfaceTypes( allowedSurfaceTypes, count )
        {
            k.setPortRef( mRefl.index, *this );
        }

        ///
        /// return array of allowed surface types
        ///
        const DynaArray<Guid> & getAllowedSurfaceTypes() const { return mAllowedSurfaceTypes; }

        ///
        /// get reflection
        ///
        const KernelPortReflection & getRefl() const { return mRefl; }

        ///
        /// check if the surface is compatible with the port
        ///
        virtual bool compatible( const Surface * ) const = 0;
    };

    ///
    /// surface factory (creator)
    ///
    typedef Surface * (*SurfaceFactory)( BaseGraphicsSystem & gs, const SurfaceDesc & desc, const SurfaceCreationHints & hints );

    ///
    /// base graphics system
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

        virtual void      registerKernelFactory( const StrA & name, KernelFactory factory, int quality );
        virtual Kernel  * getKernel( const StrA & name );
        virtual void      deleteAllKernels();
        virtual Surface * createSurface( const SurfaceCreationParameter & );

        //@}

        // ********************************
        // surface type management
        // ********************************
    public:

        //@{

        void registerSurfeceType( const Guid & type, SurfaceFactory factory, const StrA & name );
        void registerSurfaceCreationRule( const Guid & type1, const Guid & type2, const Guid & result );

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

        struct SurfaceCreationRule
        {
            Guid type1;
            Guid type2;

            bool operator<( const SurfaceCreationRule & rhs ) const
            {
                if( type1 != rhs.type1 )
                    return type1 < rhs.type1;
                else
                    return type2 < rhs.type2;
            }
        };

        typedef std::map<Guid,SurfaceFactory>      SurfaceFactoryRegistry;
        typedef std::map<SurfaceCreationRule,Guid> SurfaceCreationRuleRegistry;

        NamedHandleManager<KernelItem,UInt32> mKernels;
        SurfaceFactoryRegistry                mSurfaceFactories;
        SurfaceCreationRuleRegistry           mSurfaceRules;

        // ********************************
        // private functions
        // ********************************
    private:

        bool mergeSurfaceType( const Guid & type1, const Guid & type2, Guid & result );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_GFX2BASE_H__
