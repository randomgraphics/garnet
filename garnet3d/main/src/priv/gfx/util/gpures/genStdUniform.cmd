@echo off
setlocal

REM ============================================================================
REM
REM Generate header
REM
REM ============================================================================

set header="%~dp0..\..\..\inc\garnet\gfx\stdUniform.h"

echo Generate standard uniform header: %header%


echo // *********************************************>%header%
echo // This is script generated header. DO NOT EDIT.>>%header%
echo // *********************************************>>%header%
echo.>>%header%
echo #ifndef __GN_GFX_STDUNIFORMDEF_H__>>%header%
echo #define __GN_GFX_STDUNIFORMDEF_H__>>%header%
echo // *****************************************************************************>>%header%
echo /// \file>>%header%
echo /// \brief   Define standard uniform resources>>%header%
echo /// \author  chenli@@REDMOND (%DATE%)>>%header%
echo // *****************************************************************************>>%header%
echo.>>%header%
echo namespace GN { namespace gfx>>%header%
echo {>>%header%
echo     // Forward declaration.>>%header%
echo.    struct StandardUniformDesc;>>%header%
echo.>>%header%
echo.    /// Standard uniform type enumeration>>%header%
echo.    struct StandardUniformType>>%header%
echo.    {>>%header%
echo.        enum ENUM>>%header%
echo.        {>>%header%
for /f "eol=# delims=; tokens=1,2,3,4,5,6*" %%a in (%~dp0stdUniformMeta.txt) do echo.            %%a%%b, //^< %%g>>%header%
echo.            NUM_STANDARD_UNIFORMS , ///^< number of standard uniforms>>%header%
echo.        };>>%header%
echo.        GN_DEFINE_ENUM_CLASS_HELPERS( StandardUniformType, ENUM )>>%header%
echo.>>%header%
echo.        /// Check if the enum value is valid or not.>>%header%
echo.        bool isValid() const>>%header%
echo.        {>>%header%
echo.            return 0 ^<= (*this) ^&^& (*this) ^< NUM_STANDARD_UNIFORMS;>>%header%
echo.        }>>%header%
echo.>>%header%
echo.        /// get uniform descriptor>>%header%
echo.        const StandardUniformDesc ^& getDesc() const;>>%header%
echo.>>%header%
echo.        /// get uniform name. Return NULL for invalid uniform type.>>%header%
echo.        const char * name() const;>>%header%
echo.    };>>%header%
echo.>>%header%
echo.    /// standard uniform description>>%header%
echo.    struct StandardUniformDesc>>%header%
echo.    {>>%header%
echo.        StandardUniformType type;   //^< uniform type>>%header%
echo.        const char        * name;   //^< uniform name>>%header%
echo.        size_t              size;   //^< uniform size in bytes>>%header%
echo.        bool                global; //^< global or per-object uniform>>%header%
echo.        const char        * desc;   //^< description text>>%header%
echo.>>%header%
for /f "eol=# delims=; tokens=1,2,3,4,5,6*" %%a in (%~dp0stdUniformMeta.txt) do echo.        static const StandardUniformDesc %%a;%%b//^< %%g>>%header%
echo.    };>>%header%
echo.>>%header%
echo.    inline const StandardUniformDesc ^& StandardUniformType::getDesc() const>>%header%
echo.    {>>%header%
echo.        static const StandardUniformDesc * DESCRIPTORS[] = {>>%header%
for /f "eol=# delims=; tokens=1,2,3,4,5,6*" %%a in (%~dp0stdUniformMeta.txt) do echo.            ^&StandardUniformDesc::%%a,>>%header%
echo.        };>>%header%
echo.        GN_CASSERT( (size_t)NUM_STANDARD_UNIFORMS == GN_ARRAY_COUNT(DESCRIPTORS) );>>%header%
echo.>>%header%
echo.        GN_ASSERT( *this ^< NUM_STANDARD_UNIFORMS );>>%header%
echo.        return *DESCRIPTORS[*this];>>%header%
echo.    }>>%header%
echo.>>%header%
echo.    /// get uniform name. Return NULL for invalid uniform type.>>%header%
echo.    inline const char * StandardUniformType::name() const>>%header%
echo.    {>>%header%
echo.        return isValid() ? getDesc().name : NULL;>>%header%
echo.    }>>%header%
echo }}>>%header%
echo.>>%header%
echo // *****************************************************************************>>%header%
echo //                                     EOF>>%header%
echo // *****************************************************************************>>%header%
echo #endif // __GN_GFX_STDUNIFORMDEF_H__>>%header%





REM ============================================================================
REM
REM Generate source
REM
REM ============================================================================

set cpp=%~dp0stdUniform.cpp

echo Generate standard uniform source: %cpp%

echo // *********************************************>%cpp%
echo // This is script generated cpp. DO NOT EDIT.>>%cpp%
echo // *********************************************>>%cpp%
echo #include "pch.h">>%cpp%
echo.>>%cpp%
for /f "eol=# delims=; tokens=1,2,3,4,5,6*" %%a in (%~dp0stdUniformMeta.txt) do echo.const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::%%a%%b = { GN::gfx::StandardUniformType::%%a%%b, "%%a"%%b, sizeof(%%c)%%d, %%e, "%%g" };>>%cpp%
echo.>>%cpp%

echo Done.
