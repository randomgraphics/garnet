// This file use to define all rendering state values
//
// NOTE : internelly used by render module, do NOT include it directly in your
//        own header or source files.
//
// syntax : GNGFX_DEFINE_RSV( tag, d3dval, glval )

#ifndef GNGFX_DEFINE_RSV
#error "GNGFX_DEFINE_RSV not defined!"
#endif

//
// bool flags
//
GNGFX_DEFINE_RSV( FALSE         , 0L                    , GL_FALSE               ) //!< true
GNGFX_DEFINE_RSV( TRUE          , 1L                    , GL_TRUE                ) //!< false

//
// comparision flags
//
GNGFX_DEFINE_RSV( CMP_NEVER     , D3DCMP_NEVER          , GL_NEVER               ) //!< always failed
GNGFX_DEFINE_RSV( CMP_LESS      , D3DCMP_LESS           , GL_LESS                ) //!< <
GNGFX_DEFINE_RSV( CMP_EQUAL     , D3DCMP_EQUAL          , GL_EQUAL               ) //!< ==
GNGFX_DEFINE_RSV( CMP_LEQUAL    , D3DCMP_LESSEQUAL      , GL_LEQUAL              ) //!< <=
GNGFX_DEFINE_RSV( CMP_GREATER   , D3DCMP_GREATER        , GL_GREATER             ) //!< >
GNGFX_DEFINE_RSV( CMP_NEQUAL    , D3DCMP_NOTEQUAL       , GL_NOTEQUAL            ) //!< !=
GNGFX_DEFINE_RSV( CMP_GEQUAL    , D3DCMP_GREATEREQUAL   , GL_GEQUAL              ) //!< >=
GNGFX_DEFINE_RSV( CMP_ALWAYS    , D3DCMP_ALWAYS         , GL_ALWAYS              ) //!< always pass

//
// blending functions
//
GNGFX_DEFINE_RSV( BLEND_ZERO    , D3DBLEND_ZERO         , GL_ZERO                ) //!< zero
GNGFX_DEFINE_RSV( BLEND_ONE     , D3DBLEND_ONE          , GL_ONE                 ) //!< one
GNGFX_DEFINE_RSV( BLEND_SC      , D3DBLEND_SRCCOLOR     , GL_SRC_COLOR           ) //!< src color
GNGFX_DEFINE_RSV( BLEND_ISC     , D3DBLEND_INVSRCCOLOR  , GL_ONE_MINUS_SRC_COLOR ) //!< inverse src color
GNGFX_DEFINE_RSV( BLEND_SA      , D3DBLEND_SRCALPHA     , GL_SRC_ALPHA           ) //!< src alpha
GNGFX_DEFINE_RSV( BLEND_ISA     , D3DBLEND_INVSRCALPHA  , GL_ONE_MINUS_SRC_ALPHA ) //!< inverse src alpha
GNGFX_DEFINE_RSV( BLEND_DC      , D3DBLEND_DESTCOLOR    , GL_DST_COLOR           ) //!< dst color
GNGFX_DEFINE_RSV( BLEND_IDC     , D3DBLEND_INVDESTCOLOR , GL_ONE_MINUS_DST_COLOR ) //!< inverse dst color
GNGFX_DEFINE_RSV( BLEND_DA      , D3DBLEND_DESTALPHA    , GL_DST_ALPHA           ) //!< dst alpha
GNGFX_DEFINE_RSV( BLEND_IDA     , D3DBLEND_INVDESTALPHA , GL_ONE_MINUS_DST_ALPHA ) //!< inverse dst alpha
GNGFX_DEFINE_RSV( BLEND_SAS     , D3DBLEND_SRCALPHASAT  , GL_SRC_ALPHA_SATURATE  ) //!< src alpha saturate

//
// culling mode
//
GNGFX_DEFINE_RSV( CULL_CW       , D3DCULL_CW            , GL_BACK  ) //!< Ë³Ê±Õë
GNGFX_DEFINE_RSV( CULL_CCW      , D3DCULL_CCW           , GL_FRONT ) //!< ÄæÊ±Õë
GNGFX_DEFINE_RSV( CULL_NONE     , D3DCULL_NONE          , GL_FALSE ) //!< no culling

// polygon filling mode
GNGFX_DEFINE_RSV( FILL_SOLID    , D3DFILL_SOLID         , GL_FILL  ) //!< solid mode
GNGFX_DEFINE_RSV( FILL_LINE     , D3DFILL_WIREFRAME     , GL_LINE  ) //!< line mode
GNGFX_DEFINE_RSV( FILL_POINT    , D3DFILL_POINT         , GL_POINT ) //!< point mode

GNGFX_DEFINE_RSV( EMPTY         , 0                     , 0        ) //!< empty/unused/undefine
