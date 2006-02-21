#ifndef GN_COLOR_FORMAT
#error "GN_COLOR_FORMAT not defined"
#endif

//               format                         bits               swizzle         ch_0                   ch_1                   ch_2                   ch_3
//               |                               |                   |             shift                  shift                  shift                  shift
//               |                               |                   |             |     bits             |     bits             |     bits             |     bits
//               |                               |                   |             |     |     type       |     |     type       |     |     type       |     |     type
//               |                               |                   |             |     |     |          |     |     |          |     |     |          |     |     |

// RGBA_32_32_32_32
GN_COLOR_FORMAT( RGBA_32_32_32_32_UNORM       , 32*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM), CH(96  , 32  , UNORM) ) )
GN_COLOR_FORMAT( RGBA_32_32_32_32_SNORM       , 32*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 32  , SNORM), CH(32  , 32  , SNORM), CH(64  , 32  , SNORM), CH(96  , 32  , SNORM) ) )
GN_COLOR_FORMAT( RGBA_32_32_32_32_UINT        , 32*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 32  , UINT ), CH(32  , 32  , UINT ), CH(64  , 32  , UINT ), CH(96  , 32  , UINT ) ) )
GN_COLOR_FORMAT( RGBA_32_32_32_32_SINT        , 32*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 32  , SINT ), CH(32  , 32  , SINT ), CH(64  , 32  , SINT ), CH(96  , 32  , SINT ) ) )
GN_COLOR_FORMAT( RGBA_32_32_32_32_FLOAT       , 32*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT), CH(64  , 32  , FLOAT), CH(96  , 32  , FLOAT) ) )

// BGRA_32_32_32_32
GN_COLOR_FORMAT( BGRA_32_32_32_32_UNORM       , 32*4  , CH4( SWIZZLE(B,G,R,A) , CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM), CH(96  , 32  , UNORM) ) )
GN_COLOR_FORMAT( BGRA_32_32_32_32_SNORM       , 32*4  , CH4( SWIZZLE(B,G,R,A) , CH(0   , 32  , SNORM), CH(32  , 32  , SNORM), CH(64  , 32  , SNORM), CH(96  , 32  , SNORM) ) )
GN_COLOR_FORMAT( BGRA_32_32_32_32_UINT        , 32*4  , CH4( SWIZZLE(B,G,R,A) , CH(0   , 32  , UINT ), CH(32  , 32  , UINT ), CH(64  , 32  , UINT ), CH(96  , 32  , UINT ) ) )
GN_COLOR_FORMAT( BGRA_32_32_32_32_SINT        , 32*4  , CH4( SWIZZLE(B,G,R,A) , CH(0   , 32  , SINT ), CH(32  , 32  , SINT ), CH(64  , 32  , SINT ), CH(96  , 32  , SINT ) ) )
GN_COLOR_FORMAT( BGRA_32_32_32_32_FLOAT       , 32*4  , CH4( SWIZZLE(B,G,R,A) , CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT), CH(64  , 32  , FLOAT), CH(96  , 32  , FLOAT) ) )

// RGBX_32_32_32_32
GN_COLOR_FORMAT( RGBX_32_32_32_32_UNORM       , 32*4  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM)                        ) )
GN_COLOR_FORMAT( RGBX_32_32_32_32_SNORM       , 32*4  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , SNORM), CH(32  , 32  , SNORM), CH(64  , 32  , SNORM)                        ) )
GN_COLOR_FORMAT( RGBX_32_32_32_32_UINT        , 32*4  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , UINT ), CH(32  , 32  , UINT ), CH(64  , 32  , UINT )                        ) )
GN_COLOR_FORMAT( RGBX_32_32_32_32_SINT        , 32*4  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , SINT ), CH(32  , 32  , SINT ), CH(64  , 32  , SINT )                        ) )
GN_COLOR_FORMAT( RGBX_32_32_32_32_FLOAT       , 32*4  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT), CH(64  , 32  , FLOAT)                        ) )

