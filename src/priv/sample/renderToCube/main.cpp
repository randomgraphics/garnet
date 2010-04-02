#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

class RenderToTexture : public SampleApp
{
    AutoRef<Texture> faces[6];

    AutoRef<TextureResource> cubemap;

    AutoRef<ModelResource> box;

    Matrix44f world, view, proj, pvw;

    ArcBall arcball;

    GpuContext gc;

    bool initMesh( MeshResource * m )
    {
        Vector3f vertices[24];
        UInt16   indices[36];

        CreateBox( 100, 100, 100,
            (float*)vertices, sizeof(Vector3f),
            NULL, 0, // texcoord
            NULL, 0, // normal
            NULL, 0, // tang
            NULL, 0, // binormal
            indices,
            NULL     // quad list indices
            );

        MeshResourceDesc desc;
        desc.Clear();
        desc.prim = PrimitiveType::TRIANGLE_LIST;
        desc.numvtx = 24;
        desc.numidx = 36;
        desc.vtxfmt.numElements = 1;
        desc.vtxfmt.elements[0].BindTo( "POSITION", 0 );
        desc.vtxfmt.elements[0].format = ColorFormat::RGB_32_32_32_FLOAT;
        desc.vertices[0] = vertices;
        desc.indices = indices;

        return m->Reset( &desc );
    }

public:

    bool OnInit()
    {
        world.Identity();
        view.Translate( 0, 0, -200 );
        proj.PerspectiveD3DRh( 1.0f, 4.0f/3.0f, 100.0f, 1000.0f );
        arcball.setHandness( util::RIGHT_HAND );
        arcball.setViewMatrix( view );
        arcball.connectToInput();

        Gpu                 & gpu = GetGpu();
        GpuResourceDatabase & gdb = GetGdb();

        // load 2D faces
        StrA name = "media::/texture/cube2/a.bmp";
        for( unsigned char i = 0; i < 6; ++i )
        {
            name[22] = 'a' + i;
            faces[i].Attach( LoadTextureFromFile( gpu, name ) );
            if( 0 == faces[i] ) return false;
        }

        // create cube render target
        cubemap = gdb.CreateResource<TextureResource>( NULL );
        if( !cubemap ) return false;
        TextureDesc texdesc;
        texdesc.width = texdesc.height = 512;
        texdesc.depth = 1;
        texdesc.faces = 6;
        texdesc.levels = 1;
        texdesc.format = ColorFormat::RGBA32;
        texdesc.usage = TextureUsage::COLOR_RENDER_TARGET;
        if( !cubemap->Reset( &texdesc ) ) return false;

        // load cube texture rendering effect
        AutoRef<EffectResource> cubefx = EffectResource::LoadFromFile( gdb, "media::cube/cube_on_cube.effect.xml" );
        if( !cubefx ) return false;

        // load cube mesh
        AutoRef<MeshResource> cubemesh = gdb.CreateResource<MeshResource>( NULL );
        if( !cubemesh ) return false;
        if( !initMesh( cubemesh ) ) return false;

        // create cube model
        box = gdb.CreateResource<ModelResource>( NULL );
        if( !box ) return false;

        // setup model effect and textures
        box->SetEffectResource( cubefx );
        box->SetMeshResource( cubemesh );
        box->SetTextureResource( "cube", cubemap );

        // initial arcball window
        const DispDesc & dd = gpu.GetDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );

        // initialize GPU context
        gc.Clear();

        return true;
    }

    void OnQuit()
    {
        for( int i = 0; i < 6; ++i ) faces[i].Clear();
        cubemap.Clear();
        box.Clear();
        gc.Clear();
    }

    void OnUpdate()
    {
        world = arcball.getRotationMatrix44();
        pvw = proj * view * world;
        box->GetUniformResource("pvw")->GetUniform()->Update( pvw );
    }

    void OnRender()
    {
        Gpu            & gpu = GetGpu();
        SpriteRenderer & sr = GetSpriteRenderer();

        // draw to cubemap
        gc.colortargets.Resize( 1 );
        gc.colortargets[0].texture = cubemap->GetTexture();
        gc.colortargets[0].subsurface = 0;
        for( int i = 0; i < 6; ++i )
        {
            gc.colortargets[0].face = i;
            gpu.BindContext( gc );
            sr.DrawSingleTexturedSprite( faces[i], 0, 1, 1, 510.0f, 510.0f );
        }

        // draw the cube model to screen
        gc.colortargets.Clear();
        gpu.BindContext( gc );
        gpu.ClearScreen();
        box->Draw();
    }
};

int main( int argc, const char * argv[] )
{
    RenderToTexture app;
    return app.Run( argc, argv );
}
