/*
 * This file use to define all rendering states
 *
 * NOTE : internally used by render module, do NOT include it directly in your
 *        own header or source files.
 *
 * syntax : GN_DEFINE_RS( tag, defvalue )
 *
 */

#ifndef GN_DEFINE_RS
#error "GN_DEFINE_RS not defined!"
#endif

//!
//! alpha testing enable, default is false
//!
GN_DEFINE_RS( ALPHA_TEST      , FALSE )

//!
//! alpha testing function, default is CMP_ALWAYS
//!
GN_DEFINE_RS( ALPHA_FUNC      , CMP_ALWAYS  )

//!
//! alpha blending enable, default is false
//!
GN_DEFINE_RS( BLENDING        , TRUE )

//!
//! alpha blending source, default is BLEND_SA
//!
GN_DEFINE_RS( BLEND_SRC       , BLEND_SA    )

//!
//! alpha blending destination, default is BLEND_ISA
//!
GN_DEFINE_RS( BLEND_DST       , BLEND_ISA   )

# ifdef GN_LEFT_HAND
//!
//! culling mode, default is CULL_CCW
//!
GN_DEFINE_RS( CULL_MODE       , CULL_CCW    )
# else
//!
//! culling mode, default is CULL_CW
//!
GN_DEFINE_RS( CULL_MODE       , CULL_CW     )
# endif

//!
//! fog enable, default is false
//!
GN_DEFINE_RS( FOG             , FALSE )

//!
//! lighting enable, default is false
//!
GN_DEFINE_RS( LIGHTING        , FALSE )

//!
//! depth testing enable, default is true
//!
GN_DEFINE_RS( DEPTH_TEST      , TRUE  )

//!
//! depth testing function, default is CMP_LEQUAL
//!
GN_DEFINE_RS( DEPTH_FUNC      , CMP_LEQUAL  )

//!
//! depth writing flag, default is true
//!
GN_DEFINE_RS( DEPTH_WRITE     , TRUE  )

//!
//! polyling mode, default is FILL_SOLID
//!
GN_DEFINE_RS( FILL_MODE       , FILL_SOLID  )