// BGRX_32_32_32_32
GN_COLOR_FORMAT( BGRX_32_32_32_32_UNORM       , 32*4  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_32_32_32_32_SNORM       , 32*4  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , SNORM), CH(32  , 32  , SNORM), CH(64  , 32  , SNORM)                        ) )
GN_COLOR_FORMAT( BGRX_32_32_32_32_UINT        , 32*4  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , UINT ), CH(32  , 32  , UINT ), CH(64  , 32  , UINT )                        ) )
GN_COLOR_FORMAT( BGRX_32_32_32_32_SINT        , 32*4  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , SINT ), CH(32  , 32  , SINT ), CH(64  , 32  , SINT )                        ) )
GN_COLOR_FORMAT( BGRX_32_32_32_32_FLOAT       , 32*4  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT), CH(64  , 32  , FLOAT)                        ) )

// RGB_32_32_32
GN_COLOR_FORMAT( RGB_32_32_32_UNORM           , 32*3  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM)                        ) )
GN_COLOR_FORMAT( RGB_32_32_32_SNORM           , 32*3  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , SNORM), CH(32  , 32  , SNORM), CH(64  , 32  , SNORM)                        ) )
GN_COLOR_FORMAT( RGB_32_32_32_UINT            , 32*3  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , UINT ), CH(32  , 32  , UINT ), CH(64  , 32  , UINT )                        ) )
GN_COLOR_FORMAT( RGB_32_32_32_SINT            , 32*3  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , SINT ), CH(32  , 32  , SINT ), CH(64  , 32  , SINT )                        ) )
GN_COLOR_FORMAT( RGB_32_32_32_FLOAT           , 32*3  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT), CH(64  , 32  , FLOAT)                        ) )

// BGR_32_32_32
GN_COLOR_FORMAT( BGR_32_32_32_UNORM           , 32*3  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM)                        ) )
GN_COLOR_FORMAT( BGR_32_32_32_SNORM           , 32*3  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , SNORM), CH(32  , 32  , SNORM), CH(64  , 32  , SNORM)                        ) )
GN_COLOR_FORMAT( BGR_32_32_32_UINT            , 32*3  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , UINT ), CH(32  , 32  , UINT ), CH(64  , 32  , UINT )                        ) )
GN_COLOR_FORMAT( BGR_32_32_32_SINT            , 32*3  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , SINT ), CH(32  , 32  , SINT ), CH(64  , 32  , SINT )                        ) )
GN_COLOR_FORMAT( BGR_32_32_32_FLOAT           , 32*3  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT), CH(64  , 32  , FLOAT)                        ) )

// RG_32_32
GN_COLOR_FORMAT( RG_32_32_UNORM               , 32*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 32  , UNORM), CH(32  , 32  , UNORM)                                               ) )
GN_COLOR_FORMAT( RG_32_32_SNORM               , 32*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 32  , SNORM), CH(32  , 32  , SNORM)                                               ) )
GN_COLOR_FORMAT( RG_32_32_SINT                , 32*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 32  , SINT ), CH(32  , 32  , SINT )                                               ) )
GN_COLOR_FORMAT( RG_32_32_UINT                , 32*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 32  , UINT ), CH(32  , 32  , UINT )                                               ) )
GN_COLOR_FORMAT( RG_32_32_FLOAT               , 32*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT)                                               ) )

// R_32
GN_COLOR_FORMAT( R_32_UNORM                   , 32*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 32  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( R_32_SNORM                   , 32*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 32  , SNORM)                                                                      ) )
GN_COLOR_FORMAT( R_32_UINT                    , 32*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 32  , UINT )                                                                      ) )
GN_COLOR_FORMAT( R_32_SINT                    , 32*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 32  , SINT )                                                                      ) )
GN_COLOR_FORMAT( R_32_FLOAT                   , 32*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 32  , FLOAT)                                                                      ) )

