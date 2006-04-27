#include "../testCommon.h"
#include "garnet/GNgfx.h"
#include "garnet/gfx/effect.h"

class EffectTest : public CxxTest::TestSuite
{
public:

    void setUp()
    {
        using namespace GN;
        using namespace GN::gfx;
        TS_ASSERT( createRenderer( API_FAKE ) );
        RendererOptions ro;
        gRenderer.changeOptions( ro );
    }

    void tearDown()
    {
        GN::gfx::deleteRenderer();
    }

    void testCondExp()
    {
        using namespace GN;
        using namespace GN::gfx;

        // empty condition
        EffectDesc::CondExp emptyExp;
        TS_ASSERT( emptyExp.evaluate() );

        // comparasion
        EffectDesc::CondExp c0 = EffectDesc::CondExp::sValue(0);
        EffectDesc::CondExp c1 = EffectDesc::CondExp::sValue(1);
        TS_ASSERT( (c0< c1).evaluate() );
        TS_ASSERT( (c0<=c1).evaluate() );
        TS_ASSERT( (c0==c0).evaluate() );
        TS_ASSERT( (c0!=c1).evaluate() );
        TS_ASSERT( (c1>=c0).evaluate() );
        TS_ASSERT( (c1> c0).evaluate() );

        // relation
        TS_ASSERT( !(c0&&c1).evaluate() );
        TS_ASSERT(  (c1&&c1).evaluate() );
        TS_ASSERT( !(c0||c0).evaluate() );
        TS_ASSERT(  (c0||c1).evaluate() );

        // complex
        EffectDesc::CondExp c2 = EffectDesc::CondExp::sValue(2);
        EffectDesc::CondExp c3 = EffectDesc::CondExp::sValue(2);
        TS_ASSERT( ((c0==c1)||(c2==c3)||(c1==c3)).evaluate() );
    }

    void initDesc1( GN::gfx::EffectDesc & desc )
    {
        using namespace GN;
        using namespace GN::gfx;

        desc.clear();

        // create uniforms
        desc.uniforms["color"].hasDefaultValue = true;
        desc.uniforms["color"].defaultValue.setV( Vector4f(1,0,0,1) );

        desc.uniforms["pvw"].hasDefaultValue = true;
        desc.uniforms["pvw"].defaultValue.setM(
            Matrix44f( 2,  0,  0, -1,
                       0, -2,  0,  1,
                       0,  0,  1,  0,
                       0,  0,  0,  1 ) );

        // create textures
        desc.textures["t0"];
        desc.textures["t1"];

        // create 1 vertex shader
        EffectDesc::ShaderDesc & vs0 = desc.shaders["vs0"];
        vs0.code =
            "vs.1.1\n "
            "dcl_position v0 \n"
            "dcl_texcoord v1 \n"
            "m4x4 oPos, v0, c0 \n"
            "mov oT0, v1 \n"
            "mov oD0, c4";
        vs0.type = VERTEX_SHADER;
        vs0.lang = LANG_D3D_ASM;
        vs0.uniforms["c0"] = "pvw";
        vs0.uniforms["c4"] = "color";

        // create another vertex shader (FFP shader)
        EffectDesc::ShaderDesc & vs1 = desc.shaders["vs1"];
        vs1.type = VERTEX_SHADER;
        vs1.uniforms["FFP_TRANSFORM_VIEW"] = "pvw";

        // create 1 pixel shader
        EffectDesc::ShaderDesc & ps0 = desc.shaders["ps0"];
        ps0.code =
            "ps.1.1 \n"
            "mov r0, c0";
        ps0.type = PIXEL_SHADER;
        ps0.lang = LANG_D3D_ASM;
        ps0.uniforms["c0"] = "color";
        ps0.textures[0] = "t0";
        ps0.textures[1] = "t1";

        // create 1 technique
        EffectDesc::TechniqueDesc tech0;
        tech0.name = "t0";
        tech0.passes.resize(1);
        EffectDesc::PassDesc & p0 = tech0.passes[0];
        p0.shaders[VERTEX_SHADER] = "vs0";
        p0.shaders[PIXEL_SHADER] = "ps0";
        desc.techniques.push_back( tech0 );

        // create another technique
        EffectDesc::TechniqueDesc tech1;
        tech1.name = "t1";
        tech1.passes.resize(1);
        EffectDesc::PassDesc & p1 = tech1.passes[0];
        p1.shaders[VERTEX_SHADER] = "vs1";
        p1.shaders[PIXEL_SHADER] = "ps0";
        desc.techniques.push_back( tech1 );
    }

