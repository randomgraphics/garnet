#include "pch.h"
#include "d3d9Renderer.h"

#if !GN_XENON

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.d3d9.statedumper");

using namespace GN;
using namespace GN::gfx;

#define GET_RENDERER() ((GN::gfx::D3D9Renderer&)gRenderer)
#define GET_DEVICE()  (GET_RENDERER().getDevice())

struct DumpFile
{
    DiskFile fp;

	DumpFile()
	{
        GN::gfx::D3D9Renderer & r = GET_RENDERER();

		char fname[_MAX_PATH];
		sprintf_s( fname, "d3d9statedump_frame(%08d)_draw(%04d).xml",
            r.getFrameCounter(), r.getDrawCounter() );

		if( !fp.open( fname, "wt" ) ) return;

		fprintf(
			fp,
			"<?xml version=\"1.0\" standalone=\"yes\"?>\n"
			"<D3D9StateDump frame=\"%d\" draw=\"%d\">\n",
			r.getFrameCounter(),
			r.getDrawCounter() );
	}

	~DumpFile()
	{
		if( fp )
		{
			fprintf( fp, "</D3D9StateDump>\n" );
			fclose(fp);
		}
	}

	operator FILE*() const { return fp; }
};

//
//
// -----------------------------------------------------------------------------
static void sDumpShaderCode( FILE * fp, const void * binary, const char * tag )
{
	AutoComPtr<ID3DXBuffer> asmbuf;

	D3DXDisassembleShader( (const DWORD*)binary, false, NULL, &asmbuf );

	const char * code = (const char *)asmbuf->GetBufferPointer();
	//size_t       sz   = asmbuf->GetBufferSize();

	fprintf( fp, "\t<%s><![CDATA[\n%s]]>\n\t</%s>\n", tag, code, tag );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpVs( FILE * fp )
{
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	AutoComPtr<IDirect3DVertexShader9> vs;

	dev->GetVertexShader( &vs );

	if( !vs ) return;

	std::vector<UInt8> binbuf;
	UINT sz;
	vs->GetFunction( 0, &sz );
	binbuf.resize( sz );
	vs->GetFunction( &binbuf[0], &sz );

	sDumpShaderCode( fp, &binbuf[0], "vs" );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpPs( FILE * fp )
{
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	AutoComPtr<IDirect3DPixelShader9> ps;

	dev->GetPixelShader( &ps );

	if( !ps ) return;

	std::vector<UInt8> binbuf;
	UINT sz;
	ps->GetFunction( 0, &sz );
	binbuf.resize( sz );
	ps->GetFunction( &binbuf[0], &sz );

	sDumpShaderCode( fp, &binbuf[0], "ps" );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpConsts(
	FILE * fp,
	const float * cf, size_t nf,
	const int   * ci, size_t ni,
	const int   * cb, size_t nb )
{
	// float conts
	fprintf( fp, "\t\t<float>\n" );
	for( size_t i = 0; i < nf; ++i )
	{
		fprintf(
			fp,
			"\t\t\t<f index=\"%03d\" x=\"%f\" y=\"%f\" z=\"%f\" w=\"%f\"/>\n",
			i,
			cf[i*4+0],
			cf[i*4+1],
			cf[i*4+2],
			cf[i*4+3] );
	}
	fprintf( fp, "\t\t</float>\n" );

	// size_t conts
	fprintf( fp, "\t\t<int>\n" );
	for( size_t i = 0; i < ni; ++i )
	{
		fprintf(
			fp,
			"\t\t\t<i index=\"%03d\" x=\"%d\" y=\"%d\" z=\"%d\" w=\"%d\"/>\n",
			i,
			ci[i*4+0],
			ci[i*4+1],
			ci[i*4+2],
			ci[i*4+3] );
	}
	fprintf( fp, "\t\t</int>\n" );

	// boolean count
	fprintf( fp, "\t\t<bool>\n" );
	for( size_t i = 0; i < nb; ++i )
	{
		fprintf(
			fp,
			"\t\t\t<b index=\"%03d\" x=\"%d\"/>\n",
			i,
			cb[i] );
	}
	fprintf( fp, "\t\t</bool>\n" );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpVsConsts( FILE * fp )
{
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	float constf[256*4];
	int   consti[16*4];
	int   constb[16];

	dev->GetVertexShaderConstantF( 0, constf, 256 );
	dev->GetVertexShaderConstantI( 0, consti, 16 );
	dev->GetVertexShaderConstantB( 0, constb, 16 );

	fprintf( fp, "\t<vsc>\n" );
	sDumpConsts( fp, constf, 256, consti, 16, constb, 16 );
	fprintf( fp, "\t</vsc>\n" );
}


//
//
// -----------------------------------------------------------------------------
static void sDumpPsConsts( FILE * fp )
{
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	float constf[224*4];
	int   consti[16*4];
	int   constb[16];

	dev->GetPixelShaderConstantF( 0, constf, 224 );
	dev->GetPixelShaderConstantI( 0, consti, 16 );
	dev->GetPixelShaderConstantB( 0, constb, 16 );

	fprintf( fp, "\t<psc>\n" );
	sDumpConsts( fp, constf, 224, consti, 16, constb, 16 );
	fprintf( fp, "\t</psc>\n" );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpVtxDecl( FILE * fp )
{
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	AutoComPtr<IDirect3DVertexDeclaration9> decl;

	dev->GetVertexDeclaration( &decl );

	D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
	UINT              count;

	decl->GetDeclaration( elements, &count );

	fprintf( fp, "\t<vtxdecl count=\"%d\">\n", count );
	for( UINT i = 0; i < count; ++i )
	{
		const D3DVERTEXELEMENT9 & e = elements[i];

		fprintf( fp,
			"\t\t<element stream=\"%d\" offset=\"%d\" type=\"%d\" method=\"%d\" usage=\"%d\" index=\"%d\"/>\n",
			e.Stream, e.Offset, e.Type, e.Method, e.Usage, e.UsageIndex );
	}
	fprintf( fp, "\t</vtxdecl>\n" );
}

//
//
// -----------------------------------------------------------------------------
void sDumpBinary( const char * filename, const void * data, size_t bytes )
{
    DiskFile fp;

    if( !fp.open( filename, "wb" ) )
	{
		GN_ERROR(sLogger)( "fail to open file : %s", filename );
		return;
	}

	fwrite( data, 1, bytes, fp );

	fclose( fp );
}

//
//
// -----------------------------------------------------------------------------
void sDumpVtxBufs( FILE * fp )
{
    D3D9Renderer & r = GET_RENDERER();
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	for( DWORD i = 0; i < 16; ++i )
	{
		AutoComPtr<IDirect3DVertexBuffer9> vb;

		UINT offset, stride;

		dev->GetStreamSource( i, &vb, &offset, &stride );

		if( vb )
		{
			D3DVERTEXBUFFER_DESC desc;
			vb->GetDesc( &desc );

			void * data;

			if( S_OK == vb->Lock( 0, 0, (void**)&data, D3DLOCK_READONLY ) )
			{
				char fname[_MAX_PATH];
				sprintf_s( fname, "d3d9statedump_frame(%08d)_draw(%04d)_vtxbuf(%02d).bin",
                    r.getFrameCounter(), r.getDrawCounter(), i );
				sDumpBinary( fname, data, desc.Size );

				vb->Unlock();

				fprintf( fp, "\t<vtxbuf stream=\"%d\" offset=\"%d\" stride=\"%d\" bytes=\"%u\" ref=\"%s\"/>\n", i, offset, stride, desc.Size, fname );
			}
			else
			{
				GN_ERROR(sLogger)( "fail to lock vertex buffer." );
			}
		}
	}
}

//
//
// -----------------------------------------------------------------------------
void sDumpIdxBuf( FILE * fp )
{
    D3D9Renderer & r = GET_RENDERER();
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	AutoComPtr<IDirect3DIndexBuffer9> ib;

	dev->GetIndices( &ib );

	if( !ib ) return;

	D3DINDEXBUFFER_DESC desc;
	ib->GetDesc( &desc );

	void * data;

	if( S_OK == ib->Lock( 0, 0, (void**)&data, D3DLOCK_READONLY ) )
	{
		char fname[_MAX_PATH];
		sprintf_s( fname, "d3d9statedump_frame(%08d)_draw(%04d)_idxbuf.bin",
            r.getFrameCounter(), r.getDrawCounter() );
		sDumpBinary( fname, data, desc.Size );

		ib->Unlock();

		fprintf( fp, "\t<idxbuf format=\"%d\" startvtx=\"%d\" bytes=\"%u\" ref=\"%s\"/>\n",
            desc.Format, 0, desc.Size, fname );
	}
	else
	{
		GN_ERROR(sLogger)( "fail to lock index buffer." );
	}
}


//
//
// -----------------------------------------------------------------------------
static void sDumpTextures( FILE * fp )
{
    D3D9Renderer & r = GET_RENDERER();
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	char fname[_MAX_PATH];

	for( DWORD i = 0; i < 16; ++i )
	{
		AutoComPtr<IDirect3DBaseTexture9> tex;
		dev->GetTexture( i, &tex );

		if( tex )
		{
            sprintf_s( fname, "d3d9statedump_frame(%08d)_draw(%04d)_texture(%02d).dds",
                r.getFrameCounter(), r.getDrawCounter(), i );
			D3DXSaveTextureToFileA( fname, D3DXIFF_DDS, tex, 0 );

			fprintf( fp, "\t<texture stage=\"%d\" ref=\"%s\"/>\n", i, fname );
		}
	}
}

//
//
// -----------------------------------------------------------------------------
static void sDumpRenderTargets( FILE * fp )
{
    D3D9Renderer & r = GET_RENDERER();
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

    StrA fname;

    for( DWORD i = 0; i < 4; ++i )
    {
        AutoComPtr<IDirect3DSurface9> rt;

        dev->GetRenderTarget( i, &rt );

        if( rt )
        {
            D3DSURFACE_DESC desc;
            rt->GetDesc( &desc );

            fname.format( "d3d9statedump_frame(%08d)_draw(%04d)_rendertarget(%02d).dds",
                r.getFrameCounter(), r.getDrawCounter(), i );
			D3DXSaveSurfaceToFileA( fname.cptr(), D3DXIFF_DDS, rt, 0, 0 );

            fprintf( fp,
                "\t<rendertarget stage=\"%d\" width=\"%d\" height=\"%d\" format=\"%d\" msaa=\"%d\" quality=\"%d\" ref=\"%s\"/>\n",
                i, desc.Width, desc.Height, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, fname.cptr() );
        }
    }

    AutoComPtr<IDirect3DSurface9> ds;
    dev->GetDepthStencilSurface( &ds );
    if( ds )
    {
        D3DSURFACE_DESC desc;
        ds->GetDesc( &desc );

        fprintf( fp,
            "\t<depthstencil width=\"%d\" height=\"%d\" format=\"%d\" msaa=\"%d\" quality=\"%d\" ref=\"none\"/>\n",
            desc.Width, desc.Height, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality );
    }
}

//
//
// -----------------------------------------------------------------------------
static void sDumpRenderStates( FILE * fp )
{
	LPDIRECT3DDEVICE9 dev = GET_DEVICE();

	static D3DRENDERSTATETYPE rstypes[] = {
		D3DRS_ZENABLE                    ,
	    D3DRS_FILLMODE                   ,
	    D3DRS_SHADEMODE                  ,
	    D3DRS_ZWRITEENABLE               ,
	    D3DRS_ALPHATESTENABLE            ,
	    D3DRS_LASTPIXEL                  ,
	    D3DRS_SRCBLEND                   ,
	    D3DRS_DESTBLEND                  ,
	    D3DRS_CULLMODE                   ,
	    D3DRS_ZFUNC                      ,
	    D3DRS_ALPHAREF                   ,
	    D3DRS_ALPHAFUNC                  ,
	    D3DRS_DITHERENABLE               ,
	    D3DRS_ALPHABLENDENABLE           ,
	    D3DRS_FOGENABLE                  ,
	    D3DRS_SPECULARENABLE             ,
	    D3DRS_FOGCOLOR                   ,
	    D3DRS_FOGTABLEMODE               ,
	    D3DRS_FOGSTART                   ,
	    D3DRS_FOGEND                     ,
	    D3DRS_FOGDENSITY                 ,
	    D3DRS_RANGEFOGENABLE             ,
	    D3DRS_STENCILENABLE              ,
	    D3DRS_STENCILFAIL                ,
	    D3DRS_STENCILZFAIL               ,
	    D3DRS_STENCILPASS                ,
	    D3DRS_STENCILFUNC                ,
	    D3DRS_STENCILREF                 ,
	    D3DRS_STENCILMASK                ,
	    D3DRS_STENCILWRITEMASK           ,
	    D3DRS_TEXTUREFACTOR              ,
	    D3DRS_WRAP0                      ,
	    D3DRS_WRAP1                      ,
	    D3DRS_WRAP2                      ,
	    D3DRS_WRAP3                      ,
	    D3DRS_WRAP4                      ,
	    D3DRS_WRAP5                      ,
	    D3DRS_WRAP6                      ,
	    D3DRS_WRAP7                      ,
	    D3DRS_CLIPPING                   ,
	    D3DRS_LIGHTING                   ,
	    D3DRS_AMBIENT                    ,
	    D3DRS_FOGVERTEXMODE              ,
	    D3DRS_COLORVERTEX                ,
	    D3DRS_LOCALVIEWER                ,
	    D3DRS_NORMALIZENORMALS           ,
	    D3DRS_DIFFUSEMATERIALSOURCE      ,
	    D3DRS_SPECULARMATERIALSOURCE     ,
	    D3DRS_AMBIENTMATERIALSOURCE      ,
	    D3DRS_EMISSIVEMATERIALSOURCE     ,
	    D3DRS_VERTEXBLEND                ,
	    D3DRS_CLIPPLANEENABLE            ,
	    D3DRS_POINTSIZE                  ,
	    D3DRS_POINTSIZE_MIN              ,
	    D3DRS_POINTSPRITEENABLE          ,
	    D3DRS_POINTSCALEENABLE           ,
	    D3DRS_POINTSCALE_A               ,
	    D3DRS_POINTSCALE_B               ,
	    D3DRS_POINTSCALE_C               ,
	    D3DRS_MULTISAMPLEANTIALIAS       ,
	    D3DRS_MULTISAMPLEMASK            ,
	    D3DRS_PATCHEDGESTYLE             ,
	    D3DRS_DEBUGMONITORTOKEN          ,
	    D3DRS_POINTSIZE_MAX              ,
	    D3DRS_INDEXEDVERTEXBLENDENABLE   ,
	    D3DRS_COLORWRITEENABLE           ,
	    D3DRS_TWEENFACTOR                ,
	    D3DRS_BLENDOP                    ,
	    D3DRS_POSITIONDEGREE             ,
	    D3DRS_NORMALDEGREE               ,
	    D3DRS_SCISSORTESTENABLE          ,
	    D3DRS_SLOPESCALEDEPTHBIAS        ,
	    D3DRS_ANTIALIASEDLINEENABLE      ,
	    D3DRS_MINTESSELLATIONLEVEL       ,
	    D3DRS_MAXTESSELLATIONLEVEL       ,
	    D3DRS_ADAPTIVETESS_X             ,
	    D3DRS_ADAPTIVETESS_Y             ,
	    D3DRS_ADAPTIVETESS_Z             ,
	    D3DRS_ADAPTIVETESS_W             ,
	    D3DRS_ENABLEADAPTIVETESSELLATION ,
	    D3DRS_TWOSIDEDSTENCILMODE        ,
	    D3DRS_CCW_STENCILFAIL            ,
	    D3DRS_CCW_STENCILZFAIL           ,
	    D3DRS_CCW_STENCILPASS            ,
	    D3DRS_CCW_STENCILFUNC            ,
	    D3DRS_COLORWRITEENABLE1          ,
	    D3DRS_COLORWRITEENABLE2          ,
	    D3DRS_COLORWRITEENABLE3          ,
	    D3DRS_BLENDFACTOR                ,
	    D3DRS_SRGBWRITEENABLE            ,
	    D3DRS_DEPTHBIAS                  ,
	    D3DRS_WRAP8                      ,
	    D3DRS_WRAP9                      ,
	    D3DRS_WRAP10                     ,
	    D3DRS_WRAP11                     ,
	    D3DRS_WRAP12                     ,
	    D3DRS_WRAP13                     ,
	    D3DRS_WRAP14                     ,
	    D3DRS_WRAP15                     ,
	    D3DRS_SEPARATEALPHABLENDENABLE   ,
	    D3DRS_SRCBLENDALPHA              ,
	    D3DRS_DESTBLENDALPHA             ,
	    D3DRS_BLENDOPALPHA               ,
	};

	static D3DSAMPLERSTATETYPE sstypes[] = {
		D3DSAMP_ADDRESSU,
	    D3DSAMP_ADDRESSV,
	    D3DSAMP_ADDRESSW,
	    D3DSAMP_BORDERCOLOR,
	    D3DSAMP_MAGFILTER,
	    D3DSAMP_MINFILTER,
	    D3DSAMP_MIPFILTER,
	    D3DSAMP_MIPMAPLODBIAS,
	    D3DSAMP_MAXMIPLEVEL,
	    D3DSAMP_MAXANISOTROPY,
	    D3DSAMP_SRGBTEXTURE,
	    D3DSAMP_ELEMENTINDEX,
	    D3DSAMP_DMAPOFFSET,
	};

	// print render states
	fprintf( fp, "\t<renderstates>\n" );
	DWORD value;
	for( size_t i = 0; i < sizeof(rstypes)/sizeof(rstypes[0]); ++i )
	{
		dev->GetRenderState( rstypes[i], &value );
		fprintf( fp, "\t\t<rs type=\"%u\" value=\"%u\"/>\n", rstypes[i], value );
	}
	fprintf( fp, "\t</renderstates>\n" );

	// print sampler states
	fprintf( fp, "\t<samplerstates stagecount=\"16\">\n" );
	for( DWORD s = 0; s < 16; ++s )
	for( size_t i = 0; i < sizeof(sstypes)/sizeof(sstypes[0]); ++i )
	{
		dev->GetSamplerState( s, sstypes[i], &value );
		fprintf( fp, "\t\t<ss stage=\"%u\" type=\"%u\" value=\"%u\"/>\n", s, sstypes[i], value );
	}
	fprintf( fp, "\t</samplerstates>\n" );

	// scissor rect
	RECT scissor;
	dev->GetScissorRect( &scissor );
	fprintf(
		fp,
		"\t<scissor l=\"%d\" t=\"%d\" r=\"%d\" b=\"%d\"/>\n",
		scissor.left, scissor.top, scissor.right, scissor.bottom );

	// viewport
	D3DVIEWPORT9 vp;
	dev->GetViewport( &vp );
	fprintf(
		fp,
		"\t<viewport x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" zmin=\"%f\" zmax=\"%f\"/>\n",
		vp.X, vp.Y, vp.Width, vp.Height, vp.MinZ, vp.MaxZ );
}

//
//
// -----------------------------------------------------------------------------
static void sDumpD3D9States( FILE * fp )
{
	sDumpVs( fp );
	sDumpVsConsts( fp );
	sDumpPs( fp );
	sDumpPsConsts( fp );

	sDumpVtxDecl( fp );
	sDumpVtxBufs( fp );
	sDumpIdxBuf( fp );
	sDumpTextures( fp );

	sDumpRenderTargets( fp );

	sDumpRenderStates( fp );
}

// *****************************************************************************
// public function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::dumpD3D9Draw(
    D3DPRIMITIVETYPE prim,
    UINT startvtx,
    UINT numprim )
{
	DumpFile file;

	if( !file.fp ) return;

    fprintf(
        file.fp,
        "\t<draw prim=\"%d\" startvtx=\"%d\" minidx=\"%d\" numvtx=\"%d\" startidx=\"%d\" numprim=\"%d\"/>\n",
        prim, startvtx, numprim );

    sDumpD3D9States( file.fp );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::dumpD3D9DrawIndexed(
    D3DPRIMITIVETYPE prim,
    UINT startvtx,
    UINT minvtxidx,
    UINT numvtx,
    UINT startidx,
    UINT numprim )
{
	DumpFile file;

	if( !file.fp ) return;

    fprintf(
        file.fp,
        "\t<drawindexed prim=\"%d\" startvtx=\"%d\" minidx=\"%d\" numvtx=\"%d\" startidx=\"%d\" numprim=\"%d\"/>\n",
        prim, startvtx, minvtxidx, numvtx, startidx, numprim );

    sDumpD3D9States( file.fp );
}

#else

//
//
// -----------------------------------------------------------------------------
void GN::gfx::dumpD3D9Draw( D3DPRIMITIVETYPE, UINT, UINT )
{
    // do nothing. We use PIX.
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::dumpD3D9DrawIndexed( D3DPRIMITIVETYPE, UINT, UINT, UINT, UINT, UINT )
{
    // do nothing. We use PIX.
}

#endif
