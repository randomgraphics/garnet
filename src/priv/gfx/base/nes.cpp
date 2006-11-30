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

        static const EffectDesc DESC;

        BufferId target[4], depth, stencil;
        Vector4f c;
        float    z;
        int      s;
    };

    struct Present : public Effect
    {
        Present() : Effect(EFF_PRESENT) {}
        static const EffectDesc DESC;
        BufferId backbuf;
    };

    struct DiffuseTex : public Effect
    {
        DiffuseTex : Effect( EFF_DIFFUSE_TEX ) {}
        static const EffectDesc DESC;
        BufferId target, depth, tex;
        Vector3f lightPos;
    };

    struct EnvReflection : public Effect
    {
        EnvReflection() : Effect(EFF_ENV_REFL) {}
        static const EffectDesc DESC;
        BufferId env[6], target, depth;
        Matrix33 proj, view, world;
    };
}}}}

class TestScene
{
    BufferId mBackBuffer;
    BufferId mZBuffer;

    BufferId mCubemap;
    BufferId mCubeFaces[6];

    struct Camera
    {
        // ...
    };

    struct Object
    {
        virtual void draw( BufferId target, BufferId z, const Camera & c ) = 0;
    };

private:

    bool genBackBuffers( EffectManager & mgr )
    {
        BufferCreationParameters bcp;

        // create back buffer
        bcp.type = BT_PXLBUF;
        bcp.ca = CA_IMMUTABLE;
        bcp.sysMem = 0;
        bcp.parent = 0;
        bcp.bindToEffect( EFF_CLEAR, "target0" );
        bcp.bindToEffect( EFF_PRESENT, "backbuffer" );
        mBackBuffer = mgr.createBuffer( bcp );
        if( !mBackBuffer ) return false;

        // create default depth buffer
        bcp.bindToEffect( EFF_CLEAR, "depth_buffer" );
        mZBuffer = mgr.createBuffer( bcp );
        if( !mZBuffer ) return false;

        // success
        return true;
    }

    bool genCubemap( EffectManager & mgr )
    {
        BufferCreationParameters bcp;

        bcp.type = BT_PXLBUF;
        bcp.ca = CA_IMMUTABLE;
        bcp.sysMem = 0;
        bcp.parent = 0;
        mCubemap = mgr.createBufer( bcp );
        if( !mCubemap ) return false;

        bcp.parent = mCubemap;
        bcp.level = 0;
        bcp.slice = 0;

        for( unsigned int i = 0; i < 6; ++i )
        {
            bcp.face = i;
            mCubeFaces[i] = mgr.createBuffer( bcp );
            if( !mCubeFaces[i] ) return false;
        }

        return true;
    }

public:

    bool init( EffectManager & mgr )
    {
        // create backbuffer and z buffer
        if( !genBackBuffers( mgr ) ) return false;

        // create cubemap
        if( !genCubemap( mgr ) ) return false;

        // create constants

        // success
        return true;
    }

    void genCubemap(
        EffectManager & mgr,
        const Vector3f & center,
        const Object * const * objects,
        size_t count )
    {
        Camera c;
        BufferId depth = mgr.createBuffer( ... ); // create a temporary depth buffer
        
        for( int i = 0; i < 6; ++i )
        {
            // TODO: setup_camera( i, c );
            for( size_t o = 0; o < count; ++o )
            {
                objects[o]->draw( mCubeFaces[i], depth, c );
            }
        }

        mgr.deleteBuffer( depth );
    }

    void renderReflection( EffectManager & mgr )
    {
        stdeff::EnvReflection eff;
        eff.target = mBackBuffer;
        eff.depth = mZBuffer;
        eff.env = mCubeFaces;
        // ...
        mgr.draw( eff );
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
