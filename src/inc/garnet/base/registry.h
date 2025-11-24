#ifndef __GN_BASE_REGISTRY_H__
#define __GN_BASE_REGISTRY_H__
// *****************************************************************************
/// \file
/// \brief   tree-like registry structure.
/// \author  chenlee (2005.6.15)
// *****************************************************************************

namespace GN {
///
/// Search-able dictionary of name value pairs.
///
class GN_API Registry {
    struct Item {
        std::string    name;
        Variant value;
    };

    HandleManager<Item, uint32_t> mItems;
    StringMap<char, uint32_t>     mNames;

    static Logger * sLogger;

    const Variant * getItemByKey(uint32_t key, const char * name, bool printError) const {
        GN_GUARD_SLOW;

        if (!mItems.validHandle(key)) {
            if (printError) { GN_ERROR(sLogger)("invalid key '%d' for item named '%s'", key, name ? name : "<UNKNOWN>"); }
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
    typedef uint32_t ItemKey;

    ///
    /// Default constructor
    ///
    Registry() {}

    ///
    /// Construct from string. See importFromStr() for details of input string format.
    ///
    explicit Registry(const std::string & values) { importFromStr(values); }

    ///
    /// Destructor
    ///
    ~Registry() {}

    ///
    /// Clear to empty registry.
    ///
    void clear() {
        mItems.clear();
        mNames.clear();
    }

    ///
    /// Get item key by name
    ///
    ItemKey name2Key(const std::string & name) const {
        const ItemKey * key = mNames.find(name);
        return (NULL == key) ? 0 : *key;
    }

    ///
    /// Get item name by key. Return empty string for non-existed key
    ///
    const std::string & key2Name(ItemKey key) const { return mItems.validHandle(key) ? mItems[key].name : str::EMPTY_STRING(); }

    /// set registry value by name
    //@{
    ItemKey set(const std::string & name, const Variant & value, bool override = true);
    ItemKey sets(const std::string & name, const std::string & value, bool override = true) { return set(name, value, override); }
    //@}

    /// get registry value by key
    //@{
    const Variant * get(ItemKey key) const { return getItemByKey(key, 0, true); }
    //@}

    /// get registry value by name
    //@{
    const Variant * get(const StrA & name) const { return getItemByKey(name2Key(name), name.data(), true); }
    //@}

    /// get registry value with default value, by key
    //@{
    const Variant & get(ItemKey key, const Variant & defval) const {
        const Variant * item = getItemByKey(key, 0, false);
        return (0 == item) ? defval : *item;
    }
    //@}

    /// get registry value with default value, by name
    //@{
    const Variant & get(const std::string & name, const Variant & defval) const {
        const Variant * item = getItemByKey(name2Key(name), 0, false);
        return (0 == item) ? defval : *item;
    }
    const std::string & gets(const std::string & name, const std::string & defval) const {
        const Variant * item = getItemByKey(name2Key(name), 0, false);
        return (0 == item) ? defval : item->gets();
    }
    bool getb(const std::string & name, bool defval) const {
        const Variant * item = getItemByKey(name2Key(name), 0, false);
        return (0 == item) ? defval : item->getdb(defval);
    }
    int geti(const std::string & name, int defval) const {
        const Variant * item = getItemByKey(name2Key(name), 0, false);
        return (0 == item) ? defval : item->getdi(defval);
    }
    float getf(const std::string & name, float defval) const {
        const Variant * item = getItemByKey(name2Key(name), 0, false);
        return (0 == item) ? defval : item->getdf(defval);
    }
    void * getp(const std::string & name, void * defval) const {
        const Variant * item = getItemByKey(name2Key(name), 0, false);
        return (0 == item) ? defval : item->getdp(defval);
    }
    Vector4f getv(const std::string & name, const Vector4f & defval) const {
        const Variant * item = getItemByKey(name2Key(name), 0, false);
        return (0 == item) ? defval : item->getdv(defval);
    }
    Matrix44f getm(const std::string & name, const Matrix44f & defval) const {
        const Variant * item = getItemByKey(name2Key(name), 0, false);
        return (0 == item) ? defval : item->getdm(defval);
    }
    //@}

    ///
    /// get key of first item
    ///
    ItemKey getFirstItem() const { return mItems.first(); }

    ///
    /// get key of next item
    ///
    ItemKey getNextItem(ItemKey key) const { return mItems.next(key); }

    ///
    /// Import registry from string.
    /// Import string must be in format of: "name1=value1\nname2=value2\n..."
    ///
    void importFromStr(const std::string &);
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_REGISTRY_H__
