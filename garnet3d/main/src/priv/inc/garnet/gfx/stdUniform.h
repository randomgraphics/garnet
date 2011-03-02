// *********************************************
// This is script generated header. DO NOT EDIT.
// *********************************************

#ifndef __GN_GFX_STDUNIFORMDEF_H__
#define __GN_GFX_STDUNIFORMDEF_H__
// *****************************************************************************
/// \file
/// \brief   Define standard uniform resources
/// \author  chenli@@REDMOND (2011/03/02 ÖÜÈý)
// *****************************************************************************

namespace GN { namespace gfx
{
    // Forward declaration.
    struct StandardUniformDesc;

    /// Standard uniform type enumeration
    struct StandardUniformType
    {
        enum ENUM
        {
            MATRIX_PVW        , //< proj * view * world
            MATRIX_PVW_INV    , //< inv( proj * view * world )
            MATRIX_PVW_IT     , //< inv( transpose( proj * view * world ) )
            MATRIX_PV         , //< proj * view
            MATRIX_PV_INV     , //< inv( proj * view )
            MATRIX_PV_IT      , //< inv( transpose( proj * view ) )
            MATRIX_VW         , //< view * world
            MATRIX_VW_INV     , //< inv( view * world )
            MATRIX_VW_IT      , //< inv( transpose( view * world ) )
            MATRIX_PROJ       , //< projection matrix
            MATRIX_PROJ_INV   , //< inv( proj )
            MATRIX_PROJ_IT    , //< inv( transpose( proj ) )
            MATRIX_VIEW       , //< view matrix
            MATRIX_VIEW_INV   , //< inv( view )
            MATRIX_VIEW_IT    , //< inv( transpose( view ) )
            MATRIX_WORLD      , //< world matrix
            MATRIX_WORLD_INV  , //< inv( world )
            MATRIX_WORLD_IT   , //< inv( transpose( world ) )
            LIGHT0_POSITION   , //< Position in world space of light 0
            LIGHT0_DIRECTION  , //< Direction in world space of light 0 (undefined for point light)
            LIGHT0_DIFFUSE    , //< Diffuse color of light 0
            LIGHT0_AMBIENT    , //< Ambient color of light 0
            LIGHT0_SPECULAR   , //< Specular color of light 0
            TIME              , //< Time in seconds
            NUM_STANDARD_UNIFORMS , ///< number of standard uniforms
        };
        GN_DEFINE_ENUM_CLASS_HELPERS( StandardUniformType, ENUM )

        /// Check if the enum value is valid or not.
        bool isValid() const
        {
            return 0 <= (*this) && (*this) < NUM_STANDARD_UNIFORMS;
        }

        /// get uniform descriptor
        const StandardUniformDesc & getDesc() const;

        /// get uniform name. Return NULL for invalid uniform type.
        const char * name() const;
    };

    /// standard uniform description
    struct StandardUniformDesc
    {
        StandardUniformType type;   //< uniform type
        const char        * name;   //< uniform name
        size_t              size;   //< uniform size in bytes
        bool                global; //< global or per-object uniform
        const char        * desc;   //< description text

        static const StandardUniformDesc MATRIX_PVW;        //< proj * view * world
        static const StandardUniformDesc MATRIX_PVW_INV;    //< inv( proj * view * world )
        static const StandardUniformDesc MATRIX_PVW_IT;     //< inv( transpose( proj * view * world ) )
        static const StandardUniformDesc MATRIX_PV;         //< proj * view
        static const StandardUniformDesc MATRIX_PV_INV;     //< inv( proj * view )
        static const StandardUniformDesc MATRIX_PV_IT;      //< inv( transpose( proj * view ) )
        static const StandardUniformDesc MATRIX_VW;         //< view * world
        static const StandardUniformDesc MATRIX_VW_INV;     //< inv( view * world )
        static const StandardUniformDesc MATRIX_VW_IT;      //< inv( transpose( view * world ) )
        static const StandardUniformDesc MATRIX_PROJ;       //< projection matrix
        static const StandardUniformDesc MATRIX_PROJ_INV;   //< inv( proj )
        static const StandardUniformDesc MATRIX_PROJ_IT;    //< inv( transpose( proj ) )
        static const StandardUniformDesc MATRIX_VIEW;       //< view matrix
        static const StandardUniformDesc MATRIX_VIEW_INV;   //< inv( view )
        static const StandardUniformDesc MATRIX_VIEW_IT;    //< inv( transpose( view ) )
        static const StandardUniformDesc MATRIX_WORLD;      //< world matrix
        static const StandardUniformDesc MATRIX_WORLD_INV;  //< inv( world )
        static const StandardUniformDesc MATRIX_WORLD_IT;   //< inv( transpose( world ) )
        static const StandardUniformDesc LIGHT0_POSITION;   //< Position in world space of light 0
        static const StandardUniformDesc LIGHT0_DIRECTION;  //< Direction in world space of light 0 (undefined for point light)
        static const StandardUniformDesc LIGHT0_DIFFUSE;    //< Diffuse color of light 0
        static const StandardUniformDesc LIGHT0_AMBIENT;    //< Ambient color of light 0
        static const StandardUniformDesc LIGHT0_SPECULAR;   //< Specular color of light 0
        static const StandardUniformDesc TIME;              //< Time in seconds
    };

    inline const StandardUniformDesc & StandardUniformType::getDesc() const
    {
        static const StandardUniformDesc * DESCRIPTORS[] = {
            &StandardUniformDesc::MATRIX_PVW,
            &StandardUniformDesc::MATRIX_PVW_INV,
            &StandardUniformDesc::MATRIX_PVW_IT,
            &StandardUniformDesc::MATRIX_PV,
            &StandardUniformDesc::MATRIX_PV_INV,
            &StandardUniformDesc::MATRIX_PV_IT,
            &StandardUniformDesc::MATRIX_VW,
            &StandardUniformDesc::MATRIX_VW_INV,
            &StandardUniformDesc::MATRIX_VW_IT,
            &StandardUniformDesc::MATRIX_PROJ,
            &StandardUniformDesc::MATRIX_PROJ_INV,
            &StandardUniformDesc::MATRIX_PROJ_IT,
            &StandardUniformDesc::MATRIX_VIEW,
            &StandardUniformDesc::MATRIX_VIEW_INV,
            &StandardUniformDesc::MATRIX_VIEW_IT,
            &StandardUniformDesc::MATRIX_WORLD,
            &StandardUniformDesc::MATRIX_WORLD_INV,
            &StandardUniformDesc::MATRIX_WORLD_IT,
            &StandardUniformDesc::LIGHT0_POSITION,
            &StandardUniformDesc::LIGHT0_DIRECTION,
            &StandardUniformDesc::LIGHT0_DIFFUSE,
            &StandardUniformDesc::LIGHT0_AMBIENT,
            &StandardUniformDesc::LIGHT0_SPECULAR,
            &StandardUniformDesc::TIME,
        };
        GN_CASSERT( (size_t)NUM_STANDARD_UNIFORMS == GN_ARRAY_COUNT(DESCRIPTORS) );

        GN_ASSERT( *this < NUM_STANDARD_UNIFORMS );
        return *DESCRIPTORS[*this];
    }

    /// get uniform name. Return NULL for invalid uniform type.
    inline const char * StandardUniformType::name() const
    {
        return isValid() ? getDesc().name : NULL;
    }
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_STDUNIFORMDEF_H__
