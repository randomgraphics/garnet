#include "../testCommon.h"
#include "garnet/GNgfx.h"

class Tag2StrTest : public CxxTest::TestSuite
{
public:
    void testVertexSemantic()
    {
        GN_UNIMPL_WARNING();
    }

    void testRenderState()
    {
        GN::gfx::RenderState tag[] = {
            #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) GN::gfx::RS_##tag,
            #include "garnet/gfx/renderStateMeta.h"
            #undef GNGFX_DEFINE_RS
        };
        const char * str[] = {
            #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) #tag,
            #include "garnet/gfx/renderStateMeta.h"
            #undef GNGFX_DEFINE_RS
        };

        GN::StrA s;
        GN::gfx::RenderState t = GN::gfx::RS_INVALID;

        for( int i = 0; i < GN::gfx::NUM_RENDER_STATES; ++i )
        {
            TS_ASSERT( GN::gfx::renderState2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::gfx::renderState2Str(tag[i]) );

            TS_ASSERT( GN::gfx::str2RenderState( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::gfx::str2RenderState( str[i] ) );
        }

        TS_ASSERT( !GN::gfx::renderState2Str(s,GN::gfx::NUM_RENDER_STATES) );
        TS_ASSERT_EQUALS( "RS_INVALID", GN::gfx::renderState2Str(GN::gfx::NUM_RENDER_STATES) );
        TS_ASSERT( !GN::gfx::str2RenderState( t, "haha" ) );
        TS_ASSERT( !GN::gfx::str2RenderState( t, NULL ) );
        TS_ASSERT_EQUALS( GN::gfx::RS_INVALID, GN::gfx::str2RenderState( "haha" ) );
        TS_ASSERT_EQUALS( GN::gfx::RS_INVALID, GN::gfx::str2RenderState( NULL ) );
    }

    void testRenderStateValue()
    {
        GN::gfx::RenderStateValue tag[] = {
            #define GNGFX_DEFINE_RSV( tag, d3dval, oglval ) GN::gfx::RSV_##tag,
            #include "garnet/gfx/renderStateValueMeta.h"
            #undef GNGFX_DEFINE_RSV
        };
        const char * str[] = {
            #define GNGFX_DEFINE_RSV( tag, d3dval, oglval ) #tag,
            #include "garnet/gfx/renderStateValueMeta.h"
            #undef GNGFX_DEFINE_RSV
        };

        GN::StrA s;
        GN::gfx::RenderStateValue t = GN::gfx::RSV_INVALID;

        for( int i = 0; i < GN::gfx::NUM_RENDER_STATE_VALUES; ++i )
        {
            TS_ASSERT( GN::gfx::renderStateValue2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::gfx::renderStateValue2Str(tag[i]) );

            TS_ASSERT( GN::gfx::str2RenderStateValue( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::gfx::str2RenderStateValue( str[i] ) );
        }

        TS_ASSERT( !GN::gfx::renderStateValue2Str(s,GN::gfx::NUM_RENDER_STATE_VALUES) );
        TS_ASSERT_EQUALS( "RSV_INVALID", GN::gfx::renderStateValue2Str(GN::gfx::NUM_RENDER_STATE_VALUES) );
        TS_ASSERT( !GN::gfx::str2RenderStateValue( t, "haha" ) );
        TS_ASSERT( !GN::gfx::str2RenderStateValue( t, NULL ) );
        TS_ASSERT_EQUALS( GN::gfx::RSV_INVALID, GN::gfx::str2RenderStateValue( "haha" ) );
        TS_ASSERT_EQUALS( GN::gfx::RSV_INVALID, GN::gfx::str2RenderStateValue( NULL ) );
    }

    void testTextureState()
    {
        GN::gfx::TextureState tag[] = {
            #define GNGFX_DEFINE_TS( tag, def0, d3d, ogl1, ogl2 ) GN::gfx::TS_##tag,
            #include "garnet/gfx/textureStateMeta.h"
            #undef GNGFX_DEFINE_TS
        };
        const char * str[] = {
            #define GNGFX_DEFINE_TS( tag, def0, d3d, ogl1, ogl2 ) #tag,
            #include "garnet/gfx/textureStateMeta.h"
            #undef GNGFX_DEFINE_TS
        };

        GN::StrA s;
        GN::gfx::TextureState t = GN::gfx::TS_INVALID;

        for( int i = 0; i < GN::gfx::NUM_TEXTURE_STATES; ++i )
        {
            TS_ASSERT( GN::gfx::textureState2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::gfx::textureState2Str(tag[i]) );

            TS_ASSERT( GN::gfx::str2TextureState( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::gfx::str2TextureState( str[i] ) );
        }

        TS_ASSERT( !GN::gfx::textureState2Str(s,GN::gfx::NUM_TEXTURE_STATES) );
        TS_ASSERT_EQUALS( "TS_INVALID", GN::gfx::textureState2Str(GN::gfx::NUM_TEXTURE_STATES) );
        TS_ASSERT( !GN::gfx::str2TextureState( t, "haha" ) );
        TS_ASSERT( !GN::gfx::str2TextureState( t, NULL ) );
        TS_ASSERT_EQUALS( GN::gfx::TS_INVALID, GN::gfx::str2TextureState( "haha" ) );
        TS_ASSERT_EQUALS( GN::gfx::TS_INVALID, GN::gfx::str2TextureState( NULL ) );
    }

    void testTextureStateValue()
    {
        GN::gfx::TextureStateValue tag[] = {
            #define GNGFX_DEFINE_TSV( tag, d3d, ogl1, ogl2 ) GN::gfx::TSV_##tag,
            #include "garnet/gfx/textureStateValueMeta.h"
            #undef GNGFX_DEFINE_TSV
        };
        const char * str[] = {
            #define GNGFX_DEFINE_TSV( tag, d3d, ogl1, ogl2 ) #tag,
            #include "garnet/gfx/textureStateValueMeta.h"
            #undef GNGFX_DEFINE_TSV
        };

        GN::StrA s;
        GN::gfx::TextureStateValue t = GN::gfx::TSV_INVALID;

        for( int i = 0; i < GN::gfx::NUM_TEXTURE_STATE_VALUES; ++i )
        {
            TS_ASSERT( GN::gfx::textureStateValue2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::gfx::textureStateValue2Str(tag[i]) );

            TS_ASSERT( GN::gfx::str2TextureStateValue( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::gfx::str2TextureStateValue( str[i] ) );
        }

        TS_ASSERT( !GN::gfx::textureStateValue2Str(s,GN::gfx::NUM_TEXTURE_STATE_VALUES) );
        TS_ASSERT_EQUALS( "TSV_INVALID", GN::gfx::textureStateValue2Str(GN::gfx::NUM_TEXTURE_STATE_VALUES) );
        TS_ASSERT( !GN::gfx::str2TextureStateValue( t, "haha" ) );
        TS_ASSERT( !GN::gfx::str2TextureStateValue( t, NULL ) );
        TS_ASSERT_EQUALS( GN::gfx::TSV_INVALID, GN::gfx::str2TextureStateValue( "haha" ) );
        TS_ASSERT_EQUALS( GN::gfx::TSV_INVALID, GN::gfx::str2TextureStateValue( NULL ) );
    }

    void testShaderType()
    {
        GN::StrA str;
        GN::gfx::ShaderType type = GN::gfx::NUM_SHADER_TYPES;

        TS_ASSERT( GN::gfx::shaderType2Str( str, GN::gfx::SHADER_PS ) );
        TS_ASSERT_EQUALS( str, "PS" );
        TS_ASSERT( GN::gfx::shaderType2Str( str, GN::gfx::SHADER_VS ) );
        TS_ASSERT_EQUALS( str, "VS" );
        TS_ASSERT( GN::gfx::shaderType2Str( str, GN::gfx::SHADER_GS ) );
        TS_ASSERT_EQUALS( str, "GS" );
        TS_ASSERT( !GN::gfx::shaderType2Str( str, GN::gfx::NUM_SHADER_TYPES ) );

        TS_ASSERT_EQUALS( "PS", GN::gfx::shaderType2Str(GN::gfx::SHADER_PS) );
        TS_ASSERT_EQUALS( "VS", GN::gfx::shaderType2Str(GN::gfx::SHADER_VS) );
        TS_ASSERT_EQUALS( "GS", GN::gfx::shaderType2Str(GN::gfx::SHADER_GS) );
        TS_ASSERT_EQUALS( "BAD_SHADER_TYPE", GN::gfx::shaderType2Str(GN::gfx::NUM_SHADER_TYPES) );

        TS_ASSERT( GN::gfx::str2ShaderType(type,"VS") );
        TS_ASSERT_EQUALS( type, GN::gfx::SHADER_VS );
        TS_ASSERT( GN::gfx::str2ShaderType(type,"PS") );
        TS_ASSERT_EQUALS( type, GN::gfx::SHADER_PS );
        TS_ASSERT( GN::gfx::str2ShaderType(type,"GS") );
        TS_ASSERT_EQUALS( type, GN::gfx::SHADER_GS );
        TS_ASSERT( !GN::gfx::str2ShaderType(type,NULL) );
        TS_ASSERT( !GN::gfx::str2ShaderType(type,"haha") );
    }

    void testShadingLanguage()
    {
        GN::StrA str;
        GN::gfx::ShadingLanguage lang = GN::gfx::NUM_SHADING_LANGUAGES;

        static struct LangItem
        {
            GN::gfx::ShadingLanguage tag;
            const char * str;
        } sTable [] =
        {
            { GN::gfx::LANG_OGL_ARB, "OGL_ARB" },
            { GN::gfx::LANG_OGL_GLSL, "OGL_GLSL" },
            { GN::gfx::LANG_D3D_ASM, "D3D_ASM" },
            { GN::gfx::LANG_D3D_HLSL, "D3D_HLSL" },
            { GN::gfx::LANG_CG, "CG" },
        };
        for( size_t i = 0; i < sizeof(sTable)/sizeof(sTable[0]); ++i )
        {
            TS_ASSERT( GN::gfx::shadingLanguage2Str( str, sTable[i].tag ) );
            TS_ASSERT_EQUALS( str, sTable[i].str );

            TS_ASSERT_EQUALS( sTable[i].str, GN::gfx::shadingLanguage2Str(sTable[i].tag) );

            TS_ASSERT( GN::gfx::str2ShadingLanguage( lang, sTable[i].str ) );
            TS_ASSERT_EQUALS( lang, sTable[i].tag );
        }
        TS_ASSERT( !GN::gfx::shadingLanguage2Str( str, GN::gfx::NUM_SHADING_LANGUAGES ) );
        TS_ASSERT_EQUALS( "BAD_SHADING_LANGUAGE", GN::gfx::shadingLanguage2Str(GN::gfx::NUM_SHADING_LANGUAGES) );
        TS_ASSERT( !GN::gfx::str2ShadingLanguage( lang, NULL ) );
        TS_ASSERT( !GN::gfx::str2ShadingLanguage( lang, "haha" ) );

    }

    void testTexType()
    {
        GN::gfx::TexType tag[] = {
            GN::gfx::TEXTYPE_1D,
            GN::gfx::TEXTYPE_2D,
            GN::gfx::TEXTYPE_3D,
            GN::gfx::TEXTYPE_CUBE,
        };
        const char * str[] = {
            "1D",
            "2D",
            "3D",
            "CUBE",
        };

        GN::StrA         s;
        GN::gfx::TexType t = GN::gfx::NUM_TEXTYPES;

        for( int i = 0; i < 4; ++i )
        {
            TS_ASSERT( GN::gfx::texType2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::gfx::texType2Str(tag[i]) );

            TS_ASSERT( GN::gfx::str2TexType( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );
        }

        TS_ASSERT( !GN::gfx::texType2Str(s,GN::gfx::NUM_TEXTYPES) );
        TS_ASSERT_EQUALS( "BAD_TEXTURE_TYPE", GN::gfx::texType2Str(GN::gfx::NUM_TEXTYPES) );
        TS_ASSERT( !GN::gfx::str2TexType( t, "haha" ) );
        TS_ASSERT( !GN::gfx::str2TexType( t, NULL ) );
    }

    void testTexFilter()
    {
        GN::gfx::TexFilter tag[] = {
            GN::gfx::TEXFILTER_NEAREST,
            GN::gfx::TEXFILTER_LINEAR,
            GN::gfx::TEXFILTER_N_MIP_N,
            GN::gfx::TEXFILTER_N_MIP_L,
            GN::gfx::TEXFILTER_L_MIP_N,
            GN::gfx::TEXFILTER_L_MIP_L,
        };
        const char * str[] = {
            "NEAREST",
            "LINEAR",
            "N_MIP_N",
            "N_MIP_L",
            "L_MIP_N",
            "L_MIP_L",
        };

        GN::StrA           s;
        GN::gfx::TexFilter t = GN::gfx::NUM_TEXFILTERS;

        for( int i = 0; i < 6; ++i )
        {
            TS_ASSERT( GN::gfx::texFilter2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::gfx::texFilter2Str(tag[i]) );

            TS_ASSERT( GN::gfx::str2TexFilter( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );
        }

        TS_ASSERT( !GN::gfx::texFilter2Str(s,GN::gfx::NUM_TEXFILTERS) );
        TS_ASSERT_EQUALS( "BAD_TEXTURE_FILTER", GN::gfx::texFilter2Str(GN::gfx::NUM_TEXFILTERS) );
        TS_ASSERT( !GN::gfx::str2TexFilter( t, "haha" ) );
        TS_ASSERT( !GN::gfx::str2TexFilter( t, NULL ) );
    }

    void testTexWrap()
    {
        GN::gfx::TexWrap tag[] = {
            GN::gfx::TEXWRAP_REPEAT,
            GN::gfx::TEXWRAP_CLAMP,
            GN::gfx::TEXWRAP_CLAMP_TO_EDGE,
        };
        const char * str[] = {
            "REPEAT",
            "CLAMP",
            "CLAMP_TO_EDGE",
        };

        GN::StrA         s;
        GN::gfx::TexWrap t = GN::gfx::NUM_TEXWRAPS;

        for( int i = 0; i < 3; ++i )
        {
            TS_ASSERT( GN::gfx::texWrap2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::gfx::texWrap2Str(tag[i]) );

            TS_ASSERT( GN::gfx::str2TexWrap( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );
        }

        TS_ASSERT( !GN::gfx::texWrap2Str(s,GN::gfx::NUM_TEXWRAPS) );
        TS_ASSERT_EQUALS( "BAD_TEXTURE_WRAP", GN::gfx::texWrap2Str(GN::gfx::NUM_TEXWRAPS) );
        TS_ASSERT( !GN::gfx::str2TexWrap( t, "haha" ) );
        TS_ASSERT( !GN::gfx::str2TexWrap( t, NULL ) );
    }
};
