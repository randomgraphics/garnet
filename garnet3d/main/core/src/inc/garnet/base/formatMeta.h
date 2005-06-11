//               format                  bits           fourcc              ch_0                   ch_1                   ch_2                   ch_3
//               |                        |              |                  shift                  shift                  shift                  shift
//               |                        |              |                  |     bits             |     bits             |     bits             |     bits
//               |                        |              |                  |     |     type       |     |     type       |     |     type       |     |     type
//               |                        |              |                  |     |     |          |     |     |          |     |     |          |     |     |
GN_COLOR_FORMAT( RGBA_32_32_32_32       , 32*4  , FOURCC(R,G,B,A) , CH4( CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM), CH(96  , 32  , UNORM) ) )
GN_COLOR_FORMAT( BGRA_32_32_32_32       , 32*4  , FOURCC(B,G,R,A) , CH4( CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM), CH(96  , 32  , UNORM) ) )
GN_COLOR_FORMAT( RGBX_32_32_32_32       , 32*4  , FOURCC(R,G,B,1) , CH3( CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_32_32_32_32       , 32*4  , FOURCC(B,G,R,1) , CH3( CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM)                        ) )

GN_COLOR_FORMAT( RGB_32_32_32           , 32*3  , FOURCC(R,G,B,1) , CH3( CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM)                        ) )
GN_COLOR_FORMAT( BGR_32_32_32           , 32*3  , FOURCC(B,G,R,1) , CH3( CH(0   , 32  , UNORM), CH(32  , 32  , UNORM), CH(64  , 32  , UNORM)                        ) )

GN_COLOR_FORMAT( RG_32_32               , 32*2  , FOURCC(R,G,0,1) , CH2( CH(0   , 32  , UNORM), CH(32  , 32  , UNORM)                                               ) )

GN_COLOR_FORMAT( R_32                   , 32*1  , FOURCC(R,0,0,1) , CH1( CH(0   , 32  , UNORM)                                                                      ) )

GN_COLOR_FORMAT( RGBA_16_16_16_16       , 16*4  , FOURCC(R,G,B,A) , CH4( CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM), CH(48  , 16  , UNORM) ) )
GN_COLOR_FORMAT( BGRA_16_16_16_16       , 16*4  , FOURCC(B,G,R,A) , CH4( CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM), CH(48  , 16  , UNORM) ) )
GN_COLOR_FORMAT( RGBX_16_16_16_16       , 16*4  , FOURCC(R,G,B,1) , CH3( CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_16_16_16_16       , 16*4  , FOURCC(B,G,R,1) , CH3( CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM)                        ) )

GN_COLOR_FORMAT( RGB_16_16_16           , 16*3  , FOURCC(R,G,B,1) , CH3( CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM)                        ) )
GN_COLOR_FORMAT( BGR_16_16_16           , 16*3  , FOURCC(B,G,R,1) , CH3( CH(0   , 16  , UNORM), CH(16  , 16  , UNORM), CH(32  , 16  , UNORM)                        ) )

GN_COLOR_FORMAT( RG_16_16               , 16*2  , FOURCC(R,G,0,1) , CH2( CH(0   , 16  , UNORM), CH(16  , 16  , UNORM)                                               ) )

GN_COLOR_FORMAT( R_16                   , 16*1  , FOURCC(R,0,0,1) , CH1( CH(0   , 16  , UNORM)                                                                      ) )

GN_COLOR_FORMAT( RGBA_8_8_8_8           , 32    , FOURCC(R,G,B,A) , CH4( CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM), CH(24  , 8   , UNORM) ) )
GN_COLOR_FORMAT( BGRA_8_8_8_8           , 32    , FOURCC(B,G,R,A) , CH4( CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM), CH(24  , 8   , UNORM) ) )
GN_COLOR_FORMAT( RGBX_8_8_8_8           , 32    , FOURCC(R,G,B,1) , CH3( CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_8_8_8_8           , 32    , FOURCC(B,G,R,1) , CH3( CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM)                        ) )

GN_COLOR_FORMAT( RGB_8_8_8              , 24    , FOURCC(R,G,B,1) , CH3( CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM)                        ) )
GN_COLOR_FORMAT( BGR_8_8_8              , 24    , FOURCC(B,G,R,1) , CH3( CH(0   , 8   , UNORM), CH(8   , 8   , UNORM), CH(16  , 8   , UNORM)                        ) )

