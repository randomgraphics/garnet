// Define vertex format
//
// syntax : GN_VTXSEM(
//               semantic,
//               d3ddecl_usage,
//               d3ddecl_usage_index,
//               gl_conventional_vertex_array_name,
//               gl_vertex_attribute_array_index,
//               cg_bind_location )

//!
//! vertex position
//!
GN_VTXSEM( COORD    , D3DDECLUSAGE_POSITION    , 0 , GL_VERTEX_ARRAY              , -1 , CG_POSITION0 )

//!
//! vertex weight
//!
GN_VTXSEM( WEIGHT   , D3DDECLUSAGE_BLENDWEIGHT , 0 , -1                           ,  1 , CG_BLENDWEIGHT0 )

//!
//! vertex normal
//!
GN_VTXSEM( NORMAL   , D3DDECLUSAGE_NORMAL      , 0 , GL_NORMAL_ARRAY              , -1 , CG_NORMAL0 )

//!
//! vertex color 0 ( diffuse color )
//!
GN_VTXSEM( COLOR0   , D3DDECLUSAGE_COLOR       , 0 , GL_COLOR_ARRAY               , -1 , CG_COLOR0 )

//!
//! vertex color 1 ( specular color )
//!
GN_VTXSEM( COLOR1   , D3DDECLUSAGE_COLOR       , 1 , GL_SECONDARY_COLOR_ARRAY_EXT , -1 , CG_COLOR1 )

//!
//! vertex fog coordination
//!
GN_VTXSEM( FOGCOORD , D3DDECLUSAGE_FOG         , 0 , GL_FOG_COORDINATE_ARRAY_EXT  , -1 , CG_FOG0 )

//!
//! vertex tangent
//!
GN_VTXSEM( TANGENT  , D3DDECLUSAGE_TANGENT     , 0 , -1                           ,  6 , CG_TANGENT0 )

//!
//! vertex binormal
//!
GN_VTXSEM( BINORMAL , D3DDECLUSAGE_BINORMAL    , 0 , -1                           ,  7 , CG_BINORMAL0 )

//!
//! vertex texture 0
//!
GN_VTXSEM( TEX0     , D3DDECLUSAGE_TEXCOORD    , 0 , GL_TEXTURE_COORD_ARRAY       , -1 , CG_TEXCOORD0 )

//!
//! vertex texture 1
//!
GN_VTXSEM( TEX1     , D3DDECLUSAGE_TEXCOORD    , 1 , GL_TEXTURE_COORD_ARRAY       , -1 , CG_TEXCOORD1 )

//!
//! vertex texture 2
//!
GN_VTXSEM( TEX2     , D3DDECLUSAGE_TEXCOORD    , 2 , GL_TEXTURE_COORD_ARRAY       , -1 , CG_TEXCOORD2 )

//!
//! vertex texture 3
//!
GN_VTXSEM( TEX3     , D3DDECLUSAGE_TEXCOORD    , 3 , GL_TEXTURE_COORD_ARRAY       , -1 , CG_TEXCOORD3 )

//!
//! vertex texture 4
//!
GN_VTXSEM( TEX4     , D3DDECLUSAGE_TEXCOORD    , 4 , GL_TEXTURE_COORD_ARRAY       , -1 , CG_TEXCOORD4 )

//!
//! vertex texture 5
//!
GN_VTXSEM( TEX5     , D3DDECLUSAGE_TEXCOORD    , 5 , GL_TEXTURE_COORD_ARRAY       , -1 , CG_TEXCOORD5 )

//!
//! vertex texture 6
//!
GN_VTXSEM( TEX6     , D3DDECLUSAGE_TEXCOORD    , 6 , GL_TEXTURE_COORD_ARRAY       , -1 , CG_TEXCOORD6 )

//!
//! vertex texture 7
//!
GN_VTXSEM( TEX7     , D3DDECLUSAGE_TEXCOORD    , 7 , GL_TEXTURE_COORD_ARRAY       , -1 , CG_TEXCOORD7 )
