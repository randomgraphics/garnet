// *********************************************
// This is script generated header. DO NOT EDIT.
// *********************************************

#ifndef __GN_GFX_STDUNIFORMDEF_H__
#define __GN_GFX_STDUNIFORMDEF_H__
// *****************************************************************************
/// \file
/// \brief   Define standard uniform resources
/// \author  chenli@@REDMOND (2011/05/02)
// *****************************************************************************

namespace GN { namespace gfx
{
    /// Standard uniform definition
    struct StandardUniform
    {
        /// Standard Uniform Index
        struct Index
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
            GN_DEFINE_ENUM_CLASS_HELPERS( Index, ENUM )
        };

        /// Standard Uniform Descriptor
        struct Desc
        {
            int          index;  //< uniform index
            const char * name;   //< uniform name
            uint32       size;   //< uniform size in bytes
            bool         global; //< global or per-object uniform

            GN_API static const Desc MATRIX_PVW;        //< proj * view * world
            GN_API static const Desc MATRIX_PVW_INV;    //< inv( proj * view * world )
            GN_API static const Desc MATRIX_PVW_IT;     //< inv( transpose( proj * view * world ) )
            GN_API static const Desc MATRIX_PV;         //< proj * view
            GN_API static const Desc MATRIX_PV_INV;     //< inv( proj * view )
            GN_API static const Desc MATRIX_PV_IT;      //< inv( transpose( proj * view ) )
            GN_API static const Desc MATRIX_VW;         //< view * world
            GN_API static const Desc MATRIX_VW_INV;     //< inv( view * world )
            GN_API static const Desc MATRIX_VW_IT;      //< inv( transpose( view * world ) )
            GN_API static const Desc MATRIX_PROJ;       //< projection matrix
            GN_API static const Desc MATRIX_PROJ_INV;   //< inv( proj )
            GN_API static const Desc MATRIX_PROJ_IT;    //< inv( transpose( proj ) )
            GN_API static const Desc MATRIX_VIEW;       //< view matrix
            GN_API static const Desc MATRIX_VIEW_INV;   //< inv( view )
            GN_API static const Desc MATRIX_VIEW_IT;    //< inv( transpose( view ) )
            GN_API static const Desc MATRIX_WORLD;      //< world matrix
            GN_API static const Desc MATRIX_WORLD_INV;  //< inv( world )
            GN_API static const Desc MATRIX_WORLD_IT;   //< inv( transpose( world ) )
            GN_API static const Desc LIGHT0_POSITION;   //< Position in world space of light 0
            GN_API static const Desc LIGHT0_DIRECTION;  //< Direction in world space of light 0 (undefined for point light)
            GN_API static const Desc LIGHT0_DIFFUSE;    //< Diffuse color of light 0
            GN_API static const Desc LIGHT0_AMBIENT;    //< Ambient color of light 0
            GN_API static const Desc LIGHT0_SPECULAR;   //< Specular color of light 0
            GN_API static const Desc TIME;              //< Time in seconds
        };

        /// Check if the uniform index is valid or not.
        static bool sIsValidIndex( int i )
        {
            return 0 <= i && i < Index::NUM_STANDARD_UNIFORMS;
        }

        /// get uniform descriptor from index
        static const Desc * sIndex2Desc( int i )
        {
            static const Desc * DESCRIPTORS[] = {
                &Desc::MATRIX_PVW,
                &Desc::MATRIX_PVW_INV,
                &Desc::MATRIX_PVW_IT,
                &Desc::MATRIX_PV,
                &Desc::MATRIX_PV_INV,
                &Desc::MATRIX_PV_IT,
                &Desc::MATRIX_VW,
                &Desc::MATRIX_VW_INV,
                &Desc::MATRIX_VW_IT,
                &Desc::MATRIX_PROJ,
                &Desc::MATRIX_PROJ_INV,
                &Desc::MATRIX_PROJ_IT,
                &Desc::MATRIX_VIEW,
                &Desc::MATRIX_VIEW_INV,
                &Desc::MATRIX_VIEW_IT,
                &Desc::MATRIX_WORLD,
                &Desc::MATRIX_WORLD_INV,
                &Desc::MATRIX_WORLD_IT,
                &Desc::LIGHT0_POSITION,
                &Desc::LIGHT0_DIRECTION,
                &Desc::LIGHT0_DIFFUSE,
                &Desc::LIGHT0_AMBIENT,
                &Desc::LIGHT0_SPECULAR,
                &Desc::TIME,
            };
            GN_CASSERT( (uint32)Index::NUM_STANDARD_UNIFORMS == GN_ARRAY_COUNT(DESCRIPTORS) );

            if( 0 <= i && i < Index::NUM_STANDARD_UNIFORMS )
            {
                return DESCRIPTORS[i];
            }
            else
            {
                // Invalid index
                return NULL;
            }
        }

        /// get uniform descriptor from name.
        static const Desc * sName2Desc( const char * name )
        {
            for( int i = 0; i < Index::NUM_STANDARD_UNIFORMS; ++i )
            {
                const Desc * d = sIndex2Desc( i );
                if( 0 == str::compare( d->name, name ) ) return d;
            }
            return NULL;
        }
    };

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_STDUNIFORMDEF_H__
