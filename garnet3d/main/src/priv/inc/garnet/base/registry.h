#ifndef __GN_BASE_REGISTRY_H__
#define __GN_BASE_REGISTRY_H__
// *****************************************************************************
/// \file    registry.h
/// \brief   tree-like registry structure.
/// \author  chenlee (2005.6.15)
// *****************************************************************************

#include <map>

namespace GN
{
    ///
    /// ◊¢≤·±Ì¿‡–Õ.
    ///
    class Registry
    {
        struct Item
        {
            StrA name;
            Variant value;
        };

        HandleManager<Item,UInt32> mItems;
        std::map<StrA,UInt32> mNames;

        static Logger * sLogger;

        const Variant * getItemByKey( UInt32 key, const char * name, bool printError ) const
        {
            GN_GUARD_SLOW;

            if( !mItems.validHandle( key ) )
            {
                if( printError )
                {
                    GN_ERROR(sLogger)( "invalid key '%d' for item named '%s'", key, name ? name : "<UNKNOWN>" );
                }
                return NULL;
            }

            // success
            return &mItems[key].value;

            GN_UNGUARD_SLOW;
        }

    public:

        ///
        /// Item key
        ///
        typedef UInt32 ItemKey;

        ///
        /// Default constructor
        ///
        Registry() {}

        ///
        /// Construct from string. See importFromStr() for details of input string format.
        ///
        explicit Registry( const StrA & values ) { importFromStr(values); }

        ///
        /// Destructor
        ///
        ~Registry() {}

        ///
        /// Clear to empty registry.
        ///
        void clear() { mItems.clear(); mNames.clear(); }

        ///
        /// Get item key by name
        ///
        ItemKey name2Key( const StrA & name ) const
        {
            std::map<StrA,UInt32>::const_iterator i = mNames.find(name);
            return ( mNames.end() == i ) ? 0 : (*i).second;
        }

        ///
        /// Get item name by key. Return empty string for non-existed key
        ///
        const StrA & key2Name( ItemKey key ) const
        {
            return mItems.validHandle( key ) ? mItems[key].name : StrA::EMPTYSTR;
        }

        /// set registry value by name
        //@{
        ItemKey set( const StrA & name, const Variant & value, bool override = true );
        ItemKey sets( const StrA & name, const StrA & value, bool override = true ) { return set( name, value, override ); }
        //@}

        /// get registry value by key
        //@{
        const Variant * get( ItemKey key ) const { return getItemByKey( key, 0, true ); }
        //@}

        /// get registry value by name
        //@{
        const Variant * get( const StrA & name ) const { return getItemByKey( name2Key(name), name.cptr(), true ); }
        //@}

        /// get registry value with default value, by key
        //@{
        const Variant & get( ItemKey key, const Variant & defval ) const
        {
            const Variant * item = getItemByKey( key, 0, false );
            return (0 == item) ? defval : *item;
        }
        //@}

        /// get registry value with default value, by name
        //@{
        const Variant & get( const StrA & name, const Variant & defval ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defval : *item;
        }
        const StrA & gets( const StrA & name, const StrA & defval ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defval : item->gets();
        }
        bool getb( const StrA & name, bool defval ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defval : item->getdb( defval );
        }
        int geti( const StrA & name, int defval ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defval : item->getdi( defval );
        }
        float getf( const StrA & name, float defval ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defval : item->getdf( defval );
        }
        void * getp( const StrA & name, void * defval ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defval : item->getdp( defval );
        }
        Vector4f getv( const StrA & name, const Vector4f & defval ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defval : item->getdv( defval );
        }
        Matrix44f getm( const StrA & name, const Matrix44f & defval ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defval : item->getdm( defval );
        }
        //@}

        ///
        /// get key of first item
        ///
        ItemKey getFirstItem() const { return mItems.first(); }

        ///
        /// get key of next item
        ///
        ItemKey getNextItem( ItemKey key ) const { return mItems.next( key ); }

        ///
        /// Import registry from string.
        /// Import string must be in format of: "name1=value1\nname2=value2\n..."
        ///
        void importFromStr( const StrA & );
    };
}

// *****************************************************************************
//                           End of registry.h
// *****************************************************************************
#endif // __GN_BASE_REGISTRY_H__
