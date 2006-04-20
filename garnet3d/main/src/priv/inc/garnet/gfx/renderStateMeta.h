/*
 * This file use to define all rendering states
 *
 * NOTE : internally used by render module, do NOT include it directly in your
 *        own header or source files.
 *
 * syntax : GNGFX_DEFINE_RS( tag, type, defvalue, minValue, maxValue )
 *
 */

#ifndef GNGFX_DEFINE_RS
#error "GNGFX_DEFINE_RS not defined!"
#endif

//!
//! alpha testing enable, default is false
//!
GNGFX_DEFINE_RS( ALPHA_TEST      , ENUM             , RSV_FALSE         , RSV_FALSE         , RSV_TRUE      )

//!
//! alpha testing function, default is CMP_ALWAYS
//!
GNGFX_DEFINE_RS( ALPHA_FUNC      , ENUM             , RSV_CMP_ALWAYS   , RSV_CMP_NEVER     , RSV_CMP_ALWAYS )

//!
//! reference value for alpha test
//!
GNGFX_DEFINE_RS( ALPHA_REF       , INT              , 0                 , 0                 , 255           )

//!
//! alpha blending enable, default is false
//!
GNGFX_DEFINE_RS( BLENDING        , ENUM             , RSV_FALSE        , RSV_FALSE         , RSV_TRUE       )

//!
//! alpha blending source, default is BLEND_SA
//!
GNGFX_DEFINE_RS( BLEND_SRC       , ENUM             , RSV_BLEND_SA     , RSV_BLEND_ZERO    , RSV_BLEND_SAS  )

//!
//! alpha blending destination, default is BLEND_ISA
//!
GNGFX_DEFINE_RS( BLEND_DST       , ENUM             , RSV_BLEND_ISA    , RSV_BLEND_ZERO    , RSV_BLEND_SAS  )

//!
//! Color writeable mask, default is all channels.
//!
//! - bit 0 : red
//! - bit 1 : green
//! - bit 2 : blue
//! - bit 3 : alpha
//!
GNGFX_DEFINE_RS( COLOR0_WRITE    , INT              , 0xF              , 0                 , 0xF            )

#if GN_LEFT_HAND
//!
//! culling mode, default is CULL_CCW
//!
GNGFX_DEFINE_RS( CULL_MODE       , ENUM             , RSV_CULL_CCW     , RSV_CULL_NONE     , RSV_CULL_CCW   )
#else
//!
//! culling mode, default is CULL_CW
//!
GNGFX_DEFINE_RS( CULL_MODE       , ENUM             , RSV_CULL_CW      , RSV_CULL_NONE     , RSV_CULL_CCW   )
#endif

//!
//! depth testing enable, default is true
//!
GNGFX_DEFINE_RS( DEPTH_TEST      , ENUM             , RSV_TRUE         , RSV_FALSE         , RSV_TRUE       )

//!
//! depth testing function, default is CMP_LEQUAL
//!
GNGFX_DEFINE_RS( DEPTH_FUNC      , ENUM             , RSV_CMP_LEQUAL   , RSV_CMP_NEVER     , RSV_CMP_ALWAYS )

//!
//! depth writing flag, default is true
//!
GNGFX_DEFINE_RS( DEPTH_WRITE     , ENUM             , RSV_TRUE         , RSV_FALSE         , RSV_TRUE       )

//!
//! polyling mode, default is FILL_SOLID
//!
GNGFX_DEFINE_RS( FILL_MODE       , ENUM             , RSV_FILL_SOLID   , RSV_FILL_SOLID    , RSV_FILL_POINT )

//!
//! fog enable, default is false
//!
GNGFX_DEFINE_RS( FOG             , ENUM             , RSV_FALSE        , RSV_FALSE         , RSV_TRUE       )

//!
//! lighting enable, default is false
//!
GNGFX_DEFINE_RS( LIGHTING        , ENUM             , RSV_FALSE        , RSV_FALSE         , RSV_TRUE       )
