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
static const EffectItem EFF_GENERATE_CUBE_MAP;
static const EffectItem EFF_GENERATE_DEPTH_TEXTURE;
static const EffectItem EFF_LIGHT_MAP;
static const EffectItem EFF_DEPTH_BLUR;

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
    static BufferId mBackBuffer;
    static BufferId mZBuffer;

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

    void draw( EffectManager & mgr )
    {
        // do clear
        DrawParameters cp;
        cp.effect = EFF_CLEAR.id;
        cp.buffers["color0_buffer"] = mBackBuffer;
        cp.buffers["depth_buffer"] = mZBuffer;
        //cp.buffers["stencil_buffer"] = mStencil;
        //cp.consts["color0_value"] = Vector2f(0,0,0,0);
        //cp.consts["depth_value"] = 1.0f;
        mgr.draw( cp );
    }
};
