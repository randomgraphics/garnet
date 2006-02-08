#include "../../testCommon.h"
#include "garnet/GNgfx.h"
#include "garnet/gfx/effect.h"

class EffectTest : public CxxTest::TestSuite
{
public:


    void test1()
    {
        using namespace GN::gfx;
        using namespace GN::gfx::effect;

        EffectDesc desc;

        // create 2 uniforms
        desc.uniforms["wvp"];
        desc.uniforms["color"];

        // create 2 textures
        desc.textures["baseMap"];
        desc.textures["lightMap"];

        // create 2 shaders
        ShaderDesc & vs1 = desc.shaders["vs1"];
        //ShaderDesc & ps1 = desc.shaders["ps1"];

        // create 1 technique
        //TechniqueDesc & tech0 = desc.techniques["default"];

        // setup vs1
        vs1.code = "vs.1.1\nmov oPos, c0";
        vs1.type = VERTEX_SHADER;
        vs1.lang = LANG_D3D_ASM;
    }
};
