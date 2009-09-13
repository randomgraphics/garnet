@echo off
@echo Generate headers of standard uniforms...

set header="%~dp0standardUniforms.h"

echo // *********************************************>%header%
echo // This is script generated header. DO NOT EDIT.>>%header%
echo // *********************************************>>%header%
echo.>>%header%
echo #ifndef __GN_UTIL_STANDARUNIFORMS_H__>>%header%
echo #define __GN_UTIL_STANDARUNIFORMS_H__>>%header%
echo // *****************************************************************************>>%header%
echo /// \file>>%header%
echo /// \brief   Define standard scene parameters>>%header%
echo /// \author  chenli@@REDMOND (%DATE%)>>%header%
echo // *****************************************************************************>>%header%
echo.>>%header%
echo namespace GN { namespace util>>%header%
echo {>>%header%
echo.    /// standard uniform description>>%header%
echo.    struct StandardUniformDesc>>%header%
echo.    {>>%header%
echo.        const char * name;   ///^< parameter name in effect>>%header%
echo.        size_t       size;   ///^< parameter size in bytes>>%header%
echo.        bool         global; ///^< true: global parameter^; false: per visual node parameter>>%header%
echo.        const char * desc;   ///^< parameter description>>%header%
echo.    };>>%header%
echo.>>%header%
echo.    /// Standard uniform type enumeration>>%header%
echo.    struct StandardUniformType>>%header%
echo.    {>>%header%
echo.        enum ENUM>>%header%
echo.        {>>%header%
for /f "eol=; delims=, tokens=1,2,3,4,5*" %%a in (%~dp0standardUniformMeta.txt) do echo.            %%a%%b, ///^< %%f>>%header%
echo.            NUM_STANDARD_UNIFORMS , ///^< number of standard uniforms>>%header%
echo.        };>>%header%
echo.        GN_DEFINE_ENUM_CLASS_HELPERS( StandardUniformType, ENUM )>>%header%
echo.>>%header%
echo.        /// Check if the enum value is valid or not.>>%header%
echo.        bool isValid() const>>%header%
echo.        {>>%header%
echo.            return 0 ^<= (*this) ^&^& (*this) ^<= NUM_STANDARD_UNIFORMS;>>%header%
echo.        }>>%header%
echo.>>%header%
echo.        /// get uniform descriptor>>%header%
echo.        const StandardUniformDesc ^& desc() const>>%header%
echo.        {>>%header%
echo.            static const StandardUniformDesc DESCRIPTORS[] = {>>%header%
for /f "eol=; delims=, tokens=1,2,3,4,5*" %%a in (%~dp0standardUniformMeta.txt) do echo.                { "%%a"%%b, sizeof(%%c)%%d, %%e, "%%f" },>>%header%
echo.            };>>%header%
echo.>>%header%
echo.            GN_ASSERT( *this ^<= GN_ARRAY_COUNT(DESCRIPTORS) );>>%header%
echo.            return DESCRIPTORS[*this];>>%header%
echo.        }>>%header%
echo.>>%header%
echo.        /// get uniform name. Return NULL for invalid uniform type.>>%header%
echo.        const char * name() const>>%header%
echo.        {>>%header%
echo.            return isValid() ? desc().name : NULL;>>%header%
echo.        }>>%header%
echo.    };>>%header%
echo.>>%header%
echo }}>>%header%
echo.>>%header%
echo // *****************************************************************************>>%header%
echo //                                     EOF>>%header%
echo // *****************************************************************************>>%header%
echo #endif // __GN_UTIL_STANDARUNIFORMS_H__>>%header%

@echo Done.


:print desc
