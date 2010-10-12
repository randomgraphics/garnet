#include "pch.h"
#include "xenonShader.h"
#include "xenonTexture.h"
#include "xenonGpu.h"
#include "garnet/GNd3d.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.xenon");

using namespace GN::gfx;

//
//
// -----------------------------------------------------------------------------
template<class T, class ARRAY>
static T * sFindParameter( ARRAY & array, const char * name )
{
    for( size_t i = 0; i < array.size(); ++i )
    {
        T & t = array[i];

        if( t.namestr == name ) return &t;
    }

    return NULL;
}

static const D3DTEXTUREADDRESS ADDRESS_TO_D3D[] =
{
    D3DTADDRESS_WRAP,   // ADDRESS_REPEAT = 0,
    D3DTADDRESS_CLAMP,  // ADDRESS_CLAMP,
    D3DTADDRESS_CLAMP,  // ADDRESS_CLAMP_BORDER,
    D3DTADDRESS_MIRROR, // ADDRESS_MIRROR,
};
GN_CASSERT( GN_ARRAY_COUNT(ADDRESS_TO_D3D) == SamplerDesc::NUM_ADDRESS_MODES );

static const D3DTEXTUREFILTERTYPE FILTER_TO_D3D[] =
{
    D3DTEXF_POINT,  // FILTER_POINT  = 0,
    D3DTEXF_LINEAR, // FILTER_LINEAR,
};
GN_CASSERT( GN_ARRAY_COUNT(FILTER_TO_D3D) == SamplerDesc::NUM_FILTERS );

//
//
// -----------------------------------------------------------------------------
static inline void
sSetSampler( IDirect3DDevice9 & dev, uint32 stage, const SamplerDesc & s )
{
    dev.SetSamplerState_Inline( stage, D3DSAMP_ADDRESSU, ADDRESS_TO_D3D[s.addressU] );
    dev.SetSamplerState_Inline( stage, D3DSAMP_ADDRESSV, ADDRESS_TO_D3D[s.addressV] );
    dev.SetSamplerState_Inline( stage, D3DSAMP_ADDRESSW, ADDRESS_TO_D3D[s.addressW] );
    dev.SetSamplerState_Inline( stage, D3DSAMP_MINFILTER, FILTER_TO_D3D[s.filterMin] );
    dev.SetSamplerState_Inline( stage, D3DSAMP_MAGFILTER, FILTER_TO_D3D[s.filterMag] );
    dev.SetSamplerState_Inline( stage, D3DSAMP_MIPFILTER, FILTER_TO_D3D[s.filterMip] );
}

static const char * D3D_DECL_USAGE_TO_STRING[] =
{
    "POSITION",       // D3DDECLUSAGE_POSITION = 0,
    "BLENDWEIGHT",    // D3DDECLUSAGE_BLENDWEIGHT = 1,
    "BLENDINDICES",   // D3DDECLUSAGE_BLENDINDICES = 2,
    "NORMAL",         // D3DDECLUSAGE_NORMAL = 3,
    "PSIZE",          // D3DDECLUSAGE_PSIZE = 4,
    "TEXCOORD",       // D3DDECLUSAGE_TEXCOORD = 5,
    "TANGENT",        // D3DDECLUSAGE_TANGENT = 6,
    "BINORMAL",       // D3DDECLUSAGE_BINORMAL = 7,
    "TESSFACTOR",     // D3DDECLUSAGE_TESSFACTOR = 8,
    NULL,             // no use = 9,
    "COLOR",          // D3DDECLUSAGE_COLOR = 10,
    "FOG",            // D3DDECLUSAGE_FOG = 11,
    "DEPTH",          // D3DDECLUSAGE_DEPTH = 12,
    "SAMPLE",         // D3DDECLUSAGE_SAMPLE = 13
};

