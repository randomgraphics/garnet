#include "../testCommon.h"

class Tag2StrTest : public CxxTest::TestSuite
{
public:
    void testVertexSemantic()
    {
        GN::VertexSemantic tag[] = {
            #define GN_VERTEX_SEMANTIC( tag, d3decl, d3dindex, glname, glindex, cgname ) GN::VERTSEM_##tag,
            #include "garnet/base/vertexSemanticMeta.h"
            #undef GN_VERTEX_SEMANTIC
        };
        const char * str[] = {
            #define GN_VERTEX_SEMANTIC( tag, d3decl, d3dindex, glname, glindex, cgname ) #tag,
            #include "garnet/base/vertexSemanticMeta.h"
            #undef GN_VERTEX_SEMANTIC
        };

        StrA     s;
        GN::VertexSemantic t;

        for( int i = 0; i < GN::NUM_VERTSEMS; ++i )
        {
            TS_ASSERT( GN::vertsem2str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::vertsem2str(tag[i]) );

            TS_ASSERT( GN::str2vertsem( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2vertsem( str[i] ) );
        }

        TS_ASSERT( !GN::vertsem2str(s,GN::NUM_VERTSEMS) );
        TS_ASSERT_EQUALS( "BAD_VERTSEM", GN::vertsem2str(GN::NUM_VERTSEMS) );
        TS_ASSERT( !GN::str2vertsem( t, "haha" ) );
        TS_ASSERT( !GN::str2vertsem( t, NULL ) );
        TS_ASSERT_EQUALS( GN::VERTSEM_INVALID, GN::str2vertsem( "haha" ) );
        TS_ASSERT_EQUALS( GN::VERTSEM_INVALID, GN::str2vertsem( NULL ) );
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

        StrA s;
        GN::RenderState  t;

        for( int i = 0; i < GN::NUM_RENDER_STATES; ++i )
        {
            TS_ASSERT( GN::rs2str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::rs2str(tag[i]) );

            TS_ASSERT( GN::str2rs( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2rs( str[i] ) );
        }

        TS_ASSERT( !GN::rs2str(s,GN::NUM_RENDER_STATES) );
        TS_ASSERT_EQUALS( "BAD_RS", GN::rs2str(GN::NUM_RENDER_STATES) );
        TS_ASSERT( !GN::str2rs( t, "haha" ) );
        TS_ASSERT( !GN::str2rs( t, NULL ) );
        TS_ASSERT_EQUALS( GN::RS_INVALID, GN::str2rs( "haha" ) );
        TS_ASSERT_EQUALS( GN::RS_INVALID, GN::str2rs( NULL ) );
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

        StrA   s;
        GN::RenderStateValue t;

        for( int i = 0; i < GN::NUM_RENDER_STATE_VALUES; ++i )
        {
            TS_ASSERT( GN::rsv2str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::rsv2str(tag[i]) );

            TS_ASSERT( GN::str2rsv( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2rsv( str[i] ) );
        }

        TS_ASSERT( !GN::rsv2str(s,GN::NUM_RENDER_STATE_VALUES) );
        TS_ASSERT_EQUALS( "BAD_RSV", GN::rsv2str(GN::NUM_RENDER_STATE_VALUES) );
        TS_ASSERT( !GN::str2rsv( t, "haha" ) );
        TS_ASSERT( !GN::str2rsv( t, NULL ) );
        TS_ASSERT_EQUALS( GN::RSV_INVALID, GN::str2rsv( "haha" ) );
        TS_ASSERT_EQUALS( GN::RSV_INVALID, GN::str2rsv( NULL ) );
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

        StrA s;
        GN::TextureState t;

        for( int i = 0; i < GN::NUM_TEXTURE_STATES; ++i )
        {
            TS_ASSERT( GN::ts2str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::ts2str(tag[i]) );

            TS_ASSERT( GN::str2ts( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2ts( str[i] ) );
        }

        TS_ASSERT( !GN::ts2str(s,GN::NUM_TEXTURE_STATES) );
        TS_ASSERT_EQUALS( "BAD_TS", GN::ts2str(GN::NUM_TEXTURE_STATES) );
        TS_ASSERT( !GN::str2ts( t, "haha" ) );
        TS_ASSERT( !GN::str2ts( t, NULL ) );
        TS_ASSERT_EQUALS( GN::TS_INVALID, GN::str2ts( "haha" ) );
        TS_ASSERT_EQUALS( GN::TS_INVALID, GN::str2ts( NULL ) );
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

        StrA    s;
        GN::TextureStateValue t;

        for( int i = 0; i < GN::NUM_TEXTURE_STATE_VALUES; ++i )
        {
            TS_ASSERT( GN::tsv2str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], GN::tsv2str(tag[i]) );

            TS_ASSERT( GN::str2tsv( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );

            TS_ASSERT_EQUALS( tag[i], GN::str2tsv( str[i] ) );
        }

        TS_ASSERT( !GN::tsv2str(s,GN::NUM_TEXTURE_STATE_VALUES) );
        TS_ASSERT_EQUALS( "BAD_TSV", GN::tsv2str(GN::NUM_TEXTURE_STATE_VALUES) );
        TS_ASSERT( !GN::str2tsv( t, "haha" ) );
        TS_ASSERT( !GN::str2tsv( t, NULL ) );
        TS_ASSERT_EQUALS( GN::TSV_INVALID, GN::str2tsv( "haha" ) );
        TS_ASSERT_EQUALS( GN::TSV_INVALID, GN::str2tsv( NULL ) );
    }
/*
    //void testShaderUsage()
    {
        StrA str;
        render_i::shader_usage_t susage;

        TS_ASSERT( render_i::s_susage2str( str, render_i::SUSAGE_PIXEL ) );
        TS_ASSERT_EQUALS( str, "PIXEL" );
        TS_ASSERT( render_i::s_susage2str( str, render_i::SUSAGE_VERTEX ) );
        TS_ASSERT_EQUALS( str, "VERTEX" );
        TS_ASSERT( !render_i::s_susage2str( str, render_i::NUM_SUSAGES ) );

        TS_ASSERT_EQUALS( "PIXEL", render_i::s_susage2str(render_i::SUSAGE_PIXEL) );
        TS_ASSERT_EQUALS( "VERTEX", render_i::s_susage2str(render_i::SUSAGE_VERTEX) );
        TS_ASSERT_EQUALS( "BAD_SHADER_USAGE", render_i::s_susage2str(render_i::NUM_SUSAGES) );

        TS_ASSERT( render_i::s_str2susage(susage,"VERTEX") );
        TS_ASSERT_EQUALS( susage, render_i::SUSAGE_VERTEX );
        TS_ASSERT( render_i::s_str2susage(susage,"PIXEL") );
        TS_ASSERT_EQUALS( susage, render_i::SUSAGE_PIXEL );
        TS_ASSERT( !render_i::s_str2susage(susage,NULL) );
        TS_ASSERT( !render_i::s_str2susage(susage,"haha") );
    }


    //void testShadingLanguage()
    {
        StrA str;
        render_i::shading_language_t slang;

        static struct slang_s
        {
            render_i::shading_language_t tag;
            const char *                 str;
        } slang_table [] =
        {
            { render_i::SLANG_FFP, "FFP" },
            { render_i::SLANG_OGL_ARB, "OGL_ARB" },
            { render_i::SLANG_OGL_GLSL, "OGL_GLSL" },
            { render_i::SLANG_D3D_ASM, "D3D_ASM" },
            { render_i::SLANG_D3D_HLSL, "D3D_HLSL" },
            { render_i::SLANG_CG, "CG" },
        };
        for( size_t i = 0; i < sizeof(slang_table)/sizeof(slang_table[0]); ++i )
        {
            TS_ASSERT( render_i::s_slang2str( str, slang_table[i].tag ) );
            CHECK_EQUAL( str, slang_table[i].str );

            TS_ASSERT_EQUALS( slang_table[i].str, render_i::s_slang2str(slang_table[i].tag) );

            TS_ASSERT( render_i::s_str2slang( slang, slang_table[i].str ) );
            TS_ASSERT_EQUALS( slang, slang_table[i].tag );
        }
        TS_ASSERT( !render_i::s_slang2str( str, render_i::NUM_SLANGS ) );
        TS_ASSERT_EQUALS( "BAD_SHADING_LANGUAGE", render_i::s_slang2str(render_i::NUM_SLANGS) );
        TS_ASSERT( !render_i::s_str2slang( slang, NULL ) );
        TS_ASSERT( !render_i::s_str2slang( slang, "haha" ) );

    }

    //void testTexType()
    {
        render_i::textype_t tag[] = {
            render_i::TEXTYPE_1D,
            render_i::TEXTYPE_2D,
            render_i::TEXTYPE_3D,
            render_i::TEXTYPE_CUBE,
        };
        const char * str[] = {
            "1D",
            "2D",
            "3D",
            "CUBE",
        };

        StrA           s;
        render_i::textype_t t;

        for( int i = 0; i < 4; ++i )
        {
            TS_ASSERT( render_i::s_textype2str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], render_i::s_textype2str(tag[i]) );

            TS_ASSERT( render_i::s_str2textype( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );
        }

        TS_ASSERT( !render_i::s_textype2str(s,render_i::NUM_TEXTYPES) );
        TS_ASSERT_EQUALS( "BAD_TEXTURE_TYPE", render_i::s_textype2str(render_i::NUM_TEXTYPES) );
        TS_ASSERT( !render_i::s_str2textype( t, "haha" ) );
        TS_ASSERT( !render_i::s_str2textype( t, NULL ) );
    }

    //void testTexFilter()
    {
        render_i::texfilter_t tag[] = {
            render_i::TEXFILTER_NEAREST,
            render_i::TEXFILTER_LINEAR,
            render_i::TEXFILTER_N_MIP_N,
            render_i::TEXFILTER_N_MIP_L,
            render_i::TEXFILTER_L_MIP_N,
            render_i::TEXFILTER_L_MIP_L,
        };
        const char * str[] = {
            "NEAREST",
            "LINEAR",
            "N_MIP_N",
            "N_MIP_L",
            "L_MIP_N",
            "L_MIP_L",
        };

        StrA           s;
        render_i::texfilter_t t;

        for( int i = 0; i < 6; ++i )
        {
            TS_ASSERT( render_i::s_texfilter2str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], render_i::s_texfilter2str(tag[i]) );

            TS_ASSERT( render_i::s_str2texfilter( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );
        }

        TS_ASSERT( !render_i::s_texfilter2str(s,render_i::NUM_TEXFILTERS) );
        TS_ASSERT_EQUALS( "BAD_TEXTURE_FILTER", render_i::s_texfilter2str(render_i::NUM_TEXFILTERS) );
        TS_ASSERT( !render_i::s_str2texfilter( t, "haha" ) );
        TS_ASSERT( !render_i::s_str2texfilter( t, NULL ) );
    }

    //void testTexWrap()
    {
        render_i::texwrap_t tag[] = {
            render_i::TEXWRAP_REPEAT,
            render_i::TEXWRAP_CLAMP,
            render_i::TEXWRAP_CLAMP_TO_EDGE,
        };
        const char * str[] = {
            "REPEAT",
            "CLAMP",
            "CLAMP_TO_EDGE",
        };

        StrA           s;
        render_i::texwrap_t t;

        for( int i = 0; i < 3; ++i )
        {
            TS_ASSERT( render_i::s_texwrap2str(s,tag[i]) );
            TS_ASSERT_EQUALS( str[i], s );

            TS_ASSERT_EQUALS( str[i], render_i::s_texwrap2str(tag[i]) );

            TS_ASSERT( render_i::s_str2texwrap( t, str[i] ) );
            TS_ASSERT_EQUALS( tag[i], t );
        }

        TS_ASSERT( !render_i::s_texwrap2str(s,render_i::NUM_TEXWRAPS) );
        TS_ASSERT_EQUALS( "BAD_TEXTURE_WRAP", render_i::s_texwrap2str(render_i::NUM_TEXWRAPS) );
        TS_ASSERT( !render_i::s_str2texwrap( t, "haha" ) );
        TS_ASSERT( !render_i::s_str2texwrap( t, NULL ) );
    }*/
};
