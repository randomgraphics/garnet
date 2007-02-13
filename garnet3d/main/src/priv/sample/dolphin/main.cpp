#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static const UInt32 DOLPHIN_VERTICES  = 284;
static const UInt32 DOLPHIN_FACES     = 564;

static const UInt32 SEAFLOOR_VERTICES = 1490;
static const UInt32 SEAFLOOR_FACES    = 2830;

static const UInt32 VERTEX_STRIDE     = 32;

static const Vector4f WATER_COLOR( 0.0f, 0.25f, 0.5f, 1.0f );

static GN::Logger * sLogger = GN::getLogger("GN.sample.dolphin");

class Scene
{
    GN::app::SampleApp & mApp;
    
    AutoRef<Texture> mDolphinSkin;
    AutoRef<Texture> mSeafloor;
    AutoRef<Texture> mCaustics[32];

    AutoRef<VtxBuf> mDolphinVb[3];
    AutoRef<VtxBuf> mSeafloorVb;

    AutoRef<IdxBuf> mDolphinIb, mSeafloorIb;

    VtxFmtHandle mDolphinDecl, mSeafloorDecl;

    UInt32 mDolphinEff, mSeafloorEff;

public:

    Scene( GN::app::SampleApp & app ) : mApp(app) {}

    ~Scene() { }

    static bool loadvb( AutoRef<VtxBuf> & vb, size_t count, const char * filename )
    {
        Renderer & r = gRenderer;

        StrA fullpath = GN::app::SampleResourceManager::sSearchResourceFile( filename );
        if( fullpath.empty() )
        {
            GN_ERROR(sLogger)( "file %s not found", filename );
            return false;
        }

        // open file
        DiskFile fp;
        if( !fp.open( fullpath, "rb" ) ) return false;

        size_t bytes = count * VERTEX_STRIDE;

        // create vb
        vb.attach( r.createVtxBuf( bytes ) );
        if( vb.empty() ) return false;

        // fill vb
        void * dst = vb->lock( 0, 0, LOCK_WO );
        if( 0 == dst ) return false;
        AutoBufferUnlocker<VtxBuf> unlocker( vb );
        if( !fp.read( dst, bytes, 0 ) ) return false;

        // success
        return true;
    }

    static bool loadib( AutoRef<IdxBuf> & ib, size_t faces, const char * filename )
    {
        Renderer & r = gRenderer;

        StrA fullpath = GN::app::SampleResourceManager::sSearchResourceFile( filename );
        if( fullpath.empty() )
        {
            GN_ERROR(sLogger)( "file %s not found", filename );
            return false;
        }

        // open file
        DiskFile fp;
        if( !fp.open( fullpath, "rb" ) ) return false;

        size_t indices = faces * 3; // triangle list
        size_t bytes   = indices * 2; // 16-bit index buffer

        // create ib
        ib.attach( r.createIdxBuf( indices ) );
        if( ib.empty() ) return false;

        // fill ib
        void * dst = ib->lock( 0, 0, LOCK_WO );
        if( 0 == dst ) return false;
        AutoBufferUnlocker<IdxBuf> unlocker( ib );
        if( !fp.read( dst, bytes, 0 ) ) return false;

        // success
        return true;
    }

    static bool loadtex( AutoRef<Texture> & tex, const StrA & filename )
    {
        tex.attach( GN::app::SampleResourceManager::sCreateTextureFromFile( filename ) );
        if( 0 == tex ) return false;
        return true;
    }

