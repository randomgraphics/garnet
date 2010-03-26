#ifndef __GN_BASE_REGISTRY_H__
#define __GN_BASE_REGISTRY_H__
// *****************************************************************************
/// \file
/// \brief   tree-like registry structure.
/// \author  chenlee (2005.6.15)
// *****************************************************************************

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
        StringMap<char,UInt32> mNames;

        static Logger * sLogger;

        const Variant * GetItemByKey( UInt32 key, const char * name, bool printError ) const
        {
            GN_GUARD_SLOW;

            if( !mItems.ValidHandle( key ) )
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
        /// Construct from string. See ImportFromStr() for details of input string format.
        ///
        explicit Registry( const StrA & values ) { ImportFromStr(values); }

        ///
        /// Destructor
        ///
        ~Registry() {}

        ///
        /// Clear to empty registry.
        ///
        void Clear() { mItems.Clear(); mNames.Clear(); }

        ///
        /// Get item key by name
        ///
        ItemKey Name2Key( const StrA & name ) const
        {
            const ItemKey * key = mNames.Find(name);
            return (NULL == key) ? 0 : *key;
        }

        ///
        /// Get item name by key. Return empty string for non-existed key
        ///
        const StrA & Key2Name( ItemKey key ) const
        {
            return mItems.ValidHandle( key ) ? mItems[key].name : StrA::EMPTYSTR;
        }

        /// Set registry value by name
        //@{
        ItemKey Set( const StrA & name, const Variant & value, bool override = true );
        ItemKey SetS( const StrA & name, const StrA & value, bool override = true ) { return Set( name, value, override ); }
        //@}

        /// Get registry value by key
        //@{
        const Variant * Get( ItemKey key ) const { return GetItemByKey( key, 0, true ); }
        //@}

        /// Get registry value by name
        //@{
        const Variant * Get( const StrA & name ) const { return GetItemByKey( Name2Key(name), name.ToRawPtr(), true ); }
        //@}

        /// Get registry value with default value, by key
        //@{
        const Variant & Get( ItemKey key, const Variant & defval ) const
        {
            const Variant * item = GetItemByKey( key, 0, false );
            return (0 == item) ? defval : *item;
        }
        //@}

        /// Get registry value with default value, by name
        //@{
        const Variant & Get( const StrA & name, const Variant & defval ) const
        {
            const Variant * item = GetItemByKey( Name2Key(name), 0, false );
            return (0 == item) ? defval : *item;
        }
        const StrA & GetS( const StrA & name, const StrA & defval ) const
        {
            const Variant * item = GetItemByKey( Name2Key(name), 0, false );
            return (0 == item) ? defval : item->GetS();
        }
        bool GetB( const StrA & name, bool defval ) const
        {
            const Variant * item = GetItemByKey( Name2Key(name), 0, false );
            return (0 == item) ? defval : item->GetdB( defval );
        }
        int GetI( const StrA & name, int defval ) const
        {
            const Variant * item = GetItemByKey( Name2Key(name), 0, false );
            return (0 == item) ? defval : item->GetdI( defval );
        }
        float GetF( const StrA & name, float defval ) const
        {
            const Variant * item = GetItemByKey( Name2Key(name), 0, false );
            return (0 == item) ? defval : item->GetdF( defval );
        }
        void * GetP( const StrA & name, void * defval ) const
        {
            const Variant * item = GetItemByKey( Name2Key(name), 0, false );
            return (0 == item) ? defval : item->GetdP( defval );
        }
        Vector4f GetV( const StrA & name, const Vector4f & defval ) const
        {
            const Variant * item = GetItemByKey( Name2Key(name), 0, false );
            return (0 == item) ? defval : item->GetdV( defval );
        }
        Matrix44f GetM( const StrA & name, const Matrix44f & defval ) const
        {
            const Variant * item = GetItemByKey( Name2Key(name), 0, false );
            return (0 == item) ? defval : item->GetdM( defval );
        }
        //@}

        ///
        /// Get key of first item
        ///
        ItemKey GetFirstItem() const { return mItems.First(); }

        ///
        /// Get key of next item
        ///
        ItemKey GetNextItem( ItemKey key ) const { return mItems.Next( key ); }

        ///
        /// Import registry from string.
        /// Import string must be in format of: "name1=value1\nname2=value2\n..."
        ///
        void ImportFromStr( const StrA & );
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_REGISTRY_H__
