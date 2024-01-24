#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static const Vector4f WATER_COLOR(0.0f, 0.25f, 0.5f, 1.0f);

static GN::Logger * sLogger = GN::getLogger("GN.sample.dolphin");

class TestScene {
    AutoRef<TextureResource> mCaustics[32];

    AutoRef<ModelResource> mDolphin, mSeafloor;

public:
    TestScene(GN::util::SampleApp &) {}

    ~TestScene() {}

    bool create() {
        GpuResourceDatabase & gdb = *engine::getGdb();

        // load caustic textures
        for (int i = 0; i < 32; ++i) {
            mCaustics[i] = TextureResource::loadFromFile(gdb, str::format("media::dolphin/caust%02d.tga", i));
            if (0 == mCaustics[i]) return false;
        }

        // load dolphin and seafloor
        mDolphin  = ModelResource::loadFromFile(gdb, "media::dolphin/dolphin.model.xml");
        mSeafloor = ModelResource::loadFromFile(gdb, "media::dolphin/seafloor.model.xml");
        if (!mDolphin || !mSeafloor) return false;

        // success
        return true;
    }

    void update(float time, const Matrix44f & view, const Matrix44f & proj) {
        // update caustic parameters
        Vector4f caustics(0.05f, 0.05f, sinf(time) / 8, cosf(time) / 10);
        uint32   causticTex = ((uint32) (time * 32)) % 32;

        // update seafloor effect parameters
        mSeafloor->uniformResource("view")->uniform()->update(view);
        mSeafloor->uniformResource("proj")->uniform()->update(proj);
        mSeafloor->uniformResource("caustic")->uniform()->update(caustics);
        mSeafloor->setTextureResource("caustic", mCaustics[causticTex]);

        // Animation attributes for the dolphin
        float fKickFreq    = 2 * time;
        float fPhase       = time / 3;
        float fBlendWeight = sinf(fKickFreq);

        // Move dolphin in a circle
        Matrix44f scale, trans, rotate1, rotate2;
        scale.identity();
        scale *= 0.01f;
        scale[3][3] = 1.0f;
        rotate1.rotateZ(-cosf(fKickFreq) / 6);
        rotate2.rotateY(fPhase);
        float radius = 5;
        trans.translate(-radius * sinf(fPhase), sinf(fKickFreq) / 2, (2 * radius) - (2 * radius) * cosf(fPhase));
        Matrix44f world = trans * rotate2 * rotate1 * scale;

        // calculate vertex blending weights
        float fWeight1;
        float fWeight2;
        float fWeight3;
        if (fBlendWeight > 0.0f) {
            fWeight1 = fabsf(fBlendWeight);
            fWeight2 = 1.0f - fabsf(fBlendWeight);
            fWeight3 = 0.0f;
        } else {
            fWeight1 = 0.0f;
            fWeight2 = 1.0f - fabsf(fBlendWeight);
            fWeight3 = fabsf(fBlendWeight);
        }
        Vector3f vWeight(fWeight1, fWeight2, fWeight3);

        // update dolphin effect parameters
        mDolphin->uniformResource("pvw")->uniform()->update(proj * view * world);
        mDolphin->uniformResource("viewworld")->uniform()->update(view * world);
        mDolphin->uniformResource("weights")->uniform()->update(vWeight);
        mDolphin->setTextureResource("caustic", mCaustics[causticTex]);
    }

    void render() {
        engine::getGpu()->clearScreen(WATER_COLOR);
        mSeafloor->draw();
        mDolphin->draw();
    }
};

class Dolphin : public GN::util::SampleApp {
    TestScene * scene;

    float time;
    bool  swimming;

    Matrix44f world, view, proj;

public:
    Dolphin(): scene(0) {
        // initialize
        time     = .0f;
        swimming = true;

        Vector3f eye(0, 0, -2.0f);
        Vector3f at(0, 0, 0);
        Vector3f up(0, 1, 0);
        view.lookAtLh(eye, at, up);

        world.identity();
    }

    bool onPreInit(InitParam &) { return true; }

    bool onInit() {
        Gpu & g = *engine::getGpu();

        uint32 width  = g.getDispDesc().width;
        uint32 height = g.getDispDesc().height;

        float aspect = (float) width / height;
        g.composePerspectiveMatrixLh(proj, GN_PI / 3, aspect, 1.0f, 1000.0f);

        // create scene
        scene = new TestScene(*this);
        return scene->create();
    }

    void onQuit() { safeDelete(scene); }

    void onKeyPress(input::KeyEvent key) {
        GN::util::SampleApp::onKeyPress(key);

        if (input::KeyCode::SPACEBAR == key.code() && key.status.down) { swimming = !swimming; }
    }

    void onUpdate() {
        if (swimming) time += 1.0f / 60.0f;
        scene->update(time, view, proj);
    }

    void onRender() {
        GN_ASSERT(scene);
        scene->render();
    }
};

int main(int argc, const char * argv[]) {
    Dolphin app;
    return app.run(argc, argv);
}
