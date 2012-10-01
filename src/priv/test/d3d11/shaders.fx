struct VSInput
{
    float4 pos    : POSITION;
    float3 normal : NORMAL;
    float2 tex    : TEXCOORD0;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 clr : COLOR0;
};

uniform float4x4 gPVW;

VSOutput vsmain( VSInput i )
{
    VSOutput o;

    o.pos = mul( gPVW, i.pos );
    o.tex = i.tex;
    float3 n = 2*abs(i.normal) + i.normal;
    o.clr = float4( n/3.0, 1.0 );

    return o;
}

float4 psmain( VSOutput i ) : SV_TARGET
{
    return i.clr;
}

technique10 t0
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, vsmain() ) );
        SetPixelShader( CompileShader( ps_4_0, psmain() ) );
    }
}
