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
echo.    /// Standard uniform defintion>>%header%
echo.    struct StandardUniform>>%header%
echo.    {>>%header%
echo.        /// Standard Uniform Index>>%header%
echo.        struct Index>>%header%
echo.        {>>%header%
echo.            enum ENUM>>%header%
echo.            {>>%header%
for /f "eol=# delims=; tokens=1,2,3,4,5,6*" %%a in (%~dp0stdUniformMeta.txt) do echo.                %%a%%b, //^< %%g>>%header%
echo.                NUM_STANDARD_UNIFORMS , ///^< number of standard uniforms>>%header%
echo.            };>>%header%
echo.            GN_DEFINE_ENUM_CLASS_HELPERS( Index, ENUM )>>%header%
echo.        };>>%header%
echo.>>%header%
echo.        /// Standard Uniform Descriptor>>%header%
echo.        struct Desc>>%header%
echo.        {>>%header%
echo.            int          index;  //^< uniform index>>%header%
echo.            const char * name;   //^< uniform name>>%header%
echo.            uint32       size;   //^< uniform size in bytes>>%header%
echo.            bool         global; //^< global or per-object uniform>>%header%
echo.>>%header%
for /f "eol=# delims=; tokens=1,2,3,4,5,6*" %%a in (%~dp0stdUniformMeta.txt) do echo.            GN_API static const Desc %%a;%%b//^< %%g>>%header%
echo.        };>>%header%
echo.>>%header%
echo.        /// Check if the uniform index is valid or not.>>%header%
echo.        static bool sIsValidIndex( int i )>>%header%
echo.        {>>%header%
echo.            return 0 ^<= i ^&^& i ^< Index::NUM_STANDARD_UNIFORMS;>>%header%
echo.        }>>%header%
echo.>>%header%
echo.        /// get uniform descriptor from index>>%header%
echo.        static const Desc * sIndex2Desc( int i )>>%header%
echo.        {>>%header%
echo.            static const Desc * DESCRIPTORS[] = {>>%header%
for /f "eol=# delims=; tokens=1,2,3,4,5,6*" %%a in (%~dp0stdUniformMeta.txt) do echo.                ^&Desc::%%a,>>%header%
echo.            };>>%header%
echo.            GN_CASSERT( (uint32)Index::NUM_STANDARD_UNIFORMS == GN_ARRAY_COUNT(DESCRIPTORS) );>>%header%
echo.>>%header%
echo.            if( 0 ^<= i ^&^& i ^< Index::NUM_STANDARD_UNIFORMS )>>%header%
echo.            {>>%header%
echo.                return DESCRIPTORS[i];>>%header%
echo.            }>>%header%
echo.            else>>%header%
echo.            {>>%header%
echo.                // Invalid index>>%header%
echo.                return NULL;>>%header%
echo.            }>>%header%
echo.        }>>%header%
echo.>>%header%
echo.        /// get uniform descriptor from name.>>%header%
echo.        static const Desc * sName2Desc( const char * name )>>%header%
echo.        {>>%header%
echo.            for( int i = 0; i ^< Index::NUM_STANDARD_UNIFORMS; ++i )>>%header%
echo.            {>>%header%
echo.                const Desc * d = sIndex2Desc( i );>>%header%
echo.                if( 0 == stringCompare( d-^>name, name ) ) return d;>>%header%
echo.            }>>%header%
echo.            return NULL;>>%header%
echo.        }>>%header%
echo.    };>>%header%
echo.>>%header%
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
for /f "eol=# delims=; tokens=1,2,3,4,5,6*" %%a in (%~dp0stdUniformMeta.txt) do echo.GN_API const GN::gfx::StandardUniform::Desc GN::gfx::StandardUniform::Desc::%%a%%b = { GN::gfx::StandardUniform::Index::%%a%%b, "%%a"%%b, sizeof(%%c)%%d, %%e };>>%cpp%
echo.>>%cpp%

echo Done.
