#include "pch.h"
#include "garnet/gfx/nes.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.NES");

// *****************************************************************************
// NES test code
// *****************************************************************************

using namespace GN;
using namespace GN::gfx::nes;

struct EffectItem
{
    EffectId   id;
    EffectDesc desc;
};

static const EffectItem EFF_CLEAR;
static const EffectItem EFF_PRESENT;

static const EffectItem EFF_GENERATE_DEPTH_TEXTURE;
static const EffectItem EFF_DEPTH_BLUR;

static const EffectItem EFF_GENERATE_CUBE_MAP;
static const EffectItem EFF_ENV_REFL;

static bool sGenBackBuffers( EffectManager & mgr, BufferId & c, BufferId & z )
{
    BufferCreationParameters bcp;

    // create back buffer
    bcp.type = BT_PXLBUF;
    bcp.ca = CA_IMMUTABLE;
    //bcp.pb = ...;
    bcp.sysMem = 0;
    bcp.bindToEffect( EFF_CLEAR.id, "color0_buffer" );
    bcp.bindToEffect( EFF_PRESENT.id, "color_buffer" );
    c = mgr.createBuffer( bcp );
    if( !c ) return false;

    // create default depth buffer
    bcp.bindToEffect( EFF_CLEAR.id, "depth_buffer" );
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

private:

    bool genCubemap( EffectManager & mgr )
    {
        BufferCreationParameters bcp;

        bcp.type = BT_PXLBUF;
        bcp.ca = CA_IMMUTABLE;
        bcp.sysMem = 0;

        return true;
    }

public:

    bool init( EffectManager & mgr )
    {
        // create backbuffer and z buffer
        if( !sGenBackBuffers( mgr, mBackBuffer, mZBuffer ) ) return false;

        // create cubemap

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
        cp.buffers["target0"] = mBackBuffer;
        cp.buffers["depth"] = mZBuffer;
        mgr.draw( cp );
    }

    void renderShadowVolume( EffectManager & mgr )
    {
        DrawParameters cp;
        cp.effect = EFF_ENV_REFL.id;
        cp.buffers["envmap"] = mCubemap;
        cp.buffers["target0"] = mBackBuffer;
        cp.buffers["depth"] = mZBuffer;
        mgr.draw( cp );
    }

    void clear( EffectManager & mgr )
    {
        DrawParameters cp;

        cp.effect = EFF_CLEAR.id;
        cp.buffers["target0"] = mBackBuffer;
        cp.buffers["depth"] = mZBuffer;
        //cp.buffers["stencil"] = mStencil;
        //cp.consts["target0_value"].setv(0,0,0,0);
        //cp.consts["depth_value"].setf( 1.0f );
        //cp.consts["stencil_value"].seti( 0 );

        mgr.draw( cp );
    }

    void present( EffectManager & mgr )
    {
        DrawParameters cp;

        cp.effect = EFF_PRESENT.id;
        cp.buffers.clear();
        cp.buffers["backbuffer"] = mBackBuffer;

        mgr.draw( cp );
    }
};
