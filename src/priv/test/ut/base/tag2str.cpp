#include "../testCommon.h"
#include "garnet/GNgfx.h"

class Tag2StrTest : public CxxTest::TestSuite
{
public:
    void testVertexSemantic()
    {
        GN::VtxSem tag[] = {
            #define GN_VTXSEM( tag, d3decl, d3dindex, glname, glindex, cgname ) GN::VTXSEM_##tag,
            #include "garnet/base/vertexSemanticMeta.h"
            #undef GN_VTXSEM
        };
        const char * str[] = {
            #define GN_VTXSEM( tag, d3decl, d3dindex, glname, glindex, cgname ) #tag,
            #include "garnet/base/vertexSemanticMeta.h"
            #undef GN_VTXSEM
        };

        GN::StrA s;
        GN::VtxSem t = GN::VTXSEM_INVALID;

        for( int i = 0; i < GN::NUM_VTXSEMS; ++i )
        {
            TS_ASSERT( GN::vtxSem2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::vtxSem2Str(tag[i]) );

            TS_ASSERT( GN::str2VtxSem( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2VtxSem( str[i] ) );
        }

        TS_ASSERT( !GN::vtxSem2Str(s,GN::NUM_VTXSEMS) );
        TS_ASSERT_EQUALS( "BAD_VTXSEM", GN::vtxSem2Str(GN::NUM_VTXSEMS) );
        TS_ASSERT( !GN::str2VtxSem( t, "haha" ) );
        TS_ASSERT( !GN::str2VtxSem( t, NULL ) );
        TS_ASSERT_EQUALS( GN::VTXSEM_INVALID, GN::str2VtxSem( "haha" ) );
        TS_ASSERT_EQUALS( GN::VTXSEM_INVALID, GN::str2VtxSem( NULL ) );
    }

    void testRenderState()
    {
        GN::RenderState tag[] = {
            #define GN_DEFINE_RS( tag, defval ) GN::RS_##tag,
            #include "garnet/base/renderStateMeta.h"
            #undef GN_DEFINE_RS
        };
        const char * str[] = {
            #define GN_DEFINE_RS( tag, defval ) #tag,
            #include "garnet/base/renderStateMeta.h"
            #undef GN_DEFINE_RS
        };

        GN::StrA s;
        GN::RenderState t = GN::RS_INVALID;

        for( int i = 0; i < GN::NUM_RENDER_STATES; ++i )
        {
            TS_ASSERT( GN::renderState2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::renderState2Str(tag[i]) );

            TS_ASSERT( GN::str2RenderState( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2RenderState( str[i] ) );
        }

        TS_ASSERT( !GN::renderState2Str(s,GN::NUM_RENDER_STATES) );
        TS_ASSERT_EQUALS( "BAD_RS", GN::renderState2Str(GN::NUM_RENDER_STATES) );
        TS_ASSERT( !GN::str2RenderState( t, "haha" ) );
        TS_ASSERT( !GN::str2RenderState( t, NULL ) );
        TS_ASSERT_EQUALS( GN::RS_INVALID, GN::str2RenderState( "haha" ) );
        TS_ASSERT_EQUALS( GN::RS_INVALID, GN::str2RenderState( NULL ) );
    }

    void testRenderStateValue()
    {
        GN::RenderStateValue tag[] = {
            #define GN_DEFINE_RSV( tag, d3dval, oglval ) GN::RSV_##tag,
            #include "garnet/base/renderStateValueMeta.h"
            #undef GN_DEFINE_RSV
        };
        const char * str[] = {
            #define GN_DEFINE_RSV( tag, d3dval, oglval ) #tag,
            #include "garnet/base/renderStateValueMeta.h"
            #undef GN_DEFINE_RSV
        };

        GN::StrA s;
        GN::RenderStateValue t = GN::RSV_INVALID;

        for( int i = 0; i < GN::NUM_RENDER_STATE_VALUES; ++i )
        {
            TS_ASSERT( GN::renderStateValue2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::renderStateValue2Str(tag[i]) );

            TS_ASSERT( GN::str2RenderStateValue( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2RenderStateValue( str[i] ) );
        }

        TS_ASSERT( !GN::renderStateValue2Str(s,GN::NUM_RENDER_STATE_VALUES) );
        TS_ASSERT_EQUALS( "BAD_RSV", GN::renderStateValue2Str(GN::NUM_RENDER_STATE_VALUES) );
        TS_ASSERT( !GN::str2RenderStateValue( t, "haha" ) );
        TS_ASSERT( !GN::str2RenderStateValue( t, NULL ) );
        TS_ASSERT_EQUALS( GN::RSV_INVALID, GN::str2RenderStateValue( "haha" ) );
        TS_ASSERT_EQUALS( GN::RSV_INVALID, GN::str2RenderStateValue( NULL ) );
    }

    void testTextureState()
    {
        GN::TextureState tag[] = {
            #define GN_DEFINE_TS( tag, def0, def1, d3d, ogl1, ogl2 ) GN::TS_##tag,
            #include "garnet/base/textureStateMeta.h"
            #undef GN_DEFINE_TS
        };
        const char * str[] = {
            #define GN_DEFINE_TS( tag, def0, def1, d3d, ogl1, ogl2 ) #tag,
            #include "garnet/base/textureStateMeta.h"
            #undef GN_DEFINE_TS
        };

        GN::StrA s;
        GN::TextureState t = GN::TS_INVALID;

        for( int i = 0; i < GN::NUM_TEXTURE_STATES; ++i )
        {
            TS_ASSERT( GN::textureState2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::textureState2Str(tag[i]) );

            TS_ASSERT( GN::str2TextureState( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2TextureState( str[i] ) );
        }

        TS_ASSERT( !GN::textureState2Str(s,GN::NUM_TEXTURE_STATES) );
        TS_ASSERT_EQUALS( "BAD_TS", GN::textureState2Str(GN::NUM_TEXTURE_STATES) );
        TS_ASSERT( !GN::str2TextureState( t, "haha" ) );
        TS_ASSERT( !GN::str2TextureState( t, NULL ) );
        TS_ASSERT_EQUALS( GN::TS_INVALID, GN::str2TextureState( "haha" ) );
        TS_ASSERT_EQUALS( GN::TS_INVALID, GN::str2TextureState( NULL ) );
    }

    void testTextureStateValue()
    {
        GN::TextureStateValue tag[] = {
            #define GN_DEFINE_TSV( tag, d3d, ogl1, ogl2 ) GN::TSV_##tag,
            #include "garnet/base/textureStateValueMeta.h"
            #undef GN_DEFINE_TSV
        };
        const char * str[] = {
            #define GN_DEFINE_TSV( tag, d3d, ogl1, ogl2 ) #tag,
            #include "garnet/base/textureStateValueMeta.h"
            #undef GN_DEFINE_TSV
        };

        GN::StrA s;
        GN::TextureStateValue t = GN::TSV_INVALID;

        for( int i = 0; i < GN::NUM_TEXTURE_STATE_VALUES; ++i )
        {
            TS_ASSERT( GN::textureStateValue2Str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::textureStateValue2Str(tag[i]) );

            TS_ASSERT( GN::str2TextureStateValue( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2TextureStateValue( str[i] ) );
        }

        TS_ASSERT( !GN::textureStateValue2Str(s,GN::NUM_TEXTURE_STATE_VALUES) );
        TS_ASSERT_EQUALS( "BAD_TSV", GN::textureStateValue2Str(GN::NUM_TEXTURE_STATE_VALUES) );
        TS_ASSERT( !GN::str2TextureStateValue( t, "haha" ) );
        TS_ASSERT( !GN::str2TextureStateValue( t, NULL ) );
        TS_ASSERT_EQUALS( GN::TSV_INVALID, GN::str2TextureStateValue( "haha" ) );
        TS_ASSERT_EQUALS( GN::TSV_INVALID, GN::str2TextureStateValue( NULL ) );
    }

    void testShaderType()
    {
        GN::StrA str;
        GN::gfx::ShaderType type = GN::gfx::NUM_SHADER_TYPES;

        TS_ASSERT( GN::gfx::shaderType2Str( str, GN::gfx::PIXEL_SHADER ) );
        TS_ASSERT_EQUALS( str, "PIXEL" );
        TS_ASSERT( GN::gfx::shaderType2Str( str, GN::gfx::VERTEX_SHADER ) );
        TS_ASSERT_EQUALS( str, "VERTEX" );
        TS_ASSERT( !GN::gfx::shaderType2Str( str, GN::gfx::NUM_SHADER_TYPES ) );

        TS_ASSERT_EQUALS( "PIXEL", GN::gfx::shaderType2Str(GN::gfx::PIXEL_SHADER) );
        TS_ASSERT_EQUALS( "VERTEX", GN::gfx::shaderType2Str(GN::gfx::VERTEX_SHADER) );
        TS_ASSERT_EQUALS( "BAD_SHADER_TYPE", GN::gfx::shaderType2Str(GN::gfx::NUM_SHADER_TYPES) );

        TS_ASSERT( GN::gfx::str2ShaderType(type,"VERTEX") );
        TS_ASSERT_EQUALS( type, GN::gfx::VERTEX_SHADER );
        TS_ASSERT( GN::gfx::str2ShaderType(type,"PIXEL") );
        TS_ASSERT_EQUALS( type, GN::gfx::PIXEL_SHADER );
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
