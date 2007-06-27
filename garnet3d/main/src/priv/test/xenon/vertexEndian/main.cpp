#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static const char * vs_code =
"struct vsio { float4 pos : POSITION; float4 clr : COLOR0; };\n"
"vsio main( vsio i ) { \n"
"   vsio o; \n"
"   o.pos = i.pos; \n"
"   o.clr = i.clr; \n"
"   return o; \n"
"}";

static const char * ps_code =
"float4 main( float4 clr : COLOR0 ) : COLOR0 { \n"
"   return clr; \n"
"}";

class VtxFmtEndian : public app::SampleApp
{
    GraphicsResource * mVs;
    GraphicsResource * mPs;
    GraphicsResource * mVf;
    GraphicsResource * mVb;
    DrawContext        mCtx;

    struct Vertex
    {
        float          x, y, z, w;
        //unsigned short r, g, b, a;
		unsigned char  c8[8];
    };

public:

	VtxFmtEndian() : mVs(0), mPs(0), mVb(0) {}

	bool onInit()
	{
        RenderEngine  & re = getRenderEngine();

        mVs = re.createShader( "vs", SHADER_VS, LANG_D3D_HLSL, vs_code );
        mPs = re.createShader( "ps", SHADER_PS, LANG_D3D_HLSL, ps_code );
        if( !mVs || !mPs ) return false;

        VtxFmtDesc vfd;
        vfd.clear();
        vfd.addAttrib( 0,  0, VTXSEM_POS0, FMT_FLOAT4 );
        vfd.addAttrib( 0, 16, VTXSEM_CLR0, FMT_USHORT4 );
        mVf = re.createVtxFmt( "vf", vfd );
        if( !mVf ) return false;

        static Vertex vertices[] =
        {
			{ 0, 0, 0, 1, { 0x12, 0x34, 0x56, 0x78, 0xab, 0xcd, 0xFF, 0xFF } },
            { 0, 1, 0, 1, { 0x12, 0x34, 0x56, 0x78, 0xab, 0xcd, 0xFF, 0xFF } },
            { 0, 0, 1, 1, { 0x12, 0x34, 0x56, 0x78, 0xab, 0xcd, 0xFF, 0xFF } },
        };

        mVb = re.createVtxBuf( "vb", sizeof(vertices), false, false, vertices );
        if( 0 == mVb ) return false;

        mCtx.resetToDefault();
        mCtx.setShaders( mVs, mPs, 0 );
		mCtx.setVtxFmt( mVf );
        mCtx.setVtxBuf( 0, mVb, 0, sizeof(Vertex) );

		return true;
	}

	void onQuit()
	{
        safeFreeGraphicsResource( mVs );
        safeFreeGraphicsResource( mPs );
        safeFreeGraphicsResource( mVf );
        safeFreeGraphicsResource( mVb );
	}

	void onUpdate()
	{
	}

	void onKeyPress( input::KeyEvent ke )
	{
		app::SampleApp::onKeyPress( ke );
	}

	void onRender()
	{
        RenderEngine & re = getRenderEngine();

        re.clearScreen( Vector4f(0,0,0,1) ); // clear to pure black

        re.setContext( mCtx );

        re.draw( TRIANGLE_LIST, 1, 0 );
	}
};

int main()
{
	VtxFmtEndian app;
	return app.run( 0, 0 );
}
