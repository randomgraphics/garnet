#include "pch.h"
#include "simple.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

class ShadowMap : public SampleApp {
    SimpleShadowMap * mSimple;

public:
    ShadowMap(): mSimple(0) {}

    bool onInit() {
        mSimple = new SimpleShadowMap(*this);
        if (!mSimple->init("a/objects.actor.xml")) return false;

        // success
        return true;
    }

    void onQuit() {
        delete mSimple;
        mSimple = 0;
    }

    void onUpdate() { mSimple->update(); }

    void onRender() { mSimple->draw(); }
};

int main(int argc, const char * argv[]) {
    ShadowMap app;
    return app.run(argc, argv);
}