    bool create()
    {
        // load dolphin mesh
        if( !loadvb( mDolphinVb[0], DOLPHIN_VERTICES, "dolphin/dolphin_1.vb" ) ) return false;
        if( !loadvb( mDolphinVb[1], DOLPHIN_VERTICES, "dolphin/dolphin_2.vb" ) ) return false;
        if( !loadvb( mDolphinVb[2], DOLPHIN_VERTICES, "dolphin/dolphin_3.vb" ) ) return false;
        if( !loadib( mDolphinIb   , DOLPHIN_FACES   , "dolphin/dolphin.ib"   ) ) return false;

        // load seafloor mesh
        if( !loadvb( mSeafloorVb, SEAFLOOR_VERTICES, "dolphin/seafloor.vb" ) ) return false;
        if( !loadib( mSeafloorIb, SEAFLOOR_FACES   , "dolphin/seafloor.ib" ) ) return false;

        // load textures
        if( !loadtex( mDolphinSkin, "dolphin/dolphin.bmp" ) ) return false;
        if( !loadtex( mSeafloor, "dolphin/seafloor.bmp" ) ) return false;
        for( int i = 0; i < 32; ++i )
        {
            if( !loadtex( mCaustics[i], strFormat( "dolphin/caust%02d.tga", i ) ) ) return false;
        }

        // create decl
        VtxFmtDesc vfd;
        vfd.clear();
        vfd.addAttrib( 0,  0, VTXSEM_POS0, FMT_FLOAT3 );
        vfd.addAttrib( 0, 12, VTXSEM_NML0, FMT_FLOAT3 );
        vfd.addAttrib( 0, 24, VTXSEM_TEX0, FMT_FLOAT2 );
        vfd.addAttrib( 1,  0, VTXSEM_TEX1, FMT_FLOAT3 ); // pos1
        vfd.addAttrib( 1, 12, VTXSEM_TEX2, FMT_FLOAT3 ); // nrm1
        vfd.addAttrib( 2,  0, VTXSEM_TEX3, FMT_FLOAT3 ); // pos2
        vfd.addAttrib( 2, 12, VTXSEM_TEX4, FMT_FLOAT3 ); // tex2
        mDolphinDecl = gRenderer.createVtxFmt( vfd );
        if( 0 == mDolphinDecl ) return false;

        mSeafloorDecl = gRenderer.createVtxFmt( VtxFmtDesc::XYZ_NORM_UV );
        if( 0 == mSeafloorDecl ) return false;

        // load effect
        GN::app::SampleResourceManager & rm = mApp.getResMgr();
        mSeafloorEff = rm.effects.getResourceHandle( "dolphin/seafloor.xml" );
        if( 0 == mSeafloorEff ) return false;
        mDolphinEff = rm.effects.getResourceHandle( "dolphin/dolphin.xml" );
        if( 0 == mDolphinEff ) return false;

        // initiate constant effect parameters
        Effect * eff;
        eff = rm.effects.getResource( mSeafloorEff );
        eff->setTextureByName( "seafloor", mSeafloor );
        eff = rm.effects.getResource( mDolphinEff );
        eff->setTextureByName( "skin", mDolphinSkin );

        // success
        return true;
    }

    void update( float time, const Matrix44f & view, const Matrix44f & proj )
    {
        GN::app::SampleResourceManager & rm = mApp.getResMgr();
        Effect * eff;

        // update caustic parameters
        Vector4f caustics( 0.05f, 0.05f, sinf(time)/8, cosf(time)/10 );
        UInt32 causticTex = ((UInt32)(time*32))%32;

        // update seafloor effect parameters
        eff = rm.effects.getResource( mSeafloorEff );
        eff->setUniformByName( "view", view );
        eff->setUniformByName( "proj", proj );
        eff->setUniformByName( "caustic", caustics );
        eff->setTextureByName( "caustic", mCaustics[causticTex] );

        // Animation attributes for the dolphin
        float fKickFreq    = 2*time;
        float fPhase       = time/3;
        float fBlendWeight = sinf( fKickFreq );

        // Move dolphin in a circle
        Matrix44f scale, trans, rotate1, rotate2;
        scale.identity(); scale *= 0.01f; scale[3][3] = 1.0f;
        rotate1.rotateZ( -cosf(fKickFreq)/6 );
        rotate2.rotateY( fPhase );
        float radius = 5;
        trans.translate( -radius*sinf(fPhase), sinf(fKickFreq)/2, (2*radius)-(2*radius)*cosf(fPhase) );
        Matrix44f world = trans * rotate2 * rotate1 * scale;

        // calculate vertex blending weights
        float fWeight1;
        float fWeight2;
        float fWeight3;
        if( fBlendWeight > 0.0f )
        {
            fWeight1 = fabsf(fBlendWeight);
            fWeight2 = 1.0f - fabsf(fBlendWeight);
            fWeight3 = 0.0f;
        }
        else
        {
            fWeight1 = 0.0f;
            fWeight2 = 1.0f - fabsf(fBlendWeight);
            fWeight3 = fabsf(fBlendWeight);
        }
        Vector4f vWeight( fWeight1, fWeight2, fWeight3, 0.0f );

        // update dolphin effect parameters
        eff = rm.effects.getResource( mDolphinEff );
        eff->setUniformByName( "weights", vWeight );
        eff->setUniformByName( "viewworld", view * world );
        eff->setUniformByName( "pvw", proj * view * world );
        eff->setTextureByName( "caustic", mCaustics[causticTex] );
    }