GN_COLOR_FORMAT( RGBA_4_4_4_4           , 16    , FOURCC(R,G,B,A) , CH4( CH(0   , 4   , UNORM), CH(4   , 4   , UNORM), CH(8   , 4   , UNORM), CH(12  , 4   , UNORM) ) )
GN_COLOR_FORMAT( BGRA_4_4_4_4           , 16    , FOURCC(B,G,R,A) , CH4( CH(0   , 4   , UNORM), CH(4   , 4   , UNORM), CH(8   , 4   , UNORM), CH(12  , 4   , UNORM) ) )
GN_COLOR_FORMAT( RGBX_4_4_4_4           , 16    , FOURCC(R,G,B,1) , CH3( CH(0   , 4   , UNORM), CH(4   , 4   , UNORM), CH(8   , 4   , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_4_4_4_4           , 16    , FOURCC(B,G,R,1) , CH3( CH(0   , 4   , UNORM), CH(4   , 4   , UNORM), CH(8   , 4   , UNORM)                        ) )

GN_COLOR_FORMAT( RGBA_10_10_10_2        , 32    , FOURCC(R,G,B,A) , CH4( CH(0   , 10  , UNORM), CH(10  , 10  , UNORM), CH(20  , 10  , UNORM), CH(30  , 2   , UNORM) ) )
GN_COLOR_FORMAT( BGRA_10_10_10_2        , 32    , FOURCC(B,G,R,A) , CH4( CH(0   , 10  , UNORM), CH(10  , 10  , UNORM), CH(20  , 10  , UNORM), CH(30  , 2   , UNORM) ) )
GN_COLOR_FORMAT( RGBX_10_10_10_2        , 32    , FOURCC(R,G,B,1) , CH3( CH(0   , 10  , UNORM), CH(10  , 10  , UNORM), CH(20  , 10  , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_10_10_10_2        , 32    , FOURCC(B,G,R,1) , CH3( CH(0   , 10  , UNORM), CH(10  , 10  , UNORM), CH(20  , 10  , UNORM)                        ) )

GN_COLOR_FORMAT( RGBA_5_5_5_1           , 16    , FOURCC(R,G,B,A) , CH4( CH(0   , 5   , UNORM), CH(5   , 5   , UNORM), CH(10  , 5   , UNORM), CH(15  , 1   , UNORM) ) )
GN_COLOR_FORMAT( BGRA_5_5_5_1           , 16    , FOURCC(B,G,R,A) , CH4( CH(0   , 5   , UNORM), CH(5   , 5   , UNORM), CH(10  , 5   , UNORM), CH(15  , 1   , UNORM) ) )
GN_COLOR_FORMAT( RGBX_5_5_5_1           , 16    , FOURCC(R,G,B,1) , CH3( CH(0   , 5   , UNORM), CH(5   , 5   , UNORM), CH(10  , 5   , UNORM)                        ) )
GN_COLOR_FORMAT( BGRX_5_5_5_1           , 16    , FOURCC(B,G,R,1) , CH3( CH(0   , 5   , UNORM), CH(5   , 5   , UNORM), CH(10  , 5   , UNORM)                        ) )

GN_COLOR_FORMAT( RGB_5_6_5              , 16    , FOURCC(R,G,B,1) , CH3( CH(0   , 5   , UNORM), CH(5   , 6   , UNORM), CH(11  , 5   , UNORM)                        ) )
GN_COLOR_FORMAT( BGR_5_6_5              , 16    , FOURCC(B,G,R,1) , CH3( CH(0   , 5   , UNORM), CH(5   , 6   , UNORM), CH(11  , 5   , UNORM)                        ) )

GN_COLOR_FORMAT( LA_16_16               , 32    , FOURCC(R,R,R,G) , CH2( CH(0   , 16  , UNORM), CH(16  , 16  , UNORM)                                               ) )
GN_COLOR_FORMAT( LA_8_8                 , 16    , FOURCC(R,R,R,G) , CH2( CH(0   , 8   , UNORM), CH(8   , 8   , UNORM)                                               ) )
GN_COLOR_FORMAT( LA_4_4                 , 8     , FOURCC(R,R,R,G) , CH2( CH(0   , 4   , UNORM), CH(4   , 4   , UNORM)                                               ) )
GN_COLOR_FORMAT( L_16                   , 16    , FOURCC(R,R,R,1) , CH1( CH(0   , 16  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( L_8                    , 8     , FOURCC(R,R,R,1) , CH1( CH(0   , 8   , UNORM)                                                                      ) )
GN_COLOR_FORMAT( A_16                   , 16    , FOURCC(1,1,1,R) , CH1( CH(0   , 0   , UNORM)                                                                      ) )
GN_COLOR_FORMAT( A_8                    , 8     , FOURCC(1,1,1,R) , CH1( CH(0   , 0   , UNORM)                                                                      ) )


GN_COLOR_FORMAT( UVWQ_16_16_16_16       , 16*4  , FOURCC(R,G,B,A) , CH4( CH(0   , 16  , SNORM), CH(16  , 16  , SNORM), CH(32  , 16  , SNORM), CH(48  , 16  , SNORM) ) )
GN_COLOR_FORMAT( UV_16_16               , 32    , FOURCC(R,G,0,1) , CH2( CH(0   , 16  , SNORM), CH(16  , 16  , SNORM)                                               ) )
GN_COLOR_FORMAT( UVWQ_8_8_8_8           , 32    , FOURCC(R,G,B,A) , CH4( CH(0   , 8   , SNORM), CH(8   , 8   , SNORM), CH(16  , 8   , SNORM), CH(24  , 8   , SNORM) ) )
GN_COLOR_FORMAT( UV_8_8                 , 16    , FOURCC(R,G,0,1) , CH2( CH(0   , 8   , SNORM), CH(8   , 8   , SNORM)                                               ) )

GN_COLOR_FORMAT( D_32                   , 32    , FOURCC(R,R,R,1) , CH1( CH(0   , 32  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( D_24                   , 32    , FOURCC(R,R,R,1) , CH1( CH(0   , 24  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( D_16                   , 16    , FOURCC(R,R,R,1) , CH1( CH(0   , 16  , UNORM)                                                                      ) )
GN_COLOR_FORMAT( DS_24_8                , 32    , FOURCC(R,R,R,G) , CH2( CH(0   , 24  , UNORM), CH(24   , 8  , UINT )                                               ) )
GN_COLOR_FORMAT( DX_24_8                , 32    , FOURCC(R,R,R,1) , CH1( CH(0   , 24  , UNORM)                                                                      ) )

// floating point format
GN_COLOR_FORMAT( RGBA_32_32_32_32_FLOAT , 32*4  , FOURCC(R,G,B,A) , CH4( CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT), CH(64  , 32  , FLOAT), CH(96  , 32  , FLOAT) ) )
GN_COLOR_FORMAT( RGB_32_32_32_FLOAT     , 32*3  , FOURCC(R,G,B,1) , CH3( CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT), CH(64  , 32  , FLOAT)                        ) )
GN_COLOR_FORMAT( RG_32_32_FLOAT         , 32*2  , FOURCC(R,G,0,1) , CH2( CH(0   , 32  , FLOAT), CH(32  , 32  , FLOAT)                                               ) )
GN_COLOR_FORMAT( R_32_FLOAT             , 32*1  , FOURCC(R,0,0,1) , CH1( CH(0   , 32  , FLOAT)                                                                      ) )

GN_COLOR_FORMAT( RGBA_16_16_16_16_FLOAT , 16*4  , FOURCC(R,G,B,A) , CH4( CH(0   , 16  , FLOAT), CH(16  , 16  , FLOAT), CH(32  , 16  , FLOAT), CH(48  , 16  , FLOAT) ) )
GN_COLOR_FORMAT( RGB_16_16_16_FLOAT     , 16*3  , FOURCC(R,G,B,1) , CH3( CH(0   , 16  , FLOAT), CH(16  , 16  , FLOAT), CH(32  , 16  , FLOAT)                        ) )
GN_COLOR_FORMAT( RG_16_16_FLOAT         , 16*2  , FOURCC(R,G,0,1) , CH2( CH(0   , 16  , FLOAT), CH(16  , 16  , FLOAT)                                               ) )
GN_COLOR_FORMAT( R_16_FLOAT             , 16*1  , FOURCC(R,0,0,1) , CH1( CH(0   , 16  , FLOAT)                                                                      ) )

// compressed format
GN_COLOR_FORMAT( DXT1                   , 4     , FOURCC(D,X,T,1) , CH0() )
GN_COLOR_FORMAT( DXT2                   , 8     , FOURCC(D,X,T,2) , CH0() )
GN_COLOR_FORMAT( DXT3                   , 8     , FOURCC(D,X,T,3) , CH0() )
GN_COLOR_FORMAT( DXT4                   , 8     , FOURCC(D,X,T,4) , CH0() )
GN_COLOR_FORMAT( DXT5                   , 8     , FOURCC(D,X,T,5) , CH0() )
