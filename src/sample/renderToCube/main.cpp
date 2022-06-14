#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

class RenderToTexture : public SampleApp {
    AutoRef<Texture> faces[6];

    AutoRef<TextureResource> cubemap;

    AutoRef<ModelResource> box;

    Matrix44f world, view, proj, pvw;

    ArcBall arcball;

    GpuContext gc;

    bool initMesh(MeshResource * m) {
        Vector3f vertices[24];
        uint16   indices[36];

        createBox(100, 100, 100, (float *) vertices, sizeof(Vector3f), NULL, 0, // texcoord
                  NULL, 0,                                                      // normal
                  NULL, 0,                                                      // tang
                  NULL, 0,                                                      // binormal
                  indices,
                  NULL // quad list indices
        );

        MeshResourceDesc desc;
        desc.prim               = PrimitiveType::TRIANGLE_LIST;
        desc.numvtx             = 24;
        desc.numidx             = 36;
        desc.vtxfmt.numElements = 1;
        desc.vtxfmt.elements[0].setSemantic("POSITION0");
        desc.vtxfmt.elements[0].format = ColorFormat::RGB_32_32_32_FLOAT;
        desc.vertices[0]               = vertices;
        desc.indices                   = indices;

        return m->reset(&desc);
    }

public:
    bool onInit() {
        world.identity();
        view.translate(0, 0, -200);
        proj.perspectiveD3DRh(1.0f, 4.0f / 3.0f, 100.0f, 1000.0f);
        arcball.setHandness(util::RIGHT_HAND);
        arcball.setViewMatrix(view);
        arcball.connectToInput();

        Gpu &                 gpu = *engine::getGpu();
        GpuResourceDatabase & gdb = *engine::getGdb();

        // load 2D faces
        StrA name = "media::/texture/cube2/a.bmp";
        for (unsigned char i = 0; i < 6; ++i) {
            name[22] = 'a' + i;
            faces[i].attach(loadTextureFromFile(gpu, name));
            if (0 == faces[i]) return false;
        }

        // create cube render target
        cubemap = gdb.createResource<TextureResource>(NULL);
        if (!cubemap) return false;
        TextureDesc texdesc;
        texdesc.width = texdesc.height = 512;
        texdesc.depth                  = 1;
        texdesc.faces                  = 6;
        texdesc.levels                 = 1;
        texdesc.format                 = ColorFormat::RGBA8;
        texdesc.usage                  = TextureUsage::COLOR_RENDER_TARGET;
        if (!cubemap->reset(&texdesc)) return false;

        // load cube texture rendering effect
        AutoRef<EffectResource> cubefx = EffectResource::loadFromFile(gdb, "media::cube/cube_on_cube.effect.xml");
        if (!cubefx) return false;

        // load cube mesh
        AutoRef<MeshResource> cubemesh = gdb.createResource<MeshResource>(NULL);
        if (!cubemesh) return false;
        if (!initMesh(cubemesh)) return false;

        // create cube model
        box = gdb.createResource<ModelResource>(NULL);
        if (!box) return false;

        // setup model effect and textures
        box->setEffectResource(cubefx);
        box->setMeshResource(cubemesh);
        box->setTextureResource("cube", cubemap);

        // initial arcball window
        const DispDesc & dd = gpu.getDispDesc();
        arcball.setMouseMoveWindow(0, 0, (int) dd.width, (int) dd.height);

        // initialize GPU context
        gc.clear();

        return true;
    }

    void onQuit() {
        for (int i = 0; i < 6; ++i) faces[i].clear();
        cubemap.clear();
        box.clear();
        gc.clear();
    }

    void onUpdate() {
        world = arcball.getRotationMatrix44();
        pvw   = proj * view * world;
        box->uniformResource("pvw")->uniform()->update(pvw);
    }

    void onRender() {
        Gpu &            gpu = *engine::getGpu();
        SpriteRenderer & sr  = *engine::getSpriteRenderer();

        // draw to cubemap
        gc.colortargets.resize(1);
        gc.colortargets[0].texture    = cubemap->texture();
        gc.colortargets[0].subsurface = 0;
        for (int i = 0; i < 6; ++i) {
            gc.colortargets[0].face = i;
            gpu.bindContext(gc);
            gpu.clearScreen();
            sr.drawSingleTexturedSprite(faces[i], 0, 1, 1, 510.0f, 510.0f);
        }

        // draw the cube model to screen
        gc.colortargets.clear();
        gpu.bindContext(gc);
        gpu.clearScreen();
        box->draw();
    }
};

int main(int argc, const char * argv[]) {
    RenderToTexture app;
    return app.run(argc, argv);
}
