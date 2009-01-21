@echo off
@echo Generate headers of standard scene parameters...

set header="%~dp0..\..\inc\garnet\scene\stdparam.h"

echo // *********************************************>%header%
echo // This is script generated header. DO NOT EDIT.>>%header%
echo // *********************************************>>%header%
echo.>>%header%
echo #ifndef __GN_SCENE_STDPARAM_H__>>%header%
echo #define __GN_SCENE_STDPARAM_H__>>%header%
echo // *****************************************************************************>>%header%
echo /// \file>>%header%
echo /// \brief   Define standard scene parameters>>%header%
echo /// \author  chenli@@REDMOND (2009.1.20)>>%header%
echo // *****************************************************************************>>%header%
echo.>>%header%
echo namespace GN { namespace scene>>%header%
echo {>>%header%
echo.    /// Standard scene parameter type>>%header%
echo.    enum StandardSceneParameterType>>%header%
echo.    {>>%header%
for /f "eol=; delims=, tokens=1,2,3,4,5*" %%a in (sceneParamMeta.txt) do echo.        SCENE_PARAM_%%a%%b, ///^< %%f>>%header%
echo.        NUM_STANDARD_SCENE_PARAMETERS , ///^< number of standard scene parameters>>%header%
echo.    };>>%header%
echo.>>%header%
echo.    /// standard scene parameter description>>%header%
echo.    struct StandardSceneParameterDesc>>%header%
echo.    {>>%header%
echo.        const char * name;   ///^< parameter name in effect>>%header%
echo.        size_t       size;   ///^< parameter size in bytes>>%header%
echo.        bool         global; ///^< true: global parameter^; false: per-object parameter>>%header%
echo.        const char * desc;   ///^< parameer description>>%header%
echo.    };>>%header%
echo.>>%header%
echo.    /// get scene parameter scription>>%header%
echo.    inline const StandardSceneParameterDesc ^& getStandardSceneParameterName( size_t type )>>%header%
echo.    {>>%header%
echo.        static const StandardSceneParameterDesc descs[] = {>>%header%
for /f "eol=; delims=, tokens=1,2,3,4,5*" %%a in (sceneParamMeta.txt) do echo.            { "%%a"%%b, sizeof(%%c)%%d, %%e, "%%f" },>>%header%
echo.        };>>%header%
echo.>>%header%
echo.        GN_ASSERT( type ^<= GN_ARRAY_COUNT(descs) );>>%header%
echo.        return descs[type];>>%header%
echo.    }>>%header%
echo }}>>%header%
echo.>>%header%
echo // *****************************************************************************>>%header%
echo //                                     EOF>>%header%
echo // *****************************************************************************>>%header%
echo #endif // __GN_SCENE_STDPARAM_H__>>%header%

@echo Done.


:print desc