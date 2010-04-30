// *********************************************
// This is script generated header. DO NOT EDIT.
// *********************************************

#ifndef __GN_UTIL_STANDARUNIFORMS_H__
#define __GN_UTIL_STANDARUNIFORMS_H__
// *****************************************************************************
/// \file
/// \brief   Define standard scene parameters
/// \author  chenli@@REDMOND (Fri 09/11/2009)
// *****************************************************************************

namespace GN { namespace util
{
    /// standard uniform description
    struct StandardUniformDesc
    {
        const char * name;   ///< parameter name in effect
        size_t       size;   ///< parameter size in bytes
        bool         global; ///< true: global parameter; false: per visual node parameter
        const char * desc;   ///< parameter description
    };

    /// Standard uniform type enumeration
    struct StandardUniformType
    {
        enum ENUM
        {
            MATRIX_PVW        , ///< proj * view * world
            MATRIX_PVW_INV    , ///< inv( proj * view * world )
            MATRIX_PVW_IT     , ///< inv( transpose( proj * view * world ) )
            MATRIX_PV         , ///< proj * view
            MATRIX_PV_INV     , ///< TBD
            MATRIX_PV_IT      , ///< TBD
            MATRIX_VW         , ///< view * world
            MATRIX_VW_INV     , ///< TBD
            MATRIX_VW_IT      , ///< TBD
            MATRIX_PROJ       , ///< TBD
            MATRIX_PROJ_INV   , ///< TBD
            MATRIX_PROJ_IT    , ///< TBD
            MATRIX_VIEW       , ///< TBD
            MATRIX_VIEW_INV   , ///< TBD
            MATRIX_VIEW_IT    , ///< TBD
            MATRIX_WORLD      , ///< TBD
            MATRIX_WORLD_INV  , ///< TBD
            MATRIX_WORLD_IT   , ///< TBD
            LIGHT0_POSITION   , ///< Position in world space of light 0
            LIGHT0_DIRECTION  , ///< Direction in world space of light 0 (undefined for point light)
            LIGHT0_DIFFUSE    , ///< Diffuse color of light 0
            LIGHT0_AMBIENT    , ///< Ambient color of light 0
            LIGHT0_SPECULAR   , ///< Specular color of light 0
            TIME              , ///< Time in seconds
            NUM_STANDARD_UNIFORMS , ///< number of standard uniforms
        };
        GN_DEFINE_ENUM_CLASS_HELPERS( StandardUniformType, ENUM )

        /// Check if the enum value is valid or not.
        bool isValid() const
        {
            return 0 <= (*this) && (*this) <= NUM_STANDARD_UNIFORMS;
        }

        /// get uniform descriptor
        const StandardUniformDesc & getDesc() const
        {
            static const StandardUniformDesc DESCRIPTORS[] = {
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
            GN_CASSERT( GN_ARRAY_COUNT(DESCRIPTORS) == NUM_STANDARD_UNIFORMS );
            GN_ASSERT( *this < NUM_STANDARD_UNIFORMS );
            return DESCRIPTORS[*this];
        }

        /// get uniform name. Return NULL for invalid uniform type.
        const char * name() const
        {
            return isValid() ? getDesc().name : NULL;
        }
    };

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_STANDARUNIFORMS_H__