// RGBA_16_16_16_16
GN_COLOR_FORMAT( RGBA_16_16_16_16_UNORM       , 16*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM), CH(48  , 16  , UNORM) ) )
GN_COLOR_FORMAT( RGBA_16_16_16_16_SNORM       , 16*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 16  , SNORM), CH(16  , 16  , SNORM), CH(32  , 16  , SNORM), CH(48  , 16  , SNORM) ) )
GN_COLOR_FORMAT( RGBA_16_16_16_16_UINT        , 16*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 16  , UINT ), CH(16  , 16  , UINT ), CH(32  , 16  , UINT ), CH(48  , 16  , UINT ) ) )
GN_COLOR_FORMAT( RGBA_16_16_16_16_SINT        , 16*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 16  , SINT ), CH(16  , 16  , SINT ), CH(32  , 16  , SINT ), CH(48  , 16  , SINT ) ) )
GN_COLOR_FORMAT( RGBA_16_16_16_16_FLOAT       , 16*4  , CH4( SWIZZLE(R,G,B,A) , CH(0   , 16  , FLOAT), CH(16  , 16  , FLOAT), CH(32  , 16  , FLOAT), CH(48  , 16  , FLOAT) ) )

// BGRA_16_16_16_16
GN_COLOR_FORMAT( BGRA_16_16_16_16_UNORM       , 16*4  , CH4( SWIZZLE(B,G,R,A) , CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM), CH(48  , 16  , UNORM) ) )

// RGBX_16_16_16_16
GN_COLOR_FORMAT( RGBX_16_16_16_16_UNORM       , 16*4  , CH3( SWIZZLE(R,G,B,1) , CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM)                        ) )

// BGRX_16_16_16_16
GN_COLOR_FORMAT( BGRX_16_16_16_16_UNORM       , 16*4  , CH3( SWIZZLE(B,G,R,1) , CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM)                        ) )

// RG_16_16
GN_COLOR_FORMAT( RG_16_16_UNORM               , 16*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 16  , UNORM), CH(16  , 16  , UNORM)                                               ) )
GN_COLOR_FORMAT( RG_16_16_SNORM               , 16*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 16  , SNORM), CH(16  , 16  , SNORM)                                               ) )
GN_COLOR_FORMAT( RG_16_16_UINT                , 16*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 16  , UINT ), CH(16  , 16  , UINT )                                               ) )
GN_COLOR_FORMAT( RG_16_16_SINT                , 16*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 16  , SINT ), CH(16  , 16  , SINT )                                               ) )
GN_COLOR_FORMAT( RG_16_16_FLOAT               , 16*2  , CH2( SWIZZLE(R,G,0,1) , CH(0   , 16  , FLOAT), CH(16  , 16  , FLOAT)                                               ) )

// R_16
GN_COLOR_FORMAT( R_16_UNORM                   , 16*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 16  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( R_16_SNORM                   , 16*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 16  , SNORM)                                                                      ) )
GN_COLOR_FORMAT( R_16_UINT                    , 16*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 16  , UINT )                                                                      ) )
GN_COLOR_FORMAT( R_16_SINT                    , 16*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 16  , SINT )                                                                      ) )
GN_COLOR_FORMAT( R_16_FLOAT                   , 16*1  , CH1( SWIZZLE(R,0,0,1) , CH(0   , 16  , FLOAT)                                                                      ) )

// RGBA_8_8_8_8
GN_COLOR_FORMAT( RGBA_8_8_8_8_UNORM           , 8*4   , CH4( SWIZZLE(R,G,B,A) , CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM), CH(24  , 8   , UNORM) ) )
GN_COLOR_FORMAT( RGBA_8_8_8_8_SNORM           , 8*4   , CH4( SWIZZLE(R,G,B,A) , CH(0   , 8   , SNORM), CH(8   , 8   , SNORM), CH(16  , 8   , SNORM), CH(24  , 8   , SNORM) ) )
GN_COLOR_FORMAT( RGBA_8_8_8_8_UINT            , 8*4   , CH4( SWIZZLE(R,G,B,A) , CH(0   , 8   , UINT ), CH(8   , 8   , UINT ), CH(16  , 8   , UINT ), CH(24  , 8   , UINT ) ) )
GN_COLOR_FORMAT( RGBA_8_8_8_8_SINT            , 8*4   , CH4( SWIZZLE(R,G,B,A) , CH(0   , 8   , SINT ), CH(8   , 8   , SINT ), CH(16  , 8   , SINT ), CH(24  , 8   , SINT ) ) )
GN_COLOR_FORMAT( RGBA_8_8_8_8_FLOAT           , 8*4   , CH4( SWIZZLE(R,G,B,A) , CH(0   , 8   , FLOAT), CH(8   , 8   , FLOAT), CH(16  , 8   , FLOAT), CH(24  , 8   , FLOAT) ) )

