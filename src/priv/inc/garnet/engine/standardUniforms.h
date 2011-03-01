// *********************************************
// This is script generated header. DO NOT EDIT.
// *********************************************

#ifndef __GN_ENGINE_STANDARUNIFORMS_H__
#define __GN_ENGINE_STANDARUNIFORMS_H__
// *****************************************************************************
/// \file
/// \brief   Define standard scene parameters
/// \author  chenli@@REDMOND (2011/02/28 ÷‹“ª)
// *****************************************************************************

namespace GN { namespace engine
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
            MATRIX_PV_INV     , ///< inv( proj * view )
            MATRIX_PV_IT      , ///< inv( transpose( proj * view ) )
            MATRIX_VW         , ///< view * world
            MATRIX_VW_INV     , ///< inv( view * world )
            MATRIX_VW_IT      , ///< inv( transpose( view * world ) )
            MATRIX_PROJ       , ///< projection matrix
            MATRIX_PROJ_INV   , ///< inv( proj )
            MATRIX_PROJ_IT    , ///< inv( transpose( proj ) )
            MATRIX_VIEW       , ///< view matrix
            MATRIX_VIEW_INV   , ///< inv( view )
            MATRIX_VIEW_IT    , ///< inv( transpose( view ) )
            MATRIX_WORLD      , ///< world matrix
            MATRIX_WORLD_INV  , ///< inv( world )
            MATRIX_WORLD_IT   , ///< inv( transpose( world ) )
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
                { "MATRIX_PV_INV"     , sizeof(GN::Matrix44f) , true , "inv( proj * view )" },
                { "MATRIX_PV_IT"      , sizeof(GN::Matrix44f) , true , "inv( transpose( proj * view ) )" },
                { "MATRIX_VW"         , sizeof(GN::Matrix44f) , false, "view * world" },
                { "MATRIX_VW_INV"     , sizeof(GN::Matrix44f) , false, "inv( view * world )" },
                { "MATRIX_VW_IT"      , sizeof(GN::Matrix44f) , false, "inv( transpose( view * world ) )" },
                { "MATRIX_PROJ"       , sizeof(GN::Matrix44f) , true , "projection matrix" },
                { "MATRIX_PROJ_INV"   , sizeof(GN::Matrix44f) , true , "inv( proj )" },
                { "MATRIX_PROJ_IT"    , sizeof(GN::Matrix44f) , true , "inv( transpose( proj ) )" },
                { "MATRIX_VIEW"       , sizeof(GN::Matrix44f) , true , "view matrix" },
                { "MATRIX_VIEW_INV"   , sizeof(GN::Matrix44f) , true , "inv( view )" },
                { "MATRIX_VIEW_IT"    , sizeof(GN::Matrix44f) , true , "inv( transpose( view ) )" },
                { "MATRIX_WORLD"      , sizeof(GN::Matrix44f) , false, "world matrix" },
                { "MATRIX_WORLD_INV"  , sizeof(GN::Matrix44f) , false, "inv( world )" },
                { "MATRIX_WORLD_IT"   , sizeof(GN::Matrix44f) , false, "inv( transpose( world ) )" },
                { "LIGHT0_POSITION"   , sizeof(GN::Vector4f)  , true , "Position in world space of light 0" },
                { "LIGHT0_DIRECTION"  , sizeof(GN::Vector4f)  , true , "Direction in world space of light 0 (undefined for point light)" },
                { "LIGHT0_DIFFUSE"    , sizeof(GN::Vector4f)  , true , "Diffuse color of light 0" },
                { "LIGHT0_AMBIENT"    , sizeof(GN::Vector4f)  , true , "Ambient color of light 0" },
                { "LIGHT0_SPECULAR"   , sizeof(GN::Vector4f)  , true , "Specular color of light 0" },
                { "TIME"              , sizeof(float)         , true , "Time in seconds" },
            };

            GN_ASSERT( *this <= GN_ARRAY_COUNT(DESCRIPTORS) );
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
#endif // __GN_ENGINE_STANDARUNIFORMS_H__
