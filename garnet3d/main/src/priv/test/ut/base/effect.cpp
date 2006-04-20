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
        using namespace GN::gfx::effect;

        // empty condition
        CondExp emptyExp;
        TS_ASSERT( emptyExp.evaluate() );

        // comparasion
        CondExp c0 = CondExp::sValue(0);
        CondExp c1 = CondExp::sValue(1);
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
        CondExp c2 = CondExp::sValue(2);
        CondExp c3 = CondExp::sValue(2);
        TS_ASSERT( ((c0==c1)||(c2==c3)||(c1==c3)).evaluate() );
    }

    void initDesc1( GN::gfx::effect::EffectDesc & desc )
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

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
        desc.textures["t0"].defaultValue = "texture/rabit.png";
        desc.textures["t1"].defaultValue = "texture/earth.jpg";

        // create 1 vertex shader
        ShaderDesc & vs0 = desc.shaders["vs0"];
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
        ShaderDesc & vs1 = desc.shaders["vs1"];
        vs1.type = VERTEX_SHADER;
        vs1.uniforms["FFP_TRANSFORM_VIEW"] = "pvw";

        // create 1 pixel shader
        ShaderDesc & ps0 = desc.shaders["ps0"];
        ps0.code =
            "ps.1.1 \n"
            "mov r0, c0";
        ps0.type = PIXEL_SHADER;
        ps0.lang = LANG_D3D_ASM;
        ps0.uniforms["c0"] = "color";
        ps0.textures[0] = "t0";
        ps0.textures[1] = "t1";

        // create 1 technique
        TechniqueDesc & tech0 = desc.techniques["t0"];
        tech0.passes.resize(1);
        PassDesc & p0 = tech0.passes[0];
        p0.shaders[VERTEX_SHADER] = "vs0";
        p0.shaders[PIXEL_SHADER] = "ps0";

        // create another technique
        TechniqueDesc & tech1 = desc.techniques["t1"];
        tech1.passes.resize(1);
        PassDesc & p1 = tech1.passes[0];
        p1.shaders[VERTEX_SHADER] = "vs1";
        p1.shaders[PIXEL_SHADER] = "ps0";
    }

    void doDraw( const GN::gfx::effect::Effect & e )
    {
        using namespace GN;
        using namespace GN::gfx;

        if( !gRendererPtr ) return;

        gRenderer.clearScreen();
        if( gRenderer.drawBegin() )
        {
            size_t n = e.drawBegin();
            for( size_t i = 0; i < n; ++i )
            {
                e.passBegin( i );
                e.commitChanges();
                gRenderer.draw2DTexturedQuad( DQ_USE_CURRENT, 0.5, 0.5, 1.0, 1.0 );
                e.passEnd();
            }
            e.drawEnd();
            gRenderer.drawEnd();
        }
    }

    void testEmptyTechniqueList()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        PassDesc & p0 = desc.techniques["t0"].passes[0];
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
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        PassDesc & p0 = desc.techniques["t0"].passes[0];
        p0.shaders[VERTEX_SHADER] = "haha";

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidShaderReference2()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        PassDesc & p0 = desc.techniques["t0"].passes[0];
        p0.shaders[VERTEX_SHADER] = "ps0";

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidTechniqueRsb()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        GN::RuntimeAssertBehavior rab = GN::setRuntimeAssertBehavior( GN::RAB_LOG_ONLY );
        desc.techniques["t0"].rsb.set( RS_ALPHA_TEST, NUM_RENDER_STATE_VALUES );
        GN::setRuntimeAssertBehavior( rab );

        Effect e;
        TS_ASSERT( !e.init( desc ) );
    }

    void testInvalidGlobalRsb()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

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
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        Effect e;
        TS_ASSERT( e.init( desc ) );

        doDraw( e );
    }

    // TODO: same shader, same uniform, multiple bindings.

};
