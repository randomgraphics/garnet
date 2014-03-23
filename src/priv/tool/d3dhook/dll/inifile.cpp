#include "pch.h"
#include "inifile.h"

#pragma warning(disable:4100)

//
//
// -----------------------------------------------------------------------------
void GN::IniFile::clear()
{
}

//
//
// -----------------------------------------------------------------------------
bool GN::IniFile::loadFromFile(const wchar_t * filename)
{
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::IniFile::reads(const char * section, const char * key, StrA  & value)
{
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::IniFile::readi(const char * section, const char * key, int   & value)
{
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::IniFile::readf(const char * section, const char * key, float & value)
{
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::IniFile::readb(const char * section, const char * key, bool  & value)
{
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::IniFile::writes(const char * section, const char * key, const char * value)
{
}

//
//
// -----------------------------------------------------------------------------
void GN::IniFile::writei(const char * section, const char * key, int value)
{
}

//
//
// -----------------------------------------------------------------------------
void GN::IniFile::writef(const char * section, const char * key, float value)
{
}

//
//
// -----------------------------------------------------------------------------
void GN::IniFile::writeb(const char * section, const char * key, bool value)
{
}
