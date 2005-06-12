/*
 * This file use to define all texture stage states
 *
 * NOTE : internelly used by render module, do NOT include it directly in your
 *        own header or source files.
 *
 * syntax : GN_DEFINE_TS(
 *               ts,
 *               defvalue-for-stage-0,
 *               defvalue-for-stage-1to7,
 *               d3dname,
 *               glname1, glname2 )
 *
 */

#ifndef GN_DEFINE_TS
#error "GN_DEFINE_TS not defined!"
#endif

//!
//! color operation
//!
GN_DEFINE_TS( COLOROP      , MODULATE        , DISABLE         , D3DTSS_COLOROP   , GL_COMBINE_RGB_ARB   , GL_RGB_SCALE_ARB      )

//!
//! color argument 0
//!
GN_DEFINE_TS( COLORARG0    , TEXTURE_COLOR  , TEXTURE_COLOR  , D3DTSS_COLORARG1 , GL_SOURCE0_RGB_ARB   , GL_OPERAND0_RGB_ARB   )

//!
//!  color argument 1
//!
GN_DEFINE_TS( COLORARG1    , CURRENT_COLOR  , CURRENT_COLOR  , D3DTSS_COLORARG2 , GL_SOURCE1_RGB_ARB   , GL_OPERAND1_RGB_ARB   )

//!
//! alpha operation
//!
GN_DEFINE_TS( ALPHAOP      , ARG0            , DISABLE         , D3DTSS_ALPHAOP   , GL_COMBINE_ALPHA_ARB , GL_ALPHA_SCALE        )

//!
//! alpha argument 0
//!
GN_DEFINE_TS( ALPHAARG0    , TEXTURE_ALPHA  , TEXTURE_ALPHA  , D3DTSS_ALPHAARG1 , GL_SOURCE0_ALPHA_ARB , GL_OPERAND0_ALPHA_ARB )

//!
//! alpha argument 1
//!
GN_DEFINE_TS( ALPHAARG1    , CURRENT_ALPHA  , CURRENT_ALPHA  , D3DTSS_ALPHAARG2 , GL_SOURCE1_ALPHA_ARB , GL_OPERAND1_ALPHA_ARB )
