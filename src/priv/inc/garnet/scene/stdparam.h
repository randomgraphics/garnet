// *********************************************
// This is script generated header. DO NOT EDIT.
// *********************************************

#ifndef __GN_SCENE_STDPARAM_H__
#define __GN_SCENE_STDPARAM_H__
// *****************************************************************************
/// \file
/// \brief   Define standard scene parameters
/// \author  chenli@@REDMOND (2009.1.20)
// *****************************************************************************

namespace GN { namespace scene
{
    /// Standard scene parameter type
    enum StandardSceneParameterType
    {
        SCENE_PARAM_MATRIX_PVW        , ///< proj * view * world
        SCENE_PARAM_MATRIX_PVW_INV    , ///< inv( proj * view * world )
        SCENE_PARAM_MATRIX_PVW_IT     , ///< inv( transpose( proj * view * world ) )
        SCENE_PARAM_MATRIX_PV         , ///< proj * view
        SCENE_PARAM_MATRIX_PV_INV     , ///< TBD
        SCENE_PARAM_MATRIX_PV_IT      , ///< TBD
        SCENE_PARAM_MATRIX_VW         , ///< view * world
        SCENE_PARAM_MATRIX_VW_INV     , ///< TBD
        SCENE_PARAM_MATRIX_VW_IT      , ///< TBD
        SCENE_PARAM_MATRIX_PROJ       , ///< TBD
        SCENE_PARAM_MATRIX_PROJ_INV   , ///< TBD
        SCENE_PARAM_MATRIX_PROJ_IT    , ///< TBD
        SCENE_PARAM_MATRIX_VIEW       , ///< TBD
        SCENE_PARAM_MATRIX_VIEW_INV   , ///< TBD
        SCENE_PARAM_MATRIX_VIEW_IT    , ///< TBD
        SCENE_PARAM_MATRIX_WORLD      , ///< TBD
        SCENE_PARAM_MATRIX_WORLD_INV  , ///< TBD
        SCENE_PARAM_MATRIX_WORLD_IT   , ///< TBD
        SCENE_PARAM_LIGHT0_POSITION   , ///< Position in world space of light 0
        SCENE_PARAM_LIGHT0_DIRECTION  , ///< Direction in world space of light 0 (undefined for point light)
        SCENE_PARAM_LIGHT0_DIFFUSE    , ///< Diffuse color of light 0
        SCENE_PARAM_LIGHT0_AMBIENT    , ///< Ambient color of light 0
        SCENE_PARAM_LIGHT0_SPECULAR   , ///< Specular color of light 0
        SCENE_PARAM_TIME              , ///< Time in seconds
        NUM_STANDARD_SCENE_PARAMETERS , ///< number of standard scene parameters
    };

    /// standard scene parameter description
    struct StandardSceneParameterDesc
    {
        const char * name;   ///< parameter name in effect
        size_t       size;   ///< parameter size in bytes
        bool         global; ///< true: global parameter; false: per-object parameter
        const char * desc;   ///< parameer description
    };

    /// get scene parameter scription
    inline const StandardSceneParameterDesc & getStandardSceneParameterName( size_t type )
    {
        static const StandardSceneParameterDesc descs[] = {
            { "MATRIX_PVW"        , sizeof(GN::Matrix44f) , false, "proj * view * world" },
            { "MATRIX_PVW_INV"    , sizeof(GN::Matrix44f) , false, "inv( proj * view * world )" },
            { "MATRIX_PVW_IT"     , sizeof(GN::Matrix44f) , false, "inv( transpose( proj * view * world ) )" },
            { "MATRIX_PV"         , sizeof(GN::Matrix44f) , true , "proj * view" },
            { "MATRIX_PV_INV"     , sizeof(GN::Matrix44f) , true , "TBD" },
            { "MATRIX_PV_IT"      , sizeof(GN::Matrix44f) , true , "TBD" },
            { "MATRIX_VW"         , sizeof(GN::Matrix44f) , false, "view * world" },
            { "MATRIX_VW_INV"     , sizeof(GN::Matrix44f) , false, "TBD" },
            { "MATRIX_VW_IT"      , sizeof(GN::Matrix44f) , false, "TBD" },
            { "MATRIX_PROJ"       , sizeof(GN::Matrix44f) , true , "TBD" },
            { "MATRIX_PROJ_INV"   , sizeof(GN::Matrix44f) , true , "TBD" },
            { "MATRIX_PROJ_IT"    , sizeof(GN::Matrix44f) , true , "TBD" },
            { "MATRIX_VIEW"       , sizeof(GN::Matrix44f) , true , "TBD" },
            { "MATRIX_VIEW_INV"   , sizeof(GN::Matrix44f) , true , "TBD" },
            { "MATRIX_VIEW_IT"    , sizeof(GN::Matrix44f) , true , "TBD" },
            { "MATRIX_WORLD"      , sizeof(GN::Matrix44f) , false, "TBD" },
            { "MATRIX_WORLD_INV"  , sizeof(GN::Matrix44f) , false, "TBD" },
            { "MATRIX_WORLD_IT"   , sizeof(GN::Matrix44f) , false, "TBD" },
            { "LIGHT0_POSITION"   , sizeof(GN::Vector4f)  , true , "Position in world space of light 0" },
            { "LIGHT0_DIRECTION"  , sizeof(GN::Vector4f)  , true , "Direction in world space of light 0 (undefined for point light)" },
            { "LIGHT0_DIFFUSE"    , sizeof(GN::Vector4f)  , true , "Diffuse color of light 0" },
            { "LIGHT0_AMBIENT"    , sizeof(GN::Vector4f)  , true , "Ambient color of light 0" },
            { "LIGHT0_SPECULAR"   , sizeof(GN::Vector4f)  , true , "Specular color of light 0" },
            { "TIME"              , sizeof(float)         , true , "Time in seconds" },
        };

        GN_ASSERT( type <= GN_ARRAY_COUNT(descs) );
        return descs[type];
    }
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_STDPARAM_H__
