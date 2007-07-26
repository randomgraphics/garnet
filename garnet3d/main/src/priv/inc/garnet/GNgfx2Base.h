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
    /// dummy kernel parameter
    ///
    class DummyKernelParameter : public GN::gfx::KernelParameter
    {
        DummyKernelParameter() {}

    public:

        //@{

        static DummyKernelParameter & sGetInstance() { static DummyKernelParameter sInstance; return sInstance; }

        virtual void setb( size_t, size_t, const bool         * ) {}
        virtual void seti( size_t, size_t, const int          * ) {}
        virtual void setf( size_t, size_t, const float        * ) {}
        virtual void sets( size_t, size_t, const char * const * ) {}
        virtual void unset() {}

        //@}
    };

    ///
    /// base kernel parameter.
    ///
    class BaseKernelParameter : public KernelParameter
    {
    public:

        //@{

        BaseKernelParameter() {}

        virtual void setb( size_t, size_t, const bool         * ) { GN_UNEXPECTED(); }
        virtual void seti( size_t, size_t, const int          * ) { GN_UNEXPECTED(); }
        virtual void setf( size_t, size_t, const float        * ) { GN_UNEXPECTED(); }
        virtual void sets( size_t, size_t, const char * const * ) { GN_UNEXPECTED(); }
        virtual void unset() { GN_UNEXPECTED(); }

        //@}
    };

    ///
    /// kernel parameter that contains typed value
    ///
    template<typename T>
    struct TypedKernelParameter : public BaseKernelParameter
    {
        //@{

        T value;

        operator const T & () const { return value; }

        TypedKernelParameter( const T & initial ) : value(initial) {}

        virtual void unset() {}

        //@}
    };

    ///
    /// kernel parameter that represents boolean value.
    ///
    struct BoolKernelParameter : TypedKernelParameter<bool>
    {
        //@{

        BoolKernelParameter( bool initial ) : TypedKernelParameter( initial ) {}

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
    struct IntKernelParameter : TypedKernelParameter<T>
    {
        //@{

        IntKernelParameter( const T & initial ) : TypedKernelParameter( initial ) {}

        void seti( size_t offset, size_t count, const int * values )
        {
            if( 0 != offset || 1 != count || NULL == values )
            {
                GN_ERROR(getLogger("GN.gfx2.IntKernelParameter"))( "invalid parameter value." );
                return;
            }

            value = (T)*values;
        }

        //@}
    };

    ///
    /// kernel parameter that represents 32bit float.
    ///
    struct FloatKernelParameter : TypedKernelParameter<float>
    {
        //@{

        FloatKernelParameter( float initial ) : TypedKernelParameter( initial ) {}

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

    public:

        ///
        /// ctor
        ///
        BaseKernelPort( BaseKernel & k, const StrA & name )
            : mRefl( k.getRefl().ports[name] )
        {
            k.setPortRef( mRefl.index, *this );
        }

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