// BGRA_8_8_8_8
GN_COLOR_FORMAT( BGRA_8_8_8_8_UNORM           , 32    , CH4( SWIZZLE(B,G,R,A) , CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM), CH(24  , 8   , UNORM) ) )

// RGBX_8_8_8_8
GN_COLOR_FORMAT( RGBX_8_8_8_8_UNORM           , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM)                        ) )

// BGRX_8_8_8_8
GN_COLOR_FORMAT( BGRX_8_8_8_8_UNORM           , 32    , CH3( SWIZZLE(B,G,R,1) , CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM)                        ) )

// RGB_8_8_8
GN_COLOR_FORMAT( RGB_8_8_8_UNORM              , 24    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM)                        ) )

// BGR_8_8_8
GN_COLOR_FORMAT( BGR_8_8_8_UNORM              , 24    , CH3( SWIZZLE(B,G,R,1) , CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM)                        ) )

// RB_8_8
GN_COLOR_FORMAT( RG_8_8_UNORM                 , 16    , CH2( SWIZZLE(R,G,0,1) , CH(0   , 8   , UNORM), CH(8   , 8   , UNORM)                                               ) )
GN_COLOR_FORMAT( RG_8_8_SNORM                 , 16    , CH2( SWIZZLE(R,G,0,1) , CH(0   , 8   , SNORM), CH(8   , 8   , SNORM)                                               ) )

// 4_4_4_4
GN_COLOR_FORMAT( RGBA_4_4_4_4_UNORM           , 16    , CH4( SWIZZLE(R,G,B,A) , CH(0   , 4   , UNORM), CH(4   , 4   , UNORM), CH(8   , 4   , UNORM), CH(12  , 4   , UNORM) ) )
GN_COLOR_FORMAT( BGRA_4_4_4_4_UNORM           , 16    , CH4( SWIZZLE(B,G,R,A) , CH(0   , 4   , UNORM), CH(4   , 4   , UNORM), CH(8   , 4   , UNORM), CH(12  , 4   , UNORM) ) )
GN_COLOR_FORMAT( RGBX_4_4_4_4_UNORM           , 16    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 4   , UNORM), CH(4   , 4   , UNORM), CH(8   , 4   , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_4_4_4_4_UNORM           , 16    , CH3( SWIZZLE(B,G,R,1) , CH(0   , 4   , UNORM), CH(4   , 4   , UNORM), CH(8   , 4   , UNORM)                        ) )

// RGBA_10_10_10_2
GN_COLOR_FORMAT( RGBA_10_10_10_2_UNORM        , 32    , CH4( SWIZZLE(R,G,B,A) , CH(0   , 10  , UNORM), CH(10  , 10  , UNORM), CH(20  , 10  , UNORM), CH(30  , 2   , UNORM) ) )
GN_COLOR_FORMAT( RGBA_10_10_10_2_SNORM        , 32    , CH4( SWIZZLE(R,G,B,A) , CH(0   , 10  , SNORM), CH(10  , 10  , SNORM), CH(20  , 10  , SNORM), CH(30  , 2   , SNORM) ) )
GN_COLOR_FORMAT( RGBA_10_10_10_2_UINT         , 32    , CH4( SWIZZLE(R,G,B,A) , CH(0   , 10  , UINT ), CH(10  , 10  , UINT ), CH(20  , 10  , UINT ), CH(30  , 2   , UINT ) ) )
GN_COLOR_FORMAT( RGBA_10_10_10_2_SINT         , 32    , CH4( SWIZZLE(R,G,B,A) , CH(0   , 10  , SINT ), CH(10  , 10  , SINT ), CH(20  , 10  , SINT ), CH(30  , 2   , SINT ) ) )

