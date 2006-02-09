#include "../../testCommon.h"
#include "garnet/GNgfx.h"
#include "garnet/gfx/effect.h"

class EffectTest : public CxxTest::TestSuite
{
    GN::AutoObjPtr<GN::gfx::Renderer> mRenderer;

public:

    void setUp()
    {
        using namespace GN;
        using namespace GN::gfx;

        // create fake renderer
        RendererOptions ro;
        mRenderer.attach( createFakeRenderer(ro) );
        TS_ASSERT( mRenderer.get() );

        // initialize texture dictionary
        gTexDict.addResource( "tex0" );
    }

    void tearDown()
    {
        gTexDict.clear();
        mRenderer.clear();
    }

    void initDesc1( GN::gfx::effect::EffectDesc & desc )
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        desc.clear();

        // create 2 uniforms
        desc.uniforms["u0"];
        desc.uniforms["u1"];
        desc.uniforms["u2"].hasDefaultValue = true;
        desc.uniforms["u2"].defaultValue.setM( &Matrix44f::IDENTITY, 1 );

        // create 2 textures
        desc.textures["t0"].defaultValue = "tex0";
        desc.textures["t1"].defaultValue = "tex1";

        // create 1 vertex shader
        ShaderDesc & vs0 = desc.shaders["vs0"];
        vs0.code = "vs.1.1\nmov oPos, c0";
        vs0.type = VERTEX_SHADER;
        vs0.lang = LANG_D3D_ASM;
        vs0.uniforms["c0"] = "u0";
        vs0.uniforms["c1"] = "u1";
        vs0.uniforms["FFP_TRANSFORM_WORLD"] = "u2";

        // create another vertex shader (FFP shader)
        ShaderDesc & vs1 = desc.shaders["vs1"];
        vs1.type = VERTEX_SHADER;
        vs1.uniforms["FFP_TRANSFORM_VIEW"] = "u2";

        // create 1 pixel shader
        ShaderDesc & ps0 = desc.shaders["ps0"];
        ps0.code = "ps.1.1\nmov oPos, c0";
        ps0.type = PIXEL_SHADER;
        ps0.lang = LANG_D3D_ASM;
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

    void testEmptyTechniqueList()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        desc.techniques.clear();

        Effect e;
        TS_ASSERT( !e.init( *mRenderer, desc ) );
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
        TS_ASSERT( !e.init( *mRenderer, desc ) );
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
        TS_ASSERT( !e.init( *mRenderer, desc ) );
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
        TS_ASSERT( !e.init( *mRenderer, desc ) );
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
        TS_ASSERT( !e.init( *mRenderer, desc ) );
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
        TS_ASSERT( !e.init( *mRenderer, desc ) );
    }

    void testInvalidUniformBinding()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        desc.shaders["vs1"].uniforms["c0"] = "u0";

        Effect e;
        TS_ASSERT( !e.init( *mRenderer, desc ) );
    }

    void testInvalidPassRsb()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        PassDesc & p0 = desc.techniques["t0"].passes[0];
        p0.rsb.rs[RS_ALPHA_TEST] = NUM_RENDER_STATE_VALUES;

        Effect e;
        TS_ASSERT( !e.init( *mRenderer, desc ) );
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
        TS_ASSERT( !e.init( *mRenderer, desc ) );
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
        TS_ASSERT( !e.init( *mRenderer, desc ) );
    }

    void testInvalidTechniqueRsb()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        desc.techniques["t0"].rsb.rs[RS_ALPHA_TEST] = NUM_RENDER_STATE_VALUES;

        Effect e;
        TS_ASSERT( !e.init( *mRenderer, desc ) );
    }

    void testInvalidGlobalRsb()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        desc.rsb.rs[RS_ALPHA_TEST] = NUM_RENDER_STATE_VALUES;

        Effect e;
        TS_ASSERT( !e.init( *mRenderer, desc ) );
    }

    void testValidDesc()
    {
        using namespace GN;
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;
        initDesc1( desc );

        Effect e;
        TS_ASSERT( e.init( *mRenderer, desc ) );
    }
};
