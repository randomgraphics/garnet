#ifndef __GN_BASE_PLUGIN_H__
#define __GN_BASE_PLUGIN_H__
// *****************************************************************************
/// \file    plugin.h
/// \brief   general plugin manager
/// \author  chenlee (2005.7.23)
// *****************************************************************************

namespace GN
{
    ///
    /// Plugin type ID. "0" is invalid ID
    ///
    typedef UInt16 PluginTypeID;

    ///
    /// Plugin ID. "0" is invalid ID
    ///
    struct PluginID
    {
        union
        {
            UInt32 u32; ///< Plugin ID as unsigned integer
            struct
            {
                PluginTypeID type; ///< Plugin type ID
                UInt16     name; ///< Plugin name ID
            };
        };

        static PluginID INVALID; ///< invalid ID

        ///
        /// Default constructor
        ///
        PluginID() {}

        ///
        /// construct from unsigned integer
        ///
        PluginID( UInt32 u ) : u32(u) {}

        ///
        /// construct from type and name ID
        ///
        PluginID( PluginTypeID t, UInt16 n ) : type(t), name(n) {}

        ///
        /// copy constructor
        ///
        PluginID( const PluginID & id ) : u32(id.u32) {}

        ///
        /// Convert to unsigned integer
        ///
        operator UInt32 &() { return u32; }

        ///
        /// Convert to unsigned integer
        ///
        operator const UInt32 &() const { return u32; }

        ///
        /// Copy operator
        ///
        PluginID & operator = ( const PluginID & rhs )
        {
            u32 = rhs.u32;
            return *this;
        }

        ///
        /// Less operator
        ///
        bool operator < ( const PluginID & rhs ) const
        {
            return u32 < rhs.u32;
        }

        ///
        /// Equality operator
        ///
        bool operator == ( const PluginID & rhs ) const
        {
            return u32 == rhs.u32;
        }

        ///
        /// Equality operator
        ///
        bool operator != ( const PluginID & rhs ) const
        {
            return u32 != rhs.u32;
        }
    };

    ///
    /// Basic plugin class
    ///
    class PluginBase
    {
        PluginID mID;

        friend class PluginManager;

    public:

        ///
        /// Default constructor
        ///
        PluginBase() {}

        ///
        /// Virtual destructor
        ///
        virtual ~PluginBase() {}

        ///
        /// Get ID of the plugin
        ///
        PluginID getID() const { return mID; }
    };

    ///
    /// Plugin factory class
    ///
    class PluginFactory
    {
        typedef PluginBase* (*FactoryFuncPtr)( void * );
        typedef Delegate1<PluginBase*, void *> FactoryFunctor;

        FactoryFunctor mFunc;

    public:
        ///
        /// Default constructor
        ///
        PluginFactory() {}

        ///
        /// Construct from functor
        ///
        PluginFactory( const FactoryFunctor & func ) : mFunc(func) {}

        ///
        /// Construct from free function pointer
        ///
        PluginFactory( const FactoryFuncPtr & func ) { mFunc.bind(func); }

        ///
        /// Is NULL factory or not?
        ///
        bool empty() const { return mFunc.empty(); }

        ///
        /// Call operator
        ///
        PluginBase * operator()( void * param ) const
        {
            GN_ASSERT( !mFunc.empty() );
            return mFunc(param);
        }
    };

    ///
    /// Plugin Manager
    ///
    class PluginManager : public CrossDllSingleton<PluginManager>
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        PluginManager()          {}
        virtual ~PluginManager() { reset(); }
        //@}

        // ********************************
        /// \name Plugin Management
        // ********************************

        //@{
    public:

        ///
        /// Reset to initial status (clear all plugins)
        ///
        void reset();

        ///
        /// Get number of plugins in manager
        ///
        size_t size() const { return mPlugins.size(); }

        ///
        /// Retrieve specific plugin's type ID by its name.
        ///
        GN_INLINE PluginTypeID getPluginTypeID( const StrA & ) const;

        ///
        /// Get specific plugin's type name
        ///
        const StrA & getPluginTypeName( PluginTypeID ) const;

        ///
        /// Get specific plugin's type description
        ///
        const StrA & getPluginTypeDesc( PluginTypeID ) const;

