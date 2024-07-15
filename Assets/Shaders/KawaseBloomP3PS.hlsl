#include "KawaseBloomP2.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = float4(0, 0, 0, 1);
    col += tex0.Sample(smp, input.uv);
    col += tex1.Sample(smp, input.uv) / 2;
    col.a = 1;
    return col;
}