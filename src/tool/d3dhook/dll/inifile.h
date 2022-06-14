#ifndef __GN_BASE_INIFILE_H__
#define __GN_BASE_INIFILE_H__
// *****************************************************************************
/// \file
/// \brief   Lightweight INI file parser
/// \author  CHEN@@CHENLI-HOMEPC (2013.12.18)
// *****************************************************************************

namespace GN {
///
/// a lightweight INI file parser
///
class IniFile {
public:
    // ********************************
    // ctor/dtor
    // ********************************

    void clear();

    bool loadFromFile(const wchar_t * filename);
    bool reads(const char * section, const char * key, StrA & value);
    bool readi(const char * section, const char * key, int & value);
    bool readf(const char * section, const char * key, float & value);
    bool readb(const char * section, const char * key, bool & value);

    StrA dreads(const char * section, const char * key, const char * defaultValue) {
        StrA v;
        if (!reads(section, key, v)) v = defaultValue;
        return v;
    }
    int dreadi(const char * section, const char * key, int defaultValue) {
        int v;
        if (!readi(section, key, v)) v = defaultValue;
        return v;
    }
    float dreadf(const char * section, const char * key, float defaultValue) {
        float v;
        if (!readf(section, key, v)) v = defaultValue;
        return v;
    }
    bool dreadb(const char * section, const char * key, bool defaultValue) {
        bool v;
        if (!readb(section, key, v)) v = defaultValue;
        return v;
    }

    void writes(const char * section, const char * key, const char * value);
    void writei(const char * section, const char * key, int value);
    void writef(const char * section, const char * key, float value);
    void writeb(const char * section, const char * key, bool value);

    // ********************************
    // private variables
    // ********************************
private:
    enum ValueType {
        VT_STRING,
        VT_INT,
        VT_FLOAT,
        VT_BOOL,
    };

    struct Value {
        ValueType type;
        union {
            const char * s;
            int          i;
            float        f;
            bool         b;
        };
    };

    StringMap<char, Value> mValues;

    // ********************************
    // private functions
    // ********************************
private:
};
}; // namespace GN
// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_INIFILE_H__