    void doDraw( const GN::gfx::Effect & e )
    {
        using namespace GN;
        using namespace GN::gfx;

        if( !gRendererPtr ) return;

        gRenderer.clearScreen();
        if( gRenderer.drawBegin() )
        {
            for( size_t i = 0; i < e.getNumPasses(); ++i )
            {
                e.passBegin( i );
                e.commitChanges();
                gRenderer.draw2DTexturedQuad( DQ_USE_CURRENT, 0.5, 0.5, 1.0, 1.0 );
                e.passEnd();
            }
            gRenderer.drawEnd();
        }
    }

    void testEmptyTechniqueList()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        desc.techniques.clear();

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidShaderType()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        desc.shaders["vs0"].type = NUM_SHADER_TYPES;

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidShadingLang()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        desc.shaders["vs0"].lang = NUM_SHADING_LANGUAGES;

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidTextureReference()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        desc.shaders["ps0"].textures[0] = "haha";

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidTextureStage()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        desc.shaders["ps0"].textures[MAX_TEXTURE_STAGES] = "t0";

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidUniformReference()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        desc.shaders["vs0"].uniforms["c0"] = "haha";

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidUniformBinding()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        desc.shaders["vs0"].uniforms["haha"] = "pvw";

        Effect e;
        TS_ASSERT( e.init( desc ) );

        doDraw( e );
    }

    void testInvalidPassRsb()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        EffectDesc::PassDesc & p0 = desc.techniques[0].passes[0];
        GN::RuntimeAssertBehavior rab = GN::setRuntimeAssertBehavior( GN::RAB_LOG_ONLY );
        p0.rsb.set( RS_ALPHA_TEST, NUM_RENDER_STATE_VALUES );
        GN::setRuntimeAssertBehavior( rab );

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidShaderReference()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        EffectDesc::PassDesc & p0 = desc.techniques[0].passes[0];
        p0.shaders[VERTEX_SHADER] = "haha";

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidShaderReference2()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        EffectDesc::PassDesc & p0 = desc.techniques[0].passes[0];
        p0.shaders[VERTEX_SHADER] = "ps0";

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidTechniqueRsb()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        GN::RuntimeAssertBehavior rab = GN::setRuntimeAssertBehavior( GN::RAB_LOG_ONLY );
        desc.techniques[0].rsb.set( RS_ALPHA_TEST, NUM_RENDER_STATE_VALUES );
        GN::setRuntimeAssertBehavior( rab );

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidGlobalRsb()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        GN::RuntimeAssertBehavior rab = GN::setRuntimeAssertBehavior( GN::RAB_LOG_ONLY );
        desc.rsb.set( RS_ALPHA_TEST, NUM_RENDER_STATE_VALUES );
        GN::setRuntimeAssertBehavior( rab );

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidShaderCode()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        desc.shaders["vs0"].code = "haha";

        Effect e;
        TS_ASSERT( e.init( desc ) );

        doDraw( e );
    }

    void testDesc1()
    {
        using namespace GN;
        using namespace GN::gfx;

        EffectDesc desc;
        initDesc1( desc );

        Effect e;
        TS_ASSERT( e.init( desc ) );

        doDraw( e );
    }

    // TODO: same shader, same uniform, multiple bindings.

};
