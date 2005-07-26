#ifndef __GN_BASE_PLUGIN_H__
#define __GN_BASE_PLUGIN_H__
// *****************************************************************************
//! \file    plugin.h
//! \brief   general plugin manager
//! \author  chenlee (2005.7.23)
// *****************************************************************************

namespace GN
{
    //!
    //! Basic plugin class
    //!
    struct PluginBase : public RefCounter
    {
    };

    //!
    //! Plugin factory function
    //!
    typedef GN::AutoRef<PluginBase> (*PluginCreationFunc)( void * parameter );

    //!
    //! Plugin type ID. "0" is invalid ID
    //!
    typedef uint16_t PluginTypeID;

    //!
    //! Plugin ID. "0" is invalid ID
    //!
    struct PluginID
    {
        union
        {
            uint32_t u32;
            int32_t  i32;
            struct
            {
                PluginTypeID type;
                uint16_t     name;
            };
        };

        static PluginID INVALID;

        bool operator < ( const PluginID & rhs ) const
        {
            return u32 < rhs.u32;
        }

        bool operator == ( const PluginID & rhs ) const
        {
            return u32 == rhs.u32;
        }

        bool operator != ( const PluginID & rhs ) const
        {
            return u32 != rhs.u32;
        }
    };

    //!
    //! Plugin Manager
    //!
    class PluginManager : public StdClass
    {
         GN_DECLARE_STDCLASS( PluginManager, StdClass );

        // ********************************
        //! \name  ctor/dtor
        // ********************************

        //@{
    public:
        PluginManager()          { clear(); }
        virtual ~PluginManager() { quit(); }
        //@}

        // ********************************
        //! \name standard init/quit
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() {}
        //@}

        // ********************************
        //! \name Plugin Management
        // ********************************

        //@{
    public:

        //!
        //! Retrieve specific plugin's type ID by its name.
        //!
        PluginTypeID getPluginTypeID( const StrA & ) const;

        //!
        //! Get specific plugin's type name
        //!
        const StrA & getPluginTypeName( PluginTypeID ) const;

        //!
        //! Get specific plugin's type description
        //!
        const StrA & getPluginTypeDesc( PluginTypeID ) const;

        //!
        //! Retrieve plugin ID by its type-ID and name
        //!
        PluginID getPluginID( PluginTypeID type, const StrA & name ) const;

        //!
        //! Retrieve plugin ID by its type and name
        //!
        PluginID getPluginID( const StrA & type, const StrA & name ) const
        {
            return getPluginID( getPluginTypeID(type), name );
        }

        //!
        //! Valid plugin ID or not?
        //!
        bool validID( PluginID ) const;

        //!
        //! Get specific plugin's name
        //!
        const StrA & getPluginName( PluginID ) const;

        //!
        //! Get specific plugin's description
        //!
        const StrA & getPluginDesc( PluginID ) const;

        //!
        //! Register new plugin type. Return 0 if failed.
        //!
        PluginTypeID registerPluginType( const StrA & type, const StrA & desc );

        //!
        //! Remove existing plugin type. This will also remove
        //! all plugins of this type.
        //!
        void removePluginType( PluginTypeID );

        //!
        //! Remove existing plugin type by its name
        //!
        void removePluginType( const StrA & type )
        {
            removePluginType( getPluginTypeID( type ) );
        }

        //!
        //! Register a new plugin class
        //!
        PluginID registerPlugin(
            PluginTypeID type,
            const StrA & name,
            const StrA & desc,
            PluginCreationFunc factoryFunc,
            bool overrideExistingPlugin = false );

        //!
        //! Register a new plugin class
        //!
        PluginID registerPlugin(
            const StrA & type,
            const StrA & name,
            const StrA & desc,
            PluginCreationFunc factoryFunc,
            bool overrideExistingPlugin = false )
        {
            return registerPlugin(
                getPluginTypeID( type ),
                name, desc, factoryFunc, overrideExistingPlugin );
        }

        //!
        //! Remove one existing plugin
        //!
        void removePlugin( PluginID );

        //!
        //! Remove one existing plugin
        //!
        void removePlugin( const StrA & type, const StrA & name )
        {
            removePlugin( getPluginID( type, name ) );
        }

        //!
        //! Create new instance of specific plugin
        //!
        AutoRef<PluginBase> createInstance( PluginID id, void * param ) const;

        //!
        //! Create new instance of specific plugin
        //!
        AutoRef<PluginBase> createInstance(
            const StrA & type,
            const StrA & name,
            void * param ) const
        {
            return createInstance( getPluginID( type, name ), param );
        }

        //@}

        // ********************************
        //!\name plugin library management
        // ********************************

        //@{
    public:

        //!
        //! Not implemented
        //!
        void importPluginLibrary( const StrA & libraryPath );

        //!
        //! Not implemented
        //!
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
            StrA               desc;
            PluginCreationFunc factory;
        };

        typedef PluginTypeID TypeHandle;
        typedef uint16_t     NameHandle;

        HandleManager<TypeItem,TypeHandle> mTypes;
        HandleManager<NameItem,NameHandle> mNames;
        std::map<PluginID,PluginItem>      mPlugins;

        // ********************************
        //   private functions
        // ********************************
    private:

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