//
//
// -----------------------------------------------------------------------------
static inline const char *
sD3DDeclUsage2String( UINT decl )
{
    GN_ASSERT( 0 <= decl && decl <= GN_ARRAY_COUNT(D3D_DECL_USAGE_TO_STRING) && 9 != decl );
    return D3D_DECL_USAGE_TO_STRING[decl];
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpuProgramHLSL::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( XenonGpuProgramHLSL, () );

    GN_ASSERT( GpuProgramLanguage::HLSL9 == desc.lang );

    IDirect3DDevice9 & dev = getGpu().getDeviceInlined();

    if( desc.vs.source )
    {
        AutoComPtr<ID3DXBuffer> bin;
        mVs = d3d9::ShaderCompiler<IDirect3DVertexShader9>::compileAndCreate(
            dev,
            desc.code[ShaderStage::VS].source,
            0, // source length
            0, // compile flags
            desc.code[ShaderStage::VS].entry,
            0, // profile
            &mVsConsts,
            &bin );

        if( NULL == mVs ) return failure();

        enumerateConsts( mVsConsts, true );

        enumerateAttributes( *bin );
    }

    if( desc.ps.source )
    {
        mPs = d3d9::ShaderCompiler<IDirect3DPixelShader9>::compileAndCreate(
            dev,
            desc.code[ShaderStage::PS].source,
            0,
            0,
            desc.code[ShaderStage::PS].entry,
            0,
            &mPsConsts );
        if( NULL == mPs ) return failure();

        enumerateConsts( mPsConsts, false );
    }

    // setup parameter descriptor
    buildUnformNameAndSizeArray();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramHLSL::quit()
{
    GN_GUARD;

    safeRelease( mVs );
    safeRelease( mVsConsts );
    safeRelease( mPs );
    safeRelease( mPsConsts );

    mUniforms.clear();
    mTextures.clear();
    mAttributes.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramHLSL::apply() const
{
    IDirect3DDevice9 & dev = getGpu().getDeviceInlined();
    dev.SetVertexShader( mVs );
    dev.SetPixelShader( mPs );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramHLSL::applyUniforms(
    const Uniform * const * uniforms,
    size_t                  count,
    bool                    /*skipDirtyCheck*/ ) const
{
    IDirect3DDevice9 & dev = getGpu().getDeviceInlined();

    for( size_t i = 0; i < count; ++i )
    {
        if( i >= mUniforms.size() )
        {
            GN_WARN(sLogger)( "there are more GPU parameters than the shader needs." );
            return;
        }

        const SysMemUniform * u = (const SysMemUniform*)uniforms[i];

        if( NULL == u )
        {
            GN_ERROR(sLogger)( "Null uniform pointer." );
            continue;
        }

        const XenonUniformParamDesc & d = mUniforms[i];

        GN_ASSERT( !d.namestr.empty() && (d.vshandle || d.pshandle) && d.size );

        // check parameter size
        if( getGpu().paramCheckEnabled() )
        {
            if( u->size() != d.size )
            {
                GN_WARN(sLogger)(
                    "parameter %s: value size(%d) differs from size defined in shader code(%d).",
                    d.namestr.cptr(),
                    u->size(),
                    d.size );
            }
        }

        // apply to D3D
        if( d.vshandle )
        {
            GN_DX_CHECK( mVsConsts->SetValue( &dev, d.vshandle, u->getval(), (UINT)d.size ) );
        }
        if( d.pshandle )
        {
            GN_DX_CHECK( mPsConsts->SetValue( &dev, d.pshandle, u->getval(), (UINT)d.size ) );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramHLSL::applyTextures(
    const TextureBinding * bindings,
    size_t                 count,
    bool                   /*skipDirtyCheck*/ ) const
{
    IDirect3DDevice9 & dev = getGpu().getDeviceInlined();

    // determine effective texture count
    if( count > mParamDesc.textures.count() )
    {
        count = mParamDesc.textures.count();
    }

    bool usedStages[GPU_D3D_TEXTURE_FETCH_CONSTANT_COUNT];
    memset( &usedStages, 0, sizeof(usedStages) );

    for( size_t i = 0; i < count; ++i )
    {
        const TextureBinding & tb = bindings[i];

        const XenonTextureParamDesc & param = mTextures[i];

        XenonTexture * tex = (XenonTexture*)tb.texture.get();

        if( param.vshandle )
        {
            UINT stage = mVsConsts->GetSamplerIndex( param.vshandle );
            XenonTexture::sBindToDevice( dev, stage, tex );
            usedStages[stage] = true;

            if( tb.sampler != param.vssampler )
            {
                sSetSampler( dev, stage, tb.sampler );
                param.vssampler = tb.sampler;
            }
        }

        if( param.pshandle )
        {
            UINT stage = mPsConsts->GetSamplerIndex( param.pshandle );
            XenonTexture::sBindToDevice( dev, stage, tex );
            usedStages[stage] = true;

            if( tb.sampler != param.pssampler )
            {
                sSetSampler( dev, stage, tb.sampler );
                param.pssampler = tb.sampler;
            }
        }
    }

    // clear unused texture stages
    for( UINT i = 0; i < GPU_D3D_TEXTURE_FETCH_CONSTANT_COUNT; ++i )
    {
        if( !usedStages[i] )
        {
            dev.SetTexture( i, NULL );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpuProgramHLSL::getAttributeUsage(
    size_t attributeIndex,
    BYTE & usage,
    BYTE & usageIndex ) const
{
    if( attributeIndex >= mAttributes.size() )
    {
        GN_ERROR(sLogger)( "Invalid attribute index: %d", attributeIndex );
        return false;
    }

    const XenonAttributeParamDesc & a = mAttributes[attributeIndex];

    usage      = (BYTE)a.usage;
    usageIndex = (BYTE)a.usageIndex;

    return true;
}

// *****************************************************************************
// Private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpuProgramHLSL::enumerateConsts(
    ID3DXConstantTable * constTable,
    bool                 vs )
{
    // get number of constants
    D3DXCONSTANTTABLE_DESC tableDesc;
    GN_DX_CHECK_RETURN( constTable->GetDesc( &tableDesc ), false );
    UINT numConstant = tableDesc.Constants;

    // for each const
    for( UINT i = 0; i < numConstant; ++i )
    {
        // get handle and description
        D3DXHANDLE        c = constTable->GetConstant( NULL, i );
        D3DXCONSTANT_DESC cdarray[GPU_D3D_PIXEL_TEXTURE_FETCH_CONSTANT_COUNT];
        size_t            count;
        GN_DX_CHECK_RETURN( constTable->GetConstantDesc( c, cdarray, &count ), false );

        GN_ASSERT( count > 0 );

        for( size_t i = 0; i < count; ++i )
        {
            D3DXCONSTANT_DESC & cd = cdarray[0];

            GN_TODO( "check for uniforms with same name" );

            // determin
            switch( cd.Type )
            {
                case D3DXPT_BOOL  :
                case D3DXPT_INT   :
                case D3DXPT_FLOAT :
                {
                    XenonUniformParamDesc * u = sFindParameter<XenonUniformParamDesc>( mUniforms, cd.Name );
                    if( u )
                    {
                        if( u->size != cd.Bytes )
                        {
                            GN_ERROR(sLogger)( "VS and PS are referencing uniform with same name but different type, which is not supported by Xenon GPU." );
                            return false;
                        }

                        if( vs )
                        {
                            u->vshandle = c;
                        }
                        else
                        {
                            u->pshandle = c;
                        }
                    }
                    else
                    {
                        XenonUniformParamDesc u;
                        u.namestr = cd.Name;
                        if( vs )
                        {
                            u.vshandle = c;
                        }
                        else
                        {
                            u.pshandle = c;
                        }
                        u.size = cd.Bytes;
                        mUniforms.append( u );
                    }
                    break;
                }

                case D3DXPT_TEXTURE1D:
                case D3DXPT_TEXTURE2D:
                case D3DXPT_TEXTURE3D:
                case D3DXPT_TEXTURECUBE:
                    // do nothing, just ignore them.
                    break;

                case D3DXPT_SAMPLER:
                case D3DXPT_SAMPLER1D:
                case D3DXPT_SAMPLER2D:
                case D3DXPT_SAMPLER3D:
                case D3DXPT_SAMPLERCUBE:
                {
                    XenonTextureParamDesc * t = sFindParameter<XenonTextureParamDesc>( mTextures, cd.Name );
                    if( t )
                    {
                        if( vs )
                        {
                            t->vshandle = c;
                        }
                        else
                        {
                            t->pshandle = c;
                        }
                    }
                    else
                    {
                        XenonTextureParamDesc t;
                        t.namestr = cd.Name;
                        if( vs )
                        {
                            t.vshandle = c;
                        }
                        else
                        {
                            t.pshandle = c;
                        }
                        mTextures.append( t );
                    }
                    break;
                }

                default:
                    GN_WARN(sLogger)( "Unsupport Xenon shader constant type: %d", cd.Type );
            }
        }
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramHLSL::enumerateAttributes( ID3DXBuffer & shaderBinary )
{
    D3DXSEMANTIC semantics[MAXD3DDECLLENGTH];
    UINT count;

    if( FAILED( D3DXGetShaderInputSemantics( (const DWORD*)shaderBinary.GetBufferPointer(), semantics, &count ) ) )
    {
        GN_ERROR(sLogger)( "Fail to get shader input semantics." );
        return;
    }

    mAttributes.resize( count );
    for( size_t i = 0; i < count; ++i )
    {
        mAttributes[i].namestr = stringFormat( "%s%d", sD3DDeclUsage2String(semantics[i].Usage), semantics[i].UsageIndex );
        mAttributes[i].usage = semantics[i].Usage;
        mAttributes[i].usageIndex = semantics[i].UsageIndex;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpuProgramHLSL::buildUnformNameAndSizeArray()
{
    if( !mUniforms.empty() )
    {
        size_t count = mUniforms.size();

        GN_ASSERT( count > 0 );

        mParamDesc.mUniformArray       = mUniforms.cptr();
        mParamDesc.mUniformCount       = mUniforms.size();
        mParamDesc.mUniformArrayStride = sizeof(mUniforms[0]);

        for( size_t i = 0; i < count; ++i )
        {
            mUniforms[i].name = mUniforms[i].namestr;
        }
    }

    if( !mTextures.empty() )
    {
        size_t count = mTextures.size();

        GN_ASSERT( count > 0 );

        mParamDesc.mTextureArray       = mTextures.cptr();
        mParamDesc.mTextureCount       = mTextures.size();
        mParamDesc.mTextureArrayStride = sizeof(mTextures[0]);

        for( size_t i = 0; i < count; ++i )
        {
            mTextures[i].name = mTextures[i].namestr;
        }
    }

    if( !mAttributes.empty() )
    {
        size_t count = mAttributes.size();

        GN_ASSERT( count > 0 );

        mParamDesc.mAttributeArray       = mAttributes.cptr();
        mParamDesc.mAttributeCount       = mAttributes.size();
        mParamDesc.mAttributeArrayStride = sizeof(mAttributes[0]);

        for( size_t i = 0; i < count; ++i )
        {
            mAttributes[i].name = mAttributes[i].namestr;
        }
    }
}
