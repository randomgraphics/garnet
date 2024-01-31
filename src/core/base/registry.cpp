#include "pch.h"

GN::Logger * GN::Registry::sLogger = GN::getLogger("GN.base.Registry");

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API GN::Registry::ItemKey GN::Registry::set(const std::string & name, const Variant & value, bool overwriteExisting) {
    GN_GUARD;

    ItemKey key = name2Key(name);

    if (0 == key) {
        // insert a new item to
        Item i;
        i.name  = name;
        i.value = value;
        key     = mItems.add(i);
        if (0 == key) return 0;
        GN_ASSERT(NULL == mNames.find(name));
        mNames[name] = key;
    } else if (overwriteExisting) {
        // Override old value
        GN_ASSERT(mItems[key].name == name && *mNames.find(name) == key);
        mItems[key].value = value;
    } else {
        GN_ERROR(sLogger)("Item '%s' is already existed.!", name.data());
        return 0;
    }

    // success
    return key;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::Registry::importFromStr(const std::string & s) {
    GN_GUARD;

    if (s.empty()) return;

    const char * ptr = s.data();
    const char * end = ptr + s.size();

#define NOT_EOL (ptr < end && *ptr != '\n')

#define IS_SPACE ('\t' == *ptr || ' ' == *ptr)

    // get name

    while (NOT_EOL && IS_SPACE) ++ptr;
    const char * name_s = ptr;

    while (NOT_EOL && !IS_SPACE) ++ptr;
    const char * name_e = ptr;

    if (name_e == name_s) return;

    // check "="

    while (NOT_EOL && IS_SPACE) ++ptr;
    const char * assign_s = ptr;

    if (ptr < end && *ptr == '=') ++ptr;
    const char * assign_e = ptr;

    if (assign_s + 1 != assign_e) return;

    // get value

    while (NOT_EOL && IS_SPACE) ++ptr;
    const char * value_s = ptr;

    while (ptr < end && !IS_SPACE) ++ptr;
    const char * value_e = ptr;

    if (value_e == value_s) return;

    // add name and value into registry
    std::string name(name_s, name_e - name_s);
    std::string value(value_s, value_e - value_s);
    sets(name, value, true);

    GN_UNGUARD;
}