// BGRA_10_10_10_2
GN_COLOR_FORMAT( BGRA_10_10_10_2_UNORM        , 32    , CH4( SWIZZLE(B,G,R,A) , CH(0   , 10  , UNORM), CH(10  , 10  , UNORM), CH(20  , 10  , UNORM), CH(30  , 2   , UNORM) ) )

// RGBX_10_10_10_2
GN_COLOR_FORMAT( RGBX_10_10_10_2_UNORM        , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 10  , UNORM), CH(10  , 10  , UNORM), CH(20  , 10  , UNORM)                        ) )
GN_COLOR_FORMAT( RGBX_10_10_10_2_SNORM        , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 10  , SNORM), CH(10  , 10  , SNORM), CH(20  , 10  , SNORM)                        ) )
GN_COLOR_FORMAT( RGBX_10_10_10_2_UINT         , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 10  , UINT ), CH(10  , 10  , UINT ), CH(20  , 10  , UINT )                        ) )
GN_COLOR_FORMAT( RGBX_10_10_10_2_SINT         , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 10  , SINT ), CH(10  , 10  , SINT ), CH(20  , 10  , SINT )                        ) )

// BGRX_10_10_10_2
GN_COLOR_FORMAT( BGRX_10_10_10_2_UNORM        , 32    , CH3( SWIZZLE(B,G,R,1) , CH(0   , 10  , UNORM), CH(10  , 10  , UNORM), CH(20  , 10  , UNORM)                        ) )

// RGB_11_11_10
GN_COLOR_FORMAT( RGB_11_11_10_UNORM           , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 11  , UNORM), CH(11  , 11  , UNORM), CH(22  , 10  , UNORM)                        ) )
GN_COLOR_FORMAT( RGB_11_11_10_SNORM           , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 11  , SNORM), CH(11  , 11  , SNORM), CH(22  , 10  , SNORM)                        ) )
GN_COLOR_FORMAT( RGB_11_11_10_UINT            , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 11  , UINT ), CH(11  , 11  , UINT ), CH(22  , 10  , UINT )                        ) )
GN_COLOR_FORMAT( RGB_11_11_10_SINT            , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 11  , SINT ), CH(11  , 11  , SINT ), CH(22  , 10  , SINT )                        ) )

// RGB_10_11_11
GN_COLOR_FORMAT( RGB_10_11_11_UNORM           , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 10  , UNORM), CH(10  , 11  , UNORM), CH(21  , 11  , UNORM)                        ) )
GN_COLOR_FORMAT( RGB_10_11_11_SNORM           , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 10  , SNORM), CH(10  , 11  , SNORM), CH(21  , 11  , SNORM)                        ) )
GN_COLOR_FORMAT( RGB_10_11_11_UINT            , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 10  , UINT ), CH(10  , 11  , UINT ), CH(21  , 11  , UINT )                        ) )
GN_COLOR_FORMAT( RGB_10_11_11_SINT            , 32    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 10  , SINT ), CH(10  , 11  , SINT ), CH(21  , 11  , SINT )                        ) )