        ///
        /// Retrieve plugin ID by its type-ID and name
        ///
        PluginID getPluginID( PluginTypeID type, const StrA & name ) const;

        ///
        /// Retrieve plugin ID by its type and name
        ///
        PluginID getPluginID( const StrA & type, const StrA & name ) const
        {
            return getPluginID( getPluginTypeID(type), name );
        }

        ///
        /// Valid plugin ID or not?
        ///
        bool validID( PluginID ) const;

        ///
        /// Get specific plugin's name
        ///
        const StrA & getPluginName( PluginID ) const;

        ///
        /// Get specific plugin's description
        ///
        const StrA & getPluginDesc( PluginID ) const;

        ///
        /// Register new plugin type. Return 0 if failed.
        ///
        PluginTypeID registerPluginType( const StrA & type, const StrA & desc );

        ///
        /// Remove existing plugin type. This will also remove
        /// all plugins of this type.
        ///
        void removePluginType( PluginTypeID );

        ///
        /// Remove existing plugin type by its name
        ///
        void removePluginType( const StrA & type )
        {
            removePluginType( getPluginTypeID( type ) );
        }

        ///
        /// Register a new plugin class
        ///
        PluginID registerPlugin(
            PluginTypeID type,
            const StrA & name,
            const StrA & desc,
            const PluginFactory & factory,
            bool overrideExistingPlugin = false );

        ///
        /// Register a new plugin class
        ///
        PluginID registerPlugin(
            const StrA & type,
            const StrA & name,
            const StrA & desc,
            const PluginFactory & factory,
            bool overrideExistingPlugin = false )
        {
            return registerPlugin(
                getPluginTypeID( type ),
                name, desc, factory, overrideExistingPlugin );
        }

        ///
        /// Remove one existing plugin
        ///
        void removePlugin( PluginID );

        ///
        /// Remove one existing plugin
        ///
        void removePlugin( const StrA & type, const StrA & name )
        {
            removePlugin( getPluginID( type, name ) );
        }

        ///
        /// Create new instance of specific plugin
        ///
        template<typename T>
        T * createInstance( PluginID id, void * param = 0 ) const
        {
            return safeCast<T*>(doInstanceCreation( id, param ));
        }

        ///
        /// Create new instance of specific plugin
        ///
        template<typename T>
        T * createInstance(
            const StrA & type,
            const StrA & name,
            void * param = 0 ) const
        {
            return createInstance<T>( getPluginID( type, name ), param );
        }

        //@}

        // ********************************
        ///\name plugin library management
        // ********************************

        //@{
    public:

        ///
        /// Not implemented
        ///
        void importPluginLibrary( const StrA & libraryPath );

        ///
        /// Not implemented
        ///
        void removePluginLibrary( const StrA & libraryPath );

        //@}

        // ********************************
        //   private variables
        // ********************************
    private:

        struct TypeItem
        {
            StrA type;
            StrA desc;

            TypeItem( const StrA & t, const StrA & d ) : type(t), desc(d) {}
        };

        struct NameItem
        {
            StrA   name;
            size_t count;

            NameItem( const StrA & n ) : name(n), count(1) {}
        };

        struct PluginItem
        {
            StrA          desc;
            PluginFactory factory;
        };

        typedef PluginTypeID TypeHandle;
        typedef UInt16     NameHandle;

        HandleManager<TypeItem,TypeHandle> mTypes;
        HandleManager<NameItem,NameHandle> mNames;
        std::map<PluginID,PluginItem>      mPlugins;

        static Logger * sLogger;

        // ********************************
        //   private functions
        // ********************************
    private:

        PluginBase * doInstanceCreation( PluginID id, void * param = 0 ) const;

        struct TypeEqual
        {
            const StrA & mRef;
            TypeEqual( const StrA & ref ) : mRef(ref) {}
            bool operator () ( const TypeItem & item ) const
            {
                return item.type == mRef;
            }
        };

        struct NameEqual
        {
            const StrA & mRef;
            NameEqual( const StrA & ref ) : mRef(ref) {}
            bool operator () ( const NameItem & item ) const
            {
                return item.name == mRef;
            }
        };
    };
}

#if GN_ENABLE_INLINE
#include "plugin.inl"
#endif

// *****************************************************************************
//                           End of plugin.h
// *****************************************************************************
#endif // __GN_BASE_PLUGIN_H__
