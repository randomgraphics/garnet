#ifndef __GN_BASE_REGISTRY_H__
#define __GN_BASE_REGISTRY_H__
// *****************************************************************************
//! \file    registry.h
//! \brief   tree-like registry structure.
//! \author  chenlee (2005.6.15)
// *****************************************************************************

#include <map>

namespace GN
{
    //!
    //! ◊¢≤·±Ì¿‡–Õ.
    //!
    class Registry
    {
        struct Item
        {
            StrA name;
            Variant value;
        };

        HandleManager<Item,uint32_t> mItems;
        std::map<StrA,uint32_t> mNames;

        const Variant * getItemByKey( uint32_t key, const char * name, bool printError ) const
        {
            GN_GUARD_SLOW;

            if( !mItems.validHandle( key ) )
            {
                if( printError )
                {
                    GN_ERROR( "invalid key '%d' for item named '%s'", key, name ? name : "<UNKNOWN>" );
                }
                return NULL;
            }

            // success
            return &mItems[key].value;

            GN_UNGUARD_SLOW;
        }

    public:

        //!
        //! Item key
        //!
        typedef uint32_t ItemKey;

        //!
        //! Constructor
        //!
        Registry() {}

        //!
        //! Destructor
        //!
        ~Registry() {}

        //!
        //! Clear to empty registry.
        //!
        void clear() { mItems.clear(); mNames.clear(); }

        //!
        //! Get item key by name
        //!
        ItemKey name2Key( const StrA & name ) const
        {
            std::map<StrA,uint32_t>::const_iterator i = mNames.find(name);
            return ( mNames.end() == i ) ? 0 : (*i).second;
        }

        //!
        //! Get item name by key. Return empty string for non-existed key
        //!
        const StrA & key2Name( ItemKey key ) const
        {
            return mItems.validHandle( key ) ? mItems[key].name : StrA::EMPTYSTR;
        }

        //!
        //! Get registry item by key
        //!
        //! \return
        //!     Return NULL if the key does not exist.
        //!
        const Variant * getItem( ItemKey key ) const { return getItemByKey( key, 0, true ); }

        //!
        //! Get registry item by key
        //!
        //! \return
        //!     Return default value if the key does not exist.
        //!
        const Variant & getItem( ItemKey key, const Variant & defaultValue ) const
        {
            const Variant * item = getItemByKey( key, 0, false );
            return (0 == item) ? defaultValue : *item;
        }

        //!
        //! Get registry item by name
        //!
        //! \return
        //!     Return NULL if the name does not exist.
        //!
        const Variant * getItem( const StrA & name ) const
        {
            return getItemByKey( name2Key(name), name.cptr(), true );
        }

        //!
        //! Get registry item by name
        //!
        //! \return
        //!     Return default value if the key is not exist.
        //!
        const Variant & getItem( const StrA & name, const Variant & defaultValue ) const
        {
            const Variant * item = getItemByKey( name2Key(name), 0, false );
            return (0 == item) ? defaultValue : *item;
        }

        //!
        //! Create a new key. Will fail, if the item exist and overwriteExisting is false.
        //!
        //! \return
        //!     Return key of the item. 0 means failed.
        //!
        ItemKey setItem( const StrA & name, const Variant & value, bool overwriteExisting = true );

        //!
        //! get key of first item
        //!
        ItemKey getFirstItem() const { return mItems.first(); }

        //!
        //! get key of next item
        //!
        ItemKey getNextItem( ItemKey key ) const { return mItems.next( key ); }

        //!
        //! Import registry from string.
        //! Import string must be in format of: "name1=value1\nname2=value2\n..."
        //!
        void importFromStr( const StrA & );
    };
}

// *****************************************************************************
//                           End of registry.h
// *****************************************************************************
#endif // __GN_BASE_REGISTRY_H__