// 5_5_5_1
GN_COLOR_FORMAT( RGBA_5_5_5_1_UNORM           , 16    , CH4( SWIZZLE(R,G,B,A) , CH(0   , 5   , UNORM), CH(5   , 5   , UNORM), CH(10  , 5   , UNORM), CH(15  , 1   , UNORM) ) )
GN_COLOR_FORMAT( BGRA_5_5_5_1_UNORM           , 16    , CH4( SWIZZLE(B,G,R,A) , CH(0   , 5   , UNORM), CH(5   , 5   , UNORM), CH(10  , 5   , UNORM), CH(15  , 1   , UNORM) ) )
GN_COLOR_FORMAT( RGBX_5_5_5_1_UNORM           , 16    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 5   , UNORM), CH(5   , 5   , UNORM), CH(10  , 5   , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_5_5_5_1_UNORM           , 16    , CH3( SWIZZLE(B,G,R,1) , CH(0   , 5   , UNORM), CH(5   , 5   , UNORM), CH(10  , 5   , UNORM)                        ) )

// 5_6_5
GN_COLOR_FORMAT( RGB_5_6_5_UNORM              , 16    , CH3( SWIZZLE(R,G,B,1) , CH(0   , 5   , UNORM), CH(5   , 6   , UNORM), CH(11  , 5   , UNORM)                        ) )
GN_COLOR_FORMAT( BGR_5_6_5_UNORM              , 16    , CH3( SWIZZLE(B,G,R,1) , CH(0   , 5   , UNORM), CH(5   , 6   , UNORM), CH(11  , 5   , UNORM)                        ) )

// LA
GN_COLOR_FORMAT( LA_16_16_UNORM               , 32    , CH2( SWIZZLE(R,R,R,G) , CH(0   , 16  , UNORM), CH(16  , 16  , UNORM)                                               ) )
GN_COLOR_FORMAT( LA_8_8_UNORM                 , 16    , CH2( SWIZZLE(R,R,R,G) , CH(0   , 8   , UNORM), CH(8   , 8   , UNORM)                                               ) )
GN_COLOR_FORMAT( LA_4_4_UNORM                 , 8     , CH2( SWIZZLE(R,R,R,G) , CH(0   , 4   , UNORM), CH(4   , 4   , UNORM)                                               ) )
GN_COLOR_FORMAT( L_16_UNORM                   , 16    , CH1( SWIZZLE(R,R,R,1) , CH(0   , 16  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( L_8_UNORM                    , 8     , CH1( SWIZZLE(R,R,R,1) , CH(0   , 8   , UNORM)                                                                      ) )
GN_COLOR_FORMAT( A_16_UNORM                   , 16    , CH1( SWIZZLE(1,1,1,R) , CH(0   , 0   , UNORM)                                                                      ) )
GN_COLOR_FORMAT( A_8_UNORM                    , 8     , CH1( SWIZZLE(1,1,1,R) , CH(0   , 0   , UNORM)                                                                      ) )

// depth format
GN_COLOR_FORMAT( D_32                         , 32    , CH1( SWIZZLE(R,R,R,1) , CH(0   , 32  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( D_24                         , 32    , CH1( SWIZZLE(R,R,R,1) , CH(0   , 24  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( D_16                         , 16    , CH1( SWIZZLE(R,R,R,1) , CH(0   , 16  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( DS_24_8                      , 32    , CH2( SWIZZLE(R,R,R,G) , CH(0   , 24  , UNORM), CH(24   , 8  , UINT )                                               ) )
GN_COLOR_FORMAT( DS_24_8_FLOAT                , 32    , CH2( SWIZZLE(R,R,R,G) , CH(0   , 24  , FLOAT), CH(24   , 8  , UINT )                                               ) )
GN_COLOR_FORMAT( DX_24_8                      , 32    , CH1( SWIZZLE(R,R,R,1) , CH(0   , 24  , UNORM)                                                                      ) )

// compressed format
GN_COLOR_FORMAT( DXT1                         , 4     , FOURCC(D,X,T,1) )
GN_COLOR_FORMAT( DXT2                         , 8     , FOURCC(D,X,T,2) )
GN_COLOR_FORMAT( DXT3                         , 8     , FOURCC(D,X,T,3) )
GN_COLOR_FORMAT( DXT4                         , 8     , FOURCC(D,X,T,4) )
GN_COLOR_FORMAT( DXT5                         , 8     , FOURCC(D,X,T,5) )
