struct VSInput
{
    float4 pos    : POSITION;
    float3 normal : NORMAL;
    float2 tex    : TEXCOORD0;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float2 tex : TEXCOORD0;
    float4 clr : COLOR0;
};

uniform float4x4 gPVW;

VSOutput vsmain( VSInput i )
{
    VSOutput o;

    o.pos = mul( gPVW, i.pos );
    o.tex = i.tex;
    o.clr = 1;

    return o;
}

float4 psmain( VSOutput i ) : SV_Target0
{
    return i.clr;
}

technique
{
    pass
    {
        SetVertexShader( CompileShader( vs_4_0, vsmain() ) );
        SetPixelShader( CompileShader( ps_4_0,psmain() ) );
    }
}
