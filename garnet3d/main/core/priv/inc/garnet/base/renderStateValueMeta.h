// This file use to define all rendering state values
//
// NOTE : internelly used by render module, do NOT include it directly in your
//        own header or source files.
//
// syntax : GN_DEFINE_RSV( tag, d3dval, glval )

#ifndef GN_DEFINE_RSV
#error "GN_DEFINE_RSV not defined!"
#endif

//
// bool flags
//
GN_DEFINE_RSV( FALSE         , 0L                    , GL_FALSE               ) //!< true
GN_DEFINE_RSV( TRUE          , 1L                    , GL_TRUE                ) //!< false

//
// comparision flags
//
GN_DEFINE_RSV( CMP_NEVER     , D3DCMP_NEVER          , GL_NEVER               ) //!< always failed
GN_DEFINE_RSV( CMP_LESS      , D3DCMP_LESS           , GL_LESS                ) //!< <
GN_DEFINE_RSV( CMP_EQUAL     , D3DCMP_EQUAL          , GL_EQUAL               ) //!< ==
GN_DEFINE_RSV( CMP_LEQUAL    , D3DCMP_LESSEQUAL      , GL_LEQUAL              ) //!< <=
GN_DEFINE_RSV( CMP_GREATER   , D3DCMP_GREATER        , GL_GREATER             ) //!< >
GN_DEFINE_RSV( CMP_NEQUAL    , D3DCMP_NOTEQUAL       , GL_NOTEQUAL            ) //!< !=
GN_DEFINE_RSV( CMP_GEQUAL    , D3DCMP_GREATEREQUAL   , GL_GEQUAL              ) //!< >=
GN_DEFINE_RSV( CMP_ALWAYS    , D3DCMP_ALWAYS         , GL_ALWAYS              ) //!< always pass

//
// blending functions
//
GN_DEFINE_RSV( BLEND_ZERO    , D3DBLEND_ZERO         , GL_ZERO                ) //!< zero
GN_DEFINE_RSV( BLEND_ONE     , D3DBLEND_ONE          , GL_ONE                 ) //!< one
GN_DEFINE_RSV( BLEND_SC      , D3DBLEND_SRCCOLOR     , GL_SRC_COLOR           ) //!< src color
GN_DEFINE_RSV( BLEND_ISC     , D3DBLEND_INVSRCCOLOR  , GL_ONE_MINUS_SRC_COLOR ) //!< inverse src color
GN_DEFINE_RSV( BLEND_SA      , D3DBLEND_SRCALPHA     , GL_SRC_ALPHA           ) //!< src alpha
GN_DEFINE_RSV( BLEND_ISA     , D3DBLEND_INVSRCALPHA  , GL_ONE_MINUS_SRC_ALPHA ) //!< inverse src alpha
GN_DEFINE_RSV( BLEND_DC      , D3DBLEND_DESTCOLOR    , GL_DST_COLOR           ) //!< dst color
GN_DEFINE_RSV( BLEND_IDC     , D3DBLEND_INVDESTCOLOR , GL_ONE_MINUS_DST_COLOR ) //!< inverse dst color
GN_DEFINE_RSV( BLEND_DA      , D3DBLEND_DESTALPHA    , GL_DST_ALPHA           ) //!< dst alpha
GN_DEFINE_RSV( BLEND_IDA     , D3DBLEND_INVDESTALPHA , GL_ONE_MINUS_DST_ALPHA ) //!< inverse dst alpha
GN_DEFINE_RSV( BLEND_SAS     , D3DBLEND_SRCALPHASAT  , GL_SRC_ALPHA_SATURATE  ) //!< src alpha saturate

//
// culling mode
//
GN_DEFINE_RSV( CULL_CW       , D3DCULL_CW            , GL_BACK  ) //!< Ë³Ê±Õë
GN_DEFINE_RSV( CULL_CCW      , D3DCULL_CCW           , GL_FRONT ) //!< ÄæÊ±Õë
GN_DEFINE_RSV( CULL_NONE     , D3DCULL_NONE          , GL_FALSE ) //!< no culling

// polygon filling mode
GN_DEFINE_RSV( FILL_SOLID    , D3DFILL_SOLID         , GL_FILL  ) //!< solid mode
GN_DEFINE_RSV( FILL_LINE     , D3DFILL_WIREFRAME     , GL_LINE  ) //!< line mode
GN_DEFINE_RSV( FILL_POINT    , D3DFILL_POINT         , GL_POINT ) //!< point mode