    void render()
    {
        Renderer & r = gRenderer;

        // clear screen
        r.clearScreen( WATER_COLOR );

        // render seafloor
        r.contextUpdateBegin();
            r.setVtxFmt( mSeafloorDecl );
            r.setVtxBuf( 0, mSeafloorVb, 0, VERTEX_STRIDE );
            r.setIdxBuf( mSeafloorIb );
        r.contextUpdateEnd();
        GN::app::SampleResourceManager & rm = mApp.getResMgr();
        Effect * eff = rm.effects.getResource( mSeafloorEff );
        for( size_t i = 0; i < eff->getNumPasses(); ++i )
        {
            eff->passBegin( i );
            eff->commitChanges();
            r.drawIndexed( TRIANGLE_LIST, SEAFLOOR_FACES, 0, 0, SEAFLOOR_VERTICES, 0 );
            eff->passEnd();
        }

        // render dolphin
        r.contextUpdateBegin();
            r.setVtxFmt( mDolphinDecl );
            r.setVtxBuf( 0, mDolphinVb[0], 0, VERTEX_STRIDE );
            r.setVtxBuf( 1, mDolphinVb[1], 0, VERTEX_STRIDE );
            r.setVtxBuf( 2, mDolphinVb[2], 0, VERTEX_STRIDE );
            r.setIdxBuf( mDolphinIb );
        r.contextUpdateEnd();
        eff = rm.effects.getResource( mDolphinEff );
        for( size_t i = 0; i < eff->getNumPasses(); ++i )
        {
            eff->passBegin( i );
            eff->commitChanges();
            r.drawIndexed( TRIANGLE_LIST, DOLPHIN_FACES, 0, 0, DOLPHIN_VERTICES, 0 );
            eff->passEnd();
        }
    }
};

class Dolphin : public GN::app::SampleApp
{
    Scene * scene;

    float time;
    bool swimming;

    Matrix44f world, view, proj;

    AutoRef<Texture> rt[2];
    RenderTargetDesc rtdesc;

public:

    Dolphin() : scene(0)
    {
        // initialize 
        time = .0f;
        swimming = true;

        Vector3f eye(0,0,-2.0f);
        Vector3f at(0,0,0);
        Vector3f up(0,1,0);
        view.lookAtLh( eye, at, up );

        world.identity();
    }

    bool onRendererRestore()
    {
        Renderer & r = gRenderer;

        UInt32 width = 1024;
        UInt32 height = 720;

        float aspect = (float)width / height;
        r.composePerspectiveMatrixLh( proj, GN_PI/3, aspect, 1.0f, 1000.0f );

        // create render targets
        rt[0].attach( r.create2DTexture( width, height, 1, FMT_FLOAT16_4, TEXUSAGE_RENDER_TARGET ) );
        rt[1].attach( r.create2DTexture( width, height, 1, FMT_RG_32_32_FLOAT, TEXUSAGE_RENDER_TARGET ) );

        rtdesc.count = 2;
        rtdesc.aa = MSAA_NONE;
        rtdesc.setcbuf( 0, rt[0] );
        rtdesc.setcbuf( 1, rt[1] );
        rtdesc.setzbuf( 0 ); // use automatic z buffer.

        // create scene
        scene = new Scene(*this);
        return scene->create();
    }

    void onRendererDispose()
    {
        safeDelete( scene );
        rt[0].clear();
        rt[1].clear();
    }

    void onKeyPress( input::KeyEvent key )
    {
        GN::app::SampleApp::onKeyPress( key );

        if( input::KEY_SPACEBAR == key.code && key.status.down )
        {
            swimming = !swimming;
        }
    }

    void onUpdate()
    {
        if( swimming ) time += 1.0f/60.0f;
        Matrix44f pvw = proj * view;
        scene->update( time, view, pvw );
    }

    void onRender()
    {
        GN_ASSERT( scene );

        Renderer & r = gRenderer;

        // draw to textures
        r.setRenderTargets( rtdesc );
        scene->render();

        // draw texture to screen
        r.setRenderTargets( RenderTargetDesc::DRAW_TO_BACK_BUFFER );
        r.setTexture( 0, rt[0] );
        r.draw2DTexturedQuad( 0 );
    }
};

int main( int argc, const char * argv[] )
{
    Dolphin app;
    return app.run( argc, argv );
}
