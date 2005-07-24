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
    union PluginID
    {
        uint32_t u32;
        int32_t  i32;
        struct
        {
            PluginTypeID type;
            uint16_t     name;
        };
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
        void quit() { GN_STDCLASS_QUIT(); }
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() {}
        //@}

        // ********************************
        //   public functions
        // ********************************
    public:

        //!
        //! Create new instance of specific plugin
        //!
        AutoRef<PluginBase> createInstance( const StrA & type, const StrA & name );

        //!
        //! Retrieve specific plugin's type ID by its name.
        //!
        PluginTypeID getPluginTypeID( const StrA & );

        //!
        //! Get specific plugin's type name
        //!
        const StrA & getPluginTypeName( PluginTypeID );

        //!
        //! Get specific plugin's type description
        //!
        const StrA & getPluginTypeDesc( PluginTypeID );

        //!
        //! Retrieve plugin ID by its type and name
        //!
        PluginID getPluginID( const StrA & type, const StrA & name );

        //!
        //! Get specific plugin's name
        //!
        const StrA & getPluginName( PluginID );

        //!
        //! Get specific plugin's description
        //!
        const StrA & getPluginDesc( PluginID );

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

        // ********************************
        //!\name plugin library management
        // ********************************

        //@{

        void importPluginLibrary( const StrA & libraryPath );

        void removePluginLibrary( const StrA & libraryPath );

        //@}

        // ********************************
        //   private variables
        // ********************************
    private:

        struct PluginDesc
        {
            StrA               type;
            StrA               name;
            PluginCreationFunc factory;
        };

        std::map<PluginID,PluginDesc> mPlugins;

        // ********************************
        //   private functions
        // ********************************
    private:
    };
}

// *****************************************************************************
//                           End of plugin.h
// *****************************************************************************
#endif // __GN_BASE_PLUGIN_H__
