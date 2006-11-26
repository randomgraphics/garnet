#include "pch.h"
#include "garnet/gfx/nes.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.NES");

// *****************************************************************************
// NES test code
// *****************************************************************************

using namespace GN;
using namespace GN::gfx::nes;

static const EffectId EFF_CLEAR;
static const EffectId EFF_PRESENT;
static const EffectId EFF_DIFFUSE_TEX;
static const EffectId EFF_GENERATE_DEPTH_TEXTURE;
static const EffectId EFF_DEPTH_BLUR;
static const EffectId EFF_ENV_REFL;

namespace GN { namespace gfx { namespace nes { namespace stdeff
{
    struct Clear : public Effect
    {
        Clear() : Effect(EFF_CLEAR) {}

        static const EffectDesc & DESC;

        BufferId target[4], depth, stencil;
        Vector4f c;
        float    z;
        int      s;
    };

    struct Present : public Effect
    {
        Present() : Effect(EFF_PRESENT) {}
        static const EffectDesc & DESC;
        BufferId backbuf;
    };

    struct DiffuseTex : public Effect
    {
        DiffuseTex : Effect( EFF_DIFFUSE_TEX ) {}
        static const EffectDesc & DESC;
        BufferId target, depth, tex;
        Vector3f lightPos;
    };

    struct EnvReflection : public Effect
    {
        EnvReflection() : Effect(EFF_ENV_REFL) {}
        static const EffectDesc & DESC;
        BufferId env[6], target, depth;
        Matrix33 proj, view, world;
    };
}}}}

static bool sGenBackBuffers( EffectManager & mgr, BufferId & c, BufferId & z )
{
    BufferCreationParameters bcp;

    // create back buffer
    bcp.type = BT_PXLBUF;
    bcp.ca = CA_IMMUTABLE;
    bcp.sysMem = 0;
    bcp.parent = 0;
    bcp.bindToEffect( EFF_CLEAR, "target0" );
    bcp.bindToEffect( EFF_PRESENT, "backbuffer" );
    c = mgr.createBuffer( bcp );
    if( !c ) return false;

    // create default depth buffer
    bcp.bindToEffect( EFF_CLEAR, "depth_buffer" );
    z = mgr.createBuffer( bcp );
    if( !z ) return false;

    // success
    return true;
}

class TestScene
{
    BufferId mBackBuffer;
    BufferId mZBuffer;

    BufferId mCubemap;
    BufferId mCubeFaces[6];

private:

    bool genCubemap( EffectManager & mgr )
    {
        BufferCreationParameters bcp;

        bcp.type = BT_PXLBUF;
        bcp.ca = CA_IMMUTABLE;
        bcp.sysMem = 0;
        bcp.parent = 0;
        ...;
        mCubemap = mgr.createBufer( bcp );

        return true;
    }

public:

    bool init( EffectManager & mgr )
    {
        // create backbuffer and z buffer
        if( !sGenBackBuffers( mgr, mBackBuffer, mZBuffer ) ) return false;

        // create cubemap
        if( !genCubemap( mgr ) ) return false;

        // create constants

        // success
        return true;
    }

    void cubemap( EffectManager & mgr )
    {
        DrawParameters cp;

        cp.effect = EFF_GENERATE_CUBE_MAP.id;
        cp.buffers["cubemap"] = mCubemap;
        cp.consts["center"].setv3( x, y, z );
        //cp.consts["quality"].set("hight");

        mgr.draw( cp );
    }

    void renderReflection( EffectManager & mgr )
    {
        DrawParameters cp;
        cp.effect = EFF_ENV_REFL.id;
        cp.buffers["envmap"] = mCubemap;
        cp.buffers["target"] = mBackBuffer;
        cp.buffers["depth"] = mZBuffer;
        mgr.draw( cp );
    }

    void clear( EffectManager & mgr )
    {
        stdeff::Clear c;
        c.target[0] = mBackBuffer;
        c.depth = mZBuffer;
        //c.stencil = mStencil;
        c.c.set(0,0,0,0);
        c.z = 1.0f;
        c.s = 0;
        mgr.draw( c );
    }

    void present( EffectManager & mgr )
    {
        stdeff::Present p;
        p.backbuf = mBackBuffer;
        mgr.draw( p );
    }
};
