// *********************************************
// This is script generated cpp. DO NOT EDIT.
// *********************************************
#include "pch.h"

const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PVW         = { GN::gfx::StandardUniformType::MATRIX_PVW        , "MATRIX_PVW"        , sizeof(GN::Matrix44f) , false, "proj * view * world" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PVW_INV     = { GN::gfx::StandardUniformType::MATRIX_PVW_INV    , "MATRIX_PVW_INV"    , sizeof(GN::Matrix44f) , false, "inv( proj * view * world )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PVW_IT      = { GN::gfx::StandardUniformType::MATRIX_PVW_IT     , "MATRIX_PVW_IT"     , sizeof(GN::Matrix44f) , false, "inv( transpose( proj * view * world ) )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PV          = { GN::gfx::StandardUniformType::MATRIX_PV         , "MATRIX_PV"         , sizeof(GN::Matrix44f) , true , "proj * view" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PV_INV      = { GN::gfx::StandardUniformType::MATRIX_PV_INV     , "MATRIX_PV_INV"     , sizeof(GN::Matrix44f) , true , "inv( proj * view )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PV_IT       = { GN::gfx::StandardUniformType::MATRIX_PV_IT      , "MATRIX_PV_IT"      , sizeof(GN::Matrix44f) , true , "inv( transpose( proj * view ) )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_VW          = { GN::gfx::StandardUniformType::MATRIX_VW         , "MATRIX_VW"         , sizeof(GN::Matrix44f) , false, "view * world" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_VW_INV      = { GN::gfx::StandardUniformType::MATRIX_VW_INV     , "MATRIX_VW_INV"     , sizeof(GN::Matrix44f) , false, "inv( view * world )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_VW_IT       = { GN::gfx::StandardUniformType::MATRIX_VW_IT      , "MATRIX_VW_IT"      , sizeof(GN::Matrix44f) , false, "inv( transpose( view * world ) )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PROJ        = { GN::gfx::StandardUniformType::MATRIX_PROJ       , "MATRIX_PROJ"       , sizeof(GN::Matrix44f) , true , "projection matrix" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PROJ_INV    = { GN::gfx::StandardUniformType::MATRIX_PROJ_INV   , "MATRIX_PROJ_INV"   , sizeof(GN::Matrix44f) , true , "inv( proj )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_PROJ_IT     = { GN::gfx::StandardUniformType::MATRIX_PROJ_IT    , "MATRIX_PROJ_IT"    , sizeof(GN::Matrix44f) , true , "inv( transpose( proj ) )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_VIEW        = { GN::gfx::StandardUniformType::MATRIX_VIEW       , "MATRIX_VIEW"       , sizeof(GN::Matrix44f) , true , "view matrix" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_VIEW_INV    = { GN::gfx::StandardUniformType::MATRIX_VIEW_INV   , "MATRIX_VIEW_INV"   , sizeof(GN::Matrix44f) , true , "inv( view )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_VIEW_IT     = { GN::gfx::StandardUniformType::MATRIX_VIEW_IT    , "MATRIX_VIEW_IT"    , sizeof(GN::Matrix44f) , true , "inv( transpose( view ) )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_WORLD       = { GN::gfx::StandardUniformType::MATRIX_WORLD      , "MATRIX_WORLD"      , sizeof(GN::Matrix44f) , false, "world matrix" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_WORLD_INV   = { GN::gfx::StandardUniformType::MATRIX_WORLD_INV  , "MATRIX_WORLD_INV"  , sizeof(GN::Matrix44f) , false, "inv( world )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::MATRIX_WORLD_IT    = { GN::gfx::StandardUniformType::MATRIX_WORLD_IT   , "MATRIX_WORLD_IT"   , sizeof(GN::Matrix44f) , false, "inv( transpose( world ) )" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::LIGHT0_POSITION    = { GN::gfx::StandardUniformType::LIGHT0_POSITION   , "LIGHT0_POSITION"   , sizeof(GN::Vector4f)  , true , "Position in world space of light 0" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::LIGHT0_DIRECTION   = { GN::gfx::StandardUniformType::LIGHT0_DIRECTION  , "LIGHT0_DIRECTION"  , sizeof(GN::Vector4f)  , true , "Direction in world space of light 0 (undefined for point light)" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::LIGHT0_DIFFUSE     = { GN::gfx::StandardUniformType::LIGHT0_DIFFUSE    , "LIGHT0_DIFFUSE"    , sizeof(GN::Vector4f)  , true , "Diffuse color of light 0" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::LIGHT0_AMBIENT     = { GN::gfx::StandardUniformType::LIGHT0_AMBIENT    , "LIGHT0_AMBIENT"    , sizeof(GN::Vector4f)  , true , "Ambient color of light 0" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::LIGHT0_SPECULAR    = { GN::gfx::StandardUniformType::LIGHT0_SPECULAR   , "LIGHT0_SPECULAR"   , sizeof(GN::Vector4f)  , true , "Specular color of light 0" };
const GN::gfx::StandardUniformDesc GN::gfx::StandardUniformDesc::TIME               = { GN::gfx::StandardUniformType::TIME              , "TIME"              , sizeof(float)         , true , "Time in seconds" };

